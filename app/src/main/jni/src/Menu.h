#if !defined(AFX_MENU_H__CCD50119_39F2_4932_8DEF_B46CD7E4330A__INCLUDED_)
#define AFX_MENU_H__CCD50119_39F2_4932_8DEF_B46CD7E4330A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "network.h"
#include "MenuNetwork.h"
#include "word.h"
#include "EditWndMgr.h"//021001 lsw



//////////////////////////////////////
// ���� ���̴� ���� accessory define
#define AT_ARMOR	0
#define AT_HELMET	1
#define AT_WEAPON	2
#define AT_SHIELD	3


// SetPartyMenu()���� ����� ����
#define RELATION	100
#define PARTY		200
#define EMPLOYMENT	300
#define NATIONPARTY	500
#define MAN_LIST_MAX_	100		// ���� ���ε� ( �̺�Ʈ ��ȣ �ִ� )



/////////////////////////////////////////////////////////////////////////////////
#define MAX_LEARN_SKILL_PAGE	50	// ������ ����� ������ �ƽ�

//ACER7
#define PK_MODE	2
#define BATTLE_MODE 1
#define PEACE_MODE  0


//////////////////////////////////////////////////
#define CHOBO_LEVEL		7


////////////////////////////////////////
// sound

#define SN_CONGRATULATIONS		62
#define SN_TOUCH_START			1214
#define SN_CLICK_START			1212
#define SN_SELECT_START			1211
#define SN_DICE					1210//	�ֻ��� ������ 


//-- �߰� : 1206 (��� ������ �����) 1207 (��� �������� ���� ���)
#define SN_GET_ITEM		1206
#define SN_DROP_ITEM	1207

/////////////////////////////
// �ݷμ������� ���̴� ����
#define SN_FIGHT_WIN			251
#define SN_FIGHT_LOSE			252
#define SN_FIGHT_TIME			253
#define SN_FIGHT_STOP			254
#define SN_FIGHT_START_READY	255
#define SN_FIGHT_START_FIGHT	256


// ( ���� )
#define SN_WEAPON	1200	//	������ �� ����
#define SN_SHIELD	1201	//	����
#define SN_ARMOR	1202	//	����
#define SN_CLOTHE	1215	//	����( �Ƿ� )
#define SN_HELMET	1203	//	���
#define SN_UNIQ		1204	//	����ũ ������
#define SN_NECK		1204	//	�����
#define SN_SHOES	1205	  //  �Ź�		���� ��������  �����Ұ��


/// �춧, �ȶ�
#define SN_ITEM_BUY				1236
#define SN_ITEM_SELL			1237	//�������� �ȶ�

//// 
#define SN_EAT_BREAD			1208	//	�������� (�Һ�)
#define SN_EAT_POTION			1209	//	���� ������ (�Һ�)
#define SN_DRINK_BEER			1226	//	���� ���ñ�(�Һ�)


///////////////////
#define SN_WARNING				1213	//	�����

#define SN_BOX_OPEN				1219//	�ڽ� ������ �����
#define SN_BOX_CLOSE			1220///	�ڽ� ������ ���� ���


#define SN_MAGIC_BOOK_OPEN		1221//	����å �����
#define SN_MAGIC_BOOK_CLOSE		1222//	����å ���� ���
#define SN_GET_BOOK				1223//	// å ������ �Ҹ�
#define SN_BOOK_NEXT			1224//	// å �ѱ�� �Ҹ�

// MENU 
#define SN_MENU_OPEN	1216//	�޴��� ������
#define SN_MENU_CLOSE	1217//	�޴��� ��ĥ��

// �޴�
#define SN_MOVE_OPEN	SN_MENU_OPEN // 1214//	�޴��� �����̸鼭 ���µɰ��( �븮�� �����̴� ���� �Ҹ� )
#define SN_MOVE_CLOSE	SN_MENU_CLOSE //	�޴��� �����̸鼭 ��ĥ���
///////////////////////////////////////////////
// ���콺 ����
#define SN_TOUCH		1212	//	���콺�� ��ư�� �ٰ������
#define SN_CLICK		SN_CLICK_START//1211	///	Ŭ���Ұ��

