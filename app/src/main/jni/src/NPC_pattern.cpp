#include "StdAfx.h"

#include <Stdio.h>
#include "Dragon.h"	

#include "Path.h"
#include "CharDataTable.h"
#include "Hong_Sub.h"
#include "CounSeler.h"
#include "Item.h"

#define __NPC_PATTERN_H__
#include "NPC_Pattern.h"

#include "NetWork.h"
#include "Map.h"
#include "Hong_Sprite.h"

extern NPC_INFO g_infNpc[MAX_CHARACTER_SPRITE_]; // CSD-030419
extern ITEM_INFO g_itemref[MAX_ITEM_MUTANT_KIND]; // CSD-030419
extern struct CItem_List Item_Ref ;
extern void SendNPCAttack( t_connection c[], int npc );
extern void calcNewAbility(CHARACTER *) ;
extern int  getMaxHungry(CHARACTER *chr) ;
extern void SetDamageOfHero( CHARACTER *ch ); // OP_Chr_Status.cpp
extern ItemAttr GenerateItem( int type, int item_no, DWORD intattr2, int success_rate );
/////////////// 0124 �̱��� //////////////////
extern int  calcAC( CHARACTER *ch );
extern int  calcDefenceRate( CHARACTER *ch );
extern void SendViewTypeChange( int cn, int viewtype );


extern void NPC_Pattern_Tammed( CHARACTER *n );
extern void NPC_Pattern_GUARD( LPCHARACTER n);
extern void NPC_Pattern_MAUL( LPCHARACTER n);
extern void NPC_Pattern_8( CHARACTER *n );
extern void NPC_Pattern_SealStone( LPCHARACTER n );

//extern NPCName_by_Gender	NPC_Name_Ref[Num_Of_NPC_Name];
//extern NPC_Info				NPC_Info_Ref[Num_Of_NPC_Type];
//extern NPCLev_to_Exp		NPC_Lev_Ref[Num_Of_NPC_Lev] ;
//extern NPC_Generation		NPC_Gen_Ref[Num_Of_NPC_Generation] ;

extern int writedebug;
extern int g_DontPlayNPC;

/*
#define NPC_PATTERN_NO_MOVE_			  0		// ������ �ִ´�.
#define NPC_PATTERN_HOSTILE_			 10		// ���������� �ٲ��. 
#define NPC_PATTERN_PEACE_				 20		// ��ȭ������ �ٲ��.
#define NPC_PATTERN_FIND_CLOSE_PC_		 30		// ����� pc�� ã�´�.(������ �ٶ󺻴�. )
#define NPC_PATTERN_FIND_WEAK_PC_		 40		// ����  pc�� ã�´�.(������ �ٶ󺻴�. )
#define NPC_PATTERN_FIND_STRONG_PC_		 50		// ����  pc�� ã�´�.(������ �ٶ󺻴�. )
#define NPC_PATTERN_ACCESS_PC_			 60		// ã�� PC�� �����Ѵ�. ( Hostile�ϰ�� ���ݹ����� ������ �����Ѵ�. peace�ϰ�� �����ص� ������ �����ϴ³��� �ٶ󺻴�.  
#define NPC_PATTERN_ACCESS_PC_ATTACKED_	 70		// ���� ������ Pc�� �����Ѵ�. 
#define NPC_PATTERN_WANDER_RANGE_1_		 80		// ��ȸ�Ѵ�. ( 1�� ��ȸ������ 1�̴�. )
#define NPC_PATTERN_WANDER_RANGE_2_		 90		// ��ȭ�Ѵ�. ( 1�� ��ȸ������ 2�̴�. )
#define NPC_PATTERN_WANDER_RANGE_3_		100  	// ��ȭ�Ѵ�. ( 1�� ��ȸ������ 3�̴�. )
#define NPC_PATTERN_ESCAPE_PC_			110		// ã�� PC�� ���� ��������. 
#define NPC_PATTERN_ESCAPE_BATTLE_PC_	120		// ��ó�� ���� PC�� ������ �ָ� ��������. 
*/


////////////////////////////////////////////////////////////////////////////////////////////
///////////   Variables..

int NpcPatternTable[20] = { NPC_PATTERN_HOSTILE_,
							NPC_PATTERN_FIND_CLOSE_PC_,
							NPC_PATTERN_ACCESS_PC_,
							NPC_PATTERN_ATTACK_PC_,
							NPC_PATTERN_BACKDRAW_,
							NPC_PATTERN_RETURN_,		
};


// 0811 NPC KHS
short NPCAccessTable[400][4];



static int movep;





///////////////////////////////////////////////////////////////////////////////////////////////////
///
///				User Function Declarations..
///
void InitMoveDelayTable( void );
int Get_MoveDelayTime( CHARACTER *ch );
int InDistance( CHARACTER *s, CHARACTER *d, int dis );
int NPC_NearRoundCh( CHARACTER *n, CHARACTER *ch, int *x, int *y );
int NPC_NearCh( CHARACTER *n, CHARACTER *ch, int *x, int *y );
int NPC_NearPosition( CHARACTER *n, CHARACTER *c, int *x, int *y );
int NPC_NearBackCh( CHARACTER *n, CHARACTER *ch, int *x, int *y, int dist );
int NPC_GetAttackRange( CHARACTER *n );
int NPC_GetPatternTable( CHARACTER *n );
int NPC_GetDir( int sx, int sy, int ex, int ey );
int NPC_MakePath( CHARACTER *n, int tx, int ty, int how );
int NPC_MakePathBumn( CHARACTER *n, int tx, int ty, int how );
bool NPC_IsMoving(CHARACTER *n); // CSD-040107
int NPC_Hostile( CHARACTER *n );
int NPC_StillAttack( CHARACTER *n);
int NPC_IsAttackableRange( CHARACTER *n );
int NPC_IsInBossRange( CHARACTER *n );
int NPC_IsInHostRange( CHARACTER *n );
int NPC_IsWhoNearRedPC( CHARACTER *npc, int tilerange  );
int NPC_IsWhoNearPC( CHARACTER *npc, int tilerange );
int NPC_WhoIsAttackMyBaby( CHARACTER *n );
int NPC_WhoIsBoss( CHARACTER *npc );
int NPC_WhoIsBlocking( int sx, int sy );
CHARACTER *NPC_ReturnCharListPoint( int id );
CHARACTER *ReturnCharListPoint( char *name );
void RecvTameCommand( int cn, t_tame_command *p );
void SendTameResult( int cn, int result  );
void SendHostName( int cn, CHARACTER *n );
void CheckNPCTammingTimeElapsed( CHARACTER *n );
	
	
////////////////////////////////////////////////////////////////////////////////////////////////
////
////			   User Function
////
		

void SendNPC_parameter( int id, int type, int data )
{
	t_packet p;

	p.h.header.type = CMD_SEND_NPC_PARAMETER;

	p.u.send_npc_parameter.id	= id;
	p.u.send_npc_parameter.type	= type;
	p.u.send_npc_parameter.data = data;

	p.h.header.size = sizeof( t_send_npc_parameter );
	
	QueuePacket( &p, 1 );
}

void RecvNPC_parameter(int id, int type, int data)
{	//< CSD-031106
	LPCHARACTER ch = ReturnCharacterPoint(id);
	if (ch == NULL) return;

	switch (type)
	{
	case NPC_PARAMETER_TARGETID:
		{
			ch->targetid = data;
			break;
		}
	case NPC_PARAMETER_BOSSID:
		{
			ch->bossid = data;
			break;
		}
	case NPC_PARAMETER_HOSTID:
		{
			ch->HostId = data;
			break;
		}
	case NPC_PARAMETER_TAME_CMD:
		{
			ch->tame_cmd = data;
			break;
		}
	case NPC_PARAMETER_PATTERNTYPE:
		{
			ch->patterntype	= data;
			break;
		}
	case NPC_PARAMETER_OLDPATTERNTYPE:
		{
			ch->oldpatterntype	= data;
			break;
		}
	case NPC_PARAMETER_ATTRIBUTE:
		{
			ch->nAttr = data;
			break;
		}
	}
}	//> CSD-031106

void RecvNPC_stringparameter( int id, int type, char *data )
{
	LPCHARACTER ch = ReturnCharacterPoint( id );
	if( ch == NULL ) return;

	switch( type )
	{
	case NPC_PARAMETER_TARGETNAME	:	strcpy( ch->TargetName, data );
		break;

	default : break;
	}
}	
	
	
	
	
	
int InDistance( CHARACTER *s, CHARACTER *d, int dis )
{	
	if( s == NULL ) return 0;
	if( d == NULL ) return 0;
	
 	if( IsDead( s ) ) return 0;
	if( IsDead( d ) ) return 0;
	
	int sx = s->x;
	int sy = s->y;
	int dx = d->x;
	int dy = d->y;

	if( dis * dis > (dx-sx)*(dx-sx)+(dy-sy)*(dy-sy) ) return 1;
	
	return 0;
}	



void SendModifyPositionNPC( int id )
{
	LPCHARACTER ch = ReturnCharacterPoint( id );
	if (ch == NULL) return; // finito null check

	t_packet p;

	if( ch->patterntype == 0 && ch->HostName[0] == 0 ) return;

	p.h.header.type = CMD_NPC_MODIFY_POS;

		p.u.npc_modify_pos.id = id;
		p.u.npc_modify_pos.mx = ch->x / TILE_SIZE;
		p.u.npc_modify_pos.my = ch->y / TILE_SIZE;

	p.h.header.size = sizeof( t_npc_modify_pos );

	QueuePacket( &p, 1 );
}




void SendNPCChatArea( int id, int Num )
{
	t_packet p;
	//int Len;

	//if( data == NULL ) return;

	//Len = strlen( data );
	//if( Len <= 0 ) return;

	p.h.header.type = CMD_NPC_CHAT_DATA;
	p.u.npc_chat_data.id = id;
	p.u.npc_chat_data.Num = Num;
	//strcpy(p.u.npc_chat_data.data, data);
	p.h.header.size = sizeof( t_npc_chat_data );

	
	QueuePacket( &p, 1 );
}

