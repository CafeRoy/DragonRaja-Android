#if !defined(AFX_ITEM_H__9E46C0B2_FD86_4EBD_B107_B9D8B67B0669__INCLUDED_)
#define AFX_ITEM_H__9E46C0B2_FD86_4EBD_B107_B9D8B67B0669__INCLUDED_
//021014 lsw
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const int MAX_GROUND_ITEMS = 1000; // (你需要定義一個最大數量)
const int MAX_ITEM_SPRITES = 1000; // (你需要定義一個最大數量)

//021119 lsw
const int ITEM_EQUIP_MAX	=8;
const int ITEM_QUICK_MAX	=6;
const int ITEM_INV_MAX		=72;	//3*3*8
const int ITEM_BANK_MAX		=90;	//5*3*6
const int ITEM_HAND_MAX		=1;
const int ITEM_BUFFER_MAX	=1;

enum IA2_TYPE
{
	IA2_HIDE			= 0x00000001,
	IA2_NOTMOVE			= 0x00000002,
	IA2_POISONED		= 0x00000004,
	IA2_OPENED			= 0x00000008,
	IA2_BROKEN			= 0x00000010,
	IA2_HOUSE			= 0x00000020,
	IA2_MAGICED			= 0x00000040,
	IA2_CANNOTUSE		= 0x00000080,
	IA2_NOIDENTIFIED	= 0x00000100,
	IA2_DOOR			= 0x00000200,
	IA2_BOX				= 0x00000400,
	IA2_CHAIR			= 0x00000800,
	IA2_TABLE			= 0x00001000,
	IA2_FARM0			= 0x00002000,
	IA2_FARM1			= 0x00004000,
	IA2_FARM2			= 0x00008000,
	IA2_FARM3			= 0x00010000,
	IA2_FARM4			= 0x00020000,
	IA2_FARM5			= 0x00040000,
	IA2_FARM6			= 0x00080000,
	IA2_FARM7			= 0x00100000,
	IA2_FIRE			= 0x00200000,
	IA2_TRAPED0			= 0x00400000,
	IA2_TRAPED1			= 0x00800000,
	IA2_TRAPED2			= 0x01000000,
	IA2_LOCK_ITEM		= 0x02000000,
	IA2_SEAL_NOR		= 0x04000000,
	IA2_SEAL_SPC		= 0x08000000,
	IA2_COLOSSUS_STONE	= 0x10000000,
	IA2_SCENARIO_POTAL	= 0x20000000,
};

enum ITEM_STRUCT_INDEX
{
	IATTR_DURATION		=0,
	IATTR_LIMIT			=1,
	IATTR_ATTR			=2,
	IATTR_ID			=3,
	IATTR_RESERVED0		=4,
	IATTR_RESERVED1		=5,
};
const int IATTR_MUCH			= IATTR_DURATION;
const int IATTR_SEAL_STONE_NO	= IATTR_LIMIT; 
const int IATTR_SPRITE_TYPE		= IATTR_LIMIT;
const int IATTR_RARE_MAIN		= IATTR_ID;
const int IATTR_REFORGE			= IATTR_RESERVED0;// 裝備刷屬性記錄次數
const int IATTR_TREASURE_MAP	= IATTR_RESERVED0;	// 040105 YGI
const int IATTR_MALL_TIMED		= IATTR_RESERVED0;  // Used by items in the mall that have a time limit

