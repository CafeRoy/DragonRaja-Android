#include "stdafx.h"
#include "gameproc.h"
#include "NetWork.h"
#include "NationSys.h"
#include "menu.h"
#include "SmallMenu.h"
#include "map.h"
#include "LineCommand.h"
#include "DirectSound.h"
#include "tool.h"
#include "hong_sub.h"
#include "NATIONSUPPORT.H"//010910 LSW
#include "DualMgr.h"
extern void RecvCMD_SQUAD_CHAT(t_packet* p);
// Gloabl Variable
cNation* g_pNation = NULL;
CDontManager* g_pDontManager = NULL;
int g_Squad = 1;						// ���߿� Ŭ������ �����Ѵ�.	//�δ��������̽� ���� �ϱ����� ������
int g_Commander = 0;					// ��ɰ� �������̽� �����ϱ����� ������

int g_AttackNation = 0;				// ���ݱ� ������ȣ	// �����Ϳ� ������ ��Ʈ�ȴ�
int g_DefenceNation = 0;				// �� ������ȣ 

int g_ObjectWarfieldNo = 0;			// 2-4�Ⱓ(��ǥ->����) ������ ��� ������ ��ȣ
int g_ObjectWarfieldStatus = 0;		// �ش� �������� ���� 

int g_CommanderCandidaterLevel = 0;   //���߿� �ٲ۴�..
bool g_bWarBegin = false;				// RecvCMD_WAR_START,END���� ��� 
bool g_bNewWarBegin = false;

int g_SquadLoadingPoint;			//�ε�����Ʈ ��ȣ	// ����޴����� ����ϴ� �ӽ� ����� ����Ÿ
int g_SquadNum;						//�δ�޴�			// ��������	

//< LTH-040314-KO ���� ������ ���� �������̽��� ���� ����
DWORD g_dwRemainTime = 0;				// ���� ���� �ð�
int g_aRemainGuard[2][4] = { 0, };		// ������ ���� ��輮 �� ����. �ΰ��� ��. 0:������ȣ�� 1:������ȣ�� 2:���� 3:�ܼ���ȣ��
//> LTH-040314-KO

extern int g_iScenarioOn;	// BBD 040315

extern SHN SHideNomal[MAX_SHN][MAX_SHN_FIELD];//011004 lsw

// Tile Concern__________________________________________________________________________________	// 010906 LTS
const POINTS		Map0_133[MAP0_133] = {		// 010927 LTS
									{131,259},{131,260},{131,261},{131,262},{131,263},{131,264},{131,265},
									{131,266},{132,258},{132,259},{132,260},{132,261},{132,262},{132,263},
									{132,264},{132,265},{133,257},{133,258},{133,259},{133,260},{133,261},
									{133,262},{133,263},{133,264},{134,256},{134,257},{134,258},{134,259},
									{134,260},{134,261},{134,262},{134,263} };

const POINTS		Map0_138[MAP0_138] = {
									{135,255},{135,256},{135,257},{135,258},{135,259},{135,260},{135,261},
									{135,262},{136,255},{136,256},{136,257},{136,258},{136,259},{136,260},
									{136,261},{137,256},{137,257},{137,258},{137,259},{137,260},{138,257},
									{138,258},{138,259},{139,258} };

const POINTS		Map0_203[MAP0_203] = {
									{200,192},{200,193},{200,194},{200,195},{200,196},{200,197},{200,198},
									{200,199},{201,192},{201,193},{201,194},{201,195},{201,196},{201,197},
									{201,198},{202,193},{202,194},{202,195},{202,196},{202,197},{203,194},
									{203,195},{203,196},{204,195} };

const POINTS		Map0_198[MAP0_198] = {
									{199,193},{199,194},{199,195},{199,196},{199,197},{199,198},{199,199},
									{199,200},{198,194},{198,195},{198,196},{198,197},{198,198},{198,199},
									{198,200},{198,201},{197,195},{197,196},{197,197},{197,198},{197,199},
									{197,200},{197,201},{197,202},{196,196},{196,197},{196,198},{196,199},
									{196,200},{196,201},{196,202},{196,203},{195,196},{195,197} };

const POINTS		Map0_263[MAP0_263] = {
									{261,133},{261,134},{261,135},{261,136},{261,137},{261,138},{261,139},
									{261,140},{262,132},{262,133},{262,134},{262,135},{262,136},{262,137},
									{262,138},{262,139},{263,131},{263,132},{263,133},{263,134},{263,135},
									{263,136},{263,137},{263,138},{264,130},{264,131},{264,132},{264,133},
									{264,134},{264,135},{264,136},{264,137} };

const POINTS		Map0_268[MAP0_268] = {	// 010927 LTS
									{265,129},{265,130},{265,131},{265,132},{265,133},{265,134},{265,135},
									{265,136},{266,129},{266,130},{266,131},{266,132},{266,133},{266,134},
									{266,135},{267,130},{267,131},{267,132},{267,133},{267,134},{268,131},
									{268,132},{268,133} };
//________________________________________________________________________________________________	// 010906 LTS
//________________________________________________________________________________________________	// 010915 LTS

const POINTS		Map1_450[MAP1_450] =
{ {451,521},{451,522},{451,523},{451,524},{451,525},{451,526},
	{450,522},{450,523},{450,524},{450,525},{450,526},{450,527},
	{449,523},{449,524},{449,525},{449,526},{449,527},{449,528},
	{448,524},{448,525},{448,526},{448,527},{448,528},{448,529},
	{447,525},{447,526},{447,527},{447,528},{447,529},{446,526},
	{446,527},{446,527} };

const POINTS		Map1_527[MAP1_527] =		// 010927 LTS
{ {528,525},{528,526},{528,527},{528,528},{528,529},{528,530},
	{527,526},{527,527},{527,528},{527,529},{527,530},{527,531},
	{526,527},{526,528},{526,529},{526,530},{526,531},{526,532},
	{525,528},{525,529},{525,530},{525,531},{525,532},{525,533},
	{524,529},{524,530},{524,531},{524,532},{524,533},{523,530},
	{523,531},{523,532} };

const POINTS		Map1_524[MAP1_524] =
{ {525,451},{525,452},{525,453},{525,454},{525,455},{525,456},
	{524,452},{524,453},{524,454},{524,455},{524,456},{524,457},
	{523,453},{523,454},{523,455},{523,456},{523,457},{523,458},
	{522,454},{522,455},{522,456},{522,457},{522,458},{522,459},
	{521,455},{521,456},{521,457},{521,458},{521,459},{520,456},
	{520,457},{520,458} };

const POINTS		Map1_E450[15] =
{ {451,523},{451,524},{451,525},{450,524},{450,525},{450,526},
	{449,525},{449,526},{449,527},{448,526},{448,527},{448,528},
	{447,527},{447,528},{447,529} };
const POINTS		Map1_E527[15] =
{ {528,527},{528,528},{528,529},{527,528},{527,529},{527,530},
	{526,529},{526,530},{526,531},{525,530},{525,531},{525,532},
	{524,531},{524,532},{524,533} };
const POINTS		Map1_E524[15] =
{ {525,453},{525,454},{525,455},{524,454},{524,455},{524,456},
	{523,455},{523,456},{523,457},{522,456},{522,457},{522,458},
	{521,457},{521,458},{521,459} };
//________________________________________________________________________________________________	// 011015 LTS
const POINTS		Map2_114[MAP2_114] =
{
	{112,165},{113,164},{114,163},{112,164},{113,163},{114,162},
	{112,163},{113,162},{114,161},{112,162},{113,161},{114,160},
	{112,161},{113,160},{114,159},{111,161},{112,160},{113,159},
	{114,158},{111,160},{112,159},{113,158},{114,157},{110,160},
	{111,159},{112,158},{113,157},{114,156} };

const POINTS		Map2_119[MAP2_119] =
{
	{115,162},{116,161},{117,160},{118,159},{119,158},{115,161},
	{116,160},{117,159},{118,158},{115,160},{116,159},{117,158},
	{118,157},{115,159},{116,158},{117,157},{118,156},{115,158},
	{116,157},{117,156},{118,155},{115,157},{116,156},{117,155},
	{118,154},{115,156},{116,155},{117,154},{118,153},{115,155},
	{116,154},{117,153} };


const POINTS		Map2_151[MAP2_151] =
{
	{149,130},{150,129},{151,128},{152,127},{149,129},{150,128},
	{151,127},{152,126},{149,128},{150,127},{151,126},{152,125},
	{149,127},{150,126},{151,125},{152,124},{149,126},{150,125},
	{151,124},{152,123},{149,125},{150,124},{151,123},{152,122},
	{149,124},{150,123},{151,122},{152,121},{148,124},{149,123},
	{150,122},{151,121},{152,120} };

const POINTS		Map2_156[MAP2_156] =
{
	{153,126},{154,125},{155,124},{156,123},{153,125},{154,124},
	{155,123},{156,122},{153,124},{154,123},{155,122},{153,123},
	{154,122},{155,121},{153,122},{154,121},{155,120},{153,121},
	{154,120},{155,119},{153,120},{154,119},{155,118},{153,119},
	{154,118},{155,117} };


const POINTS		Map2_274[MAP2_274] =
{
	{272,286},{273,285},{274,284},{275,283},{272,285},{273,284},
	{274,283},{275,282},{272,284},{273,283},{274,282},{275,281},
	{272,283},{273,282},{274,281},{275,280},{272,282},{273,281},
	{274,280},{275,279},{271,282},{272,281},{273,280},{274,279},
	{275,278},{271,281},{272,280},{273,279},{274,278},{275,277},
	{270,281},{271,280},{272,279},{273,278},{274,277},{275,276} };

const POINTS		Map2_279[MAP2_279] =
{
	{276,282},{277,281},{278,280},{279,279},{276,281},{277,280},
	{278,279},{279,278},{276,280},{277,279},{278,278},{276,279},
	{277,278},{278,277},{276,278},{277,277},{276,277},{277,276},
	{276,276},{276,275} };



const POINTS		Map2_E114[8] =
{ {112,165},{113,164},{114,163},{115,162},{112,164},{113,163},
	{114,162},{115,161} };

const POINTS		Map2_E119[8] =
{ {116,161},{117,160},{118,159},{119,158},{116,160},{117,159},
	{118,158},{119,157} };
const POINTS		Map2_E151[8] =
{ {149,129},{150,128},{151,127},{152,126},{149,128},{150,127},
	{151,126},{152,125} };

const POINTS		Map2_E156[9] =
{ {153,125},{154,124},{155,123},{156,122},{157,121},{153,124},
	{154,123},{155,122},{156,121} };

const POINTS		Map2_E274[8] =
{ {272,285},{273,284},{274,283},{275,282},{272,284},{273,283},
	{274,282},{275,281} };

const POINTS		Map2_E279[8] = { {276,281},{277,280},{278,279},{279,278},{276,280},{277,279},
									{278,278},{279,277} };
//________________________________________________________________________________________________	// 011015 LTS



// Extern Variable Definition
extern SMENU SMenu[MAX_MENU];
extern DEVELOPE_ENVIR	SysInfo;


// Extern Function Definition
extern void ShowComeOutLocalWarCast(k_bbs_all* p);
extern void			ShowComeOutGM(char* name);
extern void			AddCurrentStatusMessage(const int R, const int G, const int B, char* msg, ...);
extern void			LoadHeroClothAccessoryDataOne(int sprno, int no);
extern void			FreeHeroClothAccessoryDataOne(int sprno, int no);
extern void			InsertMagic(LPCHARACTER lpChar_Own, LPCHARACTER lpChar_Target, int magicNum, int magicSubNum, int s_X, int s_Y, int t_X, int t_Y, bool firePattern = 0, int next_Effect = 0, bool failed = 0, DWORD dwDuration = 0);
extern LPCHARACTER	ExistHe(const char* name);		// �� �þ߿� �����ϴ� ������ �˾ƺ��� ��ƾ
extern int			convert565to555(Spr* sp);
extern int			convert565to555_LoadSprite(Spr* sp);
extern void			PutCompressedImageFX(int x, int y, Spr* sp, DWORD alpha, DWORD op);
extern void         PutCompressedImageFX_Raw(int x, int y, Spr* sp, DWORD alpha, DWORD op);
extern void			PutCompressedImage(int x, int y, Spr* sp);
extern void         PutCompressedImageforDrawMap(int x, int y, Spr* sp);
extern int			CheckHandleByLTSSupport(t_packet* p);		// 011201 LTS	// LTS MENU
extern void			SendCMD_LOCALWAR_STATUS();					// LTS LOCALWAR
extern int			LoadLocalWarInfoPic(char* FileName);			// LTS LOCALWAR
extern int			LoadChairPic(int ChairNum);			// LTS SITDOWN BUG
extern void			InitSitDownData();
extern void 		FreeLocalWarInfoPicBuffer();			// LTS LOCALWAR
extern void 		FreeChairPicBuffer();			// LTS SITDOWN
extern int			GetNationIndex(int Nation);		// LTS HORSERIDER





char* SquadPicBuffer = NULL;
Spr	  SquadPic[3][5];


void WarControl(char* Arg)			// 011025 LTS
{
#ifdef _DEBUG // finito 060507
	t_packet packet;				// Ex) /W 0 0   (View)	// ������������ ���¸� ���� 
	char Active;					//	   /W 1 0 1			// ���������͸� Active�� ��Ʈ�Ѵ�.
	//     /W 1 0 0			// ���������͸� DeActive�� ��Ʈ�Ѵ�.
	char Type = Arg[0] - 48;
	char WarfieldNo = Arg[2] - 48;

	if (Type) Active = Arg[4] - 48;	// Set		// ���� ���� 

	if (GetSysInfo(SI_GAME_MAKE_MODE))	// �������� ��� �϶��� ����..
	{
		if (!Type)				// View
		{
			packet.h.header.type = CMD_WAR_CONTROL;
			packet.u.NationWar.WarControl.Type = Type;
			packet.u.NationWar.WarControl.WarfieldNo = WarfieldNo;
			packet.u.NationWar.WarControl.Status = 0;					// �������� �ʿ���� ������
			packet.u.NationWar.WarControl.Active = 0;					// View������ �ʿ���� �����̴�.
			packet.h.header.size = sizeof(t_WarControl);
			AddCurrentStatusMessage(255, 255, 0, "Request : Request Current Warfield Loop Status...");
		}
		else					// Set
		{
			packet.h.header.type = CMD_WAR_CONTROL;
			packet.u.NationWar.WarControl.Type = Type;
			packet.u.NationWar.WarControl.WarfieldNo = WarfieldNo;
			packet.u.NationWar.WarControl.Status = 0;					// �������� �ʿ���� ������
			packet.u.NationWar.WarControl.Active = Active;
			packet.h.header.size = sizeof(t_WarControl);
			AddCurrentStatusMessage(255, 255, 0, "Request :Setting current warfield loop status to [%d]", Active);
		}
		QueuePacket(&packet, 1);
	}
#endif
}

void WarLoopTime(char* Arg)			// 011025 LTS
{
#ifdef _DEBUG // finito 060507
	t_packet packet;
	DWORD LoopTime;

	char Type = Arg[0] - 48;
	char WarfieldNo = Arg[2] - 48;	// �������� 

	if (Type) LoopTime = atoi(&Arg[4]);	// Set	// ����Ÿ���� �Է¹޴´�.

	if (GetSysInfo(SI_GAME_MAKE_MODE))
	{
		if (!Type)			// View
		{
			packet.h.header.type = CMD_WAR_LOOP_TIME;
			packet.u.NationWar.WarLoopTime.Type = Type;
			packet.u.NationWar.WarLoopTime.WarfieldNo = WarfieldNo;
			packet.u.NationWar.WarLoopTime.Status = 0;		//�������� �ʿ���� �����̴�.
			packet.u.NationWar.WarLoopTime.LoopTime = 0;		//�������� �ʿ���� �����̴�.
			packet.h.header.size = sizeof(t_WarLoopTime);
			AddCurrentStatusMessage(255, 255, 0, "Requesting current warfield loop time...");
		}
		else				// Set
		{
			packet.h.header.type = CMD_WAR_LOOP_TIME;
			packet.u.NationWar.WarLoopTime.Type = Type;
			packet.u.NationWar.WarLoopTime.WarfieldNo = WarfieldNo;
			packet.u.NationWar.WarLoopTime.Status = 0;		//�������� �ʿ���� �����̴�.
			packet.u.NationWar.WarLoopTime.LoopTime = LoopTime;
			packet.h.header.size = sizeof(t_WarLoopTime);
			AddCurrentStatusMessage(255, 255, 0, "Requesting current warfield loop time to alter to [%d]", LoopTime);
		}
		QueuePacket(&packet, 1);
	}
#endif
}

void SendCMD_CHECK_AM_I_DEFEAT()			//�ӽ��Լ� // 011015 LTS
{
	t_packet packet;

	packet.h.header.type = CMD_CHECK_AM_I_DEFEAT;
	packet.h.header.size = 0;
	QueuePacket(&packet, 1);

	if (Hero->name_status.nation == NW_YL) memset(&Hero->NWCharacter, 0, sizeof(DWORD));	//�Ͻ��̸� �ʱ�ȭ ��Ų��	// 011020 LTS
}

void SendCMD_CHECK_DEFEAT()
{
	t_packet packet;
	packet.h.header.type = CMD_CHECK_DEFEAT;
	packet.h.header.size = 0;
	QueuePacket(&packet, 1);

	memset(&Hero->NWCharacter, 0, sizeof(DWORD));
}

void InitSquadPic()
{
    for (int i = 0; i < 3; i++)
    {
        // [修正] 原本代碼是 j<4，但 LoadSquadPic 讀取了 5 個，這裡統一改為 5 以策安全
        for (int j = 0; j < 5; j++)
        {
            SquadPic[i][j].img = NULL;
            SquadPic[i][j].size = 0; // 建議順便初始化 size
        }
    }
}

int LoadSquadPic(char* FileName)
{
    short no = 0;
    DWORD sprfilelength = 0;
    DWORD size = 0;
    DWORD nOff = 0;
    int   i, j;
    Spr* s;

    // [Android] 路徑修正：移除開頭的 ./
    std::string path = FileName;
    if (path.find("./") == 0) path = path.substr(2);

    SDL_RWops* fp = SDL_RWFromFile(path.c_str(), "rb");
    if (fp == NULL) {
        SDL_Log("Error: Failed to open %s", path.c_str());
        return 0;
    }

    // 1. 讀取數量
    SDL_RWread(fp, &no, 2, 1);

    // 2. 移動指標 (Seek)
    // 跳過偏移表 (no * 4)
    SDL_RWseek(fp, no * 4, RW_SEEK_CUR);

    // 3. 計算 sprfilelength
    // 原本: _filelength(_fileno(fp)) - no * 4 * 2 - 2;
    // 這裡使用 SDL_RWsize 取得檔案總大小
    Sint64 totalFileSize = SDL_RWsize(fp);
    sprfilelength = (DWORD)(totalFileSize - no * 4 * 2 - 2);

    // 4. 緩衝區檢查
    // 原邏輯：如果 Buffer 非空，則返回 0 (失敗/不重複載入)
    if (SquadPicBuffer != NULL) {
        SDL_RWclose(fp);
        return 0;
    }

    // 原代碼這裡有 MemFree，但在 return 0 之後永遠執行不到。
    // 如果邏輯是「重新載入」，應該先 Free 再 Alloc。
    // 為了安全，這裡先確保 Free，雖然上面 return 擋住了。
    // MemFree(SquadPicBuffer);

    MemAlloc(SquadPicBuffer, sprfilelength);

    if (SquadPicBuffer == NULL) {
        SDL_Log("Error: Memory allocation failed for SquadPic");
        SDL_RWclose(fp);
        return 0;
    }

    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 5; j++)
        {
            s = &SquadPic[i][j];

            // 讀取單張圖片大小
            SDL_RWread(fp, &size, 4, 1);

            if (size == 0)
            {
                s->xl = 0;
                s->yl = 0;
                s->size = 0;
                s->img = NULL;
                continue;
            }

            SDL_RWread(fp, &(s->xl), sizeof(short), 1);
            SDL_RWread(fp, &(s->yl), sizeof(short), 1);
            //s->yl ++;
            SDL_RWread(fp, &(s->ox), sizeof(short), 1);
            SDL_RWread(fp, &(s->oy), sizeof(short), 1);
            SDL_RWread(fp, &(s->size), sizeof(unsigned int), 1);

            // 邊界檢查
            if (nOff + s->size > sprfilelength) // 這裡用 > 或 >= 都可以，通常檢查溢位
            {
                // JustMsg(lan->OutputMessage(6, 51), FileName, i + 1, j);
                SDL_Log("Error: SquadPic file damaged or buffer overflow %s [%d][%d]", path.c_str(), i, j);

                SDL_RWclose(fp);
                // ExitApplication(EA_LOAD_LOACL_SQUAD_SPR_DATA);
                return 0; // Android 上不要直接 Exit，改為回傳失敗
            }

            // 讀取圖片像素資料
            SDL_RWread(fp, SquadPicBuffer + nOff, s->size, 1);
            s->img = SquadPicBuffer + nOff;

            convert565to555_LoadSprite(s); //Todo finished

            nOff += size;
        }
    }
    SDL_RWclose(fp);

    // [重要] 呼叫紋理生成 (記得使用修正後的路徑)
    // 這裡我將其解鎖，因為這是 SDL 渲染的關鍵
    g_SDLInfo.CreateAtlasFromSprArray2D_CPP(path.c_str(), &SquadPic[0][0], 3, 5);

    return sprfilelength;
}

void DeleteSquadPic()
{
    MemFree(SquadPicBuffer);
    SquadPicBuffer = NULL; // 建議設為 NULL 避免懸空指標
}

// Global Function
bool InitNation()
{
    // [Android] 路徑修正：手動移除 ./
    // LoadSquadPic 內部已經有處理，但為了保險，這裡傳入修正後的字串
    // 或者依賴 LoadSquadPic 內部的處理。這裡傳入原始字串，讓函式內部處理。

    if (!LoadSquadPic((char*)"char/WarSquad.spr"))
    {
        SDL_Log("char/WarSquad.spr Error!!");
        JustMsg("char/WarSquad.spr Error!!");
        return false;
    }

    // LoadLocalWarInfoPic 也需要類似的 SDL 移植
    if (!LoadLocalWarInfoPic("data/LWInfo.dat"))
    {
        SDL_Log("data/LWInfo.dat Error!!");
        JustMsg("data/LWInfo.dat Error!!");
        return false;  // LTS LOCALWAR
    }

    InitSitDownData();       // LTS SITDOWN BUG

    g_pNation = new cNation;
    if (g_pNation == NULL) return false;

    g_pDontManager = new CDontManager;
    if (g_pDontManager == NULL)
        return false;
    else
        g_pDontManager->LoadData(); // 確保 LoadData 內部也是 SDL 相容的

    return true;
}
void CloseNation()
{
	DeleteSquadPic();
	FreeLocalWarInfoPicBuffer();				// LTS LOCALWAR
	FreeChairPicBuffer();						// LTS SITDOWN
	SAFE_DELETE(g_pNation);
	SAFE_DELETE(g_pDontManager);
}

// Very often Call Function
int CheckNationStatus(int StatusNo)
{
	int WarfieldNo = -1;
	if (g_pNation->GetStatus(0) == StatusNo) WarfieldNo = 0;
	if (g_pNation->GetStatus(1) == StatusNo) WarfieldNo = 1;
	if (g_pNation->GetStatus(2) == StatusNo) WarfieldNo = 2;
	if (g_pNation->GetNewStatus(0) == StatusNo) WarfieldNo = 3;
	//< LTH-040226-KO ���������� ���� ������ ���� ��ȯ. 1.4 ����
	if (g_pNation->GetNewStatus(WI_HADES_WARFIELD) == StatusNo) WarfieldNo = WI_HADES_WARFIELD;
	//> LTH-040226-KO
	return WarfieldNo;
}

/**
 * @fn		CheckWarfieldStatus().
 * @brief	�� �������� ���¸� ��� �����Ͽ� ��ȯ.
 * @param	int nStatusNo. ���� �� ���� ��.
 * @param	BYTE aWarfieldStatus[WI_MAX_WARFIELD]. �� �������� ��Ȳ ��ȯ ���.
 * @return	void.
 */
 //< LTH-040414-KO.
void CheckWarfieldStatus(int nStatusNo, BYTE aWarfieldStatus[WI_MAX_WARFIELD])
{
	// �ʱ�ȭ
	eNEO_WARFIELD_ID eNeoWarfieldId = (eNEO_WARFIELD_ID)0;
	for (; eNeoWarfieldId < WI_MAX_WARFIELD; eNeoWarfieldId = (eNEO_WARFIELD_ID)(eNeoWarfieldId + 1))
		aWarfieldStatus[eNeoWarfieldId] = (BYTE)FALSE;

	if (g_pNation->GetStatus(WI_MOUNTAIN_WARFIELD) == nStatusNo)
		aWarfieldStatus[WI_MOUNTAIN_WARFIELD] = (BYTE)TRUE;
	if (g_pNation->GetStatus(WI_PLAIN_WARFIELD) == nStatusNo)
		aWarfieldStatus[WI_PLAIN_WARFIELD] = (BYTE)TRUE;
	if (g_pNation->GetStatus(WI_ISLAND_WARFIELD) == nStatusNo)
		aWarfieldStatus[WI_ISLAND_WARFIELD] = (BYTE)TRUE;
	if (g_pNation->GetNewStatus(WI_SNOWY_WARFIELD - 3) == nStatusNo)
		aWarfieldStatus[WI_SNOWY_WARFIELD] = (BYTE)TRUE;
	if (g_pNation->GetNewStatus(WI_HADES_WARFIELD) == nStatusNo)
		aWarfieldStatus[WI_HADES_WARFIELD] = (BYTE)TRUE;
}
//> LTH-040414-KO

bool isNationWarfieldServer()
{
	if (MapInfo[MapNumber].port == WARFIELD_PORT1 ||
		MapInfo[MapNumber].port == WARFIELD_PORT2 ||
		MapInfo[MapNumber].port == WARFIELD_PORT3) return true;
	return false;
}

bool isNewWarfieldServer()
{
	int Port = MapInfo[MapNumber].port;
	if (Port < BASE_NEW_WARFIELD_PORT || Port >= (BASE_NEW_WARFIELD_PORT + MAX_NEW_WARFIELD))
		return false;
	return true;
}

//< LTH-040226-KO 1.4 ���� ������ ������ �ű����������� ���ϴ� �Լ�
BOOL IsNeoWarfieldServer()
{
	INT nPort = MapInfo[MapNumber].port;
	if ((nPort < WP_HADES_WARFIELD) || (nPort >= WP_MAX_WARFIELD_PORT))
		return FALSE;
	return TRUE;
}	//> LTH-040226-KO

// LTH-040207-KO  1.4 ��ġ ���� ���ο� ������ ��ȣ�ΰ�?
BOOL IsNeoWarfield(INT nWarfieldNo)
{
	if ((WI_HADES_WARFIELD > nWarfieldNo) || (WI_MAX_WARFIELD <= nWarfieldNo))
		return FALSE;
	return TRUE;
}
//> LTH-040226-KO