void SendNPCAttack( int attacker_npc_id )
{
	t_packet	p;
	int			magic_no=0;			//�Ϲ������� ������ �����ϴ� ���͸� ���� ��ȣ �ο�


	LPCHARACTER attacker_npc_ch = ReturnCharacterPoint( attacker_npc_id );
	if( attacker_npc_ch == NULL ) return;
	if( attacker_npc_ch->targetid == -1 ) return;
	LPCHARACTER diffender_npc_ch = ReturnCharacterPoint( attacker_npc_ch->targetid );

	
	if( diffender_npc_ch == NULL ) return;

	if( attacker_npc_ch->viewtype == VIEWTYPE_STONE_ ) return;	//001215 KHS
	if( diffender_npc_ch->viewtype == VIEWTYPE_STONE_ ) return;	//001215 KHS

	//switch(attacker_npc_ch->sprno)
	//{
	//case 40:						//�ǽ����Ǿ�->ȭ��� �߻�
		//magic_no = 30+rand()%3;
		//break;
	//}

	if( magic_no )							//Ư�� ������ ����� NPC�� ���
	{
		LPCHARACTER		target_ch = ReturnCharacterPoint( attacker_npc_ch->targetid );
		if(target_ch==NULL)		return;
		return;
	}
	
	
	DIRECTION dir;	
	int x, y;		
	int	t_X, t_Y;	
	x = diffender_npc_ch->x;
	y = diffender_npc_ch->y;
	int how;
	
	switch(rand()%6)
	{
	case 0:
	case 1:
	case 2:
		how = 0;
		break;
	case 3:
		how	= 12;
		break;
	case 4:
		how = 18;
		break;
	case 5:
		how = 24;
		break;
	}
	
	ChangeDirection( &dir, attacker_npc_ch->x, attacker_npc_ch->y, x, y );

	switch( dir )
	{
		case DIRECTION_DOWN		:	t_X= 0;		t_Y=how;		break;
		case DIRECTION_LEFTDOWN	:	t_X=-how;	t_Y=how;		break;
		case DIRECTION_LEFT		:	t_X=-how;	t_Y=0;			break;
		case DIRECTION_LEFTUP	:	t_X=-how;	t_Y=-how;		break;
		case DIRECTION_UP		:	t_X= 0;		t_Y=-how;		break;
		case DIRECTION_RIGHTUP	:	t_X=how;	t_Y=-how;		break;
		case DIRECTION_RIGHT	:	t_X=how; 	t_Y=0;			break;
		case DIRECTION_RIGHTDOWN:	t_X=how;	t_Y=how;		break;
		default					:	dir = DIRECTION_SAME; t_X=0;	t_Y=0;		break;
	}

	if( t_X || t_Y )	{	x += t_X;	y += t_Y;	}
	else				{	x =0;		y = 0;		}						//�ڷ� �и��� ���� �Ͼ�� �ʵ���
	{
    p.h.header.type = CMD_NPC_ATTACK;
		p.u.strike.npc_attack.idCaster = static_cast<WORD>(attacker_npc_id);
		p.u.strike.npc_attack.idTarget = static_cast<WORD>(attacker_npc_ch->targetid);
		
		if (FreeTile(diffender_npc_ch, x>>5, y>>5, x>>5, y>>5))			//���� ���� �ִ� �����ΰ�?
		{
			p.u.strike.npc_attack.nX = x;
			p.u.strike.npc_attack.nY = y;
		}
		else
		{
			p.u.strike.npc_attack.nX = 0;
			p.u.strike.npc_attack.nY = 0;
		}
		
		p.h.header.size = sizeof( t_npc_attack );
		QueuePacket( &p, 1 );    
  }
}

int NPC_NearPosition( CHARACTER *n, CHARACTER *c, int *x, int *y )
{	
	int dirt[ 5] = { 0,1,-1,-2,2 };
	int d, i, j, tempdir;
	int tx, ty;
	int ttx, tty;
	
	if( IsDead( c ) ) return 0;
	
	// ���� ����� ����..
	
	tx = c->x / TILE_SIZE;
	ty = c->y / TILE_SIZE;
	*x = n->x / TILE_SIZE;  *y = n->y / TILE_SIZE;
	tempdir = NPC_GetDir( tx, ty, n->x / TILE_SIZE, n->y / TILE_SIZE );
	
	for( j = 1 ; j < 10 ; j ++)
	{
		for( i = 0 ;  i < 5 ; i ++)
		{
			ttx = tx;	tty = ty;
			d = ( tempdir + 8 + dirt[i] ) %8;
			switch( d )
			{
			case 0 :			tty +=j; break;
			case 1 : ttx -=j;	tty +=j; break;
			case 2 : ttx -=j;			 break;
			case 3 : ttx -=j;	tty -=j; break;
			case 4 :			tty -=j; break;
			case 5 : ttx +=j;	tty -=j; break;
			case 6 : ttx +=j;			 break;
			case 7 : ttx +=j;	tty +=j; break;
			}
			if(FreeTile( n, n->x / TILE_SIZE, n->y / TILE_SIZE, ttx, tty ) )
			{
				*x = ttx;  *y = tty;
				return 1;
			}
		}
	}
	
	return 0;
}

int NPC_NearRoundCh( CHARACTER *n, CHARACTER *ch, int *x, int *y )
{	
	int dirt[ 5] = { 0,1,-1,-2,2 };
	int d, i, j, tempdir;
	int tx, ty;
	int ttx, tty;
	
	if( n->targetid == -1 ) 
	{
		n->targetid = NPC_IsWhoNearPC( n, 10 );
		if( n->targetid == -1 )  return 0;
	}
	
	LPCHARACTER tch = ReturnCharacterPoint( n->targetid );
	if (tch == NULL) return 0; // finito null check
	
	// ���� ����� ����..
	if( IsDead( tch ) ) return 0;
	
	tx = tch->x / TILE_SIZE;
	ty = tch->y / TILE_SIZE;
	*x = n->x / TILE_SIZE;  *y = n->y / TILE_SIZE;
	tempdir = NPC_GetDir( tx, ty, n->x / TILE_SIZE, n->y / TILE_SIZE );
	
	for( j = 1 ; j < 10 ; j ++)
	{
		for( i = 0 ;  i < 5 ; i ++)
		{
			ttx = tx;	tty = ty;
			d = ( tempdir + 8 + dirt[i] ) %8;
			switch( d )
			{
			case 0 :			tty +=j; break;
			case 1 : ttx -=j;	tty +=j; break;
			case 2 : ttx -=j;			 break;
			case 3 : ttx -=j;	tty -=j; break;
			case 4 :			tty -=j; break;
			case 5 : ttx +=j;	tty -=j; break;
			case 6 : ttx +=j;			 break;
			case 7 : ttx +=j;	tty +=j; break;
			}
			if(FreeTile( n, n->x / TILE_SIZE, n->y / TILE_SIZE, ttx, tty ) )
			{
				*x = ttx;  *y = tty;
				return 1;
			}
		}
	}
	return 0;
}	



#define HOME_DIST_1AREA		245
#define HOME_DIST_2AREA		625


int NPC_NearCh( CHARACTER *n, CHARACTER *ch, int *x, int *y )
{			
	int dirt[ 5] = { 0,1,-1,-2,2 };
	int d, i, j, tempdir;
	int tx, ty;
	int ttx, tty;
		
	if( ch == NULL )			return 0;
		
		
	tx = ch->x / TILE_SIZE;
	ty = ch->y / TILE_SIZE;
		
	if( ch->sprno == 0 || ch->sprno == 1 ) // PC��� 
	{	
		if( (rand()%100) < 70 )
		{
			tx = ch->gox / TILE_SIZE;
			ty = ch->goy / TILE_SIZE;
		}
	}	


	DWORD area1, area2;

	switch( n->patterntype )
	{
//------------------------------------------------------------------------------------------------------------

	case  NPC_PATTERN_ACCESS_PC_38_		:
	case  NPC_PATTERN_ATTACK_PC_38_		:
	case  NPC_PATTERN_WANDER_38_		:
	case  NPC_PATTERN_MURI_38_			:
	case  NPC_PATTERN_BACKDRAW_PC_38_	:	area1 = (n->x/TILE_SIZE - n->homex)*(n->x/TILE_SIZE - n->homex) + (n->y/TILE_SIZE - n->homey)*(n->y/TILE_SIZE - n->homey);
											area2 = (tx - n->homex)*(tx - n->homex) + (ty - n->homey)*(ty - n->homey);
											if( area1 > HOME_DIST_1AREA )
											{	
												if( area2 > HOME_DIST_2AREA )
												{
													tx = n->homex;
													ty = n->homey;
												}
											}	
								break;
/*
	case NPC_PATTERN_WANDER_8_			:
	case NPC_PATTERN_IAM_BOSS_8_		:
	case NPC_PATTERN_TOBOSS_8_			:
	case NPC_PATTERN_MURI_8_			:`
	case NPC_PATTERN_ACCESS_PC_8_		:
	case NPC_PATTERN_ATTACK_PC_8_		:
	case NPC_PATTERN_BACKDRAW_8_		:	

	case NPC_PATTERN_IAM_BOSS_28_		:
	case NPC_PATTERN_TOBOSS_28_			:
	case NPC_PATTERN_WANDER_28_			:
	case NPC_PATTERN_MURI_28_			:
	case NPC_PATTERN_BACKDRAW_PC_28_	:	

	case  NPC_PATTERN_IAM_BOSS_GUARD_	:
	case  NPC_PATTERN_TOBOSS_GUARD_		:
	case  NPC_PATTERN_WANDER_GUARD_		:
	case  NPC_PATTERN_MURI_GUARD_		:
	case  NPC_PATTERN_ACCESS_PC_GUARD_	:
	case  NPC_PATTERN_ATTACK_PC_GUARD_	:
	case  NPC_PATTERN_BACKDRAW_PC_GUARD_:	

	case NPC_PATTERN_TAME_				:	
	case NPC_PATTERN_EVENTING_MAUL_		:
	case NPC_PATTERN_WANDER_MAUL_		:
		break;
		*/
	}

	*x = n->x / TILE_SIZE;  *y = n->y / TILE_SIZE;
	tempdir = NPC_GetDir( tx, ty, n->x / TILE_SIZE, n->y / TILE_SIZE );
	
	for( j = 1 ; j < 10 ; j ++)
	{		
		for( i = 0 ;  i < 5 ; i ++)
		{	
			ttx = tx;	tty = ty;
			d = ( tempdir + 8 + dirt[i] ) %8;
			switch( d )
			{
				case 0 :			tty +=j; break;
				case 1 : ttx -=j;	tty +=j; break;
				case 2 : ttx -=j;			 break;
				case 3 : ttx -=j;	tty -=j; break;
				case 4 :			tty -=j; break;
				case 5 : ttx +=j;	tty -=j; break;
				case 6 : ttx +=j;			 break;
				case 7 : ttx +=j;	tty +=j; break;
			}
			if(FreeTile( n, n->x / TILE_SIZE, n->y / TILE_SIZE, ttx, tty ) )
			{
				*x = ttx;  *y = tty;
				return 1;
			}
		}	
	}		
			
	return 0;
}







#define TILE_BLOCK_SIZE  11


BYTE TileBlock[ TILE_BLOCK_SIZE][ TILE_BLOCK_SIZE] = {
	{ 0,0,0,0,1,1,1,0,0,0,0 },
	{ 0,0,1,1,1,1,1,1,1,0,0 },
	{ 0,1,1,1,1,1,1,1,1,1,0 },
	{ 0,1,1,1,1,1,1,1,1,1,0 },
	{ 1,1,1,1,1,1,1,1,1,1,1 },
	{ 1,1,1,1,1,1,1,1,1,1,1 },
	{ 1,1,1,1,1,1,1,1,1,1,1 },
	{ 0,1,1,1,1,1,1,1,1,1,0 },
	{ 0,1,1,1,1,1,1,1,1,1,0 },
	{ 0,0,1,1,1,1,1,1,1,0,0 },
	{ 0,0,0,0,1,1,1,0,0,0,0 }};

BYTE TileBlockBuf[ TILE_BLOCK_SIZE][ TILE_BLOCK_SIZE];

