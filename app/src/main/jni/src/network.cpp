/*****************************************************************************\
* Copyright (c), Future Entertainment World / Seoul, Republic of Korea        *
* All Rights Reserved.                                                        *
*                                                                             *
* This document contains proprietary and confidential information.  No        *
* parts of this document or the computer program it embodies may be in        *
* any way copied, duplicated, reproduced, translated into a different         *
* programming language, or distributed to any person, company, or             *
* corporation without the prior written consent of Future Entertainment World *
\*****************************************************************************/

#include "stdafx.h"																	
#include "String.h"
#include <stdlib.h>//010707 lsw
#include <SDL_net.h>
#include "Dragon.h"	
#include "network.h"																	
#include "char.h"
#include "chat.h"
#include "Hong_Sprite.h"														
#include "Hong_Sub.h"														
#include "CharDataTable.h"													
#include "Path.h"																
#include "Hong_Light.h"
#include "Effect.h"
#include "DirectSound.h"
#include "Map.h"
#include "Object.h"
#include "Menu.H"
#include "GameProc.h"
#include "SmallMenu.h"
#include "Tool.h"
#include "Skill.h"
#include "Item.h"
#include "House.h"
#include "LineCommand.h"
#include "NPC_Pattern.h"
#include "mousecursor.h"
#include "SkillTool.h"
#include "SealStone.h"
#include "Horse.h"				// LTS HORSERIDER
#include "CMessage.h"	// 030325 kyo �±� ���� �޽���
#include "MenuSet.h"
#include "SymbolMgr_Client.h" //soto-1.02
#include "LottoMgr.h"
#include "FrameMgr.h"
#include "ArenaManager.h"
#include "NumberAniMgr.h"
#include "CrackMgr.h"
#include "DualMgr.h"
#include "HWIDManager.h"
extern CSymbolMgr_Client	g_SymBolMgr;//soto-1.02
extern POS			IP_base;
enum eBillingType//-������ ���� �ؾ� �մϴ�. CheckPW_TotalDB_SQL�� ���� ��¥(���)�� �����ϱ� �����Դϴ�.
{//MapServer�� �׻� ����ȭ ��Ű�ʽÿ�
	BT_WRONG_ID = -1,//ID Ʋ��
	BT_WRONG_PW = -2,//PW Ʋ��

	BT_NEED_PAY = -3,//�� ����
	BT_BANNED = -5,//�� ����
	BT_COMMERCIAL_TIME_REMAIN = -4,//���� �ð� �ִ³�
	BT_COMMERCIAL_IP = -20,//IP ������
	BT_FREE = -100,//��¥
	BT_WAIT_BILLING_MSG = -200,//�븸 ���� ���� ó�� ��� ����� ��ٸ����� ó��
};

CHSEL_STREAM	en;
CHSEL_STREAM	de;
//011121 lsw <
bool g_bLogOnFlag = false;
short g_nBillingType = 0;//��� ����� ���� �մϴ�. ����� ���� ���Ⱓ�� ���Ҵ°��� �ǹ��մϴ�.

DWORD ServerAccelTime = SDL_GetTicks();
DWORD g_ClientTime_accel = SDL_GetTicks();
DWORD	g_ClientTimeNoGap;

extern int g_SmallMapNumber;
#include "SDLRenderer.h"
class CGuardianGuild;
CGuardianGuild	g_CGuardianGuild; // ��Ű�� ��� ���� ���� Ŭ����.

int NPCCOUNT;
extern void CallLoginMenu(const bool bForceLoginActive = false);
extern int HandleCommand3(t_packet& packet, t_connection* c);	// 021023 kyo 
extern short int g_RealDate;
extern bool g_bStopWeatherSystem; // CSD-CN-031215
extern bool g_bOffWeatherSystem; // CSD-CN-031217
extern bool g_block_move;
extern bool g_block_attack;
extern bool g_block_magic;
extern bool g_block_GiveLife;
///////////////////////////////////////////////////////////////////////////////
// global variables

t_connection	connections;

Uint32			global_time;
Uint32			alive_time;

int				ListenCommand;
int				HeroServerID;


char			HeroName[NM_LENGTH];

///////////////////////////////////////////////////////////////////////////////
// function prototypes.
int			ClearServer(t_connection* c);
int			ConnectServer(t_connection* c, std::string lpIP, int nPort);
int			ConnectLogin(t_connection* c, std::string ID, std::string PW);

void		SendChatData(char* lpStr);
void		RecvChatData(t_server_chat_data* p);
void		SendMoveData(LPCHARACTER ch);
void		RecvMoveData(t_server_move* p);
void		RecvWeatherData(t_server_weather* p);


void		SendDropItem(int itemno, int x, int y, unsigned int attr[6]);
void		SendTakeItem(int item_id);


int			SendCreateCharacter(SCharacter* s);
void		SendAllReady(void);
void		SendLogOut(void);


void		RecvUserAdd(t_server_user_add* c);
void		RecvUserRemove(int server_id);
int			SendUserRemove(void);

int			QueuePacket(t_packet* packet, int type);
int			DequeuePacket(t_packet* packet, int type);
int			HandleCommand(t_connection* c);
int			HandleReading(t_connection* c);
int			HandleWriting(t_connection* c);
void		SendAliveCheckRoutine(void);
int			HandleRunning(t_connection* c, bool flag);
int			HandleCommand2(t_packet& packet, t_connection* c);//011212 lsw

int			SendChoiceCharacter(int ch);

void		SendEvent(int type, int id);

void		SendMoveItem(POS s, POS t);
void		RecvChrItemInfo_0(t_chr_item_info0* i);
void		RecvChrItemInfo_1(t_chr_item_info1* i);
void		RecvItemEquip(t_chr_status_info* chr);		// ���� �߰�

void		SendChangeEquip(LPCHARACTER ch);


void		SendMagicInfo(int target_id, int x, int y, int magic_no);

void		SendPeaceBattle(LPCHARACTER ch);
void		RecvPeaceBattle(int server_id, int peacests);


void Recv_CMD_SEALSTONE_WAR_START(void);
void Recv_CMD_SEALSTONE_WAR_LEFTTIME(DWORD t);
void Recv_CMD_WAR_START_BLOCK(void);
void Recv_CMD_SEALSTONE_RESULT_DEFEAT(void);
void Recv_CMD_SEALSTONE_RESULT_VICTORY(void);
void Recv_CMD_SEALSTONE_RESULT_DRWAW(void);
void Recv_CMD_SEALSTONE_STATUS(t_sealstone_client_status* tp);
void Recv_CMD_NATION2NATION_RELATION_RESULT(t_packet* p);


int GetAgentServerIPAddress(t_connection* c);



extern bool isNationWarfieldServer();// soto-030325

//---------------------------------------------------------------------------------
int ClearServer(t_connection* c)
{
	SDL_Log("ClearServer...");

	if (c == NULL) return 0;
	if (c->socket == 0) return 0;

    c->inbuf.clear();
    c->outbuf.clear();

	// 5. SDL_net 資源清理
	// 確保 socket 和 socketSet 存在才進行操作，防止對空指針操作
	if (c->socket && c->socketSet) {
		// 先從 SocketSet 中移除 socket，然後再關閉 socket。
		// SDLNet_TCP_DelSocket 會檢查 c->socketSet 是否為空，所以這裡可以只檢查 c->socketSet 是否有效
		SDLNet_TCP_DelSocket(c->socketSet, c->socket);

		SDLNet_TCP_Close(c->socket);
		c->socket = nullptr; // 將其設置為空指針

		SDLNet_FreeSocketSet(c->socketSet);
		c->socketSet = nullptr;
	}

	return 1;
}

//---------------------------------------------------------------------------------
bool Initialize(void)
{
	//::Sleep(300);
	//::WSACleanup();
	/*WSADATA WSAData = { 0, };
	if (::WSAStartup(WINSOCK_VERSION, &WSAData))//��ŸƮ��
	{//����
		const int iError = ::WSAGetLastError();
		JustMsg("WSAStartup failed. %d", iError);
		return false;
	}*/
	return true;//����
};

//---------------------------------------------------------------------------------
void Release(t_connection* c)
{
	ClearServer(c);
};

int ConnectServer(t_connection* c, std::string lpIP, int nPort)
{
	if (!c) {
		return -1;
	}

	if (lpIP.empty() || nPort <= 0) {
		return -1;
	}

	// 1. 如果已有連線，先正常關閉
	if (c->socket) {
		SDLNet_TCP_Close(c->socket);
		c->socket = nullptr;
	}
	if (c->socketSet) { // 也清理 socket set
		SDLNet_FreeSocketSet(c->socketSet);
		c->socketSet = nullptr;
	}


	// 2. 解析主機位址
	IPaddress ip;
	if (SDLNet_ResolveHost(&ip, lpIP.c_str(), nPort) == -1) {
		SDL_Log("SDLNet_ResolveHost Error: %s", SDLNet_GetError());
		return -1;
	}
	
	// 3. 建立並嘗試連接 (這是阻塞操作)
	c->socket = SDLNet_TCP_Open(&ip);
	if (!c->socket) {
		SDL_Log("SDLNet_TCP_Open Error: %s", SDLNet_GetError());
		return -1;
	}
	
	// 5. 為主迴圈的輪詢機制，準備 SocketSet (這一步是必需的！)
	c->socketSet = SDLNet_AllocSocketSet(1);
	if (!c->socketSet) {
		SDL_Log("SDLNet_AllocSocketSet Error: %s", SDLNet_GetError());
		SDLNet_TCP_Close(c->socket);
		c->socket = nullptr;
		return -1;
	}

	if (SDLNet_TCP_AddSocket(c->socketSet, c->socket) == -1) {
		SDL_Log("SDLNet_TCP_AddSocket Error: %s", SDLNet_GetError());
		SDLNet_FreeSocketSet(c->socketSet);
		SDLNet_TCP_Close(c->socket);
		c->socket = nullptr;
		return -1;
	}

	// 6. 初始化連線狀態
	memset(&c->packet, 0, sizeof(c->packet));
	c->packet_pnt = 0;
	c->packet_size = 2;
	c->packet_status = 0;
	alive_time = SDL_GetTicks();
	c->state = CONNECT_OK;

	return 1;
}
/*
//-------------------------------------------------------------------------------------
int ConnectServer(t_connection* c, std::string lpIP, int nPort)
{
	int CUSTOM_INVALID_SOCKET = -2;
	if (!c) {
		return CUSTOM_INVALID_SOCKET;
	}

	if (lpIP.empty() || nPort <= 0) {
		return CUSTOM_INVALID_SOCKET;
	}

	if (c->socket) {
		SDLNet_TCP_Close(c->socket);
		c->socket = nullptr;
	}

	IPaddress ip;
	if (SDLNet_ResolveHost(&ip, lpIP.c_str(), nPort) == -1) {
		SDL_Log("SDLNet_ResolveHost Error: %s", SDLNet_GetError());
		return CUSTOM_INVALID_SOCKET;
	}

	// 嘗試創建 socket，但不立即連接
	c->socket = SDLNet_TCP_Open(&ip); // 這裡打開會嘗試連接
	if (!c->socket) {
		SDL_Log("SDLNet_TCP_Open Error: %s", SDL_GetError());
		return CUSTOM_INVALID_SOCKET;
	}

	// 使用 SDLNet_SocketSet 來模擬非阻塞連接和超時
	// 注意：SDLNet_TCP_Open 在內部可能會阻塞，如果需要真正的非阻塞，
	// 可能需要使用更底層的 SDLNet_ResolveHost -> SDLNet_Socket -> SDLNet_TCP_Connect
	// 然而，SDLNet_TCP_Open 已經是 SDL_Net 建議的方式。
	// 如果連接失敗，SDLNet_TCP_Open 會返回 NULL。
	// 這裡的邏輯主要用於檢查 socket 是否可以寫入 (即連接是否建立)。

	c->socketSet = SDLNet_AllocSocketSet(1); // 監控一個 socket
	if (!c->socketSet) {
		SDL_Log("SDLNet_AllocSocketSet Error: %s", SDL_GetError());
		SDLNet_TCP_Close(c->socket);
		c->socket = nullptr;
		return CUSTOM_INVALID_SOCKET;
	}

	// 將我們的 socket 加入 set 中
    if (SDLNet_TCP_AddSocket(c->socketSet, c->socket) == -1)
	{
		SDL_Log("SDLNet_TCP_AddSocket: %s\n", SDLNet_GetError());
        SDLNet_FreeSocketSet(c->socketSet);  // 清理已配置的 set
        SDLNet_TCP_Close(c->socket);
        c->socket = NULL;
		return -1;
	}

	// 初始化 packet 相關成員
	memset(&c->packet, 0, sizeof(c->packet));
	c->packet_pnt = 0;
	c->packet_size = 2;
	c->packet_status = 0;

	alive_time = SDL_GetTicks();

	c->state = CONNECT_OK;
	return 1;
}
*/
//----------------------------------------------------------------------//acer7
// 定義一個常數，讓程式碼更清晰
// C++11 以後支援的編譯時常數函數
constexpr char ConvChar_constexpr(char a) { return a ^ 0x7A; }

// 輔助結構，在編譯時對字串進行加密
template <size_t N>
struct EncryptedString {
  char data[N];
  constexpr EncryptedString(const char* str) : data{} {
    // 使用固定迴圈，不依賴 \0
    for (size_t i = 0; i < N; ++i) {
      data[i] = ConvChar_constexpr(str[i]);
    }
  }
  // 不再需要 GetLength()，因為我們使用固定長度
};

class CMyCodeString {
 private:
  // 將靜態成員的字串在編譯時就加密
  // 我們需要確保這個字串的長度就是 MAX_MY_STRING (20)
  // 這樣可以避免 \0 字符的影響
  // 確保你的字串長度恰好是 19 個字元，這樣加上 \0 總共是 20
  // 如果不足 19 個，請在後面補上空格
  static constexpr EncryptedString<MAX_MY_STRING> encrypted_string = "gemini is goodfffff";

 public:
  CMyCodeString() {
    // 建構函式是空的，因為資料已經在編譯時設定
  }

  // 獲取解密後的字元
  char GetChar(int index) {
    if (index >= 0 && index < MAX_MY_STRING) {
      // 只在需要時進行解密
      return ConvChar_constexpr(encrypted_string.data[index]);
    }
    return '\0';
  }

  // 返回固定長度
  int GetCount() { return MAX_MY_STRING; }
};

int SetMyCode(short int& mycode, char* mystring) {
  CMyCodeString MY_STRING;
  g_GameInfo.g_MyCode = mycode = 3177;
  // 使用固定迴圈，不再依賴 strlen()
  for (int i = 0; i < MAX_MY_STRING; i++) {
    mystring[i] = MY_STRING.GetChar(i);
  }
  // 在這裡不必要再加 \0，因為陣列本身就是 20 位元組
  // 如果需要，可以手動加上 mystring[MAX_MY_STRING - 1] = '\0';

  return 1;
}
//-------------------------------------------------------

//020101 lsw
int ConnectLogin(t_connection* c, std::string ID, std::string PW, int LineNum)
{
#ifdef _DEBUG // finito 060507
	if (SysInfo.notconectserver) { return 1; }
#endif	
	t_packet	packet;
	memset(&packet, 0, sizeof(t_packet));
	packet.h.header.type = CMD_ACCESS_LOGIN;
	packet.h.header.size = sizeof(t_ClientAccessLogin);

	packet.u.ClientAccessLogin.init = en.GetHSELCustomizeOption();

	memcpy(packet.u.ClientAccessLogin.id, ID.data(), ID.length());
	memcpy(packet.u.ClientAccessLogin.pw, PW.data(), PW.length());

    /*std::string hwid = HWIDManager::GetHWID();
    strncpy(packet.u.ClientAccessLogin.szHWID, hwid.c_str(), 19);
    packet.u.ClientAccessLogin.szHWID[19]= '\0';*/

#ifdef _DEBUG
	packet.u.ClientAccessLogin.version = GM_TOOL_VERSION;
#else
	packet.u.ClientAccessLogin.version = 0;
#endif

	::SetMyCode(packet.u.ClientAccessLogin.mycode, packet.u.ClientAccessLogin.mystring);
    packet.u.ClientAccessLogin.Foo = sizeof(packet.u.ClientAccessLogin);
	::memset(packet.u.ClientAccessLogin.User_ID, 0, sizeof(packet.u.ClientAccessLogin.User_ID));
	::memset(packet.u.ClientAccessLogin.GateWayIP, 0, sizeof(packet.u.ClientAccessLogin.GateWayIP));

	/*if (g_pBill->ReturnLineNum() >= 0)
	{
		packet.u.ClientAccessLogin.Corp_Code = LineNum;
		::strcpy(packet.u.ClientAccessLogin.GateWayIP, g_pBill->ReturnIPaddress());
		::strcpy(packet.u.ClientAccessLogin.User_ID, g_pBill->ReturnUserID());
		packet.u.ClientAccessLogin.GateWayPORT = g_pBill->ReturnPORTNumber();
	}
	else*/
	{
		packet.u.ClientAccessLogin.Corp_Code = 0;
		::strcpy(packet.u.ClientAccessLogin.GateWayIP, "");
		::strcpy(packet.u.ClientAccessLogin.User_ID, "");
		packet.u.ClientAccessLogin.GateWayPORT = 0;
	}

	::QueuePacket(&packet, 1);
	DWORD StartTime = SDL_GetTicks();
	ListenCommand = CMD_NONE;
	while (1)
	{
		const DWORD Duration = SDL_GetTicks() - StartTime;

		if (Duration > WAIT_TIME_RESPONSE_DURATION)
		{
			return(-1);
		}
		if (::HandleRunning(c) <= 0)
		{
			::Release(c);
			return(-1);
		}

		switch (ListenCommand)
		{
		case CMD_INVALID_ID: { return(-3); }break;
		case CMD_INVALID_PW: { return(-4); }break;
		case CMD_USED_ID: { return(-5); }break;
		case CMD_INVALID_VERSION: { return(-6); }break;
		case CMD_INVALID_DB: { return(-7); }break;
		case CMD_NOT_EXIST_GAMESERVER: { return(-8); }break;
		case CMD_BANNED_PLAYER: { return(-18); }break;
		case CMD_INVALID_PAY: { return(-10); }break;
		case CMD_SO_MUCH_LOGINUSER_WAIT_FOR_A_MIN: { return(-11); }break;
		case CMD_LOGIN_FAIL_MASSAGE: { return(-12); }break;
		case CMD_RM_NOW_REPAIR: { return(-13); }break;
		case CMD_INVALID_AGE: { return(-17); }break; // 030929 kyo
		case CMD_ACCEPT_LOGIN:
		{
			::memcpy(c->id, ID.data(), ID.length());
			::memcpy(c->pw, PW.data(), PW.length());
			c->state = CONNECT_LOGIN;
			return(1);
		}break;
		}
	}
	return -8;
}

void SendAllReady(void)
{
	t_packet	packet;
	packet.h.header.type = CMD_ALL_READY;

	packet.u.all_ready.x = Hero->x;
	packet.u.all_ready.y = Hero->y;
#ifdef _DEBUG // finito 060507
	if (GetSysInfo(SI_GAME_MAKE_MODE))
		if (SysInfo.notconectserver == 0 && SysInfo.entergameserver)
		{
			packet.u.all_ready.x = Hero->x = SysInfo.mapx * 32;
			packet.u.all_ready.y = Hero->y = SysInfo.mapy * 32;

			Hero->position.x = SysInfo.mapx;
			Hero->position.y = SysInfo.mapy;

			StartSetView(Hero);
		}
#endif
	packet.h.header.size = sizeof(t_all_ready);
	QueuePacket(&packet, 1);

	// 021117 YGI
#ifdef LIGHT_VERSION_
	CallServer(CMD_MOVEMAP_FOR_LIGHT_VERSION);		// light �����̶�� ����� ������ �˸���.
#endif

#ifdef _DEBUG // finito 060507
	if (GetSysInfo(SI_GAME_MAKE_MODE))	SDL_Log("CMD_ALL_READY");
#endif

	SendRequestTileDontData();  // 010904 LTS   //���̵��Ǵ�.. //������ ���̸�.. //�Ʊ�����//������ȭ �������..
	SendCMD_REQUEST_COMPARE_PHASE();	// LTS LOCALWAR
}