bool isOurSquad(LPCHARACTER hero, LPCHARACTER ch)			// 011028 LTS
{
	if (!isNationWarfieldServer() && !isNewWarfieldServer()) return true;

	if (hero->name_status.nation != ch->name_status.nation)	//�������� �ٸ� �����̸� ����̸� 
	{
		if (Hero->name_status.nation == NW_YL)	// 011019 LTS	// ���ΰ��� �Ͻ��̸�
		{
			if (ch->name_status.nation == NW_YL)	//��� �Ͻ��̸� 
			{
				if (hero->NWCharacter.YL_JoinNation == ch->NWCharacter.YL_JoinNation) return true; // ���� ���� �����ߴٸ� �츮�δ�
				else return false;	// �ٸ����� �����ߴٸ� �츮�δ� �ƴ�
			}
			else	//���ΰ��� �Ͻ� ����� �Ͻ��� �ƴϴ�
			{
				if (hero->NWCharacter.YL_JoinNation == ch->name_status.nation) return true;   // Join�� ������ ������ ���� ��
				else return false; // ������ ������ �ٸ��� 
			}
		}
		else	// Hero�� �Ͻ��� �̴ϴ� 
		{
			if (ch->name_status.nation == NW_YL)	//����� �Ͻ��̸� 
			{
				if (Hero->name_status.nation == ch->NWCharacter.YL_JoinNation) return true;
				else return false;
			}
			else return false;	// ���ΰ��� ��� �Ͻ��� �ƴϴ�. 
		}
	}
	else
	{
		if (Hero->name_status.nation == NW_YL)	// 011019 LTS	// ���ΰ��� �Ͻ��̸�
		{
			if (ch->name_status.nation == NW_YL)	//��� �Ͻ��̸� 
			{
				if (hero->NWCharacter.YL_JoinNation == ch->NWCharacter.YL_JoinNation) return true; // ���� ���� �����ߴٸ� �츮�δ�
				else return false;	// �ٸ����� �����ߴٸ� �츮�δ� �ƴ�
			}
			else	//���ΰ��� �Ͻ� ����� �Ͻ��� �ƴϴ�
			{
				if (hero->NWCharacter.YL_JoinNation == ch->name_status.nation) return true;   // Join�� ������ ������ ���� ��
				else return false; // ������ ������ �ٸ��� 
			}
		}
		else return true;	// Hero�� �Ͻ��� �̴ϴ� 
	}
}


bool isAttacker()			// ������ �ʿ����� �����ϴ�.
{
	if (Hero->name_status.nation == NW_YL)
	{
		if (Hero->NWCharacter.YL_JoinNation == g_DefenceNation) return false;
		else return true;
	}
	if (Hero->name_status.nation == g_DefenceNation) return false;
	else return true;
}

bool isAttacker(LPCHARACTER ch)				// LTS DRAGON BUG
{
	if (ch->name_status.nation == NW_YL)
	{
		if (ch->NWCharacter.YL_JoinNation == g_DefenceNation) return false;
		else return true;
	}
	if (ch->name_status.nation == g_DefenceNation) return false;
	else return true;
}

bool isEnemy(LPCHARACTER ch1, LPCHARACTER ch2)	// LTS DRAGON BUG2 reece
{
	if (isNewWarfieldServer())
	{
		int Nation1 = ch1->name_status.nation;
		int Nation2 = ch2->name_status.nation;
		if (Nation1 == NW_YL)
			Nation1 = ch1->NWCharacter.YL_JoinNation;
		if (Nation2 == NW_YL)
			Nation2 = ch2->NWCharacter.YL_JoinNation;

		if (Nation1 == Nation2)
			return false;
		else
			return true;
	}
	else
	{
		if (isAttacker(ch1) == isAttacker(ch2)) return false;
		return true;
	}
}


int GetSquadNo()
{
	return Hero->NWCharacter.SquadNo;
}


void DrawSquadPic(int x, int y, LPORDERTABLE o, LPCHARACTER ch)	// 011019 LTS	// LTS HORSERIDER
{
	//if (!isNationWarfieldServer()) return;
	//if (!g_bWarBegin) return;
	if (ch->viewtype != VIEWTYPE_TRANSPARENCY_ && ch->viewtype != VIEWTYPE_GHOST_)	// �����̰ų� �׾��ִ³��� ���� 
	{
		int NationIndex = GetNationIndex(ch->name_status.nation);

		if (isNationWarfieldServer())
		{
			if (ch->NWCharacter.isCommander) PutCompressedImage(x, y, &SquadPic[NationIndex][4]);		// ��ɰ��̸�..
			else if (ch->NWCharacter.SquadNo) PutCompressedImage(x, y, &SquadPic[NationIndex][ch->NWCharacter.SquadNo - 1]);	// �δ��̸�..
			else
			{
				if (ch->name_status.nation == NW_YL)
				{
					if (ch->NWCharacter.YL_JoinNation == NW_BY)		// ���̿뺴�̸�....
					{
						PutCompressedImage(x, y, &SquadPic[0][0]);
					}
					if (ch->NWCharacter.YL_JoinNation == NW_ZY)
					{
						PutCompressedImage(x, y, &SquadPic[1][0]);	// ���� �뺴�̸�..
					}
				}
			}
		}
		if (isNewWarfieldServer())
		{
			if (g_bWarBegin)
			{
				int Nation = ch->name_status.nation;
				if (Nation == NW_YL) Nation = ch->NWCharacter.YL_JoinNation;
				switch (Nation)
				{
				case NW_BY: PutCompressedImage(x, y, &SquadPic[0][0]); break;
				case NW_ZY: PutCompressedImage(x, y, &SquadPic[1][0]); break;
				}
			}
		}
	}
}

void WarMenuOpen()	// �������̽� ��������� �޴� ���� 
{
	MP3(50);
	CallSmallMenu(MN_NATION_BATTLE_BBS);
}

void GetTime(DWORD RemainTime, WORD& Hour, WORD& Min, WORD& Sec)		// 011020 LTS
{
	DWORD temp;
	Hour = static_cast<WORD>(RemainTime / 3600);
	temp = static_cast<DWORD>(RemainTime % 3600);
	Min = static_cast<WORD>(temp / 60);
	Sec = static_cast<WORD>(temp % 60);
}

void ShowCommanderInfo()
{
	AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 151), FALSE); //"============= ��ɰ� �޽��� ============="
	AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 152), FALSE); //"/*1 ���� : ��ɰ��� �δ��忡�� �޽����� ���� ��"
	AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 153), FALSE); //"/*2 ���� : ��ɰ��� �δ� ��ü���� �޽����� ���� ��"
	AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 154), FALSE); //"/*3 ���� : ��ɰ��� ������ �ȿ� �ִ� �ڱ��ο��� �޽����� ���� ��"
	AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 155), FALSE); //"/*4 ���� : ��ɰ��� �ڱ��� ��ü���� �޽����� ���� ��"
}			// 010924 LTS

void ShowSquadInfo()
{
	char Message[100];
	sprintf(Message, lan->OutputMessage(0, 157), Hero->NWCharacter.SquadNo); //"����� **[[�� %d �δ�]]** �� �Ҽӵ� �ֽ��ϴ�."
	AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 156), FALSE); //"============== �δ�   �Ҽ� =============="
	AddCurrentStatusMessage(255, 255, 0, Message, FALSE);
	AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 158), FALSE); //"0 �δ�� �ƹ� �δ뿡�� �Ҽӵ��� ���� ���� ��Ÿ���ϴ�."
	AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 159), FALSE); //"============== �δ� �޽��� =============="
	AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 160), FALSE); // "/*1 ���� : �δ����� ��ɰ����� �޽����� ���� ��"
	AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 161), FALSE); //"/*  ���� : �δ���� �ڽ��� �Ҽӵ� �δ������ �޽����� ���� ��"
}

void ReturnNationName(int Nation, char* Message)
{
	switch (Nation)
	{
	case 3: sprintf(Message, "%s", lan->OutputMessage(0, 205)); break; //"���̼���"
	case 4: sprintf(Message, "%s", lan->OutputMessage(0, 206)); break; //"������"
	case 6: sprintf(Message, "%s", lan->OutputMessage(0, 207)); break; //"�Ͻ�"
	}
}

void ReturnWarfieldName(int WarfieldNo, char* Message)
{
	switch (WarfieldNo)
	{
	case 0: sprintf(Message, "%s", lan->OutputMessage(0, 208)); break; //"����������"
	case 1: sprintf(Message, "%s", lan->OutputMessage(0, 209)); break; //"���������"
	case 2: sprintf(Message, "%s", lan->OutputMessage(0, 210)); break; //"��������"
	case 3: sprintf(Message, "%s", lan->OutputMessage(0, 386)); break;	// "����������"
	}
}


void SendCMD_SQUAD_CHAT(int type,const char* Message)			// Send Squad Chat Message to Server(Warfield Server)
{
	t_packet packet;

	//< LTH-040302-KO 1.4 Patch. IsNeoWarfieldServer() Add.
	if (!isNationWarfieldServer() && !isNewWarfieldServer() && !IsNeoWarfieldServer())
	{
		AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 211), FALSE); //"[������]������ ����� �� �ִ� ����Դϴ�."
		return;
	}
	//> LTH-040302-KO

	if (isNationWarfieldServer())
	{
		switch (type)  //�ȵȴٴ� �޽��� ��� 
		{
		case 0:	// "/*1"
			if (!(Hero->NWCharacter.isCommander || Hero->NWCharacter.isSquadLeader))
			{
				AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 212), FALSE); //"[��ɰ�], [�δ���]�� ���� �� �ִ� �޽��� ����Դϴ�."
				return;
			}
			break;
		case 1:	// "/*2"	
		case 2:	// "/*3"
		case 3:	// "/*4"
			if (!Hero->NWCharacter.isCommander)
			{
				AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 213), FALSE); //"[��ɰ�]�� ���� �� �ִ� �޽��� ����Դϴ�."
				return;
			}
			break;
		case 4:    // "/*"
			if (Hero->NWCharacter.SquadNo == 0)
				AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 214), FALSE); //"[�δ��]�� ���� �� �ִ� �޽��� ����Դϴ�."

		}
	}

	packet.h.header.type = CMD_SQUAD_CHAT;
	packet.u.NationWar.SquadChat.Type = type;
	packet.u.NationWar.SquadChat.Size = strlen(Message);
	strcpy(packet.u.NationWar.SquadChat.Message, Message);
	packet.h.header.size = sizeof(t_SquadChat) - MAX_STRING_PK + packet.u.NationWar.SquadChat.Size;
	QueuePacket(&packet, 1);
}


void ChangeClothe(int type)			// �� ���� �׽�Ʈ �Լ�, ������� �ʴ´�.
{
	static int i = 0;

	Hero->accessory[0] = fight_map_acc_num[i];
	Hero->accessory[1] = 1;		// �ƹ��͵� ��������
	LoadHeroClothAccessoryDataOne(Hero->sprno, Hero->accessory[0]);
	LoadHeroClothAccessoryDataOne(Hero->sprno, Hero->accessory[1]);
	AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 215), i); //"���� �ʹ�ȣ�� %d�Դϴ�."
	if (i > 3) i = 0;
	else i++;
}

void ChangeClothe2()
{
	LPCHARACTER	ch = Hero;			// �þ��� ��� ����� ���� �ٲ۴�.

	while (ch != NULL)
	{
		if (ch->sprno <= 1)
		{
			if (ch->name_status.nation == NW_BY)
			{
				if (ch->NWCharacter.isCommander || ch->NWCharacter.isSquadLeader)
					ch->accessory[0] = fight_map_acc_num[3];
				else ch->accessory[0] = fight_map_acc_num[1];
				ch->accessory[1] = 1;		// �ƹ��͵� ��������
			}
			if (ch->name_status.nation == NW_ZY)
			{
				if (ch->NWCharacter.isCommander || ch->NWCharacter.isSquadLeader)
					ch->accessory[0] = fight_map_acc_num[2];
				else ch->accessory[0] = fight_map_acc_num[0];
				ch->accessory[1] = 1;		// �ƹ��͵� ��������
			}
			if (ch->name_status.nation == NW_YL)
			{
				if (ch->NWCharacter.YL_JoinNation == NW_BY)			// ���̼����� ������ ����
				{
					if (ch->NWCharacter.isCommander || ch->NWCharacter.isSquadLeader)
						ch->accessory[0] = fight_map_acc_num[3];
					else ch->accessory[0] = fight_map_acc_num[1];
					ch->accessory[1] = 1;		// �ƹ��͵� ��������
				}
				else
				{
					if (ch->NWCharacter.isCommander || ch->NWCharacter.isSquadLeader)
						ch->accessory[0] = fight_map_acc_num[2];
					else ch->accessory[0] = fight_map_acc_num[0];
					ch->accessory[1] = 1;		// �ƹ��͵� ��������
				}
			}
			LoadHeroClothAccessoryDataOne(ch->sprno, ch->accessory[0]);
			LoadHeroClothAccessoryDataOne(ch->sprno, ch->accessory[1]);
		}
		ch = ch->lpNext;
	}
}


void RecvCMD_WAR_BEGIN(t_packet* p)
{
	g_bWarBegin = true;
	ChangeClothe2();  //�����Ϳ� ��������..
}

void RecvCMD_WAR_END(t_packet* p)
{
	g_bWarBegin = false;	//�����Ϳ��� ������.. ������ �˾Ƽ� ���� �ٲپ� �ش�.. ������ ������..
}

void SetWarArmor(t_server_change_equip* p, LPCHARACTER ch)		// NetWork.cpp RecvChangeEquip() Call
{
	if (isNationWarfieldServer())
	{
		if (ch->name_status.nation == NW_BY)
		{
			if (ch->NWCharacter.isCommander || ch->NWCharacter.isSquadLeader)	p->equip[0] = fight_map_acc_num[3];
			else p->equip[0] = fight_map_acc_num[1];
			p->equip[1] = 1;
		}
		if (ch->name_status.nation == NW_ZY)
		{
			if (ch->NWCharacter.isCommander || ch->NWCharacter.isSquadLeader)	p->equip[0] = fight_map_acc_num[2];
			else p->equip[0] = fight_map_acc_num[0];
			p->equip[1] = 1;
		}
		if (ch->name_status.nation == NW_YL)
		{
			if (ch->NWCharacter.YL_JoinNation == NW_BY)			// ���̼����� ������ ����
			{
				if (ch->NWCharacter.isCommander || ch->NWCharacter.isSquadLeader)	p->equip[0] = fight_map_acc_num[3];
				else p->equip[0] = fight_map_acc_num[1];
				p->equip[1] = 1;		// �ƹ��͵� ��������
			}
			else
			{
				if (ch->NWCharacter.isCommander || ch->NWCharacter.isSquadLeader)	p->equip[0] = fight_map_acc_num[2];
				else p->equip[0] = fight_map_acc_num[0];
				p->equip[1] = 1;		// �ƹ��͵� ��������
			}
		}
	}

	if (isNewWarfieldServer())
	{
		switch (ch->name_status.nation)
		{
		case NW_BY:
		{
			p->equip[0] = fight_map_acc_num[1];
			p->equip[1] = 1;
		}
		break;
		case NW_ZY:
		{
			p->equip[0] = fight_map_acc_num[0];
			p->equip[1] = 1;
		}
		break;
		case NW_YL:
		{
			if (ch->NWCharacter.YL_JoinNation == NW_BY)
			{
				p->equip[0] = fight_map_acc_num[1];
				p->equip[1] = 1;
			}
			else
			{
				p->equip[0] = fight_map_acc_num[0];
				p->equip[1] = 1;
			}
		}
		break;
		}
	}

	//< LTH-040302-KO 1.4 Patch. �Ͻ��� �ʰ������ڱ�~~
	if (IsNeoWarfieldServer())
	{
		switch (ch->name_status.nation)
		{
		case NW_BY:
		{
			p->equip[0] = fight_map_acc_num[1];
			p->equip[1] = 1;
		}
		break;
		case NW_ZY:
		{
			p->equip[0] = fight_map_acc_num[0];
			p->equip[1] = 1;
		}
		break;
		case NW_YL:
		{
			p->equip[0] = fight_map_acc_num[2];
			p->equip[1] = 1;
		}
		break;
		}
	}
	//> LTH-040302-KO
}

void SetWarArmor(t_server_user_add* c)		// NetWork.cpp	RecvUserAdd() Call
{
	if (isNationWarfieldServer())
	{
		if (c->nation == NW_BY)
		{
			if (c->isCommander || c->isSquadLeader) c->equip0 = fight_map_acc_num[3];
			else c->equip0 = fight_map_acc_num[1];
			c->equip1 = 1;
		}
		if (c->nation == NW_ZY)
		{
			if (c->isCommander || c->isSquadLeader) c->equip0 = fight_map_acc_num[2];
			else c->equip0 = fight_map_acc_num[0];
			c->equip1 = 1;
		}
		if (c->nation == NW_YL)
		{
			if (c->YL_JoinNation == NW_BY)			// ���̼����� ������ ����
			{
				if (c->isCommander || c->isSquadLeader)	c->equip0 = fight_map_acc_num[3];
				else c->equip0 = fight_map_acc_num[1];
				c->equip1 = 1;		// �ƹ��͵� ��������
			}
			else
			{
				if (c->isCommander || c->isSquadLeader)	c->equip0 = fight_map_acc_num[2];
				else c->equip0 = fight_map_acc_num[0];
				c->equip1 = 1;		// �ƹ��͵� ��������
			}
		}
	}

	if (isNewWarfieldServer())
	{
		switch (c->nation)
		{
		case NW_BY:
			c->equip0 = fight_map_acc_num[1];
			c->equip1 = 1;
			break;
		case NW_ZY:
			c->equip0 = fight_map_acc_num[0];
			c->equip1 = 1;
			break;
		case NW_YL:
			if (c->YL_JoinNation == NW_BY)
			{
				c->equip0 = fight_map_acc_num[1];
				c->equip1 = 1;
			}
			else
			{
				c->equip0 = fight_map_acc_num[0];
				c->equip1 = 1;
			}
			break;
		}
	}

	//< LTH-040302-KO 1.4 Patch. �Ͻ��� ���� ������¥!!!
	if (IsNeoWarfieldServer())
	{
		switch (c->nation)
		{
		case NW_BY:
		{
			c->equip0 = fight_map_acc_num[1];
			c->equip1 = 1;
		}
		break;
		case NW_ZY:
		{
			c->equip0 = fight_map_acc_num[0];
			c->equip1 = 1;
		}
		break;
		case NW_YL:
		{
			c->equip0 = fight_map_acc_num[2];
			c->equip1 = 1;
		}
		break;
		}
	}
	//> LTH-040302-KO
}

// LineCommand.cpp , Squad Chat Message 
void SendSquadChatMessage(int type,const char* Message)
{
	switch (type)
	{
	case LC_SQUAD_LEADER:	SendCMD_SQUAD_CHAT(0, Message);	break;
	case LC_SQUAD:	SendCMD_SQUAD_CHAT(1, Message);	break;
	case LC_SQUAD_MEMBER:	SendCMD_SQUAD_CHAT(2, Message);	break;
	case LC_SQUAD_MEMBER_TO_MEMBER:	SendCMD_SQUAD_CHAT(3, Message);	break;
	case LC_SQUAD_CHAT:	SendCMD_SQUAD_CHAT(4, Message);	break;
	}
}

// LineCommand.cpp, War Give Life
void WarGiveLife()
{
	t_packet p;

	//< LTH-040303-KO 1.4 Patch ������ �߰�
	if (!isNationWarfieldServer() && !isNewWarfieldServer() && !IsNeoWarfieldServer())
	{
		AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 216));
		return;
	} //"[������]������ ����� �� �ִ� ��ɾ� �Դϴ�."
	//> LTH-040303-KO
	if (!g_bWarBegin) { AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 217)); return; } //"[������]���� ����� �� �ִ� ��ɾ� �Դϴ�."
	if (Hero->hp > 0) { AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 218)); return; } //"[����] ���¿����� ����� �� �ִ� ��ɾ� �Դϴ�."

	p.h.header.type = CMD_REQUEST_WAR_GIVE_LIFE;
	p.h.header.size = 0;

	QueuePacket(&p, 1);
	AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 219)); //"���� ȸ���ϱ� ���ؼ��� ���İ� ������ �����ž� �մϴ�."
}

void WarGiveLife2(const char* Arg)
{
	t_packet packet;
	int GiveLifePoint = Arg[0] - 48;			// 48 ASCII '0'
	if (GiveLifePoint < 0 || GiveLifePoint>3) { AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 328));  return; }
	if (!isNationWarfieldServer()) { AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 216)); return; } //"[������]������ ����� �� �ִ� ��ɾ� �Դϴ�."
	if (!g_bWarBegin) { AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 217)); return; } //"[������]���� ����� �� �ִ� ��ɾ� �Դϴ�."
	if (Hero->hp > 0) { AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 218)); return; } //"[����] ���¿����� ����� �� �ִ� ��ɾ� �Դϴ�."

	packet.h.header.type = CMD_WAR_GIVE_LIFE2;
	packet.u.NationWar.CommonDataC.Data = GiveLifePoint;
	packet.h.header.size = sizeof(t_CommonDataC);
	QueuePacket(&packet, 1);
	AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 219)); //"���� ȸ���ϱ� ���ؼ��� ���İ� ������ �����ž� �մϴ�."
}
// Interface Concern Packet Send Function

void SendCMD_REQUEST_WARFIELD_STATUS(int RequestType)		// Request Warfield Status , Parameter : Button No
{
	t_packet p;

	p.h.header.type = CMD_REQUEST_WARFIELD_STATUS;
	p.u.NationWar.CommonDataC.Data = RequestType;				// Just Checking Warfield Status
	p.h.header.size = sizeof(t_CommonDataC);
	QueuePacket(&p, 1);
}

int CheckWarfieldLoop()
{
	if (g_pNation->GetStatus(0) >= 2) return 0;
	if (g_pNation->GetStatus(1) >= 2) return 1;
	if (g_pNation->GetStatus(2) >= 2) return 2;
	return -1;
}

void SendCMD_REQUEST_WARFIELD_INFO()
{
	t_packet p;

	int WarfieldNo = CheckWarfieldLoop();

	if (WarfieldNo >= 0)
	{
		p.h.header.type = CMD_REQUEST_WARFIELD_INFO;
		p.u.NationWar.CommonDataC.Data = WarfieldNo;
		p.h.header.size = sizeof(t_CommonDataC);
		QueuePacket(&p, 1);
	}
}

void SendCMD_REQUEST_VOTE_DATA(int WarfieldNo)				// Request Vote Data
{															// CMD_REQUEST_WARFIELD_STATUS -> CMD_ANSWER_WARFIELD_STATUS
	t_packet p;												// CMD_REQUEST_VOTE_DATA -> CMD_ANSWER_VOTE_DATA	
	p.h.header.type = CMD_REQUEST_VOTE_DATA;					// RecvCMD_ANSWER_VOTE_DATA() -> CallSmallMenu()
	p.u.NationWar.CommonDataC.Data = WarfieldNo;
	p.h.header.size = sizeof(t_CommonDataC);
	QueuePacket(&p, 1);
}

void RecvCMD_ANSWER_VOTE_DATA(t_packet* p)					// Nation War Join Interface Open
{															// Display Data	
	if (p->u.NationWar.VoteData.WarfieldNo >= 0)
	{
		//�׷����� �׸���
//		p->u.NationWar.VoteData.WarfieldNo, ->������ �ѹ�

		switch (p->u.NationWar.VoteData.WarfieldNo)
		{
		case 0: //����
			SMenu[MN_NATION_JOIN].nField[2].nImageNumber = 1;
			break;
		case 1: //���
			SMenu[MN_NATION_JOIN].nField[2].nImageNumber = 3;
			break;
		case 2: //��
			SMenu[MN_NATION_JOIN].nField[2].nImageNumber = 2;
			break;
		}
		//010927 lsw
		char msg[10];
		sprintf(msg, "%d", p->u.NationWar.VoteData.VoteCount);
		strcpy(SMenu[MN_NATION_JOIN].nField[5].temp, msg);
		SMenu[MN_NATION_JOIN].nField[4].nWillDo = p->u.NationWar.VoteData.VoteCount;

		char msg2[20];
		ReturnNationName(g_DefenceNation, msg2);
		char msg3[20];
		ReturnWarfieldName(p->u.NationWar.VoteData.WarfieldNo, msg3);
		//010928 lsw
		sprintf(SMenu[MN_NATION_JOIN].nField[3].temp, lan->OutputMessage(0, 142), msg2, msg3); //" [ %s ]�����κ��� [ %s ]�� Żȯ�ϱ� ���� ���� ���� ���θ� ��ǥ�ϴ� ���Դϴ�.\n ��ǥ �Ⱓ�� 5���� �־����� �� �Ⱓ �ȿ� 5���� �Ѱ� �Ǹ� �ڵ����� ���� ������ �̷����ϴ�.\n ������ ���� ���� �� 20�� �Ŀ� �������� �˴ϴ�." 

		CallSmallMenu(MN_NATION_JOIN, MN_NATION_BATTLE_BBS);
	}
	else
	{
		CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 220), FALSE); //"�������� ��ǥ�� �� �� �����ϴ�."
	}
}

//�������� ��ǥ
//�������̽� �� �����ִ��� Ȯ�� �Ѵ�.. ����� Ÿ�ӿ� ���� ���űⰣ�� �����µ��� ��ǥ�� �Ѵٸ�.. 
//��ǥ�� ���ߴٴ� ��Ŷ�� ���ƿ´�..�ƴ� ���ƿ;� �Ѵ�.
//ĳ���� ����Ʈ �� DB�� �����ؾ� �Ѵ�.
void SendCMD_VOTING_DATA()									// Send Nation War Join Data
{															// Ack Does Not Retrun
	t_packet p;

	if (Hero->NWCharacter.DoWarJoinVote)
	{
		CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 221), FALSE); //"�̹� ���� ���� ��ǥ�� �ϼ̽��ϴ�."
		return;
	}

	int WarfieldNo = CheckNationStatus(NW_VOTE);
	if (WarfieldNo >= 0)
	{
		if (Hero->name_status.nation != g_DefenceNation)
		{
			p.h.header.type = CMD_VOTING_DATA;
			p.u.NationWar.CommonDataC.Data = WarfieldNo;
			p.h.header.size = sizeof(t_CommonDataC);
			QueuePacket(&p, 1);

			char msg[80], msg2[200];
			ReturnWarfieldName(WarfieldNo, msg);
			sprintf(msg2, lan->OutputMessage(0, 222), msg); //"������ [ %s ] �� ���￡ ���� �ǻ縦 �������ϴ�."
			CallOkCancelMessageBox(TRUE, 0, 0, msg2, FALSE);
			Hero->NWCharacter.DoWarJoinVote = true;
		}
		else
		{
			CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 223), FALSE); //"������ ������ ����������ǥ�� �� �� �����ϴ�."
		}
	}
}

void SendCMD_REQUEST_SQUAD_INFO(int SquadNo)				// Request Warfield Squad Information
{															// Open Turn of Squad Menu
	t_packet p;												// Interface Click -> CMD_REQUEST_WARFIELD_STATUS ->
	int WarfieldNo = CheckNationStatus(NW_PREPARE);						// CMD_ANSWER_WARFIELD_STATUS -> CMD_REQUEST_SQUAD_INFO ->
	if (WarfieldNo < 0) WarfieldNo = CheckNationStatus(NW_WAR);		// 010927 LTS
	if (WarfieldNo >= 3)
	{
		//<! BBD 040322
		char tempMessage[200];
		switch (WarfieldNo)
		{
		case WI_SNOWY_WARFIELD:
		{
			sprintf(tempMessage, lan->OutputMessage(0, 292), lan->OutputMessage(0, 386));  //���������ʹ� �δ����� ������� �ʽ��ϴ�.
			break;
		}
		case WI_HADES_WARFIELD:
		{
			sprintf(tempMessage, lan->OutputMessage(0, 292), lan->OutputMessage(0, 619));
			break;
		}
		default:
		{
			break;
		}
		}
		CallOkCancelMessageBox(TRUE, 0, 0, tempMessage, FALSE);
		return;
		//> BBD 040322
	}
	// CMD_ANSWER_SQUAD_INFO -> CallOkCancelMessageBox(TRUE()

	if (SquadNo < 1)											// �δ� ��ȣ 0�� �δ� ����� ���� ���� ������ �����̴�.
	{
		SquadNo = 1;
		g_Squad = 1;
	}
	if (SquadNo > 4)
	{
		SquadNo = 4;
		g_Squad = 4;
	}
	if (WarfieldNo >= 0)
	{
		p.h.header.type = CMD_REQUEST_SQUAD_INFO;
		p.u.NationWar.RequestSquadInfo.Nation = Hero->name_status.nation;		// �Ͻ��ΰ��... ��ȣ�� ����.
		p.u.NationWar.RequestSquadInfo.WarfieldNo = WarfieldNo;
		p.u.NationWar.RequestSquadInfo.SquadNo = SquadNo;
		p.h.header.size = sizeof(t_RequestSquadInfo);
		QueuePacket(&p, 1);
		//AddCurrentStatusMessage( 255,255,0, "�δ���� �����͸� ��û�Ͽ����ϴ�. �δ��ȣ:%d",g_Squad);
	}
}

