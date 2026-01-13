//#include <sqlext.h>
#include "MenuDefine.h"
//#include "MyTypes.h"

#ifndef __MenuDef_h__
#define __MenuDef_h__
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define FONTSIZE_			6           //���� 1���� ���α���(����) strlen�Լ��� ����Ʈ�� ��� �ű⿡ 6�� ���Ѵ�

#define ConvertColor(r,g,b)	(	(r>>3)<<11 | (g>>3)<<6 | b>>3	)//020610 lsw

const WORD FONT_COLOR_DEFAULT			= ConvertColor(141,144, 132);		//	���� ���� (�ѱ�, ����)
const WORD FONT_COLOR_BLACK				= ConvertColor(0	,0	, 0);		//	���� ���� (�ѱ�, ����)

const WORD FONT_COLOR_SOLID_YELLOW		= ConvertColor(255,255, 0);		//	��¥ ���
const WORD FONT_COLOR_SOLID_RED			= ConvertColor(255,0, 0);		//	��¥ ����
const WORD FONT_COLOR_SOLID_BLUE		= ConvertColor(0,0, 255);		//	��¥ �Ķ�
const WORD FONT_COLOR_SOLID_GREEN		= ConvertColor(0,255, 0);		//	��¥ �ʷ�
const WORD FONT_COLOR_SOLID_VIOLET		= ConvertColor(255,0, 255);		//	��¥ ����
const WORD FONT_COLOR_SOLID_JADE		= ConvertColor(0,255, 255);		//	��¥ ����

const WORD FONT_COLOR_PLUS				= ConvertColor(255,61	, 65);		//	�ɷ�ġ���� �÷��� �� ����	(���� ��)
const WORD FONT_COLOR_NAME				= ConvertColor(255,220, 100)	;	//	�̸� (�ݻ�����)

const WORD FONT_COLOR_YELLOW			= ConvertColor(200,170, 75)	;	//	�̸� (�ݻ�����)
const WORD FONT_COLOR_ORANGE			= ConvertColor(255,220, 80)	;	//	�������������� Ƣ�� �� (��Ȳ��)

const WORD FONT_COLOR_BLIGHT_GREEN		= ConvertColor(150	,220, 30)	;	//	���λ�
const WORD FONT_COLOR_BLIGHT_GREEN2		= ConvertColor(30	,220, 150)	;	//
const WORD FONT_COLOR_BLIGHT_BLUE		= ConvertColor(30	,150, 220);		//	�����Ķ���
const WORD FONT_COLOR_BLIGHT_VIOLET		= ConvertColor(220	,30	, 150);		//	���� �����
const WORD FONT_COLOR_BLIGHT_ORANGE		= ConvertColor(220	,150, 30)	;	//	���� ��������
const WORD FONT_COLOR_BLIGHT_RED		= ConvertColor(220	,150,150);
const WORD FONT_COLOR_BLIGHT_YELLOW		= ConvertColor(30	,220,30);

const WORD FONT_COLOR_GREEN				= ConvertColor(50	,230, 50);		//	�ʷϻ�
const WORD FONT_COLOR_RED				= ConvertColor(230,50	, 50);		//	����
const WORD FONT_COLOR_BLUE				= ConvertColor(50	,50	, 230)	;	//	�Ķ���

const WORD FONT_COLOR_SPECIAL			= ConvertColor(101,169, 18)	;	//	Ư���� ��� ( �������)
const WORD FONT_COLOR_NUMBER			= ConvertColor(221,230, 206);		//	���� ����

const WORD FONT_COLOR_NORMAL			= ConvertColor(141,144, 132);
const WORD FONT_COLOR_RECT				= ConvertColor(221,230, 206);
const WORD FONT_COLOR_BUTTON			= ConvertColor(255,61	, 65);

const WORD FONT_COLOR_SKILL				= ConvertColor(4	,152, 54);		//	��ų ����

const WORD FONT_COLOR_MAX				= ConvertColor(255,255, 0	);		//  �ɷ�ġ �ְ��ϰ�� ( ����� )

const WORD FONT_COLOR_BORA				= ConvertColor(230,90	, 230)	;		// party chat color

const WORD FONT_COLOR_HIGH_RARE_MAIN	= ConvertColor(230	, 230 , 80)	;		//����
const WORD FONT_COLOR_HIGH_RARE_DISPLAY	= ConvertColor(150	, 230 , 50);		//���� ������ ����Ʈ

const WORD FONT_COLOR_RARE_MAIN			= ConvertColor(0	,240, 240)	;		//����
const WORD FONT_COLOR_RARE_DISPLAY		= ConvertColor(80	,183, 255);		//���� ������ ����Ʈ 
const WORD FONT_COLOR_RARE_PLUS_POINT	= ConvertColor(255,255, 75);		//���� ������ ����Ʈ 
const WORD FONT_COLOR_RARE_MINUS_POINT	= ConvertColor(255,0	, 0	);		//����

const WORD FONT_COLOR_WHITE				= ConvertColor(233,233,233);//020725 lsw
const WORD FONT_COLOR_EXP_PERCENT		= ConvertColor(220,230,9);//020725 lsw
extern DWORD Convert16to32(const WORD wColor);

////////////////////////////////////////////////////////////////////////////////////////////////////
#define FS_BIG		0		// ū ��
#define FS_MIDDLE	1		// �߰� ��
#define FS_SMALL	2		// ���� �� ( default )
#define FS_MINI		3		// ��Ƽ�� ��
#define FS_MICRO	4		
#define DWORD uint32_t
/////////////////////////////////////////////////
#define INN_SINGLE	10
#define INN_DOUBLE	20
#define INN_MULTI	30

///////////////////////////////////
#define LEFT		0
#define RIGHT		1

#define SCROLL_UP			0
#define SCROLL_DOWN			1

/////////// �� �̹��� /////////////////////
#define FACE_MAX 10
#define ADD_CHA_PIC 20	// 0906 kkh �߰� 

#define IIT_FRIEND					920
#define IIT_POTAL					930
#define IIT_PARTY_BUTTON			940
#define IIT_GUILD_ITEM				950
#define IIT_GUILD_CHANGE_INFO		960
//Menu_Image.txt �� �����Դϴ�. ���ļ��� �ƴϵ˴ϴ�.(��ġ�� ���ø� ������ ��ü�� �ȵ˴ϴ� �Ѥ�;..)
enum ImageFileIndex
{
	DEFAULT_MENU				=	0	,
	GOD_MENU					=	100,
	TIME_MENU					=	110,
	MEM_BOOK					=	120,
	MAIL_MENU					=	130,
	START_ETC					=	140,
	RESET_JOB_ETC				=	141,
	CHAT_MENU					=	150,
	MAGIC_BOOK_MENU				=	160,
	MAIN_ETC					=	170,
	SMALL_MAP					=	180,
	START_MAP					=	190,
	SMALL_ETC					=	200,
	SKILL_MENU					=	210,
	PUBLIC_BBS					=	220,
	HUNTER_MENU					=	230,
	QUEST_MENU					=	240,
	FAME_MENU					=	250,
	CHARACTER_COLOR_CHANGE		=	260,
	ITEM_MAKE_IMAGE				=	270,
	NATION_BBS_MENU				=	280,
	START_FIGHT_MAP				=	290,
	STOP_FIGHT_MAP				=	300,
	VOTE_MENU					=	310,
	GUILD_MARK					=	320,
	GUILD_ADMIN					=	330,
	GUILD_LIST					=	340,
	START_ETC2					=	350,
	GUILD_ADMIN_ETC				=	360,
	HELP_PCX					=	370,
	BANK_PCX					=	380,
	SALVATION_PCX				=	390,
	SALVATION_PCX_ANI			=	400,
	TEAM_BATTLE					=	410,
	SELECT_SERVER				=	420,
	CHANGE_MONEY				=	430,
	GUILD_WAIT					=	440,
	MERCHANT_IMG				=	450,
	FIGHT_MAP_TIME				=	460,
	FIGHT_MAP_WIN				=	470,
	FIGHT_MAP_LOSE				=	480,
	MAIN_ETC3					=	490,
	GUILD_MENU_IMG				=	500,
	GM_2ND_SKILL_IMG			=	510,
	NATION_BATTLE_BBS_IMG		=	520,
	NATION_INFO_IMG				=	530,
	NATION_VOTE_IMG				=	540,
	NATION_BUDAE_IMG			=	550,
	NATION_NOW_IMG				=	560,
	NATION_LADDER_IMG			=	570,
	NATION_JOIN_IMG				=	580,
	NATION_GOTO_WAR_MAP_IMG		=	590,
	NATION_GUEST_IMG			=	600,
	NATION_MSGBOX_IMG			=	610,
	NATION_DONEBOX_IMG			=	620,
	NATION_TODAY_NOTICE_IMG		=	630,
	NATION_WAR_RESULT_IMG		=	640,
	NATION_WAR_START_IMG		=	650,
	DUAL_MARK_IMG				=	730,
	ITEM_MAKE_RARE_SELECT_IMG	=	740,
	NEW_CHAT_IMG				=	750,
	DUAL_IMG					=	760,
	TUTORIAL_DISPLAY_IMG		=	770,
	RUNE_IMG					=	780,
	RUNE_EFFECT1				=	790,
	RUNE_EFFECT2				=	800,
	RUNE_EFFECT3				=	810,
	RUNE_EFFECT4				=	820,
	RUNE_EFFECT_DEFAULT			=	830,
	SKIN_MENU					=	840,
	GAMBLE_IMG					=	850,
	IIT_EXP_GAUGE				=	860,
	GAMBLE_EFFECT				=	870,
	TUTORIAL_DISPLAY_TMP_IMG	=	900,
	IIT_GUILD_MEMBER_LIST		=	910,
	ABILITY_REDISTRIBUTION_IMG	=	880,
	LOCALWAR_JOIN_IMG			=	881,
	LOCALWAR_OK_IMG				=	882,
	EVENT_LOCALWAR_OK_IMG		=	883,
	EVENT_LOCALWAR_MOVE_IMG		=	884,
	WAR_START_IMG				=	885,
	WAR_MOVE_IMG				=	886,
	WAR_STATUS_IMG				=	887,
	MONSTER_WAR_IMG				=	888,	// BBD 040311 ���� ������ ���� ȭ�� ���� ǥ�ÿ� �̹���

//���⼭ ���� Menu_Image.txt �� �����Դϴ�. ���ļ��� �ƴϵ˴ϴ�.(��ġ�� ���ø� ������ ��ü�� �ȵ˴ϴ� �Ѥ�;..)
	COMBAT_SKILL_ICON_IMG		=	10002,
	COMBAT_SKILL_POINT_IMG		=	10003,
	MAINSTART_LOGIN_IMG			=	10005,
	START_LOGO_IMG				=	10006,
	PARTY_IMG					=	10007,	
	STATUS_IMG					=	10008,	
	INVENTORY_IMG				=	10009,	
	MAININTERFACE_IMG			=	10010,
	CONNECTING_IMG				=	10011,
	SHOW_CREDIT_IMG				=	10012,
	SELECT_CHARACTER_IMG		=	10013,	
	MANA_IMG					=	10014,	
	DIVINE_IMG					=	10015,	
	HP_IMG						=	10016,	
	NEW_START_MAP_IMG			=	10017,	
	NEW_EMOTE_IMG				=	10018,	
	NEW_GHOST_TO_ALIVE_IMG		=	10019,	
	NEW_SYSTEM_IMG				=	10020,	
	NEW_CHAT_BACK_IMG			=	10021,
	SCENARIO_IMG				=	10022,
	MERCHANT_FINDER_IMG			=	10023,
	MERCHANT_BUY_IMG			=	10024,//021008 lsw
	NEW_DEST_POINT_IMG			=	10025,
	TXT_BG_BLACK_IMG			=	10026,
	NEW_TUTORIAL_TEMP2			=	10027,
	DEL_CHAR_COMFORM_IMG		=	10028,
	TXT_BG_BLUE_IMG				=	10029,	
	TXT_BG_RED_IMG				=	10030,
	TXT_BG_BLUEGREEN_IMG		=	10031,	
	TXT_BG_BROWN_IMG			=	10032,	
	TXT_BG_GREEN_IMG			=	10033,	
	SELECT_TUTORIAL_LOGIN_IMG	=	10034,
	MERCHANT_SELL_INFO_IMG		=	10035,
	MERCHANT_SELL_REGISTER_IMG	=	10036,
	MERCHANT_RESULT_TAKE_IMG	=	10037,	
	SCRIPT_TIMER_COUNTER_IMG	=	10038,	//021025 kyo Ÿ�̸Ӷ� ī���ͱ׸�
	MERCHANT_DIRECT_EXCHANGE_IMG =	10039,
	EXCHANGE_IMG				=	10040,//��ȯâ �׸�
	MAN_TO_MAN_IMG				=	10041,//���� �׸�
	SKILL_MASTER_QUEST_IMG		=	10042,//
	SKILL_MASTER_KIND_SELECT_IMG=	10043,//�⼺ ���� �����
	SKILL_MASTER_MAKE_IMG		=	10044,//����� �׸�
	EXCHANGE_BOND_MONEY_IMG		=	10045,//����ȯâ
	SKILL_MASTER_MARK_IMG		=	10046,//�⼺ ���� ��ũ 
	MERCHANT_BBS_BACK_IMG		=	10047,//���ΰ��� ä��â
	NEW_BANK_IMG				=	10048,//���ο� ���� 

