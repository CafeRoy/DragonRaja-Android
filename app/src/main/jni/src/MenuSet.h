#ifndef __MENUSET_H__
#define __MENUSET_H__

#include "StdAfx.h"
#include "MenuNetwork.h"		//021030 YGI
#include <string>
#include <vector>
#include <memory>
#define DUR_RATIO		.70		// repair���� ����� duration ����
#define RARE_DUR_RATIO	.50	//020505 lsw ���� ���� ����

#define RED_COLOR		RGB(255,0,0)
#define YELLOW_COLOR	RGB( 255, 255, 0 )
#define BLUE_COLOR		RGB( 0, 0, 255 )
#define GREEN_COLOR		RGB( 0, 255, 0 )
#define GRAY_COLOR		RGB( 100, 100, 100 )

extern const int MAX_START;//020815-2 lsw

extern int GetItemMuchByMuchItem( int item_no );		// ������ �ִ� �������� ���� �ľ��ϱ�

#define SCRIPT_IS_READ_NATION_BBS		46
////////////////////////////////////
#define MAX_BUY_ITEM	50
extern void MenuSet();
extern void SmallMenuSet();
extern bool	g_MagicMenuOn;
extern bool	g_explain_skill_on;
extern int	g_explain_skill;
extern bool	g_explain_magic_on;
extern int	g_explain_magic;
extern bool	g_explain_menu_on;
extern char	g_explain_menu[MAX_STRING];
extern int	magic_plus[MAX_MAGIC];		// ���� ����� ���� ����
extern int	magic_plus_count;			// �� ����
extern int	magic_plus_start;			// ���� �̹��� start�κ�
extern bool recv_learn_item;
extern int	danger_by_hunter;
extern int	man_to_man_start;
extern int	fight_map_stop;

extern int before_skill_no;

extern char explain_tactics[13][20];


extern Spr open_spr[75];
extern Spr spr_alphabet[36];

extern Spr	ch_armor_image_after[7][80];
extern Spr	ch_body_image_after[7][80];
extern Spr	ch_line_image_after[7][80];
extern Spr	ch_armor_image_after_b[7][80];
extern Spr	ch_body_image_after_b[7][80];
extern Spr	ch_line_image_after_b[7][80];

extern Spr	skill_spr[MAX_SKILL];

extern	BOOL LButtonDown;       
extern	POINT StateMousePoint;	
extern	SHN SHideNomal[MAX_SHN][MAX_SHN_FIELD];   
extern	SCharacter SCharacterData;
extern	CHARACTERLIST g_CharacterList;
extern	char y_temp[20];           

extern bool	no_active_box_yet;				// �������� ���� ���� ������ false set.
extern	int cDiceCount;			
extern	int y_delay;			
extern	int y_MenuFocus;		
								
extern	std::string sId;
extern  std::string sPassword;
extern	SMENU SMenu[MAX_MENU];

extern	BOOL	RButtonDown;
extern	BOOL	MouseDrag;
extern	int		LButtonCount;		

extern	int		y_PutBarSize;       
extern	bool	id_password;  
extern	bool	MessageOn;
extern	char	Message[MAX_STRING];

extern	RECT	rScreenRect;      
extern	int		YouGotItemFlag;

extern	int		MouseItemInRectOn;	
extern	POINT	MouseItemInRect;

extern	int		cRMoveMenu;       
extern	int		cLMoveMenu;       
extern	int		fRMoveMenu;   
extern	int		fLMoveMenu;	

extern	int		nPartyOther;
extern	int		nPartyState;
extern	int		nPartyShow;	

extern	struct InterfaceFlag	main_interface;

extern	int RectImageOn;		
extern	int nBankTemp;			

extern	int WearAble[13];
extern	int HandBackUp;
extern	int HandBackUptype;
extern	int HandItemOn;

extern	int aMagicItem[3][4][3][5];
extern	ItemAttr	aItemInventoryFull;		
extern	int SkillInventory[2][2][4];
extern	int Skill_Know_Inv[3][3];
extern	int aSellBuyItemPos[MAX_BUY_ITEM*3*5];
extern	int aSellBuyItemMax;

extern	bool	LeftShiftOn; 

extern	int		AbilityToInt[20];
extern	ItemAttr aSellBuyItemAttr[MAX_BUY_ITEM*3*5];
extern	int		storelistindex ;

extern ItemAttr itemMallAttr[MAX_BUY_ITEM*3*5];
extern int itemMallMax;

extern	bool	is_buymenu_On;
extern	int		motherSkillImageNo[2];
extern	int		motherSkillImageType[2];  // ��ų ���⿡�� ������ �̹��� Ÿ��
extern	int		SkillInventory[2][2][4];