//����
#define SN_FIGHT		1218
#define SN_LOGOUT		1240   // �α׾ƿ��Ҷ�

#define SN_RECV_BBS		64
#define SN_FACE_PUT		64		// ǥ�� ����
#define SN_MAGIC_BALL	1225	// ������ ��¦�̴� �Ҹ�( ���콺�� ������ ���� ���� ó���ϴ°� ���ڴ� )

#define SN_SKILL		1218//	// ��ų ������ Ŭ���� ���
//#define SN_DEFAULT		 9

#define SN_SLEEP				1250//	�������� ���ڱ�
#define SN_REST					1250//	�������� ����
#define SN_TENT					1250//	��Ʈ���� ����

//#define SN_TOUCH_POTION			//����//	������ ��� �ְų� ���콺�� ���ǿ� ������ �������

//-- ���� (����)

#define SN_IRON_DRUM			1227	// �����
#define SN_DRUM					1228	// ����
#define SN_NEW_CHARACTER		1229	//	���ο� ĳ���͸� ������� ���

// #define SN_SELL_ITEM ?
// #define SN_BUY_ITEM ?

#define SN_BUY_HOUSE			1230//	���� ���� ���
#define SN_ITEM_REPAR			1231//	�������� ��ĥ��
#define SN_LEARN_ITEM			1232//	����� ���� �������� ��ﶧ
#define SN_LEARN_MAGIC			1232//	������ ��ﶧ
#define SN_MAGIC				1218    //���� ������ Ŭ���� ���
#define SN_LEARN_SKILL			1232	// ��ų�� ��ﶧ
#define SN_LV_EACH_UP			1233	// ������ ������ ����Ʈ�� �ø���, 
#define SN_LV_EACH_DOWN			1233	// ������ ������ ����Ʈ�� ������, 

//#define SN_DO_MEMORIZE			����		//	�ӽ�	// �޸����� ���� ��� -- �޸������ ���� �迭���� ����Ÿ�� Ʋ����..����Ʈ ����...or 1230�� ����Ÿ�� ����..
#define SN_DO_MEMORIZE			1230	// �ӽ�	// �޸����� ���� ���

#define SN_GET_MONEY			1206	//���� �鶧
#define SN_PUT_MONEY			1236	///���� ������


//- ���� : ������ �������� ������ â�� ���϶��� �Ҹ� (�� �� ������ ���� ������ â�� ���϶��� �Ҹ��� �ٸ� ��� �� �ش� ȿ���� �߻�..)
#define SN_GAMEEND				1240	//������ ������
	
#define SN_GOD_REGIST			1230	//������ ����Ҷ�
#define SN_GOD_MEETING			1252	// ���� ����

#define SN_EAR_MESSAGE_RECV		1241	// �ӼӸ� �޾��� ��

#define SN_SEND_MAIL			1243	// ���� ������
#define SN_RECV_MAIL			1242	// ���� ������


////////////////////////////////////////////////////////////
#define	SN_KILL_HUNTER_OK		1230		// ������� ����ϴµ� �������� ���
#define	SN_KILL_HUNTER_NOT		1213		// ������� ���� ���� ����� ������ ��� 
#define SN_START_FIGHT			SN_FIGHT	// 1:1 ���� ����

/////////////////////////////////////////////////////////////

//#define SN_FIGHT_READY				61
//#define SN_FIGHT_START				68

#define SN_KILL_WHO					55
////////////////////////////////////////////////

#define SN_OPEN_NATION_BBS			67
/////////// 0808 lkh ���� ///////////
#define	SN_START		1	//	���� �����Ҷ�
#define	SN_BGMEND		6		//������ ������ǹ�ȣ
#define SN_X_MAS_BGM	9			// ũ�������� �����

/////////////////////////////////////////////////


#define SN_QUEST_START	50
#define SN_QUEST_END	51

#define SN_GM_REGIST_OK	51