void GetTileBlock( int x, int y )
{

	int sx, sy, ex, ey;

	sx = x - (TILE_BLOCK_SIZE / 2 );
	sy = y - (TILE_BLOCK_SIZE / 2 );
	ex = x + (TILE_BLOCK_SIZE / 2 );
	ey = y + (TILE_BLOCK_SIZE / 2 );

	
	for( ; sx <= ex ; sx ++ )
	{
		for( ; sy <= ey ; sy ++ )
		{

		}
	}
}


void PutTileBlock( int x, int y, int xl, int yl )
{
}



// ���ǿ� ���� ��ġ�� ã�´�. 
int NPC_NearChPart( CHARACTER *n, CHARACTER *ch, int *x, int *y )
{
	int dirt[ 5] = { 0,1,-1,-2,2 };
	int d, i, j, tempdir;
	int tx, ty;
	int ttx, tty;

	if( n->targetid == -1 ) 
	{
		n->targetid = NPC_IsWhoNearPC( n, 10 );
		if( n->targetid == -1 )  return 0;
	}

	// ���� ����� ����..
	LPCHARACTER tch = ReturnCharacterPoint( n->targetid );
	if (tch == NULL) return 0; // finito null check
	if( IsDead( tch ) ) return 0;

	tx = tch->x / TILE_SIZE;
	ty = tch->y / TILE_SIZE;
	*x = n->x / TILE_SIZE;  *y = n->y / TILE_SIZE;
	tempdir = NPC_GetDir( tx, ty, n->x / TILE_SIZE, n->y / TILE_SIZE );
	
	for( j = 1 ; j < 10 ; j ++)
	{
		for( i = 0 ;  i < 5 ; i ++)
		{
			ttx = tx;	tty = ty;
			d = ( tempdir + 8 + dirt[i] ) %8;
			switch( d )
			{
			case 0 :			tty +=j; break;
			case 1 : ttx -=j;	tty +=j; break;
			case 2 : ttx -=j;			 break;
			case 3 : ttx -=j;	tty -=j; break;
			case 4 :			tty -=j; break;
			case 5 : ttx +=j;	tty -=j; break;
			case 6 : ttx +=j;			 break;
			case 7 : ttx +=j;	tty +=j; break;
			}
			if(FreeTile( n, n->x / TILE_SIZE, n->y / TILE_SIZE, ttx, tty ) )
			{
				*x = ttx;  *y = tty;
				return 1;
			}
		}
	}
	return 0;
}	
	
	
// ������ġ���� distŸ�ϸ�ŭ  �ڷ� ������. 
int NPC_NearBackCh( CHARACTER *n, CHARACTER *ch, int *x, int *y, int dist )
{	
	int dirt[ 5] = { 0,1,-1,-2,2 };
	int d, i, j, tempdir;
	int tx, ty;
	int ttx, tty;
	
	if(!n || n->targetid == -1 || !ch) 
	{
//		n->targetid = NPC_IsWhoNearPC( n, 10  );
//		if( n->targetid == -1 )  
		return 0;
	}
	
	// ���� ����� ����..
	LPCHARACTER tch = ReturnCharacterPoint( n->targetid );
	if (tch == NULL) return 0; // finito null check
	if( IsDead( tch ) ) return 0;

	tx = tch->x / TILE_SIZE;
	ty = tch->y / TILE_SIZE;
	*x = n->x / TILE_SIZE;  *y = n->y / TILE_SIZE;
	tempdir = NPC_GetDir( tx, ty, n->x / TILE_SIZE, n->y / TILE_SIZE );
	
 	switch( tempdir )
	{
		case 0 :			ty +=dist;	break;
		case 1 : tx -=dist;	ty +=dist;	break;
		case 2 : tx -=dist;				break;
		case 3 : tx -=dist;	ty -=dist;	break;
		case 4 :			ty -=dist;	break;
		case 5 : tx +=dist;	ty -=dist;	break;
		case 6 : tx +=dist;				break;
		case 7 : tx +=dist;	ty +=dist;	break;
	}
	
	for( j = 0 ; j < 10 ; j ++)
	{
		for( i = 0 ;  i < 5 ; i ++)
		{
			ttx = tx;	tty = ty;
			d = ( tempdir + 8 + dirt[i] ) %8;
			switch( d )
			{
			case 0 :			tty +=j; break;
			case 1 : ttx -=j;	tty +=j; break;
			case 2 : ttx -=j;			 break;
			case 3 : ttx -=j;	tty -=j; break;
			case 4 :			tty -=j; break;
			case 5 : ttx +=j;	tty -=j; break;
			case 6 : ttx +=j;			 break;
			case 7 : ttx +=j;	tty +=j; break;
			}

			if(FreeTile( n, n->x / TILE_SIZE, n->y / TILE_SIZE, ttx, tty ) )
			{
				*x = ttx;  *y = tty;
				return 1;
			}
		}
	}
	
	return 0;
}	
	
	
	
	
//	���� �Ÿ� 
int NPC_GetAttackRange( CHARACTER *n )
{	
	// ���߿� Table������ ��ġ......
	switch( n->sprno )
	{
	case 8 :	return 50 * 50 * 2;
	case 38 :	return 80 * 80 * 2;
	default : 	return 80 * 80 * 2; 
	}
}	
	
	
	
int NPC_GetPatternTable( CHARACTER *n )
{	
	if( n->patterntype >= 20 ) n->patterntype = 0;
	return NpcPatternTable[n->patterntype];
}	
/********************
      3  4  5 
       \ | /
    2- sx,sy -6
       / | \ 
	  1  0  7   
*********************/		 
int NPC_GetDir( int sx, int sy, int ex, int ey )
{	
	if( sx > ex )  // 1/2/3
	{
		if( sy > ey )		return 3;
		else if( sy < ey )	return 1; 
		else				return 2;
	}
	else if( sx < ex ) // 5/6/7
	{
		if( sy > ey )		return 5;
		else if( sy < ey )	return 7; 
		else				return 6;
	}
	else 
	{
		if( sy > ey )		return 4;
		else if( sy < ey )	return 0;
	}
		
	return -1;
}		
			
	
	
	
	
/*	
	
int NPC_MakePath( CHARACTER *n, int tx, int ty, int how = 0 )
{		
	int dir;
	int c, i;
	int nx, ny, px, py;
		
	n->tx = tx;
	n->ty = ty;
	n->how = how;
	
	if( n->pathcount < 2 )
	{
		if( PathBuild( n, tx, ty ) == false ) { return 0; }
	
		int bOdd = 0;
	
		if( n->pathcount )
		{
			px = n->path[ n->pathcount -1][0];
			py = n->path[ n->pathcount -1][1];
	
			c = 0;
			for( i = n->pathcount -2 ; i >= 0 ; i -- )
			{		
				nx = n->path[i][0];
				ny = n->path[i][1];
					
				if( nx > px ) 
				{	
					if( ny > py )		{ dir = 7; }
					else if( ny < py )	{ dir = 5; }
					else				{ dir = 6; }
				}	
				else if( nx < px )
				{	
					if( ny > py )		{ dir = 1; }
					else if( ny < py )	{ dir = 3; }
					else				{ dir = 2; }
				}	
				else 
				{	
					if( ny > py )		{ dir = 0; }
					else				{ dir = 4; }
				}	
	
	
				if (  bOdd == 0){	n->MoveDirTbl[ (c/2) ] = dir << 4;	}
				else			{	n->MoveDirTbl[ (c/2) ] += dir;		}
				c++;
				bOdd = !bOdd;
				px = nx;
				py = ny;
			}
	
			n->MoveLength		= c;
			n->pathcount	= 0;
			n->MoveGox			= tx * TILE_SIZE + 16 -5 + Random(10);
			n->goy			= ty * TILE_SIZE + 16 -5 + Random(10);
			n->WalkTime			= g_ClientTime; // SDL_GetTicks();
			n->MoveType			= 0;
	
			
			return 1;
		}
	
		return 0;
	}		
	
	
	return 0;
}				
	
	
*/	
	

					
						
// �������� �����Ѱ����� �ƴ����� Check�Ѵ�. 
// NPC�� ����ó�� ���� ����. 

// ���� ���°��̸� 0�� �����Ѵ�. 
bool CheckDestinationOk( int tx, int ty )
{					
	LPITEMGROUND  i = g_Item;

	return 1;
					
	while( i != NULL )
	{				
		switch( i->type )
		{			
		case ITEMTYPE_DOOR : 
			{		
				if( Distance( i->x, i->y, tx, ty ) < 18432 ) //Ÿ�� 3�� ����...
				{	
//					return 0;
				}	
			}		

		}			
		i = i->next;
	}	

	return 1;

}		
					
int NPC_MakePath( CHARACTER *n, int tx, int ty, int how = 0 )
{					
	CHARACTER t{};
	// �Ʒ� ���¿����� �̵��� �Ұ���
	if (n->condition&CON_STONE)   return 0;
	if (n->eventnpc)              return 0;

	//if( CheckDestinationOk( tx * TILE_SIZE, ty * TILE_SIZE ) == 0 ) return 0;

	static DWORD time;
	static int   count;
	static DWORD addtime;
	static DWORD temptime;

	time = SDL_GetTicks();


			
	memcpy( &t, n , sizeof( CHARACTER ) );
	t.moveFlag = TRUE;
	t.movetype = 0;
	t.gox = tx * TILE_SIZE + 16;
	t.goy = ty * TILE_SIZE + 16;
	t.destx = tx, t.desty = ty;

	n->MoveP	= 0;

	if ( PathBuild( &t ) )
	{	
			t.moveFlag = FALSE;
			t.movetype += 100;
			SendMoveData( &t );	
			t.movetype -= 100;

			temptime =  SDL_GetTicks() - time;
			addtime += temptime;
			count ++;

			// 010414 KHS deletion for "temptime.txt" file generation...
			//fp = Fopen( "temptime.txt", "at+" );
			//fprintf( fp, "%3d  : %5d    %d \n", count, addtime, addtime / count );
			//fclose(fp);
		
			return 1;
	}		
	
	n->MoveP	= 0;

			temptime =  SDL_GetTicks() - time;
			addtime += temptime;
			count ++;

			// 010414 KHS deletion for "temptime.txt" file generation...
			//fp = Fopen( "temptime.txt", "at+" );
			//fprintf( fp, "%3d  : %5d    %d \n", count, addtime, addtime / count );
			//fclose(fp);

	return 0;
}
	
	
	
/*	
int NPC_MakePath( CHARACTER *n, int tx, int ty, int how = 0 )
{		
	int dir;
	int c, i;
	int nx, ny, px, py;


	if( g_block_move )
	{
		return 0;
	}

	n->destx = tx;
	n->desty = ty;
	n->how = how;
	if( n->pathcount < 2 )
	{
		if( PathBuild( n, tx, ty ) == false ) { return 0; }
	
		int bOdd = 0;
	
		if( n->pathcount )
		{
			px = n->path[ n->pathcount -1][0];
			py = n->path[ n->pathcount -1][1];
	
			c = 0;
			for( i = n->pathcount -2 ; i >= 0 ; i -- )
			{		
				nx = n->path[i][0];
				ny = n->path[i][1];
					
				if( nx > px ) 
				{	
					if( ny > py )		{ dir = 7; }
					else if( ny < py )	{ dir = 5; }
					else				{ dir = 6; }
				}	
				else if( nx < px )
				{	
					if( ny > py )		{ dir = 1; }
					else if( ny < py )	{ dir = 3; }
					else				{ dir = 2; }
				}	
				else 
				{	
					if( ny > py )		{ dir = 0; }
					else				{ dir = 4; }
				}	
	
	
				if (  bOdd == 0){	n->MoveDirTbl[ (c/2) ] = dir << 4;	}
				else			{	n->MoveDirTbl[ (c/2) ] += dir;		}
				c++;
				bOdd = !bOdd;
				px = nx;
				py = ny;
			}
	
			n->MoveLength		= c;
			n->pathcount	= 0;
			n->MoveGox			= tx * TILE_SIZE + 16 -5 + Random(10);
			n->goy			= ty * TILE_SIZE + 16 -5 + Random(10);
			n->WalkTime			= g_ClientTime; // SDL_GetTicks();
			n->MoveType			= 0;
	
	

			return 1;
		}
	

		return 0;

	}		
	
	

	return 0;
}			*/
	
