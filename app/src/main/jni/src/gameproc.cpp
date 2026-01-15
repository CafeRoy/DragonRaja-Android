#include "stdafx.h"
#include "gameproc.h"
#include "map.h"
#include "char.h"
#include "path.h"
#include "network.h"
#include "object.h"
#include "Hong_Sprite.h"
#include "Hong_Light.h"
#include "Hong_Sub.h"
#include "menu.h"
#include "Hangul.h"
#include "dragon.h"
#include "Effect.h"
#include "DirectSound.H"
#include "CharDataTable.h"
#include "Tool.h"
#include "MouseCursor.h"
#include "Skill.h"
#include "Item.h"
#include "SkillTool.h"
#include "SmallMenu.h"
#include "LineCommand.h"
#include "Counseler.h"
#include "NPC_Pattern.h"
#include "chat.h"
#include "cScreenEffect.h"
#include "CurrentMsgMgr.h"
#include "ExchangeMgr.h"
#include "NumberAniMgr.h"
#include "TextOutMgr.h"
#include "FrameMgr.h"
#include <SDL.h>
#include "CRenderMethod.h"
#include "CharacterContxtMenu.h"
#include "utf8.h"
#include "CrackMgr.h"
#include "RouteSelector.h"
#include "CimGui.h"

#include "MobileBridge.h"
///////////////////////////////////////////////////////////////////////////////
// 
extern DWORD ServerAccelTime;
extern DWORD g_ClientTime_accel;
extern DWORD g_ClientTimeNoGap;		//011122 lsw
extern void HprintBold(const int x, const int y, const int fc, const int bc, char* s, ...);
extern void Hprint(const int x, const int y, char* destbuf,const char* szStr);
extern int RecvChangeMap(t_server_change_map* p);
extern void MouseProcess(SDL_Event* e);
extern NPC_INFO g_infNpc[MAX_CHARACTER_SPRITE_]; // CSD-030419
extern CHARACTERLIST	g_CharacterList;
extern void DisplayEmote(const int x, const int y, const int iSmileNo);//020515 lsw
extern BOOL LoadGameInfo(void);
extern int	GetItemAttr(int item_id, int attr); // MenuSet.h

extern void SendAction(int i, int Direction);			// LTS ACTION
extern void SWD_LOGON_Clicked(const int nMenuIndex);

extern bool g_bWarBegin;	// 011022 LTS
extern int	g_EventLocalWarBegin;		// 020115 LTS

extern int g_MouseItemType;	// menu.cpp
extern int g_MouseItemNumber;
extern int g_ActionAnger;		// LTS ACTION
extern int g_BoadAndSignNumber;	// CSD-030324 // thai2 YGI

extern bool g_allowDrop; //Eleval 22/05/09 for /drop command

////////////////////// SoundUp lkh �߰� ///////////////////////
extern int	BGM_SoundVolume;
extern BOOL	BGM_FadeInOut;
std::string title = "";
ORDERIFNO		g_OrderInfo;
int				g_OldOrderInfoCount;
int g_nBaseOrderCount = 0; // 用來記錄邏輯層產生的物件數量
int g_nMagicAddedLastFrame = 0;	// 用來記錄上一幀新增的魔法物件數量
static DWORD STDTime;
extern int NPCCOUNT;
extern	int			ListenCommand;		// network.cpp
extern	SystemInfo	system_info;
extern  int BetaTest;
int TestDeter;
DWORD	BeforeWalkClickTime, AfterWalkClickTime;
bool	DontMoveAttackFlag = false;

bool    CheckPC = false;

int NewOrderC = 0;
ORDERTABLE neworder[MAX_ORDERTABLE] = { 0, };

int PathBuildSuccess = 0;

bool HeroActionProcChecked;//  ���� ���콺Ŭ����   HeroActionProc()�� ȣ����� �ʾҴٸ� DoLButtonUp()���� ȣ���ϰ� �Ѵ�. 


const char ordermsg[6][30] = { "Body.Weapon.Shield",
								"Body.Shield.Weapon",
								"Weapon.Body.Shield",
								"Weapon.Shield,Body",
								"Shield.Weapon,Body",
								"Shield.Body,Weapon" };



int AutoAttackFlag;
int AutoAttackSpriteType;
LPCHARACTER AutoAttacklpcharacter;
int AutoAttackx, AutoAttacky;

bool ScreenCaptureStartFlag;

int real_move = 0;
int YouCanHeroActionProc=0;
int YouCanViewTipsCheck=0;
bool ViewTipToggle=0;		// 0818 khs

int Moveoldx, Moveoldy;	// �̵��� ���� ���콺 Ŭ���� ������ ����ϰ� �ִ´�. 
DWORD Movetime;

LPCHARACTER MouseCheckCharacterName;  //  ���콺�� ���� ���õǴ� 





///////////////// 0430 lkh �߰� ////////////////////
BOOL	g_AttackSkill_Trace1 = FALSE;
BOOL	g_AttackSkill_Trace2 = FALSE;


int ChangeMapFlag;				// 0825 KHS
t_server_change_map ChangeMapPacket;

//string inputText;

///////////////////////////////////////////////////////////////////////////////
//

BOOL		GameProc();
void		KeyboardProc(void);
void		MouseProc(void);

int			ProtocolProc(t_connection* c, bool flag);
void		CharacterProcCheck(void);
void		CharacterProcFrameCount(void);

void		DoLButtonDown(void);
void		DoLButtonUp(void);
void		DoRButtonDown(void);
void		DoRButtonUp(void);
void		DoLDButtonDown(void);
void		DoRDButtonDown(void);
void		DoLbuttonStill();

void		CalcOrder(void);
void		DisplaySprite(void);
void		DisplaySkillCursor(void);
void		DisplayTileFirst(void);
void		DisplaySpriteBottom(void);
void		DisplayShadowFirst(void);
void		DisplaySpriteCeiling(void);
void		DisplaySpriteTrans(void);
void		DisplaySpriteUnderTile(void);
void		DisplaySpriteNo(void);

void		_DisplayMapObjectAfter(void);

void		SettingDestPoint(int x, int y);
void		PutDestPoint(void);
void		UnDisplayDestPoing(LPCHARACTER ch);
void		DrawTextOnScreen();

void		DoHeroAttack(int x, int y);

///////////////////// 0204 lkh ���� /////////////////////////
bool		ReturnCheckObject(LPMAPOBJECT mo);
bool		GetBoardData(int board_Number, char* str_Data);
bool		GetDirectBoardData(int board_Number, DIRECTBOARDDATA* directboardData);
extern void	PutBoardMenu(int board_Number, char* str_Data);
extern void	PutDirectBoardMenu(const DIRECTBOARDDATA* directboardData);

extern void DisplayFightStone(void);
extern bool isOurSquad(LPCHARACTER hero, LPCHARACTER ch);		// 011018 LTS
extern void LocalWarInfoDisPlay();								// LTS LOCALWAR

extern void	CheckParticleEffect();	// LTS DRAGON

extern void ShowMonsterWarInfo();	// BBD 040311
bool CompareOrderItems_Safe(const ORDERTABLE& a, const ORDERTABLE& b);
///////////////////// SoundUp lkh �߰� /////////////////////////
void	ObjectSoundPlay(void)
{
	int soundNo = 0, soundFrame = 0, soundDelay = 0;
	int	soundCount = 0, soundOverlap = 0, soundType = 0;

	LPMAPOBJECT		mo;
	for (int i = 0; i < TotalMapObject; i++)
	{
		mo = &Mo[i];
		int mox = mo->x;
		int moy = mo->y;
		soundNo = mo->soundno % 1000;
		if (IsScreenAreaView(mox, moy))
		{
            Spr* sp = &MoImage[mo->id - 1].spr[(int)mo->curframe];


			if (soundNo > 0)
			{
				soundCount = mo->soundno / 1000;

				soundFrame = mo->soundframe % 1000;
				soundOverlap = mo->soundframe / 1000;		//��ø ȿ���� ����� ����

				soundDelay = mo->sounddelay % 1000;
				soundType = mo->sounddelay / 1000;		//���� ��� ���

				if (mo->delay == 0)// || soundFrame==99)		//map_delay�� 0�� ��� �Ǵ� ������ ��ø ���� ����� ���
				{
					int sx = mo->x;
					int	sy = mo->y;

					//int distance_X = abs(Hero->x - sx)/(TILE_SIZE);		//�Ÿ��� Ÿ�� ������ ȯ��	// LTS SOUND
					//int distance_Y = abs(Hero->y - sy)/(TILE_SIZE);									// LTS SOUND

					int distance_X = sx - Hero->x;		//�Ÿ��� Ÿ�� ������ ȯ��	// LTS SOUND
					int distance_Y = sy - Hero->y;		

					//if( sx < Hero->x)	distance_X = -distance_X;
					//if( sy < Hero->y)	distance_Y = -distance_Y;

					if (soundNo && (mo->curframe == soundFrame || soundFrame == 99))			//���� ȿ�����̰ų� ���� ��� �ش� �������� ����� ���
					{
						BOOL	play = 1;

						if (soundType != 2 && soundDelay)		// type->random ���
						{
							if ((soundCount) <= 0)
								mo->soundno = (mo->soundno % 1000) + soundDelay * 1000;		//soundcount set
							else
								play = 0;
							mo->soundno -= 1000;			//soundcount--
						}

						int	sound_Num = 0;

						BOOL	play_Rate = 0;//	= (mo->sounddelay%1000)/100;	//0->�ش� �����ӿ��� ������ ��� 1->�ش� �����ӿ��� �ش� ������ ���� ���

						int		probability = 0;

						switch (soundType)
						{
						case 1:		//3way
							sound_Num = soundNo + rand() % 3;
							break;
						case 2:		//Ȯ���� ���� �Ҹ� ���
							play_Rate = 1;
							probability = soundDelay;
							sound_Num = soundNo;
							break;
						default:
							sound_Num = soundNo;
							break;
						}

						if (play)
						{
							if ((play_Rate && (rand() % 101 <= probability)) || !play_Rate)	//Ȯ���� ���� ��µǴ� ���� Ȯ���� ���� ���� ���
							{
								if (soundOverlap)		//true->������ �ش� �����ӿ��� ���(������Ʈ�� �������� ���� ��� ��ø��)
								{
									PlayListAutoSounds(sound_Num, distance_X, distance_Y, 0);
									
								}
								else					//false->��� �����ӿ� ������� ��ø���� �ʰ� 
								{
									if (!IsPlaying(sound_Num))
										PlayListAutoSounds(sound_Num, distance_X, distance_Y, 0);	//3D sound�� ǥ�� // LTS SOUND
								}
							}
						}
					}
				}
			}
		}
		else
		{
			soundOverlap = mo->soundframe / 1000;
			if(soundNo > 0 && soundOverlap)
			{   
				 if (IsPlaying(soundNo)) StopWave(soundNo);
			}
		}
	}
}

void DisplayDebug(void)
{
	ViewCounselerStatus();
#ifdef _DEBUG		
	int i;
	//ViewCounselerStatus();

	/*if (tool_ID_INPUT_MAP_ATTR)
	{
		Hcolor(0, 255, 0);
		Hprint2(10, 300, g_DestBackBuf, "Working Map Attr.");
		if (tMapAttrAllAffect == IDC_SOURCE_AFFECT_YES)
		{
			Hcolor(255, 0, 0);
			Hprint2(10, 320, g_DestBackBuf, "Effect to All Source");
		}

		switch (tMapAttr)
		{
		case IDC_ATTR_DONT:	Hcolor(255, 0, 0);
			Hprint2(10, 340, g_DestBackBuf, "Dont Attr");
			break;
		case IDC_ATTR_RIVER:	Hcolor(0, 100, 200);
			Hprint2(10, 340, g_DestBackBuf, "Watter Attr");
			break;
		case IDC_ATTR_GEFFECT:	Hcolor(0xffff);
			Hprint2(10, 340, g_DestBackBuf, "EffectWall Attr");
			break;
		}
	}*/

	Hcolor(0, 255, 0);
	if (GetSysInfo(SI_GAME_MAKE_MODE) && Hero)
	{
		Hprint2(400, 400, g_DestBackBuf, "%d ", Hero->toflag);
		if (tool_MouseCursorBlock)
		{
			Hprint2(10 - 1, 110 - 1, g_DestBackBuf, "CursorAni NO : %d ", CurMouseCursorNo);
			Hprint2(10 + 1, 110 + 1, g_DestBackBuf, "CursorAni NO : %d ", CurMouseCursorNo);
			Hcolor(255, 0, 0);
			Hprint2(10, 110, g_DestBackBuf, "CursorAni NO : %d ", CurMouseCursorNo);
			Hcolor(0, 255, 0);
		}

		if (SkillStatus)
		{
			switch (SkillStatus)
			{
			case SKILL_ON: Hprint2(200, 180, g_DestBackBuf, "Skill_ON");		break;
			case SKILL_ING:  Hprint2(200, 180, g_DestBackBuf, "Skill_ING");	break;
			}
		}

		LPCHARACTER ch = Hero;
		i = 1;
		int npcno = 0, pcno = 0;
		int itemno_ = 0;
		if (SysInfo.notconectserver == 0)
			while (ch != NULL)
			{
				if (ch->id >= 10000) npcno++;
				else pcno++;
				ch = ch->lpNext;
			}
		LPITEMGROUND titem = g_Item;
		while (titem)
		{
			itemno_++;
			titem = titem->next;
		}
		Hprint2(540, 57, g_DestBackBuf, "NPC: %d  PC:%d  Item:%d ", npcno, pcno, itemno_);

		if (tool_IDD_INPUT_MAPOBJECT)
		{
			static int tt, tto;
			if (tto) { tt++; if (tt > 5) { tto = !tto; } }
			else { tt--; if (tt < -5) { tto = !tto; } }

			if (TempMoImage.spr[0].img)	PutCompressedImage(160, 300 + tt, &TempMoImage.spr[0]);
		}
	}

	if (SysInfo.viewcharacterframe && Hero)
	{
		h_DisplayPC_Temp(Hero);
		Hcolor(0xffff);
		Hprint2(10, 120, g_DestBackBuf, "Dir:%d  Frame:%d ", SpriteOrderDir, SpriteOrderFrame);
		Hprint2(10, 140, g_DestBackBuf, "%s", ordermsg[SpriteOrder[Hero->sprno][SpriteOrderDir][SpriteOrderFrame]]);
	}
#endif
}

void SendReqHpAllCharacter(LPCHARACTER ch) {

	if (ch->viewdebugcharinfo >= 1)
		if (g_GameInfo.lpcharacter == ch)
		{
			ch->viewdebugcharinfo--;
		}
	if (ch->viewdebugcharinfo == 0)
	{
		ch->viewdebugcharinfo = -1;
		SendReqCharInfo(ch->id);
		SendReqSkillInfo(ch);				//1004 YGI
	}
}


// 在你的 GameUpdate 或 Render 開始處呼叫這個
void UpdateTextCache() {
    std::vector<TextResult> results;
    // 一次拿回所有做好的
    AsyncTextManager::Instance().FetchResults(results);

    for (const auto& res : results) {
        if (res.surface) {
            // 存入快取
            TextRenderKey new_key = res.key;
            new_key.lastUsed = std::chrono::steady_clock::now();

            g_textSurfaceCache.emplace(
                    new_key,
                    std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)>(res.surface, &SDL_FreeSurface)
            );
        }
    }
}

BOOL StartMenuProc()
{
    // 1. 處理輸入 (每一幀都要做)
    MouseProc();
    if (ProtocolProc(&connections) < 0) return FALSE;

    // 2. 邏輯區 (Logic)
    g_FrameMgr.UpdateTimers();

    // ★ 修正點：邏輯更新
    if (g_FrameMgr.ShouldUpdateLogic())
    {
        g_SDLInfo.g_linesToDrawThisFrame.clear();
        StartMenuChecking();

        // 這裡放其他的邏輯更新...
        if (!EWndMgr.checkInputBoxVisibility(HWND_1) && g_StartMenuOn && SMenu[MN_LOGIN].bActive) {
            EWndMgr.showInputBox(HWND_2);
            EWndMgr.showInputBox(HWND_1);
        }
        g_GameInfo.AutoLogin++;

        // ★ 修正點：鼠標動畫放在這裡！
        // 這樣無論 FPS 是 60 還是 200，鼠標動畫速度都固定 (跟隨邏輯頻率)
        CurMouseCursorFrameCounter();

        // ★ 關鍵修正：按鍵重置必須放在這裡！
        // 只有當邏輯真正「處理過」按鍵後，才能把它重置。
        // 否則如果渲染跑得比邏輯快，按鍵訊號會被「誤殺」。
        g_nLButtonState = STATE_BUTTON_DOUBLECLICK;
        g_nRButtonState = STATE_BUTTON_RELEASED;
        g_nLDButtonState = STATE_BUTTON_RELEASED;
        g_nRDButtonState = STATE_BUTTON_RELEASED;
    }

    // 3. 渲染區 (Render)

    g_SDLInfo.setBackbuffersurface(g_DestBackBuf, dDxSize, wDxSize);
    UpdateTextCache();

    StartMenuDisplay();
    g_RouteSelector.Render(g_SDLInfo.gFont);
    g_SDLInfo.SDL_DrawInputbox();

    // 繪製滑鼠 (位置更新要每幀做，但動畫換幀由上面邏輯控制)
    CursorDisplayBack();
    // 4. 上傳與顯示
    // 因為開了 VSync，這裡會自動等待，鎖定 60 FPS
    g_SDLInfo.SDL_FlipScreen();

    return TRUE;
}

void setWindowTitle()
{
	title = "";
	title += g_FrameMgr.PrintRenderInfo();
	title += " - FPS: " + std::to_string(g_FrameMgr.GetRenderFPS()); /*+ " UPS: " + std::to_string(g_FrameMgr.GetLogicUPS());*/
	title += " - ping: " + g_GameInfo.GetAveragePing() + "ms";
	title += " - [Del]AutoClick[";
	std::string s = g_GameInfo.mouseClick ? "On" : "Off";
	title += s + "] ";

	//title += std::to_string(g_OldOrderInfoCount);

	SDL_SetWindowTitle(g_SDLInfo.window, title.c_str());

}
void mouseClick()
{
  static unsigned long click = 0;
  int mouse_speed = 101 - system_info.mouse_speed;
  if (g_GameInfo.mouseClick && (click++ % mouse_speed)==0) {
    YouCanHeroActionProc = 1;
    YouCanViewTipsCheck = 1;
    g_nLButtonState = STATE_BUTTON_RELEASED;
    LButtonDownIng = 1;
  } else {
  }
}

// 判斷座標是否在螢幕範圍內 (給魔法特效用)
// 參數 x, y: 世界座標 (World Coordinates)，也就是 pML->fVisualX
bool IsScreenArea(int x, int y)
{
    // 算出物件在螢幕上的相對位置
    int screen_x = x - Mapx;
    int screen_y = y - Mapy;

    // 設定一個寬鬆的邊界 (Margin)，比如 200 像素
    // 這是為了防止特效很大，中心點出了螢幕，但邊緣還在螢幕內卻被切掉的情況
    const int MARGIN = 200;

    if (screen_x < -MARGIN) return false; // 太左邊
    if (screen_y < -MARGIN) return false; // 太上面
    if (screen_x > GAME_SCREEN_XSIZE + MARGIN) return false; // 太右邊
    if (screen_y > GAME_SCREEN_YSIZE + MARGIN) return false; // 太下面

    return true;
}

void ReputMagicEffect() {

    // =========================================================
    // 步驟 1: 先把原本的 Order 表 (角色、道具、傷害) 複製到 neworder
    // =========================================================
    // 這是最安全的做法，完全不動 g_OrderInfo，只讀取它
    NewOrderC = 0;
    if (g_OrderInfo.count > 0) {
        memcpy(neworder, g_OrderInfo.order, sizeof(ORDERTABLE) * g_OrderInfo.count);
        NewOrderC = g_OrderInfo.count;
    }
    // 設定目前的渲染數量 (以此為基礎往上加特效)


    // =========================================================
    // 步驟 2: 把特效直接加入到 neworder (渲染陣列)
    // =========================================================
    for (const auto& cmd : g_EffectBuffer)
    {
        MAGICLIST* pML = cmd.pOwner;
        if (!pML) continue;

        // --- 這裡保留你已經驗證成功的「相對座標」邏輯 ---
        float finalWorldX, finalWorldY;

        if (cmd.anchorType == 1 && pML->lpChar_Own) {
            finalWorldX = pML->lpChar_Own->visual_x + cmd.relativeOffsetX;
            finalWorldY = pML->lpChar_Own->visual_y + cmd.relativeOffsetY;
        }
        else if (cmd.anchorType == 2 && pML->lpChar_Target) {
            finalWorldX = pML->lpChar_Target->visual_x + cmd.relativeOffsetX;
            finalWorldY = pML->lpChar_Target->visual_y + cmd.relativeOffsetY;
        }
        else {
            finalWorldX = pML->fVisualX + cmd.relativeOffsetX;
            finalWorldY = pML->fVisualY + cmd.relativeOffsetY;
        }

        // --- 螢幕剔除 ---
        if (!IsScreenArea((int)finalWorldX, (int)finalWorldY)) continue;

        // --- 安全檢查 ---
        if (NewOrderC >= MAX_ORDERTABLE) break;

        // --- 【關鍵】直接填入 neworder，完全不碰 g_OrderInfo ---
        LPORDERTABLE o = &neworder[NewOrderC];

        o->wType = cmd.sort_Type;
        o->height = cmd.height;
        o->alpha = cmd.alpha;
        o->rgb = cmd.rgb;
        o->lpvData = (LPVOID)cmd.sp;

        switch (cmd.type / 10) {
            case 1: o->type = 1; break;
            case 2: o->type = 2; break;
            default: o->type = cmd.type; break;
        }

        int shakeX = 0, shakeY = 0;
        if (o->type == 7) {
            shakeX = (rand() % 5 - 2);
            shakeY = (rand() % 5 - 2);
        }

        o->x = (int)finalWorldX + shakeX;
        o->y = (int)finalWorldY + shakeY;
        o->offset = (int)finalWorldY; // Y軸排序關鍵
        o->show = 1;
        o->sort = 1;

        // 渲染數量 +1
        NewOrderC++;
    }

    // =========================================================
    // 步驟 3: 最後統一排序
    // =========================================================
    if (NewOrderC > 0) {
        // 這會把「角色/道具」和「剛剛加進去的特效」混在一起依照 Y 軸排序
        // 這樣遮擋關係就正確了
        std::sort(neworder, neworder + NewOrderC, CompareOrderItems_Safe);
    }
}
void HandleCameraScrollingPerFrame()
{
    if (Hero == NULL) return;
    if (g_IsJoystickMoving)
    {
        // 取得螢幕能夠顯示多少格 Tile (例如 800px / 32 = 25格)
        // 注意：這裡假設 GAME_SCREEN_XSIZE 是您的解析度寬度常數
        int screenTileW = GAME_SCREEN_XSIZE / 32;
        int screenTileH = GAME_SCREEN_YSIZE / 32;

        // 計算主角所在的 Tile 座標
        int heroTileX = Hero->visual_x / 32;
        int heroTileY = Hero->visual_y / 32;

        // ★ 核心修正：主角座標 - 半個螢幕寬度 = 鏡頭左上角座標 ★
        g_Map.tox = heroTileX - (screenTileW / 2);
        g_Map.toy = heroTileY - (screenTileH / 2);

        // 防止鏡頭跑出地圖邊界 (Clamp)
        // 這是為了避免主角在邊緣時，鏡頭看到地圖外面的黑邊
        if (g_Map.tox < 0) g_Map.tox = 0;
        if (g_Map.toy < 0) g_Map.toy = 0;
    }
    // 1. 選單判斷
    bool bMovingMenu = (!IsChatBoxLock() && g_MouseInMenu && TileMap[Hero->x >> 5][Hero->y >> 5].attr_no_battle);
    if (bMovingMenu) return;

    // 2. 累加器
    static float fAccumTox = (float)g_Map.tox;
    static float fAccumToy = (float)g_Map.toy;

    if (fabs(fAccumTox - (float)g_Map.tox) > 1.0f) fAccumTox = (float)g_Map.tox;
    if (fabs(fAccumToy - (float)g_Map.toy) > 1.0f) fAccumToy = (float)g_Map.toy;

    // 3. 邊界計算
    float half_w_tile = (float)GAME_SCREEN_XSIZE / 32.0f / 2.0f;
    float half_h_tile = (float)GAME_SCREEN_YSIZE / 32.0f / 2.0f;
    float ideal_tox = (Hero->visual_x / 32.0f) - half_w_tile;
    float ideal_toy = (Hero->visual_y / 32.0f) - half_h_tile;

    float max_offset = (float)Hero->sight;
    float min_limit_x = ideal_tox - max_offset - 3.0f;
    float max_limit_x = ideal_tox + max_offset + 3.0f;
    float min_limit_y = ideal_toy - max_offset + 5.0f;
    float max_limit_y = ideal_toy + max_offset - 5.0f;

    // 4. 捲動參數
    const float step = 0.5f + static_cast<float>(system_info.scroll_speed / 50.0f);
    const int margin = 3;
    bool isInputActive = false;

    // --- 輸入處理 ---
    if (!IsChatBoxActive() && !g_IsJoystickMoving)
    {
        if (isKeyPressed(SDL_SCANCODE_LEFT)) { fAccumTox -= step; isInputActive = true; }
        if (isKeyPressed(SDL_SCANCODE_RIGHT)) { fAccumTox += step; isInputActive = true; }
        if (isKeyPressed(SDL_SCANCODE_UP)) { fAccumToy -= step; isInputActive = true; }
        if (isKeyPressed(SDL_SCANCODE_DOWN)) { fAccumToy += step; isInputActive = true; }
    }

    if (!isInputActive && g_StartMenuOn == false)
    {
        if (g_pointMouseX < margin) { fAccumTox -= step; isInputActive = true; }
        else if (g_pointMouseX > SCREEN_WIDTH - margin) { fAccumTox += step; isInputActive = true; }

        if (g_pointMouseY < margin) { fAccumToy -= step; isInputActive = true; }
        else if (g_pointMouseY > SCREEN_HEIGHT - margin) { fAccumToy += step; isInputActive = true; }
    }

    // 更新全域狀態
    g_GameInfo.g_bIsManualScrolling = isInputActive;

    // 如果發現滑鼠/搖桿沒有在邊緣推圖，就自動同步 static 變數
    if (!isInputActive) {
        // 讓 static 累加器同步當前地圖位置，防止彈跳
        fAccumTox = (float)g_Map.tox;
        fAccumToy = (float)g_Map.toy;
    }
    // 【關鍵重點：硬邊界】
    // 這裡用 Clamp 確保數值絕對不超標，這樣就不會有「超出去又拉回來」的抖動
    if (isInputActive)
    {
        if (fAccumTox < min_limit_x) fAccumTox = min_limit_x;
        if (fAccumTox > max_limit_x) fAccumTox = max_limit_x;
        if (fAccumToy < min_limit_y) fAccumToy = min_limit_y;
        if (fAccumToy > max_limit_y) fAccumToy = max_limit_y;
    }

    // 5. 緩慢回正 (只有放手時才執行)
    if (!isInputActive)
    {
        float return_speed = 0.2f;
        if (fAccumTox < min_limit_x) fAccumTox += return_speed;
        if (fAccumTox > max_limit_x) fAccumTox -= return_speed;
        if (fAccumToy < min_limit_y) fAccumToy += return_speed;
        if (fAccumToy > max_limit_y) fAccumToy -= return_speed;
    }

    // 6. 物理極限
    float map_physical_max_x = (float)g_Map.file.wWidth - (float)GAME_SCREEN_XSIZE / 32.0f;
    float map_physical_max_y = (float)g_Map.file.wHeight - (float)GAME_SCREEN_YSIZE / 32.0f;

    if (fAccumTox < 0) fAccumTox = 0; if (fAccumToy < 0) fAccumToy = 0;
    if (fAccumTox > map_physical_max_x) fAccumTox = map_physical_max_x;
    if (fAccumToy > map_physical_max_y) fAccumToy = map_physical_max_y;

    // 7. 同步
    g_Map.tox = (int)fAccumTox;
    g_Map.toy = (int)fAccumToy;
}
void UpdateCameraSmoothly() {
    static float fMapX = (float)(g_Map.x * TILE_SIZE - g_Map.oX);
    static float fMapY = (float)(g_Map.y * TILE_SIZE - g_Map.oY);

    float target_px = (float)(g_Map.tox * TILE_SIZE);
    float target_py = (float)(g_Map.toy * TILE_SIZE);

    // 1. 先算出「目前鏡頭」跟「目標點」的直線距離 (像素)
    float diffX = target_px - fMapX;
    float diffY = target_py - fMapY;
    //float distance = sqrtf(diffX * diffX + diffY * diffY);

    float currentLerp;

    if (g_GameInfo.g_bIsManualScrolling) {
        // 【動態阻尼算法】
        // 基礎值 0.1 (保證有基本的反應速度)
        // 加成值 (distance * 0.0005)：距離越遠，Lerp 越大
        // 先轉成 float
        float speed_val = static_cast<float>(system_info.scroll_speed);

        // 映射公式: 0~100 -> 0.01~0.1
        currentLerp = 0.01f + (speed_val * 0.0009f);

        // 【安全防護】: 防止玩家改設定檔把速度改成超過 100 或小於 0
        if (currentLerp < 0.01f) currentLerp = 0.01f;
        if (currentLerp > 0.1f)  currentLerp = 0.1f;

    }
    else {
        // 自動跟隨模式 (保持柔和)
        //currentLerp = 0.05f;
        currentLerp = static_cast<float>(system_info.scroll_speed / 100.0f);
    }

    // 應用計算
    fMapX += diffX * currentLerp;
    fMapY += diffY * currentLerp;

    // 1. 強制整數座標
    Mapx = (int)fMapX;
    Mapy = (int)fMapY;

    // 2. 拆解變數
    g_Map.x = Mapx / TILE_SIZE;
    g_Map.y = Mapy / TILE_SIZE;
    g_Map.oX = -(Mapx % TILE_SIZE);
    g_Map.oY = -(Mapy % TILE_SIZE);
    Mox = Mapx + g_pointMouseX;
    Moy = Mapy + g_pointMouseY;
}

