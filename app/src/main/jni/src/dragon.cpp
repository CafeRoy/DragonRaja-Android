#include "stdafx.h"
#include <cstdio>
#include <fcntl.h>   // For _O_TEXT
#include "dragon.h"
#include <cmath>
#include "gameproc.h"
#include "network.h"
#include "map.h"
#include "char.h"
#include "object.h"
#include "Hong_Sprite.h"
#include "Hong_Light.h"
#include "Hong_Sub.h"
#include "Hangul.h"
#include "Menu.h"
#include "MenuStartSet.h"
#include "directsound.h"
#include "Effect.h"
#include "LineCommand.h"
#include "Tool.h"
#include "ItemTable.h"
#include "MouseCursor.h"
#include "SkillTool.h"
#include "Skill.h"
#include "house.h"
#include "CharDataTable.h"
#include "Item.h"
#include "SmallMenu.h"
#include "Counseler.h"
#include "NPC_Pattern.h"
#include "FrameMgr.h"
#include "LottoMgr.h"
#include <filesystem>
#include "path.h"
#include "SDLRenderer.h"
#include "inicpp.h"
#include "Horse.h"
#include "cScreenEffect.h"
#include "CMessage.h"
#include "NumberAniMgr.h"
#include "CharacterContxtMenu.h"
#include "Chat.h"
#include "CrackMgr.h"
#include "Ability.h"
#include "CimGui.h"
// 引入標頭檔
#include "RouteSelector.h"
#include "SimpleUpdater.h"
extern SMENU    SMenu[MAX_MENU];		// startmenu.cpp
extern NPC_INFO g_infNpc[MAX_CHARACTER_SPRITE_]; // CSD-030419
extern ITEM_INFO g_itemref[MAX_ITEM_MUTANT_KIND]; // CSD-030419
extern int ListenCommand;		// network.cpp
extern void DoQuickmemoryByKeyInput(SDL_Scancode iInputKey, bool bIsDikCheck);
extern void SaveMemorize();
extern char* g_DestBackBuf;
void SetCurrentWorkingDirectory(void);
///////////////////////////////////////////////////////////////////////////////
// Global Variables:
HWND				g_hwndMain;
HINSTANCE			g_hInstance;

GAMEINFO			g_GameInfo;
ini::IniFile        InfoFile;

std::string 		g_szCWD;

BOOL				g_bIsActive = TRUE;
//////////// 0309 lkh 추가 //////////////
BOOL				g_bCommandMode = CM_COMMAND;

CHARACTERLIST		g_CharacterList;


POINT				g_pointMouse;
int				g_pointMouseX, g_pointMouseY;
int					g_nLButtonState, g_nRButtonState;
int					g_nLDButtonState, g_nRDButtonState;
int					g_nOldLButtonState, g_nOldRButtonState;
int					LButtonDownIng=0, RButtonDownIng=0;

DWORD				g_CurrentTime=0;			// 서버에서 알려준 현재 시간.
DWORD				g_ClientTime=0;			// GameProc()에서 계속적으로 Check 한다. 
DWORD				g_ServerClientTimeGap=0;

DWORD				g_curr_time=0;			//  서버에서보내주는 현재의 시간. 초단위..
DWORD				g_packet_recv_send_checktime=0;
DWORD				g_Current_Server_Time=0; // finito 060707 holds the current server time
///////////////// 0311 lkh 추가 /////////////////
int					g_Operator_Function;	//운영자용 효과 구현을 위한 스위칭 변수

uint16_t SCREEN_WIDTH = 1280;
uint16_t SCREEN_HEIGHT = 720;
uint16_t  GAME_SCREEN_XSIZE = SCREEN_WIDTH;
uint16_t  GAME_SCREEN_YSIZE = SCREEN_HEIGHT;
uint16_t  NORMAL_SIGHT = 18;
std::string IIDD;
std::string PPWW;
int CheckSumError;
// ------------------- khs 0804
char* g_DestBackBuf = nullptr;
int					Mox, Moy;		// 마우스의 Map의 절대좌표.
SafeValue<int>					Mapx;
SafeValue<int>                  Mapy;		// 현재보여지는 맵의 시작절대좌표.
int					MapTx, MapTy;	// 현재보여지는 맵의 시작절대좌표.
LPCHARACTER         Hero = NULL;			// 주인공의 Point..
//------------------------------
int					QuitFlag = false;

DWORD  g_RecvBytes, g_SendBytes;

bool				MouseCursorOnOff = true;
bool				g_Inside;

int					NowLoading;

//void CALLBACK TimerProc(HWND hWnd, UINT nMsg, UINT nTimerid, DWORD dwTime); // CSD-CN-031215

/////////////// 0128 이규훈 /////////////////	
bool  g_Volume_Off = 0;

extern void		ItemRead();
extern void PutStartLodingImg();		// 0127 YGI


bool g_bIsChristmas = false;

///////////////////////////////////////////////////////////////////////////////
// Functions.

//lsw		
extern char explain_tactics[13][20];
extern char FightTeam[5][25];
extern char NationName[8][25];			// 010227 YGI
// thai2 YGI
extern char	subFarmType[20][25];
extern char	subMineType[20][25];
extern char	subHubType[20][25];

//________________________ // 010904 LTS
extern bool InitNation();
extern void CloseNation();
extern void InitHorseAniTable();		// LTS HORSERIDER
//________________________ // 010904 LTS

static int FirstCheck = 0;

HWND g_hMultiDlg = NULL;
SDL_TimerID myTimerID;