void RecvCMD_ANSWER_SQUAD_INFO(t_packet* p)					// Squad Information Packet process
{
	//MN �δ� ���� ���̽�
	g_SquadLoadingPoint = p->u.NationWar.SquadInfo.LoadingPoint;
	g_SquadNum = p->u.NationWar.SquadInfo.SquadNo;

	NationSupportNationSquadLeader(p);//010910 lsw
}

void RecvCMD_ANSWER_SQUAD_INFO2(t_packet* p)
{
	NationSupportNationSquadLeader2(p);//010910 lsw
}

//��ɰ� ��ǥ����Ÿ ��û
void SendCMD_REQUEST_COMMANDER_VOTE_DATA(int CandidaterNo)	// Request Commander Candidater Information, Parameter : Candidater No
{
	t_packet p;
	int WarfieldNo = CheckNationStatus(NW_PREPARE);

	if (CandidaterNo < 0)								// ��ɰ� ��ȣ�� üũ..
	{
		g_Commander = 0;
		CandidaterNo = 0;
	}
	if (CandidaterNo > 5)
	{
		g_Commander = 5;
		CandidaterNo = 5;
	}

	if (WarfieldNo >= 0)
	{
		p.h.header.type = CMD_REQUEST_COMMANDER_VOTE_DATA;
		p.u.NationWar.RequestCommanderVoteData.Nation = Hero->name_status.nation;
		p.u.NationWar.RequestCommanderVoteData.WarfieldNo = WarfieldNo;
		p.u.NationWar.RequestCommanderVoteData.CandidaterNo = CandidaterNo;
		p.h.header.size = sizeof(t_RequestCommanderVoteData);
		QueuePacket(&p, 1);
		//AddCurrentStatusMessage( 255,255,0, "��ɰ� ��ǥ�ڷḦ ��û�Ͽ����ϴ�. ������ȣ:%d, ��ɰ� ��ǥ�ڷ�:%d",WarfieldNo,CandidaterNo);
	}
}
void RecvCMD_ANSWER_COMMANDER_VOTE_DATA(t_packet* p)		// Answer Commander Candidater Information
{
	//�̰ŵ� �ٲ�� �� ����� ������
//	AddCurrentStatusMessage( 255,255,0, "��ɰ� �ĺ� %d�� �������Դϴ�.",p->u.NationWar.CommanderVoteData.VoteNo);
//	AddCurrentStatusMessage( 255,255,0, "�ĺ�ID : %d, �ĺ��̸� : %s",p->u.NationWar.CommanderVoteData.CandidaterID,p->u.NationWar.CommanderVoteData.Name);
//	AddCurrentStatusMessage( 255,255,0, "���� : %d, ���ġ : %d, ����ǥ :%d",p->u.NationWar.CommanderVoteData.Level,p->u.NationWar.CommanderVoteData.Fame,p->u.NationWar.CommanderVoteData.VoteCount);
	g_CommanderCandidaterLevel = p->u.NationWar.CommanderVoteData.Level;

	if (p->u.NationWar.CommanderVoteData.CandidaterID != 0)
	{
		sprintf(SMenu[MN_NATION_VOTE].nField[5].temp, "%d", SHideNomal[HN_MAKECHARACTER_CLASS_TEXT][p->u.NationWar.CommanderVoteData.VoteCount].nType);
		sprintf(SMenu[MN_NATION_VOTE].nField[6].temp, "%s", p->u.NationWar.CommanderVoteData.Name);
		const __int8 nBefore = p->u.NationWar.CommanderVoteData.Class;	// Ŭ����		// 011020 LTS
		const __int8 nAfter = p->u.NationWar.CommanderVoteData.nDual;	// ���..		// 011020 LTS
		const __int8 nStep = p->u.NationWar.CommanderVoteData.nStep;	// ����			// 011020 LTS
		const string strName = g_mgrDual.GetClassName(nBefore, nAfter, nStep, g_CommanderCandidaterLevel);
		sprintf(SMenu[MN_NATION_VOTE].nField[7].temp, "%s", strName.c_str());
		sprintf(SMenu[MN_NATION_VOTE].nField[8].temp, "%d", p->u.NationWar.CommanderVoteData.Fame);
		sprintf(SMenu[MN_NATION_VOTE].nField[9].temp, "%lu", p->u.NationWar.CommanderVoteData.LadderScore);
		SMenu[MN_NATION_VOTE].nField[10].nWillDo = 1;//�� �̹��� �ѱ�
		SMenu[MN_NATION_VOTE].nField[10].nImageType = p->u.NationWar.CommanderVoteData.Gender;
		SMenu[MN_NATION_VOTE].nField[10].nRectImage = p->u.NationWar.CommanderVoteData.Face;
	}
	else
	{
		SMenu[MN_NATION_VOTE].nField[10].nWillDo = 0;
		sprintf(SMenu[MN_NATION_VOTE].nField[5].temp, "%s", lan->OutputMessage(0, 224)); //"��ϴ��"
		sprintf(SMenu[MN_NATION_VOTE].nField[6].temp, "%s", lan->OutputMessage(0, 224)); //"��ϴ��"
		sprintf(SMenu[MN_NATION_VOTE].nField[7].temp, "%s", lan->OutputMessage(0, 224)); //"��ϴ��"
		sprintf(SMenu[MN_NATION_VOTE].nField[8].temp, "%s", lan->OutputMessage(0, 224)); //"��ϴ��"
		sprintf(SMenu[MN_NATION_VOTE].nField[9].temp, "%s", lan->OutputMessage(0, 224)); //"��ϴ��"
	}
}

void RecvCMD_ANSWER_COMMANDER_VOTE_DATA2(t_packet* p)
{
	char msg[200];
	memset(msg, 0, 200);

	for (int i = 0; i < 6; i++)//011001 lsw
	{
		sprintf(msg, "%s\n   [%d]", p->u.NationWar.CommanderVoteData2.CandidaterName[i], p->u.NationWar.CommanderVoteData2.VoteCount[i]);
		strcpy(SMenu[MN_NATION_VOTE].nField[11 + i].temp, msg);
	}

	//011005 lsw >
	CallSmallMenu(MN_NATION_VOTE, MN_NATION_BATTLE_BBS);	// Commander Candidater Interface Open
	//011005 lsw <
}


bool CheckJoinLevel(int SquadNo)	// 011015 LTS
{
	switch (SquadNo)
	{
	case 1:
		if (Hero->lv >= NW_SQUAD1_LEVEL_MIN && Hero->lv <= NW_SQUAD1_LEVEL_MAX) return true;
		return false;
	case 2:
		if (Hero->lv >= NW_SQUAD2_LEVEL_MIN && Hero->lv <= NW_SQUAD2_LEVEL_MAX) return true;
		return false;
	case 3:
		if (Hero->lv >= NW_SQUAD3_LEVEL_MIN && Hero->lv <= NW_SQUAD3_LEVEL_MAX) return true;
		return false;
	case 4:
		if (Hero->lv >= NW_SQUAD4_LEVEL_MIN && Hero->lv <= NW_SQUAD4_LEVEL_MAX) return true;
		return false;
	}
	return true;

}
//�δ븦 ���� �Ѵ�.. 
//�δ��ȣ�� �����ȣ�� �Բ� DB�� ����Ǿ�� �Ѵ�.
//�̹� �δ밡 �ִ°��... //�����ȣ�� �񱳸� �Ѵ�. // ���� �δ����̸�.. �ȵȴ�..
//�̹� �δ밡 �ִ� ���  
//���� �δ��� ��ȣ  //ĳ���� ����Ʈ�� �߰��Ǿ�� �ϴ� ���� 
//�δ뿡 ���� �����ϴ� ��� 
//���� �δ��� ��ȣ  //ĳ���� ����Ʈ�� �߰��Ǿ�� �ϴ� ���� 
void SendCMD_CHANGE_SQUAD(int SquadNo)						// Squad Registration Change 
{
	t_packet p;
	int WarfieldNo = CheckNationStatus(NW_PREPARE);

	if (Hero->NWCharacter.isSquadLeader)
	{
		CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 225), FALSE); //"�δ����� �δ븦 �ٲٽ� �� �����ϴ�."
		return;
	}

	if (Hero->NWCharacter.SquadNo) //���� �ʱⰪ���� 0�̾ƴ� ���� �־�� �Ѵ�. // If Hero Have Old Squad No
	{														// Send CMD_CHANGE_SQUAD Packet to Manage Server
		CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 226), FALSE); //"�ٸ� �δ�� ����Ͻ� �� �����ϴ�."
	}
	else													// Hero Does Not Have Old Squad No
	{														// Send CMD_JOIN_SQUAD Packet to Manage Server
		if (CheckJoinLevel(SquadNo))						// 011015 LTS
		{
			p.h.header.type = CMD_JOIN_SQUAD;						// Answer was CMD_JOIN_SQUAD or CMD_YOU_ARE_SQUAD_LEADER
			p.u.NationWar.JoinSquad.Nation = Hero->name_status.nation;
			p.u.NationWar.JoinSquad.WarfieldNo = WarfieldNo;
			p.u.NationWar.JoinSquad.SquadNo = g_Squad; //�ٲ��.. �������̽� ��...
			p.h.header.size = sizeof(t_JoinSquad);
			QueuePacket(&p, 1);

			char msg[80], msg2[200];
			ReturnWarfieldName(p.u.NationWar.JoinSquad.WarfieldNo, msg);
			sprintf(msg2, lan->OutputMessage(0, 227), msg, p.u.NationWar.JoinSquad.SquadNo); //"[ %s ]�� [�� %d �δ�] �δ������ ��ϵƽ��ϴ�."
			PlayListAutoSounds(257, 0, 0, 0);
			CallOkCancelMessageBox(TRUE, 0, 0, msg2, FALSE);
		}
		else CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 228), FALSE); //"�ش� �δ뿡 ����ϱ⿡ ���� ���� �����Դϴ�."
	}
}

void RecvCMD_YOU_ARE_SQUAD_LEADER(t_packet* p)
{
	//�δ����̹Ƿ�.. ���õ� ����Ÿ�� ��Ʈ�Ѵ�.. ��.��
	if (Hero->name_status.nation == NW_YL) Hero->NWCharacter.YL_JoinNation = p->u.NationWar.JoinSquad.Nation;
	Hero->NWCharacter.WarfieldNo = p->u.NationWar.JoinSquad.WarfieldNo;
	Hero->NWCharacter.SquadNo = p->u.NationWar.JoinSquad.SquadNo;
	Hero->NWCharacter.isSquadLeader = 1;
	char msg[80], msg2[200];
	ReturnWarfieldName(Hero->NWCharacter.WarfieldNo, msg);
	sprintf(msg2, lan->OutputMessage(0, 229), msg, Hero->NWCharacter.SquadNo); //"[ %s ]�� [�� %d �δ�] �δ��忡 ��ϵƽ��ϴ�."

	CallOkCancelMessageBox(TRUE, 0, 0, msg2, FALSE);
}

void RecvCMD_JOIN_SQUAD(t_packet* p)
{
	if (Hero->name_status.nation == NW_YL) Hero->NWCharacter.YL_JoinNation = p->u.NationWar.JoinSquad.Nation;
	Hero->NWCharacter.WarfieldNo = p->u.NationWar.JoinSquad.WarfieldNo;
	Hero->NWCharacter.SquadNo = p->u.NationWar.JoinSquad.SquadNo;
	char msg[80], msg2[200];
	ReturnWarfieldName(Hero->NWCharacter.WarfieldNo, msg);
	sprintf(msg2, lan->OutputMessage(0, 227), msg, Hero->NWCharacter.SquadNo); //"[ %s ]�� [�� %d �δ�] �δ������ ��ϵƽ��ϴ�."

	CallOkCancelMessageBox(TRUE, 0, 0, msg2, FALSE);

}

//�δ� �ε�����Ʈ�� ���� 
//�δ������� Ȯ���Ѵ�.
void SendCMD_SET_SQUAD_LODING_POINT(int SquadNo, int LoadingPoint)
{
	t_packet p;

	if (Hero->name_status.nation == g_DefenceNation)
	{
		CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 230), FALSE); //"[����]�� ���ݰ����� �������� �����ŵ� �˴ϴ�."
		return;
	}

	if (!Hero->NWCharacter.isSquadLeader)
	{
		CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 231), FALSE); //"[�δ���]�� �ƴϸ� ���ݰ����� ������ �� �����ϴ�."
		return;
	}

	int WarfieldNo = CheckNationStatus(NW_PREPARE);

	if (WarfieldNo >= 0)
	{
		p.h.header.type = CMD_SET_SQUAD_LOADING_POINT;
		p.u.NationWar.SetLoadingPoint.Nation = Hero->name_status.nation;
		p.u.NationWar.SetLoadingPoint.WarfieldNo = WarfieldNo;
		p.u.NationWar.SetLoadingPoint.SquadNo = SquadNo;
		p.u.NationWar.SetLoadingPoint.LoadingPoint = LoadingPoint;
		p.h.header.size = sizeof(t_SetLoadingPoint);
		QueuePacket(&p, 1);

		//011010 lsw >
		char msg[80];
		ReturnWarfieldName(WarfieldNo, msg);
		char msgbody[160];
		sprintf(msgbody, lan->OutputMessage(0, 232), msg, SquadNo, LoadingPoint); //"[ %s ]�� [�� %d �δ�] ���ݷθ� [ %d �� ]�������� ���߽��ϴ�."
		CallOkCancelMessageBox(TRUE, 0, 0, msgbody, TRUE);

		SMenu[MN_NATION_INFO].nField[7].nWillDo = 1;
		SMenu[MN_NATION_INFO].nField[7].x = SMenu[MN_NATION_INFO].nField[LoadingPoint].x;
		SMenu[MN_NATION_INFO].nField[7].y = SMenu[MN_NATION_INFO].nField[LoadingPoint].y;
		//011010 lsw <

		Hero->NWCharacter.WarfieldNo = WarfieldNo;
		Hero->NWCharacter.SquadNo = SquadNo;				//�ڽ��� �δ��ȣ�� �����Ѵ�.   // ������ ���� Ȯ�� �޽��� ���� ���� 


	}

	g_SquadLoadingPoint = LoadingPoint;//010901 lsw
}

void SendCMD_COMMANDER_REG_DATA(int CandidaterNo) //��ɰ� ��� 
{
	//�ڱ��ڽ��� �ĺ��� ó���Ѵ�... //���������� �˻���.. ������ ���� ���� �ִ�.
	t_packet p;
	int WarfieldNo = CheckNationStatus(NW_PREPARE);
	if (WarfieldNo >= 0)
	{
		if (!Hero->NWCharacter.isCommanderCandidater)
		{
			if ((Hero->lv >= NW_COMMANDER_LEVEL_MIN) && (Hero->fame >= NW_COMMANDER_FAME_MIN)) // 010927 LTS
			{
				p.h.header.type = CMD_REQUEST_COMMANDER_REG_DATA;
				p.u.NationWar.RequestCommanderVoteData.Nation = Hero->name_status.nation;
				p.u.NationWar.RequestCommanderVoteData.WarfieldNo = WarfieldNo;
				p.u.NationWar.RequestCommanderVoteData.CandidaterNo = CandidaterNo;
				p.h.header.size = sizeof(t_RequestCommanderVoteData);
				QueuePacket(&p, 1);
				char msg[80];
				ReturnWarfieldName(WarfieldNo, msg);
				AddCurrentStatusMessage(255, 255, 0,
					lan->OutputMessage(0, 233), msg, CandidaterNo); //"[ %s ]�� [ %d�� ] �ĺ��� ��� ��û �ϼ̽��ϴ�."
			}
			else CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 234), FALSE); //"��ɰ� �ĺ��� ��ϵǱ⿡�� �ɷ��� �����մϴ�."
		}
		else AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 235)); //"�̹� ��ɰ� �ĺ��� ����� �ϼ̽��ϴ�."
	}
}

void RecvCMD_ANSWER_COMMANDER_REG_DATA(t_packet* p)
{
	switch (p->u.NationWar.CommonDataC.Data)
	{
	case 0:
		CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 236), FALSE); //"��ɰ� �ĺ��� ��ϵ��� ���߽��ϴ�."
		break;
	case 1:
		Hero->NWCharacter.isCommanderCandidater = 1;
		CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 237), FALSE); //"[��ɰ�] �ĺ��� ����� �ϼ̽��ϴ�."
		break;
	case 2:
		CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 234), FALSE); //"��ɰ� �ĺ��� ��ϵǱ⿡�� �ɷ��� �����մϴ�."
		break;
	}
}


void SendCMD_COMMANDER_VOTING_DATA(int CandidaterNo) //��ɰ� ��ǥ 
{
	t_packet p;

	if (Hero->NWCharacter.DoCommanderVote)
	{
		CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 238), FALSE); //"�̹� ��ɰ� ��ǥ�� �߽��ϴ�."
		return;
	}
	int WarfieldNo = CheckNationStatus(NW_PREPARE);

	if (WarfieldNo >= 0)
	{
		p.h.header.type = CMD_COMMANDER_VOTING_DATA;
		p.u.NationWar.RequestCommanderVoteData.Nation = Hero->name_status.nation;
		p.u.NationWar.RequestCommanderVoteData.WarfieldNo = WarfieldNo;
		p.u.NationWar.RequestCommanderVoteData.CandidaterNo = CandidaterNo;
		p.h.header.size = sizeof(t_RequestCommanderVoteData);
		QueuePacket(&p, 1);
		char msg[80];
		ReturnWarfieldName(WarfieldNo, msg);
		//011008 lsw >
		char msgbody[120];
		sprintf(msgbody, lan->OutputMessage(0, 239), msg, CandidaterNo + 1); //"[ %s ]�� ��ɰ����� [ %d�� ] �ĺ��� �����߽��ϴ�." // 011020 LTS
		CallOkCancelMessageBox(FALSE, 0, 0, msgbody, TRUE);
		//011008 lsw <
		Hero->NWCharacter.DoCommanderVote = true;
	}
}

void SendCMD_REQUEST_CHECK_WARFIELD_POSSESSION(int Type)
{
	t_packet packet;

	packet.h.header.type = CMD_REQUEST_CHECK_WARFIELD_POSSESSION;
	packet.u.NationWar.CommonDataC.Data = Type;
	packet.h.header.size = sizeof(t_CommonDataC);
	QueuePacket(&packet, 1);
}

void SendCMD_REQUEST_WARFIELD_POSSESSION()
{
	t_packet packet;

	packet.h.header.type = CMD_REQUEST_WARFIELD_POSSESSION;
	packet.h.header.size = 0;
	QueuePacket(&packet, 1);
}

void SendCMD_FREELANCER_SQUAD_JOIN(int Nation)
{
	t_packet packet;

	Hero->NWCharacter.YL_JoinNation = Nation;		// 011020 LTS
	packet.h.header.type = CMD_FREELANCER_SQUAD_JOIN;
	packet.u.NationWar.CommonDataC.Data = Nation;
	packet.h.header.size = sizeof(t_CommonDataC);
	QueuePacket(&packet, 1);

	PlayListAutoSounds(257, 0, 0, 0);		// 010927 LTS

	switch (Nation)						// 010927 LTS
	{
	case NW_ZY:
		CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 240), FALSE); //"�̹� ���￡�� [������] �뺴���� �Ҽӵƽ��ϴ�."
		//		Hero->NWCharacter.YL_JoinNation=NW_ZY;	// 011020 LTS
		break;
	case NW_BY:
		CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 241), FALSE); //"�̹� ���￡�� [���̼���] �뺴���� �Ҽӵƽ��ϴ�."
		//		Hero->NWCharacter.YL_JoinNation=NW_BY;	// 011020 LTS
		break;
	}
}

void SendCMD_REQUEST_WARFIELD_WAR_DATA(int WarfieldNo)
{
	t_packet packet;

	packet.h.header.type = CMD_REQUEST_WARFIELD_WAR_DATA;
	packet.u.NationWar.CommonDataC.Data = WarfieldNo;
	packet.h.header.size = sizeof(t_CommonDataC);
	QueuePacket(&packet, 1);
}

void SendCMD_REQUEST_NEW_WARFIELD_DATA(int WarfieldNo)
{
	t_packet packet;

	packet.h.header.type = CMD_REQUEST_NEW_WARFIELD_DATA;
	packet.u.NationWar.CommonDataC.Data = WarfieldNo;
	packet.h.header.size = sizeof(t_CommonDataC);

	QueuePacket(&packet, 1);
}

void SendJoinNewWarfield(int Nation)
{
	if (Hero->NWCharacter.YL_JoinNation == NW_BY || Hero->NWCharacter.YL_JoinNation == NW_ZY)
	{
		CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 293), FALSE);   // "�̹� ������ ����Ǿ����ϴ�."
		return;
	}

	t_packet packet;
	packet.h.header.type = CMD_WAR_YL_JOIN;
	packet.u.NationWar.CommonDataC.Data = Nation;
	packet.h.header.size = sizeof(t_CommonDataC);

	QueuePacket(&packet, 1);
}

void RecvCMD_ANSWER_WARFIELD_STATUS(t_packet* p)			// Answer from Nation Manage Server
{
	int  WarfieldNo = -1;

	//AddCurrentStatusMessage( 255,255,0, "CMD_ANS_WARFIELD_STATUS %d,%d,%d",p->u.NationWar.WarfieldStatus.Status[0],p->u.NationWar.WarfieldStatus.Status[1],p->u.NationWar.WarfieldStatus.Status[2] );
	g_pNation->SetStatus(0, p->u.NationWar.WarfieldStatus.Status[0]);
	g_pNation->SetStatus(1, p->u.NationWar.WarfieldStatus.Status[1]);
	g_pNation->SetStatus(2, p->u.NationWar.WarfieldStatus.Status[2]);

	SendCMD_REQUEST_WARFIELD_INFO();		//�������� ���� ������ ��� ������ ���Ѵ�. // ���漭�� �˼�������.. ���ݱ����� �������� �Ŀ� �˼� �ִ�. (�Ͻ�,���̼���,������ ���Ｑ���� �� �� �ִٸ�)

	switch (p->u.NationWar.WarfieldStatus.PacketStatus)
	{
	case BUTTON_NATION_JOIN:  //������������ 
		WarfieldNo = CheckNationStatus(NW_VOTE); //��ǥ�ϴ� ����Ȯ��  //���� ����Ȯ�� // 010715 LTS TEMP
		if (WarfieldNo >= 0)
		{
			if (Hero->name_status.nation != g_DefenceNation)		// 010927 LTS
			{
				if (Hero->name_status.nation != NW_YL)
				{
					char msg[80];
					ReturnWarfieldName(WarfieldNo, msg);
					AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 242), msg); //"[ %s ] ���� ������ ���ؼ� ���ε��� ��ǥ�� �ް� �ֽ��ϴ�."
					SendCMD_REQUEST_VOTE_DATA(WarfieldNo);
				}
				else CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 243), FALSE); //"�Ͻ� ������ ���� ���� ��ǥ�� �� �� �����ϴ�."
			}
			else CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 244), FALSE); //"������ ������ ���� ������ǥ�� �� �� �����ϴ�."
		}
		else CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 245), FALSE); //"���� ���� ��ǥ�� �ް� �ִ� �����Ͱ� �����ϴ�."
		break;
	case BUTTON_NATION_SQUAD: //�δ��������̽� ����		// 010927 LTS
		if (Hero->name_status.nation == NW_YL)
		{
			CallOkCancelMessageBox(TRUE, 0, 0,
				lan->OutputMessage(0, 246), FALSE); // "�Ͻ� ������ [�뺴���] �޴��� �����ؼ� �뺴���� ���￡ �����ؾ� �մϴ�."
			return;
		}
		WarfieldNo = CheckNationStatus(NW_PREPARE);
		if (WarfieldNo >= 0)
		{
			char msg[80];
			ReturnWarfieldName(WarfieldNo, msg);
			AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 247), msg); //"[ %s ]�� ���￡ ���� �� �δ븦 �����ϰ� �ֽ��ϴ�."
			SendCMD_REQUEST_SQUAD_INFO(g_Squad);			// �������̽��� ȭ��ǥ�� ���� ���ڰ� �����ȴ�.. ��ó���� 0�̴�.	
		}
		else
		{
			WarfieldNo = CheckNationStatus(NW_WAR);
			if (WarfieldNo >= 0)
			{
				SendCMD_REQUEST_SQUAD_INFO(g_Squad); // �������� �ʴ´�.		 ���ݷ� ����, �δ����, ��ɰ� ��� 
			}
			else CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 248), FALSE); //"[�����غ���], [������] �Ⱓ���� ����� �� �ֽ��ϴ�."
		}
		break;
	case	BUTTON_NATION_CHECK_COMMANDER:
		if (g_bWarBegin) break;			// 010927 LTS		// �������̸� ������ �ʴ´�.
		WarfieldNo = CheckNationStatus(NW_PREPARE);
		if (WarfieldNo >= 0)
		{
			SendCMD_REQUEST_COMMANDER_VOTE_DATA(g_Commander);  // �������̽��� ȭ��ǥ�� ���� ���ڰ� �����ȴ�.. ��ó���� 0�̴�.
		}
		else CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 249), FALSE); //"[�����غ���] �Ⱓ���� ����� �� �ֽ��ϴ�."

		break;
	case	BUTTON_COMMANDER_REG:
		WarfieldNo = CheckNationStatus(NW_PREPARE);
		if (WarfieldNo >= 0)
		{
			SendCMD_COMMANDER_REG_DATA(g_Commander);  //�ӽ� // �������̽��� ȭ��ǥ�� ���� ���ڰ� �����ȴ�.. ��ó���� 0�̴�.
		}
		else CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 250), FALSE); //"[��ɰ� ��ǥ] �Ⱓ�� �ƴմϴ�."

		break;
	case	BUTTON_COMMANDER_VOTE:
		WarfieldNo = CheckNationStatus(NW_PREPARE);
		if (WarfieldNo >= 0)
		{
			if (g_CommanderCandidaterLevel == 0)
				CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 251), FALSE); //"[��ɰ� �ĺ�]�� ��� ���� �ʾҽ��ϴ�."
			else SendCMD_COMMANDER_VOTING_DATA(g_Commander);				// �������̽��� ȭ��ǥ�� ���� ���ڰ� �����ȴ�.. ��ó���� 0�̴�.
		}
		else CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 252), FALSE); //"��ǥ�� �� �ƽ��ϴ�."
		break;

	case	BUTTON_SQUAD_JOIN:
		if (g_bWarBegin) break;			// 010927 LTS		// �������̸� ������ �ʴ´�.
		WarfieldNo = CheckNationStatus(NW_PREPARE);
		if (WarfieldNo >= 0)
		{
			SendCMD_CHANGE_SQUAD(g_Squad);				// �������̽��� ȭ��ǥ�� ���� ���ڰ� �����ȴ�.. ��ó���� 0�̴�.
		}
		else CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 253), FALSE); //"�δ뿡 ����� �� �ƽ��ϴ�."
		break;
	case	BUTTON_SQUAD_LODINGPOINT:
		if (g_bWarBegin) break;			// 010927 LTS		// �������̸� ������ �ʴ´�.
		WarfieldNo = CheckNationStatus(NW_PREPARE);
		if (WarfieldNo >= 0)
		{
			if (Hero->name_status.nation == g_DefenceNation)
				CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 254), FALSE); //"�δ��� ���ݰ����� �������� �ʾҽ��ϴ�."
			else SendCMD_SET_SQUAD_LODING_POINT(g_Squad, 1);	// �������̽��� ȭ��ǥ�� ���� ���ڰ� �����ȴ�.. ��ó���� 0�̴�.
		}
		else CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 255), FALSE); //"�Ͻ� ���θ� ����� �� �ֽ��ϴ�."
		break;
	case	BUTTON_JOIN_FREELANCER:
		if (Hero->name_status.nation == NW_YL)  // 010927 LTS
		{
			WarfieldNo = CheckNationStatus(NW_PREPARE);
			if (WarfieldNo >= 0)	CallSmallMenu(MN_NATION_GUEST, MN_NATION_BATTLE_BBS);
			else
			{
				WarfieldNo = CheckNationStatus(NW_WAR);
				if (WarfieldNo >= 3)
				{	//< LTH-040316-KO ���� �����Ϳ��� �Ͻ��� �뺴���� �޴� ó��
					if (!IsNeoWarfield(WarfieldNo))
						CallSmallMenu(MN_NATION_GUEST, MN_NATION_BATTLE_BBS);
					else
						CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 622), FALSE); //"���� �����Ϳ��� �Ͻ��� �뺴���� �������� �ʽ��ϴ�"
				}	//> LTH-040316-KO
				else
					CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 249), FALSE); //"[�����غ���] �Ⱓ���� ����� �� �ֽ��ϴ�."
			}
		}
		else CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 255), FALSE); //"�Ͻ� ���θ� ����� �� �ֽ��ϴ�."

		break;
	case	BUTTON_FREELANCER_SQUAD:
		WarfieldNo = CheckNationStatus(NW_PREPARE);
		if (WarfieldNo >= 0)
		{
			//CallOkCancelMessageBox(TRUE, 0,0, "���øʿ����� �������� �ʴ� ����Դϴ�.", FALSE);
			SendCMD_REQUEST_CHECK_WARFIELD_POSSESSION(1);  //�Ͻ� �δ� ���
		}
		else CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 256), FALSE); //"�뺴�δ븦 ����� �� �ִ� ��Ȳ�� �ƴմϴ�."
		break;
	case	BUTTON_BY_JOIN:
		WarfieldNo = CheckNationStatus(NW_PREPARE);
		if (WarfieldNo >= 0)
		{
			SendCMD_REQUEST_CHECK_WARFIELD_POSSESSION(4); // ��������..		// 011020 LTS
			//if (Hero->NWCharacter.YL_JoinNation==0) SendCMD_FREELANCER_SQUAD_JOIN(NW_BY);  // define
			//else CallOkCancelMessageBox(TRUE, 0,0, lan->OutputMessage(0,257), FALSE); //"�̹� �뺴���� ������ ������ �����߽��ϴ�."
		}
		else
		{
			WarfieldNo = CheckNationStatus(NW_WAR);
			if (WarfieldNo >= 3)
			{	//< LTH-040316-KO �Ͻ� �뺴���� ó�� ���� ������������ �Ͻ��� �뺴�� �ƴϴ�
				if (!IsNeoWarfield(WarfieldNo))
				{
					SendJoinNewWarfield(NW_BY);
					CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 294), FALSE);	// "���̼��� ���������� ����Ǿ����ϴ�."
				}
				else
					CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 622), FALSE); //"���� �����Ϳ��� �Ͻ��� �뺴���� �������� �ʽ��ϴ�"
			}	//> LTH-040316-KO
			else
				CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 249), FALSE); //"[�����غ���] �Ⱓ���� ����� �� �ֽ��ϴ�."
		}
		break;
	case	BUTTON_ZY_JOIN:
		WarfieldNo = CheckNationStatus(NW_PREPARE);
		if (WarfieldNo >= 0)
		{
			SendCMD_REQUEST_CHECK_WARFIELD_POSSESSION(5); // ��������..		// 011020 LTS
			//if (Hero->NWCharacter.YL_JoinNation==0) SendCMD_FREELANCER_SQUAD_JOIN(NW_BY);  // define
			//else CallOkCancelMessageBox(TRUE, 0,0, lan->OutputMessage(0,257), FALSE); //"�̹� �뺴���� ������ ������ �����߽��ϴ�."
		}
		else
		{
			WarfieldNo = CheckNationStatus(NW_WAR);
			if (WarfieldNo >= 3)
			{	//< LTH-040316-KO �Ͻ� �뺴���� ó�� ���� ������������ �Ͻ��� �뺴�� �ƴϴ�
				if (!IsNeoWarfield(WarfieldNo))
				{
					SendJoinNewWarfield(NW_ZY);
					CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 295), FALSE);		//  "������ ���������� ����Ǿ����ϴ�."
				}
				else
					CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 622), FALSE); //"���� �����Ϳ��� �Ͻ��� �뺴���� �������� �ʽ��ϴ�"
			}	//> LTH-040316-KO
			else
				CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 249), FALSE); //"[�����غ���] �Ⱓ���� ����� �� �ֽ��ϴ�."
		}
		break;
	case	BUTTON_NATION_STATUS:
		WarfieldNo = CheckNationStatus(NW_WAR);
		if (WarfieldNo >= 0)
		{
			if (WarfieldNo >= 3)
			{
				SendCMD_REQUEST_NEW_WARFIELD_DATA(WarfieldNo);
			}
			else
				SendCMD_REQUEST_WARFIELD_WAR_DATA(WarfieldNo);
		}
		else
		{
			SendCMD_LOCALWAR_STATUS();
			//CallOkCancelMessageBox(TRUE, 0,0, lan->OutputMessage(0,258), FALSE); //"[������] �Ⱓ���� ����� �� �ֽ��ϴ�."
		}
		break;
	case BUTTON_NATION_NOTICE:
		SendCMD_REQUEST_CHECK_WARFIELD_POSSESSION(2);  //���� ���� 

		break;
	case BUTTON_NATIONWAR_JOIN_VOTE:
		SendCMD_VOTING_DATA();
		break;
	}
}