void UpdateCharacterVisual_xy() {
    // 4. 視覺更新
    LPCHARACTER ch_iterator = Hero;
    while (ch_iterator != NULL) {
        UpdateCharacterVisuals(ch_iterator);
        ch_iterator->hat_is_hidden = false;

        if (MapNumber == 81) {//GM_EVENT MAP
            ch_iterator->namedisplaycount = 0;
            ch_iterator->smiledelay = 0;
            ch_iterator->ChatDelay = 0;
        }

        ch_iterator = ch_iterator->lpNext;
    }
}
void RenderGame() {

    // ★★★ [插入點 3-A] 開始新的一幀 (必須在任何繪圖之前) ★★★
    // 這會重置 ImGui 的狀態，計算 delta time 等等
    CimGui::Instance().NewFrame();
    g_SDLInfo.setBackbuffersurface(g_DestBackBuf, dDxSize, wDxSize);
    g_SDLInfo.SDL_EraseScreen();

    EWndMgr.SDLCommonEditWndProc(g_SDLInfo.keyboardEventThisframe);
    DisplaySpriteUnderTile();
    MapDisplay(0);
    HandleRunning(&connections);
    DrawSkilltile();

    DisplaySpriteBottom();
    PutDestPoint();
    DisplayShadowFirst();
    MapDisplay(2);
    HandleRunning(&connections);
    DisplaySprite();

    MapDisplay(1);

    g_SDLInfo.launchfirework(1);

    DisplaySpriteCeiling();
    HandleRunning(&connections);
    DisplaySkillCursor();

    DisplaySpriteTrans();

    MapDisplay_Attr();
    HandleRunning(&connections);
    DisplayFightStone();

    _DisplayMapObjectAfter();

    ViewRoofData();

    SnowAnimationOutput();
    HandleRunning(&connections);
    RainAnimationOutput();

    g_SDLInfo.FireFlySystemRender();
    DisplaySpriteNo();

    HandleRunning(&connections);

    DrawParticleEffect({ Mapx, Mapy, g_curr_time });  //改為無狀態 (stateless)
    MapBright();

    DisplayChatAndNumber();

    LocalWarInfoDisPlay();		// LTS LOCALWAR
    ShowMonsterWarInfo();		// BBD 040311

    HandleRunning(&connections);

    UpdateClockLogic(); //              // finito 06/07/07 altered the function to draw the current server time.

    ShowBuffTimer();

    DrawTextOnScreen();

    MenuDisplay();     // ޴             浹üũMenuChecking();     // ޴             浹üũMenuDisplay();      // ޴    Ȱ     θ   Ǵ        ϴ   Լ
    ChatMgr.DisplayContextMenu();
    CharContxtMenu.DisplayContextMenu();
    HandleRunning(&connections);
    gr.GuideGhost_OutPut();

    CurrentMsgMgr.ViewCurrentStatusMessage();

    ViewTips();

    ViewLoadMask();

    DisplayDebug();

    g_SDLInfo.SDL_DrawInputbox();
    CursorDisplayBack();



    g_SDLInfo.SDL_FlipScreen();
}
void UpdateGame() {
    g_OldOrderInfoCount = g_OrderInfo.count;
    g_OrderInfo.count = 0;
    LightCount = 0;
    CrackMgr.AutoCheckAccel();
    SendHexaEditing();// convert to send ping 2025
    setWindowTitle();
    if (g_GameInfo.displayOptionDelay > 0)
    {
        if (g_GameInfo.displayOption == DISPLAY_EARTHQUAKE)
        {
            g_Map.oX = (rand() % 10) - 5;
            g_Map.oY = (rand() % 10) - 5;
        }
        g_GameInfo.displayOptionDelay--;
        if (g_GameInfo.displayOptionDelay <= 0) 		g_GameInfo.displayOption = 0;
    }

    ///////////////// 0604 lkh  ߰ (        콺 ť                       ִ     : ó         ) //////////////
    if (g_queue.lb && g_queue.ch != Hero)	//       ư        ť
    {
        if (g_queue.ch == g_GameInfo.lpcharacter && g_queue.ch != NULL && g_GameInfo.lpcharacter != NULL)
        {
            if (!(Hero->animationType == ACTION_ATTACKED || Hero->animationType == ACTION_DIE ||
                  Hero->animationType == ACTION_NONE || Hero->animationType == ACTION_BASIC_WALK) && !IsDead(g_queue.ch))
            {
                int minrange, maxrange;
                minrange = AttackRangeTable[Hero->sprno][Hero->accessory[2]][0];
                maxrange = AttackRangeTable[Hero->sprno][Hero->accessory[2]][1];

                int minormax = IsAttackRange(&minrange, &maxrange, Hero, g_queue.ch->x, g_queue.ch->y);
                if (minormax == ATTACKRANGE_IN_AREA_)
                {
                    g_mgrBattle.SendPcAttack(Hero, g_queue.ch);
                    ZeroMemory(&g_queue, sizeof(t_queue));
                    goto	JUMP_SKIP;
                }
            }
        }
    }
    mouseClick();
    KeyboardProc();
    MouseProc();
    JUMP_SKIP:
    CharacterProcCheck();
    //::InsertMagic(Hero, Hero, 587, 9, 0, 0, Hero->x, Hero->y);
    //::InsertMagic(Hero, Hero, 590, 9, 0, 0, Hero->x, Hero->y);
    CircleMagic();
    EffectProc();

    //CrackMgr.count_magiclist_for_cheat_detection();//檢查魔法/技能特效數量檢測是否外掛

    DrawItemList();
    OutputSkillGauge();

    CheckTime_SkillMsg();

    CalcOrder();
    g_nBaseOrderCount = g_OrderInfo.count;
// 所以「上一幀魔法數」歸零
    g_nMagicAddedLastFrame = 0;

    if (!tool_ViewMenu)
        MenuChecking();     // ޴             浹üũMenuChecking();

    g_nLButtonState = 0;	g_nRButtonState = 0;
    g_nLDButtonState = 0;	g_nRDButtonState = 0;

    RainAnimationSetting();
    SnowAnimationSetting();
    CurrentMsgMgr.CheckCurrentStatusMessage();

    ViewCheckRoutine(70);

    OutputSkillCursor();
    CheckEventAttrTile();
    // SoundUp
    BackEffectSound2(MapNumber);
    PlayRainSound();
    PlayThunder(); //Eleval 12/08/09 - To play thunder sound if thunderstorm

    ViewCheckRoutine(80);

    g_pNumberAniMgr.Update();

    gr.GuideGhost_PreSet();

    CursorStatus();
    ViewCheckRoutine(100);
    CheckTrap();
#ifdef _DEBUG
    if (g_SDLInfo.bOutputSmallMap) {
		OutputSmallMap_Reloo();
		g_SDLInfo.bOutputSmallMap = false;
	}
#endif
    WeatherFXSound();
    CurMouseCursorFrameCounter();
    StartSound(0);
    CharacterProcFrameCount();
    ObjectSoundPlay();

    LPCHARACTER ch_iterator = Hero;
    while (ch_iterator != NULL) {
        ch_iterator->previousAction = ch_iterator->nCurrentAction;
        SendReqHpAllCharacter(ch_iterator);
        ch_iterator = ch_iterator->lpNext;
    }
    /*if (CrackMgr.IsHardwareBreakpointSet()) {
         //std::quick_exit(EA_CRACK_THREAD_FIND_CRACK);//todo
    }*/
    // 4. 重置按鍵狀態 (這部分可以保留)
    LButtonDownIng = 0;

}
BOOL GameProc() {
    DWORD time = SDL_GetTicks();
    g_ClientTime = time + g_ServerClientTimeGap;

    float dt = g_FrameMgr.GetDeltaTime();

    if (NowLoading) return TRUE;

    g_FrameMgr.UpdateTimers();

    // 1. 處理網路協定
    if (ProtocolProc(&connections, false) < 0) return FALSE;

    if (ChangeMapFlag) {
        if (RecvChangeMap(&ChangeMapPacket) == FALSE) return FALSE;
    }

    UpdateTextCache();
    g_MouseInMenuThisFrame = MouseOnMenu();
    // 2. 邏輯迴圈 (固定 17 UPS)
    while (g_FrameMgr.ShouldUpdateLogic()) {
        g_SDLInfo.g_linesToDrawThisFrame.clear();
        FlushPendingDeletesItemList();
        ProcessDeferredDeletion();
        CleanUpSkillCursors();
        UpdateGame(); // 這裡不再計算 Mapx
    }

    if (ProtocolWritingProc(&connections) <= 0) return FALSE;

    HandleCameraScrollingPerFrame();
    UpdateSnowSmoothly(dt);
    UpdateRainSmoothly(dt);
    // 3. 視覺與攝像機更新 (每幀執行 - 144 FPS)
    UpdateCharacterVisual_xy(); // 先更新人物視覺位置
    UpdateCameraSmoothly();     // 【關鍵】更新攝像機平滑位置
    ReputMagicEffect();
    // 4. 渲染
    RenderGame();

    return TRUE;
}

// 階段一：視覺與功能性測試
void RunFunctionalTest() {
  SDL_Log("Running Functional Test...");

  // --- 測試 1: 創建霧效 (Fog) ---
  int fogSpriteIndex = 1;

  // *** 關鍵修正：在創建粒子前，預先載入它會用到的 Sprite 資源 ***
  SDL_Log("  - Preloading sprite data for index %d...", fogSpriteIndex);
  g_EffectSpriteManager.CheckAndLoadSpriteData(fogSpriteIndex);


  // --- 測試 1: 創建一個霧效 (Fog) ---
  FogArgs fogArgs = {};
  fogArgs.SpriteIndex = fogSpriteIndex;

  LONG candidate_world_x = static_cast<LONG>(Mapx + (rand() % 1280));
  LONG candidate_world_y = static_cast<LONG>(Mapy + (rand() % 720));
  // Mapx 和 Mapy 是您遊戲中的全域攝影機座標。
  fogArgs.Location = {static_cast<int>(candidate_world_x), static_cast<int>(candidate_world_y)};
  fogArgs.StartTime = g_curr_time;
  fogArgs.DelayTime = g_curr_time + 5;

  if (auto fogParticle = g_ParticleFactory.createFog(fogArgs)) {
    g_ParticleManager.Insert(std::move(fogParticle));
    SDL_Log("  - Fog particle created successfully.");
  } else {
    SDL_Log("  - FAILED to create fog particle.");
  }

  // --- 測試 2: 創建龍之吐息 (Breath) ---
  int breathSpriteIndex = 3;

  // *** 關鍵修正：預載入吐息的 Sprite ***
  SDL_Log("  - Preloading sprite data for index %d...", breathSpriteIndex);
  g_EffectSpriteManager.CheckAndLoadSpriteData(breathSpriteIndex);

  // --- 測試 2: 創建一個龍之吐息 (Breath) ---
  BreathArgs breathArgs;
  breathArgs.SpriteIndex = breathSpriteIndex;
  breathArgs.StartLocation = {Mapx + rand() % 1280, Mapy + rand()%720};
  breathArgs.Direction = rand()%16;
  breathArgs.Power = 30;
  breathArgs.StartTime = g_curr_time;

  if (auto breathParticle = g_ParticleFactory.createBreath(breathArgs)) {
    g_ParticleManager.Insert(std::move(breathParticle));
    SDL_Log("  - Breath particle created successfully.");
  } else {
    SDL_Log("  - FAILED to create breath particle.");
  }
}

// 階段二：效能壓力測試
void RunStressTest(int particleCount) {
  SDL_Log("Running Stress Test with %d particles...", particleCount);

  // *** 關鍵修正：壓力測試同樣需要預先載入所有可能會用到的資源 ***
  // 根據迴圈中的邏輯 `3 + (rand() % 3)`，我們知道會用到 3, 4, 5 這三個索引。
  SDL_Log("  - Preloading sprites for stress test (indices 3, 4, 5)...");

  // 假設您有 GetCurrentTimeInSeconds() 函式來獲取時間
  float startTime = static_cast<float>(SDL_GetTicks()) / 1000.0f;

  for (int i = 0; i < particleCount; ++i) {
    BreathArgs args = {};
    args.SpriteIndex = rand() % 10;
    args.StartLocation = {Mapx + rand() % 1280, Mapy + rand() % 720};
    args.Direction = rand()%16;
    args.Power = 51 + (rand() % 68);
    args.StartTime = g_curr_time;

    if (auto p = g_ParticleFactory.createBreath(args)) {
      g_ParticleManager.Insert(std::move(p));
    }
  }

  float endTime = static_cast<float>(SDL_GetTicks()) / 1000.0f;
  SDL_Log("  - Time to create %d particles: %f ms", particleCount,
          (endTime - startTime) * 1000.0f);
}

// 階段三：資源與邊界測試
void RunPoolExhaustionTest() {
  // 我們在 ParticleFactory 中設定霧效池的大小為 200
  int pool_size = 25;
  int test_count = rand()%pool_size + 25;  // 故意多創建 50 個

  SDL_Log("Running Pool Exhaustion Test (Size: %d, Attempting: %d)...",
          pool_size, test_count);

  int success_count = 0;
  for (int i = 0; i < test_count; ++i) {
    FogArgs args = {};
    args.SpriteIndex = rand()%10;
    args.Location = {Mapx+ rand() % 1280, Mapy + rand()%720};
    args.StartTime = g_curr_time;
    args.DelayTime = g_curr_time + 5;

    if (auto p = g_ParticleFactory.createFog(args)) {
      g_ParticleManager.Insert(std::move(p));
      success_count++;
    }
  }

  SDL_Log("  - Successfully created %d particles.", success_count);
}

// 總測試觸發函式
void RunAllParticleTests() {
  for (int i = 0; i < 10; i++) {
    g_EffectSpriteManager.CheckAndLoadSpriteData(i);
  }
  //RunFunctionalTest();
  RunStressTest(rand()%25+25);
  RunPoolExhaustionTest();
}


static int	effect_Test = 1;
void KeyboardProc(void)
{
    // 判斷是否具備移動選單的條件
    bool bCanMoveMenu = (!IsChatBoxLock() && g_MouseInMenu && TileMap[Hero->x >> 5][Hero->y >> 5].attr_no_battle);

    // --- 1. 處理選單位移 (邏輯層 17Hz 就夠了，不需要太快) ---
    if (bCanMoveMenu)
    {
        if (isKeyPressed(SDL_SCANCODE_LEFT))  SMenu[g_MouseInMenu].x -= 2;
        if (isKeyPressed(SDL_SCANCODE_RIGHT)) SMenu[g_MouseInMenu].x += 2;
        if (isKeyPressed(SDL_SCANCODE_UP))    SMenu[g_MouseInMenu].y -= 2;
        if (isKeyPressed(SDL_SCANCODE_DOWN))  SMenu[g_MouseInMenu].y += 2;

        // 只有在真的移動選單時才印 Log
        if (isKeyPressed(SDL_SCANCODE_LEFT) || isKeyPressed(SDL_SCANCODE_RIGHT) ||
            isKeyPressed(SDL_SCANCODE_UP) || isKeyPressed(SDL_SCANCODE_DOWN)) {
            //SDL_Log("MenuID(%d) (%d,%d)", g_MouseInMenu, SMenu[g_MouseInMenu].x, SMenu[g_MouseInMenu].y);
        }
    }

    // --- 2. 處理功能鍵 (一次性動作或狀態切換) ---
    if (!IsChatBoxActive())
    {
        if (isKeyPressed(SDL_SCANCODE_SPACE)) {
            SetHeroPosition(1); // 會更新 tox, toy，144Hz 的平滑公式會立刻接手
        }

        if (isKeyPressed(SDL_SCANCODE_RETURN) && !EWndMgr.IsMultiLineBoxShow()) {
            SetChatMode(CM_MESSAGE);
        }
    }

    // 3. 自動跟隨英雄 (g_center_hero)
    if (g_center_hero) {
        SetHeroPosition(1);
    }

    // 4. 處理組合鍵狀態
    //DontMoveAttackFlag = (isKeyPressed(SDL_SCANCODE_LSHIFT) || isKeyPressed(SDL_SCANCODE_RSHIFT));
    //CheckPC = (isKeyPressed(SDL_SCANCODE_LCTRL) || isKeyPressed(SDL_SCANCODE_RCTRL));
    //Hero->display_item = (isKeyPressed(SDL_SCANCODE_LALT) || isKeyPressed(SDL_SCANCODE_RALT));
}
// 輔助函式：鼠標圖示切換
void UpdateCursorIcons(int margin) {
    if (g_pointMouseX < margin) {
        PushMouseCursor();
        if (g_pointMouseY < margin) CursorNo(11);
        else if (g_pointMouseY > SCREEN_HEIGHT - margin) CursorNo(9);
        else CursorNo(10);
    }
    else if (g_pointMouseX > SCREEN_WIDTH - margin) {
        PushMouseCursor();
        if (g_pointMouseY < margin) CursorNo(13);
        else if (g_pointMouseY > SCREEN_HEIGHT - margin) CursorNo(15);
        else CursorNo(14);
    }
    else if (g_pointMouseY < margin) {
        PushMouseCursor(); CursorNo(12);
    }
    else if (g_pointMouseY > SCREEN_HEIGHT - margin) {
        PushMouseCursor(); CursorNo(8);
    }
    else {
        PopMouseCursor();
    }
}

void MouseProc(void) {
    if (!Hero) return;

    // 1. 只更新視野常量
    SetView(Hero, Hero->sight);

    // 2. 移除原本的 MoveScreen 呼叫 (已經移到 HandleCameraScrollingPerFrame 了)
    if (g_StartMenuOn == false) {
        const int margin = 3;
        UpdateCursorIcons(margin); // 僅更新鼠標樣式
    }

    // 4. 處理點擊與按鈕狀態機
    if ((g_GameInfo.checkcount >= 16) || g_GameInfo.mouseClick) {
        DoLbuttonStill();
    }

    switch (g_nLButtonState) {
        case STATE_BUTTON_PRESSED:  DoLButtonDown();  break;
        case STATE_BUTTON_RELEASED: DoLButtonUp();    break;
    }

    switch (g_nRButtonState) {
        case STATE_BUTTON_PRESSED:  DoRButtonDown();  break;
        case STATE_BUTTON_RELEASED: DoRButtonUp();    break;
    }

    if (g_nLDButtonState == STATE_BUTTON_DOUBLECLICK) DoLDButtonDown();
    if (g_nRDButtonState == STATE_BUTTON_DOUBLECLICK) DoRDButtonDown();
}


int ConnectProxyServer(t_connection* c)
{
	double		StartTime, Duration;

	if (ConnectServer(c, g_GameInfo.proxy_ip, g_GameInfo.proxy_port) <= 0)
	{
		return -21;
	}

	StartTime = ::SDL_GetTicks();
	ListenCommand = 0;
	while (1)
	{
		Duration = SDL_GetTicks() - StartTime;
		if (Duration > WAIT_TIME_RESPONSE_DURATION) // 030930 kyo
		{
			ClearServer(c);
			return(-1);
		}
		if (HandleRunning(c) <= 0)
		{
			Release(c);
			return(-2);
		}
		if (ListenCommand == CMD_AGENT_SERVER_IP)
		{
			ClearServer(c);
			return 1;
		}
		else if (ListenCommand == CMD_NOT_EXIST_GAMESERVER)
		{
			ClearServer(c);
			return -8;
		}
		// 010301 KHS
		else if (ListenCommand == CMD_SO_MUCH_LOGINUSER_WAIT_FOR_A_MIN)
		{
			ClearServer(c);
			return (-11);
		}
	}
}

// LogIn Server�� ����...
int CheckAgentServer(t_connection* c, std::string id, std::string password) {
  int ret = 0;
  Release(c);
  if (ConnectServer(c, g_GameInfo.agent_ip, g_GameInfo.agent_port) <= 0) {
    // 連線失敗的邏輯
    ret = -1;
  } else {
    // 連線成功的邏輯
    ret = 1;
  }
  // 成功與失敗路徑共同的清理程式碼
  IIDD = id;
  PPWW = password;
  sId = "";
  sPassword = "";
  EWndMgr.ClearAllTxt();
  return ret;
}