void lightXOR(std::string& str) {
  for (size_t i = 0; i < str.size(); ++i) {
    str[i] ^= 0xFD;  // 直接在原始字串上進行 XOR 運算
  }
}

void updateTimer() {
	if (g_SDLInfo.updateTimer)
	{
		g_SDLInfo.updateTimer = false;
		g_curr_time++;
		g_packet_recv_send_checktime++;
		//DecMallItemTime();
		g_Current_Server_Time++;
		g_SDLInfo.cleanupTextCache(std::chrono::milliseconds(30000));
		g_GameInfo.ClearBuffTimer(false);
		connections.connect_time++;
	}
}
Uint32 myTimerCallback(Uint32 interval, void* param) {
	// 這裡是在獨立的線程中執行

	// 從 param 恢復傳入的數據 (例如，一個字符串)
	//std::string* message = static_cast<std::string*>(param);

	//g_DBGLog.Log(LOG_LV1, "Timer triggered! Message from callback: %s", *message);

	// --- 發送自定義事件到主遊戲迴圈 ---
	// 如果你需要在回調中改變主線程的遊戲狀態，
	// 應通過發送自定義事件的方式。
	SDL_Event event;
	event.type = MY_CUSTOM_EVENT_CODE; // 設定為我們定義的自定義事件類型
	event.user.code = 1; // 自定義代碼，用於進一步區分事件
	// 你可以通過 event.user.data1 和 data2 傳遞額外的數據
	// 注意：這裡如果傳遞字串，需要小心管理其生命週期，例如傳遞字串拷貝或指標
	// 簡單起見，這裡只傳遞一個標誌。
	event.user.data1 = nullptr; // 沒有額外的數據
	event.user.data2 = nullptr;

	SDL_PushEvent(&event); // 將事件推入 SDL 事件隊列

	// 返回 interval，表示定時器會繼續運行，每 interval 毫秒再次觸發
	return interval;
	// 如果返回 0，定時器將被取消，只觸發一次。
}

void CharToUpper(char* str) {
	size_t len = strlen(str); // 只計算一次長度
	for (size_t i = 0; i < len; i++) {
		str[i] = toupper(static_cast<unsigned char>(str[i]));
	}
}


//---------------------------------------------------------------------------------------------
//  게임을	실행하기전 화일이름의 변동이라든가 자동패치로 못하고 프로그램적으로
//  처리해야 하는 것을 여기서 처리한다. 
//---------------------------------------------------------------------------------------------
void BeforeExe(char* lpCmdLine) // thai2 YGI
{	//< CSD-030324			

	int i, j;
	int c = 0;

	if (!FirstCheck)
	{
		int ii;
		for (ii = 0; ii < 13; ii++)
		{
			char* temp = lan->OutputMessage(9, ii + 451);
			if (strlen(temp) >= 20) JustMsg("1 : %d", strlen(temp));
			sprintf(explain_tactics[ii], "%s", lan->OutputMessage(9, ii + 451));
		}

		for (ii = 0; ii < 5; ii++)
		{
			char* temp = lan->OutputMessage(9, ii + 441);
			//if (strlen(temp) >= 13) JustMsg("2: %d", strlen(temp));

			sprintf(FightTeam[ii], "%s", lan->OutputMessage(9, ii + 441));
		}

		for (ii = 0; ii < 8; ii++)
		{
			char* temp = lan->OutputMessage(9, ii + 421);
			//if (strlen(temp) >= 20) JustMsg("3: %d", strlen(temp));
			sprintf(NationName[ii], "%s", lan->OutputMessage(9, ii + 421));
		}
		// 010314 KHS
		for (ii = 0; ii < MAX_CHARACTER_SPRITE_; ++ii)
		{	//< CSD-030419
			char* temp = lan->OutputMessage(4, ii + 201);
			if (strlen(temp) >= 40)
			{
				JustMsg("4: %d", strlen(temp));
			}

			sprintf(g_infNpc[ii].szName, "%s", lan->OutputMessage(4, ii + 201));
		}	//> CSD-030419

		for (ii = 0; ii < 20; ii++)
		{
			char* temp = lan->OutputMessage(4, ii + 401);
			if (strlen(temp) >= 25) JustMsg("5: %d", strlen(temp));
			sprintf(subFarmType[ii], "%s", lan->OutputMessage(4, ii + 401));
		}


		for (ii = 0; ii < 8; ii++)
		{
			char* temp = lan->OutputMessage(4, ii + 421);
			if (strlen(temp) >= 25) JustMsg("6: %d", strlen(temp));
			sprintf(subMineType[ii], "%s", lan->OutputMessage(4, ii + 421));
		}


		for (ii = 0; ii < 11; ii++)
		{
			char* temp = lan->OutputMessage(4, ii + 121);
			if (strlen(temp) >= 25) JustMsg("7: %d", strlen(temp));
			sprintf(subHubType[ii], "%s", lan->OutputMessage(4, ii + 121));
		}

		for (int j = 0; j < MAX_LINE_COMMAND_; j++)//010216 lsw 라인 커멘드 / 뒤에 명령어들  501 번부터 시작
		{
			std::string command = lan->OutputMessage(9, j + 501);
            if (command.length() >= 35){
              SDL_Log("command too long %d : %s", j, command.c_str());
		    }


            g_LineCommandMap.emplace(command, static_cast<LineCommandIndex>(j));
#ifdef _DEBUG
            SDL_Log("command[%d][%s]", j + 501, command.c_str());
#endif
		}
		FirstCheck = 1;
	}
}

