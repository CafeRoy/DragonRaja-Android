#ifndef __QUESTTEXT_H__
#define __QUESTTEXT_H__

#define QUEST_NAME 512
#define QUEST_MAX 200
// ����Ʈ �ؽ�Ʈ �ٳ����� 
#define QUEST_TEXT_DEVISION		20

#define		SCREEN_SIZE		SCREEN_HEIGHT
//����Ʈ ���� ���� �⺻���� 
#define		QUEST_IF_COUNT					10		//����Ʈ ���� ��/*��Ȯ�� ��ġ �Է�*/  
	
#define		QUEST_IF_LEVEL					1
#define		QUEST_IF_CLASS					2
#define		QUEST_IF_JOB					3
#define		QUEST_IF_SKILL					4
#define		QUEST_IF_MEMBERCOUNT			5
#define		QUEST_IF_ITEM					6
#define		QUEST_IF_FAME					7
#define		QUEST_IF_JOB_FAME				8
#define		QUEST_IF_QUEST_NUM				9
#define		QUEST_IF_QUEST_TIME				10
// ����Ʈ �Ϸ�� �־����� ��ġ�� ���� 
#define		QUEST_TAKE_COUNT				9		//����Ʈ ���� ��/*��Ȯ�� ��ġ �Է�*/  

#define		QUEST_TAKE_MONEY				1
#define		QUEST_TAKE_FIGHT_FAME			2
#define     QUEST_TAKE_JOB_FAME				3
#define		QUEST_TAKE_FIGHT_DO				4
#define		QUEST_TAKE_JOB_DO				5
#define		QUEST_TAKE_ITEM					6
#define		QUEST_TAKE_WSWIZ				7
#define		QUEST_TAKE_PSWIZ				8
#define		QUEST_TAKE_POINT				9

extern void LoadQuestTable(const char* filename, const char* filename2, const char* filename_bin);

//extern void ReadQuestData(  int no, char *buf );
extern void kh_InitMenu();
extern void SetEndQuest(int num);
extern void SetStartQuest(int num);
//ReadQuestTable ob;

extern int	QUEST_GIVE;
extern int	QUEST_COUNT;
extern int end_quest[QUEST_MAX];

extern  void CallKhMenuProc( int menu, int field );		// ȭ�� ���
extern bool DoButtonCheckOfMenu_kh( int i, int j );//020410 lsw

extern int kein_kh_key_on;
extern void LoadSpriteOnlyOne( Spr *s, char filename[MAX_FILENAME] );		// ��������Ʈ ȭ�� �ϳ��� �о� �ö�
extern void FreeSpriteOnlyOne( Spr *s );
extern void FreeSpriteOnlyOne( Spr *s );
extern void kh_menu_set( );
extern void lsw_menu_set( );//010830 lsw
extern int	quest_num[QUEST_MAX] ; 
extern int	QUEST_COUNT ;
extern void kh_KeyProc();







//kkh ����Ʈ���� �� ������ �־�� �� ���� ���� �ð��� �����ð� �� �ɸ��ð� 
class QuestTable {
public :
//	ReadQuestTable*		next;	//Linked list
// �ʵ忡�� ��ȣ	�̸�. ��Ÿ ���� ,,,
	int		num;							//	����Ʈ ��ȣ
	int		code;							//	����Ʈ �ڵ�
	char	name[QUEST_NAME];				//	����Ʈ �̸� 
	int		place;							//	����Ʈ �߻���� 
	char	ch[QUEST_NAME];					//	����Ʈ ���� 
	int		quest_if[QUEST_IF_COUNT];		//  ����Ʈ �߻� ���� 
	int     quest_take[QUEST_TAKE_COUNT];	//  ����Ʈ �Ϸ�� �־����� ��ġ ���� 	
	int		picnum;	
	char	picture[QUEST_NAME];
	Spr		sq;
//......................................
	//int Q_Conut[QUEST_INDEX_VIEW];
//	int	req_lv;
//	int	req_str;
	int count;
	char* buf;
	char* token;
// FUNCTION............................................................
	void LoadTable ( char *filename );	// ���ڿ����� ����Ÿ�� �и�
	void LoadPicture ( char *filename );	// 
	void SaveTable();		// txt ���� ���ؿ���
	//	void ReadQuestData( int no, char *buf );
	QuestTable() { num = -1; /*next = NULL;*/ }
	QuestTable(int n) { num = n;/*next = NULL;*/}
/*	void QUEST_ON(int QUEST_NUM);
	void QUEST_OFF(int QUEST_NUM);
	void QUEST_PRINT();*/	//Linked list
//.....................................................................
};
	



extern QuestTable quest_text[QUEST_MAX];
extern void FreeQuest();			// ��������Ʈ ����
extern void QestDataFree( );		// ����Ÿ ����ü 0���� ����
extern void CharacterWorkingWater( LPCHARACTER ch );





#endif