extern	Spr		ch_armor_image_after[7][80];
extern	Spr		ch_body_image_after[7][80];
extern	Spr		ch_line_image_after[7][80];
extern	Spr		ch_armor_image_after_b[7][80];
extern	Spr		ch_body_image_after_b[7][80];
extern	Spr		ch_line_image_after_b[7][80];
extern	Spr		spr2[MAX_IMAGE2];	

extern	int		nItemOther;
extern	K_ITEM	i_DivideItem;		// �������� �Ӽ��� ��ġ�� �ִ�.
extern	const int y_ItemSubMenu[];//021014 lsw

extern	int		memorize[12];

extern void PutMapBright2_CPP( LPVOID dest, LPVOID alpha);
//===================================================================================================================================
extern void kein_TextPut(int x, int y,const std::string& sSource);
extern void FieldTypeTextPut(int mx,int my,int x,int y,char *temp);  
extern void LoadMenuData(int nNum);                 
extern void FreeMenuData(int nNum);                 
extern void PutImageCenter(int mx, int my, int x, int y, int nImageNumber);
extern BOOL MouseInRectCheak(int x, int y, RECT Box, BOOL flag = TRUE );
extern void SetCharacterData();  
extern void SetRect(RECT &rect,int left, int top, int right, int bottom);
extern void MoveTextPut(int x, int y, char *sSource, int type=0);

extern void SendBankInfo(int type,  DWORD money );
extern void RecvBankDeposit(DWORD);
extern void RecvBankRepayment(DWORD, DWORD, DWORD);
extern int	YesOrNo( char *s, char *title );


extern void		ItemRead();
extern void		PutItemIcon(int x, int y, int item_number, int delay=0, int effect = 2, int is_not_equip = 0);
extern int     CheckAndCraftVertical(int page, int column);
extern void		PutItemIconAni(int x, int y, int item_number);
extern int		GetItemAttr(int item_id, int attr);
extern char		*GetItemName( int item );

extern void		SendMoveItem( int item_no, POS s, POS t, bool sound = true);
extern void		GetItemAnimationNumber(int num, int &a, int &b, int &c, int &d, int &e, int &f, int &g);

extern bool		ReadSkillTable( );
extern bool		ReadHouseTable();
extern bool		LoadItemJoinTable();

extern void     PutMagicIconNotSameAsSpell(int x, int y, int num, int flag = 0, int type = 2);
extern void		PutMagicIcon( int x, int y, int num, int flag=0, int type=2 );
extern void		PutSkillIcon( int x, int y, int num, int flag=0, int flag2=2 );
extern void		ChangeSkillMenuInventory( int x );

extern	BOOL	MenuFocus(int menu);
extern	inline void RightClose();
extern	bool	IsLeftWindowOpen( );
extern	bool	IsRightWindowOpen();
extern	void	AbilityOpenWhenLevelUp();
extern	void	SetPartyMenu( int type );

extern	void	ChangeItem( int start );	// �ӽ� ������ �ٲٱ� 
extern	void	ItemSound( int item );
extern	void	EquipSound( int item_no );

extern	int		GetItemValueBuy( int item_id );
extern	int		GetItemValueSell( ItemAttr item );

extern	void	GetLearnSkillInv( int type, int start = 0 );
extern	void	GetLearnKnowlageInv( int start = 0 );
extern	int		mother[32][2];
extern	bool	IsLearnSkill( int m );
extern	void	MagicSetting();

extern	int		CalcTextToPage( int width, int height, char *source );
extern	void	BbsTextPut( int x, int y, int width, int height, char *source, int page );
extern	void	BbsTextPut2( int x, int y, int width, int max_line, char *source, int page );
//extern	void	CallOkCancelMessageBox( int menu, int x, int y, char *source, bool ok_cancel=true, int param = 0 );
extern	void	CallItemMessageBox( int menu, int item_num, char *source );
extern	void	MenuButtonRightExplain( int x, int y, int &count, char * title );
extern	void	MenuButtonLeftExplain( int x, int y, int &count, char * title );
extern	void	LeftMenuClose();
extern	int		dw_TotalImageSize;
extern	void	LoofSetHeroAbility( int ability[] );

//extern	void	SendReCallItemFromServer( int type );

extern	void	FreeChImage();
extern	void	FreeOpenningSpr( BYTE **buf );

extern	void	SetItemMakeOk( ItemAttr item );

extern	int		UseItemByRbutton(POS pos_s, ItemAttr &item_attr);						// �������� ������ ��ư���� Ŭ������ ��

extern  int		Call_GM_2ND_Skill_Menu();//021014 lsw
extern  int		Call_Skill_Master_Menu();//021111 lsw
extern  int		Call_MERCHANT_REGISTER_Menu();//021126 lsw
extern  int		Call_EXCHANGE_BOND_MONEY_Menu();