	LUCK_MENU_IMG				=	10049,//��� �޴�
	LOTTO_MENU_IMG				=	10050,//�ζ� �޴�.
	LUCKY_EVENT_IMG				=	10051,//��÷ Ȯ��â.

	ARENA_IMG					=	10052,
	OBSERVER_IMG				=	10053,
	ITEMMALL_IMG				=	10054,
	MANTLEDYE_IMG				=	10056, // Fintio 23/06/08 - mantle dye menu

};

#define NOT		0
#define OK		1

////////////////////////////////////////////////////////////////////////////
// class define 
enum ClassType//021008 lsw
{
	CLASSTYPEMIN = -1,
	WARRIOR		= 0,
	THIEF		= 1,
	ARCHER		= 2,
	WIZARD		= 3,
	PRIEST		= 4,
	CLASSTYPEMAX = 5
};

/////////////////////////////////////////////////////////////////////////////
// job define 
#define		J_FAMER				20
#define		J_MINNER			21
#define		J_FISHER			22
#define		J_LUMBERJACK		23
#define		J_BUTCHER			24
#define		J_HERB_DIGGER		25
#define		J_HERDSMAN			26
#define		J_CARPENTER			27
#define		J_BLACKSMITH		28
#define		J_COOKER			29
#define		J_TAILOR			30
#define		J_BOWCRAFT			31
#define		J_ALCHEMIST			32
#define		J_CANDLEMAKER		33
#define		J_MERCHANT			34