////////////////////////////////////////////////////////////////////////////////
int ProtocolProc(t_connection* c, bool flag)
{
	if (HandleRunning(c, flag) <= 0)
	{
		Release(c);
	}

	return	0;
}


int ProtocolWritingProc(t_connection* c)
{
	/*int ret;
	struct timeval	tv;
	fd_set			wfds;

	if (!c->socket)	return(1);
	if (c->outbuf == NULL) return 1;

	FD_ZERO(&wfds);
	FD_SET(c->socket, &wfds);


	tv.tv_sec = 0;
	tv.tv_usec = 0;
	ret = select(c->socket, NULL, &wfds, NULL, &tv);

	if (ret)
		ret = HandleWriting(c);

	if (ret < 0)
	{
		Release(c);
		return 0;
	}

	return 1;*/

	// 1. 基本檢查
    if (c == nullptr || c->socket == nullptr) {
        return 1;
    }

    // 檢查是否有待發送的數據包 (隊列或發送緩衝區)
    if (c->outbuf.empty()) {
        return 1; // 沒有待發送數據，直接返回
    }
    
    // 這裡我們不使用 SDLNet_CheckSockets，因為它會檢查讀取活動。
    // 如果你已經在 HandleRunning 中處理了讀取，那這裡就不需要重複檢查。
    // 根據你的 HandleWriting 邏輯，它應該會嘗試將所有數據發送出去，直到發送緩衝區滿了為止。
    
    // 直接調用 HandleWriting 函數來處理寫入
    int ret = HandleWriting(c);
    
    // 處理 HandleWriting 的返回值
    if (ret < 0) {
        SDL_Log("ProtocolWritingProc: HandleWriting reported error %d. Releasing connection.", ret);
        ClearServer(c); // 調用清理函數
        return 0; // 返回 0 表示發送失敗且連接已斷開
    }
    
    return 1; // 返回 1 表示函數執行成功
}



void CharacterProcCheck(void)
{
	LPCHARACTER		tempch, ch = Hero;
	int sprc;

	//	if( ch != NULL)						
	//		if( IsDead( ch ) && ch->viewtype != VIEWTYPE_GHOST_ )				
	//			if( ch->nCurrentAction != ACTION_DIE )	{ ch->nCurrentAction = ACTION_DIE; ch->nCurrentFrame.PutEncrypted(0); }
	if (ch->position.x <= 0 || ch->position.x >= VILLAGE_SIZE) return;

	while (ch != NULL)
	{
		switch (ch->animationType)
		{
		case ANIMATION_TYPE_TEST:
			AnimationTypeTestCheck(ch);
			break;

		case ANIMATION_TYPE_MAN:
		case ANIMATION_TYPE_WOMAN:
			AnimationTypeWomanCheck(ch);
			break;

		case ANIMATION_TYPE_MON1:
			if (ch->dieani == 1)
			{
				tempch = ch->lpNext;
				DeleteCharacter(&g_CharacterList, ch);
				ch = tempch;
				continue;
			}

			AnimationType_Mon1_Check(ch);
			Check_NPC_Pattern(ch);
			break;
		}
		// ���ΰ��� �������� ���ų� ���ͼ� g_Inside���� �ٲ�� �ִ�. 
		if (ch == Hero)
		{
			DWORD	mx = ch->position.x;
			DWORD	my = ch->position.y;
			g_Inside = (TileMap[mx][my].attr_inside == 1);
		}

		sprc = ch->AnimationTable[ch->nCurrentAction].cFrame[ch->nCurrentFrame.GetDecrypted()];
		ch->height = ch->AnimationTable[ch->nCurrentAction].height[ch->nCurrentFrame.GetDecrypted()];
		ch->sp = &CharSpr[ch->sprno].sp[ch->direction][sprc];

		if (ch->flagattacked > 0)	//  �������� �״� �÷��װ� �Ѿ���� �׾�� �Ѵ�. 
		{
			ch->flagattacked--;
			if (ch->flagattacked < 1)
			{
				if (Hero == ch)
				{
					SCharacterData.nCharacterHP = 0;
					Hero->hp = SCharacterData.nCharacterHP; //Eleval 18/08/09 - HP BUG
				}
				ch->hp = 0;
			}
		}

		//  ���콺�� ���� Check�Ǿ������  �̸��� �����ֿ��� �ϴµ�..   MouseCheckCharacterName�� Setting�ǹǷ� �̸� �̿��Ͽ� �̸��� ����Ѵ�.  �ణ�� Delay�� �־� ������� �ʰ� �Ѵ�. 
		if (ch->viewtype != VIEWTYPE_TRANSPARENCY_) // �����ΰ� ������ ���� �̸����� �������� �ʴ´�.
			if (ch == MouseCheckCharacterName)
			{
				if (ch->namedisplaycount == 0)
				{
					if (ch != Hero)
					{
						int	auto_Wave = 0;
						int DistanceX = (ch->x - Hero->x)/* / TILE_SIZE*/;	// LTS SOUND
						int DistanceY = (ch->y - Hero->y)/* / TILE_SIZE*/;	// LTS SOUND

						//////////////////////// 0810 lkh ���콺 Ŀ���� �÷� ������ ���� ȿ���� ������ ////////////////////////
						switch (ch->animationType)
						{
						case ANIMATION_TYPE_MAN:
						case ANIMATION_TYPE_WOMAN:
							if (ch->sex)
								auto_Wave = 70 + rand() % 3;
							else	auto_Wave = 75 + rand() % 3;

							PlayListAutoSounds(auto_Wave, DistanceX, DistanceY, 0);	//3D sound�� ǥ�� // LTS SOUND
							//PlayListAutoSounds(auto_Wave, DistanceX,DistanceY,0);	// LTS SOUND
							break;
						case ANIMATION_TYPE_MON1:
							//	auto_Wave = ReturnWaveNatural( ch );
							//	PlayListAutoSounds(auto_Wave, 0,6,0);
							break;
						default:
							break;
						}
					}
				}
				if (ch->namedisplaycount < 50) ch->namedisplaycount = 50;
			}
			else
			{
				if (ch->namedisplaycount > 0) ch->namedisplaycount--;
			}

		if (ch->animationType == ANIMATION_TYPE_MON1)
		{
			ch->dirdelay = !ch->dirdelay;
			if (ch->dirdelay == 0)
			{
				int todir = (int)ch->todir;
				int dir = (int)ch->direction;
				if (todir - dir > 0)
				{
					if (abs(todir - dir) <= 3) { dir++; dir %= 8; }
					else { dir += 7; dir %= 8; }
				}
				else if (todir - dir < 0)
				{
					if (abs(dir - todir) <= 4) { dir += 7; dir %= 8; }
					else { dir++; dir %= 8; }
				}

				ch->direction = (DIRECTION)dir;
			}
		}

		ch = ch->lpNext;
	}

	if (LButtonDownIng == 1)
	{
		if (Hero != NULL)	// ���Ӿȿ� ���Դٸ�..				
		{
			if (Hero->peacests == 0 && SMenu[MN_PARTY].bActive == true)// ��Ƽ�� �����ϱ� ���� 
			{
				if (g_GameInfo.lpcharacter == g_GameInfo.lpoldcharacter)
				{
					g_GameInfo.PopupFaceCount++;
					if (g_GameInfo.PopupFaceCount == 15)
					{
						switch (g_GameInfo.nSelectedSpriteType)
						{
						case SPRITETYPE_ON_THE_CEILING_CHAR:
						case SPRITETYPE_CHARACTER:
						case SPRITETYPE_NPC:
						case SPRITETYPE_MONSTER:		g_MouseItemType = PARTY;
							SetMouseCh(g_GameInfo.lpcharacter);	// 0929 YGI
							break;
						default:
							break;
						}
					}
				}
				else g_GameInfo.PopupFaceCount = 0;
			}

			g_GameInfo.checkcount++;
		}
	}
	else
	{
		g_GameInfo.checkcount = 0;
		g_GameInfo.PopupFaceCount = 0;
	}
	g_GameInfo.lpoldcharacter = g_GameInfo.lpcharacter;
}


void  CharacterProcFrameCount(void)
{
	LPCHARACTER		ch = Hero;

	while (ch != NULL)
	{
		switch (ch->animationType)
		{
		case ANIMATION_TYPE_TEST:
			AnimationTypeTestFrameCount(ch);
			break;

		case ANIMATION_TYPE_MAN:
		case ANIMATION_TYPE_WOMAN:
			AnimationTypeWomanFrameCount(ch);
			break;

		case ANIMATION_TYPE_MON1:
			AnimationType_Mon1_FrameCount(ch);
			break;
		}

		ch = ch->lpNext;
	}

}





void AttackProc(LPCHARACTER ch, int minr, int maxr)
{
	LPCHARACTER dch;
	LPITEMGROUND i;
	int ani;
	//	ĳ���͸� ���� �������� ����...


	switch (ch->animationType)
	{
	case ANIMATION_TYPE_TEST:
		break;

	case ANIMATION_TYPE_MAN:
	case ANIMATION_TYPE_WOMAN:		if (ch->changeActionFlag == 0) // �ٸ� ���ݵ����� �ϰ� ���� �ʴٸ�,
	{
		switch (ch->nAttackedType)
		{
		case SPRITETYPE_ITEM:
		{
			i = (LPITEMGROUND)ch->lpAttacked;

			ChangeDirection(&ch->direction, ch->x, ch->y, i->x, i->y);
			ch->todir = ch->direction;
			ani = AttackAction(ch);
			if (ani == ch->nCurrentAction)	return;
			else  ch->nCurrentAction = ani;
			ch->nCurrentFrame.PutEncrypted(0);
		}
		break;


		case SPRITETYPE_CHARACTER:
		case SPRITETYPE_NPC:
		case SPRITETYPE_MONSTER:
		{
			ch->attackMinRange = minr;
			ch->attackMaxRange = maxr;


			dch = (LPCHARACTER)(ch->lpAttacked);
			ani = AttackAction(ch);
			if (ani == ch->nCurrentAction)	return;
			else  ch->nCurrentAction = ani;
			ChangeDirection(&ch->direction, ch->x, ch->y, dch->x, dch->y);
			ch->todir = ch->direction;
			ch->nCurrentFrame.PutEncrypted(0);
		}
		break;
		}
	}
							 break;

	case ANIMATION_TYPE_MON1:		if (ch->changeActionFlag == 0) // �ٸ� ���ݵ����� �ϰ� ���� �ʴٸ�,
	{
		ch->attackMinRange = minr;
		ch->attackMaxRange = maxr;
		dch = (LPCHARACTER)(ch->lpAttacked);
		ChangeDirection(&ch->direction, ch->x, ch->y, dch->x, dch->y);
		ch->todir = ch->direction;
		ani = AttackAction(ch);
		if (ani == ch->nCurrentAction)	return;
		else  ch->nCurrentAction = ani;
		ch->nCurrentFrame.PutEncrypted(0);
	}
							break;
	}


}








///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//////			
//////			
//////			Mouse ó��....
//////			
//////			

/* // �̺κ��� Server���� ����� �ް� �̵��Ѵ�.
if ( Hero->nCurrentAction != ACTION_RUN )
{
Hero->nCurrentAction = ACTION_RUN;
Hero->nCurrentFrame.PutEncrypted(0);
}
*/
// type : 0- walk..
// type : 1- run..



void DoHeroMove(int movetype, int x, int y)
{
	int mx = x / TILE_SIZE;
	int my = y / TILE_SIZE;
	CHARACTER t{};
	DWORD temptime=0;

	static int oldmovetype;

	if (DontMoveAttackFlag == TRUE || Hero->DontMoveAttackFlag)
		return;
	if (g_MouseInMenu) return;

	if (Hero->nCurrentAction == 17)		// LTS 011214 LTS			// ���� ���¿��� ������ �ٲپ� �ش�.
	{
		/*AddCurrentStatusMessage( 255,255,0,"�ɱ� ���¿����� �����ϼ� �����ϴ�.");
		switch(Hero->direction)
		{
		case DIRECTION_UP			: Hero->direction=DIRECTION_RIGHTUP;	break;
		case DIRECTION_RIGHTUP		: Hero->direction=DIRECTION_RIGHT;		break;
		case DIRECTION_RIGHT		: Hero->direction=DIRECTION_RIGHTDOWN;	break;
		case DIRECTION_RIGHTDOWN	: Hero->direction=DIRECTION_DOWN;		break;
		case DIRECTION_DOWN			: Hero->direction=DIRECTION_LEFTDOWN;	break;
		case DIRECTION_LEFTDOWN		: Hero->direction=DIRECTION_LEFT;		break;
		case DIRECTION_LEFT			: Hero->direction=DIRECTION_LEFTUP;		break;
		case DIRECTION_LEFTUP		: Hero->direction=DIRECTION_UP;			break;
		}
		t_packet p;
		p.h.header.type=CMD_SITDOWN_DIRECTION_CHANGED;
		p.u.LocalWar.SDChanged.Direction=Hero->direction;
		p.h.header.size=sizeof(t_SDChanged);
		QueuePacket(&p,1);*/
		return;	// LTS SITDOWN
	}

	if (mx <= 0 || my <= 0 ||
		mx >= g_Map.file.wWidth || my >= g_Map.file.wHeight)
	{
		int eventno = ReturnEventNo(mx, my);
		if (eventno == -1)
			return;
	}

	PathBuildSuccess = 0;

	if (SkillStatus == SKILL_OFF)
	{
	}
	else
	{
		return;
	}

	if (IsDead(Hero) && Hero->viewtype != VIEWTYPE_GHOST_)
	{
		if (!Hero->sex)		//����
			PlayListAutoSounds(80, 0, 0, 0); // �̵�����.
		else
			PlayListAutoSounds(81, 0, 0, 0); // �̵�����.
		AddCurrentStatusMessage(255, 100, 100, lan->OutputMessage(5, 20));//010216 lsw
		return;
	}
	else
		if (SCharacterData.nCharacterSP <= 0 && !IsDead(Hero))
		{
			//	0910 YGI
			//		if(!Hero->sex)		//����
			//			PlayListAutoSounds( 80,0,0,0 ); // �̵�����.
			//		else
			//			PlayListAutoSounds( 81,0,0,0 ); // �̵�����.
			AddCurrentStatusMessage(255, 100, 100, lan->OutputMessage(9, 3));//010215 lsw
		}

	if (TileMap[Hero->x / TILE_SIZE][Hero->y / TILE_SIZE].attr_dont) return;		// ��Ʈ�ڽ��� �������δ�. 

	if (!TileMap[mx][my].attr_dont && FreeTile(Hero, mx, my, mx, my))
	{
		//		if( IsTooNearCharacter( x, y, Hero ) )	
		//		{
		//			PlayListAutoSounds( 10,0,0,0 ); // �̵�����.
		//			return; // ���ݹ������� �������� �����Ѵٸ�..
		//		}
		//		�޸��µ��� 500ms���� ���콺�� Ŭ���Ǹ� 

		temptime = SDL_GetTicks();
		if (temptime - Movetime < 300)
		{
			return;
		}
		if (Moveoldx == mx && Moveoldy == my)
		{
			if (mx != Hero->x / TILE_SIZE || my != Hero->y / TILE_SIZE)
				return;
		}

		//		
		//			if( oldmovetype == 1 )
		//			{
		//				movetype = 1;
		//				if( oldx == mx && oldy == my ) return;
		//			}
		//			else
		//			{
		//			}

		Movetime = temptime;
		Moveoldx = mx;
		Moveoldy = my;

		memcpy(&t, Hero, sizeof(CHARACTER));
		t.moveFlag = TRUE;
		t.movetype = movetype;
		t.gox = x;
		t.goy = y;
		t.destx = mx, t.desty = my;
		if (PathBuild(&t))
		{
			SettingDestPoint(x, y);

			PathBuildSuccess = 1;

			if (Hero)
			{
				Hero->moveFlag = TRUE;
				Hero->gox = x;
				Hero->goy = y;
				Hero->movetype = movetype;

				memcpy(Hero, &t, sizeof(CHARACTER));

				if (movetype == 0)
					if (Hero->pathcount > 5) movetype = 1;

				switch (movetype)
				{
				case 0:	if (Hero->nCurrentAction != Hero->basicWalk)
				{
					Hero->nCurrentAction = Hero->basicWalk;
					Hero->nCurrentFrame.PutEncrypted(0);
				}
					  break;
				case 1:	if (Hero->nCurrentAction != ACTION_RUN)
				{
					Hero->nCurrentAction = ACTION_RUN;
					Hero->nCurrentFrame.PutEncrypted(0);
				}
					  break;
				}

				SendMoveData(&t);
			}
			else
			{
				t.moveFlag = FALSE;
				t.gox = x;
				t.goy = y;
				SendMoveData(&t);
			}
		}
		else
		{
			if (Hero->x != x || Hero->y != y) //  �ٸ� �ڸ��� �̵��Ϸ� �Ѵٸ�.
				if (Hero->x / TILE_SIZE == mx && Hero->y / TILE_SIZE == my)
				{
					Hero->gox = x;
					Hero->goy = y;
					Hero->moveFlag = TRUE;
					Hero->movetype = 1;

					if (Hero->nCurrentAction != ACTION_RUN)
					{
						Hero->nCurrentAction = ACTION_RUN;
						Hero->nCurrentFrame.PutEncrypted(0);
					}
				}

			/*if (!Hero->sex)		//����
				PlayListAutoSounds(80, 0, 0, 0); // �̵�����.
			else
				PlayListAutoSounds(81, 0, 0, 0); // �̵�����.*/
			//SDL_Log("1 PlayListAutoSounds [8%d]", Hero->sex);
		}
	}
	else
	{

		if (Hero->x != x || Hero->y != y) //  �ٸ� �ڸ��� �̵��Ϸ� �Ѵٸ�.
			if (Hero->x / TILE_SIZE == mx && Hero->y / TILE_SIZE == my)
			{
				Hero->gox = x;
				Hero->goy = y;
				Hero->moveFlag = TRUE;
				Hero->movetype = 1;

				if (Hero->nCurrentAction != ACTION_RUN)
				{
					Hero->nCurrentAction = ACTION_RUN;
					Hero->nCurrentFrame.PutEncrypted(0);
				}
			}

		/*if (!Hero->sex)		//����
			PlayListAutoSounds(80, 0, 0, 0); // �̵�����.
		else
			PlayListAutoSounds(81, 0, 0, 0); // �̵�����.*/
		//SDL_Log("2 PlayListAutoSounds [8%d]", Hero->sex);
	}

	if (!IsHeroDead()) CheckNpcTalk();	// 0923 YGI �߰� 
}

/*

  GetItemAttr( itemno, 		ITEM_KIND );




		  */

void DoHeroAttack(int x, int y)
{
	int minrange, maxrange, range;
	int minormax;
	int dx, dy;
	int destx, desty;
	LPCHARACTER d{};
	LPITEMGROUND i{};
	CHARACTER t{};

	AutoAttackx = x;
	AutoAttacky = y;

	if (IsDead(Hero) || Hero->nCurrentAction == ACTION_DIE)
	{
		PlayListAutoSounds(82, 0, 0, 0); // �̵�����.
		AddCurrentStatusMessage(255, 100, 100, lan->OutputMessage(9, 4));
		return;
	}

	if (SCharacterData.nCharacterSP == 0)
	{
		PlayListAutoSounds(82, 0, 0, 0); // �̵�����.
		AddCurrentStatusMessage(255, 100, 100, lan->OutputMessage(9, 5));//010215 lsw
		return;
	}

	if (g_GameInfo.lpcharacter == NULL) return;

	minrange = AttackRangeTable[Hero->sprno][Hero->accessory[2]][0];
	maxrange = AttackRangeTable[Hero->sprno][Hero->accessory[2]][1];

	switch (g_GameInfo.nSelectedSpriteType)
	{
	case SPRITETYPE_ITEM:	i = (LPITEMGROUND)(g_GameInfo.lpcharacter);
		destx = i->x + i->offx;
		desty = i->y + i->offy;
		minormax = IsAttackRange(&minrange, &maxrange, Hero, destx, desty);
		if (minormax == ATTACKRANGE_IN_AREA_)
		{
			if (i->type == ITEMTYPE_COLOSSUS_STONE)
			{
				if (IsPossibleAttackColossusStone(i))
				{
					SendAttackColossusItem(i);
				}
				break;
			}
			else
			{
				if (Hero->sprno == 0) //�����̸�.
				{
					switch (Hero->accessory[2])
					{
					case 76:case 77:case 78: case 79: // Ȱ�̳� ����ΰ��� �̹���� Magicó������ Send�ϰԵȴ�. 
						Send_CMD_JUST_ATTACK_ANIMATION(i);
						break;
					default:
						//Send_CMD_ITEM_BOX_BREAK(i);
						break;
					}
				}
				else
				{
					switch (Hero->accessory[2])
					{
					case 78: case 79:case 80:case 81:	// Ȱ�̳� ����ΰ��� �̹���� Magicó������ Send�ϰԵȴ�. 
						Send_CMD_JUST_ATTACK_ANIMATION(i);
						break;
					default:
					{
						//Send_CMD_ITEM_BOX_BREAK(i);
						break;
					}
					}
				}
			}
			return;
		}
		break;

	case SPRITETYPE_ON_THE_CEILING_CHAR:
	case SPRITETYPE_CHARACTER:
	case SPRITETYPE_MONSTER:
	case SPRITETYPE_NPC:
		if (IsDead(g_GameInfo.lpcharacter)) return;
		if (Hero == g_GameInfo.lpcharacter) return;

		if (g_GameInfo.lpcharacter->sprno == 100 ||
			g_GameInfo.lpcharacter->sprno == 101)
		{
			return;
		}


		d = g_GameInfo.lpcharacter;
		destx = d->x;
		desty = d->y;

		////////////////////// 0609 lkh �߰�(��.��ֹ� �ڿ��� �����ϴ� ��Ȳ ��������) ////////////////////////
		minormax = IsAttackRange(&minrange, &maxrange, Hero, destx, desty);
		if (TileLineCheck(destx, desty, Hero->x, Hero->y))		//��ֹ��� ���� ���
		{
			if (minormax == ATTACKRANGE_IN_AREA_)
			{
				CursorNo(53);
				g_mgrBattle.SendPcAttack(Hero, g_GameInfo.lpcharacter);
				return;
			}
		}
		else
		{
			return;
		}
		break;
	default: return;
	}

	switch (minormax)
	{
		//////////////////////// 0614 lkh �߰� /////////////////////
	case ATTACKRANGE_IN_MIN_:	range = minrange + 16;				break;		//0525 lkh ��ġ ����
	case ATTACKRANGE_OUT_MAX_:	range = maxrange - 16;				break;		//			"

	case ATTACKRANGE_IN_AREA_:
	default:						range = (minrange + maxrange) / 2;	break;

	}

	//	if( ReturnAccesablePosition( Hero, destx, desty, Hero->x, Hero->y, range, &dx, &dy ) )

	int ttt = 0;
	if (DontMoveAttackFlag == FALSE && Hero->DontMoveAttackFlag == false)
		if (Hero->nCurrentAction != Hero->basicWalk)
			if (ReturnXYNearPosition(&x, &y, &dx, &dy, range, Hero, destx, desty))
			{
				ttt = 1;
				memcpy(&t, Hero, sizeof(CHARACTER));
				t.moveFlag = TRUE;
				t.movetype = 1;

				//////////////////////// 0625 lkh �߰� ////////////////////////////
				if (GetIndexOfCondition(SCharacterData.condition) == CC_DAZE)		return;//ȥ������


				t.gox = x;
				t.goy = y;

				if (Hero->destx == dx && Hero->desty == dy) return;

				t.destx = dx, t.desty = dy;
				if (PathBuild(&t))
				{
					ttt = 2;
					if (Hero)
					{
						memcpy(Hero, &t, sizeof(CHARACTER));
						Hero->moveFlag = TRUE;
						Hero->gox = x;
						Hero->goy = y;
						Hero->movetype = 1;

						if (Hero->nCurrentAction != ACTION_RUN)
						{
							Hero->nCurrentAction = ACTION_RUN;
							Hero->nCurrentFrame.PutEncrypted(0);
						}

						SendMoveData(Hero);
					}
					else
					{
						t.moveFlag = FALSE;
						t.gox = x;
						t.goy = y;
						SendMoveData(&t);
					}
				}
				else
				{
					if (Hero->x != x || Hero->y != y) //  �ٸ� �ڸ��� �̵��Ϸ� �Ѵٸ�.
					{
						if ((Hero->x - x) * (Hero->x - x) + (Hero->y - y) * (Hero->y - y) < 2048) // Ÿ���ϳ� ���� ���� ũ�� ����.
						{
							//if( Hero->x /TILE_SIZE == dx && Hero->y /TILE_SIZE == dy ) 

							Hero->gox = x;
							Hero->goy = y;
							Hero->moveFlag = TRUE;
							Hero->movetype = 1;

							if (Hero->nCurrentAction != ACTION_RUN)
							{
								Hero->nCurrentAction = ACTION_RUN;
								Hero->nCurrentFrame.PutEncrypted(0);
							}
						}
					}
				}
			}
}