extern	void	SetLearnItemData( int menu, ItemJoinSelect list[]);	//������ ���� ����Ʈ �迭 ����
extern	int		GetResultItemBySkill( int skill, int item[10] );	// �� ��ų�� ����� �ִ� ������ ��� �������� 
extern	bool	FindLearnItem( int item );							// ���� ��� ���������� �Ǵ�
extern	ItemAttr *GetItemByPos( POS item );
extern	bool	DivideItem( K_ITEM *item, int much );
extern	int		NotNeededMouseLButtonClick( void );		// ���콺�� Ȱ��ȭ�� �޴� �ȿ� �ִ����� üũ
extern	int		GetWeight( ItemAttr item );
extern	int		SearchLearnItem( int item );			// ���� ��� ��������ġ ã��
extern	void	PrintTextOnMenu2();
extern	void	PrintTextOnMenu();


extern Spr	openhouse[MAX_HOUSE];
extern Spr	closehouse[MAX_HOUSE];
extern void LoadHouseImage( int is_open );
extern void FreeHouseImage( );
extern void PutHouseImage( int x, int y, int num, bool is_open );
extern void LoadHouse( );


///  ���� ���� �� �޼ҵ��...

extern int SearchNextMagicNumByClass( int num, int cls );		// � ���� ������ ���� �迭�� ������ ã�´�.
extern int GetMagicMaxByClass( int cls );						// �װ迭�� �� ���� ���� ���Ѵ�.
extern int GetLearnableMagicMaxByClass( int cls );				// �� �迭���� ���� ��� �� �ִ�  ���� ���� ���Ѵ�. (�̹� ��� ���� ���� )
extern bool CanLearnMagic( int num );							// �� ������ ���� �ֳ�?

extern MagicByClass	magic_by_class;
extern void GetDivedeMagicByClass( );			// �迭�� ���� �ִ� ����... ��

extern void ServerConnectMenuClose();	// ������ ������ �ִ� �޴��� ���� ��..

extern DWORD GetMoneyByItem( );		// �κ��丮 â�� �ִ� �� ���
extern DWORD GetNewMoneyByItem( );
extern int SearchCoinItem( int &a, int &b, int &c);
extern int SearchNewCoinItem( int &a, int &b, int &c);
extern int SearchItemCount(int item_no);
extern int SearchItemPosByNumber(ItemAttr *item, int &a, int &b, int &c);
extern int SubtractMoney( DWORD money );
extern int AddMoney( DWORD money );
extern void SetMemorizeArray( int num );


extern int DoMemorize( int magic_num );
extern void InitMemorize( );	// �޸������ �ʱ�ȭ

extern int MemorizeYet( int magic_num );
extern int GetNoLearnMagic( int cls, int &magic_num );	// ������ ���� �̸��� �� ������ �˾ƿ��� �Լ�	// magic_num���� ���� ��ȣ�� �Ѱ� �ش�.
extern void CheckPartyFace( );		// ��Ƽ������ �ִ� ���� üũ�ؼ� ȭ�鿡 �ѷ��ش�.
extern void SetExistMeOfParty( );		// ��Ƽ���� ���� �׻� ������ ���´�..

extern int SearchMyPartyOfMySight( LpCharacterParty party );	// �� �þ߿� ��Ƽ���� �ִ���?
extern void ViewSmallMapCheck();

//extern	K_ITEM give_item;	// ��ȯ�� �������� ���� �ִ� ������
//extern	K_ITEM recv_item;	// ��ȯ�� �������� ���� �޴� ������ 

extern inline int CheckMouseInMenu( int i );
extern void DropItemSound( int item );		// ������ ���� ����Ʈ���� �Ҹ�
extern inline void LeftClose(); // ���� â ��� �ݱ�

extern int dexDefenseCor( int dex );
extern void MenucheckWhenHeroDie( );	// �׾����� �޴�üŷ
extern void SendSmileFace( int smileno );
extern bool g_bMagicSelect;
extern int isCrashSmallMapByOther();
extern void CheckFocusMenu();
extern void GrobalValueSeet();
extern int CanLearnMagicByCount( int num );
extern void InitPartyState( );

extern void LoadSmallMenuImage2( int num );		// �߰� �޴� �̹��� �ε��� ����..
extern void FreeSmallMenuImage2( );

extern void FreeMagicIcon( );

extern int GetBelieveGodByMenu( int menu, char *God );		// ���� �޴����� �ϴ� ������...
extern void CallGodMenuMessage( char *msg, int believe );
extern int GetMenuNumByBelieveGod( int god );
extern void PutSmallmenuImage(int mx, int my, int x,int y, int image_number, int type);
extern void FreeSmallMenuETC();
extern int QuickKey( int menu, HWND active_hwnd = NULL);


extern bool bCallGodMeetingMenu;
extern Spr *GetSprOfMenu( int type, int imagenumber );
extern int GetHourToRate( int h );