/////////////////////////////////////////////////////////
//	���� ���� define ( location )
#define		L_VILLAGE			0
#define		L_HELTENT			1
#define		L_HUDAIN			2
#define		L_GREV				3


/////////////////////////////////////////////////////////
// ���� define
#define N_NEUTRAL			0
#define N_VYSEUS			3
#define	N_ZYPERN			4
#define	N_HEGEMONIA			5
#define	N_YILSE				6
#define N_NATION_ALL		7


///////////���� ���ÿ� ���� ���� ��ȣ ���� //////////////
//					( select city )
enum eSelectMap
{
	SC_HELTERNT_CASTLE	=1,
	SC_HELTERNT_VILLAGE	=2,
	SC_HUDAIN_RIVER		=3,
	SC_GRAY_MOUNTAIN	=4,
	SC_CALAHIL			=5,
	SC_BAISUS_IMPEL		=6,
	SC_RENUS		 	=7,
	SC_DEL_HAPPA		=8,
	SC_IRAMUS			=9,
	SC_DIPH_UBEN		=10,
	SC_SCHOLIUM			=11,
	SC_TYREN			=12,
	SC_BARANTAN			=13,	
	SC_SCHOLIUM2		=14,		// �߰��� StartMapPosition�� �߰��Ѵ�.	// 20�� ������..//020815-2 lsw
	SC_RECENT_LOGOUT = 99,
};

//��Ƽ �޴��� ������, ���� �޴����� ����
#define EDIT_NAME					0
#define EDIT_ID						1
#define EDIT_PASS					2

#define ITEM_BUY		100			// BUY �޴� ���Ҷ�..
#define ITEM_SELL		200			// SELL �޴� ���Ҷ�..
#define ITEM_REPAIR		300			// REPAIR �޴�..
#define ITEM_BUY_MALL	400

// ĳ���� nCharacterAbility�� �迭 ÷�ڿ� ����ϱ� ���� define
enum CharInfoType
{
	STR			=0,		// ��
	DEX			=1,		// �ǰ�
	CON			=2,		// ��ø��
	WIS			=3,		// ����
	INT_		=4,		// ����
	CHA			=5,		// �ŷ�
	MOVP		=6,		// �̵���
	ENDU		=7,		// �γ�
	MOR			=8,		// ���
	WSPS		=9,		// ���� �Ź�
	LUCK		=10,		// ���
	HEALTH		=11,		
	MOV			=12,
	AC			=13,
	FAME		=14,
	DAMAGE		=15,
	HIT			=16,
	TACTICE		=17,
	MONEY		=18,
	LVPOINT		=19,
	LEV			=20,
	EXP			=21,
	HP			=22,
	MAX_HP		=23,
	MP			=24,
	MAX_MP		=25,
	SP			=26,
	MAX_SP		=27,
	CP	  		=28,
	MAX_CP		=29,
	BP			=30,
	MAX_BP		=31,
	R_FIR		=32,
	R_ICE		=33,
	R_ELE		=34,
	DAM_MIN		=35,
	DAM_MAX		=36,
	HIT_RAT		=37,
	DEF_RAT		=38,	
	TAC_L1		=39,
	TAC_L2		=40,
	TAC_L3		=41,
	TAC_L4		=42,
	TAC_L5		=43,
	TAC_L6		=44,
	TAC_L7		=45,
	TAC_L8		=46,
	TAC_L9		=47,
	TAC_L10		=48,
	TAC_L11		=49,
	TAC_L12		=50,
	TAC_L13		=51	,
	TAC_E1		=52,
	TAC_E2		=53,
	TAC_E3		=54,
	TAC_E4		=55,
	TAC_E5		=56,
	TAC_E6		=57,
	TAC_E7		=58,
	TAC_E8		=59,
	TAC_E9		=60,
	TAC_E10		=61,
	TAC_E11		=62,
	TAC_E12		=63,
	TAC_E13		=64,
	BANKMONEY	=65,
	FAITH		=66,
	VIEWGHOST	=67,//020314 lsw
	VIEWBOMB	=68,
	VIEWTRAP	=69,
	CIB_POTION_BOX	=70,		// YGI 020418
	NATION_PARTY    =71,
	GUILD_PARTY     =72,
	ADD_EXP = 73, 
};
//######## SCharacter ����ü�ȿ� nCharacterData �迭�� ����� ����ü ##########

#define GENDER				0
#define FACE				1     
#define ARIGEMENT			2
#define CLASS				3
#define SPELL				4
#define TACTICS_MAN			9
#define TACTICS_WOMAN		10 
#define JOB					11  

//######## SCharacter ����ü�ȿ� disease �迭�� ����� ������ ( OR ��� ) ##########
#define		DISEASE1		0x00000001
#define		DISEASE2		0x00000002
#define		DISEASE3		0x00000004
#define		DISEASE4		0x00000008
#define		DISEASE5		0x00000010

#define		PREVENTION1		0x00000020
#define		PREVENTION2		0x00000040
#define		PREVENTION3		0x00000080
#define		PREVENTION4		0x00000100
#define		PREVENTION5		0x00000200