/*	
int NPC_MakePath( CHARACTER *n, int tx, int ty, int how = 0 )
{
	return NPC_MakePathBumn( n, tx, ty, how );
}
*/



int NPC_MakePathBumn( CHARACTER *n, int tx, int ty, int how = 0 )
{
	CHARACTER t{};

	// 010515 KHS
	if( n->eventnpc ) return 0;

	//if( CheckDestinationOk( tx * TILE_SIZE, ty * TILE_SIZE ) == 0 ) return 0;
			
	memcpy( &t, n , sizeof( CHARACTER ) );
	t.moveFlag = TRUE;
	t.movetype = 0;
	t.gox = tx * TILE_SIZE + 16;
	t.goy = ty * TILE_SIZE + 16;
	t.destx = tx, t.desty = ty;

	n->MoveP = 0;

	if ( PathBuild( &t ) )
	{	
			t.moveFlag = FALSE;
			t.movetype += 100;
			SendMoveData( &t );	
			t.movetype -= 100;
			
			//n->sendmove = 1;

			return 1;
	}		

	//n->sendmove = 0;
	return 0;

	
}			


int NPC_MakePathGuard( CHARACTER *n, int tx, int ty, int how )
{
	CHARACTER t{};

	// 010515 KHS
	if( n->eventnpc ) return 0;

	if( CheckDestinationOk( tx * TILE_SIZE, ty * TILE_SIZE ) == 0 ) return 0;
			
	memcpy( &t, n , sizeof( CHARACTER ) );
	t.moveFlag = TRUE;
	t.movetype = 0;
	t.gox = tx * TILE_SIZE + 16;
	t.goy = ty * TILE_SIZE + 16;
	t.destx = tx, t.desty = ty;

	t.MoveP = 0;

	if ( PathBuild( &t ) )
	{	
			t.moveFlag = FALSE;
			t.movetype += 100;
			SendMoveData( &t );	
			t.movetype -= 100;

			return 1;
	}		

	return 0;
}			


int NPC_MakePathMaul( CHARACTER *n, int tx, int ty, int how )
{
	CHARACTER t{};
					
	// 010515 KHS
	if( n->eventnpc ) return 0;
	if( CheckDestinationOk( tx * TILE_SIZE, ty * TILE_SIZE ) == 0 ) return 0;
			
	memcpy( &t, n , sizeof( CHARACTER ) );
	t.moveFlag = TRUE;
	t.movetype = 0;
	t.gox = tx * TILE_SIZE + 16;
	t.goy = ty * TILE_SIZE + 16;
	t.destx = tx, t.desty = ty;

	t.MoveP = 0;

	if ( PathBuild( &t ) )
	{		
		t.moveFlag = FALSE;
		t.movetype += 100;

		SendMoveData( &t );	
		t.movetype -= 100;
		
		return 1;
	}		

	return 0;
}			



/*

int NPC_MakePathBumn( CHARACTER *n, int tx, int ty, int how )
{
	int dir;
	int oldx, oldy;
	int num;
	int i;

	n->tx = tx;
	n->ty = ty;
	n->how = how;

	if( n->pathcount < 2 )
	{
		int x = n->x / TILE_SIZE;
		int y = n->y / TILE_SIZE;

		dir  = NPC_GetDir( x, y, tx, ty );

		for( i = 0 ; i < how ; i ++)
		{
			oldx = x; oldy = y;
			
			switch( dir )
			{
			case 0 :	   y ++; break;
			case 1 : x --; y ++; break;
			case 2 : x --;       break;
			case 3 : x --; y --; break;
			case 4 :       y --; break;
			case 5 : x ++; y --; break;
			case 6 : x ++;       break;
			case 7 : x ++; y ++; break;
			}
			
			if( FreeTile( n->x / TILE_SIZE, n->y / TILE_SIZE, x, y ) )
			{
				if (  i%2 == 0){	n->MoveDirTbl[ (i/2) ] = dir << 4;	}
				else			{	n->MoveDirTbl[ (i/2) ] += dir;		}
			}
			else 
			{
				x = oldx;
				y = oldy;
				break;
			}
		}	
/		n->MoveLength		= i;
		n->pathcount	= 0;
		n->MoveGox			= x * TILE_SIZE + 16;
		n->goy			= y * TILE_SIZE + 16;
		n->WalkTime			= g_ClientTime; // SDL_GetTicks();
		n->MoveType			= 0;

		printf( "NPC Move... \n");
	}		
			
	return i;
}			
*/			
	
/*
	Added by Tya @ 12 Mar 2006
extern allows the client to be compiled with optimizations on. If not a linker error will be thrown.
*/		
bool NPC_IsMoving(CHARACTER* n)
{	//< CSD-040107
	return (n->desttime > g_ClientTime) ? true:false;
}	//> CSD-040107
			
int NPC_Hostile( CHARACTER *n )
{			
	return n->peacests;
}			
				
			
int NPC_StillAttack( CHARACTER *n)
{			
	if( n->attackcount > 6 +Random(5) )
	{		
		n->attackcount = 0;
		return 0;
	}		
			
	return 1;
}			
			
				
int NPC_IsAttackableRange( CHARACTER *n )
{			
	CHARACTER *ch;
	int t;	
			
	int x;	
	int y;	
	int tx;	
	int ty;	
			
	
	if( n->targetid < 0 ) 	goto FAIL_;
	
	ch = ReturnCharacterPoint( n->targetid );
	if( ch == NULL ) goto FAIL_;
	
	if(	IsDead( ch ) ) goto FAIL_;
	
	x = n->x;
	y = n->y;
	tx = ch->x;
	ty = ch->y;
	t = (tx-x)*(tx-x) + (ty-y)*(ty-y);
	if( t < NPC_GetAttackRange(n)  )  // ### ������ �����Ÿ� �ʿ�..
	{		
		// ###���� ���̿� ��� ���� ������ return 0;
	
		return 1;
	}	
	
FAIL_:
	
	return 0;
}		
		
int NPC_IsInBossRange( CHARACTER *n )
{		
	
	if( n->bossid == -1 ) return 0;
		
	LPCHARACTER boss = ReturnCharacterPoint( n->bossid );
	if( boss == NULL ) return 0;


	if( !IsDead( boss ) )
	{
		if( BoxAndDotCrash( boss->x / TILE_SIZE-4, boss->y / TILE_SIZE - 4, 8, 8, n->x / TILE_SIZE, n->y / TILE_SIZE ))
		{
			return 1;
		}
	}	
		
	return 0;
}	
	
int NPC_IsInHostRange( CHARACTER *n )
{		
	if( n->HostId == -1 ) return 0;

	CHARACTER *host = ReturnCharacterPoint( n->HostId );
	if( host == NULL ) return 0;

	if( strcmp( host->name, n->HostName ) ) return 0; // ID�� �̸��� �޶�......
	
	if( !IsDead( host ) )
	{	
		if( BoxAndDotCrash( host->x / TILE_SIZE-4, host->y / TILE_SIZE - 4, 8, 8, n->x / TILE_SIZE, n->y / TILE_SIZE ))
		{
			return 1;
		}
	}	
		
	return 0;
}


// ���� ����� ����ϸ� pk�� ���� ã�´�. 
//
int NPC_IsWhoNearRedPC(CHARACTER *npc, int range)
{	//< CSD-040107
	LPCHARACTER tempch = NULL;
	
#ifdef _NATION_APPLY_
	const int nation = MapInfo[MapNumber].nation;
	// ���� �ٸ� ������� ������ �׳��� �����Ѵ�.
	if (MapNumber != 30)
	{
		if (npc->nRace == GUARD)
		{
			tempch = Hero;

			while (tempch)
			{
				if (!IsDead(tempch) && tempch->lv > 7 && !tempch->IsCounselor())
				{
					if (tempch->sprno == 1 || tempch->sprno == 0)
					{
						if (IsNK(tempch, nation) >= 5 || IsEnemyNation(tempch->name_status.nation, nation))
						{
							return tempch->id;
						}
					}
				}
				
				tempch = tempch->lpNext;
			}
		}
	}
#endif
	tempch = Hero;

	while (tempch)
	{
		if (!IsDead(tempch))
		{
			if (IsNK(tempch) >= 5 || tempch->dummy_pk > 10)
			{
				return tempch->id;
			}
		}
		
		tempch = tempch->lpNext;
	}
	
	return -1;
}	//> CSD-040107


// NPC�� ���� PC�� �ִ°�?
int NPC_IsTherePC( CHARACTER *npc, int range )
{
	LPCHARACTER tempch = Hero;
	while( tempch )
	{
		if( !IsDead( tempch )) 
		if( tempch->type != SPRITETYPE_NPC && tempch->type != SPRITETYPE_MONSTER )
		{
			return tempch->id;
		}

		tempch = tempch->lpNext;
	}
	return -1;
}




int NPC_IsWhoNearOtherNationPc( int npc_nation, LPCHARACTER npc, int range )
{
	int x = npc->x;		
	int y = npc->y;		
	int tx, ty;			
	DWORD tmin, min = 0xFFFFFFFF;
	int	findch = -1;	
	int	sx, ex;			
	int sy, ey;			
						
	LPCHARACTER tempch;	
						
	int mx  = x / TILE_SIZE;
	int my  = y / TILE_SIZE;
						
	sx = mx - range;	if( sx < 0 ) sx = 0;
	ex = mx + range;    if( ex >= g_Map.file.wWidth )  ex = g_Map.file.wWidth-1;
	sy = my - range;	if( sy < 0 ) sy = 0;
	ey = my + range;    if( ey >= g_Map.file.wHeight ) ey = g_Map.file.wHeight -1;
						
	tempch = Hero;		
	while( tempch )		
	{					
		if( !IsDead( tempch )) 
		{				
			switch( tempch->sprno )
			{			
			case 0 :	
			case 1 :	
//			case 18 :	
//			case 19 :	
//			case 20 :	
						if( npc_nation == tempch->name_status.nation ) break;
						tx = tempch->x;
						ty = tempch->y;
						tmin = (tx-x)*(tx-x) + (ty-y)*(ty-y);
						if( tmin < min ) 
						{
							min = tmin;
							findch = tempch->id;
						}
					break;
			}			
		}				
		tempch = tempch->lpNext;
	}					
						
	return findch;		
}