extern void PrintQuickKey( char *text);
extern int GetMoveMenuDefineByMenuNumber( int menu, int is_open );
extern void CheckAutoDivideMoney( ItemAttr *item );

extern bool LoadMagicName( );

//extern int GetAbilityMax( int ability_type );

extern void OpenRecvEarMessageMenu();
extern void CloseRecvEarMessageMenu();

extern int ExistBox( DWORD box_id );
extern int GetItemAttrWeapon( int item );		// ���������� ���߿� ���� ������ �������� Ȯ���ϴ� ��.
//extern int ExplainItemBySelectItemAfter( int x, int y, int item_number, char explain[5][40] );
extern void GetSupperItem( int god );
extern int ViewSmallMap2( int x, int y);
extern void SetSmallMapMenu( );
extern void PrintNoteMeeting( int g_Note_Meeting );
extern void CheckErrorMessage( char* str );
extern int OnlyAlphabet( char *str, int len );
extern int OnlyNumber( char *str, int len );
extern void PutDot( int x, int y, WORD color );

extern int ExplainBackView( int &x, int &y, int count );	// ���� ��ǥ�� ��������.
extern void ShowExplain();							// ���� ����� �����ֱ� 

extern void CheckMagicExplain( int magic_no );			// ���� ����
extern void CheckSkillExplain( int skill_no );			// ��ų ����

extern int ItemExplainAfterMenuDisplay( int prev );			// ���߿� ������ ����.
extern void SetMenuExplain( int type, int menu, int start_field );

extern void ExplainMenuSet( char *temp );
extern int InsertIntArray( int node, int *int_array, int &curr_count, int array_max );
extern int DeleteIntArray( int node, int *int_array, int &curr_count, int array_max );
extern int IsJustViewMenuWhenDead( int dest_menu );
extern void PutMagicIconAll();
extern int QuickKeyFirst( int menu );
extern int IsExistNodeToArray( int node, int *int_array, int curr_count );
extern void SetLearnItem();
extern int ConvStoreListToMenu( int prev, bool is_store );

extern void lsw_menu_set( );//010830 lsw

extern bool CanLearnItemByNeedSkill( int join_no );

extern void SmallMapClose();
extern void GetTatics();
extern int SetQuickSkill( int *index, int start, int type );

extern void LoadFilterWord( );
extern bool CheckNumber( char *text, int len );		// ���� �̿��� ���� �ִ����� üũ�Ѵ�.	������ true ������ false
extern void kh_menu_set( );
extern void ItemSoundOfDropItem( int item_no );
extern void StopWaveOfMenu();
extern void GetNationBBS( char *filename );
extern void OpenFightMapSignMenuOpen( int tema[] );
extern int	IsSameNation( int type, LpCharacterParty lpParty );
extern void LoadTextDefault( char *filename, char *text, int text_max );
extern int CheckRegistCandidate( );
extern void CallMessageBox(BOOL bBig, int x, int y, char *source, BOOL bType=FALSE);
extern void MessagePut();

extern void	LottoMenuSet(); //soto-030425
extern void	LuckyMenuSet();	//soto-030429
extern void	LuckyEventSet(); //soto-030502

extern void	LuckyMenuOpen();//soto-030429
extern void	LottoMenuOpen();//soto-030429
extern void	WinnerMenu();//soto-030502

//020530 lsw
#define MAX_MAGIC_MEMORY	13//020420 lsw
#define QM_MAGIC			1//QM_ QuickMemory
#define QM_SKILL			2
#define QM_COMBATSKILL		3
typedef struct QUICKMEMORY
{
	char OwnerName[21];//020701 lsw
	int iType;// 1 ���� 2 ��ų 3 ������ų
	int iNo;//���� �ѹ�
	int iBuffer;
	void SetQuickMemory(const char* name ,const int iTypeValue,const int iTypeNo)
	{
		if(name)
		{ 
			const int iLen = strlen(name);
			if(21 < iLen|| 0 >= iLen )
			{
				Clear();
				return;
			}
			strcpy(OwnerName, name);
			iType = iTypeValue;
			iNo = iTypeNo;
		}
		else
		{
			Clear();
		}
		return;
	}
	void Clear()
	{
		iType	= 0;
		iNo		= 0;
	}
}t_QuickMemory,*LPQUICKMEMORY;

extern t_QuickMemory SaveMemory[MAX_MAGIC_MEMORY];		// ����Ű�� �����ѵ� ��� //020530 lsw
extern bool        EquipSaveFlag[5];
extern ItemAttr	SaveArmor[5][8];
extern t_QuickMemory HotKeyMemory;	// ��ư �Ѱ��� ��� �ϴ� Ram ���� ����
extern void ClearQuickMemory();
extern void DisplayQuickMemory(const int iType, const int iTargetNo, const int iX, const int iY);
extern void SetQuickMemoryByKeyInput(const int iType, const int iNo, const int iBuffer = 0);