//######## tactics �� ��� #############
#define		CRAPPLE				0
#define		SWORD				1
#define		ARCHERY				2		
#define		FENCING				3
#define		MACE				4	
#define		PIERCE				5		
#define		WHIRL				6		
#define		HURL 				7		
#define		PARRYING			8	
#define		D_SWORD				9			
#define		D_MACE				10		
#define		MAGERY				11
#define		ORISON				12

///////////////////////////////////////////////////
#define		WIZARD_SPELL				0
#define		PRIEST_SPELL				1		
#define   ACTIVE_COMBAT       3
#define   PASSIVE_COMBAT      4
#define		RECOVERY_COMBAT   5  	// 030415 kyo
//////////////////////////////////////////////////
#define		FEMALE				0
#define		MALE				1


// ���� ���ϸ��̼ǿ��� ����
#define		WARRIOR			0
#define		THIEF			1
#define		ARCHER			2
#define		WIZARD			3
#define		PRIEST			4

////////////////////////////////////////////////////////////////
// �� ( GOD )
#define		EDELBLHOY		1
#define		TEFFERY			2
#define		LETTY			3
#define		OREM			4


/////////////////////////////////////////////////////////////////
// �޴� ���� type
#define EXPLAIN_MAININTERFACE		1
#define EXPLAIN_TACTICS_MAN			2
#define EXPLAIN_TACTICS_WOMAN		3

//////////////////////////////////////////////////////////////

//�޴����� ����ü ����
//FT_HIDE_NOMAL_GROUP_PUT ,FT_HIDE_NOMAL_ONE_PUT �� ����� ����ü ����
typedef struct 
{ 
	int nType;                //�޴� ������� ó�� Ÿ���� ������ �ִ�. �� FT_TEXT_PUT, FT_TEXT_PUT_CENTER, FT_NOMAL_PUT, FT_NOMAL_PUT_CENTER�� ����
	int x;                    //�ڽ��� �����ϴ� �޴���������� ���� �޴��� ���� ������� x����� ��ǥ
	int y;					  //�ڽ��� �����ϴ� �޴���������� ���� �޴��� ���� ������� y����� ��ǥ 
	char temp[MAX_STRING];    //���ڸ� ����� ��� ����� �� 
	int nImageNumber;         //�̹��� ��ȣ
	int nData;                //�̹��� �ܿ� �ٸ� ������ ���
	BOOL bCheak;              //����üũ�� ���� �׳� ����� ������ ��Ÿ��
	RECT rCheakBox;           //���콺�� ����ؼ� üũ�Ұ������ ��ǥ
	BOOL bShow;               //�� �̹����� ����ؾ������� ��Ÿ��
	bool bRect;				// Rect�� ���콺 �浹�� ��� 
}SHN;

//�޴� ������� ����ü ����
typedef struct
{ 
	int nType;			       //�޴� ������Ұ� �ؾ��� Ÿ�Թ�ȣ ����
	int x;					   //������Ұ� ���� ���� �޴��� ���� ������� x�� ��ǥ
	int y;					   //������Ұ� ���� ���� �޴��� ���� ������� y�� ��ǥ
	int nImageNumber;		   //��������� �̹��� ��ȣ, DP_ ������ ����Ѵ�
	int nRectImage;				// ���콺 �浹���� �̹���
	int	nImageType;				//������ �̹����� ��������Ʈ ���� Ÿ��
	char temp[MAX_STRING];     //��������� ���� ���� 
	int nWillDo;	           //������Ұ� ���õ������ �ؾ�����(�ٸ� �޴�ȣ��...),FT_HIDE_SCROLL_PUT�϶��� ��ũ���� FT_HIDE_NOMAL_ ������Ҹ� ����
	int nSpecialWillDo;        //�ܼ��� �ٸ��޴�ȣ���� �ƴ� Ư���� ���� �ؾ��� ��� ���
	BOOL fLButtonDown;         //���� ��ư�� ���Ⱦ������� ��Ÿ��
	BOOL fCheakFlag;           //������Ұ� üũ�ƴ����� ��Ÿ�� (���콺�� ���ȴٰ� �������� üũ) //FT_DATA_PUT������ ������Ұ� ������ �ִ°��� �������� �������� ��Ÿ���ش�
	BOOL fRButtonDown;
	BOOL fRButtonCheck;
	BOOL fRectMouse;		   //���� �ȿ� ���콺 �����Ͱ� �ִ��� ������...
	int RectCount;
	int DragCount;				// ��ư�� ������ �ִ� ī��Ʈ 
	RECT rCheakBox;	           //�ڽ��� ���� �޴��� ������� ���� üũ���� �����ϰ� �ִ�.(�迭�� üũ�̸� ���� �������� �ִ�)
	int nSHideNomalNumber;     //FT_HIDE_NOMAL_ �� ����� ����ü ��ȣ
	int nSHideNomalCount;      //FT_HIDE_NOMAL_ �� ����� ����ü�� ����
	int nSHideNomalStart;      //FT_HIDE_NOMAL_ �� ����Ҷ� �������� ����� ������� ����� ���������� ����
	int nShideNomalPlus;       //�ѹ��� �����ټ� �ִ� �ִ�ġ nSHideNomalCount�� ���� ���ϼ��� �ִ�
	int *nValue;				// ������ �ޱ�����
	void *m_lpTemp;
	void (*m_pFunction)();
} SFIELD;