#define SN_INFO			400
#define SN_DUAL_MUSIC  67
#define SN_DUAL_SELECT 151
#define SN_DUAL_OK     2409

enum eRIGHT_CLICK_FINCTION
{
	RC_FUNC_HAMMER,
	RC_FUNC_MAGICITEM,
	RC_FUNC_MAGICITEM2,
	RC_FUNC_LEGEND_ITEM,
	RC_FUNC_MAX,
};
//////////////////////////////////////////////////////////////////////////////////////
// �߰� �ϰ� ������
// #define SN_KILL_PC		?
// #define SN_END_FIGHT		?

const int SHOW_MAGIC = 12;
const int ONE_MAGIC_RECT = 44;


extern void MenuDisplay();      //�޴��� Ȱ�����θ� �Ǵ��� ����ϴ� �Լ�
extern void MenuSetting();      //������ �����ϱ� ���� �޴� ����ü���� �ʱ�ȭ  GamePorc() ���� ȣ��
extern void StartMenuSetting();      //������ �����ϱ� ���� �޴� ����ü���� �ʱ�ȭ  GamePorc() ���� ȣ��
extern void MenuChecking();     //�޴� ��������� �浹üũ

extern void StartMenuDisplay();      //�޴��� Ȱ�����θ� �Ǵ��� ����ϴ� �Լ�
extern void StartMenuChecking();     //�޴� ��������� �浹üũ
extern void LoadMenuData(int nNum);                   //�ʿ��� �޴� �̹��� ����Ÿ�� �޸𸮿� �ε�
extern void FreeMenuData(int nNum);                    //�ʿ� ���� �޴� �̹��� ����Ÿ �޸𸮸� ��ȯ
extern void CallTidyInventory();

extern bool g_DropOnHero;
extern int StartPosition ;
extern int StartMap;
//021014 lsw
extern SMENU	SMenu[MAX_MENU]; 
extern const int mainmenu[];
extern const int smallmenu[];
extern const int aNoCloseMenu[];
extern const int aNoCloseOfMainMenu[];

extern const int MAX_MAIN;
extern const int MAX_SMALL;
extern const int max_submenu;
extern const int MAX_NO_CLOSE_MENU;
extern const int MAX_NO_CLOSE_MAIN_MENU;
extern bool CanMoveItemToMenu(const int iFieldType);

extern int		RectImageOn;
extern int		DayLightControl;		// 15 ���� ũ�� �� , ������ ��
extern bool		is_mainmenu_active;
extern bool		g_bDualMagicBookOpen;//soto-030421
extern int		g_StartMenuOn;
extern BOOL		MouseDrag;

extern	CharacterParty mouse_ch;
extern bool g_bTimeMenuOn;			//	time menu open or close
extern bool	g_ExplainMenuOn;		//  Do open or close for explain menu 

extern	ItemJoinSelect Item_Select[MAX_LEARN_SKILL_PAGE][10];		// ������ ����� ���� �����ϴ� ���
extern	ItemJoinSelect item_select_result;		// ������ ����⿡�� ������ ������

extern	PosItem		skill_use_item[5][10];
extern	PosItem		skill_use_item_result;
extern	int SkillOn;							// quick skill 0n
extern	bool g_GoDice;						// �ֻ����� ������ �Ǵ°� �ȵǴ°�...
extern	bool g_GoDice2;
extern	int nDiceNumber[5];
extern	bool g_GoDiceRecv;

extern	k_client_candidate		Candidate[3];	// ���ĺ� ����
extern int	g_Candidate_max;
extern int g_CurrVote;
extern int g_CanRegistCandidate;

extern char FightTeam[5][25];
extern	DWORD FightTeamColor[5];

extern BYTE	*buf_alphabet;
extern Spr	spr_alphabet[36];