//011008 lsw  >
void 	SendCMD_GET_LOADING_POINT(int type)
{
	SMenu[MN_NATION_INFO].nField[7].nWillDo = 0;

	//���� �� �޾� �ð� ������ �޾� ������ 
	if (Hero->NWCharacter.isSquadLeader)
	{
		if (Hero->NWCharacter.SquadNo == g_SquadNum)
		{
			switch (CheckNationStatus(NW_PREPARE))
			{
			case 0: //����
				/*
				[142.216]-2�����ݰ��
				[221.202]-1�����ݰ��
				[251.103]-3�����ݰ��
				*/
				SMenu[MN_NATION_INFO].nField[0].nImageNumber = 4;
				//1 �� ����
				SMenu[MN_NATION_INFO].nField[1].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
				SMenu[MN_NATION_INFO].nField[1].x = 18 + 221;
				SMenu[MN_NATION_INFO].nField[1].y = 27 + 202;

				//2 �� ����
				SMenu[MN_NATION_INFO].nField[2].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
				SMenu[MN_NATION_INFO].nField[2].x = 18 + 142;
				SMenu[MN_NATION_INFO].nField[2].y = 27 + 216;

				//3 �� ����
				SMenu[MN_NATION_INFO].nField[3].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
				SMenu[MN_NATION_INFO].nField[3].x = 18 + 251;
				SMenu[MN_NATION_INFO].nField[3].y = 27 + 103;

				SMenu[MN_NATION_INFO].nField[4].nType = 0;
				break;
			case 1: //���
				/*
				[227.200]-1�����ݰ��
				[225.70]-3�����ݰ��
				[83.218]-2�����ݰ��
				*/
				SMenu[MN_NATION_INFO].nField[0].nImageNumber = 6;
				//1 �� ����
				SMenu[MN_NATION_INFO].nField[1].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
				SMenu[MN_NATION_INFO].nField[1].x = 18 + 227;
				SMenu[MN_NATION_INFO].nField[1].y = 27 + 200;

				//2 �� ����
				SMenu[MN_NATION_INFO].nField[2].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
				SMenu[MN_NATION_INFO].nField[2].x = 18 + 83;
				SMenu[MN_NATION_INFO].nField[2].y = 27 + 218;

				//3 �� ����
				SMenu[MN_NATION_INFO].nField[3].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
				SMenu[MN_NATION_INFO].nField[3].x = 18 + 225;
				SMenu[MN_NATION_INFO].nField[3].y = 27 + 70;

				SMenu[MN_NATION_INFO].nField[4].nType = 0;

				break;
			case 2: //��
				/*
				[29.67] - 4�����ݰ��
				[29.7] - 3�����ݰ��
				[258.161]-2�����ݰ��
				[256.228]-1�����ݰ��
				*/
				SMenu[MN_NATION_INFO].nField[0].nImageNumber = 5;
				//1 �� ����
				SMenu[MN_NATION_INFO].nField[1].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
				SMenu[MN_NATION_INFO].nField[1].x = 18 + 256;
				SMenu[MN_NATION_INFO].nField[1].y = 27 + 228;

				//2 �� ����
				SMenu[MN_NATION_INFO].nField[2].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
				SMenu[MN_NATION_INFO].nField[2].x = 18 + 258;
				SMenu[MN_NATION_INFO].nField[2].y = 27 + 161;

				//3 �� ����
				SMenu[MN_NATION_INFO].nField[3].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
				SMenu[MN_NATION_INFO].nField[3].x = 18 + 29;
				SMenu[MN_NATION_INFO].nField[3].y = 27 + 7;
				//4 �� ����
				SMenu[MN_NATION_INFO].nField[4].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
				SMenu[MN_NATION_INFO].nField[4].x = 18 + 29;
				SMenu[MN_NATION_INFO].nField[4].y = 27 + 67;

				break;
			}
			//011010 lsw >
			SetRect(SMenu[MN_NATION_INFO].nField[1].rCheakBox, SMenu[MN_NATION_INFO].nField[1].x - 10, SMenu[MN_NATION_INFO].nField[1].y - 10,
				SMenu[MN_NATION_INFO].nField[1].x + 24, SMenu[MN_NATION_INFO].nField[1].y + 40);
			SetRect(SMenu[MN_NATION_INFO].nField[2].rCheakBox, SMenu[MN_NATION_INFO].nField[2].x - 10, SMenu[MN_NATION_INFO].nField[2].y - 10,
				SMenu[MN_NATION_INFO].nField[2].x + 24, SMenu[MN_NATION_INFO].nField[2].y + 40);
			SetRect(SMenu[MN_NATION_INFO].nField[3].rCheakBox, SMenu[MN_NATION_INFO].nField[3].x - 10, SMenu[MN_NATION_INFO].nField[3].y - 10,
				SMenu[MN_NATION_INFO].nField[3].x + 24, SMenu[MN_NATION_INFO].nField[3].y + 40);
			SetRect(SMenu[MN_NATION_INFO].nField[4].rCheakBox, SMenu[MN_NATION_INFO].nField[4].x - 10, SMenu[MN_NATION_INFO].nField[4].y - 10,
				SMenu[MN_NATION_INFO].nField[4].x + 24, SMenu[MN_NATION_INFO].nField[4].y + 40);
			//011010 lsw <

			if (g_SquadLoadingPoint)
			{
				//���ݷΰ� �Ǿ� �ִ� ����
				SMenu[MN_NATION_INFO].nField[7].nWillDo = 1;
				SMenu[MN_NATION_INFO].nField[7].x = SMenu[MN_NATION_INFO].nField[g_SquadLoadingPoint].x;
				SMenu[MN_NATION_INFO].nField[7].y = SMenu[MN_NATION_INFO].nField[g_SquadLoadingPoint].y;
			}
			//010927 lsw
			//		sprintf(SMenu[MN_NATION_INFO].nField[6].temp, lan->OutputMessage(0,113),g_SquadLoadingPoint+1); //"�δ� ���ݷθ� �����ϼ̽��ϴ�. �ѹ� ���� ���ݷδ� �ٲ� �� �����ϴ�."
			CallSmallMenu(MN_NATION_INFO, MN_NATION_BATTLE_BBS);
		}
		else
		{
			CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 259), FALSE); //"�ٸ� �δ��� ���ݷθ� ������ �� �����ϴ�."
		}
	}
	else
	{
		//�ƴ϶�� �޼���
		CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 231), FALSE); //"[�δ���]�� �ƴϸ� ���ݰ����� ������ �� �����ϴ�."
	}
}
//011008 lsw  <

//-------------------------------------------------------------------------------------
// InterFace Concern
//-------------------------------------------------------------------------------------

void SendCMD_REQUEST_NEW_WARFIELD_STATUS()
{
	t_packet packet;

	packet.h.header.type = CMD_REQUEST_NEW_WARFIELD_STATUS;
	packet.h.header.size = 0;

	QueuePacket(&packet, 1);
}

void SendNationPacket(int type)//010730 lsw ������ �����������̽� (q �����鳪���°� ) ��Ŷ�� ���ư��°�
{
	SendCMD_REQUEST_NEW_WARFIELD_STATUS();

	switch (type)
	{
	case	BUTTON_NATION_STATUS:
		SendCMD_REQUEST_WARFIELD_STATUS(type);
		break;
	case	BUTTON_NATION_SQUAD:
		SendCMD_REQUEST_WARFIELD_STATUS(type);
		break;
	case	BUTTON_NATION_JOIN:
		SendCMD_REQUEST_WARFIELD_STATUS(type);
		break;
	case	BUTTON_HALLOFFAME:
		break;
	case	BUTTON_HALLOFWAR:
		break;
	case	BUTTON_WAR_DESCRIPTION:
		SendCMD_REQUEST_WARFIELD_POSSESSION();
		break;
		//__________Nation War Join Interface
	case	BUTTON_NATIONWAR_JOIN_VOTE:
		SendCMD_REQUEST_WARFIELD_STATUS(type);
		break;
		//__________Squad Commander Interface
	case	BUTTON_NATION_CHECK_COMMANDER:
		SendCMD_REQUEST_WARFIELD_STATUS(type);
		break;
	case	BUTTON_COMMANDER_REG:
		SendCMD_REQUEST_WARFIELD_STATUS(type);
		break;
	case	BUTTON_COMMANDER_VOTE:
		SendCMD_REQUEST_WARFIELD_STATUS(type);
		break;
		//__________Squad Interface
	case	BUTTON_SQUAD_JOIN:
		SendCMD_REQUEST_WARFIELD_STATUS(type);
		break;
	case	BUTTON_SQUAD_LODINGPOINT:						// Current Loading Point Set Menu Open
		SendCMD_REQUEST_WARFIELD_STATUS(type);
		break;
	case	BUTTON_JOIN_FREELANCER:
		SendCMD_REQUEST_WARFIELD_STATUS(type);
		break;
	case	BUTTON_FREELANCER_SQUAD:
		SendCMD_REQUEST_WARFIELD_STATUS(type);
		break;
	case	BUTTON_BY_JOIN:
		SendCMD_REQUEST_WARFIELD_STATUS(type);
		break;
	case	BUTTON_ZY_JOIN:
		SendCMD_REQUEST_WARFIELD_STATUS(type);
		break;
	case	BUTTON_NATION_NOTICE:
		SendCMD_REQUEST_WARFIELD_STATUS(type);
		break;

	case	BUTTON_NATION_LOADING_POINT:
		SendCMD_GET_LOADING_POINT(type);
		break;
	}
}


void WarMenuNoticeOpen()	// 011017 LTS
{
	t_packet packet;

	packet.h.header.type = CMD_REQUEST_NOTICE;
	packet.h.header.size = 0;
	QueuePacket(&packet, 1);
}

LPCHARACTER FindHim(int ServerID)
{
	LPCHARACTER temp = Hero;
	while (temp)
	{
		if (temp->id == ServerID)
		{
			return temp;
		}
		temp = temp->lpNext;
	}
	return NULL;
}

LPCHARACTER FindHimBySprNo(int SprNo)		// LTS DRAGON MODIFY
{
	LPCHARACTER temp = Hero;
	while (temp)
	{
		if (temp->sprno == SprNo)
		{
			return temp;
		}
		temp = temp->lpNext;
	}
	return NULL;
}


// BBS Packet Processing Routine __________________________________________________________
void ProcessBBS_GENERAL(t_packet* p, char* Message)				// �Ϲ����� ����� ó�� 
{
	PlayListAutoSounds(50, 0, 0, 0);
}

void ProcessBBS_PEACE(t_packet* p, char* Message)
{
	PlayListAutoSounds(62, 0, 0, 0);	// 010927 LTS
	AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 260)); //"�� ���� ������ �Ͼ�� �ʽ��ϴ�. ���̼����� �������� ���� ���¸� �����ϰ� �˴ϴ�."
}

void ProcessBBS_WAR_JOIN_VOTE(t_packet* p, char* Message)
{
	char temp[80], temp2[80];
	int DefenceNation = p->u.NationWar.WarBBS.Arg[0];

	g_pNation->SetStatus(p->u.NationWar.WarBBS.WarfieldNo, NW_VOTE);
	ReturnWarfieldName(p->u.NationWar.WarBBS.WarfieldNo, temp);
	ReturnNationName(DefenceNation, temp2);

	PlayListAutoSounds(62, 0, 0, 0);			// 010927 LTS
	AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 101), temp2, temp); //"[ %s ]������ [ %s ]�� �ĵ�� ���� ���� ��ǥ�� �ϰ� �ֽ��ϴ�. ��������� ��ư�� ���� ��ǥ�� ������ �ֽñ� �ٶ��ϴ�." 
}

void ProcessBBS_WAR_PREPARE(t_packet* p, char* Message)
{
	char temp[80], temp2[80], temp3[80];
	int AttackNation = p->u.NationWar.WarBBS.Arg[0];
	int DefenceNation = p->u.NationWar.WarBBS.Arg[1];

	g_pNation->SetStatus(p->u.NationWar.WarBBS.WarfieldNo, NW_PREPARE);
	ReturnWarfieldName(p->u.NationWar.WarBBS.WarfieldNo, temp);
	ReturnNationName(4, temp2);
	ReturnNationName(3, temp3);

	if (Hero->name_status.nation == AttackNation)
	{
		switch (AttackNation)
		{
		case 3: AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 102), temp2, temp); break; //"[ %s ]������ [[ %s ]]�� 20 �� �Ŀ� ������ �ϰ� �� ���Դϴ�."
		case 4: AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 102), temp3, temp); break; //"[ %s ]������ [[ %s ]]�� 20 �� �Ŀ� ������ �ϰ� �� ���Դϴ�."
		}

	}
	else if (Hero->name_status.nation == DefenceNation)
	{
		switch (DefenceNation)
		{
		case 3: AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 105), temp3, temp); break; //"�������� ���� ������ �߽��ϴ�. [ %s ]������ [ %s ]�� ��� �غ� �Ͻñ� �ٶ��ϴ�."
		case 4: AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 105), temp2, temp); break; //"�������� ���� ������ �߽��ϴ�. [ %s ]������ [ %s ]�� ��� �غ� �Ͻñ� �ٶ��ϴ�."
		}
	}
	else
	{
		switch (AttackNation)
		{
		case 3:
			AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 106), temp2, temp3, temp); //"[ %s ]������ [ %s ]������ ���� ������ �߽��ϴ�. 20�� �Ŀ� ������ ������ ������ [[ %s ]]�Դϴ�."
			break;
		case 4:
			AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 106), temp3, temp2, temp); //"[ %s ]������ [ %s ]������ ���� ������ �߽��ϴ�. 20�� �Ŀ� ������ ������ ������ [[ %s ]]�Դϴ�."
			break;
		}

	}
	PlayListAutoSounds(271, 0, 0, 0);
}

//011008 lsw > //011011 lsw >
void ProcessBBS_WAR_BEGIN(t_packet* p, char* Message)
{
	char tempZy[80] = { 0 }, tempBy[80] = { 0 }, tempField[80] = { 0 }, tempDefNa[80] = { 0 }, tempAttNa[80] = {0};
	//���⼭ ���� ���� �м� �Ͽ� 
	int iLoadingPonint;
	char SquadLoadingPoint[NW_SQUAD_KIND][NW_SQUAD_MAX] = { 0 };
	int DefenceNation = p->u.NationWar.WarBBS.Arg[0];
	int WarfieldCode = p->u.NationWar.WarBBS.WarfieldNo;

	if (!g_pNation) return;

	g_pNation->SetStatus(WarfieldCode, NW_WAR);

	SMenu[MN_NATION_WAR_START].nField[1].nWillDo = 0;
	SMenu[MN_NATION_WAR_START].nField[2].nWillDo = 0;
	SMenu[MN_NATION_WAR_START].nField[3].nWillDo = 0;
	SMenu[MN_NATION_WAR_START].nField[4].nWillDo = 0;

	memcpy(SquadLoadingPoint, &p->u.NationWar.WarBBS.Arg[1], p->u.NationWar.WarBBS.Size - 1);

	if (isAttacker()) { iLoadingPonint = SquadLoadingPoint[NW_ATTACKER][Hero->NWCharacter.SquadNo]; }//����ٰ� ������ �˴ϴ� 
	else { iLoadingPonint = SquadLoadingPoint[NW_DEFENCER][Hero->NWCharacter.SquadNo]; }//����ٰ� ������ �˴ϴ�	// �Ͻ��̸� ������ �ִ�.

	//011010 lsw >
	if (iLoadingPonint < 1 || iLoadingPonint > 4) { iLoadingPonint = 1; }
	//011010 lsw <

	ReturnWarfieldName(p->u.NationWar.WarBBS.WarfieldNo, tempField);
	ReturnNationName(NW_ZY, tempZy);
	ReturnNationName(NW_BY, tempBy);
	ReturnNationName(DefenceNation, tempDefNa);

	if (DefenceNation == NW_ZY)
	{
		ReturnNationName(NW_BY, tempAttNa);
	}
	else
	{
		ReturnNationName(NW_ZY, tempAttNa);
	}

	if (//�����
		(
			Hero->name_status.nation == NW_YL &&
			Hero->NWCharacter.YL_JoinNation == DefenceNation)
		|| Hero->name_status.nation == DefenceNation) //��
	{
		iLoadingPonint = 7;
	}

	AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 261), tempDefNa, tempField); //"[ %s ]�� [ %s ]���� ������ �Ͼ���ϴ�. ��� ������ ���￡ ������ �ֽñ� �ٶ��ϴ�."

	switch (WarfieldCode)
	{
	case 0: //����
		/*
		[142.216]-2�����ݰ��
		[221.202]-1�����ݰ��
		[251.103]-3�����ݰ��
		[94.67]-���
		*/
		SMenu[MN_NATION_WAR_START].nField[0].nImageNumber = 4;
		//1 �� ����
		SMenu[MN_NATION_WAR_START].nField[1].x = 18 + 221;
		SMenu[MN_NATION_WAR_START].nField[1].y = 27 + 202;
		//2 �� ����
		SMenu[MN_NATION_WAR_START].nField[2].x = 18 + 142;
		SMenu[MN_NATION_WAR_START].nField[2].y = 27 + 216;
		//3 �� ����
		SMenu[MN_NATION_WAR_START].nField[3].x = 18 + 251;
		SMenu[MN_NATION_WAR_START].nField[3].y = 27 + 103;

		SMenu[MN_NATION_WAR_START].nField[7].x = 18 + 94;
		SMenu[MN_NATION_WAR_START].nField[7].y = 27 + 67;

		break;
	case 1: //���
		/*
		[227.200]-1�����ݰ��
		[225.70]-3�����ݰ��
		[83.218]-2�����ݰ��
		[64.77]-���
		*/
		SMenu[MN_NATION_WAR_START].nField[0].nImageNumber = 6;
		//1 �� ����
		SMenu[MN_NATION_WAR_START].nField[1].x = 18 + 227;
		SMenu[MN_NATION_WAR_START].nField[1].y = 27 + 200;
		//2 �� ����
		SMenu[MN_NATION_WAR_START].nField[2].x = 18 + 225;
		SMenu[MN_NATION_WAR_START].nField[2].y = 27 + 70;
		//3 �� ����
		SMenu[MN_NATION_WAR_START].nField[3].x = 18 + 83;
		SMenu[MN_NATION_WAR_START].nField[3].y = 27 + 218;

		SMenu[MN_NATION_WAR_START].nField[7].x = 18 + 64;
		SMenu[MN_NATION_WAR_START].nField[7].y = 27 + 77;

		break;
	case 2: //��
		/*
		[29.67] - 4�����ݰ��
		[29.7] - 3�����ݰ��
		[258.161]-2�����ݰ��
		[256.228]-1�����ݰ��
		[143.173]-���
		*/
		SMenu[MN_NATION_WAR_START].nField[0].nImageNumber = 5;
		//1 �� ����
		SMenu[MN_NATION_WAR_START].nField[1].x = 18 + 256;
		SMenu[MN_NATION_WAR_START].nField[1].y = 27 + 228;
		//2 �� ����
		SMenu[MN_NATION_WAR_START].nField[2].x = 18 + 258;
		SMenu[MN_NATION_WAR_START].nField[2].y = 27 + 161;
		//3 �� ����
		SMenu[MN_NATION_WAR_START].nField[3].x = 18 + 29;
		SMenu[MN_NATION_WAR_START].nField[3].y = 27 + 7;

		SMenu[MN_NATION_WAR_START].nField[4].x = 18 + 29;
		SMenu[MN_NATION_WAR_START].nField[4].y = 27 + 67;

		SMenu[MN_NATION_WAR_START].nField[7].x = 18 + 143;
		SMenu[MN_NATION_WAR_START].nField[7].y = 27 + 173;

		break;
	}//0 ���� 1 ��� 2 ��

	SMenu[MN_NATION_WAR_START].nField[iLoadingPonint].nWillDo = 1;//���ݷ� ���� //Ŭ�� �Ұ�

	if (Hero->name_status.nation == NW_YL)
	{
		if (Hero->NWCharacter.YL_JoinNation == DefenceNation)	//�����
		{
			AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 114), tempDefNa, tempField); //"[ %s ]������ [ %s ]���� ������ ���۵ƽ��ϴ�. ��� ���ε��� ���￡ ������ �ֽñ� �ٶ��ϴ�."
			sprintf(SMenu[MN_NATION_WAR_START].nField[5].temp, lan->OutputMessage(0, 147), tempAttNa, tempField);//����ڲ� ���� " [ %s ]�������� [ %s ]�� ���ѱ� ���� ������ �����׽��ϴ�. \n ������ �������κ��� ���� ��Ű�� ���ؼ��� ���ȿ� �ִ� ��ȣ���� �μ����� �ʵ��� ���Ѿ� �մϴ�. \n �����Ϳ��� ������ ��쿡 [[[ /��Ȱ ]]]�Ͻø� �ڵ����� ��Ƴ��� �˴ϴ�."
			CallSmallMenu(MN_NATION_WAR_START);// ���ڰ� �־�� ��..
		}
		else
		{
			//010927 lsw
			sprintf(SMenu[MN_NATION_WAR_START].nField[5].temp, lan->OutputMessage(0, 148), tempDefNa, tempField);//�����ڲ� ���� " [ %s ]������ [ %s ]�� �����ϱ� ���� ������ ���۵ƽ��ϴ�. \n �����͸� �����ϱ� ���ؼ��� ���ȿ� �ִ� ��ȣ�� 4���� �μ� �Ŀ� ���� �ȿ� �ִ� ���� ��ȣ���� �ν��߸� �մϴ�.\n �����Ϳ��� ������ ��쿡 [[[ /��Ȱ ]]]�Ͻø� �ڵ����� ��Ƴ��� �˴ϴ�."
			AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 115), tempDefNa, tempField); //"[ %s ]������ [ %s ]�� �Ѱ����� �����߽��ϴ�."
		}
	}

	if (Hero->name_status.nation == DefenceNation) //��
	{
		sprintf(SMenu[MN_NATION_WAR_START].nField[5].temp, lan->OutputMessage(0, 147), tempAttNa, tempField);//����ڲ� ���� " [ %s ]�������� [ %s ]�� ���ѱ� ���� ������ �����׽��ϴ�. \n ������ �������κ��� ���� ��Ű�� ���ؼ��� ���ȿ� �ִ� ��ȣ���� �μ����� �ʵ��� ���Ѿ� �մϴ�. \n �����Ϳ��� ������ ��쿡 [[[ /��Ȱ ]]]�Ͻø� �ڵ����� ��Ƴ��� �˴ϴ�."
		AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 114), tempDefNa, tempField); //"[ %s ]������ [ %s ]���� ������ ���۵ƽ��ϴ�. ��� ���ε��� ���￡ ������ �ֽñ� �ٶ��ϴ�."
		CallSmallMenu(MN_NATION_WAR_START);	// �̶��� �ٸ��� 
	}
	else //���ݱ�
	{
		sprintf(SMenu[MN_NATION_WAR_START].nField[5].temp, lan->OutputMessage(0, 148), tempDefNa, tempField);//�����ڲ� ���� " [ %s ]������ [ %s ]�� �����ϱ� ���� ������ ���۵ƽ��ϴ�. \n �����͸� �����ϱ� ���ؼ��� ���ȿ� �ִ� ��ȣ�� 4���� �μ� �Ŀ� ���� �ȿ� �ִ� ���� ��ȣ���� �ν��߸� �մϴ�.\n �����Ϳ��� ������ ��쿡 [[[ /��Ȱ ]]]�Ͻø� �ڵ����� ��Ƴ��� �˴ϴ�."
		AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 115), tempDefNa, tempField); //"[ %s ]������ [ %s ]�� �Ѱ����� �����߽��ϴ�."
		CallSmallMenu(MN_NATION_WAR_START);
	}

}
//011011 lsw <
//011008 lsw <