extern int ViewTipsFlag;
extern void ExplainItem2( int x, int y, ItemAttr *olg_item );
//extern void LoadManList();
extern int GetItemExplainLine( ItemAttr *olg_item );
extern void GetWORDOfDWORD( DWORD dword, WORD &word1, WORD &word2 );

extern int bar1_r, bar1_g, bar1_b;
extern int bar2_r, bar2_g, bar2_b;
extern int g_RecvLoanInfo;


extern int g_BattleStatusView;
extern char g_szMoneyName[];//021014 lsw

/////////////////////////////////////////////
#define AT_JOIN_COLOSSUS		1// �ݷμ��� ������ �����Ѵ�.
struct k_assent		// ������ ������ �����Ҷ��� ����� ���� ����
{
	int		type;
	DWORD	time;		// �ð�

	union 
	{
		char	data[512];
		WORD	default_word;
	}u;

	k_assent()
	{
		type = 0;
	}
};

/////////////////////////////////////////////
class CGuildItem
{
public :
	int m_Max;
	int old_page;
	DWORD *m_pIndex;
	ItemAttr *m_pItem;
	DWORD	m_BoxMoney;

public :
	CGuildItem()
	{
		m_pIndex = 0;
		m_pItem = 0;
		clear();
	}
	int clear()
	{
		m_Max = 0;
		if( m_pIndex ) delete m_pIndex;
		if( m_pItem ) delete m_pItem;
		m_pIndex = 0;
		m_pItem = 0;
		old_page = 0;
		m_BoxMoney = -1;		// -1�϶� �������� ��ƾ�� �����Ű�� �ʴ´�.

		return 1;
	}
	int clear( int ct )
	{
		memset( &m_pItem[ct], 0, sizeof( ItemAttr ) );
		m_pIndex[ct] = 0;
		return 1;
	}
	~CGuildItem()
	{
		clear();
	}
	DWORD GetIndex( int ct )		// �ε����� ��������
	{
		if( !m_pIndex ) return 0;
		if( ct > m_Max ) return 0;
		return m_pIndex[ct];
	}
	int InsertItem( int ct, int index, ItemAttr *item )	// ������ ����
	{
		if( !m_pIndex ) return 0;
		if( ct > m_Max ) return 0;
		m_pIndex[ct] = index;
		m_pItem[ct] = *item;
		return 1;
	}
	int SerchFirstIndex( int find_index )	// Ư�� �ε������� ������ �ִ� ��ȣ�� ã�´�.
	{
		if( !m_pIndex ) return -1;
		for( int i=0; i<m_Max; i++ )
		{
			if( m_pIndex[i] >= find_index ) return i;
		}
		return -1;
	}
	ItemAttr *GetItemByParm( int ct )		// �Ķ���ͷ� ������ ��������
	{
		if( !m_pIndex ) return 0;
		if( ct > m_Max ) return 0;
		return &m_pItem[ct]; 
	}
	int GetMax() { return m_Max; }
};

/////////////////////////////////////////////
#define MAX_POTAL_MAP 40

// 1. 將 CPotalMenuUnit 改為純資料的 struct，並使用 std::string
//    這樣就不再需要手動管理記憶體 (new/delete)，更加安全
struct PotalMapInfo {
  std::string map_name;
  int map_number = 0;
  int money = 0;
  int type = 0;
  int img_type = 0;
  int level = 0;
  std::string explain;  // 說明文字也用 std::string
  void Clear() { explain = ""; }
};

// 2. 重新設計 CPotalMenu class
class CPotalMenu {
 public:
  // 3. 建構函式和解構函式可以省略，C++ 會提供預設版本
  //    因為 std::vector 會自動管理其內容，不再需要手寫 clear()
  CPotalMenu() = default;
  ~CPotalMenu() = default;

  // 載入資料的函式，回傳 bool 表示成功/失敗
  bool LoadPotalMap(int nation);

  // 清空所有資料
  void Clear() { m_Maps.clear(); }

  // 4. Getter 函式更新，並加上 const
  //    使用 at(idx) 會進行邊界檢查，比 [] 更安全
  size_t GetMax() const { return m_Maps.size(); }
  int GetMapNumber(size_t idx) const { return m_Maps.at(idx).map_number; }
  int GetType(size_t idx) const { return m_Maps.at(idx).type; }
  int GetImgType(size_t idx) const { return m_Maps.at(idx).img_type; }
  int GetMapMoney(size_t idx) const { return m_Maps.at(idx).money; }
  const char* GetMapName(size_t idx) const {
    return m_Maps.at(idx).map_name.c_str();
  }
  const char* GetMapExplain(size_t idx) const {
    return m_Maps.at(idx).explain.c_str();
  }
  int GetMapLevel(size_t idx) const { return m_Maps.at(idx).level; }

