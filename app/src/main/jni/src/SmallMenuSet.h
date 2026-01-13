#ifndef	__SmallMenuSet_h__
#define	__SmallMenuSet_h__

extern void SmallMenuSet();
extern int GetCurrentBankItemPay();	// ���� ������ ���Կ� ���� ������

extern int GetBankItemWeight();		// ���Կ� ���� ������....

extern	SCharacter SCharSource;
extern	void	PrintDate(SMENU* SubMenu , int j, DWORD time, int width);
extern	void	SmallMenuChecking();
extern	void	MenuSubProcessType(SMENU *SubMenu );								//�޴� ������ҿ� Ÿ���� �Ǻ��ؼ� ����
extern	void	PutMouseItem();													//���콺�� �������� ������ ���콺�� ���� �������� ����ش�
extern	void	kein_KeyProc();													// �����̽� �ٸ� ������ ��� �޴��� �����. (�������� �޴� ���ٿ�����.)
extern	void	ChangeEquip( LPCHARACTER lpCharacter, ItemAttr *equip_item, int type);		// ����ȭ���� �ٲܶ�.
extern	void	MoveMenu();														// �޴� ������ �� 
extern	int		ItemDrop();															// ������ ��� ��ų��
extern	void	PrintMoney(SMENU* SubMenu, int j, DWORD result, int BoxSize);		// �ڽ� �ȿ� �� ����Ҷ�(õ�������� , ǥ��)

extern	void	ItemExplain(ItemAttr item);
extern	void	ItemExplain(int item);
extern	void	PrintDate(SMENU* SubMenu , int j, DWORD time, int width);
extern	void	PrintText(SMENU* , int, char*, int ); // ���ڿ� ���
extern	int		IsMagicWindowOn();		// ���� �����찡 ���ȴ���..
extern	void	MagicExplain(int num);												// ���� ������ ����

extern	void	DoButtonCheckOfMenu( int i, int j );
extern	void	DoLButtonDownOfMenu( int i, int j );
extern	void	DoRButtonDownOfMenu( int i, int j );
extern	void	DoRButtonCheckOfMenu( int i, int j );
extern	void	DoLButtonDoubleClickOfMenu( int i, int j );

extern	void	PutMouseItemCheck();
extern	void	InitMenu();				// �����Ҷ�, �¾��� ��Ȳ�� ���� �ؾ� �� ��� 
extern	void	Small_KeyProc() ; // ���� �޴��� ������ �����...
extern	char	*GetMotherSkillName( int s );

extern	void	PutCharImage( int x, int y, int num, int flag=0, int size = FS_SMALL, int flag2 =2, int Gender = 100);
extern	int		IsSameCharOfParty( int type, LpCharacterParty ch );		// ��Ƽ�� ������ ���� ����� �ִ����� �Ǵ�.
extern	int		IsCheckLevelOfParty( int type, LpCharacterParty man, int pos );
extern	int		GetTextImageWidth( char *buf );	// ���ڿ��� �׸� �̹���  �� ���� ���ϱ�

extern BYTE *open_spr_buf;
extern void	LoadOpenningSpr( char *filename, BYTE **buf, Spr *spr, int nMaxCount = 0 );		// 020701 YGI

extern int		chat_scroll_count;

extern BYTE		*divine_gauge;
extern Spr		spr_divine_gauge[45];

extern void ConvertSkillTable( );

#define MT_DUAL_CHANGE 100
#define MT_DUAL_DIVIDE 200
#define MT_DUAL_RESET_SYMBOL 300

extern int CallDualInterFace(int type);
extern void DoDualOKButton(int type, int select);
extern int CallInitAbility();
extern void DoInitAbility();
const int HOTKEY_ICON_POS_X = 800-42;
const int HOTKEY_ICON_POS_Y = -(42*2)+1;

extern bool SendChatWisper(const char *name,const char *msg);
extern bool SendChatNormal(char* msg);
extern bool SendChatGuild(std::string msg);  // 020515 lsw
extern void SetChatTarget(const int iType);
extern bool IsChatBoxLock();
extern int GetChatTarget();//���� ä�� ��� ������ ����
extern void DoQuickmemory(const int iQMNo);//020530 lsw
extern void DoQuickmemoryByKeyInput(SDL_Scancode iInputKey,bool bIsDikCheck);//020530 lsw



//���� �������̽� �ʵ� ��ȣ
const int TODAY_NOTICE_FIELD = 16;	
const int COMBAT_SKILL_POINT_FIELD = 18;
const int PEACE_BATTLE_FIELD	=17;
const int MAGIC_FIELD =19;
const int INVENTORY_FIELD =20;
const int PARTY_FIELD =21;
const int OPTION_FIELD = 22;
const int SKILL_FIELD =23;
const int NATION_BBS_FIELD =24;
const int HOT_KEY_FIELD = 28;
const int FRIEND_FIELD =46;
const int MAP_FIELD =15;
const int MAIL_FIELD = 33;
const int STATUS_FIELD = 0;

extern void CallLoginMenu(const bool bForceLoginActive = false);//020710 lsw
extern void CallDelCharComform();//020802 lsw
#endif