char*
EatFrontWhiteChar(char* pStr)
{
	char* szWhite = " \t\n\r";

	if (pStr)
	{
		while (*pStr)
		{
			if (strchr(szWhite, *pStr))
			{
				pStr++;
			}
			else
			{
				break;
			}
		}
	}

	return  pStr;
}

void EatRearWhiteChar(std::string& s) {
  if (s.empty()) {
    return;
  }

  // 從字串末尾開始，使用 std::isspace 檢查是否為空白字元
  // std::find_if 會找到第一個「不是」空白字元的位置
  s.erase(std::find_if(s.rbegin(), s.rend(),
                       [](unsigned char ch) { return !std::isspace(ch); })
              .base(),
          s.end());
}

char* EatRearWhiteChar(char* pStr) {
  if (!pStr || *pStr == '\0') {
    return pStr;
  }

  // 將 C 風格字串轉換為 std::string 進行安全操作
  std::string s = pStr;

  // 使用 C++ 標準函式庫來找到第一個非空白字元
  auto it = s.rbegin();
  while (it != s.rend() && std::isspace(static_cast<unsigned char>(*it))) {
    ++it;
  }

  // 取得新的有效長度
  size_t new_len = s.rend() - it;

  // 將 std::string 的內容複製回原始的 char* 陣列
  // std::copy(s.begin(), s.begin() + new_len, pStr);
  // 或是更直接的方式
  for (size_t i = 0; i < new_len; ++i) {
    pStr[i] = s[i];
  }

  // 確保以 null 結尾
  pStr[new_len] = '\0';

  return pStr;
}


BOOL LoadGameInfo(void)
{
	g_GameInfo.InitPingList();
    switch(system_info.route)
    {
        case 0:g_GameInfo.proxy_ip = g_GameInfo.agent_ip = "116.241.108.100"; break;
        case 1:g_GameInfo.proxy_ip = g_GameInfo.agent_ip = "47.74.13.199"; break;
        default: return 0;
    }
	g_GameInfo.proxy_port = 9004;
	g_GameInfo.agent_port = 9001;
	
	return	TRUE;
}

BOOL InitApplication()
{
	srand(SDL_GetTicks());
	SetCurrentWorkingDirectory();
	SetInfoFile();
	LoadGameInfo();
    InitializeCharacterPool();
	InitializeItemGroundPool();
	InitializeItemSpritePool();
	g_pointMouseX = SCREEN_WIDTH / 2;
	g_pointMouseY = SCREEN_HEIGHT / 2;
    g_nLButtonState = STATE_BUTTON_DOUBLECLICK;
    g_nRButtonState = g_nOldLButtonState = g_nOldRButtonState = STATE_BUTTON_RELEASED;
    //system_info.lang=0;
	// read fullscreen or windowed mode from config
    switch (SCREEN_WIDTH) {
      case 1920:
        GAME_SCREEN_YSIZE = SCREEN_HEIGHT = 1080;
        NORMAL_SIGHT = 18;
        break;
      case 1280:
        GAME_SCREEN_YSIZE = SCREEN_HEIGHT = 720;
        NORMAL_SIGHT = static_cast<uint16_t>(NORMAL_SIGHT * 0.75);
        break;
      default:
        GAME_SCREEN_XSIZE = SCREEN_WIDTH = 800;
        GAME_SCREEN_YSIZE = SCREEN_HEIGHT = 600;
        NORMAL_SIGHT = static_cast<uint16_t>(NORMAL_SIGHT * 0.45);
        break;
    }

	return	TRUE;
}


bool SDL_LoadSoundList()
{
	// 4. 初始化 AudioManager
	if (!g_audioManager.Init()) {
		g_DBGLog.Log(LOG_LV1, "Failed to initialize AudioManager.");
		ExitApplication(EA_NORMAL);
		return false;
	}
        g_audioManager.SetDefaultVolume(system_info.sound);  // 設定預設音量，範圍 0-128
	// 5. 載入音效列表
	if (!g_audioManager.LoadSoundList("./Sound/SOUNDLIST.DAT", "./Sound/SOUNDLIST.BIN")) {
		g_DBGLog.Log(LOG_LV1, "Failed to load sound list.");
		// 清理 AudioManager 和 SDL 資源
		ExitApplication(EA_NORMAL);
		return false;
	}

	return true;
}