int CheckLeftControlKey()		// LTS ACTION
{
	if (isKeyDown(SDL_SCANCODE_LCTRL) || isKeyDown(SDL_SCANCODE_RCTRL)) return true;
	return false;
}


void DoHeroEvent(int x, int y)
{
	int range = 100;
	if (CheckLeftControlKey()) range = 32;	// LTS DRAGON MODIFY
	int sx, sy, ex, ey;
	int dx, dy;

	if (Hero == NULL) return;

	if (IsDead(Hero)) return;

	ex = ((LPCHARACTER)(Hero->lpAttacked))->x;
	ey = ((LPCHARACTER)(Hero->lpAttacked))->y;
	sx = Hero->x;
	sy = Hero->y;

	if ((ex - sx) * (ex - sx) + (ey - sy) * (ey - sy) > 20000) goto ACCESS_;

	if (Hero == (LPCHARACTER)(Hero->lpAttacked)) return;

	if (CheckEventWith_NPC(Hero, (LPCHARACTER)(Hero->lpAttacked)) == FALSE)
	{
	ACCESS_:

		if (ReturnXYNearNPC(&x, &y, &dx, &dy, range, Hero, (LPCHARACTER)(Hero->lpAttacked)))
		{
			g_ActionAnger = TRUE;						// LTS ACTION
			Hero->destx = dx, Hero->desty = dy;
			if (PathBuild(Hero))
			{
				// ### ������ ������. 
#ifdef _DEBUG // finito 060507
				if (SysInfo.notconectserver)
				{
					if (Hero->nCurrentAction != Hero->basicWalk)
					{
						Hero->moveFlag = TRUE;
						Hero->gox = x;
						Hero->goy = y;
						Hero->movetype = 0;
						Hero->nCurrentAction = Hero->basicWalk;
						Hero->nCurrentFrame.PutEncrypted(0);
					}
				}
				else
#endif
				{
					if (GetIndexOfCondition(SCharacterData.condition) == CC_DAZE)		return;//ȥ������

					Hero->moveFlag = TRUE;
					Hero->gox = x;
					Hero->goy = y;

					if (Hero->peacests)
					{
						Hero->movetype = 1;
					}
					else
					{
						if (Hero->pathcount > 5) Hero->movetype = 1;
						else Hero->movetype = 0;
					}

					switch (Hero->movetype)
					{
					case 0:
						if (Hero->nCurrentAction != Hero->basicWalk)
						{
							if (Hero->nCurrentAction != ACTION_SITDOWN)		// 011213 LTS
							{
								Hero->nCurrentAction = Hero->basicWalk;
								Hero->nCurrentFrame.PutEncrypted(0);
							}
						}
						break;

					case 1:
						if (Hero->nCurrentAction != ACTION_RUN)
						{
							if (Hero->nCurrentAction != ACTION_SITDOWN)		// 011214 LTS
							{
								Hero->nCurrentAction = ACTION_RUN;
								Hero->nCurrentFrame.PutEncrypted(0);
							}
						}
						break;
					}

					if (CheckLeftControlKey())			// LTS DRAGON MODIFY
					{
						LPCHARACTER tempch = (LPCHARACTER)(Hero->lpAttacked);

						if (abs(tempch->x - Hero->x) < 50 && abs(tempch->y - Hero->y) < 50)	//���ְ���� �Ÿ��� �־ �н���尡 �����Ѱ��
						{
							ChangeDirection(&Hero->direction, Hero->x, Hero->y, tempch->x, tempch->y);
							SendAction(ACTION_ANGER, Hero->direction);					// LTS ACTION
						}
						else
						{
							SendMoveData(Hero);
						}
					}
					else
					{
						SendMoveData(Hero);
					}

					if ((LPCHARACTER)(Hero->lpAttacked))
					{
						if (((LPCHARACTER)(Hero->lpAttacked))->id < 10000)
						{
							SendChatData(GreetingStr);
						}
					}
				}
			}
			else
			{
			}
		}
	}
}

int NotNeededMouseLButtonClick(void)
{
	if (Hero == NULL) return 1;

	if (g_StartMenuOn == TRUE)  return 1;
	if (g_pointMouseY > GAME_SCREEN_YSIZE) return 1;
	if (g_MouseInMenu) return 1;		// 0306 YGI

	return 0;
}


//////////////////////////////

//////////////////////////////// 0313 lkh ���� ////////////////////////////////
///////////////////////// �̱��� 1220 /////////////////////////////
// ���콺�� Ŭ���Ѱ��� Map��ǥ�� ��´�.
// ���� ���� ���ϸ� �����Ѱ��� ��ǥ�� �ش�. 
void MouseClickPosition(int* mx, int* my)
{
	int distance_X = 0;
	int	temp_Dist_X = 0;
	int distance_Y = 0;
	int	temp_Dist_Y = 0;

	if (GetIndexOfCondition(SCharacterData.condition) == CC_DAZE)		//ȥ������		// 0116 YGI
	{
		////////////////////// 0331 lkh ���� ////////////////////////
		temp_Dist_X = abs(Hero->x - Mox);
		temp_Dist_Y = abs(Hero->y - Moy);


		if (temp_Dist_X < 32)		temp_Dist_X = 32;
		if (temp_Dist_Y < 32)		temp_Dist_Y = 32;

		distance_X = (temp_Dist_X * 2) + ((rand() % temp_Dist_X / 2) - (temp_Dist_X / 4));
		distance_Y = (temp_Dist_Y * 2) + ((rand() % temp_Dist_Y / 2) - (temp_Dist_Y / 4));

		switch (rand() % 4)
		{
		case 0:		//x ��Ī
			if (Hero->x > Mox)
				Mox += distance_X;
			else
				Mox -= distance_X;
			break;
		case 1:		//y ��Ī
			if (Hero->y > Moy)
				Moy += distance_Y;
			else
				Moy -= distance_Y;
			break;
		case 2:		//x,y ��Ī
			if (Hero->x > Mox)
				Mox += distance_X;
			else
				Mox -= distance_X;
			if (Hero->y > Moy)
				Moy += distance_Y;
			else
				Moy -= distance_Y;
			break;
		case 3:		//random pattern
			Mox = rand() % distance_X - distance_X / 2;
			Moy = rand() % distance_Y - distance_Y / 2;
			break;
		}
	}


	*mx = Mox;
	*my = Moy;
}


#define MELEE				0
#define RANGE_BOW			1	
#define RANGE_DEFAULT		2

int GetItemKind(int itemno)
{
	int itemkind = GetItemAttr(itemno, ITEM_KIND);
	if (itemkind == 0) return -1;

	switch (itemkind)
	{
	case IK_LONG_BOW:
	case IK_ARROW_POINT:
	case IK_HURL:
	case IK_SHORT_BOW:
	case IK_DUAL_BOW://030102 lsw
	case IK_TWO_ARROW:
	case IK_DUAL_ARROW://030102 lsw
	case IK_SNOW_BALL:
	{ // ��Ÿ� ����
		return RANGE_BOW;
	}
	case IK_WHIRL:
	case IK_STAFE:
	case IK_SPEAR:
	case IK_FORK:
	{ // �߰Ÿ� ����
		return RANGE_DEFAULT;
	}
	}

	return MELEE; // �ܰŸ�����
}

// mousebutton : 0 : LeftClick 
//				 1 : Left Double Click..
//////////////////////////// 0522 LKH ���� //////////////////////////////
void HeroActionProc(int mousebutton, int mx, int my)
{	//< CSD-030930
	if (g_StartMenuOn == TRUE)  return;
	if (!Hero) return;

	if (IsMouseOnMenu()) return;
	if (g_block_move) return;
	mx /= TILE_SIZE;	mx *= TILE_SIZE;	mx += (TILE_SIZE / 2);
	my /= TILE_SIZE;	my *= TILE_SIZE;	my += (TILE_SIZE / 2);

	if (Hero)
	{
		if (abs(mx - Hero->x) / TILE_SIZE > Hero->sight + 10)	return;
		if (abs(my - Hero->y) / TILE_SIZE > Hero->sight + 10)	return;
	}
	//< CSD-021021
	if (Hero->condition & CON_STONE)
	{ // �Ʒ� ���¿����� ������ ���� �� �̵��Ұ�
		return;
	}
	// �ٽ� ���� ȸ��
	if (Hero->viewtype == VIEWTYPE_STONE_)
	{
		Hero->viewtype = VIEWTYPE_NORMAL_;
	}
	//> CSD-021021
	switch (Hero->peacests)
	{
	case PC_STATUS_PEACE:
	{
		switch (g_GameInfo.nSelectedSpriteType)
		{
		case SPRITETYPE_NONE:  // ������ ���¿��� �ƹ��͵� Checking�Ȱ��� ���ٸ�.. �׳� �̵��Ѵ�.
		{
			Hero->nAttackedType = 0;
			Hero->lpAttacked = NULL;
			DoHeroMove(mousebutton, mx, my);
			break;
		}
		case SPRITETYPE_ON_THE_CEILING_CHAR:
		case SPRITETYPE_CHARACTER:
		case SPRITETYPE_MONSTER:
		case SPRITETYPE_NPC:
		{
			Hero->nAttackedType = g_GameInfo.nSelectedSpriteType;
			Hero->lpAttacked = (void*)g_GameInfo.lpcharacter;
			DoHeroEvent(mx, my);
			break;
		}
		}

		break;
	}
	case PC_STATUS_BATTLE:
	{
		switch (g_GameInfo.nSelectedSpriteType)
		{
		case SPRITETYPE_NONE:
		{	// ������ ���¿��� �ƹ��͵� Checking�Ȱ��� ���ٸ�.. �׳� �̵��Ѵ�. 
			if (ACTION_ATTACK_BOW == Hero->nCurrentAction || ACTION_THROW == Hero->nCurrentAction)
			{	//ȭ���⳪ �� ������ �����߿� Ÿ���� �ʱ�ȭ ���� ����� �Ѵ�.
				goto NEXT_PROCESS_;
			}	// 0303011 kyo

			Hero->nAttackedType = 0;
			Hero->lpAttacked = NULL;
			DoHeroMove(mousebutton, mx, my);
			break;
		}
		case SPRITETYPE_ON_THE_CEILING_CHAR:
		case SPRITETYPE_CHARACTER:
		case SPRITETYPE_MONSTER:
		case SPRITETYPE_NPC:
		{
			switch ((g_GameInfo.lpcharacter)->sprno)	//���࿡ ���� �����Ϸ��� ���̰� Į�� ���� ������....
			{
			case 92:
			case 93:
			{	// �ܰŸ� ���� ������ ����
				if (GetItemKind(EquipItemAttr[WT_WEAPON].item_no) != MELEE)
				{
					AddCurrentStatusMessage(255, 100, 100, lan->OutputMessage(9, 6));//010215 lsw
					goto RET_;
				}

				goto NEXT_PROCESS_;
			}
			case 94:
			{	// ��Ÿ� ���� ������	
				if (GetItemKind(EquipItemAttr[WT_WEAPON].item_no) != RANGE_BOW)
				{
					AddCurrentStatusMessage(255, 100, 100, lan->OutputMessage(9, 7));//010215 lsw
					goto RET_;
				}

				goto NEXT_PROCESS_;
			}
			case 95:
			{	// �߰Ÿ� ���� ������
				if (GetItemKind(EquipItemAttr[WT_WEAPON].item_no) != RANGE_DEFAULT)
				{
					AddCurrentStatusMessage(255, 100, 100, lan->OutputMessage(9, 8));//010215 lsw
					goto RET_;
				}

				goto NEXT_PROCESS_;
			}
			}
		}
		case SPRITETYPE_ITEM:
		{
		NEXT_PROCESS_:
			DoHeroAttack(mx, my);
			break;
		}
		}
		break;
	}
	}

RET_:
	HeroActionProcChecked = true;
}	//> CSD-030930

int g_CurrCharStatus;
#define CCS_NONE		0	
#define CCS_ATTACKING	1	// �������̾���.
#define CCS_MOVING		2	// �ȴ����̾���.




void DoLbuttonStill()
{
	static bool flag = true;
	switch (g_CurrCharStatus)
	{
	case CCS_ATTACKING:
	{
		static int ct = 1;
		if (!(ct % 8))		// 15���� 1��
		{
			YouCanHeroActionProc = 1;
			DoLButtonDown();
		}
		ct++;
		break;
	}
	default: flag = true;
	}
}

void DoLButtonDown(void)	// LButton�� �ٿ�Ǹ� ����Ǿ� ���°�...
{
	int mx, my;
	if (ViewTipsCheck()) return;
	if (!MouseCursorOnOff) return;
	if (NotNeededMouseLButtonClick()) return;
	if (CounselerSelectChar()) return;
	if (CheckSkill()) return;
    if(g_MouseInMenuThisFrame) return;


	if (tool_ID_INPUT_MYHOUSE)
		return;


	if (tool_mapobject_modify == 0) IDCheckedMapObject = -1;
	if (IDCheckedMapObject != -1)
	{
		//ViewMapObjectInfo();
		return;
	}
	if (tool_IDD_INPUT_MAPOBJECT)
	{
		InsertMapObject(tool_CurMapObjectId, Mox, Moy);
		return;
	}

	if (YouCanHeroActionProc)
	{
		YouCanHeroActionProc = 0; real_move = 1;
		MouseClickPosition(&mx, &my);
		if (CheckEventWith_Item()) return;
		HeroActionProc(Lc_DefaultMoveType, mx, my);
		g_CurrCharStatus = CCS_ATTACKING;
		real_move = 0;
	}
	else return;

	// 011210 YGI
	ClickCharacter();		//  Ŭ�� 
}


extern	void	PutMouseItemCheck();
void DoLButtonUp(void)
{
	g_CurrCharStatus = CCS_NONE;
	//	int mx, my;										
	if (ViewTipsCheck())  return;

	if (g_MouseItemType == 1)//�������� ��� �ְ� ��򰡿� ���´�
	{
		switch (g_GameInfo.nSelectedSpriteType)
		{
		case SPRITETYPE_ON_THE_CEILING_CHAR:
		case SPRITETYPE_MONSTER:
		case SPRITETYPE_NPC:	break;
		case SPRITETYPE_CHARACTER:	//021030 lsw //�� ����� ĳ���Ͷ��
		{
			if (g_GameInfo.lpcharacter && g_allowDrop)		// 0131 YGI //Eleval 22/05/09: to disable droping on char that end in an exchange when g_allowDrop == false
			{
				if (g_MouseInMenu)
				{
					return;		// 0224 YGI
				}
				if (g_GameInfo.lpcharacter->id >= 0 && g_GameInfo.lpcharacter->id <= 10000)
				{
					if (g_GameInfo.lpcharacter != Hero)//��밡 �� ĳ���Ͱ� �ƴ϶��
					{
						if (CheckPC && g_GameInfo.lpcharacter->viewtype != VIEWTYPE_GHOST_)//030127 lsw
						{
							LButtonDown = 0;
							MouseDrag = 0;
							::PutMouseItemCheck();// ������ �������� �ǵ��� ���´�. ( �� ���ڸ���.. )
							::SendExchangeItemWithOtherCh(g_GameInfo.lpcharacter->id, EXCHANGE_TYPE_NORMAL);//021030 lsw
							g_DropOnHero = true;
						}
					}
				}
			}
		}break;
		}
		return;
	}

	if (YouCanHeroActionProc)
	{
		int mx, my;
		YouCanHeroActionProc = 0; real_move = 1;
		MouseClickPosition(&mx, &my);
		if (CheckEventWith_Item()) return;
		HeroActionProc(Lc_DefaultMoveType, mx, my);
		real_move = 0;
	}
	else return;

}



void DoRButtonDown(void)
{
	static int t;

	int x = 0, y = 0;

	if (!MouseCursorOnOff) return;// ���������.

	//////////////////// 0311 lkh �߰� ////////////////////
	switch (g_Operator_Function)
	{
	case OPERATOR_PLAYSOUND_EVENT1:
		SendCMD_CHARACTER_DEMAND(CCD_PLAYSOUND_EVENT1, Hero->id);
		g_Operator_Function = 0;
		break;
	case OPERATOR_PLAYSOUND_EVENT2:
		SendCMD_CHARACTER_DEMAND(CCD_PLAYSOUND_EVENT2, Hero->id);
		g_Operator_Function = 0;
		break;
	case OPERATOR_OUTPUT_CELEBRATE:
		if (g_GameInfo.lpcharacter == NULL)
		{
			g_Operator_Function = 0;
			break;
		}
		if (g_GameInfo.lpcharacter->type == SPRITETYPE_CHARACTER || g_GameInfo.lpcharacter->type == SPRITETYPE_MONSTER)
		{	//< CSD-TW-030606
			g_mgrBattle.SendMagicExecute((char)OPERATOR_OUTPUT_CELEBRATE, g_GameInfo.lpcharacter, g_GameInfo.lpcharacter->x, g_GameInfo.lpcharacter->y);
			g_Operator_Function = 0;
		}	//> CSD-TW-030606
		break;
	}
	//////////////////// ������� ////////////////////////

	SkillSettingClear();

	if (g_StartMenuOn) return;


	//	if( CursorCheckedItem != NULL ) { ViewItemInfo(); return; }

		// 011210 YGI
	/*if (tool_MultiChar || (IsReporterMode() && TileMap[Mox / TILE_SIZE][Moy / TILE_SIZE].attr_dont == 0))
	{
		//Mox-=TILE_SIZE;
		Moy -= TILE_SIZE;
		TileMap[Hero->position.x][Hero->position.y].occupied = 0;
		Hero->x = Mox;
		Hero->y = Moy;
		Hero->gox = Mox;
		Hero->goy = Moy;
		Hero->position.x = Mox / TILE_SIZE;
		Hero->position.y = Moy / TILE_SIZE;
		SendMoveData(Hero);
		HeroActionProc(Lc_DefaultMoveType, Mox, Moy + TILE_SIZE);
		return;
	}*/

	if (tool_IDD_INPUT_MAPOBJECT)
	{
		DeleteMapObject(IDCheckedMapObject);
		return;
	}


	/////////////////////////// 0430 lkh ���� //////////////////////////////
	if (!IsMenuActive())		// Ȱ��ȭ�� �޴��ȿ� ���콺�� �ִ����� �Ǵ�
	{
		if (g_AttackSkill_Trace1)
		{
			//InsertMagic( Hero, Hero, 8, 1, 0, 0, 0, 0, 0); // 030402 kyo //�̻� ��¡ ����
		}
		else if (g_AttackSkill_Trace2)
		{
			//InsertMagic( Hero, Hero, 8, 2, 0, 0, 0, 0, 0); // 030402 kyo
		}
		else
		{
			CheckDoingAppointMagic();
		}
	}

	if (Hero->nCurrentAction == 17)		// 011214 LTS // ���� ���¿��� ������ �ٲپ� �ش�.
	{
		switch (Hero->direction)
		{
		case DIRECTION_UP: Hero->direction = DIRECTION_RIGHTUP;	break;
		case DIRECTION_RIGHTUP: Hero->direction = DIRECTION_RIGHT;		break;
		case DIRECTION_RIGHT: Hero->direction = DIRECTION_RIGHTDOWN;	break;
		case DIRECTION_RIGHTDOWN: Hero->direction = DIRECTION_DOWN;		break;
		case DIRECTION_DOWN: Hero->direction = DIRECTION_LEFTDOWN;	break;
		case DIRECTION_LEFTDOWN: Hero->direction = DIRECTION_LEFT;		break;
		case DIRECTION_LEFT: Hero->direction = DIRECTION_LEFTUP;		break;
		case DIRECTION_LEFTUP: Hero->direction = DIRECTION_UP;			break;
		}
		t_packet p;
		p.h.header.type = CMD_SITDOWN_DIRECTION_CHANGED;
		p.u.LocalWar.SDChanged.Direction = Hero->direction;
		p.h.header.size = sizeof(t_SDChanged);
		QueuePacket(&p, 1);
		return;
	}

}



void DoRButtonUp(void)
{
}

void DoLDButtonDown(void)
{
	int mx, my;

	return;

	if (NotNeededMouseLButtonClick()) return;

	MouseClickPosition(&mx, &my);

	if (Lc_DefaultMoveType == 0) //  �⺻�޸���� �����Ǿ��ִٸ� DoubleClick �� �����ϳ� �׷��� ������ ȣ������ �ʴ´�.  
		HeroActionProc(1, mx, my);
}


void DoRDButtonDown(void)
{

}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//////			
//////			
//////			����������  ����ϴ°�..
//////			Sorting�Ͽ� ����Ѵ�.
//////					

int BoxAndBoxCrash_(int ax, int ay, int axl, int ayl, int bx, int by, int bxl, int byl)
{
	if (ax + axl >= bx && bx + bxl >= ax && ay + ayl >= by && by + byl >= ay)
		return 1;

	return 0;
}


int CrashTileandEffect(LPORDERTABLE oj, LPORDERTABLE oi)
{
	int ax, ay, axl, ayl, bx, by, bxl, byl;
	Spr* sp;

	bx = oj->x, by = oj->y;
	bxl = TILE_SIZE;  byl = TILE_SIZE;
	sp = (Spr*)(oi->lpvData);
	ax = oi->x - sp->ox; ay = oi->y - sp->oy - oi->height;
	axl = sp->xl; ayl = sp->yl;

	return BoxAndBoxCrash_(ax, ay, axl, ayl, bx, by, bxl, byl);
}



int CrashOiOj(LPORDERTABLE oi, LPORDERTABLE oj)
{
	//	return 1;
	int ax, ay, axl, ayl, bx, by, bxl, byl;
	LPCHARACTER		ch;
	LPITEMGROUND    i;
	LPMAPOBJECT		mo;
	Spr* sp;


	switch (oi->wType)
	{
	case SPRITETYPE_ALPHAOBJECT:
	case SPRITETYPE_OBJECT:
	{
		switch (oj->wType)
		{
		case SPRITETYPE_CHARACTER:
		case SPRITETYPE_NPC:
		case SPRITETYPE_MONSTER:
			mo = (LPMAPOBJECT)oi->lpvData;
			sp = &MoImage[mo->id - 1].spr[0];
			ax = mo->x - sp->ox + mo->offx; ay = mo->y - sp->oy + mo->offy;
			axl = sp->xl; ayl = sp->yl;

			ch = (LPCHARACTER)oj->lpvData;
			sp = ch->sp;
			bx = oj->x - sp->ox, by = oj->y - sp->oy;
			bxl = sp->xl;   byl = sp->yl;

			return BoxAndBoxCrash_(ax, 0, axl, 2, bx, 0, bxl, 3);
			break;
		case SPRITETYPE_ITEM:		i = (LPITEMGROUND)oj->lpvData;
			//											if( ( i->ddx[i->curframe/2] != 0 ) && ( i->ddy[i->curframe/2] != 0 )  ) return 0;
			sp = i->spr[i->curframe / 2];
			bx = oj->x - sp->ox, by = oj->y - sp->oy;
			bxl = sp->xl;  byl = sp->yl;

			mo = (LPMAPOBJECT)oi->lpvData;
			sp = &MoImage[mo->id - 1].spr[0];
			ax = mo->x - sp->ox + mo->offx; ay = mo->y - sp->oy + mo->offy;
			axl = sp->xl; ayl = sp->yl;

			break;
		case SPRITETYPE_TILE:		bx = oj->x, by = oj->y;
			bxl = TILE_SIZE;  byl = TILE_SIZE;
			mo = (LPMAPOBJECT)oi->lpvData;
			sp = &MoImage[mo->id - 1].spr[0];
			ax = mo->x - sp->ox; ay = mo->y - sp->oy;
			axl = sp->xl; ayl = sp->yl;
			break;

		case SPRITETYPE_ALPHAOBJECT:
		case SPRITETYPE_OBJECT: 	mo = (LPMAPOBJECT)oi->lpvData;
			sp = &MoImage[mo->id - 1].spr[0];
			ax = mo->x - sp->ox + mo->offx; ay = mo->y - sp->oy + mo->offy;
			axl = sp->xl; ayl = sp->yl;

			mo = (LPMAPOBJECT)oj->lpvData;
			sp = &MoImage[mo->id - 1].spr[0];
			bx = mo->x - sp->ox + mo->offx; by = mo->y - sp->oy + mo->offy;
			bxl = sp->xl; byl = sp->yl;

			return BoxAndBoxCrash_(ax, 0, axl, 2, bx, 0, bxl, 3);


			break;



		default: return 0;
		}

		//			int t = 

		return BoxAndBoxCrash_(ax, ay, axl, ayl, bx, by, bxl, byl);
	}
	break;

	case SPRITETYPE_ITEM:
	{
		switch (oj->wType)
		{
		case SPRITETYPE_CHARACTER:
		case SPRITETYPE_NPC:
		case SPRITETYPE_MONSTER:
			i = (LPITEMGROUND)oi->lpvData;
			sp = i->spr[i->curframe / 2];
			ax = oi->x - sp->ox, ay = oi->y - sp->oy;
			axl = sp->xl;  ayl = sp->yl;

			ch = (LPCHARACTER)oj->lpvData;
			sp = ch->sp;
			bx = oj->x - sp->ox, by = oj->y - sp->oy;
			bxl = sp->xl;   byl = sp->yl;
			break;
		case SPRITETYPE_ALPHAOBJECT:
		case SPRITETYPE_OBJECT:	mo = (LPMAPOBJECT)oi->lpvData;
			if (mo->attr2) return 0;
			sp = &MoImage[mo->id - 1].spr[0];
			bx = mo->x - sp->ox + mo->offx; by = mo->y - sp->oy + mo->offy;
			bxl = sp->xl; byl = sp->yl;

			i = (LPITEMGROUND)oi->lpvData;
			sp = i->spr[i->curframe / 2];
			ax = oi->x - sp->ox, ay = oi->y - sp->oy;
			axl = sp->xl;  ayl = sp->yl;
			break;
		case SPRITETYPE_TILE:		bx = oj->x, by = oj->y;
			bxl = TILE_SIZE;  byl = TILE_SIZE;
			i = (LPITEMGROUND)oi->lpvData;
			sp = i->spr[i->curframe / 2];
			ax = oi->x - sp->ox, ay = oi->y - sp->oy;
			axl = sp->xl;  ayl = sp->yl;
			break;
		default: return 0;
		}
		return BoxAndBoxCrash_(ax, ay, axl, ayl, bx, by, bxl, byl);
	}
	break;
	default: return 0;
	}

	return 0;
}