extern BYTE	*facebuf_man_b;
extern BYTE	*facebuf_woman_b;
extern BYTE	*facebuf_man_m;
extern BYTE	*facebuf_woman_m;
extern BYTE	*facebuf_man_s;
extern BYTE	*facebuf_woman_s;
extern BYTE	*facebuf_man_p;
extern BYTE	*facebuf_woman_p;
extern BYTE	*facebuf_man_mm;
extern BYTE	*facebuf_woman_mm;


extern Spr spr_face_man_b[FACE_MAX];
extern Spr spr_face_woman_b[FACE_MAX];
extern Spr spr_face_man_m[FACE_MAX];
extern Spr spr_face_woman_m[FACE_MAX];
extern Spr spr_face_man_s[FACE_MAX];
extern Spr spr_face_woman_s[FACE_MAX];
extern Spr spr_face_man_p[FACE_MAX];
extern Spr spr_face_woman_p[FACE_MAX];
extern Spr spr_face_man_mm[FACE_MAX];
extern Spr spr_face_woman_mm[FACE_MAX];


// 0906 kkh �߰� 
extern Spr spr_face_man_b_dash[ADD_CHA_PIC];
extern Spr spr_face_woman_b_dash[ADD_CHA_PIC];
extern Spr spr_face_man_m_dash[ADD_CHA_PIC];
extern Spr spr_face_woman_m_dash[ADD_CHA_PIC];
extern Spr spr_face_man_s_dash[ADD_CHA_PIC];
extern Spr spr_face_woman_s_dash[ADD_CHA_PIC];
extern Spr spr_face_man_p_dash[ADD_CHA_PIC];
extern Spr spr_face_woman_p_dash[ADD_CHA_PIC];
extern Spr spr_face_man_mm_dash[ADD_CHA_PIC];
extern Spr spr_face_woman_mm_dash[ADD_CHA_PIC];

extern BYTE	*facebuf_man_b_dash;
extern BYTE	*facebuf_woman_b_dash;
extern BYTE	*facebuf_man_m_dash;
extern BYTE	*facebuf_woman_m_dash;
extern BYTE	*facebuf_man_s_dash;
extern BYTE	*facebuf_woman_s_dash;
extern BYTE	*facebuf_man_p_dash;
extern BYTE	*facebuf_woman_p_dash;
extern BYTE	*facebuf_man_mm_dash;
extern BYTE	*facebuf_woman_mm_dash;


///////////////////////////////////////////////////
extern BYTE	*god_spr_buf;
extern Spr	god_spr[GOD_MENU_IMG_MAX];		// ���� �޴� 

extern BYTE	*time_spr_buf;
extern Spr	time_spr[TIME_MENU_IMG_MAX];

extern BYTE	*membook_spr_buf;
extern Spr	membook_spr[MEMBOOK_MENU_IMG_MAX];		

extern BYTE	*magicbook_spr_buf;
extern Spr	magicbook_spr[MAGICBOOK_MENU_IMG_MAX];		

extern BYTE	*mail_spr_buf;
extern Spr	mail_spr[MAIL_MENU_IMG_MAX];	

extern BYTE	*start_etc_buf;
extern Spr	start_etc_spr[START_ETC_IMG_MAX];	

extern BYTE	*start_etc2_buf;
extern Spr	start_etc2_spr[START_ETC2_IMG_MAX];	

extern BYTE	*job_etc_buf;
extern Spr	job_etc_spr[JOB_ETC_IMG_MAX];

extern BYTE	*select_server_buf;
extern Spr	select_server_spr[SELECT_SERVER_IMG_MAX];	

extern BYTE	*start_map_buf;
extern Spr	start_map_spr[START_MAP_IMG_MAX];	

extern BYTE	*chat_menu_buf;
extern Spr	chat_menu_spr[CHAT_MENU_IMG_MAX];	

extern BYTE	*main_etc_buf;
extern Spr	main_etc_spr[MAIN_ETC_IMG_MAX];	

extern BYTE	*main_etc3_buf;
extern Spr	main_etc3_spr[MAIN_ETC3_IMG_MAX];	

extern BYTE	*skill_menu_buf;
extern Spr	skill_menu_spr[SKILL_MENU_IMG_MAX];	

