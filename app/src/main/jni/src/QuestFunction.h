
#ifndef _CScriptTimer_H_
#define _CScriptTimer_H_

#include "network.h"
#include <string>
using namespace std;

//script�� ���� enum
enum SCRIPT
{
	SCRIPT_COUNTER_TYPE__NPCKILL = 0,
	SCRIPT_COUNTER_TYPE__ITEMCOUNT = 1,
};

///////CLASS CScriptTimer
class CScriptTimer
{
	//������ �޸� Ŭ���̾�Ʈ�� ȭ�鿡 ���� Ÿ�̸Ӹ� �ѷ��ֱ⸸ �ϸ� �ȴ�.
	//�׷��� id�� name�� �ʿ䰡 ����.

public:
	CScriptTimer(){ClearTimer();};
	~CScriptTimer(){};

private:
	DWORD	m_dwTimerTime;		//�Ҵ�� Ÿ��
	DWORD	m_dwSpendTime;		//���� �ð�. ����ȭ�� ����
	bool	m_bStart;			//Ÿ�̸Ӱ� �õ��Ǿ��°�?
	DWORD	m_dwSynchTime;		//�󸶸��� �ð����� ����ȭ �ϴ°�? �ƴϸ� �� �ñ⸶�� � �Լ��� �����Ұ��ΰ�?
	bool	m_bShow;

public:
	DWORD	m_dwOld;			//ī���� ���������� �� ����
	DWORD	m_dwSynch;			

	void	SetTimerTime( DWORD dwTime);				//Ÿ�̸��� �ð��� �����Ѵ�. 
	void	SetSynchroneTime( DWORD dwSyncTime );		//����ȭ �ϴ� �ð��� ����

	DWORD	GetCurretTimerTime() const { return m_dwTimerTime;};			//���� ���� �ð��� �˾ƺ���.
	DWORD	GetCurretSynchTime() const { return m_dwSynchTime;};		//������ ����ȭ �ð��� �˾ƺ���.
	DWORD	GetSpendTime() const { return m_dwSpendTime;};
	bool	GetShowMenu() const { return m_bShow; };

	bool	IsRunning();				//�������ΰ�?
	bool	IsStart() const { return m_bStart;};
	void	ClearTimer();								//Ÿ�̸� ������ ���� 0����
	void	FormatTimer( const DWORD& dwTime, char* szTime );
	bool	ConfirmSyncTimer( DWORD dwRecvTime );		//����ȭ �ð��̶� �� ���ð��̶� ��

	void	SendSCRIPT_TIEMER( DWORD dwSpendTime , int iType);		//��ũ�a ���߱� ���� ��Ŷ�� ������.
	void	RecvSCRIPT_TIMER_SYNC( t_script_timer *tp );//����ȭ ��Ŷ�� ������.
	
	bool	StartTimer();				//�ð��� �����Ǿ� ������ m_bStart = true�Ѵ�.
	void	RunTimer();					//����ؼ� ����
	void	EndTimer();
	void	ShowTimer( char* szMsg);	//Ÿ�̸� �ڷ���..
	void	ShowTimerMenu();
	void	HideTimerMenu();
};


///////CLASS CScriptCounter
class CScriptCounter
{
public:
	CScriptCounter(){};
	~CScriptCounter(){};

private:
	int		m_iNum;
	int		m_iType;
	char	m_szType[21];
	int		m_iMax;
	int		m_iCounter;
	bool	m_bStart;
	bool	m_bShow;
	int		m_iFlag;			// 030108 Ŭ���̾�Ʈ ī���Ͱ� ���� ���ٴ°��� �˸���

public:
	void	SetNum( const int num ) { m_iNum = num;};
	void	SetType( const int type );// { m_iType = type; if( m_iType ==};
	void	SetMax( const int max ) { m_iMax = max;};
	void	SetCounter( const int con) { m_iCounter = con;};
	void	SetFlag( const int flag) { m_iFlag = flag;};

	int		GetNum() const { return m_iNum;};
	int		GetType() const { return m_iType;};
	int		GetMax() const { return m_iMax;};
	int		GetCounter() const { return m_iCounter;};
	bool	GetShowMenu() const { return m_bShow; };

	void	ClearCounter(){ m_iNum=0; m_iType=0; m_iMax=0; m_iCounter=0; m_bStart= false; m_szType[0] = '\0'; m_iFlag=0;};
	void	ShowCounter( char* szMsg);

	bool	StartCounter();
	void	AddCounter(){ m_iCounter++;};
	void	EndCounter();
	void	ShowCountMenu();
	void	HideCountMenu();
	void	SendSyncCounter(int iCountNum);	// 030108 //�������� ������ ī��Ʈ���� ������. 
};


//////CLASS CQuestMgr
class CQuestMgr
{
public:
	CQuestMgr(){};
	~CQuestMgr(){};


//script timer����
	CScriptTimer	*m_cTimer;
	void	InitScriptTimer( const DWORD& dwTimerTime, const DWORD& dwSyncTime);
	void 	RunScriptTimer( char *szMsg);	//���鼭 �޴��� �Ѹ� ���ڸ� �����.
	void	DeleteScriptTimer();

//script counter����
	CScriptCounter	*m_cCounter;
	void	InitScriptCounter( t_script_counter *tp );//const int& iNum, const int& iType, const int& iMuch);
	void	RunScriptCounter( char *szMsg);//���鼭 �޴��� �Ѹ� ���ڸ� �����.
	void	SyncScriptCounter( t_script_counter *tp );	//monster kill�϶�
	void	CheckItemCount();							//item search �϶�
	void	DeleteScriptCounter();

//spellmapmove����
	string	m_szSpell;
  void	SetSpellWord( const char* szWord ){ m_szSpell.assign( szWord ); };	//ġ�z ���ڸ� ��� ����ض�.
	void	EmptySpellWord();													//�ֹ� ������ ���̵� �Ҷ� ������	
	void	CheckSpellWord( t_script_spellmapmove *spell);						//�ֹ��� �´��� Ȯ���ؼ� �ʿ��� ��� ����
	void	RecvSpellWord_Suc( char* szMap );
	void	RecvSpellWord_Fail();
	void	SendSpellWord(const t_script_spellmapmove *spell);
};

extern CQuestMgr	g_cQuest;

#endif//	_CScriptTimer_H_