int SortWithItem(LPORDERTABLE oi, LPORDERTABLE oj)
{
	if (oi->wType != SPRITETYPE_ITEM) return 0;

	LPITEMGROUND i = (LPITEMGROUND)oi->lpvData;

	if (i->type != ITEMTYPE_DOOR) return 0;

	return CrashOiOj(oi, oj);
}

int SortWithObject(LPORDERTABLE oi, LPORDERTABLE oj)
{
	LPMAPOBJECT mo;

	if (oi->wType != SPRITETYPE_OBJECT) return 0;
	if (oi->wType != SPRITETYPE_ALPHAOBJECT) return 0;
	mo = ((LPMAPOBJECT)(oi->lpvData));
	if (mo->attr2 == 0) return 0;

	//	if( oj->wType == SPRITETYPE_OBJECT ) 
	//	{
	//		mo = ((LPMAPOBJECT )(oj->lpvData));
	//		if( mo->attr2 ) return 0; 
	//	}

	return CrashOiOj(oi, oj);
}

int SortWithObjectObject(LPORDERTABLE oi, LPORDERTABLE oj)
{
	if (oi->wType != SPRITETYPE_OBJECT) return 0;
	if (oi->wType != SPRITETYPE_ALPHAOBJECT) return 0;
	if (oj->wType != SPRITETYPE_OBJECT) return 0;
	if (oj->wType != SPRITETYPE_ALPHAOBJECT) return 0;

	return 1;
}


// -----------------------------------------------------------------------------------
// 3. 全新的「比較函式」- 100% 邏輯等價 (修正版)
// -----------------------------------------------------------------------------------
bool CompareOrderItems_Safe(const ORDERTABLE& a, const ORDERTABLE& b)
{
	// std::sort 的比較函式：如果 a 應該排在 b 前面 (a < b)，則返回 true。
	LPORDERTABLE pa = const_cast<LPORDERTABLE>(&a);
	LPORDERTABLE pb = const_cast<LPORDERTABLE>(&b);

	// ----------------------------------------------------------------
	// 這一段邏輯，是嚴格按照您舊有選擇排序的 if/else if 優先級，
	// 進行的 1:1 邏輯翻譯。
	// 舊邏輯: if (condition) { lpmin = oj; } -> 意思是 oj < lpmin
	// 新邏輯: if (condition_for(b,a)) { return false; } -> 意思是 b < a
	// ----------------------------------------------------------------

	if (SortWithItem(pa, pb)) {
		LPITEMGROUND ti = (LPITEMGROUND)(pa->lpvData);
		int c = ti->curframe >> 1;
		if (Determinant(ti->dsx[c], ti->dsy[c], ti->ddx[c], ti->ddy[c], pb->x, pb->offset) < 0) return false; // b < a
	}
	else if (SortWithItem(pb, pa)) {
		LPITEMGROUND tj = (LPITEMGROUND)(pb->lpvData);
		int c = tj->curframe >> 1;
		if (Determinant(tj->dsx[c], tj->dsy[c], tj->ddx[c], tj->ddy[c], pa->x, pa->offset) > 0) return true; // a < b
	}
	else if (SortWithObjectObject(pb, pa)) { // 注意：這裡是 (b, a)
		LPMAPOBJECT mo1 = (LPMAPOBJECT)(pb->lpvData);
		LPMAPOBJECT mo2 = (LPMAPOBJECT)(pa->lpvData);
		int mo1minoff = min(mo1->y + mo1->dy0[mo1->curframe], mo1->y + mo1->dy1[mo1->curframe]);
		int mo2minoff = min(mo2->y + mo2->dy0[mo2->curframe], mo2->y + mo2->dy1[mo2->curframe]);
		if (mo1minoff < mo2minoff) return false; // b < a
	}
	else if (SortWithObject(pb, pa)) {
		LPMAPOBJECT mo = (LPMAPOBJECT)(pb->lpvData);
		int c = mo->curframe;
		if (Determinant(pb->x + mo->dx0[c], pb->y + mo->dy0[c], pb->x + mo->dx1[c], pb->y + mo->dy1[c], pa->x, pa->offset) > 0) return true; // a < b
	}
	else if (SortWithObject(pa, pb)) {
		LPMAPOBJECT mo = (LPMAPOBJECT)(pa->lpvData);
		int c = mo->curframe;
		if (Determinant(pa->x + mo->dx0[c], pa->y + mo->dy0[c], pa->x + mo->dx1[c], pa->y + mo->dy1[c], pb->x, pb->offset) < 0) return false; // b < a
	}
	// 【關鍵修正 1】將主要的 Y 軸排序，作為所有特殊規則之後的判斷
	else if (a.offset != b.offset) {
		return a.offset < b.offset; // Y 座標小的排在前面 (先畫)
	}
	// 【關鍵修正 2】處理 Y 軸相同時的 tie-breaking 規則
	else if (a.offset == b.offset) {
		if (b.wType == SPRITETYPE_TILE) {
			if (a.wType == SPRITETYPE_OBJECT || a.wType == SPRITETYPE_ALPHAOBJECT) {
				LPMAPOBJECT mo = (LPMAPOBJECT)a.lpvData;
				if (mo->attr == OBJECTATTR_BACK_ABOVE || mo->attr == OBJECTATTR_FRONT_ABOVE || mo->attr == OBJECTATTR_FRONT_BELOW) return false; // Tile(b) < Object(a)
			}
		}
		// 反向檢查
		if (a.wType == SPRITETYPE_TILE) {
			if (b.wType == SPRITETYPE_OBJECT || b.wType == SPRITETYPE_ALPHAOBJECT) {
				LPMAPOBJECT mo = (LPMAPOBJECT)b.lpvData;
				if (mo->attr == OBJECTATTR_BACK_ABOVE || mo->attr == OBJECTATTR_FRONT_ABOVE || mo->attr == OBJECTATTR_FRONT_BELOW) return true; // Tile(a) < Object(b)
			}
		}
	}

	// 【關鍵修正 3】如果以上所有規則都無法決定順序，則它們是相等的。
	// 為了滿足 std::sort 的要求，我們返回 false。
	return false;
}

void handleItemSelection(const LPORDERTABLE pi, int Mox, int Moy) {
	auto item = static_cast<LPITEMGROUND>(pi->lpvData);
	auto sp = item->spr[item->curframe / 2];

	if ((item->attr[0] & (IA2_BOX | IA2_COLOSSUS_STONE)) &&
		BoxAndDotCrash(item->x + item->offx - sp->ox, item->y + item->offy - sp->oy, sp->xl, sp->yl, Mox, Moy)) {
		g_GameInfo.lpvSelectedSprite = item;
		g_GameInfo.nSelectedSpriteType = pi->wType;
		g_GameInfo.lpcharacter = reinterpret_cast<LPCHARACTER>(item);
	}
}

void handleCharacterSelection(const LPORDERTABLE pi, int Mox, int Moy) {
	auto ch = static_cast<LPCHARACTER>(pi->lpvData);

	if (ch->just_ani) {
		return;
	}
	int dir = ch->direction;
	int sprc = ch->AnimationTable[ch->nCurrentAction].cFrame[ch->nCurrentFrame.GetDecrypted()];
	int xl, yl;
	int ox, oy;
	auto sp = &(CharSpr[ch->sprno].sp[dir][sprc]);

	ox = sp->ox;
	oy = sp->oy;
	xl = sp->xl; if (xl < 15) { ox <<= 2;  xl <<= 2; }
	yl = sp->yl;	if (yl < 15) { oy <<= 2;  xl <<= 2; }

	if (!BoxAndDotCrash(pi->x - ox, pi->y - oy - ch->height, xl, yl, Mox, Moy)) {
		return;
	}

	if (ch->viewtype == VIEWTYPE_OBSERVE_) {
		return;
	}

	if (ch->viewtype == VIEWTYPE_GHOST_) {
		if (IsHidenGhost(ch) || (!CanViewGhost(Hero) && Hero->viewtype != VIEWTYPE_GHOST_)) {
			return;
		}
	}
	else {
		MouseCheckCharacterName = ch;
		if (IsDead(ch) && SkillOn == -1) {
			return;
		}

		bool bClickOn = false;
		if (ch->id < 10000) {
			if (g_FightMapStart)
			{
				if (Hero->fight_map_team_num && ch->fight_map_team_num && Hero->fight_map_team_num != ch->fight_map_team_num) bClickOn = true;
			}
			else if (ch->name_status.nation != Hero->name_status.nation && ch->lv >= 1)	// 011022 LTS
			{	//< CSD-040414
				/*if (!CheckNotClickNation())
				{
					bClickOn = true;	// Ŭ�� �Ұ����� �ƴϸ� // 010724 YGI

					for (int i = 0; i < 6; ++i)
					{
						if (SCharacterData.party[i].ch == ch)
						{
							bClickOn = false;
							break;
						}
					}
				}*/
			}	
			else if (IsNK(ch) >= 5 && ch != Hero)				bClickOn = true;
			else if (Hero->fight_flag && Hero->fight_id == ch->id) bClickOn = true; // ������.. 

			if (g_bWarBegin)	// �������̸�	// 011022 LTS
			{
				if (isOurSquad(Hero, ch)) bClickOn = false; //�������̰� ��Ÿ����̸�
				else bClickOn = true;
			}

			if (Hero->DragonLordWarTeam > 0 && ch->DragonLordWarTeam > 0)		// LTS DRAGONLORD
			{
				if (Hero->DragonLordWarTeam != ch->DragonLordWarTeam) bClickOn = true;		// �ٸ����̸� 
				else bClickOn = false;
			}
			if ((MapNumber == 39) && (g_EventLocalWarBegin == 3)) bClickOn = true;	// LTS BUG

			if (GetBattlePeace() == PK_MODE) bClickOn = true;
				
			//公會組隊,國家組隊
			if (Hero->guild_party) {
				bClickOn = (Hero->GetGuildCode() == ch->GetGuildCode()) ? false : true;
			}

			if (Hero->nation_party) {
				bClickOn = (Hero->name_status.nation == ch->name_status.nation) ? false : true;
			}

			if (!bClickOn)
			{
				if (CheckPC == false && SkillOn == -1 && g_mgrBattle.IsSelect(ch))
				{
					return;
				}
			}
		}
		else
		{
            if (!CheckPC && ch->nRace == GUARD) {
                return;
            }

            if (CheckPC == false && (ch->HostName[0]))
			{
				if (GetBattlePeace() != PK_MODE)
				{
					return;
				}
			}
		}
	}

	// 所有條件都通過後，集中進行賦值
	g_GameInfo.lpvSelectedSprite = ch;
	g_GameInfo.nSelectedSpriteType = pi->wType;
	g_GameInfo.lpcharacter = ch;
	MouseCheckCharacterName = ch;
}

// -----------------------------------------------------------------------------------
// 4. 重構後的 CalcOrder 函式
// -----------------------------------------------------------------------------------
void CalcOrder(void)
{
	// --- 1. 場景剔除 (與您舊有的邏輯完全相同) ---
	WaveTileStart++;
	WaveTileStart %= 39;

	LPCHARACTER ch = Hero;
	while (ch != NULL) {
		if (IsScreenArea(ch)) {
			if (g_OrderInfo.count >= MAX_ORDERTABLE) break;
			LPORDERTABLE o = &g_OrderInfo.order[g_OrderInfo.count];
			if (ch->height > 100) o->wType = SPRITETYPE_ON_THE_CEILING_CHAR;
			else o->wType = ch->type;

			o->x = ch->x;
			o->y = ch->y;
			o->height = ch->height;
			o->offset = ch->y;
			o->lpvData = ch;
			o->show = 1;
			o->sort = 1;
			g_OrderInfo.count++;

		}
		ch = ch->lpNext;
	}

	IDCheckedMapObject = -1;
	g_BoadAndSignNumber = 0;

	if (tool_DrawObject == 0) {
		for (int i = 0; i < TotalMapObject; i++) {
			LPMAPOBJECT mo = &Mo[i];
			if (IsScreenAreaCSP(mo->x + mo->offx, mo->y + mo->offy, &MoImage[mo->id - 1].spr[0])) {
				bool check = ReturnCheckObject(mo);
				CheckMapObject(mo);
				if (g_OrderInfo.count >= MAX_ORDERTABLE) break;
				LPORDERTABLE o = &g_OrderInfo.order[g_OrderInfo.count];
				if (check)	o->wType = SPRITETYPE_ALPHAOBJECT;
				else		o->wType = SPRITETYPE_OBJECT;
				o->x = mo->x;
				o->y = mo->y;
				o->offset = mo->y;
				o->rgb = i; 

				o->height = 0;
				o->lpvData = (LPVOID)(mo);
				o->show = 1;
				o->sort = 1;

				if (mo->light) OutputLight(mo->x + mo->offx + mo->lightx, mo->y + mo->offy + mo->lighty, mo->light);
				g_OrderInfo.count++;
			}
#ifdef _DEBUG // finito 060507							
			if (GetSysInfo(SI_GAME_MAKE_MODE))
			{
				// MapObject   Check Ҷ .1.        ǥ    浹 Ǵ  MapObject   ã ´ . 
				if (BoxAndDotCrash(mo->x - 10, mo->y - 10, 20, 20, Mox, Moy))
				{
					IDCheckedMapObject = i;
				}
			}
#endif
		}
	}
	/*NewOrderC = 0;
	if (g_OrderInfo.count > 0) {
		std::sort(g_OrderInfo.order, g_OrderInfo.order + g_OrderInfo.count, CompareOrderItems_Safe);

		for (int i = 0; i < g_OrderInfo.count; ++i) {
			LPORDERTABLE o = &g_OrderInfo.order[i];
			o->show = 1;
			o->sort = 0;
		}
		memcpy(neworder, g_OrderInfo.order, sizeof(ORDERTABLE) * g_OrderInfo.count);
		NewOrderC = g_OrderInfo.count;
	}*/

	// --- 3. 滑鼠碰撞檢測 (與您舊有的邏輯完全相同，順序也相同) ---
	IDCheckedMapObject = -1;
	if (GetSysInfo(SI_GAME_MAKE_MODE)) { // 假設 SI_GAME_MAKE_MODE 是 0
		for (int i = 0; i < TotalMapObject; i++) {
			LPMAPOBJECT mo = &Mo[i];
			if (BoxAndDotCrash(mo->x - 10, mo->y - 10, 20, 20, Mox, Moy)) {
				IDCheckedMapObject = i;
			}
		}
		if (IDCheckedMapObject == -1) {
			for (int i = 0; i < TotalMapObject; i++) {
				LPMAPOBJECT mo = &Mo[i];
				if (IsScreenAreaCSP(mo->x + mo->offx, mo->y + mo->offy, &MoImage[mo->id - 1].spr[0])) {
					Spr* sp = &MoImage[mo->id - 1].spr[mo->curframe];
					if (BoxAndDotCrash(mo->x + mo->offx - sp->ox, mo->y + mo->offy - sp->oy, sp->xl, sp->yl, Mox, Moy)) {
						IDCheckedMapObject = i;
					}
				}
			}
		}
	}
	MouseCheckCharacterName = NULL;
	g_GameInfo.nSelectedSpriteType = SPRITETYPE_NONE;
	g_GameInfo.lpcharacter = NULL;
	// --- 4. 最終的滑鼠選取邏輯 (與您舊有的邏輯完全相同) ---
	for (int i = g_OrderInfo.count - 1; i >= 0; --i) {
		LPORDERTABLE pi = &g_OrderInfo.order[i];
		pi->rank = i; // 您的舊邏輯中有這一步
		//  switch 結構，用來判斷滑鼠點中了哪個物件 ...
		switch (pi->wType)
		{
		case SPRITETYPE_ITEM:
		{
			handleItemSelection(pi, Mox, Moy);
		}break;
		case SPRITETYPE_ON_THE_CEILING_CHAR:
		case SPRITETYPE_MONSTER:
		case SPRITETYPE_NPC:
		case SPRITETYPE_CHARACTER:
		{
			handleCharacterSelection(pi, Mox, Moy);
		}break;

		}
	}
}

/*void CalcOrder(void)
{
	int	i, j;
	LPORDERTABLE	pi;

	LPCHARACTER		ch;
	//	LPTILE			lpTile;

	Spr* s;
	LPORDERTABLE	o, oj, lpmin;
	LPMAPOBJECT		mo;
	ORDERTABLE min;

	//	MapObjectTouchOrder();

	WaveTileStart++;
	WaveTileStart %= 39;

	ch = Hero;
	int cc = 0;
	while (ch != NULL)
	{
		cc++;
		if (IsScreenArea(ch))
		{
			if (g_OrderInfo.count >= MAX_ORDERTABLE) break;

			o = &g_OrderInfo.order[g_OrderInfo.count];

			if (ch->height > 100) o->wType = SPRITETYPE_ON_THE_CEILING_CHAR;
			else o->wType = ch->type;
			o->x = ch->x;
			o->y = ch->y;
			o->height = ch->height;
			o->offset = ch->y;
			o->lpvData = ch;
			o->show = 1;
			o->sort = 1;
			g_OrderInfo.count++;


		}
		ch = ch->lpNext;
	}




	// object				
	//lpObjectHeader = g_ObjectListHeader.lpHead;
	IDCheckedMapObject = -1;
	g_BoadAndSignNumber = 0; // CSD-030324 // thai2 YGI

	if (tool_DrawObject == 0)
		for (i = 0; i < TotalMapObject; i++)
		{
			mo = &Mo[i];
			if (IsScreenAreaCSP(mo->x + mo->offx, mo->y + mo->offy, &MoImage[mo->id - 1].spr[0]))
			{
				bool	check = ReturnCheckObject(mo);

				//////////////////////////// 0812 lkh �߰� ///////////////////////////////
				//	����� �Խ����� ����Ŭ��(���ʹ�ư)�ϴ� ��� �Խ��� �޴� ���.
	//			Spr *sp = &MoImage[ mo->id-1].spr[0];
	///			if( mo->id >= 2482 && mo->id <=2485 )
	///				if( BoxAndDotCrash( (int)(mo->x+mo->offx-sp->ox), (int)(mo->y+mo->offy-sp->oy), sp->xl, sp->yl, Mox, Moy) && g_MouseInMenu == false )
	//					if( g_nLDButtonState )
	//						CallSmallMenu(MN_HUNTER_MAIN);
				//////////////////////////////////////////////////////////////////////////

				CheckMapObject(mo);
				{
					if (g_OrderInfo.count >= MAX_ORDERTABLE) break;

					o = &g_OrderInfo.order[g_OrderInfo.count];

					if (check)	o->wType = SPRITETYPE_ALPHAOBJECT;
					else		o->wType = SPRITETYPE_OBJECT;		//ALPHAOBJECT
					o->x = mo->x;
					o->y = mo->y;
					o->offset = mo->y;
					o->rgb = i; // rgb�� ���°��� �ƴ϶� �迭��ȣ�� ����. 

					o->height = 0;
					o->lpvData = (LPVOID)(mo);
					o->show = 1;
					o->sort = 1;
					if (mo->light) OutputLight(mo->x + mo->offx + mo->lightx, mo->y + mo->offy + mo->lighty, mo->light);
					g_OrderInfo.count++;
				}
#ifdef _DEBUG // finito 060507							
				if (GetSysInfo(SI_GAME_MAKE_MODE))
				{
					// MapObject�� Check�Ҷ�.1. ���� ��ǥ�� �浹�Ǵ� MapObject�� ã�´�. 
					if (BoxAndDotCrash(mo->x - 10, mo->y - 10, 20, 20, Mox, Moy))
					{
						IDCheckedMapObject = i;
					}
				}
#endif
			}
		}


	// MapObject�� Check�Ҷ�.2. ���콺�� �浹�Ǵ� MapObject�� ã�´�. 
	if (GetSysInfo(SI_GAME_MAKE_MODE))
		if (IDCheckedMapObject == -1)
			for (i = 0; i < TotalMapObject; i++)
			{
				mo = &Mo[i];
				if (IsScreenAreaCSP(mo->x + mo->offx, mo->y + mo->offy, &MoImage[mo->id - 1].spr[0]))
				{
					{
						Spr* sp = &MoImage[mo->id - 1].spr[mo->curframe];
						if (BoxAndDotCrash(mo->x + mo->offx - sp->ox, mo->y + mo->offy - sp->oy, sp->xl, sp->yl, Mox, Moy))
						{
							IDCheckedMapObject = i;
						}
					}
				}
			}

	int minid;

	NewOrderC = 0;
	for (i = 0; i < g_OrderInfo.count; i++)
	{
		for (j = 0; j < g_OrderInfo.count; j++)
		{
			oj = &g_OrderInfo.order[j];
			if (oj->sort == 0) continue;

			switch (oj->wType)
			{
			case SPRITETYPE_DETECT_TILE:
			case SPRITETYPE_SKILLCURSOR:
				continue;
			}
			memcpy(&min, oj, sizeof(ORDERTABLE));
			lpmin = oj;
			minid = j;
			break;
		}

		for (j = 0; j < g_OrderInfo.count; j++)
		{
			if (minid == j) continue;

			oj = &g_OrderInfo.order[j];
			if (oj->sort == 0) continue;

			switch (oj->wType)
			{
			case SPRITETYPE_DETECT_TILE:
			case SPRITETYPE_SKILLCURSOR:
				continue;
			}

			if (SortWithItem(lpmin, oj))
				//if( CrashlpminOj( lpmin, oj ) )
			{
				LPITEMGROUND ti = (LPITEMGROUND)(lpmin->lpvData);
				int c = ti->curframe >> 1;
				int kkx = lpmin->x, kky = lpmin->y;
				if (Determinant(ti->dsx[c], ti->dsy[c], ti->ddx[c], ti->ddy[c], oj->x, oj->offset) < 0)
				{
					//memcpy( &min, oj, sizeof( ORDERTABLE ) );
					lpmin = oj;//min;
					minid = j;
				}
			}
			else if (SortWithItem(oj, lpmin))
			{
				LPITEMGROUND tj = (LPITEMGROUND)(oj->lpvData);
				int c = tj->curframe >> 1;
				int kkx = oj->x, kky = oj->y;
				if (Determinant(tj->dsx[c], tj->dsy[c], tj->ddx[c], tj->ddy[c], lpmin->x, lpmin->offset) > 0)
				{
					//memcpy( &min, oj, sizeof( ORDERTABLE ) );
					lpmin = oj;//min;
					minid = j;
				}
			}
			else if (SortWithObjectObject(oj, lpmin))
			{
				LPMAPOBJECT mo1 = (LPMAPOBJECT)(oj->lpvData);
				LPMAPOBJECT mo2 = (LPMAPOBJECT)(lpmin->lpvData);

				int mo1minoff = min(mo1->y + mo1->dy0[mo1->curframe], mo1->y + mo1->dy1[mo1->curframe]);
				int mo2minoff = min(mo2->y + mo2->dy0[mo2->curframe], mo2->y + mo2->dy1[mo2->curframe]);

				if (mo1minoff < mo2minoff)
				{
					lpmin = oj;//min;
					minid = j;
				}
			}
			else if (SortWithObject(oj, lpmin))
			{
				LPMAPOBJECT mo = (LPMAPOBJECT)(oj->lpvData);
				int kkx = mo->x, kky = mo->y;
				int c = mo->curframe;
				if (Determinant(kkx + mo->dx0[c], kky + mo->dy0[c], kkx + mo->dx1[c], kky + mo->dy1[c], lpmin->x, lpmin->offset) > 0)
				{
					//memcpy( &min, oj, sizeof( ORDERTABLE ) );
					lpmin = oj;//min;
					minid = j;
				}
			}

			else if (SortWithObject(lpmin, oj))
			{
				mo = (LPMAPOBJECT)(lpmin->lpvData);
				int kkx = lpmin->x, kky = lpmin->y;
				int c = mo->curframe;
				TestDeter = Determinant(kkx + mo->dx0[c], kky + mo->dy0[c], kkx + mo->dx1[c], kky + mo->dy1[c], oj->x, oj->offset);
				if (TestDeter < 0)
				{
					//memcpy( &min, oj, sizeof( ORDERTABLE ) );
					lpmin = oj;//min;
					minid = j;
				}
			}
			else if (lpmin->offset > oj->offset)
			{
				//memcpy( &min, oj, sizeof( ORDERTABLE ) );
				lpmin = oj;//min;
				minid = j;
			}
			else if (lpmin->offset == oj->offset)
			{
				if (oj->wType == SPRITETYPE_TILE)
				{
					switch (lpmin->wType)
					{
						//						case	SPRITETYPE_CHARACTER:
						//						{		
						//							memcpy( &temp,	oj, sizeof( ORDERTABLE ) );
						//							memcpy( oj,		lpmin, sizeof( ORDERTABLE ) );
						//							memcpy( lpmin,	 &temp, sizeof( ORDERTABLE ) );
						//							break;
						//						}		
					case SPRITETYPE_ALPHAOBJECT:
					case	SPRITETYPE_OBJECT:
					{
						LPMAPOBJECT	mo = (LPMAPOBJECT)lpmin->lpvData;

						if (mo->attr == OBJECTATTR_BACK_ABOVE || mo->attr == OBJECTATTR_FRONT_ABOVE || mo->attr == OBJECTATTR_FRONT_BELOW)
						{
							//memcpy( &min, oj, sizeof( ORDERTABLE ) );
							lpmin = oj;//min;
							minid = j;
						}
						break;
					}
					}
				}
			}
		}

		g_OrderInfo.order[minid].sort = 0;
		memcpy(&neworder[NewOrderC], &g_OrderInfo.order[minid], sizeof(ORDERTABLE));
		NewOrderC++;
	}

	for (i = 0; i < g_OrderInfo.count; i++)
	{
		pi = &g_OrderInfo.order[i];

		if (pi->sort)
		{
			pi->sort = 0;

			memcpy(&neworder[NewOrderC], pi, sizeof(ORDERTABLE));
			NewOrderC++;
		}
	}

	LPITEMGROUND item;
	Spr* sp;
	bool bClickOn = false;

	MouseCheckCharacterName = NULL;
	g_GameInfo.nSelectedSpriteType = SPRITETYPE_NONE;
	g_GameInfo.lpcharacter = NULL;
	for (i = 0; i < g_OrderInfo.count; i++)
	{
		pi = &g_OrderInfo.order[i];
		pi->rank = i;

		switch (pi->wType)
		{
		case SPRITETYPE_ITEM:
			item = (LPITEMGROUND)pi->lpvData;
			sp = item->spr[item->curframe / 2];
			if (item->attr[IATTR_ATTR] & IA2_BOX || item->attr[IATTR_ATTR] & IA2_COLOSSUS_STONE)
			{
				if (BoxAndDotCrash(item->x + item->offx - sp->ox, item->y + item->offy - sp->oy, sp->xl, sp->yl, Mox, Moy))
				{
					g_GameInfo.lpvSelectedSprite = pi->lpvData;
					g_GameInfo.nSelectedSpriteType = pi->wType;
					g_GameInfo.lpcharacter = (LPCHARACTER)pi->lpvData;
					return;
				}
			}
			break;

		case SPRITETYPE_ON_THE_CEILING_CHAR:
		case SPRITETYPE_MONSTER:
		case SPRITETYPE_NPC:
		case SPRITETYPE_CHARACTER:

			ch = (LPCHARACTER)pi->lpvData;
			int dir = ch->direction;
			int sprc = ch->AnimationTable[ch->nCurrentAction].cFrame[ch->nCurrentFrame.GetDecrypted()];
			int xl, yl;
			int ox, oy;
			//int height	= ch->AnimationTable[ ch->nCurrentAction].height[  ch->nCurrentFrame.GetDecrypted() ];
			s = &(CharSpr[ch->sprno].sp[dir][sprc]);

			ox = s->ox;
			oy = s->oy;
			xl = s->xl; if (xl < 15) { ox <<= 2;  xl <<= 2; }
			yl = s->yl;	if (yl < 15) { oy <<= 2;  xl <<= 2; }


			if (ch->just_ani == false)
				if (BoxAndDotCrash(pi->x - ox, pi->y - oy - ch->height, xl, yl, Mox, Moy))
				{
					if (ch->viewtype == VIEWTYPE_OBSERVE_)
					{	//< CSD-030509
						return;
					}	//> CSD-030509
					//	���� �����̰ų� ������ �� ���� ���� ��������� �����Ҽ� �ִ�. 
					if (ch->viewtype == VIEWTYPE_GHOST_)
					{
						// YGI ACER
						if (IsHidenGhost(ch)) break;//020510 lsw
						if (!CanViewGhost(Hero) && Hero->viewtype != VIEWTYPE_GHOST_) break;
						g_GameInfo.lpvSelectedSprite = pi->lpvData;
						g_GameInfo.nSelectedSpriteType = pi->wType;
						g_GameInfo.lpcharacter = (LPCHARACTER)pi->lpvData;
						MouseCheckCharacterName = (LPCHARACTER)pi->lpvData;		//1027 YGI
						return;
					}

					MouseCheckCharacterName = (LPCHARACTER)pi->lpvData;

					//	�׾� �������..............
					if (IsDead(ch))
					{
						if (SkillOn == -1) break;
					}


					//	�Ϲ� PC�ϰϿ쿡�� 
					if (ch->id < 10000)			// 0906 YGI	fightmap_2
					{
						if (g_FightMapStart) 
						{
							if (Hero->fight_map_team_num && ch->fight_map_team_num && Hero->fight_map_team_num != ch->fight_map_team_num) bClickOn = true;
						}

#ifdef _NATION_APPLY_
						else if (ch->name_status.nation != Hero->name_status.nation && ch->lv >= 1)	// 011022 LTS
						{	//< CSD-040414
							if (!CheckNotClickNation())
							{
								bClickOn = true;	// Ŭ�� �Ұ����� �ƴϸ� // 010724 YGI

								for (int i = 0; i < 6; ++i)
								{
									if (SCharacterData.party[i].ch == ch)
									{
										bClickOn = false;
										break;
									}
								}
							}
						}	//> CSD-040414
#endif
						else if (IsNK(ch) >= 5 && ch != Hero)				bClickOn = true;
						else if (Hero->fight_flag && Hero->fight_id == ch->id) bClickOn = true; // ������.. 

						if (g_bWarBegin)	// �������̸�	// 011022 LTS
						{
							if (isOurSquad(Hero, ch)) bClickOn = false; //�������̰� ��Ÿ����̸�
							else bClickOn = true;
						}
						if (Hero->DragonLordWarTeam > 0 && ch->DragonLordWarTeam > 0)		// LTS DRAGONLORD
						{
							if (Hero->DragonLordWarTeam != ch->DragonLordWarTeam) bClickOn = true;		// �ٸ����̸� 
							else bClickOn = false;
						}
						if ((MapNumber == 39) && (g_EventLocalWarBegin == 3)) bClickOn = true;	// LTS BUG

						if (!bClickOn)
							if (CheckPC == false && SkillOn == -1 && g_mgrBattle.IsSelect(ch) && !bClickOn)
							{
								break;
							}
					}
					else
					{	//< CSD-031013
						if (CheckPC == false && (ch->HostName[0]))
						{
							if (GetBattlePeace() != PK_MODE)
							{
								break;
							}
						}
					}	//> CSD-031013

					g_GameInfo.lpvSelectedSprite = pi->lpvData;
					g_GameInfo.nSelectedSpriteType = pi->wType;
					g_GameInfo.lpcharacter = (LPCHARACTER)pi->lpvData;
					return;
				}
			break;
		}
	}
}*/