 private:
  // 5. 使用 std::vector 取代 C-style 陣列，不再有固定數量限制
  std::vector<PotalMapInfo> m_Maps;
};


/////////////////////////////////////////////
struct CFriend
{
	int m_bLogin;		// ���� ���̴� �ƴϴ�.
	char name[20];
	RECT rect1;		// �ӼӸ�
	RECT rect2;		// ����
	RECT rect3;      //name

	CFriend()
	{
		clear();
	}
	int clear()
	{
		m_bLogin = 0;
		name[0] = 0;
		SetRect( rect1, 0, 0, 0, 0 );
		SetRect( rect2, 0, 0, 0, 0 );
		return 1;
	}
};

const int MAX_FRIEND_GROUP = 30;
class CFriendGrup
{
public :
	int count;
	int click;
	CFriend m_Friend[MAX_FRIEND_GROUP];
public :
	CFriendGrup()
	{
		count = 0; click = 0;
	}

	int SetFriendName( int cn, char *name );
	int SetFriendLogin( int cn, int login );
	char *GetFriend( int cn, int &login );
	int InsertFriend( char *name, int login );
	int clear();
	int SerchName( const char *name );//030504 lsw
	int DeleteName( int cn );
	int SetRect( int cn, int type, int left, int top, int right, int bottom );
	char *CheckRect( int cn, int type, int x, int y );
};

/////////////////////////////////////////////
#define MAX_RUNE 25

struct CRuneExplain
{
	char m_szTitle[4][MAX_RUNE+1][128];
	char m_szExplain[4][MAX_RUNE+1][2048];
	string	m_szExplainTitle[4];
public :
	CRuneExplain() 
	{ 
		memset( m_szTitle, 0, sizeof( char )*4*(MAX_RUNE+1)*128 ); 
		memset( m_szExplain, 0, sizeof( char )*4*(MAX_RUNE+1)*2048 );
		LoadLuneText();
	};

public :	
	int LoadLuneText();
	int LoadLuneText2( int lune_num );
	char * GetTitleExplain(int lune_num );
};

struct k_RuneMenu
{
	int m_nRuneNum;				// �� ��ȣ
	int m_nLeftRuneImage;		// ���ʿ� ���� �� ��ȣ
	int m_nRightRuneImage;		// �����ʿ� ���� �� ��ȣ
	int x;
	int y;
};

class CRuneMenu
{
public :
	int m_nQuestStep;
	int m_nSelectRune;
	k_RuneMenu m_stRune[MAX_RUNE];	
public :
	CRuneMenu();
	int LoadRuneMenuXY();
};

struct k_guild_degree
{
	char m_pszGuildDegree[5][20];
};
struct k_guild_regist_explain
{
	Uint8 r[100], g[100], b[100];
	std::string m_pString[100];
};
struct k_char_rgb
{
	int		body_r;			//  �ܸ� ��
	int		body_g;					
	int		body_b;
	int		cloth_r;
	int		cloth_g;
	int		cloth_b;
	
	k_char_rgb() { set(); }
	~k_char_rgb();
	int set();
};

struct k_view_nation_money
{
	DWORD money3;
	DWORD money4;
	DWORD money6;
	k_view_nation_money()
	{
		money3= 0;
		money4= 0;
		money6= 0;
	}

};

class CNameOfGuildMemberList
{
public:
	int m_iDegree;		// ���
	bool m_bRecv;		// �̹� ������ �ִ�. 
	int m_iTotal;
	int *m_bpIsConnect;		// ������ �ִ� ĳ������ Ȯ��
	string	*m_lpszName;

public :
	CNameOfGuildMemberList()
	{
		m_lpszName = 0;
		m_bpIsConnect = 0;
		clear();
	}
	int clear()
	{
		if( m_lpszName ) delete[] m_lpszName;
		if( m_bpIsConnect ) delete[] m_bpIsConnect;
		m_bRecv = 0;
		m_iTotal = 0;
		m_bRecv = 0;
		m_lpszName = 0;
		m_bpIsConnect = 0;
		return 1;
	}
	~CNameOfGuildMemberList()
	{
		clear();
	}
};
class CGuildMemberName
{
public :
	int m_nSubMasterNumber_degree;		// ���� �����Ͱ� ���� �ִ� ���� ���
	int m_nSubMasterNumber;				
	CNameOfGuildMemberList m_degree[5];
public :
	int clear()
	{
		m_nSubMasterNumber_degree = -1;
		for( int i=0; i<5; i++ ) m_degree[i].clear();
		return 1;
	}
	~CGuildMemberName()
	{
		clear();
	}
	int GetRecv( int degree )
	{
		return m_degree[degree].m_bRecv;
	}
	int SetRecv( int degree )
	{
		m_degree[degree].m_bRecv = 1;
		return 1;
	}
	int SetCount( int degree, int count, k_get_guild_memeber_list_name *data );
	int GetCount( int degree )
	{
		return m_degree[degree].m_iTotal;
	}
	const char *GetName( int degree, int count )
	{
		if( count >= GetCount(degree ) ) return NULL;
		return m_degree[degree].m_lpszName[count].c_str();
	}
	int GetIsConnect( int degree, int count )
	{
		if( count >= GetCount(degree ) ) return NULL;
		return m_degree[degree].m_bpIsConnect[count];
	}
	bool GetSubMasterNumber( int &degree, int &number )
	{
		degree = m_nSubMasterNumber_degree;
		number = m_nSubMasterNumber;
		if( degree == -1 ) return 0;
		return 1;
	}
};


