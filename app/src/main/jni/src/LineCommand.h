#if !defined(AFX_LINECOMMAND_H__589944BD_DCE5_4B05_9D6B_82C08591AC88__INCLUDED_)
#define AFX_LINECOMMAND_H__589944BD_DCE5_4B05_9D6B_82C08591AC88__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
	
class CSmallTipCount
{
public:
	char m_szKey[50];		// 020925 YGI
	int	m_nPage;
	CSmallTipCount *next;

public:
	CSmallTipCount() { next = NULL; m_nPage = 0; m_szKey[0] = 0; }
	~CSmallTipCount() { SAFE_DELETE(next); }

	int AddData( int number, char *str );
	int GetPage(const char *key );
};

enum LineCommandIndex {
  LC_NO_COMMAND = 0,
  LC_GO_NORTH = 1,
  LC_GO_WEST = 2,
  LC_GO_EAST = 3,
  LC_GO_SOUTH = 4,
  LC_EAT_ITEM = 5,
  LC_NOT_GO = 6,
  LC_CHAT_WHISPER = 7,
  LC_TOTAL_CONNECTIONS = 8,
  LC_CHAT_GUILD = 9,
  LC_TOTAL_MAP_CONNECTIONS = 10,
  LC_CHAT_PARTY = 11,
  LC_MAKE_PARTY = 12,
  LC_DEFAULT_WALK = 13,
  LC_DEFAULT_RUN = 14,
  LC_GUILD_PARTY = 15,// text is GP
  LC_NATION_PARTY = 16,// Text is NP
  LC_ENTERHOUSE = 17,
  LC_TAME_STOP = 18,
  LC_TAME_ATTACK = 19,
  LC_TAME_FOLLOWME = 20,
  LC_TAME_NAME = 21,
  LC_GO_STOP = 22,
  LC_GO_RANDOM = 23,
  LC_OPEN_TIMEMENU = 24,
  LC_CLOSE_TIMEMENU = 25,
  LC_CLOSE_ALL = 26,
  LC_EAR_MESSAGE_REFUSE = 27,
  LC_EAR_MESSAGE_OK = 28,
  LC_REPRINT_EAR_MESSAGE = 29,
  LC_EAR_MESSAGE_REFUSE_WHO = 30,
  LC_GREETING = 40,
  LC_EXPLAIN_OPEN = 41,
  LC_EXPLAIN_CLOSE = 42,
  LC_REQ_MAN_TO_MAN = 43,
  LC_RESET_MAGIC_ARRAY = 44,
  LC_MSG_ALL_NATION = 45,
  LC_GUILD_EXIT = 46,
  LC_GUILD_JOIN = 47,
  LC_GUILD_ALL_MSG = 48,
  LC_GUILD_CHANGE_EACH_LEVEL = 49,
  LC_GUILD_CMD_INFO = 50,
  LC_CHANGE_JOB = 51,
  LC_STOP_WAR = 52,
  LC_SCAN_TARGET_VYSEUS = 53,
  LC_SCAN_TARGET_ZYPERN = 54,
  LC_KILL_CHARACTER = 55,
  LC_CENTER_HERO = 56,
  LC_SHIFT = 57,
  LC_EXP = 58,
  LC_OPEN_DISPLAY_TAC_EXP = 59,
  LC_CLOSE_DISPLAY_TAC_EXP = 60,
  LC_LIGHTOFRESCUE = 61,
  LC_AUTOLEVELUP_CLOSE = 62,
  LC_AUTOLEVELUP_OPEN = 63,
  LC_ANNONCE_ON = 64,
  LC_ANNONCE_OFF = 65,
  LC_EAR_COMMAND_INFO = 66,
  LC_FIGHTMAP_ALL_CHAT = 68,
  LC_SMALL_TIPS = 69,
  LC_EMOTION = 70,
  LC_EXIT_GUILD = 71,
  LC_VIEW_NK = 72,
  LC_SQUAD_LEADER = 73,
  LC_SQUAD = 74,
  LC_SQUAD_MEMBER = 75,
  LC_SQUAD_MEMBER_TO_MEMBER = 76,
  LC_WAR_MENU_OPEN = 77,
  LC_WAR_MENU_NOTICE_OPEN = 78,
  LC_GUILD_MAIL = 79,
  LC_SQUAD_CHAT = 80,
  LC_GUILD_CHAT = 81,
  LC_COMMANDER_INFO = 82,
  LC_SQUAD_INFO = 83,
  LC_GUILD_CHAT_ON = 84,
  LC_GUILD_CHAT_OFF = 85,
  LC_RESET_ABILITY = 86,
  LC_WAR_GIVE_LIFE = 87,
  LC_PARTY_ALL_FREE = 88,
  LC_SEND_GUILD_MAIL = 89,
  LC_SEND_MAIL = 90,
  LC_SHOW_ME_DUAL = 91,
  LC_REPORTER_MODE = 92,
  LC_FRIEND_MENU = 93,
  LC_WAR_GIVE_LIFE2 = 94,
  LC_SCAN_TARGET_YILSE = 95,
  LC_GETCOLOSSUS_INFO = 96,
  LC_INVITE_COLOSSUS = 97,
  LC_JOIN_COLOSSUS = 98,
  LC_PK_ON_OFF = 99,
  LC_COMBAT_SKILL_ON_OFF = 100,
  LC_SET_NORMAL_CHAT_MODE = 109,
  LC_LOCALWAR_CHAT = 111,
  LC_QUESTMODE = 112,
  LC_WEATHER_ON = 113,
  LC_WEATHER_OFF = 114,
  LC_COMBAT_RESET = 115,
  LC_MAGIC_DISPLAY_ON = 116,
  LC_MAGIC_DISPLAY_OFF = 117,
  LC_CHATLOG = 118,
  LC_REPRINT_GUILD_CHAT = 119,
  LC_REPRINT_GUILD_BBS = 120,
  LC_TRADE = 121,
  LC_CHECK_DUAL_FAME = 122,
  LC_WHAT_QUEST_IN_MAP = 123,
  LC_MERCHANT_BBS = 124,
  LC_MERCHANT_EXCHANGE = 125,
  LC_EVENT_MOVE_MAP = 126,
  LC_EMOTI_LOVE = 127,
  LC_EMOTI_SMILE = 128,
  LC_EMOTI_HAPPY = 129,
  LC_EMOTI_SHY = 130,
  LC_EMOTI_CRY = 131,
  LC_EMOTI_GOOD = 132,
  LC_EMOTI_SORRY = 133,
  LC_EMOTI_ANGER = 134,
  LC_EMOTI_THANKS = 135,
  LC_EMOTI_TELL = 136,
  LC_GAME_SCISSOR = 137,
  LC_GAME_ROCK = 138,
  LC_GAME_PAPER = 139,
  LC_GAME_GO = 140,
  LC_GAME_HELP = 141,
  LC_GAME_PARTY = 142,
  LC_GAME_HEAL = 143,
  LC_GAME_TRANSACTION = 144,
  LC_GAME_BEG = 145,
  LC_GAME_CONGRAGULATION = 146,
  LC_BLOCK = 147,
  LC_BBS_ON = 148,
  LC_BBS_OFF = 149,
  LC_RAID_START = 150,
  LC_RAID_END = 151,
  LC_GET_SMP = 152,
  LC_REPRINT_PARTY_CHAT = 153,
  LC_DENY_DUEL_REQ = 154,
  LC_ALLOW_DUEL_REQ = 155,
  LC_TOGGLE_DROP = 156,
  LC_TAME_AUTO_ATTACK = 157,
  LC_TAME_AUTO_DEFENCE = 158,
  LC_TAME_ALL_FOLLOW = 159,
  LC_TAME_ALL_STOP = 160,
  LC_REPORTER_TELEPORT = 161,
  LC_HIDE = 162,
  LC_HONOR = 163,
  LC_EXP_RESET = 164,
  LC_NATION_CHAT = 165,
  LC_WORLDBBS_CHAT = 166,
  LC_USER_IMPRISON = 167,
  LC_USER_RELEASE = 168,
  LC_USER_BAN = 169,
};