void DisplaySpriteUnderTile(void)
{
	int i;

	for (i = 0; i < g_OrderInfo.count; i++)
	{
		LPORDERTABLE o = &g_OrderInfo.order[i];

		//if (o->show == 0) continue;

		switch (o->wType)
		{
		case SPRITETYPE_ALPHAOBJECT:
		case SPRITETYPE_OBJECT:
		{
			//				LPMAPOBJECT mo;
			if (((LPMAPOBJECT)o->lpvData)->attr == OBJECTATTR_BACK_BELOW)
			{
				//o->show = 0;
				DisplayMapObject((LPMAPOBJECT)o->lpvData);
			}
			break;
		}
		}
	}
}

void ShowBuffTimer()
{
	uint16_t verticalSpaceCount = 0;
	

	for (const auto& itr : g_GameInfo.m_BuffTimer)
	{
		int nMagicID = itr.second.magicid;
		int Duration = itr.second.duration;
		int y = 62;
		if (nMagicID > 214)// START FROM LIGHTNING_BOOM = 215
		{
			const int iImageNo = SkillIdToCombatSkillIconImage(CombatSkill[nMagicID- 215].iSkillNo, false);
			FieldTypeNomalPut((SCREEN_WIDTH / 2) - 166 + verticalSpaceCount, y , 0, 0, iImageNo, COMBAT_SKILL_ICON_IMG);
		}
		else
		{
			if (nMagicID > 150) //RESTORE_AUTHORITY = 151
			{
				if (SCharacterData.nCharacterData[SPELL] == PRIEST_SPELL)
					PutMagicIcon((SCREEN_WIDTH / 2) - 140 + verticalSpaceCount, y, nMagicID-150);
				else PutMagicIconNotSameAsSpell((SCREEN_WIDTH / 2) - 140 + verticalSpaceCount, y, nMagicID - 150);
				
			}
			else
			{
				if (SCharacterData.nCharacterData[SPELL] == PRIEST_SPELL)
					PutMagicIconNotSameAsSpell((SCREEN_WIDTH / 2) - 140 + verticalSpaceCount, y, nMagicID);
				else PutMagicIcon((SCREEN_WIDTH / 2) - 140 + verticalSpaceCount, y, nMagicID);
			}
		}
		HprintBold((SCREEN_WIDTH / 2) - 146+ verticalSpaceCount, y , FONT_COLOR_SOLID_YELLOW, 0, "%ds", Duration);
		verticalSpaceCount += 38;
	}
}

void DrawTextOnScreen() {

	if (g_SDLInfo.bFullscreen && system_info.counter)
	{
		HprintBold(10, 10, RGB(0, 255, 100), 0, title.data());
	}

	if (g_GameInfo.exp_event) 
	{
		HprintBold(10, 88, RGB(0, 255, 100), 0, "🔥 EXP🚀");
	}

	if (Hero->display_item)
	{
		DrawItemInfoOnScreen();
	}
}

void DisplaySpriteBottom(void)
{
	int	i = 0;

	for (i = 0; i < g_OrderInfo.count; i++)
	{
		LPCHARACTER		ch = (LPCHARACTER)g_OrderInfo.order[i].lpvData;
		LPORDERTABLE o = &g_OrderInfo.order[i];

		switch (o->wType)
		{
		case SPRITETYPE_EFFECT_BOTTOM:
		{
			//o->show = 0;
			switch (o->type)
			{
			case 1:
			case 2:
			case 3:
			case 4:
				PutCompressedImageFX(o->x - Mapx, o->y - o->height - Mapy, (Spr*)(o->lpvData), o->alpha, o->type);
				break;
			case 5:
				PutCompressedImage(o->x - Mapx, o->y - o->height - Mapy, (Spr*)(o->lpvData));
				break;
			case 6:
				PutCompressedImageBlendRGB(o->x - Mapx, o->y - o->height - Mapy, (Spr*)(o->lpvData), o->rgb);
				break;

			case 7:
				PutCompressedImageVoid(o->x - Mapx, o->y - o->height - Mapy, (Spr*)(o->lpvData));
				break;

			}
			break;
		}
		case SPRITETYPE_ALPHAOBJECT:
		case SPRITETYPE_OBJECT:
		{
			//				LPMAPOBJECT mo;
			if (((LPMAPOBJECT)o->lpvData)->attr == OBJECTATTR_BACK_ABOVE)
			{
				//o->show = 0;
				DisplayMapObject((LPMAPOBJECT)o->lpvData);
				HandleRunning(&connections);
			}
			break;
		}
		case SPRITETYPE_ITEM_BOTTOM:
		{
			//o->show = 0;

			LPITEMGROUND  item = (LPITEMGROUND)o->lpvData;
			Spr* sp = item->spr[item->curframe / 2];

			if (item->shadow)	PutCompressedImageShadow(o->x - Mapx, o->y - Mapy, sp);
			PutCompressedImage(o->x - Mapx + item->offx, o->y - Mapy + item->offy, sp);
			if (o->rgb && item->getable)	PutCompressedImageFX(o->x - Mapx + item->offx, o->y - Mapy + item->offy, sp, o->alpha, 2);
			MaskMapItem(o->x + item->offx, o->y + item->offy, o->y, sp);
#ifdef _DEBUG
			if (GetSysInfo(SI_GAME_MAKE_MODE))
			{
				if (item->type == ITEMTYPE_DOOR)
				{
					Hcolor(0, 0xff, 0);
					Hprint2(item->dx[0] - Mapx, item->dy[0] - 40 - Mapy, g_DestBackBuf, "ITEMTYPE_DOOR");
					Hcolor(0xffff);
				}
				Hprint2(item->dx[0], item->dy[0] - 20, g_DestBackBuf, "%d", item->no);

				Box(item->dx[0], item->dy[0], item->dx[1], item->dy[1], item->dx[2], item->dy[2], item->dx[3], item->dy[3], 0xffff);
			}
#endif
			break;
		}
		//			case  SPRITETYPE_DEST_POINT_BOTTOM :
		//			{
		//				o->show = 0;
		//				PutCompressedImageFX( o->x-Mapx, o->y-Mapy, (Spr*)(o->lpvData), o->alpha, 1 );
		//				break;
		//			}

		}
	}
}


void DisplayShadowFirst(void)
{
	int	i;


	for (i = 0; i < g_OrderInfo.count; i++)
	{
		LPORDERTABLE o = &g_OrderInfo.order[i];

		switch (o->wType)
		{
		case SPRITETYPE_ON_THE_CEILING_CHAR:
		case SPRITETYPE_CHARACTER:
		case SPRITETYPE_MONSTER:
		case SPRITETYPE_NPC:
		{
			LPCHARACTER		ch = (LPCHARACTER)g_OrderInfo.order[i].lpvData;
			if (ch->sprno > 1)	h_DisplayNPC(ch, 0);
			else h_DisplayPC(ch, 0);
			break;
		}
		}
	}
}



void DisplayTileFirst(void)
{
	int	i;

	for (i = 0; i < g_OrderInfo.count; i++)
	{
		LPCHARACTER		ch = (LPCHARACTER)g_OrderInfo.order[i].lpvData;
		LPORDERTABLE	o = &g_OrderInfo.order[i];

		//if (o->show == 0) continue;

		switch (o->wType)
		{
		case	SPRITETYPE_TILE:
		{
			//o->show = 0;
			int mx = o->x - Mapx;
			int my = o->y - Mapy;
			int front = ((LPTILE)o->lpvData)->front;

			if (Sou[front].xl == -1)  PutTile(mx, my, &Sou[front], Sou[front].img, g_DestBackBuf);
			else						PutCompressedImage(mx, my, &Sou[front]);
			break;
		}
		}
	}
}



extern int PutGrowingGauge(int x, int y);
extern Spr	  SquadPic[3][4];		// 011018 LTS
extern void DrawSquadPic(int x, int y, LPORDERTABLE o, LPCHARACTER ch);
extern void	DrawLocalWarSquadPic(int x, int y, LPORDERTABLE o, LPCHARACTER ch);	// LTS HORSERIDER
extern bool isEnemy(LPCHARACTER ch1, LPCHARACTER ch2);	// LTS DRAGON BUG
extern int		g_LocalWarBegin;		// LTS DRAGON BUG

void DisplaySprite_Character(int& x, int& y, LPCHARACTER ch, LPORDERTABLE o)//021108 lsw
{
	if (tool_MultiChar)
	{
		int tx = ch->x;
		int ty = ch->y;
		ch->x = o->x;
		ch->y = o->y;
		if (TileMap[ch->x / TILE_SIZE][ch->y / TILE_SIZE].attr_dont == 0)
		{
			if (ch->sprno > 1)	h_DisplayNPC(ch, 1);
			else h_DisplayPC(ch, 1);
		}
		ch->x = tx;
		ch->y = ty;
	}
	else
	{
		if (ch->sprno > 1)	h_DisplayNPC(ch, 1);
		else h_DisplayPC(ch, 1);
	}

	//x = ch->x - Mapx + 1;
	//y = ch->y - Mapy - CharSpr[ch->sprno].sp[0][0].yl - 20;
	x = static_cast<int>(ch->visual_x - Mapx + 1);
	y = static_cast<int>(ch->visual_y - Mapy - CharSpr[ch->sprno].sp[0][0].yl - 20);

	if (o->wType == SPRITETYPE_CHARACTER) // 011018 LTS
	{
		if (ch == Hero)//020626 lsw
		{
			switch (Hero->viewtype)
			{	//< CSD-030509
			case VIEWTYPE_GHOST_:
			case VIEWTYPE_OBSERVE_:
			{
				break;
			}
			default:
			{
				if (Hero->HorseNo)
				{
					::PutHeroHpGauge(x - 15, y - ch->height - 30);	   // npc HP������ ���  // 1015 YGI
					::PutHeroMpGauge(x - 15, y - ch->height + 3 - 30); // npc HP������ ���  // 1015 YGI
				}
				else
				{
					::PutHeroHpGauge(x - 15, y - ch->height + 10);	   // npc HP������ ���  // 1015 YGI
					::PutHeroMpGauge(x - 15, y - ch->height + 3 + 10); // npc HP������ ���  // 1015 YGI
				}

				break;
			}
			}	//> CSD-030509		
		}

		if (ch->HorseNo)
		{
			//::DrawSquadPic(o->x - Mapx, o->y - Mapy - 115, o, ch);
			::DrawSquadPic(x, y - 55, o, ch);
		}
		else
		{
			//::DrawSquadPic(o->x - Mapx, o->y - Mapy - 90, o, ch);
			::DrawSquadPic(x, y - 50, o, ch);
		}
		if (ch->JoinLocalWar)
		{
			if (ch->HorseNo)
			{
				//DrawLocalWarSquadPic(o->x - Mapx, o->y - Mapy - 115, o, ch);	// LTS HORSERIDER
				DrawLocalWarSquadPic(x, y - 55, o, ch);	// LTS HORSERIDER
			}
			else
			{
				//DrawLocalWarSquadPic(o->x - Mapx, o->y - Mapy - 90, o, ch);	// LTS HORSERIDER
				DrawLocalWarSquadPic(x, y - 50, o, ch);	// LTS HORSERIDER
			}
		}
	}
#ifdef _DEBUG
	if (GetSysInfo(SI_GAME_MAKE_MODE))
	{
		if (ch->dieani)  // 0910 KHS
		{
			::HprintBold(x + 10 - 40, y - 4 - ch->height, RGB(100, 100, 0xff), 0, "%d", ch->dieani);
		}

		if (ch->id > 10000)
		{
			if (ch->ChairNum)
			{
				::HprintBold(x + 10 - 40, y - 4 - ch->height + 20, RGB(255, 255, 255), 0, "S_AI");	// LTS AI
			}
			else
			{
				::HprintBold(x + 10 - 40, y - 4 - ch->height + 40, RGB(255, 255, 255), 0, "C_AI");	// LTS AI
			}
		}
	}
#endif

	int ttty;
	if (ch->viewtype == VIEWTYPE_GHOST_)
	{
		ttty = -15;		// 1027 YGI
	}
	else
	{
		ttty = 0;
	}

	if (!GetSysInfo(SI_GAME_MAKE_MODE))
	{
		if (ch != Hero)					// LTS DRAGON MODIFY
		{	//< CSD-040322 : �������� Ÿ������ ������ ���̵��� ����
			
			//> CSD-040322
			if (g_bWarBegin)
			{
				if (ch->sprno <= 1)
				{
					if (isEnemy(Hero, ch)) { return; }
				}
			}
			if (g_EventLocalWarBegin)
			{
				if (MapInfo[MapNumber].port == 5390)
				{
					if (ch->sprno <= 1) { return; }
				}
			}
			if (Hero->DragonLordWarTeam > 0 && ch->DragonLordWarTeam > 0)		// LTS DRAGONLORD
			{
				if (Hero->DragonLordWarTeam != ch->DragonLordWarTeam)		// �ٸ����̸� 
				{
					return;
				}
			}
		}
	}


	if (ch->smiledelay)
	{// 031112 kyo
		::DisplayEmote(x, y, ch->smileno);//020515 lsw
	}

	//角色顯示頭像namedisplaycount
	if (ch->namedisplaycount > 1 && !ch->ChatDelay)
	{
		int tttx = spr[71].xl / 2; // �̸��۾��ڿ� ������ ���..
		if (ch->type == SPRITETYPE_CHARACTER) // PC�鸸 ������ �ִ�....
		{	//< CSD-030324
			if (!CheckPC && CheckGuildCode(ch->GetGuildCode()))		// 1027 YGI		// ��Ʈ���� ������ ��� 
			{
				::PutGuildImage(x - tttx + -20, y - 24 - ch->height + ttty, ch->GetGuildCode());		// ��� ��ũ�� ��´�.
			}
			else
			{
				::PutCharImage(x - tttx + -20, y - 24 - ch->height + ttty, ch->face, 0, FS_MICRO, 0, ch->sex);
			}
		}	//> CSD-030324
		else
		{
			::PutHpGauge(x + 5, y - ch->height, ch);		// npc HP������ ���  // 1015 YGI
		}

		if (ch->name[0])
		{	//< CSD-031013	
			if (ch->type == SPRITETYPE_CHARACTER)
			{
#ifdef _NATION_APPLY_
				if (!CheckPC)// 1027 YGI
				{
					if (CheckGuildCode(ch->GetGuildCode()))// 1004 YGI
					{	//< CSD-030324
						/*std::string guild_text = "";
						if (g_GuildMgr[ch->GetGuildCode()].first)
						{
							guild_text = g_GuildMgr[ch->GetGuildCode()].m_szName;
						}
						else
						{
							guild_text = CallGuildDegreeName(ch->GetGuildCode(), ch->name_status.guild_master);
						}*/
						PutLvNameByColor(x - tttx + 116, y - 4 - 15 - ch->height + ttty, ch);
					}	//> CSD-030324
					else
					{
						PutLvNameByColor(x - tttx + 10, y - 4 - 15 - ch->height + ttty, ch);		// ���� ���
					}
					PutDualMark(x - tttx + 5, y - 4 - 20 - ch->height + ttty, ch);
				}
				else
				{
					// ���� ���� ���
					if (ch->job)
					{
						PutJobLv(x - tttx + 10, y - 19 - ch->height + ttty, ch->job, ch->call_job_lv);		// 1004 YGI( ���� ��� ��� )
					}
				}

#else
				PutLvNameByColor(x - tttx + 10, y - 4 - 15 - ch->height + ttty, ch);		// 0811 YGI
#endif
				char nation[20];
				int  nk;

				nk = IsNK(ch, nation);
				if (nk >= 5 || (nk > 0 && ch == Hero))
				{
					HprintBold(x - tttx + 10, y - 4 - ch->height + ttty, RGB(0xff, 0xff, 0xff), 0, "%s", ch->name);
					HprintBold(x - tttx + 10 + strlen(ch->name) * 6 + 6, y - 4 - ch->height + ttty, RGB(0xff, 0, 0), 0, "(%s%d)", nation, nk);
				}
				else
				{
					HprintBold(x - tttx + 10, y - 4 - ch->height + ttty, RGB(0xff, 0xff, 0xff), 0, "%s", ch->name);
				}
				// �����̸��� ���.....
#ifdef _NATION_APPLY_				
				if (!CheckPC && CheckGuildCode(ch->GetGuildCode()))// 1004 YGI
				{	//< CSD-030324
					PutGuildLvByColor(x - tttx + 10, y - 19 - ch->height + ttty, ch->GetGuildCode(), ch->name_status.guild_master);	// ��峻 ��å ���
				}	//> CSD-030324
//					else
				{
					switch (ch->name_status.nation)		// 1004 YGI
					{
					case 0:
						break;
					default:	::HprintBold(x - tttx + 10, y - 34 - ch->height + ttty, NationColor[ch->name_status.nation], 0, "%s", NationName[ch->name_status.nation]);		// 001029 YGI( ����� ��� )
						break;
					}
				}

				if (ch->name_status.king)
				{
					::HprintBold(x - tttx + 10, y - 52 - ch->height + ttty, RGB(200, 0xff, 0xff), 0, "[%s]", PosOfNation[ch->sex]);		// 1031 YGI( ����� ��� )
				}
#endif						
				if (ch->name_status.reporter)
				{
					// ���ڴ�
					::HprintBold(x - tttx + 10, y - 52 - ch->height + ttty, RGB(200, 0xff, 0xff), 0, kein_GetMenuString(93));
				}
				/*if (ch->name_status.ChampionTitle)
				{

					switch (ch->name_status.ChampionTitle)
					{

					case 1:

						HprintBold(x - tttx + 10, y - 52 - ch->height + ttty, RGB(250, 255, 255), 0, "[Champion 2023]");
						break;
					case 2:
						HprintBold(x - tttx + 10, y - 52 - ch->height + ttty, RGB(250, 255, 255), 0, "[Champion 2023]");
						break;
					case 3:
						HprintBold(x - tttx + 10, y - 52 - ch->height + ttty, RGB(250, 255, 255), 0, "[Champion 2024]");
						break;
					case 4:
						HprintBold(x - tttx + 10, y - 52 - ch->height + ttty, RGB(250, 255, 255), 0, "[Champion 2025]");
						break;
					case 5:
						HprintBold(x - tttx + 10, y - 52 - ch->height + ttty, RGB(250, 255, 255), 0, "[Champion 2026]");
						break;
					}



				}
				else*/
				{
					switch (ch->name_status.counselor)
					{
					case 1:	// ���
					{
						HprintBold(x - tttx + 10, y - 52 - ch->height + ttty, RGB(250, 255, 255), 0, kein_GetMenuString(42)); //Game Master
						break;
					}
					case 2:	// �����
					{
						HprintBold(x - tttx + 10, y - 52 - ch->height + ttty, RGB(250, 255, 255), 0, kein_GetMenuString(43)); //Helper
						break;
					}
					case 3:
					{
						HprintBold(x - tttx + 10, y - 52 - ch->height + ttty, RGB(250, 255, 255), 0, "[Development Team]");
					}
					}
				}
			}
			else
			{
				if (ch->nRace != HUMAN)
				{
					EatRearWhiteChar(g_infNpc[ch->sprno].szName);
					//::HprintBold( x -tttx+ 10, y-4-15-ch->height , RGB( 0, 0xff, 0 ),0, "%s(Lv:%d)", MonsterName[ ch->sprno].name, ch->lv );		// 0808 YGI NPC
					const int nBaseX = x - tttx + 10;
					const int nBaseY = y - ch->height - 19;

					if (ch->nAttr == AT_NORMAL)
					{
#ifdef _DEBUG
						HprintBold(nBaseX, nBaseY, RGB(0xff, 0xff, 0x00), 0, "%s(spr:%d)", g_infNpc[ch->sprno].szName, ch->sprno);
#else	
                        HprintBold(nBaseX, nBaseY, RGB(0xff, 0xff, 0), 0, "%s", g_infNpc[ch->sprno].szName);
#endif
				    }
					else
					{
						const int nName = 190 + ch->nAttr;
						const int nOffset = (strlen(lan->OutputMessage(4, nName)) + 1) * FONTSIZE_;
						HprintBold(nBaseX, nBaseY, RGB(0xff, 0, 0), 0, "%s", lan->OutputMessage(4, nName));
						HprintBold(nBaseX + nOffset, nBaseY, RGB(0xff, 0xff, 0), 0, "%s", g_infNpc[ch->sprno].szName);
					}
				}
				// Tamming�� Character
				if (ch->HostName[0])
				{
					HprintBold(x - tttx + 10, y - 4 - ch->height, RGB(100, 100, 0xff), 0, "%s(%s)", ch->name, ch->HostName);
				}
				else
				{
					HprintBold(x - tttx + 10, y - 4 - ch->height, RGB(0xff, 0xff, 0xff), 0, "%s", ch->name);
				}
			}
		}
#ifdef _DEBUG
		if (GetSysInfo(SI_GAME_MAKE_MODE))
		{
			::HprintBold(x - tttx - 30, y - 4 - ch->height + 32, RGB(0xff, 0xff, 0xff), 0, "%d, %d", ch->sprno, ch->id);
		}
#endif
	}
	else if (ch->fight_map_team_num)// 0906 YGI	fightmap_2
	{
		switch (ch->viewtype)
		{	//< CSD-030520
		case VIEWTYPE_GHOST_:
		case VIEWTYPE_OBSERVE_:
		case VIEWTYPE_TRANSPARENCY_:
		{
			break;
		}
		default:
		{
			::HprintBold(x - 10, y - 4 - ch->height, FightTeamColor[ch->fight_map_team_num - 1], 0, "%s", FightTeam[ch->fight_map_team_num - 1]);
			break;
		}
		}	//> CSD-030520
	}

	//o->show = 0;
}