int main(int argc, char* argv[])
{
	// =========================================================================
	// 步驟一：一次性的應用程式初始化 (對應舊程式碼迴圈前的所有內容)
	// 這部分程式碼在整個遊戲生命週期中只會執行一次。
	// =========================================================================
#ifndef _DEBUG
	//g_DBGLog.Init(OUTPUT_JUST_FILE, "CustomerService");
#else
	g_DBGLog.Init(OUTPUT_FILE_AND_CONSOLE, "CustomerService");
#endif

	CrackMgr.SetInitClock();
	CrackMgr.InitCrackMgr();

	InitApplication();
	// 在這裡建立視窗和渲染器 (確保在 InitSDLGraphics 內部啟用了 VSync)
	// SDL_CreateRenderer(..., SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!InitSDLGraphics(g_hwndMain)) {
		g_DBGLog.Log(LOG_LV1, "Failed to initialize SDL Graphics.");
		return FALSE;
	}
    SimpleUpdater updater(g_SDLInfo.renderer);
    if(!updater.Run())
    {
        SDL_Log("Update failed or cancelled.");
        return 0;
    }

    // ★★★ 魔法指令：切換工作目錄 ★★★
    std::string dataPath = GetWritablePath() + "assets/";

    // 如果目錄不存在，chdir 會失敗，那後面就全完了
    if (chdir(dataPath.c_str()) == 0) {
        SDL_Log("★ Working Directory Changed to: %s", dataPath.c_str());
    } else {
        SDL_Log("CRITICAL ERROR: Failed to change directory! errno: %d", errno);
        // 這裡建議報錯並退出，不然讀不到檔案也是死
        return 0;
    }

    if (!CrackMgr.StartCrackThread() || !CrackMgr.LoadCrcDat()) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Init Error", "Can Not Run Program.", NULL);
        return FALSE;
    }

    if (!LoadLanguageText(system_info.lang)) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "File Error", "Language Text file Error.", NULL);
        return FALSE;
    }

	BeforeExe(argv[0]);
	// 準備傳遞給回調函數的數據
	std::string callbackMessage = "Hello from Timer!";
    myTimerID= SDL_AddTimer(1000, myTimerCallback, &callbackMessage);

    if (myTimerID == 0) {
		g_DBGLog.Log(LOG_LV1, "set timer failed! error: %s ", SDL_GetError());
		return 1;
	}

	InitSpriteTransTable();
	/*if (CrackMgr.IsTimeDiscrepancy2()) {
		return FALSE;
	}*/
	// 顯示遊戲啟動時的讀取畫面
	//g_SDLInfo.setBackbuffersurface(g_DestBackBuf, dDxSize, wDxSize);
	//g_SDLInfo.SDL_EraseScreen();
	//PutStartLodingImg();
	//g_SDLInfo.SDL_FlipScreen();
	//SDL_Delay(500);
	// 載入所有遊戲啟動時必需的資源
	SDL_LoadSoundList(); 
	LoadConditionTable(); 
	
	if (!LoadMagicTable2()) {
		g_DBGLog.Log(LOG_LV1, "ERROR LoadMagicTable2");
		return FALSE;
	}
	if (!LoadEffect()) {
		g_DBGLog.Log(LOG_LV1, "ERROR LoadEffect");
		return FALSE;
	}
	
	LoadBGESoundTable(); 
	LoadSmallMapBack(); 
	ViewTipsLoad(0); 
	MakergbTable();
	LoadLevelExpTable();
	LoadCursorAni();
	LoadMouseCursor(std::string("cursor.spr"));
	LoadChatImage(); 
	LoadChatColours(); 
	if (!LoadItemTableData()) {
		g_DBGLog.Log(LOG_LV1, "ItemTableData Init Failed");
		return FALSE;
	}
	if (!g_mgrLimit.Load("./data/AbilityLimit.bin")) {
		g_DBGLog.Log(LOG_LV1, "Loading LimitData Error");
		return FALSE;
	}
	if (!InitNation()) {
		g_DBGLog.Log(LOG_LV1, "Loading NationData Error");
		return FALSE;
	}
	InitHorseAniTable();
	LoadClothOrderData();
	LoadBrightData();
	g_EffectSpriteManager.LoadAllSprite();
	//g_HorseRider.PreloadAllHorseSprites();
	// 初始化我們的 FrameMgr，設定目標邏輯更新率為 17 UPS
	g_FrameMgr.Init(15.0f);
	
	CharContxtMenu.InitContextMenu(system_info.lang); // 角色右鍵選單初始化
	ChatMgr.InitContextMenu(system_info.lang);
	// =========================================================================
	// 步驟二：主迴圈與狀態機 (取代所有 goto 邏輯)
	// =========================================================================
	GameState currentState = STATE_MENU; // 遊戲從選單開始
	bool isRunning = true;

	while (isRunning)
	{
		switch (currentState)
		{
			//對應舊程式碼的 MAIN_MENU_ 標籤
		case STATE_MENU:
		{
			SDL_Delay(500);
            g_RouteSelector.Init(1000, 50, 150, 25,&system_info.route);
			// --- 每次進入選單時的準備工作 ---
			LoadMenuData(0); //g_DBGLog.Log(LOG_LV1, "LoadMenuData(0)");
			LoadMenuData(1); //g_DBGLog.Log(LOG_LV1, "LoadMenuData(1)");
			LoadMenuData(2); //g_DBGLog.Log(LOG_LV1, "LoadMenuData(2)");
			LoadMenuData(4); //g_DBGLog.Log(LOG_LV1, "LoadMenuData(4)");
			LoadMenuData(5); //g_DBGLog.Log(LOG_LV1, "LoadMenuData(5)");

			g_SDLInfo.CreateAtlasFromLoadedData("./data/system.dat", &spr[0], SYSTEM_SPRITE_MAX_);
			StartMenuSetting();
			//MenuSetting(); // 你的原始碼呼叫了兩次，此處保留
			g_DBGLog.Log(LOG_LV1, "StartMenuSetting");
			EWndMgr.setupInputBoxCoordinate(0);
			CursorNo(1);
			
			UpdateClockLogic();
			LoadNPCAccessTable(); g_DBGLog.Log(LOG_LV1, "LoadNPCAccessTable");
			LoadHeroSpriteData(); g_DBGLog.Log(LOG_LV1, "LoadHeroSpriteData");
			// --- 選單的主迴圈 ---
			bool keepMenuRunning = true;
			while (keepMenuRunning)
			{
				SDL_input();
				updateTimer();
				
				// 選單邏輯與渲染。VSync 會在此處自動限制迴圈速度，防止 CPU 100%。
				if (!StartMenuProc()) {
					currentState = STATE_EXIT;
					keepMenuRunning = false;
					continue;
				}

				/*if (CrackMgr.IsTimeDiscrepancy()) {
					keepMenuRunning = false;
				}
				if (CrackMgr.IsDebugger()) {
					keepMenuRunning = false;
				}*/
				// 檢查狀態轉換
				if (SMenu[MN_MAININTERFACE].bActive == TRUE) {
					// 對應 goto START_GAME_
					currentState = STATE_STARTING_GAME;
					keepMenuRunning = false;
				}
				else if (QuitFlag == SWD_QUIT || QuitFlag == SWD_LOGON) {
					// 對應 goto END_
					currentState = STATE_EXIT;
					keepMenuRunning = false;
				}
				//為了避免垂直用步沒有開啟這裡也加上延遲
				g_FrameMgr.LimitRenderFPS();
			}
			break;
		}

		// 對應舊程式碼的 START_GAME_ 標籤下的準備工作
		case STATE_STARTING_GAME:
		{
			MenuSetting();
			SendAllReady();
			if (SCharacterData.nLevel < 8) ViewTipToggle = 1;
			ProtocolProc(&connections);
			YouCanHeroActionProc = 0; YouCanViewTipsCheck = 0;
			EWndMgr.setupInputBoxCoordinate(1);
			CLottoMgr::Create();
			SDL_Delay(2000);
			currentState = STATE_IN_GAME; // 準備完畢，切換到遊戲狀態
            if(Hero){
                // 這樣 GM 面板才能讀取主角座標、發送封包
                CimGui::Instance().SetInGame(true, Hero);
            }
			break;
		}

		// 對應舊程式碼 START_GAME_ 標籤下的 while(TRUE) 迴圈
		case STATE_IN_GAME:
		{
			bool keepGameRunning = true;
			//EWndMgr.showInputBox(HWND_CHAT_INPUT);
			while (keepGameRunning)
			{
				SDL_input();
				updateTimer();
				if (!GameProc())
				{
					currentState = STATE_EXIT;
					keepGameRunning = false;
				}

				if (QuitFlag == SWD_QUIT || QuitFlag == SWD_LOGON) {
					// 對應 goto GOTO_MAIN_MENU_
					currentState = STATE_CLEANING_UP_TO_MENU;
					keepGameRunning = false;
				}
                g_FrameMgr.LimitRenderFPS();

                /*if (CrackMgr.IsDebugger()) {

                    return FALSE;
                }*/
			}
			break;
		}

		// 對應舊程式碼的 GOTO_MAIN_MENU_ 標籤
		case STATE_CLEANING_UP_TO_MENU:
		{
			ReStartMenu();
			g_audioManager.StopAllSounds();
			Release(&connections);
			FreeTOI();
			DestroyRoofHeader(&g_RoofHeader);
			DestroyEventList();
			FreeMapSource();
			DestroyItemList();
			FreeCharacterSpriteDataAll(0, MAX_CHARACTER_SPRITE_);
			DestroyCharacterListExceptHero();
			CLottoMgr::Destroy();
			g_GameInfo.AutoLogin = 0;
            CimGui::Instance().SetInGame(false, nullptr);
			currentState = STATE_MENU; // 清理完畢，回到選單
			break;
		}
		// 對應退出迴圈的邏輯
		case STATE_EXIT:
		{
			isRunning = false;
			break;
		}
		}
	}

	// =========================================================================
	// 步驟三：最終的應用程式清理 (對應舊程式碼的 END_ 標籤)
	// =========================================================================
	CloseNation();
	ExitApplication(EA_NORMAL);

	return 0;
}