int NPC_IsWhoNearPC( CHARACTER *npc, int range  )
{							
	int x = npc->x;		
	int y = npc->y;		
	int tx, ty;			
	DWORD tmin, min = 0xFFFFFFFF;
	int	findch = -1;	
	int	sx, ex;			
	int sy, ey;			
						
	LPCHARACTER tempch;	
						
	int mx  = x / TILE_SIZE;
	int my  = y / TILE_SIZE;
						
	sx = mx - range;	if( sx < 0 ) sx = 0;
	ex = mx + range;    if( ex >= g_Map.file.wWidth )  ex = g_Map.file.wWidth-1;
	sy = my - range;	if( sy < 0 ) sy = 0;
	ey = my + range;    if( ey >= g_Map.file.wHeight ) ey = g_Map.file.wHeight -1;
						
	tempch = Hero;		
	while( tempch )		
	{	
		if (!IsDead(tempch) && !(tempch->condition&CON_STONE) && !(tempch->viewtype == VIEWTYPE_OBSERVE_))
		{	//< CSD-TW-030606
			switch( tempch->sprno )
			{			
			case 0:	
			case 1:	
				{
					tx = tempch->x;
					ty = tempch->y;
					tmin = (tx-x)*(tx-x) + (ty-y)*(ty-y);
					
					if( tmin < min ) 
					{
						min = tmin;
						findch = tempch->id;
					}

					break;
				}
			}			
		}	//> CSD-TW-030606

		tempch = tempch->lpNext;
	}					
						
	return findch;		
}						
							
int NPC_WhoIsAttackMyBaby( CHARACTER *n )
{							
	LPCHARACTER tempch = Hero;
	while( tempch )		
	{					
		if( !IsDead( tempch ) && n != tempch && tempch->type == SPRITETYPE_MONSTER )
		{					
			if( tempch->bossid == n->id )
			{			
				if( tempch->attacked != -1 )
				{		
					tempch->attacked = -1;
					return tempch->id;
				}		
			}			
		}				
		tempch = tempch->lpNext;
	}					
							
	return -1;
}			
			
			
			
			
			
int NPC_WhoIsBoss( CHARACTER *n )
{					
	DWORD	maxexp = 0;	
	int		x = n->x, y = n->y;
	int		bossid = -1;
	int		ex, ey;
	DWORD	dis;
	int		c = 0;
				
//	n->bossid = -1;	
				
	LPCHARACTER tempch = Hero;
	while( tempch )
	{			
		if( !IsDead( tempch ) && tempch->type == SPRITETYPE_MONSTER )
		{		
			if( n->sprno == tempch->sprno )
			{	
				ex = tempch->x;	ey = tempch->y;
				dis = (ex-x)*(ex-x)+(ey-y)*(ey-y);
				if( dis < (DWORD)(TILE_SIZE * 10)*(DWORD)(TILE_SIZE * 10) )
				{	
					c++;
					if( tempch->Exp > maxexp )
					{
						maxexp		= tempch->Exp;
						bossid		= tempch->id;
					}
				}	
			}	
		}		
		tempch = tempch->lpNext;
	}			

	if( c == 1 ) bossid = -1;	// ���ۿ� Checkking�� ���� �ʾҴٴ� ��� ............
	return bossid;
}




int NPC_WhoIsBlocking( int sx, int sy )
{
	LPCHARACTER tempch = Hero;
	while( tempch )
	{
		if( !IsDead( tempch ) )
		{
			if( tempch->x / TILE_SIZE && tempch->y/ TILE_SIZE )
			{
				return tempch->id;
			}
		}

		tempch = tempch->lpNext;
	}
	return -1;
}

CHARACTER *NPC_ReturnCharListPoint( int id )
{
	if( id < 0 ) return NULL;

	LPCHARACTER tempch = ReturnCharacterPoint( id );
	if( tempch == NULL ) return NULL;

	if( IsDead( tempch) ) return NULL;
		
	return tempch;
}
		
CHARACTER *ReturnCharListPoint( char *name )
{		
	if( name == NULL ) return NULL;

	LPCHARACTER tempch = Hero;
	while( tempch )
	{	
		if( strcmp( tempch->name, name ) == 0 && tempch->type != SPRITETYPE_MONSTER )
		{
			return tempch;
		}

		tempch = tempch->lpNext;
	}
	
	return NULL;
}
		
		
		
		
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		User Functions.....
//


void NPC_Pattern_28( LPCHARACTER n)
{			
	int ex, ey;
			
	switch( n->patterntype )
	{		
	case NPC_PATTERN_WANDER_28_ :

			int bossid;
			if( g_ClientTime - n->aitimedelay > n->aidelayhowmuch ) 
			{	
				n->aitimedelay = g_ClientTime;
				n->aidelayhowmuch = 1000  + (rand()%1000);
			}
			else break;

				// �׳� ��ȸ�Ѵ�. 
			if( NPC_IsMoving( n ) )
			{
			}
			else 
			{
				int targetid = NPC_IsWhoNearPC( n, 10 );
				if( targetid != -1 )
				{
					if( n->bossid == -1 )
					{
						n->patterntype = NPC_PATTERN_BACKDRAW_PC_28_;
						n->targetid = targetid;
						break;
					}		
					else	
					{		
						LPCHARACTER tempch = Hero;
						while( tempch )
						{	
							if( tempch->patterntype != NPC_PATTERN_TAME_ )
							if( !IsDead( tempch ) && tempch->type == SPRITETYPE_MONSTER ) 
							{
								if( tempch->bossid == n->bossid )
								{
									tempch->targetid	= n->targetid;
									tempch->patterntype = NPC_PATTERN_BACKDRAW_PC_28_;
								}
							}
							tempch = tempch->lpNext;
						}	
						break;
					}			
				}			
				else		
				{			
					bossid = NPC_WhoIsBoss( n );
					if( bossid != -1 )	// Yes.. ������ ã�Ҿ�!!
					{		
						if( bossid != n->id )	// ��. ������ Exp�� ���� ���� �ֱ�.. 
						{	
							n->bossid = bossid;
							SendNPC_parameter( n->id, NPC_PARAMETER_BOSSID, bossid );
							n->patterntype = NPC_PATTERN_TOBOSS_28_;
							n->targetid = -1;
							break;
						}	
						else  // ���� Boss�̴�. 
						{	
							n->bossid = bossid;
							SendNPC_parameter( n->id, NPC_PARAMETER_BOSSID, bossid );
							n->patterntype = NPC_PATTERN_IAM_BOSS_28_;
							n->targetid = -1;
							break;
						}	
					}		
					else	// ��. ������ ����...
					{		
						int dir;
						int how;
						int tx, ty;
							
						dir = Random(8);
						how = Random(8)+2;
						tx = n->x / TILE_SIZE;
						ty = n->y / TILE_SIZE;
						switch( dir )
						{
							case 0 :			ty +=how; break;
							case 1 : tx -=how;	ty +=how; break;
							case 2 : tx -=how;            break;
							case 3 : tx -=how;	ty -=how; break;
							case 4 :			ty -=how; break;
							case 5 : tx +=how;	ty -=how; break;
							case 6 : tx +=how;		      break;
							case 7 : tx +=how;	ty +=how; break;
						}
						NPC_MakePathBumn( n, tx, ty, how );
					}
				}
			}
			
		break;

	case NPC_PATTERN_IAM_BOSS_28_ :
		{
			int bossid;

			if( g_ClientTime - n->aitimedelay > n->aidelayhowmuch ) 
			{	
				n->aitimedelay = g_ClientTime;
				n->aidelayhowmuch = 1000  + (rand()%1000);
				if( NPC_IsMoving( n ) )
				{


				}
				else 
				{
					bossid = NPC_WhoIsBoss( n );
					if( bossid == -1 )	 // ��ȥ�ڱ�..
					{	
						n->patterntype	= NPC_PATTERN_WANDER_28_;

						n->targetid		= -1;
						n->bossid		= -1;
						SendNPC_parameter( n->id, NPC_PARAMETER_BOSSID,   -1);
						SendNPC_parameter( n->id, NPC_PARAMETER_TARGETID, -1);
						break;
					}
					else if( bossid != n->id )	// dld��. ������ Exp�� ���� ���� �ֱ�.. 
					{
							n->bossid		= bossid;
							SendNPC_parameter( n->id, NPC_PARAMETER_BOSSID, bossid );
							n->patterntype	= NPC_PATTERN_TOBOSS_28_;

							break;
					}
					else  // ���� ���� Boss�� !  �׷� ������ ���� �ؾ���.
					{
						n->bossid = bossid;


						int attacker = NPC_WhoIsAttackMyBaby( n );
						if( attacker != -1) // �� �ư����� �´¾ְ� ������..
						{
							LPCHARACTER tempch = Hero;
							while( tempch )
							{	
								if( !IsDead( tempch ) && tempch->type == SPRITETYPE_MONSTER && tempch != n ) 
								{
									if( tempch->bossid == n->bossid )
									{
										tempch->targetid	= n->targetid;
										SendNPC_parameter( n->id, NPC_PARAMETER_TARGETID,n->targetid );
										tempch->patterntype = NPC_PATTERN_BACKDRAW_PC_28_;
									}
								}
								tempch = tempch->lpNext;
							}	
							break;
						}
						else
						{
							n->targetid = NPC_IsWhoNearPC( n, 6 );
							LPCHARACTER tempch = ReturnCharacterPoint( n->targetid );
							if (tempch) // finito null check
							{
								if( n->targetid == -1 )
								{
									if( (Random(15)) == 0 )
									{// �������� �ణ�� �����δ�. 
										int dir;
										int how;
										int tx, ty;

										dir = Random(8);
										how = Random(2)+2;
										tx = n->x / TILE_SIZE;
										ty = n->y / TILE_SIZE;
										switch( dir )
										{
											case 0 :			ty +=how; break;
											case 1 : tx -=how;	ty +=how; break;
											case 2 : tx -=how;            break;
											case 3 : tx -=how;	ty -=how; break;
											case 4 :			ty -=how; break;
											case 5 : tx +=how;	ty -=how; break;
											case 6 : tx +=how;		      break;
											case 7 : tx +=how;	ty +=how; break;
										}
		
										//n->MoveP = Random(50) + 50;
										NPC_MakePathBumn( n, tx, ty, how );
									}

								}
								else
								{
									if( IsDead( tempch ) )
									{
										n->targetid = -1;
									}
									else
									{
										if( InDistance( n, tempch,  TILE_SIZE * 7 ) )	// �� PC��  5 Ÿ�Ͼȿ� �ִٸ�.
										{	
											LPCHARACTER tempch = Hero;
											while( tempch )
											{	
												if( !IsDead( tempch ) && tempch->type == SPRITETYPE_MONSTER && tempch != n ) 
												{
													if( tempch->bossid == n->bossid )
													{
														tempch->targetid	= n->targetid;
														SendNPC_parameter( n->id, NPC_PARAMETER_TARGETID, n->targetid );
														tempch->patterntype = NPC_PATTERN_BACKDRAW_PC_28_;
													}
												}
												tempch = tempch->lpNext;
											}	
										}
									}
								}
							}
						}
					}
				}
			}
		}
		break;

	case NPC_PATTERN_TOBOSS_28_ :
		{
			int ex, ey;
			int bossid;

			if( g_ClientTime - n->aitimedelay > n->aidelayhowmuch ) 
			{	
				n->aitimedelay = g_ClientTime;
				n->aidelayhowmuch = 5000  + (rand()%2000);

				if( NPC_IsMoving( n ) )
				{

				}
				else 
				{
					int targetid = NPC_IsWhoNearPC( n, 10 );
					if( targetid != -1 )
					{
						n->patterntype = NPC_PATTERN_BACKDRAW_PC_28_;
						n->targetid = targetid;
						break;
					}

					if( NPC_IsInBossRange( n ) )	// Boss�ȿ� ��������..
					{
						bossid = NPC_WhoIsBoss( n );
						if( bossid != -1 )	// ȥ�ڶ�� ���.
						{	
							if( bossid != n->id )	// ��. ������ Exp�� ���� ���� �ֱ�.. 
							{
								n->bossid = bossid;
								SendNPC_parameter( n->id, NPC_PARAMETER_BOSSID, bossid);
								n->patterntype = NPC_PATTERN_TOBOSS_28_;

								goto TO_BOSS_;
							}
							else  // ���� Boss�̴�. 
							{
								n->bossid = bossid;
								n->patterntype = NPC_PATTERN_IAM_BOSS_28_;

								n->targetid = -1;
								SendNPC_parameter( n->id, NPC_PARAMETER_BOSSID, bossid);
								SendNPC_parameter( n->id, NPC_PARAMETER_TARGETID, -1 );
							}
						}
						else 
						{
							n->patterntype = NPC_PATTERN_WANDER_28_;

							n->targetid = -1;
							n->bossid	= -1;
							SendNPC_parameter( n->id, NPC_PARAMETER_BOSSID, -1 );
							SendNPC_parameter( n->id, NPC_PARAMETER_TARGETID, -1 );

							break;
						}
					}
					else //  Boss������ ������...
					{
TO_BOSS_:				if( n->bossid != -1 )
						{
							LPCHARACTER tempch = ReturnCharacterPoint( n->bossid );
							if (tempch) // finito null check
							{
								if( NPC_NearPosition( n, tempch, &ex, &ey ))
								{
									NPC_MakePath( n, ex, ey, Random(6));
									break;
								}
							}
						}
						else
						{
							n->patterntype = NPC_PATTERN_MURI_28_;
							n->targetid = -1;
							break;
						}
					}
				}
			}
		}
		break;
			
		//	��������ٴѴ�... 28
		case NPC_PATTERN_MURI_28_ :
		{	
			int bossid = -1;
			if( g_ClientTime - n->aitimedelay > n->aidelayhowmuch ) 
			{	
				n->aitimedelay = g_ClientTime;
				n->aidelayhowmuch = 2000  + (rand()%1000);

				if( NPC_IsMoving( n ) )
				{
				}
				else
				{
					bossid = NPC_WhoIsBoss( n );
					if( bossid != -1 )	// ȥ�ڶ�� ���.
					{	
						if( bossid != n->id )	// ��. ������ Exp�� ���� ���� �ֱ�.. 
						{
							n->bossid = bossid;
							SendNPC_parameter( n->id, NPC_PARAMETER_BOSSID, bossid);
							n->patterntype = NPC_PATTERN_TOBOSS_28_;
						}
						else  // ���� Boss�̴�. 
						{
							n->bossid = bossid;
							n->patterntype = NPC_PATTERN_IAM_BOSS_28_;
							n->targetid = -1;
							SendNPC_parameter( n->id, NPC_PARAMETER_BOSSID, bossid);
							SendNPC_parameter( n->id, NPC_PARAMETER_TARGETID, -1 );

						}
					}
					else 
					{
						n->patterntype = NPC_PATTERN_WANDER_28_;
						n->bossid = -1;
						n->targetid = -1;
						SendNPC_parameter( n->id, NPC_PARAMETER_BOSSID, -1);
						SendNPC_parameter( n->id, NPC_PARAMETER_TARGETID, -1);

					}
				}
			}	
		}		
		break;	
				
								
		case NPC_PATTERN_BACKDRAW_PC_28_ :
			{					
			if( g_ClientTime - n->aitimedelay > n->aidelayhowmuch ) 
			{	
				n->aitimedelay = g_ClientTime;
				n->aidelayhowmuch = 1000  + (rand()%400);
								
					if( NPC_IsMoving( n ) )
					{			
								
					}			
					else		
					{			
						if( n->targetid != -1 )
						{		
							LPCHARACTER tempch  = ReturnCharacterPoint( n->targetid );
							if (tempch) // finito null check
							{
								if( InDistance( n, tempch,  TILE_SIZE * 10 ) )	// �� PC��  10 Ÿ�Ͼȿ� �ִٸ�.
								{	
									if( NPC_NearBackCh( n, tempch, &ex, &ey, 10 ) )
									{	
										n->MoveP = 140 + Random(120);
										if( NPC_MakePath( n, ex, ey, Random(10)+5 ) )
										{
										}
										else 
										{
											n->targetid = -1;
											n->patterntype = NPC_PATTERN_WANDER_28_;
										}
									}	
									else 
									{
									}
								}
								else	// ����� �ָ��ֱ�.
								{
									n->patterntype = NPC_PATTERN_WANDER_28_;
								}
							}
						}		
						else	
						{		
							n->patterntype = NPC_PATTERN_WANDER_28_;
						}		
					}				
				}				
				break;			
			}				
		}

}