void DisplaySprite_Item(int& x, int& y, LPCHARACTER ch, LPORDERTABLE o)//021108 lsw
{
	//o->show = 0;
	LPITEMGROUND  item = (LPITEMGROUND)o->lpvData;

	int skip = 0;
	const int iIX = item->x;
	const int iIY = item->y;
	const int iIOX = item->offx;
	const int iIOY = item->offy;

	switch (item->Anitype)
	{
	case ITEMANIMATION_FIRE:
	{
		::PutCompressedImage(iIX - Mapx + iIOX, iIY - Mapy + iIOY, item->spr[0]);
	}break;
	case ITEMANIMATION_ACTIVE: // 020701 YGI
	{
		switch (item->attr[IATTR_SPRITE_TYPE] & 0x0003)
		{
		case OBJECTSPRITE_ALPHA:
		{
			::PutCompressedImage(iIX - Mapx, iIY - Mapy, item->spr[0]);
		}
		case OBJECTSPRITE_TRANS:
		{
			::PutCompressedImageFX(iIX - Mapx, iIY - Mapy, item->spr[item->curframe / 2], item->attr[IATTR_SPRITE_TYPE] >> 3, 2);
			OutputLight(iIX, iIY, 16);		// ����
		}break;
		}
		skip = 1;
	}break;
	}
	if (skip) { return; }
	if (item->curframe < 0)
	{
		item->curframe = 0;		// 020701 YGI
	}
	///////////////////////////////////////

	Spr* sp = item->spr[item->curframe / 2];
	//if( item->shadow)	::PutCompressedImageShadow(  o->x - Mapx, o->y-Mapy, sp );	

	if (::CanViewBomb(Hero)) //020320 lsw
	{
		if (item->attr[IATTR_ATTR] & IA2_TRAPED0)
		{
			::PutCompressedImage(iIX - Mapx + iIOX, iIY - Mapy + iIOY, sp);
		}
	}
	if (::CanViewTrap(Hero)) //020320 lsw
	{
		if (item->attr[IATTR_ATTR] & IA2_TRAPED1)
		{
			::PutCompressedImage(iIX - Mapx + iIOX, iIY - Mapy + iIOY, sp);
		}
	}

	if (item->attr[IATTR_ATTR] & IA2_FIRE) // ���� �ǿ���..
	{
		::PutCompressedImageFX(iIX - Mapx + iIOX, iIY - Mapy + iIOY - 16, sp, 10 + (rand() % 10), 2);
	}
	else if (item->attr[IATTR_ATTR] & IA2_HIDE) // �Ⱥ��̴� �Ӽ��̸�...
	{
		if (Hero->canviewitem)
		{
			::PutCompressedImageFX(iIX - Mapx + iIOX, iIY - Mapy + iIOY, sp, rand() % 15, 2);
			::PutCompressedImageVoid(iIX - Mapx + iIOX + rand() % 4 - 2, iIY - Mapy + iIOY + rand() % 4 - 2, sp);
		}
		else
		{
			// �����.. ��.. �Ⱥ��̴ϱ�...
		}
	}
	else if (item->attr[IATTR_ATTR] & IA2_COLOSSUS_STONE)
	{	//���� ���� �÷� ��ȭ..
		const int team = GetTeamByStone(item->attr[IATTR_ATTR]);
		sp = item->spr[team + 1];
		if (sp->img)
		{
			::PutCompressedImage(iIX - Mapx + iIOX, iIY - Mapy + iIOY, sp);
		}
	}
	else if (item->attr[IATTR_ATTR] & IA2_DOOR)
	{
		::PutCompressedImage(iIX - Mapx + iIOX, iIY - Mapy + iIOY, sp);
	}
	else
	{	//< CSD-031029
		if (item->no / 1000 == 6)
		{	//< CSD-031101
			CItem_Weapon* pWeapon = static_cast<CItem_Weapon*>(ItemUnit(item->no));

			if (pWeapon != NULL)
			{
				const int nEffect = pWeapon->effect;

				if (nEffect > 0)
				{

					::PutCompressedImage(iIX - Mapx + iIOX, iIY - Mapy + iIOY, item->spr[0]);

					if (item->curframe / 2 != 0)
					{
						::PutCompressedImageFX(iIX - Mapx + iIOX, iIY - Mapy + iIOY, item->spr[item->curframe / 2], 10 + Random(4), 2);
					}
					else
					{
						::PutCompressedImageFX(iIX - Mapx + iIOX, iIY - Mapy + iIOY, item->spr[1], 10 + Random(4), 2);
					}
				}
				else
				{
					if (item->attr[IATTR_RARE_MAIN]) //������� ���� ���̶��
					{
						::PutCompressedImage(iIX - Mapx + iIOX, iIY - Mapy + iIOY, sp);
						::PutCompressedImageFX(iIX - Mapx + iIOX, iIY - Mapy + iIOY, sp, rand() % 15, 2);
					}
					else
					{
						::PutCompressedImage(iIX - Mapx + iIOX, iIY - Mapy + iIOY, sp);
					}
				}
			}
			else
			{
				if (item->attr[IATTR_RARE_MAIN]) //������� ���� ���̶��
				{
					::PutCompressedImage(iIX - Mapx + iIOX, iIY - Mapy + iIOY, sp);
					::PutCompressedImageFX(iIX - Mapx + iIOX, iIY - Mapy + iIOY, sp, rand() % 15, 2);
				}
				else
				{
					::PutCompressedImage(iIX - Mapx + iIOX, iIY - Mapy + iIOY, sp);
				}
			}
		}	//> CSD-031101
		else
		{
			if (item->attr[IATTR_RARE_MAIN]) //������� ���� ���̶��
			{
				::PutCompressedImage(iIX - Mapx + iIOX, iIY - Mapy + iIOY, sp);
				::PutCompressedImageFX(iIX - Mapx + iIOX, iIY - Mapy + iIOY, sp, rand() % 15, 2);
			}
			else
			{
				::PutCompressedImage(iIX - Mapx + iIOX, iIY - Mapy + iIOY, sp);
			}
		}
	}	//> CSD-031029

	if (o->rgb)
	{	//	�Ⱥ����� ������ �ɼ� �ִ�. 
		if ((CanViewBomb(Hero) && (item->attr[IATTR_ATTR] & IA2_TRAPED0))
			|| (CanViewTrap(Hero) && (item->attr[IATTR_ATTR] & IA2_TRAPED1)))
		{
		}
		else
		{
			if (item->attr[IATTR_ATTR] & IA2_FIRE) // ���� �ǿ���..
			{
				::PutCompressedImageFX(iIX - Mapx + iIOX, iIY - Mapy + iIOY - 16, sp, o->alpha, 2);
			}
			else
			{
				::PutCompressedImageFX(iIX - Mapx + iIOX, iIY - Mapy + iIOY, sp, o->alpha, 2);
			}
		}

		if (CursorCheckedItem)
		{
			if (CursorCheckedItem->attr[IATTR_ATTR] & ITEMANIMATION_FARM0)
			{
				::PutGrowingGauge(CursorCheckedItem->x - Mapx + CursorCheckedItem->offx, CursorCheckedItem->y - Mapy + CursorCheckedItem->offy - 20);
			}
			else if (CursorCheckedItem->attr[IATTR_ATTR] & ITEMANIMATION_FARM1)
			{
				::PutGrowingGauge(CursorCheckedItem->x - Mapx + CursorCheckedItem->offx, CursorCheckedItem->y - Mapy + CursorCheckedItem->offy - 20);
			}
			else if (CursorCheckedItem->attr[IATTR_ATTR] & ITEMANIMATION_FARM2)
			{
				::PutGrowingGauge(CursorCheckedItem->x - Mapx + CursorCheckedItem->offx, CursorCheckedItem->y - Mapy + CursorCheckedItem->offy - 20);
			}
			else if (CursorCheckedItem->attr[IATTR_ATTR] & ITEMANIMATION_FARM3)
			{
				::PutGrowingGauge(CursorCheckedItem->x - Mapx + CursorCheckedItem->offx, CursorCheckedItem->y - Mapy + CursorCheckedItem->offy - 20);
			}
			else if (CursorCheckedItem->attr[IATTR_ATTR] & ITEMANIMATION_FARM4)
			{
				::PutGrowingGauge(CursorCheckedItem->x - Mapx + CursorCheckedItem->offx, CursorCheckedItem->y - Mapy + CursorCheckedItem->offy - 20);
			}
			else if (CursorCheckedItem->attr[IATTR_ATTR] & ITEMANIMATION_FARM5)
			{
				::PutGrowingGauge(CursorCheckedItem->x - Mapx + CursorCheckedItem->offx, CursorCheckedItem->y - Mapy + CursorCheckedItem->offy - 20);
			}
			else if (CursorCheckedItem->attr[IATTR_ATTR] & ITEMANIMATION_FARM6)
			{
				::PutGrowingGauge(CursorCheckedItem->x - Mapx + CursorCheckedItem->offx, CursorCheckedItem->y - Mapy + CursorCheckedItem->offy - 20);
			}
			else if (CursorCheckedItem->attr[IATTR_ATTR] & ITEMANIMATION_FARM7)
			{
				::PutGrowingGauge(CursorCheckedItem->x - Mapx + CursorCheckedItem->offx, CursorCheckedItem->y - Mapy + CursorCheckedItem->offy - 20);
			}
		}
	}

	if (sp)
	{
		::MaskMapItem(iIX + iIOX, iIY + iIOY, iIY, sp); // 020620 YGI
	}
#ifdef _DEBUG	
	if (GetSysInfo(SI_GAME_MAKE_MODE))
	{
		if (item->type == ITEMTYPE_DOOR)
		{
			Hcolor(0, 0xff, 0);
			::Hprint2(item->dx[0] - Mapx, item->dy[0] - 40 - Mapy, g_DestBackBuf, "ITEMTYPE_DOOR");
			Hcolor(0xffff);
		}

		::Hprint2(item->dx[0] - Mapx, item->dy[0] - 20 - Mapy, g_DestBackBuf, "%d", item->no);
		Box(item->dx[0], item->dy[0], item->dx[1], item->dy[1], item->dx[2], item->dy[2], item->dx[3], item->dy[3], 0xffff);

		int cc = item->curframe / 2;
		Line(item->dsx[cc], item->dsy[cc], item->ddx[cc], item->ddy[cc], RGB16(0, 0, 255), false);
	}
#endif
}

void DisplaySprite_Effect(int& x, int& y, LPCHARACTER ch, LPORDERTABLE o)//021108 lsw
{
	switch (o->type)
	{
	case 1:
	case 2:
	case 3:
	case 4:
		::PutCompressedImageFX(o->x - Mapx, o->y - o->height - Mapy, (Spr*)(o->lpvData), o->alpha, o->type);
		break;
	case 5:
		::PutCompressedImage(o->x - Mapx, o->y - o->height - Mapy, (Spr*)(o->lpvData));
		break;
	case 6:
		::PutCompressedImageBlendRGB(o->x - Mapx, o->y - o->height - Mapy, (Spr*)(o->lpvData), o->rgb);
		break;
	case 7:
		::PutCompressedImageVoid(o->x - Mapx, o->y - o->height - Mapy, (Spr*)(o->lpvData));
		break;
	case 8:			//0502 lkh �߰�
		::PutCompressedImageVoid(o->x + (rand() % 5 - 2) - Mapx, o->y - o->height + (rand() % 5 - 2) - Mapy, (Spr*)(o->lpvData));
		::PutCompressedImageFX(o->x - Mapx, o->y - o->height - Mapy, (Spr*)(o->lpvData), o->alpha, 2);
		break;
	}
	::MaskMapEffect(o->x, o->y, o->height, (Spr*)(o->lpvData));
}

void DisplaySprite_Tile(int& x, int& y, LPCHARACTER ch, LPORDERTABLE o)//021108 lsw
{
#ifdef _DEBUG	
	if (tool_DrawMap) { return; }
#endif
	//o->show = 0;
	const int mx = o->x - Mapx;
	const int my = o->y - Mapy;
	const int front = ((LPTILE)o->lpvData)->front;

	if (Sou[front].xl == -1)
	{
		//		if( mx >= 0 && mx < SCREEN_WIDTH && my >= 0 && my < SCREEN_HEIGHT )
		::PutTile(mx, my, &Sou[front], Sou[front].img, g_DestBackBuf);
	}
	else
	{
		::PutCompressedImage(mx, my, &Sou[front]);
	}
}

void DisplaySprite_DestPoint(int& x, int& y, LPCHARACTER ch, LPORDERTABLE o)//021108 lsw
{
	//o->show = 0;
	if (o->rgb)
	{
		::PutCompressedImage(o->x - Mapx, o->y - Mapy, (Spr*)(o->lpvData));
	}
	else
	{
		::PutCompressedImageFX(o->x - Mapx, o->y - Mapy, (Spr*)(o->lpvData), o->alpha, 1);
	}
}

//////////////////////// 0502 lkh �߰� (8�� ����->Void & High Trans ����) ////////////////////////////
void DisplaySprite(void)
{
	int x = 0, y = 0;
	int count = 0;

	for (int i = 0; i < NewOrderC; i++)
	{
		LPCHARACTER	ch = (LPCHARACTER)neworder[i].lpvData;
		LPORDERTABLE o = &neworder[i];
		//HandleRunning(&connections);
		
		switch (o->wType)
		{
		case SPRITETYPE_CHARACTER:
		case SPRITETYPE_MONSTER:
		case SPRITETYPE_NPC:
		{
			g_SDLInfo.particle_effect_Render(ch);
			::DisplaySprite_Character(x, y, ch, o);//021108 lsw
			HandleRunning(&connections);
		}break;
		case SPRITETYPE_ALPHAOBJECT:
		case SPRITETYPE_OBJECT:
		{
			if (((LPMAPOBJECT)o->lpvData)->attr != OBJECTATTR_BACK_ABOVE)
				if (((LPMAPOBJECT)o->lpvData)->attr != OBJECTATTR_BACK_BELOW)
				{
					::DisplayMapObject((LPMAPOBJECT)o->lpvData);
				}
			//o->show = 0;
		}break;
		case SPRITETYPE_EFFECT_NORMAL:
		{
			if (system_info.counter)
			::DisplaySprite_Effect(x, y, ch, o);
		}break;
		case SPRITETYPE_ITEM:
		{//021108 lsw
			::DisplaySprite_Item(x, y, ch, o);
		}break;
		case	SPRITETYPE_TILE:
		{
			::DisplaySprite_Tile(x, y, ch, o);
		}break;
		case  SPRITETYPE_DEST_POINT:
		{
			::DisplaySprite_DestPoint(x, y, ch, o);
		}break;
		case SPRITETYPE_SKILLGAUGE:
		{
			//o->show = 0;
			LPSKILLGAUGE n = (LPSKILLGAUGE)o->lpvData;
			::DrawSkillGauge(n);
		}break;
		//////////////////////////// 0126 lkh ////////////////////////////
		case SPRITETYPE_CLOCKNIDDLE:
		{
			//o->show = 0;
		}break;
		default:
		{
			continue;
		}break;
		}
	}
}

void DisplaySpriteNo(void)
{
#ifdef _DEBUG // finito 060507
	int	i;
	int count = 0;

	if (GetSysInfo(SI_GAME_MAKE_MODE) == 0) return;

	for (i = 0; i < NewOrderC; i++)
	{
		LPCHARACTER		ch = (LPCHARACTER)neworder[i].lpvData;
		LPORDERTABLE o = &neworder[i];

		switch (neworder[i].wType)
		{
		case SPRITETYPE_CHARACTER:
		{
			if (ch->nk3)HprintBold(o->x - Mapx, o->y - Mapy - 60, RGB(0xff, 0, 0), 0, "      VY_NK:%2d", ch->nk3);
			if (ch->nk4)HprintBold(o->x - Mapx, o->y - Mapy - 45, RGB(0xff, 0, 0), 0, "      ZY_NK:%2d", ch->nk4);
			if (ch->nk6)HprintBold(o->x - Mapx, o->y - Mapy - 30, RGB(0xff, 0, 0), 0, "      YI_NK:%2d", ch->nk6);
		}break;
		case SPRITETYPE_MONSTER:
		{
			//HprintBold(o->x - Mapx, o->y - Mapy - 60, RGB(0, 0xff, 0), 0, "      SPRNO:%2d", ch->sprno);
			HprintBold(o->x - Mapx, o->y - Mapy - 45, RGB(0, 0xff, 0), 0, "      HP:%2d", ch->_HpMax);
			//(o->x - Mapx, o->y - Mapy - 30, RGB(0, 0xff, 0), 0, "      EVENTNO:%2d", ch->id-10000);
		}break;
		case SPRITETYPE_NPC:
		{
			HprintBold(o->x - Mapx, o->y - Mapy - 60, RGB(0, 0xff, 0), 0, "      SPRNO:%2d", ch->sprno);
			HprintBold(o->x - Mapx, o->y - Mapy - 45, RGB(0, 0xff, 0), 0, "      EVENTNO:%2d", ch->event_no);
		}break;
		}
		count++;
	}
#endif
}

void DisplaySkillCursor(void)
{
	int i;

	for (i = 0; i < g_OrderInfo.count; i++)
	{
		LPSKILLCURSOR sc = (LPSKILLCURSOR)g_OrderInfo.order[i].lpvData;
		LPORDERTABLE o = &g_OrderInfo.order[i];

		//if (o->show == 0) continue;
		
		switch (o->wType)
		{
		case SPRITETYPE_SKILLCURSOR:
		{
			HandleRunning(&connections);
			PutCompressedImage(o->x - Mapx, o->y - Mapy, sc->sp);
#ifdef _DEBUG
			if (GetSysInfo(SI_GAME_MAKE_MODE))
			{
				HprintBold(o->x - Mapx, o->y - Mapy, 0xffff, 0, "%d %d", sc->skillno, sc->cursorno);
			}
#endif
		}break;
		}
	}
}



void DisplaySpriteCeiling(void)
{
	//return;

	int	i;

	for (i = 0; i < g_OrderInfo.count; i++)
	{
		LPCHARACTER		ch = (LPCHARACTER)g_OrderInfo.order[i].lpvData;
		LPORDERTABLE o = &g_OrderInfo.order[i];
		
		//if (o->show == 0) continue;

		switch (o->wType)
		{
		case SPRITETYPE_EFFECT_CEILING:
		{
			switch (o->type)
			{
			case 1:
			case 2:
			case 3:
			case 4:
				PutCompressedImageFX(o->x - Mapx, o->y - o->height - Mapy, (Spr*)(o->lpvData), o->alpha, o->type);
				break;
			case 5:
				PutCompressedImage(o->x - Mapx, o->y - o->height - Mapy, (Spr*)(o->lpvData));
				break;
			case 6:
				PutCompressedImageBlendRGB(o->x - Mapx, o->y - o->height - Mapy, (Spr*)(o->lpvData), o->rgb);
				break;

			case 7:
				PutCompressedImageVoid(o->x - Mapx, o->y - o->height - Mapy, (Spr*)(o->lpvData));
				break;

			}
			break;
		}

		case SPRITETYPE_ON_THE_CEILING_CHAR:
		{

			if (tool_MultiChar)
			{
				int tx = ch->x;
				int ty = ch->y;
				ch->x = o->x;
				ch->y = o->y;
				if (TileMap[ch->x / TILE_SIZE][ch->y / TILE_SIZE].attr_dont == 0)
				{
					if (ch->sprno > 1)	h_DisplayNPC(ch, 1);
					else h_DisplayPC(ch, 1);
				}
				ch->x = tx;
				ch->y = ty;
			}
			else
			{
				if (ch->sprno > 1)	h_DisplayNPC(ch, 1);
				else h_DisplayPC(ch, 1);
			}

			//int x = ch->x - Mapx + 1;
			//int y = ch->y - Mapy - CharSpr[ch->sprno].sp[0][0].yl - 20;
			int x = ch->visual_x - Mapx + 1;
			int y = ch->visual_y - Mapy - CharSpr[ch->sprno].sp[0][0].yl - 20;

			//角色顯示頭像namedisplaycount
			if (ch->namedisplaycount > 5 && !ch->ChatDelay)
			{
				int tttx = spr[71].xl / 2; // �̸��۾��ڿ� ������ ���..
				if (ch->type == SPRITETYPE_CHARACTER) // PC�鸸 ������ �ִ�....
				{
					PutCharImage(x - tttx + -20, y - 24 - ch->height, ch->face, 0, FS_MICRO, 0, ch->sex);
				}
				if (ch->name[0])
				{

					if (ch->HostName[0]) //  Tamming�� Character.
					{
						HprintBold(x - tttx + 10, y - 4 - ch->height, RGB(100, 100, 0xff), 0, "%s(%s)", ch->name, ch->HostName);
						
						/*if (strcmp(ch->HostName, Hero->HostName) == 0)
						{
							//  �� ������ ���� ������ �����ش�. 
						}*/

					}
					else
					{
						HprintBold(x - tttx + 10, y - 4 - ch->height, RGB(0xff, 0xff, 0xff), 0, "%s", ch->name);
					}
				}
#ifdef _DEBUG
				if (GetSysInfo(SI_GAME_MAKE_MODE))
				{
					HprintBold(x - tttx - 70, y - 4 - ch->height, RGB(0xff, 0xff, 0xff), 0, "Sprno:%d", ch->sprno);
				}
#endif
			}
			else
				if (ch->smiledelay)
				{
					PutCompressedImage(x - 16, y, &spr[267 + ch->smileno]);
				}

			//o->show = 0;
			break;
		}
		}
	}
}