void ProcessBBS_COMMANDER_ELECT(t_packet* p, char* Message)
{
	char temp[MAX_STRING_PK];

	memcpy(temp, p->u.NationWar.WarBBS.Arg, p->u.NationWar.WarBBS.Size);
	*(temp + p->u.NationWar.WarBBS.Size) = '\0';

	LPCHARACTER ch = ExistHe(temp);
	if (ch != NULL) InsertMagic(ch, ch, 452, 9, 0, 0, ch->x, ch->y);
	if (temp != NULL)	AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 109), temp); //"�̹� ������ ��ɰ����� --==[ %s ]==--�� ����ƽ��ϴ�."
}

void ProcessBBS_SQUAD_LEADER_ELECT(t_packet* p, char* Message)
{
	char temp[MAX_STRING_PK];

	memcpy(temp, p->u.NationWar.WarBBS.Arg, p->u.NationWar.WarBBS.Size);
	*(temp + p->u.NationWar.WarBBS.Size) = '\0';

	LPCHARACTER ch = ExistHe(temp);
	if (ch != NULL) InsertMagic(ch, ch, 452, 9, 0, 0, ch->x, ch->y);
	AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 196), temp); //"< %s >�� �δ����� �ƽ��ϴ�. �������� �δ����� �ӹ��� �Ͻð� �˴ϴ�."
}

void ProcessBBS_GUARDSTONE_BROKED(t_packet* p, char* Message)	// 011022 LTS �Լ� ��ü 
{
	int ServerID;
	WORD Index, Locationx, Locationy;

	memcpy(&ServerID, &p->u.NationWar.WarBBS.Arg[1], sizeof(int));
	memcpy(&Locationx, &p->u.NationWar.WarBBS.Arg[5], sizeof(WORD));
	memcpy(&Locationy, &p->u.NationWar.WarBBS.Arg[7], sizeof(WORD));
	memcpy(&Index, &p->u.NationWar.WarBBS.Arg[9], sizeof(WORD));

	LPCHARACTER temp = FindHim(ServerID + 10000);
	if (temp != NULL)
	{
		temp->GuardStoneAni = 1;		// h_DisPlayNPC()���� ���ϸ��̼��� ó���ϱ� ���� ������ 
		temp->SetAni2Zero = 1;
		temp->sealstoneani = 0;
	}
	PlayListAutoSounds(260, 0, 0, 0);
	//	else PlayListAutoSounds(261,0,0,0);
	g_GameInfo.displayOption = DISPLAY_EARTHQUAKE;  g_GameInfo.displayOptionDelay = 40;	// ���� ó���κ�..
	AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 262), Index, Locationx, Locationy); //"�������� [ %d�� ] ��ȣ���� ������ �޾� �μ������ϴ�."
}

void ProcessBBS_CASTLEGATEWAY_BROKED(t_packet* p, char* Message)		// 011022 LTS �Լ���ü 
{
	int ServerID;
	WORD Index, Locationx, Locationy;

	memcpy(&ServerID, &p->u.NationWar.WarBBS.Arg[1], sizeof(int));
	memcpy(&Locationx, &p->u.NationWar.WarBBS.Arg[5], sizeof(WORD));
	memcpy(&Locationy, &p->u.NationWar.WarBBS.Arg[7], sizeof(WORD));
	memcpy(&Index, &p->u.NationWar.WarBBS.Arg[9], sizeof(WORD));

	PlayListAutoSounds(262, 0, 0, 0);
	LPCHARACTER temp = FindHim(ServerID + 10000);
	if (temp != NULL)
	{
		InsertMagic(temp, temp, 453, 4, 0, 0, temp->x, temp->y);
		temp->GuardStoneAni = 1;		// h_DisPlayNPC()���� ���ϸ��̼��� ó���ϱ� ���� ������ 
		temp->SetAni2Zero = 1;
		temp->sealstoneani = 0;
	}
	g_GameInfo.displayOption = DISPLAY_EARTHQUAKE;  g_GameInfo.displayOptionDelay = 40;	// KR-LAN�ٲ�
	AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 263), Index, Locationx, Locationy); //"�������� [ %d�� ] ������ ������ �޾� �μ������ϴ�. ������ �μ��� �� ������ ħ���� �����ϰ� �˴ϴ�."
}

void ProcessBBS_COMMANDER_DEATH(t_packet* p, char* Message)
{
	char temp[MAX_STRING_PK];

	memcpy(temp, p->u.NationWar.WarBBS.Arg, p->u.NationWar.WarBBS.Size);
	*(temp + p->u.NationWar.WarBBS.Size) = '\0';

	switch (p->u.NationWar.WarBBS.LanNo)
	{
	case 0: AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 264), temp); break; //"[���ݱ���] ��ɰ� [ %s ]���� ����Ͽ����ϴ�."
	case 1: AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 265), temp); break; //"[���ݱ���] �δ��� [ %s ]���� ����Ͽ����ϴ�."
	case 2: AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 266), temp); break; //"[����] ��ɰ� [ %s ]���� ����Ͽ����ϴ�."
	case 3: AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 267), temp); break; //"[����] �δ��� [ %s ]���� ����Ͽ����ϴ�."
	}

	switch (p->u.NationWar.WarBBS.LanNo)
	{
	case 0:
	case 1:
		if (isAttacker()) PlayListAutoSounds(259, 0, 0, 0);
		else PlayListAutoSounds(51, 0, 0, 0);
		break;
	case 2:
	case 3:
		if (isAttacker()) PlayListAutoSounds(51, 0, 0, 0);
		else PlayListAutoSounds(259, 0, 0, 0);
		break;
	}
}

void ProcessBBS_COMMANDER_CHAT(t_packet* p, char* Message)
{
	char temp[MAX_STRING_PK];

	memcpy(temp, p->u.NationWar.WarBBS.Arg, p->u.NationWar.WarBBS.Size);
	*(temp + p->u.NationWar.WarBBS.Size) = '\0';

	AddCurrentStatusMessage(255, 255, 0, "%s", temp);
	PlayListAutoSounds(258, 0, 0, 0);
}

void ProcessBBS_WAR_VICTORY(t_packet* p, char* Message)
{
	int VictoryNation = p->u.NationWar.WarBBS.Arg[0];
	NationSupportRecvNationWarResult(p);
	int WarfieldNo = g_pNation->GetStatus(NW_WAR);
	g_pNation->SetStatus(WarfieldNo, NW_PEACE);
	if (Hero->name_status.nation == NW_YL)
	{
		if (Hero->NWCharacter.YL_JoinNation == VictoryNation)
		{
			PlayListAutoSounds(273, 0, 0, 0);	// �¸����� 
		}
		else
		{
			PlayListAutoSounds(274, 0, 0, 0);	// �й� ���� 
		}
		return;
	}
	if (Hero->name_status.nation == VictoryNation)
	{
		PlayListAutoSounds(273, 0, 0, 0);
	}
	else
	{
		PlayListAutoSounds(274, 0, 0, 0);
	}
}

void ProcessBBS_BONUS_OPEN(t_packet* p, char* Message)
{
	char temp[80];
	ReturnWarfieldName(p->u.NationWar.WarBBS.WarfieldNo, temp);
	PlayListAutoSounds(62, 0, 0, 0);		// 010927 LTS
	AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 192), temp); //"< %s > ��ȣ���� ���� ������ ��������� ���Ƚ��ϴ�."
}

void ProcessBBS_KILLER(t_packet* p, char* Message)
{
	char temp[MAX_STRING_PK];

	memcpy(temp, p->u.NationWar.WarBBS.Arg, p->u.NationWar.WarBBS.Size);
	*(temp + p->u.NationWar.WarBBS.Size) = '\0';

	LPCHARACTER ch = ExistHe(temp);
	if (ch != NULL) InsertMagic(ch, ch, 451, 9, 0, 0, ch->x, ch->y);
}

void ProcessBBS_NATION_MONEY(t_packet* p, char* Message)		// 010927 LTS	// 011022 LTS �Լ� ��ü 
{
	char temp[80], WarfieldName[80];	// 011022 LTS
	char Nation = p->u.NationWar.WarBBS.Arg[0];
	char WarfieldNo = p->u.NationWar.WarBBS.Arg[1];	// 011022 LTS
	int Size = p->u.NationWar.WarBBS.Size - 1;

	ReturnWarfieldName(WarfieldNo, WarfieldName);

	memcpy(temp, &p->u.NationWar.WarBBS.Arg[2], Size);	// 011022 LTS
	*(temp + Size) = '\0';


	if (Hero->name_status.nation == Nation)	//�й豹 �̸�
	{
		if (p->u.NationWar.WarBBS.BBSType == BBS_NATION_MONEY_USE)	// ���� ���
		{
			switch (p->u.NationWar.WarBBS.LanNo)
			{
			case 0:
			{
				char temp2[80];
				ReturnWarfieldName(p->u.NationWar.WarBBS.WarfieldNo, temp2);
				AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 268), temp2, temp); //"������ �й�� [ %s ]�� ������ ������ �Ǿ���, �������� ���� < %s Crit >�� ���Ѱ���ϴ�."
				break;	// 011020 LTS
			}
			case 1: AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 269), temp); break; //"��ɰ��� ������ �������� [ %s Crit]�� �ݾ��� �Ҹ��Ͽ����ϴ�."
			case 2: AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 270), temp); break; //"��ɰ��� ���� �������� [ %s Crit]�� �ݾ��� �Ҹ��Ͽ����ϴ�."
			}
		}
		else	// ���� ���� ���ߴ�
		{
			switch (p->u.NationWar.WarBBS.LanNo)
			{
			case 0: AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 281), WarfieldName); break; //"������ �й�� [ %s ]�� ������ ������ ������ ���� �ڱ����� ����� ����ִ� �����Դϴ�."
			case 1: AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 271), temp); break; //"��ɰ��� ������ �������� [ %s Crit]�� �ݾ��� �Ҹ��Ͽ��� ������ ����� �����մϴ�."
			case 2: AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 272), temp); break; //"��ɰ��� ���� �������� [ %s Crit]�� �ݾ��� �Ҹ��Ͽ��� ������ ����� �����մϴ�."
			}
		}
	}
	else	//�¸���
	{
		if (Hero->name_status.nation != NW_YL)
		{
			if (p->u.NationWar.WarBBS.BBSType == BBS_NATION_MONEY_USE)	// ���� ���
			{
				switch (p->u.NationWar.WarBBS.LanNo)
				{
				case 0:
				{
					char temp2[80];		//�ݾ��� %s�� �Ǿ� �ִ�. -> ���߿� %d�� �ٲ۴�. // ���װ� ���Դ�.
					ReturnWarfieldName(p->u.NationWar.WarBBS.WarfieldNo, temp2);
					AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 273), temp2, temp); //"������ �¸��� [ %s ]�� �ڱ��� ������ �Ǿ���, �������� ���� < %s Crit >�� ���Ҿҽ��ϴ�."
					break;
				}
				case 1: AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 269), temp); break; //"��ɰ��� ������ �������� [ %s Crit]�� �ݾ��� �Ҹ��Ͽ����ϴ�."
				case 2: AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 270), temp); break; //"��ɰ��� ���� �������� [ %s Crit]�� �ݾ��� �Ҹ��Ͽ����ϴ�."
				}
			}
			else				// ���� ���ߴ�
			{
				switch (p->u.NationWar.WarBBS.LanNo)
				{
				case 0: AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 280), WarfieldName); break; // "������ �¸��� [ %s ]�� �ڱ��� ������ ������ ������ ����� �� ����־� ����� ������ ���߽��ϴ�."
				case 1: AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 271), temp); break; //"��ɰ��� ������ �������� [ %s Crit]�� �ݾ��� �Ҹ��Ͽ��� ������ ����� �����մϴ�."
				case 2: AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 272), temp); break; //"��ɰ��� ���� �������� [ %s Crit]�� �ݾ��� �Ҹ��Ͽ��� ������ ����� �����մϴ�."
				}
			}
		}
	}
}

// BBS Packet Processing Routine End_______________________________________________________


//010924 lsw
void RecvCMD_WAR_BBS(t_packet* p)						// ���� BBS���� ������ ó���Ѵ�.
{
	char msg[500];
	int Type = 0;
	//sprintf(msg,lan->OutputMessage(0,300)); //"0��������         ������ ���̻� ���� ������ �����ϴ�."

	TodayNoticeButtonIntoColor(2);


	switch (p->u.NationWar.WarBBS.BBSType)
	{
	case BBS_GENERAL:	ProcessBBS_GENERAL(p, msg);					break;
	case BBS_PEACE:	ProcessBBS_PEACE(p, msg);					break;
	case BBS_WAR_JOIN_VOTE:	ProcessBBS_WAR_JOIN_VOTE(p, msg);			break;
	case BBS_WAR_PREPARE:	ProcessBBS_WAR_PREPARE(p, msg);				break;
	case BBS_WAR_BEGIN:	ProcessBBS_WAR_BEGIN(p, msg);				break;
	case BBS_COMMANDER_ELECT:	ProcessBBS_COMMANDER_ELECT(p, msg);			break;
	case BBS_SQUADLEADER_ELECT:	ProcessBBS_SQUAD_LEADER_ELECT(p, msg);		break;
	case BBS_GUARDSTONE_BROKED:	ProcessBBS_GUARDSTONE_BROKED(p, msg);		break;
	case BBS_CASTLEGATEWAY_BROKED:	ProcessBBS_CASTLEGATEWAY_BROKED(p, msg);		break;
	case BBS_COMMANDER_DEATH:	ProcessBBS_COMMANDER_DEATH(p, msg);			break;
	case BBS_COMMANDER_CHAT:	ProcessBBS_COMMANDER_CHAT(p, msg);			break;
		//		case BBS_WAR_DEFEAT :
	case BBS_WAR_VICTORY:	ProcessBBS_WAR_VICTORY(p, msg);				break;
	case BBS_BONUS_OPEN:	ProcessBBS_BONUS_OPEN(p, msg);				break;
	case BBS_KILLER:	ProcessBBS_KILLER(p, msg);					break;
	case BBS_NATION_MONEY_USE:
	case BBS_NATION_MONEY_NOT_USE:	ProcessBBS_NATION_MONEY(p, msg);				break;
	}
	//IntoNationNotice(msg);

	return;
}

void SendCMD_REQUEST_NATION_WAR_COUNT()
{
	t_packet p;

	p.h.header.type = CMD_REQUEST_NATION_WAR_COUNT;
	p.h.header.size = 0;
	QueuePacket(&p, 1);
}

void RecvCMD_ANSWER_NATION_WAR_COUNT(t_packet* p)
{
	Hero->NWCharacter.WarNo = p->u.NationWar.CommonDataDw.Data;
}

void SendCMD_NW_MAP_MOVE(int Type, int WarfieldNo)	// 011022 LTS		// type : 0 : old, 1:New
{
	t_packet p;

	//< LTH-040226-KO 1.4 ���� ������ �Ͻ��� ������ �ȴٸ� ���￡ �ܵ� ���� ����
	if (!IsNeoWarfield(WarfieldNo))
	{
		if (Hero->name_status.nation == NW_YL)
		{
			if (Hero->NWCharacter.YL_JoinNation == 0)
			{
				CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 274), FALSE); //"�����ͷ� �̵��� �� �ִ� ��Ȳ�� �ƴմϴ�."
				return;
			}
		}
	}
	//> LTH-040226-KO

	if (Hero->lv <= 10) // 011022 LTS
	{
		CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 282), FALSE);
		return;
	}

	SendCMD_REQUEST_CHECK_WARFIELD_POSSESSION(0);

	p.h.header.type = CMD_NW_MAP_MOVE;
	if (Type == 1)
		p.u.NationWar.NWMapMove.WarfieldNo = WarfieldNo;		// LTH-040226-KO 1.4 ���Ŀ��� ��ĥ�ʿ� ����. �����ȣ 4�� �����´�
	else
		p.u.NationWar.NWMapMove.WarfieldNo = WarfieldNo - 1;
	p.u.NationWar.NWMapMove.SquadNo = Hero->NWCharacter.SquadNo;
	p.h.header.size = sizeof(t_NWMapMove);
	QueuePacket(&p, 1);
}

void RecvCMD_NW_MAP_MOVE_FAIL(t_packet* p)			// 011213 LTS
{
	switch (p->u.NationWar.CommonDataC.Data)
	{
	case 1: CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 329), FALSE); break;
	case 2: CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 330), FALSE); break;
	case 3: CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 331), FALSE); break;
	case 4: CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 332), FALSE); break;
	case 5: CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 288), FALSE); break;
	case 6: CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 616), FALSE); break;
	case 7: CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 626), FALSE); break;	// "������ ���۵��� 30���� ���� �̵��� �� �����ϴ�"
	}
}

void RecvCMD_ANSWER_TILE_DONT(t_packet* p)
{
	g_pNation->TileDont(p->u.NationWar.TileDont.WarfieldNo,
		p->u.NationWar.TileDont.TypeNumber,
		p->u.NationWar.TileDont.Value);
}

void RecvCMD_TILE_DONT_CHANGED(t_packet* p)
{
	g_pNation->TileDont(p->u.NationWar.TileDont.WarfieldNo,
		p->u.NationWar.TileDont.TypeNumber,
		p->u.NationWar.TileDont.Value);
}

void RecvCMD_CLEAR_SQUAD_DATA(t_packet* p)			// 011020 LTS
{
	if (isNationWarfieldServer())
	{
		if (Hero->name_status.nation != NW_YL) memset(&Hero->NWCharacter, 0, sizeof(DWORD));
	}
	else memset(&Hero->NWCharacter, 0, sizeof(DWORD));		// 010915 LTS
}

void RecvCMD_ANSWER_WARFIELD_INFO(t_packet* p)
{
	g_AttackNation = p->u.NationWar.WarfieldInfo.AttackWarfieldNo;
	g_DefenceNation = p->u.NationWar.WarfieldInfo.DefenceWarfieldNo;
}


void RecvCMD_ANSWER_CHECK_WARFIELD_POSSESSION(t_packet* p)
{
	bool AllPossession = true;
	int WarfieldNo;

	switch (p->u.NationWar.WarfieldPossession.RequestType)
	{
	case 1:	//�Ͻ� �δ����
	{
		int PossessionNation = p->u.NationWar.WarfieldPossession.Possession[0];
		if (PossessionNation != p->u.NationWar.WarfieldPossession.Possession[1]) AllPossession = false;
		if (PossessionNation != p->u.NationWar.WarfieldPossession.Possession[2]) AllPossession = false;
		if (AllPossession)
		{
			WarfieldNo = CheckNationStatus(NW_PREPARE);
			if (WarfieldNo >= 0)
			{
				char msg[80];
				ReturnWarfieldName(WarfieldNo, msg);
				AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 247), msg); //"[ %s ]�� ���￡ ���� �� �δ븦 �����ϰ� �ֽ��ϴ�."
				SendCMD_REQUEST_SQUAD_INFO(g_Squad);			// �������̽��� ȭ��ǥ�� ���� ���ڰ� �����ȴ�.. ��ó���� 0�̴�.	
			}
			else CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 249), FALSE); //"[�����غ���] �Ⱓ���� ����� �� �ֽ��ϴ�."
		}
		else CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 256), FALSE); //"�뺴�δ븦 ����� �� �ִ� ��Ȳ�� �ƴմϴ�."
		break;
	}
	case 3:	// ���� �÷��� 
	{
		if (Hero->name_status.nation == NW_YL)
		{
			if (Hero->NWCharacter.YL_JoinNation ==
				p->u.NationWar.WarfieldPossession.Possession[MapInfo[MapNumber].port - BASE_WARFIELD_PORT])
			{
				PlayListAutoSounds(7, 0, 0, 1);	//���� ����
			}
			else
			{
				PlayListAutoSounds(8, 0, 0, 1);	//���ݱ��� ����
			}
			return;
		}
		if (Hero->name_status.nation ==
			p->u.NationWar.WarfieldPossession.Possession[MapInfo[MapNumber].port - BASE_WARFIELD_PORT])
		{
			PlayListAutoSounds(7, 0, 0, 1);	//���� ����
		}
		else
		{
			PlayListAutoSounds(8, 0, 0, 1);	//���ݱ��� ����
		}
		break;
	}

	case 4:	// ��������
	{
		int PossessionNation = p->u.NationWar.WarfieldPossession.Possession[0];
		if (PossessionNation != p->u.NationWar.WarfieldPossession.Possession[1]) AllPossession = false;
		if (PossessionNation != p->u.NationWar.WarfieldPossession.Possession[2]) AllPossession = false;
		if (!AllPossession)
		{
			if (Hero->NWCharacter.YL_JoinNation == 0) SendCMD_FREELANCER_SQUAD_JOIN(NW_BY);  // define
			else CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 257), FALSE); //"�̹� �뺴���� ������ ������ �����߽��ϴ�."
		}
		else CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 283), FALSE); //"�뺴�δ븦 ����� �� �ִ� ��Ȳ�� �ƴմϴ�."	// 011025 LTS
		break;
	}
	case 5:	// �������� 
	{
		int PossessionNation = p->u.NationWar.WarfieldPossession.Possession[0];
		if (PossessionNation != p->u.NationWar.WarfieldPossession.Possession[1]) AllPossession = false;
		if (PossessionNation != p->u.NationWar.WarfieldPossession.Possession[2]) AllPossession = false;
		if (!AllPossession)
		{
			if (Hero->NWCharacter.YL_JoinNation == 0) SendCMD_FREELANCER_SQUAD_JOIN(NW_ZY);
			else CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 257), FALSE); //"�̹� �뺴���� ������ ������ �����߽��ϴ�."
		}
		else CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 283), FALSE); //"�뺴�δ븦 ����� �� �ִ� ��Ȳ�� �ƴմϴ�."	// 011025 LTS
		break;
	}
	}
}

void RecvCMD_ANSWER_WARFIELD_POSSESSION(t_packet* p)
{
	// ���⼭ ����â�� ����
	int Possession1 = p->u.NationWar.WarfieldPossession.Possession[0];
	int Possession2 = p->u.NationWar.WarfieldPossession.Possession[1];
	int Possession3 = p->u.NationWar.WarfieldPossession.Possession[2];

	g_pNation->SetWarfieldPoss(0, Possession1);
	g_pNation->SetWarfieldPoss(1, Possession2);
	g_pNation->SetWarfieldPoss(2, Possession3);
	//������. 
	//	AddCurrentStatusMessage( 255,255,0, "��������� : %d, ���������� : %d , �������� : %d",Possession1,Possession2,Possession3);
	sprintf(SMenu[MN_NATION_DONEBOX].nField[1].temp, lan->OutputMessage(0, 149), NationName[Possession1], NationName[Possession2], NationName[Possession3]); //" <<<<<<  ���� ������ ���� ���� >>>>>> \n  <<���� ������>> : [ %s ] \n  <<��� ������>> : [ %s ] \n  << ��  ������>> : [ %s ] \n ���� ���� ��� : ���� ������ ���� ���� ��ǥ���� ���� ��ǥ�� ������ �� ������ ���� ������ �Ǹ�, 20�� �Ŀ� ������ ���۵˴ϴ�. \n ���� ���� ��� : ���� ���� �Ŀ� ���̼����� ������ �δ� ����� ���� ���￡ �����ϰ� �Ǹ�, �Ͻ��� �뺴���μ� ���� ������ �����մϴ�."

	CallSmallMenu(MN_NATION_DONEBOX, MN_NATION_BATTLE_BBS);
}

void RecvCMD_WARFIELD_WAR_DATA1(t_packet* p)
{
	//������ ��Ȳ
	char  GuardStoneSize = p->u.data[0];
	char  CastleSize = p->u.data[GuardStoneSize + 1];
	char* GuardStoneStatus = new char[GuardStoneSize];
	char* CastleStatus = new char[CastleSize];

	memcpy(GuardStoneStatus, (p->u.data + 1), GuardStoneSize);
	memcpy(CastleStatus, (p->u.data + GuardStoneSize + 2), CastleSize);
	NationSupportNationNow1(p);//010910 lsw

	SAFE_DELETE(GuardStoneStatus);
	SAFE_DELETE(CastleStatus);
}
void RecvCMD_WARFIELD_WAR_DATA2(t_packet* p)
{
	DWORD RemainTime = p->u.NationWar.WarfieldData2.RemainTime;
	WORD  Hour, Min, Sec;	// 011020 LTS
	GetTime(RemainTime, Hour, Min, Sec);
	NationSupportNationNow2(p);//010910 lsw
}

void RecvCMD_WARFIELD_WAR_DATA3(t_packet* p)
{
	NationSupportNationNow3(p);//010910 lsw
}

void RecvCMD_CLEAR_WAR_CHARACTER_DATA(t_packet* p)
{
	memset(&Hero->NWCharacter, 0, sizeof(DWORD));
	Hero->NWCharacter.WarNo = p->u.NationWar.CommonDataC.Data;
}

void RecvCMD_YOU_ARE_SQUAD_COMMANDER(t_packet* p)
{
	Hero->NWCharacter.isCommander = 1;
	if (Hero->name_status.nation == NW_YL) Hero->NWCharacter.YL_JoinNation = p->u.NationWar.CommonDataC.Data;
	AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 276)); //"[��ɰ�]�� �缱�ƽ��ϴ�. ���￡�� ��ɰ��� �ӹ��� �����ϰ� �˴ϴ�."
	InsertMagic(Hero, Hero, 451, 9, 0, 0, Hero->x, Hero->y);
}


void SendRequestTileDontData()								// � �ʿ��� ó�� ������ ��Ŷ�� ������ ���̴�.
{
	if (isNationWarfieldServer())
	{
		t_packet packet;
		packet.h.header.type = CMD_REQUEST_TILE_DONT;
		packet.h.header.size = 0;
		QueuePacket(&packet, 1);
		SendCMD_REQUEST_CHECK_WARFIELD_POSSESSION(3);		// ���带 �÷��� �Ѵ�.
		SendCMD_REQUEST_WARFIELD_INFO();					// ���ݱ��� ���� Ȯ�� �ϱ� ���ؼ� ������
	}

	SendNationPacket(BUTTON_NATION_NOTICE);					// ������ ������ ���� 

	//< LTH-040302-KO 1.4 Patch Snowy Warfield same packet
	if (isNewWarfieldServer() || IsNeoWarfieldServer())
	{
		t_packet packet;
		packet.h.header.type = CMD_REQUEST_NEW_WARFIELD_TILE_DONT;
		packet.h.header.size = 0;
		QueuePacket(&packet, 1);
		PlayListAutoSounds(7, 0, 0, 1);
	}
	//> LTH-040302-KO

	//< LTH-040302-KO 1.4 Patch Add!
	if (!isNationWarfieldServer() && !isNewWarfieldServer() && !IsNeoWarfieldServer())
	{
		
	}
	//> LTH-040302-KO
}

void RecvCMD_SQUAD_JOIN_RESULT(t_packet* p)
{
	switch (p->u.NationWar.CommonDataC.Data)
	{
	case 0: CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 253), FALSE); break; //"�δ뿡 ����� �� �ƽ��ϴ�."
	case 1: CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 277), FALSE); break; //"[�δ�]�� �Ҽӵƽ��ϴ�."
	case 2: CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 278), FALSE); break; //"[�δ���]�� �Ǳ� ���� [�ɷ�ġ]�� �����մϴ�."
	case 3: CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 279), FALSE); break; //"�δ�� ���� ��� á�� ������ �δ뿡 ����Ͻ� �� �����ϴ�."
	}

}
void ProcessNoticeJoinA(t_packet* p)
{
	char tempMessage[500];
	char WarfieldName[20];
	WORD Hour, Min, Sec;	// 011020 LTS
	ReturnWarfieldName(p->u.NationWar.NoticeJoinVoteA.WarfieldNo, WarfieldName);
	GetTime(p->u.NationWar.NoticeJoinVoteA.RemainTime, Hour, Min, Sec);
	sprintf(tempMessage, lan->OutputMessage(0, 301), WarfieldName, p->u.NationWar.NoticeJoinVoteA.VoteCount, Hour, Min);
	IntoNationNotice(ALL_CLEAR, 1, NULL);
	IntoNationNotice(FORCE_TYPE, 1, tempMessage);
	CallSmallMenu(MN_NATION_TODAY_NOTICE);
}