class CGuildHouseInfo
{
public:
	int m_max;
	k_guild_house_info_unit *m_data;

public:
	CGuildHouseInfo()
	{
		m_max =0 ;
		m_data = 0;
	}
	CGuildHouseInfo( int max )
	{
		m_max = max;
		if( !max ) return;
		m_data = new k_guild_house_info_unit[max];
	}
	~CGuildHouseInfo()
	{
		if( m_data ) delete [] m_data;
	}
	int CopyInfo( int index, k_guild_house_info_unit *data )
	{
		if( !m_data ) return 0;
		if( index >= m_max ) return 0;
		m_data[index] = *data;
		return 1;
	}
	bool IsExistGuild( int guild_code )
	{
		for( int i=0; i<m_max; i++ )
		{
			if( guild_code == m_data[i].guild_code ) return true;
		}
		return false;
	}
	int GetGuildCode( int index ) { return m_data[index].guild_code; }
	bool CheckSignet( int index ) 
	{
		if( GetItemMuchByMuchItem( IN_BLUE_SIGNET ) < m_data[index].blue ) return false;
		if( GetItemMuchByMuchItem( IN_RED_SIGNET ) < m_data[index].red ) return false;
		return true;
	}
};

///////////////////////////////////////
// m_PartySelect ������ ����� �� ����
#define PS_EXP			10
#define PS_TACTIC		20
#define PS_MAGIC_TAC	30
//////////////////////////////////////


// 020428 YGI acer
////////////////////////////////////////////////
// �迭��ȣ�� ���� �ߴ��� ���ߴ����� �ľ��ϴ� Ŭ���� ����
class CAlreadyCheck
{
private : 
	int m_nProcessId;
	CAlreadyCheck *next;
public :
	CAlreadyCheck()
	{
		m_nProcessId = 0;
		next = 0;
	}
	~CAlreadyCheck()
	{
		if( next ) delete next;
	}
	bool CheckProcessId( int check_id )
	{
		CAlreadyCheck *temp = next;
		while( next )
		{
			if( m_nProcessId == check_id ) return true;
			temp = temp->next;
		}
		return false;
	}
	int AddProceessId( int add_id ) 
	{
		CAlreadyCheck *prev = this;

		CAlreadyCheck *temp = next;
		for( ; temp; prev = temp, temp = temp->next )
		{
			if( temp->m_nProcessId == add_id ) return false;
		}
		CAlreadyCheck *data = new CAlreadyCheck;
		data->m_nProcessId = add_id;
		prev->next = data;
		return true;
	}
};

class CScenarioTrapInfo
{
public :
	int m_nTotalTrap;
	int m_nActiveTrap;
	int m_nWaitTimeOfDungeon;
	int m_nWaitTimeOfTrap;
public : 
	CScenarioTrapInfo()
	{
		memset( &m_nTotalTrap, 0, sizeof(int)*4 );
	}
};

// 之前定義的現代化類型
using CMapDinamicTile = std::unordered_map<int, std::vector<POINTS>>;
// MODERN: 全局變數使用 unique_ptr 自動管理 CMapDinamicTile 的生命週期
struct MenuVariable {
    std::unique_ptr<CMapDinamicTile> m_pDinamicTile;
};
// 021107 YGI--------------------
extern DWORD g_curr_time;
class CEventMoveMap
{
public : 
	k_event_move_map_rep	data;
	DWORD					m_dwRecvTime;

public :
	CEventMoveMap()
	{
		memset( &data, 0, sizeof( k_event_move_map_rep ) );
		m_dwRecvTime = 0;
	}
	void Set( k_event_move_map_rep *pData )
	{
		data = *pData;
	}
	CEventMoveMap operator = ( k_event_move_map_rep &data )
	{
		m_dwRecvTime = g_curr_time;
		Set( &data );
		return *this;
	}
	bool IsCanMove()
	{
		if( !m_dwRecvTime ) return 0;
		if( g_curr_time - m_dwRecvTime > 120 ) return 0;		// 120 ���� ������.
		return 1;
	}
};