void NPC_Pattern_38( LPCHARACTER n)
{
	int ex, ey;

	switch( n->patterntype )
	{
	case NPC_PATTERN_WANDER_38_ :

			if( g_ClientTime - n->aitimedelay > n->aidelayhowmuch ) 
			{	
				n->aitimedelay = g_ClientTime;
				n->aidelayhowmuch = 2000  + (rand()%500);
			}
			else break;

				// �׳� ��ȸ�Ѵ�. 
			if( NPC_IsMoving( n ) )
			{
			}
			else 
			{
				int targetid = NPC_IsWhoNearPC( n, 15 );
				if( targetid != -1 )
				{
					n->patterntype = NPC_PATTERN_ACCESS_PC_38_;
					n->targetid = targetid;
				}
				else 
				{
						int dir;
						int how;
						int tx, ty;

						dir = Random(8);
						how = Random(8)+2;
						tx = n->x / TILE_SIZE;
						ty = n->y / TILE_SIZE;
						switch( dir )
						{
							case 0 :			ty +=how; break;
							case 1 : tx -=how;	ty +=how; break;
							case 2 : tx -=how;            break;
							case 3 : tx -=how;	ty -=how; break;
							case 4 :			ty -=how; break;
							case 5 : tx +=how;	ty -=how; break;
							case 6 : tx +=how;		      break;
							case 7 : tx +=how;	ty +=how; break;
						}

						NPC_MakePathBumn( n, tx, ty, how );

				}
			}
			
		break;

		case NPC_PATTERN_ACCESS_PC_38_ :
			{	
			if( g_ClientTime - n->aitimedelay > n->aidelayhowmuch ) 
			{	
					n->aitimedelay = g_ClientTime;
					
					n->aidelayhowmuch = PCFINDACCESS(n);  // 0813 NPC KHS

					if( NPC_IsMoving( n ) )
					{
						LPCHARACTER tempch = ReturnCharacterPoint( n->targetid );
						if( tempch )
						{
							if( !IsDead( tempch ) )
							{
								if( NPC_IsAttackableRange( n ) )
								{
									SendModifyPositionNPC( n->id ); 
									SendNPCAttack( n->id );
									n->attackcount ++;
									n->aitimedelay = g_ClientTime;		//  �̰� �����ϱ�.. ��Ÿ�� ���´�. 
									n->aidelayhowmuch = ATTACKACCESS(n)  + (rand()%300);
									n->patterntype = NPC_PATTERN_ATTACK_PC_38_;
									break;
								}
							}
						}
					}
					else
					{
//						if( NPC_Hostile( n ) ) // ���ݸ���̸�...
						LPCHARACTER tempch = ReturnCharacterPoint( n->targetid );
						if( tempch )
						{
							if( !IsDead( tempch ) )
							{
								if( NPC_IsAttackableRange( n ) )// ���� ������ �Ÿ����� �Գ� ?
								{
									SendNPCAttack( n->id );
									n->attackcount ++;
									n->aitimedelay = g_ClientTime;		//  �̰� �����ϱ�.. ��Ÿ�� ���´�. 
									n->aidelayhowmuch = ATTACKACCESS(n)  + (rand()%600);
									n->patterntype = NPC_PATTERN_ATTACK_PC_38_;

									break;
								}
								else	// ���ݰ����� �Ÿ��� �ƴϸ� ��ó���� ����. 
								{
									if( NPC_NearCh( n, tempch, &ex, &ey ) )
									{
										n->MoveP = 60 + Random(30 );
										if( NPC_MakePath( n, ex, ey, Random(6) + 2 ) )
										{
											n->patterntype = NPC_PATTERN_ATTACK_PC_38_;
										}
										else
										{
											n->patterntype = NPC_PATTERN_WANDER_38_;
										}
									}
									else
									{
										n->patterntype = NPC_PATTERN_WANDER_38_;

	//									n->bossid = -1;
	//									n->server_id = -1;
									}
								}
							}	
							else 
							{
								n->patterntype = NPC_PATTERN_WANDER_38_;
								n->targetid = -1;
								break;
							}
						}
					}	
				}		
			}			
			break;		
						
		case NPC_PATTERN_ATTACK_PC_38_ :
			{
				LPCHARACTER tempch = nullptr;
				if( g_ClientTime - n->aitimedelay > n->aidelayhowmuch ) 
				{		
					n->aitimedelay = g_ClientTime;
					n->aidelayhowmuch =  ATTACKACCESS(n)  + (rand()%600);
				}	
				else break;
					
					
				if( NPC_IsMoving(n) )	//  ���� �̵����̸� 
				{	
					tempch = ReturnCharacterPoint( n->targetid );
					if( tempch )
					{
						if( !IsDead( tempch ) )
						if( NPC_IsAttackableRange( n ) )	// �̵� ���� ���ݴ��ĳ���� �����Ҽ� �ִٸ� �����Ѵ�. 
						{
							goto ATTACK_38_NEXT__;
						}
					}
					break;
				}	
					
				tempch = ReturnCharacterPoint( n->targetid );
				if (tempch) // finito null check
				{
					if( NPC_IsAttackableRange( n ) )// ���� ������ �Ÿ����� �Գ� ?
					{					
ATTACK_38_NEXT__:	
						if( NPC_StillAttack(n) )
						{											
							SendNPCAttack(  n->id );
							n->attackcount ++;
							//n->patterntype = NPC_PATTERN_WANDER_38_;	
						}								
						else							
						{											
							if( NPC_NearBackCh( n, tempch, &ex, &ey, 5 ) )
							{													
								n->MoveP = 70 + Random(30 );
								if( NPC_MakePath( n, ex, ey, Random(2)+2 ) ) 
								{
									n->patterntype =NPC_PATTERN_ACCESS_PC_38_;		//WANDER_8_;
								}
								else
								{
									n->targetid = -1;
									n->patterntype =NPC_PATTERN_WANDER_38_;		//WANDER_8_;
								}
							}									
							else						
							{							
								n->patterntype =NPC_PATTERN_ACCESS_PC_38_;		//WANDER_8_;
							}		
						}			
					}				
					else			
					{				
						if( n->targetid != -1 )
						{			
							if( !IsDead( tempch ) )
							{		
								if( tempch == NULL ) 
								{
									n->patterntype =NPC_PATTERN_WANDER_38_;		//WANDER_8_;
								}
								else if( NPC_NearCh( n, tempch, &ex, &ey ) )
								{		
									//if( NPC_EnemyOnMyWay( 		
									// �ϵ��� ���ݻ�뿡�� �����ϱ� ���� ��ƾ�� �ִ´�. ( '��' '��'���� 
									n->MoveP = 60 + Random(30 );
									if( NPC_MakePath( n, ex, ey, Random(6)+2 ) ) 
									{
										n->patterntype =NPC_PATTERN_ACCESS_PC_38_;		//WANDER_8_;
									}
									else
									{
										n->targetid = -1;
										n->patterntype =NPC_PATTERN_WANDER_38_;		//WANDER_8_;
									}	
								}			
								else		
								{	
									n->targetid = -1;
									n->patterntype = NPC_PATTERN_WANDER_38_;
	//								n->bossid	 = -1;
	//								n->server_id = -1;
	//								JustMessage( " %d�� Pattern��ȣ %d ", n->server_id, n->patterntype );
	//								JustMessage( " %d�� Pattern��ȣ %d ", n->patterntype );
								}				
							}
							else 
							{
								n->patterntype =NPC_PATTERN_WANDER_38_;		//WANDER_8_;
							}
						}		
						else
						{
							n->patterntype =NPC_PATTERN_WANDER_38_;		//WANDER_8_;
						}
					}
				}				
			break;					
			}
		
		case NPC_PATTERN_BACKDRAW_PC_38_ :
			{					
			if( g_ClientTime - n->aitimedelay > n->aidelayhowmuch ) 
			{	
				n->aitimedelay = g_ClientTime;
				n->aidelayhowmuch = 500  + (rand()%500);

				LPCHARACTER tempch = ReturnCharacterPoint( n->targetid );
				if (tempch) // finito null check
				{
					if( NPC_IsMoving( n ) )
					{			
								
					}			
					else		
					{			
						if( n->targetid != -1 )
						{		
							if( InDistance( n, tempch,  TILE_SIZE * 10 ) )	// �� PC��  10 Ÿ�Ͼȿ� �ִٸ�.
							{	
								n->patterntype = NPC_PATTERN_WANDER_38_;
								if( NPC_NearBackCh( n, tempch, &ex, &ey, 10 ) )
								{	
									n->MoveP = 60 + Random(30 );
									NPC_MakePath( n, ex, ey, Random(10)+5 );
								}	
								else 
								{
								
								}
							}
							else	// ����� �ָ��ֱ�.
							{
								n->patterntype = NPC_PATTERN_WANDER_38_;

							}
						}
						else
						{
							n->patterntype = NPC_PATTERN_WANDER_38_;
						}
					}
				}
			}
				break;			
			}
	}
}