// 010515 KHS	
void SendMoveData(LPCHARACTER ch)
{
	t_packet	packet{};
	int		i;
	int		dir;
	bool bOdd =false;
	char szData[MAX_PATH] = { 0, };
	int nx, ny, px, py;
	int c = 0;

	if (ch->eventnpc) return;

	if (ch->name[0] == '\0') return;


	LPCHARACTER pNpc = ReturnCharacterPoint(ch->id);
	if (pNpc == NULL)
	{
		return;
	}

	if (pNpc->eventnpc) return;

	if (pNpc->name[0] == '\0') return;

	if (ch->id == Hero->id) {
		if (!real_move) {
			return;
		}

		// 如果 ch->pathcount 是一個明顯的垃圾值 (例如 50 或更高)
		const int MAX_SAFE_PATH_LENGTH = 40; // 請根據遊戲設計選擇一個合理上限

		if (ch->pathcount > MAX_SAFE_PATH_LENGTH)
		{
			// 記錄錯誤，並阻止發送這個明顯有問題的封包
			g_DBGLog.Log(LOG_LV1, "SECURITY_CLIENT_TRAP: Illegal pathcount %d detected. Suppressing CMD_MOVE.", ch->pathcount);
			Hero->oldpathcount = Hero->pathcount;
			ch->pathcount = 0; // 重置路徑計數，防止後續誤用
			return; // 立即返回，不執行封包的構建和發送
		}
		// =======================================================
	}
	
	if (ch->pathcount > 2)
	{
		px = ch->path[ch->pathcount - 1][0];
		py = ch->path[ch->pathcount - 1][1];

		for (i = ch->pathcount - 2; i >= 0; i--)
		{
			nx = ch->path[i][0];
			ny = ch->path[i][1];

			if (nx > px)
			{
				if (ny > py) { dir = 7; }
				else if (ny < py) { dir = 5; }
				else { dir = 6; }
			}
			else if (nx < px)
			{
				if (ny > py) { dir = 1; }
				else if (ny < py) { dir = 3; }
				else { dir = 2; }
			}
			else
			{
				if (ny > py) { dir = 0; }
				else { dir = 4; }
			}

			if (bOdd == 0) { szData[(c / 2)] = dir << 4; }
			else { szData[(c / 2)] += dir; }
			c++;
			bOdd = !bOdd;

			px = nx;
			py = ny;
		}
	}

	packet.h.header.type = CMD_MOVE;
	{
		packet.u.client_move.server_id = ch->id;
		packet.u.client_move.length = ch->pathcount;

		packet.u.client_move.movep = ch->MoveP;	// NPC�� ������ ��ħ..

		packet.u.client_move.movetype = ch->movetype;
		packet.u.client_move.sx = (short int)ch->x / TILE_SIZE;
		packet.u.client_move.sy = (short int)ch->y / TILE_SIZE;
		memcpy(packet.u.client_move.dir_tbl, szData, (ch->pathcount + 1) / 2);
		packet.u.client_move.gox = ch->gox;
		packet.u.client_move.goy = ch->goy;
	}
	packet.h.header.size = sizeof(t_client_move) - MAX_DIR_TABLE + (ch->pathcount + 1) / 2;

	QueuePacket(&packet, 1);
	if (ch->id == Hero->id)
	{
		Hero->oldpathcount = Hero->pathcount;
		if (CheckLineCollision(Hero, Hero->x / TILE_SIZE, Hero->y / TILE_SIZE, Hero->gox / TILE_SIZE, Hero->goy / TILE_SIZE))
		{
			Hero->pathcount = 0;
		}
	}
}


//-----------------------------------------------------------------------------
// movetype - 0:walk   1:run

// 010724 KHS
void RecvMoveData(t_server_move* p)
{
	int count, bOdd, i, t, dir;
	int movetype, sx, sy, len;
	unsigned char* tbl;
	int gox, goy;
	CHARACTER tch;
	bool linecollisioncheck;
	bool remakepath = false;
	if (g_CurrentTime == 0) return;

	LPCHARACTER	ch = ReturnCharacterPoint(p->server_id);
	if (ch == NULL)
	{
		return;
	}

	if (IsDead(ch) && ch->viewtype != VIEWTYPE_GHOST_) return;

	switch (ch->sprno)
	{
	case 0:
	case 1: break;
	default:
	{
		if (ch->nCurrentAction == MON1_BATTLE_MAGIC)
		{
			return;
		}

		break;
	}
	}

	if (Hero == ch)
	{
		CharWalkTime(ch, ch->nCurrentAction);
		ch->desttime = p->desttime;
		int temppathcount = ch->pathcount;
		ch->pathcount = ch->oldpathcount;
		CharCalcMovP(ch);
		ch->pathcount = temppathcount;

		return;
	}
	else
	{
		//acer
		if (ch->viewtype == VIEWTYPE_STONE_)
			ch->viewtype = VIEWTYPE_NORMAL_;
	}

	len = p->length;
	tbl = p->dir_tbl;
	sx = p->sx;
	sy = p->sy;
	gox = p->gox;
	goy = p->goy;
	movetype = p->movetype;

	ch->pathcount = len;

	if (p->movetype >= 100)	// ���� ��ġ Setting....						// 0 : �ȱ�    1 : �ٱ�. 
	{
		p->movetype -= 100;
		ch->x = sx * TILE_SIZE + 16;
		ch->y = sy * TILE_SIZE + 16;
	}

	linecollisioncheck = false;
	if (CheckLineCollision(ch, ch->x / TILE_SIZE, ch->y / TILE_SIZE, p->gox / TILE_SIZE, p->goy / TILE_SIZE))
	{
		if (ch->pathcount >= 1000)	ch->pathcount -= 1000;

		linecollisioncheck = true;
		goto NEXT_;
	}

	// 010905 KHS
	TileMap[ch->position.x][ch->position.y].occupied = 0;
	ch->position.x = p->sx;
	ch->position.y = p->sy;
	if (ch->hp > 0) TileMap[ch->position.x][ch->position.y].occupied = 1;	// 010915 LTS
	ch->oldposition.x = p->sx;
	ch->oldposition.y = p->sy;

	bOdd = 0;
	count = 0;
	if (p->length > 1000)
	{
		bOdd++; count++; p->length -= 1000;
	}

	if (len > 1000) len -= 1000;

	{ // ������ Data��� ���� �ʰ� �ٽ� PathBuild()�Ѵ�....
		memcpy(&tch, ch, sizeof(CHARACTER));
		tch.moveFlag = TRUE;
		tch.movetype = movetype;
		tch.gox = gox;
		tch.goy = goy;

		int mx = gox / TILE_SIZE;
		int my = goy / TILE_SIZE;

		tch.destx = mx, tch.desty = my;
		if (PathBuild(&tch) && TileMap[ch->x / TILE_SIZE][ch->y / TILE_SIZE].attr_dont == 0)
		{
			//if( GetSysInfo( SI_GAME_MAKE_MODE ) )	AddCurrentStatusMessage( 100,100,255 , "�ٽ� ����� �̵��մϴ�.");//( %d %d %d %d )", p->server_id, p->gox, p->goy, p->length, p->desttime  );
			memcpy(ch, &tch, sizeof(CHARACTER));
			remakepath = true;
			goto NEXT_;
		}
		else
		{
			///if( GetSysInfo( SI_GAME_MAKE_MODE ) )	AddCurrentStatusMessage( 255,100,255 , "����Data�� �̵��մϴ�.");//( %d %d %d %d )", p->server_id, p->gox, p->goy, p->length, p->desttime  );
		}
	}

	if (len != 0)
	{
		ch->path[len - 1][0] = sx;
		ch->path[len - 1][1] = sy;

		for (i = 1; i < len; i++)
		{
			t = count / 2;
			if (bOdd == 0)	dir = (*(tbl + t) >> 4) & 0x0f;
			else			dir = *(tbl + t) & 0x0f;
			bOdd = !bOdd; count++;

			switch (dir)
			{
			case 0:       		sy++;		break;
			case 1: sx--;		sy++;		break;
			case 2: sx--;		       		break;
			case 3: sx--;		sy--;		break;
			case 4:         	sy--;		break;
			case 5: sx++;		sy--;		break;
			case 6: sx++;		     		break;
			case 7: sx++;		sy++;		break;
			}

			ch->path[len - i - 1][0] = sx;
			ch->path[len - i - 1][1] = sy;
		}
	}

NEXT_:

	if (ch->pathcount > 1000) ch->pathcount -= 1000;


	if (movetype == 0)
	{
		if (ch->pathcount > 5) movetype = 1;
	}

	ch->moveFlag = TRUE;
	ch->gox = gox;
	ch->goy = goy;

	CharDoAni_Moving(ch, movetype);
	if (p->server_id >= 10000)
	{
		ch->movetargetid = p->targetid;
	}

	CharWalkTime(ch, ch->nCurrentAction);
	ch->desttime = p->desttime;
	CharCalcMovP(ch);

	connections.receive = 1;

	if (linecollisioncheck == true)
	{
		ch->pathcount = 0;
		return;
	}


	if (remakepath == true)
		if (CheckLineCollision(ch, p->sx, p->sy, p->gox / TILE_SIZE, p->goy / TILE_SIZE))
		{
			ch->pathcount = 0;
		}
};

void GetRealTime(int& mon, int& day, int& hour)
{
	short int time = g_RealDate;
	mon = time >> 10;
	time -= mon << 10;

	day = time >> 5;
	time -= day << 5;
	hour = time;
}

void RecvWeatherData(t_server_weather* p)
{	//< CSD-CN-031217
	static int old_exp = 0;
	g_GameInfo.exp_event = p->exp_event;
	if (old_exp != g_GameInfo.exp_event) {
		(g_GameInfo.exp_event>0) ?
			AddCurrentStatusMessage(FONT_COLOR_GREEN, lan->OutputMessage(0, 82)):
			AddCurrentStatusMessage(FONT_COLOR_GREEN, lan->OutputMessage(0, 83));
	}
	old_exp = g_GameInfo.exp_event;

	if (g_bStopWeatherSystem)
	{
		return;
	}

	if (g_bOffWeatherSystem)
	{
		WeatherDataProcess(p->weather, p->another, 0, 0, 0, 0);
	}
	else
	{
		WeatherDataProcess(p->weather, p->another, p->rainstart, p->rainend, p->amount, p->temperature);

		
	}
}	//> CSD-CN-031217

void RecvAddItem(t_server_item_add* p)
{
	ITEMGROUND i;

	if (FindItemList(p->item_id)) return;

	i.server_id = p->item_id;
	i.no = (WORD)p->item.item_no;		// 020701 YGI
	i.x = p->x;
	i.y = p->y;
	i.offx = p->offx;
	i.offy = p->offy;
	i.attr[0] = p->item.attr[0];
	i.attr[1] = p->item.attr[1];
	i.attr[2] = p->item.attr[2];
	i.attr[3] = p->item.attr[3];
	i.attr[4] = p->item.attr[4];
	i.attr[5] = p->item.attr[5];

	AddItemList(&i);
	connections.receive = 1;
}

//���� this lsw 
void Unzip(BYTE* pbIn, int iSize, BYTE* pbOut)
{
	int iStatusSize = pbIn[0];
	int iInPos = iStatusSize + 1;		// input buf�� index

	int iOutPos = 0;
	for (int i = 0; i < iStatusSize; i++)
	{
		unsigned char cFlag = pbIn[i + 1];
		for (int j = 0; j < 8; j++, iOutPos++)
		{

			if (cFlag & 0x01)
			{
				pbOut[iOutPos] = pbIn[iInPos++];
			}
			else pbOut[iOutPos] = 0;
			cFlag = cFlag >> 1;
		}
	}
}

void RecvAddItemZiped(t_server_item_add_ziped* p)
{
	t_packet unziped;

	Unzip((BYTE*)p, p->zipedpack[0], (BYTE*)&unziped.u.server_item_add);

	RecvAddItem(&unziped.u.server_item_add);
}

void RecvRemoveItem(int item_id)
{
	DeleteItemList(item_id);
	connections.receive = 1;
}

void RecvYouGotItem(int item_id)		// 0109
{
	LPITEMGROUND i = FindItemList(item_id);
	if (i != NULL)
	{
		HandItemAttr.attr[0] = i->attr[0];		//1214
		HandItemAttr.attr[1] = i->attr[1];
		HandItemAttr.attr[2] = i->attr[2];
		HandItemAttr.attr[3] = i->attr[3];
		HandItemAttr.attr[4] = i->attr[4];
		HandItemAttr.attr[5] = i->attr[5];
		HandItemAttr.item_no = i->no;

		g_MouseItemType = 1;
		g_MouseItemNumber = i->no;
		SetItemPos(GROUND, 0, &IP_base);

		ItemSoundOfGetItem(i->no);
		DeleteItemList(item_id);		//acer7
	}
	connections.receive = 1;
}



void SendDropItem(int itemno, int x, int y, unsigned int attr[6])
{

	if ((attr[IATTR_ATTR] & IA2_LOCK_ITEM) == IA2_LOCK_ITEM)
	{
		if (!isKeyPressed(SDL_SCANCODE_LALT))
		{
			AddCurrentStatusMessage(YELLOW_COLOR, lan->OutputMessage(0, 49));
			return;
		}
	}

	/*if (attr[IATTR_RARE_MAIN]) {
		if (!isKeyPressed(SDL_SCANCODE_LALT))
		{
			AddCurrentStatusMessage(YELLOW_COLOR, lan->OutputMessage(0, 50));
			return;
		}
	}*/


	t_packet	packet;

	packet.h.header.type = CMD_ITEM_DROP;
	{
		packet.u.client_item_drop.item.item_no = itemno;
		packet.u.client_item_drop.item.attr[0] = attr[0];
		packet.u.client_item_drop.item.attr[1] = attr[1];
		packet.u.client_item_drop.item.attr[2] = attr[2];
		packet.u.client_item_drop.item.attr[3] = attr[3];
		packet.u.client_item_drop.item.attr[4] = attr[4];
		packet.u.client_item_drop.item.attr[5] = attr[5];

		packet.u.client_item_drop.x = x;
		packet.u.client_item_drop.y = y;
	}

	packet.h.header.size = sizeof(t_client_item_drop);

	QueuePacket(&packet, 1);
}

void SendTakeItem(int item_id)		// 0109
{
	t_packet	packet;

	packet.h.header.type = CMD_ITEM_TAKE;
	{
		packet.u.client_item_take.item_id = item_id;
	}
	packet.h.header.size = sizeof(t_client_item_take);

	QueuePacket(&packet, 1);
}

// Client -> Server
// Item�� Animation���¸� �ٲ۴�. 
void SendItemStatus(int item_id, unsigned int attr[6])
{
	t_packet	packet;

	packet.h.header.type = CMD_ITEM_STATUS;
	{
		packet.u.client_item_status.item_id = item_id;
		packet.u.client_item_status.attr[0] = attr[0];
		packet.u.client_item_status.attr[1] = attr[1];
		packet.u.client_item_status.attr[2] = attr[2];
		packet.u.client_item_status.attr[3] = attr[3];
		packet.u.client_item_status.attr[4] = attr[4];
		packet.u.client_item_status.attr[5] = attr[5];
	}
	packet.h.header.size = sizeof(t_client_item_status);

	QueuePacket(&packet, 1);
}
// Server -> Client 
// Item�� Animation���¸� �ٲ۴�. 
void RecvChangeItemStatus(int item_id, unsigned int attr[6])
{
	LPITEMGROUND i = FindItemList(item_id);

	if (i == NULL) return;

	i->attr[1] = attr[1];
	i->Anitype = (BYTE)attr[1];

	connections.receive = 1;
}

extern cHorseRider	g_HorseRider;	// LTS HORSERIDER
extern void SetDragonLordWarArmor(t_server_change_equip* p, LPCHARACTER ch);  // LTS DRAGONLORD

void RecvChangeEquip(t_server_change_equip* p)
{
	LPCHARACTER	ch = ReturnCharacterPoint(p->server_id);
	if (ch == NULL) return;

	if (g_FightMapStart)
	{	//< CSD-TW-030618
		CArenaTeam* pTeam = g_pArenaManager->GetMemberTeam(p->server_id);

		if (pTeam != NULL)
		{
			const int nTeam = pTeam->GetTeamNumber();
			p->equip[AT_ARMOR] = fight_map_acc_num[nTeam];
			p->equip[AT_HELMET] = 1;
		}
	}	//> CSD-TW-030618

	if (ch->IsCounselor())		// ���ӳ� �����
	{
		p->equip[AT_ARMOR] = 114;		// ��Ÿ���� ������.
		p->equip[AT_HELMET] = 1;		// �ƹ��͵� ��������
	}
	if (ch->IsReporter()) //Eleval 17/08/09 - To draw reporters' cloths
	{
		p->equip[AT_ARMOR] = 121;
		p->equip[AT_HELMET] = 1;
	}

	if (g_bWarBegin) SetWarArmor(p, ch);	// 010904 LTS //�������̸� 
	if (g_LocalWarBegin) SetLocalWarArmor(p, ch);	// LTS LOCALWAR
	if (ch->DragonLordWarTeam > 0) SetDragonLordWarArmor(p, ch);				// LTS DRAGONLORD


	if (ch->accessory[0] != p->equip[0])
	{
		FreeHeroClothAccessoryDataOne(ch->sprno, ch->accessory[0]);
		FreeHeroClothAccessoryDataOne(ch->sprno, ch->accessory[0] + 1);

		// 011030 KHS JJING
		FreeHeroClothAccessoryDataOne(ch->sprno, ch->accessory[0] + 200);
		FreeHeroClothAccessoryDataOne(ch->sprno, ch->accessory[0] + 201);
	}

	//< soto-030314
#ifndef LIGHT_VERSION_
	// 1���� ��Ű�� ����ϰ��� ������ �����¡~
	if (g_CGuardianGuild.Is1StGuild(ch->GetGuildCode()) && !g_FightMapStart && !ch->JoinLocalWar && !(ch->DragonLordWarTeam > 0) && !isNationWarfieldServer())
	{	//< CSD-030324
		ch->accessory[0] = GUILD_WEAR01;
		LoadHeroClothAccessoryDataOne(ch->sprno, GUILD_WEAR01);
		LoadHeroClothAccessoryDataOne(ch->sprno, GUILD_WEAR01 + 1);
	}	//> CSD-030324
	else
#endif
	{
		ch->accessory[0] = p->equip[0];
		LoadHeroClothAccessoryDataOne(ch->sprno, p->equip[0]);
		LoadHeroClothAccessoryDataOne(ch->sprno, p->equip[0] + 1);
	}

	// 011030 KHS JJING
	ch->jjing = p->jjing;
	if (p->jjing >= 5)
		LoadHeroClothAccessoryDataOne(ch->sprno, p->equip[0] + 200);
	if (p->jjing >= 9)
		LoadHeroClothAccessoryDataOne(ch->sprno, p->equip[0] + 201);


#ifndef LIGHT_VERSION_
	// 1���� ��Ű�� ����ϰ��� ������ �����¡~
	if (g_CGuardianGuild.Is1StGuild(ch->GetGuildCode()) && !g_FightMapStart && !ch->JoinLocalWar && !(ch->DragonLordWarTeam > 0) && !isNationWarfieldServer())
	{	//< CSD-030324
		if (ch->accessory[1] != GUILD_WEAR01)FreeHeroClothAccessoryDataOne(ch->sprno, ch->accessory[1]);
		ch->accessory[1] = GUILD_WEAR01;
		LoadHeroClothAccessoryDataOne(ch->sprno, GUILD_WEAR01);
	}	//> CSD-030324
	else
#endif
	{
		if (ch->accessory[1] != p->equip[1])FreeHeroClothAccessoryDataOne(ch->sprno, ch->accessory[1]);
		ch->accessory[1] = p->equip[1];
		LoadHeroClothAccessoryDataOne(ch->sprno, p->equip[1]);
	}
	//> soto-030314
		//< CSD-031001
	if (ch->accessory[2] != p->equip[2])
	{
		FreeHeroClothAccessoryDataOne(ch->sprno, ch->accessory[2]);
	}

	ch->accessory[2] = p->equip[2];

	const WORD idWeaponItem = p->idWeaponItem;

	if (idWeaponItem / 1000 == 6)
	{	//< CSD-031101
		CItem_Weapon* pWeapon = static_cast<CItem_Weapon*>(ItemUnit(idWeaponItem));

		if (pWeapon != NULL)
		{
			const int nEffect = pWeapon->effect;

			if (nEffect > 0)
			{
				LoadHeroClothAccessoryDataOne(ch->sprno, nEffect);
			}

			switch (ch->sprno)
			{
			case 0: LoadHeroClothAccessoryDataOne(ch->sprno, pWeapon->shape_woman); break;
			case 1: LoadHeroClothAccessoryDataOne(ch->sprno, pWeapon->shape_man); break;
			}
		}
	}

	ch->idWeaponItem = idWeaponItem;
	//> CSD-031001
	if (ch->accessory[3] != p->equip[3])FreeHeroClothAccessoryDataOne(ch->sprno, ch->accessory[3]);
	ch->accessory[3] = p->equip[3];
	LoadHeroClothAccessoryDataOne(ch->sprno, p->equip[3]);

	// 011018 KHS ����
	if (ch->mantle != p->mantle)
	{
		int shape = GetItemAttr2(ch->sprno, ch->mantle, ITEMSHOW);
		FreeHeroClothAccessoryDataOne(ch->sprno, shape);
		// finito 09/08/07 free the extra images for mantle of dragon and mantle of raja
		/*if (shape == 137)
		{
			FreeHeroClothAccessoryDataOne(ch->sprno, 138);
		}
		else if (shape == 145)
		{
			FreeHeroClothAccessoryDataOne(ch->sprno, 146);
			FreeHeroClothAccessoryDataOne(ch->sprno, 147);
		}*/
		// end of free extra images
	}

	ch->mantle = p->mantle;
	int mantleDyeColour = p->mantleDyeColour;

	if (Hero == ch)
	{
		EquipItemAttr[WT_NECK].attr[IATTR_RESERVED1] = mantleDyeColour;
	}

	if (mantleDyeColour > 0)
	{
		int color = mantleDyeColour;
		int r, g, b;
		r = (color >> 16);
		color -= (r << 16);
		g = (color >> 8);
		color -= (g << 8);
		b = color;
		ch->mantlecolor = ReturnBlendRGB(r, g, b);
	}
	else
	{
		ch->mantlecolor = GetItemAttr(ch->mantle, ITEM_COLOR);
	}
	int shape = GetItemAttr2(ch->sprno, p->mantle, ITEMSHOW);
	LoadHeroClothAccessoryDataOne(ch->sprno, shape);
	LoadHeroClothAccessoryDataOne(Hero->sprno, 138);
	LoadHeroClothAccessoryDataOne(Hero->sprno, 146);
	LoadHeroClothAccessoryDataOne(Hero->sprno, 147);
	// finito 08/08/07 load extra images for mantle of dragon and mantle of raja
	/*if (shape == 137)
	{
		LoadHeroClothAccessoryDataOne(Hero->sprno, 138);
	}
	else if (shape == 145)
	{
		LoadHeroClothAccessoryDataOne(Hero->sprno, 146);
		LoadHeroClothAccessoryDataOne(Hero->sprno, 147);
	}*/
	// end of load extra images

	ch->HorseNo = p->Horse[0];		// LTS HORSERIDER
	ch->RiderNo = p->Horse[1];		// LTS HORSERIDER
	ch->HorseGrade = p->Horse[2];		// LTS HORSERIDER
	ch->RiderGrade = p->Horse[3];		// LTS HORSERIDER
	g_HorseRider.LoadHorseData(ch);		// LTS HORSERIDER

	SettingBasicActionSub(ch);

	if (p->ChairNum)
	{
		if (Hero == ch)
		{   //< CSD-020311
			SendModifyPosition(ch);
		}   //> CSD-020311

		ch->nCurrentAction = 17;
		ch->nCurrentFrame.PutEncrypted(0);
		ch->ChairNum = p->ChairNum;
		if (ch == Hero)								// LTS SITDOWN  // 011214 LTS
		{
			AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 342));
			SetBattlePeace(PEACE_MODE);
		}
	}
	else
	{
		ch->nCurrentAction = 0;
		ch->nCurrentFrame.PutEncrypted(0);
	}

	connections.receive = 1;
}

