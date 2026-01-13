// HunterSystem.cpp: implementation of the CHunterSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Chat.h"
#include "Hangul.h"
#include "menu.h"
#include "dragon.h"
#include "Hong_Sub.h"
#include "Map.h"
#include "CharDataTable.h"
#include "TeamBattle.h"
#include "GameProc.h"
#include "LineCommand.h"
#include "Tool.h"
#include "Hong_Sprite.h"
#include "directsound.h"
#include "SmallMenu.h"
#include "TextOutMgr.h"
#include "MailMgr.h"
#include "HunterSystem.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// ����� �ý���

CHunterList *my_hunter_list = nullptr;
CHunterList	*hunter_list= nullptr;
CHunterList *curr_hunter_list= nullptr;

CHunterSystem::CHunterSystem()
{

}

CHunterSystem::~CHunterSystem()
{

}



CHunterList* CHunterList::AddNode( k_hunter_list *value )
{
	EatRearWhiteChar( value->target_name );
	EatRearWhiteChar( value->regist_name );
	CHunterList *temp = this;

	if( value->index == 0 ) 
	{
		while( temp->next ) temp = temp->next;
		temp->is_end = 1;
		return temp;
	}
	else
	{
		while( temp->next )
		{
			if( value->index == temp->list.index ) return NULL;
			temp = temp->next;
		}
	}

	if( !list.index )		// ��ó��
	{
		LPCHARACTER ch = ExistHe( value->target_name );		// �� �þ߿� �ִ� ���ΰ�...
		if( ch ) 
		{
			my_sight = 1;
			ch->hunter_target = true;
		}
		list = *value;
		return this;
	}

	CHunterList *data = new CHunterList;

	LPCHARACTER ch = ExistHe( value->target_name );		// �� �þ߿� �ִ� ���ΰ�...
	if( ch ) 
	{
		data->my_sight = 1;
		ch->hunter_target = true;
	}
	data->list = *value;
	temp->next = data;
	data->prev = temp;
	return data;
}

int CHunterList::print( int x, int y )
{
	if( !list.index ) return 0;
	PutCharImage( x+21, y+58, list.face, 0, FS_SMALL, 0, list.gender );
	Hcolor( FONT_COLOR_NAME );
	Hprint2( x+110, y+64, g_DestBackBuf, "%s", list.target_name );
	Hprint2( x+110, y+154, g_DestBackBuf, "%s", list.regist_name );
	
	Hcolor( FONT_COLOR_NUMBER );
	Hprint2( x+110, y+109, g_DestBackBuf, "%d", list.money );

	RectTextPut( x+25, y+199, 174, list.text );		// ��ũ�� ��������...
#ifdef _DEBUG // finito 060507
	if( GetSysInfo( SI_GAME_MAKE_MODE ) )
	{
		Hprint2( x+176, y+17, g_DestBackBuf, "%d",list.index );
		Hprint2( x+22, y+17, g_DestBackBuf, is_end?"END":"CONTINUE" );
	}
#endif
	if( is_end ) return -1;
	return 1;
}

void CHunterList::DeleteAll()
{
	SAFE_DELETE(next);
}

int CHunterList::DeleteNode( char *name )
{
	CHunterList *temp = this;
	while( temp )
	{
		if( ::stricmp( temp->list.target_name, name ) == 0 )
		{
			if( temp->prev ) temp->prev->next = temp->next;
			if( temp->next ) temp->next->prev = temp->prev;
			temp->next = NULL;	// �Ҹ��� ������...
			SAFE_DELETE(temp);
			// � ������� �޴� ���� ���Ѱ� ���⼭ ó���ص� �ȴ�.
			return 1;
		}
		
		temp = temp->next;
	}
	return 0;
}

int CHunterList::CheckList( char *name, int type )		// �� �þ߿� 1:��� ������, 0:���� ������ 
{
	int ret = 0;
	CHunterList *temp = this;
	while( temp )
	{
		if( ::stricmp( temp->list.target_name, name ) == 0 )//021230 lsw Name Error
		{
			if( type ) temp->my_sight = 80;
			else temp->my_sight = 0;
			ret = 1;
		}
		temp = temp->next;
	}
	return ret;
}