void ExitApplication(const eExitAppType eEAType)
{
	WriteInfoFile();
	// 3. 移除定時器 (程式結束前)
	SDL_RemoveTimer(myTimerID);


	CrackMgr.StopCrackThread();

	SendLogOut();
	GameEndFree();

	::Release(&connections);

	DestroyCharacterList(&g_CharacterList);

	FreeAllOfMenu();		// 0927 YGI
	DeleteAllSkillData();
	if (EA_NORMAL != eEAType)
	{
		g_DBGLog.Log(LOG_LV1, "ExitApplication Call(%d)", eEAType);
	}

	if (CheckSumError)
	{
		g_DBGLog.Log(LOG_LV1,"Found corrupted or tampered files while loading.Exiting program. [%d]", CheckSumError);
	}

	//FreeItemTable();
	FreeAllEffectSprite();
	if (lan) delete lan;
	if(g_cMsg) delete g_cMsg;
	g_SDLInfo.CleanupSDLGraphics();
    //  make sure everything flushes
    std::quick_exit(eEAType);
	//std::exit(0);
}

void SetCurrentWorkingDirectory(void)
{
	try {
		// 取得當前程式碼的運行路徑 (Current Working Directory)
		// 這通常是你在終端機中執行程式的目錄
		std::__fs::filesystem::path currentPath = std::__fs::filesystem::current_path();
        g_szCWD = currentPath.string();
	}
	catch (const std::__fs::filesystem::filesystem_error& e) {
		g_szCWD = ".";
	}
}

const char* GetCurrentWorkingDirectory(void)
{
	return	g_szCWD.c_str();
}