extern BYTE	*small_map_buf;
extern Spr	small_map_spr[SMALL_MAP_MAX];

extern BYTE	*small_etc_buf;
extern Spr	small_etc_spr[SMALL_ETC_MAX];

extern BYTE	*public_bbs_buf;
extern Spr	public_bbs_spr[PUBLIC_BBS_MENU_IMG_MAX];

extern BYTE	*hunter_buf;
extern Spr	hunter_spr[HUNTER_MENU_IMG_MAX];

extern BYTE	*questmenu_buf;
extern Spr	questmenu_spr[QUEST_MENU_MAX];
// ���ϰ� 
extern BYTE	*famemenu_buf;
extern Spr	famemenu_spr[FAME_MENU_MAX];

// 0905 kkh �߰� 
extern BYTE	*chcolorcha_buf;
extern Spr	chcolorcha_spr[COLOR_CHANGE_MENU_MAX];
//	0908 KKH �߰� 
extern BYTE	*item_make_image_buf;
extern Spr	item_make_image_spr[ITEM_MAKE_IMAGE_MAX];

extern BYTE	*nation_buf;
extern Spr	nation_spr[NATION_MENU_IMG_MAX];

extern BYTE	*start_fight_buf;
extern Spr	start_fight_spr[START_FIGHT_IMG_MAX];

extern BYTE	*stop_fight_buf;
extern Spr	stop_fight_spr[STOP_FIGHT_IMG_MAX];

extern BYTE	*fightmap_time_buf;
extern Spr	fightmap_time_spr[FIGHTMAP_TIME_IMG_MAX];

extern BYTE	*fightmap_win_buf;
extern Spr	fightmap_win_spr[FIGHTMAP_WIN_IMG_MAX];

extern BYTE	*fightmap_lose_buf;
extern Spr	fightmap_lose_spr[FIGHTMAP_LOSE_IMG_MAX];

extern Spr scenario_spr;		// �ó�����..

extern BYTE	*vote_buf;
extern Spr	vote_spr[VOTE_MENU_IMG_MAX];

extern BYTE	*guild_mark_buf;
extern Spr	guild_mark_spr[GUILD_MARK_MAX];

extern BYTE	*guild_admin_buf;
extern Spr	guild_admin_spr[GUILD_ADMIN_MAX];

extern BYTE	*guild_skill_list_buf;
extern Spr	guild_skill_list_spr[GUILD_SKILL_LIST_MAX];

extern BYTE	*guild_admin_etc_buf;
extern Spr	guild_admin_etc_spr[GUILD_ADMIN_ETC_MAX];

extern BYTE	*help_pcx_buf;
extern Spr	help_pcx_spr[HELP_PCX_MAX];

extern BYTE	*bank_buf;
extern Spr	bank_spr[BANK_MAX];

extern BYTE	*salvation_buf;
extern Spr	salvation_spr[SALVATION_MAX];

extern BYTE	*salvation_ani_buf;
extern Spr	salvation_ani_spr[SALVATION_ANI_MAX];

extern BYTE	*team_battle_buf;
extern Spr	team_battle_spr[TEAM_BATTLE_MAX];

extern BYTE	*cahnge_money_buf;
extern Spr	cahnge_money_spr[CAHNGE_MONEY_MAX];

extern BYTE	*guild_wait_buf;
extern Spr	guild_wait_spr[GUILD_WAIT_SPR_MAX];

extern BYTE	*merchant_buf;
extern Spr	merchant_spr[MERCHANT_SPR_MAX];

extern BYTE	*guild_menu_buf;
extern Spr guild_menu_spr[GUILD_MENU_SPR_MAX];

extern BYTE	*dual_img_buf;
extern Spr dual_img_spr[DUAL_IMG_SPR_MAX];


///////////////////////////////////////////////////////

extern int g_VoteSignExist;
extern int BetaTest;
extern int SoundOn;