int CHunterList::print2( int x, int y )
{
	//���� ���� �����ֱ⿡ ���� �ִ�.
	if( IsRightWindowOpen() ) return 0;		// ������ â�� ������, 
	
	int ret = 0;
	Hcolor( FONT_COLOR_NAME );
	CHunterList *temp = this;
	int count = 1;
	
	while( temp )
	{
		if( !temp->list.index ) 
		{
			temp = temp->next;
			continue;
		}
		ret = 1;
		if( temp->my_sight > 1)
		{
			if( temp->my_sight % 2 )
				Hcolor( FONT_COLOR_PLUS );
			else Hcolor( FONT_COLOR_NAME );

			Hprint2( x, y+count*18, g_DestBackBuf, "%s", temp->list.target_name );
			Hcolor( FONT_COLOR_NAME );

			temp->my_sight--;
		}
		else if( temp->my_sight == 1 )
		{
			Hcolor( FONT_COLOR_PLUS );
			Hprint2( x, y+count*18, g_DestBackBuf, "%s", temp->list.target_name );
			Hcolor( FONT_COLOR_NAME );
		}		
		else Hprint2( x, y+count*18, g_DestBackBuf, "%s", temp->list.target_name );
		temp = temp->next;
		count++;
	}

	if( ret )
	{
		Hcolor( FONT_COLOR_DEFAULT );
		Hprint2( x, y, g_DestBackBuf, "-= LIST =-" );
	}

	return 1;
}

void CHunterList_Next( CHunterList *&target )
{
	if( target->next ) target = target->next;
}
void CHunterList_Prev( CHunterList *&target )
{
	if( target->prev ) target = target->prev;
}

int CHunterList::KillCharacter( char *name )
{
	CHunterList *temp = this;
	int ret = 0;
	while( temp )
	{
		if( ::stricmp( temp->list.target_name, name )==0 )
		{
			// SendKillHunterList( temp->list.index ); finito 15/08/07 this shouldn't be in the client!
			ret = 1;
		}
		temp = temp->next;
	}
	return ret;
}

int CHunterList::CompName( char *name )		// ����� ����� ����..
{
	return ::stricmp( list.regist_name, name );
}

int CHunterList::CompTargetName( char *name )		// ��ǥ�� ����..
{
	return ::stricmp( list.target_name, name );
}

int CHunterList::CompTargetId( short int id )
{
	CHunterList *temp = this;
	int ret = 0;
	while( temp )
	{
		if( temp->list.index == id ) return 1;
		temp = temp->next;
	}
	return 0;
}

CHunterList *DeleteMyHunterList( CHunterList *header, char *name )	// �̸����� ����� ��� �����ϱ�
{
	CHunterList *temp = header;
	while( temp )
	{
		if( ::stricmp( temp->list.target_name, name )==0 )
		{
			if( temp == header ) header = header->next;
			CHunterList *buf = temp->next;
			if( temp->prev ) temp->prev->next = temp->next;
			if( temp->next ) temp->next->prev = temp->prev;
			temp->next = NULL;
			SAFE_DELETE(temp);
			temp = buf;
		}
		else temp = temp->next;
	}
	
	if( !header ) header = new CHunterList;		// ���̻� ����� ��尡 ������
	return header;
}

bool CHunterList::ExistMyHunterList( char *name )	// �� ���� ��Ͽ� ���Ե� ������ �˾ƺ��� ��ƾ
{
    if(name[0] <= 0) return false;

	CHunterList *temp = this;
	while( temp )
	{
		if( !strcasecmp( temp->list.target_name, name ) ) return true;
		temp = temp->next;
	}
	return false;
}

void SendHunterRegist( char *name, DWORD money, char *text )
{
  std::string txt(text);
	ConvString(txt);
	t_packet p;

	p.h.header.type = CMD_HUNTER_REGIST;
		strcpy( p.u.kein.hunter_regist.name, name );
		p.u.kein.hunter_regist.money = money;
		strcpy( p.u.kein.hunter_regist.text, txt.c_str());

	p.h.header.size = sizeof( k_hunter_regist )-1000+txt.length();
	QueuePacket( &p, 1 );
}