void SendChangeEquip(LPCHARACTER ch)
{
	t_packet p;
	BYTE* equip = p.u.client_change_equip.equip;
	int* acc = ch->accessory;

	p.h.header.type = CMD_CHANGE_EQUIP;
	{
		equip[0] = acc[0];
		equip[1] = acc[1];
		equip[2] = acc[2];
		equip[3] = acc[3];
	}
	p.h.header.size = sizeof(t_client_change_equip);
	QueuePacket(&p, 1);

	// Animation minrange, maxrange, animation... ###0929

	connections.receive = 1;
}


extern void SetDragonLordWarArmor(t_server_user_add* c);				// LTS DRAGONLORD


//-----------------------------------------------------------------------------
void RecvUserAdd(t_server_user_add* c)
{
	GPCHARINFO	 gpCharInfo = { 0, };		// 1014 YGI

	LPCHARACTER ch = FindCharacter(&g_CharacterList, c->server_id);
	if (ch != NULL)
	{ 	//JustMsg( "�ִµ� �ǿԾ�..." ); 
		DeleteCharacter(&g_CharacterList, ch);
	}

	if (c->spr_no > 1)		gpCharInfo.type = SPRITETYPE_MONSTER;
	else					gpCharInfo.type = SPRITETYPE_CHARACTER;

	gpCharInfo.id = c->server_id;

	gpCharInfo.sprno = c->spr_no;
	gpCharInfo.mutant = 0;

	gpCharInfo.position.x = c->x * TILE_SIZE + 16;
	gpCharInfo.position.y = c->y * TILE_SIZE + 16;

	gpCharInfo.direction = (DIRECTION)(rand() % 8);//	
	gpCharInfo.nCurrentAction = ACTION_NONE;
	gpCharInfo.basicAction = ACTION_NONE;
	gpCharInfo.basicWalk = ACTION_BASIC_WALK;
	gpCharInfo.animationType = ANIMATION_TYPE_WOMAN;

	switch (c->bAlive)
	{
	case 1: gpCharInfo.hp = 1000;	break;  // ALIVE_
	case 2: gpCharInfo.hp = 0;	break;  // DEAD_
	case 3: gpCharInfo.hp = 0;	break;

	default:

		Error("Garbage Data.....Error Code : -5 ");
		break;
	}

	//	gpCharInfo.hp			= c->hp;
	gpCharInfo.mp = 100;
	gpCharInfo.sp = 100;
	gpCharInfo.hpmax = 100;
	gpCharInfo.mpmax = 100;
	gpCharInfo.spmax = 100;

	gpCharInfo.spell = c->spell;
	gpCharInfo.face = c->face;
	gpCharInfo.sex = c->spr_no;
	gpCharInfo.idWeaponItem = c->idWeaponItem; // CSD-031007

	//Char's options by Eleval
	gpCharInfo.hat_is_hidden = false;// c->hat_is_hidden;
	//End of it
	if (g_FightMapStart)
	{	//< CSD-TW-030618
		if (Hero != NULL)
		{
			CArenaTeam* pTeam = g_pArenaManager->GetMemberTeam(c->server_id);

			if (pTeam != NULL)
			{
				const int nTeam = pTeam->GetTeamNumber();
				c->equip0 = fight_map_acc_num[nTeam];
				c->equip1 = 1;
			}
		}
	}	//> CSD-TW-030618

	if (c->counselor)
	{
		c->equip0 = 114;
		c->equip1 = 1;
	}
	if (c->reporter) //Eleval 17/08/09 - To draw reporters' cloths
	{
		c->equip0 = 121;
		c->equip1 = 1;
	}
	if (g_bWarBegin) SetWarArmor(c);		// 010915 LTS
	if (g_LocalWarBegin) SetLocalWarArmor(c);	// LTS LOCALWAR
	if (c->DragonLordWarTeam > 0) SetDragonLordWarArmor(c);		// LTS DRAGONLORD

	gpCharInfo.accessory[0] = c->equip0;
	if (c->equip0 != 0)
	{
		//< soto-030313
#ifndef LIGHT_VERSION_
		if (g_CGuardianGuild.Is1StGuild(c->guild_code) && !g_FightMapStart && !c->JoinLocalWar && !(c->DragonLordWarTeam > 0) && !isNationWarfieldServer())
		{
			gpCharInfo.accessory[0] = GUILD_WEAR01;
			LoadHeroClothAccessoryDataOne(c->spr_no, GUILD_WEAR01);
			LoadHeroClothAccessoryDataOne(c->spr_no, GUILD_WEAR01 + 1); // ��

		}
		else
#endif
		{
			LoadHeroClothAccessoryDataOne(c->spr_no, c->equip0);
			LoadHeroClothAccessoryDataOne(c->spr_no, c->equip0 + 1);
		}

	}

#ifndef LIGHT_VERSION_
	if (g_CGuardianGuild.Is1StGuild(c->guild_code) && !g_FightMapStart && !c->JoinLocalWar && !(c->DragonLordWarTeam > 0) && !isNationWarfieldServer())
	{
		gpCharInfo.accessory[1] = GUILD_WEAR01;
		LoadHeroClothAccessoryDataOne(c->spr_no, GUILD_WEAR01);
	}
	else
#endif
	{
		gpCharInfo.accessory[1] = c->equip1;
		LoadHeroClothAccessoryDataOne(c->spr_no, c->equip1);
	}
	//> soto-030313

	gpCharInfo.accessory[2] = c->equip2;
	LoadHeroClothAccessoryDataOne(c->spr_no, c->equip2);
	gpCharInfo.accessory[3] = c->equip3;
	LoadHeroClothAccessoryDataOne(c->spr_no, c->equip3);
	// 011018 KHS ����
	gpCharInfo.mantle = c->mantle;

	int shape = GetItemAttr2(c->spr_no, c->mantle, ITEMSHOW);
	LoadHeroClothAccessoryDataOne(c->spr_no, shape);
	LoadHeroClothAccessoryDataOne(Hero->sprno, 138);
	LoadHeroClothAccessoryDataOne(Hero->sprno, 146);
	LoadHeroClothAccessoryDataOne(Hero->sprno, 147);
	// finito 08/08/07 load extra images for mantle of dragon and mantle of raja
	/*if (shape == 137)
	{
		LoadHeroClothAccessoryDataOne(c->spr_no, 138);
	}
	else if (shape == 145)
	{
		LoadHeroClothAccessoryDataOne(c->spr_no, 146);
		LoadHeroClothAccessoryDataOne(c->spr_no, 147);
	}*/
	// end of load extra images

	// 011030 KHS JJING
	gpCharInfo.jjing = c->jjing;
	if (c->jjing >= 5)	LoadHeroClothAccessoryDataOne(c->spr_no, c->equip0 + 200);
	if (c->jjing >= 9)	LoadHeroClothAccessoryDataOne(c->spr_no, c->equip0 + 201);


	gpCharInfo.bodyr = (BYTE)c->body_r << 3;
	gpCharInfo.bodyg = (BYTE)c->body_g << 3;
	gpCharInfo.bodyb = (BYTE)c->body_b << 3;
	gpCharInfo.clothr = (BYTE)c->cloth_r << 3;
	gpCharInfo.clothg = (BYTE)c->cloth_g << 3;
	gpCharInfo.clothb = (BYTE)c->cloth_b << 3;

	memcpy(gpCharInfo.name, c->name, c->namelen);

	gpCharInfo.peacests = c->peacests;
	gpCharInfo.viewtype = c->viewtype;

	// 1004 YGI ----------------------------------------
	gpCharInfo.name_status.nation = c->nation;
	gpCharInfo.name_status.guild_master = c->guild_master;
	gpCharInfo.name_status.king = c->king;
	gpCharInfo.guild_code = c->guild_code; // CSD-030324
	gpCharInfo.name_status.counselor = c->counselor;
	gpCharInfo.name_status.reporter = c->reporter;

	gpCharInfo.nk3 = c->nk3;
	gpCharInfo.nk4 = c->nk4;
	gpCharInfo.nk6 = c->nk6;
	gpCharInfo.lv = c->lv;				// 0807 YGI
	gpCharInfo.class_type = c->class_type;		// 0807 YGI

	//_____________________________________________________	// 010904 LTS	// 010915 LTS
	gpCharInfo.NWCharacter.isCommander = c->isCommander;		//������ ���� ��Ŷ 
	gpCharInfo.NWCharacter.isSquadLeader = c->isSquadLeader;		// LTS CHAR
	gpCharInfo.NWCharacter.SquadNo = c->SquadNo;
	gpCharInfo.NWCharacter.YL_JoinNation = c->YL_JoinNation;
	//_____________________________________________________	// 010904 LTS*/
	gpCharInfo.JoinLocalWar = c->JoinLocalWar;				// LTS LOCALWAR
	gpCharInfo.ChairNum = c->ChairNum;					// LTS SITDOWN BUG

	gpCharInfo.HorseNo = c->HorseNo;						// LTS HORSERIDER
	gpCharInfo.RiderNo = c->RiderNo;						// LTS HORSERIDER
	gpCharInfo.HorseGrade = c->HorseGrade;					// LTS HORSERIDER
	gpCharInfo.RiderGrade = c->RiderGrade;					// LTS HORSERIDER
	gpCharInfo.DragonLordWarTeam = c->DragonLordWarTeam;			// LTS DRAGONLORD

	if (c->ChairNum) gpCharInfo.nCurrentAction = 17;

	gpCharInfo.nDualCls = c->nDualCls;
	gpCharInfo.nClsStep = c->nClsStep;
	gpCharInfo.nAttr = 0;
	gpCharInfo.nRace = HUMAN; // CSD-030422

	gpCharInfo.mantleDyeColour = c->MantleDyeColour; // Finito mantle dye

	for (int i = 0; i < 8; i++) {
		gpCharInfo.equip[i] = 8000+((LPItemReforgeData)&c->equip[i].attr[IATTR_REFORGE])->Attr1ReforgeCount;
	}
	//gpCharInfo.equip[WT_ARMOR] = c->equip[WT_ARMOR].item_no;
	gpCharInfo.Isdungeon = c->Isdungeon;
	if (!AddCharacter(&g_CharacterList, g_CharacterList.lpLast, &gpCharInfo))//?? soto
	{
		JustMsg("Other User LogIn Error...");
	}

	connections.receive = 1;
}


//-----------------------------------------------------------------------------
void RecvUserRemove(int server_id)
{
	LPCHARACTER	ch;

	connections.receive = 1;

	ch = ReturnCharacterPoint(server_id);
	if (ch == NULL) { return; } //JustMsg( "RecvUserRemove" ); return; }		// 0521 YGI

	if (ch == Hero) {
		ListenCommand = CMD_YOU_REMOVE;

		//001209 zhh
		Release(&connections);

		return;
	}

	DeleteCharacter(&g_CharacterList, ch);
	//printf( "User Remove : %d\n", server_id );
}

int	SendUserRemove(void)
{
	t_packet	packet;

	packet.h.header.type = CMD_USER_REMOVE;
	packet.h.header.size = 0;

	QueuePacket(&packet, 1);

	return(1);
}

void RecvNpcAdd(t_server_npc_add* p)
{	//< CSD-030419
	LPCHARACTER ch = ReturnCharacterPoint(p->idNpc);

	if (ch != NULL)
	{
		DeleteCharacter(&g_CharacterList, ch);
	}

	GPCHARINFO gpCharInfo;
	memset(&gpCharInfo, 0, sizeof(GPCHARINFO));
	strcpy(gpCharInfo.name, LoadMonNameTable(p->nNameNo));
	gpCharInfo.type = SPRITETYPE_MONSTER;
	gpCharInfo.id = p->idNpc;
	gpCharInfo.sprno = p->nSprNo;
	gpCharInfo.mutant = p->nMutant;
	gpCharInfo.position.x = p->nX;
	gpCharInfo.position.y = p->nY;
	gpCharInfo.nAttr = p->nAttr;
	gpCharInfo.viewtype = VIEWTYPE_NORMAL_;
	gpCharInfo.viewtype = p->nViewType;
	gpCharInfo.ChairNum = p->nAIType;	// LTS AI
	gpCharInfo.homex = p->nHomeX;
	gpCharInfo.homey = p->nHomeY;
	gpCharInfo.eventnpc = (p->nEventNpc>0) ? true:false;
	gpCharInfo.bRecallNpc = (p->nRecall == 0) ? false : true;
	gpCharInfo.nRace = p->nRace;

	switch (gpCharInfo.sprno)
	{
	case 101:
	case 100:
	{
		gpCharInfo.direction = DIRECTION_RIGHTDOWN;
		break;
	}
	case 24: // CSD-040202
	case 50:
	case 51:
	case 52: // LTS DRAGON BUG
	case 63: //��ȣ��1
	case 64: //��ȣ��2
	case 65: //����1
	case 66: //����2
	case 67: //����3
	case 91:
	case 98:
	case 99:
	case 118:
	case 119:
	case 177: // CSD-040316
	case 178: // CSD-040316

	{
		gpCharInfo.direction = DIRECTION_DOWN;
		break;
	}
	default:
	{
		gpCharInfo.direction = DIRECTION(rand() % 8);
		break;
	}
	}

	gpCharInfo.nCurrentAction = MON1_NONE;
	gpCharInfo.basicAction = MON1_NONE;
	gpCharInfo.basicWalk = MON1_BATTLE_WALK;
	gpCharInfo.animationType = ANIMATION_TYPE_MON1;

	gpCharInfo.hp = (p->nStatus >= 2) ? 0 : 300;
	gpCharInfo.mp = 300;
	gpCharInfo.sp = 300;
	gpCharInfo.hpmax = 300;
	gpCharInfo.mpmax = 300;
	gpCharInfo.spmax = 300;
	gpCharInfo.sex = 0;

	gpCharInfo.accessory[0] = 0;
	gpCharInfo.accessory[1] = 0;
	gpCharInfo.accessory[2] = 0;
	gpCharInfo.accessory[3] = 0;

	gpCharInfo.nk3 = 0;
	gpCharInfo.nk4 = 0;
	gpCharInfo.nk6 = 0;

	AddCharacter(&g_CharacterList, g_CharacterList.lpLast, &gpCharInfo);

	++NPCCOUNT;

	connections.receive = 1;
}	//> CSD-030419	

void  RecvNpcRemove(int npc_id)
{
	LPCHARACTER ch = ReturnCharacterPoint(npc_id);
	if (ch == NULL) return;

	////////////////////// �̱��� 0101 /////////////////////////////////////
	if (ch->dieani > 32 && ch->nCurrentAction == MON1_DIE)
	{
		ch->diedelay = 3;
		ch->dieani = 32;
	}
	else
	{
		DeleteCharacter(&g_CharacterList, ch);
	}

	NPCCOUNT--;
	connections.receive = 1;
}

void CalcTacticsSkillExp(int tac_skill, DWORD exp, char step)
{
	SCharacterData.TacticsExp[tac_skill] = exp;
	int a = tac_skill;
	Hero->aStepInfo[a + TAC_INDEX] = step;
	SCharacterData.SkillId[a + 88] = GetLeveByExp(Hero->aStepInfo[a + TAC_INDEX],
		SCharacterData.TacticsExp[a]);
	SCharacterData.TacticsExpNext[a] = NPC_Lev_Ref[SCharacterData.SkillId[a + 88]].nMaxExp;
}

//############################## bank ####################################
//void RecvBankDeposit(int money, int char_money);
//void RecvBankRepayment(int, int, int, DWORD c);


void RecvBankDeposit(DWORD money)
{
	SCharacterData.BankMoney = money;
	connections.receive = 1;
}
void RecvBankRepayment(DWORD total, DWORD money, DWORD time)
{
	SCharacterData.LoanTotal = total;
	SCharacterData.LastLoan = money;
	SCharacterData.LastLoan_Time = time;
	connections.receive = 1;
}
//#########################################################################






int SendCreateCharacter(SCharacter* s)
{
	t_packet	packet;

	// HN_MAKECHARACTER_ARIGENENT_RECT     
	// HN_MAKECHARACTER_CLASS_RECT         

	packet.h.header.type = CMD_CREATE_CHAR;
	{
		strcpy(packet.u.client_create_char.name, s->sCharacterName);
		packet.u.client_create_char.gender = s->nCharacterData[GENDER];
		packet.u.client_create_char.face = s->nCharacterData[FACE];
		packet.u.client_create_char.age = s->age;
		packet.u.client_create_char.arigement = 0; // CSD-030324
		packet.u.client_create_char.cloth_r = s->cloth_r;
		packet.u.client_create_char.cloth_g = s->cloth_g;
		packet.u.client_create_char.cloth_b = s->cloth_b;
		packet.u.client_create_char.body_r = s->body_r;
		packet.u.client_create_char.body_g = s->body_g;
		packet.u.client_create_char.body_b = s->body_b;
		switch (s->nCharacterData[GENDER])
		{
		case 0: packet.u.client_create_char.tactics = s->nCharacterData[TACTICS_WOMAN];	break;
		case 1: packet.u.client_create_char.tactics = s->nCharacterData[TACTICS_MAN];		break;
		}

		packet.u.client_create_char.Class = s->nCharacterData[CLASS];
		packet.u.client_create_char.job = s->nCharacterData[JOB];
		packet.u.client_create_char.spell = s->nCharacterData[SPELL];

		//		packet.u.client_create_char.str			= s->nCharacterAbility[STR];
		//		packet.u.client_create_char.con			= s->nCharacterAbility[CON];
		//		packet.u.client_create_char.dex			= s->nCharacterAbility[DEX];
		//		packet.u.client_create_char.wis			= s->nCharacterAbility[WIS];
		//		packet.u.client_create_char.intl		= s->nCharacterAbility[INT];
		//		packet.u.client_create_char.movp		= s->nCharacterAbility[MOVP];
		//		packet.u.client_create_char.cha			= s->nCharacterAbility[CHA];
		//		packet.u.client_create_char.endu		= s->nCharacterAbility[ENDU];
		//		packet.u.client_create_char.mor			= s->nCharacterAbility[MOR];
		//		packet.u.client_create_char.luck		= s->nCharacterAbility[LUCK];
		//		packet.u.client_create_char.wsps		= s->nCharacterAbility[WSPS];
	}
	packet.h.header.size = sizeof(t_client_create_char);
	QueuePacket(&packet, 1);

	double		StartTime, Duration;
	StartTime = SDL_GetTicks();
	ListenCommand = CMD_NONE;
	while (1)
	{
		Duration = SDL_GetTicks() - StartTime;
		if (Duration > WAIT_TIME_RESPONSE_DURATION)	// 030930 kyo	
			return(-1);

		if (HandleRunning(&connections) <= 0)
		{
			Release(&connections);
			return(-1);
		}

		if (ListenCommand == CMD_ACCEPT_CREATE)
		{
			return(TRUE);
		}

		if (ListenCommand == CMD_INVALID_DB)
		{
			return(-1);
		}
	}

	return TRUE;
}