//�޴� ����ü ����
typedef struct
{
	int		Id;
	bool	bActive;            //���� �޴��� Ȱ��ȭ �ƴ����� ��Ÿ��
 	int		x;					//���� �޴��� ���� ȭ�鿡 ������� x�� ��ǥ
	int		y;					//���� �޴��� ���� ȭ�鿡 ������� y�� ��ǥ
	int		xl;
	int		yl;
	int		nImageNumber;       //���� �޴��� �̹��� ��ȣ
	int		nImageType;
	BYTE	CheakType;          // ���۸޴� : ����üũ�� rect�ϴ��� �迭�� �ϴ����� �Ǵ�, ���θ޴� : ��ó���� �ѹ��� ó���Ҽ� �ְ�
	int		nFieldCount;        //���� �޴��� ������ �ִ� �޴� ��������� ����
	int		nTemp;				//�� �ܰ� �޴� ��ȣ�� ������ �ִ�.
	int		work;				//���� ���� �� �ӽ� ����
	int		key;				// �������� �ʴ� �ӽ� ���� 
	int		key2;				//����Ű ������ ��...
    SFIELD nField[MAX_FIELD];	//�޴����� ��� �迭
} SMENU;


//##########################################################
//					����ü �� Ŭ����                      //
//##########################################################

#define		MAX_NAME			41
#define		MAX_MOTHER_SKILL	45
/////////////////////////////////////////////////////////////
//		���� �ý��ۿ��� ����� ������ 
/////////////////////////////////////////////////////////////

#define		RESIDING		0			// �ְſ�
#define		GUILD			1			// ����
#define		NOTRESIDING		2			// ���ְſ�

#define		MAX_FAMILY		20
#define		MAX_MATE		20
#define		MAX_GUEST		20
#define		MAX_NAME_		31

#define		MAX_HOUSE		40			// �̹��� �ִ�
	
#define		HOUSE_MAX_TABLE	32			// ���̺� �ִ�
#define     MAX_HOUSENAME   21

/////////// �� ���� ������ ��ɿ��� ��� /////////////////
#define OPEN_HOUSE	0
#define CLOSE_HOUSE	1


//////////// �ǹ� Ŭ���� ///////////////
class CHouse
{
protected :

public :

	int		m_Id;
	int		m_Image;
	char	m_Name[MAX_NAME_];
	char	m_HanName[MAX_NAME_];
	int		m_Width;				// ����
	int		m_Wall;					// ���� ����
	int		m_Roof;					// ���� ����
	int		m_LV;					
	int		m_ForResiding;			// �ְſ�
	int		m_ForGuild;				// ����
	int		m_ForNotResiding;		// ���ְſ�
	int		m_Forforth;				// ����
	int		m_ForFifth;				// ����
	int		m_Item;					// ���� ������ȭ
	int		m_Price;				// ����
	int		m_Duration;				// ������
	int		m_DurationPerTime;		// ������ ���Һ�		(*1000)
	int		m_Custody;				// ������
	int		m_Safety;				// ������
	int		m_Maxfamily;			// �������� �ִ뷮
	int		m_MaxMate;				// ģ�� �ִ뷮
	int		m_Hiling;				// �ð��� �Ҵ緮

	char	m_Explain1[30];
	char	m_Explain2[30];
	char	temp[30];

	char m_Owner[MAX_HOUSENAME];  // �� �ǹ� ����
	char	m_Kind[MAX_NAME_];		// �ǹ� ����
	int		m_Kind2;
	char	m_HouseKind[30];
	int		m_DurationMax;
	int		m_Key;					// �� �ǹ� ����

	char m_Family[MAX_FAMILY][MAX_HOUSENAME];
        char m_Mate[MAX_MATE][MAX_HOUSENAME];
        char m_Guest[MAX_GUEST][MAX_HOUSENAME];


public:
	CHouse();
	char	*GetDur();
	void	SetExplain(char *, char *);
	void	ReadHouse( char * );
};


class CMyHouse
{
protected : 
public :
	CHouse		data;		//	CHouse class�� ����Ŵ
	DWORD		date;		//	�� �ǹ��� ���� ��¥�� ����Ѵ�.
	CMyHouse	*next;
	CMyHouse	*prev;

public :
	CMyHouse() : date(0) { next = NULL; prev = NULL; }
	void FreeAll();
	void AddHouse( CHouse house, int *count );
	void DeleteHouse( int *count );
	int	Repair();
	int GetImageNumber( ) { return data.m_Image; }
};