#define TAME_FAIL_CHANGE_NAME		0 // �̸��ٲٱ⸦  ����. 
#define TAME_NO_CHAR				1 // �׷��̸����� ����� ������ �����ϴ�.  
#define TAME_NO_NAME_CHAR			2 // �׷� �̸��� ���� ����� �����ϴ�. 
#define TAME_FOLLOW					3 // ���󰩴ϴ�. 
#define TAME_STOP					4 // ���� ���ϴ�. 
#define TAME_ATTACK					5 // �����մϴ�. 
#define TAME_SUCCESS_CHANGE_NAME	6 // �̸��� �ٲپ����ϴ�. 
#define MAX_LINE_COMMAND_ 200	

extern std::map<std::string, LineCommandIndex> g_LineCommandMap;

/////////////////////////////////////////////////////////////////////////////////////
extern char	Lc_ChatType;
extern char	Lc_TempChatName[ 31];
extern char	Lc_DefaultMoveType;
extern char GreetingStr[ 100];
extern int IsLineCommand(const char *message, int nsize );
extern void SendTotalMapConnections( void );
extern void SendTotalConnections( void );
extern void RecvTotalMapConnections( int no);
extern void RecvTotalConnections( int no);
extern void RecvTameCommandResult( int result );
extern void RecvTameNameChange( t_tame_name_change *p );
extern void RecvHostName( t_tame_host_name *p );
extern void RecvHostEnd(  tame_end *p );

extern void SendBBS( char *bbs);		// �������� ����...
extern void SendBBSAll(const char *bbs);

extern void RecvBbs_quest( char *bbs );					// 0810 lhs ����Ʈ ����

extern void SendChatDataGlobalFightMap(const char *lpStr );
extern void CallViewSmallTips(std::string &txt);
extern void SendExitGuild( );
extern void ViewNk(std::string &name);
extern bool SendChatGuild(std::string msg);
extern void RecvGuildChatData( t_packet *p );
extern void CheckShowMeDual();
extern void SendJoinColossusMap();
extern void TradeONOFF(const char *t );//020808 YGI 
extern int LoadSmallTipsPage();
#endif //AFX_LINECOMMAND_H__589944BD_DCE5_4B05_9D6B_82C08591AC88__INCLUDED_