void InitConfigPath() {
    // 參數1: 公司/組織名, 參數2: 遊戲名
    // 這會在 Android 建立: /data/data/com.DragonRefactor.dragonraja/files/
    // 這會在 Windows 建立: C:\Users\User\AppData\Roaming\DragonRefactor\DragonRaja

    char* path = SDL_GetPrefPath("DragonRefactor", "DragonRaja");
    if (path) {
        g_GameInfo.configFilePath = std::string(path) + "setting.cfg";
        g_GameInfo.RootPath = std::string(path);
        SDL_free(path); // 記得釋放 SDL 分配的字串記憶體
    } else {
        // 如果失敗，退回到暫存目錄 (極少發生)
        SDL_Log("Error getting pref path: %s", SDL_GetError());
        g_GameInfo.configFilePath = "setting.cfg";
    }

    SDL_Log("Config path set to: %s", g_GameInfo.configFilePath.c_str());
}

void WriteInfoFile(void)
{
    try {
        // [Options]
        InfoFile["options"]["ScreenWidth"] = SCREEN_WIDTH;

        // 簡化寫法：不需要 ? true : false，直接賦值即可
        InfoFile["options"]["dx"] = g_SDLInfo.bFullscreen;

        // --- 修正 Bug ---
        InfoFile["options"]["sound"] = system_info.sound; // 修正對應
        InfoFile["options"]["music"] = system_info.music; // 修正對應
        // ----------------

        InfoFile["options"]["gore"] = system_info.gore;
        InfoFile["options"]["lang"] = system_info.lang;
        InfoFile["options"]["route"] = system_info.route;

        // [Account] (如果日後要啟用的安全寫法)
        /*
        if (!IIDD.empty()) {
            InfoFile["account"]["id"] = IIDD;

            // 安全的加密存檔方式：不要修改原始 PPWW
            std::string encryptedPW = PPWW;
            lightXOR(encryptedPW);
            InfoFile["account"]["pw"] = encryptedPW;
        }
        */

        // 執行存檔 (加入錯誤檢查)
        InfoFile.save(g_GameInfo.configFilePath);

        SDL_Log("Config saved successfully to %s", g_GameInfo.configFilePath.c_str());

    }
    catch (const std::exception& e) {
        // 如果硬碟滿了，或是檔案被設為唯讀，這裡會捕捉到錯誤避免閃退
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to save config: %s", e.what());
    }
}

// 假設你的 IniFile 類別介面類似 map，這裡定義一個 Helper
// 如果你的 INI 庫有提供類似 Get(section, key, default) 的功能，可以直接用
template <typename T>
T LoadIniValue(ini::IniFile& file, const std::string& section, const std::string& key, T defaultValue) {
    // 檢查 Section 和 Key 是否存在
    if (file.count(section) && file[section].count(key)) {
        try {
            // 嘗試讀取並轉換
            return file[section][key].as<T>();
        }
        catch (...) {
            // 轉換失敗 (例如格式錯誤)，回傳預設值
            SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Config format error: [%s] %s. Using default.", section.c_str(), key.c_str());
        }
    }

    // 如果不存在或是讀取失敗，將預設值寫入結構中 (以便之後存檔)
    file[section][key] = defaultValue;
    return defaultValue;
}

void SetInfoFile(void)
{
    InitConfigPath();
    // 1. 嘗試讀取檔案
    bool bFileExists = false;
    if (!g_GameInfo.configFilePath.empty() && std::filesystem::exists(g_GameInfo.configFilePath)) {
        InfoFile.load(g_GameInfo.configFilePath);
        bFileExists = true;
    }
    else {
        SDL_Log("Config file not found, creating new one.");
    }

    // 2. 讀取數值 (使用 Helper，如果找不到會自動填入預設值)
    // 這樣就不需要分 if-else 兩塊寫，邏輯統一

    // [Options]
    int defaultWidth = 1280;
    int width = LoadIniValue<int>(InfoFile, "options", "ScreenWidth", defaultWidth);
    GAME_SCREEN_XSIZE = SCREEN_WIDTH = (width > 0) ? width : defaultWidth; // 簡單防呆

    bool bDx = LoadIniValue<bool>(InfoFile, "options", "dx", false);
    g_SDLInfo.bFullscreen = SysInfo.dx = bDx;

    // 音量讀取並限制範圍 (0-100)，防止手改改壞
    int soundVol = LoadIniValue<int>(InfoFile, "options", "sound", 50);
    system_info.sound = std::clamp(soundVol, 0, 100);

    if(system_info.sound <=0) {
        SDL_Log("system_info.sound==0");
        system_info.sound=50;
    }

    int musicVol = LoadIniValue<int>(InfoFile, "options", "music", 50);
    system_info.music = std::clamp(musicVol, 0, 100);

    if(system_info.music <=0) {
        SDL_Log("system_info.music==0");
        system_info.music =50;
    }

    int scroll = LoadIniValue<int>(InfoFile, "options", "scroll", 2);
    system_info.scroll_speed = std::clamp(scroll, 2, 100);

    system_info.gore = LoadIniValue<bool>(InfoFile, "options", "gore", true);
    system_info.lang = 1;//LoadIniValue<int>(InfoFile, "options", "lang", 0);

    // 這裡就是你原本會崩潰的地方，現在安全了，如果沒有 route，會預設為 0
    system_info.route = LoadIniValue<int>(InfoFile, "options", "route", 0);

    // [Network] (原本你在 else 裡有寫，這裡補上以防萬一)
    // 這裡只是確保 Key 存在，變數可能在別處用到
    LoadIniValue<int>(InfoFile, "network", "Server_count", 0);

    // [Account] (註解掉的部分保留邏輯框架)
    /*
    std::string id = LoadIniValue<std::string>(InfoFile, "account", "id", "");
    std::string pw = LoadIniValue<std::string>(InfoFile, "account", "pw", "");
    if (!id.empty()) { ... }
    */

    // 3. 設置全域設定
    g_SelectServerIndex = 0;
    g_audioManager.SetVolumePtr(&system_info.music, &system_info.sound);

    // 4. 存檔機制
    // 如果檔案原本不存在，或者我們在讀取過程中補入了缺失的 Key (透過 LoadIniValue 寫入)
    // 就執行存檔。這樣可以確保玩家的 config 檔永遠是最新的結構。
    try {
        InfoFile.save(g_GameInfo.configFilePath);
    }
    catch (const std::exception& e) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to save config: %s", e.what());
    }

    // Debug 顯示 (只在 Debug 模式且檔案存在時顯示，避免洗版)
