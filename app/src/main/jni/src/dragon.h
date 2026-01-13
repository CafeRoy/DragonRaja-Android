#ifndef	__DRAGON_H__
#define	__DRAGON_H__

//#include <windows.h>
//#include <time.h>

//#include "MenuDef.h"
#include "network.h"
#include "Item.h"
#include <numeric> // 為了使用 std::accumulate
#include "SDLRenderer.h"

#define	CM_COMMAND			0
#define	CM_MESSAGE			1

#define CONNECT_REFUSE		10101010
////////////// 0427 lkh �߰� /////////////////
#define	DISPLAY_EARTHQUAKE	10
#define	DISPLAY_WAVEEARTH	1

extern HWND					g_hwndMain;
extern HINSTANCE			g_hInstance;

extern std::string			g_szCWD;

extern BOOL					g_bIsActive;
extern BOOL					g_bCommandMode;

extern POINT				g_pointMouse;
extern int					g_pointMouseX, g_pointMouseY;
extern int					g_nLButtonState, g_nRButtonState;
extern int					g_nLDButtonState, g_nRDButtonState;
extern int					g_nOldLButtonState, g_nOldRButtonState;

extern int					LButtonDownIng, RButtonDownIng;
//extern int					LButtonDown, RButtonDown;
extern DWORD				g_CurrentTime;
extern DWORD				g_ClientTime;	// GameProc()���� ��������� Check �Ѵ�. 
extern DWORD				g_ServerClientTimeGap;

extern DWORD				g_curr_time;  //  �������������ִ� ������ �ð�. �ʴ���..
extern DWORD				g_packet_recv_send_checktime;
extern DWORD				g_Current_Server_Time;
////////////////////// 0128 lkh /////////////////////////
extern bool					g_Volume_Off;	//������ �Ҹ��� ���ϱ�?

extern int					g_Operator_Function;

// ----------------------- khs 0804

extern char				  *g_DestBackBuf;
extern int					Mox, Moy;
extern SafeValue<int>					Mapx;
extern SafeValue<int>					Mapy;
extern LPCHARACTER			Hero;

//-----------------------------------

extern int					QuitFlag;

extern DWORD				g_RecvBytes, g_SendBytes;

extern bool					MouseCursorOnOff;

extern bool				g_Inside;

extern int					NowLoading;


extern std::string IIDD;
extern std::string PPWW;

extern int	CheckSumError;	

extern int recvAttackResultCount;
extern FILE *keyplayfp;
extern FILE *keykeyplayfp;

extern bool		g_bIsChristmas;

struct BuffTimer
{
	int magicid;
	int duration;

	BuffTimer(int id, int dur) : magicid(id), duration(dur) {}
};

enum GameState {
	STATE_MENU,
	STATE_STARTING_GAME,
	STATE_IN_GAME,
	STATE_CLEANING_UP_TO_MENU,
	STATE_EXIT
};

typedef struct	tagGAMEINFO
{
	std::string	proxy_ip;
	int			proxy_port;
	std::string	agent_ip;
	int			agent_port;
    std::string configFilePath;
    std::string RootPath;
    int			g_MyCode;
	int			map_port; // �ʼ����� port��ȣ Port�� �˸� MapName�� �˼� �ִ�. 
	unsigned int	AutoLogin;
	time_t		timeGame;
	int			exp_event;
	BYTE		cWindVelocity;
	BYTE		cHumidity;

	POINT		pointMouse, pointFuzz;
	int			nLButtonState, nRButtonState;
	int			nOldLButtonState, nOldRButtonState;
	int			nSensitivity;

	
	DWORD		dwFrameRate;
	DWORD		dwFrameCount, dwFrameCount0;
	DWORD		dwFrameTime, dwFrameTime0;

	RECT		rectView, rectScreen;

	LPCHARACTER  lpcharacter;
	LPCHARACTER  lpoldcharacter;

	int			nSelectedSpriteType;
	LPVOID		lpvSelectedSprite;

	bool	     mouseClick = false;
	int			 PopupFaceCount;
	int			 checkcount; // ���콺�� �浹�ϸ� Count�� 5���̵Ǹ� ȭ�鿡 �̸��� �����ش�. 
	
	int			displayOption;		
	int			displayOptionDelay;

	int			preventSelectYilse=1;

    bool g_bIsManualScrolling = false; // 預設速度

	std::map<int, BuffTimer> m_BuffTimer;

    std::map<std::string, int> err_file;
	// 我們儲存最近 5 次的 Ping 值來計算平均
	static const int BUFFER_SIZE = 5;
	std::vector<float> m_ping_history;
	int m_current_index = 0;

	void ClearBuffTimer(bool clear_pasivecombat)
	{
		if (!clear_pasivecombat)
		{
			for (auto& itr : m_BuffTimer)
			{
				itr.second.duration--;
			}
		}
		const int LIGHTNING_BOOM = 215; // 這是閃電爆的魔法ID
		const int WIND_EXTREME = 250;   // 這是究極風的魔法ID
		// 遍歷 map 並安全地刪除元素
		auto it = m_BuffTimer.begin();
		while (it != m_BuffTimer.end()) {
			if (it->second.duration <= 0 || 
				(clear_pasivecombat && it->first >= LIGHTNING_BOOM && it->first <= WIND_EXTREME)) 
			{
				// 如果滿足刪除條件，erase() 會返回指向下一個元素的迭代器
				it = m_BuffTimer.erase(it);
			}
			else {
				// 如果不滿足刪除條件，移動到下一個元素
				++it;
			}
		}
	}

	void InsertBufferTimer(int nMagic, int Dur)
	{
		auto it = m_BuffTimer.find(nMagic);
		if (it != m_BuffTimer.end())
		{
			it->second.duration = Dur;
		}
		else
		{
			m_BuffTimer.emplace(nMagic, BuffTimer(nMagic, Dur));
		}
	}

	void InitPingList()
	{
		m_ping_history.assign(BUFFER_SIZE, 30.0f);
        AutoLogin = 85;
	}

	void PutPing(float new_ping)
	{
		// 將新的 Ping 值，加入到我們的歷史紀錄中
		m_ping_history[m_current_index] = new_ping;

		// 更新索引，讓它在 0 到 BUFFER_SIZE-1 之間循環
		m_current_index = (m_current_index + 1) % BUFFER_SIZE;
	}

	std::string  GetAveragePing()
	{
		// 計算歷史紀錄中所有 Ping 值的總和
		float sum = std::accumulate(m_ping_history.begin(), m_ping_history.end(), 0.0f);

		// 返回平均值
		int avg =  static_cast<int>(sum / BUFFER_SIZE);

		return std::to_string(avg);
	}
	
} GAMEINFO, *LPGAMEINFO;