typedef struct tagCharacterParty
{
	int		m_Id;
	char	m_Name[21];
	int		m_Gender;
	int		m_ImageNo;
	int		m_Lv;
	int		m_Str;
	int		m_Class;
	BYTE	m_Condition;
	int		m_Hp;
	int		m_HpMax;
	
	int		m_Join;			// ������ ���³�...? 0->1 : ����, 1->0 ���� ����
	int		m_JoinPrev;

	int		m_JoinOk;		// �Ѵ� ��Ƽ�� �δ´�.
	int		m_Sight;		// �� �þ߿� ��� �Դ�.

	int		x;
	int		y;
	int		xy_on;
	
	LPCHARACTER ch;		// �þ߿� ���� ������, 
}CharacterParty, *LpCharacterParty;

typedef struct tagNationParty
{	//< CSD-030723
	short int	On;
	int nationcode;
	LPCHARACTER ch;
}NationParty,*LpNationParty;	//> CSD-030723

#define DN_TYPHOID		0	// ��ƼǪ�� (Typhoid Fever)		DN_ ( Disease Name )
#define DN_YUT			1	// ���༺ ������(yut)
#define DN_SITOTOXISM	2	// ���ߵ� (sitotoxism)
#define DN_SPIRITLESS	3	// ������� (Spiritless)
#define DN_COLD			4	// ����(Cold)
#define	DN_VENEREAL		5	// ����(Venereal Disease)

#define CC_NORMAL	0
#define CC_POISON	1	// ���� �ɸ� ����
#define CC_STONE	2	// ��ȭ
#define CC_DAZE		3
#define CC_FROZEN 4	// ��������.
#define CC_CURSE	5	// Ȱ������ ����
#define CC_SLOW		6
#define CC_DEATH	7   // �׾��ִ� ����.

////////////////// ( character sleep condition ) //////////////////////////
#define CSC_AWAKE		0			// ���� �ִ� ����
#define CSC_SLEEP		1			// ���ڰ� �ִ� ����
#define CSC_REST		2			// �޽��� ���ϰ� �ִ� ����

const int  MAX_SKILL_ID  = 200;

typedef struct SkillExp
{
	DWORD skillexp	: 20;
	DWORD rare		: 12;
}t_SkillExp,*LPSKILLEXP;

typedef struct SkillExp2
{
	DWORD	skillexp	:	20	;//��ų ����ġ
	DWORD	makecount	:	6	;//���� ����
	DWORD	day 		:	3	;//����� ��¥
	DWORD	month 		:	3	;//����� ��¥
}t_SkillExp2,*LPSKILLEXP2;

typedef struct SkillExp3//021030 lsw
{
	DWORD	skillType	:	8	;//��ųŸ��
	DWORD	year		:	12	;//�⵵
	DWORD	day 		:	12	;//��¥
}t_SkillExp3,*LPSKILLEXP3;

enum eRESIST_TYPE
{
	RT_POISON	=0,
	RT_CURSE	=1,
	RT_HOLY 	=2,
	RT_FIRE		=3,
	RT_ICE		=4,
	RT_ELECT	=5,
	MAX_RESIST_TYPE = 6
};

