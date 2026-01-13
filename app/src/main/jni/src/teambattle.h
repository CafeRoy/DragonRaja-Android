#ifndef __teambattle_h__
#define __teambattle_h__
#include <unordered_map>
class CBattleTeam 
{
public : 
	int count;
	k_team_member_info_unit member[MAX_TEAM_MEMBER];

public :
	CBattleTeam();
	void ReSetClass();
	int AddMember( k_team_member_info_unit *info );
	int DeleteMember( char *name );
};

class CBattleZone
{
public : 
	CBattleTeam		m_team[ATI_MAX_TEAM];			// �ִ� 4��

public : 
	CBattleZone();
	void ReSetClass();
	int SetAllInfo( k_team_member_info_all *info );
	int AddMember( int team, k_team_member_info_unit *info );
	int GetCount( int team );
	int IsExist( char *name );
};


//////////////////////////////////////////////////////////////

#define MAX_FIGHT_MAP_CH	20

struct WINLOSEPOINT
{
	DWORD win			: 13;
	DWORD defeat		: 12;
	DWORD disconnect	: 7 ;
};

class CFightMapCharInfo
{
public :
	DWORD				m_TotalId;
	WINLOSEPOINT		m_WinDefeat;
	DWORD				m_dwLadderScore;
	int					type;
	char				m_szCallLevel[20];
	WORD				m_wColor;


	CFightMapCharInfo	*next;

public :
	CFightMapCharInfo() : m_TotalId(0), m_WinDefeat({}), m_dwLadderScore(0), type(0), m_wColor(0), next(0) { memset(m_szCallLevel, 0, sizeof(m_szCallLevel)); }
	~CFightMapCharInfo() { if( next ) delete next; }

	// ���⼭ type�� ���� ������ �ޱ� �� ���� �ĸ� ��Ÿ����.
	void AddCharInfo( DWORD total_id, DWORD win_defeat, DWORD ladder, int level, int Class, int type, int dual_cls, int cls_step );
	CFightMapCharInfo *GetFightMapCharInfo( DWORD total_id );
	void DeleteAll() { m_TotalId = 0; delete next; next = 0; }


	
};

////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////
// ��� ���� �޴� ȣ��� ����ϴ� define
#define GE_REGIST_IMSI_GUILD		1		// �ӽ� ��� ��� ����
#define GE_REGIST_GUILD_MARK		2		// ��� ��ũ ��Ͽ� ���� ����
#define GE_REGIST_GUILD_MENUAL		3		// ��� ��Ͽ� ���� �������� ���� �޴�
#define GE_REGIST_GUILD				4		// ���� �۵� ���
#define GM_INPUT_SUB_MASTER			5		// �α渶 �Է¶�
#define GM_CHANGE_INFO_MENUAL		6		// ��� ���� ���� ����




class CGuildBasic
{
public :
	int  m_bInfo;			// ������ �� ����� ������ ��û�� ���� �ִ� ������ �÷���
	int	 m_bActive;			// ���� ����̴� �ƴϴ�.
	int	 m_nCode;				// ��� �ڵ�
	char m_szName[20];			// ��� ��
	char m_szGuldMasterName[20];			// �渶 �̸�
	char m_szSubGuldMasterName[20];			// ���� �渶 �̸�
	char m_aDegree[5][20];		// ��å	1: �渶 ~ 0 : �Ϲ�

public :
	CGuildBasic()
	{
		InitData();
	}
	char *CallGuildDegreeName( int nDegree )
	{
		if( nDegree <0 || nDegree >= 5 ) return NULL;
		return m_aDegree[nDegree];
	}
	int InitData();
};

class CGuild : public CGuildBasic		// text�� �ʿ��� �͸� �ε��ؿö�.. ���� bin���� �ε� �� ��쿣 �ٸ���.....
{
public :
	int m_nLevel;				// ��� ���

	int	first;
	int mark_num;
	int make;
	int info_num;

	int m_nAllianceGuildCode;	// ���� ��� �ڵ�	
	int m_nHostilityGuildCode;	// ���� ��� �ڵ�

	int m_nRader;				// ���� ����
	int m_nWin;				// ����� �̱� ��
	int m_nLoss;				// ��������� �� �� 

	bool save_flag;
	Spr m_Spr;					// ��� ��ũ
	Spr m_Spr_save;					// ��� ��ũ

	char *m_szExplain;			// ��� ����

public : 
	CGuild(){ InitData(); }		// �ʱ�ȭ
	~CGuild(){ if( m_szExplain ) delete m_szExplain; }
	int InitData();
};

extern std::unordered_map<int, CGuild> g_GuildMgr;  // CSD-030324

// ��� Ŭ������ ���õ� �Լ���
extern char *CallGuildDegreeName( int guild_num, int degree_num );		// ��� ��å�� ����...
extern char *GetGuildName( int guild_num );
extern int GetGuildCode( LPCHARACTER ch );

///////////////////////////////////////////////////////////////////////////////////////////////////
// ��� �Ұ� �Ҷ� ����� ������ ��� ����

class CGuildIntroductionGuild
{
public : 
	int x;
	BYTE line;
	BYTE page;
	BYTE size;

	RECT rect;
	int	 m_nCode;				// ��� �ڵ�
	char m_szName[20];			// ��� ��
};

class CGuildIntroduction
{	//< CSD-030324
public :
	int m_Nation;
	int m_Count;
	std::unordered_map<int, CGuildIntroductionGuild> m_Guild;
public :
	CGuildIntroduction() : m_Nation(0), m_Count(0)
	{
	}
};	//> CSD-030324

//< soto-030312
#define		MAX_GUARDIAN_GUILD	3

#define		GUARDIAN_GUILD_1ST_TEST	379		//���̼���(�������� �׳�)		�渶 : DR������ 030318����
#define		GUARDIAN_GUILD_2ST_TEST	197		//�Ͻ�(��ɻ����)				�渶 : ������ ���ü� 030318����
#define		GUARDIAN_GUILD_3ST_TEST	241		//������(Glory of Streak)		�渶 : �̽�����

#define		GUARDIAN_GUILD_1ST	27		//���̼��� : �غ�					�渶 : ���Ǵ����3   	GuildCode : 27
#define		GUARDIAN_GUILD_2ST	343		//������ : �����ݿ���				�渶 : �ΰ�����  	GuildCode : 343
#define		GUARDIAN_GUILD_3ST	9		//�Ͻ� : ����Ƽ��  ����			�渶 : ����Ż		GuildCode : 9


// ��Ű�� ���. // ������ �ٲ�� ���� 1����常 �ٲ��.
class CGuardianGuild
{
private:
	int m_nGuildCount; // ���� ��Ű�� ����� ���� ��Ÿ����.
	int	m_n1stGuildCode; // 1���� ��� �ڵ带 �˾Ƴ���.
	vector<int>m_vtGuildList;
	
	// �����ڵ带 �˻����� �ʰ� ��� �ڵ带 ����Ѵ�.(�ߺ��� ������ �ִ�)
	int	ForcingAddGuild(int nGuildCode)
	{		
		m_vtGuildList.push_back(nGuildCode);
		return ++m_nGuildCount;
	}
public:
	CGuardianGuild() //������
	{ 
		m_vtGuildList.reserve(MAX_GUARDIAN_GUILD);
		m_nGuildCount = 0;
		m_n1stGuildCode = -1; 
	} // ������.
	
	bool IsGuardianGuild(int nGuildCode)// �� ����ڵ尡 ��Ű�� ����� �ڵ��̳�?
	{
		for(int i = 0;i < m_nGuildCount;i++)
		{
			if(m_vtGuildList[i] == nGuildCode) return true;
		}
		return false;
	}	
	
	int AddGuild(int nGuildCode)//�ߺ����� �ʰ� �߰� ��Ų��.
	{
		if(!IsGuardianGuild(nGuildCode))
		{
			m_nGuildCount = ForcingAddGuild(nGuildCode);
		}

		return m_nGuildCount;
	}
	