enum ITEMANIMATION_TYPE
{
	ITEMANIMATION_NO		=0,	
	ITEMANIMATION_REPEAT	=1,
	ITEMANIMATION_RETURN	=2,
	ITEMANIMATION_OPENNING	=3,
	ITEMANIMATION_OPENED	=4,
	ITEMANIMATION_CLOSING	=5,
	ITEMANIMATION_CLOSED	=6,
	ITEMANIMATION_FARM0		=7,
	ITEMANIMATION_FARM1		=8,
	ITEMANIMATION_FARM2		=9,
	ITEMANIMATION_FARM3		=10,
	ITEMANIMATION_FARM4		=11,
	ITEMANIMATION_FARM5		=12,
	ITEMANIMATION_FARM6		=13,
	ITEMANIMATION_FARM7		=14,
	ITEMANIMATION_FIRE		=15,
	ITEMANIMATION_ACTIVE	=16,
};
//# Type - 0:���� Item  1: ����   2: Table  3: ����  4:Box 
enum ITEMTYPE_INDEX
{
	ITEMTYPE_NORMAL			=0,
	ITEMTYPE_CHAIR			=1,
	ITEMTYPE_TABLE			=2,
	ITEMTYPE_DOOR			=3,
	ITEMTYPE_BOX			=4,
	ITEMTYPE_SIGNBOARD		=5,
	ITEMTYPE_DIRECTBOARD	=6,
};
enum ITEMTYPE_WORKTABLE_TYPE
{
	ITEMTYPE_WORKTABLE_MINING		=71,	// �۾��� �뱤��(71)
	ITEMTYPE_WORKTABLE_BATTLE       =72,	// �۾��� ��Ʋ
	ITEMTYPE_WORKTABLE_SUNBAN		=73,  // �۾��� ����
	ITEMTYPE_WORKTABLE_MORU			=74,	// �۾��� ���
	ITEMTYPE_WORKTABLE_HWADUK		=75,	// �۾��� ȭ��
	ITEMTYPE_WORKTABLE_SIHUMKWAN	=76,  // �۾��� �������
	ITEMTYPE_WORKTABLE_MULAE		=77,	// �۾��� ����
	ITEMTYPE_WORKTABLE_BULDAE		=78,// �۾��� �Ҵ� 
	ITEMTYPE_SEAL_NOR				=79,
	ITEMTYPE_SEAL_SPC				=80,
	ITEMTYPE_HP_UP					=81,
	ITEMTYPE_MP_UP					=82,
	ITEMTYPE_SCALE					=83,
	ITEMTYPE_NATION_STONE			=84,
	ITEMTYPE_GUILD_STONE			=85,
	ITEMTYPE_SENARIO_STONE			=86,
	ITEMTYPE_COLOSSUS_STONE			=87,
};

typedef struct tagItemSprite
{
	// 使用初始化列表初始化成員變量
	tagItemSprite() :
		no(0),
		count(0),
		sp(), // 呼叫 Spr 的預設建構函式 (如果沒有定義，會自動零初始化)
		prev(nullptr), // 指標初始化
		next(nullptr)  // 指標初始化
	{
		// 由於我們也需要確保 'sp' (Spr 型別) 的成員被初始化，
		// 且 Spr 結構體中包含指標 (char *img)，
		// 確保 sp() 是零初始化或有明確的 Spr 建構函式是關鍵。
		// 如果 'Spr' 是一個 C 風格的結構體且沒有定義建構函式，
		// 'sp()' 會導致聚合初始化 (C++11)，將所有成員設為零。
	}

	int   no;
	short count;
	Spr sp;
	struct tagItemSprite *prev, *next;
}ITEMSPRITE, *LPITEMSPRITE;

