#pragma once
#define		CCD_ARIGEMENT							1			//��ġ�� ���� �䱸
#define		CCD_PLAYSOUND_EVENT1					851			//���� �̺�Ʈ�� ����
#define		CCD_PLAYSOUND_EVENT2					853
////////////////////////////// 0803 yjs �߰� ///////////////////////////////////
#define		CCD_FIGHT_LOSE							10000		//�����
#define		CCD_FIGHT_DRAW							10001		//�����
#define		CCD_FIGHT_LOGOUT						10002		//��밡 �α׾ƿ��� ���

///////////////////// 0810 lhs �߰� ////////////////////////
#define		CCD_START_QUEST							15000			//����Ʈ ���� ȿ����
#define		CCD_END_QUEST							15001
#define		CCD_QUEST_SALUTE						15002			//����Ʈ ���� ȿ��
#define		CCD_QUEST_GETSKILL						15003
#define		CCD_QUEST_GETITEM						15004
#define		CCD_QUEST_MONSTER						15005
#define		CCD_START_QUEST_BBS						15006
#define		CCD_END_QUEST_BBS						15007
#define		CCD_COMPLETE_QUEST						15008

#define		CMD_SERVER_MAGIC_POSITION				20000
#define		CMD_SERVER_CHARACTER_CONDITION			20001
#define		CMD_CLIENT_CHARACTER_DEMAND				20002
#define		CMD_SERVER_ARIGEMENT_RESPONSE			20003
#define		CMD_SERVER_MAGICCOMMAND					20004
#define		CMD_SERVER_MAGICTRAP_EXPOSE				20005
#define		CMD_SERVER_MAGICAL_DETECT				20006
#define		CMD_SERVER_MAGICVIEWTYPE				20007
////////////////////// 0802 yjs �߰� //////////////////////
#define		CMD_SERVER_FIGHT_STONE					20010		//1:1 ����..

////////////////////// 0816 lkh �߰� //////////////////////
#define		CMD_HEXAEDITING							20011		//��翡������ �õ��� ��� ������ �˸���.

#define		CMD_SERVER_CHARACTER_CONDITION2			20012

///////////////// 0810 lhs ����Ʈ�� ������  ///////////////////////////////////
#define		CMD_BBS_QUEST							25000
#define		CMD_QSF_COMMAND							25001
#define		QSF_COMMAND_TELEPORT					1
/////////////////////////////////////////////////////////////////
#include "StdAfx.h"

///////////////////// 0802 yjs �߰� ////////////////////
typedef struct fight_stone
{
	BOOL		fight_flag;	//���� �÷���..
	int			fight_id;
	short int	x;			//x��ǥ.. 
	short int	y;			//y��ǥ.. 
}	t_fight_stone;


typedef struct  client_character_demand
{
	short int	command_type;
	short int	target_id;
}   t_client_character_demand;

typedef struct	server_arigement_response
{
	short int	target_id;
	short int	arigement;
}	t_server_arigement_response;

struct t_server_magic_position
{
  BYTE nType; // ����� ����Ʈ ��ȣ
  WORD idTarget; // ������ ����� �����
  __int16 nX; // �̵��� ��ġ�� X��ǥ
  __int16 nY; // �̵��� ��ġ�� Y��ǥ
};

struct t_server_character_condition
{
  WORD idTarget;
  __int32 nLife;
  BYTE nState;
  __int32 nHP;
  __int32 nMaxHp;
};

typedef struct  server_magic_command
{
	//short int	target_id;		//��� ID
	short int	command;				//��� Ŭ���̾�Ʈ�� ���� ȿ���� ��ġ�� ������ ���(slot1)
	short int	sub_command;		//                   "									���� ���(slot2)
	short int	sub_term;				//                   "   ȿ���� ��ġ�� �Ⱓ				(slot3)
}	t_server_magic_command;

typedef struct  server_magictrap_expose
{
	short int	target_id;
	short int	x;
	short int	y;
	short int	trap_Type;
}	t_server_magictrap_expose;

typedef struct	server_magical_detect
{
	short int	tile_X;
	short int	tile_Y;
	short int	type;
}	t_server_magical_detect;

typedef struct	server_magicviewtype
{
	short int	target_id;
	short int	view_Type;
	short int	second_Time;
}	t_server_magicviewtype;

//////////// 0810 lhs //////////////////
typedef struct	qsf_command				//����Ʈ���� �������� ����ϱ� ���� ����ü
{
	short int	command;
	short int	target_id;
	short int	x;
	short int	y;
}	t_qsf_command;


/////////// 0816 lkh �߰� ///////////
typedef struct client_editing_table
{
	uint32_t	time_get_time;
}	t_client_editing_table;

typedef union	angra_pub
{
	t_server_character_condition		server_character_condition;
	t_server_magic_position				server_magic_position;
	t_client_character_demand			client_character_demand;
	t_server_arigement_response			server_arigement_response;
	t_server_magic_command				server_magic_command;
	t_server_magictrap_expose			server_magictrap_expose;
	t_server_magical_detect				server_magical_detect;
	t_server_magicviewtype				server_magicviewtype;
	////////////////////// 0802 yjs �߰� /////////////////////////////
	t_fight_stone						server_fight_stone;	//
	///////////////////// ����Ʈ �� �ĸǵ�... 0810 lhs //////////////////
	t_qsf_command						qsf_command;

	////////////////////// Hexa Edit �õ��� ��� ������ �˸��� ���� ////////////////////
	t_client_editing_table				client_editing_table;

}	t_angra;




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	                                      KBS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define CMD_RM_CHANGE_WEATHER		26000


// 011212 KBS
typedef struct rm_change_weather
{
	BYTE	bWeather;		//0�̸� ����, 1�̸� ��, 2�̸� �� 
	BYTE	bStopWeather;
	DWORD	dwAmount;

}t_rm_change_weather;
//


extern void RecvChangeWeather( t_rm_change_weather* p );