void Check_NPC_Pattern( LPCHARACTER  n )
{
	if( n->type != SPRITETYPE_MONSTER ) return;

	if( IsDead( n ) ) return;

	switch( n->patterntype )
	{
//------------------------------------------------------------------------------------------------------------
	case NPC_PATTERN_WANDER_8_			:
	case NPC_PATTERN_IAM_BOSS_8_		:
	case NPC_PATTERN_TOBOSS_8_			:
	case NPC_PATTERN_MURI_8_			:
	case NPC_PATTERN_ACCESS_PC_8_		:
	case NPC_PATTERN_ATTACK_PC_8_		:
	case NPC_PATTERN_BACKDRAW_8_		:	NPC_Pattern_8(n);
		break;

	case NPC_PATTERN_IAM_BOSS_28_		:
	case NPC_PATTERN_TOBOSS_28_			:
	case NPC_PATTERN_WANDER_28_			:
	case NPC_PATTERN_MURI_28_			:
	case NPC_PATTERN_BACKDRAW_PC_28_	:	NPC_Pattern_28(n);
		break;

	case  NPC_PATTERN_ACCESS_PC_38_		:
	case  NPC_PATTERN_ATTACK_PC_38_		:
	case  NPC_PATTERN_WANDER_38_		:
	case  NPC_PATTERN_MURI_38_			:
	case  NPC_PATTERN_BACKDRAW_PC_38_	:	NPC_Pattern_38(n);
		break;

	case  NPC_PATTERN_IAM_BOSS_GUARD_	:
	case  NPC_PATTERN_TOBOSS_GUARD_		:
	case  NPC_PATTERN_WANDER_GUARD_		:
	case  NPC_PATTERN_MURI_GUARD_		:
	case  NPC_PATTERN_ACCESS_PC_GUARD_	:
	case  NPC_PATTERN_ATTACK_PC_GUARD_	:
	case  NPC_PATTERN_BACKDRAW_PC_GUARD_:	NPC_Pattern_GUARD(n);
		break;

	case NPC_PATTERN_TAME_				:	NPC_Pattern_Tammed(n);
		break;

	case NPC_PATTERN_EVENTING_MAUL_		:
	case NPC_PATTERN_WANDER_MAUL_		:	NPC_Pattern_MAUL(n);
		break;

	case NPC_PATTERN_SEALSTONE_NO_OPERATE_ : 
	case NPC_PATTERN_SEALSTONE_ATTACK_ 	:	NPC_Pattern_SealStone( n );
		break;
	}
}


void RecvNK( int id, int nk3, int nk4, int nk6 )
{
	LPCHARACTER ch = ReturnCharacterPoint( id );
	if( ch == NULL ) return;
	ch->nk3 = nk3;
	ch->nk4 = nk4;
	ch->nk6 = nk6;
	ch->dummy_pk = 0;
}


void RecvServerKillPc(  int id, int killpc )
{	
	LPCHARACTER ch = ReturnCharacterPoint( id );
	if( ch == NULL ) return;
	ch->killpc = killpc;
}				
	
/**
 * @brief 檢查 NPC 對玩家的 PK 行為並觸發相關對話。
 *
 * 這是一個重構後的版本，使用現代 C++17 的特性來簡化邏輯和提高可讀性。
 * 它將重複的 if-else if 區塊替換為資料驅動的設計（Data-driven design），
 * 並使用具名常數來取代魔術數字，使程式碼更易於理解與維護。
 *
 * @param aid 攻擊者 (Attacker) 的 ID。
 * @param did 防禦者 (Defender) 的 ID，此處為 NPC。
 */