////////////////////////////////////////////////////
class CMenuVariable		// �޴� �ѱ��������� ����� ���� ���� ����
{
	//021030 YGI
public : 
	enum
	{
		ACTIVE_EVENT_OBJECT = 0x01,
		ACTIVE_EVENT_SOUND = 0x02,
	};

public :
	CRuneMenu *m_pRune;
	int m_bLoadGuildMark;
	int	m_GuildSaveId;
	char *m_pSubGuildMaster;
	//k_guild_degree *m_pGuildDegree; // ��� ��å �Է� �ӽ� ����
	k_guild_regist_explain m_pGuildRegistExplain;
	//char (*m_ppGuildRegistExplain)[256];
	int	m_bGuildChatOn;
	CRuneExplain	*m_clsRuneExplain;
	k_char_rgb		*m_stRGB;
	bool			m_bChangeColor;
	k_view_nation_money	*m_stNationMoney;
	bool			bCheckSalvation;
	MakeItem		*pGambleItem;
	BYTE			m_GambleRune;
	int				m_GambleRand;		// �鵹�ư��� �ּ����� ���
	CGuildMemberName m_GuildMemberName;
	char			*m_szGuildPublicNotics;
	CFriendGrup		*m_FriendGrup;
	ItemAttr		*m_TodayGambleItem;
	char			m_PartySelect;
	CPotalMenu		*m_PotalMap;
	char			m_bOXTime;
	DWORD			m_OXTime;
	CGuildItem		*m_pGuildItem;
	POS				*m_pPutGuildItem;
	int				m_nCheckSubMaster;
	string			m_szFileTextPut;
	k_nation_item	m_nation_item;
	k_assent		m_assent;
	CGuildHouseInfo	*m_pGuildHouseInfo;
	CAlreadyCheck	*m_CheckProcess;
	//acer5
	k_char_info		*m_pOtherChar;
	// 020620 YGI
    std::unique_ptr<CMapDinamicTile> m_pDinamicTile;  // 自動管理記憶體
	CScenarioTrapInfo	*m_pScenarioTrapInfo;
	k_scenario_trap_key	*m_pScenarioTrapKey;		// 020701 YGI
	bool			m_bTradeON;		//020808 YGI 	

	k_event_object_sound	*m_pEventObjcet;		//021030 YGI
	CEventMoveMap			*m_pEventMoveMap;		// 021107 YGI
			
public :
	CMenuVariable()
	{
		m_bTradeON = 1;		//020808 YGI 
		//m_pGuildDegree = NULL;
		m_pSubGuildMaster = NULL;
		//m_pGuildRegistExplain =NULL;
		m_bGuildChatOn = 1;
		m_bLoadGuildMark = 0;
		m_pRune = NULL;
		m_clsRuneExplain = 0;
		m_stRGB = 0;
		m_bChangeColor = 0;
		m_stNationMoney = 0;
		bCheckSalvation = 0;
		pGambleItem = 0;
		m_GambleRune = 0;
		m_GambleRand = 0;
		m_GuildMemberName.clear();
		m_szGuildPublicNotics = 0;
		m_FriendGrup = 0;
		m_TodayGambleItem = 0;
		m_PartySelect = 0;
		m_PotalMap = 0;
		m_bOXTime = 0;
		m_OXTime = 0;
		m_pGuildItem = 0;
		m_pPutGuildItem = 0;
		m_nCheckSubMaster = 0;
		m_szFileTextPut = "";
		m_nation_item.clear();
		m_pGuildHouseInfo = 0;
		//acer5
		m_pOtherChar = 0;
		// 020620 YGI
		m_pDinamicTile = 0;		
		m_pScenarioTrapInfo = 0;
		m_pScenarioTrapKey = 0;		// 020701 YGI
		m_pEventObjcet = 0;		//021030 YGI
		m_pEventMoveMap = 0;
	}
	~CMenuVariable()
	{
		clear4();
		clear2();
	}
	int clear();		// �޴��� ���� �ɶ� �������.
	int clear2();		// ������ �ٽ� ���۵ɶ� �������. clear3() clear() �� �����Ѵ�. 
	int clear3();		// ���� �ٲ�.. clear() �� �����ϰ� �ִ�.
	int clear4();
	int LoadingGuildRegistExplain( char *szFileName );
	int GetEventSound();		//021030 YGI
};


extern CMenuVariable g_Menu_Variable;
extern int CallGuildChangeInfo( );
extern DWORD ReturnBlendRGB( int r, int g, int b );

extern char *ChangeString( char *str, char sor, char tag );
extern char *ChangeString2( char *str, char *sor, char *tag );
extern DWORD GetNationMoney( int nation);
extern void LoadMemorize();
extern void SaveMemorize();


#include "menufunctions.h"


#endif