typedef struct sItem 
{ 
	sItem() :
		server_id(0),
		no(0),
		status(0),
		sx(0), sy(0),
		offx(0), offy(0),
		x(0), y(0),
		mx(0), my(0),
		host(0),
		type(0),
		getable(0),
		Anitype(0),
		Sorttype(0),
		Where(0),
		shadow(0),
		framecount(0),
		dirno(0),
		curframe(0),
		anitoggle(0),
		prev(nullptr), // 指標初始化
		next(nullptr)  // 指標初始化
	{
		// 陣列無法在初始化列表中統一初始化，需在函式體內進行
		// 但如果編譯器支援 C++11 聚合初始化，可以考慮用 {} 進行統一零初始化 (Zero-initialization)。
		// 為了通用性，我們在函式體內初始化陣列。

		// 初始化陣列為 0
		for (int i = 0; i < 6; ++i) attr[i] = 0;
		for (int i = 0; i < 10; ++i) {
			spr[i] = nullptr; // 指標陣列初始化
			dsx[i] = 0;
			dsy[i] = 0;
			ddx[i] = 0;
			ddy[i] = 0;
		}
		for (int i = 0; i < 4; ++i) {
			dx[i] = 0;
			dy[i] = 0;
		}
	}

	int server_id;
	int no;
	unsigned int  attr[6];
	short status;
	short sx, sy;
	short offx, offy;
	short x, y;
	short mx, my;
	Spr *spr[ 10];
	short  dsx[ 10], dsy[10], ddx[ 10], ddy[10];

	DWORD	host;
	BYTE	type;
	BYTE	getable;
	BYTE	Anitype;
	char	Sorttype;
	BYTE	Where;
	BYTE	shadow;
	char	framecount;
	char	dirno;
	short	dx[4], dy[4];

	short  curframe;
	char   anitoggle;

	struct sItem *prev;
	struct sItem *next;
}ITEMGROUND, *LPITEMGROUND;


struct ItemGroundPool {
	ITEMGROUND pool_data[MAX_GROUND_ITEMS]; // (你需要定義 MAX_GROUND_ITEMS)
	LPITEMGROUND free_list_head;
	int free_count;
};
extern ItemGroundPool g_itemground_pool; // 宣告為全域變數

// --- 【新代碼】定義 ITEMSPRITE 物件池 ---
struct ItemSpritePool {
	ITEMSPRITE pool_data[MAX_ITEM_SPRITES]; // (你需要定義 MAX_ITEM_SPRITES)
	LPITEMSPRITE free_list_head;
	int free_count;
};
extern ItemSpritePool g_itemsprite_pool; // 宣告為全域變數

extern void AddItemList( LPITEMGROUND i );
extern void DeleteItemList( int server_id );
extern void FlushPendingDeletesItemList();
extern LPITEMGROUND FindItemList( int x, int y );
extern void DrawItemList( void );
extern void DrawObjectItem_SealStone( void );
extern void LoadItem( LPITEMGROUND i );
extern void FreeItem( LPITEMGROUND i );
extern int CheckEventWith_Item( void );
extern LPITEMGROUND FindItemList( int server_id );
extern void DestroyItemList( void );
extern void RecvItemAttrChange( t_item_attr_change *p );
extern void Send_CMD_ITEM_BOX_BREAK( LPITEMGROUND i );
extern void Send_CMD_JUST_ATTACK_ANIMATION( LPITEMGROUND i );
extern void Recv_CMD_ITEM_BOX_BREAK( t_server_item_box_break *p );
extern void Send_CMD_ITEM_BOX_BREAK_RESULT( int item_id );
extern void Recv_CMD_ITEM_BOX_MAGIC_BREAK_RESULT( t_server_item_box_magic_break_result *p );
extern void Send_CMD_ITEM_BOX_MAGIC_BREAK( int item_id );
extern bool IsPossibleAttackColossusStone( LPITEMGROUND item );
extern void SendAttackColossusItem( LPITEMGROUND item );
extern void CheckTrap( void );
extern void RecvGiveItem( t_server_give_item *p );
extern LPITEMGROUND g_Item, CursorCheckedItem;
extern ITEMGROUND   temp_Item;
extern void InitItemAttrs();
extern bool GetItemDurTxt(ItemAttr *pItem,char* szTxt);
extern char* GetTacticName(const int iTacticKind);
extern void InitializeItemGroundPool();
extern void InitializeItemSpritePool();
extern void DrawItemInfoOnScreen();
#endif // !defined(AFX_ITEM_H__9E46C0B2_FD86_4EBD_B107_B9D8B67B0669__INCLUDED_)