extern void SetRect(RECT &rect,int left, int top, int right, int bottom);
extern void FieldTypeNomalPut(int mx, int my, int x, int y, int nImageNumber, int type = 0);
extern void FieldTypeNomalPutFx(int mx, int my, int x, int y, int nImageNumber, int t, int j);
extern void FieldTypeNomalPutFx3(int mx, int my, int x,int y,int nImageNumber, int type , int t, int j);//010910 lsw

//######## ���� �������̽����� ������ ������ ���� ##########
#define IF_MAP				0		// IF -> info_flag
#define IF_CHAT_MESSAGE		1
#define IF_BATTLE_PEACE		2		//	0:peace, 1:battle

#define INFO_FLAG_MAX		3		// ���� �÷��� ����

struct	InterfaceFlag
{
	//acer7
	int data[INFO_FLAG_MAX];
};

extern struct	InterfaceFlag main_interface;
extern bool		IsMenuActive();					// Ȱ��ȭ�� �޴��ȿ� ���콺�� �ִ����� �Ǵ�
extern void		LoadSmallMenuImage( int num );	// ���� �׸� �ε��ؿ´�.
extern void		CallMenu(int Menu, int x, int y, BOOL bMenuFocus=FALSE);
extern void		CallMenu( int Menu, int temp=0, bool is_focus = false );




extern	void	SendLevelUpPoint( WORD p[11] ) ;
extern	void	RecvLevelUpPoint( t_server_levelup_point *p );
extern	void	RecvLevelUpPointResult( int ok );
extern	void	RecvCallSmallMenu( t_server_call_smallmenu *p );
extern	void	RecvCallDualMagicLearnBook();//soto-030421
extern	void	ReqSmallMenu( int menu, int storelistindex );
extern	void	RecvStoreItemList( t_server_store_itemlist *p );
extern  void	RecvMallItemList(t_MallItemList *p);

extern	void	SendItemBuy( int item_pos, int inv_pos, int store_id );
extern	void	SendItemSell( int item_pos, int store_menu );
extern	void	SendMallItemBuy(int num, int inv_pos);
extern	int		SendDeleteCharacter( const char *szName, const char *szSecretKeyCode);

extern	int		GetMagicCastingTime( int magic_no, SCharacter &ch );
extern	int		CanMakeItemListSet( int s , int menu=0);
extern	void	DropItemOnHero( ItemAttr &item_attr );
extern	void	CallOtherCharacterInv( ItemAttr inv[3][3][8], ItemAttr equip[8], ItemAttr quick[7] );



extern	int		SearchEmpty(int&, int&, int&);
extern	ItemAttr *SearchEmpty();
extern	int SearchEmptyQuickItem();
extern	int SearchEmptyBank(int &a, int &b, int &c);
extern	int SearchEmptyBank2(int& a, int& b, int& c);
extern	ItemJoinSelect learn_item_list[MAX_LEARN_ITEM];
extern	void	CallSmallMenu(int menu, int pre=0, int sound=SN_MENU_OPEN);											// ���� �޴� ���Ҷ� ���


extern	void	CallServer( int type, void *msg =NULL, int size=0 );			// ������ ������ ��û�Ҷ�

extern	int		g_MouseItemNumber;
extern	int		g_MouseItemType;

extern int GetIndexOfCondition( BYTE condition );
extern int RectAndRectCrash( const RECT *box1, const RECT *box2 );
extern int RectAndDotCrash( const RECT *box1, const int x, const int y );

extern void FreeOpenningSpr( BYTE **buf );
extern void ReStartMenu( );

extern int IsMouseOnMenu();
extern void SmallMenuClose( );
extern void MainMenuClose( );


extern Spr spr2[MAX_IMAGE2];		// �߰� �޴� �̹��� 
extern void FieldTypeNomalPutFx2(int mx, int my, int x,int y,int nImageNumber, int t, int j);
extern void FieldTypeNomalPut2(int mx, int my, int x,int y,int nImageNumber);
extern void LoadSmallMenuImage2( int num );
extern void UpperMenuNomalPut(int menui, int &x, int y,int nImageNumber, int type);