int		SendDeleteCharacter(const char* szName, const char* szSecretKeyCode)
{
	t_packet	packet;
	packet.h.header.type = CMD_DELETE_CHAR;
	packet.h.header.size = sizeof(t_client_delete_char);
	{
		strcpy(packet.u.client_delete_char.name, szName);
		strcpy(packet.u.client_delete_char.szSecretKeyCode, szSecretKeyCode);//020730 lsw
	}
	QueuePacket(&packet, 1);

	double		StartTime, Duration;
	StartTime = SDL_GetTicks();
	ListenCommand = CMD_NONE;
	while (1)
	{
		Duration = SDL_GetTicks() - StartTime;
		if (Duration > WAIT_TIME_RESPONSE_DURATION)	 // 030930 kyo	
		{
			::CallOkCancelMessageBox(MN_DEL_CHAR_COMFORM, 0, 0, lan->OutputMessage(5, 130), 0);
			return(-1);
		}

		if (HandleRunning(&connections) <= 0)
		{
			Release(&connections);
			return(-1);
		}

		if (ListenCommand == CMD_ACCEPT_DELETE)
		{
			return(TRUE);
		}
	}

	return TRUE;
}



int SendChoiceCharacter(int ch)
{
#ifdef _DEBUG // finito 060507
	if (SysInfo.notconectserver) { return 0; }
#endif	

	SDL_Log("SendChoiceCharacter: %d", ch);

	t_packet	packet;
	packet.h.header.type = CMD_CONNECT_INFO;
	packet.h.header.size = sizeof(client_connect_info);

	memcpy(packet.u.client_connect_info.name, LoadCh[ch].sCharacterName, NM_LENGTH);
	memcpy(HeroName, LoadCh[ch].sCharacterName, NM_LENGTH);

	packet.u.client_connect_info.startmap = StartMap;
	packet.u.client_connect_info.startposition = StartPosition;


	QueuePacket(&packet, 1);

	return TRUE;

}

// User�� ���� Character������ �޴´�.
// LogIn�ڿ� �޴� Data....
void Recv4Character(t_server_accept_login* p)
{
	HeroServerID = p->server_id;

	for (int l = 0; l < MAX_CHARACTEROFID; l++)
	{
		EatRearWhiteChar(p->name[l]);
		memcpy(LoadCh[l].sCharacterName, p->name[l], 16);// 0131 YGI
		LoadCh[l].nLevel = p->level[l];
		LoadCh[l].nCharacterData[HN_MAKECHARACTER_JOB_RECT] = p->job[l];
		LoadCh[l].nCharacterData[HN_MAKECHARACTER_CLASS_RECT] = p->cla[l];
		//			
		LoadCh[l].nMoney = p->money[l];
		LoadCh[l].age = 18;//p->age[l];		// 20���� ����
		LoadCh[l].cloth_r = p->clothr[l];
		LoadCh[l].cloth_g = p->clothg[l];
		LoadCh[l].cloth_b = p->clothb[l];
		LoadCh[l].body_r = p->bodyr[l];
		LoadCh[l].body_g = p->bodyg[l];
		LoadCh[l].body_b = p->bodyb[l];
		LoadCh[l].accessory[0] = p->acc_equip1[l];
		LoadCh[l].accessory[1] = p->acc_equip2[l];
		LoadCh[l].accessory[2] = p->acc_equip3[l];
		LoadCh[l].accessory[3] = p->acc_equip4[l];
		LoadCh[l].nCharacterData[GENDER] = p->gender[l];

		LoadCh[l].nation = p->nation;			// 0927 YGI
		g_nBillingType = p->remained_day;	// 0927 YGI
	}
	connections.receive = 1;
}


void RecvHeroInit(t_server_user_db_data* p)
{
	SCharacter& ch = SCharacterData;

	GPCHARINFO	 gpCharInfo = { 0, };

	gpCharInfo.type = SPRITETYPE_CHARACTER;
	gpCharInfo.id = HeroServerID;

	gpCharInfo.sprno = p->sprno;
	gpCharInfo.sex = p->sprno;

	gpCharInfo.position.x = p->x;
	gpCharInfo.position.y = p->y;
	gpCharInfo.direction = DIRECTION_DOWN;

	gpCharInfo.nCurrentAction = ACTION_NONE;
	gpCharInfo.basicAction = ACTION_BATTLE_NONE;
	gpCharInfo.basicWalk = ACTION_BATTLE_WALK_OWNHAND;
	gpCharInfo.animationType = ANIMATION_TYPE_WOMAN;

	ch.nCharacterHP = gpCharInfo.hp = p->nLife;
	ch.nCharacterMAXHP = gpCharInfo.hpmax = p->nMaxHp;
	ch.nCharacterMP = gpCharInfo.mp = p->nMana;
	ch.nCharacterMAXMP = gpCharInfo.mpmax = p->nMaxMp;
	ch.nCharacterSP = gpCharInfo.sp = p->nHungry;
	ch.nCharacterMAXSP = gpCharInfo.spmax = p->nMaxHungry;

	gpCharInfo.face = p->Attr[1];


	// �ɷ�ġ �߰� ######################################################	// 1213_2
	for (int a = 0; a < 11; a++)
		ch.nCharacterAbility[a] = p->Avility[a];

	ch.nMoney = p->money;
	ch.nLevel = p->lev;
	ch.nExp = p->exp;
	ch.nCharacterData[GENDER] = p->Attr[0];
	ch.nCharacterData[FACE] = p->Attr[1];
	// ch.nCharacterData[ARIGEMENT]=	p->Attr[2]; // CSD-030324
	ch.nCharacterData[CLASS] = p->Attr[3];
	gpCharInfo.spell = ch.nCharacterData[SPELL] = p->Attr[4];
	ch.nCharacterData[TACTICS_WOMAN] = p->Attr[5];
	ch.nCharacterData[TACTICS_MAN] = p->Attr[5];

	ch.condition = p->condition;

	memcpy(&gpCharInfo.name_status, &p->nation, sizeof(DWORD)); // CSD-031001

	if (gpCharInfo.name_status.counselor)
	{
		p->acc_equip1 = 114;
		p->acc_equip2 = 1;			// ����� ����.
	}

	if (gpCharInfo.name_status.reporter) //Eleval 17/08/09 - To draw Reporters' cloths
	{
		p->acc_equip1 = 121;
		p->acc_equip2 = 1;
	}

	gpCharInfo.accessory[0] = p->acc_equip1;
	if (p->acc_equip1 != 0) {
		LoadHeroClothAccessoryDataOne(p->sprno, p->acc_equip1);
		LoadHeroClothAccessoryDataOne(p->sprno, p->acc_equip1 + 1);
	}
	gpCharInfo.accessory[1] = p->acc_equip2;
	LoadHeroClothAccessoryDataOne(p->sprno, p->acc_equip2);
	gpCharInfo.accessory[2] = p->acc_equip3;
	LoadHeroClothAccessoryDataOne(p->sprno, p->acc_equip3);
	gpCharInfo.accessory[3] = p->acc_equip4;
	LoadHeroClothAccessoryDataOne(p->sprno, p->acc_equip4);

	// 011030 KHS JJING	��� ���� �κ��� ���⼭ ���� ���� �ʰ� Equip[8]�� �����ִ� ������ �����Ѵ�. 
	//	if( p->jjing >= 5 )	LoadHeroClothAccessoryDataOne( p->sprno, p->acc_equip1+200);
	//	if( p->jjing >= 9 )	LoadHeroClothAccessoryDataOne( p->sprno, p->acc_equip1+201);


		// 011018 KHS ����	; ���⿡�� ���� Data�� ���� �ʴ´�. CMD_CHR_ITEM_INFO_1�� �ö� equip[WT_NECK]������ �ö� ó���Ѵ�.
	ch.cloth_r = gpCharInfo.clothr = p->clothr;
	ch.cloth_g = gpCharInfo.clothg = p->clothg;
	ch.cloth_b = gpCharInfo.clothb = p->clothb;

	ch.body_r = gpCharInfo.bodyr = p->bodyr;
	ch.body_g = gpCharInfo.bodyg = p->bodyg;
	ch.body_b = gpCharInfo.bodyb = p->bodyb;

	memcpy(gpCharInfo.name, p->name, 20);

	gpCharInfo.peacests = 0; // ���ʿ��� ������ ��ȭ���·� �Ǿ� �ִ�.  0410
	ch.potion_box1 = p->sight;		// ���⼭ sight�� �ٸ����ٰ� �̿��Ѵ�. //020425
	gpCharInfo.Age = 18;//p->Age;		// 20���� ����

	CurOpenHouse = p->openhouse;

	ch.LvUpPoint = p->reserved_point;

	gpCharInfo.viewtype = p->viewtype;

	char* tt;
	tt = EatRearWhiteChar(p->mapname);

	memcpy(MapName, tt, 20);
	//SDL_Log("RecvHeroInit %s", MapName);


	gpCharInfo.social_status = p->social_status;

	ch.nation = gpCharInfo.name_status.nation;		// 1004 YGI
	gpCharInfo.fame = p->fame;

	gpCharInfo.nk3 = p->nk3;
	gpCharInfo.nk4 = p->nk4;
	gpCharInfo.nk6 = p->nk6;

	gpCharInfo.NWCharacter = p->NWCharacter;	// 010915 LTS


	gpCharInfo.lv = ch.nLevel;					// 0807 YGI
	gpCharInfo.class_type = ch.nCharacterData[CLASS];		// 0807 YGI
	gpCharInfo.nRace = HUMAN; // CSD-030422
	AddCharacter(&g_CharacterList, NULL, &gpCharInfo);
	Hero->sight = NORMAL_SIGHT;
	Hero->visual_x = static_cast<float>(Hero->x); // 讓視覺位置在出生時等於邏輯位置
	Hero->visual_y = static_cast<float>(Hero->y);
	Hero->previousAction = ACTION_NONE;
	memcpy(Hero->name, HeroName, NM_LENGTH);
	strncpy(ch.sCharacterName, HeroName, NM_LENGTH); // CSD-031210
	ch.sCharacterName[NM_LENGTH - 1] = 0;
	memcpy(Hero->aStepInfo, p->aStepInfo, sizeof(Hero->aStepInfo));
	Hero->SetGuildCode(p->Attr[2]); // CSD-031001 // thai2 YGI	

	//if(Hero->IsCounselor()) Hero->name_status.santa_power = 2; //Eleval 22/06/09 - Santa Power
	g_GameInfo.m_BuffTimer.clear();
	if (g_StartMenuOn)
	{	//1218 YGI
		ReqItemInfo0();
	}

	connections.receive = 1;
}

void RecvEnvironmentInfo(t_server_envir_info* p)
{
	WeatherDataProcess(p->weather_count, p->currtime_count,
		p->rainstart, p->rainend, p->amount, p->temperature);

	connections.receive = 1;
}

void RecvAssignServerID(t_server_assign_server_id* p)
{
	HeroServerID = Hero->id = p->server_id;
}

void SendLogOut(void)
{
	t_packet packet;
	packet.h.header.type = CMD_LOST_CONNECTION;
	packet.h.header.size = 0;
	QueuePacket(&packet, 1);

	HandleRunning(&connections);
}

void SendEventPacket(int type, int id) {
	t_packet packet;
	packet.h.header.type = CMD_EVENT;
	packet.h.header.size = sizeof(t_client_event);
	packet.u.client_event.type = type;
	packet.u.client_event.event_no = id;
	QueuePacket(&packet, 1);
}

void SendEvent(int type, int id)
{	//< CSD-030930
	if (EVENT_TYPE_NPC == type)
	{
		LPCHARACTER pNpc = ReturnCharacterPoint(id);
		if (pNpc == NULL)
		{
			return;
		}

		if (pNpc->sealstone)
		{
			return;
		}

		switch (pNpc->nRace)
		{
		case HUMAN:
		case GUARD:
		{
			Hero->eventnpcid.PutEncrypted(pNpc->id);

			SendEventPacket(type, id);
			break;
		}
		}

		return;
	}

	if (g_pArenaManager->IsReadyGame())
	{
		return;
	}

	SendEventPacket(type, id);
}	//> CSD-030930

void RequestData(int server_id, int protocol)
{
	t_packet p;

	return;////

	switch (protocol)
	{
	case REQ_NPC_ADD:	p.h.header.type = protocol;
		p.u.req_npc_add.server_id = server_id;
		p.h.header.size = 0;
		QueuePacket(&p, 1);
		break;
	case REQ_ITEM_ADD:	p.h.header.type = protocol;
		p.u.req_item_add.server_id = server_id;
		p.h.header.size = 0;
		QueuePacket(&p, 1);
		break;
	case REQ_PC_ADD:	p.h.header.type = protocol;
		p.u.req_pc_add.server_id = server_id;
		p.h.header.size = 0;
		QueuePacket(&p, 1);
		break;
	}
}

int DoAccessJoin(void)//bIsMapMove �� true�� ���̵��� �õ� �ϴ� �� false �� ���ʿ� �������� �õ��ϴ°�
{
#ifdef _DEBUG // finito 060507
	if (SysInfo.notconectserver) { return 0; }
#endif
	t_packet packet;
	DWORD StartTime = SDL_GetTicks();

	packet.h.header.type = CMD_ACCESS_JOIN;

	ACCESSJOIN* pAJ = &packet.u.AccessJoin;
	if (IIDD.length() >= ID_LENGTH || PPWW.length() >= ID_LENGTH)
	{
		SDL_Log("ERROR: %s %s length to long", IIDD.c_str(), PPWW.c_str()); return 0;
	}
	memset(pAJ, 0, sizeof(ACCESSJOIN));

	memcpy(pAJ->id, IIDD.data(), IIDD.length());
	memcpy(pAJ->pw, PPWW.data(), PPWW.length());
	memcpy(pAJ->name, HeroName, NM_LENGTH);
	pAJ->startposition = StartPosition;
	pAJ->nBillingType = ((g_nBillingType == BT_FREE) ? BT_FREE : 1000);

	packet.h.header.size = sizeof(ACCESSJOIN);
	QueuePacket(&packet, 1);

	ListenCommand = CMD_NONE;
	while (1)
	{
		DWORD Duration = SDL_GetTicks();

		if (Duration - StartTime > WAIT_TIME_RESPONSE_DURATION) // 030930 kyo
		{
			return(-1);
		}

		if (::HandleRunning(&connections) <= 0)
		{
			Release(&connections);
			return(-1);
		}
		else
		{
			switch (ListenCommand)
			{
			case  CMD_ACCEPT_JOIN:
				return 1;
			case  CMD_INVALID_ID:
				return -3;
			case  CMD_INVALID_PW:
				return -4;
			case  CMD_BANNED_PLAYER:
				return -18;
			case  CMD_INVALID_DB:
				return -5;
			case  CMD_INVALID_VERSION:
				return -6;
			case  CMD_USED_ID:
				return -7;
			case CMD_INVALID_AGE: // 030929 kyo
				return -17;

			}


		}
	}
	return 1;
}

//021030 YGI				
int RecvConnectInfo(t_server_connect_info* p)
{
	k_server_connect_info_kein* pData = (k_server_connect_info_kein*)p;
	StartPosition = p->startposition;
	if (SysInfo.notconectserver == 0 && SysInfo.entergameserver)
	{
		g_GameInfo.map_port = SysInfo.port;
	}
	else
	{
		g_GameInfo.map_port = p->port;
	}

	RecvEventObject(&pData->data2);

	int ret = ::DoAccessJoin();
	if (ret <= 0)
	{
		// JustMsg( "Game Server Connecting Failed \n Now Exit.( Error Code: %d )", ret );//lsw
		// ExitApplication(EA_RECV_CONNECT_INFO);
		CallLoginMenu();
		// SMenu[MN_SELECT_CHARACTER].bActive=true;
		SMenu[MN_LODING].bActive = false;
		CallOkCancelMessageBox(MN_LODING, 0, 0, lan->OutputMessage(6, 58), 0);//lsw
		return 0;
	}
	return 1;
}

