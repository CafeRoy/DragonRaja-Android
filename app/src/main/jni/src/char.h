/*

*/

#ifndef	__CHAR_H__
#define	__CHAR_H__

#include "dragon.h"

#define ANIMATION_TYPE_WOMAN		0	// 0
#define ANIMATION_TYPE_MAN			1	// 1
#define ANIMATION_TYPE_MON1 		2	// 16
#define ANIMATION_TYPE_MON2 		2
#define ANIMATION_TYPE_MON3 		2

#define ANIMATION_TYPE_TEST			100	// 1

enum ATTR_TYPE
{	//< CSD-031027
	AT_NORMAL = 0,
	AT_FIRE = 1,
	AT_ICE = 2,
	AT_LIGHTING = 3,
	AT_UNDEAD = 4,
	AT_PIERCING = 5,
	AT_ANTIMAGIC = 6,
	AT_BOSS = 7,
	AT_SUMMON = 8,
	AT_TAMING = 9,
	AT_SEMIBOSS = 10
};	//> CSD-031027

//feel type
#define	FT_BAD_							10					

struct NPC_INFO
{	//< CSD-030419
	char szName[40];
	int lv;
};	//> CSD-030419
struct ITEM_INFO
{	//< CSD-030419
	char szName[40];
};	//> CSD-030419

///////////////////////////////////////////////////////////////////////////////

#define MAX_POOL_CHARACTERS 1500  // 假設你最多需要1000個 CHARACTER

typedef struct CharacterPool {
  CHARACTER pool_data[MAX_POOL_CHARACTERS];
  CHARACTER *free_list_head;  // 指向可用物件鏈表的頭部
  int free_count;             // 記錄可用物件的數量
} CharacterPool;

extern CharacterPool g_character_pool;  // 全域的 CHARACTER 記憶體池
extern void InitializeCharacterPool();
extern int IsNK(LPCHARACTER ch);
extern int IsNK( LPCHARACTER ch, int nation );
extern int IsNK( LPCHARACTER ch, char *nation );
extern  bool IsEnemyNation(int anation, int bnation); // CSD-040107
extern void LoadChatImage();//010928 lsw
extern void LoadChatColours();// finito 12/07/07

//011008 lsw >
extern bool		LoadItemSokSungTable();
extern bool		LoadItemSokSungKindTable();
extern bool		LoadItemSokSungGradeTable();
extern bool     LoadItemJoinTable();
//011008 lsw <


#ifdef __cplusplus
extern "C"
{
#endif


extern BOOL			BuildCharacterList(  LPCHARACTERLIST lpCharacterList );
extern void			DestroyCharacterList(  LPCHARACTERLIST lpCharacterList );
extern void			DestroyCharacterListExceptHero( void );

extern void         ProcessDeferredDeletion();
extern void			DisplayPC( LPCHARACTER lpCharacter, int tag );
extern BOOL			AddCharacter( LPCHARACTERLIST lpCharacterList, LPCHARACTER lpCharacter, LPGPCHARINFO	lpgpCharInfo );
extern void			DeleteCharacter(  LPCHARACTERLIST lpCharacterList, LPCHARACTER lpCharacter );
extern LPCHARACTER	FindCharacter( LPCHARACTERLIST lpCharacterList, DWORD dwID );

extern BOOL			ChangeDirection( DIRECTION* lpDirection, int sx, int sy, int dx, int dy );
extern BOOL			ChangeDirection16( DIRECTION* direction, int sx, int sy, int dx, int dy );


extern void h_DisplayPC( LPCHARACTER lpCharacter, int shadow );
extern void h_DisplayPC2( LPCHARACTER lpCharacter );
extern void h_DisplayNPC( LPCHARACTER lpCharacter, int shadow );
extern void h_DisplayPC_Temp( LPCHARACTER lpCharacter );



extern void UpdateCharacterVisuals(LPCHARACTER ch);
extern void AnimationTypeWomanCheck( LPCHARACTER lpCharacter );
extern void AnimationTypeWomanFrameCount( LPCHARACTER lpCharacter );

extern void AnimationType_Mon1_Check( LPCHARACTER lpCharacter );
extern void AnimationType_Mon1_FrameCount( LPCHARACTER lpCharacter );

extern void AnimationTypeTestCheck( LPCHARACTER lpCharacter );
extern void AnimationTypeTestFrameCount( LPCHARACTER lpCharacter );



extern int CharDoAni( LPCHARACTER lpChar, DIRECTION dir, int anino );
extern int CharDoAni_Moving( LPCHARACTER ch, int movetype );


extern void LoadAttackRangeTable( int sprno );
extern int IsTooNearCharacter( int x, int y, LPCHARACTER lpCharacter );
extern int IsAttackRange( 	int *minrange, int  *maxrange, LPCHARACTER lpCharacter, int ex, int ey );

extern int ReturnXYNearPosition( int *x, int *y, int *dx, int *dy, int range, LPCHARACTER lpherocharacter, int tox, int toy );
extern int ReturnXYNearNPC( int *x, int *y, int *dx, int *dy, int range, LPCHARACTER lpherocharacter, LPCHARACTER lpmonstercharacter );
extern int ReturnAccesablePosition( LPCHARACTER ch, int sx, int sy, int ex, int ey, int range, int *dx, int *dy );

extern int AttackAction( LPCHARACTER lpCharacter );
extern int AttackedAction( LPCHARACTER ch );
extern int DieAction( LPCHARACTER ch );


extern void SettingBasicActionSub( LPCHARACTER ch );
extern void SettingBasicAction( int peacests );

extern void SendModifyPosition( LPCHARACTER ch );
extern void RecvModifyPosition( t_server_modify_position *p );
extern void RecvModifyPositionAbsolute( t_server_modify_position_absolute *p );

//================================================
extern void RecvAcceptJoin( t_server_accept_join  *p );

void RecvMantleCandle( t_mantle_candle *tp );
void RecvCandleOn( t_candle_on  *tp);
void RecvCandleOff( t_candle_off  *tp);

char* GetNpcName(int nSprNo);     // CSD-030419
int GetNpcSprNo(CHARACTER* pNpc); // CSD-030419
int GetItemNo(RareMain* item);
bool IsExistNpcSprNo(int nSprNo); // CSD-030419
int GetCharAccessory(CHARACTER* pTarget); // CSD-030723

extern BYTE SpriteOrder[4][8][75];
extern int AttackRangeTable[2][CHARACTER_ACCESSORY_MAX_][10];

#ifdef __cplusplus
}
#endif

#endif	// __CHAR_H__