void SendGetHunterList( int id )
{
	t_packet p;

	p.h.header.type = CMD_GET_HUNTER_LIST;
		p.u.kein.get_hunter_list.list_id = id;

	p.h.header.size = sizeof( k_get_hunter_list );
	QueuePacket( &p, 1 );
}

void RecvHunterList( k_hunter_list *hunterlist )		// ����� ����Ʈ ��������
{
	curr_hunter_list = curr_hunter_list->AddNode( hunterlist );
}

void SendKillHunterList( int id )		// ������� ������� ����Ʈ���̵� ������.
{
/*	t_packet p;

	p.h.header.type = CMD_KILL_HUNTER_LIST;
		p.u.kein.get_hunter_list.list_id = id;

	p.h.header.size = sizeof( k_get_hunter_list );
	QueuePacket( &p, 1 );
	
finito 15/08/07 this should not be in the client! */
}

//////////////////////////////////////////////////////////////
//	���� ������ �׿���.
void RecvKillWho( char *who )
{
	// ��������� üũ..
	// MP3( SN_KILL_PC );
    if(!my_hunter_list) return;

 	int ret = my_hunter_list->KillCharacter( who ); // üũ�ؼ� ������ ����� ���̵� ������ ������.

	LPCHARACTER target = ExistHe( who);		// �� ĳ������ �����͸� ������
	if( ret )		// ������̶��		// ���� ������� ���ΰ� �����ش�.	// ���� Ŭ���̾�Ʈ ����� ����
	{
		my_hunter_list = DeleteMyHunterList( my_hunter_list, who ); // ������� ��Ͽ��� �����Ѵ�.

		t_packet p;

		p.h.header.type = CMD_KILLED_BY_HUNTER;
			strcpy( p.u.kein.kill_who.name , who );
		p.h.header.size = sizeof( k_kill_who );
		QueuePacket( &p, 1 );
	}

	if( target )
	{
		target->hunter_target = false;// ����� ����� Ǯ���ش�.

		/*
		//////////////////// ���� ���� ��� //////////////////
		if( g_FightMapStart )
		{
			if( Hero->fight_map_team_num && target->fight_map_team_num )
			{
				AddCurrentStatusMessage( 0, 126, 200, lan->OutputMessage(6,28), FightTeam[target->fight_map_team_num-1], target->name );//010215 lsw
				MP3( SN_KILL_WHO );
			}
		}*/
	}
}

// �����մϴ�. �������� ������� ��µ� �����߽��ϴ�.
// ������� �� �� �Դϴ�.
void RecvKillHunterOk( k_kill_hunter_ok *result )
{
	MP3( SN_KILL_HUNTER_OK );
	AddCurrentStatusMessage( 200, 170, 75, lan->OutputMessage(6,29) );//010215 lsw
	AddCurrentStatusMessage( 200, 170, 75, lan->OutputMessage(6,30), result->money );//010215 lsw
//	AddMoney( result->money );
}

//#define CMD_HUNTER_REGIST_RESULT			10191	// ���������� ��Ų��� ����� �Խ��ǿ� ����� ���������� ���̴µ� �����߽��ϴ�.
void RecvHunterRegistResult( char *hunter, char *target )
{
	MP3( SN_RECV_MAIL );
	AddCurrentStatusMessage( 200, 170, 75, lan->OutputMessage(6,31), hunter, target );//010215 lsw
}

void SendDangerOfHunter( short int target_id, char type )
{
	t_packet p;

	p.h.header.type = CMD_DANGER_OF_HUNTER;
		p.u.kein.danger_of_hunter.target_id = target_id;
		p.u.kein.danger_of_hunter.type = type;
	p.h.header.size = sizeof( k_danger_of_hunter );
	QueuePacket( &p, 1 );
}

	
void RecvDangerByHunter( char type )
{
	if( type )
	{
		danger_by_hunter = 60;
	}
	else 
	{
		danger_by_hunter = 0;
	}
}