void DisplaySpriteTrans(void)
{
#ifndef _DEBUG
	return;
#endif
	int	i;
	LPORDERTABLE o;


	for (i = 0; i < g_OrderInfo.count; i++)
	{
		if (0)
			if (GetSysInfo(SI_GAME_MAKE_MODE))
			{
				switch (g_OrderInfo.order[i].wType)
				{
				case	SPRITETYPE_CHARACTER:
				case	SPRITETYPE_MONSTER:
				case	SPRITETYPE_NPC:
				{
					LPCHARACTER		ch = (LPCHARACTER)g_OrderInfo.order[i].lpvData;
					Box_(ch->x - ch->sp->ox, ch->y - ch->sp->oy, ch->sp->xl, ch->sp->yl, RGB16(0, 255, 0));
					break;
				}
				//				case SPRITETYPE_ALPHAOBJECT :
				//				case SPRITETYPE_OBJECT :
				//				{	
				//					DisplayMapObject(( LPMAPOBJECT )o->lpvData );
				//				}	
				case	SPRITETYPE_TILE:
				{
					Box_(o->x, o->y, TILE_SIZE, TILE_SIZE, RGB16(255, 255, 0));
					break;
				}
				case  SPRITETYPE_DEST_POINT_BOTTOM:
				{
					Spr* sp = (Spr*)(o->lpvData);
					Box_(o->x - sp->ox, o->y - sp->oy, sp->xl, sp->yl, RGB16(255, 255, 255));
					break;
				}
				case SPRITETYPE_ITEM:
				case SPRITETYPE_ITEM_BOTTOM:
				{
					LPITEMGROUND  item = (LPITEMGROUND)o->lpvData;
					Spr* sp = item->spr[item->curframe / 2];
					Box_(o->x - sp->ox + item->offx, o->y - sp->oy + item->offy, sp->xl, sp->yl, RGB16(255, 255, 255));

					Box_(o->x - sp->ox + item->offx, o->y - sp->oy + item->offy, sp->xl, sp->yl, RGB16(255, 255, 255));
					break;
				}
				case SPRITETYPE_EFFECT_CEILING:
				case SPRITETYPE_EFFECT_NORMAL:
				case SPRITETYPE_EFFECT_BOTTOM:
				{
					Spr* sp = (Spr*)(o->lpvData);
					Box_(o->x - sp->ox, o->y - sp->oy, sp->xl, sp->yl, RGB16(0, 0, 255));
					break;
				}
				}
				Hcolor(255, 0, 0);
				Hprint2(o->x - Mapx, o->y - Mapy, g_DestBackBuf, "%d", o->rank);
				Hcolor(255, 255, 255);
				Hprint2(o->x - Mapx, o->y - Mapy, g_DestBackBuf, "%d", o->rank);
			}

		switch (g_OrderInfo.order[i].wType)
		{
		case	SPRITETYPE_CHARACTER:
		case	SPRITETYPE_MONSTER:
		case	SPRITETYPE_NPC:
		{
			LPCHARACTER		ch = (LPCHARACTER)g_OrderInfo.order[i].lpvData;
			switch (ch->sprno)
			{
			case 0:
			case 1: h_DisplayPC2(ch);	break;
			}
			break;
		}
		}
	}
}

void _DisplayMapObjectAfter(void)
{
#ifdef _DEBUG // finito 060507
	int i;

	if (!GetSysInfo(SI_GAME_MAKE_MODE)) return;


	for (i = 0; i < g_OrderInfo.count; i++)
	{
		LPORDERTABLE o = &g_OrderInfo.order[i];
		LPMAPOBJECT mo = (LPMAPOBJECT)o->lpvData;
		switch (o->wType)
		{
		case SPRITETYPE_ALPHAOBJECT:
		case	SPRITETYPE_OBJECT:
		{
			DisplayMapObjectAfter(mo, IDCheckedMapObject == (int)o->rgb);


			switch (mo->objectoritem)
			{
				//  ������۾���..
			case 71:		break;//010215 lsw
			case 72:HprintBold(o->x - Mapx, o->y - Mapy + 12, RGB(255, 255, 255), RGB16(0, 0, 0), lan->OutputMessage(9, 12));		break;//
			case 73:HprintBold(o->x - Mapx, o->y - Mapy + 12, RGB(255, 255, 255), RGB16(0, 0, 0), lan->OutputMessage(9, 13));		break;//
			case 74:HprintBold(o->x - Mapx, o->y - Mapy + 12, RGB(255, 255, 255), RGB16(0, 0, 0), lan->OutputMessage(9, 14));		break;//
			case 75:HprintBold(o->x - Mapx, o->y - Mapy + 12, RGB(255, 255, 255), RGB16(0, 0, 0), lan->OutputMessage(9, 15));		break;//
			case 76:HprintBold(o->x - Mapx, o->y - Mapy + 12, RGB(255, 255, 255), RGB16(0, 0, 0), lan->OutputMessage(9, 16));	break;//
			case 77:HprintBold(o->x - Mapx, o->y - Mapy + 12, RGB(255, 255, 255), RGB16(0, 0, 0), lan->OutputMessage(9, 17));		break;//
			case 78:HprintBold(o->x - Mapx, o->y - Mapy + 12, RGB(255, 255, 255), RGB16(0, 0, 0), lan->OutputMessage(9, 18));		break;//010215 lsw
				break;
			}

			if (mo->objectoritem == 0) // mapobject
			{
			}
			else
			{
				HprintBold(o->x - Mapx, o->y - Mapy, RGB(255, 255, 255), RGB16(0, 0, 0), "Item:(%d)", mo->dum);
			}
			break;
		}
		}
	}
#endif
}


int	 DestPointX, DestPointY;
int	 DestPointAni;
char DestPointOutFlag;

void SettingDestPoint(int x, int y)
{
	DestPointX = x, DestPointY = y;
	DestPointAni = 22;
	DestPointOutFlag = 0;
	CursorNo(3);
}


void UnDisplayDestPoing(LPCHARACTER ch)
{
	if (ch == Hero)  DestPointOutFlag = 1;
}


void PutDestPoint(void)
{

	int  sp;

	if (DestPointAni == 0) return;

	switch (DestPointAni)
	{
	case 22: sp = 957; break;
	case 21: sp = 957; break;
	case 20: sp = 957; break;
	case 19: sp = 957; break;
	case 18: sp = 957; break;
	case 17: sp = 957; break;
	case 16: sp = 958; break;
	case 15: sp = 959; break;
	case 14: sp = 960; break;
	case 13: sp = 961; break;
		//	case 12 : sp = 962; break;
		//	case 11 : sp = 963; break;
	default: sp = 962; break;
	}

	PutCompressedImage(DestPointX - Mapx, DestPointY - Mapy, &spr[sp]);

	if (DestPointOutFlag == 0 && DestPointAni > 11)	DestPointAni--;
	if (DestPointOutFlag == 1) DestPointAni--;
}



extern void GetRealTime(int& mon, int& day, int& hour);
// 010724 KHS	
static 	char nMapBGM[MAX_MAP_] = { 3,6,2,5,0, 4,6,2,2,5,
									5,4,4,0,0, 0,0,0,3,3,
									4,2,0,0,6, 6,4,0,2,2,
									3,3,3,3,0, 3,3,4,4,0,
									4,4,0,0,0, 5,0,0,0,0,
									0,0,0,0,0, 0,0,0,0,4,
									5,3,3,0,3, 0,0,0,0, };

//021030 YGI
int	ReturnBGMNumber(int map_Number)
{
	if (map_Number < 0) return 1;
	int sound = g_Menu_Variable.GetEventSound();
	if (sound > 0) return sound;
	
	return nMapBGM[map_Number];
}	// 010724 KHS	
//021030 YGI
void BackgroundMusic()
{
	g_audioManager.StopAllSounds();
	int return_BGM = 0;
	return_BGM = ReturnBGMNumber(MapNumber);
	if (return_BGM)
	{
		g_audioManager.PlaySoundA(return_BGM, system_info.music, LOOP);
	}
}

void SendIsThereCharName(char* name)
{
	t_packet p;
	p.h.header.type = CMD_ISTHERE_CHARNAME;
	strcpy(p.u.client_isthere_charname.name, name);
	p.u.client_isthere_charname.name[19] = '\0';
	p.h.header.size = sizeof(t_client_isthere_charname) - NM_LENGTH + strlen(name);
	QueuePacket(&p, 1);
}

//   Create Character �Ҷ�,  �̸��� �����ϴ��� �˾ƺ���. 

//   name�� DB�� �����ϸ� 1�� �����Ѵ�. 
//   name�� DB�� �������� ������ 0�� �����Ѵ�. 
bool CheckCharName(char* name)
{
	if (NULL == name)
	{
		return false;
	}

#ifdef 	USA_LOCALIZING_
	/*if (!::IsASCII(name))
	{
		return false;
	}
	
	std::string strName(name);
    if (containsNonAscii(strName)) {
		return false; // 如果包含非 ASCII 字元，返回 false
    }*/
#endif
	
	if (lan->contains_emoji_in_name(name))
	{
		return false;
	}

	// 2. 禁止全形空格 (UTF-8 3-byte sequence: E3 80 80)
	// 警告: 確保您的原始碼檔案是以 UTF-8 編碼儲存，否則可能會出錯。
	const std::string fullWidthSpace = "　";
	std::string str_name = name;
	if (str_name.find(fullWidthSpace) != std::string::npos) {
		//std::cout << "錯誤：包含全形空格\n";
		return false;
	}

	// 3. 禁止半形空格 (如果也需要的話)
	if (str_name.find(' ') != std::string::npos) {
		//std::cout << "錯誤：包含半形空格\n";
		return false;
	}

	if (strlen(name) >= 15) {
		return false;
	}

	SendIsThereCharName(name);

	DWORD StartTime = SDL_GetTicks();
	ListenCommand = CMD_NONE;
	while (1)
	{
		DWORD Duration = SDL_GetTicks() - StartTime;	// 5�ʸ� ��ٸ���. 
		if (Duration > WAIT_TIME_RESPONSE_DURATION)
		{
			return false;
		}

		if (HandleRunning(&connections) <= 0)
		{
			Release(&connections);
			return false;
		}
		else if (ListenCommand == CMD_THEREIS_NO_CHARNAME)
		{
			return true;
		}
		else if (ListenCommand == CMD_THEREIS_CHARNAME)
		{
			return false;
		}
	}
}

void SendDebugReqCharInfo(int id)
{
	t_packet p;

	p.h.header.type = CMD_DEBUG_REQ_CHAR_INFO;
	p.u.debug_client_req_char_info.id = id;
	p.h.header.size = sizeof(t_debug_client_req_char_info);

	QueuePacket(&p, 1);
}

void RecvDebugReqCharInfo(t_debug_server_req_char_info* p)
{
	LPCHARACTER ch = ReturnCharacterPoint(p->id);
	if (ch == NULL) return;

	ch->_Hp = p->Hp;
	ch->_HpMax = p->HpMax;
	ch->event_no = p->event_no;	// 0131 YGI
	ch->_lev = p->lv;
	ch->viewdebugcharinfo = 75;
}

void RecvPleaseGiveLife(int id, int hp, int mana, int hungryp, int who)
{
	LPCHARACTER ch = ReturnCharacterPoint(id);
	if (ch == NULL) return;

	// Healingȿ��......

//	if( ch->type != SPRITETYPE_MONSTER ) 
//		if( ch->viewtype != VIEWTYPE_GHOST_  && ch->hp != 0 ) return;

	if (ch == Hero)
	{
		SCharacterData.nCharacterHP = hp;
		SCharacterData.nCharacterMP = mana;
		SCharacterData.nCharacterSP = hungryp;
		SCharacterData.condition = CON_NORMAL;
		// ��Ȱ �� ������ų �ʱ�ȭ
		iCSPCMax = iCSPCNow = 0; // 030226 
		CSDMainIconClear(); // 030428 kyo

		Hero->hp = SCharacterData.nCharacterHP; //Eleval 18/08/09 - HP BUG
	}
	//< CSD-020826
	ch->hp = hp;
	ch->condition = CON_NORMAL;
	ch->blood_Set = false;
	ch->viewtype = VIEWTYPE_NORMAL_;
	ch->dieani = 0;
	ch->peacests = 0;
	ch->dummy_pk = 0;
	ch->gox = ch->x;
	ch->goy = ch->y;

	//> CSD-020826
	EndCastMagic(ch);
	SettingBasicActionSub(ch);



	if (ch == Hero)
	{
		PlayListAutoSounds(2408, 0, 0, 0);	// LTS SOUND

		//<soto-030627
		g_mgrBattle.ClearDelay(); // CSD-TW-030624
		g_mgrBattle.ClearSwitchDelay(); // CSD-TW-030624
		//>soto-030627

	}
	else
	{
		int distx = ch->x - Hero->x;
		int disty = ch->y - Hero->y;
		PlayListAutoSounds(2408, distx, disty, 0);
	}

	InsertMagic(ch, ch, 160, 0, 0, 0, ch->x, ch->y);
	if (ch->sealstone)
	{
		InsertMagic(ch, ch, 160, 0, 0, 0, ch->x + (rand() % 100) - 50, ch->y + (rand() % 100) - 50);
		InsertMagic(ch, ch, 160, 0, 0, 0, ch->x + (rand() % 100) - 50, ch->y + (rand() % 100) - 50);
		InsertMagic(ch, ch, 160, 0, 0, 0, ch->x + (rand() % 100) - 50, ch->y + (rand() % 100) - 50);
		InsertMagic(ch, ch, 160, 0, 0, 0, ch->x + (rand() % 100) - 50, ch->y + (rand() % 100) - 50);
		InsertMagic(ch, ch, 160, 0, 0, 0, ch->x + (rand() % 100) - 50, ch->y + (rand() % 100) - 50);
		InsertMagic(ch, ch, 160, 0, 0, 0, ch->x + (rand() % 100) - 50, ch->y + (rand() % 100) - 50);
		InsertMagic(ch, ch, 160, 0, 0, 0, ch->x + (rand() % 100) - 50, ch->y + (rand() % 100) - 50);
		InsertMagic(ch, ch, 160, 0, 0, 0, ch->x + (rand() % 100) - 50, ch->y + (rand() % 100) - 50);
		InsertMagic(ch, ch, 160, 0, 0, 0, ch->x + (rand() % 100) - 50, ch->y + (rand() % 100) - 50);

		return;
	}

	if (ch->changeActionFlag == 1)		ch->changeActionFlag = 0;
	CharDoAni(ch, ch->direction, ch->basicAction);

	LPCHARACTER whoch = ReturnCharacterPoint(who);
	if (whoch == NULL) return;
	if (ch == Hero)
	{
		AddCurrentStatusMessage(250, 200, 200, lan->OutputMessage(9, 21), whoch->name);//010215 lsw
	}
	else if (whoch != Hero)
	{

		AddCurrentStatusMessage(0, 200, 200, lan->OutputMessage(9, 22), whoch->name, ch->name);//010215 lsw
	}
}






/////////////////////////// 0206 lkh ���� ���� ///////////////////////////////////
bool	ReturnCheckObject(LPMAPOBJECT		mo)
{
	static int			buttonOnCount;
	static int			selectObject = 0;				//���콺�� ���� üũ�� ������Ʈ ��ȣ

	static int			selectObject_X;					//���õ� ������Ʈ�� X��
	static int			selectObject_Y;					//���õ� ������Ʈ�� Y��

	static int			tempObject = 0;				//������ ���õ� ������Ʈ ���
	static int			objectType = 0;				//üũ�� ������Ʈ�� ����(5:����/6:����ǥ)
	static int			displayBoard = 0;			//���� �������� �ִ� ���ǹ�ȣ
	static int			displayDirect = 0;			//���� �������� �ִ� ����ǥ��ȣ	

	char				str_Data[80];

	DIRECTBOARDDATA		directboardData;
	int					onCount = 15;				//1��

	if (g_MouseInMenu)  return 0;

	//WORD  objectoritem;				//object Type(0~65535)

	//////////////////////////////// 0420 lkh �߰� (�޴��� ���� ���콺�� �޴����� �ִ� ���� üũ���� ����) 
	Spr* sp = &MoImage[mo->id - 1].spr[0];
	if (BoxAndDotCrash((int)(mo->x + mo->offx - sp->ox), (int)(mo->y + mo->offy - sp->oy), sp->xl, sp->yl, Mox, Moy))
	{
		selectObject = mo->dum;
		selectObject_X = mo->x;
		selectObject_Y = mo->y;
		objectType = mo->objectoritem;

		if (!IsDead(Hero))
			if (g_nLButtonState == STATE_BUTTON_PRESSED)
			{
				switch (objectType)
				{
				case ITEMTYPE_SEAL_NOR:
					break;
				case ITEMTYPE_SEAL_SPC:
					break;
				case ITEMTYPE_HP_UP:
					break;
				case ITEMTYPE_MP_UP:
					break;
				case ITEMTYPE_NATION_STONE:	if (MapInfo[MapNumber].nation == Hero->name_status.nation)		// 1004 YGI---
				{
					MP3(SN_OPEN_NATION_BBS, 1);				// 1027 YGI
					CallSmallMenu(MN_NATION_BBS);
				}
										  break;
				case ITEMTYPE_GUILD_STONE:
					break;
				case ITEMTYPE_SENARIO_STONE:	CallSmallMenu(MN_SCENARIO); break;
				case ITEMTYPE_SCALE:	CallSmallMenu(MN_SCALE); break;

				default:
					if ((mo->id >= 336 && mo->id <= 337) || (mo->id >= 2482 && mo->id <= 2485))
					{
						CallSmallMenu(MN_HUNTER_MAIN);
					}
					break;
				}
			}


		if ((mo->objectoritem == ITEMTYPE_SIGNBOARD || mo->objectoritem == ITEMTYPE_DIRECTBOARD) && !g_MouseInMenu)
		{
			buttonOnCount++;
			if (buttonOnCount >= onCount + 10)	buttonOnCount = onCount + 10;
			if (buttonOnCount == onCount || (buttonOnCount >= onCount && selectObject != tempObject))
			{
				if (objectType == ITEMTYPE_SIGNBOARD)
				{
					g_BoadAndSignNumber = mo->dum; // CSD-030324 // thai2 YGI

					if (GetBoardData(mo->dum, str_Data) == true)
					{
						displayBoard = mo->dum;
						PutBoardMenu(mo->dum, str_Data);
						tempObject = selectObject;
						SMenu[MN_DISTANCE_TABLE].bActive = false;
						displayDirect = 0;
					}
					return true;
				}
				else if (objectType == ITEMTYPE_DIRECTBOARD)
				{
					g_BoadAndSignNumber = mo->dum + 10000; // CSD-030324 // thai2 YGI

					if (GetDirectBoardData(mo->dum, &directboardData) == true)
					{
						displayDirect = mo->dum;
						PutDirectBoardMenu(&directboardData);
						tempObject = selectObject;
						SMenu[MN_STORE_SIGN].bActive = false;
						displayBoard = 0;
					}
					return true;
				}

			}
		}
		else
		{
			if ((selectObject == mo->dum && objectType == mo->objectoritem && (selectObject_X == mo->x && selectObject_Y == mo->y)) || (selectObject == 0))
			{
				buttonOnCount--;
				if (buttonOnCount < 0)		buttonOnCount = 0;
				selectObject = 0;
				selectObject_X = 0;
				selectObject_Y = 0;
				tempObject = 0;
				objectType = 0;

				if (buttonOnCount == onCount)
				{
					if (displayBoard != 0)
					{
						SMenu[MN_STORE_SIGN].bActive = false;
						displayBoard = 0;
					}
					if (displayDirect != 0)
					{
						SMenu[MN_DISTANCE_TABLE].bActive = false;
						displayDirect = 0;
					}
				}
			}
		}
	}

	/*
	else		//�ٸ� Ÿ���� ������Ʈ�� �������� ��
	{
		Spr *sp = &MoImage[ mo->id-1].spr[ mo->curframe ];
		if( BoxAndDotCrash( (int)(mo->x+mo->offx-sp->ox), (int)(mo->y+mo->offy-sp->oy), sp->xl, sp->yl, Mox, Moy) )
		{
			buttonOnCount--;
			selectObject=0;
			objectType=0;

			if(buttonOnCount<0)		buttonOnCount=0;
			if(buttonOnCount==onCount)
			{
				if(displayBoard != 0)
				{
					SMenu[MN_STORE_SIGN].bActive = false;
					displayBoard = 0;
				}
				if(displayDirect !=0 )
				{
					SMenu[MN_DISTANCE_TABLE].bActive = false;
					displayDirect = 0;
				}
			}
		}
	}
	*/
	return false;
}

// thai2 YGI
extern bool CheckContinueLine(char check);
bool	GetBoardData(int board_Number, char* str_Data)
{	//< CSD-030324
	char* token;

	char		str_boardData[100];	//�ѱ� 50��/���� 100�ڸ� ����� ����
	BOARDDATA	boardData;
	FILE* fp;
	char		DataPath[MAX_PATH];

	sprintf(DataPath, "./map/%s.bsd", MapName);

	fp = Fopen(DataPath, "rt");			//read only+text file
	if (fp == NULL)	return false;

	while (!feof(fp))
	{
		fgets(str_boardData, 99, fp);
		if (CheckContinueLine(str_boardData[0])) continue;

		token = strtok(str_boardData, "\t\n");
		if (!token) continue;

		boardData.boardNumber = atoi(token);
		token = strtok(NULL, "\t\n");
		if (token)  strcpy(boardData.str_boardData, token);

		if (boardData.boardNumber == board_Number)
		{
			strcpy(str_Data, boardData.str_boardData);
			fclose(fp);
			return true;
		}
		//return	boardData.str_boardData;
	}
	fclose(fp);
	return	false;
}	//> CSD-030324

// thai2 YGI
bool	GetDirectBoardData(int board_Number, DIRECTBOARDDATA* directboardData)
{	//< CSD-030324
	char* token;

	char		str_boardData[160];	//�ѱ� 50��/���� 100�ڸ� ����� ����
	//DIRECTBOARDDATA		directboardData;
	FILE* fp;
	char		DataPath[MAX_PATH];

	sprintf(DataPath, "./map/%s.dsd", MapName);

	fp = Fopen(DataPath, "rt");			//read only+text file
	if (fp == NULL)	return false;

	while (!feof(fp))
	{
		fgets(str_boardData, 159, fp);

		if (CheckContinueLine(str_boardData[0])) continue;
		token = strtok(str_boardData, "\t\n");
		if (!token) continue;

		directboardData->boardNumber = atoi(token);

		token = strtok(NULL, "\t\n");
		strcpy(directboardData->str_Title, token);
		if (!strncmp(directboardData->str_Title, ".", 1))	memset(directboardData->str_Title, 0, sizeof(char[30]));

		token = strtok(NULL, "\t\n");
		strcpy(directboardData->str_East, token);
		if (!strncmp(directboardData->str_East, ".", 1))		memset(directboardData->str_East, 0, sizeof(char[30]));

		token = strtok(NULL, "\t\n");
		strcpy(directboardData->str_South, token);
		if (!strncmp(directboardData->str_South, ".", 1))	memset(directboardData->str_South, 0, sizeof(char[30]));

		token = strtok(NULL, "\t\n");
		strcpy(directboardData->str_West, token);
		if (!strncmp(directboardData->str_West, ".", 1))		memset(directboardData->str_West, 0, sizeof(char[30]));

		token = strtok(NULL, "\t\n");
		strcpy(directboardData->str_North, token);
		if (!strncmp(directboardData->str_North, ".", 1))	memset(directboardData->str_North, 0, sizeof(char[30]));

		if (directboardData->boardNumber == board_Number)
		{
			fclose(fp);
			return true;
		}
	}

	fclose(fp);
	return	false;
}	//> CSD-030324

////////////////////////////////// ������� ////////////////////////////////////////

extern bool isInWarMap(); //Eleval 15/08/09

bool IsHidenGhost(LPCHARACTER ch)		// ���� ���� ���Ʈ �ΰ�??//��� ���ǵ帮�� 
{
	if (Hero->IsCounselor() || Hero->IsReporter()) return false;
	if ((ch->IsCounselor() || ch->IsReporter()) && ch->viewtype == VIEWTYPE_GHOST_) return true;
	if (ch->accessory[0] == 114) return true;
	if (g_LocalWarBegin && isInWarMap()) //Eleval 15/08/09 - So players from X Nation can't see ghosts from Y Nation
	{
		if (Hero->JoinLocalWar && ch->JoinLocalWar)
		{
			if (Hero->name_status.nation == ch->name_status.nation)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	}
	return false;
}