void ProcessNoticeJoinD(t_packet* p)
{
	char tempMessage[500];
	char NationName[20];
	ReturnNationName(p->u.NationWar.NoticeJoinVoteD.Nation, NationName);
	sprintf(tempMessage, lan->OutputMessage(0, 302), NationName);
	IntoNationNotice(ALL_CLEAR, 1, NULL);
	IntoNationNotice(FORCE_TYPE, 1, tempMessage);
	CallSmallMenu(MN_NATION_TODAY_NOTICE);
}

void ProcessNoticeJoinReinforce(t_packet* p)
{
	char tempMessage[500];
	char NationName[20];
	ReturnNationName(p->u.NationWar.NoticeJoinVoteD.Nation, NationName);
	sprintf(tempMessage, lan->OutputMessage(0, 303), NationName);
	IntoNationNotice(ALL_CLEAR, 1, NULL);
	IntoNationNotice(FORCE_TYPE, 1, tempMessage);
	CallSmallMenu(MN_NATION_TODAY_NOTICE);
}

void ProcessPrepare1(t_packet* p)
{
	char tempMessage[500];
	IntoNationNotice(ALL_CLEAR, 1, NULL);
	sprintf(tempMessage, lan->OutputMessage(0, 304), p->u.NationWar.NoticePrepare1.LeadCandidaterName);
	IntoNationNotice(FORCE_TYPE, 1, tempMessage);
	char WarfieldName[20];
	WORD Hour, Min, Sec;	// 011020 LTS
	ReturnWarfieldName(p->u.NationWar.NoticePrepare1.WarfieldNo, WarfieldName);
	GetTime(p->u.NationWar.NoticePrepare1.RemainTime, Hour, Min, Sec);
	sprintf(tempMessage, lan->OutputMessage(0, 305), WarfieldName, Hour, Min);
	IntoNationNotice(FORCE_TYPE, 2, tempMessage);
	CallSmallMenu(MN_NATION_TODAY_NOTICE);
}

void ProcessPrepare1Reinforce(t_packet* p)
{
	char tempMessage[500];
	char WarfieldName[20];
	WORD Hour, Min, Sec;	// 011020 LTS
	char AttackNationName[20], DefenceNationName[20];
	IntoNationNotice(ALL_CLEAR, 1, NULL);
	ReturnNationName(p->u.NationWar.NoticePrepare1Reinforce.AttackNation, AttackNationName);
	ReturnNationName(p->u.NationWar.NoticePrepare1Reinforce.DefenceNation, DefenceNationName);
	ReturnWarfieldName(p->u.NationWar.NoticePrepare1Reinforce.WarfieldNo, WarfieldName);
	GetTime(p->u.NationWar.NoticePrepare1Reinforce.RemainTime, Hour, Min, Sec);
	sprintf(tempMessage, lan->OutputMessage(0, 306), AttackNationName, DefenceNationName, WarfieldName, Hour, Min);
	IntoNationNotice(FORCE_TYPE, 1, tempMessage);
	CallSmallMenu(MN_NATION_TODAY_NOTICE);
}

void ProcessPrepare2(t_packet* p)
{
	char tempMessage[500];
	char WarfieldName[20];
	WORD Hour, Min, Sec;	// 011020 LTS
	IntoNationNotice(ALL_CLEAR, 1, NULL);
	ReturnWarfieldName(p->u.NationWar.NoticePrepare2.WarfieldNo, WarfieldName);
	GetTime(p->u.NationWar.NoticePrepare2.RemainTime, Hour, Min, Sec);
	sprintf(tempMessage, lan->OutputMessage(0, 307), p->u.NationWar.NoticePrepare2.CommanderName);
	IntoNationNotice(FORCE_TYPE, 1, tempMessage);
	sprintf(tempMessage, lan->OutputMessage(0, 305), WarfieldName, Hour, Min);
	IntoNationNotice(FORCE_TYPE, 2, tempMessage);
	CallSmallMenu(MN_NATION_TODAY_NOTICE);
}

void ProcessPrepare2Reinforce(t_packet* p)
{
	char tempMessage[500];
	char AttackNationName[20], DefenceNationName[20];
	char WarfieldName[20];
	WORD Hour, Min, Sec;	// 011020 LTS
	IntoNationNotice(ALL_CLEAR, 1, NULL);
	ReturnWarfieldName(p->u.NationWar.NoticePrepare2Reinforce.WarfieldNo, WarfieldName);
	ReturnNationName(p->u.NationWar.NoticePrepare2Reinforce.AttackNation, AttackNationName);
	ReturnNationName(p->u.NationWar.NoticePrepare2Reinforce.DefenceNation, DefenceNationName);
	GetTime(p->u.NationWar.NoticePrepare2Reinforce.RemainTime, Hour, Min, Sec);
	sprintf(tempMessage, lan->OutputMessage(0, 306), AttackNationName, DefenceNationName, WarfieldName, Hour, Min);
	IntoNationNotice(FORCE_TYPE, 1, tempMessage);
	CallSmallMenu(MN_NATION_TODAY_NOTICE);
}

void ProcessPrepare3(t_packet* p)
{
	char tempMessage[500];
	char WarfieldName[20];
	WORD Hour, Min, Sec;	// 011020 LTS
	IntoNationNotice(ALL_CLEAR, 1, NULL);
	ReturnWarfieldName(p->u.NationWar.NoticePrepare3.WarfieldNo, WarfieldName);
	GetTime(p->u.NationWar.NoticePrepare3.RemainTime, Hour, Min, Sec);
	sprintf(tempMessage, lan->OutputMessage(0, 304), p->u.NationWar.NoticePrepare3.LeadCandidaterName);
	IntoNationNotice(FORCE_TYPE, 1, tempMessage);
	sprintf(tempMessage, lan->OutputMessage(0, 305), WarfieldName, Hour, Min);
	IntoNationNotice(FORCE_TYPE, 2, tempMessage);
	CallSmallMenu(MN_NATION_TODAY_NOTICE);
}

void ProcessPrepare3Reinforce(t_packet* p)
{
	char tempMessage[500];
	char DefenceNationName[20];
	WORD Hour, Min, Sec;	// 011020 LTS
	IntoNationNotice(ALL_CLEAR, 1, NULL);
	ReturnNationName(p->u.NationWar.NoticePrepare3Reinforce.DefenceNation, DefenceNationName);
	GetTime(p->u.NationWar.NoticePrepare3Reinforce.RemainTime, Hour, Min, Sec);
	sprintf(tempMessage, lan->OutputMessage(0, 308), DefenceNationName, Hour, Min);
	IntoNationNotice(FORCE_TYPE, 1, tempMessage);
	CallSmallMenu(MN_NATION_TODAY_NOTICE);
}

void ProcessPrepare4(t_packet* p)
{
	char tempMessage[500];
	char WarfieldName[20];
	WORD Hour, Min, Sec;	// 011020 LTS
	IntoNationNotice(ALL_CLEAR, 1, NULL);
	ReturnWarfieldName(p->u.NationWar.NoticePrepare4.WarfieldNo, WarfieldName);
	GetTime(p->u.NationWar.NoticePrepare4.RemainTime, Hour, Min, Sec);
	sprintf(tempMessage, lan->OutputMessage(0, 307), p->u.NationWar.NoticePrepare4.CommanderName);
	IntoNationNotice(FORCE_TYPE, 1, tempMessage);
	sprintf(tempMessage, lan->OutputMessage(0, 305), WarfieldName, Hour, Min);
	IntoNationNotice(FORCE_TYPE, 2, tempMessage);
	CallSmallMenu(MN_NATION_TODAY_NOTICE);
}

void ProcessPrepare4Reinforce(t_packet* p)
{
	char tempMessage[500];
	char DefenceNationName[20];
	WORD Hour, Min, Sec;	// 011020 LTS
	IntoNationNotice(ALL_CLEAR, 1, NULL);
	ReturnNationName(p->u.NationWar.NoticePrepare4Reinforce.DefenceNation, DefenceNationName);
	GetTime(p->u.NationWar.NoticePrepare4Reinforce.RemainTime, Hour, Min, Sec);
	sprintf(tempMessage, lan->OutputMessage(0, 308), DefenceNationName, Hour, Min);
	IntoNationNotice(FORCE_TYPE, 1, tempMessage);
	CallSmallMenu(MN_NATION_TODAY_NOTICE);
}

void ProcessWar1A(t_packet* p)
{
	char tempMessage[500];
	char DefenceNationName[20], WarfieldName[20];
	WORD Hour, Min, Sec;	// 011020 LTS
	IntoNationNotice(ALL_CLEAR, 1, NULL);
	ReturnNationName(p->u.NationWar.NoticeWar1A.DefenceNation, DefenceNationName);
	ReturnWarfieldName(p->u.NationWar.NoticeWar1A.WarfieldNo, WarfieldName);
	GetTime(p->u.NationWar.NoticeWar1A.RemainTime, Hour, Min, Sec);
	sprintf(tempMessage, lan->OutputMessage(0, 310), DefenceNationName, WarfieldName, Hour, Min);
	IntoNationNotice(FORCE_TYPE, 1, tempMessage);
	CallSmallMenu(MN_NATION_TODAY_NOTICE);
}

void ProcessWar1D(t_packet* p)
{
	char tempMessage[500];
	char AttackNationName[20], WarfieldName[20];
	WORD Hour, Min, Sec;	// 011020 LTS

	int WarfieldNo = p->u.NationWar.NoticeWar1D.WarfieldNo;
	IntoNationNotice(ALL_CLEAR, 1, NULL);
	if (WarfieldNo >= 0 && WarfieldNo <= 2)
	{
		ReturnNationName(p->u.NationWar.NoticeWar1D.AttackNation, AttackNationName);
		ReturnWarfieldName(p->u.NationWar.NoticeWar1D.WarfieldNo, WarfieldName);
		GetTime(p->u.NationWar.NoticeWar1D.RemainTime, Hour, Min, Sec);
		sprintf(tempMessage, lan->OutputMessage(0, 309), AttackNationName, WarfieldName, Hour, Min);
	}
	else
		if (WarfieldNo >= 3)
		{
			//<! BBD 040315	
			switch (WarfieldNo)
			{
			case WI_HADES_WARFIELD:		// ����������(���� ������)
			{
				//< LTH-040324-KO �ð� ���������� ǥ��...
				g_dwRemainTime = p->u.NationWar.NoticeWar1D.RemainTime + (g_ClientTime / 1000);
				//> LTH-040324-KO
				GetTime(p->u.NationWar.NoticeWar1D.RemainTime, Hour, Min, Sec);
				sprintf(tempMessage, lan->OutputMessage(0, 617), Hour, Min);
				break;
			}
			default:	// ���������� WarfieldNo = 3
			{
				if (Hero->name_status.nation == NW_YL)
				{
					GetTime(p->u.NationWar.NoticeWar1D.RemainTime, Hour, Min, Sec);
					sprintf(tempMessage, lan->OutputMessage(0, 384), Hour, Min);
				}
				else
				{
					GetTime(p->u.NationWar.NoticeWar1D.RemainTime, Hour, Min, Sec);
					sprintf(tempMessage, lan->OutputMessage(0, 385), Hour, Min);
				}
				break;
			}
			}
			//> BBD 040315
		}
	IntoNationNotice(FORCE_TYPE, 1, tempMessage);
	CallSmallMenu(MN_NATION_TODAY_NOTICE);
}

void ProcessWar1Reinforce(t_packet* p)
{
	char tempMessage[500];
	char AttackNationName[20], DefenceNationName[20], WarfieldName[20];
	WORD Hour, Min, Sec;	// 011020 LTS
	IntoNationNotice(ALL_CLEAR, 1, NULL);
	ReturnNationName(p->u.NationWar.NoticeWar1Reinforce.AttackNation, AttackNationName);
	ReturnNationName(p->u.NationWar.NoticeWar1Reinforce.DefenceNation, DefenceNationName);
	ReturnWarfieldName(p->u.NationWar.NoticeWar1Reinforce.WarfieldNo, WarfieldName);
	GetTime(p->u.NationWar.NoticeWar1Reinforce.RemainTime, Hour, Min, Sec);
	sprintf(tempMessage, lan->OutputMessage(0, 311), AttackNationName, DefenceNationName, WarfieldName, Hour, Min);
	IntoNationNotice(FORCE_TYPE, 1, tempMessage);
	sprintf(tempMessage, "%s", lan->OutputMessage(0, 319));
	IntoNationNotice(FORCE_TYPE, 2, tempMessage);
	CallSmallMenu(MN_NATION_TODAY_NOTICE);
}

void ProcessWar2A(t_packet* p)
{
	char tempMessage[500];
	char DefenceNationName[20], WarfieldName[20];
	WORD Hour, Min, Sec;	// 011020 LTS
	IntoNationNotice(ALL_CLEAR, 1, NULL);
	ReturnNationName(p->u.NationWar.NoticeWar2A.DefenceNation, DefenceNationName);
	ReturnWarfieldName(p->u.NationWar.NoticeWar2A.WarfieldNo, WarfieldName);
	GetTime(p->u.NationWar.NoticeWar2A.RemainTime, Hour, Min, Sec);
	sprintf(tempMessage, lan->OutputMessage(0, 322), DefenceNationName, WarfieldName, Hour, Min);
	IntoNationNotice(FORCE_TYPE, 1, tempMessage);
	CallSmallMenu(MN_NATION_TODAY_NOTICE);
}

void ProcessWar2D(t_packet* p)
{
	char tempMessage[500];
	char AttackNationName[20], WarfieldName[20], DefenceNationName[20];
	WORD Hour, Min, Sec; // 011020 LTS
	IntoNationNotice(ALL_CLEAR, 1, NULL);
	ReturnNationName(p->u.NationWar.NoticeWar2D.AttackNation, AttackNationName);
	ReturnNationName(p->u.NationWar.NoticeWar2D.DefenceNation, DefenceNationName);
	ReturnWarfieldName(p->u.NationWar.NoticeWar2D.WarfieldNo, WarfieldName);
	GetTime(p->u.NationWar.NoticeWar2D.RemainTime, Hour, Min, Sec);
	sprintf(tempMessage, lan->OutputMessage(0, 321), DefenceNationName, AttackNationName, Hour, Min, WarfieldName);
	IntoNationNotice(FORCE_TYPE, 1, tempMessage);
	CallSmallMenu(MN_NATION_TODAY_NOTICE);
}

void ProcessWar2Reinforce(t_packet* p)
{
	char tempMessage[500];
	char DefenceNationName[20], WarfieldName[20];
	WORD Hour, Min, Sec;	// 011020 LTS
	IntoNationNotice(ALL_CLEAR, 1, NULL);
	ReturnNationName(p->u.NationWar.NoticeWar2Reinforce.DefenceNation, DefenceNationName);
	ReturnWarfieldName(p->u.NationWar.NoticeWar2Reinforce.WarfieldNo, WarfieldName);
	GetTime(p->u.NationWar.NoticeWar2Reinforce.RemainTime, Hour, Min, Sec);
	sprintf(tempMessage, lan->OutputMessage(0, 323), DefenceNationName, WarfieldName, Hour, Min);
	IntoNationNotice(FORCE_TYPE, 1, tempMessage);
	sprintf(tempMessage, "%s", lan->OutputMessage(0, 320));
	IntoNationNotice(FORCE_TYPE, 2, tempMessage);
	CallSmallMenu(MN_NATION_TODAY_NOTICE);
}

void ProcessPeace1V(t_packet* p)		//011025 LTS
{
	char tempMessage[500];
	char SecretWarfieldName[20];
	WORD Hour1, Min1, Sec1;
	WORD Hour2, Min2, Sec2;
	WORD Hour3, Min3, Sec3;

	IntoNationNotice(ALL_CLEAR, 1, NULL);
	ReturnWarfieldName(p->u.NationWar.NoticeWarPeace1V.SecretWarfieldNo, SecretWarfieldName);
	sprintf(tempMessage, lan->OutputMessage(0, 312), SecretWarfieldName);
	IntoNationNotice(FORCE_TYPE, 1, tempMessage);
	//	ReturnWarfieldName(p->u.NationWar.NoticeWarPeace1V.WarfieldNo,WarfieldName);
	//	sprintf(tempMessage,lan->OutputMessage(0,313),WarfieldName,p->u.NationWar.NoticeWarPeace1V.Money);
	//	IntoNationNotice(FORCE_TYPE,2,tempMessage);
	sprintf(tempMessage, "%s", lan->OutputMessage(0, 317));
	IntoNationNotice(FORCE_TYPE, 2, tempMessage);
	GetTime(p->u.NationWar.NoticeWarPeace1V.Warfield1RemainTime, Hour1, Min1, Sec1);
	GetTime(p->u.NationWar.NoticeWarPeace1V.Warfield2RemainTime, Hour2, Min2, Sec2);
	GetTime(p->u.NationWar.NoticeWarPeace1V.Warfield3RemainTime, Hour3, Min3, Sec3);
	sprintf(tempMessage, lan->OutputMessage(0, 325), Hour1, Min1, Hour2, Min2, Hour3, Min3);
	//<! BBD 040316
	char temp2[500];
	sprintf(temp2, lan->OutputMessage(0, 618), p->u.NationWar.NoticeWarPeace.aLocalWarPoint[0],
		p->u.NationWar.NoticeWarPeace.aLocalWarPoint[1], p->u.NationWar.NoticeWarPeace.aLocalWarPoint[2]);
	sprintf(tempMessage, "%s%s", tempMessage, temp2);
	//> BBD 040316
	IntoNationNotice(FORCE_TYPE, 3, tempMessage);
	CallSmallMenu(MN_NATION_TODAY_NOTICE);
}

void ProcessPeace1D(t_packet* p)		// 011025 LTS
{
	char tempMessage[500];
	WORD Hour1, Min1, Sec1;
	WORD Hour2, Min2, Sec2;
	WORD Hour3, Min3, Sec3;

	IntoNationNotice(ALL_CLEAR, 1, NULL);
	//	ReturnWarfieldName(p->u.NationWar.NoticeWarPeace1D.WarfieldNo,WarfieldName);
	//	sprintf(tempMessage,lan->OutputMessage(0,314),WarfieldName,p->u.NationWar.NoticeWarPeace1D.Money);
	//	IntoNationNotice(FORCE_TYPE,1,tempMessage);
	sprintf(tempMessage, "%s", lan->OutputMessage(0, 317));
	IntoNationNotice(FORCE_TYPE, 1, tempMessage);
	GetTime(p->u.NationWar.NoticeWarPeace1D.Warfield1RemainTime, Hour1, Min1, Sec1);
	GetTime(p->u.NationWar.NoticeWarPeace1D.Warfield2RemainTime, Hour2, Min2, Sec2);
	GetTime(p->u.NationWar.NoticeWarPeace1D.Warfield3RemainTime, Hour3, Min3, Sec3);
	sprintf(tempMessage, lan->OutputMessage(0, 325), Hour1, Min1, Hour2, Min2, Hour3, Min3);
	//<! BBD 040316
	char temp2[500];
	sprintf(temp2, lan->OutputMessage(0, 618), p->u.NationWar.NoticeWarPeace.aLocalWarPoint[0],
		p->u.NationWar.NoticeWarPeace.aLocalWarPoint[1], p->u.NationWar.NoticeWarPeace.aLocalWarPoint[2]);
	sprintf(tempMessage, "%s%s", tempMessage, temp2);
	//> BBD 040316
	IntoNationNotice(FORCE_TYPE, 2, tempMessage);

	CallSmallMenu(MN_NATION_TODAY_NOTICE);

}

void ProcessPeace1Reinforce(t_packet* p)	// 011025 LTS
{
	char tempMessage[500];
	char SecretWarfieldName[20], WarfieldName[20], VictoryNationName[20], DefeatNationName[20];
	WORD Hour1, Min1, Sec1;
	WORD Hour2, Min2, Sec2;
	WORD Hour3, Min3, Sec3;

	IntoNationNotice(ALL_CLEAR, 1, NULL);
	ReturnWarfieldName(p->u.NationWar.NoticeWarPeace1Reinforce.SecretWarfieldNo, SecretWarfieldName);
	ReturnWarfieldName(p->u.NationWar.NoticeWarPeace1Reinforce.WarfieldNo, WarfieldName);
	ReturnNationName(p->u.NationWar.NoticeWarPeace1Reinforce.VictoryNation, VictoryNationName);
	ReturnNationName(p->u.NationWar.NoticeWarPeace1Reinforce.DefeatNation, DefeatNationName);
	sprintf(tempMessage, lan->OutputMessage(0, 315), SecretWarfieldName);
	IntoNationNotice(FORCE_TYPE, 1, tempMessage);
	//sprintf(tempMessage,lan->OutputMessage(0,324),WarfieldName,VictoryNationName,DefeatNationName);
	//IntoNationNotice(FORCE_TYPE,2,tempMessage);
	sprintf(tempMessage, "%s", lan->OutputMessage(0, 318));
	IntoNationNotice(FORCE_TYPE, 2, tempMessage);
	GetTime(p->u.NationWar.NoticeWarPeace1Reinforce.Warfield1RemainTime, Hour1, Min1, Sec1);
	GetTime(p->u.NationWar.NoticeWarPeace1Reinforce.Warfield2RemainTime, Hour2, Min2, Sec2);
	GetTime(p->u.NationWar.NoticeWarPeace1Reinforce.Warfield3RemainTime, Hour3, Min3, Sec3);
	sprintf(tempMessage, lan->OutputMessage(0, 325), Hour1, Min1, Hour2, Min2, Hour3, Min3);
	//<! BBD 040316
	char temp2[500];
	sprintf(temp2, lan->OutputMessage(0, 618), p->u.NationWar.NoticeWarPeace.aLocalWarPoint[0],
		p->u.NationWar.NoticeWarPeace.aLocalWarPoint[1], p->u.NationWar.NoticeWarPeace.aLocalWarPoint[2]);
	sprintf(tempMessage, "%s%s", tempMessage, temp2);
	//> BBD 040316
	IntoNationNotice(FORCE_TYPE, 3, tempMessage);

	CallSmallMenu(MN_NATION_TODAY_NOTICE);

}

void GetDayName(int Day, char* lpStr)			// KR-LAN���� ����
{
	switch (Day)
	{
	case 0: 	strcpy(lpStr, lan->OutputMessage(9, 437)); break;
	case 1:	strcpy(lpStr, lan->OutputMessage(9, 431)); break;
	case 2: 	strcpy(lpStr, lan->OutputMessage(9, 432)); break;
	case 3: 	strcpy(lpStr, lan->OutputMessage(9, 433)); break;
	case 4:  	strcpy(lpStr, lan->OutputMessage(9, 434)); break;
	case 5: 	strcpy(lpStr, lan->OutputMessage(9, 435)); break;
	case 6: 	strcpy(lpStr, lan->OutputMessage(9, 436)); break;
	default:   strcpy(lpStr, "  "); break;
	}
}

void ProcessPeace2(t_packet* p)		// 011025 LTS
{
	char tempMessage[500];
	char tempString[3][MAX_PATH];
	char tempString2[3][MAX_PATH];

	IntoNationNotice(ALL_CLEAR, 1, NULL);
	sprintf(tempMessage, "%s", lan->OutputMessage(0, 317));
	IntoNationNotice(FORCE_TYPE, 1, tempMessage);

	GetDayName(p->u.NationWar.NoticeWarPeace.RemainTime[0][0], tempString[0]);
	GetDayName(p->u.NationWar.NoticeWarPeace.RemainTime[1][0], tempString[1]);
	GetDayName(p->u.NationWar.NoticeWarPeace.RemainTime[2][0], tempString[2]);

	int WarfieldNo[2];
	WarfieldNo[0] = p->u.NationWar.NoticeWarPeace.WarNo % 3;
	WarfieldNo[1] = (p->u.NationWar.NoticeWarPeace.WarNo + 1) % 3;
	ReturnWarfieldName(WarfieldNo[0], tempString2[0]);
	ReturnWarfieldName(WarfieldNo[1], tempString2[1]);
	ReturnWarfieldName(3, tempString2[2]);

	sprintf(tempMessage, lan->OutputMessage(0, 325),
		tempString[0],										// ����
		p->u.NationWar.NoticeWarPeace.RemainTime[0][1],		// ��	
		tempString2[0],										// ���
		tempString[1],										// ����
		p->u.NationWar.NoticeWarPeace.RemainTime[1][1],		// ��
		tempString2[1],										// ���
		tempString[2],										// ����
		p->u.NationWar.NoticeWarPeace.RemainTime[2][1],  	// ��
		tempString2[2]);									// ���
	//<! BBD 040316
	char temp2[500];
	sprintf(temp2, lan->OutputMessage(0, 618), p->u.NationWar.NoticeWarPeace.aLocalWarPoint[0],
		p->u.NationWar.NoticeWarPeace.aLocalWarPoint[1], p->u.NationWar.NoticeWarPeace.aLocalWarPoint[2]);
	sprintf(tempMessage, "%s%s", tempMessage, temp2);
	//> BBD 040316	
	IntoNationNotice(FORCE_TYPE, 2, tempMessage);

	if (p->u.NationWar.NoticeWarPeace.SOpenWarfieldNo >= 0)
	{
		memset(tempMessage, 0, 500);
		switch (p->u.NationWar.NoticeWarPeace.SOpenWarfieldNo)
		{
		case 0: sprintf(tempMessage, lan->OutputMessage(0, 312), lan->OutputMessage(0, 208)); break;	// BBD 040315
		case 1: sprintf(tempMessage, lan->OutputMessage(0, 312), lan->OutputMessage(0, 209)); break;	// BBD 040315
		case 2: sprintf(tempMessage, lan->OutputMessage(0, 312), lan->OutputMessage(0, 210)); break;	// BBD 040315
		case 3: sprintf(tempMessage, lan->OutputMessage(0, 312), lan->OutputMessage(0, 386)); break;	// BBD 040315
		case 4: sprintf(tempMessage, lan->OutputMessage(0, 312), lan->OutputMessage(0, 619)); break;	// BBD 040315
		}
		IntoNationNotice(FORCE_TYPE, 3, tempMessage);
	}

	CallSmallMenu(MN_NATION_TODAY_NOTICE);
}

void ProcessPeace2Reinforce(t_packet* p)	// 011025 LTS
{
	ProcessPeace2(p);
	return;

	/*	char tempMessage[500];
		WORD Hour1,Min1,Sec1;
		WORD Hour2,Min2,Sec2;
		WORD Hour3,Min3,Sec3;

		IntoNationNotice(ALL_CLEAR,1,NULL);
		sprintf(tempMessage,lan->OutputMessage(0,318));
		IntoNationNotice(FORCE_TYPE,1,tempMessage);
		GetTime(p->u.NationWar.NoticeWarPeace2.Warfield1RemainTime,Hour1,Min1,Sec1);
		GetTime(p->u.NationWar.NoticeWarPeace2.Warfield2RemainTime,Hour2,Min2,Sec2);
		GetTime(p->u.NationWar.NoticeWarPeace2.Warfield3RemainTime,Hour3,Min3,Sec3);
		sprintf(tempMessage,lan->OutputMessage(0,325),Hour1,Min1,Hour2,Min2,Hour3,Min3);
		IntoNationNotice(FORCE_TYPE,2,tempMessage);

		CallSmallMenu(MN_NATION_TODAY_NOTICE);*/
}