extern GAMEINFO		g_GameInfo;


extern const char*	GetCurrentWorkingDirectory( void );
extern void		SetInfoFile( void );
extern void		WriteInfoFile(void);
extern char*	EatFrontWhiteChar( char* );
extern char*	EatRearWhiteChar( char* );
extern void     EatRearWhiteChar(std::string& s);
extern BOOL		ShowErrorMessage(std::string msg);

enum eExitAppType
{
	EA_NORMAL = 0,//�������� ����.
	EA_CHECK_ACCEL= 1,
	EA_BEFORE_EXE= 2,
	EA_BEFORE_EXE2= 3,
	EA_LOAD_CHAR_SPR_DATA_SUB= 4,
	EA_LOAD_RIDER_SPR_DATA_SUB= 5,
	EA_LOAD_RIDER_BAND_SPR_DATA_SUB= 6,
	EA_LOAD_RIDER_MANTLE_SPR_DATA_SUB= 7,
	EA_LOAD_WEAPON_SPR_DATA= 8,
	EA_LOAD_GUARD_SPR_DATA= 9,
	EA_LOAD_LOACL_WAR_INFO_SPR_DATA= 10,
	EA_LOAD_SPRITE_INFO= 11,
	EA_LOAD_RIDER_GRADE_SPR_DATA_SUB= 12,
	EA_LOAD_HORSE_SPR_DATA_SUB= 13,
	EA_LOAD_HORSE_SADDEL_SPR_DATA_SUB= 14,
	EA_LOAD_HORSE_GRADE_SPR_DATA_SUB= 15,
	EA_LOAD_LOACL_CHAIR_SPR_DATA= 16,
	EA_LOAD_LOACL_SQUAD_SPR_DATA= 17,
	EA_RECV_CONNECT_INFO= 18,
	EA_CPU_CLOCK_ERROR = 19,
	EA_CRACK_THREAD_FIND_CRACK = 20,
	EA_LANGUAGE_ERROR = 21,
	EA_JAPAN_LOGIN_ERROR = 22,
	EA_CRACK_DAT_FILE	=23,
};

extern void		ExitApplication( const eExitAppType eEAType );

extern char*	EatRearWhiteChar( char* pStr );

enum eLoadingGameDataStep
{
	LGDS_FIRST = 0,
	LGDS_STEP1 = 1,
	LGDS_STEP2 = 2,
	LGDS_STEP3 = 3,
	LGDS_STEP4 = 4,
	LGDS_STEP5 = 5,
	LGDS_STEP6 = 6,
	LGDS_STEP7 = 7,
	LGDS_STEP8 = 8,
	LGDS_STEP9 = 9,
	LGDS_STEP10 = 10,
	LGDS_STEP11 = 11,
	LGDS_STEP12 = 12,
	LGDS_END	= 13,
};
extern int		LoadingGameData();
// lsw
extern bool IsChatBoxActive();
extern void SetChatMode(const int iMode, bool bIsForce = false);
extern void CharToUpper(char* str);
enum eCHATTARGETNORMAL
{
	CHAT_TARGET_NORMAL		= 0,//���� ä�� ���
	CHAT_TARGET_GUILD		= 1,//��� ä�� ���
	CHAT_TARGET_PARTY		= 2,// Finito for use with new pary chat type
	SELECT_WISPER_MODE		= 3,//�Ӹ���� ���� �ϱ�
	CHAT_TARGET_WISPER1		= 4,//�Ӹ���� ���� �ϱ�	//�ʵ�ѹ��� ��ġ
	CHAT_TARGET_WISPER2		= 5,//�Ӹ���� ���� �ϱ�
	CHAT_TARGET_WISPER3		= 6,//�Ӹ���� ���� �ϱ�
	CHAT_TARGET_WISPER4		= 7,//�Ӹ���� ���� �ϱ�
	CHAT_TARGET_WISPER5		= 8,//�Ӹ���� ���� �ϱ�
	CHAT_TARGET_WISPER6		= 9,//�Ӹ���� ���� �ϱ�
	CHAT_TARGET_WISPER7		= 10,//�Ӹ���� ���� �ϱ�
	CHAT_TARGET_WISPER8		= 11,//�Ӹ���� ���� �ϱ�
	CHAT_TARGET_WISPER9		= 12,//�Ӹ���� ���� �ϱ�
	CHAT_TARGET_WISPER10	= 13,//�Ӹ���� ���� �ϱ�
	
	
};
const int CHAT_TARGET_WISPER_START	= CHAT_TARGET_WISPER1;//�Ӹ���� ���� �ϱ� ������
const int CHAT_TARGET_WISPER_END	= CHAT_TARGET_WISPER5;//�Ӹ���� ���� �ϱ� ����

#endif	// __DRAGON_H__