extern bool bCallGodMeetingMenu;
extern void CloseAllMenu();
extern int CheckLanguageForMenu( const char* lpStr );
extern int GetDiceAbility( int DiceNumber );		// ���̽� ������
extern void AddWaveOfEndString( char *text );		// ������ �ٿ��ش�.		���ڿ��� �˳��� ���� �־�� �Ѵ�.

extern int RectTextPut(const int x, const int y, int nWidth, char *sSource, const int hight = 18, const int line_max = 0);
extern int RectTextPutNotEatWhiteChar(const int x, const int y,int nWidth, char *sSource,const int hight = 18, const int line_max = 0);
extern int RectBoldTextPut(const int x, const int y,int nWidth, char *sSource, const int hight=18, const int line_max= 0);//020730 lsw
extern int RectBoldTextPutNotEatWhiteChar(const int x, const int y,int nWidth, char *sSource, const int hight=18, const int line_max= 0);//020730 lsw
extern int GetRectTextLine( char *sSource, int nWidth, const int line_max = 0);//020703 lsw
extern char *DivideStringByRect( int nWidth, char *sSource, int max_line = 0 );
extern bool CanActiveMessageLine();
extern int GetLineRectTextPut( int nWidth, char *sSource ); // line�� ���� üũ�Ͽ�..
extern int SkipRectTextPut( int x, int y, int nWidth, int skip, char *sSource, int line_max = 0, int fx_type = 0 ); // line�� ���� üũ�Ͽ�..
extern void GameEndFree();
extern int LoadSmallMapSprData(int mapnumber); // CSD-030324 // thai2 YGI
extern int  PutHpGauge( int x, int y, LPCHARACTER ch );
extern int  PutHeroHpGauge( const int x, const int y);//020626 lsw
extern int  PutHeroMpGauge( const int x, const int y);
extern int GetTextLine( char *filename );
extern void InsertMagicQuickArray( int magic_no );
extern void DeleteMagicQuickArray( int magic_no );
extern void ResetMagicQuickArray( );
extern void MagicSetting2();
extern bool	ReadMagicTable();
extern bool ConvertMagicBinToTxt(const std::string &bin_filename, const std::string &txt_filename);
extern char *GetNameById(int id);  // ���̵�� �̸��� ���� �´�.
extern int ConvMapnameToMapnumber( char *mapname );
extern Spr *GetSprOfMenu( int type, int nImageNumber );		// ��������Ʈ ����Ÿ
extern void MP3( int n, int ing=0 );
extern void PutLvNameByColor( int x, int y, LPCHARACTER ch );
extern void PutDualMark(int x, int y, LPCHARACTER ch);
extern bool IsHeroDead();
extern char NationName[8][25];
extern const DWORD NationColor[7];
extern const WORD NationColor16[7];
extern int	fight_map_live_man[4];
extern void FreeAllOfMenu();		// 0927 YGI
extern void DeleteMouseCh( char *name );
extern void SetMouseCh( LPCHARACTER lpTarget );
extern LPCHARACTER ExistHe(const char *name );		// �� �þ߿� �����ϴ� ������ �˾ƺ��� ��ƾ
extern LPCHARACTER ExistHeCase( char *name );
extern void InitMenuByChangeMap();

extern int LoadGuildTableEx( );
extern void PutJobLv( int x, int y, int job, int call_job_lv );		// 1004 YGI( ���� ��� ��� )
extern int GetCallJob( int call_job_lv );		// ���� �ɷ� ���� ��������

extern void ViewGuildCommandInfo( );		// ��� ��� ����
extern char *Kein_EatFrontWhiteChar( char *pStr );

extern void ViewEarCommandInfo();


extern void KeinMessageBox( int next_menu, char *msg, ... );// 020701 YGI

extern int AutoLevelUpOn;
extern DWORD AutoLevelUpCount;
extern void AutoLevelUp();
//extern char man_list[MAX_MAP_][MAN_LIST_MAX_][20];		// ���� ���� �̸���
extern int TotalSalvation;

extern int g_SetGuildMarkViewOn;
extern int g_GuildMax;