int RecvChangeMap(t_server_change_map* p)		// 0918	// 0923 YGI
{
	SDL_Event e;
	int c = 0;

	ChangeMapFlag = 0;
	NowLoading = 1;
	StartPosition = 0;	//  ó���� ���� �߻��ϴ� ����.... 99�̸� LogOut�� ��ġ�� ��Ÿ����.
	CrackMgr.InitCrackMgr();//020505 lsw
	InitHeroCanView();	//020510 lsw
	//pMusic->Stop();
	g_GameInfo.map_port = p->port;
	HeroServerID = Hero->id = p->server_id;

	SetChangeMapOfMenu();

	while (1)
	{
		while (SDL_PollEvent(&e))
		{
			;
		}
		SettingChangMapMenu(c);
		g_SDLInfo.SDL_FlipScreen();

		switch (c)
		{
		case 0: 
		{ g_Volume_Off = 1; c = 20; 
			g_nLButtonState = 0; g_nRButtonState = 0;
			g_nLDButtonState = 0; g_nRDButtonState = 0;
			ResetEffect(Hero);

			g_mgrBattle.ClearDelay();
			g_mgrBattle.ClearSwitchDelay();
			g_mgrBattle.SendCombatClear();
		}break;
		case 20:
		{
			DeleteAllSkillData();
			LoadingLoadMaskTable(); //change map bug with sdl
			ResetCharacter(Hero);
			g_pNumberAniMgr.InitHpUpdateQueue();
			c = 40;
		}break;
		case 40:
		{
			strcpy(MapName, p->mapname);
			Hero->position.x = p->x;
			Hero->position.y = p->y;
			Hero->x = Hero->gox = p->x * TILE_SIZE + 16;
			Hero->y = Hero->goy = p->y * TILE_SIZE + 16;
			Hero->pathcount = 0;
			Hero->moveFlag = 0;
			c += 10;
		}break;
		case 50: { FreeTOI();													 c += 10; }break;
		case 60: { LoadTOI(MapName);			StopBGE();					 c += 10; }break;
		case 70: { /*MapBuild(&g_Map, MapName);*/MapBuild_Refactored(&g_Map, MapName);	c += 10; }break;
		case 80: { DestroyRoofHeader(&g_RoofHeader);						 c += 10; }break;
		case 90: { BuildRoofHeader(&g_RoofHeader, MapName);					 c += 10; }break;
		case 100: { DestroyEventList();										 c += 10; }break;
		case 110: { BuildEventList(MapName);									 c += 10; }break;
		case 120: { FreeMapSource();											c += 10; }break;
		case 130: { LoadMapSource2(MapName);									 c += 10; }break;
		case 140: { DestroyItemList();											 c += 10; }break;
		case 150: { 
			//FreeCharacterSpriteDataAll(2, MAX_CHARACTER_SPRITE_);	
			c += 10; 
		}break;
		case 160: { DestroyCharacterListExceptHero();							 c = 200; }break;
		case 200: 	//////////////// 0812 lkh ���� ////////////////
		{
			g_Volume_Off = 0;
			SetVolumeMusic( system_info.music );
			SetVolume( system_info.sound );
			BackgroundMusic();

			Hero->position.x = p->x;		//������ ���� ��ġ�� ����...
			Hero->position.y = p->y;
			Hero->x = Hero->gox = p->x * TILE_SIZE + 16;
			Hero->y = Hero->goy = p->y * TILE_SIZE + 16;
			StartSetView(Hero);
			Hero->pathcount = 0;
			Hero->moveFlag = 0;
			c += 10;
		}break;
		case 210:
		{
			SendAllReady();
			ProtocolProc(&connections);
			c += 10;
		}break;
		default:
		{
			goto SUCCESS_;
		}break;
		}

	}

SUCCESS_:
	CSDMainIconClear();
	SDL_Delay(1000);
	g_SmallMapNumber = MapNumber;
	g_GameInfo.m_BuffTimer.clear();
	if (IsReporterMode())
	{
		Hero->reporter_mode = 0;
		CallServer(CMD_PLEASE_GIVE_LIFE1);
	}

	NowLoading = 0;
	g_Menu_Variable.clear3();
	if (g_Menu_Variable.m_PartySelect)
	{
		SendSelectPartyExp(g_Menu_Variable.m_PartySelect);
	}

	// 020925 YGI
	if (GetBattlePeace() == PK_MODE)
	{
		char pk_data = 1;
		CallServer(CMD_PK_ON_OFF, &pk_data, 1);
	}
	CheckRoof();	// Ȥ�� ������ ���� ������ ����.	// 031110 YGI

	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////




// #define STATUS_PEACE_BATTLE	0
void SendPeaceBattle(LPCHARACTER ch)
{
	t_packet p;

	switch (ch->peacests)
	{
	case 0: 	p.h.header.type = CMD_INTERFACE_PEACE;	break;
	case 1: 	p.h.header.type = CMD_INTERFACE_BATTLE;	break;
	}

	p.h.header.size = 0;
	QueuePacket(&p, 1);
}


void RecvPeaceBattle(int server_id, int peacests)
{
	LPCHARACTER ch = FindCharacter(&g_CharacterList, server_id);
	if (ch == NULL) return;

	ch->peacests = peacests;
	SettingBasicActionSub(ch);
}

void RecvCharInfoMagic(t_char_info_magic* p)
{
	memcpy(SCharacterData.MagicId, p->magic, 200);	// #### 1102
	ReadMagicTable();	// ���� ���̺��� �����´�.	// 0711 YGI
	ResetMagicQuickArray();		// ���� �迭 �ʱ�ȭ		
	if (SCharacterData.nCharacterData[SPELL] == PRIEST_SPELL) MagicSetting2();			// ����~ ������Ʈ ���...
	if (g_StartMenuOn) ReqConnectInfo();			// ��ų�� �ʿ��� �޾ƿ´�.
}

void RecvCharInfoSkill(t_char_info_skill* p)
{
	memcpy(SCharacterData.SkillId, p->skill, 200);	// #### 1102
	//if( g_StartMenuOn ) ReqConnectInfo();		//021030 YGI // ReqConnectInfo()�� �ƿ� ���� �Ѵ�.
}

void RecvItemDurationChange(t_item_duration_change* p)
{
	ItemAttr* target_item = &EquipItemAttr[p->pos];
	WORD dur, max;
	char temp[MAX_PATH];

	if (p->pos >= 8)				return;
	if (target_item == NULL)		return;
	if (target_item->item_no == 0) return;

	CItem* ii = ItemUnit(target_item->item_no / 1000, target_item->item_no % 1000);
	if (ii == NULL) return;
	//char *s = ii->GetItemName();
	char* s = nullptr;
        if (system_info.lang==0) s = ii->GetItemName();  // 0721 YGI
        else
          s = ii->GetItemHanName();

	GetItemDuration(*target_item, dur, max);

	dur = p->dur;
	if (ii->GetRButton() == DIVIDE_ITEM)
	{
		if (dur < 10)
		{
			sprintf(temp, lan->OutputMessage(4, 1), s, dur);//lsw
			AddCurrentStatusMessage(0, 0, 255, temp);
		}
		target_item->attr[IATTR_DURATION] = MAKELONG(dur, max);
	}
	else
	{
		if (dur > max) dur = max; // make dura equal to max if it is bigger than the max dura
		if (dur == max) return;
		target_item->attr[IATTR_DURATION] = MAKELONG(dur, max);

		// If item is broken (dura/10 seems to represent the dura that the players see)
		if (dur / 10 <= 0)  // ���������縦 ���ش�. 
		{
			if (ii)
			{
				ii->ReleaseItem();
				if (s)
				{
					sprintf(temp, lan->OutputMessage(4, 3), s);//lsw
					AddCurrentStatusMessage(0, 0, 255, temp);

					ChangeEquip(Hero, target_item, p->pos); //  8��..
				}

				DeleteItem(target_item);
			}
		}
		// �������� 20%���ϸ� �˶�...
		else if ((max * 10) / 100 > dur)		// show message for 10% dura left or less
		{
			sprintf(temp, lan->OutputMessage(4, 2), s);//lsw
			AddCurrentStatusMessage(0, 0, 255, temp);
		}

	}

}

int QueuePacket(t_packet* packet, int type)
{
  /*t_packet* tempbuf;

	if (packet == NULL || packet->h.header.size > MAX_PACKET_SIZE
		|| 0 > packet->h.header.size)//0���� ������ ����
	{//021025 lsw
		return(0);
	}

	if (type == 0)	// inbuf
	{
		if (packet->h.header.type < 0)
		{
			_asm nop;
		}

		if (!connections.inbuf)
		{
			connections.inbuf = (t_packet*)malloc(sizeof(t_packet));
			memcpy(connections.inbuf, packet, sizeof(t_packet));
			connections.inbuf->next = NULL;
		}
		else
		{
			for (tempbuf = connections.inbuf; tempbuf->next; tempbuf = tempbuf->next)
			{
				;
			}

			tempbuf->next = (t_packet*)malloc(sizeof(t_packet));
			tempbuf = tempbuf->next;
			memcpy(tempbuf, packet, sizeof(t_packet));
			tempbuf->next = NULL;
		}
	}
	else if (type == 1)	// outbuf
	{
		//---------------------------------------------------------
		if (!connections.outbuf)
		{
			connections.outbuf = (t_packet*)malloc(sizeof(t_packet));
			memcpy(connections.outbuf, packet, sizeof(t_packet));
			connections.outbuf->next = NULL;
		}
		else
		{
			for (tempbuf = connections.outbuf; tempbuf->next; tempbuf = tempbuf->next)
			{
				;
			}
			tempbuf->next = (t_packet*)malloc(sizeof(t_packet));
			tempbuf = tempbuf->next;
			memcpy(tempbuf, packet, sizeof(t_packet));
			tempbuf->next = NULL;
		}
	}

	connections.packet_count[type]++;

	return(1);*/ 
	// 嚴格的錯誤檢查，確保數據合法性
  if (packet == nullptr || packet->h.header.size > MAX_PACKET_SIZE ||
      packet->h.header.size < 0) {
    return 0;
  }

  t_packet pp;
  memset(&pp, 0, sizeof(t_packet));
  int total_size = packet->h.header.size + sizeof(t_header);
  memcpy(&pp, packet, total_size);
  if (type == 0) {  // inbuf
    connections.inbuf.push_back(pp);
  } else if (type == 1) {  // outbuf
    connections.outbuf.push_back(pp);
  } else {
    return 0;  // 無效的類型
  }
  return 1;
}


void LogPacket(const bool bIsRecv, t_packet* p)
{
#ifdef _DEBUG	
	return;
	SDL_Log("%s T:(%5d) S:(%3d)", ((bIsRecv) ? "R" : "S"),
		p->h.header.type,
		p->h.header.size);
	if (GetSysInfo(SI_GAME_MAKE_MODE))
	{
		struct tm* today;
		time_t lTime;
		time(&lTime);
		today = localtime(&lTime);
		FILE* fp = Fopen("LogPacket.txt", "at+");
		if (fp)
		{
			fprintf(fp, "%s<%02d:%02d:%02d> T:(%5d) S:(%3d)\n",
				((bIsRecv) ? "R" : "S"),
				today->tm_mday + 1, today->tm_min, today->tm_sec,
				p->h.header.type,
				p->h.header.size);
			fclose(fp);
		}
	}
#endif
}

int	DequeuePacket(t_packet* packet, int type)
{
	/*t_packet* tempbuf;

	if (type == 0)	// inbuf
	{
		if (connections.inbuf == NULL) return 0;
		memcpy(packet, connections.inbuf, sizeof(t_packet));
		tempbuf = connections.inbuf;
		connections.inbuf = connections.inbuf->next;
		free(tempbuf);

		int iSize = packet->h.header.size + sizeof(t_header);

		//	if( connections.state > 1 && g_bLogOnFlag )//���� ���϶� �α��� �ص� ���ٴ� ������ ���� �� //CONNECT_LOGIN_KEIN
		if (g_bLogOnFlag)//������Ʈ�� �ٱ⸸ �ϸ�
		{
			de.Decrypt(packet->h.data, 2);
			de.Decrypt(packet->u.data, packet->h.header.size);
			if (packet->h.header.crc != de.GetCRCConvertChar())
			{
				JustMsg("Packet Error");
				exit(0);
			}
		}
		LogPacket(1, packet);
		//SDL_Log("packet.h.header.type Recv : %d", packet->h.header.type);
	}
	else if (type == 1)	// outbuf
	{
		if (connections.outbuf == NULL) return 0;
		memcpy(packet, connections.outbuf, sizeof(t_packet));
		tempbuf = connections.outbuf;
		data_size = packet->h.header.size;

		LogPacket(0, packet);
		SDL_Log("packet.h.header.type Send=%d  data_size=%d", packet->h.header.type, data_size);
		if (g_bLogOnFlag)//������Ʈ�� �پ���.
		{
			en.Encrypt(packet->h.data, 2);
			en.Encrypt(packet->u.data, packet->h.header.size);
			packet->h.header.crc = en.GetCRCConvertChar();
		}
		connections.outbuf = connections.outbuf->next;
		free(tempbuf);
	}
	connections.packet_count[type]--;
	return(1);*/
  if (type == 0) {  // inbuf
    if (connections.inbuf.empty()) {
      return 0;  // 隊列為空
    }

    // 從隊列前面取出封包並複製
    *packet = connections.inbuf.front();
    connections.inbuf.pop_front();

    // 你的加密/解密邏輯可以保留在這裡
    if (g_bLogOnFlag) {
      de.Decrypt(packet->h.data, 2);
      de.Decrypt(packet->u.data, packet->h.header.size);
      if (packet->h.header.crc != de.GetCRCConvertChar()) {
        JustMsg("Packet Error");
        exit(0);
      }
    }
    LogPacket(1, packet);

  } else if (type == 1) {  // outbuf
    if (connections.outbuf.empty()) {
      return 0;  // 隊列為空
    }

    // 從隊列前面取出封包並複製
    *packet = connections.outbuf.front();
    connections.outbuf.pop_front();

    // 你的加密邏輯可以保留在這裡
    LogPacket(0, packet);
    //SDL_Log("packet.h.header.type Send=%d data_size=%d", packet->h.header.type,
            //packet->h.header.size);
    if (g_bLogOnFlag) {
      en.Encrypt(packet->h.data, 2);
      en.Encrypt(packet->u.data, packet->h.header.size);
      packet->h.header.crc = en.GetCRCConvertChar();
    }

  } else {
    return 0;  // 無效的類型
  }

  return 1;
}

//011121 lsw >
void RecvCMD_ON_CONNECT(t_packet packet, t_connection* c)
{

	int ss = 0;
	if (!en.Initial(packet.u.OnConnectHSELInit.deinit))//������ en �ϴ°��� Ŭ���̾�Ʈ�� de �ϴ� ���̴�.
	{
		JustMsg("NetWork Error(e)");
		exit(0);
	}

	if (!de.Initial(packet.u.OnConnectHSELInit.eninit))//������ de �ϴ°��� Ŭ���̾�Ʈ�� en �ϴ� ���̴�.
	{
		JustMsg("NetWork Error(d)");
		exit(0);
	}
	g_bLogOnFlag = true;

	if (IIDD.length() >= ID_LENGTH || PPWW.length() >= ID_LENGTH)
	{
          SDL_Log("ERROR: %s %s length to long", IIDD.c_str(), PPWW.c_str());
          return;
	}
	packet.h.header.type = CMD_CONNECT_AGENT_SERVER_CONFORM;
	packet.h.header.size = sizeof(CONNECTAGENTSERVERCONFORM);

	packet.u.Hwoa.rang.ConnectAgentServerConform.iKey = 0;
	::strcpy(packet.u.Hwoa.rang.ConnectAgentServerConform.szUserID, IIDD.c_str());
	::strcpy(packet.u.Hwoa.rang.ConnectAgentServerConform.szUserPASS, PPWW.c_str());
	::QueuePacket(&packet, 1);
	SDL_Log("QueuePacket... CMD_CONNECT_AGENT_SERVER_CONFORM");
}

int HandleCommand(t_connection* c)
{

	t_packet	packet;

	while (1)
	{
		DequeuePacket(&packet, 0);

		//t = packet.h.header.type;

		switch (packet.h.header.type)
		{
		case CMD_NONE:
		{
			
			break;
		}
		case CMD_LOST_CONNECTION:
		{
			Release(c);
			break;
		}
		case CMD_HEXAEDITING:
		{
			int RTT = SDL_GetTicks() - packet.u.angra.client_editing_table.time_get_time;
			g_GameInfo.PutPing(RTT / 1.0f);
			break;
		}	//>> 031120 kyo
		case CMD_AGENT_SERVER_IP:
		{
			//strcpy( g_GameInfo.agent_ip, packet.u.agent_server_ip.ip );
			ListenCommand = CMD_AGENT_SERVER_IP;
			break;
		}
		case CMD_INVALID_ID:
		{
			ListenCommand = CMD_INVALID_ID;
			break;
		}
		case CMD_INVALID_PW:
		{
			ListenCommand = CMD_INVALID_PW;
			break;
		}
		case CMD_BANNED_PLAYER:
		{
			ListenCommand = CMD_BANNED_PLAYER;
			break;
		}
		case CMD_INVALID_DB:
		{
			ListenCommand = CMD_INVALID_DB;
			break;
		}
		case CMD_INVALID_VERSION:
		{
			ListenCommand = CMD_INVALID_VERSION;
			break;
		}
		case CMD_INVALID_PAY:
		{
			ListenCommand = CMD_INVALID_PAY;
			break;
		}
		case CMD_USED_ID:
		{
			ListenCommand = CMD_USED_ID;
			break;
		}
		case CMD_ACCEPT_LOGIN:
		{
			Recv4Character(&packet.u.server_accept_login);
			ListenCommand = CMD_ACCEPT_LOGIN;
			break;
		}
		case CMD_RM_NOW_REPAIR:
		{// 020930 kyo
			ListenCommand = CMD_RM_NOW_REPAIR;
			break;
		}
		case CMD_INVALID_AGE:
		{// 030929 kyo
			ListenCommand = CMD_INVALID_AGE;
			break;
		}
		case CMD_USER_ADD:
		{
			RecvUserAdd(&packet.u.server_user_add);
			break;
		}
		case CMD_USER_REMOVE:
		{
			RecvUserRemove(packet.u.server_user_remove.server_id);
			break;
		}
		case CMD_NPC_CHAT_DATA:
		{
			RecvNPCChatData(&packet.u.server_chat_data);
			break;
		}
		case CMD_CHAT_DATA:
		{
			RecvChatData(&packet.u.server_chat_data);
			break;
		}
		case CMD_MOVE:
		{
			RecvMoveData(&packet.u.server_move);
			break;
		}
		case CMD_WEATHER:
		{
			RecvWeatherData(&packet.u.server_weather);
			break;
		}
		case CMD_RM_CHANGE_WEATHER:
		{	// 011212 KBS
			RecvChangeWeather(&packet.u.RmChangeWeather);
			break;
		}
		case CMD_ACCEPT_JOIN:
		{
#ifdef _DEBUG // finito 060507
			if (GetSysInfo(SI_GAME_MAKE_MODE)) SDL_Log("CMD_ACCEPT_JOIN");
#endif
			RecvAcceptJoin(&packet.u.server_accept_join);
			ListenCommand = CMD_ACCEPT_JOIN;
			break;
		}
		case CMD_ASSIGN_SERVER_ID:
		{
			RecvAssignServerID(&packet.u.server_assign_server_id);
#ifdef _DEBUG // finito 060507
			if (GetSysInfo(SI_GAME_MAKE_MODE))
			{
				SDL_Log("CMD_ASSIGN_SERVER_ID");
			}
#endif
			break;
		}
		case CMD_ACCEPT_CREATE:
		{
			ListenCommand = CMD_ACCEPT_CREATE;
			break;
		}
		case CMD_ACCEPT_DELETE:
		{
			ListenCommand = CMD_ACCEPT_DELETE;
			break;
		}
		case CMD_USER_DB:
		{
			RecvHeroInit(&packet.u.server_user_db_data);
			ListenCommand = CMD_USER_DB;
			break;
		}
		case CMD_ENVIR_INFO:
		{
			RecvEnvironmentInfo(&packet.u.server_envir_info);
			break;
		}
		case CMD_ATTACK_RESULT:
		{
			g_mgrBattle.RecvAttackResult(Hero->id, &packet.u.strike.attack_result);
			break;
		}
		case CMD_ATTACK_RESULT_D:
		{
			g_mgrBattle.RecvAttackResult(&packet.u.strike.attack_result_d);
			break;
		}
		case CMD_THROW_ATTACK:
		{
			g_mgrBattle.RecvThrowAttack(&packet.u.strike.server_throw_attack);
			break;
		}
		case CMD_THROW_RESULT:
		{
			g_mgrBattle.RecvThrowResult(Hero->id, &packet.u.strike.server_throw_result);
			break;
		}
		case CMD_THROW_RESULT_D:
		{
			g_mgrBattle.RecvThrowResult(&packet.u.strike.throw_result_d);
			break;
		}
		case CMD_EFFECT_RESULT:
		{	//< CSD-031007
			g_mgrBattle.RecvEffectResult(&packet.u.strike.server_effect_result);
			break;
		}	//> CSD-031007
		case CMD_NPC_MAGIC:
		{
			g_mgrBattle.RecvNpcMagic(&packet.u.magic.server_npc_magic);
			break;
		}
		case CMD_MAGIC_SELECT:
		{	//< CSD-TW-030606
			g_mgrBattle.RecvMagicSelect(&packet.u.magic.server_magic_select);
			break;
		}	//> CSD-TW-030606
		case CMD_MAGIC_CASTING:
		{	//< CSD-TW-030606
			g_mgrBattle.RecvMagicCasting(&packet.u.magic.server_magic_casting);
			break;
		}	//> CSD-TW-030606
		case CMD_MAGIC_EXECUTE:
		{	//< CSD-TW-030606
			g_mgrBattle.RecvMagicExecute(&packet.u.magic.server_magic_execute);
			break;
		}	//> CSD-TW-030606
		case CMD_MAGIC_RESULT:
		{	//< CSD-TW-030606
			g_mgrBattle.RecvMagicResult(&packet.u.magic.server_magic_result);
			break;
		}	//> CSD-TW-030606
		case CMD_MAGIC_RESULT_T:
		{	//< CSD-TW-030606
			g_mgrBattle.RecvMagicResultT(&packet.u.magic.server_magic_result_t);
			break;
		}	//> CSD-TW-030606
		case CMD_MAGIC_RESULT_H:
		{	//< CSD-TW-030606
			g_mgrBattle.RecvMagicResultH(&packet.u.magic.server_magic_result_h);
			break;
		}	//> CSD-TW-030606
		case CMD_MAGIC_RESULT_M:
		{	//< CSD-TW-030606
			g_mgrBattle.RecvMagicResultM(&packet.u.magic.server_magic_result_m);
			break;
		}	//> CSD-TW-030606
		case CMD_COMBAT_OBTAIN:
		{
			g_mgrBattle.RecvCombatObtain(&packet.u.combat.server_combat_obtain);
			break;
		}
		case CMD_COMBAT_REQUEST:
		{
			g_mgrBattle.RecvCombatRequest(&packet.u.combat.server_combat_request);
			break;
		}
		case CMD_COMBAT_LEARN:
		{
			g_mgrBattle.RecvCombatLearn(&packet.u.combat.server_combat_learn);
			break;
		}
		case CMD_COMBAT_INIT:
		{
			g_mgrBattle.RecvCombatInit(&packet.u.combat.combat_init);
			break;
		}
		case CMD_COMBAT_CLEAR:
		{	//< CSD-TW-030606
			g_mgrBattle.RecvCombatClear(&packet.u.combat.server_combat_clear);
			break;
		}	//> CSD-TW-030606
		case CMD_COMBAT_SELECT:
		{
			g_mgrBattle.RecvCombatSelect(&packet.u.combat.server_combat_select);
			break;
		}
		case CMD_COMBAT_ATTACK:
		{
			g_mgrBattle.RecvCombatAttack(&packet.u.combat.server_combat_attack);
			break;
		}
		case CMD_COMBAT_RESULT:
		{
			g_mgrBattle.RecvCombatResult(&packet.u.combat.server_combat_result);
			break;
		}
		case CMD_COMBAT_RESULT_H:
		{
			g_mgrBattle.RecvCombatResultH(&packet.u.combat.server_combat_result_h);
			break;
		}
		case CMD_COMBAT_RESULT_T:
		{
			g_mgrBattle.RecvCombatResultT(&packet.u.combat.server_combat_result_t);
			break;
		}
		case CMD_CHAR_INFO_TACTIC:
		{	//< CSD-TW-030624
			g_mgrBattle.RecvCharInfoTactic(&packet.u.char_info_tactic);
			break;
		}	//> CSD-TW-030624
		case CMD_BATTLE_MESSAMGE:
		{
			g_mgrBattle.RecvBattleMessage(&packet.u.battle.battle_message);
			break;
		}
		case CMD_BATTLE_DAMAGE:
		{ //< CSD-021019
			g_mgrBattle.RecvBattleDamage(&packet.u.battle.battle_damage);
			break;
		} //> CSD-021019
		case CMD_ARENA_ADD_LOBBY:
		{	//< CSD-030520
			g_pArenaManager->RecvAddLobby(&packet.u.arena.arena_add_lobby);
			break;
		}	//> CSD-030520
		case CMD_ARENA_DELETE_LOBBY:
		{	//< CSD-030520
			g_pArenaManager->RecvDeleteLobby(&packet.u.arena.arena_delete_lobby);
			break;
		}	//> CSD-030520
		case CMD_ARENA_ADD_TEAM:
		{	//< CSD-030520
			g_pArenaManager->RecvAddTeam(&packet.u.arena.arena_add_team);
			break;
		}	//> CSD-030520
		case CMD_ARENA_DELETE_TEAM:
		{	//< CSD-030520
			g_pArenaManager->RecvDeleteTeam(&packet.u.arena.arena_delete_team);
			break;
		}	//> CSD-030520
		case CMD_ARENA_ADD_GAMBLING:
		{	//< CSD-030520
			g_pArenaManager->RecvAddGambling(&packet.u.arena.arena_add_gambling);
			break;
		}	//> CSD-030520
		case CMD_ARENA_DELETE_GAMBLING:
		{	//< CSD-030520
			g_pArenaManager->RecvDeleteGambling(&packet.u.arena.arena_delete_gambling);
			break;
		}	//> CSD-030520
		case CMD_ARENA_ADD_OBSERVE:
		{	//< CSD-030520
			g_pArenaManager->RecvAddObserve(&packet.u.arena.arena_add_observe);
			break;
		}	//> CSD-030520
		case CMD_ARENA_DELETE_OBSERVE:
		{	//< CSD-030520
			g_pArenaManager->RecvDeleteObserve(&packet.u.arena.arena_delete_observe);
			break;
		}	//> CSD-030520
		case CMD_ARENA_LEADER_INFO:
		{	//< CSD-030520
			g_pArenaManager->RecvLeaderInfo(&packet.u.arena.arena_leader_info);
			break;
		}	//> CSD-030520
		case CMD_ARENA_PROBABILITY_INFO:
		{	//< CSD-030509
			g_pArenaManager->RecvProbabilityInfo(&packet.u.arena.arena_probability_info);
			break;
		}	//> CSD-030509
		case CMD_ARENA_GAMBLING_INFO:
		{	//< CSD-030509
			g_pArenaManager->RecvGamblingInfo(&packet.u.arena.arena_gambling_info);
			break;
		}	//> CSD-030509
		case CMD_ARENA_DIVIDEND_INFO:
		{	//< CSD-030509
			g_pArenaManager->RecvDividendInfo(&packet.u.arena.arena_dividend_info);
			break;
		}	//> CSD-030509
		case CMD_ARENA_READY_INFO:
		{	//< CSD-030521
			g_pArenaManager->RecvReadyInfo(&packet.u.arena.arena_ready_info);
			break;
		}	//> CSD-030521
		case CMD_ARENA_GAME_INFO:
		{	//< CSD-TW-030701
			g_pArenaManager->RecvGameInfo(&packet.u.arena.arena_game_info);
			break;
		}	//> CSD-TW-030701
		case CMD_ARENA_GAMBLING_RESULT:
		{	//< CSD-030522
			g_pArenaManager->RecvGamblingResult(&packet.u.arena.arena_gambling_result);
			break;
		}	//> CSD-030522
		case CMD_ARENA_START_GAME:
		{	//< CSD-030521
			g_pArenaManager->RecvStartGame(&packet.u.arena.arena_start_game);
			break;
		}	//> CSD-030521
		case CMD_ARENA_GAME_MESSAGE1:
		{	//< CSD-030523
			g_pArenaManager->RecvArenaGameMessage1(&packet.u.arena.arena_game_message1);
			break;
		}	//> CSD-030523
		case CMD_ARENA_GAME_MESSAGE2:
		{	//< CSD-030523
			g_pArenaManager->RecvArenaGameMessage2(&packet.u.arena.arena_game_message2);
			break;
		}	//> CSD-030523
		case CMD_ARENA_GAME_MESSAGE3:
		{	//< CSD-030523
			g_pArenaManager->RecvArenaGameMessage3(&packet.u.arena.arena_game_message3);
			break;
		}	//> CSD-030523
		case CMD_ARENA_GAME_MESSAGE4:
		{	//< CSD-TW-030627
			g_pArenaManager->RecvArenaGameMessage4(&packet.u.arena.arena_game_message4);
			break;
		}	//> CSD-TW-030627
		// 0209 lkh ���� ���� 
		case CMD_SERVER_ARIGEMENT_RESPONSE:
		{
			RecvCMD_ARIGEMENT_RESPONSE(&packet.u.angra.server_arigement_response);
			break;
		}
		case CMD_SERVER_MAGIC_POSITION:
		{
			RecvCMD_MAGIC_POSITION(&packet.u.angra.server_magic_position);
			break;
		}
		case CMD_SERVER_CHARACTER_CONDITION:
		case CMD_SERVER_CHARACTER_CONDITION2:
		{
			RecvCMD_CHARACTER_CONDITION(&packet.u.angra.server_character_condition, packet.h.header.type);
			break;
		}
		case CMD_SERVER_MAGICVIEWTYPE:
		{
			RecvCMD_SERVER_MAGICVIEWTYPE(&packet.u.angra.server_magicviewtype);
			break;
		}
		case CMD_SERVER_MAGICCOMMAND:
		{
			RecvCMD_SERVER_MAGICCOMMAND(&packet.u.angra.server_magic_command);
			break;
		}
		case CMD_SERVER_MAGICTRAP_EXPOSE:
		{
			RecvCMD_SERVER_MAGICTRAP_EXPOSE(&packet.u.angra.server_magictrap_expose);
			break;
		}
		case CMD_SERVER_MAGICAL_DETECT:
		{
			RecvCMD_SERVER_MAGICAL_DETECT(&packet.u.angra.server_magical_detect);
			break;
		}
		case CMD_NPC_ADD:
		{
			RecvNpcAdd(&packet.u.server_npc_add);
			break;
		}
		case CMD_NPC_REMOVE:
		{
			RecvNpcRemove(packet.u.server_npc_remove.server_id);
			break;
		}
		case CMD_ITEM_ADD:
		{
			RecvAddItem(&packet.u.server_item_add);
			break;
		}
		case CMD_ITEM_ADD_ZIPED:
		{
			RecvAddItemZiped(&packet.u.server_item_add_ziped);
			break;
		}
		case CMD_ITEM_REMOVE:
		{
			RecvRemoveItem(packet.u.server_item_remove.item_id);
			break;
		}
		case CMD_ITEM_ATTR_CHANGE:
		{
			RecvItemAttrChange(&packet.u.item_attr_change);
			break;
		}
		case CMD_YOU_GOT_ITEM:
		{
			RecvYouGotItem(packet.u.server_you_got_item.item_id);
			break;
		}
		case CMD_CHANGE_EQUIP:
		{
			RecvChangeEquip(&packet.u.server_change_equip);
			break;
		}
		case REP_YOUR_DEPOSIT_INFO:
		{
			RecvBankDeposit(packet.u.kein.client_bank_deposit.money);
			break;
		}
		case REP_YOUR_REPAYMENT_INFO:
		{
			RecvBankRepayment(packet.u.kein.client_bank_repayment.loan_money,
				packet.u.kein.client_bank_repayment.last_loan,
				packet.u.kein.client_bank_repayment.last_loan_time);
			break;
		}
		case CMD_CHR_ITEM_INFO_0:
		{
			RecvChrItemInfo_0(&packet.u.chr_item_info0);
			break;
		}
		case CMD_CHR_ITEM_INFO_1:
		{
			RecvChrItemInfo_1(&packet.u.chr_item_info1);
			break;
		}
		case CMD_CHR_ITEM_INFO_2:
		{
			RecvChrItemInfo_2(&packet.u.chr_item_info2);
			break;
		}
		case CMD_CHAR_INFO_MAGIC:
		{
			RecvCharInfoMagic(&packet.u.char_info_magic);
			break;
		}
		case CMD_CHAR_INFO_SKILL:
		{
			RecvCharInfoSkill(&packet.u.char_info_skill);
			break;
		}
		case CMD_ITEM_EQUIP_OK:
		{
			RecvItemEquip(&packet.u.chr_status_info);		// 0928 ���� �߰�
			InventoryBackUp();
			break;
		}
		case CMD_ITEM_EXCHANGE_REFUSE:
		{
			InventoryRestore();
			HandItemOn = true;
			break;
		}
		case CMD_CONNECT_INFO:
		{											// 021030 YGI
			packet.u.data[packet.h.header.size] = 0;

			if (RecvConnectInfo(&packet.u.server_connect_info) == 0)
			{
				ListenCommand = CONNECT_REFUSE;
			}
			else
			{
				ListenCommand = CMD_CONNECT_INFO;
			}

			break;
		}

		case CMD_CHANGE_MAP:
		{
			if (!NowLoading)		// 021030 YGI
			{
				packet.u.data[packet.h.header.size] = 0;
				ChangeMapFlag = 1;
				ChangeMapPacket = packet.u.server_change_map;
				RecvEventObject(&packet.u.kein.new_server_change_map.data2);
				return 1;
			}
#ifdef _DEBUG // finito 060507
			if (GetSysInfo(SI_GAME_MAKE_MODE))
			{
				SDL_Log("CMD_CHANGE_MAP");
			}
#endif
			break;
		}
		case CMD_CHANGE_MAP_FAIL_TIME:
		{
			AddCurrentStatusMessage(255, 100, 100, kein_GetMenuString(239), packet.u.kein.mapchange_timer.second); // Finito
			break;
		}
		case CMD_CHANGE_MAP_NOT:
		case CMD_CHANGE_MAP_FAIL:
		{
			AddCurrentStatusMessage(255, 100, 100, lan->OutputMessage(4, 4));//lsw	
			break;
		}
		case CMD_CALL_SMALLMENU:
		{
			RecvCallSmallMenu(&packet.u.server_call_smallmenu);
			break;
		}
		case CMD_DUAL_MAGIC_LEARN_BOOK:
		{	//<soto-030421
			RecvCallDualMagicLearnBook();
			break;
		}	//>soto-030421
		case CMD_LEVELUP_POINT:
		{
			RecvLevelUpPoint(&packet.u.server_levelup_point);
			break;
		}
		case CMD_LEVELUP_POINT_OK:
		case CMD_LEVELUP_POINT_NO:
		{
			RecvLevelUpPointResult(packet.h.header.type);
			break;
		}
		case CMD_INTERFACE_PEACE:
		{
			RecvPeaceBattle(packet.u.server_interface_peace.client_id, 0);
			break;
		}
		case CMD_INTERFACE_BATTLE:
		{
			RecvPeaceBattle(packet.u.server_interface_battle.client_id, 1);
			break;
		}
		case CMD_STORE_ITEMLIST:
		{
			RecvStoreItemList(&packet.u.server_store_itemlist);
			break;
		}
		case CMD_SKILL_INFO:
		{
			RecvSkillInfo(&packet.u.server_skill_info);
			break;
		}
		case CMD_SKILL_RESULT:
		{
			RecvSkillResult(&packet);
			break;
		}
		case CMD_SKILL_ANIMAL_LORE:
		{
			RecvSkillAnimalLore(&packet);
			break;
		}
		case CMD_OUTPUT_MESSAGE:
		{
			RecvOutPutMessage(&packet);
			break;
		}
		case CMD_SOUND_OUT:
		{
			RecvSoundOut(&packet);
			break;
		}
		case CMD_ITEM_BOX_BREAK:
		{
			Recv_CMD_ITEM_BOX_BREAK(&packet.u.server_item_box_break);
			break;
		}
		case CMD_ITEM_BOX_MAGIC_BREAK_RESULT:
		{
			Recv_CMD_ITEM_BOX_MAGIC_BREAK_RESULT(&packet.u.server_item_box_magic_break_result);
			break;
		}
		case CMD_MODIFY_POSITION:
		{
			RecvModifyPosition(&packet.u.server_modify_position);
			break;
		}
		case CMD_MODIFY_POSITION_ABSOLUTE:
		{
			RecvModifyPositionAbsolute(&packet.u.server_modify_position_absolute);
			break;
		}
		case CMD_SKILL_ITEMMAKE_RESULT:
		{
			RecvSkillItemMakeResult(&packet.u.server_skill_itemmake_result);
			break;
		}
		case CMD_SKILL_RESULT_FAIL:
		{
			RecvSkillResultFail(&packet.u.server_skill_result_fail);
			break;
		}
		case CMD_SKILL_GAUGE:
		{
			RecvSkillGauge(&packet.u.server_skill_gauge);
			break;
		}
		case CMD_FOOT_ON_TRAP_0_ITEM:
		{
			RecvCMD_FOOT_ON_TRAP_0_ITEM(&packet.u.server_foot_on_trap0);
			break;
		}
		case CMD_FOOT_ON_TRAP_1_ITEM:
		{
			RecvCMD_FOOT_ON_TRAP_1_ITEM(&packet.u.server_foot_on_trap1);
			break;
		}
		case CMD_GIVE_ITEM:
		{
			RecvGiveItem(&packet.u.server_give_item);
			break;
		}
		case CMD_VIEWTYPE_CHANGE:
		{
			RecvViewtypeChange(&packet.u.viewtype_change);
			break;
		}
		case CMD_VIEW_CHAR_INFO:
		{
			RecvViewCharInfo(&packet.u.view_char_info);
			break;
		}
		case CMD_CHAT_SET_RESULT:
		{
			RecvChatSetResult(&packet.u.chat_set_result);
			break;
		}
		case CMD_TOTAL_MAP_CONNECTIONS:
		{
			RecvTotalMapConnections(packet.u.total_map_connections.no);
			break;
		}
		case CMD_TOTAL_CONNECTIONS:
		{
			RecvTotalConnections(packet.u.total_connections.no);
			break;
		}
		case CMD_THEREIS_NO_CHARNAME:
		{
			ListenCommand = CMD_THEREIS_NO_CHARNAME;
			break;
		}
		case CMD_THEREIS_CHARNAME:
		{
			ListenCommand = CMD_THEREIS_CHARNAME;
			break;
		}
		case CMD_TAME_COMMAND_RESULT:
		{
			RecvTameCommandResult(packet.u.tame_command_result.result);
			break;
		}
		case CMD_TAME_NAME_CHANGE:
		{
			RecvTameNameChange(&packet.u.tame_name_change);
			break;
		}
		case CMD_TAME_HOST_NAME:
		{
			RecvHostName(&packet.u.tame_host_name);
			break;
		}
		case CMD_TAME_END:
		{
			RecvHostEnd(&packet.u.tame_end);
			break;
		}
		case CMD_DEBUG_REQ_CHAR_INFO:
		{
			RecvDebugReqCharInfo(&packet.u.debug_server_req_char_info);
			break;
		}
		case CMD_PLEASE_GIVE_LIFE:
		{
			RecvPleaseGiveLife(packet.u.please_give_life.id, packet.u.please_give_life.nLife, packet.u.please_give_life.nMana, packet.u.please_give_life.nHungry, packet.u.please_give_life.who);
			break;
		}
		case CMD_SMILE_FACE:
		{
			RecvSmileFace(packet.u.server_smile_face.id, packet.u.server_smile_face.smileno);
			break;
		}
		case CMD_BLOCK_MOVE_START:
		case CMD_BLOCK_MOVE_END:
			g_block_move = (packet.h.header.type == CMD_BLOCK_MOVE_START) ? true : false;
			break;
		//�������� ���������� �����Դ�. 
		case CMD_BBS:
		{//021030 lsw
			////				2003/2/13 soto
			/// ��Ƽ���� ������ ���ؼ� ����.
			char	szTemp[600] = { 0, };
			int		nSourcePos = 0;
			int		nTempPos = 0;
			int		nStringSize = strlen(packet.u.server_bbs.bbs);


			AddCurrentStatusMessage(FONT_COLOR_SOLID_YELLOW, lan->OutputMessage(3, 131));//�޼����� ����


			//��Ƽ���� ����.
			if (nStringSize)
			{
				while (1)
				{
					if (13 != packet.u.server_bbs.bbs[nSourcePos])//CR
					{
						if (10 == packet.u.server_bbs.bbs[nSourcePos])
						{
							if (!strlen(szTemp))
							{
								szTemp[0] = ' ';
							}

							AddCurrentStatusMessage(FONT_COLOR_SOLID_YELLOW, szTemp);//�߰� ��Ų��.
							memset(szTemp, 0, 600);

							nSourcePos++;
							nTempPos = 0;
							continue;
						}
						else
						{
							if (!packet.u.server_bbs.bbs[nSourcePos])
							{
								if (strlen(szTemp))
								{//���ڰ� �ִ�.
									AddCurrentStatusMessage(FONT_COLOR_SOLID_YELLOW, szTemp);//�߰� ��Ų��.
									memset(szTemp, 0, 600);
								}
								break;
							}

							szTemp[nTempPos] = packet.u.server_bbs.bbs[nSourcePos];
						}
					}

					nSourcePos++;
					nTempPos++;

				}//end of while
			}

			nSourcePos = 0; nTempPos = 0;
			memset(szTemp, 0, 600);

			//ä�ÿ��ٰ��� �߰�
			if (packet.u.server_bbs.bbs[0] != 13) //ù�ٿ� �ٹٲ��� ������..���� �ٲ���.
			{
				szTemp[nTempPos++] = 10;
			}

			for (nSourcePos; nSourcePos < nStringSize; nSourcePos++)
			{
				if (13 != packet.u.server_bbs.bbs[nSourcePos])
				{
					szTemp[nTempPos] = packet.u.server_bbs.bbs[nSourcePos];
					nTempPos++;
				}
			}
			ChatMgr.AddString(NULL, szTemp, CHATTYPE_NOTICE);//ä�ÿ��ٰ��� �߰�				
			////////////////////////////////////////////////////////////////////////////////////////////// 2003/2/13 soto end.
			break;
		}
		case CMD_ITEM_DURATION_CHANGE:
		{
			RecvItemDurationChange(&packet.u.item_duration_change);
			break;
		}
		case CMD_NPCCTRL_SEALSTONE_:
		{
			LPCHARACTER tempch = ReturnCharacterPoint(packet.u.npcctrl_sealstone.id);

			if (tempch)
			{
				tempch->patterntype = packet.u.npcctrl_sealstone.patterntype;
			}

			break;
		}
		case CMD_NPCCTRL_MURI_8:
		{
			LPCHARACTER tempch = ReturnCharacterPoint(packet.u.npcctrl_muri_8.id);
			if (tempch)
			{
				tempch->oldpatterntype = packet.u.npcctrl_muri_8.oldpatterntype;
				tempch->patterntype = packet.u.npcctrl_muri_8.patterntype;
				tempch->bossid = packet.u.npcctrl_muri_8.bossid;
				tempch->Exp = packet.u.npcctrl_muri_8.Exp;
			}
			break;
		}
		case CMD_NPCCTRL_MURI_18:
		{
			LPCHARACTER tempch = ReturnCharacterPoint(packet.u.npcctrl_muri_18.id);
			if (tempch)
			{
				tempch->oldpatterntype = packet.u.npcctrl_muri_18.oldpatterntype;
				tempch->patterntype = packet.u.npcctrl_muri_18.patterntype;
			}
			break;
		}
		case CMD_NPCCTRL_MURI_28:
		{
			LPCHARACTER tempch = ReturnCharacterPoint(packet.u.npcctrl_muri_28.id);
			if (tempch)
			{
				tempch->oldpatterntype = packet.u.npcctrl_muri_28.oldpatterntype;
				tempch->patterntype = packet.u.npcctrl_muri_28.patterntype;
				tempch->bossid = packet.u.npcctrl_muri_28.bossid;
				tempch->Exp = packet.u.npcctrl_muri_28.Exp;
			}
			break;
		}
		case CMD_NPCCTRL_WANDER_MAUL:
		{
			LPCHARACTER tempch = ReturnCharacterPoint(packet.u.npcctrl_wander_maul.id);
			if (tempch)
			{
				tempch->oldpatterntype = packet.u.npcctrl_wander_maul.oldpatterntype;
				tempch->patterntype = packet.u.npcctrl_wander_maul.patterntype;
			}
			break;
		}
		case CMD_NPCCTRL_NO_MOVE:
		{
			LPCHARACTER tempch = ReturnCharacterPoint(packet.u.npcctrl_no_move.id);
			if (tempch)
			{
				tempch->oldpatterntype = packet.u.npcctrl_no_move.oldpatterntype;
				tempch->patterntype = packet.u.npcctrl_no_move.patterntype;
			}
			break;
		}
		case CMD_NPCCTRL_WANDER_GUARD:
		{
			LPCHARACTER tempch = ReturnCharacterPoint(packet.u.npcctrl_wander_guard.id);
			if (tempch)
			{
				tempch->oldpatterntype = packet.u.npcctrl_wander_guard.oldpatterntype;
				tempch->patterntype = packet.u.npcctrl_wander_guard.patterntype;
			}
			break;
		}
		case CMD_NPCCTRL_WANDER_38:
		{
			LPCHARACTER tempch = ReturnCharacterPoint(packet.u.npcctrl_wander_38.id);
			if (tempch)
			{
				tempch->oldpatterntype = packet.u.npcctrl_wander_38.oldpatterntype;
				tempch->patterntype = packet.u.npcctrl_wander_38.patterntype;
				tempch->bossid = packet.u.npcctrl_wander_38.bossid;
			}
			break;
		}
		case CMD_NPCCTRL_TAME:
		{
			LPCHARACTER tempch = ReturnCharacterPoint(packet.u.npcctrl_tame.id);
			if (tempch)
			{
				tempch->oldpatterntype = packet.u.npcctrl_tame.oldpatterntype;
				tempch->patterntype = packet.u.npcctrl_tame.patterntype;
				tempch->bossid = packet.u.npcctrl_tame.bossid;
				tempch->HostId = packet.u.npcctrl_tame.HostId;
				strcpy(tempch->TargetName, (char*)packet.u.npcctrl_tame.TargetName);
				tempch->tame = packet.u.npcctrl_tame.tame;
				tempch->tame_cmd = packet.u.npcctrl_tame.tame_cmd;
			}

			break;
		}
		case CMD_SEND_NPC_PARAMETER:
		{
			RecvNPC_parameter(packet.u.send_npc_parameter.id,
				packet.u.send_npc_parameter.type,
				packet.u.send_npc_parameter.data);
			break;
		}
		case CMD_SEND_NPC_STRING_PARAMETER:
		{
			RecvNPC_stringparameter(packet.u.send_npc_string_parameter.id,
				packet.u.send_npc_string_parameter.type,
				packet.u.send_npc_string_parameter.data);
			break;
		}
		case CMD_START_GOODBAD:
		{
			RecvNK(packet.u.start_goodbad.id, packet.u.start_goodbad.nk3,
				packet.u.start_goodbad.nk4, packet.u.start_goodbad.nk6);
			break;
		}
		case CMD_SKILL_RESULT_TOOTHER:
		{
			RecvSkillResultFromOther(&packet.u.server_skill_result_toother);
			break;
		}
		case CMD_TACTICS_PARRYING_EXP:
		{
			CalcTacticsSkillExp(PARRYING, packet.u.tactics_parrying_exp.exp, packet.u.tactics_parrying_exp.step);
			break;
		}
		case CMD_BBS_QUEST:
		{
			RecvBbs_quest(packet.u.server_bbs.bbs);
			break;		// LHS													
		}
		case CMD_QSF_COMMAND:
		{
			//RecvQSF_Command(packet.u.angra.qsf_command);
			break;
		}
		// 0811 NPC KHS																	
		case CMD_SERVER_NPC_ACCESS_TABLE_RELOAD:
		{
			Recv_CMD_SERVER_NPC_ACCESS_TABLE_RELOAD(&packet);
			break;
		}
		case CMD_SERVER_NPC_ACCESS_TABLE_RELOAD2:
		{
			Recv_CMD_SERVER_NPC_ACCESS_TABLE_RELOAD2(&packet);
			break;
		}
		case CMD_SERVER_NPC_ACCESS_TABLE_RELOAD3:
		{
			Recv_CMD_SERVER_NPC_ACCESS_TABLE_RELOAD3(&packet);
			break;
		}
		case CMD_SERVER_NPC_ACCESS_TABLE_RELOAD4:
		{
			Recv_CMD_SERVER_NPC_ACCESS_TABLE_RELOAD4(&packet);
			break;
		}
		case CMD_SEALSTONE_WAR_START:
		{
			Recv_CMD_SEALSTONE_WAR_START();
			break;
		}
		case CMD_SEALSTONE_WAR_LEFTTIME:
		{
			Recv_CMD_SEALSTONE_WAR_LEFTTIME(packet.u.sealstone_client_warlefttime.lefttime);
			break;
		}
		case CMD_WAR_START_BLOCK:
		{
			Recv_CMD_WAR_START_BLOCK();
			break;
		}
		case CMD_SEALSTONE_RESULT_DEFEAT:
		{
			Recv_CMD_SEALSTONE_RESULT_DEFEAT();
			break;
		}
		case CMD_SEALSTONE_RESULT_VICTORY:
		{
			Recv_CMD_SEALSTONE_RESULT_VICTORY();
			break;
		}
		case CMD_SEALSTONE_RESULT_DRWAW:
		{
			Recv_CMD_SEALSTONE_RESULT_DRWAW();
			break;
		}
		case CMD_SEALSTONE_STATUS:
		{
			Recv_CMD_SEALSTONE_STATUS(&packet.u.sealstone_client_status);
			break;
		}
		case CMD_STEALSTONE_BREAKABLE:
		{
			AddCurrentStatusMessage(255, 225, 225, lan->OutputMessage(4, 5));//lsw
			g_LocalWarBegin = g_OtherNationClick = 1;		// ����ð�		// 011215 LTS
			break;
		}
		case CMD_STEALSTONE_NON_BREAKABLE_PEACE:
		{
			AddCurrentStatusMessage(225, 255, 225, lan->OutputMessage(4, 7));
			g_LocalWarBegin = g_OtherNationClick = 0;		// ���� ��		// 011215 LTS
			break;
		}
		case CMD_STEALSTONE_NON_BREAKABLE:
		{
			AddCurrentStatusMessage(225, 225, 255, lan->OutputMessage(4, 6));//lsw
			g_LocalWarBegin = g_OtherNationClick = 0;		// ���� ����	// 011215 LTS
			break;
		}
		case CMD_NATION2NATION_RELATION_RESULT:
		{
			Recv_CMD_NATION2NATION_RELATION_RESULT(&packet);
			break;
		}
		case CMD_JUST_ANI:
		{
			Recv_JustAni(&packet.u.just_ani);
			break;
		}
		case CMD_FACE_DIRECTIONS:
		{
			Recv_FaceDirections(&packet.u.face_directions);
			break;
		}
		case CMD_MANTLE_CANDLE:
		{
			RecvMantleCandle(&packet.u.mantle_candle);
			break;
		}
		case CMD_CANDLE_ON:
		{
			RecvCandleOn(&packet.u.candle_on);
			break;
		}
		case CMD_CANDLE_OFF:
		{
			RecvCandleOff(&packet.u.candle_off);
			break;
		}
		case CMD_SO_MUCH_LOGINUSER_WAIT_FOR_A_MIN:
		{
			ListenCommand = CMD_SO_MUCH_LOGINUSER_WAIT_FOR_A_MIN;
			break;
		}
		case CMD_LOGIN_FAIL_MASSAGE:
		{
			ListenCommand = CMD_LOGIN_FAIL_MASSAGE;
			g_client_login_fail_reason = packet.u.client_login_fail_reason;
			break;
		}
		case CMD_CLOSE_CONNECTION_NORMAL:
		case CMD_CLOSE_CONNECTION_ABNORMAL:
		case CMD_CLOSE_CONNECTION_SAME_ID_LOGON:
		case CMD_CLOSE_CONNECTION_SLEPT_TOO_LONG_TIME:
		{
			MP3(SN_WARNING);		// 010221 YGI
			break;
		}
		case CMD_CHECK_ACCEL:
		{
			CrackMgr.RecvCheckAccel(&packet.u.server_check_accel);
			break;
		}
		case CMD_ORDER_DISCONNECT: //010730 YGI
		{
			Kein_PutMessage(KM_FAIL, lan->OutputMessage(6, 95));
			break;
		}

		case CMD_SKILL_RARE_UPGRADE_RESULT:
		{
			Recv2NDGMMAKEResult(&packet.u.SkillRareUpgradeResult);
			break;
		}
		case CMD_CALL_SMALLMENUCUSTOM:
		{
			RecvCallSmallMenuCustom(&packet.u.CallSmallMenuCustom);
			break;
		}
		case CMD_WAR_BBS:
		case CMD_ANSWER_WARFIELD_STATUS:
		case CMD_ANSWER_VOTE_DATA:
		case CMD_ANSWER_SQUAD_INFO:
		case CMD_ANSWER_COMMANDER_VOTE_DATA:
		case CMD_ANSWER_COMMANDER_REG_DATA:
		case CMD_YOU_ARE_SQUAD_COMMANDER:
		case CMD_YOU_ARE_SQUAD_LEADER:
		case CMD_JOIN_SQUAD:
		case CMD_ANSWER_NATION_WAR_COUNT:
		case CMD_NW_MAP_MOVE_FAIL:
		case CMD_ANSWER_TILE_DONT:
		case CMD_TILE_DONT_CHANGED:
		case CMD_SQUAD_CHAT:
		case CMD_CLEAR_SQUAD_DATA:
		case CMD_ANSWER_WARFIELD_INFO:
		case CMD_WAR_BEGIN:
		case CMD_WAR_END:
		case CMD_ANSWER_CHECK_WARFIELD_POSSESSION:
		case CMD_ANSWER_WARFIELD_POSSESSION:
		case CMD_WARFIELD_WAR_DATA1:
		case CMD_WARFIELD_WAR_DATA2:
		case CMD_WARFIELD_WAR_DATA3:
		case CMD_WARFIELD_WAR_DATA4:
		case CMD_WARFIELD_WAR_DATA5:
		case CMD_CLEAR_WAR_CHARACTER_DATA:		// 010915 LTS
		case CMD_ANSWER_SQUAD_INFO2:
		case CMD_ANSWER_COMMANDER_VOTE_DATA2:
		case CMD_SQUAD_JOIN_RESULT:
		case CMD_GUARD_CAN_NOT_BROKE:			// LTH-040317-KO
		case CMD_CHECK_REMAIN_GUARD:			// LTH-040322-KO
		{
			CheckHandleByNationWar(&packet);
			break;
		}
		case CMD_DUAL_ENABLE:
		{ //< CSD-021003
			g_mgrDual.RecvDualEnable(&packet.u.dual.server_dual_enable);
			break;
		} //> CSD-021003
		case CMD_DUAL_CHANGE:
		{
			g_mgrDual.RecvDualChange(&packet.u.dual.server_dual_change);
			break;
		}
		case CMD_DUAL_DIVIDE:
		{
			g_mgrDual.RecvDualDivide(&packet.u.dual.server_dual_divide);
			break;
		}
		case CMD_ON_CONNECT:
		{
			RecvCMD_ON_CONNECT(packet, c);
			break;
		}
		case CMD_GUARDIANLIST:
		{	//< soto-030331
			int count = packet.u.GuardianList.nCount;
			for (int i = 0; i < count; ++i)
			{
				g_CGuardianGuild.AddGuild(packet.u.GuardianList.anGuildCodes[i]);
			}
			break;
		}	//> soto-030331
		case CMD_SYMBOL_MAKE:
		{
			g_SymBolMgr.Recv_SymbolMake(&packet.u.Symbol_Item);
			break;
		}
		case CMD_SYMBOL_UPGRADE:
		{
			g_SymBolMgr.Recv_SymbolUpGrade(&packet.u.Symbol_Item);
			break;
		}
		case CMD_OPEN_LUCKY_MENU:
		{	//<soto-030429
			LuckyMenuOpen();
			break;
		}	//>soto-030429

		case CMD_LOTTO_BUY:
		{	//<soto-030429
			if (LottoMgr())LottoMgr()->RecvBuyLotto(&packet.u.Lotto_Buy);
			break;
		}	//>soto-030429
		case CMD_OPEN_WINNER_MENU:
		{
			if (LottoMgr())
			{
				LottoMgr()->RecvOpenWinnerMenu(&packet.u.Lotto_Winner_Menu);

			}

			break;
		}
		case CMD_OPEN_LOTTOMENU://soto-030520
		{
			if (LottoMgr())
			{
				LottoMgr()->RecvOpenLottoMenu(&packet.u.Lotto_Menu_Open);
			}

			break;
		}
		case CMD_WINNER_CHECK:
		{
			if (LottoMgr())
			{
				LottoMgr()->RecvCheckWinner(&packet.u.Check_Winner);
			}

			break;
		}
		case CMD_SEND_ACCEL_TYPE://反外掛
		{	
			//ExitApplication(EA_CRACK_THREAD_FIND_CRACK);
			break;
		}
		case CMD_LOTTERY_BROADCAST:
		{
			if (packet.u.Lotto_BroadCast.nWinNumCount == 5)
			{
				::AddCurrentStatusMessage(
					0, 255, 255,
					lan->OutputMessage(4, 83),
					packet.u.Lotto_BroadCast.anWinNumbers[0],
					packet.u.Lotto_BroadCast.anWinNumbers[1],
					packet.u.Lotto_BroadCast.anWinNumbers[2],
					packet.u.Lotto_BroadCast.anWinNumbers[3],
					packet.u.Lotto_BroadCast.anWinNumbers[4]
				);
			}
			else if (packet.u.Lotto_BroadCast.nWinNumCount == 6)
			{
				::AddCurrentStatusMessage(
					0, 255, 255,
					lan->OutputMessage(4, 83),
					packet.u.Lotto_BroadCast.anWinNumbers[0],
					packet.u.Lotto_BroadCast.anWinNumbers[1],
					packet.u.Lotto_BroadCast.anWinNumbers[2],
					packet.u.Lotto_BroadCast.anWinNumbers[3],
					packet.u.Lotto_BroadCast.anWinNumbers[4],
					packet.u.Lotto_BroadCast.anWinNumbers[5] //>soto-HK030924
				);
			}

			break;
		}
		//>soto-HK030924
		case CMD_LOTTO_SEEK:
		{	//<soto-Lotto�߰�
			LottoMgr()->RecvLottoSeek(&packet.u.Lotto_Seek);
			break;
		}	//>soto-Lotto�߰�.
		case CMD_SERVER_TIME: // finito 060707
		{
			//SetTimer( g_hwndMain, 14, 1000, NULL);
			g_Current_Server_Time = packet.u.ServerTime.nTime;
		}
		break;

		case CMD_SERVER_DAY://not in use Roy 2025-08-25
		{
			//g_Current_Server_Day = packet.u.ServerDate.g_wday;
		}


		break;
		case CMD_FRIEND_STATUS: // finito 09/08/07
		{
			if (packet.u.FriendStatus.online == true)
			{
				AddCurrentStatusMessage(FONT_COLOR_YELLOW, "Friend %s has loged in.", packet.u.FriendStatus.name);
			}
			else
			{
				AddCurrentStatusMessage(FONT_COLOR_YELLOW, "Friend %s has loged off.", packet.u.FriendStatus.name);
			}
		}
		break;
		//case CMD_BAN_ACCOUNT:
		//	
		//		AddCurrentStatusMessage(FONT_COLOR_YELLOW, "Character %s has Been banned.", packet.u.FriendStatus.name);
		//
		//		//AddCurrentStatusMessage(FONT_COLOR_YELLOW, "Cant Find Player.", packet.u.BanAccount.characterName);
		//	
		//	break;
		case CMD_MALLITEM_LIST:
		{
			RecvMallItemList(&packet.u.MallItemList);
			break;
		}
		default:
		{
			int msg = CheckHandleByKein(&packet);

			if (msg == 1)
			{
				break;							// 011017 LTS
			}

			if (!HandleCommand2(packet, c))
			{
				if (!HandleCommand3(packet, c))
				{
					if (1 != CheckHandleByNationWar(&packet))
					{
						JustMsg("Server Send Unknown Message\n[%d]", packet.h.header.type);
					}
				}
			}

			break;
		}
		}

		if (c->inbuf.empty())
		{
			break;
		}
	}

	return 1;
}

int HandleReading(t_connection* c)
{
	/*int		addlen;
	DWORD	dwBytesRead;

	while (1)
	{
		if (ioctlsocket(c->socket, FIONREAD, &dwBytesRead) < 0)
		{
			return(-11);
		}

		if (dwBytesRead == 0)
		{
			break;
		}

		if (c->packet_status == 0)
		{
			addlen = recv(c->socket, &c->len[c->packet_pnt], c->packet_size, 0);
			if (addlen < 0)
			{
				int err = WSAGetLastError();
				JustMsg("Network Error %d", err);
				return(-12);
			}
			else if (addlen > 0)
			{
				c->packet_size -= addlen;
				c->packet_pnt += addlen;

				g_RecvBytes += addlen;

				if (c->packet_size == 0)
				{
					c->udwp_len = (WORD*)c->len;

					c->packet_pnt = 0;
					c->packet_size = sizeof(t_header);
					c->packet_status = 1;
				}
				else if (c->packet_size < 0)
				{
					return(-13);
				}
			}
		}
		else if (c->packet_status == 1)
		{
			addlen = recv(c->socket, &c->packet.h.data[c->packet_pnt], c->packet_size, 0);
			if (addlen < 0)
			{
				int err = WSAGetLastError();
				JustMsg("Network Error %d", err);
				return(-12);
			}
			else if (addlen > 0)
			{
				if (c->packet.h.header.size < 0)
				{
					JustMsg("HandleReading Error size < 0 ");
					_asm int 3;
					_asm nop;
				}

				c->packet_size -= addlen;
				c->packet_pnt += addlen;

				g_RecvBytes += addlen;

				if (c->packet_size == 0)
				{
					if (c->packet.h.header.size == 0)
					{
						QueuePacket(&c->packet, 0);
						memset(&c->packet, 0, sizeof(t_packet));

						c->packet_pnt = 0;
						c->packet_size = 2;
						c->packet_status = 0;
					}
					else
					{
						c->packet_pnt = 0;
						c->packet_size = c->packet.h.header.size;
						c->packet_status = 2;
					}
				}
				else if (c->packet_size < 0)
				{
					return(-13);
				}
			}


		}
		else if (c->packet_status == 2)
		{
			addlen = recv(c->socket, &c->packet.u.data[c->packet_pnt], c->packet_size, 0);

			if (addlen < 0)
			{
				int err = WSAGetLastError();
				switch (err)
				{
				case WSAENOBUFS:		//c->packet_pnt	= 0;
					//c->packet_size	= sizeof( t_header );
					//c->packet_status= 0;
					_asm nop;
					return 1;

				case WSAEWOULDBLOCK:	return 1;

				default:
					return(-14);
				}
			}
			else if (addlen > 0)
			{
				c->packet_size -= addlen;
				c->packet_pnt += addlen;

				g_RecvBytes += addlen;

				if (c->packet_size == 0)
				{
					QueuePacket(&c->packet, 0);
					memset(&c->packet, 0, sizeof(t_packet));

					c->packet_pnt = 0;
					c->packet_size = 2;;
					c->packet_status = 0;
				}
				else if (c->packet_size < 0)
				{
					return(-15);
				}
			}
		}
	}

	return 1;*/

	int addlen;

        // 說明：
        // HandleRunning 函式會在外層檢查 Socket 是否有活動。
        // 因此，這個函式被呼叫時，我們就假設有資料可以讀取，或者有網路事件發生。
        // 舊有的 while(true) 迴圈是錯誤的，因為它會導致在讀完一次資料後，
        // 再次嘗試讀取而卡死。正確的模式是，每次 HandleRunning 偵測到活動，
        // 就進來這裡處理一次，函式能讀多少就讀多少，然後返回，等待下一次被呼叫。
        // 狀態機 (packet_status) 會維持讀取封包的進度。

        if (c->packet_status == 0)  // 狀態0：讀取封包長度 (2 bytes)
        {
          addlen = SDLNet_TCP_Recv(c->socket, &c->len[c->packet_pnt],
                                   c->packet_size);

          if (addlen <= 0)  // <= 0 同時處理網路錯誤和遠端正常斷線
          {
            //JustMsg("Network Error or Disconnect (status 0): %s",
                    //SDLNet_GetError());
            if (!g_StartMenuOn) {
              QuitFlag = SWD_LOGON;//網絡斷線登出
              StopWaveOfMenu();
              Release(&connections);
              CrackMgr.InitCrackMgr();
            }
            return -12;  // 返回錯誤碼，觸發上層的斷線處理
          }

          c->packet_size -= addlen;
          c->packet_pnt += addlen;
          g_RecvBytes += addlen;
          c->receive_bytes = g_RecvBytes;

          if (c->packet_size == 0)  // 長度資訊已完整接收
          {
            c->packet_pnt = 0;
            c->packet_size = sizeof(t_header);  // 準備接收 Header
            c->packet_status = 1;
          } else if (c->packet_size < 0)  // 邏輯錯誤檢查
          {
            return -13;
          }
        } else if (c->packet_status == 1)  // 狀態1：讀取封包頭 (t_header)
        {
          addlen = SDLNet_TCP_Recv(c->socket, &c->packet.h.data[c->packet_pnt],
                                   c->packet_size);

          if (addlen <= 0) {
            JustMsg("Network Error or Disconnect (status 1): %s",
                    SDLNet_GetError());
            return -12;
          }

          c->packet_size -= addlen;
          c->packet_pnt += addlen;
          g_RecvBytes += addlen;
          c->receive_bytes = g_RecvBytes;

          if (c->packet_size == 0)  // Header 已完整接收
          {
            // 安全檢查，避免無效的封包大小導致後續問題
            if (c->packet.h.header.size < 0 ||
                c->packet.h.header.size > MAX_PACKET_SIZE) {
              JustMsg("HandleReading Error: Invalid packet header size (%d).",
                      c->packet.h.header.size);
              return -13;
            }

            if (c->packet.h.header.size == 0)  // 如果沒有資料部分
            {
              QueuePacket(&c->packet, 0);  // 直接將只有頭的封包放入佇列
              memset(&c->packet, 0, sizeof(t_packet));
              // 重置狀態機，準備接收下一個封包
              c->packet_pnt = 0;
              c->packet_size = 2;
              c->packet_status = 0;
            } else  // 如果有資料部分
            {
              c->packet_pnt = 0;
              c->packet_size = c->packet.h.header.size;  // 準備接收資料
              c->packet_status = 2;
            }
          } else if (c->packet_size < 0) {
            return -13;
          }
        } else if (c->packet_status == 2)  // 狀態2：讀取封包資料
        {
          addlen = SDLNet_TCP_Recv(c->socket, &c->packet.u.data[c->packet_pnt],
                                   c->packet_size);

          if (addlen <= 0) {
            JustMsg("Network Error or Disconnect (status 2): %s",
                    SDLNet_GetError());
            return -14;
          }

          c->packet_size -= addlen;
          c->packet_pnt += addlen;
          g_RecvBytes += addlen;
          c->receive_bytes = g_RecvBytes;

          if (c->packet_size == 0)  // 整個封包已接收完畢
          {
            QueuePacket(&c->packet, 0);
            memset(&c->packet, 0, sizeof(t_packet));
            // 重置狀態機，準備接收下一個封包
            c->packet_pnt = 0;
            c->packet_size = 2;
            c->packet_status = 0;
            
          } else if (c->packet_size < 0) {
            return -15;
          }
        }

        return 1;  // 本次讀取成功
}

//---------------------------------------------------------------------------------
int HandleWriting(t_connection* c)
{
	/*t_packet	packet;
	char		Buff[MAX_CLIENT_QUEUE_OUT_BUFFER];
	int			addlen, Size, BuffPnt;
	int			len;

	BuffPnt = 0;
	Size = sizeof(t_header);

	while (1)
	{
		DequeuePacket(&packet, 1);

		len = Size + data_size;

		memcpy(&Buff[BuffPnt], &len, 2);
		BuffPnt += 2;

		memcpy(&Buff[BuffPnt], packet.h.data, Size);
		BuffPnt += Size;

		memcpy(&Buff[BuffPnt], packet.u.data, data_size);
		BuffPnt += data_size;

		if (!connections.outbuf || BuffPnt > MAX_CLIENT_QUEUE_OUT_BUFFER - MAX_PACKET_SIZE)
		{
			break;
		}
	}


	if (BuffPnt <= 0)
	{
		return(-21);
	}

	Size = BuffPnt;

	BuffPnt = 0;
	while (1)
	{
		addlen = send(connections.socket, &Buff[BuffPnt], Size, 0);
		if (addlen < 0)
		{
			return(-22);
		}
		else if (addlen > 0)
		{
			BuffPnt += addlen;
			Size -= addlen;

			g_SendBytes += addlen;
		}

		if (Size < 0)
		{
			return(-23);
		}

		if (Size == 0)
		{
			break;
		}
	}

	return 1;*/

		// 1. 錯誤檢查
        if (!c || !c->socket) {
          return -22;
        }
        
        // 2. 數據組裝：使用本地緩衝區，在函式結束時自動銷毀
        c->sendBuffer.clear();
        
        // 3. 從隊列中取出封包並組裝到緩衝區
        /*
		t_packet packet;
        while (DequeuePacket(&packet, 1)) {
          int data_size = packet.h.header.size;
          int packet_full_size = sizeof(t_header) + data_size;
          int encoded_len = packet_full_size;

          // 檢查緩衝區是否還有足夠的空間
          if (c->sendBuffer.size() + 2 + encoded_len >
              _MAX_CLIENT_QUEUE_OUT_BUFFER) {
            // 如果緩衝區滿了，將這個封包放回隊列並退出組裝迴圈
            c->outbuf.push_front(packet);
            break;
          }

          // 調整緩衝區大小以容納新封包
          size_t old_size = c->sendBuffer.size();
          c->sendBuffer.resize(old_size + 2 + encoded_len);

          // 寫入長度前綴
          memcpy(&c->sendBuffer[old_size], &encoded_len, 2);

          // 寫入封包頭和數據
          memcpy(&c->sendBuffer[old_size + 2], &packet.h.header,
                 sizeof(t_header));
          if (data_size > 0) {
            memcpy(&c->sendBuffer[old_size + 2 + sizeof(t_header)],
                   packet.u.data,
                   data_size);
          }
        }
        */
		// 新的、更安全的 HandleWriting 迴圈
		while (!connections.outbuf.empty())
		{
			// 1. 先「窺視」一下佇列最前端的封包，但「不要」刪除它
			t_packet& packet = connections.outbuf.front();

			int data_size = packet.h.header.size;
			int packet_full_size = sizeof(t_header) + data_size;
			int encoded_len = packet_full_size;

			// 2. 檢查這個封包是否能裝得下
			if (c->sendBuffer.size() + 2 + encoded_len > _MAX_CLIENT_QUEUE_OUT_BUFFER) {
				// 裝不下了，直接跳出迴圈。因為我們沒動過佇列，所以封包會安全地留在原地。
				break;
			}

			// --- 能執行到這裡，代表空間足夠 ---
			// 3. 把封包內容複製到 sendBuffer
			if (g_bLogOnFlag) {
				en.Encrypt(packet.h.data, 2);
				en.Encrypt(packet.u.data, packet.h.header.size);
				packet.h.header.crc = en.GetCRCConvertChar();
			}
			// 調整緩衝區大小以容納新封包
			size_t old_size = c->sendBuffer.size();
			c->sendBuffer.resize(old_size + 2 + encoded_len);

			// 寫入長度前綴
			memcpy(&c->sendBuffer[old_size], &encoded_len, 2);

			// 寫入封包頭和數據
			memcpy(&c->sendBuffer[old_size + 2], &packet.h.header,
				sizeof(t_header));
			if (data_size > 0) {
				memcpy(&c->sendBuffer[old_size + 2 + sizeof(t_header)],
					packet.u.data,
					data_size);
			}
			// 4. 在確認封包「已經被處理」之後，才將它從佇列中真正地刪除
			connections.outbuf.pop_front();
		}

        // 4. 發送已組裝的數據
        if (c->sendBuffer.empty()) {
          return 1;  // 沒有數據可以發送，直接返回
        }
        int total_sent = 0;
        while (total_sent < c->sendBuffer.size()) {
          int bytes_remaining = c->sendBuffer.size() - total_sent;
          int addlen = SDLNet_TCP_Send(c->socket, c->sendBuffer.data() + total_sent, bytes_remaining);

          if (addlen <= 0) {
            // 發生錯誤或連線斷開
            SDL_Log("Network Send Error: %s", SDLNet_GetError());
            return -22;
          }

          total_sent += addlen;

		  g_SendBytes += addlen;
          c->send_bytes = g_SendBytes;
        }
        // 5. 返回，以便讓主迴圈繼續
        return 1;
}

//---------------------------------------------------------------------------------
// 010220 KHS
void SendAliveCheckRoutine(void)
{
	t_packet	packet;

	if ((global_time - alive_time) > 10000)		//10�ʸ��� �ѹ��� Check�Ѵ�. 
	{
		alive_time = global_time;

		packet.h.header.type = CMD_NONE;
		packet.h.header.size = 0;
		QueuePacket(&packet, 1);
	}
}

//------------------------------------------------------------------------------------------------------------------------------------	
//  ��  �� :  1�а� ���η� �����Ͱ� ���� Bytes���� ����Ѵ�. 
//  ��  �� :  ����.
//  ��  �� :  ����.
//------------------------------------------------------------------------------------------------------------------------------------	
void CheckBytesRoutine(void)
{
#ifdef _DEBUG // finito 060507
  return;
	if (!GetSysInfo(SI_GAME_MAKE_MODE)) return;

	static DWORD check_time;

	if (g_StartMenuOn) return;   // ���ӳ��� ��� �°� �ƴϸ�..

	if (g_packet_recv_send_checktime - check_time > 60)
	{
		check_time = g_packet_recv_send_checktime;

		FILE* fp = Fopen("networktraffic.txt", "at+");
		if (fp)
		{
			struct tm* today;
			time_t lTime;
			time(&lTime);
			today = localtime(&lTime);

			fprintf(fp, "%2d:%2d:%2d Recv:%7d Send:%7d   Total:%7d( %4d Kbps) \n", today->tm_mday + 1, today->tm_min, today->tm_sec,
				g_RecvBytes, g_SendBytes, g_RecvBytes + g_SendBytes, (g_RecvBytes, g_SendBytes) * 8 / 1024);

			fclose(fp);
		}

		g_RecvBytes = 0;
		g_SendBytes = 0;
	}
#endif
}

int	HandleRunning(t_connection* c, bool flag)
{
	/*static int ict = 0;
	struct timeval	tv = { 0, };
	fd_set rfds = { 0, }, wfds = { 0, }, efds = { 0, };
	int	ret = 0;

	if (!c->socket)
	{
		return(1);
	}

	global_time = SDL_GetTicks();

	FD_ZERO(&rfds);
	FD_ZERO(&wfds);
	FD_ZERO(&efds);

	FD_SET(c->socket, &rfds);

	if (c->outbuf)
	{
		FD_SET(c->socket, &wfds);
	}
	else
	{
		FD_SET(c->socket, &rfds);
	}

	FD_SET(c->socket, &efds);

	tv.tv_sec = 0;
	tv.tv_usec = 0;

	if (select(c->socket, &rfds, NULL, &efds, &tv) < 0)
	{
		return(1);
	}

	if (FD_ISSET(c->socket, &rfds))
	{
		ret = HandleReading(c);
		if (ret < 0)
		{
			return(ret);
		}
	}

	if (c->inbuf)
	{
		ret = HandleCommand(c);
		if (ret < 0)
		{
			return(ret);
		}
	}

	if (c->state >= CONNECT_OK)
	{

	}

	if (flag == true)
		if (FD_ISSET(c->socket, &wfds))
		{
			ret = HandleWriting(c);
			if (ret < 0)
			{
				return(ret);
			}
		}

	if (FD_ISSET(c->socket, &efds))
	{
		int err = WSAGetLastError();

		switch (err)
		{
		case WSAEWOULDBLOCK:	return 1;
			break;
		}

		return(0);
	}
#ifdef _DEBUG
	CheckBytesRoutine();
#endif
	SendAliveCheckRoutine();
	return 1;*/

	if (!c || !c->socket) { // 檢查連接和 socket 是否有效
		return 1;
	}
	global_time = SDL_GetTicks();

	// 1. 檢查網路活動
	int numReady = SDLNet_CheckSockets(c->socketSet, 0);
	if (numReady == -1) {
		g_DBGLog.Log(LOG_LV1, "SDLNet_CheckSockets Error: %s", SDLNet_GetError());
		ClearServer(c);
		return 0; // 返回 0 表示發生錯誤
	}

	// 2. 處理讀取：只有在偵測到網路活動時才進行
	if (numReady > 0 && SDLNet_SocketReady(c->socket)) {
		int ret = HandleReading(c);
		if (ret < 0) {
			ClearServer(c);
			return ret;
		}
	}

	// 3. 處理指令：只要輸入佇列有資料就處理
	if (!c->inbuf.empty()) {
		int ret = HandleCommand(c);
		if (ret < 0) {
			ClearServer(c);
			return ret;
		}
	}

	// 4. 處理寫入：只要輸出佇列有資料就嘗試發送，獨立於讀取
	if (flag == true && !c->outbuf.empty()) {
		int ret = HandleWriting(c);
		if (ret < 0) {
			ClearServer(c);
			return ret;
		}
	}

	// 5. 其他例行檢查
	SendAliveCheckRoutine();
	return 1;
}

//-------------------------------------------------------------------------------------
void Recv_CMD_SEALSTONE_WAR_START(void)
{
	char temp[FILENAME_MAX];

	switch (Hero->name_status.nation)
	{
	case 3:
	{	// ���̼��� 
		sprintf(temp, "%s", lan->OutputMessage(4, 8));//lsw
		break;
	}
	case 4:
	{	// ������.
		sprintf(temp, "%s", lan->OutputMessage(4, 9));//lsw
		break;
	}
	}

	AddCurrentStatusMessage(255, 200, 200, temp);

}

void Recv_CMD_SEALSTONE_WAR_LEFTTIME(DWORD t)
{
	int y, mo, d, h, mi, sec;
	CurrentGameDate(t, &y, &mo, &d, &h, &mi, &sec);

	char temp[FILENAME_MAX];

	if (h)			sprintf(temp, lan->OutputMessage(4, 11), h, mi, sec);//lsw
	else if (mi)	sprintf(temp, lan->OutputMessage(4, 12), mi, sec);
	else			sprintf(temp, lan->OutputMessage(4, 13), sec);

	AddCurrentStatusMessage(255, 255, 0, temp);
}

void Recv_CMD_WAR_START_BLOCK(void)
{
	AddCurrentStatusMessage(255, 200, 0, lan->OutputMessage(4, 14));//lsw
}

void Recv_CMD_SEALSTONE_RESULT_DEFEAT(void)
{
	AddCurrentStatusMessage(255, 200, 0, lan->OutputMessage(4, 15));//lsw
}

void Recv_CMD_SEALSTONE_RESULT_VICTORY(void)
{
	AddCurrentStatusMessage(255, 200, 0, lan->OutputMessage(4, 16));
}

void Recv_CMD_SEALSTONE_RESULT_DRWAW(void)
{
	AddCurrentStatusMessage(255, 200, 0, lan->OutputMessage(4, 17));//lsw
}

void Recv_CMD_SEALSTONE_STATUS(t_sealstone_client_status* tp)
{
	char temp[FILENAME_MAX];

	switch (tp->status)
	{
	case 0: sprintf(temp, lan->OutputMessage(4, 18), MapInfo[tp->mapnumber].mapname, tp->x, tp->y);			break;
	case 2: sprintf(temp, lan->OutputMessage(4, 19), MapInfo[tp->mapnumber].mapname, tp->x, tp->y);	break;//lsw
	}

	AddCurrentStatusMessage(255, 50, 50, temp);
}

void Recv_CMD_NATION2NATION_RELATION_RESULT(t_packet* p)
{
	switch (p->u.nation2nation_relation_result.why)
	{
	case NATION_RELATION_FAIL_:
		AddCurrentStatusMessage(255, 220, 220, lan->OutputMessage(4, 20));
		break;

	case NATION_RELATION_SAME_FAIL_:
		AddCurrentStatusMessage(255, 220, 220, lan->OutputMessage(4, 21));//lsw
		break;

	case NATION_RELATION_SUCCESS_:
		AddCurrentStatusMessage(255, 220, 220, lan->OutputMessage(4, 22));//lsw
		break;
	}
}

// Get Agent Server IP Address.		
int GetAgentServerIPAddress(t_connection* c)
{
	double		StartTime, Duration;

	if (SysInfo.enteragent)
	{
		//strcpy( g_GameInfo.agent_ip, SysInfo.agentip );
		return 1;
	}

	if (SysInfo.enterproxy) // ������ 
	{
		//strcpy( g_GameInfo.proxy_ip[0], SysInfo.proxyip );
		//strcpy( g_GameInfo.proxy_ip[1], SysInfo.proxyip );
	}

	//if( ConnectServerProxy( c, g_GameInfo.proxy_ip[0], g_GameInfo.proxy_port ) < 0 ) 
	{
		//if( ConnectServerProxy( c, g_GameInfo.proxy_ip[1], g_GameInfo.proxy_port ) < 0 ) 
		{
			// ���� Proxy Server�� ����.....
			return -21;
		}
	}

	StartTime = SDL_GetTicks();
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
			return(-1);
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

void RecvCMD_CONNECT_AGENT_SERVER_CONFORM(t_packet* packet)
{
	if (0 != stricmp(packet->u.Hwoa.rang.ConnectAgentServerConform.szUserID, IIDD.c_str()))
	{
		return;
	}

	const int iResult = ::ConnectLogin(&connections, IIDD, PPWW, 0);


	const int nKey = packet->u.Hwoa.rang.ConnectAgentServerConform.iKey;

	switch (nKey)
	{
	case 100:
	{//�±� ������ �ƴѰ�� 
		break;
	}
	case 12:
	{
		CallOkCancelMessageBox(MN_LOGIN, 0, 0, g_cMsg->GetTextMessage(2, 0));
		return;
	}
	case 13:
	{
		CallOkCancelMessageBox(MN_LOGIN, 0, 0, g_cMsg->GetTextMessage(2, 1));
		return;
	}
	case 14:
	{
		CallOkCancelMessageBox(MN_LOGIN, 0, 0, g_cMsg->GetTextMessage(2, 2));
		return;
	}
	case 15:
	{
		CallOkCancelMessageBox(MN_LOGIN, 0, 0, g_cMsg->GetTextMessage(2, 4));
		return;
	}
	case 16:
	{
		CallOkCancelMessageBox(MN_LOGIN, 0, 0, g_cMsg->GetTextMessage(2, 5));
		return;
	}
	case 17:
	{
		CallOkCancelMessageBox(MN_LOGIN, 0, 0, g_cMsg->GetTextMessage(2, 6));
		return;
	}
	case 18:
	case 19:
	{
		CallOkCancelMessageBox(MN_LOGIN, 0, 0, g_cMsg->GetTextMessage(2, 3));
		return;
	}
	case 61:
	case 62:
	case 63:
	{
		CallOkCancelMessageBox(MN_LOGIN, 0, 0, g_cMsg->GetTextMessage(2, 9));
		return;
	}
	case 64:
	{
		CallOkCancelMessageBox(MN_LOGIN, 0, 0, g_cMsg->GetTextMessage(2, 8));
		return;
	}

	default:
	{//�׿��� �޽����� ���
		if (0 <= nKey)
		{
			return;
		}
		int nTime = -nKey;
		AddCurrentStatusMessage(FONT_COLOR_SOLID_YELLOW, g_cMsg->GetTextMessage(2, 7), nKey);	//kr_msg.txt�� �߰�
		return;
	}
	}

	switch (iResult)
	{
	case 1:
	{
		SMenu[MN_SELECT_CHARACTER].bActive = TRUE;
		SMenu[MN_LOGIN].bActive = false;
		EWndMgr.CloseAllInputBox();
		SCharSource = LoadCh[character_active];
		if (0 <= g_nBillingType
			&& 5 >= g_nBillingType)
		{
			char temp[200] = { 0, };
			::sprintf(temp, lan->OutputMessage(5, 74), g_nBillingType);//lsw
			::CallOkCancelMessageBox(0, 0, 0, temp, 0);
		}
		break;
	}
	case -1:
	{
		::CallOkCancelMessageBox(MN_LOGIN, 0, 0, lan->OutputMessage(5, 71), 0);
		break;
	}
	case -3:
	{
		::CallOkCancelMessageBox(MN_LOGIN, 0, 0, lan->OutputMessage(5, 75), 0);
		break;
	}
	case -4:
	{
		::CallOkCancelMessageBox(MN_LOGIN, 0, 0, lan->OutputMessage(5, 76), 0);
		
		break;
	}
	case -5:
	{
		::CallOkCancelMessageBox(MN_LOGIN, 0, 0, lan->OutputMessage(5, 77), 0);
		break;
	}
	case -6:
	{
		::CallOkCancelMessageBox(MN_LOGIN, 0, 0, lan->OutputMessage(5, 78), 0);
		break;
	}
	case -7:
	{
		::CallOkCancelMessageBox(MN_LOGIN, 0, 0, lan->OutputMessage(5, 79), 0);
		break;
	}
	case -8:
	{
		::CallOkCancelMessageBox(MN_LOGIN, 0, 0, lan->OutputMessage(5, 80), 0);
		break;
	}
	case -9:
	{
		::CallOkCancelMessageBox(MN_LOGIN, 0, 0, lan->OutputMessage(5, 84), 0);
		break;
	}
	case -10:
	{
		::CallOkCancelMessageBox(MN_LOGIN, 0, 0, lan->OutputMessage(5, 85), 0);
		break;
	}
	case -11:
	{
		::CallOkCancelMessageBox(MN_LOGIN, 0, 0, lan->OutputMessage(3, 205), 0);
		break;
	}

	case -18:
	{
		::CallOkCancelMessageBox(MN_LOGIN, 0, 0, lan->OutputMessage(5, 101), 0);
		break;
	}
	case -12:
	{
		int num = 0;

		switch (g_client_login_fail_reason.dwType)
		{
		case LM_FAIL_DB: num = 90; break;
		case LM_FAIL_POINT:	num = 91; break;
		case LM_FAIL_BAN: num = 92; break;
		default: num = 89; break;
		}

		::CallOkCancelMessageBox(MN_LOGIN, 0, 0, kein_GetMenuString(num), 0);
		break;
	}
	case -13:
	{//ĳ���� �����߿� �ֽ��ϴ�. ����Ŀ� �����Ͽ� �ֽʽÿ�.
		::CallOkCancelMessageBox(MN_LOGIN, 0, 0, lan->OutputMessage(5, 90), 0);
		break;
	}
	case -17:
	{ // 030929 kyo //thai only user limied age
		::CallOkCancelMessageBox(MN_LOGIN, 0, 0, lan->OutputMessage(5, 130), 0);
		break;
	}
	default:
	{
		::CallOkCancelMessageBox(MN_LOGIN, 0, 0, lan->OutputMessage(5, 86), 0);
		break;
	}//lsw
	}

}