void ProcessSopen(t_packet* p)
{
	char tempMessage[500];
	IntoNationNotice(ALL_CLEAR, 1, NULL);
	sprintf(tempMessage, "%s", lan->OutputMessage(0, 317));
	IntoNationNotice(FORCE_TYPE, 1, tempMessage);

	switch (p->u.NationWar.NoticeSOpen.WarfieldNo)
	{
	case 0: sprintf(tempMessage, lan->OutputMessage(0, 312), lan->OutputMessage(0, 208)); break;
	case 1: sprintf(tempMessage, lan->OutputMessage(0, 312), lan->OutputMessage(0, 209)); break;
	case 2: sprintf(tempMessage, lan->OutputMessage(0, 312), lan->OutputMessage(0, 210)); break;
	case 3: sprintf(tempMessage, lan->OutputMessage(0, 312), lan->OutputMessage(0, 386)); break;
	case 4: sprintf(tempMessage, lan->OutputMessage(0, 312), lan->OutputMessage(0, 619)); break;	// BBD 040315
	}
	IntoNationNotice(FORCE_TYPE, 2, tempMessage);
	CallSmallMenu(MN_NATION_TODAY_NOTICE);
}

void ProcessSopenError(t_packet* p)
{
	char tempMessage[500];
	IntoNationNotice(ALL_CLEAR, 1, NULL);
	sprintf(tempMessage, "%s", lan->OutputMessage(0, 317));
	IntoNationNotice(FORCE_TYPE, 1, tempMessage);
	CallSmallMenu(MN_NATION_TODAY_NOTICE);
}

void RecvCMD_ANSWER_NOTICE(t_packet* p) //������ ��Ƽ�� 
{
	char NoticeType = p->u.data[0];

	//AddCurrentStatusMessage( 255,255,0, "��Ƽ����ȣ : %d ",NoticeType);

	switch (NoticeType)
	{
	case NW_NOTICE_JOIN_ATTACK: ProcessNoticeJoinA(p);				break;
	case NW_NOTICE_JOIN_DEFENCE: ProcessNoticeJoinD(p);				break;
	case NW_NOTICE_JOIN_REINFORCE: ProcessNoticeJoinReinforce(p);		break;
	case NW_NOTICE_PREPARE1: ProcessPrepare1(p);					break;
	case NW_NOTICE_PREPARE1_REINFORCE: ProcessPrepare1Reinforce(p);			break;
	case NW_NOTICE_PREPARE2: ProcessPrepare2(p);					break;
	case NW_NOTICE_PREPARE2_REINFORCE: ProcessPrepare2Reinforce(p);			break;
	case NW_NOTICE_PREPARE3: ProcessPrepare3(p);					break;
	case NW_NOTICE_PREPARE3_REINFORCE: ProcessPrepare3Reinforce(p);			break;
	case NW_NOTICE_PREPARE4: ProcessPrepare4(p);					break;
	case NW_NOTICE_PREPARE4_REINFORCE: ProcessPrepare4Reinforce(p);			break;
	case NW_NOTICE_WAR1A: ProcessWar1A(p);						break;
	case NW_NOTICE_WAR1D: ProcessWar1D(p);						break;
	case NW_NOTICE_WAR1_REINFORCE: ProcessWar1Reinforce(p);				break;
	case NW_NOTICE_WAR2A: ProcessWar2A(p);						break;
	case NW_NOTICE_WAR2D: ProcessWar2D(p);						break;
	case NW_NOTICE_WAR2_REINFORCE: ProcessWar2Reinforce(p);				break;
	case NW_NOTICE_PEACE1_VICTORY: ProcessPeace1V(p);					break;
	case NW_NOTICE_PEACE1_DEFEAT: ProcessPeace1D(p);					break;
	case NW_NOTICE_PEACE1_REINFORCE: ProcessPeace1Reinforce(p);			break;
	case NW_NOTICE_PEACE2: ProcessPeace2(p);						break;
	case NW_NOTICE_PEACE2_REINFORCE: ProcessPeace2Reinforce(p);			break;
	case NW_NOTICE_SOPEN: ProcessSopen(p);						break;
	case NW_NOTICE_SOPEN_ERROR: ProcessSopenError(p);					break;			// �̰����� ������ WarfieldNo�� �߸��Ȱ���
	}
}

void RecvCMD_WAR_CONTROL(t_packet* p)		// 011020 LTS
{
#ifdef _DEBUG // finito 060507
	if (GetSysInfo(SI_GAME_MAKE_MODE))	// �������� ��� �϶��� ����..
	{
		AddCurrentStatusMessage(255, 255, 0, "The current warfield warfield status is status : [%d], loop status : [%d].",
			p->u.NationWar.WarControl.Status, p->u.NationWar.WarControl.Active);
	}
#endif
}

void RecvCMD_WAR_LOOP_TIME(t_packet* p)
{
	if (GetSysInfo(SI_GAME_MAKE_MODE))	// �������� ��� �϶��� ����..
	{
		AddCurrentStatusMessage(255, 255, 0, "The current warfield warfield status is status : [%d], loop time : [%d].",
			p->u.NationWar.WarLoopTime.Status, p->u.NationWar.WarLoopTime.LoopTime);
	}
}

void RecvCMD_FAME_EXP_CHANGE(t_packet* p)		// 020115 LTS	// LTS NEW LOCALWAR
{
	short int	tempFame = p->u.NationWar.FameExpChange.GainedFame;
	DWORD		tempExp = p->u.NationWar.FameExpChange.GainedExp;
	PlayListAutoSounds(62, 0, 0, FALSE);
	/*	AddCurrentStatusMessage( 255,255,0,"============< ���� ���� >==============");
		AddCurrentStatusMessage( 255,255,0,"���￡�� ���� ���� ���� �������� �Ʒ��� ���� ������ �ϰԉ���ϴ�.");
		AddCurrentStatusMessage( 255,255,0,"   [ ��    ��] : %d",tempFame);
		AddCurrentStatusMessage( 255,255,0,"   [ �� �� ġ] : %d",tempExp);*/
	AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 198));
	AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 199));
	AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 200), tempFame);
	AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 201), tempExp);

}

//< LTH-040318-KO ���� ������� ���� �޾Ҵ�
void RecvCMD_CHECK_REMAIN_GUARD(t_packet* p)
{
	int nI, nJ;
	if (IsNeoWarfield(p->u.NationWar.RemainGuard.nWarfieldNo))
	{
		//< LTH-040324-KO ��Ȯ�� �ð�����. ���� �ð�.
		g_dwRemainTime = p->u.NationWar.RemainGuard.dwRemainTime + (g_ClientTime / 1000);
		//> LTH-040324-KO
		for (nJ = 0; nJ < MAX_TEAM; ++nJ)
		{
			for (nI = 0; nI < NEO_NUMOFGUARDTYPE; ++nI)
			{
				g_aRemainGuard[nJ][nI] = p->u.NationWar.RemainGuard.aRemainGuardCount[nJ][nI];
			}
		}
	}
}
//> LTH-040318-KO

void RecvCMD_NWAFIELD_START_WAR(t_packet* p)
{
	g_bWarBegin = true;
	g_pNation->SetNewStatus(p->u.NationWar.NewWarStart.WarfieldNo, NW_WAR);
	//<! BBD 040311
		//< LTH-040130-KO 1.4 Patch
	switch (p->u.NationWar.NewWarStart.WarfieldNo)
	{
	case 0:
		SMenu[MN_WAR_START].nField[0].nRectImage = 4;				// ���� ������ �̹���
		SMenu[MN_WAR_START].nField[0].nImageNumber = 4;

		SMenu[MN_WAR_START].nField[2].x = 188;						// ���� ��������ũ
		SMenu[MN_WAR_START].nField[2].y = 125;

		SMenu[MN_WAR_START].nField[3].x = 116;					// ȭ��ǥ
		SMenu[MN_WAR_START].nField[3].y = 200;
		SetRect(SMenu[MN_WAR_START].nField[3].rCheakBox, 116, 200, 180, 300);
		SMenu[MN_WAR_START].nField[3].nRectImage = 12;
		SMenu[MN_WAR_START].nField[3].nImageNumber = 12;

		SMenu[MN_WAR_START].nField[4].x = 55;						// ������ ���� ��ũ
		SMenu[MN_WAR_START].nField[4].y = 120;
		AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 297));

		sprintf(SMenu[MN_WAR_START].nField[5].temp, lan->OutputMessage(0, 387), p->u.NationWar.NewWarStart.LevelMin, p->u.NationWar.NewWarStart.LevelMax);		// ����
		break;		//"������������ ������ ���۵Ǿ����ϴ�."

	case WI_HADES_WARFIELD:
		SMenu[MN_WAR_START].nField[0].nImageNumber = 13;			// ���� ������ �̹���
		SMenu[MN_WAR_START].nField[2].nType = FT_NONE;			// ������ ��ũ�� �����̰� �� �ʿ����.
		SMenu[MN_WAR_START].nField[3].nType = FT_NONE;
		SMenu[MN_WAR_START].nField[4].nType = FT_NONE;

		//< LTH-040312-KO �������̽��� ����
		int nTerm = (int)(p->u.NationWar.NewWarStart.dwTerm / 60);
		sprintf(SMenu[MN_WAR_START].nField[5].temp, lan->OutputMessage(0, 615), nTerm, p->u.NationWar.NewWarStart.btNationPoint);		// ����
		AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 613));	// "���� �����Ͱ� ���� ��¼��..."
		g_dwRemainTime = p->u.NationWar.NewWarStart.dwTerm + (g_ClientTime / 1000);
		//> LTH-040312-KO
		break;
	}
	//> LTH-040130-KO
//> BBD 040311

	CallSmallMenu(MN_WAR_START);
}

void RecvCMD_NWARFIELD_END_WAR(t_packet* p)
{
	g_bWarBegin = false;

	//< LTH-040303-KO 1.4 Patch. ���������� �߰�
	INT nWarfieldNo = p->u.NationWar.CommonDataC.Data;
	if (!IsNeoWarfield(nWarfieldNo))
	{
		g_pNation->SetNewStatus(nWarfieldNo, NW_PEACE);
		AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 298));		// "������������ ������ �������ϴ�."
	}
	else
	{
		g_pNation->SetNewStatus(nWarfieldNo, NW_SOPEN);
		AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 614));		// "���� �������� �������ϴ�."	// BBD 040311
	}
	//> LTH-040303-KO
}

void RecvCMD_FAME_CHANGE(t_packet* p)
{
	PlayListAutoSounds(62, 0, 0, FALSE);
	AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 198));
	AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 203));
	AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 200), p->u.NationWar.CommonDataDw.Data);
}

void CallWarResult(WAR_RESULT tWarResult, bool bAmIWin)
{
	SMenu[MN_NATION_WAR_RESULT].nField[1].nWillDo = tWarResult.BYDeathCount;
	SMenu[MN_NATION_WAR_RESULT].nField[2].nWillDo = tWarResult.ZYDeathCount;

	SMenu[MN_NATION_WAR_RESULT].nField[3].nWillDo = tWarResult.BYCount;
	SMenu[MN_NATION_WAR_RESULT].nField[4].nWillDo = tWarResult.YLBYCount;

	SMenu[MN_NATION_WAR_RESULT].nField[5].nWillDo = tWarResult.ZYCount;
	SMenu[MN_NATION_WAR_RESULT].nField[6].nWillDo = tWarResult.YLZYCount;


	if (bAmIWin)
	{
		SMenu[MN_NATION_WAR_RESULT].nField[0].nImageType = 0;
		if (tWarResult.VictoryTeam == 0)	//���̼����� �̱�
			SMenu[MN_NATION_WAR_RESULT].nField[7].nImageNumber = 1;
		else
			SMenu[MN_NATION_WAR_RESULT].nField[7].nImageNumber = 3;
	}
	else
	{
		SMenu[MN_NATION_WAR_RESULT].nField[0].nImageType = 1;
		if (tWarResult.VictoryTeam == 0)	//�������� ��
			SMenu[MN_NATION_WAR_RESULT].nField[7].nImageNumber = 4;
		else
			SMenu[MN_NATION_WAR_RESULT].nField[7].nImageNumber = 2;
	}

	SMenu[MN_NATION_WAR_RESULT].nField[9].nImageNumber = 19;	// BBD 040311

	CallSmallMenu(MN_NATION_WAR_RESULT);
	return;
}

//< LTH-040303-KO 1.4 Patch. �� ��� ȭ���� ���� �ٸ� ��Ŷ������ ���´�
void CallWarResult2(NEO_WAR_RESULT tagNeoWarResult, BOOL bAmIWin)
{
	//<! BBD 040311
	SMenu[MN_NATION_WAR_RESULT].nField[1].nWillDo = tagNeoWarResult.nBYDeathCount;
	SMenu[MN_NATION_WAR_RESULT].nField[2].nWillDo = tagNeoWarResult.nZYDeathCount;
	SMenu[MN_NATION_WAR_RESULT].nField[3].nWillDo = tagNeoWarResult.nYLDeathCount;

	SMenu[MN_NATION_WAR_RESULT].nField[4].nWillDo = tagNeoWarResult.nBYMemberCount;
	SMenu[MN_NATION_WAR_RESULT].nField[5].nWillDo = tagNeoWarResult.nZYMemberCount;
	SMenu[MN_NATION_WAR_RESULT].nField[6].nWillDo = tagNeoWarResult.nYLMemberCount;

	SMenu[MN_NATION_WAR_RESULT].nField[9].nImageNumber = 20;	//  ���� ��������

	if (bAmIWin)
	{
		SMenu[MN_NATION_WAR_RESULT].nField[0].nImageType = 0;
		SMenu[MN_NATION_WAR_RESULT].nField[7].nImageNumber = 17;	// �´�
	}
	else
	{
		SMenu[MN_NATION_WAR_RESULT].nField[0].nImageType = 1;
		SMenu[MN_NATION_WAR_RESULT].nField[7].nImageNumber = 18;	// ���

	}
	//> BBD 040311
	CallSmallMenu(MN_NATION_WAR_RESULT);
}
//> LTH-040303-KO

void RecvCMD_WAR_RESULT(t_packet* p)
{
	bool AmIWin = false;
	g_bWarBegin = false;
	//< LTH-040303-KO 1.4 Patch. �� �������� ���ο� ��� â
	//< LTH-040427-KO ���� �������� ��Ȯ�� ������ ��ȣ�� ���� �߰�
	INT nWarfieldNo = p->u.NationWar.WarResult.WarfieldPort - WP_BASE_PORT + 1;
	if (!IsNeoWarfield(nWarfieldNo))
	{
		nWarfieldNo = p->u.NationWar.NeoWarResult.wWarfieldPort - WP_BASE_PORT + 1;
		if (!IsNeoWarfield(nWarfieldNo))
			nWarfieldNo = p->u.NationWar.WarResult.WarfieldPort - WP_BASE_PORT + 1;
	}
	//> LTH-040427-KO
	if (!IsNeoWarfield(nWarfieldNo))
	{
		g_pNation->SetNewStatus(nWarfieldNo, NW_PEACE);
		switch (Hero->name_status.nation)
		{
		case NW_BY:
			if (p->u.NationWar.WarResult.VictoryTeam == 0)
				AmIWin = true;
			else
				AmIWin = false;
			break;
		case NW_ZY:
			if (p->u.NationWar.WarResult.VictoryTeam == 0)
				AmIWin = false;
			else
				AmIWin = true;
			break;
		case NW_YL:
			if (Hero->NWCharacter.YL_JoinNation == NW_BY)
			{
				if (p->u.NationWar.WarResult.VictoryTeam == 0)
					AmIWin = true;
				else
					AmIWin = false;
			}
			else
			{
				if (p->u.NationWar.WarResult.VictoryTeam == 0)
					AmIWin = false;
				else
					AmIWin = true;
			}
			break;
		default:
		{// 030506 kyo ��籹�� �й�
			AmIWin = false;
			break;
		}
		}

		CallWarResult(p->u.NationWar.WarResult, AmIWin);
	}
	else
	{	//< LTH-040303-KO 1.4 Patch.
		g_pNation->SetNewStatus(nWarfieldNo, NW_SOPEN);
		if (TEAM1 == p->u.NationWar.NeoWarResult.cVictoryTeam)
			AmIWin = TRUE;
		else
			AmIWin = FALSE;

		//< LTH-040413-KO ��� ������ ���Ϳ��� ���� �޾����� ���� ������ â�� ����� �ʴ´�.
		if (TRUE == p->u.NationWar.NeoWarResult.bAllNationJoin)
			CallWarResult2(p->u.NationWar.NeoWarResult, AmIWin);
		//> LTH-040413-KO
	}	//< LTH-040303-KO
	//> LTH-040303-KO
}

void RecvCMD_ANSWER_NEW_WARFIELD_TILE_DONT(t_packet* p)
{
	if (g_pDontManager == NULL)
		return;

	for (int i = 0; i < p->u.NationWar.DontData.Size; i++)
	{
		g_pDontManager->ActiveDont(p->u.NationWar.DontData.DontNo[i], p->u.NationWar.DontData.Status[i]);
	}
}

void RecvCMD_GUARD_KILLER(t_packet* p)
{
	LPCHARACTER temp = FindHim(p->u.NationWar.GuardKiller.GuardID + 10000);
	if (temp != NULL)
	{
		temp->GuardStoneAni = 1;		// h_DisPlayNPC()���� ���ϸ��̼��� ó���ϱ� ���� ������ 
		temp->SetAni2Zero = 1;
		temp->sealstoneani = 0;
	}
	temp = FindHim(p->u.NationWar.GuardKiller.KillerID);
	if (temp != NULL)
	{
		InsertMagic(temp, temp, 451, 9, 0, 0, temp->x, temp->y);
	}
}

void RecvCMD_DONT_CHANGE(t_packet* p)
{
	g_pDontManager->ActiveDont(p->u.NationWar.DontChange.DontNo, p->u.NationWar.DontChange.Status);
}

void RecvCMD_ANSWER_NEW_WARFIELD_STATUS(t_packet* p)
{
	//< LTH-040303-KO 1.4 Patch. ���������ʹ� 0 �� ���� �����ʹ� 1 + 3 = 4 �� �ȴ�
	for (int i = 0; i < MAX_NEW_WARFIELD + 1; i++)
	{
		//< LTH-040414-KO �����������̸�?
		if (i == 0)
			g_pNation->SetNewStatus(i, p->u.NationWar.NWStatus.Status[i]);
		else
			g_pNation->SetNewStatus(i + 3, p->u.NationWar.NWStatus.Status[i]);
		//> LTH-040414-KO
	}
	//> LTH-040303-KO
}

int GetHeroTeamNo()
{
	int Nation = Hero->name_status.nation;
	if (Nation == NW_YL)
		Nation = Hero->NWCharacter.YL_JoinNation;

	switch (Nation)
	{
	case NW_BY: return 0;
	case NW_ZY: return 1;
	}
	return 0;
}



void RecvCMD_ANSWER_NEW_WARFIELD_DATA(t_packet* p)
{
	int HeroTeamNo = GetHeroTeamNo();
	WORD Hour, Min, Sec;

	//< LTH-040302-KO 1.4 Patch. IsNeoWarfieldServer() Add.
	if (isNewWarfieldServer() || IsNeoWarfieldServer())				// �����Ϳ� ������ ���ΰ��� �׷��ش�.
	{
		//<! BBD 040315
		float InterPoleX = 0.0f;
		float InterPoleY = 0.0f;
		int PosX = 0;
		int PosY = 0;
		// ���� �����Ϳ� ���������ʹ� �� ũ�Ⱑ �ٸ���
		if (!IsNeoWarfieldServer())
		{	// ���� ������
			InterPoleX = (Hero->x / TILE_SIZE) / (float)439;
			InterPoleY = (Hero->y / TILE_SIZE) / (float)439;
			PosX = 20 + static_cast<int>((286 * InterPoleX));
			PosY = 36 + static_cast<int>((286 * InterPoleY));
		}
		else
		{	// ���� ������
			InterPoleX = (Hero->x / TILE_SIZE) / (float)820;
			InterPoleY = (Hero->y / TILE_SIZE) / (float)820;
			PosX = static_cast<int>((280 * InterPoleX) + 30);
			PosY = static_cast<int>((280 * InterPoleY) + 40);
		}
		//> BBD 040315

		SMenu[MN_WAR_STATUS].nField[2].x = PosX;						//���ΰ��� ��ġ
		SMenu[MN_WAR_STATUS].nField[2].y = PosY;

		switch (Hero->class_type)
		{
		case WARRIOR: SMenu[MN_WAR_STATUS].nField[2].nImageNumber = 13; break;
		case THIEF: SMenu[MN_WAR_STATUS].nField[2].nImageNumber = 14; break;
		case ARCHER: SMenu[MN_WAR_STATUS].nField[2].nImageNumber = 15; break;
		case WIZARD: SMenu[MN_WAR_STATUS].nField[2].nImageNumber = 16; break;
		case PRIEST: SMenu[MN_WAR_STATUS].nField[2].nImageNumber = 17; break;
		}
		SMenu[MN_WAR_STATUS].nField[2].nType = FT_NOMAL_PUT;
	}
	else
	{
		SMenu[MN_WAR_STATUS].nField[2].nType = FT_NONE;
	}
	//> LTH-040302-KO

	//< LTH-040302-KO 1.4 Patch. �� �����ʹ� ��Ŷ������ �ٸ���. ���� �߰����� ���ľ��ϴٴ� -_-;
	if (!IsNeoWarfield(p->u.NationWar.NeoWarfieldData.nWarfieldNo))
	{
		//<! BBD 040311
		// ���� �����ͷ� �޴� �̹��� ����
		SMenu[MN_WAR_STATUS].nField[0].nImageNumber = 18;	// ���� ������ �̸�
		SMenu[MN_WAR_STATUS].nField[1].nImageNumber = 3;	// ���� ������ ��
		SMenu[MN_WAR_STATUS].nField[15].nImageNumber = 21;	// ���̼��� ������ ���� �̹���
		//> BBD 040311

		GetTime(p->u.NationWar.NWarfieldData1.RemainTime, Hour, Min, Sec);
		sprintf(SMenu[MN_WAR_STATUS].nField[11].temp, "%02d:%02d", Hour, Min);	// �����ð�

		sprintf(SMenu[MN_WAR_STATUS].nField[12].temp, "%d", p->u.NationWar.NWarfieldData1.TeamCount[0]);	// ��ī��Ʈ
		sprintf(SMenu[MN_WAR_STATUS].nField[13].temp, "%d", p->u.NationWar.NWarfieldData1.TeamCount[1]);

		// ���̼���
		if (!p->u.NationWar.NWarfieldData1.GuardStatus[0][0])	// ���� ���彺��
			SMenu[MN_WAR_STATUS].nField[3].nType = FT_NONE;
		else
			SMenu[MN_WAR_STATUS].nField[3].nType = FT_NOMAL_PUT;

		if (!p->u.NationWar.NWarfieldData1.GuardStatus[0][1])	// ����1
			SMenu[MN_WAR_STATUS].nField[4].nType = FT_NONE;
		else
			SMenu[MN_WAR_STATUS].nField[4].nType = FT_NOMAL_PUT;

		if (!p->u.NationWar.NWarfieldData1.GuardStatus[0][2])	// ����2
			SMenu[MN_WAR_STATUS].nField[5].nType = FT_NONE;
		else
			SMenu[MN_WAR_STATUS].nField[5].nType = FT_NOMAL_PUT;

		if (!p->u.NationWar.NWarfieldData1.GuardStatus[0][3] ||
			!p->u.NationWar.NWarfieldData1.GuardStatus[0][4])	// ����3
			SMenu[MN_WAR_STATUS].nField[6].nImageNumber = 6;
		else
			SMenu[MN_WAR_STATUS].nField[6].nImageNumber = 5;

		// ������
		if (!p->u.NationWar.NWarfieldData1.GuardStatus[1][0])	// ���� ���彺��
			SMenu[MN_WAR_STATUS].nField[7].nType = FT_NONE;
		else
			SMenu[MN_WAR_STATUS].nField[7].nType = FT_NOMAL_PUT;

		if (!p->u.NationWar.NWarfieldData1.GuardStatus[1][1])	// ����1
			SMenu[MN_WAR_STATUS].nField[8].nType = FT_NONE;
		else
			SMenu[MN_WAR_STATUS].nField[8].nType = FT_NOMAL_PUT;

		if (!p->u.NationWar.NWarfieldData1.GuardStatus[1][2])	// ����2
			SMenu[MN_WAR_STATUS].nField[9].nType = FT_NONE;
		else
			SMenu[MN_WAR_STATUS].nField[9].nType = FT_NOMAL_PUT;

		if (!p->u.NationWar.NWarfieldData1.GuardStatus[1][3] ||
			!p->u.NationWar.NWarfieldData1.GuardStatus[1][4])	// ����3
			SMenu[MN_WAR_STATUS].nField[10].nImageNumber = 6;
		else
			SMenu[MN_WAR_STATUS].nField[10].nImageNumber = 5;
	}
	else
	{	//< LTH-040302-KO 1.4 Patch.

		//<! BBD 040311
		// ���� �����ͷ� �޴� �̹��� ����
		SMenu[MN_WAR_STATUS].nField[0].nImageNumber = 20;	// ���� ������ �̸�
		SMenu[MN_WAR_STATUS].nField[1].nImageNumber = 19;	// ���� ������ ��
		SMenu[MN_WAR_STATUS].nField[15].nImageNumber = 22;	// ���� ��輮 ���� �̹���

		//���� �����Ϳ����� ���� ���µ��� ���δ�
		for (int i = 3; i <= 10; i++)
		{
			SMenu[MN_WAR_STATUS].nField[i].nType = FT_NONE;
		}
		// ȭ�� ���ܿ� ���� �����Ȳ�� gameproc���� ȣ��ȴ�.

		//> BBD 040311

		g_dwRemainTime = p->u.NationWar.NeoWarfieldData.dwRemainTime + (g_ClientTime / 1000);	// LTH-040314-KO ������ ���ܿ� ����� ����. �ð�
		GetTime(p->u.NationWar.NeoWarfieldData.dwRemainTime, Hour, Min, Sec);
		sprintf(SMenu[MN_WAR_STATUS].nField[11].temp, "%02d:%02d", Hour, Min);	// �����ð�

		sprintf(SMenu[MN_WAR_STATUS].nField[12].temp, "%d", \
			p->u.NationWar.NeoWarfieldData.nRemainGuardCount[TEAM2][2]);	// ������ ���� ���� ����
		//< LTH-040314-KO ������ ���ܿ� ����� ����. ���� ������� ��
		int nI;
		for (nI = 0; nI < 4; ++nI)
			g_aRemainGuard[TEAM2][nI] = p->u.NationWar.NeoWarfieldData.nRemainGuardCount[TEAM2][nI];
		//> LTH-040314-KO

		sprintf(SMenu[MN_WAR_STATUS].nField[13].temp, "%d", \
			p->u.NationWar.NeoWarfieldData.nRemainGuardCount[TEAM2][1] + \
			p->u.NationWar.NeoWarfieldData.nRemainGuardCount[TEAM2][3]);	// ������ ��輮 ���� ����. ���� + �ܼ�

	}	//> LTH-040302-KO
	//> LTH-040302-KO

	CallSmallMenu(MN_WAR_STATUS);
}

void RecvCMD_WAR_YL_JOIN(t_packet* p)
{
	Hero->NWCharacter.YL_JoinNation = p->u.NationWar.CommonDataC.Data;
}