#ifdef _DEBUG
    if (bFileExists) {
		SDL_Log("Config loaded: %dx%d, FS:%d, S:%d, M:%d",
			GAME_SCREEN_XSIZE, GAME_SCREEN_XSIZE * 3 / 4, // 假設 4:3 或自定義
			g_SDLInfo.bFullscreen, system_info.sound, system_info.music);
	}
#endif
}

BOOL
ShowErrorMessage(std::string msg)
{
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Dragon Raja Online", msg.c_str(), g_SDLInfo.window);
	return	FALSE;
}






///////////////////////////////////////////////////////////////////////////////
// window procedure										

// 假設 g_SDLInfo.renderer 是一個全域或外部可訪問的 SDL_Renderer*
// 假設 g_pointMouseX/Y, g_pointMouse, Mapx, Mapy, Mox, Moy 等是全域變數

void MouseProcess(SDL_Event* e)
{
	if (e)
	{
		switch (e->type)
		{
		case SDL_MOUSEBUTTONDOWN:
		{
			if (e->button.button == SDL_BUTTON_LEFT) {
				// printf("Left mouse button pressed at (%d, %d)\n", mouseX, mouseY);
			    YouCanHeroActionProc = 1; YouCanViewTipsCheck = 1; g_nLButtonState = STATE_BUTTON_PRESSED;	 LButtonDownIng = 1;

				// 檢查雙擊
				if (e->button.clicks == 2) {
					// printf("Double click detected!\n");
					g_nLDButtonState = STATE_BUTTON_DOUBLECLICK;
				}
			}
			else if (e->button.button == SDL_BUTTON_RIGHT) {
				// printf("Right mouse button pressed at (%d, %d)\n", mouseX, mouseY);
				g_nRButtonState = STATE_BUTTON_PRESSED;	 RButtonDownIng = 1;

				// 檢查雙擊
				if (e->button.clicks == 2) {
					// printf("Double click detected!\n");
					g_nRDButtonState = STATE_BUTTON_DOUBLECLICK;
				}
			}
			else if (e->button.button == SDL_BUTTON_MIDDLE) {
				// printf("Middle mouse button pressed at (%d, %d)\n", mouseX, mouseY);
			}

		}break;
		case SDL_MOUSEBUTTONUP:
		{
			if (e->button.button == SDL_BUTTON_LEFT) {
				// printf("Left mouse button pressed at (%d, %d)\n", mouseX, mouseY);
				g_nLButtonState = STATE_BUTTON_RELEASED; LButtonDownIng = 0; YouCanHeroActionProc = 0;
			}
			else if (e->button.button == SDL_BUTTON_RIGHT) {
				// printf("Right mouse button pressed at (%d, %d)\n", mouseX, mouseY);
				g_nRButtonState = STATE_BUTTON_RELEASED; RButtonDownIng = 0;
			}
			else if (e->button.button == SDL_BUTTON_MIDDLE) {
				// printf("Middle mouse button pressed at (%d, %d)\n", mouseX, mouseY);

			}
			// 檢查雙擊
			if (e->button.clicks == 2) {
				// printf("Double click detected!\n");
			}
		}break;
		}

		// 1. 取得當前鼠標的實際物理座標
		/*float current_physicalX, current_physicalY;
		SDL_RenderWindowToLogical(
			g_SDLInfo.renderer, // 傳入您的 renderer
			e->motion.x,     // 原始 X
			e->motion.y,     // 原始 Y
			&current_physicalX,     // 轉換後的 X (使用 float)
			&current_physicalY      // 轉換後的 Y (使用 float)
		);*/
		g_pointMouseX = e->motion.x;
		g_pointMouseY = e->motion.y;
		// 3. 更新其他依賴邏輯座標的變數
		g_pointMouse.x = g_pointMouseX;
		g_pointMouse.y = g_pointMouseY;

		// Mox/Moy 現在也將是基於正確邏輯座標計算出來的地圖絕對座標
		Mox = Mapx + g_pointMouseX;
		Moy = Mapy + g_pointMouseY;
	}
	/*if (LButtonDownIng == 1)
	{
		g_DragMouse.ex = x + Mapx;	//마우스의 화면좌표에 맵의 절대 좌표를 더함
		g_DragMouse.ey = y + Mapy;
	}*/
}