//-----------------------------------------------------------
//	ĳ���� ���̺�	-	�޴����� ����� �͸�
//-----------------------------------------------------------
typedef struct 
{ 
	__int16 nLevel;
	DWORD	nExp;				
	DWORD	nExpNext;

	char	sCharacterName[MAX_NAME];
	char	nGuild[MAX_NAME];

	int		nCharacterData[MAX_CHARACTERDATA];    // ����,��,��ġ��,Ŭ����,����,�Ӹ���,�Ǻλ�,�ʻ�,��Ļ�,��ƽ����,��ƽ����,����
	int		nCharacterAbility[20];				//STR,CON,DEX,WIS,INT,CHA,MOVP,ENDU,MOR,CONV,LUCK, WSPS,
	int		nAbilityMax[20];					//STR,CON,DEX,WIS,INT,CHA,MOVP,ENDU,MOR,CONV,LUCK, WSPS,
	
	DWORD	nMoney;		// ��

	int		nCharacterFace;						// ĳ���� �� ǥ���� ���� ��8������ �ִ�
	int		nCharacterHP;						// HP�� MP�� �����ϴ� ����
	int		nCharacterMP;
	int		nCharacterMAXHP;					// HP�� MP�� �ִ�ġ
	int		nCharacterMAXMP;
	int		nCharacterSP;				
	int		nCharacterMAXSP;

	BYTE	condition;		// ĳ������ ���¸� �����ϴ� ���� 0�̸� �����̴�. 

	int		nWeight;					// ��� �ٴϴ� �������� ����
	DWORD	BankMoney;					//	
	int		LoanTotal;
	int		LastLoan;
	DWORD	LastLoan_Time;

//	int		nJobSkill_Lv[31];				// ĳ���� ������ų �ɷ�ġ
//	DWORD	nJobSkill[MAX_MOTHER_SKILL];					// ��� ����ġ
	t_SkillExp	nJobSkill[MAX_MOTHER_SKILL];					// ��� ����ġ//011013 lsw 
//	int		nJopSkillHit[31];				// ������

	BYTE	MagicId[MAX_MAGIC];					// ����� �� �ִ� ����
	BYTE	SkillId[MAX_SKILL_ID];					// �Ҽ� �ִ� ��ų
	short int	ItemId[MAX_LEARN_ITEM];			// ����� �ִ� ������
	int		ItemIdCount;					// ���� ����� �ִ� ������ ��...

	int		curr_magic[MEMORIZE_MAX];		// ���� �޸������ �� ����
	int		curr_magic_ct;					//						   �� �Ѽ�

	int		memorize_point;					// ���� �޸������ �ϰ� ���� ����Ʈ
	int		memorize_point_max;				// �޸������ �� �� �ִ� ����Ʈ �ִ�ġ	// ���� wsps+3
	

//	int		TacticsLv[13];					// ��ƽ�� ����
	DWORD	TacticsExp[13];					// ��ƽ�� ����ġ
	DWORD	TacticsExpNext[13];

	short naBasicResist[MAX_RESIST_TYPE];
	short naExtentionResist[MAX_RESIST_TYPE];

	int		LvUpPoint;
	WORD 	EachUp[11];

	int		body_r;			//  �ܸ� ��
	int		body_g;					
	int		body_b;
	int		cloth_r;
	int		cloth_g;
	int		cloth_b;

	int		mantle_r; // finito 23/06/08 - mantle dye
	int		mantle_g;
	int		mantle_b;

	int		nk3, nk4, nk6;		// ���� ���밨

	int		age;			// ����

	int		tatics;			// ���� ���� �������� ��ƽ��
	int		beat;			// ���� ���� ������ ��
	int		ac;				// ����
	int		mov;			// �������ͽ� â�� ��Ÿ���� �̵���

	int		accessory[4];	// ���� �����ϰ� �ִ� ����, ���� ���...

	BYTE	believe_god;			// ����� �� 0: ����... ~
	int		faith;					// �žӽ�
	char	much_supper;			// �������� ���� �����Ҷ� �� ����
	
	int		dmg_count;		// first	// 3d2 ( 1~2�� �ֻ����� 3�� ������.)
	int		dmg_max;		// second

//	BYTE	dmg_count_of_inv[8];
//	BYTE	dmg_max_of_inv[8];		// ���� ���� �ʿ���� Ÿ�� ����Ҷ� �׶����� ����Ѵ�.

	bool	disease[6];			// ������ �ɷȴ���...
	DWORD	disease_no_time[6];

	DWORD	time_maintain1;				// ���� ȿ�� ���� �ð�
	DWORD	time_maintain2;				// �ɷ� ȿ�� ���� �ð�
	DWORD	time_maintain3;				// ���� ��ȯ ȿ�� ���� �ð�

	CharacterParty	party[6];			// ���� �ι��� �̸�
	CharacterParty	relation[3];
	CharacterParty	employment[6];
	NationParty     NationParty;
	int		nAbilityPlusOfPotion[20];	// �Դ� �ſ� ���� �Ŀ��� ( ��ġ��� �� �ȴ�. )
	DWORD	dwAbilityPluseTime[20];		// �Դ� �ſ� ���� �Ŀ����� ��� �� ���� �ð�.
	int		nAbilityPlusOfWeapon[20];	// ������ ������ ���� �Ŀ��� ( ��ġ�� ���� )
	// ���� �κ� �ʿ�

	BYTE	magic_casting_time[200];		// ����:��*10

	int		sleep;			// �ڰ� �Ͼ� ���ų� ���� �������� ���...

	int		class_famous;
	int		job_famous;
	int		nation;

	//char aStepInfo[20];
	int m_nQuestStep;	// ������ ����Ʈ
	int m_nSelectRune;
	int potion_box1;		// ���ǹڽ�

	// 020428 YGI acer
	DWORD attack_time;

}SCharacter;

typedef struct tag_client_candidate		// �ĺ� ����ü
{
	char		name[20];
	short int	number;				// ���� ��ȣ

	int			lv;
	int			class_type;
	int			gender;
	int			nation;
	int			guild_code;
	int			face;
	
	int			score;		// ��ǥ�� 
}k_client_candidate;

extern CHouse	House[HOUSE_MAX_TABLE];
extern CHouse	*house_sub_residing;
extern CHouse	*house_sub_guild;
extern CHouse	*house_sub_not_res;

extern int		house_res_ct;
extern int		house_guild_ct;
extern int		house_not_res_ct;

extern SCharacter SCharacterData;
extern int g_MouseInMenu;	// 031110 YGI
extern int g_MouseInMenuPreviousFrame;
extern int IsMouseOnMenu();




extern int CheckAndSetPartyOnMySight( LPCHARACTER ch, int type );		// ��Ƽ�� �þ߿� ���Դ��� �������� �Ǵ�.		1: ���ö�, 0: ������
extern  bool CheckGuildCode( int guild_code );
extern  bool CheckGuildMaster( LPCHARACTER ch );
extern  bool IsSubMaster( LPCHARACTER ch );
extern  bool IsSubMaster( int guild_code, char *name );
#endif