void CheckNPCPk(int aid, int did)
{
	// 取得 NPC（防禦者）的指標，如果不存在則直接返回
	LPCHARACTER dch = ReturnCharacterPoint(did);
	if (dch == NULL) return;

	// 原始碼中只有當 NPC 種族是 GUARD 時才執行後續邏輯
	if (dch->nRace != GUARD) return;

	// 取得玩家（攻擊者）的指標
	LPCHARACTER ach = ReturnCharacterPoint(aid);

	// 檢查玩家指標是否有效，並排除 ID 大於等於 10000 的 NPC
	if (ach == NULL || ach->id >= 10000) return;

	// 只有當玩家滿足特定 PK 條件且與 NPC 屬於同一國家時，才執行此區塊
	// IsNK 函式可能代表 "是否是 NPK" 或 "Nation Killer"，< 5 可能是某個PK值門檻
	// ach->name_status.nation == MapInfo[MapNumber].nation 檢查是否同國籍
	if (IsNK(ach, MapInfo[MapNumber].nation) < 5 && ach->name_status.nation == MapInfo[MapNumber].nation)
	{
		// 增加玩家的 dummy_pk 值，這可能是用來追蹤玩家的惡意行為或觸發對話的計數器
		++ach->dummy_pk;
		//char* temp = nullptr;

		// 使用 std::vector 儲存不同 NPC 類型對應的訊息編號。
		// 這取代了原始碼中重複的三個 switch 區塊，讓程式碼更簡潔。
		std::vector<int> msg_ids;
		int dummyPkAdjust = 0;
		int Num = -1;
		// 根據 NPC 的 ID 尾數（dch->id % 10）來選擇對應的訊息集。
		// 這表明不同 ID 尾數的 NPC 有不同的對話內容。
		switch (dch->id % 10)
		{
		case 0:
			msg_ids = { 41, 42, 43, 44, 45, 46, 47, 48, 49 };
			break;
		case 2:
			msg_ids = { 51, 52, 53, 54, 55, 56, 57, 58, 59 };
			break;
		case 3:
			// ID 為 3 的 NPC 訊息編號不連續，因此需要手動列出。
			msg_ids = { 61, 62, 63, 64, 65, 66, 67 };
			break;
		default:
			// 如果沒有對應的 NPC 類型，則直接返回
			return;
		}

		// 只呼叫一次 rand() 並儲存結果，以確保所有判斷都基於同一個隨機數。
		int randVal = rand() % 3;

		// 根據玩家的 dummy_pk 值來決定要使用的訊息編號。
		// 這裡的邏輯是根據 dummy_pk 相比 randVal 的偏移量來選擇對應的訊息。

		// 處理特殊情況：當 dummy_pk 達到較高門檻時，觸發特殊對話並調整 dummy_pk。
		// 這兩個判斷 (>= randVal + 8 和 == randVal + 8) 必須放在前面，
		// 以確保它們優先於其他較小的判斷條件。
		if (ach->dummy_pk >= randVal + 8)
		{
			// 處理 case 3 和其他 case 不同的訊息編號
			if (dch->id % 10 == 3) {
				//temp = lan->OutputMessage(4, msg_ids[6]); // 對應原始碼中的 67
				Num = msg_ids[6];
			}
			else {
				//temp = lan->OutputMessage(4, msg_ids[8]); // 對應原始碼中的 49, 59
				Num = msg_ids[8];
			}
			dummyPkAdjust = 10;
		}
		else if (ach->dummy_pk == randVal + 8)
		{
			// 處理 case 3 和其他 case 不同的訊息編號
			if (dch->id % 10 == 3) {
				//temp = lan->OutputMessage(4, msg_ids[5]); // 對應原始碼中的 66
				Num = msg_ids[5];
			}
			else {
				//temp = lan->OutputMessage(4, msg_ids[7]); // 對應原始碼中的 48, 58
				Num = msg_ids[7];
			}
			dummyPkAdjust = 10;
		}
		else
		{
			// 處理其餘連續的判斷。將 if-else if 鏈轉換為索引偏移量。
			int offset;
			if (ach->dummy_pk == randVal) offset = 0;
			else if (ach->dummy_pk == randVal + 1) offset = 1;
			else if (ach->dummy_pk == randVal + 2) offset = 2;
			else if (ach->dummy_pk == randVal + 3) offset = 3;
			else if (ach->dummy_pk == randVal + 4) offset = 4;
			else if (ach->dummy_pk == randVal + 5) offset = 5;
			else if (ach->dummy_pk == randVal + 6) offset = 6;
			else offset = -1; // 不符合任何條件

			// 確保計算出的索引在 vector 的範圍內，避免越界錯誤
			if (offset != -1 && offset < msg_ids.size()) {
				//temp = lan->OutputMessage(4, msg_ids[offset]);
				Num = msg_ids[offset];
			}
		}

		// 如果有需要調整 dummy_pk 的情況（例如觸發了特殊對話），則進行調整。
		if (dummyPkAdjust > 0) {
			ach->dummy_pk += dummyPkAdjust;
		}

		// 有 1/25 的機率觸發 NPC 在區域範圍內說話
		if (Num != -1 && (rand() % 25) == 0)
		{
			SendNPCChatArea(dch->id, Num);
		}
	}
	// 玩家與 NPC 屬於不同國籍時的邏輯
	else
	{
		ach->dummy_pk += 2;
		// 有 50% 的機率觸發不同國籍的對話
		if ((rand() % 100) < 50)
		{
			int nation = MapInfo[MapNumber].nation;
			int randCase = rand() % 5;
			if (nation == N_VYSEUS && randCase < 2)
			{
				SendNPCChatArea(dch->id, 71 + randCase);
			}
			else if (nation == N_ZYPERN && randCase < 2)
			{
				SendNPCChatArea(dch->id, 73 + randCase);
			}
		}
	}
}
/*
//  ��� NPC�� PK�Ҷ� ĳ������ dummy_pk�� �����Ѵ�. 
void CheckNPCPk( int aid, int did )
{				
	LPCHARACTER dch = ReturnCharacterPoint( did );	
	if( dch == NULL ) return;

	switch (dch->nRace)
	{	//< CSD-TW-030606
	case GUARD:
		{
			LPCHARACTER ach = ReturnCharacterPoint(aid);
			if (ach == NULL) return; // finito null check
			
			if (ach && ach->id < 10000)
			{
				if (IsNK( ach, MapInfo[MapNumber].nation ) < 5)
				{
					if (ach->name_status.nation == MapInfo[MapNumber].nation)
					{
						++ach->dummy_pk;
						char* temp = NULL;
						
						switch (dch->id%10)
						{
							case 0:
							{
								if( ach->dummy_pk	   == rand()%3  )		{  temp =  lan->OutputMessage(4,41); }//lsw
								else if( ach->dummy_pk == rand()%3 +1 )		{  temp =  lan->OutputMessage(4,42); }
								else if( ach->dummy_pk == rand()%3 + 2 )	{  temp =  lan->OutputMessage(4,43); }
								else if( ach->dummy_pk == rand()%3 + 3  )	{  temp =  lan->OutputMessage(4,44); }
								else if( ach->dummy_pk == rand()%3 + 4  )	{  temp =  lan->OutputMessage(4,45); }
								else if( ach->dummy_pk == rand()%3 + 5  )	{  temp =  lan->OutputMessage(4,46) ; }
								else if( ach->dummy_pk == rand()%3 + 6  )	{  temp =  lan->OutputMessage(4,47); ach->dummy_pk += 10; }
								else if( ach->dummy_pk == rand()%3 + 8  )	{  temp =  lan->OutputMessage(4,48);  ach->dummy_pk += 10; }
								else if( ach->dummy_pk >= rand()%3 + 8  )	{  temp =  lan->OutputMessage(4,49);  ach->dummy_pk += 10; }//lsw
								break;
							}
							case 2:
							{
								if( ach->dummy_pk	   == rand()%3  )		{  temp =  lan->OutputMessage(4,51); }//lsw
								else if( ach->dummy_pk == rand()%3 +1 )		{  temp =  lan->OutputMessage(4,52); }
								else if( ach->dummy_pk == rand()%3 + 2 )	{  temp =  lan->OutputMessage(4,53); }
								else if( ach->dummy_pk == rand()%3 + 3  )	{  temp =  lan->OutputMessage(4,54); }
								else if( ach->dummy_pk == rand()%3 + 4  )	{  temp =  lan->OutputMessage(4,55); }
								else if( ach->dummy_pk == rand()%3 + 5  )	{  temp =  lan->OutputMessage(4,56); }
								else if( ach->dummy_pk == rand()%3 + 6  )	{  temp =  lan->OutputMessage(4,57); ach->dummy_pk += 10; }
								else if( ach->dummy_pk == rand()%3 + 8  )	{  temp =  lan->OutputMessage(4,58) ;   ach->dummy_pk += 10; }
								else if( ach->dummy_pk >= rand()%3 + 8  )	{  temp =  lan->OutputMessage(4,59);   ach->dummy_pk += 10; }
								break;
							}
							case 3:
							{
								if( ach->dummy_pk	   == rand()%3  )		{  temp =  lan->OutputMessage(4,61); }//lsw
								else if( ach->dummy_pk == rand()%3 +1 )		{  temp =  lan->OutputMessage(4,62); }
								else if( ach->dummy_pk == rand()%3 + 2 )	{  temp =  lan->OutputMessage(4,63); }
								else if( ach->dummy_pk == rand()%3 + 3  )	{  temp =  lan->OutputMessage(4,64); }
								else if( ach->dummy_pk == rand()%3 + 4  )	{  temp =  lan->OutputMessage(4,65); }
								else if( ach->dummy_pk == rand()%3 + 8  )	{  temp =  lan->OutputMessage(4,66);  ach->dummy_pk += 10; }
								else if( ach->dummy_pk >= rand()%3 + 8  )	{  temp =  lan->OutputMessage(4,67);   ach->dummy_pk += 10; }//lsw
								break;
							}
						}
						// 0810 KHS
						if (temp && (rand()%25) == 0)
						{	
							SendNPCChatArea(dch->id, temp);
						}	
					}
					else // ���� �ٸ� �����ϰ��.. ����..
					{
						ach->dummy_pk += 2;

						if( (rand()%100) < 50 )
						{
							switch (MapInfo[MapNumber].nation)
							{
							case N_VYSEUS:
								{
									switch (rand()%5)
									{
										case 0:	SendNPCChatArea(dch->id, lan->OutputMessage(4,71) );	break;//lsw
										case 1:	SendNPCChatArea(dch->id, lan->OutputMessage(4,72) );	break;
									}

									break;
								}
							case N_ZYPERN:
								{
									switch (rand()%5)
									{
										case 0:	SendNPCChatArea(dch->id, lan->OutputMessage(4,73));	break;
										case 1:	SendNPCChatArea(dch->id, lan->OutputMessage(4,74));	break;//lsw
									}

									break;
								}
							}
						}
					}
				}
			}

			break;
		}
	}	//> CSD-TW-030606
}
*/



// 0811 NPC KHS

void SaveNPCAcessTable( void )
{
	FILE *fp = Fopen("./char/011105.spr","wb" );
	if( fp )
	{
		fwrite( NPCAccessTable, sizeof( short ) * 400 * 4, 1, fp );
		fclose(fp);
	}

	FILE *wp = Fopen( "./char/011105.spr", "wt" );
	if( wp )
	{
		for( int i = 0 ; i < 400 ;  i ++)
		{	//< CSD-030419
			fprintf(fp, "%3d(%20s): %3d %3d %3d %3d \n", 
				    i, 
					GetNpcName(i),
				    NPCAccessTable[i][0], 
					NPCAccessTable[i][1], 
					NPCAccessTable[i][2], 
					NPCAccessTable[i][3]);
		}	//> CSD-030419

		fclose(wp );
	}
}

void LoadNPCAccessTable( void )
{
    // [Android] 1. 路徑處理：移除開頭的 ./
    std::string path = "./char/011105.spr";
    if (path.find("./") == 0) path = path.substr(2);

    // [Android] 2. 開啟檔案
    SDL_RWops *fp = SDL_RWFromFile(path.c_str(), "rb" );

    if( fp )
    {
        // [Android] 3. 讀取數據
        // 讀取總大小 = sizeof(short) * 1600 (3200 bytes)
        SDL_RWread(fp, NPCAccessTable, sizeof( short ) * 400 * 4, 1);

        // [Android] 4. 關閉檔案
        SDL_RWclose(fp);
    }
    else
    {
        // (選用) 加入錯誤 Log
        SDL_Log("LoadNPCAccessTable Error: Could not open %s", path.c_str());
    }
}

void LoadItemAccessTable(void)
{
    // [Android] 1. 開啟檔案 (直接讀取 Assets 根目錄)
    SDL_RWops* fp = SDL_RWFromFile("011105.spr", "rb");

    if (fp)
    {
        // [Android] 2. 安全檢查 (選用)
        // 預期大小 = 2 bytes * 400 * 4 = 3200 bytes
        Sint64 fileSize = SDL_RWsize(fp);
        size_t expectedSize = sizeof(short) * 400 * 4;

        if (fileSize != expectedSize) {
            SDL_Log("LoadItemAccessTable Warning: File size mismatch! (File: %lld, Expected: %zu)", fileSize, expectedSize);
            // 視情況決定是否要 return，或者嘗試讀取部分
        }

        // [Android] 3. 讀取數據
        // 參數: (context, ptr, object_size, object_count)
        // 這裡我們讀取 1600 個 short
        SDL_RWread(fp, NPCAccessTable, sizeof(short), 400 * 4);

        // [Android] 4. 關閉檔案
        SDL_RWclose(fp);
    }
    else
    {
        SDL_Log("LoadItemAccessTable Error: '011105.spr' not found.");
    }
}


void Recv_CMD_SERVER_NPC_ACCESS_TABLE_RELOAD( t_packet *p )
{	
	memcpy( &NPCAccessTable[  0][0] ,p->u.server_npc_access_table_reload.data, sizeof( short )* 400 );	
}
void Recv_CMD_SERVER_NPC_ACCESS_TABLE_RELOAD2( t_packet *p )
{	
	memcpy( &NPCAccessTable[100][0] ,p->u.server_npc_access_table_reload.data, sizeof( short )* 400 );	
}
void Recv_CMD_SERVER_NPC_ACCESS_TABLE_RELOAD3( t_packet *p )
{	
	memcpy( &NPCAccessTable[200][0] ,p->u.server_npc_access_table_reload.data, sizeof( short )* 400 );
}
void Recv_CMD_SERVER_NPC_ACCESS_TABLE_RELOAD4( t_packet *p )
{	
	memcpy( &NPCAccessTable[300][0] ,p->u.server_npc_access_table_reload.data, sizeof( short )* 400 );
	SaveNPCAcessTable();
}




std::vector<char> g_MonNameBuffer;
static char monname_temp[ 17]{};
// 初始化時呼叫一次
void InitMonNameTable() {
    SDL_RWops *fp = SDL_RWFromFile("data/mon_name.bin", "rb");
    if (fp) {
        Sint64 size = SDL_RWsize(fp);
        g_MonNameBuffer.resize(size);
        SDL_RWread(fp, g_MonNameBuffer.data(), size, 1);
        SDL_RWclose(fp);
    }
}

// 讀取時直接從記憶體拿 (超快，零 IO)
char *LoadMonNameTable( int namenumber )
{
    if(g_MonNameBuffer.empty()) InitMonNameTable();
    size_t offset = namenumber * 17;
    if (offset + 17 <= g_MonNameBuffer.size()) {
        // 直接回傳記憶體中的位址
        // 注意：這裡假設 bin 檔內的 17 bytes 已經包含 \0，或者您只讀取前 16 個字
        // 為了安全，最好拷貝到 monname_temp 並補 \0
        memcpy(monname_temp, &g_MonNameBuffer[offset], 17);
        monname_temp[17] = '\0';
        return monname_temp;
    }
    return (char*)"";
}


