int LoadingGameData()
{	// eLoadingGameDataStep 를 사용
	static int s_iLoadingPos = LGDS_FIRST;

	switch (s_iLoadingPos)
	{
	case LGDS_FIRST:
	{
		LoadAttackRangeTable(0);
		LoadAttackRangeTable(1);
#ifdef _DEBUG // finito 060507
		if (SysInfo.notconectserver)
		{
			s_iLoadingPos = LGDS_STEP1;
			return s_iLoadingPos;
		}
#endif

		DWORD StartTime = SDL_GetTicks();
		ListenCommand = CMD_NONE;
		while (true)
		{
			DWORD Duration = SDL_GetTicks() - StartTime;

			if (Duration > WAIT_TIME_RESPONSE_DURATION) // 030930 kyo
			{
				s_iLoadingPos = LGDS_FIRST;
				return -1;
			}

			if (HandleRunning(&connections) <= 0)
			{
				Release(&connections);
				s_iLoadingPos = LGDS_FIRST;
				return -2;
			}

			if (ListenCommand == CONNECT_REFUSE)
			{
				s_iLoadingPos = LGDS_FIRST;
				return -3;
			}

			if (ListenCommand == CMD_CONNECT_INFO)
			{
				s_iLoadingPos = LGDS_STEP1;
				return s_iLoadingPos;
			}
		}

		break;
	}
	case LGDS_STEP1:
	{
		HandleRunning(&connections);
		g_Volume_Off = 1; // 로딩이 끝날 때까지 배경음악 이외의 소리 안나오게
#ifdef _SDL2 //for test many Hero in the map
		if (!BuildCharacterList(&g_CharacterList)) {
            JustMsg(lan->OutputMessage(6, 8));  // 010215 lsw
        }
#endif
		s_iLoadingPos = LGDS_STEP2;
		break;
	}
	case LGDS_STEP2:
	{
		//MapBuild(&g_Map, MapName);	// map
		MapBuild_Refactored(&g_Map, MapName);

		if (HandleRunning(&connections) <= 0)
		{
			Release(&connections);
			return -2;
		}

		s_iLoadingPos = LGDS_STEP3;
		break;
	}
	case LGDS_STEP3:
	{
		LoadTOI(MapName);
		HandleRunning(&connections);
		BuildRoofHeader(&g_RoofHeader, MapName);					// rof
		HandleRunning(&connections);
		BuildEventList(MapName);									// ent
		HandleRunning(&connections);

		ReqTacSkillExp();	// 전체 택틱 수치 가져오기
		HandleRunning(&connections);
		AllReqSkillExp();	// 전체 스킬 경험치 가져오기
		HandleRunning(&connections);
		s_iLoadingPos = LGDS_STEP4;
		break;
	}
	case LGDS_STEP4:
	{
		LoadMapSource2(MapName);									// sou		
		std::string Map = "./map/" + std::string(MapName) + ".sou2";
		g_SDLInfo.CreateAtlasForMap_CPP(Map.c_str(), Sou, TILE_MAX_, true);
		HandleRunning(&connections);
		s_iLoadingPos = LGDS_STEP5;
		break;
	}
	case LGDS_STEP5:
	{
		//LoadCharSpriteData("./char/000000.spr", &CharBuf[0], &CharBufLength[0], &CharSpr[0], LD_CHAR_SPRITE_ALL_LOAD);
		HandleRunning(&connections);
		s_iLoadingPos = LGDS_STEP6;
		break;
	}
	case LGDS_STEP6:
	{
		//LoadCharSpriteData("./char/001000.spr", &CharBuf[1], &CharBufLength[1], &CharSpr[1], LD_CHAR_SPRITE_ALL_LOAD);
		//LoadHeroClothAccessoryData(104, 107);

		char tempfilename[FILENAME_MAX];
		const int preloadcharsprite[21] = { 29,30,37,38,46,72,79,1005,1006,1017,1018,1019,1020,1027,1028,1041,1060,1061,1075,1083,1092 };

		for (int i = 0; i < 21; ++i)
		{
			int tt = preloadcharsprite[i];
			sprintf(tempfilename, "./char/%06d.spr", tt);
			LoadCharSpriteData(tempfilename, &CharAccessoryBuf[tt / 1000][tt % 1000], &CharAccessoryBufLength[tt / 1000][tt % 1000], &CharAccessorySpr[tt / 1000][tt % 1000], LD_CHAR_SPRITE_ALL_LOAD);
			HandleRunning(&connections);
		}
		s_iLoadingPos = LGDS_STEP7;
		break;
	}
	case LGDS_STEP7:
	{
		//LoadClothOrderData();
		
		HandleRunning(&connections);
		s_iLoadingPos = LGDS_STEP8;
		break;
	}
	case LGDS_STEP8:
	{
		//LoadBrightData();
		HandleRunning(&connections);
		s_iLoadingPos = LGDS_STEP9;
		break;
	}
	case LGDS_STEP9:
	{
		StartSetView(Hero);
		HandleRunning(&connections);
		s_iLoadingPos = LGDS_STEP10;
		break;
	}
	case LGDS_STEP10:
	{
		//LoadMenuData(1);
		HandleRunning(&connections);
		s_iLoadingPos = LGDS_STEP11;
		break;
	}
	case LGDS_STEP11:
	{
		//LoadMenuData(2);
		HandleRunning(&connections);
		s_iLoadingPos = LGDS_STEP12;
		break;
	}
	case LGDS_STEP12:
	{
		LoadHongSkillTable();
		HandleRunning(&connections);
		//LoadMenuData(5);
		//HandleRunning(&connections);

		if (CurOpenHouse >= 0)
		{
			RecvRoofOpen(CurOpenHouse);
		}

		g_pNumberAniMgr.InitHpUpdateQueue();
		HandleRunning(&connections);
		s_iLoadingPos = LGDS_FIRST;
		
		return LGDS_END; // 이것만 예외 처리
	}
	}

	return s_iLoadingPos;
}