	int FirstGuild()//1�� ����ڵ带 �˾Ƴ���.// �������� -1�� ����.
	{
		if(!m_nGuildCount)return -1;

		return m_n1stGuildCode;
	}

	int GetCount()
	{
		return m_vtGuildList.size();
	}

	int	GetGuildCode(int nIndex)
	{
		if(nIndex < m_nGuildCount)
			return m_vtGuildList[nIndex];
		return -1;
	}

	// �ϵ��� ���� �ϳ��� �ƴ϶�.. ���������� �ϳ��� �ٲ����. ��� 3����.
	bool	Is1StGuild(int nGuildCode){ return IsGuardianGuild(nGuildCode); }// soto-030318

	void Set1stGuild(int nGuildCode)//1���� ��带 ����Ѵ�.
	{
		if(!m_nGuildCount)//���� ��尡 �ƹ��� ��� �Ǿ� ���� ������.
		{
			ForcingAddGuild(nGuildCode);
			m_n1stGuildCode = nGuildCode;
		}
		else // �ϳ� �̻��� ��尡 ��ϵǾ� ������.
		{
			if(IsGuardianGuild(nGuildCode)) // ����Ʈ�� ��ϵ� �ڵ��ΰ�?
			{
				m_n1stGuildCode = nGuildCode;
			}
			else
			{
				ForcingAddGuild(nGuildCode);
				m_n1stGuildCode = nGuildCode;
			}
		}
	}

	void ClearGuild()
	{ 
		m_vtGuildList.clear();
		m_n1stGuildCode = -1;
		m_nGuildCount = 0;
	}
};


extern bool	CheckGuardianGuildWear();// �þ��� ������� ����üũ �Ͽ� ��庹���� ���� ������.


#define		GUILD_WEAR01		102//��Ű�� ��庹

extern CGuardianGuild	g_CGuardianGuild; // ��Ű�� ��� ���� ���� Ŭ����. // soto
extern CBattleZone	g_BattleZone;
extern void RecvAllTeamInfo( t_packet *p );
extern void SendAddMeOfTeamBattle( int team );
extern void RecvAddMemberInfo( t_packet *p );

extern bool	g_FightMapStart;
extern void StartTembattleWaitMenu(int nGame); // CSD-030506
extern void ViewFightMapCharInfo( char *name, DWORD total_id, int x, int y );
extern void ChangeEquipForFightMapStart();
extern void RecvFightRemainTime( t_packet *p );
extern void FighMapReset();
extern void ResetMagicFlag( LPCHARACTER ch );
extern int g_save_load_flag;
extern void LoadGuildMarkEx();
extern CGuildIntroduction *g_Guild_Introduction;


// �� ������ ������� ã�� �ߴ°�?
#define STONE_SCORE_FIRST	IA2_NOTMOVE		//  1: �ʱ� �϶� 
#define STONE_SCORE_2	IA2_OPENED			//	2: �ڸ�
#define STONE_SCORE_1	IA2_BROKEN			// ù°�ڸ�


#define GetAttr2( attr2, a )		((attr2) & (a))
#define SetAttr2( attr2, b )		{(attr2) |=(b);}
extern int GetTeamByStone( DWORD attr );
extern WORD GetRGBByTeam( int team, int &r, int &g, int &b );

extern int GetArenaObserverTarget();
extern int GetArenaBettingTarget();
extern void SetArenaObserverTarget(const int iTeamType);
extern void SetArenaBettingTarget(const int iTeamType);
extern void SendObserverTeamSelect();
extern void SendMyArenaBettingMoney(const DWORD dwMoney);

extern DWORD GetTeamBettingMoney(const int iTeamType);//���� �ɸ� ��
extern float GetTeamWinRate(const int iTeamType);//���� �ɸ� ��
extern DWORD GetMyBettingMoney();//���ñ�
extern DWORD GetMyBettingResult();//����
extern DWORD GetAreanWaitTime();
extern bool CanCallBettingMenu();
extern bool CanCallObserverMenu();
#endif