extern void PrintMagicExplain( int magic_no );
extern int	g_CurrUsedMagic;		// ���� ĳ���� �ǰ� �ִ� ����

// ok�� �ִ� ���� ok_cancel�� �ִ� �޽��� �ڽ������� ����,  menu�� ȣ���� �޴��� �ִ´�
extern void CallOkCancelMessageBox( int menu, int x, int y, const char *source, bool ok_cancel = true, int param = 0 );

extern int g_fight_win;
extern int g_fight_lose;

extern int g_RemainTimeType; 
extern int g_RemainTimeDelay;

extern int ConvString(std::string &szStr);  // �弳 ����
extern bool bSmallTips;
extern char *kein_GetMenuString( int index );

extern char *g_szScale;


#define KM_INFO		0		// Kein_PutMessage
#define KM_FAIL		1
#define KM_OK		2
#define KM_BBS		3

extern void Kein_PutMessage( int type, char *msg, ... );
extern int g_SelectServerIndex;


#define SI_GAME_MAKE_MODE		0
#define SI_NOT_CONNECT_MODE		1

extern int GetSysInfo( int type );
extern bool IsReporterMode();

#define PAGE_NUMBER__	 2//020515 lsw �̵� �߽��ϴ�

extern bool g_bLogOnFlag;
extern SCharacter		LoadCh[4];								// �������� ĳ�� ������ ���۹޴´�extern SCharacter LoadCh[4];								
extern t_client_login_fail_reason g_client_login_fail_reason;		// �α��� �� �޽���

extern void RecvChatSetResult( t_chat_set_result *p );
extern CHARACTERLIST	g_CharacterList;											
extern SCharacter		LoadCh[4];								// �������� ĳ�� ������ ���۹޴´�extern SCharacter LoadCh[4];								
// �������� ĳ�� ������ ���۹޴´�

extern int		YouGotItemFlag;
extern BOOL		MouseDrag;


extern ItemAttr InvItemAttr[3][3][8];		//### 1214 YGI

extern void SettingChangMapMenu( int ct ); // smallmenu.cpp
extern void RecvPleaseGiveLife( int id, int hp, int mana, int hungryp, int who );

extern void	RecvQSF_Command( t_qsf_command	qc );

extern int CheckHandleByNationWar(t_packet *p);  // 010904 LTS

extern void Recv2NDGMMAKEResult(t_SkillRareUpgradeResult *p);//011004 lsw
extern void RecvCallSmallMenuCustom(t_server_call_smallmenucustom *p);//011011 lsw


extern bool	DoButtonCheckOfMenu_SWD(int nMenuIndex,int nFieldIndex);//soto-030430

extern bool CheckLegendItem(ItemAttr* item);
extern bool		IsCraftEquip(int item_no);
extern  int		GetItemAttr( int item_id, int attr);				//011018 KHS ����
extern  int		GetItemAttr2(int sprno, int item_id, int attr);		//011018 KHS ����


extern void SendRequestTileDontData();		// 010904 LTS
extern void SendCMD_REQUEST_COMPARE_PHASE();	// LTS LOCALWAR	
extern void SetLocalWarArmor(t_server_change_equip *p,LPCHARACTER ch);	// LTS LOCALWAR
extern void SetLocalWarArmor(t_server_user_add *c);	// LTS LOCALWAR
extern bool g_bWarBegin; 
extern int	g_LocalWarBegin;					// LTS LOCALWAR
extern void SetWarArmor(t_server_change_equip *p,LPCHARACTER ch);
extern void SetWarArmor(t_server_user_add *c);	// 010904 LTS
extern int GetLeveByExp(int nStep, DWORD exp);
extern ItemAttr *GetItemByPos( POS item ); // itemtable.cpp
extern 	SCharacter			SCharSource;		
extern 	int					character_active;
extern	int					g_LocalWarBegin;			// 011215 LTS	
#endif//AFX_MENU_H__CCD50119_39F2_4932_8DEF_B46CD7E4330A__INCLUDED_