void RecvCMD_GUARD_BROKE(t_packet* p)
{	// LTH-040303-KO 1.4 Patch. ���� ���������� �� ������ �ٸ��� ������ ������.
	INT nWarfieldNo = p->u.NationWar.GuardBroke.nWarfieldNo;

	if (!IsNeoWarfield(nWarfieldNo))
	{
		switch (p->u.NationWar.GuardBroke.Type)
		{
		case 0:																	// ��ȣ��
			if (p->u.NationWar.GuardBroke.BrokeTeamNo == 0)		//���̼����� ������..
			{
				AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 388), lan->OutputMessage(0, 205), p->u.NationWar.GuardBroke.X, p->u.NationWar.GuardBroke.Y);
			}
			else
			{
				AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 388), lan->OutputMessage(0, 206), p->u.NationWar.GuardBroke.X, p->u.NationWar.GuardBroke.Y);
			}
			break;
		case 1:										// ���� 
			if (p->u.NationWar.GuardBroke.BrokeTeamNo == 0)		//���̼����� ������..
			{
				AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 389), lan->OutputMessage(0, 205), p->u.NationWar.GuardBroke.X, p->u.NationWar.GuardBroke.Y);
			}
			else
			{
				AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 389), lan->OutputMessage(0, 206), p->u.NationWar.GuardBroke.X, p->u.NationWar.GuardBroke.Y);
			}
			break;
		}
	}
	else
	{	//< LTH-040303-KO �ٷ� �̺κ��� �ٲ� �κ��̴�
		switch (p->u.NationWar.GuardBroke.Type)
		{
		case 1:		// ���� ��ȣ��
			if (TEAM2 == p->u.NationWar.GuardBroke.BrokeTeamNo)		// �������� �μ�����
			{	// LTH-040316-KO
				AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 623), \
					p->u.NationWar.GuardBroke.X, p->u.NationWar.GuardBroke.Y);
			}
			break;

		case 2:		// ����
			if (TEAM2 == p->u.NationWar.GuardBroke.BrokeTeamNo)		// �������� �μ�����
			{	// LTH-040316-KO
				AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 624), \
					p->u.NationWar.GuardBroke.X, p->u.NationWar.GuardBroke.Y);
			}
			break;

		case 3:		// �ܼ� ��ȣ��
			if (TEAM2 == p->u.NationWar.GuardBroke.BrokeTeamNo)		// �������� �μ�����
			{	// LTH-040316-KO
				AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 623), \
					p->u.NationWar.GuardBroke.X, p->u.NationWar.GuardBroke.Y);
			}
			break;
		}	//> LTH-040303-KO

		g_aRemainGuard[TEAM2][p->u.NationWar.GuardBroke.Type] = \
			p->u.NationWar.GuardBroke.aRemainGuard[TEAM2][p->u.NationWar.GuardBroke.Type];	// LTH-040314-KO �����߿� ��Ÿ�� ����
	}
}	//> LTH-040303-KO

//< LTH-040317-KO ���尡 �μ����� �ʴ� ������ �����Ѵ�
void RecvCMD_GUARD_CAN_NOT_BROKE(t_packet* p)
{
	if (IsNeoWarfield(p->u.NationWar.GuardCanNotBroke.nWarfieldNo))
	{
		switch (p->u.NationWar.GuardCanNotBroke.nType)
		{
		case 1:		// ���� ��ȣ���� �μ����� �ʴ� ����
			AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 620));	// "���� ��ȣ���� ���� �μ����� �ʾ� ������ �� �����ϴ�."
			break;

		case 2:
			AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 625));	// "������ ���� �μ����� �ʾ� ������ �� �����ϴ�."
			break;

		case 3:
			AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 621));	// "�ܼ� ��ȣ���� ���� �μ����� �ʾ� ������ �� �����ϴ�."
			break;
		}
	}
}
//> LTH-040317-KO

int CheckHandleByNationWar(t_packet* p)
{
	switch (p->h.header.type)
	{
	case CMD_WAR_BBS:
		RecvCMD_WAR_BBS(p);
		break;
	case CMD_ANSWER_WARFIELD_STATUS:
		RecvCMD_ANSWER_WARFIELD_STATUS(p);
		break;
	case CMD_ANSWER_VOTE_DATA:
		RecvCMD_ANSWER_VOTE_DATA(p);
		break;
	case CMD_ANSWER_SQUAD_INFO:
		RecvCMD_ANSWER_SQUAD_INFO(p);
		break;
	case CMD_ANSWER_SQUAD_INFO2:
		RecvCMD_ANSWER_SQUAD_INFO2(p);
		break;
	case CMD_ANSWER_COMMANDER_VOTE_DATA:
		RecvCMD_ANSWER_COMMANDER_VOTE_DATA(p);
		break;
	case CMD_ANSWER_COMMANDER_REG_DATA:
		RecvCMD_ANSWER_COMMANDER_REG_DATA(p);
		break;
	case CMD_YOU_ARE_SQUAD_LEADER:
		RecvCMD_YOU_ARE_SQUAD_LEADER(p);
		break;
	case CMD_JOIN_SQUAD:
		RecvCMD_JOIN_SQUAD(p);
		break;
	case CMD_ANSWER_NATION_WAR_COUNT:
		RecvCMD_ANSWER_NATION_WAR_COUNT(p);
		break;
	case CMD_NW_MAP_MOVE_FAIL:
		RecvCMD_NW_MAP_MOVE_FAIL(p);
		break;
	case CMD_ANSWER_TILE_DONT:
		RecvCMD_ANSWER_TILE_DONT(p);
		break;
	case CMD_TILE_DONT_CHANGED:
		RecvCMD_TILE_DONT_CHANGED(p);
		break;
	case CMD_SQUAD_CHAT:
		RecvCMD_SQUAD_CHAT(p);
		break;
	case CMD_CLEAR_SQUAD_DATA:
		RecvCMD_CLEAR_SQUAD_DATA(p);
		break;
	case CMD_ANSWER_WARFIELD_INFO:
		RecvCMD_ANSWER_WARFIELD_INFO(p);
		break;
	case CMD_WAR_BEGIN:
		RecvCMD_WAR_BEGIN(p);
		break;
	case CMD_WAR_END:
		RecvCMD_WAR_END(p);
		break;
	case CMD_ANSWER_CHECK_WARFIELD_POSSESSION:
		RecvCMD_ANSWER_CHECK_WARFIELD_POSSESSION(p);
		break;
	case CMD_ANSWER_WARFIELD_POSSESSION:
		RecvCMD_ANSWER_WARFIELD_POSSESSION(p);
		break;
	case CMD_WARFIELD_WAR_DATA1:
		RecvCMD_WARFIELD_WAR_DATA1(p);
		break;
	case CMD_WARFIELD_WAR_DATA2:
		RecvCMD_WARFIELD_WAR_DATA2(p);
		break;
	case CMD_WARFIELD_WAR_DATA3:
		RecvCMD_WARFIELD_WAR_DATA3(p);
		break;
	case CMD_CLEAR_WAR_CHARACTER_DATA:
		RecvCMD_CLEAR_WAR_CHARACTER_DATA(p);
		break;
	case CMD_YOU_ARE_SQUAD_COMMANDER:
		RecvCMD_YOU_ARE_SQUAD_COMMANDER(p);
		break;
	case CMD_ANSWER_COMMANDER_VOTE_DATA2:
		RecvCMD_ANSWER_COMMANDER_VOTE_DATA2(p);
		break;
	case CMD_SQUAD_JOIN_RESULT:
		RecvCMD_SQUAD_JOIN_RESULT(p);
		break;
	case CMD_ANSWER_NOTICE:
		RecvCMD_ANSWER_NOTICE(p);
		break;
	case CMD_WAR_CONTROL:						// 011020 LTS
		RecvCMD_WAR_CONTROL(p);
		break;
	case CMD_WAR_LOOP_TIME:					// 011025 LTS
		RecvCMD_WAR_LOOP_TIME(p);
		break;
	case CMD_FAME_EXP_CHANGE:					// 020115 LTS
		RecvCMD_FAME_EXP_CHANGE(p);
		break;
	case CMD_NWARFIELD_START_WAR:
		RecvCMD_NWAFIELD_START_WAR(p);
		break;
	case CMD_NWARFIELD_END_WAR:
		RecvCMD_NWARFIELD_END_WAR(p);
		break;
	case CMD_FAME_CHANGE:
		RecvCMD_FAME_CHANGE(p);
		break;
	case CMD_WAR_RESULT:
		RecvCMD_WAR_RESULT(p);
		break;
	case CMD_ANSWER_NEW_WARFIELD_TILE_DONT:
		RecvCMD_ANSWER_NEW_WARFIELD_TILE_DONT(p);
		break;
	case CMD_GUARD_KILLER:
		RecvCMD_GUARD_KILLER(p);
		break;
	case CMD_DONT_CHANGE:
		RecvCMD_DONT_CHANGE(p);
		break;
	case CMD_ANSWER_NEW_WARFIELD_STATUS:
		RecvCMD_ANSWER_NEW_WARFIELD_STATUS(p);
		break;
	case CMD_ANSWER_NEW_WARFIELD_DATA:
		RecvCMD_ANSWER_NEW_WARFIELD_DATA(p);
		break;
	case CMD_WAR_YL_JOIN:
		RecvCMD_WAR_YL_JOIN(p);
		break;
	case CMD_GUARD_BROKE:
		RecvCMD_GUARD_BROKE(p);
		break;

		//< LTH-040317-KO ���尡 �μ����� �ʴ� ������ �޾Ҵ�
	case CMD_GUARD_CAN_NOT_BROKE:
		RecvCMD_GUARD_CAN_NOT_BROKE(p);
		break;
		//> LTH-040317-KO

		//< LTH-040322-KO ���� ������� ���� ��ȯ�Ѵ�
	case CMD_CHECK_REMAIN_GUARD:
		RecvCMD_CHECK_REMAIN_GUARD(p);
		break;
		//> LTH-040322-KO

	default: CheckHandleByLTSSupport(p);

	}
	return 1;
}

// cNation Class Definition

cNation::cNation()
{
	int i, j;
	m_Status[0] = 1;  // Loop UROLL
	m_Status[1] = 1;
	m_Status[2] = 1;

	for (j = 0; j < NW_NATION_COUNT; j++)
		for (i = 0; i < MAX_MONSTER_POINT; i++)
		{
			m_tMonsterPoint[j][i].MonsterIndex = 0;
			m_tMonsterPoint[j][i].Size = 0;
			m_tMonsterPoint[j][i].Points = NULL;
		}

	LoadMonsterPoint();

	//< LTH-040226-KO 1.4 ���� ���ο� ������ ���� ����Ҹ� �ʱ�ȭ
	for (i = 0; i < WI_MAX_WARFIELD; ++i)
		m_btNeoStatus[i] = NW_PEACE;
	//> LTH-040226-KO
}


cNation::~cNation()   //��Ʈ���͸� ������ ȣ���̵Ǵٴ�... ��.�� 
{
	for (int j = 0; j < NW_NATION_COUNT; j++)
	{
		for (int i = 0; i < MAX_MONSTER_POINT; i++)
		{
			SAFE_DELETE_ARRAY(m_tMonsterPoint[j][i].Points);
		}
	}
}

int cNation::GetStatus(int WarfieldNo)
{
	if (WarfieldNo >= 0 && WarfieldNo <= 2)
		return m_Status[WarfieldNo];
	else
	{	//< LTH-040226-KO 1.4 ��ġ�� �ű������͸� ���������Ϳ� �������´�
		if (!IsNeoWarfield(WarfieldNo))
			return m_NewStatus[WarfieldNo - 3];
		else
			return m_btNeoStatus[WarfieldNo];
	}	//> LTH-040226-KO
}

void cNation::SetStatus(int WarfieldNo, int Status)
{
	if (WarfieldNo >= 0 && WarfieldNo <= 2)
		m_Status[WarfieldNo] = Status;
	else
	{	//< LTH-040226-KO 1.4 ��ġ�� �ű������͸� ���������Ϳ� �������´�
		if (!IsNeoWarfield(WarfieldNo))
			m_NewStatus[WarfieldNo - 3] = Status;
		else
			m_btNeoStatus[WarfieldNo] = Status;
	}	//> LTH-040226-KO
}

int cNation::GetNewStatus(int WarfieldNo)
{	//< LTH-040226-KO 1.4 ��ġ�� �� �����͸� ���� �� �б⸦ �������
	if (!IsNeoWarfield(WarfieldNo))
	{
		if (WarfieldNo < 0 || WarfieldNo >= MAX_NEW_WARFIELD)
			return NW_PEACE;
		return m_NewStatus[WarfieldNo];
	}
	else
	{
		if ((WarfieldNo < WI_HADES_WARFIELD) || WarfieldNo >= WI_MAX_WARFIELD)
			return NW_PEACE;
		return m_btNeoStatus[WarfieldNo];
	}
}	//> LTH-040226-KO

void cNation::SetNewStatus(int WarfieldNo, int Status)
{	//< LTH-040130-KO hades War ������ if���߰�
	if (!IsNeoWarfield(WarfieldNo))
	{
		if (WarfieldNo < 0 || WarfieldNo >= MAX_NEW_WARFIELD)
			return;
		m_NewStatus[WarfieldNo] = Status;
	}
	else
	{
		if ((WarfieldNo >= WI_HADES_WARFIELD) && (WarfieldNo < WI_MAX_WARFIELD))
			m_btNeoStatus[WarfieldNo] = Status;
	}
}	//> LTH-040130-KO

int cNation::GetWarfieldPoss(int WarfieldNo)
{
	if (WarfieldNo < 0 || WarfieldNo >= MAX_NEW_WARFIELD)
		return -1;
	return m_Possession[WarfieldNo];
}

void cNation::SetWarfieldPoss(int WarfieldNo, int Nation)
{
	m_Possession[WarfieldNo] = Nation;
}

bool cNation::LoadMonsterPoint()				// ��ȣ��,������ ���� ��Ʈ�� ó�� 
{
	//���� ���Ͽ��� �о� �´�. 
	// Nation 1______________________________________________________________________________________
	m_tMonsterPoint[0][0].MonsterIndex = 17;
	m_tMonsterPoint[0][0].Size = MAP0_133;
	m_tMonsterPoint[0][0].Points = new POINTS[m_tMonsterPoint[0][0].Size];
	memcpy(m_tMonsterPoint[0][0].Points, Map0_133, sizeof(POINTS) * m_tMonsterPoint[0][0].Size);

	m_tMonsterPoint[0][1].MonsterIndex = 18;
	m_tMonsterPoint[0][1].Size = MAP0_138;
	m_tMonsterPoint[0][1].Points = new POINTS[m_tMonsterPoint[0][1].Size];
	memcpy(m_tMonsterPoint[0][1].Points, Map0_138, sizeof(POINTS) * m_tMonsterPoint[0][1].Size);

	m_tMonsterPoint[0][2].MonsterIndex = 19;
	m_tMonsterPoint[0][2].Size = MAP0_203;
	m_tMonsterPoint[0][2].Points = new POINTS[m_tMonsterPoint[0][2].Size];
	memcpy(m_tMonsterPoint[0][2].Points, Map0_203, sizeof(POINTS) * m_tMonsterPoint[0][2].Size);

	m_tMonsterPoint[0][3].MonsterIndex = 20;
	m_tMonsterPoint[0][3].Size = MAP0_198;
	m_tMonsterPoint[0][3].Points = new POINTS[m_tMonsterPoint[0][3].Size];
	memcpy(m_tMonsterPoint[0][3].Points, Map0_198, sizeof(POINTS) * m_tMonsterPoint[0][3].Size);

	m_tMonsterPoint[0][4].MonsterIndex = 41;
	m_tMonsterPoint[0][4].Size = MAP0_263;
	m_tMonsterPoint[0][4].Points = new POINTS[m_tMonsterPoint[0][4].Size];
	memcpy(m_tMonsterPoint[0][4].Points, Map0_263, sizeof(POINTS) * m_tMonsterPoint[0][4].Size);

	m_tMonsterPoint[0][5].MonsterIndex = 42;
	m_tMonsterPoint[0][5].Size = MAP0_268;
	m_tMonsterPoint[0][5].Points = new POINTS[m_tMonsterPoint[0][5].Size];
	memcpy(m_tMonsterPoint[0][5].Points, Map0_268, sizeof(POINTS) * m_tMonsterPoint[0][5].Size);
	// ____________________________________________________________________________________________

	// Nation 2______________________________________________________________________________________
	m_tMonsterPoint[1][0].MonsterIndex = 21;
	m_tMonsterPoint[1][0].Size = MAP1_450;
	m_tMonsterPoint[1][0].Points = new POINTS[m_tMonsterPoint[1][0].Size];
	memcpy(m_tMonsterPoint[1][0].Points, Map1_450, sizeof(POINTS) * m_tMonsterPoint[1][0].Size);

	m_tMonsterPoint[1][1].MonsterIndex = 22;
	m_tMonsterPoint[1][1].Size = MAP1_527;
	m_tMonsterPoint[1][1].Points = new POINTS[m_tMonsterPoint[1][1].Size];
	memcpy(m_tMonsterPoint[1][1].Points, Map1_527, sizeof(POINTS) * m_tMonsterPoint[1][1].Size);

	m_tMonsterPoint[1][2].MonsterIndex = 23;
	m_tMonsterPoint[1][2].Size = MAP1_524;
	m_tMonsterPoint[1][2].Points = new POINTS[m_tMonsterPoint[1][2].Size];
	memcpy(m_tMonsterPoint[1][2].Points, Map1_524, sizeof(POINTS) * m_tMonsterPoint[1][2].Size);
	// ____________________________________________________________________________________________

	// Nation 3______________________________________________________________________________________
	m_tMonsterPoint[2][0].MonsterIndex = 39;
	m_tMonsterPoint[2][0].Size = MAP2_274;
	m_tMonsterPoint[2][0].Points = new POINTS[m_tMonsterPoint[2][0].Size];
	memcpy(m_tMonsterPoint[2][0].Points, Map2_274, sizeof(POINTS) * m_tMonsterPoint[2][0].Size);

	m_tMonsterPoint[2][1].MonsterIndex = 40;
	m_tMonsterPoint[2][1].Size = MAP2_279;
	m_tMonsterPoint[2][1].Points = new POINTS[m_tMonsterPoint[2][1].Size];
	memcpy(m_tMonsterPoint[2][1].Points, Map2_279, sizeof(POINTS) * m_tMonsterPoint[2][1].Size);

	m_tMonsterPoint[2][2].MonsterIndex = 37;
	m_tMonsterPoint[2][2].Size = MAP2_151;
	m_tMonsterPoint[2][2].Points = new POINTS[m_tMonsterPoint[2][2].Size];
	memcpy(m_tMonsterPoint[2][2].Points, Map2_151, sizeof(POINTS) * m_tMonsterPoint[2][2].Size);

	m_tMonsterPoint[2][3].MonsterIndex = 38;
	m_tMonsterPoint[2][3].Size = MAP2_156;
	m_tMonsterPoint[2][3].Points = new POINTS[m_tMonsterPoint[2][3].Size];
	memcpy(m_tMonsterPoint[2][3].Points, Map2_156, sizeof(POINTS) * m_tMonsterPoint[2][3].Size);

	m_tMonsterPoint[2][4].MonsterIndex = 35;
	m_tMonsterPoint[2][4].Size = MAP2_114;
	m_tMonsterPoint[2][4].Points = new POINTS[m_tMonsterPoint[2][4].Size];
	memcpy(m_tMonsterPoint[2][4].Points, Map2_114, sizeof(POINTS) * m_tMonsterPoint[2][4].Size);

	m_tMonsterPoint[2][5].MonsterIndex = 36;
	m_tMonsterPoint[2][5].Size = MAP2_119;
	m_tMonsterPoint[2][5].Points = new POINTS[m_tMonsterPoint[2][5].Size];
	memcpy(m_tMonsterPoint[2][5].Points, Map2_119, sizeof(POINTS) * m_tMonsterPoint[2][5].Size);



	// ____________________________________________________________________________________________


	return true;

}

void cNation::SaveMonsterPoint()
{
}

void cNation::SetTileDont(int WarfieldNo, int MonNo, int type) //���� SQL �ʵ� INDEX, 0,1 : 
{
	for (int i = 0; i < MAX_MONSTER_POINT; i++)
	{
		if (m_tMonsterPoint[WarfieldNo][i].MonsterIndex == MonNo)
		{
			for (int j = 0; j < m_tMonsterPoint[WarfieldNo][i].Size; j++)
			{
				TileMap[m_tMonsterPoint[WarfieldNo][i].Points[j].x][m_tMonsterPoint[WarfieldNo][i].Points[j].y].attr_dont = type;
			}
		}
	}
}


void cNation::TileDont(int WarfieldNo, int TypeNum, int Value)
{
	for (int i = 0; i < MAX_MONSTER_POINT; i++)
	{
		if (m_tMonsterPoint[WarfieldNo][i].MonsterIndex == TypeNum)
		{
			SetTileDont(WarfieldNo, m_tMonsterPoint[WarfieldNo][i].MonsterIndex, Value);
		}
	}
}

CDontManager::CDontManager()
{
	m_pDontData = NULL;
	m_iSize = 0;
}

CDontManager::~CDontManager()
{
	Clear();
}

void CDontManager::Clear()
{
	if (m_pDontData)
	{
		for (int i = 0; i < m_iSize; i++)
		{
			SAFE_DELETE_ARRAY(m_pDontData[i].lpPoint);
		}
		SAFE_DELETE_ARRAY(m_pDontData);
	}
	m_iSize = 0;
}

void CDontManager::LoadData()
{
    // [Android] 1. 使用 SDL_RWFromFile 開啟檔案
    // 注意路徑：Android 的 AssetManager 通常不區分大小寫，但最好確保檔名大小寫一致
    SDL_RWops* fp = SDL_RWFromFile("data/LTS000.DAT", "rb");

    if (!fp) {
        // 加入 Log 方便除錯
        SDL_Log("CDontManager::LoadData Error: Could not open Data/LTS000.DAT");
        return;
    }

    Clear();

    // [Android] 2. 讀取 m_iSize
    SDL_RWread(fp, &m_iSize, sizeof(int), 1);

    m_pDontData = new DONTDATA[m_iSize];

    for (int i = 0; i < m_iSize; i++)
    {
        // [Android] 3. 讀取 FieldSize
        SDL_RWread(fp, &m_pDontData[i].FieldSize, sizeof(int), 1);

        m_pDontData[i].lpPoint = new POINT[m_pDontData[i].FieldSize];

        // [Android] 4. 讀取 POINT 陣列
        // 讀取數量為 FieldSize 個 POINT 結構
        SDL_RWread(fp, m_pDontData[i].lpPoint, sizeof(POINT), m_pDontData[i].FieldSize);
    }

    // [Android] 5. ★修復原版 Bug：關閉檔案★
    // 原來的代碼這裡漏了 fclose，這在長時間運行後會導致崩潰
    SDL_RWclose(fp);

    return;
}

void CDontManager::ActiveDont(int DontNo, int type)
{
	if (m_pDontData == NULL)
		return;

	if (DontNo < 0 || DontNo >= m_iSize)
		return;

	POINT tempPoint;

	for (int i = 0; i < m_pDontData[DontNo].FieldSize; i++)
	{
		tempPoint = m_pDontData[DontNo].lpPoint[i];
		if (type)
			TileMap[tempPoint.x][tempPoint.y].attr_dont = 1;
		else
			TileMap[tempPoint.x][tempPoint.y].attr_dont = 0;
	}
}

void SendWarMove()
{
	int WarfieldNo = CheckNationStatus(NW_WAR);
	if (WarfieldNo == -1)
	{
		CallOkCancelMessageBox(TRUE, 0, 0, lan->OutputMessage(0, 299), FALSE); // ���� �������� ���� �����ϴ�.
		return;
	}
	SendCMD_NW_MAP_MOVE(1, WarfieldNo);		// NewType
}

void CallMapMoveMenu(int WarfieldNo)
{
	if (WarfieldNo >= 0 && WarfieldNo <= 2)
	{
		SMenu[MN_WAR_MOVE].nField[0].nRectImage = WarfieldNo + 1;
		SMenu[MN_WAR_MOVE].nField[0].nImageNumber = WarfieldNo + 1;
		if (g_pNation->GetWarfieldPoss(WarfieldNo) == NW_BY)		// ����ڰ� ���� 
		{
			SMenu[MN_WAR_MOVE].nField[3].nRectImage = 12;				// ������
			SMenu[MN_WAR_MOVE].nField[3].nImageNumber = 12;
			SMenu[MN_WAR_MOVE].nField[4].nRectImage = 13;				// ����� 
			SMenu[MN_WAR_MOVE].nField[4].nImageNumber = 13;
			SMenu[MN_WAR_MOVE].nField[5].nRectImage = 11;				// ȭ��ǥ
			SMenu[MN_WAR_MOVE].nField[5].nImageNumber = 11;
		}
		else													// ����ڰ� ����
		{
			SMenu[MN_WAR_MOVE].nField[3].nRectImage = 13;				// ������
			SMenu[MN_WAR_MOVE].nField[3].nImageNumber = 13;
			SMenu[MN_WAR_MOVE].nField[4].nRectImage = 12;				// ����� 
			SMenu[MN_WAR_MOVE].nField[4].nImageNumber = 12;
			SMenu[MN_WAR_MOVE].nField[5].nRectImage = 11;				// ȭ��ǥ
			SMenu[MN_WAR_MOVE].nField[5].nImageNumber = 11;
		}

		switch (WarfieldNo)
		{
		case 0:	// ���� 
			SMenu[MN_WAR_MOVE].nField[3].x = 186;
			SMenu[MN_WAR_MOVE].nField[3].y = 207;
			SMenu[MN_WAR_MOVE].nField[4].x = 84;
			SMenu[MN_WAR_MOVE].nField[4].y = 106;
			SMenu[MN_WAR_MOVE].nField[5].x = 135;
			SMenu[MN_WAR_MOVE].nField[5].y = 157;
			break;
		case 2:
			SMenu[MN_WAR_MOVE].nField[3].x = 184;
			SMenu[MN_WAR_MOVE].nField[3].y = 207;
			SMenu[MN_WAR_MOVE].nField[4].x = 84;
			SMenu[MN_WAR_MOVE].nField[4].y = 106;
			SMenu[MN_WAR_MOVE].nField[5].x = 135;
			SMenu[MN_WAR_MOVE].nField[5].y = 157;
			break;
		case 3:
			SMenu[MN_WAR_MOVE].nField[3].x = 186;
			SMenu[MN_WAR_MOVE].nField[3].y = 207;
			SMenu[MN_WAR_MOVE].nField[4].x = 84;
			SMenu[MN_WAR_MOVE].nField[4].y = 106;
			SMenu[MN_WAR_MOVE].nField[5].x = 135;
			SMenu[MN_WAR_MOVE].nField[5].y = 157;
			break;
		}
	}
	else														// New Warfield
	{	//< LTH-040309-KO 1.4 Patch.
		//<! BBD 040311 ������ ���Ϳ� ���� �ٸ��� �۵��Ѵ�
		if (!IsNeoWarfield(WarfieldNo))
		{	// ����������

			SMenu[MN_WAR_MOVE].nField[0].nRectImage = 4;				// ���������� �׸����� �ٲ۴�.
			SMenu[MN_WAR_MOVE].nField[0].nImageNumber = 4;
			SMenu[MN_WAR_MOVE].nField[3].nRectImage = 12;				// ������
			SMenu[MN_WAR_MOVE].nField[3].nImageNumber = 12;
			SMenu[MN_WAR_MOVE].nField[3].x = 161;
			SMenu[MN_WAR_MOVE].nField[3].y = 97;
			SMenu[MN_WAR_MOVE].nField[4].nRectImage = 13;				// ����� 
			SMenu[MN_WAR_MOVE].nField[4].nImageNumber = 13;
			SMenu[MN_WAR_MOVE].nField[4].x = 38;
			SMenu[MN_WAR_MOVE].nField[4].y = 96;
			SMenu[MN_WAR_MOVE].nField[5].nRectImage = 15;				// ȭ��ǥ
			SMenu[MN_WAR_MOVE].nField[5].nImageNumber = 15;
			SMenu[MN_WAR_MOVE].nField[5].x = 80;
			SMenu[MN_WAR_MOVE].nField[5].y = 154;
		}
		else
		{	// ���� ������
			SMenu[MN_WAR_MOVE].nField[0].nImageNumber = 16;				// ���� ������ �׸����� �ٲ۴�.
			SMenu[MN_WAR_MOVE].nField[3].nType = FT_NONE;				// �����ڰ� �����̰� ǥ�� ���Ѵ�
			SMenu[MN_WAR_MOVE].nField[4].nType = FT_NONE;
			SMenu[MN_WAR_MOVE].nField[5].nType = FT_NONE;
		}
		//> BBD 040311 ������ ���Ϳ� ���� �ٸ��� �۵��Ѵ�
	}	//> LTH-040309-KO

	CallSmallMenu(MN_WAR_MOVE);
}

void SendWarStatusRefresh()
{

}