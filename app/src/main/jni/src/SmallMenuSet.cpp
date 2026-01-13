#include "stdafx.h"
//#include "MenuDef.h"
#include "kh_cpp.h"
#include "menu.h"
#include "teambattle.h"
#include "menudef2.h"
#include "menufunctions.h"


extern SMENU SMenu[MAX_MENU];
extern SHN SHideNomal[MAX_SHN][MAX_SHN_FIELD];
extern Spr		god_spr[GOD_MENU_IMG_MAX];		// ���� �޴� 
extern void SetRect(RECT &rect,int left, int top, int right, int bottom);    
extern WORD RGB16( int r, int g, int b );
extern void CallGuildMemberListMenu();

void SmallMenuSet()
{
	int x, y;
/////////////////////////////////////////////////////////////////////////////////////
//////////////////FT_HIDE_NOMAL_�� ����� ����ü ������ ����/////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
//TEMPLE���� INTRODUTION�� ����� ���� ���
	SHideNomal[HN_TEMPLE_INTRODUTION_TEXT][0].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_TEMPLE_INTRODUTION_TEXT][0].y=113;
	sprintf(SHideNomal[HN_TEMPLE_INTRODUTION_TEXT][0].temp,"RULE");
	SHideNomal[HN_TEMPLE_INTRODUTION_TEXT][1].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_TEMPLE_INTRODUTION_TEXT][1].y=154;
	sprintf(SHideNomal[HN_TEMPLE_INTRODUTION_TEXT][1].temp,"MEMBER");
	SHideNomal[HN_TEMPLE_INTRODUTION_TEXT][2].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_TEMPLE_INTRODUTION_TEXT][2].y=195;
	sprintf(SHideNomal[HN_TEMPLE_INTRODUTION_TEXT][2].temp,"TAME");
	SHideNomal[HN_TEMPLE_INTRODUTION_TEXT][3].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_TEMPLE_INTRODUTION_TEXT][3].y=236;
	sprintf(SHideNomal[HN_TEMPLE_INTRODUTION_TEXT][3].temp,"RANKING");

/////////////////////////////////////////////////////////////////////////////////////
	//TEMPLE���� OPERATION�� ����� ���� ���
	SHideNomal[HN_TEMPLE_OPERATION_TEXT][0].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_TEMPLE_OPERATION_TEXT][0].y=109;
	sprintf(SHideNomal[HN_TEMPLE_OPERATION_TEXT][0].temp,"RULE");
	SHideNomal[HN_TEMPLE_OPERATION_TEXT][1].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_TEMPLE_OPERATION_TEXT][1].y=140;
	sprintf(SHideNomal[HN_TEMPLE_OPERATION_TEXT][1].temp,"NAME");
	SHideNomal[HN_TEMPLE_OPERATION_TEXT][2].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_TEMPLE_OPERATION_TEXT][2].y=171;
	sprintf(SHideNomal[HN_TEMPLE_OPERATION_TEXT][2].temp,"INCLINATION");
	SHideNomal[HN_TEMPLE_OPERATION_TEXT][3].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_TEMPLE_OPERATION_TEXT][3].y=202;
	sprintf(SHideNomal[HN_TEMPLE_OPERATION_TEXT][3].temp,"CONSTRUCTION");
	SHideNomal[HN_TEMPLE_OPERATION_TEXT][4].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_TEMPLE_OPERATION_TEXT][4].y=233;
	sprintf(SHideNomal[HN_TEMPLE_OPERATION_TEXT][4].temp,"REQUEST");
	SHideNomal[HN_TEMPLE_OPERATION_TEXT][5].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_TEMPLE_OPERATION_TEXT][5].y=264;
	sprintf(SHideNomal[HN_TEMPLE_OPERATION_TEXT][5].temp,"WAR");

//////////////////////////////////////////////////////////////////////////////////////
	//TEMPLE���� TEMPLE1�� ����� ���� ���
	SHideNomal[HN_TEMPLE_TEMPLE1_TEXT][0].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_TEMPLE_TEMPLE1_TEXT][0].y=100;
	sprintf(SHideNomal[HN_TEMPLE_TEMPLE1_TEXT][0].temp,"BULLETIN BOARD");
	SHideNomal[HN_TEMPLE_TEMPLE1_TEXT][1].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_TEMPLE_TEMPLE1_TEXT][1].y=136;
	sprintf(SHideNomal[HN_TEMPLE_TEMPLE1_TEXT][1].temp,"LOYALTY");
	SHideNomal[HN_TEMPLE_TEMPLE1_TEXT][2].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_TEMPLE_TEMPLE1_TEXT][2].y=172;
	sprintf(SHideNomal[HN_TEMPLE_TEMPLE1_TEXT][2].temp,"ENTRANCE");
	SHideNomal[HN_TEMPLE_TEMPLE1_TEXT][3].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_TEMPLE_TEMPLE1_TEXT][3].y=208;
	sprintf(SHideNomal[HN_TEMPLE_TEMPLE1_TEXT][3].temp,"TOGGLE");
	SHideNomal[HN_TEMPLE_TEMPLE1_TEXT][4].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_TEMPLE_TEMPLE1_TEXT][4].y=244;
	sprintf(SHideNomal[HN_TEMPLE_TEMPLE1_TEXT][4].temp,"SECESSION");

//////////////////////////////////////////////////////////////////////////////////////////
	//TEMPLE���� TEMPLE2�� ����� ���� ���
	SHideNomal[HN_TEMPLE_TEMPLE2_TEXT][0].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_TEMPLE_TEMPLE2_TEXT][0].y=110;
	sprintf(SHideNomal[HN_TEMPLE_TEMPLE2_TEXT][0].temp,"MALE");
	SHideNomal[HN_TEMPLE_TEMPLE2_TEXT][1].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_TEMPLE_TEMPLE2_TEXT][1].y=180;
	sprintf(SHideNomal[HN_TEMPLE_TEMPLE2_TEXT][1].temp,"FEMALE");


/////////////////////////////////////////////////////////////////////////////////////
//GUILD���� INTRODUTION�� ����� ���� ���
	SHideNomal[HN_GUILD_INTRODUTION_TEXT][0].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_GUILD_INTRODUTION_TEXT][0].y=113;
	sprintf(SHideNomal[HN_GUILD_INTRODUTION_TEXT][0].temp,"RULE");
	SHideNomal[HN_GUILD_INTRODUTION_TEXT][1].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_GUILD_INTRODUTION_TEXT][1].y=154;
	sprintf(SHideNomal[HN_GUILD_INTRODUTION_TEXT][1].temp,"MEMBER");
	SHideNomal[HN_GUILD_INTRODUTION_TEXT][2].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_GUILD_INTRODUTION_TEXT][2].y=195;
	sprintf(SHideNomal[HN_GUILD_INTRODUTION_TEXT][2].temp,"TAME");
	SHideNomal[HN_GUILD_INTRODUTION_TEXT][3].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_GUILD_INTRODUTION_TEXT][3].y=236;
	sprintf(SHideNomal[HN_GUILD_INTRODUTION_TEXT][3].temp,"RANKING");

/////////////////////////////////////////////////////////////////////////////////////
	//GUILD���� OPERATION�� ����� ���� ���
	SHideNomal[HN_GUILD_OPERATION_TEXT][0].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_GUILD_OPERATION_TEXT][0].y=109;
	sprintf(SHideNomal[HN_GUILD_OPERATION_TEXT][0].temp,"RULE");
	SHideNomal[HN_GUILD_OPERATION_TEXT][1].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_GUILD_OPERATION_TEXT][1].y=140;
	sprintf(SHideNomal[HN_GUILD_OPERATION_TEXT][1].temp,"NAME");
	SHideNomal[HN_GUILD_OPERATION_TEXT][2].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_GUILD_OPERATION_TEXT][2].y=171;
	sprintf(SHideNomal[HN_GUILD_OPERATION_TEXT][2].temp,"INCLINATION");
	SHideNomal[HN_GUILD_OPERATION_TEXT][3].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_GUILD_OPERATION_TEXT][3].y=202;
	sprintf(SHideNomal[HN_GUILD_OPERATION_TEXT][3].temp,"CONSTRUCTION");
	SHideNomal[HN_GUILD_OPERATION_TEXT][4].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_GUILD_OPERATION_TEXT][4].y=233;
	sprintf(SHideNomal[HN_GUILD_OPERATION_TEXT][4].temp,"REQUEST");
	SHideNomal[HN_GUILD_OPERATION_TEXT][5].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_GUILD_OPERATION_TEXT][5].y=264;
	sprintf(SHideNomal[HN_GUILD_OPERATION_TEXT][5].temp,"WAR");

//////////////////////////////////////////////////////////////////////////////////////
	//GUILD���� GUILD1�� ����� ���� ���
	SHideNomal[HN_GUILD_GUILD1_TEXT][0].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_GUILD_GUILD1_TEXT][0].y=100;
	sprintf(SHideNomal[HN_GUILD_GUILD1_TEXT][0].temp,"BULLETIN BOARD");
	SHideNomal[HN_GUILD_GUILD1_TEXT][1].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_GUILD_GUILD1_TEXT][1].y=136;
	sprintf(SHideNomal[HN_GUILD_GUILD1_TEXT][1].temp,"LOYALTY");
	SHideNomal[HN_GUILD_GUILD1_TEXT][2].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_GUILD_GUILD1_TEXT][2].y=172;
	sprintf(SHideNomal[HN_GUILD_GUILD1_TEXT][2].temp,"ENTRANCE");
	SHideNomal[HN_GUILD_GUILD1_TEXT][3].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_GUILD_GUILD1_TEXT][3].y=208;
	sprintf(SHideNomal[HN_GUILD_GUILD1_TEXT][3].temp,"TOGGLE");
	SHideNomal[HN_GUILD_GUILD1_TEXT][4].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_GUILD_GUILD1_TEXT][4].y=244;
	sprintf(SHideNomal[HN_GUILD_GUILD1_TEXT][4].temp,"SECESSION");

//////////////////////////////////////////////////////////////////////////////////////////
	//GUILD���� GUILD2�� ����� ���� ���
	SHideNomal[HN_GUILD_GUILD2_TEXT][0].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_GUILD_GUILD2_TEXT][0].y=110;
	sprintf(SHideNomal[HN_GUILD_GUILD2_TEXT][0].temp,"MALE");
	SHideNomal[HN_GUILD_GUILD2_TEXT][1].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_GUILD_GUILD2_TEXT][1].y=180;
	sprintf(SHideNomal[HN_GUILD_GUILD2_TEXT][1].temp,"FEMALE");

/////////////////////////////////////////////////////////////////////////////////////////////
	//TOWNHALL���� ����� ���� ���
	int cn = 0;
	SHideNomal[HN_TOWNHALL_TEXT][0].nType=FT_TEXT_PUT_CENTER;
	cn=77+8; SHideNomal[HN_TOWNHALL_TEXT][0].y=cn;
	sprintf(SHideNomal[HN_TOWNHALL_TEXT][0].temp,"VOTE");
	SHideNomal[HN_TOWNHALL_TEXT][1].nType=FT_TEXT_PUT_CENTER;
	cn+=31; SHideNomal[HN_TOWNHALL_TEXT][1].y=cn;
	sprintf(SHideNomal[HN_TOWNHALL_TEXT][1].temp,"BUILDING");
	SHideNomal[HN_TOWNHALL_TEXT][2].nType=FT_TEXT_PUT_CENTER;
	cn+=31; SHideNomal[HN_TOWNHALL_TEXT][2].y=cn;
	sprintf(SHideNomal[HN_TOWNHALL_TEXT][2].temp,"GUILD");
	SHideNomal[HN_TOWNHALL_TEXT][3].nType=FT_TEXT_PUT_CENTER;
	cn+=31; SHideNomal[HN_TOWNHALL_TEXT][3].y=cn;
	sprintf(SHideNomal[HN_TOWNHALL_TEXT][3].temp,"FORUM");
	SHideNomal[HN_TOWNHALL_TEXT][4].nType=FT_TEXT_PUT_CENTER;
	cn+=31; SHideNomal[HN_TOWNHALL_TEXT][4].y=cn;
	sprintf(SHideNomal[HN_TOWNHALL_TEXT][4].temp,"EVENT");
	SHideNomal[HN_TOWNHALL_TEXT][5].nType=FT_TEXT_PUT_CENTER;
	cn+=31; SHideNomal[HN_TOWNHALL_TEXT][5].y=cn;
	sprintf(SHideNomal[HN_TOWNHALL_TEXT][5].temp,"RANK");

/////////////////////////////////////////////////////////////////////////////////////////////
	//TOWNHALL�� construction���� ����� ���� ���
	SHideNomal[HN_TOWNHALL_CONSTRUCTION_TEXT][0].nType=FT_TEXT_PUT;
	SHideNomal[HN_TOWNHALL_CONSTRUCTION_TEXT][0].x=85;
	SHideNomal[HN_TOWNHALL_CONSTRUCTION_TEXT][0].y=77;
	sprintf(SHideNomal[HN_TOWNHALL_CONSTRUCTION_TEXT][0].temp,"RESIDING");
	SHideNomal[HN_TOWNHALL_CONSTRUCTION_TEXT][1].nType=FT_TEXT_PUT;
	SHideNomal[HN_TOWNHALL_CONSTRUCTION_TEXT][1].x=95;
	SHideNomal[HN_TOWNHALL_CONSTRUCTION_TEXT][1].y=115;
	sprintf(SHideNomal[HN_TOWNHALL_CONSTRUCTION_TEXT][1].temp,"GUILD");
	SHideNomal[HN_TOWNHALL_CONSTRUCTION_TEXT][2].nType=FT_TEXT_PUT;
	SHideNomal[HN_TOWNHALL_CONSTRUCTION_TEXT][2].x=70;
	SHideNomal[HN_TOWNHALL_CONSTRUCTION_TEXT][2].y=153;
	sprintf(SHideNomal[HN_TOWNHALL_CONSTRUCTION_TEXT][2].temp,"NOT RESIDING");

////////////////////////////////////////////////////////////////////////////////////////////
// ����
	SHideNomal[HN_DRINK_MAIN_TEXT][0].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_DRINK_MAIN_TEXT][0].y=131;
	sprintf(SHideNomal[HN_DRINK_MAIN_TEXT][0].temp,"RUMOR");
	SHideNomal[HN_DRINK_MAIN_TEXT][1].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_DRINK_MAIN_TEXT][1].y=167;
	sprintf(SHideNomal[HN_DRINK_MAIN_TEXT][1].temp,"ITEM BUY");
	
////////////////////////////////////////////////////////////////////////////////////////////
// ��ȭ��
	SHideNomal[HN_SHOP_MAIN_TEXT][0].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_SHOP_MAIN_TEXT][0].y=150;
	sprintf(SHideNomal[HN_SHOP_MAIN_TEXT][0].temp,"ITEM BUY");
	SHideNomal[HN_SHOP_MAIN_TEXT][1].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_SHOP_MAIN_TEXT][1].y=182;
	sprintf(SHideNomal[HN_SHOP_MAIN_TEXT][1].temp,"ITEM SELL");

////////////////////////////////////////////////////////////////////////////////////////////
// ����
	SHideNomal[HN_HEAL_MAIN_TEXT][0].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_HEAL_MAIN_TEXT][0].y=140;
	sprintf(SHideNomal[HN_HEAL_MAIN_TEXT][0].temp,"CURE");
	SHideNomal[HN_HEAL_MAIN_TEXT][1].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_HEAL_MAIN_TEXT][1].y=171;
	sprintf(SHideNomal[HN_HEAL_MAIN_TEXT][1].temp,"PREVENTING");
	SHideNomal[HN_HEAL_MAIN_TEXT][2].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_HEAL_MAIN_TEXT][2].y=202;
	sprintf(SHideNomal[HN_HEAL_MAIN_TEXT][2].temp,"LEARN SKILL");

////////////////////////////////////////////////////////////////////////////////////////////
// ����

	SHideNomal[HN_HEAL_PREVENTING][0].nType=FT_TEXT_PUT_CENTER_X;
	SHideNomal[HN_HEAL_PREVENTING][0].x=224;
	SHideNomal[HN_HEAL_PREVENTING][0].y=72;
	sprintf(SHideNomal[HN_HEAL_PREVENTING][0].temp,"TYPHOID");		// ��ƼǪ��
	SHideNomal[HN_HEAL_PREVENTING][1].nType=FT_TEXT_PUT_CENTER_X;
	SHideNomal[HN_HEAL_PREVENTING][1].x=224;
	SHideNomal[HN_HEAL_PREVENTING][1].y=107;
	sprintf(SHideNomal[HN_HEAL_PREVENTING][1].temp,"YUT");			// ���༺ ������
	SHideNomal[HN_HEAL_PREVENTING][2].nType=FT_TEXT_PUT_CENTER_X;
	SHideNomal[HN_HEAL_PREVENTING][2].x=224;
	SHideNomal[HN_HEAL_PREVENTING][2].y=142;
	sprintf(SHideNomal[HN_HEAL_PREVENTING][2].temp,"SITOTOXISM");	// ���ߵ�
	SHideNomal[HN_HEAL_PREVENTING][3].nType=FT_TEXT_PUT_CENTER_X;
	SHideNomal[HN_HEAL_PREVENTING][3].x=224;
	SHideNomal[HN_HEAL_PREVENTING][3].y=72;
	sprintf(SHideNomal[HN_HEAL_PREVENTING][3].temp,"SPIRITLESS");	// �������
	SHideNomal[HN_HEAL_PREVENTING][4].nType=FT_TEXT_PUT_CENTER_X;
	SHideNomal[HN_HEAL_PREVENTING][4].x=224;
	SHideNomal[HN_HEAL_PREVENTING][4].y=107;
	sprintf(SHideNomal[HN_HEAL_PREVENTING][4].temp,"COLD");			// ����
	SHideNomal[HN_HEAL_PREVENTING][5].nType=FT_TEXT_PUT_CENTER_X;
	SHideNomal[HN_HEAL_PREVENTING][5].x=224;
	SHideNomal[HN_HEAL_PREVENTING][5].y=142;
	sprintf(SHideNomal[HN_HEAL_PREVENTING][5].temp,"VENEREAL");		// ����

////////////////////////////////////////////////////////////////////////////////////////////
// ����
	SHideNomal[HN_INN_MAIN_TEXT][0].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_INN_MAIN_TEXT][0].y=118;
	sprintf(SHideNomal[HN_INN_MAIN_TEXT][0].temp,"REST");
	SHideNomal[HN_INN_MAIN_TEXT][1].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_INN_MAIN_TEXT][1].y=147;
	sprintf(SHideNomal[HN_INN_MAIN_TEXT][1].temp,"SLEEP");
	SHideNomal[HN_INN_MAIN_TEXT][2].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_INN_MAIN_TEXT][2].y=176;
	sprintf(SHideNomal[HN_INN_MAIN_TEXT][2].temp,"LOGOUT");

////////////////////////////////////////////////////////////////////////////////////////////
// �������� ���� �޴�
/*	SHideNomal[HN_INN_REST_TEXT][0].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_INN_REST_TEXT][0].y=99;
	sprintf(SHideNomal[HN_INN_REST_TEXT][0].temp,"SINGLE");
	SHideNomal[HN_INN_REST_TEXT][1].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_INN_REST_TEXT][1].y=133;
	sprintf(SHideNomal[HN_INN_REST_TEXT][1].temp,"DOUBLE");
	SHideNomal[HN_INN_REST_TEXT][2].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_INN_REST_TEXT][2].y=167;
	sprintf(SHideNomal[HN_INN_REST_TEXT][2].temp,"MULTI");
*/
	SHideNomal[HN_INN_REST_TEXT][0].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_INN_REST_TEXT][0].y=133;
	sprintf(SHideNomal[HN_INN_REST_TEXT][0].temp,"SINGLE");

////////////////////////////////////////////////////////////////////////////////////////////
// Dungeon �޴� ���� ��

	// ARTIFICIAL���� �����
/*	char cArtificial[] = "�̰��� �°��� ȯ���Ѵ�. �̰��� �ʺ� ���谡�� ���� ��. ����� �ʺ��� �����ϸ� ���͵� ����� ����.";
	SHideNomal[HN_DUNGEON_ARTIFICAL_TEXT][0].nType=FT_TEXT_PUT_CUT;  // �� ���ڿ� �߶� ���
	SHideNomal[HN_DUNGEON_ARTIFICAL_TEXT][0].x=47;
	SHideNomal[HN_DUNGEON_ARTIFICAL_TEXT][0].y=84;
	sprintf(SHideNomal[HN_DUNGEON_ARTIFICAL_TEXT][0].temp,cArtificaial);
*/



///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////�޴� ������ ����/////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

	//	���� ����
	SMenu[MN_BANK_MAIN].x=366+GABX_SCREEN;
	SMenu[MN_BANK_MAIN].y=20;
	SMenu[MN_BANK_MAIN].nImageNumber=0;
	SMenu[MN_BANK_MAIN].nImageType=NEW_BANK_IMG;
//	SMenu[MN_BANK_MAIN].nImageNumber=284;
	SMenu[MN_BANK_MAIN].nFieldCount=8;
	SMenu[MN_BANK_MAIN].CheakType=0;

	// Ŭ���� ����
	SMenu[MN_BANK_MAIN].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_BANK_MAIN].nField[0].nSpecialWillDo=SWD_WILLDO;
	SMenu[MN_BANK_MAIN].nField[0].nWillDo=MN_BANK_DEPOSIT;
	SMenu[MN_BANK_MAIN].nField[0].x=38;	
	SMenu[MN_BANK_MAIN].nField[0].y=70;
	SMenu[MN_BANK_MAIN].nField[0].nImageNumber=16;
	SMenu[MN_BANK_MAIN].nField[0].nRectImage=16;
	SetRect(SMenu[MN_BANK_MAIN].nField[0].rCheakBox,38,69,205,95);

	SMenu[MN_BANK_MAIN].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_BANK_MAIN].nField[1].nSpecialWillDo=SWD_WILLDO;
	SMenu[MN_BANK_MAIN].nField[1].nWillDo=MN_BANK_DEFRAYAL;
	SMenu[MN_BANK_MAIN].nField[1].x=38;	
	SMenu[MN_BANK_MAIN].nField[1].y=100;
	SMenu[MN_BANK_MAIN].nField[1].nImageNumber=16;
	SMenu[MN_BANK_MAIN].nField[1].nRectImage=16;
	SetRect(SMenu[MN_BANK_MAIN].nField[1].rCheakBox,38,100,205,126);

	SMenu[MN_BANK_MAIN].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_BANK_MAIN].nField[2].nSpecialWillDo = SWD_BANK_ITEM;
	SMenu[MN_BANK_MAIN].nField[2].nWillDo=MN_BANK_CUSTODY;
	SMenu[MN_BANK_MAIN].nField[2].x=38;
	SMenu[MN_BANK_MAIN].nField[2].y=131;
	SMenu[MN_BANK_MAIN].nField[2].nImageNumber=16;
	SMenu[MN_BANK_MAIN].nField[2].nRectImage=16;
	SetRect(SMenu[MN_BANK_MAIN].nField[2].rCheakBox,38,131,205,157);

	SMenu[MN_BANK_MAIN].nField[3].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	//SMenu[MN_BANK_MAIN].nField[3].nSpecialWillDo=SWD_REQ_LOAN;
	SMenu[MN_BANK_MAIN].nField[3].nSpecialWillDo=SWD_SORRY;
	SMenu[MN_BANK_MAIN].nField[3].nWillDo=MN_BANK_LOAN;
	SMenu[MN_BANK_MAIN].nField[3].x=38;
	SMenu[MN_BANK_MAIN].nField[3].y=162;
	SMenu[MN_BANK_MAIN].nField[3].nImageNumber=16;
	SMenu[MN_BANK_MAIN].nField[3].nRectImage=16;
	SetRect(SMenu[MN_BANK_MAIN].nField[3].rCheakBox,38,162,205,188);

	SMenu[MN_BANK_MAIN].nField[4].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	//SMenu[MN_BANK_MAIN].nField[4].nSpecialWillDo=SWD_REQ_LOAN;
	SMenu[MN_BANK_MAIN].nField[4].nSpecialWillDo=SWD_SORRY;
	SMenu[MN_BANK_MAIN].nField[4].nWillDo=MN_BANK_REPAYMENT;
	SMenu[MN_BANK_MAIN].nField[4].x=38;	
	SMenu[MN_BANK_MAIN].nField[4].y=193;
	SMenu[MN_BANK_MAIN].nField[4].nImageNumber=16;
	SMenu[MN_BANK_MAIN].nField[4].nRectImage=16;
	SetRect(SMenu[MN_BANK_MAIN].nField[4].rCheakBox,38,193,205,219);

	SMenu[MN_BANK_MAIN].nField[5].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	//SMenu[MN_BANK_MAIN].nField[5].nSpecialWillDo=SWD_WILLDO;
	SMenu[MN_BANK_MAIN].nField[5].nSpecialWillDo=SWD_CALL_EXCHANGE_BOND_MONEY;//021126 lsw
	SMenu[MN_BANK_MAIN].nField[5].nWillDo=0;
	SMenu[MN_BANK_MAIN].nField[5].x=38;	
	SMenu[MN_BANK_MAIN].nField[5].y=224;
	SMenu[MN_BANK_MAIN].nField[5].nImageNumber=16;
	SMenu[MN_BANK_MAIN].nField[5].nRectImage=16;
	SetRect(SMenu[MN_BANK_MAIN].nField[5].rCheakBox,38,224,205,250);

	// Done Button
	SMenu[MN_BANK_MAIN].nField[6].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_BANK_MAIN].nField[6].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_BANK_MAIN].nField[6].x=89;
	SMenu[MN_BANK_MAIN].nField[6].y=281;
	SMenu[MN_BANK_MAIN].nField[6].nRectImage=492;
	SMenu[MN_BANK_MAIN].nField[6].nImageNumber=493;
	SetRect(SMenu[MN_BANK_MAIN].nField[6].rCheakBox,89,281,155,302);

	// �巡�׸� �̿��� �޴� �̵�
//	SMenu[MN_BANK_MAIN].nField[7].nType=FT_MENU_MOVE;
//	SetRect(SMenu[MN_BANK_MAIN].nField[7].rCheakBox,0,0,246,31);

	
//////////////////////////////////////////////////////////////////////////////////////////////////
	// DEPOSIT
	SMenu[MN_BANK_DEPOSIT].x=366+GABX_SCREEN;
	SMenu[MN_BANK_DEPOSIT].y=20;
//	SMenu[MN_BANK_DEPOSIT].nImageNumber=290;
	SMenu[MN_BANK_DEPOSIT].nImageNumber=3;
	SMenu[MN_BANK_DEPOSIT].nImageType=BANK_PCX;
	SMenu[MN_BANK_DEPOSIT].nFieldCount=8;
	SMenu[MN_BANK_DEPOSIT].CheakType=0;

	// �巡�׸� �̿��� �޴� �̵�
//	SMenu[MN_BANK_DEPOSIT].nField[0].nType=FT_MENU_MOVE;
//	SetRect(SMenu[MN_BANK_DEPOSIT].nField[0].rCheakBox,0,0,246,31);

	// ok, cancel  Button 
	SMenu[MN_BANK_DEPOSIT].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_BANK_DEPOSIT].nField[1].nSpecialWillDo=SWD_BANK_OK;
	SMenu[MN_BANK_DEPOSIT].nField[1].x=23;
	SMenu[MN_BANK_DEPOSIT].nField[1].y=274;
	SMenu[MN_BANK_DEPOSIT].nField[1].nRectImage=819;
	SMenu[MN_BANK_DEPOSIT].nField[1].nImageNumber=820;
	SetRect(SMenu[MN_BANK_DEPOSIT].nField[1].rCheakBox,23,274,89,295);

	SMenu[MN_BANK_DEPOSIT].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_BANK_DEPOSIT].nField[2].nSpecialWillDo=SWD_SUBMENU_BACK;
	SMenu[MN_BANK_DEPOSIT].nField[2].x=155;
	SMenu[MN_BANK_DEPOSIT].nField[2].y=274;
	SMenu[MN_BANK_DEPOSIT].nField[2].nRectImage=935;
	SMenu[MN_BANK_DEPOSIT].nField[2].nImageNumber=936;
	SetRect(SMenu[MN_BANK_DEPOSIT].nField[2].rCheakBox,156,274,222,295);

	// scroll button
	SMenu[MN_BANK_DEPOSIT].nField[3].nType=FT_DO;
	SMenu[MN_BANK_DEPOSIT].nField[3].nWillDo = DO_BANK_SCROLL_UP;
	SMenu[MN_BANK_DEPOSIT].nField[3].x=32;
	SMenu[MN_BANK_DEPOSIT].nField[3].y=203;
	SMenu[MN_BANK_DEPOSIT].nField[3].nRectImage=296;
	SMenu[MN_BANK_DEPOSIT].nField[3].nImageNumber=298;
	SetRect(SMenu[MN_BANK_DEPOSIT].nField[3].rCheakBox,32,203,54,218);

	SMenu[MN_BANK_DEPOSIT].nField[4].nType=FT_DO;
	SMenu[MN_BANK_DEPOSIT].nField[4].nWillDo = DO_BANK_SCROLL_DOWN;
	SMenu[MN_BANK_DEPOSIT].nField[4].x=32;
	SMenu[MN_BANK_DEPOSIT].nField[4].y=219;
	SMenu[MN_BANK_DEPOSIT].nField[4].nRectImage=297;
	SMenu[MN_BANK_DEPOSIT].nField[4].nImageNumber=299;
	SetRect(SMenu[MN_BANK_DEPOSIT].nField[4].rCheakBox,32,219,54,234);

	// ���� ������ â
	SMenu[MN_BANK_DEPOSIT].nField[5].nType=FT_DATA_PUT;
	SMenu[MN_BANK_DEPOSIT].nField[5].x=51;
	SMenu[MN_BANK_DEPOSIT].nField[5].y=212;
//	SMenu[MN_BANK_DEPOSIT].nField[5].nWillDo=0;
	SMenu[MN_BANK_DEPOSIT].nField[5].nImageNumber=DP_TEMP_MONEY;
//	SetRect(SMenu[MN_BANK_DEPOSIT].nField[5].rCheakBox,64,190,217,206);

	// ������ �ݾװ� ������ �ݾ��� ���ؼ� ������
	SMenu[MN_BANK_DEPOSIT].nField[6].nType=FT_DATA_PUT;
	SMenu[MN_BANK_DEPOSIT].nField[6].x=51;
	SMenu[MN_BANK_DEPOSIT].nField[6].y=130;
	SMenu[MN_BANK_DEPOSIT].nField[6].nImageNumber=DP_BANK_TOTAL;
//	SetRect(SMenu[MN_BANK_DEPOSIT].nField[5].rCheakBox,64,100,217,206);

	// ���� ������ �ݾ�
	SMenu[MN_BANK_DEPOSIT].nField[7].nType=FT_DATA_PUT;
	SMenu[MN_BANK_DEPOSIT].nField[7].x=51;
	SMenu[MN_BANK_DEPOSIT].nField[7].y=90;
	SMenu[MN_BANK_DEPOSIT].nField[7].nImageNumber=DP_BANK_SAVE;
//	SetRect(SMenu[MN_BANK_DEPOSIT].nField[5].rCheakBox,64,100,217,206);


///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
	// DEFRAYAL
	SMenu[MN_BANK_DEFRAYAL].x=366+GABX_SCREEN;
	SMenu[MN_BANK_DEFRAYAL].y=20;
//	SMenu[MN_BANK_DEFRAYAL].nImageNumber=290;
	SMenu[MN_BANK_DEFRAYAL].nImageNumber=3;
	SMenu[MN_BANK_DEFRAYAL].nImageType=BANK_PCX;
	SMenu[MN_BANK_DEFRAYAL].nFieldCount=8;
	SMenu[MN_BANK_DEFRAYAL].CheakType=0;

	// DEFRAYAL ���� ���
	SMenu[MN_BANK_DEFRAYAL].nField[0].nType=FT_NOMAL_PUT;
	SMenu[MN_BANK_DEFRAYAL].nField[0].x=64;
	SMenu[MN_BANK_DEFRAYAL].nField[0].y=13;
//	SMenu[MN_BANK_DEFRAYAL].nField[0].nImageNumber=289;
	SMenu[MN_BANK_DEFRAYAL].nField[0].nImageNumber=2;
	SMenu[MN_BANK_DEFRAYAL].nField[0].nImageType=BANK_PCX;
	
	// ok, cancel  Button 
	SMenu[MN_BANK_DEFRAYAL].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_BANK_DEFRAYAL].nField[1].nSpecialWillDo=SWD_BANK_OK;
	SMenu[MN_BANK_DEFRAYAL].nField[1].x=23;
	SMenu[MN_BANK_DEFRAYAL].nField[1].y=274;
	SMenu[MN_BANK_DEFRAYAL].nField[1].nRectImage=819;
	SMenu[MN_BANK_DEFRAYAL].nField[1].nImageNumber=820;
	SetRect(SMenu[MN_BANK_DEFRAYAL].nField[1].rCheakBox,23,274,89,295);

//	SMenu[MN_BANK_DEFRAYAL].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
//	SMenu[MN_BANK_DEFRAYAL].nField[2].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_BANK_DEFRAYAL].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_BANK_DEFRAYAL].nField[2].nSpecialWillDo=SWD_SUBMENU_BACK;
	SMenu[MN_BANK_DEFRAYAL].nField[2].x=155;
	SMenu[MN_BANK_DEFRAYAL].nField[2].y=274;
	SMenu[MN_BANK_DEFRAYAL].nField[2].nRectImage=935;
	SMenu[MN_BANK_DEFRAYAL].nField[2].nImageNumber=936;
	SetRect(SMenu[MN_BANK_DEFRAYAL].nField[2].rCheakBox,156,274,222,295);

	// scroll button
	SMenu[MN_BANK_DEFRAYAL].nField[3].nType=FT_DO;
	SMenu[MN_BANK_DEFRAYAL].nField[3].nWillDo = DO_BANK_SCROLL_UP;
	SMenu[MN_BANK_DEFRAYAL].nField[3].x=32;
	SMenu[MN_BANK_DEFRAYAL].nField[3].y=203;
	SMenu[MN_BANK_DEFRAYAL].nField[3].nRectImage=296;
	SMenu[MN_BANK_DEFRAYAL].nField[3].nImageNumber=298;
	SetRect(SMenu[MN_BANK_DEFRAYAL].nField[3].rCheakBox,32,203,54,218);

	SMenu[MN_BANK_DEFRAYAL].nField[4].nType=FT_DO;
	SMenu[MN_BANK_DEFRAYAL].nField[4].nWillDo = DO_BANK_SCROLL_DOWN;
	SMenu[MN_BANK_DEFRAYAL].nField[4].x=32;
	SMenu[MN_BANK_DEFRAYAL].nField[4].y=219;
	SMenu[MN_BANK_DEFRAYAL].nField[4].nRectImage=297;
	SMenu[MN_BANK_DEFRAYAL].nField[4].nImageNumber=299;
	SetRect(SMenu[MN_BANK_DEFRAYAL].nField[4].rCheakBox,32,219,54,234);

	// ���� ����� â
	SMenu[MN_BANK_DEFRAYAL].nField[5].nType=FT_DATA_PUT;
	SMenu[MN_BANK_DEFRAYAL].nField[5].x=51;
	SMenu[MN_BANK_DEFRAYAL].nField[5].y=212;
//	SMenu[MN_BANK_DEFRAYAL].nField[5].nWillDo=0;
	SMenu[MN_BANK_DEFRAYAL].nField[5].nImageNumber=DP_TEMP_MONEY;
//	SetRect(SMenu[MN_BANK_DEFRAYAL].nField[5].rCheakBox,64,190,217,206);

	// ����� �ݾװ� ���ݵ� �ݾ��� ����ؼ� ������
	SMenu[MN_BANK_DEFRAYAL].nField[6].nType=FT_DATA_PUT;
	SMenu[MN_BANK_DEFRAYAL].nField[6].x=51;
	SMenu[MN_BANK_DEFRAYAL].nField[6].y=130;
	SMenu[MN_BANK_DEFRAYAL].nField[6].nImageNumber=DP_BANK_TOTAL;
//	SetRect(SMenu[MN_BANK_DEFRAYAL].nField[5].rCheakBox,64,100,217,206);

	// ���� ���ݵ� �ݾ�
	SMenu[MN_BANK_DEFRAYAL].nField[7].nType=FT_DATA_PUT;
	SMenu[MN_BANK_DEFRAYAL].nField[7].x=51;
	SMenu[MN_BANK_DEFRAYAL].nField[7].y=90;
	SMenu[MN_BANK_DEFRAYAL].nField[7].nImageNumber=DP_BANK_SAVE;
//	SetRect(SMenu[MN_BANK_DEFRAYAL].nField[5].rCheakBox,64,100,217,206);

///////////////////////////////////////////////////////////////////////////////////////////////
	//���� �޴����� CUSTODY�ٿ��

	//���� �޴� �ٿ��
	SMenu[MN_BANK_CUSTODY].x=150;
	SMenu[MN_BANK_CUSTODY].y=20;
	SMenu[MN_BANK_CUSTODY].nImageNumber=1;
	SMenu[MN_BANK_CUSTODY].nImageType=BANK_PCX;
	SMenu[MN_BANK_CUSTODY].nFieldCount=9;
	SMenu[MN_BANK_CUSTODY].CheakType=0;

	//���� �κ��丮â ��ũ���� ���� ���� �ʵ�
	SMenu[MN_BANK_CUSTODY].nField[0].nType=FT_HIDE_NOMAL_GROUP_PUT;
	SMenu[MN_BANK_CUSTODY].nField[0].nSHideNomalNumber=HN_INVENTORY;
	SMenu[MN_BANK_CUSTODY].nField[0].nSHideNomalCount=5;
	SMenu[MN_BANK_CUSTODY].nField[0].nSHideNomalStart=0;
	SMenu[MN_BANK_CUSTODY].nField[0].nShideNomalPlus=1;

	//���� �κ��丮â ��ũ�ѿ� �ʵ�					
	SMenu[MN_BANK_CUSTODY].nField[1].nType=FT_HIDE_DSCROLL_AUTO_PUT;
	SMenu[MN_BANK_CUSTODY].nField[1].nWillDo=0;
	SMenu[MN_BANK_CUSTODY].nField[1].x=220;
	SMenu[MN_BANK_CUSTODY].nField[1].y=209;
	SMenu[MN_BANK_CUSTODY].nField[1].nImageNumber=439;
	SMenu[MN_BANK_CUSTODY].nField[1].nRectImage=437;
	SetRect(SMenu[MN_BANK_CUSTODY].nField[1].rCheakBox,220,209,237,237);

	//���� �κ��丮â ��ũ�ѿ� �ʵ�
	SMenu[MN_BANK_CUSTODY].nField[2].nType=FT_HIDE_ISCROLL_AUTO_PUT;
	SMenu[MN_BANK_CUSTODY].nField[2].nWillDo=0;
	SMenu[MN_BANK_CUSTODY].nField[2].x=220;
	SMenu[MN_BANK_CUSTODY].nField[2].y=283;
	SMenu[MN_BANK_CUSTODY].nField[2].nImageNumber=440;
	SMenu[MN_BANK_CUSTODY].nField[2].nRectImage=438;
	SetRect(SMenu[MN_BANK_CUSTODY].nField[2].rCheakBox,220,281,237,309);

	//���� ���콺 �̺�Ʈ�� üũ�ϴ� ���� �κ��丮 �ʵ�
	SMenu[MN_BANK_CUSTODY].nField[3].nType=FT_BANK_INVENTORY;
	//	SetRect(SMenu[MN_BANK_CUSTODY].nField[3].rCheakBox,7,206,217,315);
	SetRect(SMenu[MN_BANK_CUSTODY].nField[3].rCheakBox,7,206,217,311);//020508 lsw


	//���� �κ��丮�� ������ �����ִ� �ʵ�
	//SMenu[MN_BANK_CUSTODY].nField[4].nType=FT_DATA_PUT;
	//SMenu[MN_BANK_CUSTODY].nField[4].nImageNumber=DP_BANK_INVENTORY;

	// ���� ������
	SMenu[MN_BANK_CUSTODY].nField[5].nType=FT_DATA_PUT;
	SMenu[MN_BANK_CUSTODY].nField[5].x=29;
	SMenu[MN_BANK_CUSTODY].nField[5].y=112;
	SMenu[MN_BANK_CUSTODY].nField[5].nImageNumber=DP_BANK_CURRENT_PAY;
	// �����ؾ��� ������
	SMenu[MN_BANK_CUSTODY].nField[6].nType=FT_DATA_PUT;
	SMenu[MN_BANK_CUSTODY].nField[6].x=89;
	SMenu[MN_BANK_CUSTODY].nField[6].y=145;
	SMenu[MN_BANK_CUSTODY].nField[6].nImageNumber=DP_BANK_PAY;

	// ���� �κ��丮�� �ִ� �������� ���� 
	SMenu[MN_BANK_CUSTODY].nField[7].nType=FT_DATA_PUT;
	SMenu[MN_BANK_CUSTODY].nField[7].x=52;
	SMenu[MN_BANK_CUSTODY].nField[7].y=60;
	SMenu[MN_BANK_CUSTODY].nField[7].nImageNumber=DP_WEIGHT;

	// ���� �κ��丮�� DONE
	SMenu[MN_BANK_CUSTODY].nField[8].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_BANK_CUSTODY].nField[8].nSpecialWillDo=SWD_BANK_ITEM_END;
	SMenu[MN_BANK_CUSTODY].nField[8].x=181;
	SMenu[MN_BANK_CUSTODY].nField[8].y=174;
	SMenu[MN_BANK_CUSTODY].nField[8].nRectImage=441;
	SMenu[MN_BANK_CUSTODY].nField[8].nImageNumber=442;
	SetRect(SMenu[MN_BANK_CUSTODY].nField[8].rCheakBox,177,171,233,198);


///////////////////////////////////////////////////////////////////////////////////////////////////
	//���� �޴����� LOAN�޴� �ٿ��

	//���� �޴� �ٿ��
	SMenu[MN_BANK_LOAN].x=366+GABX_SCREEN;
	SMenu[MN_BANK_LOAN].y=20;
#ifdef CHANGE_MONEY_		// 010808 YGI
	SMenu[MN_BANK_LOAN].nImageNumber=2;			// 
	SMenu[MN_BANK_LOAN].nImageType=CHANGE_MONEY;
#else 
	SMenu[MN_BANK_LOAN].nImageNumber=303;
#endif
	SMenu[MN_BANK_LOAN].nFieldCount=10;
	SMenu[MN_BANK_LOAN].CheakType=0;

	// �巡�׸� �̿��� �޴� �̵�
//	SMenu[MN_BANK_LOAN].nField[0].nType=FT_MENU_MOVE;
//	SetRect(SMenu[MN_BANK_LOAN].nField[0].rCheakBox,0,0,246,31);

	SMenu[MN_BANK_LOAN].nField[0].nType=FT_BANK_LOAN;

	// ok, cancel  Button 
	SMenu[MN_BANK_LOAN].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_BANK_LOAN].nField[1].nSpecialWillDo=SWD_BANK_OK;
	SMenu[MN_BANK_LOAN].nField[1].x=24;
	SMenu[MN_BANK_LOAN].nField[1].y=279;
	SMenu[MN_BANK_LOAN].nField[1].nRectImage=819;
	SMenu[MN_BANK_LOAN].nField[1].nImageNumber=820;
	SetRect(SMenu[MN_BANK_LOAN].nField[1].rCheakBox,24,279,90,300);

	SMenu[MN_BANK_LOAN].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_BANK_LOAN].nField[2].nSpecialWillDo=SWD_SUBMENU_BACK;
	SMenu[MN_BANK_LOAN].nField[2].x=156;
	SMenu[MN_BANK_LOAN].nField[2].y=279;
	SMenu[MN_BANK_LOAN].nField[2].nRectImage=935;
	SMenu[MN_BANK_LOAN].nField[2].nImageNumber=936;
	SetRect(SMenu[MN_BANK_LOAN].nField[2].rCheakBox,156,279,222,300);

	// scroll button
	SMenu[MN_BANK_LOAN].nField[3].nType=FT_DO;
	SMenu[MN_BANK_LOAN].nField[3].nWillDo = DO_BANK_SCROLL_UP;
	SMenu[MN_BANK_LOAN].nField[3].x=31;
	SMenu[MN_BANK_LOAN].nField[3].y=235;
	SMenu[MN_BANK_LOAN].nField[3].nRectImage=296;
	SMenu[MN_BANK_LOAN].nField[3].nImageNumber=298;
	SetRect(SMenu[MN_BANK_LOAN].nField[3].rCheakBox,31,235,53,251);

	SMenu[MN_BANK_LOAN].nField[4].nType=FT_DO;
	SMenu[MN_BANK_LOAN].nField[4].nWillDo = DO_BANK_SCROLL_DOWN;
	SMenu[MN_BANK_LOAN].nField[4].x=31;
	SMenu[MN_BANK_LOAN].nField[4].y=251;
	SMenu[MN_BANK_LOAN].nField[4].nRectImage=297;
	SMenu[MN_BANK_LOAN].nField[4].nImageNumber=299;
	SetRect(SMenu[MN_BANK_LOAN].nField[4].rCheakBox,31,252,53,267);

	// ���� ������ â
	SMenu[MN_BANK_LOAN].nField[5].nType=FT_DATA_PUT;
	SMenu[MN_BANK_LOAN].nField[5].x=55;
	SMenu[MN_BANK_LOAN].nField[5].y=245;
	SMenu[MN_BANK_LOAN].nField[5].nImageNumber=DP_TEMP_MONEY;

	// �ſ��� ǥ��
	SMenu[MN_BANK_LOAN].nField[6].nType=FT_DATA_PUT;
	SMenu[MN_BANK_LOAN].nField[6].x=37;
	SMenu[MN_BANK_LOAN].nField[6].y=62;
	SMenu[MN_BANK_LOAN].nField[6].nImageNumber=DP_CREDIT_TEXT;

	// ���� �Ҽ� �ִ� �Ѿ�
	SMenu[MN_BANK_LOAN].nField[7].nType=FT_DATA_PUT;
	SMenu[MN_BANK_LOAN].nField[7].x=88;
	SMenu[MN_BANK_LOAN].nField[7].y=115;
	SMenu[MN_BANK_LOAN].nField[7].nImageNumber=DP_LOAN_TOTAL;

	// ���� ���ɾ�
	SMenu[MN_BANK_LOAN].nField[8].nType=FT_DATA_PUT;
	SMenu[MN_BANK_LOAN].nField[8].x=88;
	SMenu[MN_BANK_LOAN].nField[8].y=154;
	SMenu[MN_BANK_LOAN].nField[8].nImageNumber=DP_LOAN_ABLE;

	// ���ݱ��� ������ �ݾ�
	SMenu[MN_BANK_LOAN].nField[9].nType=FT_DATA_PUT;
	SMenu[MN_BANK_LOAN].nField[9].x=88;
	SMenu[MN_BANK_LOAN].nField[9].y=192;
	SMenu[MN_BANK_LOAN].nField[9].nImageNumber=DP_LAST_LOAN;


//////////////////////////////////////////////////////////////////////////////////////////////////
	//���� �޴����� REPAYMENT�޴� �ٿ��

	//���� �޴� �ٿ��
	SMenu[MN_BANK_REPAYMENT].x=366+GABX_SCREEN;
	SMenu[MN_BANK_REPAYMENT].y=20;
#ifdef CHANGE_MONEY_		// 010808 YGI
	SMenu[MN_BANK_REPAYMENT].nImageNumber=1;			// 
	SMenu[MN_BANK_REPAYMENT].nImageType=CHANGE_MONEY;
#else 
	SMenu[MN_BANK_REPAYMENT].nImageNumber=304;
#endif
	SMenu[MN_BANK_REPAYMENT].nFieldCount=10;
	SMenu[MN_BANK_REPAYMENT].CheakType=0;		

	// �巡�׸� �̿��� �޴� �̵�
//	SMenu[MN_BANK_LOAN].nField[0].nType=FT_MENU_MOVE;
//	SetRect(SMenu[MN_BANK_LOAN].nField[0].rCheakBox,0,0,246,31);

	// �������� ������ ������ ����
	SMenu[MN_BANK_REPAYMENT].nField[0].nType=FT_BANK_LOAN;

	// ok, cancel  Button 
	SMenu[MN_BANK_REPAYMENT].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_BANK_REPAYMENT].nField[1].nSpecialWillDo=SWD_BANK_OK;
	SMenu[MN_BANK_REPAYMENT].nField[1].x=24;
#ifdef KOREA_LOCALIZING_//010809 lms
	SMenu[MN_BANK_REPAYMENT].nField[1].y=279;
#else
	SMenu[MN_BANK_REPAYMENT].nField[1].y=277;
#endif
	SMenu[MN_BANK_REPAYMENT].nField[1].nRectImage=819;
	SMenu[MN_BANK_REPAYMENT].nField[1].nImageNumber=820;
	SetRect(SMenu[MN_BANK_REPAYMENT].nField[1].rCheakBox,24,279,90,300);

	SMenu[MN_BANK_REPAYMENT].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_BANK_REPAYMENT].nField[2].nSpecialWillDo=SWD_SUBMENU_BACK;
	SMenu[MN_BANK_REPAYMENT].nField[2].x=156;
#ifdef KOREA_LOCALIZING_//010809 lms
	SMenu[MN_BANK_REPAYMENT].nField[2].y=279;
#else
	SMenu[MN_BANK_REPAYMENT].nField[2].y=277;
#endif
	SMenu[MN_BANK_REPAYMENT].nField[2].nRectImage=935;
	SMenu[MN_BANK_REPAYMENT].nField[2].nImageNumber=936;
	SetRect(SMenu[MN_BANK_REPAYMENT].nField[2].rCheakBox,156,279,222,300);

	// scroll button
	SMenu[MN_BANK_REPAYMENT].nField[3].nType=FT_DO;
	SMenu[MN_BANK_REPAYMENT].nField[3].nWillDo = DO_BANK_SCROLL_UP;
	SMenu[MN_BANK_REPAYMENT].nField[3].x=31;
#ifdef KOREA_LOCALIZING_//010809 lms
	SMenu[MN_BANK_REPAYMENT].nField[3].y=235;
#else
	SMenu[MN_BANK_REPAYMENT].nField[3].y=225;
#endif
	SMenu[MN_BANK_REPAYMENT].nField[3].nRectImage=296;
	SMenu[MN_BANK_REPAYMENT].nField[3].nImageNumber=298;
	SetRect(SMenu[MN_BANK_REPAYMENT].nField[3].rCheakBox,31,235,53,251);

	SMenu[MN_BANK_REPAYMENT].nField[4].nType=FT_DO;
	SMenu[MN_BANK_REPAYMENT].nField[4].nWillDo = DO_BANK_SCROLL_DOWN;
	SMenu[MN_BANK_REPAYMENT].nField[4].x=31;
#ifdef KOREA_LOCALIZING_//010809 lms
	SMenu[MN_BANK_REPAYMENT].nField[4].y=251;
#else
	SMenu[MN_BANK_REPAYMENT].nField[4].y=241;
#endif
	SMenu[MN_BANK_REPAYMENT].nField[4].nRectImage=297;
	SMenu[MN_BANK_REPAYMENT].nField[4].nImageNumber=299;
	SetRect(SMenu[MN_BANK_REPAYMENT].nField[4].rCheakBox,20,244,48,262);

	// ��ȯ�� �ݾ� ǥ�� â
	SMenu[MN_BANK_REPAYMENT].nField[5].nType=FT_DATA_PUT;
	SMenu[MN_BANK_REPAYMENT].nField[5].x=55;
	SMenu[MN_BANK_REPAYMENT].nField[5].y=245;
	SMenu[MN_BANK_REPAYMENT].nField[5].nImageNumber=DP_TEMP_MONEY;

	// �ſ��� ǥ��
	SMenu[MN_BANK_REPAYMENT].nField[6].nType=FT_DATA_PUT;
	SMenu[MN_BANK_REPAYMENT].nField[6].x=37;
	SMenu[MN_BANK_REPAYMENT].nField[6].y=62;
	SMenu[MN_BANK_REPAYMENT].nField[6].nImageNumber=DP_CREDIT_TEXT;

	// ������� ������ �Ѿ�
	SMenu[MN_BANK_REPAYMENT].nField[7].nType=FT_DATA_PUT;
	SMenu[MN_BANK_REPAYMENT].nField[7].x=88;
	SMenu[MN_BANK_REPAYMENT].nField[7].y=115;
	SMenu[MN_BANK_REPAYMENT].nField[7].nImageNumber=DP_LOAN_TOTAL;

	// �����Ҽ� �ִ� �ݾ�
	SMenu[MN_BANK_REPAYMENT].nField[8].nType=FT_DATA_PUT;
	SMenu[MN_BANK_REPAYMENT].nField[8].x=88;
	SMenu[MN_BANK_REPAYMENT].nField[8].y=154;
	SMenu[MN_BANK_REPAYMENT].nField[8].nImageNumber=DP_LOAN_ABLE;

	// ���ݱ��� ������ �ݾ�
	SMenu[MN_BANK_REPAYMENT].nField[9].nType=FT_DATA_PUT;
	SMenu[MN_BANK_REPAYMENT].nField[9].x=88;
	SMenu[MN_BANK_REPAYMENT].nField[9].y=192;
	SMenu[MN_BANK_REPAYMENT].nField[9].nImageNumber=DP_LAST_LOAN;


//////////////////////////////////////////////////////////////////////////////////////////////////
	//���� �޴����� ACUTION�޴� �ٿ��

	//���� �޴� �ٿ��
	SMenu[MN_BANK_AUCTION].x=366+GABX_SCREEN;
	SMenu[MN_BANK_AUCTION].y=20;
	SMenu[MN_BANK_AUCTION].nImageNumber=300;
	SMenu[MN_BANK_AUCTION].nFieldCount=10;
	SMenu[MN_BANK_AUCTION].CheakType=0;		

	// �巡�׸� �̿��� �޴� �̵�
//	SMenu[MN_BANK_AUCTION].nField[0].nType=FT_MENU_MOVE;
//	SetRect(SMenu[MN_BANK_AUCTION].nField[0].rCheakBox,0,0,246,31);

	// ok, cancel  Button 
	SMenu[MN_BANK_AUCTION].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_BANK_AUCTION].nField[1].nSpecialWillDo=SWD_BANK_OK;
	SMenu[MN_BANK_AUCTION].nField[1].x=15;
	SMenu[MN_BANK_AUCTION].nField[1].y=265;
	SMenu[MN_BANK_AUCTION].nField[1].nRectImage=294;
	SMenu[MN_BANK_AUCTION].nField[1].nImageNumber=295;
	SetRect(SMenu[MN_BANK_AUCTION].nField[1].rCheakBox,13,263,57,307);

//	SMenu[MN_BANK_AUCTION].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
//	SMenu[MN_BANK_AUCTION].nField[2].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_BANK_AUCTION].nField[2].nType=FT_HIDE_WILLDO_AUTO_PUT;
	SMenu[MN_BANK_AUCTION].nField[2].nWillDo=MN_BANK_MAIN;
	SMenu[MN_BANK_AUCTION].nField[2].x=190;
	SMenu[MN_BANK_AUCTION].nField[2].y=265;
	SMenu[MN_BANK_AUCTION].nField[2].nRectImage=292;
	SMenu[MN_BANK_AUCTION].nField[2].nImageNumber=293;
	SetRect(SMenu[MN_BANK_AUCTION].nField[2].rCheakBox,188,263,232,307);

	// sale �޴��� ��ȯ
	SMenu[MN_BANK_AUCTION].nField[3].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_BANK_AUCTION].nField[3].nSpecialWillDo=SWD_TWO_MENU_OPEN;
	SMenu[MN_BANK_AUCTION].nField[3].nWillDo=MN_BANK_AUCTION_SALE;
	SMenu[MN_BANK_AUCTION].nField[3].nSHideNomalNumber=MN_BANK_CUSTODY;
	SMenu[MN_BANK_AUCTION].nField[3].x=84;
	SMenu[MN_BANK_AUCTION].nField[3].y=273;
	SMenu[MN_BANK_AUCTION].nField[3].nRectImage=443;
	SMenu[MN_BANK_AUCTION].nField[3].nImageNumber=444;
	SetRect(SMenu[MN_BANK_AUCTION].nField[3].rCheakBox,82,271,160,301);

	// scroll button	// ��
	SMenu[MN_BANK_AUCTION].nField[4].nType=FT_DO;
	SMenu[MN_BANK_AUCTION].nField[4].nWillDo = DO_BANK_SCROLL_UP;
	SMenu[MN_BANK_AUCTION].nField[4].x=23;
	SMenu[MN_BANK_AUCTION].nField[4].y=152;
	SMenu[MN_BANK_AUCTION].nField[4].nRectImage=452;
	SMenu[MN_BANK_AUCTION].nField[4].nImageNumber=453;
	SetRect(SMenu[MN_BANK_AUCTION].nField[4].rCheakBox,23,152,41,167);

	SMenu[MN_BANK_AUCTION].nField[5].nType=FT_DO;
	SMenu[MN_BANK_AUCTION].nField[5].nWillDo = DO_BANK_SCROLL_DOWN;
	SMenu[MN_BANK_AUCTION].nField[5].x=23;
	SMenu[MN_BANK_AUCTION].nField[5].y=167;
	SMenu[MN_BANK_AUCTION].nField[5].nRectImage=454;
	SMenu[MN_BANK_AUCTION].nField[5].nImageNumber=455;
	SetRect(SMenu[MN_BANK_AUCTION].nField[5].rCheakBox,23,168,41,183);

	// scroll button	// ���� (��, ��)
	SMenu[MN_BANK_AUCTION].nField[8].nType=FT_DO;
	SMenu[MN_BANK_AUCTION].nField[8].nWillDo = DO_BANK_SCROLL_LEFT;
	SMenu[MN_BANK_AUCTION].nField[8].x=27;
	SMenu[MN_BANK_AUCTION].nField[8].y=85;
	SMenu[MN_BANK_AUCTION].nField[8].nRectImage=450;
	SMenu[MN_BANK_AUCTION].nField[8].nImageNumber=451;
	SetRect(SMenu[MN_BANK_AUCTION].nField[8].rCheakBox,24,82,47,114);

	SMenu[MN_BANK_AUCTION].nField[9].nType=FT_DO;
	SMenu[MN_BANK_AUCTION].nField[9].nWillDo = DO_BANK_SCROLL_RIGHT;
	SMenu[MN_BANK_AUCTION].nField[9].x=203;
	SMenu[MN_BANK_AUCTION].nField[9].y=85;
	SMenu[MN_BANK_AUCTION].nField[9].nRectImage=448;
	SMenu[MN_BANK_AUCTION].nField[9].nImageNumber=449;
	SetRect(SMenu[MN_BANK_AUCTION].nField[9].rCheakBox,201,82,224,114);

/////////////////////////////////////////////////////////////////////////////////////////////////
// ��� �ȱ�

	//���� �޴� �ٿ��
	SMenu[MN_BANK_AUCTION_SALE].x=364;
	SMenu[MN_BANK_AUCTION_SALE].y=20;
	SMenu[MN_BANK_AUCTION_SALE].nImageNumber=300;
	SMenu[MN_BANK_AUCTION_SALE].nFieldCount=10;
	SMenu[MN_BANK_AUCTION_SALE].CheakType=0;		

	// sale button
	SMenu[MN_BANK_AUCTION_SALE].nField[0].nType=FT_NOMAL_PUT;
	SMenu[MN_BANK_AUCTION_SALE].nField[0].x=84;
	SMenu[MN_BANK_AUCTION_SALE].nField[0].y=273;
	SMenu[MN_BANK_AUCTION_SALE].nField[0].nImageNumber=445;

	// ok, cancel  Button 
	SMenu[MN_BANK_AUCTION_SALE].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_BANK_AUCTION_SALE].nField[1].nSpecialWillDo=SWD_BANK_OK;
	SMenu[MN_BANK_AUCTION_SALE].nField[1].x=15;
	SMenu[MN_BANK_AUCTION_SALE].nField[1].y=265;
	SMenu[MN_BANK_AUCTION_SALE].nField[1].nRectImage=294;
	SMenu[MN_BANK_AUCTION_SALE].nField[1].nImageNumber=295;
	SetRect(SMenu[MN_BANK_AUCTION_SALE].nField[1].rCheakBox,13,263,57,307);

	SMenu[MN_BANK_AUCTION_SALE].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_BANK_AUCTION_SALE].nField[2].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_BANK_AUCTION_SALE].nField[2].x=190;
	SMenu[MN_BANK_AUCTION_SALE].nField[2].y=265;
	SMenu[MN_BANK_AUCTION_SALE].nField[2].nRectImage=292;
	SMenu[MN_BANK_AUCTION_SALE].nField[2].nImageNumber=293;
	SetRect(SMenu[MN_BANK_AUCTION_SALE].nField[2].rCheakBox,188,263,232,305);

	// BUY �޴��� ��ȯ
	SMenu[MN_BANK_AUCTION_SALE].nField[3].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_BANK_AUCTION_SALE].nField[3].nSpecialWillDo=SWD_SIMPLE_CLOSE;
	SMenu[MN_BANK_AUCTION_SALE].nField[3].nWillDo=MN_BANK_AUCTION;
	SMenu[MN_BANK_AUCTION_SALE].nField[3].x=84;
	SMenu[MN_BANK_AUCTION_SALE].nField[3].y=273;
	SMenu[MN_BANK_AUCTION_SALE].nField[3].nRectImage=446;
	SMenu[MN_BANK_AUCTION_SALE].nField[3].nImageNumber=447;
	SetRect(SMenu[MN_BANK_AUCTION_SALE].nField[3].rCheakBox,82,271,160,301);

	// scroll button	// ��
	SMenu[MN_BANK_AUCTION_SALE].nField[4].nType=FT_DO;
	SMenu[MN_BANK_AUCTION_SALE].nField[4].nWillDo = DO_BANK_SCROLL_UP;
	SMenu[MN_BANK_AUCTION_SALE].nField[4].x=23;
	SMenu[MN_BANK_AUCTION_SALE].nField[4].y=152;
	SMenu[MN_BANK_AUCTION_SALE].nField[4].nRectImage=452;
	SMenu[MN_BANK_AUCTION_SALE].nField[4].nImageNumber=453;
	SetRect(SMenu[MN_BANK_AUCTION_SALE].nField[4].rCheakBox,23,152,41,167);

	SMenu[MN_BANK_AUCTION_SALE].nField[5].nType=FT_DO;
	SMenu[MN_BANK_AUCTION_SALE].nField[5].nWillDo = DO_BANK_SCROLL_DOWN;
	SMenu[MN_BANK_AUCTION_SALE].nField[5].x=23;
	SMenu[MN_BANK_AUCTION_SALE].nField[5].y=167;
	SMenu[MN_BANK_AUCTION_SALE].nField[5].nRectImage=454;
	SMenu[MN_BANK_AUCTION_SALE].nField[5].nImageNumber=455;
	SetRect(SMenu[MN_BANK_AUCTION_SALE].nField[5].rCheakBox,23,168,41,185);

	// scroll button	// �ð�
	SMenu[MN_BANK_AUCTION_SALE].nField[6].nType=FT_DO;
	SMenu[MN_BANK_AUCTION_SALE].nField[6].nWillDo = DO_BANK_TIME_SCROLL_UP;
	SMenu[MN_BANK_AUCTION_SALE].nField[6].x=23;
	SMenu[MN_BANK_AUCTION_SALE].nField[6].y=189;
	SMenu[MN_BANK_AUCTION_SALE].nField[6].nRectImage=452;
	SMenu[MN_BANK_AUCTION_SALE].nField[6].nImageNumber=453;
	SetRect(SMenu[MN_BANK_AUCTION_SALE].nField[6].rCheakBox,23,189,41,204);

	SMenu[MN_BANK_AUCTION_SALE].nField[7].nType=FT_DO;
	SMenu[MN_BANK_AUCTION_SALE].nField[7].nWillDo = DO_BANK_TIME_SCROLL_DOWN;
	SMenu[MN_BANK_AUCTION_SALE].nField[7].x=23;
	SMenu[MN_BANK_AUCTION_SALE].nField[7].y=204;
	SMenu[MN_BANK_AUCTION_SALE].nField[7].nRectImage=454;
	SMenu[MN_BANK_AUCTION_SALE].nField[7].nImageNumber=455;
	SetRect(SMenu[MN_BANK_AUCTION_SALE].nField[7].rCheakBox,23,205,41,221);

	// scroll button	// ���� (��, ��)
	SMenu[MN_BANK_AUCTION_SALE].nField[8].nType=FT_DO;
	SMenu[MN_BANK_AUCTION_SALE].nField[8].nWillDo = DO_BANK_SCROLL_LEFT;
	SMenu[MN_BANK_AUCTION_SALE].nField[8].x=27;
	SMenu[MN_BANK_AUCTION_SALE].nField[8].y=85;
	SMenu[MN_BANK_AUCTION_SALE].nField[8].nRectImage=450;
	SMenu[MN_BANK_AUCTION_SALE].nField[8].nImageNumber=451;
	SetRect(SMenu[MN_BANK_AUCTION_SALE].nField[8].rCheakBox,24,82,47,114);

	SMenu[MN_BANK_AUCTION_SALE].nField[9].nType=FT_DO;
	SMenu[MN_BANK_AUCTION_SALE].nField[9].nWillDo = DO_BANK_SCROLL_RIGHT;
	SMenu[MN_BANK_AUCTION_SALE].nField[9].x=203;
	SMenu[MN_BANK_AUCTION_SALE].nField[9].y=85;
	SMenu[MN_BANK_AUCTION_SALE].nField[9].nRectImage=448;
	SMenu[MN_BANK_AUCTION_SALE].nField[9].nImageNumber=449;
	SetRect(SMenu[MN_BANK_AUCTION_SALE].nField[9].rCheakBox,201,82,224,114);

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
    //���� �޴��� ��Ƽ���� �޴� ����
    // 帳號互通
	SMenu[MN_BANK_CUSTODY2].x = 150;
	SMenu[MN_BANK_CUSTODY2].y = 20;
	SMenu[MN_BANK_CUSTODY2].nImageNumber = 1;
	SMenu[MN_BANK_CUSTODY2].nImageType = BANK_PCX;
	SMenu[MN_BANK_CUSTODY2].nFieldCount = 9;
	SMenu[MN_BANK_CUSTODY2].CheakType = 0;

	//���� �κ��丮â ��ũ���� ���� ���� �ʵ�
	SMenu[MN_BANK_CUSTODY2].nField[0].nType = FT_HIDE_NOMAL_GROUP_PUT;
	SMenu[MN_BANK_CUSTODY2].nField[0].nSHideNomalNumber = HN_INVENTORY;
	SMenu[MN_BANK_CUSTODY2].nField[0].nSHideNomalCount = 1;//改成5就可以5頁
	SMenu[MN_BANK_CUSTODY2].nField[0].nSHideNomalStart = 0;
	SMenu[MN_BANK_CUSTODY2].nField[0].nShideNomalPlus = 1;

	//���� �κ��丮â ��ũ�ѿ� �ʵ�					
	SMenu[MN_BANK_CUSTODY2].nField[1].nType = FT_HIDE_DSCROLL_AUTO_PUT;
	SMenu[MN_BANK_CUSTODY2].nField[1].nWillDo = 0;
	SMenu[MN_BANK_CUSTODY2].nField[1].x = 220;
	SMenu[MN_BANK_CUSTODY2].nField[1].y = 209;
	SMenu[MN_BANK_CUSTODY2].nField[1].nImageNumber = 439;
	SMenu[MN_BANK_CUSTODY2].nField[1].nRectImage = 437;
	SetRect(SMenu[MN_BANK_CUSTODY2].nField[1].rCheakBox, 220, 209, 237, 237);

	//���� �κ��丮â ��ũ�ѿ� �ʵ�
	SMenu[MN_BANK_CUSTODY2].nField[2].nType = FT_HIDE_ISCROLL_AUTO_PUT;
	SMenu[MN_BANK_CUSTODY2].nField[2].nWillDo = 0;
	SMenu[MN_BANK_CUSTODY2].nField[2].x = 220;
	SMenu[MN_BANK_CUSTODY2].nField[2].y = 283;
	SMenu[MN_BANK_CUSTODY2].nField[2].nImageNumber = 440;
	SMenu[MN_BANK_CUSTODY2].nField[2].nRectImage = 438;
	SetRect(SMenu[MN_BANK_CUSTODY2].nField[2].rCheakBox, 220, 281, 237, 309);

	//���� ���콺 �̺�Ʈ�� üũ�ϴ� ���� �κ��丮 �ʵ�
	SMenu[MN_BANK_CUSTODY2].nField[3].nType = FT_BANK2_INVENTORY;
	//	SetRect(SMenu[MN_BANK_CUSTODY].nField[3].rCheakBox,7,206,217,315);
	SetRect(SMenu[MN_BANK_CUSTODY2].nField[3].rCheakBox, 7, 206, 217, 311);//020508 lsw
	// ���� ������
	/*SMenu[MN_BANK_CUSTODY2].nField[5].nType = FT_DATA_PUT;
	SMenu[MN_BANK_CUSTODY2].nField[5].x = 29;
	SMenu[MN_BANK_CUSTODY2].nField[5].y = 112;
	SMenu[MN_BANK_CUSTODY2].nField[5].nImageNumber = DP_BANK_CURRENT_PAY;
	// �����ؾ��� ������
	SMenu[MN_BANK_CUSTODY2].nField[6].nType = FT_DATA_PUT;
	SMenu[MN_BANK_CUSTODY2].nField[6].x = 89;
	SMenu[MN_BANK_CUSTODY2].nField[6].y = 145;
	SMenu[MN_BANK_CUSTODY2].nField[6].nImageNumber = DP_BANK_PAY;

	// ���� �κ��丮�� �ִ� �������� ���� 
	SMenu[MN_BANK_CUSTODY2].nField[7].nType = FT_DATA_PUT;
	SMenu[MN_BANK_CUSTODY2].nField[7].x = 52;
	SMenu[MN_BANK_CUSTODY2].nField[7].y = 60;
	SMenu[MN_BANK_CUSTODY2].nField[7].nImageNumber = DP_WEIGHT;*/

	// ���� �κ��丮�� DONE
	SMenu[MN_BANK_CUSTODY2].nField[8].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_BANK_CUSTODY2].nField[8].nSpecialWillDo = SWD_BANK_ITEM_END;
	SMenu[MN_BANK_CUSTODY2].nField[8].x = 181;
	SMenu[MN_BANK_CUSTODY2].nField[8].y = 174;
	SMenu[MN_BANK_CUSTODY2].nField[8].nRectImage = 441;
	SMenu[MN_BANK_CUSTODY2].nField[8].nImageNumber = 442;
	SetRect(SMenu[MN_BANK_CUSTODY2].nField[8].rCheakBox, 177, 171, 233, 198);
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���� �޴� �� ���� ���޴� ����
// ���尣
	SMenu[MN_BLACKSMITH].x=378+GABX_SCREEN;//216;
	SMenu[MN_BLACKSMITH].y=24;
	SMenu[MN_BLACKSMITH].nImageNumber=586;
	SMenu[MN_BLACKSMITH].nFieldCount=6;
	SMenu[MN_BLACKSMITH].CheakType=0;

	// Exit Button 
	SMenu[MN_BLACKSMITH].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_BLACKSMITH].nField[0].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_BLACKSMITH].nField[0].x=69;
	SMenu[MN_BLACKSMITH].nField[0].y=246;
	SMenu[MN_BLACKSMITH].nField[0].nRectImage=607;
	SMenu[MN_BLACKSMITH].nField[0].nImageNumber=608;
	SetRect(SMenu[MN_BLACKSMITH].nField[0].rCheakBox,69,246,141,273);

	//���� ���� �޴��� �θ����� nTemp�׸� ������ �޴� ��ȣ�� �Ѱ��ش�.
	// BUY menu call
	SMenu[MN_BLACKSMITH].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_BLACKSMITH].nField[1].nSpecialWillDo=SWD_SELL_BUY;
	SMenu[MN_BLACKSMITH].nField[1].nWillDo = MN_SKILLGUILD_BUY;
	SMenu[MN_BLACKSMITH].nField[1].x=31;
	SMenu[MN_BLACKSMITH].nField[1].y=120;
	SMenu[MN_BLACKSMITH].nField[1].nRectImage=597;
	SMenu[MN_BLACKSMITH].nField[1].nImageNumber=598;
	SetRect(SMenu[MN_BLACKSMITH].nField[1].rCheakBox,31,120,101,143);

	// SELL menu call
	SMenu[MN_BLACKSMITH].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_BLACKSMITH].nField[2].nSpecialWillDo=SWD_SELL_BUY;
	SMenu[MN_BLACKSMITH].nField[2].nWillDo = MN_SKILLGUILD_SELL;
	SMenu[MN_BLACKSMITH].nField[2].x=110;
	SMenu[MN_BLACKSMITH].nField[2].y=120;
	SMenu[MN_BLACKSMITH].nField[2].nRectImage=599;
	SMenu[MN_BLACKSMITH].nField[2].nImageNumber=600;
	SetRect(SMenu[MN_BLACKSMITH].nField[2].rCheakBox,110,120,180,143);

	// Learn Skill menu call
	SMenu[MN_BLACKSMITH].nField[3].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_BLACKSMITH].nField[3].nSpecialWillDo = SWD_SKILLGUILD_LEARN;
	SMenu[MN_BLACKSMITH].nField[3].nWillDo = MN_SKILLGUILD_SKILL;
	SMenu[MN_BLACKSMITH].nField[3].x=31;
	SMenu[MN_BLACKSMITH].nField[3].y=153;
	SMenu[MN_BLACKSMITH].nField[3].nRectImage=601;
	SMenu[MN_BLACKSMITH].nField[3].nImageNumber=602;
	SetRect(SMenu[MN_BLACKSMITH].nField[3].rCheakBox,31,153,181,175);

	// Learn Item menu call
	SMenu[MN_BLACKSMITH].nField[4].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_BLACKSMITH].nField[4].nSpecialWillDo = SWD_SKILLGUILD_LEARN;
	SMenu[MN_BLACKSMITH].nField[4].nWillDo = MN_SKILLGUILD_ITEM;
	SMenu[MN_BLACKSMITH].nField[4].x=31;
	SMenu[MN_BLACKSMITH].nField[4].y=184;
	SMenu[MN_BLACKSMITH].nField[4].nRectImage=603;
	SMenu[MN_BLACKSMITH].nField[4].nImageNumber=604;
	SetRect(SMenu[MN_BLACKSMITH].nField[4].rCheakBox,32,184,181,206);

	// Repair menu call
	SMenu[MN_BLACKSMITH].nField[5].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_BLACKSMITH].nField[5].nSpecialWillDo=SWD_SELL_BUY;
	SMenu[MN_BLACKSMITH].nField[5].nWillDo = MN_SKILLGUILD_REPAIR;
	SMenu[MN_BLACKSMITH].nField[5].x=31;
	SMenu[MN_BLACKSMITH].nField[5].y=214;
	SMenu[MN_BLACKSMITH].nField[5].nRectImage=605;
	SMenu[MN_BLACKSMITH].nField[5].nImageNumber=606;
	SetRect(SMenu[MN_BLACKSMITH].nField[5].rCheakBox,32,214,181,236);

/////////////////////////////////////////////////////
// ����
	SMenu[MN_FARM].x=378+GABX_SCREEN;
	SMenu[MN_FARM].y=24;
	SMenu[MN_FARM].nImageNumber=10032;
	SMenu[MN_FARM].nFieldCount=6;
	SMenu[MN_FARM].CheakType=0;

	// Exit Button 
	SMenu[MN_FARM].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_FARM].nField[0].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_FARM].nField[0].x=69;
	SMenu[MN_FARM].nField[0].y=246;
	SMenu[MN_FARM].nField[0].nRectImage=607;
	SMenu[MN_FARM].nField[0].nImageNumber=608;
	SetRect(SMenu[MN_FARM].nField[0].rCheakBox,69,246,141,273);

	//���� ���� �޴��� �θ����� nTemp�׸� ������ �޴� ��ȣ�� �Ѱ��ش�.
	// BUY menu call
	SMenu[MN_FARM].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_FARM].nField[1].nSpecialWillDo=SWD_SELL_BUY;
	SMenu[MN_FARM].nField[1].nWillDo = MN_SKILLGUILD_BUY;
	SMenu[MN_FARM].nField[1].x=31;
	SMenu[MN_FARM].nField[1].y=134;
	SMenu[MN_FARM].nField[1].nRectImage=597;
	SMenu[MN_FARM].nField[1].nImageNumber=598;
	SetRect(SMenu[MN_FARM].nField[1].rCheakBox,31,134,101,157);

	// SELL menu call
	SMenu[MN_FARM].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_FARM].nField[2].nSpecialWillDo=SWD_SELL_BUY;
	SMenu[MN_FARM].nField[2].nWillDo = MN_SKILLGUILD_SELL;
	SMenu[MN_FARM].nField[2].x=110;
	SMenu[MN_FARM].nField[2].y=134;
	SMenu[MN_FARM].nField[2].nRectImage=599;
	SMenu[MN_FARM].nField[2].nImageNumber=600;
	SetRect(SMenu[MN_FARM].nField[2].rCheakBox,110,134,180,157);

	// Learn Skill menu call
	SMenu[MN_FARM].nField[3].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_FARM].nField[3].nSpecialWillDo = SWD_SKILLGUILD_LEARN;
	SMenu[MN_FARM].nField[3].nWillDo = MN_SKILLGUILD_SKILL;
	SMenu[MN_FARM].nField[3].x=31;
	SMenu[MN_FARM].nField[3].y=167;
	SMenu[MN_FARM].nField[3].nRectImage=601;
	SMenu[MN_FARM].nField[3].nImageNumber=602;
	SetRect(SMenu[MN_FARM].nField[3].rCheakBox,31,167,180,189);

	// Learn Item menu call
	SMenu[MN_FARM].nField[4].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_FARM].nField[4].nSpecialWillDo = SWD_SKILLGUILD_LEARN;
	SMenu[MN_FARM].nField[4].nWillDo = MN_SKILLGUILD_ITEM;
	SMenu[MN_FARM].nField[4].x=31;
	SMenu[MN_FARM].nField[4].y=198;
	SMenu[MN_FARM].nField[4].nRectImage=603;
	SMenu[MN_FARM].nField[4].nImageNumber=604;
	SetRect(SMenu[MN_FARM].nField[4].rCheakBox,31,198,180,220);

	// ���� �̸� �� ���̱�
	SMenu[MN_FARM].nField[5].nType = FT_NOMAL_PUT;
	SMenu[MN_FARM].nField[5].x = 23;
	SMenu[MN_FARM].nField[5].y = 44;
	SMenu[MN_FARM].nField[5].nImageNumber = 2;
	SMenu[MN_FARM].nField[5].nImageType = SMALL_ETC;

/////////////////////////////////////////////////////
// ������
	SMenu[MN_RESTAURANT].x=378+GABX_SCREEN;
	SMenu[MN_RESTAURANT].y=24;
	SMenu[MN_RESTAURANT].nImageNumber=10033;
	SMenu[MN_RESTAURANT].nFieldCount=6;
	SMenu[MN_RESTAURANT].CheakType=0;

	// Exit Button 
	SMenu[MN_RESTAURANT].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_RESTAURANT].nField[0].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_RESTAURANT].nField[0].x=69;
	SMenu[MN_RESTAURANT].nField[0].y=246;
	SMenu[MN_RESTAURANT].nField[0].nRectImage=607;
	SMenu[MN_RESTAURANT].nField[0].nImageNumber=608;
	SetRect(SMenu[MN_RESTAURANT].nField[0].rCheakBox,69,246,141,273);

	//���� ���� �޴��� �θ����� nTemp�׸� ������ �޴� ��ȣ�� �Ѱ��ش�.
	// BUY menu call
	SMenu[MN_RESTAURANT].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_RESTAURANT].nField[1].nSpecialWillDo=SWD_SELL_BUY;
	SMenu[MN_RESTAURANT].nField[1].nWillDo = MN_SKILLGUILD_BUY;
	SMenu[MN_RESTAURANT].nField[1].x=31;
	SMenu[MN_RESTAURANT].nField[1].y=134;
	SMenu[MN_RESTAURANT].nField[1].nRectImage=597;
	SMenu[MN_RESTAURANT].nField[1].nImageNumber=598;
	SetRect(SMenu[MN_RESTAURANT].nField[1].rCheakBox,31,134,101,157);

	// SELL menu call
	SMenu[MN_RESTAURANT].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_RESTAURANT].nField[2].nSpecialWillDo=SWD_SELL_BUY;
	SMenu[MN_RESTAURANT].nField[2].nWillDo = MN_SKILLGUILD_SELL;
	SMenu[MN_RESTAURANT].nField[2].x=110;
	SMenu[MN_RESTAURANT].nField[2].y=134;
	SMenu[MN_RESTAURANT].nField[2].nRectImage=599;
	SMenu[MN_RESTAURANT].nField[2].nImageNumber=600;
	SetRect(SMenu[MN_RESTAURANT].nField[2].rCheakBox,110,134,180,157);

	// Learn Skill menu call
	SMenu[MN_RESTAURANT].nField[3].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_RESTAURANT].nField[3].nSpecialWillDo = SWD_SKILLGUILD_LEARN;
	SMenu[MN_RESTAURANT].nField[3].nWillDo = MN_SKILLGUILD_SKILL;
	SMenu[MN_RESTAURANT].nField[3].x=31;
	SMenu[MN_RESTAURANT].nField[3].y=167;
	SMenu[MN_RESTAURANT].nField[3].nRectImage=601;
	SMenu[MN_RESTAURANT].nField[3].nImageNumber=602;
	SetRect(SMenu[MN_RESTAURANT].nField[3].rCheakBox,31,167,180,189);

	// Learn Item menu call
	SMenu[MN_RESTAURANT].nField[4].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_RESTAURANT].nField[4].nSpecialWillDo = SWD_SKILLGUILD_LEARN;
	SMenu[MN_RESTAURANT].nField[4].nWillDo = MN_SKILLGUILD_ITEM;
	SMenu[MN_RESTAURANT].nField[4].x=31;
	SMenu[MN_RESTAURANT].nField[4].y=198;
	SMenu[MN_RESTAURANT].nField[4].nRectImage=603;
	SMenu[MN_RESTAURANT].nField[4].nImageNumber=604;
	SetRect(SMenu[MN_RESTAURANT].nField[4].rCheakBox,31,198,180,220);

/*
	// Repair menu call
	SMenu[MN_RESTAURANT].nField[5].nType=FT_HIDE_WILLDO_AUTO_PUT;
	SMenu[MN_RESTAURANT].nField[5].nWillDo = MN_SKILLGUILD_REPAIR;
	SMenu[MN_RESTAURANT].nField[5].x=38;
	SMenu[MN_RESTAURANT].nField[5].y=238;
	SMenu[MN_RESTAURANT].nField[5].nRectImage=605;
	SMenu[MN_RESTAURANT].nField[5].nImageNumber=606;
	SetRect(SMenu[MN_RESTAURANT].nField[5].rCheakBox,34,236,137,261);
*/

/////////////////////////////////////////////////////
// ������
	SMenu[MN_HERBSHOP].x=378+GABX_SCREEN;
	SMenu[MN_HERBSHOP].y=24;
	SMenu[MN_HERBSHOP].nImageNumber=10034;
	SMenu[MN_HERBSHOP].nFieldCount=6;
	SMenu[MN_HERBSHOP].CheakType=0;

	// Exit Button 
	SMenu[MN_HERBSHOP].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_HERBSHOP].nField[0].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_HERBSHOP].nField[0].x=69;
	SMenu[MN_HERBSHOP].nField[0].y=246;
	SMenu[MN_HERBSHOP].nField[0].nRectImage=607;
	SMenu[MN_HERBSHOP].nField[0].nImageNumber=608;
	SetRect(SMenu[MN_HERBSHOP].nField[0].rCheakBox,69,246,141,273);

	//���� ���� �޴��� �θ����� nTemp�׸� ������ �޴� ��ȣ�� �Ѱ��ش�.
	// BUY menu call
	SMenu[MN_HERBSHOP].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_HERBSHOP].nField[1].nSpecialWillDo=SWD_SELL_BUY;
	SMenu[MN_HERBSHOP].nField[1].nWillDo = MN_SKILLGUILD_BUY;
	SMenu[MN_HERBSHOP].nField[1].x=31;
	SMenu[MN_HERBSHOP].nField[1].y=134;
	SMenu[MN_HERBSHOP].nField[1].nRectImage=597;
	SMenu[MN_HERBSHOP].nField[1].nImageNumber=598;
	SetRect(SMenu[MN_HERBSHOP].nField[1].rCheakBox,31,134,101,157);

	// SELL menu call
	SMenu[MN_HERBSHOP].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_HERBSHOP].nField[2].nSpecialWillDo=SWD_SELL_BUY;
	SMenu[MN_HERBSHOP].nField[2].nWillDo = MN_SKILLGUILD_SELL;
	SMenu[MN_HERBSHOP].nField[2].x=110;
	SMenu[MN_HERBSHOP].nField[2].y=134;
	SMenu[MN_HERBSHOP].nField[2].nRectImage=599;
	SMenu[MN_HERBSHOP].nField[2].nImageNumber=600;
	SetRect(SMenu[MN_HERBSHOP].nField[2].rCheakBox,110,134,180,157);

	// Learn Skill menu call
	SMenu[MN_HERBSHOP].nField[3].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_HERBSHOP].nField[3].nSpecialWillDo = SWD_SKILLGUILD_LEARN;
	SMenu[MN_HERBSHOP].nField[3].nWillDo = MN_SKILLGUILD_SKILL;
	SMenu[MN_HERBSHOP].nField[3].x=31;
	SMenu[MN_HERBSHOP].nField[3].y=167;
	SMenu[MN_HERBSHOP].nField[3].nRectImage=601;
	SMenu[MN_HERBSHOP].nField[3].nImageNumber=602;
	SetRect(SMenu[MN_HERBSHOP].nField[3].rCheakBox,31,167,180,189);

	// Learn Item menu call
	SMenu[MN_HERBSHOP].nField[4].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_HERBSHOP].nField[4].nSpecialWillDo = SWD_SKILLGUILD_LEARN;
	SMenu[MN_HERBSHOP].nField[4].nWillDo = MN_SKILLGUILD_ITEM;
	SMenu[MN_HERBSHOP].nField[4].x=31;
	SMenu[MN_HERBSHOP].nField[4].y=198;
	SMenu[MN_HERBSHOP].nField[4].nRectImage=603;
	SMenu[MN_HERBSHOP].nField[4].nImageNumber=604;
	SetRect(SMenu[MN_HERBSHOP].nField[4].rCheakBox,31,198,180,220);

/*
	// Repair menu call
	SMenu[MN_HERBSHOP].nField[5].nType=FT_HIDE_WILLDO_AUTO_PUT;
	SMenu[MN_HERBSHOP].nField[5].nWillDo = MN_SKILLGUILD_REPAIR;
	SMenu[MN_HERBSHOP].nField[5].x=38;
	SMenu[MN_HERBSHOP].nField[5].y=238;
	SMenu[MN_HERBSHOP].nField[5].nRectImage=605;
	SMenu[MN_HERBSHOP].nField[5].nImageNumber=606;
	SetRect(SMenu[MN_HERBSHOP].nField[5].rCheakBox,34,236,137,261);
*/

/////////////////////////////////////////////////////
// Ǫ�ְ�
	SMenu[MN_SHAMBLES].x=378+GABX_SCREEN;
	SMenu[MN_SHAMBLES].y=24;
	SMenu[MN_SHAMBLES].nImageNumber=10035;
	SMenu[MN_SHAMBLES].nFieldCount=6;
	SMenu[MN_SHAMBLES].CheakType=0;

	// Exit Button 
	SMenu[MN_SHAMBLES].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_SHAMBLES].nField[0].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_SHAMBLES].nField[0].x=69;
	SMenu[MN_SHAMBLES].nField[0].y=246;
	SMenu[MN_SHAMBLES].nField[0].nRectImage=607;
	SMenu[MN_SHAMBLES].nField[0].nImageNumber=608;
	SetRect(SMenu[MN_SHAMBLES].nField[0].rCheakBox,69,246,141,273);

	//���� ���� �޴��� �θ����� nTemp�׸� ������ �޴� ��ȣ�� �Ѱ��ش�.
	// BUY menu call
	SMenu[MN_SHAMBLES].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_SHAMBLES].nField[1].nSpecialWillDo=SWD_SELL_BUY;
	SMenu[MN_SHAMBLES].nField[1].nWillDo = MN_SKILLGUILD_BUY;
	SMenu[MN_SHAMBLES].nField[1].x=31;
	SMenu[MN_SHAMBLES].nField[1].y=134;
	SMenu[MN_SHAMBLES].nField[1].nRectImage=597;
	SMenu[MN_SHAMBLES].nField[1].nImageNumber=598;
	SetRect(SMenu[MN_SHAMBLES].nField[1].rCheakBox,31,134,101,157);

	// SELL menu call
	SMenu[MN_SHAMBLES].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_SHAMBLES].nField[2].nSpecialWillDo=SWD_SELL_BUY;
	SMenu[MN_SHAMBLES].nField[2].nWillDo = MN_SKILLGUILD_SELL;
	SMenu[MN_SHAMBLES].nField[2].x=110;
	SMenu[MN_SHAMBLES].nField[2].y=134;
	SMenu[MN_SHAMBLES].nField[2].nRectImage=599;
	SMenu[MN_SHAMBLES].nField[2].nImageNumber=600;
	SetRect(SMenu[MN_SHAMBLES].nField[2].rCheakBox,110,134,180,157);

	// Learn Skill menu call
	SMenu[MN_SHAMBLES].nField[3].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_SHAMBLES].nField[3].nSpecialWillDo = SWD_SKILLGUILD_LEARN;
	SMenu[MN_SHAMBLES].nField[3].nWillDo = MN_SKILLGUILD_SKILL;
	SMenu[MN_SHAMBLES].nField[3].x=31;
	SMenu[MN_SHAMBLES].nField[3].y=167;
	SMenu[MN_SHAMBLES].nField[3].nRectImage=601;
	SMenu[MN_SHAMBLES].nField[3].nImageNumber=602;
	SetRect(SMenu[MN_SHAMBLES].nField[3].rCheakBox,31,167,180,189);

	// Learn Item menu call
	SMenu[MN_SHAMBLES].nField[4].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_SHAMBLES].nField[4].nSpecialWillDo = SWD_SKILLGUILD_LEARN;
	SMenu[MN_SHAMBLES].nField[4].nWillDo = MN_SKILLGUILD_ITEM;
	SMenu[MN_SHAMBLES].nField[4].x=31;
	SMenu[MN_SHAMBLES].nField[4].y=198;
	SMenu[MN_SHAMBLES].nField[4].nRectImage=603;
	SMenu[MN_SHAMBLES].nField[4].nImageNumber=604;
	SetRect(SMenu[MN_SHAMBLES].nField[4].rCheakBox,31,198,180,220);

/*
	// Repair menu call
	SMenu[MN_SHAMBLES].nField[5].nType=FT_HIDE_WILLDO_AUTO_PUT;
	SMenu[MN_SHAMBLES].nField[5].nWillDo = MN_SKILLGUILD_REPAIR;
	SMenu[MN_SHAMBLES].nField[5].x=38;
	SMenu[MN_SHAMBLES].nField[5].y=238;
	SMenu[MN_SHAMBLES].nField[5].nRectImage=605;
	SMenu[MN_SHAMBLES].nField[5].nImageNumber=606;
	SetRect(SMenu[MN_SHAMBLES].nField[5].rCheakBox,34,236,137,261);
*/

/////////////////////////////////////////////////////
// ������
	SMenu[MN_CANDLE].x=378+GABX_SCREEN;
	SMenu[MN_CANDLE].y=24;
	SMenu[MN_CANDLE].nImageNumber=10036;
	SMenu[MN_CANDLE].nFieldCount=6;
	SMenu[MN_CANDLE].CheakType=0;

	// Exit Button 
	SMenu[MN_CANDLE].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_CANDLE].nField[0].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_CANDLE].nField[0].x=69;
	SMenu[MN_CANDLE].nField[0].y=246;
	SMenu[MN_CANDLE].nField[0].nRectImage=607;
	SMenu[MN_CANDLE].nField[0].nImageNumber=608;
	SetRect(SMenu[MN_CANDLE].nField[0].rCheakBox,69,246,141,273);

	//���� ���� �޴��� �θ����� nTemp�׸� ������ �޴� ��ȣ�� �Ѱ��ش�.
	// BUY menu call
	SMenu[MN_CANDLE].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_CANDLE].nField[1].nSpecialWillDo=SWD_SELL_BUY;
	SMenu[MN_CANDLE].nField[1].nWillDo = MN_SKILLGUILD_BUY;
	SMenu[MN_CANDLE].nField[1].x=31;
	SMenu[MN_CANDLE].nField[1].y=134;
	SMenu[MN_CANDLE].nField[1].nRectImage=597;
	SMenu[MN_CANDLE].nField[1].nImageNumber=598;
	SetRect(SMenu[MN_CANDLE].nField[1].rCheakBox,31,134,101,157);

	// SELL menu call
	SMenu[MN_CANDLE].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_CANDLE].nField[2].nSpecialWillDo=SWD_SELL_BUY;
	SMenu[MN_CANDLE].nField[2].nWillDo = MN_SKILLGUILD_SELL;
	SMenu[MN_CANDLE].nField[2].x=110;
	SMenu[MN_CANDLE].nField[2].y=134;
	SMenu[MN_CANDLE].nField[2].nRectImage=599;
	SMenu[MN_CANDLE].nField[2].nImageNumber=600;
	SetRect(SMenu[MN_CANDLE].nField[2].rCheakBox,110,134,180,157);

	// Learn Skill menu call
	SMenu[MN_CANDLE].nField[3].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_CANDLE].nField[3].nSpecialWillDo = SWD_SKILLGUILD_LEARN;
	SMenu[MN_CANDLE].nField[3].nWillDo = MN_SKILLGUILD_SKILL;
	SMenu[MN_CANDLE].nField[3].x=31;
	SMenu[MN_CANDLE].nField[3].y=167;
	SMenu[MN_CANDLE].nField[3].nRectImage=601;
	SMenu[MN_CANDLE].nField[3].nImageNumber=602;
	SetRect(SMenu[MN_CANDLE].nField[3].rCheakBox,31,167,180,189);

	// Learn Item menu call
	SMenu[MN_CANDLE].nField[4].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_CANDLE].nField[4].nSpecialWillDo = SWD_SKILLGUILD_LEARN;
	SMenu[MN_CANDLE].nField[4].nWillDo = MN_SKILLGUILD_ITEM;
	SMenu[MN_CANDLE].nField[4].x=31;
	SMenu[MN_CANDLE].nField[4].y=198;
	SMenu[MN_CANDLE].nField[4].nRectImage=603;
	SMenu[MN_CANDLE].nField[4].nImageNumber=604;
	SetRect(SMenu[MN_CANDLE].nField[4].rCheakBox,31,198,180,220);

/*	
	// Repair menu call
	SMenu[MN_CANDLE].nField[5].nType=FT_HIDE_WILLDO_AUTO_PUT;
	SMenu[MN_CANDLE].nField[5].nWillDo = MN_SKILLGUILD_REPAIR;
	SMenu[MN_CANDLE].nField[5].x=38;
	SMenu[MN_CANDLE].nField[5].y=238;
	SMenu[MN_CANDLE].nField[5].nRectImage=605;
	SMenu[MN_CANDLE].nField[5].nImageNumber=606;
	SetRect(SMenu[MN_CANDLE].nField[5].rCheakBox,34,236,137,261);
*/

/////////////////////////////////////////////////////
// �ʰ���
	SMenu[MN_SLOPSHOP].x=378+GABX_SCREEN;
	SMenu[MN_SLOPSHOP].y=24;
	SMenu[MN_SLOPSHOP].nImageNumber=592;
	SMenu[MN_SLOPSHOP].nFieldCount=6;
	SMenu[MN_SLOPSHOP].CheakType=0;

	// Exit Button 
	SMenu[MN_SLOPSHOP].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_SLOPSHOP].nField[0].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_SLOPSHOP].nField[0].x=69;
	SMenu[MN_SLOPSHOP].nField[0].y=246;
	SMenu[MN_SLOPSHOP].nField[0].nRectImage=607;
	SMenu[MN_SLOPSHOP].nField[0].nImageNumber=608;
	SetRect(SMenu[MN_SLOPSHOP].nField[0].rCheakBox,69,246,141,273);

	//���� ���� �޴��� �θ����� nTemp�׸� ������ �޴� ��ȣ�� �Ѱ��ش�.
	// BUY menu call
	SMenu[MN_SLOPSHOP].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_SLOPSHOP].nField[1].nSpecialWillDo=SWD_SELL_BUY;
	SMenu[MN_SLOPSHOP].nField[1].nWillDo = MN_SKILLGUILD_BUY;
	SMenu[MN_SLOPSHOP].nField[1].x=31;
	SMenu[MN_SLOPSHOP].nField[1].y=120;
	SMenu[MN_SLOPSHOP].nField[1].nRectImage=597;
	SMenu[MN_SLOPSHOP].nField[1].nImageNumber=598;
	SetRect(SMenu[MN_SLOPSHOP].nField[1].rCheakBox,31,120,101,143);

	// SELL menu call
	SMenu[MN_SLOPSHOP].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_SLOPSHOP].nField[2].nSpecialWillDo=SWD_SELL_BUY;
	SMenu[MN_SLOPSHOP].nField[2].nWillDo = MN_SKILLGUILD_SELL;
	SMenu[MN_SLOPSHOP].nField[2].x=110;
	SMenu[MN_SLOPSHOP].nField[2].y=120;
	SMenu[MN_SLOPSHOP].nField[2].nRectImage=599;
	SMenu[MN_SLOPSHOP].nField[2].nImageNumber=600;
	SetRect(SMenu[MN_SLOPSHOP].nField[2].rCheakBox,110,120,180,143);

	// Learn Skill menu call
	SMenu[MN_SLOPSHOP].nField[3].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_SLOPSHOP].nField[3].nSpecialWillDo = SWD_SKILLGUILD_LEARN;
	SMenu[MN_SLOPSHOP].nField[3].nWillDo = MN_SKILLGUILD_SKILL;
	SMenu[MN_SLOPSHOP].nField[3].x=31;
	SMenu[MN_SLOPSHOP].nField[3].y=153;
	SMenu[MN_SLOPSHOP].nField[3].nRectImage=601;
	SMenu[MN_SLOPSHOP].nField[3].nImageNumber=602;
	SetRect(SMenu[MN_SLOPSHOP].nField[3].rCheakBox,32,153,181,175);

	// Learn Item menu call
	SMenu[MN_SLOPSHOP].nField[4].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_SLOPSHOP].nField[4].nSpecialWillDo = SWD_SKILLGUILD_LEARN;
	SMenu[MN_SLOPSHOP].nField[4].nWillDo = MN_SKILLGUILD_ITEM;
	SMenu[MN_SLOPSHOP].nField[4].x=31;
	SMenu[MN_SLOPSHOP].nField[4].y=184;
	SMenu[MN_SLOPSHOP].nField[4].nRectImage=603;
	SMenu[MN_SLOPSHOP].nField[4].nImageNumber=604;
	SetRect(SMenu[MN_SLOPSHOP].nField[4].rCheakBox,32,184,181,206);

	// Repair menu call
	SMenu[MN_SLOPSHOP].nField[5].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_SLOPSHOP].nField[5].nSpecialWillDo=SWD_SELL_BUY;
	SMenu[MN_SLOPSHOP].nField[5].nWillDo = MN_SKILLGUILD_REPAIR;
	SMenu[MN_SLOPSHOP].nField[5].x=31;
	SMenu[MN_SLOPSHOP].nField[5].y=214;
	SMenu[MN_SLOPSHOP].nField[5].nRectImage=605;
	SMenu[MN_SLOPSHOP].nField[5].nImageNumber=606;
	SetRect(SMenu[MN_SLOPSHOP].nField[5].rCheakBox,32,214,181,236);

/////////////////////////////////////////////////////
// ���ݼ���
	SMenu[MN_ALCHEMY].x=378+GABX_SCREEN;
	SMenu[MN_ALCHEMY].y=24;
	SMenu[MN_ALCHEMY].nImageNumber=10037;
	SMenu[MN_ALCHEMY].nFieldCount=6;
	SMenu[MN_ALCHEMY].CheakType=0;

	// Exit Button 
	SMenu[MN_ALCHEMY].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_ALCHEMY].nField[0].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_ALCHEMY].nField[0].x=69;
	SMenu[MN_ALCHEMY].nField[0].y=246;
	SMenu[MN_ALCHEMY].nField[0].nRectImage=607;
	SMenu[MN_ALCHEMY].nField[0].nImageNumber=608;
	SetRect(SMenu[MN_ALCHEMY].nField[0].rCheakBox,69,246,141,273);

	//���� ���� �޴��� �θ����� nTemp�׸� ������ �޴� ��ȣ�� �Ѱ��ش�.
	// BUY menu call
	SMenu[MN_ALCHEMY].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_ALCHEMY].nField[1].nSpecialWillDo=SWD_SELL_BUY;
	SMenu[MN_ALCHEMY].nField[1].nWillDo = MN_SKILLGUILD_BUY;
	SMenu[MN_ALCHEMY].nField[1].x=31;
	SMenu[MN_ALCHEMY].nField[1].y=134;
	SMenu[MN_ALCHEMY].nField[1].nRectImage=597;
	SMenu[MN_ALCHEMY].nField[1].nImageNumber=598;
	SetRect(SMenu[MN_ALCHEMY].nField[1].rCheakBox,31,134,101,157);

	// SELL menu call
	SMenu[MN_ALCHEMY].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_ALCHEMY].nField[2].nSpecialWillDo=SWD_SELL_BUY;
	SMenu[MN_ALCHEMY].nField[2].nWillDo = MN_SKILLGUILD_SELL;
	SMenu[MN_ALCHEMY].nField[2].x=110;
	SMenu[MN_ALCHEMY].nField[2].y=134;
	SMenu[MN_ALCHEMY].nField[2].nRectImage=599;
	SMenu[MN_ALCHEMY].nField[2].nImageNumber=600;
	SetRect(SMenu[MN_ALCHEMY].nField[2].rCheakBox,110,134,180,157);

	// Learn Skill menu call
	SMenu[MN_ALCHEMY].nField[3].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_ALCHEMY].nField[3].nSpecialWillDo = SWD_SKILLGUILD_LEARN;
	SMenu[MN_ALCHEMY].nField[3].nWillDo = MN_SKILLGUILD_SKILL;
	SMenu[MN_ALCHEMY].nField[3].x=31;
	SMenu[MN_ALCHEMY].nField[3].y=167;
	SMenu[MN_ALCHEMY].nField[3].nRectImage=601;
	SMenu[MN_ALCHEMY].nField[3].nImageNumber=602;
	SetRect(SMenu[MN_ALCHEMY].nField[3].rCheakBox,31,167,180,189);

	// Learn Item menu call
	SMenu[MN_ALCHEMY].nField[4].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_ALCHEMY].nField[4].nSpecialWillDo = SWD_SKILLGUILD_LEARN;
	SMenu[MN_ALCHEMY].nField[4].nWillDo = MN_SKILLGUILD_ITEM;
	SMenu[MN_ALCHEMY].nField[4].x=31;
	SMenu[MN_ALCHEMY].nField[4].y=198;
	SMenu[MN_ALCHEMY].nField[4].nRectImage=603;
	SMenu[MN_ALCHEMY].nField[4].nImageNumber=604;
	SetRect(SMenu[MN_ALCHEMY].nField[4].rCheakBox,31,198,180,220);

/*
	// Repair menu call
	SMenu[MN_ALCHEMY].nField[5].nType=FT_HIDE_WILLDO_AUTO_PUT;
	SMenu[MN_ALCHEMY].nField[5].nWillDo = MN_SKILLGUILD_REPAIR;
	SMenu[MN_ALCHEMY].nField[5].x=38;
	SMenu[MN_ALCHEMY].nField[5].y=238;
	SMenu[MN_ALCHEMY].nField[5].nRectImage=605;
	SMenu[MN_ALCHEMY].nField[5].nImageNumber=606;
	SetRect(SMenu[MN_ALCHEMY].nField[5].rCheakBox,34,236,137,261);
*/

/////////////////////////////////////////////////////
// �����
	SMenu[MN_WOODWORKING].x=378+GABX_SCREEN;
	SMenu[MN_WOODWORKING].y=24;
	SMenu[MN_WOODWORKING].nImageNumber=594;
	SMenu[MN_WOODWORKING].nFieldCount=6;
	SMenu[MN_WOODWORKING].CheakType=0;

	// Exit Button 
	SMenu[MN_WOODWORKING].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_WOODWORKING].nField[0].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_WOODWORKING].nField[0].x=69;
	SMenu[MN_WOODWORKING].nField[0].y=246;
	SMenu[MN_WOODWORKING].nField[0].nRectImage=607;
	SMenu[MN_WOODWORKING].nField[0].nImageNumber=608;
	SetRect(SMenu[MN_WOODWORKING].nField[0].rCheakBox,69,246,141,273);

	//���� ���� �޴��� �θ����� nTemp�׸� ������ �޴� ��ȣ�� �Ѱ��ش�.
	// BUY menu call
	SMenu[MN_WOODWORKING].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_WOODWORKING].nField[1].nSpecialWillDo=SWD_SELL_BUY;
	SMenu[MN_WOODWORKING].nField[1].nWillDo = MN_SKILLGUILD_BUY;
	SMenu[MN_WOODWORKING].nField[1].x=31;
	SMenu[MN_WOODWORKING].nField[1].y=120;
	SMenu[MN_WOODWORKING].nField[1].nRectImage=597;
	SMenu[MN_WOODWORKING].nField[1].nImageNumber=598;
	SetRect(SMenu[MN_WOODWORKING].nField[1].rCheakBox,31,120,101,143);

	// SELL menu call
	SMenu[MN_WOODWORKING].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_WOODWORKING].nField[2].nSpecialWillDo=SWD_SELL_BUY;
	SMenu[MN_WOODWORKING].nField[2].nWillDo = MN_SKILLGUILD_SELL;
	SMenu[MN_WOODWORKING].nField[2].x=110;
	SMenu[MN_WOODWORKING].nField[2].y=120;
	SMenu[MN_WOODWORKING].nField[2].nRectImage=599;
	SMenu[MN_WOODWORKING].nField[2].nImageNumber=600;
	SetRect(SMenu[MN_WOODWORKING].nField[2].rCheakBox,110,120,180,143);

	// Learn Skill menu call
	SMenu[MN_WOODWORKING].nField[3].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_WOODWORKING].nField[3].nSpecialWillDo = SWD_SKILLGUILD_LEARN;
	SMenu[MN_WOODWORKING].nField[3].nWillDo = MN_SKILLGUILD_SKILL;
	SMenu[MN_WOODWORKING].nField[3].x=31;
	SMenu[MN_WOODWORKING].nField[3].y=153;
	SMenu[MN_WOODWORKING].nField[3].nRectImage=601;
	SMenu[MN_WOODWORKING].nField[3].nImageNumber=602;
	SetRect(SMenu[MN_WOODWORKING].nField[3].rCheakBox,32,153,181,175);

	// Learn Item menu call
	SMenu[MN_WOODWORKING].nField[4].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_WOODWORKING].nField[4].nSpecialWillDo = SWD_SKILLGUILD_LEARN;
	SMenu[MN_WOODWORKING].nField[4].nWillDo = MN_SKILLGUILD_ITEM;
	SMenu[MN_WOODWORKING].nField[4].x=31;
	SMenu[MN_WOODWORKING].nField[4].y=184;
	SMenu[MN_WOODWORKING].nField[4].nRectImage=603;
	SMenu[MN_WOODWORKING].nField[4].nImageNumber=604;
	SetRect(SMenu[MN_WOODWORKING].nField[4].rCheakBox,32,184,181,206);

	// Repair menu call
	SMenu[MN_WOODWORKING].nField[5].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_WOODWORKING].nField[5].nSpecialWillDo=SWD_SELL_BUY;
	SMenu[MN_WOODWORKING].nField[5].nWillDo = MN_SKILLGUILD_REPAIR;
	SMenu[MN_WOODWORKING].nField[5].x=31;
	SMenu[MN_WOODWORKING].nField[5].y=214;
	SMenu[MN_WOODWORKING].nField[5].nRectImage=605;
	SMenu[MN_WOODWORKING].nField[5].nImageNumber=606;
	SetRect(SMenu[MN_WOODWORKING].nField[5].rCheakBox, 32,214,181,236);

/////////////////////////////////////////////////////
// BowCraft
	SMenu[MN_BOWCRAFT].x=378+GABX_SCREEN;
	SMenu[MN_BOWCRAFT].y=24;
	SMenu[MN_BOWCRAFT].nImageNumber=595;
	SMenu[MN_BOWCRAFT].nFieldCount=6;
	SMenu[MN_BOWCRAFT].CheakType=0;

	// Exit Button 
	SMenu[MN_BOWCRAFT].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_BOWCRAFT].nField[0].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_BOWCRAFT].nField[0].x=69;
	SMenu[MN_BOWCRAFT].nField[0].y=246;
	SMenu[MN_BOWCRAFT].nField[0].nRectImage=607;
	SMenu[MN_BOWCRAFT].nField[0].nImageNumber=608;
	SetRect(SMenu[MN_BOWCRAFT].nField[0].rCheakBox,69,246,141,273);

	//���� ���� �޴��� �θ����� nTemp�׸� ������ �޴� ��ȣ�� �Ѱ��ش�.
	// BUY menu call
	SMenu[MN_BOWCRAFT].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_BOWCRAFT].nField[1].nSpecialWillDo=SWD_SELL_BUY;
	SMenu[MN_BOWCRAFT].nField[1].nWillDo = MN_SKILLGUILD_BUY;
	SMenu[MN_BOWCRAFT].nField[1].x=31;
	SMenu[MN_BOWCRAFT].nField[1].y=120;
	SMenu[MN_BOWCRAFT].nField[1].nRectImage=597;
	SMenu[MN_BOWCRAFT].nField[1].nImageNumber=598;
	SetRect(SMenu[MN_BOWCRAFT].nField[1].rCheakBox,31,120,101,143);

	// SELL menu call
	SMenu[MN_BOWCRAFT].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_BOWCRAFT].nField[2].nSpecialWillDo=SWD_SELL_BUY;
	SMenu[MN_BOWCRAFT].nField[2].nWillDo = MN_SKILLGUILD_SELL;
	SMenu[MN_BOWCRAFT].nField[2].x=110;
	SMenu[MN_BOWCRAFT].nField[2].y=120;
	SMenu[MN_BOWCRAFT].nField[2].nRectImage=599;
	SMenu[MN_BOWCRAFT].nField[2].nImageNumber=600;
	SetRect(SMenu[MN_BOWCRAFT].nField[2].rCheakBox,110,120,180,143);

	// Learn Skill menu call
	SMenu[MN_BOWCRAFT].nField[3].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_BOWCRAFT].nField[3].nSpecialWillDo = SWD_SKILLGUILD_LEARN;
	SMenu[MN_BOWCRAFT].nField[3].nWillDo = MN_SKILLGUILD_SKILL;
	SMenu[MN_BOWCRAFT].nField[3].x=31;
	SMenu[MN_BOWCRAFT].nField[3].y=153;
	SMenu[MN_BOWCRAFT].nField[3].nRectImage=601;
	SMenu[MN_BOWCRAFT].nField[3].nImageNumber=602;
	SetRect(SMenu[MN_BOWCRAFT].nField[3].rCheakBox,32,153,181,175);

	// Learn Item menu call
	SMenu[MN_BOWCRAFT].nField[4].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_BOWCRAFT].nField[4].nSpecialWillDo = SWD_SKILLGUILD_LEARN;
	SMenu[MN_BOWCRAFT].nField[4].nWillDo = MN_SKILLGUILD_ITEM;
	SMenu[MN_BOWCRAFT].nField[4].x=31;
	SMenu[MN_BOWCRAFT].nField[4].y=184;
	SMenu[MN_BOWCRAFT].nField[4].nRectImage=603;
	SMenu[MN_BOWCRAFT].nField[4].nImageNumber=604;
	SetRect(SMenu[MN_BOWCRAFT].nField[4].rCheakBox,32,184,181,206);

	// Repair menu call
	SMenu[MN_BOWCRAFT].nField[5].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_BOWCRAFT].nField[5].nSpecialWillDo=SWD_SELL_BUY;
	SMenu[MN_BOWCRAFT].nField[5].nWillDo = MN_SKILLGUILD_REPAIR;
	SMenu[MN_BOWCRAFT].nField[5].x=31;	
	SMenu[MN_BOWCRAFT].nField[5].y=214;
	SMenu[MN_BOWCRAFT].nField[5].nRectImage=605;
	SMenu[MN_BOWCRAFT].nField[5].nImageNumber=606;
	SetRect(SMenu[MN_BOWCRAFT].nField[5].rCheakBox,32,214,181, 236);

/////////////////////////////////////////////////////
// ������
	SMenu[MN_SHIPYARD].x=378+GABX_SCREEN;
	SMenu[MN_SHIPYARD].y=24;
	SMenu[MN_SHIPYARD].nImageNumber=596;
	SMenu[MN_SHIPYARD].nFieldCount=6;
	SMenu[MN_SHIPYARD].CheakType=0;

	// Exit Button 
	SMenu[MN_SHIPYARD].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_SHIPYARD].nField[0].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_SHIPYARD].nField[0].x=69;
	SMenu[MN_SHIPYARD].nField[0].y=246;
	SMenu[MN_SHIPYARD].nField[0].nRectImage=607;
	SMenu[MN_SHIPYARD].nField[0].nImageNumber=608;
	SetRect(SMenu[MN_SHIPYARD].nField[0].rCheakBox,69,246,141,273);

	//���� ���� �޴��� �θ����� nTemp�׸� ������ �޴� ��ȣ�� �Ѱ��ش�.
	// BUY menu call
	SMenu[MN_SHIPYARD].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_SHIPYARD].nField[1].nSpecialWillDo=SWD_SELL_BUY;
	SMenu[MN_SHIPYARD].nField[1].nWillDo = MN_SKILLGUILD_BUY;
	SMenu[MN_SHIPYARD].nField[1].x=31;
	SMenu[MN_SHIPYARD].nField[1].y=120;
	SMenu[MN_SHIPYARD].nField[1].nRectImage=597;
	SMenu[MN_SHIPYARD].nField[1].nImageNumber=598;
	SetRect(SMenu[MN_SHIPYARD].nField[1].rCheakBox,31,120,101,143);

	// SELL menu call
	SMenu[MN_SHIPYARD].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_SHIPYARD].nField[2].nSpecialWillDo=SWD_SELL_BUY;
	SMenu[MN_SHIPYARD].nField[2].nWillDo = MN_SKILLGUILD_SELL;
	SMenu[MN_SHIPYARD].nField[2].x=110;
	SMenu[MN_SHIPYARD].nField[2].y=120;
	SMenu[MN_SHIPYARD].nField[2].nRectImage=599;
	SMenu[MN_SHIPYARD].nField[2].nImageNumber=600;
	SetRect(SMenu[MN_SHIPYARD].nField[2].rCheakBox,110,120,180,143);

	// Learn Skill menu call
	SMenu[MN_SHIPYARD].nField[3].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_SHIPYARD].nField[3].nSpecialWillDo = SWD_SKILLGUILD_LEARN;
	SMenu[MN_SHIPYARD].nField[3].nWillDo = MN_SKILLGUILD_SKILL;
	SMenu[MN_SHIPYARD].nField[3].x=31;
	SMenu[MN_SHIPYARD].nField[3].y=153;
	SMenu[MN_SHIPYARD].nField[3].nRectImage=601;
	SMenu[MN_SHIPYARD].nField[3].nImageNumber=602;
	SetRect(SMenu[MN_SHIPYARD].nField[3].rCheakBox,31,153,181,175);

	// Learn Item menu call
	SMenu[MN_SHIPYARD].nField[4].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_SHIPYARD].nField[4].nSpecialWillDo = SWD_SKILLGUILD_LEARN;
	SMenu[MN_SHIPYARD].nField[4].nWillDo = MN_SKILLGUILD_ITEM;
	SMenu[MN_SHIPYARD].nField[4].x=31;
	SMenu[MN_SHIPYARD].nField[4].y=184;
	SMenu[MN_SHIPYARD].nField[4].nRectImage=603;
	SMenu[MN_SHIPYARD].nField[4].nImageNumber=604;
	SetRect(SMenu[MN_SHIPYARD].nField[4].rCheakBox,31,184,181,206);

	// Repair menu call
	SMenu[MN_SHIPYARD].nField[5].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_SHIPYARD].nField[5].nSpecialWillDo=SWD_SELL_BUY;
	SMenu[MN_SHIPYARD].nField[5].nWillDo = MN_SKILLGUILD_REPAIR;
	SMenu[MN_SHIPYARD].nField[5].x=31;
	SMenu[MN_SHIPYARD].nField[5].y=214;
	SMenu[MN_SHIPYARD].nField[5].nRectImage=605;
	SMenu[MN_SHIPYARD].nField[5].nImageNumber=606;
	SetRect(SMenu[MN_SHIPYARD].nField[5].rCheakBox,31,214,181,236);


//////////////////////////////////////////////////////////////////////////////////////////
// ������ ���
//
	SMenu[MN_MAGIC_GUILD].x=378+GABX_SCREEN;
	SMenu[MN_MAGIC_GUILD].y=24;
	SMenu[MN_MAGIC_GUILD].nImageNumber=491;
	SMenu[MN_MAGIC_GUILD].nFieldCount=3;
	SMenu[MN_MAGIC_GUILD].CheakType=0;

	// Exit Button 
	SMenu[MN_MAGIC_GUILD].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_MAGIC_GUILD].nField[0].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_MAGIC_GUILD].nField[0].x=69;
	SMenu[MN_MAGIC_GUILD].nField[0].y=246;
	SMenu[MN_MAGIC_GUILD].nField[0].nRectImage=607;
	SMenu[MN_MAGIC_GUILD].nField[0].nImageNumber=608;
	SetRect(SMenu[MN_MAGIC_GUILD].nField[0].rCheakBox,69,246,141,273);

	//���� ���� �޴��� �θ����� nTemp�׸� ������ �޴� ��ȣ�� �Ѱ��ش�.
	// BUY menu call
	SMenu[MN_MAGIC_GUILD].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_MAGIC_GUILD].nField[1].nSpecialWillDo=SWD_SELL_BUY;
	SMenu[MN_MAGIC_GUILD].nField[1].nWillDo = MN_SKILLGUILD_BUY;
	SMenu[MN_MAGIC_GUILD].nField[1].x=31;
	SMenu[MN_MAGIC_GUILD].nField[1].y=138;
	SMenu[MN_MAGIC_GUILD].nField[1].nRectImage=518;
	SMenu[MN_MAGIC_GUILD].nField[1].nImageNumber=519;
	SetRect(SMenu[MN_MAGIC_GUILD].nField[1].rCheakBox,31,138,180,160);

	// Learn magic menu call
	SMenu[MN_MAGIC_GUILD].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_MAGIC_GUILD].nField[2].nSpecialWillDo = SWD_SMALL_MENU;
	SMenu[MN_MAGIC_GUILD].nField[2].nWillDo = MN_LEARN_MAGIC_BOOK;
	SMenu[MN_MAGIC_GUILD].nField[2].x=31;
	SMenu[MN_MAGIC_GUILD].nField[2].y=199;
	SMenu[MN_MAGIC_GUILD].nField[2].nRectImage=539;
	SMenu[MN_MAGIC_GUILD].nField[2].nImageNumber=540;
	SetRect(SMenu[MN_MAGIC_GUILD].nField[2].rCheakBox,31,199,180,221);

//////////////////////////////////////////////////////////////////////////////////////////
// ��ī����
//
	SMenu[MN_ACADEMY].x=378+GABX_SCREEN;
	SMenu[MN_ACADEMY].y=24;
	SMenu[MN_ACADEMY].nImageNumber=490;
	SMenu[MN_ACADEMY].nFieldCount=3;
	SMenu[MN_ACADEMY].CheakType=0;

	// Exit Button 
	SMenu[MN_ACADEMY].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_ACADEMY].nField[0].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_ACADEMY].nField[0].x=69;
	SMenu[MN_ACADEMY].nField[0].y=246;
	SMenu[MN_ACADEMY].nField[0].nRectImage=607;
	SMenu[MN_ACADEMY].nField[0].nImageNumber=608;
	SetRect(SMenu[MN_ACADEMY].nField[0].rCheakBox,69,246,141,273);

	//���� ���� �޴��� �θ����� nTemp�׸� ������ �޴� ��ȣ�� �Ѱ��ش�.
	// Learn Skill
	SMenu[MN_ACADEMY].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_ACADEMY].nField[1].nSpecialWillDo=SWD_SKILLGUILD_LEARN;
	SMenu[MN_ACADEMY].nField[1].nWillDo = MN_SKILLGUILD_SKILL;
	SMenu[MN_ACADEMY].nField[1].x=31;
	SMenu[MN_ACADEMY].nField[1].y=138;
	SMenu[MN_ACADEMY].nField[1].nRectImage=601;
	SMenu[MN_ACADEMY].nField[1].nImageNumber=602;
	SetRect(SMenu[MN_ACADEMY].nField[1].rCheakBox,31,138,180,160);

	// Learn Knowlage
	SMenu[MN_ACADEMY].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_ACADEMY].nField[2].nSpecialWillDo = SWD_SORRY;
//	SMenu[MN_ACADEMY].nField[2].nSpecialWillDo = SWD_SKILLGUILD_LEARN;
	SMenu[MN_ACADEMY].nField[2].nWillDo = MN_SKILLGUILD_KNOWLAGE;
	SMenu[MN_ACADEMY].nField[2].x=31;
	SMenu[MN_ACADEMY].nField[2].y=199;
	SMenu[MN_ACADEMY].nField[2].nRectImage=537;
	SMenu[MN_ACADEMY].nField[2].nImageNumber=520;
	SetRect(SMenu[MN_ACADEMY].nField[2].rCheakBox,31,199,180,221);


////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���� ��忡�� ����� ���� �޴�

//BUY Menu	
//	SMenu[MN_SKILLGUILD_BUY].nTemp;		// �� �ܰ� �޴� ��ȣ�� �޾� �־� �д�
	SMenu[MN_SKILLGUILD_BUY].x=345+GABX_SCREEN;//197;
	SMenu[MN_SKILLGUILD_BUY].y=20;
#ifdef CHANGE_MONEY_
	SMenu[MN_SKILLGUILD_BUY].nImageNumber=4;			// 
	SMenu[MN_SKILLGUILD_BUY].nImageType=CHANGE_MONEY;
#else 
	SMenu[MN_SKILLGUILD_BUY].nImageNumber=609;
#endif
	SMenu[MN_SKILLGUILD_BUY].nFieldCount=8;
//	SMenu[MN_SKILLGUILD_BUY].CheakType=0;

	// ���� ����	
	SMenu[MN_SKILLGUILD_BUY].nField[0].nType=FT_NOMAL_PUT;
	SMenu[MN_SKILLGUILD_BUY].nField[0].x=67;
	SMenu[MN_SKILLGUILD_BUY].nField[0].y=8;
	SMenu[MN_SKILLGUILD_BUY].nField[0].nImageNumber=610;

	// ok, cancel  Button 
	SMenu[MN_SKILLGUILD_BUY].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_SKILLGUILD_BUY].nField[1].nSpecialWillDo=SWD_ITEM_SELLBUY_OK;
	SMenu[MN_SKILLGUILD_BUY].nField[1].nWillDo=ITEM_BUY;
	SMenu[MN_SKILLGUILD_BUY].nField[1].x=23;
	SMenu[MN_SKILLGUILD_BUY].nField[1].y=252;
	SMenu[MN_SKILLGUILD_BUY].nField[1].nRectImage=294;
	SMenu[MN_SKILLGUILD_BUY].nField[1].nImageNumber=295;
	SetRect(SMenu[MN_SKILLGUILD_BUY].nField[1].rCheakBox,20,250, 65,295);

	SMenu[MN_SKILLGUILD_BUY].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_SKILLGUILD_BUY].nField[2].nSpecialWillDo=SWD_ITEM_SELLBUY_CANCEL;
	SMenu[MN_SKILLGUILD_BUY].nField[2].x=181;
	SMenu[MN_SKILLGUILD_BUY].nField[2].y=252;
	SMenu[MN_SKILLGUILD_BUY].nField[2].nRectImage=292;
	SMenu[MN_SKILLGUILD_BUY].nField[2].nImageNumber=293;
	SetRect(SMenu[MN_SKILLGUILD_BUY].nField[2].rCheakBox,179,250,223,295);

	// job guild item inventory 
	SMenu[MN_SKILLGUILD_BUY].nField[3].nType=FT_JOB_ITEM;
	SMenu[MN_SKILLGUILD_BUY].nField[3].nWillDo=ITEM_BUY;
	SMenu[MN_SKILLGUILD_BUY].nField[3].nSHideNomalCount=1;
	SMenu[MN_SKILLGUILD_BUY].nField[3].nSHideNomalStart=0;
	SMenu[MN_SKILLGUILD_BUY].nField[3].nShideNomalPlus=1;	
	SetRect(SMenu[MN_SKILLGUILD_BUY].nField[3].rCheakBox,25,35,200,140);			// �Ʒ��� �� �ʵ�� �پ�ٳ����

	// ������ ȭ�� ���
	SMenu[MN_SKILLGUILD_BUY].nField[4].nType=FT_DATA_PUT;
	SMenu[MN_SKILLGUILD_BUY].nField[4].nWillDo=ITEM_BUY;
	SMenu[MN_SKILLGUILD_BUY].nField[4].x=37;
	SMenu[MN_SKILLGUILD_BUY].nField[4].y=168;
	SMenu[MN_SKILLGUILD_BUY].nField[4].nImageNumber=DP_SELECT_ITEM;

	// �� ���
	SMenu[MN_SKILLGUILD_BUY].nField[5].nType=FT_DATA_PUT;
	SMenu[MN_SKILLGUILD_BUY].nField[5].nWillDo=75;
	SMenu[MN_SKILLGUILD_BUY].nField[5].x=75;
	SMenu[MN_SKILLGUILD_BUY].nField[5].y=266;
	SMenu[MN_SKILLGUILD_BUY].nField[5].nImageNumber=DP_CHARACTER_MONEY;

	// scroll butoon	// up
	SMenu[MN_SKILLGUILD_BUY].nField[6].nType=FT_HIDE_DSCROLL_AUTO_PUT;
	SMenu[MN_SKILLGUILD_BUY].nField[6].nWillDo=3;
	SMenu[MN_SKILLGUILD_BUY].nField[6].x=205;
	SMenu[MN_SKILLGUILD_BUY].nField[6].y=37;
	SMenu[MN_SKILLGUILD_BUY].nField[6].nRectImage=611;
	SMenu[MN_SKILLGUILD_BUY].nField[6].nImageNumber=612;
	SetRect(SMenu[MN_SKILLGUILD_BUY].nField[6].rCheakBox,203,35,223,70);
						// down
	SMenu[MN_SKILLGUILD_BUY].nField[7].nType=FT_HIDE_ISCROLL_AUTO_PUT;
	SMenu[MN_SKILLGUILD_BUY].nField[7].nWillDo=3;
	SMenu[MN_SKILLGUILD_BUY].nField[7].x=205;
	SMenu[MN_SKILLGUILD_BUY].nField[7].y=108;
	SMenu[MN_SKILLGUILD_BUY].nField[7].nRectImage=613;
	SMenu[MN_SKILLGUILD_BUY].nField[7].nImageNumber=614;
	SetRect(SMenu[MN_SKILLGUILD_BUY].nField[7].rCheakBox,203,106,223,141);

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Sell Menu
//	SMenu[MN_SKILLGUILD_SELL].nTemp;		// �� �ܰ� �޴� ��ȣ�� �޾� �־� �д�
	SMenu[MN_SKILLGUILD_SELL].x=345+GABX_SCREEN;
	SMenu[MN_SKILLGUILD_SELL].y=20;
#ifdef CHANGE_MONEY_
	SMenu[MN_SKILLGUILD_SELL].nImageNumber=4;			// 
	SMenu[MN_SKILLGUILD_SELL].nImageType=CHANGE_MONEY;
#else 
	SMenu[MN_SKILLGUILD_SELL].nImageNumber=609;
#endif
	SMenu[MN_SKILLGUILD_SELL].nFieldCount=8;
	SMenu[MN_SKILLGUILD_SELL].CheakType=0;

	// ���� ����	
//	SMenu[MN_SKILLGUILD_SELL].nField[0].nType=FT_NO_CHECK;
//	SMenu[MN_SKILLGUILD_SELL].nField[0].x=66;
//	SMenu[MN_SKILLGUILD_SELL].nField[0].y=10;
//	SMenu[MN_SKILLGUILD_SELL].nField[0].nImageNumber=610;

	// ok, cancel  Button 
	SMenu[MN_SKILLGUILD_SELL].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_SKILLGUILD_SELL].nField[1].nSpecialWillDo=SWD_ITEM_SELLBUY_OK;
	SMenu[MN_SKILLGUILD_SELL].nField[1].nWillDo=ITEM_SELL;
	SMenu[MN_SKILLGUILD_SELL].nField[1].x=23;
	SMenu[MN_SKILLGUILD_SELL].nField[1].y=252;
	SMenu[MN_SKILLGUILD_SELL].nField[1].nRectImage=294;
	SMenu[MN_SKILLGUILD_SELL].nField[1].nImageNumber=295;
	SetRect(SMenu[MN_SKILLGUILD_SELL].nField[1].rCheakBox,20,250, 65,295);

	SMenu[MN_SKILLGUILD_SELL].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_SKILLGUILD_SELL].nField[2].nSpecialWillDo=SWD_ITEM_SELLBUY_CANCEL;
	SMenu[MN_SKILLGUILD_SELL].nField[2].x=181;
	SMenu[MN_SKILLGUILD_SELL].nField[2].y=252;
	SMenu[MN_SKILLGUILD_SELL].nField[2].nRectImage=292;
	SMenu[MN_SKILLGUILD_SELL].nField[2].nImageNumber=293;
	SetRect(SMenu[MN_SKILLGUILD_SELL].nField[2].rCheakBox,179,250,223,295);

	// job guild item inventory 
	SMenu[MN_SKILLGUILD_SELL].nField[3].nType=FT_JOB_ITEM;
	SMenu[MN_SKILLGUILD_SELL].nField[3].nWillDo=ITEM_SELL;
	SMenu[MN_SKILLGUILD_SELL].nField[3].nSHideNomalCount=1;
	SMenu[MN_SKILLGUILD_SELL].nField[3].nSHideNomalStart=0;
	SMenu[MN_SKILLGUILD_SELL].nField[3].nShideNomalPlus=1;	
	SetRect(SMenu[MN_SKILLGUILD_SELL].nField[3].rCheakBox,25,35,200,140);			// �Ʒ��� �� �ʵ�� �پ�ٳ����

	// ������ ȭ�� ���
	SMenu[MN_SKILLGUILD_SELL].nField[4].nType=FT_DATA_PUT;
	SMenu[MN_SKILLGUILD_SELL].nField[4].nWillDo=ITEM_SELL;
	SMenu[MN_SKILLGUILD_SELL].nField[4].x=37;
	SMenu[MN_SKILLGUILD_SELL].nField[4].y=168;
	SMenu[MN_SKILLGUILD_SELL].nField[4].nImageNumber=DP_SELECT_ITEM; // DP_SELECT_ITEM

	// �� ���
	SMenu[MN_SKILLGUILD_SELL].nField[5].nType=FT_DATA_PUT;
	SMenu[MN_SKILLGUILD_SELL].nField[5].nWillDo=75;
	SMenu[MN_SKILLGUILD_SELL].nField[5].x=75;
	SMenu[MN_SKILLGUILD_SELL].nField[5].y=266;
	SMenu[MN_SKILLGUILD_SELL].nField[5].nImageNumber=DP_CHARACTER_MONEY;

	// scroll butoon	// up
	SMenu[MN_SKILLGUILD_SELL].nField[6].nType=FT_HIDE_DSCROLL_AUTO_PUT;
	SMenu[MN_SKILLGUILD_SELL].nField[6].nWillDo=3;
	SMenu[MN_SKILLGUILD_SELL].nField[6].x=205;
	SMenu[MN_SKILLGUILD_SELL].nField[6].y=37;
	SMenu[MN_SKILLGUILD_SELL].nField[6].nRectImage=611;
	SMenu[MN_SKILLGUILD_SELL].nField[6].nImageNumber=612;
	SetRect(SMenu[MN_SKILLGUILD_SELL].nField[6].rCheakBox,203,35,223,70);
						// down
	SMenu[MN_SKILLGUILD_SELL].nField[7].nType=FT_HIDE_ISCROLL_AUTO_PUT;
	SMenu[MN_SKILLGUILD_SELL].nField[7].nWillDo=3;
	SMenu[MN_SKILLGUILD_SELL].nField[7].x=205;
	SMenu[MN_SKILLGUILD_SELL].nField[7].y=108;
	SMenu[MN_SKILLGUILD_SELL].nField[7].nRectImage=613;
	SMenu[MN_SKILLGUILD_SELL].nField[7].nImageNumber=614;
	SetRect(SMenu[MN_SKILLGUILD_SELL].nField[7].rCheakBox,203,106,223,141);


////////////////////////////////////////////////////////////////////////////////////////////////////////
// Learn Skill Menu

	SMenu[MN_SKILLGUILD_SKILL].x=345+GABX_SCREEN;
	SMenu[MN_SKILLGUILD_SKILL].y=20;
#ifdef CHANGE_MONEY_
	SMenu[MN_SKILLGUILD_SKILL].nImageNumber=5;			// 
	SMenu[MN_SKILLGUILD_SKILL].nImageType=CHANGE_MONEY;
#else 
	SMenu[MN_SKILLGUILD_SKILL].nImageNumber=615;
#endif
	SMenu[MN_SKILLGUILD_SKILL].nFieldCount=7;

	// ok, cancel  Button 
	SMenu[MN_SKILLGUILD_SKILL].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_SKILLGUILD_SKILL].nField[0].nSpecialWillDo= SWD_LEARNSKILL_OK;
	SMenu[MN_SKILLGUILD_SKILL].nField[0].x=23;
	SMenu[MN_SKILLGUILD_SKILL].nField[0].y=279;
	SMenu[MN_SKILLGUILD_SKILL].nField[0].nRectImage=940;
	SMenu[MN_SKILLGUILD_SKILL].nField[0].nImageNumber=939;
	SetRect(SMenu[MN_SKILLGUILD_SKILL].nField[0].rCheakBox,23,279,89 ,300);

	SMenu[MN_SKILLGUILD_SKILL].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_SKILLGUILD_SKILL].nField[1].nSpecialWillDo=SWD_ITEM_SELLBUY_CANCEL;
	SMenu[MN_SKILLGUILD_SKILL].nField[1].x=155;
	SMenu[MN_SKILLGUILD_SKILL].nField[1].y=279;
	SMenu[MN_SKILLGUILD_SKILL].nField[1].nRectImage=938;
	SMenu[MN_SKILLGUILD_SKILL].nField[1].nImageNumber=937;
	SetRect(SMenu[MN_SKILLGUILD_SKILL].nField[1].rCheakBox,155,279,221,300);

	// ������ ȭ�� ���
	SMenu[MN_SKILLGUILD_SKILL].nField[2].nType=FT_DO;
	SMenu[MN_SKILLGUILD_SKILL].nField[2].nWillDo=DO_LEARNSKILL_ICON;
	SMenu[MN_SKILLGUILD_SKILL].nField[2].x=30;
	SMenu[MN_SKILLGUILD_SKILL].nField[2].y=86;
	SMenu[MN_SKILLGUILD_SKILL].nField[2].nSHideNomalCount=1;
	SMenu[MN_SKILLGUILD_SKILL].nField[2].nSHideNomalStart=0;
	SMenu[MN_SKILLGUILD_SKILL].nField[2].nShideNomalPlus=1;
	SetRect(SMenu[MN_SKILLGUILD_SKILL].nField[2].rCheakBox,25,77,199,112);

	// scroll button
	SMenu[MN_SKILLGUILD_SKILL].nField[3].nType=FT_HIDE_DSCROLL_AUTO_PUT;
	SMenu[MN_SKILLGUILD_SKILL].nField[3].nWillDo=2;
	SMenu[MN_SKILLGUILD_SKILL].nField[3].x=211;
	SMenu[MN_SKILLGUILD_SKILL].nField[3].y=66;
	SMenu[MN_SKILLGUILD_SKILL].nField[3].nRectImage=616;
	SMenu[MN_SKILLGUILD_SKILL].nField[3].nImageNumber=617;
	SetRect(SMenu[MN_SKILLGUILD_SKILL].nField[3].rCheakBox,213,60,234,91);
						// down
	SMenu[MN_SKILLGUILD_SKILL].nField[4].nType=FT_HIDE_ISCROLL_AUTO_PUT;
	SMenu[MN_SKILLGUILD_SKILL].nField[4].nWillDo=2;
	SMenu[MN_SKILLGUILD_SKILL].nField[4].x=211;
	SMenu[MN_SKILLGUILD_SKILL].nField[4].y=112;
	SMenu[MN_SKILLGUILD_SKILL].nField[4].nRectImage=618;
	SMenu[MN_SKILLGUILD_SKILL].nField[4].nImageNumber=619;
	SetRect(SMenu[MN_SKILLGUILD_SKILL].nField[4].rCheakBox,213,107,234,138);

	// other
	SMenu[MN_SKILLGUILD_SKILL].nField[5].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_SKILLGUILD_SKILL].nField[5].nSpecialWillDo=SWD_LEARNSKILL_OTHER;
	SMenu[MN_SKILLGUILD_SKILL].nField[5].nWillDo=6;
	SMenu[MN_SKILLGUILD_SKILL].nField[5].x=83;
	SMenu[MN_SKILLGUILD_SKILL].nField[5].y=251;
	SMenu[MN_SKILLGUILD_SKILL].nField[5].nRectImage=621;
	SMenu[MN_SKILLGUILD_SKILL].nField[5].nImageNumber=622;
	SetRect(SMenu[MN_SKILLGUILD_SKILL].nField[5].rCheakBox,83,251,161,272);

	// ���� ����	
	SMenu[MN_SKILLGUILD_SKILL].nField[6].nType=FT_NOMAL_PUT;
	SMenu[MN_SKILLGUILD_SKILL].nField[6].x=19;
	SMenu[MN_SKILLGUILD_SKILL].nField[6].y=44;
//	SMenu[MN_SKILLGUILD_SKILL].nField[6].nSHideNomalCount=2;	��Ȳ�� ���� �ٲ�
	SMenu[MN_SKILLGUILD_SKILL].nField[6].nSHideNomalStart=0;
	SMenu[MN_SKILLGUILD_SKILL].nField[6].nShideNomalPlus=1;
//	SMenu[MN_SKILLGUILD_SKILL].nField[6].nImageNumber=000;		��Ȳ�� ���� �ٲ�

//	SMenu[MN_SKILLGUILD_SKILL].nField[7].nType=FT_DATA_PUT;
//	SMenu[MN_SKILLGUILD_SKILL].nField[7].x=45;
//	SMenu[MN_SKILLGUILD_SKILL].nField[7].y=180;
//	SMenu[MN_SKILLGUILD_SKILL].nField[7].nImageNumber=DP_SELECT_SKILL;

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Learn Skill MN_SKILLGUILD_KNOWLAGE

	SMenu[MN_SKILLGUILD_KNOWLAGE].x=345+GABX_SCREEN;
	SMenu[MN_SKILLGUILD_KNOWLAGE].y=20;
	SMenu[MN_SKILLGUILD_KNOWLAGE].nImageNumber=615;
	SMenu[MN_SKILLGUILD_KNOWLAGE].nFieldCount=8;

	// ok, cancel  Button 
	SMenu[MN_SKILLGUILD_KNOWLAGE].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_SKILLGUILD_KNOWLAGE].nField[0].nSpecialWillDo= SWD_LEARNSKILL_OK;
	SMenu[MN_SKILLGUILD_KNOWLAGE].nField[0].x=23;
	SMenu[MN_SKILLGUILD_KNOWLAGE].nField[0].y=279;
	SMenu[MN_SKILLGUILD_KNOWLAGE].nField[0].nRectImage=940;
	SMenu[MN_SKILLGUILD_KNOWLAGE].nField[0].nImageNumber=939;
	SetRect(SMenu[MN_SKILLGUILD_KNOWLAGE].nField[0].rCheakBox,23,279,89 ,300);

	SMenu[MN_SKILLGUILD_KNOWLAGE].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_SKILLGUILD_KNOWLAGE].nField[1].nSpecialWillDo=SWD_ITEM_SELLBUY_CANCEL;
	SMenu[MN_SKILLGUILD_KNOWLAGE].nField[1].x=155;
	SMenu[MN_SKILLGUILD_KNOWLAGE].nField[1].y=279;
	SMenu[MN_SKILLGUILD_KNOWLAGE].nField[1].nRectImage=938;
	SMenu[MN_SKILLGUILD_KNOWLAGE].nField[1].nImageNumber=937;
	SetRect(SMenu[MN_SKILLGUILD_KNOWLAGE].nField[1].rCheakBox,155,279,221,300);

	// ������ ȭ�� ���
	SMenu[MN_SKILLGUILD_KNOWLAGE].nField[2].nType=FT_DO;
	SMenu[MN_SKILLGUILD_KNOWLAGE].nField[2].nWillDo=DO_LEARNSKILL_ICON;
	SMenu[MN_SKILLGUILD_KNOWLAGE].nField[2].x=30;
	SMenu[MN_SKILLGUILD_KNOWLAGE].nField[2].y=86;
	SMenu[MN_SKILLGUILD_KNOWLAGE].nField[2].nSHideNomalCount=1;
	SMenu[MN_SKILLGUILD_KNOWLAGE].nField[2].nSHideNomalStart=0;
	SMenu[MN_SKILLGUILD_KNOWLAGE].nField[2].nShideNomalPlus=1;
	SetRect(SMenu[MN_SKILLGUILD_KNOWLAGE].nField[2].rCheakBox,25,77,199,112);

	// scroll button
	SMenu[MN_SKILLGUILD_KNOWLAGE].nField[3].nType=FT_HIDE_DSCROLL_AUTO_PUT;
	SMenu[MN_SKILLGUILD_KNOWLAGE].nField[3].nWillDo=2;
	SMenu[MN_SKILLGUILD_KNOWLAGE].nField[3].x=211;
	SMenu[MN_SKILLGUILD_KNOWLAGE].nField[3].y=65;
	SMenu[MN_SKILLGUILD_KNOWLAGE].nField[3].nRectImage=616;
	SMenu[MN_SKILLGUILD_KNOWLAGE].nField[3].nImageNumber=617;
	SetRect(SMenu[MN_SKILLGUILD_KNOWLAGE].nField[3].rCheakBox,213,60,234,91);
						// down
	SMenu[MN_SKILLGUILD_KNOWLAGE].nField[4].nType=FT_HIDE_ISCROLL_AUTO_PUT;
	SMenu[MN_SKILLGUILD_KNOWLAGE].nField[4].nWillDo=2;
	SMenu[MN_SKILLGUILD_KNOWLAGE].nField[4].x=211;
	SMenu[MN_SKILLGUILD_KNOWLAGE].nField[4].y=110;
	SMenu[MN_SKILLGUILD_KNOWLAGE].nField[4].nRectImage=618;
	SMenu[MN_SKILLGUILD_KNOWLAGE].nField[4].nImageNumber=619;
	SetRect(SMenu[MN_SKILLGUILD_KNOWLAGE].nField[4].rCheakBox,213,107,234,138);

	// other
	SMenu[MN_SKILLGUILD_KNOWLAGE].nField[5].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_SKILLGUILD_KNOWLAGE].nField[5].nSpecialWillDo=SWD_LEARNSKILL_OTHER;
	SMenu[MN_SKILLGUILD_KNOWLAGE].nField[5].nWillDo=6;
	SMenu[MN_SKILLGUILD_KNOWLAGE].nField[5].x=83;
	SMenu[MN_SKILLGUILD_KNOWLAGE].nField[5].y=251;
	SMenu[MN_SKILLGUILD_KNOWLAGE].nField[5].nRectImage=621;
	SMenu[MN_SKILLGUILD_KNOWLAGE].nField[5].nImageNumber=622;
	SetRect(SMenu[MN_SKILLGUILD_KNOWLAGE].nField[5].rCheakBox,83,251,161,272);

	// ���� ���� ( ��ų �߰� ���� ) - ������ �ٲ��ش�. 
	SMenu[MN_SKILLGUILD_KNOWLAGE].nField[6].nType=FT_NOMAL_PUT;
	SMenu[MN_SKILLGUILD_KNOWLAGE].nField[6].x=19;
	SMenu[MN_SKILLGUILD_KNOWLAGE].nField[6].y=44;

	// knowlage ��� �ֱ� 
	SMenu[MN_SKILLGUILD_KNOWLAGE].nField[7].nType=FT_NOMAL_PUT;
	SMenu[MN_SKILLGUILD_KNOWLAGE].nField[7].x=0;
	SMenu[MN_SKILLGUILD_KNOWLAGE].nField[7].y=0;
	SMenu[MN_SKILLGUILD_KNOWLAGE].nField[7].nImageNumber=285;


////////////////////////////////////////////////////////////////////////////////////////////////////////
// Lean Item Menu
	SMenu[MN_SKILLGUILD_ITEM].x=345+GABX_SCREEN;
	SMenu[MN_SKILLGUILD_ITEM].y=20;
#ifdef CHANGE_MONEY_
	SMenu[MN_SKILLGUILD_ITEM].nImageNumber=0;			// 
	SMenu[MN_SKILLGUILD_ITEM].nImageType=CHANGE_MONEY;
#else 
	SMenu[MN_SKILLGUILD_ITEM].nImageNumber=10;
	SMenu[MN_SKILLGUILD_ITEM].nImageType=SMALL_ETC;
#endif
	SMenu[MN_SKILLGUILD_ITEM].nFieldCount=5;

	// ok, cancel  Button 
	SMenu[MN_SKILLGUILD_ITEM].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_SKILLGUILD_ITEM].nField[0].nSpecialWillDo= SWD_LEARNITEM_OK;
	SMenu[MN_SKILLGUILD_ITEM].nField[0].nWillDo=3;
	SMenu[MN_SKILLGUILD_ITEM].nField[0].x=23;
	SMenu[MN_SKILLGUILD_ITEM].nField[0].y=279;
	SMenu[MN_SKILLGUILD_ITEM].nField[0].nRectImage=940;
	SMenu[MN_SKILLGUILD_ITEM].nField[0].nImageNumber=939;
	SetRect(SMenu[MN_SKILLGUILD_ITEM].nField[0].rCheakBox,23,279,89,300);

	SMenu[MN_SKILLGUILD_ITEM].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_SKILLGUILD_ITEM].nField[1].nSpecialWillDo=SWD_ITEM_SELLBUY_CANCEL;
	SMenu[MN_SKILLGUILD_ITEM].nField[1].x=155;
	SMenu[MN_SKILLGUILD_ITEM].nField[1].y=279;
	SMenu[MN_SKILLGUILD_ITEM].nField[1].nRectImage=938;
	SMenu[MN_SKILLGUILD_ITEM].nField[1].nImageNumber=937;
	SetRect(SMenu[MN_SKILLGUILD_ITEM].nField[1].rCheakBox,155,279,221,300);

	// -->
	SMenu[MN_SKILLGUILD_ITEM].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_SKILLGUILD_ITEM].nField[2].nSpecialWillDo=SWD_LEARNSKILL_OTHER;
	SMenu[MN_SKILLGUILD_ITEM].nField[2].nWillDo=3;
	SMenu[MN_SKILLGUILD_ITEM].nField[2].x=121;
	SMenu[MN_SKILLGUILD_ITEM].nField[2].y=252;
	SMenu[MN_SKILLGUILD_ITEM].nField[2].nSHideNomalCount=RIGHT;
	SMenu[MN_SKILLGUILD_ITEM].nField[2].nRectImage=7;
	SMenu[MN_SKILLGUILD_ITEM].nField[2].nImageNumber=6;
	SMenu[MN_SKILLGUILD_ITEM].nField[2].nImageType=SMALL_ETC;
	SetRect(SMenu[MN_SKILLGUILD_ITEM].nField[2].rCheakBox,122,252,157,270);

	// display item
	SMenu[MN_SKILLGUILD_ITEM].nField[3].nType=FT_LEARN_ITEM;
	SMenu[MN_SKILLGUILD_ITEM].nField[3].x=30;
	SMenu[MN_SKILLGUILD_ITEM].nField[3].y=70;
	SetRect(SMenu[MN_SKILLGUILD_ITEM].nField[3].rCheakBox,17,52,230,208);
	
	// <--
//	SMenu[MN_SKILLGUILD_ITEM].nField[4].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_SKILLGUILD_ITEM].nField[4].nType=FT_NOMAL_PUT;
	SMenu[MN_SKILLGUILD_ITEM].nField[4].nSpecialWillDo=SWD_LEARNSKILL_OTHER;
	SMenu[MN_SKILLGUILD_ITEM].nField[4].nWillDo=3;
	SMenu[MN_SKILLGUILD_ITEM].nField[4].x=88;
	SMenu[MN_SKILLGUILD_ITEM].nField[4].y=252;
	SMenu[MN_SKILLGUILD_ITEM].nField[4].nSHideNomalCount=LEFT;
	SMenu[MN_SKILLGUILD_ITEM].nField[4].nRectImage=9;
	SMenu[MN_SKILLGUILD_ITEM].nField[4].nImageNumber=8;
	SMenu[MN_SKILLGUILD_ITEM].nField[4].nImageType=SMALL_ETC;
	SetRect(SMenu[MN_SKILLGUILD_ITEM].nField[4].rCheakBox,88,252,123,270);





////////////////////////////////////////////////////////////////////////////////////////////////////////
// Repair Menu

//	SMenu[MN_SKILLGUILD_REPAIR].nTemp;		// �� �ܰ� �޴� ��ȣ�� �޾� �־� �д�
	SMenu[MN_SKILLGUILD_REPAIR].x=345+GABX_SCREEN;
	SMenu[MN_SKILLGUILD_REPAIR].y=20;
#ifdef CHANGE_MONEY_
	SMenu[MN_SKILLGUILD_REPAIR].nImageNumber=4;			// 
	SMenu[MN_SKILLGUILD_REPAIR].nImageType=CHANGE_MONEY;
#else 
	SMenu[MN_SKILLGUILD_REPAIR].nImageNumber=609;
#endif
	SMenu[MN_SKILLGUILD_REPAIR].nFieldCount=8;
	SMenu[MN_SKILLGUILD_REPAIR].CheakType=0;

	// ���� ����	
	SMenu[MN_SKILLGUILD_REPAIR].nField[0].nType=FT_NOMAL_PUT;
	SMenu[MN_SKILLGUILD_REPAIR].nField[0].x=67;
	SMenu[MN_SKILLGUILD_REPAIR].nField[0].y=8;
	SMenu[MN_SKILLGUILD_REPAIR].nField[0].nImageNumber=548;

	// ok, cancel  Button 
	SMenu[MN_SKILLGUILD_REPAIR].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_SKILLGUILD_REPAIR].nField[1].nSpecialWillDo=SWD_ITEM_SELLBUY_OK;
	SMenu[MN_SKILLGUILD_REPAIR].nField[1].nWillDo=ITEM_REPAIR;
	SMenu[MN_SKILLGUILD_REPAIR].nField[1].x=23;
	SMenu[MN_SKILLGUILD_REPAIR].nField[1].y=252;
	SMenu[MN_SKILLGUILD_REPAIR].nField[1].nRectImage=294;
	SMenu[MN_SKILLGUILD_REPAIR].nField[1].nImageNumber=295;
	SetRect(SMenu[MN_SKILLGUILD_REPAIR].nField[1].rCheakBox,20,250, 65,295);

	SMenu[MN_SKILLGUILD_REPAIR].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_SKILLGUILD_REPAIR].nField[2].nSpecialWillDo=SWD_ITEM_SELLBUY_CANCEL;
	SMenu[MN_SKILLGUILD_REPAIR].nField[2].x=181;
	SMenu[MN_SKILLGUILD_REPAIR].nField[2].y=252;
	SMenu[MN_SKILLGUILD_REPAIR].nField[2].nRectImage=292;
	SMenu[MN_SKILLGUILD_REPAIR].nField[2].nImageNumber=293;
	SetRect(SMenu[MN_SKILLGUILD_REPAIR].nField[2].rCheakBox,179,250,223,295);

	// job guild item inventory 
	SMenu[MN_SKILLGUILD_REPAIR].nField[3].nType=FT_JOB_ITEM;
	SMenu[MN_SKILLGUILD_REPAIR].nField[3].nWillDo=ITEM_REPAIR;
	SMenu[MN_SKILLGUILD_REPAIR].nField[3].nSHideNomalCount=1;
	SMenu[MN_SKILLGUILD_REPAIR].nField[3].nSHideNomalStart=0;
	SMenu[MN_SKILLGUILD_REPAIR].nField[3].nShideNomalPlus=1;	
	SetRect(SMenu[MN_SKILLGUILD_REPAIR].nField[3].rCheakBox,25,35,200,140);			// �Ʒ��� �� �ʵ�� �پ�ٳ����

	// ������ ȭ�� ���
	SMenu[MN_SKILLGUILD_REPAIR].nField[4].nType=FT_DATA_PUT;
	SMenu[MN_SKILLGUILD_REPAIR].nField[4].nWillDo=ITEM_REPAIR;
	SMenu[MN_SKILLGUILD_REPAIR].nField[4].x=37;
	SMenu[MN_SKILLGUILD_REPAIR].nField[4].y=168;
	SMenu[MN_SKILLGUILD_REPAIR].nField[4].nImageNumber=DP_SELECT_ITEM;

	// �� ���
	SMenu[MN_SKILLGUILD_REPAIR].nField[5].nType=FT_DATA_PUT;
	SMenu[MN_SKILLGUILD_REPAIR].nField[5].nWillDo=75;
	SMenu[MN_SKILLGUILD_REPAIR].nField[5].x=75;
	SMenu[MN_SKILLGUILD_REPAIR].nField[5].y=266;
	SMenu[MN_SKILLGUILD_REPAIR].nField[5].nImageNumber=DP_CHARACTER_MONEY;

	// scroll butoon	// up
	SMenu[MN_SKILLGUILD_REPAIR].nField[6].nType=FT_HIDE_DSCROLL_AUTO_PUT;
	SMenu[MN_SKILLGUILD_REPAIR].nField[6].nWillDo=3;
	SMenu[MN_SKILLGUILD_REPAIR].nField[6].x=205;
	SMenu[MN_SKILLGUILD_REPAIR].nField[6].y=37;
	SMenu[MN_SKILLGUILD_REPAIR].nField[6].nRectImage=611;
	SMenu[MN_SKILLGUILD_REPAIR].nField[6].nImageNumber=612;
	SetRect(SMenu[MN_SKILLGUILD_REPAIR].nField[6].rCheakBox,203,35,223,70);
						// down
	SMenu[MN_SKILLGUILD_REPAIR].nField[7].nType=FT_HIDE_ISCROLL_AUTO_PUT;
	SMenu[MN_SKILLGUILD_REPAIR].nField[7].nWillDo=3;
	SMenu[MN_SKILLGUILD_REPAIR].nField[7].x=205;
	SMenu[MN_SKILLGUILD_REPAIR].nField[7].y=108;
	SMenu[MN_SKILLGUILD_REPAIR].nField[7].nRectImage=613;
	SMenu[MN_SKILLGUILD_REPAIR].nField[7].nImageNumber=614;
	SetRect(SMenu[MN_SKILLGUILD_REPAIR].nField[7].rCheakBox,203,106,223,141);



////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
// ������ ���� â
	SMenu[MN_ITEM_EXPLAN].x=197;
	SMenu[MN_ITEM_EXPLAN].y=20;
#ifdef CHANGE_MONEY_
	SMenu[MN_ITEM_EXPLAN].nImageNumber=4;			// 
	SMenu[MN_ITEM_EXPLAN].nImageType=CHANGE_MONEY;
#else 
	SMenu[MN_ITEM_EXPLAN].nImageNumber=609;
#endif
	SMenu[MN_ITEM_EXPLAN].nFieldCount=3;

	// ���� ����	
	SMenu[MN_ITEM_EXPLAN].nField[0].nType=FT_NOMAL_PUT;
	SMenu[MN_ITEM_EXPLAN].nField[0].x=66;
	SMenu[MN_ITEM_EXPLAN].nField[0].y=10;
	SMenu[MN_ITEM_EXPLAN].nField[0].nImageNumber=610;

	// ok, cancel  Button 
	SMenu[MN_ITEM_EXPLAN].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_ITEM_EXPLAN].nField[1].nSpecialWillDo=SWD_ITEM_SELLBUY_OK;
	SMenu[MN_ITEM_EXPLAN].nField[1].x=23;
	SMenu[MN_ITEM_EXPLAN].nField[1].y=252;
	SMenu[MN_ITEM_EXPLAN].nField[1].nRectImage=294;
	SMenu[MN_ITEM_EXPLAN].nField[1].nImageNumber=295;
	SetRect(SMenu[MN_ITEM_EXPLAN].nField[1].rCheakBox,20,250, 65,295);

	SMenu[MN_ITEM_EXPLAN].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_ITEM_EXPLAN].nField[2].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_ITEM_EXPLAN].nField[2].x=181;
	SMenu[MN_ITEM_EXPLAN].nField[2].y=252;
	SMenu[MN_ITEM_EXPLAN].nField[2].nRectImage=292;
	SMenu[MN_ITEM_EXPLAN].nField[2].nImageNumber=293;
	SetRect(SMenu[MN_ITEM_EXPLAN].nField[2].rCheakBox,179,250,223,295);


////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���� ������ ���� â


////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��ų ������ ���� â




////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���� ȸ�� �޴�

	//���� �޴� �ٿ��
//	SMenu[MN_TOWNHALL].bActive=TRUE;
	SMenu[MN_TOWNHALL].x=366+GABX_SCREEN;
	SMenu[MN_TOWNHALL].y=20;
	SMenu[MN_TOWNHALL].nImageNumber=402;
	SMenu[MN_TOWNHALL].nFieldCount=8;
	SMenu[MN_TOWNHALL].CheakType=0;

	//EXIT��ư
	SMenu[MN_TOWNHALL].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL].nField[0].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_TOWNHALL].nField[0].x=85;
	SMenu[MN_TOWNHALL].nField[0].y=279;
	SMenu[MN_TOWNHALL].nField[0].nRectImage=403;
	SMenu[MN_TOWNHALL].nField[0].nImageNumber=404;
	SetRect(SMenu[MN_TOWNHALL].nField[0].rCheakBox,85,279, 158,301);

	//�޴� ����
	SMenu[MN_TOWNHALL].nField[1].nType=FT_HIDE_NOMAL_GROUP_PUT;
	SMenu[MN_TOWNHALL].nField[1].nSHideNomalNumber=HN_TOWNHALL_TEXT;
	SMenu[MN_TOWNHALL].nField[1].nSHideNomalCount=6;
	SMenu[MN_TOWNHALL].nField[1].nSHideNomalStart=0;
	SMenu[MN_TOWNHALL].nField[1].nShideNomalPlus=6;

	//vote �簢��
	SMenu[MN_TOWNHALL].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL].nField[2].nSpecialWillDo=SWD_GO_VOTE;
	SMenu[MN_TOWNHALL].nField[2].x=35;
	SMenu[MN_TOWNHALL].nField[2].y=77;
	SMenu[MN_TOWNHALL].nField[2].nRectImage=16;
	SMenu[MN_TOWNHALL].nField[2].nImageNumber=16;
	SetRect(SMenu[MN_TOWNHALL].nField[2].rCheakBox,35,77,204,107);

	//BUILDING�� �簢��
	SMenu[MN_TOWNHALL].nField[3].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL].nField[3].nSpecialWillDo=SWD_WILLDO;
	SMenu[MN_TOWNHALL].nField[3].nWillDo=MN_TOWNHALL_BUILDING;
	SMenu[MN_TOWNHALL].nField[3].x=35;
	SMenu[MN_TOWNHALL].nField[3].y=108;
	SMenu[MN_TOWNHALL].nField[3].nRectImage=16;
	SMenu[MN_TOWNHALL].nField[3].nImageNumber=16;
	SetRect(SMenu[MN_TOWNHALL].nField[3].rCheakBox,35,108,204,138);

	//GUILD�� �簢��
	SMenu[MN_TOWNHALL].nField[4].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL].nField[4].nSpecialWillDo=SWD_WILLDO;
//	SMenu[MN_TOWNHALL].nField[4].nSpecialWillDo=SWD_SORRY;
	SMenu[MN_TOWNHALL].nField[4].nWillDo=MN_TOWNHALL_GUILD;
	SMenu[MN_TOWNHALL].nField[4].x=35;
	SMenu[MN_TOWNHALL].nField[4].y=139;
	SMenu[MN_TOWNHALL].nField[4].nRectImage=16;
	SMenu[MN_TOWNHALL].nField[4].nImageNumber=16;
	SetRect(SMenu[MN_TOWNHALL].nField[4].rCheakBox,35,139,204,169);

	//FORUM�� �簢��
	SMenu[MN_TOWNHALL].nField[5].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL].nField[5].nSpecialWillDo=SWD_TOWNHALL_TITLE;
//	SMenu[MN_TOWNHALL].nField[5].nSpecialWillDo=SWD_SORRY;
	SMenu[MN_TOWNHALL].nField[5].nWillDo=MN_TOWNHALL_FORUM;
	SMenu[MN_TOWNHALL].nField[5].x=35;
	SMenu[MN_TOWNHALL].nField[5].y=170;
	SMenu[MN_TOWNHALL].nField[5].nRectImage=16;
	SMenu[MN_TOWNHALL].nField[5].nImageNumber=16;
	SetRect(SMenu[MN_TOWNHALL].nField[5].rCheakBox,35,170,204,200);

	//EVENT�� �簢��
	SMenu[MN_TOWNHALL].nField[6].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL].nField[6].nSpecialWillDo=SWD_TOWNHALL_TITLE;
//	SMenu[MN_TOWNHALL].nField[6].nSpecialWillDo=SWD_SORRY;
	SMenu[MN_TOWNHALL].nField[6].nWillDo=MN_TOWNHALL_EVENT;
	SMenu[MN_TOWNHALL].nField[6].x=35;
	SMenu[MN_TOWNHALL].nField[6].y=201;
	SMenu[MN_TOWNHALL].nField[6].nRectImage=16;
	SMenu[MN_TOWNHALL].nField[6].nImageNumber=16;
	SetRect(SMenu[MN_TOWNHALL].nField[6].rCheakBox,35,201,204,231);

	//RANK�� �簢��
	SMenu[MN_TOWNHALL].nField[7].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL].nField[7].nSpecialWillDo=SWD_TOWNHALL_TITLE;
//	SMenu[MN_TOWNHALL].nField[7].nSpecialWillDo=SWD_SORRY;
	SMenu[MN_TOWNHALL].nField[7].nWillDo=MN_TOWNHALL_RANK1;
	SMenu[MN_TOWNHALL].nField[7].x=35;
	SMenu[MN_TOWNHALL].nField[7].y=232;
	SMenu[MN_TOWNHALL].nField[7].nRectImage=16;
	SMenu[MN_TOWNHALL].nField[7].nImageNumber=16;
	SetRect(SMenu[MN_TOWNHALL].nField[7].rCheakBox,37,232,204,262);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//TOWNHALL���� PUBLICNOTICE�޴�

	SMenu[MN_TOWNHALL_PUBLICNOTICE].x=366+GABX_SCREEN;
	SMenu[MN_TOWNHALL_PUBLICNOTICE].y=20;
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nImageNumber=412;
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nFieldCount=11;
	SMenu[MN_TOWNHALL_PUBLICNOTICE].CheakType=0;

	//EXIT��ư
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[0].nSpecialWillDo=SWD_SUBMENU_BACK;
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[0].x=129;
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[0].y=270;
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[0].nRectImage=403;
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[0].nImageNumber=404;
	SetRect(SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[0].rCheakBox,129,270,202,292);

	//read��ư
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[1].nSpecialWillDo=SWD_BBS_READ ;
//	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[1].nWillDo=MN_TOWNHALL_NOTICE1;
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[1].nWillDo=MN_TOWNHALL_BBS1;
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[1].nSHideNomalCount = 2; // �Ʒ� �ʵ带 ����Ŵ
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[1].x=42;
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[1].y=270;
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[1].nRectImage=413;
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[1].nImageNumber=414;
	SetRect(SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[1].rCheakBox,41,270,106,292);

	// �������� ���� ���		// �Ʒ� �� �ʵ�� ���� �ٳ�� �Ѵ�. �������
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[2].nType=FT_DATA_PUT;
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[2].nWillDo=160;
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[2].nSpecialWillDo=30;
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[2].x=42;
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[2].y=75;
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[2].nSHideNomalCount=10;
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[2].nSHideNomalStart=0;
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[2].nShideNomalPlus=3;
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[2].nImageNumber=DP_TOWNGALL_TITLE;

	//���� ��ũ�� ��ư
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[3].nType=FT_HIDE_DSCROLL_AUTO_PUT;;
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[3].nWillDo=2;
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[3].x=139;
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[3].y=174;
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[3].nRectImage=417;
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[3].nImageNumber=418;
	SetRect(SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[3].rCheakBox,139,173,171,192);

	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[4].nType=FT_HIDE_ISCROLL_AUTO_PUT;;
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[4].nWillDo=2;
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[4].x=172;
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[4].y=174;
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[4].nRectImage=419;
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[4].nImageNumber=420;
	SetRect(SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[4].rCheakBox,172,173,208,192);

	// �Խ��� �޴� ����
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[5].nType=FT_BBS_GO;
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[5].nSHideNomalCount = 0;		// ���� â�� ��ġ
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[5].nSHideNomalStart = 2;		// ��ũ�ѵɰ��� �ʵ� ��ȣ
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[5].nShideNomalPlus = 8;	// ��¥ �̸��� ǥ���� ���� �ʵ� ��ȣ
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[5].x=36;
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[5].y=67;
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[5].nRectImage=16;
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[5].nImageNumber=16;
	SetRect(SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[5].rCheakBox,36,67,200,96 );
	
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[6].nType=FT_BBS_GO;
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[6].nSHideNomalCount = 1;		// ���� â�� ��ġ
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[6].nSHideNomalStart = 2;		// ��ũ�ѵ� ���� �ʵ� ��ȣ
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[6].nShideNomalPlus = 8;	// ��¥ �̸��� ǥ���� ���� �ʵ� ��ȣ
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[6].x=36;
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[6].y=97;
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[6].nRectImage=16;
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[6].nImageNumber=16;
	SetRect(SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[6].rCheakBox,36,97,200,126);

	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[7].nType=FT_BBS_GO;
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[7].nSHideNomalCount = 2;
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[7].nSHideNomalStart = 2;
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[7].nShideNomalPlus = 8;	// ��¥ �̸��� ǥ���� ���� �ʵ� ��ȣ
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[7].x=36;
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[7].y=127;
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[7].nRectImage=16;
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[7].nImageNumber=16;
	SetRect(SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[7].rCheakBox,36,127,200,156);

	// ��¥ �̸�...
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[8].nType=FT_NO_CHECK;
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[8].nWillDo=160;
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[8].x=42;
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[8].y=227;
	SMenu[MN_TOWNHALL_PUBLICNOTICE].nField[8].nImageNumber=DP_TOWNGALL_TITLE_DATE;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//TOWNHALL���� BUILDING�޴�

	SMenu[MN_TOWNHALL_BUILDING].x=366+GABX_SCREEN;
	SMenu[MN_TOWNHALL_BUILDING].y=20;
	SMenu[MN_TOWNHALL_BUILDING].nImageNumber=405;
	SMenu[MN_TOWNHALL_BUILDING].nFieldCount=4;
	SMenu[MN_TOWNHALL_BUILDING].CheakType=0;

	//EXIT
	SMenu[MN_TOWNHALL_BUILDING].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL_BUILDING].nField[0].nSpecialWillDo=SWD_SUBMENU_BACK;
	SMenu[MN_TOWNHALL_BUILDING].nField[0].x=92;
	SMenu[MN_TOWNHALL_BUILDING].nField[0].y=254;
	SMenu[MN_TOWNHALL_BUILDING].nField[0].nRectImage=431;
	SMenu[MN_TOWNHALL_BUILDING].nField[0].nImageNumber=432;
	SetRect(SMenu[MN_TOWNHALL_BUILDING].nField[0].rCheakBox,91,253,147,276);

	//construction
	SMenu[MN_TOWNHALL_BUILDING].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL_BUILDING].nField[1].nSpecialWillDo=SWD_WILLDO;
	SMenu[MN_TOWNHALL_BUILDING].nField[1].nWillDo=MN_TOWNHALL_CONSTRUCTION;
	SMenu[MN_TOWNHALL_BUILDING].nField[1].x=61;
	SMenu[MN_TOWNHALL_BUILDING].nField[1].y=86;
	SMenu[MN_TOWNHALL_BUILDING].nField[1].nRectImage=406;
	SMenu[MN_TOWNHALL_BUILDING].nField[1].nImageNumber=407;
	SetRect(SMenu[MN_TOWNHALL_BUILDING].nField[1].rCheakBox,61,86,176,104);

	//COMMERCE
	SMenu[MN_TOWNHALL_BUILDING].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL_BUILDING].nField[2].nSpecialWillDo=SWD_WILLDO;
	SMenu[MN_TOWNHALL_BUILDING].nField[2].nWillDo=MN_TOWNHALL_COMMERCE2;
	SMenu[MN_TOWNHALL_BUILDING].nField[2].x=61;
	SMenu[MN_TOWNHALL_BUILDING].nField[2].y=148;
	SMenu[MN_TOWNHALL_BUILDING].nField[2].nRectImage=408;
	SMenu[MN_TOWNHALL_BUILDING].nField[2].nImageNumber=409;
	SetRect(SMenu[MN_TOWNHALL_BUILDING].nField[2].rCheakBox,61,147,176,165);

	//reapir
	SMenu[MN_TOWNHALL_BUILDING].nField[3].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL_BUILDING].nField[3].nSpecialWillDo=SWD_WILLDO;
	SMenu[MN_TOWNHALL_BUILDING].nField[3].nWillDo=MN_TOWNHALL_HOUSEREPAIR;
	SMenu[MN_TOWNHALL_BUILDING].nField[3].nSHideNomalCount=4;					// ����� �޴����� �׸� ������ ���� �ʵ� ��ȣ
	SMenu[MN_TOWNHALL_BUILDING].nField[3].x=61;
	SMenu[MN_TOWNHALL_BUILDING].nField[3].y=212;
	SMenu[MN_TOWNHALL_BUILDING].nField[3].nRectImage=410;
	SMenu[MN_TOWNHALL_BUILDING].nField[3].nImageNumber=411;
	SetRect(SMenu[MN_TOWNHALL_BUILDING].nField[3].rCheakBox,61,212,176,230);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//TOWNHALL�޴����� COMMERCE1�޴�

	SMenu[MN_TOWNHALL_COMMERCE1].x=197;
	SMenu[MN_TOWNHALL_COMMERCE1].y=20;
	SMenu[MN_TOWNHALL_COMMERCE1].nImageNumber=421;
	SMenu[MN_TOWNHALL_COMMERCE1].nFieldCount=4;
	SMenu[MN_TOWNHALL_COMMERCE1].CheakType=0;

	//CNACEL
	SMenu[MN_TOWNHALL_COMMERCE1].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL_COMMERCE1].nField[0].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_TOWNHALL_COMMERCE1].nField[0].x=181;
	SMenu[MN_TOWNHALL_COMMERCE1].nField[0].y=256;
	SMenu[MN_TOWNHALL_COMMERCE1].nField[0].nRectImage=292;
	SMenu[MN_TOWNHALL_COMMERCE1].nField[0].nImageNumber=293;
	SetRect(SMenu[MN_TOWNHALL_COMMERCE1].nField[0].rCheakBox,180,255,220,295);

	//OK
	SMenu[MN_TOWNHALL_COMMERCE1].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL_COMMERCE1].nField[1].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_TOWNHALL_COMMERCE1].nField[1].x=22;
	SMenu[MN_TOWNHALL_COMMERCE1].nField[1].y=256;
	SMenu[MN_TOWNHALL_COMMERCE1].nField[1].nRectImage=294;
	SMenu[MN_TOWNHALL_COMMERCE1].nField[1].nImageNumber=295;
	SetRect(SMenu[MN_TOWNHALL_COMMERCE1].nField[1].rCheakBox,22,256,59,293);

	//���� ��ũ�� ��ư
	SMenu[MN_TOWNHALL_COMMERCE1].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL_COMMERCE1].nField[2].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_TOWNHALL_COMMERCE1].nField[2].x=98;
	SMenu[MN_TOWNHALL_COMMERCE1].nField[2].y=243;
	SMenu[MN_TOWNHALL_COMMERCE1].nField[2].nRectImage=427;
	SMenu[MN_TOWNHALL_COMMERCE1].nField[2].nImageNumber=428;
	SetRect(SMenu[MN_TOWNHALL_COMMERCE1].nField[2].rCheakBox,97,243,117,253);

	//������ ��ũ�� ��ư
	SMenu[MN_TOWNHALL_COMMERCE1].nField[3].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL_COMMERCE1].nField[3].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_TOWNHALL_COMMERCE1].nField[3].x=122;
	SMenu[MN_TOWNHALL_COMMERCE1].nField[3].y=243;
	SMenu[MN_TOWNHALL_COMMERCE1].nField[3].nRectImage=430;
	SMenu[MN_TOWNHALL_COMMERCE1].nField[3].nImageNumber=429;
	SetRect(SMenu[MN_TOWNHALL_COMMERCE1].nField[3].rCheakBox,121,243,141,253);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//TOWNHALL�޴����� COMMERCE2�޴�

	SMenu[MN_TOWNHALL_COMMERCE2].x=197;
	SMenu[MN_TOWNHALL_COMMERCE2].y=20;
	SMenu[MN_TOWNHALL_COMMERCE2].nImageNumber=422;
	SMenu[MN_TOWNHALL_COMMERCE2].nFieldCount=4;
	SMenu[MN_TOWNHALL_COMMERCE2].CheakType=0;

	//CANCEL
	SMenu[MN_TOWNHALL_COMMERCE2].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL_COMMERCE2].nField[0].nSpecialWillDo=SWD_SUBMENU_BACK;
	SMenu[MN_TOWNHALL_COMMERCE2].nField[0].x=181;
	SMenu[MN_TOWNHALL_COMMERCE2].nField[0].y=256;
	SMenu[MN_TOWNHALL_COMMERCE2].nField[0].nRectImage=292;
	SMenu[MN_TOWNHALL_COMMERCE2].nField[0].nImageNumber=293;
	SetRect(SMenu[MN_TOWNHALL_COMMERCE2].nField[0].rCheakBox,180,255,220,295);

	//OK
	SMenu[MN_TOWNHALL_COMMERCE2].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL_COMMERCE2].nField[1].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_TOWNHALL_COMMERCE2].nField[1].x=22;
	SMenu[MN_TOWNHALL_COMMERCE2].nField[1].y=256;
	SMenu[MN_TOWNHALL_COMMERCE2].nField[1].nRectImage=294;
	SMenu[MN_TOWNHALL_COMMERCE2].nField[1].nImageNumber=295;
	SetRect(SMenu[MN_TOWNHALL_COMMERCE2].nField[1].rCheakBox,22,256,59,293);

	//���� ��ũ�� ��ư
	SMenu[MN_TOWNHALL_COMMERCE2].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL_COMMERCE2].nField[2].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_TOWNHALL_COMMERCE2].nField[2].x=97;
	SMenu[MN_TOWNHALL_COMMERCE2].nField[2].y=280;
	SMenu[MN_TOWNHALL_COMMERCE2].nField[2].nRectImage=424;
	SMenu[MN_TOWNHALL_COMMERCE2].nField[2].nImageNumber=423;
	SetRect(SMenu[MN_TOWNHALL_COMMERCE2].nField[2].rCheakBox,96,280,116,290);

	//�Ʒ��� ��ũ�� ��ư
	SMenu[MN_TOWNHALL_COMMERCE2].nField[3].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL_COMMERCE2].nField[3].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_TOWNHALL_COMMERCE2].nField[3].x=121;
	SMenu[MN_TOWNHALL_COMMERCE2].nField[3].y=280;
	SMenu[MN_TOWNHALL_COMMERCE2].nField[3].nRectImage=426;
	SMenu[MN_TOWNHALL_COMMERCE2].nField[3].nImageNumber=425;
	SetRect(SMenu[MN_TOWNHALL_COMMERCE2].nField[3].rCheakBox,120,280,140,290);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//TOWNHALL�޴����� COMMERCE3�޴�

	SMenu[MN_TOWNHALL_COMMERCE3].x=193;
	SMenu[MN_TOWNHALL_COMMERCE3].y=75;
	SMenu[MN_TOWNHALL_COMMERCE3].nImageNumber=645;
	SMenu[MN_TOWNHALL_COMMERCE3].nFieldCount=2;
	SMenu[MN_TOWNHALL_COMMERCE3].CheakType=0;

	//OK
	SMenu[MN_TOWNHALL_COMMERCE3].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL_COMMERCE3].nField[0].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_TOWNHALL_COMMERCE3].nField[0].x=210;
	SMenu[MN_TOWNHALL_COMMERCE3].nField[0].y=18;
	SMenu[MN_TOWNHALL_COMMERCE3].nField[0].nRectImage=646;
	SMenu[MN_TOWNHALL_COMMERCE3].nField[0].nImageNumber=647;
	SetRect(SMenu[MN_TOWNHALL_COMMERCE3].nField[0].rCheakBox,208,17,238,47);

	//CANCEL
	SMenu[MN_TOWNHALL_COMMERCE3].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL_COMMERCE3].nField[1].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_TOWNHALL_COMMERCE3].nField[1].x=210;
	SMenu[MN_TOWNHALL_COMMERCE3].nField[1].y=67;
	SMenu[MN_TOWNHALL_COMMERCE3].nField[1].nRectImage=648;
	SMenu[MN_TOWNHALL_COMMERCE3].nField[1].nImageNumber=649;
	SetRect(SMenu[MN_TOWNHALL_COMMERCE3].nField[1].rCheakBox,208,66,238,96);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//TOWNHALL�޴����� COMMERCE4�޴�

	SMenu[MN_TOWNHALL_COMMERCE4].x=189;
	SMenu[MN_TOWNHALL_COMMERCE4].y=78;
	SMenu[MN_TOWNHALL_COMMERCE4].nImageNumber=706;
	SMenu[MN_TOWNHALL_COMMERCE4].nFieldCount=1;
	SMenu[MN_TOWNHALL_COMMERCE4].CheakType=0;

	//ok
	SMenu[MN_TOWNHALL_COMMERCE4].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL_COMMERCE4].nField[0].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_TOWNHALL_COMMERCE4].nField[0].x=210;
	SMenu[MN_TOWNHALL_COMMERCE4].nField[0].y=65;
	SMenu[MN_TOWNHALL_COMMERCE4].nField[0].nRectImage=646;
	SMenu[MN_TOWNHALL_COMMERCE4].nField[0].nImageNumber=647;
	SetRect(SMenu[MN_TOWNHALL_COMMERCE4].nField[0].rCheakBox,208,66,238,96);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//townhall�޴����� house repair�޴�

	SMenu[MN_TOWNHALL_HOUSEREPAIR].x=366+GABX_SCREEN;
	SMenu[MN_TOWNHALL_HOUSEREPAIR].y=20;
	SMenu[MN_TOWNHALL_HOUSEREPAIR].nImageNumber=650;
	SMenu[MN_TOWNHALL_HOUSEREPAIR].nFieldCount=5;
	SMenu[MN_TOWNHALL_HOUSEREPAIR].CheakType=0;

	//CNACEL
	SMenu[MN_TOWNHALL_HOUSEREPAIR].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL_HOUSEREPAIR].nField[0].nSpecialWillDo=SWD_SUBMENU_BACK;
	SMenu[MN_TOWNHALL_HOUSEREPAIR].nField[0].x=183;
	SMenu[MN_TOWNHALL_HOUSEREPAIR].nField[0].y=261;
	SMenu[MN_TOWNHALL_HOUSEREPAIR].nField[0].nRectImage=292;
	SMenu[MN_TOWNHALL_HOUSEREPAIR].nField[0].nImageNumber=293;
	SetRect(SMenu[MN_TOWNHALL_HOUSEREPAIR].nField[0].rCheakBox,180,255,220,295);

	//OK
	SMenu[MN_TOWNHALL_HOUSEREPAIR].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL_HOUSEREPAIR].nField[1].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_TOWNHALL_HOUSEREPAIR].nField[1].x=24;
	SMenu[MN_TOWNHALL_HOUSEREPAIR].nField[1].y=261;
	SMenu[MN_TOWNHALL_HOUSEREPAIR].nField[1].nRectImage=294;
	SMenu[MN_TOWNHALL_HOUSEREPAIR].nField[1].nImageNumber=295;
	SetRect(SMenu[MN_TOWNHALL_HOUSEREPAIR].nField[1].rCheakBox,22,256,59,293);

	//���� ��ũ�� ��ư
	SMenu[MN_TOWNHALL_HOUSEREPAIR].nField[2].nType=FT_HIDE_DSCROLL_AUTO_PUT;
	SMenu[MN_TOWNHALL_HOUSEREPAIR].nField[2].nWillDo=4;
	SMenu[MN_TOWNHALL_HOUSEREPAIR].nField[2].x=91;
	SMenu[MN_TOWNHALL_HOUSEREPAIR].nField[2].y=186;
	SMenu[MN_TOWNHALL_HOUSEREPAIR].nField[2].nRectImage=651;
	SMenu[MN_TOWNHALL_HOUSEREPAIR].nField[2].nImageNumber=652;
	SetRect(SMenu[MN_TOWNHALL_HOUSEREPAIR].nField[2].rCheakBox,90,185,117,203);

	//������ ��ũ�� ��ư
	SMenu[MN_TOWNHALL_HOUSEREPAIR].nField[3].nType=FT_HIDE_ISCROLL_AUTO_PUT;
	SMenu[MN_TOWNHALL_HOUSEREPAIR].nField[3].nWillDo=4;
	SMenu[MN_TOWNHALL_HOUSEREPAIR].nField[3].x=120;
	SMenu[MN_TOWNHALL_HOUSEREPAIR].nField[3].y=186;
	SMenu[MN_TOWNHALL_HOUSEREPAIR].nField[3].nRectImage=653;
	SMenu[MN_TOWNHALL_HOUSEREPAIR].nField[3].nImageNumber=654;
	SetRect(SMenu[MN_TOWNHALL_HOUSEREPAIR].nField[3].rCheakBox,120,185,147,203);

	// ��ĥ �� ǥ��
	SMenu[MN_TOWNHALL_HOUSEREPAIR].nField[4].nType=FT_DATA_PUT;
	SMenu[MN_TOWNHALL_HOUSEREPAIR].nField[4].x=34;
	SMenu[MN_TOWNHALL_HOUSEREPAIR].nField[4].y=64;
	SMenu[MN_TOWNHALL_HOUSEREPAIR].nField[4].nSHideNomalCount=0;
	SMenu[MN_TOWNHALL_HOUSEREPAIR].nField[4].nSHideNomalStart=0;
	SMenu[MN_TOWNHALL_HOUSEREPAIR].nField[4].nShideNomalPlus=1;
	SMenu[MN_TOWNHALL_HOUSEREPAIR].nField[4].nImageNumber=DP_HOUSE_REPAIR;





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//townhall�޴����� organize guild�޴�

	SMenu[MN_TOWNHALL_GUILD].x=366+GABX_SCREEN;
	SMenu[MN_TOWNHALL_GUILD].y=20;
	SMenu[MN_TOWNHALL_GUILD].nImageNumber=655;
	SMenu[MN_TOWNHALL_GUILD].nFieldCount=2;
	SMenu[MN_TOWNHALL_GUILD].CheakType=0;

	//OK
	SMenu[MN_TOWNHALL_GUILD].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL_GUILD].nField[0].nSpecialWillDo=SWD_SUBMENU_BACK;
	SMenu[MN_TOWNHALL_GUILD].nField[0].x=31;
	SMenu[MN_TOWNHALL_GUILD].nField[0].y=239;
	SMenu[MN_TOWNHALL_GUILD].nField[0].nRectImage=656;
	SMenu[MN_TOWNHALL_GUILD].nField[0].nImageNumber=657;
	SetRect(SMenu[MN_TOWNHALL_GUILD].nField[0].rCheakBox,31,239,84,284);

	//CANCEL
	SMenu[MN_TOWNHALL_GUILD].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL_GUILD].nField[1].nSpecialWillDo=SWD_SUBMENU_BACK;
	SMenu[MN_TOWNHALL_GUILD].nField[1].x=157;
	SMenu[MN_TOWNHALL_GUILD].nField[1].y=239;
	SMenu[MN_TOWNHALL_GUILD].nField[1].nRectImage=658;
	SMenu[MN_TOWNHALL_GUILD].nField[1].nImageNumber=659;
	SetRect(SMenu[MN_TOWNHALL_GUILD].nField[1].rCheakBox,157,239,210,284);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//townhall�޴����� forum�޴�

	SMenu[MN_TOWNHALL_FORUM].x=366+GABX_SCREEN;
	SMenu[MN_TOWNHALL_FORUM].y=20;
	SMenu[MN_TOWNHALL_FORUM].nImageNumber=660;
	SMenu[MN_TOWNHALL_FORUM].nFieldCount=10;

	//write
	SMenu[MN_TOWNHALL_FORUM].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL_FORUM].nField[0].nSpecialWillDo=SWD_GO_BBS_WRITE;
//	SMenu[MN_TOWNHALL_FORUM].nField[0].nSpecialWillDo=SWD_SORRY;
	SMenu[MN_TOWNHALL_FORUM].nField[0].nWillDo=MN_TOWNHALL_BBS3;
	SMenu[MN_TOWNHALL_FORUM].nField[0].x=21;
	SMenu[MN_TOWNHALL_FORUM].nField[0].y=253;
	SMenu[MN_TOWNHALL_FORUM].nField[0].nRectImage=661;
	SMenu[MN_TOWNHALL_FORUM].nField[0].nImageNumber=662;
	SetRect(SMenu[MN_TOWNHALL_FORUM].nField[0].rCheakBox,22,253,80,298);

	//read
	SMenu[MN_TOWNHALL_FORUM].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL_FORUM].nField[1].nSpecialWillDo=SWD_BBS_READ;
	SMenu[MN_TOWNHALL_FORUM].nField[1].nWillDo=MN_TOWNHALL_NOTICE1;
	SMenu[MN_TOWNHALL_FORUM].nField[1].nSHideNomalCount = 3; // �Ʒ� �ʵ带 ����Ŵ
	SMenu[MN_TOWNHALL_FORUM].nField[1].x=91;
	SMenu[MN_TOWNHALL_FORUM].nField[1].y=253;
	SMenu[MN_TOWNHALL_FORUM].nField[1].nRectImage=663;
	SMenu[MN_TOWNHALL_FORUM].nField[1].nImageNumber=664;
	SetRect(SMenu[MN_TOWNHALL_FORUM].nField[1].rCheakBox,92,253,156,298);

	//exit
	SMenu[MN_TOWNHALL_FORUM].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL_FORUM].nField[2].nSpecialWillDo=SWD_SUBMENU_BACK;
	SMenu[MN_TOWNHALL_FORUM].nField[2].x=161;
	SMenu[MN_TOWNHALL_FORUM].nField[2].y=253;
	SMenu[MN_TOWNHALL_FORUM].nField[2].nRectImage=665;
	SMenu[MN_TOWNHALL_FORUM].nField[2].nImageNumber=666;
	SetRect(SMenu[MN_TOWNHALL_FORUM].nField[2].rCheakBox,162,253,220,298);

	// �������� ���� ���		// �� �� �ʵ�� ���� �ٳ�� �Ѵ�. �������
	SMenu[MN_TOWNHALL_FORUM].nField[3].nType=FT_DATA_PUT;
	SMenu[MN_TOWNHALL_FORUM].nField[3].nWillDo=160;						// ����
	SMenu[MN_TOWNHALL_FORUM].nField[3].nSpecialWillDo=32;				// ���� �׸���� �Ÿ�
	SMenu[MN_TOWNHALL_FORUM].nField[3].x=44;
	SMenu[MN_TOWNHALL_FORUM].nField[3].y=68;
	SMenu[MN_TOWNHALL_FORUM].nField[3].nSHideNomalCount=10;
	SMenu[MN_TOWNHALL_FORUM].nField[3].nSHideNomalStart=0;
	SMenu[MN_TOWNHALL_FORUM].nField[3].nShideNomalPlus=1;
	SMenu[MN_TOWNHALL_FORUM].nField[3].nImageNumber=DP_TOWNGALL_TITLE;

	//���� ��ũ��
	SMenu[MN_TOWNHALL_FORUM].nField[4].nType=FT_HIDE_DSCROLL_AUTO_PUT;
	SMenu[MN_TOWNHALL_FORUM].nField[4].nWillDo=3;
	SMenu[MN_TOWNHALL_FORUM].nField[4].x=162;
	SMenu[MN_TOWNHALL_FORUM].nField[4].y=171;
	SMenu[MN_TOWNHALL_FORUM].nField[4].nRectImage=667;
	SMenu[MN_TOWNHALL_FORUM].nField[4].nImageNumber=668;
	SetRect(SMenu[MN_TOWNHALL_FORUM].nField[4].rCheakBox,162,170,189,188);

	//�Ʒ��� ��ũ��
	SMenu[MN_TOWNHALL_FORUM].nField[5].nType=FT_HIDE_ISCROLL_AUTO_PUT;
	SMenu[MN_TOWNHALL_FORUM].nField[5].nWillDo=3;
	SMenu[MN_TOWNHALL_FORUM].nField[5].x=192;
	SMenu[MN_TOWNHALL_FORUM].nField[5].y=171;
	SMenu[MN_TOWNHALL_FORUM].nField[5].nRectImage=669;
	SMenu[MN_TOWNHALL_FORUM].nField[5].nImageNumber=670;
	SetRect(SMenu[MN_TOWNHALL_FORUM].nField[5].rCheakBox,191,170,218,188);

	// �Խ��� �޴� ����
	SMenu[MN_TOWNHALL_FORUM].nField[6].nType=FT_BBS_GO;
	SMenu[MN_TOWNHALL_FORUM].nField[6].nSHideNomalCount = 0;		// ���� â�� ��ġ
	SMenu[MN_TOWNHALL_FORUM].nField[6].nSHideNomalStart = 3;		// ��ũ�ѵɰ��� �ʵ� ��ȣ
	SMenu[MN_TOWNHALL_FORUM].nField[6].nShideNomalPlus = 9;	// ��¥ �̸��� ǥ���� ���� �ʵ� ��ȣ
	SMenu[MN_TOWNHALL_FORUM].nField[6].x=36;
	SMenu[MN_TOWNHALL_FORUM].nField[6].y=60;
	SMenu[MN_TOWNHALL_FORUM].nField[6].nRectImage=16;
	SMenu[MN_TOWNHALL_FORUM].nField[6].nImageNumber=16;
	SetRect(SMenu[MN_TOWNHALL_FORUM].nField[6].rCheakBox,36,60,207,90 );
	
	SMenu[MN_TOWNHALL_FORUM].nField[7].nType=FT_BBS_GO;
	SMenu[MN_TOWNHALL_FORUM].nField[7].nSHideNomalCount = 1;		// ���� â�� ��ġ
	SMenu[MN_TOWNHALL_FORUM].nField[7].nSHideNomalStart = 3;		// ��ũ�ѵ� ���� �ʵ� ��ȣ
	SMenu[MN_TOWNHALL_FORUM].nField[7].nShideNomalPlus = 9;	// ��¥ �̸��� ǥ���� ���� �ʵ� ��ȣ
	SMenu[MN_TOWNHALL_FORUM].nField[7].x=36;
	SMenu[MN_TOWNHALL_FORUM].nField[7].y=92;
	SMenu[MN_TOWNHALL_FORUM].nField[7].nRectImage=16;
	SMenu[MN_TOWNHALL_FORUM].nField[7].nImageNumber=16;
	SetRect(SMenu[MN_TOWNHALL_FORUM].nField[7].rCheakBox,36,92,207,122);

	SMenu[MN_TOWNHALL_FORUM].nField[8].nType=FT_BBS_GO;
	SMenu[MN_TOWNHALL_FORUM].nField[8].nSHideNomalCount = 2;
	SMenu[MN_TOWNHALL_FORUM].nField[8].nSHideNomalStart = 3;
	SMenu[MN_TOWNHALL_FORUM].nField[8].nShideNomalPlus = 9;	// ��¥ �̸��� ǥ���� ���� �ʵ� ��ȣ
	SMenu[MN_TOWNHALL_FORUM].nField[8].x=36;
	SMenu[MN_TOWNHALL_FORUM].nField[8].y=124;
	SMenu[MN_TOWNHALL_FORUM].nField[8].nRectImage=16;
	SMenu[MN_TOWNHALL_FORUM].nField[8].nImageNumber=16;
	SetRect(SMenu[MN_TOWNHALL_FORUM].nField[8].rCheakBox,36,124,207,154);

	// ��¥ �̸�...
	SMenu[MN_TOWNHALL_FORUM].nField[9].nType=FT_NO_CHECK;
	SMenu[MN_TOWNHALL_FORUM].nField[9].nWillDo=160;
	SMenu[MN_TOWNHALL_FORUM].nField[9].x=41;
	SMenu[MN_TOWNHALL_FORUM].nField[9].y=204;
	SMenu[MN_TOWNHALL_FORUM].nField[9].nImageNumber=DP_TOWNGALL_TITLE_DATE;





/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//townhall�޴����� event�޴�

	SMenu[MN_TOWNHALL_EVENT].x=366+GABX_SCREEN;
	SMenu[MN_TOWNHALL_EVENT].y=20;
	SMenu[MN_TOWNHALL_EVENT].nImageNumber=671;
	SMenu[MN_TOWNHALL_EVENT].nFieldCount=9;
	SMenu[MN_TOWNHALL_EVENT].CheakType=0;

	//exit
	SMenu[MN_TOWNHALL_EVENT].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL_EVENT].nField[0].nSpecialWillDo=SWD_SUBMENU_BACK ;
	SMenu[MN_TOWNHALL_EVENT].nField[0].x=161;
	SMenu[MN_TOWNHALL_EVENT].nField[0].y=253;
	SMenu[MN_TOWNHALL_EVENT].nField[0].nRectImage=665;
	SMenu[MN_TOWNHALL_EVENT].nField[0].nImageNumber=666;
	SetRect(SMenu[MN_TOWNHALL_EVENT].nField[0].rCheakBox,162,253,220,298);

	//read
	SMenu[MN_TOWNHALL_EVENT].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL_EVENT].nField[1].nSpecialWillDo=SWD_BBS_READ;
	SMenu[MN_TOWNHALL_EVENT].nField[1].nWillDo=MN_TOWNHALL_BBS2;
	SMenu[MN_TOWNHALL_EVENT].nField[1].nSHideNomalCount = 2; // �Ʒ� �ʵ带 ����Ŵ
	SMenu[MN_TOWNHALL_EVENT].nField[1].x=21;
	SMenu[MN_TOWNHALL_EVENT].nField[1].y=253;
	SMenu[MN_TOWNHALL_EVENT].nField[1].nRectImage=663;
	SMenu[MN_TOWNHALL_EVENT].nField[1].nImageNumber=664;
	SetRect(SMenu[MN_TOWNHALL_EVENT].nField[1].rCheakBox,22,253,80,298);

	// �������� ���� ���		// �Ʒ� �� �ʵ�� ���� �ٳ�� �Ѵ�. �������
	SMenu[MN_TOWNHALL_EVENT].nField[2].nType=FT_DATA_PUT;
	SMenu[MN_TOWNHALL_EVENT].nField[2].nWillDo=160;
	SMenu[MN_TOWNHALL_EVENT].nField[2].nSpecialWillDo=31;
	SMenu[MN_TOWNHALL_EVENT].nField[2].x=43;
	SMenu[MN_TOWNHALL_EVENT].nField[2].y=68;
	SMenu[MN_TOWNHALL_EVENT].nField[2].nSHideNomalCount=10;
	SMenu[MN_TOWNHALL_EVENT].nField[2].nSHideNomalStart=0;
	SMenu[MN_TOWNHALL_EVENT].nField[2].nShideNomalPlus=1;
	SMenu[MN_TOWNHALL_EVENT].nField[2].nImageNumber=DP_TOWNGALL_TITLE;
	
	//���� ��ũ�� ��ư
	SMenu[MN_TOWNHALL_EVENT].nField[3].nType=FT_HIDE_DSCROLL_AUTO_PUT;
	SMenu[MN_TOWNHALL_EVENT].nField[3].nWillDo=2;
	SMenu[MN_TOWNHALL_EVENT].nField[3].x=162;
	SMenu[MN_TOWNHALL_EVENT].nField[3].y=171;
	SMenu[MN_TOWNHALL_EVENT].nField[3].nRectImage=667;
	SMenu[MN_TOWNHALL_EVENT].nField[3].nImageNumber=668;
	SetRect(SMenu[MN_TOWNHALL_EVENT].nField[3].rCheakBox,162,170,189,188);

	//�Ʒ��� ��ũ�� ��ư
	SMenu[MN_TOWNHALL_EVENT].nField[4].nType=FT_HIDE_ISCROLL_AUTO_PUT;
	SMenu[MN_TOWNHALL_EVENT].nField[4].nWillDo=2;
	SMenu[MN_TOWNHALL_EVENT].nField[4].x=192;
	SMenu[MN_TOWNHALL_EVENT].nField[4].y=171;
	SMenu[MN_TOWNHALL_EVENT].nField[4].nRectImage=669;
	SMenu[MN_TOWNHALL_EVENT].nField[4].nImageNumber=670;
	SetRect(SMenu[MN_TOWNHALL_EVENT].nField[4].rCheakBox,191,170,218,188);

	// �Խ��� �޴� ����
	SMenu[MN_TOWNHALL_EVENT].nField[5].nType=FT_BBS_GO;
	SMenu[MN_TOWNHALL_EVENT].nField[5].nSHideNomalCount = 0;		// ���� â�� ��ġ
	SMenu[MN_TOWNHALL_EVENT].nField[5].nSHideNomalStart = 2;		// ��ũ�ѵɰ��� �ʵ� ��ȣ
	SMenu[MN_TOWNHALL_EVENT].nField[5].nShideNomalPlus = 8;	// ��¥ �̸��� ǥ���� ���� �ʵ� ��ȣ
	SMenu[MN_TOWNHALL_EVENT].nField[5].x=37;
	SMenu[MN_TOWNHALL_EVENT].nField[5].y=61;
	SMenu[MN_TOWNHALL_EVENT].nField[5].nRectImage=16;
	SMenu[MN_TOWNHALL_EVENT].nField[5].nImageNumber=16;
	SetRect(SMenu[MN_TOWNHALL_EVENT].nField[5].rCheakBox,37,61,208,91 );
	
	SMenu[MN_TOWNHALL_EVENT].nField[6].nType=FT_BBS_GO;
	SMenu[MN_TOWNHALL_EVENT].nField[6].nSHideNomalCount = 1;		// ���� â�� ��ġ
	SMenu[MN_TOWNHALL_EVENT].nField[6].nSHideNomalStart = 2;		// ��ũ�ѵ� ���� �ʵ� ��ȣ
	SMenu[MN_TOWNHALL_EVENT].nField[6].nShideNomalPlus = 8;	// ��¥ �̸��� ǥ���� ���� �ʵ� ��ȣ
	SMenu[MN_TOWNHALL_EVENT].nField[6].x=37;
	SMenu[MN_TOWNHALL_EVENT].nField[6].y=92;
	SMenu[MN_TOWNHALL_EVENT].nField[6].nRectImage=16;
	SMenu[MN_TOWNHALL_EVENT].nField[6].nImageNumber=16;
	SetRect(SMenu[MN_TOWNHALL_EVENT].nField[6].rCheakBox,37,92,208,122);

	SMenu[MN_TOWNHALL_EVENT].nField[7].nType=FT_BBS_GO;
	SMenu[MN_TOWNHALL_EVENT].nField[7].nSHideNomalCount = 2;
	SMenu[MN_TOWNHALL_EVENT].nField[7].nSHideNomalStart = 2;
	SMenu[MN_TOWNHALL_EVENT].nField[7].nShideNomalPlus = 8;	// ��¥ �̸��� ǥ���� ���� �ʵ� ��ȣ
	SMenu[MN_TOWNHALL_EVENT].nField[7].x=37;
	SMenu[MN_TOWNHALL_EVENT].nField[7].y=123;
	SMenu[MN_TOWNHALL_EVENT].nField[7].nRectImage=16;
	SMenu[MN_TOWNHALL_EVENT].nField[7].nImageNumber=16;
	SetRect(SMenu[MN_TOWNHALL_EVENT].nField[7].rCheakBox,37,123,208,153);

	// ��¥ �̸�...
	SMenu[MN_TOWNHALL_EVENT].nField[8].nType=FT_NO_CHECK;
	SMenu[MN_TOWNHALL_EVENT].nField[8].nWillDo=154;
	SMenu[MN_TOWNHALL_EVENT].nField[8].x=40;
	SMenu[MN_TOWNHALL_EVENT].nField[8].y=202;
	SMenu[MN_TOWNHALL_EVENT].nField[8].nImageNumber=DP_TOWNGALL_TITLE_DATE;




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//townhall�޴��� rank1�޴�

	SMenu[MN_TOWNHALL_RANK1].x=366+GABX_SCREEN;
	SMenu[MN_TOWNHALL_RANK1].y=3;
	SMenu[MN_TOWNHALL_RANK1].nImageNumber=672;
	SMenu[MN_TOWNHALL_RANK1].nFieldCount=9;
	SMenu[MN_TOWNHALL_RANK1].CheakType=0;

	// �������� ���� ���		// �Ʒ� �� �ʵ�� ���� �ٳ�� �Ѵ�. �������
	SMenu[MN_TOWNHALL_RANK1].nField[0].nType=FT_DATA_PUT;
	SMenu[MN_TOWNHALL_RANK1].nField[0].nWillDo=160;
	SMenu[MN_TOWNHALL_RANK1].nField[0].nSpecialWillDo=35;
	SMenu[MN_TOWNHALL_RANK1].nField[0].x=43;
	SMenu[MN_TOWNHALL_RANK1].nField[0].y=105;
	SMenu[MN_TOWNHALL_RANK1].nField[0].nSHideNomalCount=10;
	SMenu[MN_TOWNHALL_RANK1].nField[0].nSHideNomalStart=0;
	SMenu[MN_TOWNHALL_RANK1].nField[0].nShideNomalPlus=3;
	SMenu[MN_TOWNHALL_RANK1].nField[0].nImageNumber=DP_TOWNGALL_TITLE;

	//���� ��ũ�� ��ư
	SMenu[MN_TOWNHALL_RANK1].nField[1].nType=FT_HIDE_DSCROLL_AUTO_PUT;
	SMenu[MN_TOWNHALL_RANK1].nField[1].nWillDo=0;
	SMenu[MN_TOWNHALL_RANK1].nField[1].x=97;
	SMenu[MN_TOWNHALL_RANK1].nField[1].y=204;
	SMenu[MN_TOWNHALL_RANK1].nField[1].nRectImage=673;
	SMenu[MN_TOWNHALL_RANK1].nField[1].nImageNumber=674;
	SetRect(SMenu[MN_TOWNHALL_RANK1].nField[1].rCheakBox,97,204,129,224);

	//�Ʒ��� ��ũ�� ��ư
	SMenu[MN_TOWNHALL_RANK1].nField[2].nType=FT_HIDE_ISCROLL_AUTO_PUT;
	SMenu[MN_TOWNHALL_RANK1].nField[2].nWillDo=0;
	SMenu[MN_TOWNHALL_RANK1].nField[2].x=132;
	SMenu[MN_TOWNHALL_RANK1].nField[2].y=204;
	SMenu[MN_TOWNHALL_RANK1].nField[2].nRectImage=675;
	SMenu[MN_TOWNHALL_RANK1].nField[2].nImageNumber=676;
	SetRect(SMenu[MN_TOWNHALL_RANK1].nField[2].rCheakBox,132,204,164,224);

	//ok
	SMenu[MN_TOWNHALL_RANK1].nField[3].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL_RANK1].nField[3].nSpecialWillDo=SWD_BBS_READ;
	SMenu[MN_TOWNHALL_RANK1].nField[3].nWillDo=MN_TOWNHALL_RANK2;	
	SMenu[MN_TOWNHALL_RANK1].nField[3].nSHideNomalCount = 0; // DATA_PUT �ʵ带 ����Ŵ
	SMenu[MN_TOWNHALL_RANK1].nField[3].x=58;
	SMenu[MN_TOWNHALL_RANK1].nField[3].y=275;
	SMenu[MN_TOWNHALL_RANK1].nField[3].nRectImage=677;
	SMenu[MN_TOWNHALL_RANK1].nField[3].nImageNumber=678;
	SetRect(SMenu[MN_TOWNHALL_RANK1].nField[3].rCheakBox,58,275,92,309);

	//cancel
	SMenu[MN_TOWNHALL_RANK1].nField[4].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL_RANK1].nField[4].nSpecialWillDo=SWD_SUBMENU_BACK;
	SMenu[MN_TOWNHALL_RANK1].nField[4].x=168;
	SMenu[MN_TOWNHALL_RANK1].nField[4].y=275;
	SMenu[MN_TOWNHALL_RANK1].nField[4].nRectImage=679;
	SMenu[MN_TOWNHALL_RANK1].nField[4].nImageNumber=680;
	SetRect(SMenu[MN_TOWNHALL_RANK1].nField[4].rCheakBox,168,275,202,309);

	// �Խ��� �޴� ����
	SMenu[MN_TOWNHALL_RANK1].nField[5].nType=FT_BBS_GO;
	SMenu[MN_TOWNHALL_RANK1].nField[5].nSHideNomalCount = 0;		// ���� â�� ��ġ
	SMenu[MN_TOWNHALL_RANK1].nField[5].nSHideNomalStart = 0;		// ��ũ�ѵɰ��� �ʵ� ��ȣ
	SMenu[MN_TOWNHALL_RANK1].nField[5].nShideNomalPlus = 8;	// ��¥ �̸��� ǥ���� ���� �ʵ� ��ȣ
	SMenu[MN_TOWNHALL_RANK1].nField[5].x=40;
	SMenu[MN_TOWNHALL_RANK1].nField[5].y=97;
	SMenu[MN_TOWNHALL_RANK1].nField[5].nRectImage=16;
	SMenu[MN_TOWNHALL_RANK1].nField[5].nImageNumber=16;
	SetRect(SMenu[MN_TOWNHALL_RANK1].nField[5].rCheakBox,40,97,211,127 );
	
	SMenu[MN_TOWNHALL_RANK1].nField[6].nType=FT_BBS_GO;
	SMenu[MN_TOWNHALL_RANK1].nField[6].nSHideNomalCount = 1;		// ���� â�� ��ġ
	SMenu[MN_TOWNHALL_RANK1].nField[6].nSHideNomalStart = 0;		// ��ũ�ѵ� ���� �ʵ� ��ȣ
	SMenu[MN_TOWNHALL_RANK1].nField[6].nShideNomalPlus = 8;	// ��¥ �̸��� ǥ���� ���� �ʵ� ��ȣ
	SMenu[MN_TOWNHALL_RANK1].nField[6].x=40;
	SMenu[MN_TOWNHALL_RANK1].nField[6].y=132;
	SMenu[MN_TOWNHALL_RANK1].nField[6].nRectImage=16;
	SMenu[MN_TOWNHALL_RANK1].nField[6].nImageNumber=16;
	SetRect(SMenu[MN_TOWNHALL_RANK1].nField[6].rCheakBox,40,132,211,162);

	SMenu[MN_TOWNHALL_RANK1].nField[7].nType=FT_BBS_GO;
	SMenu[MN_TOWNHALL_RANK1].nField[7].nSHideNomalCount = 2;
	SMenu[MN_TOWNHALL_RANK1].nField[7].nSHideNomalStart = 0;
	SMenu[MN_TOWNHALL_RANK1].nField[7].nShideNomalPlus = 8;	// ��¥ �̸��� ǥ���� ���� �ʵ� ��ȣ
	SMenu[MN_TOWNHALL_RANK1].nField[7].x=40;
	SMenu[MN_TOWNHALL_RANK1].nField[7].y=167;
	SMenu[MN_TOWNHALL_RANK1].nField[7].nRectImage=16;
	SMenu[MN_TOWNHALL_RANK1].nField[7].nImageNumber=16;
	SetRect(SMenu[MN_TOWNHALL_RANK1].nField[7].rCheakBox,40,167,211,197);

	// ��¥ �̸�...
	SMenu[MN_TOWNHALL_RANK1].nField[8].nType=FT_NO_CHECK;
	SMenu[MN_TOWNHALL_RANK1].nField[8].nWillDo=180;
	SMenu[MN_TOWNHALL_RANK1].nField[8].x=37;
	SMenu[MN_TOWNHALL_RANK1].nField[8].y=241;
	SMenu[MN_TOWNHALL_RANK1].nField[8].nImageNumber=DP_TOWNGALL_TITLE_DATE;




/////////////////////////////////////////////////////////////////////////////////////////////////
	//townhall�޴��� rank2�޴�

	SMenu[MN_TOWNHALL_RANK2].x=162;
	SMenu[MN_TOWNHALL_RANK2].y=46;
	SMenu[MN_TOWNHALL_RANK2].nImageNumber=681;
	SMenu[MN_TOWNHALL_RANK2].nFieldCount=4;
	SMenu[MN_TOWNHALL_RANK2].CheakType=0;
	
	// ���ڿ� ���
	SMenu[MN_TOWNHALL_RANK2].nField[0].nType=FT_DATA_PUT;
	SMenu[MN_TOWNHALL_RANK2].nField[0].x=64;
	SMenu[MN_TOWNHALL_RANK2].nField[0].y=73;
	SMenu[MN_TOWNHALL_RANK2].nField[0].nWillDo=100;
	SMenu[MN_TOWNHALL_RANK2].nField[0].nRectImage=158;
	SMenu[MN_TOWNHALL_RANK2].nField[0].nSHideNomalCount=1;
	SMenu[MN_TOWNHALL_RANK2].nField[0].nSHideNomalStart=0;
	SMenu[MN_TOWNHALL_RANK2].nField[0].nShideNomalPlus=1;	
	SMenu[MN_TOWNHALL_RANK2].nField[0].nImageNumber=DP_BBS_PUT;
	
	//���� ��ũ�� ��ư
	SMenu[MN_TOWNHALL_RANK2].nField[1].nType=FT_HIDE_DSCROLL_AUTO_PUT;
	SMenu[MN_TOWNHALL_RANK2].nField[1].nWillDo=0;
	SMenu[MN_TOWNHALL_RANK2].nField[1].x=282;
	SMenu[MN_TOWNHALL_RANK2].nField[1].y=82;
	SMenu[MN_TOWNHALL_RANK2].nField[1].nRectImage=682;
	SMenu[MN_TOWNHALL_RANK2].nField[1].nImageNumber=683;
	SetRect(SMenu[MN_TOWNHALL_RANK2].nField[1].rCheakBox,282,82,301,114);

	//�Ʒ��� ��ũ�� ��ư
	SMenu[MN_TOWNHALL_RANK2].nField[2].nType=FT_HIDE_ISCROLL_AUTO_PUT;
	SMenu[MN_TOWNHALL_RANK2].nField[2].nWillDo=0;
	SMenu[MN_TOWNHALL_RANK2].nField[2].x=282;
	SMenu[MN_TOWNHALL_RANK2].nField[2].y=127;
	SMenu[MN_TOWNHALL_RANK2].nField[2].nRectImage=684;
	SMenu[MN_TOWNHALL_RANK2].nField[2].nImageNumber=685;
	SetRect(SMenu[MN_TOWNHALL_RANK2].nField[2].rCheakBox,282,127,301,159);

	//exit
	SMenu[MN_TOWNHALL_RANK2].nField[3].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL_RANK2].nField[3].nSpecialWillDo=SWD_SUBMENU_BACK;
	SMenu[MN_TOWNHALL_RANK2].nField[3].x=266;
	SMenu[MN_TOWNHALL_RANK2].nField[3].y=182;
	SMenu[MN_TOWNHALL_RANK2].nField[3].nRectImage=686;
	SMenu[MN_TOWNHALL_RANK2].nField[3].nImageNumber=687;
	SetRect(SMenu[MN_TOWNHALL_RANK2].nField[3].rCheakBox,266,182,319,227);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//townhall�޴��� notice4�޴�

	SMenu[MN_TOWNHALL_NOTICE4].x=96;
	SMenu[MN_TOWNHALL_NOTICE4].y=38;
	SMenu[MN_TOWNHALL_NOTICE4].nImageNumber=688;
	SMenu[MN_TOWNHALL_NOTICE4].nFieldCount=5;
	SMenu[MN_TOWNHALL_NOTICE4].CheakType=0;

	// ������ ���
	SMenu[MN_TOWNHALL_NOTICE4].nField[0].nType=FT_DATA_PUT;
	SMenu[MN_TOWNHALL_NOTICE4].nField[0].x=59;
	SMenu[MN_TOWNHALL_NOTICE4].nField[0].y=42;
	SMenu[MN_TOWNHALL_NOTICE4].nField[0].nWillDo=180;
	SMenu[MN_TOWNHALL_NOTICE4].nField[0].nRectImage=280;
	SMenu[MN_TOWNHALL_NOTICE4].nField[0].nSHideNomalCount=1;
	SMenu[MN_TOWNHALL_NOTICE4].nField[0].nSHideNomalStart=0;
	SMenu[MN_TOWNHALL_NOTICE4].nField[0].nShideNomalPlus=1;	
	SMenu[MN_TOWNHALL_NOTICE4].nField[0].nImageNumber=DP_BBS_PUT;

	//preview
	SMenu[MN_TOWNHALL_NOTICE4].nField[1].nType=FT_HIDE_DSCROLL_AUTO_PUT;;
	SMenu[MN_TOWNHALL_NOTICE4].nField[1].nWillDo=0;
	SMenu[MN_TOWNHALL_NOTICE4].nField[1].x=394;
	SMenu[MN_TOWNHALL_NOTICE4].nField[1].y=25;
	SMenu[MN_TOWNHALL_NOTICE4].nField[1].nRectImage=689;
	SMenu[MN_TOWNHALL_NOTICE4].nField[1].nImageNumber=690;
	SetRect(SMenu[MN_TOWNHALL_NOTICE4].nField[1].rCheakBox,394,23,438,68);

	//next
	SMenu[MN_TOWNHALL_NOTICE4].nField[2].nType=FT_HIDE_ISCROLL_AUTO_PUT;
	SMenu[MN_TOWNHALL_NOTICE4].nField[2].nWillDo=0;
	SMenu[MN_TOWNHALL_NOTICE4].nField[2].x=394;
	SMenu[MN_TOWNHALL_NOTICE4].nField[2].y=84;
	SMenu[MN_TOWNHALL_NOTICE4].nField[2].nRectImage=691;
	SMenu[MN_TOWNHALL_NOTICE4].nField[2].nImageNumber=692;
	SetRect(SMenu[MN_TOWNHALL_NOTICE4].nField[2].rCheakBox,394,81,438,126);

	//join
	SMenu[MN_TOWNHALL_NOTICE4].nField[3].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL_NOTICE4].nField[3].nSpecialWillDo=SWD_EVENT_JOIN;
	SMenu[MN_TOWNHALL_NOTICE4].nField[3].x=393;
	SMenu[MN_TOWNHALL_NOTICE4].nField[3].y=143;
	SMenu[MN_TOWNHALL_NOTICE4].nField[3].nRectImage=693;
	SMenu[MN_TOWNHALL_NOTICE4].nField[3].nImageNumber=694;
	SetRect(SMenu[MN_TOWNHALL_NOTICE4].nField[3].rCheakBox,394,141,438,186);

	//cancel
	SMenu[MN_TOWNHALL_NOTICE4].nField[4].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL_NOTICE4].nField[4].nSpecialWillDo=SWD_SUBMENU_BACK;
	SMenu[MN_TOWNHALL_NOTICE4].nField[4].x=394;
	SMenu[MN_TOWNHALL_NOTICE4].nField[4].y=204;
	SMenu[MN_TOWNHALL_NOTICE4].nField[4].nRectImage=695;
	SMenu[MN_TOWNHALL_NOTICE4].nField[4].nImageNumber=696;
	SetRect(SMenu[MN_TOWNHALL_NOTICE4].nField[4].rCheakBox,394,202,438,247);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//townhall�޴��� notice3�޴�

	SMenu[MN_TOWNHALL_NOTICE3].x=96;
	SMenu[MN_TOWNHALL_NOTICE3].y=38;
	SMenu[MN_TOWNHALL_NOTICE3].nImageNumber=697;
	SMenu[MN_TOWNHALL_NOTICE3].nFieldCount=4;
	SMenu[MN_TOWNHALL_NOTICE3].CheakType=0;

	//preview
	SMenu[MN_TOWNHALL_NOTICE3].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL_NOTICE3].nField[0].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_TOWNHALL_NOTICE3].nField[0].x=396;
	SMenu[MN_TOWNHALL_NOTICE3].nField[0].y=27;
	SMenu[MN_TOWNHALL_NOTICE3].nField[0].nRectImage=698;
	SMenu[MN_TOWNHALL_NOTICE3].nField[0].nImageNumber=699;
	SetRect(SMenu[MN_TOWNHALL_NOTICE3].nField[0].rCheakBox,396,23,438,68);

	//next
	SMenu[MN_TOWNHALL_NOTICE3].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL_NOTICE3].nField[1].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_TOWNHALL_NOTICE3].nField[1].x=396;
	SMenu[MN_TOWNHALL_NOTICE3].nField[1].y=86;
	SMenu[MN_TOWNHALL_NOTICE3].nField[1].nRectImage=700;
	SMenu[MN_TOWNHALL_NOTICE3].nField[1].nImageNumber=701;
	SetRect(SMenu[MN_TOWNHALL_NOTICE3].nField[1].rCheakBox,396,81,438,126);

	//ok
	SMenu[MN_TOWNHALL_NOTICE3].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL_NOTICE3].nField[2].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_TOWNHALL_NOTICE3].nField[2].x=396;
	SMenu[MN_TOWNHALL_NOTICE3].nField[2].y=145;
	SMenu[MN_TOWNHALL_NOTICE3].nField[2].nRectImage=702;
	SMenu[MN_TOWNHALL_NOTICE3].nField[2].nImageNumber=703;
	SetRect(SMenu[MN_TOWNHALL_NOTICE3].nField[2].rCheakBox,396,141,438,186);

	//cancel
	SMenu[MN_TOWNHALL_NOTICE3].nField[3].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL_NOTICE3].nField[3].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_TOWNHALL_NOTICE3].nField[3].x=396;
	SMenu[MN_TOWNHALL_NOTICE3].nField[3].y=206;
	SMenu[MN_TOWNHALL_NOTICE3].nField[3].nRectImage=704;
	SMenu[MN_TOWNHALL_NOTICE3].nField[3].nImageNumber=705;
	SetRect(SMenu[MN_TOWNHALL_NOTICE3].nField[3].rCheakBox,396,202,438,247);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//townhall�޴��� notice2�޴�

	SMenu[MN_TOWNHALL_NOTICE2].x=96;
	SMenu[MN_TOWNHALL_NOTICE2].y=38;
	SMenu[MN_TOWNHALL_NOTICE2].nImageNumber=707;
	SMenu[MN_TOWNHALL_NOTICE2].nFieldCount=4;
	SMenu[MN_TOWNHALL_NOTICE2].CheakType=0;

	//preview
	SMenu[MN_TOWNHALL_NOTICE2].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL_NOTICE2].nField[0].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_TOWNHALL_NOTICE2].nField[0].x=395;
	SMenu[MN_TOWNHALL_NOTICE2].nField[0].y=27;
	SMenu[MN_TOWNHALL_NOTICE2].nField[0].nRectImage=708;
	SMenu[MN_TOWNHALL_NOTICE2].nField[0].nImageNumber=709;
	SetRect(SMenu[MN_TOWNHALL_NOTICE2].nField[0].rCheakBox,394,23,438,68);

	//next
	SMenu[MN_TOWNHALL_NOTICE2].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL_NOTICE2].nField[1].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_TOWNHALL_NOTICE2].nField[1].x=395;
	SMenu[MN_TOWNHALL_NOTICE2].nField[1].y=82;
	SMenu[MN_TOWNHALL_NOTICE2].nField[1].nRectImage=710;
	SMenu[MN_TOWNHALL_NOTICE2].nField[1].nImageNumber=711;
	SetRect(SMenu[MN_TOWNHALL_NOTICE2].nField[1].rCheakBox,394,81,438,126);

	//write
	SMenu[MN_TOWNHALL_NOTICE2].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL_NOTICE2].nField[2].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_TOWNHALL_NOTICE2].nField[2].x=395;
	SMenu[MN_TOWNHALL_NOTICE2].nField[2].y=137;
	SMenu[MN_TOWNHALL_NOTICE2].nField[2].nRectImage=712;
	SMenu[MN_TOWNHALL_NOTICE2].nField[2].nImageNumber=713;
	SetRect(SMenu[MN_TOWNHALL_NOTICE2].nField[2].rCheakBox,394,141,438,186);

	//exit
	SMenu[MN_TOWNHALL_NOTICE2].nField[3].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL_NOTICE2].nField[3].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_TOWNHALL_NOTICE2].nField[3].x=400;
	SMenu[MN_TOWNHALL_NOTICE2].nField[3].y=226;
	SMenu[MN_TOWNHALL_NOTICE2].nField[3].nRectImage=714;
	SMenu[MN_TOWNHALL_NOTICE2].nField[3].nImageNumber=715;
	SetRect(SMenu[MN_TOWNHALL_NOTICE2].nField[3].rCheakBox,399,226,433,256);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//townhall�޴��� notice1�޴�

	SMenu[MN_TOWNHALL_NOTICE1].x=96;
	SMenu[MN_TOWNHALL_NOTICE1].y=38;
	SMenu[MN_TOWNHALL_NOTICE1].nImageNumber=716;
	SMenu[MN_TOWNHALL_NOTICE1].nFieldCount=4;
	SMenu[MN_TOWNHALL_NOTICE1].CheakType=0;

	// ������ ���
	SMenu[MN_TOWNHALL_NOTICE1].nField[0].nType=FT_DATA_PUT;
	SMenu[MN_TOWNHALL_NOTICE1].nField[0].x=59;
	SMenu[MN_TOWNHALL_NOTICE1].nField[0].y=42;
	SMenu[MN_TOWNHALL_NOTICE1].nField[0].nWillDo=180;
	SMenu[MN_TOWNHALL_NOTICE1].nField[0].nRectImage=280;
	SMenu[MN_TOWNHALL_NOTICE1].nField[0].nSHideNomalCount=1;
	SMenu[MN_TOWNHALL_NOTICE1].nField[0].nSHideNomalStart=0;
	SMenu[MN_TOWNHALL_NOTICE1].nField[0].nShideNomalPlus=1;	
	SMenu[MN_TOWNHALL_NOTICE1].nField[0].nImageNumber=DP_BBS_PUT;

	//preview
	SMenu[MN_TOWNHALL_NOTICE1].nField[1].nType=FT_HIDE_DSCROLL_AUTO_PUT;;
	SMenu[MN_TOWNHALL_NOTICE1].nField[1].nWillDo=0;
	SMenu[MN_TOWNHALL_NOTICE1].nField[1].x=395;
	SMenu[MN_TOWNHALL_NOTICE1].nField[1].y=27;
	SMenu[MN_TOWNHALL_NOTICE1].nField[1].nRectImage=717;
	SMenu[MN_TOWNHALL_NOTICE1].nField[1].nImageNumber=718;
	SetRect(SMenu[MN_TOWNHALL_NOTICE1].nField[1].rCheakBox,395,26,440,98);

	//next
	SMenu[MN_TOWNHALL_NOTICE1].nField[2].nType=FT_HIDE_ISCROLL_AUTO_PUT;;
	SMenu[MN_TOWNHALL_NOTICE1].nField[2].nWillDo=0;
	SMenu[MN_TOWNHALL_NOTICE1].nField[2].x=395;
	SMenu[MN_TOWNHALL_NOTICE1].nField[2].y=107;
	SMenu[MN_TOWNHALL_NOTICE1].nField[2].nRectImage=719;
	SMenu[MN_TOWNHALL_NOTICE1].nField[2].nImageNumber=720;
	SetRect(SMenu[MN_TOWNHALL_NOTICE1].nField[2].rCheakBox,395,106,440,178);

	//exit
	SMenu[MN_TOWNHALL_NOTICE1].nField[3].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL_NOTICE1].nField[3].nSpecialWillDo=SWD_SUBMENU_BACK;
	SMenu[MN_TOWNHALL_NOTICE1].nField[3].x=400;
	SMenu[MN_TOWNHALL_NOTICE1].nField[3].y=226;
	SMenu[MN_TOWNHALL_NOTICE1].nField[3].nRectImage=714;
	SMenu[MN_TOWNHALL_NOTICE1].nField[3].nImageNumber=715;
	SetRect(SMenu[MN_TOWNHALL_NOTICE1].nField[3].rCheakBox,399,226,433,256);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//townhall�޴����� construction�޴�

	SMenu[MN_TOWNHALL_CONSTRUCTION].x=111;
	SMenu[MN_TOWNHALL_CONSTRUCTION].y=26;
#ifdef CHANGE_MONEY_
	SMenu[MN_TOWNHALL_CONSTRUCTION].nImageNumber=6;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nImageType=CHANGE_MONEY;
#else 
	SMenu[MN_TOWNHALL_CONSTRUCTION].nImageNumber=721;
#endif
	SMenu[MN_TOWNHALL_CONSTRUCTION].nFieldCount=14;
	SMenu[MN_TOWNHALL_CONSTRUCTION].CheakType=0;
	SMenu[MN_TOWNHALL_CONSTRUCTION].work=CLOSE_HOUSE;

	//���� ��ũ�� ��ư
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[2].nType=FT_HIDE_DSCROLL_AUTO_PUT;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[2].nWillDo=12;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[2].x=244;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[2].y=184;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[2].nRectImage=726;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[2].nImageNumber=727;
	SetRect(SMenu[MN_TOWNHALL_CONSTRUCTION].nField[2].rCheakBox,243,183,263,203);

	//������ ��ũ�� ��ư
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[3].nType=FT_HIDE_ISCROLL_AUTO_PUT;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[3].nWillDo=12;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[3].x=398;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[3].y=184;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[3].nRectImage=728;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[3].nImageNumber=729;
	SetRect(SMenu[MN_TOWNHALL_CONSTRUCTION].nField[3].rCheakBox,397,184,417,201);

	//������ cancel
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[4].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[4].nSpecialWillDo=SWD_SUBMENU_BACK;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[4].x=377;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[4].y=236;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[4].nRectImage=292;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[4].nImageNumber=293;
	SetRect(SMenu[MN_TOWNHALL_CONSTRUCTION].nField[4].rCheakBox,377,236,416,276);

	//������OK
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[5].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[5].nSpecialWillDo=SWD_BUY_HOUSE;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[5].x=242;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[5].y=236;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[5].nRectImage=294;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[5].nImageNumber=295;
	SetRect(SMenu[MN_TOWNHALL_CONSTRUCTION].nField[5].rCheakBox,240,236,282,276);

	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[6].nType=FT_TOWN_CONSTRUCTION;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[6].x=285;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[6].y=178;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[6].nImageNumber=0;

	// �Ѳ� ���ݱ�
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[7].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[7].nSpecialWillDo=SWD_TOWN_CONSTRUCTION;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[7].x=285;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[7].y=178;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[7].nRectImage=731;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[7].nImageNumber=731;
	SetRect(SMenu[MN_TOWNHALL_CONSTRUCTION].nField[7].rCheakBox,316, 190, 343, 215);

	//�޴� ����
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[8].nType=FT_HIDE_NOMAL_GROUP_PUT;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[8].nSHideNomalNumber=HN_TOWNHALL_CONSTRUCTION_TEXT;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[8].nSHideNomalCount=3;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[8].nSHideNomalStart=0;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[8].nShideNomalPlus=3;

	//residing��ư
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[9].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[9].nSpecialWillDo=SWD_SELECT_HOUSEKIND;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[9].nWillDo=RESIDING;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[9].nSHideNomalCount=12;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[9].nSHideNomalStart=13;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[9].x=46;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[9].y=68;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[9].nRectImage=23;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[9].nImageNumber=23;
	SetRect(SMenu[MN_TOWNHALL_CONSTRUCTION].nField[9].rCheakBox,47,68,174,95);

	//guild��ư
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[10].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[10].nSpecialWillDo=SWD_SELECT_HOUSEKIND;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[10].nWillDo=GUILD;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[10].nSHideNomalCount=12;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[10].nSHideNomalStart=13;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[10].x=46;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[10].y=106;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[10].nRectImage=23;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[10].nImageNumber=23;
	SetRect(SMenu[MN_TOWNHALL_CONSTRUCTION].nField[10].rCheakBox,47,106,174,133);

	//not residing��ư
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[11].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[11].nSpecialWillDo=SWD_SELECT_HOUSEKIND;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[11].nWillDo=NOTRESIDING;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[11].nSHideNomalCount=12;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[11].nSHideNomalStart=13;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[11].x=46;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[11].y=144;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[11].nRectImage=23;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[11].nImageNumber=23;
	SetRect(SMenu[MN_TOWNHALL_CONSTRUCTION].nField[11].rCheakBox,47,144,174,171);

	// �� ���
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[12].nType=FT_DATA_PUT;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[12].nWillDo = 100;		// default ��....
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[12].x=252;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[12].y=55;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[12].nSHideNomalCount = 0;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[12].nSHideNomalStart = 0;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[12].nShideNomalPlus = 1;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[12].nImageNumber=DP_HOUSE_PUT;

	// ���� �� ���� 
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[13].nType=FT_NOMAL_PUT;
//	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[13].x=249;
//	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[13].y=55;
	SMenu[MN_TOWNHALL_CONSTRUCTION].nField[13].nImageNumber=0;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���� �������̽�	// OK CANCEL Button

	SMenu[MN_TOWNHALL_BBS1].x=96;
	SMenu[MN_TOWNHALL_BBS1].y=38;
	SMenu[MN_TOWNHALL_BBS1].nImageNumber=10006;
	SMenu[MN_TOWNHALL_BBS1].nFieldCount=5;
	SMenu[MN_TOWNHALL_BBS1].CheakType=0;

	// ������ ���
	SMenu[MN_TOWNHALL_BBS1].nField[0].nType=FT_DATA_PUT;
	SMenu[MN_TOWNHALL_BBS1].nField[0].x=59;
	SMenu[MN_TOWNHALL_BBS1].nField[0].y=42;
	SMenu[MN_TOWNHALL_BBS1].nField[0].nWillDo=180;
	SMenu[MN_TOWNHALL_BBS1].nField[0].nRectImage=280;
	SMenu[MN_TOWNHALL_BBS1].nField[0].nSHideNomalCount=1;
	SMenu[MN_TOWNHALL_BBS1].nField[0].nSHideNomalStart=0;
	SMenu[MN_TOWNHALL_BBS1].nField[0].nShideNomalPlus=1;	
	SMenu[MN_TOWNHALL_BBS1].nField[0].nImageNumber=DP_BBS_PUT;

	//preview
	SMenu[MN_TOWNHALL_BBS1].nField[1].nType=FT_HIDE_DSCROLL_AUTO_PUT;;
	SMenu[MN_TOWNHALL_BBS1].nField[1].nWillDo=0;
	SMenu[MN_TOWNHALL_BBS1].nField[1].x=52;
	SMenu[MN_TOWNHALL_BBS1].nField[1].y=37;
	SMenu[MN_TOWNHALL_BBS1].nField[1].nRectImage=10018;
	SMenu[MN_TOWNHALL_BBS1].nField[1].nImageNumber=10019;
	SetRect(SMenu[MN_TOWNHALL_BBS1].nField[1].rCheakBox,52,37,118,72);

	//next
	SMenu[MN_TOWNHALL_BBS1].nField[2].nType=FT_HIDE_ISCROLL_AUTO_PUT;;
	SMenu[MN_TOWNHALL_BBS1].nField[2].nWillDo=0;
	SMenu[MN_TOWNHALL_BBS1].nField[2].x=336;
	SMenu[MN_TOWNHALL_BBS1].nField[2].y=36;
	SMenu[MN_TOWNHALL_BBS1].nField[2].nRectImage=10020;
	SMenu[MN_TOWNHALL_BBS1].nField[2].nImageNumber=10021;
	SetRect(SMenu[MN_TOWNHALL_BBS1].nField[2].rCheakBox,337,36,403,71);

	//ok
	SMenu[MN_TOWNHALL_BBS1].nField[3].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL_BBS1].nField[3].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_TOWNHALL_BBS1].nField[3].x=52;
	SMenu[MN_TOWNHALL_BBS1].nField[3].y=207;
	SMenu[MN_TOWNHALL_BBS1].nField[3].nRectImage=10008;
	SMenu[MN_TOWNHALL_BBS1].nField[3].nImageNumber=10009;
	SetRect(SMenu[MN_TOWNHALL_BBS1].nField[3].rCheakBox,52,207,118,242);

	//cancel
	SMenu[MN_TOWNHALL_BBS1].nField[4].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL_BBS1].nField[4].nSpecialWillDo=SWD_SUBMENU_BACK;
	SMenu[MN_TOWNHALL_BBS1].nField[4].x=338;
	SMenu[MN_TOWNHALL_BBS1].nField[4].y=203;
	SMenu[MN_TOWNHALL_BBS1].nField[4].nRectImage=10010;
	SMenu[MN_TOWNHALL_BBS1].nField[4].nImageNumber=10011;
	SetRect(SMenu[MN_TOWNHALL_BBS1].nField[4].rCheakBox,338,203,404,238);


/////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���� �������̽�	// JOIN CANCEL Button

	SMenu[MN_TOWNHALL_BBS2].x=96;
	SMenu[MN_TOWNHALL_BBS2].y=38;
	SMenu[MN_TOWNHALL_BBS2].nImageNumber=10005;
	SMenu[MN_TOWNHALL_BBS2].nFieldCount=5;
	SMenu[MN_TOWNHALL_BBS2].CheakType=0;

	// ������ ���
	SMenu[MN_TOWNHALL_BBS2].nField[0].nType=FT_DATA_PUT;
	SMenu[MN_TOWNHALL_BBS2].nField[0].x=59;
	SMenu[MN_TOWNHALL_BBS2].nField[0].y=42;
	SMenu[MN_TOWNHALL_BBS2].nField[0].nWillDo=180;
	SMenu[MN_TOWNHALL_BBS2].nField[0].nRectImage=280;
	SMenu[MN_TOWNHALL_BBS2].nField[0].nSHideNomalCount=1;
	SMenu[MN_TOWNHALL_BBS2].nField[0].nSHideNomalStart=0;
	SMenu[MN_TOWNHALL_BBS2].nField[0].nShideNomalPlus=1;	
	SMenu[MN_TOWNHALL_BBS2].nField[0].nImageNumber=DP_BBS_PUT;

	//preview
	SMenu[MN_TOWNHALL_BBS2].nField[1].nType=FT_HIDE_DSCROLL_AUTO_PUT;;
	SMenu[MN_TOWNHALL_BBS2].nField[1].nWillDo=0;
	SMenu[MN_TOWNHALL_BBS2].nField[1].x=52;
	SMenu[MN_TOWNHALL_BBS2].nField[1].y=37;
	SMenu[MN_TOWNHALL_BBS2].nField[1].nRectImage=10018;
	SMenu[MN_TOWNHALL_BBS2].nField[1].nImageNumber=10019;
	SetRect(SMenu[MN_TOWNHALL_BBS2].nField[1].rCheakBox,52,37,118,72);

	//next
	SMenu[MN_TOWNHALL_BBS2].nField[2].nType=FT_HIDE_ISCROLL_AUTO_PUT;;
	SMenu[MN_TOWNHALL_BBS2].nField[2].nWillDo=0;
	SMenu[MN_TOWNHALL_BBS2].nField[2].x=336;
	SMenu[MN_TOWNHALL_BBS2].nField[2].y=36;
	SMenu[MN_TOWNHALL_BBS2].nField[2].nRectImage=10020;
	SMenu[MN_TOWNHALL_BBS2].nField[2].nImageNumber=10021;
	SetRect(SMenu[MN_TOWNHALL_BBS2].nField[2].rCheakBox,337,36,403,71);

	//Join
	SMenu[MN_TOWNHALL_BBS2].nField[3].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL_BBS2].nField[3].nSpecialWillDo=SWD_EVENT_JOIN;
	SMenu[MN_TOWNHALL_BBS2].nField[3].x=52;
	SMenu[MN_TOWNHALL_BBS2].nField[3].y=207;
	SMenu[MN_TOWNHALL_BBS2].nField[3].nRectImage=10014;
	SMenu[MN_TOWNHALL_BBS2].nField[3].nImageNumber=10015;
	SetRect(SMenu[MN_TOWNHALL_BBS2].nField[3].rCheakBox,52,207,118,242);

	//cancel
	SMenu[MN_TOWNHALL_BBS2].nField[4].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL_BBS2].nField[4].nSpecialWillDo=SWD_SUBMENU_BACK;
	SMenu[MN_TOWNHALL_BBS2].nField[4].x=338;
	SMenu[MN_TOWNHALL_BBS2].nField[4].y=203;
	SMenu[MN_TOWNHALL_BBS2].nField[4].nRectImage=10010;
	SMenu[MN_TOWNHALL_BBS2].nField[4].nImageNumber=10011;
	SetRect(SMenu[MN_TOWNHALL_BBS2].nField[4].rCheakBox,338,203,404,238);



/////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���� �������̽�	// WRITE/EXIT Button

	SMenu[MN_TOWNHALL_BBS3].x=96;
	SMenu[MN_TOWNHALL_BBS3].y=38;
	SMenu[MN_TOWNHALL_BBS3].nImageNumber=10007;
	SMenu[MN_TOWNHALL_BBS3].nFieldCount=5;
	SMenu[MN_TOWNHALL_BBS3].CheakType=0;

	// ������ ���
	SMenu[MN_TOWNHALL_BBS3].nField[0].nType=FT_DATA_PUT;
	SMenu[MN_TOWNHALL_BBS3].nField[0].x=59;
	SMenu[MN_TOWNHALL_BBS3].nField[0].y=42;
	SMenu[MN_TOWNHALL_BBS3].nField[0].nWillDo=180;
	SMenu[MN_TOWNHALL_BBS3].nField[0].nRectImage=280;
	SMenu[MN_TOWNHALL_BBS3].nField[0].nSHideNomalCount=1;
	SMenu[MN_TOWNHALL_BBS3].nField[0].nSHideNomalStart=0;
	SMenu[MN_TOWNHALL_BBS3].nField[0].nShideNomalPlus=1;	
	SMenu[MN_TOWNHALL_BBS3].nField[0].nImageNumber=DP_BBS_WRITE_PUT;

	//preview
//	SMenu[MN_TOWNHALL_BBS3].nField[1].nType=FT_HIDE_DSCROLL_AUTO_PUT;
//	SMenu[MN_TOWNHALL_BBS3].nField[1].nWillDo=0;
//	SMenu[MN_TOWNHALL_BBS3].nField[1].x=52;
//	SMenu[MN_TOWNHALL_BBS3].nField[1].y=37;
//	SMenu[MN_TOWNHALL_BBS3].nField[1].nRectImage=10018;
//	SMenu[MN_TOWNHALL_BBS3].nField[1].nImageNumber=10019;
//	SetRect(SMenu[MN_TOWNHALL_BBS3].nField[1].rCheakBox,52,37,118,72);

	//next
///	SMenu[MN_TOWNHALL_BBS3].nField[2].nType=FT_HIDE_ISCROLL_AUTO_PUT;
//	SMenu[MN_TOWNHALL_BBS3].nField[2].nWillDo=0;
//	SMenu[MN_TOWNHALL_BBS3].nField[2].x=336;
//	SMenu[MN_TOWNHALL_BBS3].nField[2].y=36;
//	SMenu[MN_TOWNHALL_BBS3].nField[2].nRectImage=10020;
//	SMenu[MN_TOWNHALL_BBS3].nField[2].nImageNumber=10021;
//	SetRect(SMenu[MN_TOWNHALL_BBS3].nField[2].rCheakBox,337,36,403,71);

	//write
	SMenu[MN_TOWNHALL_BBS3].nField[3].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL_BBS3].nField[3].nSpecialWillDo=SWD_BBS_WRITE_SEND;
//	SMenu[MN_TOWNHALL_BBS3].nField[3].nSpecialWillDo=SWD_SORRY;
	SMenu[MN_TOWNHALL_BBS3].nField[3].x=52;
	SMenu[MN_TOWNHALL_BBS3].nField[3].y=207;
	SMenu[MN_TOWNHALL_BBS3].nField[3].nRectImage=10016;
	SMenu[MN_TOWNHALL_BBS3].nField[3].nImageNumber=10017;
	SetRect(SMenu[MN_TOWNHALL_BBS3].nField[3].rCheakBox,52,207,118,242);

	//EXIT
	SMenu[MN_TOWNHALL_BBS3].nField[4].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_TOWNHALL_BBS3].nField[4].nSpecialWillDo=SWD_BBS_WRITE_EXIT;
	SMenu[MN_TOWNHALL_BBS3].nField[4].x=338;
	SMenu[MN_TOWNHALL_BBS3].nField[4].y=203;
	SMenu[MN_TOWNHALL_BBS3].nField[4].nRectImage=10012;
	SMenu[MN_TOWNHALL_BBS3].nField[4].nImageNumber=10013;
	SetRect(SMenu[MN_TOWNHALL_BBS3].nField[4].rCheakBox,338,203,404,238);





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	���丮 ����
	SMenu[MN_GOD_TEFFERY].x=216+GABX_SCREEN;
	SMenu[MN_GOD_TEFFERY].y=20;
	SMenu[MN_GOD_TEFFERY].nImageNumber=1;
	SMenu[MN_GOD_TEFFERY].nImageType=GOD_MENU;
	SMenu[MN_GOD_TEFFERY].nFieldCount=7;
	SMenu[MN_GOD_TEFFERY].CheakType=0;

// EXIT
	SMenu[MN_GOD_TEFFERY].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GOD_TEFFERY].nField[0].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_GOD_TEFFERY].nField[0].x=70;
	SMenu[MN_GOD_TEFFERY].nField[0].y=280;
	SMenu[MN_GOD_TEFFERY].nField[0].nRectImage=12;
	SMenu[MN_GOD_TEFFERY].nField[0].nImageNumber=13;
	SMenu[MN_GOD_TEFFERY].nField[0].nImageType = GOD_MENU;
	SetRect(SMenu[MN_GOD_TEFFERY].nField[0].rCheakBox, 70, 280, 142, 307);

// INTRUDUCTION	// ���� ����
	SMenu[MN_GOD_TEFFERY].nField[1].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GOD_TEFFERY].nField[1].nSpecialWillDo = SWD_FOCUS_WILLDO;
	SMenu[MN_GOD_TEFFERY].nField[1].nWillDo = MN_GOD_INTRUDUCTION;
	SMenu[MN_GOD_TEFFERY].nField[1].x=32;
	SMenu[MN_GOD_TEFFERY].nField[1].y=136;
	SMenu[MN_GOD_TEFFERY].nField[1].nRectImage=18;
	SMenu[MN_GOD_TEFFERY].nField[1].nImageNumber=19;
	SMenu[MN_GOD_TEFFERY].nField[1].nImageType = GOD_MENU;
	SetRect(SMenu[MN_GOD_TEFFERY].nField[1].rCheakBox, 32, 136, 181, 158);

// METTING		// ���� ����
	SMenu[MN_GOD_TEFFERY].nField[2].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GOD_TEFFERY].nField[2].nSpecialWillDo = SWD_FOCUS_WILLDO;
	SMenu[MN_GOD_TEFFERY].nField[2].nWillDo = MN_GOD_MEETING;
	SMenu[MN_GOD_TEFFERY].nField[2].x=32;
	SMenu[MN_GOD_TEFFERY].nField[2].y=162;
	SMenu[MN_GOD_TEFFERY].nField[2].nRectImage=20;
	SMenu[MN_GOD_TEFFERY].nField[2].nImageNumber=21;
	SMenu[MN_GOD_TEFFERY].nField[2].nImageType = GOD_MENU;
	SetRect(SMenu[MN_GOD_TEFFERY].nField[2].rCheakBox, 32, 162, 181, 184);

// REGISTRATION	// ���
	SMenu[MN_GOD_TEFFERY].nField[3].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GOD_TEFFERY].nField[3].nSpecialWillDo = SWD_FOCUS_WILLDO;
	SMenu[MN_GOD_TEFFERY].nField[3].nWillDo = MN_GOD_REGIST1;
	SMenu[MN_GOD_TEFFERY].nField[3].x=32;
	SMenu[MN_GOD_TEFFERY].nField[3].y=188;
	SMenu[MN_GOD_TEFFERY].nField[3].nRectImage=22;
	SMenu[MN_GOD_TEFFERY].nField[3].nImageNumber=23;
	SMenu[MN_GOD_TEFFERY].nField[3].nImageType = GOD_MENU;
	SetRect(SMenu[MN_GOD_TEFFERY].nField[3].rCheakBox, 32, 188, 181, 210);

// MISSION
	SMenu[MN_GOD_TEFFERY].nField[4].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GOD_TEFFERY].nField[4].nSpecialWillDo = SWD_SORRY;
//	SMenu[MN_GOD_TEFFERY].nField[4].nSpecialWillDo = SWD_SMALLMENU_END;
	SMenu[MN_GOD_TEFFERY].nField[4].x=32;
	SMenu[MN_GOD_TEFFERY].nField[4].y=215;
	SMenu[MN_GOD_TEFFERY].nField[4].nRectImage=24;
	SMenu[MN_GOD_TEFFERY].nField[4].nImageNumber=25;
	SMenu[MN_GOD_TEFFERY].nField[4].nImageType = GOD_MENU;
	SetRect(SMenu[MN_GOD_TEFFERY].nField[4].rCheakBox, 32, 215, 182, 237);

// SUPPER
	SMenu[MN_GOD_TEFFERY].nField[5].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
//	SMenu[MN_GOD_TEFFERY].nField[5].nSpecialWillDo = SWD_SORRY;
	SMenu[MN_GOD_TEFFERY].nField[5].nSpecialWillDo = SWD_FOCUS_WILLDO;
	SMenu[MN_GOD_TEFFERY].nField[5].nWillDo = MN_GOD_SUPPER;
	SMenu[MN_GOD_TEFFERY].nField[5].x=32;
	SMenu[MN_GOD_TEFFERY].nField[5].y=241;
	SMenu[MN_GOD_TEFFERY].nField[5].nRectImage=26;
	SMenu[MN_GOD_TEFFERY].nField[5].nImageNumber=27;
	SMenu[MN_GOD_TEFFERY].nField[5].nImageType = GOD_MENU;
	SetRect(SMenu[MN_GOD_TEFFERY].nField[5].rCheakBox, 32, 241, 181, 263);

// ���� ���丮�������� �ٲٱ�
	SMenu[MN_GOD_TEFFERY].nField[6].nType = FT_NOMAL_PUT;
	SMenu[MN_GOD_TEFFERY].nField[6].x=0;
	SMenu[MN_GOD_TEFFERY].nField[6].y=-21;
	SMenu[MN_GOD_TEFFERY].nField[6].nImageNumber = 2;
	SMenu[MN_GOD_TEFFERY].nField[6].nImageType = GOD_MENU;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	���� ����
	SMenu[MN_GOD_LETTY].x=216+GABX_SCREEN;
	SMenu[MN_GOD_LETTY].y=20;
	SMenu[MN_GOD_LETTY].nImageNumber=1;
	SMenu[MN_GOD_LETTY].nImageType=GOD_MENU;
	SMenu[MN_GOD_LETTY].nFieldCount=7;
	SMenu[MN_GOD_LETTY].CheakType=0;

// EXIT
	SMenu[MN_GOD_LETTY].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GOD_LETTY].nField[0].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_GOD_LETTY].nField[0].x=70;
	SMenu[MN_GOD_LETTY].nField[0].y=280;
	SMenu[MN_GOD_LETTY].nField[0].nRectImage=12;
	SMenu[MN_GOD_LETTY].nField[0].nImageNumber=13;
	SMenu[MN_GOD_LETTY].nField[0].nImageType = GOD_MENU;
	SetRect(SMenu[MN_GOD_LETTY].nField[0].rCheakBox, 70, 280, 142, 307);

// INTRUDUCTION	// ���� ����
	SMenu[MN_GOD_LETTY].nField[1].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GOD_LETTY].nField[1].nSpecialWillDo = SWD_FOCUS_WILLDO;
	SMenu[MN_GOD_LETTY].nField[1].nWillDo = MN_GOD_INTRUDUCTION;
	SMenu[MN_GOD_LETTY].nField[1].x=32;
	SMenu[MN_GOD_LETTY].nField[1].y=136;
	SMenu[MN_GOD_LETTY].nField[1].nRectImage=18;
	SMenu[MN_GOD_LETTY].nField[1].nImageNumber=19;
	SMenu[MN_GOD_LETTY].nField[1].nImageType = GOD_MENU;
	SetRect(SMenu[MN_GOD_LETTY].nField[1].rCheakBox, 32, 136, 181, 158);

// METTING		// ���� ����
	SMenu[MN_GOD_LETTY].nField[2].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GOD_LETTY].nField[2].nSpecialWillDo = SWD_FOCUS_WILLDO;
	SMenu[MN_GOD_LETTY].nField[2].nWillDo = MN_GOD_MEETING;
	SMenu[MN_GOD_LETTY].nField[2].x=32;
	SMenu[MN_GOD_LETTY].nField[2].y=162;
	SMenu[MN_GOD_LETTY].nField[2].nRectImage=20;
	SMenu[MN_GOD_LETTY].nField[2].nImageNumber=21;
	SMenu[MN_GOD_LETTY].nField[2].nImageType = GOD_MENU;
	SetRect(SMenu[MN_GOD_LETTY].nField[2].rCheakBox, 32, 162, 181, 184);

// REGISTRATION	// ���
	SMenu[MN_GOD_LETTY].nField[3].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GOD_LETTY].nField[3].nSpecialWillDo = SWD_FOCUS_WILLDO;
	SMenu[MN_GOD_LETTY].nField[3].nWillDo = MN_GOD_REGIST1;
	SMenu[MN_GOD_LETTY].nField[3].x=32;
	SMenu[MN_GOD_LETTY].nField[3].y=188;
	SMenu[MN_GOD_LETTY].nField[3].nRectImage=22;
	SMenu[MN_GOD_LETTY].nField[3].nImageNumber=23;
	SMenu[MN_GOD_LETTY].nField[3].nImageType = GOD_MENU;
	SetRect(SMenu[MN_GOD_LETTY].nField[3].rCheakBox, 32, 188, 181, 210);

// MISSION
	SMenu[MN_GOD_LETTY].nField[4].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GOD_LETTY].nField[4].nSpecialWillDo = SWD_SORRY;
//	SMenu[MN_GOD_LETTY].nField[4].nSpecialWillDo = SWD_SMALLMENU_END;
	SMenu[MN_GOD_LETTY].nField[4].x=32;
	SMenu[MN_GOD_LETTY].nField[4].y=215;
	SMenu[MN_GOD_LETTY].nField[4].nRectImage=24;
	SMenu[MN_GOD_LETTY].nField[4].nImageNumber=25;
	SMenu[MN_GOD_LETTY].nField[4].nImageType = GOD_MENU;
	SetRect(SMenu[MN_GOD_LETTY].nField[4].rCheakBox, 32, 215, 182, 237);

// SUPPER
	SMenu[MN_GOD_LETTY].nField[5].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
//	SMenu[MN_GOD_LETTY].nField[5].nSpecialWillDo = SWD_SORRY;
	SMenu[MN_GOD_LETTY].nField[5].nSpecialWillDo = SWD_FOCUS_WILLDO;
	SMenu[MN_GOD_LETTY].nField[5].nWillDo = MN_GOD_SUPPER;
	SMenu[MN_GOD_LETTY].nField[5].x=32;
	SMenu[MN_GOD_LETTY].nField[5].y=241;
	SMenu[MN_GOD_LETTY].nField[5].nRectImage=26;
	SMenu[MN_GOD_LETTY].nField[5].nImageNumber=27;
	SMenu[MN_GOD_LETTY].nField[5].nImageType = GOD_MENU;
	SetRect(SMenu[MN_GOD_LETTY].nField[5].rCheakBox, 32, 241, 181, 263);

// ���� ��Ƽ �������� �ٲٱ�
	SMenu[MN_GOD_LETTY].nField[6].nType = FT_NOMAL_PUT;
	SMenu[MN_GOD_LETTY].nField[6].x=0;
	SMenu[MN_GOD_LETTY].nField[6].y=-6;
	SMenu[MN_GOD_LETTY].nField[6].nImageNumber = 3;
	SMenu[MN_GOD_LETTY].nField[6].nImageType = GOD_MENU;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	��������� ����
	SMenu[MN_GOD_EDELBLHOY].x=216+GABX_SCREEN;
	SMenu[MN_GOD_EDELBLHOY].y=20;
	SMenu[MN_GOD_EDELBLHOY].nImageNumber=1;
	SMenu[MN_GOD_EDELBLHOY].nImageType=GOD_MENU;
	SMenu[MN_GOD_EDELBLHOY].nFieldCount=6;
	SMenu[MN_GOD_EDELBLHOY].CheakType=0;

// EXIT
	SMenu[MN_GOD_EDELBLHOY].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GOD_EDELBLHOY].nField[0].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_GOD_EDELBLHOY].nField[0].x=70;
	SMenu[MN_GOD_EDELBLHOY].nField[0].y=280;
	SMenu[MN_GOD_EDELBLHOY].nField[0].nRectImage=12;
	SMenu[MN_GOD_EDELBLHOY].nField[0].nImageNumber=13;
	SMenu[MN_GOD_EDELBLHOY].nField[0].nImageType = GOD_MENU;
	SetRect(SMenu[MN_GOD_EDELBLHOY].nField[0].rCheakBox, 70, 280, 142, 307);

// INTRUDUCTION	// ���� ����
	SMenu[MN_GOD_EDELBLHOY].nField[1].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GOD_EDELBLHOY].nField[1].nSpecialWillDo = SWD_FOCUS_WILLDO;
	SMenu[MN_GOD_EDELBLHOY].nField[1].nWillDo = MN_GOD_INTRUDUCTION;
	SMenu[MN_GOD_EDELBLHOY].nField[1].x=32;
	SMenu[MN_GOD_EDELBLHOY].nField[1].y=136;
	SMenu[MN_GOD_EDELBLHOY].nField[1].nRectImage=18;
	SMenu[MN_GOD_EDELBLHOY].nField[1].nImageNumber=19;
	SMenu[MN_GOD_EDELBLHOY].nField[1].nImageType = GOD_MENU;
	SetRect(SMenu[MN_GOD_EDELBLHOY].nField[1].rCheakBox, 32, 136, 181, 158);

// METTING		// ���� ����
	SMenu[MN_GOD_EDELBLHOY].nField[2].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GOD_EDELBLHOY].nField[2].nSpecialWillDo = SWD_FOCUS_WILLDO;
	SMenu[MN_GOD_EDELBLHOY].nField[2].nWillDo = MN_GOD_MEETING;
	SMenu[MN_GOD_EDELBLHOY].nField[2].x=32;
	SMenu[MN_GOD_EDELBLHOY].nField[2].y=162;
	SMenu[MN_GOD_EDELBLHOY].nField[2].nRectImage=20;
	SMenu[MN_GOD_EDELBLHOY].nField[2].nImageNumber=21;
	SMenu[MN_GOD_EDELBLHOY].nField[2].nImageType = GOD_MENU;
	SetRect(SMenu[MN_GOD_EDELBLHOY].nField[2].rCheakBox, 32, 162, 181, 184);

// REGISTRATION	// ���
	SMenu[MN_GOD_EDELBLHOY].nField[3].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GOD_EDELBLHOY].nField[3].nSpecialWillDo = SWD_FOCUS_WILLDO;
	SMenu[MN_GOD_EDELBLHOY].nField[3].nWillDo = MN_GOD_REGIST1;
	SMenu[MN_GOD_EDELBLHOY].nField[3].x=32;
	SMenu[MN_GOD_EDELBLHOY].nField[3].y=188;
	SMenu[MN_GOD_EDELBLHOY].nField[3].nRectImage=22;
	SMenu[MN_GOD_EDELBLHOY].nField[3].nImageNumber=23;
	SMenu[MN_GOD_EDELBLHOY].nField[3].nImageType = GOD_MENU;
	SetRect(SMenu[MN_GOD_EDELBLHOY].nField[3].rCheakBox, 32, 188, 181, 210);

// MISSION
	SMenu[MN_GOD_EDELBLHOY].nField[4].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GOD_EDELBLHOY].nField[4].nSpecialWillDo = SWD_SORRY;
//	SMenu[MN_GOD_EDELBLHOY].nField[4].nSpecialWillDo = SWD_SMALLMENU_END;
	SMenu[MN_GOD_EDELBLHOY].nField[4].x=32;
	SMenu[MN_GOD_EDELBLHOY].nField[4].y=215;
	SMenu[MN_GOD_EDELBLHOY].nField[4].nRectImage=24;
	SMenu[MN_GOD_EDELBLHOY].nField[4].nImageNumber=25;
	SMenu[MN_GOD_EDELBLHOY].nField[4].nImageType = GOD_MENU;
	SetRect(SMenu[MN_GOD_EDELBLHOY].nField[4].rCheakBox, 32, 215, 182, 237);

// SUPPER
	SMenu[MN_GOD_EDELBLHOY].nField[5].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
//	SMenu[MN_GOD_EDELBLHOY].nField[5].nSpecialWillDo = SWD_SORRY;
	SMenu[MN_GOD_EDELBLHOY].nField[5].nSpecialWillDo = SWD_FOCUS_WILLDO;
	SMenu[MN_GOD_EDELBLHOY].nField[5].nWillDo = MN_GOD_SUPPER;
	SMenu[MN_GOD_EDELBLHOY].nField[5].x=32;
	SMenu[MN_GOD_EDELBLHOY].nField[5].y=241;
	SMenu[MN_GOD_EDELBLHOY].nField[5].nRectImage=26;
	SMenu[MN_GOD_EDELBLHOY].nField[5].nImageNumber=27;
	SMenu[MN_GOD_EDELBLHOY].nField[5].nImageType = GOD_MENU;
	SetRect(SMenu[MN_GOD_EDELBLHOY].nField[5].rCheakBox, 32, 241, 181, 263);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	���� ����
	SMenu[MN_GOD_OREM].x=216+GABX_SCREEN;
	SMenu[MN_GOD_OREM].y=20;
	SMenu[MN_GOD_OREM].nImageNumber=1;
	SMenu[MN_GOD_OREM].nImageType=GOD_MENU;
	SMenu[MN_GOD_OREM].nFieldCount=7;
	SMenu[MN_GOD_OREM].CheakType=0;

// EXIT
	SMenu[MN_GOD_OREM].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GOD_OREM].nField[0].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_GOD_OREM].nField[0].x=70;
	SMenu[MN_GOD_OREM].nField[0].y=280;
	SMenu[MN_GOD_OREM].nField[0].nRectImage=12;
	SMenu[MN_GOD_OREM].nField[0].nImageNumber=13;
	SMenu[MN_GOD_OREM].nField[0].nImageType = GOD_MENU;
	SetRect(SMenu[MN_GOD_OREM].nField[0].rCheakBox, 70, 280, 142, 307);

// INTRUDUCTION	// ���� ����
	SMenu[MN_GOD_OREM].nField[1].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GOD_OREM].nField[1].nSpecialWillDo = SWD_FOCUS_WILLDO;
	SMenu[MN_GOD_OREM].nField[1].nWillDo = MN_GOD_INTRUDUCTION;
	SMenu[MN_GOD_OREM].nField[1].x=32;
	SMenu[MN_GOD_OREM].nField[1].y=136;
	SMenu[MN_GOD_OREM].nField[1].nRectImage=18;
	SMenu[MN_GOD_OREM].nField[1].nImageNumber=19;
	SMenu[MN_GOD_OREM].nField[1].nImageType = GOD_MENU;
	SetRect(SMenu[MN_GOD_OREM].nField[1].rCheakBox, 32, 136, 181, 158);

// METTING		// ���� ����
	SMenu[MN_GOD_OREM].nField[2].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GOD_OREM].nField[2].nSpecialWillDo = SWD_FOCUS_WILLDO;
	SMenu[MN_GOD_OREM].nField[2].nWillDo = MN_GOD_MEETING;
	SMenu[MN_GOD_OREM].nField[2].x=32;
	SMenu[MN_GOD_OREM].nField[2].y=162;
	SMenu[MN_GOD_OREM].nField[2].nRectImage=20;
	SMenu[MN_GOD_OREM].nField[2].nImageNumber=21;
	SMenu[MN_GOD_OREM].nField[2].nImageType = GOD_MENU;
	SetRect(SMenu[MN_GOD_OREM].nField[2].rCheakBox, 32, 162, 181, 184);

// REGISTRATION	// ���
	SMenu[MN_GOD_OREM].nField[3].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GOD_OREM].nField[3].nSpecialWillDo = SWD_FOCUS_WILLDO;
	SMenu[MN_GOD_OREM].nField[3].nWillDo = MN_GOD_REGIST1;
	SMenu[MN_GOD_OREM].nField[3].x=32;
	SMenu[MN_GOD_OREM].nField[3].y=188;
	SMenu[MN_GOD_OREM].nField[3].nRectImage=22;
	SMenu[MN_GOD_OREM].nField[3].nImageNumber=23;
	SMenu[MN_GOD_OREM].nField[3].nImageType = GOD_MENU;
	SetRect(SMenu[MN_GOD_OREM].nField[3].rCheakBox, 32, 188, 181, 210);

// MISSION
	SMenu[MN_GOD_OREM].nField[4].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GOD_OREM].nField[4].nSpecialWillDo = SWD_SORRY;
//	SMenu[MN_GOD_OREM].nField[4].nSpecialWillDo = SWD_SMALLMENU_END;
	SMenu[MN_GOD_OREM].nField[4].x=32;
	SMenu[MN_GOD_OREM].nField[4].y=215;
	SMenu[MN_GOD_OREM].nField[4].nRectImage=24;
	SMenu[MN_GOD_OREM].nField[4].nImageNumber=25;
	SMenu[MN_GOD_OREM].nField[4].nImageType = GOD_MENU;
	SetRect(SMenu[MN_GOD_OREM].nField[4].rCheakBox, 32, 215, 182, 237);

// SUPPER
	SMenu[MN_GOD_OREM].nField[5].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
//	SMenu[MN_GOD_OREM].nField[5].nSpecialWillDo = SWD_SORRY;
	SMenu[MN_GOD_OREM].nField[5].nSpecialWillDo = SWD_FOCUS_WILLDO;
	SMenu[MN_GOD_OREM].nField[5].nWillDo = MN_GOD_SUPPER;
	SMenu[MN_GOD_OREM].nField[5].x=32;
	SMenu[MN_GOD_OREM].nField[5].y=241;
	SMenu[MN_GOD_OREM].nField[5].nRectImage=26;
	SMenu[MN_GOD_OREM].nField[5].nImageNumber=27;
	SMenu[MN_GOD_OREM].nField[5].nImageType = GOD_MENU;
	SetRect(SMenu[MN_GOD_OREM].nField[5].rCheakBox, 32, 241, 181, 263);

// ���� ���� �������� �ٲٱ�
	SMenu[MN_GOD_OREM].nField[6].nType = FT_NOMAL_PUT;
	SMenu[MN_GOD_OREM].nField[6].x=49;
	SMenu[MN_GOD_OREM].nField[6].y=86;
	SMenu[MN_GOD_OREM].nField[6].nImageNumber = 43;
	SMenu[MN_GOD_OREM].nField[6].nImageType = GOD_MENU;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	���� INTRUDUCTION
	SMenu[MN_GOD_INTRUDUCTION].x=155+GABX_SCREEN;
	SMenu[MN_GOD_INTRUDUCTION].y=20;
	SMenu[MN_GOD_INTRUDUCTION].nImageNumber=8;
	SMenu[MN_GOD_INTRUDUCTION].nImageType=GOD_MENU;
	SMenu[MN_GOD_INTRUDUCTION].nFieldCount=3;
	SMenu[MN_GOD_INTRUDUCTION].CheakType=0;

// ���� ���
	SMenu[MN_GOD_INTRUDUCTION].nField[0].nType=FT_GOD_INTRUDUCTION;
	SMenu[MN_GOD_INTRUDUCTION].nField[0].x=34;
	SMenu[MN_GOD_INTRUDUCTION].nField[0].y=75;
	SMenu[MN_GOD_INTRUDUCTION].nField[0].nRectImage=242;

// EXIT
	SMenu[MN_GOD_INTRUDUCTION].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GOD_INTRUDUCTION].nField[1].nSpecialWillDo=SWD_SUBMENU_BACK;
	SMenu[MN_GOD_INTRUDUCTION].nField[1].x=135;
	SMenu[MN_GOD_INTRUDUCTION].nField[1].y=221;
	SMenu[MN_GOD_INTRUDUCTION].nField[1].nRectImage=12;
	SMenu[MN_GOD_INTRUDUCTION].nField[1].nImageNumber=13;
	SMenu[MN_GOD_INTRUDUCTION].nField[1].nImageType=GOD_MENU;
	SetRect(SMenu[MN_GOD_INTRUDUCTION].nField[1].rCheakBox, 135, 221, 207, 248);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	���� MEETING
	SMenu[MN_GOD_MEETING].x=155+GABX_SCREEN;
	SMenu[MN_GOD_MEETING].y=20;
	SMenu[MN_GOD_MEETING].nImageNumber=8;
	SMenu[MN_GOD_MEETING].nImageType=GOD_MENU;
	SMenu[MN_GOD_MEETING].nFieldCount=5;
	SMenu[MN_GOD_MEETING].CheakType=0;

// ���� ���
	SMenu[MN_GOD_MEETING].nField[0].nType=FT_GOD_MEETING;
	SMenu[MN_GOD_MEETING].nField[0].x=34;
	SMenu[MN_GOD_MEETING].nField[0].y=75;
	SMenu[MN_GOD_MEETING].nField[0].nSHideNomalCount=2;
	SMenu[MN_GOD_MEETING].nField[0].nSHideNomalStart=0;
	SMenu[MN_GOD_MEETING].nField[0].nShideNomalPlus=1;	
	SMenu[MN_GOD_MEETING].nField[0].nRectImage=242;

// EXIT
	SMenu[MN_GOD_MEETING].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GOD_MEETING].nField[1].nSpecialWillDo=SWD_SUBMENU_BACK;
	SMenu[MN_GOD_MEETING].nField[1].x=135;
	SMenu[MN_GOD_MEETING].nField[1].y=221;
	SMenu[MN_GOD_MEETING].nField[1].nRectImage=12;
	SMenu[MN_GOD_MEETING].nField[1].nImageNumber=13;
	SMenu[MN_GOD_MEETING].nField[1].nImageType=GOD_MENU;
	SetRect(SMenu[MN_GOD_MEETING].nField[1].rCheakBox, 135, 221, 207, 248);

// scroll button	// up
	SMenu[MN_GOD_MEETING].nField[2].nType=FT_HIDE_DSCROLL_AUTO_PUT;
	SMenu[MN_GOD_MEETING].nField[2].nWillDo=0;
	SMenu[MN_GOD_MEETING].nField[2].x=307;
	SMenu[MN_GOD_MEETING].nField[2].y=51;
	SMenu[MN_GOD_MEETING].nField[2].nRectImage=14;
	SMenu[MN_GOD_MEETING].nField[2].nImageNumber=15;
	SMenu[MN_GOD_MEETING].nField[2].nImageType=GOD_MENU;
	SetRect(SMenu[MN_GOD_MEETING].nField[2].rCheakBox,307,51,330,125);
					// down
	SMenu[MN_GOD_MEETING].nField[3].nType=FT_HIDE_ISCROLL_AUTO_PUT;
	SMenu[MN_GOD_MEETING].nField[3].nWillDo=0;
	SMenu[MN_GOD_MEETING].nField[3].x=307;
	SMenu[MN_GOD_MEETING].nField[3].y=125;
	SMenu[MN_GOD_MEETING].nField[3].nRectImage=16;
	SMenu[MN_GOD_MEETING].nField[3].nImageNumber=17;
	SMenu[MN_GOD_MEETING].nField[3].nImageType=GOD_MENU;
	SetRect(SMenu[MN_GOD_MEETING].nField[3].rCheakBox,307,125,330,201);

// MEETING ���� ���
	SMenu[MN_GOD_MEETING].nField[4].nType=FT_NOMAL_PUT;
	SMenu[MN_GOD_MEETING].nField[4].nSpecialWillDo=SWD_SUBMENU_BACK;
	SMenu[MN_GOD_MEETING].nField[4].x=21;
	SMenu[MN_GOD_MEETING].nField[4].y=47;
	SMenu[MN_GOD_MEETING].nField[4].nImageNumber=9;
	SMenu[MN_GOD_MEETING].nField[4].nImageType=GOD_MENU;
	SetRect(SMenu[MN_GOD_MEETING].nField[4].rCheakBox, 135, 221, 207, 248);




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// REGISTRATION
	SMenu[MN_GOD_REGIST1].x=216+GABX_SCREEN;
	SMenu[MN_GOD_REGIST1].y=53;
	SMenu[MN_GOD_REGIST1].nImageNumber=28;
	SMenu[MN_GOD_REGIST1].nImageType=GOD_MENU;
	SMenu[MN_GOD_REGIST1].nFieldCount=3;
	SMenu[MN_GOD_REGIST1].CheakType=0;

	//OK
	SMenu[MN_GOD_REGIST1].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GOD_REGIST1].nField[0].nSpecialWillDo=SWD_GOD_REGIST_OK;
	SMenu[MN_GOD_REGIST1].nField[0].x=25;
	SMenu[MN_GOD_REGIST1].nField[0].y=205;
	SMenu[MN_GOD_REGIST1].nField[0].nRectImage=29;
	SMenu[MN_GOD_REGIST1].nField[0].nImageNumber=30;
	SMenu[MN_GOD_REGIST1].nField[0].nImageType=GOD_MENU;
	SetRect(SMenu[MN_GOD_REGIST1].nField[0].rCheakBox,25,205,91,226);

	//CANCEL
	SMenu[MN_GOD_REGIST1].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GOD_REGIST1].nField[1].nSpecialWillDo=SWD_SUBMENU_BACK;
	SMenu[MN_GOD_REGIST1].nField[1].x=119;
	SMenu[MN_GOD_REGIST1].nField[1].y=205;
	SMenu[MN_GOD_REGIST1].nField[1].nRectImage=31;
	SMenu[MN_GOD_REGIST1].nField[1].nImageNumber=32;
	SMenu[MN_GOD_REGIST1].nField[1].nImageType=GOD_MENU;
	SetRect(SMenu[MN_GOD_REGIST1].nField[1].rCheakBox,119,205,185,226);

	//Message
	SMenu[MN_GOD_REGIST1].nField[2].nType=FT_GOD_REGIST_OKCANCEL;		// ����ϰڽ��ϱ�?
	SMenu[MN_GOD_REGIST1].nField[2].x=31;
	SMenu[MN_GOD_REGIST1].nField[2].y=114;
	SMenu[MN_GOD_REGIST1].nField[2].nRectImage=148;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ������ ��� Ȯ�� �޴�
	SMenu[MN_GOD_PREACH].x=216+GABX_SCREEN;
	SMenu[MN_GOD_PREACH].y=53;
	SMenu[MN_GOD_PREACH].nImageNumber=28;
	SMenu[MN_GOD_PREACH].nImageType=GOD_MENU;
	SMenu[MN_GOD_PREACH].nFieldCount=4;
	SMenu[MN_GOD_PREACH].CheakType=0;

	//OK
	SMenu[MN_GOD_PREACH].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GOD_PREACH].nField[0].nSpecialWillDo=SWD_PREACH_OK;
	SMenu[MN_GOD_PREACH].nField[0].nWillDo=3;	// �̸� ���� �ʵ���...
	SMenu[MN_GOD_PREACH].nField[0].x=25;
	SMenu[MN_GOD_PREACH].nField[0].y=205;
	SMenu[MN_GOD_PREACH].nField[0].nRectImage=29;
	SMenu[MN_GOD_PREACH].nField[0].nImageNumber=30;
	SMenu[MN_GOD_PREACH].nField[0].nImageType=GOD_MENU;
	SetRect(SMenu[MN_GOD_PREACH].nField[0].rCheakBox,25,205,91,226);

	//CANCEL
	SMenu[MN_GOD_PREACH].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GOD_PREACH].nField[1].nSpecialWillDo=SWD_PREACH_CANCEL;
	SMenu[MN_GOD_PREACH].nField[1].x=119;
	SMenu[MN_GOD_PREACH].nField[1].y=205;
	SMenu[MN_GOD_PREACH].nField[1].nRectImage=31;
	SMenu[MN_GOD_PREACH].nField[1].nImageNumber=32;
	SMenu[MN_GOD_PREACH].nField[1].nImageType=GOD_MENU;
	SetRect(SMenu[MN_GOD_PREACH].nField[1].rCheakBox,119,205,185,226);

	//Message
	SMenu[MN_GOD_PREACH].nField[2].nType=FT_DATA_PUT;		  // ������ ����� �̸��� �����ּ���.
	SMenu[MN_GOD_PREACH].nField[2].x=31;
	SMenu[MN_GOD_PREACH].nField[2].y=84;
	SMenu[MN_GOD_PREACH].nField[2].nRectImage=148;
	SMenu[MN_GOD_PREACH].nField[2].nImageNumber=DP_RECT_STRING_PUT;
	sprintf(SMenu[MN_GOD_PREACH].nField[2].temp,"%s", lan->OutputMessage(3,137));

	// name
	SMenu[MN_GOD_PREACH].nField[3].nType=FT_PREACH_NAME;		// ������ ��� �̸� ���� ��ƾ
	SMenu[MN_GOD_PREACH].nField[3].x=22;
	SMenu[MN_GOD_PREACH].nField[3].y=167;
	SMenu[MN_GOD_PREACH].nField[3].nRectImage=150;
	SMenu[MN_GOD_PREACH].nField[3].nImageNumber = 41;
	SMenu[MN_GOD_PREACH].nField[3].nImageType = GOD_MENU;
	SetRect(SMenu[MN_GOD_PREACH].nField[3].rCheakBox,22,167,187,199);


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���� ��� ��� �޴�
	SMenu[MN_GOD_PREACH_RESULT].x=216+GABX_SCREEN;
	SMenu[MN_GOD_PREACH_RESULT].y=53;
	SMenu[MN_GOD_PREACH_RESULT].nImageNumber=28;
	SMenu[MN_GOD_PREACH_RESULT].nImageType=GOD_MENU;
	SMenu[MN_GOD_PREACH_RESULT].nFieldCount=3;
	
	//OK
	SMenu[MN_GOD_PREACH_RESULT].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GOD_PREACH_RESULT].nField[0].nSpecialWillDo=SWD_GOD_PREACH_RESULT_OK;
	SMenu[MN_GOD_PREACH_RESULT].nField[0].x=25;
	SMenu[MN_GOD_PREACH_RESULT].nField[0].y=205;
	SMenu[MN_GOD_PREACH_RESULT].nField[0].nRectImage=29;
	SMenu[MN_GOD_PREACH_RESULT].nField[0].nImageNumber=30;
	SMenu[MN_GOD_PREACH_RESULT].nField[0].nImageType=GOD_MENU;
	SetRect(SMenu[MN_GOD_PREACH_RESULT].nField[0].rCheakBox,25,205,91,226);

	//CANCEL
	SMenu[MN_GOD_PREACH_RESULT].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GOD_PREACH_RESULT].nField[1].nSpecialWillDo=SWD_GOD_REGIST_OK;
	SMenu[MN_GOD_PREACH_RESULT].nField[1].x=119;
	SMenu[MN_GOD_PREACH_RESULT].nField[1].y=205;
	SMenu[MN_GOD_PREACH_RESULT].nField[1].nRectImage=31;
	SMenu[MN_GOD_PREACH_RESULT].nField[1].nImageNumber=32;
	SMenu[MN_GOD_PREACH_RESULT].nField[1].nImageType=GOD_MENU;
	SetRect(SMenu[MN_GOD_PREACH_RESULT].nField[1].rCheakBox,119,205,185,226);

	//Message
	SMenu[MN_GOD_PREACH_RESULT].nField[2].nType=FT_DATA_PUT;
	SMenu[MN_GOD_PREACH_RESULT].nField[2].x=31;
	SMenu[MN_GOD_PREACH_RESULT].nField[2].y=84;
	SMenu[MN_GOD_PREACH_RESULT].nField[2].nRectImage=148;
	SMenu[MN_GOD_PREACH_RESULT].nField[2].nImageNumber=DP_RECT_STRING_PUT;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ������ ��� Ȯ�� ��� �޴�	// ������ ����� ���� ����� ��쳪 �� ������ ��ϵ� ����� �ƴѰ��
	SMenu[MN_GOD_PREACH_ERROR1].x=216+GABX_SCREEN;
	SMenu[MN_GOD_PREACH_ERROR1].y=53;
	SMenu[MN_GOD_PREACH_ERROR1].nImageNumber=28;
	SMenu[MN_GOD_PREACH_ERROR1].nImageType=GOD_MENU;
	SMenu[MN_GOD_PREACH_ERROR1].nFieldCount=3;
	SMenu[MN_GOD_PREACH_ERROR1].CheakType=0;

	// OK Button ���� �����
	SMenu[MN_GOD_PREACH_ERROR1].nField[0].nType=FT_NOMAL_PUT;
	SMenu[MN_GOD_PREACH_ERROR1].nField[0].x=19;
	SMenu[MN_GOD_PREACH_ERROR1].nField[0].y=204;
	SMenu[MN_GOD_PREACH_ERROR1].nField[0].nImageNumber=33;
	SMenu[MN_GOD_PREACH_ERROR1].nField[0].nImageType=GOD_MENU;

	//OK
	SMenu[MN_GOD_PREACH_ERROR1].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GOD_PREACH_ERROR1].nField[1].nSpecialWillDo=SWD_GOD_REGIST_OK;
	SMenu[MN_GOD_PREACH_ERROR1].nField[1].x=70;
	SMenu[MN_GOD_PREACH_ERROR1].nField[1].y=205;
	SMenu[MN_GOD_PREACH_ERROR1].nField[1].nRectImage=29;
	SMenu[MN_GOD_PREACH_ERROR1].nField[1].nImageNumber=30;
	SMenu[MN_GOD_PREACH_ERROR1].nField[1].nImageType=GOD_MENU;
	SetRect(SMenu[MN_GOD_PREACH_ERROR1].nField[1].rCheakBox,70,205,136,226);

	//Message
	SMenu[MN_GOD_PREACH_ERROR1].nField[2].nType=FT_DATA_PUT;		  // ������ ����� �̸��� �����ּ���.
	SMenu[MN_GOD_PREACH_ERROR1].nField[2].x=31;
	SMenu[MN_GOD_PREACH_ERROR1].nField[2].y=84;
	SMenu[MN_GOD_PREACH_ERROR1].nField[2].nRectImage=148;
	SMenu[MN_GOD_PREACH_ERROR1].nField[2].nImageNumber=DP_RECT_STRING_PUT;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// REGISTRATION_ERROR	// ������ �̹� ����� �Ǿ� �ְų� �ٸ� ������ ��ϵǾ� ������
	SMenu[MN_GOD_REGIST_ERROR].x=216+GABX_SCREEN;
	SMenu[MN_GOD_REGIST_ERROR].y=53;
	SMenu[MN_GOD_REGIST_ERROR].nImageNumber=28;
	SMenu[MN_GOD_REGIST_ERROR].nImageType=GOD_MENU;
	SMenu[MN_GOD_REGIST_ERROR].nFieldCount=4;
	SMenu[MN_GOD_REGIST_ERROR].CheakType=0;

	// YES_NO Button ���� �����
	SMenu[MN_GOD_REGIST_ERROR].nField[0].nType=FT_NOMAL_PUT;
	SMenu[MN_GOD_REGIST_ERROR].nField[0].x=16;
	SMenu[MN_GOD_REGIST_ERROR].nField[0].y=199;
	SMenu[MN_GOD_REGIST_ERROR].nField[0].nImageNumber=40;
	SMenu[MN_GOD_REGIST_ERROR].nField[0].nImageType=GOD_MENU;

	//YES
	SMenu[MN_GOD_REGIST_ERROR].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GOD_REGIST_ERROR].nField[1].nSpecialWillDo=SWD_GOD_REGIST_ERROR_OK;
	SMenu[MN_GOD_REGIST_ERROR].nField[1].x=25;
	SMenu[MN_GOD_REGIST_ERROR].nField[1].y=205;
	SMenu[MN_GOD_REGIST_ERROR].nField[1].nRectImage=36;
	SMenu[MN_GOD_REGIST_ERROR].nField[1].nImageNumber=37;
	SMenu[MN_GOD_REGIST_ERROR].nField[1].nImageType=GOD_MENU;
	SetRect(SMenu[MN_GOD_REGIST_ERROR].nField[1].rCheakBox,25,205,91,226);

	//NO
	SMenu[MN_GOD_REGIST_ERROR].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GOD_REGIST_ERROR].nField[2].nSpecialWillDo=SWD_SUBMENU_BACK;
	SMenu[MN_GOD_REGIST_ERROR].nField[2].x=119;
	SMenu[MN_GOD_REGIST_ERROR].nField[2].y=205;
	SMenu[MN_GOD_REGIST_ERROR].nField[2].nRectImage=38;
	SMenu[MN_GOD_REGIST_ERROR].nField[2].nImageNumber=39;
	SMenu[MN_GOD_REGIST_ERROR].nField[2].nImageType=GOD_MENU;
	SetRect(SMenu[MN_GOD_REGIST_ERROR].nField[2].rCheakBox,119,205,185,226);

	//Message
	SMenu[MN_GOD_REGIST_ERROR].nField[3].nType=FT_GOD_REGIST_ERROR;
	SMenu[MN_GOD_REGIST_ERROR].nField[3].x=31;
	SMenu[MN_GOD_REGIST_ERROR].nField[3].y=84;
	SMenu[MN_GOD_REGIST_ERROR].nField[3].nRectImage=148;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ok ��ư�� �ִ� ���� ��Ͽ� ���� �޽��� ó�� �޴�
	SMenu[MN_GOD_REGIST_MESSAGE].x=216+GABX_SCREEN;
	SMenu[MN_GOD_REGIST_MESSAGE].y=53;
	SMenu[MN_GOD_REGIST_MESSAGE].nImageNumber=28;
	SMenu[MN_GOD_REGIST_MESSAGE].nImageType=GOD_MENU;
	SMenu[MN_GOD_REGIST_MESSAGE].nFieldCount=3;
	SMenu[MN_GOD_REGIST_MESSAGE].CheakType=0;

	// OK Button ���� �����
	SMenu[MN_GOD_REGIST_MESSAGE].nField[0].nType=FT_NOMAL_PUT;
	SMenu[MN_GOD_REGIST_MESSAGE].nField[0].x=19;
	SMenu[MN_GOD_REGIST_MESSAGE].nField[0].y=204;
	SMenu[MN_GOD_REGIST_MESSAGE].nField[0].nImageNumber=33;
	SMenu[MN_GOD_REGIST_MESSAGE].nField[0].nImageType=GOD_MENU;

	//OK
	SMenu[MN_GOD_REGIST_MESSAGE].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GOD_REGIST_MESSAGE].nField[1].nSpecialWillDo=SWD_SUBMENU_BACK;
	SMenu[MN_GOD_REGIST_MESSAGE].nField[1].x=70;
	SMenu[MN_GOD_REGIST_MESSAGE].nField[1].y=205;
	SMenu[MN_GOD_REGIST_MESSAGE].nField[1].nRectImage=29;
	SMenu[MN_GOD_REGIST_MESSAGE].nField[1].nImageNumber=30;
	SMenu[MN_GOD_REGIST_MESSAGE].nField[1].nImageType=GOD_MENU;
	SetRect(SMenu[MN_GOD_REGIST_MESSAGE].nField[1].rCheakBox,70,205,136,226);

	//Message
	SMenu[MN_GOD_REGIST_MESSAGE].nField[2].nType=FT_DATA_PUT;
	SMenu[MN_GOD_REGIST_MESSAGE].nField[2].x=31;
	SMenu[MN_GOD_REGIST_MESSAGE].nField[2].y=84;
	SMenu[MN_GOD_REGIST_MESSAGE].nField[2].nRectImage=148;
	SMenu[MN_GOD_REGIST_MESSAGE].nField[2].nImageNumber=DP_RECT_STRING_PUT;
//	sprintf(SMenu[MN_GOD_REGIST_MESSAGE].nField[2].temp, "����� �츮 ������ ����� ��� �ϼ̽��ϴ�.");

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �ð��� ��Ұ� ���� �� �ߴ� ���� �޴�

	SMenu[MN_GOD_MEETING_MAIN].x=155+GABX_SCREEN;
	SMenu[MN_GOD_MEETING_MAIN].y=20;
	SMenu[MN_GOD_MEETING_MAIN].nImageNumber=8;
	SMenu[MN_GOD_MEETING_MAIN].nImageType=GOD_MENU;
	SMenu[MN_GOD_MEETING_MAIN].nFieldCount=3;
	SMenu[MN_GOD_MEETING_MAIN].CheakType=0;

	// ��ư ��� ������
	SMenu[MN_GOD_MEETING_MAIN].nField[0].nType=FT_NOMAL_PUT;
	SMenu[MN_GOD_MEETING_MAIN].nField[0].x=16;
	SMenu[MN_GOD_MEETING_MAIN].nField[0].y=43;
	SMenu[MN_GOD_MEETING_MAIN].nField[0].nImageNumber=11;
	SMenu[MN_GOD_MEETING_MAIN].nField[0].nImageType=GOD_MENU;

	//Message
	SMenu[MN_GOD_MEETING_MAIN].nField[1].nType=FT_VIEW_GOD_MEETING;
	SMenu[MN_GOD_MEETING_MAIN].nField[1].x=45;
	SMenu[MN_GOD_MEETING_MAIN].nField[1].y=72;
	SMenu[MN_GOD_MEETING_MAIN].nField[1].nRectImage=255;

	// �޴� �����̱�
	SMenu[MN_GOD_MEETING_MAIN].nField[2].nType=FT_MENU_MOVE_DEFAULT;
	SetRect( SMenu[MN_GOD_MEETING_MAIN].nField[2].rCheakBox,0,0,348,262 );


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	�������� ���� ���� �޴�   // SUPPER

	SMenu[MN_GOD_SUPPER].x=198+GABX_SCREEN;
	SMenu[MN_GOD_SUPPER].y=61;
	SMenu[MN_GOD_SUPPER].nImageNumber=42;
	SMenu[MN_GOD_SUPPER].nImageType=GOD_MENU;
	SMenu[MN_GOD_SUPPER].nFieldCount=3;
	SMenu[MN_GOD_SUPPER].CheakType=0;

	SMenu[MN_GOD_SUPPER].nField[0].nType=FT_SUPPER_INV;				//���� ������ �ϴ� �κ� ����
	SMenu[MN_GOD_SUPPER].nField[0].x=32;
	SMenu[MN_GOD_SUPPER].nField[0].y=70;
	SetRect(SMenu[MN_GOD_SUPPER].nField[0].rCheakBox,32,70,207,175);

	//OK
	SMenu[MN_GOD_SUPPER].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GOD_SUPPER].nField[1].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_GOD_SUPPER].nField[1].nWillDo=3;	// �̸� ���� �ʵ���...
	SMenu[MN_GOD_SUPPER].nField[1].x=34+3;
	SMenu[MN_GOD_SUPPER].nField[1].y=186+3;
	SMenu[MN_GOD_SUPPER].nField[1].nRectImage=29;
	SMenu[MN_GOD_SUPPER].nField[1].nImageNumber=30;
	SMenu[MN_GOD_SUPPER].nField[1].nImageType=GOD_MENU;
	SetRect(SMenu[MN_GOD_SUPPER].nField[1].rCheakBox,34,186,106,213);

	//EXIT
	SMenu[MN_GOD_SUPPER].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GOD_SUPPER].nField[2].nSpecialWillDo=SWD_SUPPER_BACK;
	SMenu[MN_GOD_SUPPER].nField[2].x=137;
	SMenu[MN_GOD_SUPPER].nField[2].y=186;
	SMenu[MN_GOD_SUPPER].nField[2].nRectImage=12;
	SMenu[MN_GOD_SUPPER].nField[2].nImageNumber=13;
	SMenu[MN_GOD_SUPPER].nField[2].nImageType=GOD_MENU;
	SetRect(SMenu[MN_GOD_SUPPER].nField[2].rCheakBox,137,186,209,213);


/*
	SMenu[MN_GOD_SUPPER].nField[0].nType=FT_NOMAL_PUT;
	SMenu[MN_GOD_SUPPER].nField[0].x=16;
	SMenu[MN_GOD_SUPPER].nField[0].y=43;
	SMenu[MN_GOD_SUPPER].nField[0].nImageNumber=11;
	SMenu[MN_GOD_SUPPER].nField[0].nImageType=GOD_MENU;

	SMenu[MN_GOD_SUPPER].nField[1].nType=FT_VIEW_GOD_MEETING;
	SMenu[MN_GOD_SUPPER].nField[1].x=31;
	SMenu[MN_GOD_SUPPER].nField[1].y=84;
	SMenu[MN_GOD_SUPPER].nField[1].nRectImage=148;
*/ 



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�����޴�2
	SMenu[MN_GOD2].x=216+GABX_SCREEN;
	SMenu[MN_GOD2].y=20;
	SMenu[MN_GOD2].nImageNumber=792;
	SMenu[MN_GOD2].nFieldCount=2;
	SMenu[MN_GOD2].CheakType=0;

	//OK
	SMenu[MN_GOD2].nField[0].nType=FT_HIDE_WILLDO_AUTO_PUT;
	SMenu[MN_GOD2].nField[0].nWillDo=MN_GOD3;
	SMenu[MN_GOD2].nField[0].x=21;
	SMenu[MN_GOD2].nField[0].y=259;
	SMenu[MN_GOD2].nField[0].nRectImage=794;
	SMenu[MN_GOD2].nField[0].nImageNumber=795;
	SetRect(SMenu[MN_GOD2].nField[0].rCheakBox,21,259,89,279);

	//CANCEL
	SMenu[MN_GOD2].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GOD2].nField[1].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_GOD2].nField[1].x=123;
	SMenu[MN_GOD2].nField[1].y=259;
	SMenu[MN_GOD2].nField[1].nRectImage=796;
	SMenu[MN_GOD2].nField[1].nImageNumber=797;
	SetRect(SMenu[MN_GOD2].nField[1].rCheakBox,120,259,188,279);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//�����޴�3

	SMenu[MN_GOD3].x=216+GABX_SCREEN;
	SMenu[MN_GOD3].y=20;
	SMenu[MN_GOD3].nImageNumber=793;
	SMenu[MN_GOD3].nFieldCount=1;
	SMenu[MN_GOD3].CheakType=0;

	//OK
	SMenu[MN_GOD3].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GOD3].nField[0].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_GOD3].nField[0].x=72;
	SMenu[MN_GOD3].nField[0].y=259;
	SMenu[MN_GOD3].nField[0].nRectImage=794;
	SMenu[MN_GOD3].nField[0].nImageNumber=795;
	SetRect(SMenu[MN_GOD3].nField[0].rCheakBox,71,259,139,279);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//��� ������ �޴�

	SMenu[MN_FUNCTION].x=197;
	SMenu[MN_FUNCTION].y=20;
	SMenu[MN_FUNCTION].nImageNumber=800;
	SMenu[MN_FUNCTION].nFieldCount=2;
	SMenu[MN_FUNCTION].CheakType=0;

	//CNACEL
	SMenu[MN_FUNCTION].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_FUNCTION].nField[0].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_FUNCTION].nField[0].x=156;
	SMenu[MN_FUNCTION].nField[0].y=233;
	SMenu[MN_FUNCTION].nField[0].nRectImage=292;
	SMenu[MN_FUNCTION].nField[0].nImageNumber=293;
	SetRect(SMenu[MN_FUNCTION].nField[0].rCheakBox,157,234,190,268);

	//OK
	SMenu[MN_FUNCTION].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_FUNCTION].nField[1].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_FUNCTION].nField[1].x=48;
	SMenu[MN_FUNCTION].nField[1].y=233;
	SMenu[MN_FUNCTION].nField[1].nRectImage=294;
	SMenu[MN_FUNCTION].nField[1].nImageNumber=295;
	SetRect(SMenu[MN_FUNCTION].nField[1].rCheakBox,49,234,83,268);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	���� 
	SMenu[MN_DRINK_MAIN].x = 378+GABX_SCREEN;
	SMenu[MN_DRINK_MAIN].y = 26;
	SMenu[MN_DRINK_MAIN].nImageNumber = 529;
	SMenu[MN_DRINK_MAIN].nFieldCount = 4;

	SMenu[MN_DRINK_MAIN].nField[0].nType = FT_HIDE_NOMAL_GROUP_PUT;
	SMenu[MN_DRINK_MAIN].nField[0].nSHideNomalNumber=HN_DRINK_MAIN_TEXT;
	SMenu[MN_DRINK_MAIN].nField[0].nSHideNomalCount=2;
	SMenu[MN_DRINK_MAIN].nField[0].nSHideNomalStart=0;
	SMenu[MN_DRINK_MAIN].nField[0].nShideNomalPlus=2;

	// done button
	SMenu[MN_DRINK_MAIN].nField[1].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_DRINK_MAIN].nField[1].nSpecialWillDo = SWD_SMALLMENU_END;
	SMenu[MN_DRINK_MAIN].nField[1].x = 66;
	SMenu[MN_DRINK_MAIN].nField[1].y = 240;
	SMenu[MN_DRINK_MAIN].nField[1].nRectImage = 530;
	SMenu[MN_DRINK_MAIN].nField[1].nImageNumber = 531;
	SetRect(SMenu[MN_DRINK_MAIN].nField[1].rCheakBox,67,240,141,266);

	// Rumor open
	SMenu[MN_DRINK_MAIN].nField[2].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_DRINK_MAIN].nField[2].nSpecialWillDo = SWD_WILLDO;
	SMenu[MN_DRINK_MAIN].nField[2].nWillDo = MN_DRINK_RUMOR;
	SMenu[MN_DRINK_MAIN].nField[2].x = 51;
	SMenu[MN_DRINK_MAIN].nField[2].y = 123;
	SMenu[MN_DRINK_MAIN].nField[2].nRectImage = 758;
	SMenu[MN_DRINK_MAIN].nField[2].nImageNumber = 758;
	SetRect(SMenu[MN_DRINK_MAIN].nField[2].rCheakBox,51,126,155,152);

	// Buy open
	SMenu[MN_DRINK_MAIN].nField[3].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_DRINK_MAIN].nField[3].nSpecialWillDo = SWD_SELL_BUY;
	SMenu[MN_DRINK_MAIN].nField[3].nWillDo = MN_SKILLGUILD_BUY;
	SMenu[MN_DRINK_MAIN].nField[3].x = 51;
	SMenu[MN_DRINK_MAIN].nField[3].y = 158;
	SMenu[MN_DRINK_MAIN].nField[3].nRectImage = 758;
	SMenu[MN_DRINK_MAIN].nField[3].nImageNumber = 758;
	SetRect(SMenu[MN_DRINK_MAIN].nField[3].rCheakBox,51,161,155,187);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	���� - rumor

	SMenu[MN_DRINK_RUMOR].x = 222+GABX_SCREEN;
	SMenu[MN_DRINK_RUMOR].y = 80;
	SMenu[MN_DRINK_RUMOR].nImageNumber = 532;
	SMenu[MN_DRINK_RUMOR].nFieldCount = 3;

	SMenu[MN_DRINK_RUMOR].nField[0].nType = FT_DATA_PUT;
	SMenu[MN_DRINK_RUMOR].nField[0].x=48;
	SMenu[MN_DRINK_RUMOR].nField[0].y=67;
	SMenu[MN_DRINK_RUMOR].nField[0].nImageNumber=DP_RECT_STRING_PUT;
	SMenu[MN_DRINK_RUMOR].nField[0].nRectImage=118;
#ifdef CHANGE_MONEY_
	sprintf(SMenu[MN_DRINK_RUMOR].nField[0].temp,"%s", lan->OutputMessage(3,138));//sw
#else
	sprintf(SMenu[MN_DRINK_RUMOR].nField[0].temp,lan->OutputMessage(3,139));//lsw
#endif

	// OK button
	SMenu[MN_DRINK_RUMOR].nField[1].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_DRINK_RUMOR].nField[1].nSpecialWillDo = SWD_RUMOR_OK;
	SMenu[MN_DRINK_RUMOR].nField[1].x = 23;
	SMenu[MN_DRINK_RUMOR].nField[1].y = 180;
	SMenu[MN_DRINK_RUMOR].nField[1].nRectImage = 533;
	SMenu[MN_DRINK_RUMOR].nField[1].nImageNumber = 534;
	SetRect(SMenu[MN_DRINK_RUMOR].nField[1].rCheakBox,23,180,93,207);

	// done Button
	SMenu[MN_DRINK_RUMOR].nField[2].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_DRINK_RUMOR].nField[2].nSpecialWillDo = SWD_WILLDO ;
	SMenu[MN_DRINK_RUMOR].nField[2].nWillDo = MN_DRINK_MAIN;
	SMenu[MN_DRINK_RUMOR].nField[2].x = 116;
	SMenu[MN_DRINK_RUMOR].nField[2].y = 180;
	SMenu[MN_DRINK_RUMOR].nField[2].nRectImage = 535;
	SMenu[MN_DRINK_RUMOR].nField[2].nImageNumber = 536;
	SetRect(SMenu[MN_DRINK_RUMOR].nField[2].rCheakBox,116,180,186,207);


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	�ҹ��� �˷��ش�.
	SMenu[MN_DRINK_RUMOR_VIEW].x = 222+GABX_SCREEN;
	SMenu[MN_DRINK_RUMOR_VIEW].y = 80;
	SMenu[MN_DRINK_RUMOR_VIEW].nImageNumber = 532;
	SMenu[MN_DRINK_RUMOR_VIEW].nFieldCount = 3;

	SMenu[MN_DRINK_RUMOR_VIEW].nField[0].nType = FT_DATA_PUT;
	SMenu[MN_DRINK_RUMOR_VIEW].nField[0].x=48;
	SMenu[MN_DRINK_RUMOR_VIEW].nField[0].y=67;
	SMenu[MN_DRINK_RUMOR_VIEW].nField[0].nImageNumber=DP_RECT_STRING_PUT;
	SMenu[MN_DRINK_RUMOR_VIEW].nField[0].nRectImage=118;
	sprintf(SMenu[MN_DRINK_RUMOR_VIEW].nField[0].temp,"%s", lan->OutputMessage(3,140));//lsw

	// OK button
	SMenu[MN_DRINK_RUMOR_VIEW].nField[1].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_DRINK_RUMOR_VIEW].nField[1].nSpecialWillDo = SWD_SMALLMENU_END;
	SMenu[MN_DRINK_RUMOR_VIEW].nField[1].x = 23;
	SMenu[MN_DRINK_RUMOR_VIEW].nField[1].y = 180;
	SMenu[MN_DRINK_RUMOR_VIEW].nField[1].nRectImage = 533;
	SMenu[MN_DRINK_RUMOR_VIEW].nField[1].nImageNumber = 534;
	SetRect(SMenu[MN_DRINK_RUMOR_VIEW].nField[1].rCheakBox,23,180,93,207);

	// done Button
	SMenu[MN_DRINK_RUMOR_VIEW].nField[2].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_DRINK_RUMOR_VIEW].nField[2].nSpecialWillDo = SWD_SMALLMENU_END;
	SMenu[MN_DRINK_RUMOR_VIEW].nField[2].x = 116;
	SMenu[MN_DRINK_RUMOR_VIEW].nField[2].y = 180;
	SMenu[MN_DRINK_RUMOR_VIEW].nField[2].nRectImage = 535;
	SMenu[MN_DRINK_RUMOR_VIEW].nField[2].nImageNumber = 536;
	SetRect(SMenu[MN_DRINK_RUMOR_VIEW].nField[2].rCheakBox,116,180,186,207);




///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	���� - buy

	SMenu[MN_DRINK_BUY].x=345+GABX_SCREEN;
	SMenu[MN_DRINK_BUY].y=20;
#ifdef CHANGE_MONEY_
	SMenu[MN_DRINK_BUY].nImageNumber=4;
	SMenu[MN_DRINK_BUY].nImageType=CHANGE_MONEY;
#else 
	SMenu[MN_DRINK_BUY].nImageNumber=609;
#endif
	SMenu[MN_DRINK_BUY].nFieldCount=9;
	SMenu[MN_DRINK_BUY].CheakType=0;

	// �� �����
//	SMenu[MN_DRINK_BUY].nField[0].nType=FT_NOMAL_PUT;
//	SMenu[MN_DRINK_BUY].nField[0].x=0;
//	SMenu[MN_DRINK_BUY].nField[0].y=0;
//	SMenu[MN_DRINK_BUY].nField[0].nImageNumber=537;

	// ���� ����	
	SMenu[MN_DRINK_BUY].nField[1].nType=FT_NOMAL_PUT;
	SMenu[MN_DRINK_BUY].nField[1].x=66;
	SMenu[MN_DRINK_BUY].nField[1].y=10;
	SMenu[MN_DRINK_BUY].nField[1].nImageNumber=610;

	// ok, cancel  Button 
	SMenu[MN_DRINK_BUY].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
//	SMenu[MN_DRINK_BUY].nField[2].nSpecialWillDo=SWD_ITEM_SELLBUY_OK;
	SMenu[MN_DRINK_BUY].nField[2].nSpecialWillDo=SWD_SORRY;
	SMenu[MN_DRINK_BUY].nField[2].nWillDo=ITEM_BUY;
	SMenu[MN_DRINK_BUY].nField[2].x=23;
	SMenu[MN_DRINK_BUY].nField[2].y=252;
	SMenu[MN_DRINK_BUY].nField[2].nRectImage=294;
	SMenu[MN_DRINK_BUY].nField[2].nImageNumber=295;
	SetRect(SMenu[MN_DRINK_BUY].nField[2].rCheakBox,20,250, 65,295);

	SMenu[MN_DRINK_BUY].nField[3].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_DRINK_BUY].nField[3].nSpecialWillDo=SWD_ITEM_SELLBUY_CANCEL;
	SMenu[MN_DRINK_BUY].nField[3].x=181;
	SMenu[MN_DRINK_BUY].nField[3].y=252;
	SMenu[MN_DRINK_BUY].nField[3].nRectImage=292;
	SMenu[MN_DRINK_BUY].nField[3].nImageNumber=293;
	SetRect(SMenu[MN_DRINK_BUY].nField[3].rCheakBox,179,250,223,295);

	// scroll butoon	// up
	SMenu[MN_DRINK_BUY].nField[4].nType=FT_HIDE_DSCROLL_AUTO_PUT;
	SMenu[MN_DRINK_BUY].nField[4].nWillDo=6;
	SMenu[MN_DRINK_BUY].nField[4].x=205;
	SMenu[MN_DRINK_BUY].nField[4].y=37;
	SMenu[MN_DRINK_BUY].nField[4].nRectImage=611;
	SMenu[MN_DRINK_BUY].nField[4].nImageNumber=612;
	SetRect(SMenu[MN_DRINK_BUY].nField[4].rCheakBox,203,35,223,70);
						// down					
	SMenu[MN_DRINK_BUY].nField[5].nType=FT_HIDE_ISCROLL_AUTO_PUT;
	SMenu[MN_DRINK_BUY].nField[5].nWillDo=6;
	SMenu[MN_DRINK_BUY].nField[5].x=205;
	SMenu[MN_DRINK_BUY].nField[5].y=108;
	SMenu[MN_DRINK_BUY].nField[5].nRectImage=613;
	SMenu[MN_DRINK_BUY].nField[5].nImageNumber=614;
	SetRect(SMenu[MN_DRINK_BUY].nField[5].rCheakBox,203,106,223,141);

	// item inventory 
	SMenu[MN_DRINK_BUY].nField[6].nType=FT_JOB_ITEM;
	SMenu[MN_DRINK_BUY].nField[6].nWillDo=ITEM_BUY;
	SMenu[MN_DRINK_BUY].nField[6].nSHideNomalCount=1;
	SMenu[MN_DRINK_BUY].nField[6].nSHideNomalStart=0;
	SMenu[MN_DRINK_BUY].nField[6].nShideNomalPlus=1;	
	SetRect(SMenu[MN_DRINK_BUY].nField[6].rCheakBox,25,35,200,140);

	// ������ ȭ�� ���
	SMenu[MN_DRINK_BUY].nField[7].nType=FT_DATA_PUT;
	SMenu[MN_DRINK_BUY].nField[7].nWillDo=ITEM_BUY;
	SMenu[MN_DRINK_BUY].nField[7].x=37;
	SMenu[MN_DRINK_BUY].nField[7].y=168;
	SMenu[MN_DRINK_BUY].nField[7].nImageNumber=DP_SELECT_ITEM;

	// �� ���
	SMenu[MN_DRINK_BUY].nField[8].nType=FT_DATA_PUT;
	SMenu[MN_DRINK_BUY].nField[8].nWillDo=75;
	SMenu[MN_DRINK_BUY].nField[8].x=75;
	SMenu[MN_DRINK_BUY].nField[8].y=266;
	SMenu[MN_DRINK_BUY].nField[8].nImageNumber=DP_CHARACTER_MONEY;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////		
//////////////////////////////////////////////////////////////////////////////////////////////////////////////	
//	��ȭ�� 
	SMenu[MN_SHOP_MAIN].x = 378+GABX_SCREEN;
	SMenu[MN_SHOP_MAIN].y = 26;
	SMenu[MN_SHOP_MAIN].nImageNumber = 538;
	SMenu[MN_SHOP_MAIN].nFieldCount = 5;

	SMenu[MN_SHOP_MAIN].nField[0].nType = FT_HIDE_NOMAL_GROUP_PUT;
	SMenu[MN_SHOP_MAIN].nField[0].nSHideNomalNumber=HN_SHOP_MAIN_TEXT;
	SMenu[MN_SHOP_MAIN].nField[0].nSHideNomalCount=2;
	SMenu[MN_SHOP_MAIN].nField[0].nSHideNomalStart=0;
	SMenu[MN_SHOP_MAIN].nField[0].nShideNomalPlus=2;

	// done button
	SMenu[MN_SHOP_MAIN].nField[1].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_SHOP_MAIN].nField[1].nSpecialWillDo = SWD_SMALLMENU_END;
	SMenu[MN_SHOP_MAIN].nField[1].x = 72;
	SMenu[MN_SHOP_MAIN].nField[1].y = 249;
	SMenu[MN_SHOP_MAIN].nField[1].nRectImage = 492;
	SMenu[MN_SHOP_MAIN].nField[1].nImageNumber = 493;
	SetRect(SMenu[MN_SHOP_MAIN].nField[1].rCheakBox,72,249,138,270 );

	// Buy open
	SMenu[MN_SHOP_MAIN].nField[2].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_SHOP_MAIN].nField[2].nSpecialWillDo = SWD_SELL_BUY;
	SMenu[MN_SHOP_MAIN].nField[2].nWillDo = MN_SKILLGUILD_BUY;
	SMenu[MN_SHOP_MAIN].nField[2].x = 52;
	SMenu[MN_SHOP_MAIN].nField[2].y = 142;
	SMenu[MN_SHOP_MAIN].nField[2].nRectImage = 758;
	SMenu[MN_SHOP_MAIN].nField[2].nImageNumber = 758;
	SetRect(SMenu[MN_SHOP_MAIN].nField[2].rCheakBox,52,142,156,168);

	// SELL open
	SMenu[MN_SHOP_MAIN].nField[3].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_SHOP_MAIN].nField[3].nSpecialWillDo = SWD_SELL_BUY;
	SMenu[MN_SHOP_MAIN].nField[3].nWillDo = MN_SKILLGUILD_SELL;
	SMenu[MN_SHOP_MAIN].nField[3].x = 52;
	SMenu[MN_SHOP_MAIN].nField[3].y = 173;
	SMenu[MN_SHOP_MAIN].nField[3].nRectImage = 758;
	SMenu[MN_SHOP_MAIN].nField[3].nImageNumber = 758;
	SetRect(SMenu[MN_SHOP_MAIN].nField[3].rCheakBox,52,173,156,199);

	// ���� �̸� �� ���̱�
	SMenu[MN_SHOP_MAIN].nField[4].nType = FT_NOMAL_PUT;
	SMenu[MN_SHOP_MAIN].nField[4].x = 24;
	SMenu[MN_SHOP_MAIN].nField[4].y = 45;
	SMenu[MN_SHOP_MAIN].nField[4].nImageNumber = 1;
	SMenu[MN_SHOP_MAIN].nField[4].nImageType = SMALL_ETC;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	��ȭ�� - BUY

	SMenu[MN_SHOP_BUY].x=345+GABX_SCREEN;
	SMenu[MN_SHOP_BUY].y=20;
#ifdef CHANGE_MONEY_
	SMenu[MN_SHOP_BUY].nImageNumber=4;
	SMenu[MN_SHOP_BUY].nImageType=CHANGE_MONEY;
#else 
	SMenu[MN_SHOP_BUY].nImageNumber=609;
#endif
	SMenu[MN_SHOP_BUY].nFieldCount=9;
	SMenu[MN_SHOP_BUY].CheakType=0;

	// �� �����
	SMenu[MN_SHOP_BUY].nField[0].nType=FT_NOMAL_PUT;
	SMenu[MN_SHOP_BUY].nField[0].x=0;
	SMenu[MN_SHOP_BUY].nField[0].y=0;
	SMenu[MN_SHOP_BUY].nField[0].nImageNumber=541;

	// ���� ����	
	SMenu[MN_SHOP_BUY].nField[1].nType=FT_NOMAL_PUT;
	SMenu[MN_SHOP_BUY].nField[1].x=66;
	SMenu[MN_SHOP_BUY].nField[1].y=10;
	SMenu[MN_SHOP_BUY].nField[1].nImageNumber=610;

	// ok, cancel  Button 
	SMenu[MN_SHOP_BUY].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_SHOP_BUY].nField[2].nSpecialWillDo=SWD_ITEM_SELLBUY_OK;
	SMenu[MN_SHOP_BUY].nField[2].nWillDo=ITEM_BUY;
	SMenu[MN_SHOP_BUY].nField[2].x=23;
	SMenu[MN_SHOP_BUY].nField[2].y=252;
	SMenu[MN_SHOP_BUY].nField[2].nRectImage=294;
	SMenu[MN_SHOP_BUY].nField[2].nImageNumber=295;
	SetRect(SMenu[MN_SHOP_BUY].nField[2].rCheakBox,20,250, 65,295);

	SMenu[MN_SHOP_BUY].nField[3].nType=FT_HIDE_WILLDO_AUTO_PUT;
	SMenu[MN_SHOP_BUY].nField[3].nWillDo=MN_SHOP_MAIN;
	SMenu[MN_SHOP_BUY].nField[3].x=181;
	SMenu[MN_SHOP_BUY].nField[3].y=252;
	SMenu[MN_SHOP_BUY].nField[3].nRectImage=292;
	SMenu[MN_SHOP_BUY].nField[3].nImageNumber=293;
	SetRect(SMenu[MN_SHOP_BUY].nField[3].rCheakBox,179,250,223,295);

	// scroll butoon	// up
	SMenu[MN_SHOP_BUY].nField[4].nType=FT_HIDE_DSCROLL_AUTO_PUT;
	SMenu[MN_SHOP_BUY].nField[4].nWillDo=6;
	SMenu[MN_SHOP_BUY].nField[4].x=205;
	SMenu[MN_SHOP_BUY].nField[4].y=37;
	SMenu[MN_SHOP_BUY].nField[4].nRectImage=611;
	SMenu[MN_SHOP_BUY].nField[4].nImageNumber=612;
	SetRect(SMenu[MN_SHOP_BUY].nField[4].rCheakBox,203,35,223,70);
						// down					
	SMenu[MN_SHOP_BUY].nField[5].nType=FT_HIDE_ISCROLL_AUTO_PUT;
	SMenu[MN_SHOP_BUY].nField[5].nWillDo=6;
	SMenu[MN_SHOP_BUY].nField[5].x=205;
	SMenu[MN_SHOP_BUY].nField[5].y=108;
	SMenu[MN_SHOP_BUY].nField[5].nRectImage=613;
	SMenu[MN_SHOP_BUY].nField[5].nImageNumber=614;
	SetRect(SMenu[MN_SHOP_BUY].nField[5].rCheakBox,203,106,223,141);

	// item inventory 
	SMenu[MN_SHOP_BUY].nField[6].nType=FT_JOB_ITEM;
	SMenu[MN_SHOP_BUY].nField[6].nWillDo=ITEM_BUY;
	SMenu[MN_SHOP_BUY].nField[6].nSHideNomalCount=5;
	SMenu[MN_SHOP_BUY].nField[6].nSHideNomalStart=0;
	SMenu[MN_SHOP_BUY].nField[6].nShideNomalPlus=1;	
	SetRect(SMenu[MN_SHOP_BUY].nField[6].rCheakBox,25,35,200,140);

	// ������ ȭ�� ���
	SMenu[MN_SHOP_BUY].nField[7].nType=FT_DATA_PUT;
	SMenu[MN_SHOP_BUY].nField[7].nWillDo=ITEM_BUY;
	SMenu[MN_SHOP_BUY].nField[7].x=37;
	SMenu[MN_SHOP_BUY].nField[7].y=168;
	SMenu[MN_SHOP_BUY].nField[7].nImageNumber=DP_SELECT_ITEM;

	// �� ���
	SMenu[MN_SHOP_BUY].nField[8].nType=FT_DATA_PUT;
	SMenu[MN_SHOP_BUY].nField[8].nWillDo=75;
	SMenu[MN_SHOP_BUY].nField[8].x=75;
	SMenu[MN_SHOP_BUY].nField[8].y=266;
	SMenu[MN_SHOP_BUY].nField[8].nImageNumber=DP_CHARACTER_MONEY;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	��ȭ�� - SELL
	SMenu[MN_SHOP_SELL].x=345+GABX_SCREEN;
	SMenu[MN_SHOP_SELL].y=20;
#ifdef CHANGE_MONEY_
	SMenu[MN_SHOP_SELL].nImageNumber=4;
	SMenu[MN_SHOP_SELL].nImageType=CHANGE_MONEY;
#else 
	SMenu[MN_SHOP_SELL].nImageNumber=609;
#endif
	SMenu[MN_SHOP_SELL].nFieldCount=9;
	SMenu[MN_SHOP_SELL].CheakType=0;

	// �� �����
	SMenu[MN_SHOP_SELL].nField[0].nType=FT_NOMAL_PUT;
	SMenu[MN_SHOP_SELL].nField[0].x=0;
	SMenu[MN_SHOP_SELL].nField[0].y=0;
	SMenu[MN_SHOP_SELL].nField[0].nImageNumber=541;

	// ���� ����	
	SMenu[MN_SHOP_SELL].nField[1].nType=FT_NO_CHECK;
	SMenu[MN_SHOP_SELL].nField[1].x=66;
	SMenu[MN_SHOP_SELL].nField[1].y=10;
	SMenu[MN_SHOP_SELL].nField[1].nImageNumber=610;

	// ok, cancel  Button 
	SMenu[MN_SHOP_SELL].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_SHOP_SELL].nField[2].nSpecialWillDo=SWD_ITEM_SELLBUY_OK;
	SMenu[MN_SHOP_SELL].nField[2].nWillDo=ITEM_SELL;
	SMenu[MN_SHOP_SELL].nField[2].x=23;
	SMenu[MN_SHOP_SELL].nField[2].y=252;
	SMenu[MN_SHOP_SELL].nField[2].nRectImage=294;
	SMenu[MN_SHOP_SELL].nField[2].nImageNumber=295;
	SetRect(SMenu[MN_SHOP_SELL].nField[2].rCheakBox,20,250, 65,295);

	SMenu[MN_SHOP_SELL].nField[3].nType=FT_HIDE_WILLDO_AUTO_PUT;
	SMenu[MN_SHOP_SELL].nField[3].nWillDo=MN_SHOP_MAIN;
	SMenu[MN_SHOP_SELL].nField[3].x=181;
	SMenu[MN_SHOP_SELL].nField[3].y=252;
	SMenu[MN_SHOP_SELL].nField[3].nRectImage=292;
	SMenu[MN_SHOP_SELL].nField[3].nImageNumber=293;
	SetRect(SMenu[MN_SHOP_SELL].nField[3].rCheakBox,179,250,223,295);

	// scroll butoon	// up
	SMenu[MN_SHOP_SELL].nField[4].nType=FT_HIDE_DSCROLL_AUTO_PUT;
	SMenu[MN_SHOP_SELL].nField[4].nWillDo=6;
	SMenu[MN_SHOP_SELL].nField[4].x=205;
	SMenu[MN_SHOP_SELL].nField[4].y=37;
	SMenu[MN_SHOP_SELL].nField[4].nRectImage=611;
	SMenu[MN_SHOP_SELL].nField[4].nImageNumber=612;
	SetRect(SMenu[MN_SHOP_SELL].nField[4].rCheakBox,203,35,223,70);
						// down					
	SMenu[MN_SHOP_SELL].nField[5].nType=FT_HIDE_ISCROLL_AUTO_PUT;
	SMenu[MN_SHOP_SELL].nField[5].nWillDo=6;
	SMenu[MN_SHOP_SELL].nField[5].x=205;
	SMenu[MN_SHOP_SELL].nField[5].y=108;
	SMenu[MN_SHOP_SELL].nField[5].nRectImage=613;
	SMenu[MN_SHOP_SELL].nField[5].nImageNumber=614;
	SetRect(SMenu[MN_SHOP_SELL].nField[5].rCheakBox,203,106,223,141);

	// item inventory 
	SMenu[MN_SHOP_SELL].nField[6].nType=FT_JOB_ITEM;
	SMenu[MN_SHOP_SELL].nField[6].nWillDo=ITEM_SELL;
	SMenu[MN_SHOP_SELL].nField[6].nSHideNomalCount=5;
	SMenu[MN_SHOP_SELL].nField[6].nSHideNomalStart=0;
	SMenu[MN_SHOP_SELL].nField[6].nShideNomalPlus=1;
	SetRect(SMenu[MN_SHOP_SELL].nField[6].rCheakBox,25,35,200,140);

	// ������ ȭ�� ���
	SMenu[MN_SHOP_SELL].nField[7].nType=FT_DATA_PUT;
	SMenu[MN_SHOP_SELL].nField[7].nWillDo=ITEM_SELL;
	SMenu[MN_SHOP_SELL].nField[7].x=37;
	SMenu[MN_SHOP_SELL].nField[7].y=168;
	SMenu[MN_SHOP_SELL].nField[7].nImageNumber=DP_SELECT_ITEM;

	// �� ���
	SMenu[MN_SHOP_SELL].nField[8].nType=FT_DATA_PUT;
	SMenu[MN_SHOP_SELL].nField[8].nWillDo=75;
	SMenu[MN_SHOP_SELL].nField[8].x=75;
	SMenu[MN_SHOP_SELL].nField[8].y=266;
	SMenu[MN_SHOP_SELL].nField[8].nImageNumber=DP_CHARACTER_MONEY;


////////////////////////////////////////////////////////////////////////////////////////////	
////////////////////////////////////////////////////////////////////////////////////////////
// �Ƿ�� - ġ��, ����, ��� ����
	
	SMenu[MN_HEAL_MAIN].x = 366+GABX_SCREEN;
	SMenu[MN_HEAL_MAIN].y = 20;
	SMenu[MN_HEAL_MAIN].nImageNumber=542;
	SMenu[MN_HEAL_MAIN].nFieldCount = 5;

	SMenu[MN_HEAL_MAIN].nField[0].nType = FT_HIDE_NOMAL_GROUP_PUT;
	SMenu[MN_HEAL_MAIN].nField[0].nSHideNomalNumber=HN_HEAL_MAIN_TEXT;
	SMenu[MN_HEAL_MAIN].nField[0].nSHideNomalCount=3;
	SMenu[MN_HEAL_MAIN].nField[0].nSHideNomalStart=0;
	SMenu[MN_HEAL_MAIN].nField[0].nShideNomalPlus=3;

	// done button
	SMenu[MN_HEAL_MAIN].nField[1].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_HEAL_MAIN].nField[1].nSpecialWillDo = SWD_SMALLMENU_END;
	SMenu[MN_HEAL_MAIN].nField[1].x = 77;
	SMenu[MN_HEAL_MAIN].nField[1].y = 253;
	SMenu[MN_HEAL_MAIN].nField[1].nRectImage = 492;
	SMenu[MN_HEAL_MAIN].nField[1].nImageNumber = 493;
	SetRect(SMenu[MN_HEAL_MAIN].nField[1].rCheakBox,78,253,144,274);

	// healing open
	SMenu[MN_HEAL_MAIN].nField[2].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_HEAL_MAIN].nField[2].nSpecialWillDo = SWD_WILLDO;
	SMenu[MN_HEAL_MAIN].nField[2].nWillDo = MN_HEAL_HEALING;
	SMenu[MN_HEAL_MAIN].nField[2].x = 58;
	SMenu[MN_HEAL_MAIN].nField[2].y = 132;
	SMenu[MN_HEAL_MAIN].nField[2].nRectImage = 758;
	SMenu[MN_HEAL_MAIN].nField[2].nImageNumber = 758;
	SetRect(SMenu[MN_HEAL_MAIN].nField[2].rCheakBox,58,132,166,162);

	// preventing open
	SMenu[MN_HEAL_MAIN].nField[3].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_HEAL_MAIN].nField[3].nSpecialWillDo = SWD_WILLDO;
	SMenu[MN_HEAL_MAIN].nField[3].nWillDo = MN_HEAL_PREVENTING;
	SMenu[MN_HEAL_MAIN].nField[3].x = 58;
	SMenu[MN_HEAL_MAIN].nField[3].y = 163;
	SMenu[MN_HEAL_MAIN].nField[3].nRectImage = 758;
	SMenu[MN_HEAL_MAIN].nField[3].nImageNumber = 758;
	SetRect(SMenu[MN_HEAL_MAIN].nField[3].rCheakBox,58,163,166,193);

	// learn open
	SMenu[MN_HEAL_MAIN].nField[4].nType = FT_NO_CHECK;
//	SMenu[MN_HEAL_MAIN].nField[4].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_HEAL_MAIN].nField[4].nSpecialWillDo = SWD_WILLDO;
	SMenu[MN_HEAL_MAIN].nField[4].nWillDo = MN_HEAL_LEARN;
	SMenu[MN_HEAL_MAIN].nField[4].x = 58;
	SMenu[MN_HEAL_MAIN].nField[4].y = 194;
	SMenu[MN_HEAL_MAIN].nField[4].nRectImage = 758;
	SMenu[MN_HEAL_MAIN].nField[4].nImageNumber = 758;
	SetRect(SMenu[MN_HEAL_MAIN].nField[4].rCheakBox,58,194,166,224);


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// HEALING

	SMenu[MN_HEAL_HEALING].x=345+GABX_SCREEN;
	SMenu[MN_HEAL_HEALING].y=20;
#ifdef CHANGE_MONEY_
	SMenu[MN_HEAL_HEALING].nImageNumber=3;
	SMenu[MN_HEAL_HEALING].nImageType=CHANGE_MONEY;
#else 
	SMenu[MN_HEAL_HEALING].nImageNumber=545;
#endif
	SMenu[MN_HEAL_HEALING].nFieldCount=12;
	SMenu[MN_HEAL_HEALING].CheakType=0;

	// ���� ����	
	SMenu[MN_HEAL_HEALING].nField[1].nType=FT_NO_CHECK;
	SMenu[MN_HEAL_HEALING].nField[1].x=58;
	SMenu[MN_HEAL_HEALING].nField[1].y=26;
	SMenu[MN_HEAL_HEALING].nField[1].nImageNumber=546;

	// ok, cancel  Button 
	SMenu[MN_HEAL_HEALING].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_HEAL_HEALING].nField[2].nSpecialWillDo=SWD_HEALING_OK;
	SMenu[MN_HEAL_HEALING].nField[2].nWillDo=4;
	SMenu[MN_HEAL_HEALING].nField[2].x=28;
	SMenu[MN_HEAL_HEALING].nField[2].y=257;
	SMenu[MN_HEAL_HEALING].nField[2].nRectImage=294;
	SMenu[MN_HEAL_HEALING].nField[2].nImageNumber=295;
	SetRect(SMenu[MN_HEAL_HEALING].nField[2].rCheakBox,20,250, 65,295);

	SMenu[MN_HEAL_HEALING].nField[3].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_HEAL_HEALING].nField[3].nSpecialWillDo=SWD_SUBMENU_BACK;
	SMenu[MN_HEAL_HEALING].nField[3].x=172;
	SMenu[MN_HEAL_HEALING].nField[3].y=257;
	SMenu[MN_HEAL_HEALING].nField[3].nRectImage=292;
	SMenu[MN_HEAL_HEALING].nField[3].nImageNumber=293;
	SetRect(SMenu[MN_HEAL_HEALING].nField[3].rCheakBox,179,250,223,295);

	// ���� �̸� ���	
	SMenu[MN_HEAL_HEALING].nField[4].nType = FT_DATA_PUT;
//	SMenu[MN_HEAL_HEALING].nField[4].nWillDo						// �ɸ� ���� ��ȣ...
	SMenu[MN_HEAL_HEALING].nField[4].x=33;							// �ɸ� ���� ���� ��� �޽��� ����� ����
	SMenu[MN_HEAL_HEALING].nField[4].y=81;
	SMenu[MN_HEAL_HEALING].nField[4].nRectImage = 159;				// ���� �޽����� ���� �� ����...
	SMenu[MN_HEAL_HEALING].nField[4].nSHideNomalCount=6;
	SMenu[MN_HEAL_HEALING].nField[4].nSHideNomalStart=0;
	SMenu[MN_HEAL_HEALING].nField[4].nShideNomalPlus=3;
	SMenu[MN_HEAL_HEALING].nField[4].nImageNumber = DP_DISEASE_PUT;

	
	// ���� ��ư ���( ġ�� )
	SMenu[MN_HEAL_HEALING].nField[5].nType = FT_HEAL_TEXT_BUTTON;
	SMenu[MN_HEAL_HEALING].nField[5].x=27;
	SMenu[MN_HEAL_HEALING].nField[5].y=62;
	SMenu[MN_HEAL_HEALING].nField[5].nWillDo=4;
	SMenu[MN_HEAL_HEALING].nField[5].nSpecialWillDo=1;		// ù��° �ڸ�
	SMenu[MN_HEAL_HEALING].nField[5].nRectImage=16;
	SMenu[MN_HEAL_HEALING].nField[5].nImageNumber=16;
	SetRect(SMenu[MN_HEAL_HEALING].nField[5].rCheakBox,28,62,197,90);

	SMenu[MN_HEAL_HEALING].nField[6].nType = FT_HEAL_TEXT_BUTTON;
	SMenu[MN_HEAL_HEALING].nField[6].x=27;
	SMenu[MN_HEAL_HEALING].nField[6].y=97;
	SMenu[MN_HEAL_HEALING].nField[6].nWillDo=4;
	SMenu[MN_HEAL_HEALING].nField[6].nSpecialWillDo=2;		// �ι�° �ڸ�
	SMenu[MN_HEAL_HEALING].nField[6].nRectImage=16;
	SMenu[MN_HEAL_HEALING].nField[6].nImageNumber=16;
	SetRect(SMenu[MN_HEAL_HEALING].nField[6].rCheakBox,28,97,197,129);

	SMenu[MN_HEAL_HEALING].nField[7].nType = FT_HEAL_TEXT_BUTTON;
	SMenu[MN_HEAL_HEALING].nField[7].x=27;
	SMenu[MN_HEAL_HEALING].nField[7].y=132;
	SMenu[MN_HEAL_HEALING].nField[7].nWillDo=4;
	SMenu[MN_HEAL_HEALING].nField[7].nSpecialWillDo=3;		// ����° �ڸ�
	SMenu[MN_HEAL_HEALING].nField[7].nRectImage=16;
	SMenu[MN_HEAL_HEALING].nField[7].nImageNumber=16;
	SetRect(SMenu[MN_HEAL_HEALING].nField[7].rCheakBox,28,132,197,160);
		

	// ġ��� ���		// MessageOn ���� ���
	SMenu[MN_HEAL_HEALING].nField[8].nType=FT_DATA_PUT;
	SMenu[MN_HEAL_HEALING].nField[8].nWillDo=4;
	SMenu[MN_HEAL_HEALING].nField[8].x=96;
	SMenu[MN_HEAL_HEALING].nField[8].y=192;
	SMenu[MN_HEAL_HEALING].nField[8].nRectImage=54;
	SMenu[MN_HEAL_HEALING].nField[8].nImageNumber=DP_HEAL_MONEY;

	// �� ���
	SMenu[MN_HEAL_HEALING].nField[9].nType=FT_DATA_PUT;
	SMenu[MN_HEAL_HEALING].nField[9].nWillDo=75;
	SMenu[MN_HEAL_HEALING].nField[9].x=80;
	SMenu[MN_HEAL_HEALING].nField[9].y=220;
	SMenu[MN_HEAL_HEALING].nField[9].nImageNumber=DP_CHARACTER_MONEY;

	// scroll butoon	// up
	SMenu[MN_HEAL_HEALING].nField[10].nType=FT_HIDE_DSCROLL_AUTO_PUT;
	SMenu[MN_HEAL_HEALING].nField[10].nWillDo=4;
	SMenu[MN_HEAL_HEALING].nField[10].x=203;
	SMenu[MN_HEAL_HEALING].nField[10].y=72;
	SMenu[MN_HEAL_HEALING].nField[10].nRectImage=616;
	SMenu[MN_HEAL_HEALING].nField[10].nImageNumber=617;
	SetRect(SMenu[MN_HEAL_HEALING].nField[10].rCheakBox,203,72,220,99);
						// down
	SMenu[MN_HEAL_HEALING].nField[11].nType=FT_HIDE_ISCROLL_AUTO_PUT;
	SMenu[MN_HEAL_HEALING].nField[11].nWillDo=4;
	SMenu[MN_HEAL_HEALING].nField[11].x=203;
	SMenu[MN_HEAL_HEALING].nField[11].y=119;
	SMenu[MN_HEAL_HEALING].nField[11].nRectImage=618;
	SMenu[MN_HEAL_HEALING].nField[11].nImageNumber=619;
	SetRect(SMenu[MN_HEAL_HEALING].nField[11].rCheakBox,203,119,220,147);
	


///////////////////////////////////////////////////////////////////////////////////////////////
// PREVENTING

	SMenu[MN_HEAL_PREVENTING].x=345+GABX_SCREEN;
	SMenu[MN_HEAL_PREVENTING].y=20;
#ifdef CHANGE_MONEY_
	SMenu[MN_HEAL_PREVENTING].nImageNumber=3;
	SMenu[MN_HEAL_PREVENTING].nImageType=CHANGE_MONEY;
#else 
	SMenu[MN_HEAL_PREVENTING].nImageNumber=545;
#endif
	SMenu[MN_HEAL_PREVENTING].nFieldCount=12;
	SMenu[MN_HEAL_PREVENTING].CheakType=0;

	// ���� ����	
	SMenu[MN_HEAL_PREVENTING].nField[1].nType=FT_NOMAL_PUT;
	SMenu[MN_HEAL_PREVENTING].nField[1].x=58;
	SMenu[MN_HEAL_PREVENTING].nField[1].y=26;
	SMenu[MN_HEAL_PREVENTING].nField[1].nImageNumber=546;

	// ok, cancel  Button 
	SMenu[MN_HEAL_PREVENTING].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_HEAL_PREVENTING].nField[2].nSpecialWillDo=SWD_PREVENTING_OK;
	SMenu[MN_HEAL_PREVENTING].nField[2].nWillDo=4;
	SMenu[MN_HEAL_PREVENTING].nField[2].x=28;
	SMenu[MN_HEAL_PREVENTING].nField[2].y=257;
	SMenu[MN_HEAL_PREVENTING].nField[2].nRectImage=294;
	SMenu[MN_HEAL_PREVENTING].nField[2].nImageNumber=295;
	SetRect(SMenu[MN_HEAL_PREVENTING].nField[2].rCheakBox,20,250, 65,295);

	SMenu[MN_HEAL_PREVENTING].nField[3].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_HEAL_PREVENTING].nField[3].nSpecialWillDo=SWD_SUBMENU_BACK;
	SMenu[MN_HEAL_PREVENTING].nField[3].x=172;
	SMenu[MN_HEAL_PREVENTING].nField[3].y=257;
 	SMenu[MN_HEAL_PREVENTING].nField[3].nRectImage=292;
	SMenu[MN_HEAL_PREVENTING].nField[3].nImageNumber=293;
	SetRect(SMenu[MN_HEAL_PREVENTING].nField[3].rCheakBox,179,250,223,295);

	SMenu[MN_HEAL_PREVENTING].nField[4].nType = FT_HIDE_NOMAL_GROUP_PUT;
	SMenu[MN_HEAL_PREVENTING].nField[4].nSHideNomalNumber=HN_HEAL_PREVENTING;
	SMenu[MN_HEAL_PREVENTING].nField[4].nSHideNomalCount=6;
	SMenu[MN_HEAL_PREVENTING].nField[4].nSHideNomalStart=0;
	SMenu[MN_HEAL_PREVENTING].nField[4].nShideNomalPlus=3;

	// �� ���� �̸� ��� ( ���� )
	SMenu[MN_HEAL_PREVENTING].nField[5].nType = FT_TEXT_BUTTON;
	SMenu[MN_HEAL_PREVENTING].nField[5].x=27;
	SMenu[MN_HEAL_PREVENTING].nField[5].y=62;
	SMenu[MN_HEAL_PREVENTING].nField[5].nWillDo=4;
	SMenu[MN_HEAL_PREVENTING].nField[5].nSpecialWillDo=1;		// ù��° �ڸ�
	SMenu[MN_HEAL_PREVENTING].nField[5].nRectImage=16;
	SMenu[MN_HEAL_PREVENTING].nField[5].nImageNumber=16;
	SetRect(SMenu[MN_HEAL_PREVENTING].nField[5].rCheakBox,28,62,197,90);

	SMenu[MN_HEAL_PREVENTING].nField[6].nType = FT_TEXT_BUTTON;
	SMenu[MN_HEAL_PREVENTING].nField[6].x=27;
	SMenu[MN_HEAL_PREVENTING].nField[6].y=97;
	SMenu[MN_HEAL_PREVENTING].nField[6].nWillDo=4;
	SMenu[MN_HEAL_PREVENTING].nField[6].nSpecialWillDo=2;		// �ι�° �ڸ�
	SMenu[MN_HEAL_PREVENTING].nField[6].nRectImage=16;
	SMenu[MN_HEAL_PREVENTING].nField[6].nImageNumber=16;
	SetRect(SMenu[MN_HEAL_PREVENTING].nField[6].rCheakBox,28,97,197,129);

	SMenu[MN_HEAL_PREVENTING].nField[7].nType = FT_TEXT_BUTTON;
	SMenu[MN_HEAL_PREVENTING].nField[7].x=27;
	SMenu[MN_HEAL_PREVENTING].nField[7].y=132;
	SMenu[MN_HEAL_PREVENTING].nField[7].nWillDo=4;
	SMenu[MN_HEAL_PREVENTING].nField[7].nSpecialWillDo=3;		// ����° �ڸ�
	SMenu[MN_HEAL_PREVENTING].nField[7].nRectImage=16;
	SMenu[MN_HEAL_PREVENTING].nField[7].nImageNumber=16;
	SetRect(SMenu[MN_HEAL_PREVENTING].nField[7].rCheakBox,28,132,197,160);

	// ġ��� ���		// ���� ���̺� ������ �߰�
	SMenu[MN_HEAL_PREVENTING].nField[8].nType=FT_DATA_PUT;
	SMenu[MN_HEAL_PREVENTING].nField[8].nWillDo=4;
	SMenu[MN_HEAL_PREVENTING].nField[8].x=96;
	SMenu[MN_HEAL_PREVENTING].nField[8].y=192;
	SMenu[MN_HEAL_PREVENTING].nField[8].nRectImage=54;
	SMenu[MN_HEAL_PREVENTING].nField[8].nImageNumber=DP_PREVENT_MONEY;

	// �� ���
	SMenu[MN_HEAL_PREVENTING].nField[9].nType=FT_DATA_PUT;
	SMenu[MN_HEAL_PREVENTING].nField[9].nWillDo=75;
	SMenu[MN_HEAL_PREVENTING].nField[9].x=79;
	SMenu[MN_HEAL_PREVENTING].nField[9].y=220;
	SMenu[MN_HEAL_PREVENTING].nField[9].nImageNumber=DP_CHARACTER_MONEY;

	// scroll butoon	// up
	SMenu[MN_HEAL_PREVENTING].nField[10].nType=FT_HIDE_DSCROLL_AUTO_PUT;
	SMenu[MN_HEAL_PREVENTING].nField[10].nWillDo=4;
	SMenu[MN_HEAL_PREVENTING].nField[10].x=203;
	SMenu[MN_HEAL_PREVENTING].nField[10].y=72;
	SMenu[MN_HEAL_PREVENTING].nField[10].nRectImage=616;
	SMenu[MN_HEAL_PREVENTING].nField[10].nImageNumber=617;
	SetRect(SMenu[MN_HEAL_PREVENTING].nField[10].rCheakBox,203,72,220,99);
						// down
	SMenu[MN_HEAL_PREVENTING].nField[11].nType=FT_HIDE_ISCROLL_AUTO_PUT;
	SMenu[MN_HEAL_PREVENTING].nField[11].nWillDo=4;
	SMenu[MN_HEAL_PREVENTING].nField[11].x=203;
	SMenu[MN_HEAL_PREVENTING].nField[11].y=119;
	SMenu[MN_HEAL_PREVENTING].nField[11].nRectImage=618;
	SMenu[MN_HEAL_PREVENTING].nField[11].nImageNumber=619;
	SetRect(SMenu[MN_HEAL_PREVENTING].nField[11].rCheakBox,203,119,220,147);


///////////////////////////////////////////////////////////////////////////////////////////////
// LEARN

	SMenu[MN_HEAL_LEARN].x=345+GABX_SCREEN;
	SMenu[MN_HEAL_LEARN].y=20;
	SMenu[MN_HEAL_LEARN].nImageNumber=545;
	SMenu[MN_HEAL_LEARN].nFieldCount=9;
	SMenu[MN_HEAL_LEARN].CheakType=0;

	// ���� ����	
	SMenu[MN_HEAL_LEARN].nField[1].nType=FT_NOMAL_PUT;
	SMenu[MN_HEAL_LEARN].nField[1].x=58;
	SMenu[MN_HEAL_LEARN].nField[1].y=26;
	SMenu[MN_HEAL_LEARN].nField[1].nImageNumber=547;

	// ok, cancel  Button 
	SMenu[MN_HEAL_LEARN].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_HEAL_LEARN].nField[2].nSpecialWillDo=SWD_LEARN;
	SMenu[MN_HEAL_LEARN].nField[2].x=28;
	SMenu[MN_HEAL_LEARN].nField[2].y=257;
	SMenu[MN_HEAL_LEARN].nField[2].nRectImage=294;
	SMenu[MN_HEAL_LEARN].nField[2].nImageNumber=295;
	SetRect(SMenu[MN_HEAL_LEARN].nField[2].rCheakBox,20,250, 65,295);

	SMenu[MN_HEAL_LEARN].nField[3].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_HEAL_LEARN].nField[3].nSpecialWillDo=SWD_SUBMENU_BACK;
	SMenu[MN_HEAL_LEARN].nField[3].x=172;
	SMenu[MN_HEAL_LEARN].nField[3].y=257;
	SMenu[MN_HEAL_LEARN].nField[3].nRectImage=292;
	SMenu[MN_HEAL_LEARN].nField[3].nImageNumber=293;
	SetRect(SMenu[MN_HEAL_LEARN].nField[3].rCheakBox,179,250,223,295);

	// ��ũ�� ��ư ����
	SMenu[MN_HEAL_LEARN].nField[4].nType=FT_NOMAL_PUT;
	SMenu[MN_HEAL_LEARN].nField[4].x=197;
	SMenu[MN_HEAL_LEARN].nField[4].y=53;
	SMenu[MN_HEAL_LEARN].nField[4].nImageNumber=759 ;


	// ��ų ���� ���		//
	SMenu[MN_HEAL_LEARN].nField[7].nType=FT_DATA_PUT;
	SMenu[MN_HEAL_LEARN].nField[7].nImageNumber=DP_HEALING_EXPLAIN;

	// �� ���
	SMenu[MN_HEAL_LEARN].nField[8].nType=FT_DATA_PUT;
	SMenu[MN_HEAL_LEARN].nField[8].nWillDo=75;
	SMenu[MN_HEAL_LEARN].nField[8].x=79;
	SMenu[MN_HEAL_LEARN].nField[8].y=220;
	SMenu[MN_HEAL_LEARN].nField[8].nImageNumber=DP_CHARACTER_MONEY;


/////////////////////////////////////////////////////////////////////////////////////////////////////	
/////////////////////////////////////////////////////////////////////////////////////////////////////	
// ���� - �޽�, HP, MP ����
	
	SMenu[MN_INN_MAIN].x = 378+GABX_SCREEN;
	SMenu[MN_INN_MAIN].y = 26;
	SMenu[MN_INN_MAIN].nImageNumber = 818;
	SMenu[MN_INN_MAIN].nFieldCount = 6;

	SMenu[MN_INN_MAIN].nField[0].nType = FT_HIDE_NOMAL_GROUP_PUT;
	SMenu[MN_INN_MAIN].nField[0].nSHideNomalNumber=HN_INN_MAIN_TEXT;
	SMenu[MN_INN_MAIN].nField[0].nSHideNomalCount=3;
	SMenu[MN_INN_MAIN].nField[0].nSHideNomalStart=0;
	SMenu[MN_INN_MAIN].nField[0].nShideNomalPlus=3;

	// done button
	SMenu[MN_INN_MAIN].nField[1].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_INN_MAIN].nField[1].nSpecialWillDo = SWD_SMALLMENU_END;
	SMenu[MN_INN_MAIN].nField[1].x = 63;
	SMenu[MN_INN_MAIN].nField[1].y = 229;
	SMenu[MN_INN_MAIN].nField[1].nRectImage = 530;
	SMenu[MN_INN_MAIN].nField[1].nImageNumber = 531;
	SetRect(SMenu[MN_INN_MAIN].nField[1].rCheakBox,64,230,138,256);

	// Rest open
	SMenu[MN_INN_MAIN].nField[2].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_INN_MAIN].nField[2].nSpecialWillDo = SWD_WILLDO;
//	SMenu[MN_INN_MAIN].nField[2].nSpecialWillDo = SWD_SORRY;
	SMenu[MN_INN_MAIN].nField[2].nWillDo = MN_INN_REST;
	SMenu[MN_INN_MAIN].nField[2].x = 51;
	SMenu[MN_INN_MAIN].nField[2].y = 110;
	SMenu[MN_INN_MAIN].nField[2].nRectImage = 758;
	SMenu[MN_INN_MAIN].nField[2].nImageNumber = 758;
	SetRect(SMenu[MN_INN_MAIN].nField[2].rCheakBox,46,110,155,136);

	// Sleep open
	SMenu[MN_INN_MAIN].nField[3].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_INN_MAIN].nField[3].nSpecialWillDo = SWD_WILLDO;
//	SMenu[MN_INN_MAIN].nField[3].nSpecialWillDo = SWD_SORRY;
	SMenu[MN_INN_MAIN].nField[3].nWillDo = MN_INN_SLEEP;
	SMenu[MN_INN_MAIN].nField[3].x = 51;
	SMenu[MN_INN_MAIN].nField[3].y = 139;
	SMenu[MN_INN_MAIN].nField[3].nRectImage = 758;
	SMenu[MN_INN_MAIN].nField[3].nImageNumber = 758;
	SetRect(SMenu[MN_INN_MAIN].nField[3].rCheakBox,46,139,155,165);

	// LogOut open
	SMenu[MN_INN_MAIN].nField[4].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_INN_MAIN].nField[4].nSpecialWillDo = SWD_WILLDO;
	SMenu[MN_INN_MAIN].nField[4].nWillDo = MN_SYSTEM_LOGOUT;
	SMenu[MN_INN_MAIN].nField[4].x = 51;
	SMenu[MN_INN_MAIN].nField[4].y = 168;
	SMenu[MN_INN_MAIN].nField[4].nRectImage = 758;
	SMenu[MN_INN_MAIN].nField[4].nImageNumber = 758;
	SetRect(SMenu[MN_INN_MAIN].nField[4].rCheakBox,46,168,165,194);

	// ���� �̸� �� ���̱�
	SMenu[MN_INN_MAIN].nField[5].nType = FT_NOMAL_PUT;
	SMenu[MN_INN_MAIN].nField[5].x = 21;
	SMenu[MN_INN_MAIN].nField[5].y = 27;
	SMenu[MN_INN_MAIN].nField[5].nImageNumber = 3;
	SMenu[MN_INN_MAIN].nField[5].nImageType = SMALL_ETC;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MN_INN_REST �޴�

	SMenu[MN_INN_REST].x = 378+GABX_SCREEN;
	SMenu[MN_INN_REST].y = 26;
#ifdef CHANGE_MONEY_
	SMenu[MN_INN_REST].nImageNumber = 7;
	SMenu[MN_INN_REST].nImageType = CHANGE_MONEY;
#else 
	SMenu[MN_INN_REST].nImageNumber=821;
#endif
	SMenu[MN_INN_REST].nFieldCount = 7;

	SMenu[MN_INN_REST].nField[0].nType = FT_HIDE_NOMAL_GROUP_PUT;
	SMenu[MN_INN_REST].nField[0].nSHideNomalNumber=HN_INN_REST_TEXT;
	SMenu[MN_INN_REST].nField[0].nSHideNomalCount=1;
	SMenu[MN_INN_REST].nField[0].nSHideNomalStart=0;
	SMenu[MN_INN_REST].nField[0].nShideNomalPlus=1;

	// ok, cancel  Button 
	SMenu[MN_INN_REST].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_INN_REST].nField[1].nSpecialWillDo=SWD_INN_OK;
	SMenu[MN_INN_REST].nField[1].x=23;
	SMenu[MN_INN_REST].nField[1].y=249;
	SMenu[MN_INN_REST].nField[1].nRectImage=819;
	SMenu[MN_INN_REST].nField[1].nImageNumber=820;
	SetRect(SMenu[MN_INN_REST].nField[1].rCheakBox,23,249,89,270);
	
	SMenu[MN_INN_REST].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_INN_REST].nField[2].nSpecialWillDo=SWD_SUBMENU_BACK;
//	SMenu[MN_INN_REST].nField[2].nWillDo=MN_INN_MAIN;
	SMenu[MN_INN_REST].nField[2].x=121;
	SMenu[MN_INN_REST].nField[2].y=249;
	SMenu[MN_INN_REST].nField[2].nRectImage=935;
	SMenu[MN_INN_REST].nField[2].nImageNumber=936;
	SetRect(SMenu[MN_INN_REST].nField[2].rCheakBox,121,249,187,270);

	// single
//	SMenu[MN_INN_REST].nField[3].nType = FT_INN;
//	SMenu[MN_INN_REST].nField[3].nWillDo=INN_SINGLE;
//	SMenu[MN_INN_REST].nField[3].x = 51;
//	SMenu[MN_INN_REST].nField[3].y = 91;
//	SMenu[MN_INN_REST].nField[3].nRectImage = 758;
//	SMenu[MN_INN_REST].nField[3].nImageNumber = 758;
//	SetRect(SMenu[MN_INN_REST].nField[3].rCheakBox,51,91,155,117);

	// double
	SMenu[MN_INN_REST].nField[4].nType = FT_INN;
	SMenu[MN_INN_REST].nField[4].nWillDo=INN_SINGLE;//INN_DOUBLE;
	SMenu[MN_INN_REST].nField[4].x = 51;
	SMenu[MN_INN_REST].nField[4].y = 125;
	SMenu[MN_INN_REST].nField[4].nRectImage = 758;
	SMenu[MN_INN_REST].nField[4].nImageNumber = 758;
	SetRect(SMenu[MN_INN_REST].nField[4].rCheakBox,51,125,155,151);

	// multi
//	SMenu[MN_INN_REST].nField[5].nType = FT_INN;
//	SMenu[MN_INN_REST].nField[5].nWillDo=INN_MULTI;
//	SMenu[MN_INN_REST].nField[5].x = 51;
//	SMenu[MN_INN_REST].nField[5].y = 159;
//	SMenu[MN_INN_REST].nField[5].nRectImage = 758;
//	SMenu[MN_INN_REST].nField[5].nImageNumber = 758;
//	SetRect(SMenu[MN_INN_REST].nField[5].rCheakBox,51,159,155,185);

	// ������ �簢�� ��� �����ֱ�
	SMenu[MN_INN_REST].nField[6].nType = FT_DATA_PUT;
	SMenu[MN_INN_REST].nField[6].nImageNumber = DP_INN;
	


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MN_INN_SLEEP �޴�

	SMenu[MN_INN_SLEEP].x = 378+GABX_SCREEN;
	SMenu[MN_INN_SLEEP].y = 26;
#ifdef CHANGE_MONEY_
	SMenu[MN_INN_SLEEP].nImageNumber = 7;
	SMenu[MN_INN_SLEEP].nImageType = CHANGE_MONEY;
#else 
	SMenu[MN_INN_SLEEP].nImageNumber=821;
#endif
	SMenu[MN_INN_SLEEP].nFieldCount = 8;

	SMenu[MN_INN_SLEEP].nField[0].nType = FT_HIDE_NOMAL_GROUP_PUT;
	SMenu[MN_INN_SLEEP].nField[0].nSHideNomalNumber=HN_INN_REST_TEXT;
	SMenu[MN_INN_SLEEP].nField[0].nSHideNomalCount=1;
	SMenu[MN_INN_SLEEP].nField[0].nSHideNomalStart=0;
	SMenu[MN_INN_SLEEP].nField[0].nShideNomalPlus=1;

	// ok, cancel  Button 
	SMenu[MN_INN_SLEEP].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_INN_SLEEP].nField[1].nSpecialWillDo=SWD_INN_OK;
	SMenu[MN_INN_SLEEP].nField[1].x=23;
	SMenu[MN_INN_SLEEP].nField[1].y=249;
	SMenu[MN_INN_SLEEP].nField[1].nRectImage=819;
	SMenu[MN_INN_SLEEP].nField[1].nImageNumber=820;
	SetRect(SMenu[MN_INN_SLEEP].nField[1].rCheakBox,23,249,89,270);

	SMenu[MN_INN_SLEEP].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_INN_SLEEP].nField[2].nSpecialWillDo=SWD_SUBMENU_BACK;
//	SMenu[MN_INN_SLEEP].nField[2].nWillDo=MN_INN_MAIN;
	SMenu[MN_INN_SLEEP].nField[2].x=121;
	SMenu[MN_INN_SLEEP].nField[2].y=249;
	SMenu[MN_INN_SLEEP].nField[2].nRectImage=935;
	SMenu[MN_INN_SLEEP].nField[2].nImageNumber=936;
	SetRect(SMenu[MN_INN_SLEEP].nField[2].rCheakBox,121,249,187,270);

	// single
//	SMenu[MN_INN_SLEEP].nField[3].nType = FT_INN;
//	SMenu[MN_INN_SLEEP].nField[3].nWillDo=INN_SINGLE;
//	SMenu[MN_INN_SLEEP].nField[3].x = 51;
//	SMenu[MN_INN_SLEEP].nField[3].y = 91;
//	SMenu[MN_INN_SLEEP].nField[3].nRectImage = 758;
//	SMenu[MN_INN_SLEEP].nField[3].nImageNumber = 758;
//	SetRect(SMenu[MN_INN_SLEEP].nField[3].rCheakBox,51,91,155,117);

	// double
	SMenu[MN_INN_SLEEP].nField[4].nType = FT_INN;
	SMenu[MN_INN_SLEEP].nField[4].nWillDo=INN_SINGLE;//INN_DOUBLE;
	SMenu[MN_INN_SLEEP].nField[4].x = 51;
	SMenu[MN_INN_SLEEP].nField[4].y = 125;
	SMenu[MN_INN_SLEEP].nField[4].nRectImage = 758;
	SMenu[MN_INN_SLEEP].nField[4].nImageNumber = 758;
	SetRect(SMenu[MN_INN_SLEEP].nField[4].rCheakBox,51,125,155,151);

	// multi
//	SMenu[MN_INN_SLEEP].nField[5].nType = FT_INN;
//	SMenu[MN_INN_SLEEP].nField[5].nWillDo=INN_MULTI;
//	SMenu[MN_INN_SLEEP].nField[5].x = 51;
//	SMenu[MN_INN_SLEEP].nField[5].y = 159;
//	SMenu[MN_INN_SLEEP].nField[5].nRectImage = 758;
//	SMenu[MN_INN_SLEEP].nField[5].nImageNumber = 758;
//	SetRect(SMenu[MN_INN_SLEEP].nField[5].rCheakBox,51,159,155,185);

	// ���� �� ���
	SMenu[MN_INN_SLEEP].nField[6].nType=FT_NOMAL_PUT;
	SMenu[MN_INN_SLEEP].nField[6].x=37;
	SMenu[MN_INN_SLEEP].nField[6].y=22;
	SMenu[MN_INN_SLEEP].nField[6].nImageNumber=822;

	// ������ �簢�� ��� �����ֱ�
	SMenu[MN_INN_SLEEP].nField[7].nType = FT_DATA_PUT;
	SMenu[MN_INN_SLEEP].nField[7].nImageNumber = DP_INN;


///////////////////////////////////////////////////////////////////////////////////////////////////
// ������ ���� â

	SMenu[MN_ITEM_MESSAGEBOX].x=193;
	SMenu[MN_ITEM_MESSAGEBOX].y=76;
	SMenu[MN_ITEM_MESSAGEBOX].nImageNumber=706;
	SMenu[MN_ITEM_MESSAGEBOX].nFieldCount=2;

	// OK
	SMenu[MN_ITEM_MESSAGEBOX].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_ITEM_MESSAGEBOX].nField[0].nSpecialWillDo=SWD_OKCANCEL_OK;
	SMenu[MN_ITEM_MESSAGEBOX].nField[0].x=210;
	SMenu[MN_ITEM_MESSAGEBOX].nField[0].y=65;
	SMenu[MN_ITEM_MESSAGEBOX].nField[0].nRectImage=646;
	SMenu[MN_ITEM_MESSAGEBOX].nField[0].nImageNumber=647;
	SetRect(SMenu[MN_ITEM_MESSAGEBOX].nField[0].rCheakBox,208,66,238,96);

	// Message
	SMenu[MN_ITEM_MESSAGEBOX].nField[1].nType=FT_DATA_PUT;
	SMenu[MN_ITEM_MESSAGEBOX].nField[1].x=35;
	SMenu[MN_ITEM_MESSAGEBOX].nField[1].y=22;
	SMenu[MN_ITEM_MESSAGEBOX].nField[1].nRectImage=160;
	SMenu[MN_ITEM_MESSAGEBOX].nField[1].nImageNumber=DP_ITEM_MESSAGE;

//////////////////////////////////////////////////////////////////////////////////////
// DIVIDE_OPEN
	SMenu[MN_DIVIDE_ITEM].x= 280;
	SMenu[MN_DIVIDE_ITEM].y= 254;
	SMenu[MN_DIVIDE_ITEM].nFieldCount = 6;
	SMenu[MN_DIVIDE_ITEM].nImageNumber = 942;

	//OK
	SMenu[MN_DIVIDE_ITEM].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_DIVIDE_ITEM].nField[0].nSpecialWillDo=SWD_DIVIDE_OK;
	SMenu[MN_DIVIDE_ITEM].nField[0].x=24;
	SMenu[MN_DIVIDE_ITEM].nField[0].y=61;
	SMenu[MN_DIVIDE_ITEM].nField[0].nRectImage=722;
	SMenu[MN_DIVIDE_ITEM].nField[0].nImageNumber=723;
	SetRect(SMenu[MN_DIVIDE_ITEM].nField[0].rCheakBox,24,61,74,85);

	//CANCEL
	SMenu[MN_DIVIDE_ITEM].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_DIVIDE_ITEM].nField[1].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_DIVIDE_ITEM].nField[1].x=126;
	SMenu[MN_DIVIDE_ITEM].nField[1].y=61;
	SMenu[MN_DIVIDE_ITEM].nField[1].nRectImage=724;
	SMenu[MN_DIVIDE_ITEM].nField[1].nImageNumber=725;
	SetRect(SMenu[MN_DIVIDE_ITEM].nField[1].rCheakBox,125,61,175,85);

	// scroll butoon	// up
	SMenu[MN_DIVIDE_ITEM].nField[2].nType=FT_DO;
	SMenu[MN_DIVIDE_ITEM].nField[2].nWillDo=DO_DIVIDE_SCROLL_UP;
	SMenu[MN_DIVIDE_ITEM].nField[2].nSpecialWillDo=4;
	SMenu[MN_DIVIDE_ITEM].nField[2].x=18;
	SMenu[MN_DIVIDE_ITEM].nField[2].y=16;
	SMenu[MN_DIVIDE_ITEM].nField[2].nRectImage=296;
	SMenu[MN_DIVIDE_ITEM].nField[2].nImageNumber=298;
	SetRect(SMenu[MN_DIVIDE_ITEM].nField[2].rCheakBox,18,16,40,30);
						// down
	SMenu[MN_DIVIDE_ITEM].nField[3].nType=FT_DO;
	SMenu[MN_DIVIDE_ITEM].nField[3].nWillDo=DO_DIVIDE_SCROLL_DOWN;
	SMenu[MN_DIVIDE_ITEM].nField[3].nSpecialWillDo=4;
	SMenu[MN_DIVIDE_ITEM].nField[3].x=18;
	SMenu[MN_DIVIDE_ITEM].nField[3].y=32;
	SMenu[MN_DIVIDE_ITEM].nField[3].nRectImage=297;
	SMenu[MN_DIVIDE_ITEM].nField[3].nImageNumber=299;
	SetRect(SMenu[MN_DIVIDE_ITEM].nField[3].rCheakBox,18,32,40,47);

	SMenu[MN_DIVIDE_ITEM].nField[4].nType=FT_DIVIDE_ITEM_PUT;
	SMenu[MN_DIVIDE_ITEM].nField[4].nWillDo=116;
	SMenu[MN_DIVIDE_ITEM].nField[4].x=62;
	SMenu[MN_DIVIDE_ITEM].nField[4].y=29;
//	SetRect(SMenu[MN_DIVIDE_ITEM].nField[4].rCheakBox,12,15,187,120);

	// �޴� �ű� 
	SMenu[MN_DIVIDE_ITEM].nField[5].nType=FT_MENU_MOVE;
	SetRect(SMenu[MN_DIVIDE_ITEM].nField[5].rCheakBox,0,0,222,13);

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
	// ĳ���Ͱ� ������ �ִ� �ڽ� ���� 

	SMenu[MN_CHARACTER_ITEM].x= 101+GABX_SCREEN;
	SMenu[MN_CHARACTER_ITEM].y= 196;
	SMenu[MN_CHARACTER_ITEM].nFieldCount = 6;
	SMenu[MN_CHARACTER_ITEM].nImageNumber = 941;

	//OK
	SMenu[MN_CHARACTER_ITEM].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_CHARACTER_ITEM].nField[0].nSpecialWillDo=SWD_RMENUCLOSE;						// �ܼ� ����
	SMenu[MN_CHARACTER_ITEM].nField[0].x=21;
	SMenu[MN_CHARACTER_ITEM].nField[0].y=125;
	SMenu[MN_CHARACTER_ITEM].nField[0].nRectImage=722;
	SMenu[MN_CHARACTER_ITEM].nField[0].nImageNumber=723;
	SetRect(SMenu[MN_CHARACTER_ITEM].nField[0].rCheakBox,21,125,71,149);

	//CANCEL
	SMenu[MN_CHARACTER_ITEM].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_CHARACTER_ITEM].nField[1].nSpecialWillDo=SWD_MOVE_OPEN_CLOSE;
	SMenu[MN_CHARACTER_ITEM].nField[1].nSHideNomalCount=0;	// 0:close, 1:opem -> mp3(?)�� ����
	SMenu[MN_CHARACTER_ITEM].nField[1].x=148;
	SMenu[MN_CHARACTER_ITEM].nField[1].y=125;
	SMenu[MN_CHARACTER_ITEM].nField[1].nRectImage=724;
	SMenu[MN_CHARACTER_ITEM].nField[1].nImageNumber=725;
	SetRect(SMenu[MN_CHARACTER_ITEM].nField[1].rCheakBox,148,125,198,149);

	// scroll butoon	// up
	SMenu[MN_CHARACTER_ITEM].nField[2].nType=FT_HIDE_DSCROLL_AUTO_PUT;
	SMenu[MN_CHARACTER_ITEM].nField[2].nWillDo=4;
	SMenu[MN_CHARACTER_ITEM].nField[2].x=197;
	SMenu[MN_CHARACTER_ITEM].nField[2].y=17;
	SMenu[MN_CHARACTER_ITEM].nField[2].nRectImage=611;
	SMenu[MN_CHARACTER_ITEM].nField[2].nImageNumber=612;
	SetRect(SMenu[MN_CHARACTER_ITEM].nField[2].rCheakBox,197,17,213,48);
						// down
	SMenu[MN_CHARACTER_ITEM].nField[3].nType=FT_HIDE_ISCROLL_AUTO_PUT;
	SMenu[MN_CHARACTER_ITEM].nField[3].nWillDo=4;
	SMenu[MN_CHARACTER_ITEM].nField[3].x=197;
	SMenu[MN_CHARACTER_ITEM].nField[3].y=88;
	SMenu[MN_CHARACTER_ITEM].nField[3].nRectImage=613;
	SMenu[MN_CHARACTER_ITEM].nField[3].nImageNumber=614;
	SetRect(SMenu[MN_CHARACTER_ITEM].nField[3].rCheakBox,197,88,213,119);

	SMenu[MN_CHARACTER_ITEM].nField[4].nType=FT_CHARACTER_ITEM;
	SMenu[MN_CHARACTER_ITEM].nField[4].x=14;
	SMenu[MN_CHARACTER_ITEM].nField[4].y=16;
	SMenu[MN_CHARACTER_ITEM].nField[4].nSHideNomalCount=1;
	SMenu[MN_CHARACTER_ITEM].nField[4].nSHideNomalStart=0;
	SMenu[MN_CHARACTER_ITEM].nField[4].nShideNomalPlus=1;
	SetRect(SMenu[MN_CHARACTER_ITEM].nField[4].rCheakBox,12,15,187,120);

	// �޴� �ű� 
	SMenu[MN_CHARACTER_ITEM].nField[5].nType=FT_MENU_MOVE;
	SetRect(SMenu[MN_CHARACTER_ITEM].nField[5].rCheakBox,0,0,222,13);

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// ����å �������̽�			// ( temp --> �迭 , �迭�� ������.. )

	// --- UI 佈局設定 ---
	// 定義全域位移量，只套用在最上層的容器
	const int X_OFFSET = 240;
	const int Y_OFFSET = 60;

	// 設定主選單的基本屬性，並在此處加上位移
	SMenu[MN_LEARN_MAGIC_BOOK].x = 63 + GABX_SCREEN + X_OFFSET;
	SMenu[MN_LEARN_MAGIC_BOOK].y = 8 + Y_OFFSET;
	SMenu[MN_LEARN_MAGIC_BOOK].nFieldCount = 20;
	SMenu[MN_LEARN_MAGIC_BOOK].nImageNumber = 965;

	// --- 欄位詳細設定 ---

	// Field[0]: 按鈕功能定義
	SMenu[MN_LEARN_MAGIC_BOOK].nField[0].nType = FT_MAGIC_BOOK_ETC;

	// Fields[1] ~ [12]: 使用迴圈動態產生 12 個魔法系別按鈕
	// !!注意：此處的 x, y 使用的是相對於 SMenu 的原始座標，不再另外加上位移
	const int CLASS_BUTTON_BASE_X = 510;
	const int CLASS_BUTTON_START_Y = 9;
	const int CLASS_BUTTON_Y_GAP = 24;
	const int CLASS_BUTTON_WIDTH = 38;
	const int CLASS_BUTTON_HEIGHT = 22;

	for (int i = 1; i <= 12; ++i)
	{
		auto& field = SMenu[MN_LEARN_MAGIC_BOOK].nField[i];

		field.nType = FT_MAGIC_CLASS;
		field.nRectImage = 13;
		field.nImageNumber = i;
		field.nImageType = MAGIC_BOOK_MENU;

		// 使用原始的相對座標
		field.x = CLASS_BUTTON_BASE_X;
		field.y = CLASS_BUTTON_START_Y + (i - 1) * CLASS_BUTTON_Y_GAP;

		// SetRect 也使用原始的相對座標來定義點擊區域
		SetRect(field.rCheakBox,
			field.x,
			field.y,
			field.x + CLASS_BUTTON_WIDTH,
			field.y + CLASS_BUTTON_HEIGHT);
	}

	// 後續所有 nfield 的 x, y 和 SetRect 都使用原始座標，不再加 OFFSET

	// Field[13]: 向下捲動書頁按鈕
	SMenu[MN_LEARN_MAGIC_BOOK].nField[13].nType = FT_DSCROLL_BOOK;
	SMenu[MN_LEARN_MAGIC_BOOK].nField[13].x = 18;
	SMenu[MN_LEARN_MAGIC_BOOK].nField[13].y = 297;
	SMenu[MN_LEARN_MAGIC_BOOK].nField[13].nRectImage = 16;
	SMenu[MN_LEARN_MAGIC_BOOK].nField[13].nImageNumber = 17;
	SMenu[MN_LEARN_MAGIC_BOOK].nField[13].nImageType = MAGIC_BOOK_MENU;
	SetRect(SMenu[MN_LEARN_MAGIC_BOOK].nField[13].rCheakBox, 18, 297, 51, 319);

	// Field[14]: 向上捲動書頁按鈕
	SMenu[MN_LEARN_MAGIC_BOOK].nField[14].nType = FT_ISCROLL_BOOK;
	SMenu[MN_LEARN_MAGIC_BOOK].nField[14].x = 466;
	SMenu[MN_LEARN_MAGIC_BOOK].nField[14].y = 297;
	SMenu[MN_LEARN_MAGIC_BOOK].nField[14].nRectImage = 18;
	SMenu[MN_LEARN_MAGIC_BOOK].nField[14].nImageNumber = 19;
	SMenu[MN_LEARN_MAGIC_BOOK].nField[14].nImageType = MAGIC_BOOK_MENU;
	SetRect(SMenu[MN_LEARN_MAGIC_BOOK].nField[14].rCheakBox, 466, 297, 499, 319);

	// Field[15]: 左側魔法資料顯示區域
	SMenu[MN_LEARN_MAGIC_BOOK].nField[15].nType = FT_DATA_PUT;
	SMenu[MN_LEARN_MAGIC_BOOK].nField[15].nWillDo = 0;
	SMenu[MN_LEARN_MAGIC_BOOK].nField[15].x = 109;
	SMenu[MN_LEARN_MAGIC_BOOK].nField[15].y = 35;
	SMenu[MN_LEARN_MAGIC_BOOK].nField[15].nImageNumber = DP_MAGIC_PUT;

	// Field[16]: 右側魔法資料顯示區域
	SMenu[MN_LEARN_MAGIC_BOOK].nField[16].nType = FT_DATA_PUT;
	SMenu[MN_LEARN_MAGIC_BOOK].nField[16].nWillDo = 1;
	SMenu[MN_LEARN_MAGIC_BOOK].nField[16].x = 371;
	SMenu[MN_LEARN_MAGIC_BOOK].nField[16].y = 36;
	SMenu[MN_LEARN_MAGIC_BOOK].nField[16].nImageNumber = DP_MAGIC_PUT;

	// Field[17]: 離開 (Exit) 按鈕
	SMenu[MN_LEARN_MAGIC_BOOK].nField[17].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_LEARN_MAGIC_BOOK].nField[17].nSpecialWillDo = SWD_SMALLMENU_END;
	SMenu[MN_LEARN_MAGIC_BOOK].nField[17].x = -55;
	SMenu[MN_LEARN_MAGIC_BOOK].nField[17].y = 273;
	SMenu[MN_LEARN_MAGIC_BOOK].nField[17].nRectImage = 14;
	SMenu[MN_LEARN_MAGIC_BOOK].nField[17].nImageNumber = 14;
	SMenu[MN_LEARN_MAGIC_BOOK].nField[17].nImageType = MAGIC_BOOK_MENU;
	SetRect(SMenu[MN_LEARN_MAGIC_BOOK].nField[17].rCheakBox, -55, 273, 6, 323);

	// Field[18]: 左側學習魔法 (Learn) 按鈕
	SMenu[MN_LEARN_MAGIC_BOOK].nField[18].nType = FT_LEARN_MAGIC;
	SMenu[MN_LEARN_MAGIC_BOOK].nField[18].nWillDo = 0; // left
	SMenu[MN_LEARN_MAGIC_BOOK].nField[18].x = 107;
	SMenu[MN_LEARN_MAGIC_BOOK].nField[18].y = 250;
	SMenu[MN_LEARN_MAGIC_BOOK].nField[18].nImageNumber = 991;
	SetRect(SMenu[MN_LEARN_MAGIC_BOOK].nField[18].rCheakBox, 107, 250, 163, 269);

	// Field[19]: 右側學習魔法 (Learn) 按鈕
	SMenu[MN_LEARN_MAGIC_BOOK].nField[19].nType = FT_LEARN_MAGIC;
	SMenu[MN_LEARN_MAGIC_BOOK].nField[19].nWillDo = 1; // right
	SMenu[MN_LEARN_MAGIC_BOOK].nField[19].x = 358;
	SMenu[MN_LEARN_MAGIC_BOOK].nField[19].y = 250;
	SMenu[MN_LEARN_MAGIC_BOOK].nField[19].nImageNumber = 991;
	SetRect(SMenu[MN_LEARN_MAGIC_BOOK].nField[19].rCheakBox, 358, 250, 414, 269);
	
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// �޸������ �������̽�			// ( temp --> �迭 , �迭�� ������.. )

	SMenu[MN_MEM_BOOK].x= 132+GABX_SCREEN;		
	SMenu[MN_MEM_BOOK].y= 50;				//  main.cpp ���� �ٲٴ� �κ��� ����.
	SMenu[MN_MEM_BOOK].nFieldCount = 20;
	SMenu[MN_MEM_BOOK].nImageType= MEM_BOOK;
	SMenu[MN_MEM_BOOK].nImageNumber = 0;

	// ���� å�� �ѱ��
	SMenu[MN_MEM_BOOK].nField[0].nType = FT_DSCROLL_MEM_BOOK;
	SMenu[MN_MEM_BOOK].nField[0].x=13+35;
	SMenu[MN_MEM_BOOK].nField[0].y=208+26;
	SMenu[MN_MEM_BOOK].nField[0].nRectImage=1003;
	SMenu[MN_MEM_BOOK].nField[0].nImageNumber=1004;
	SetRect(SMenu[MN_MEM_BOOK].nField[0].rCheakBox,13+35,208+26,36+35,223+26);

	// ������ å�� �ѱ��
	SMenu[MN_MEM_BOOK].nField[1].nType = FT_ISCROLL_MEM_BOOK;
	SMenu[MN_MEM_BOOK].nField[1].x=325+35;
	SMenu[MN_MEM_BOOK].nField[1].y=208+26;
	SMenu[MN_MEM_BOOK].nField[1].nRectImage=1001;
	SMenu[MN_MEM_BOOK].nField[1].nImageNumber=1002;
	SetRect(SMenu[MN_MEM_BOOK].nField[1].rCheakBox,325+35,208+26,348+35,223+26);

	// EXIT ��ư ��� ( �׻� ��� )
	SMenu[MN_MEM_BOOK].nField[2].nType = FT_MEM_BOOK_ETC;
	SMenu[MN_MEM_BOOK].nField[2].x=-30+35;
	SMenu[MN_MEM_BOOK].nField[2].y=3+26;
	SMenu[MN_MEM_BOOK].nField[2].nImageNumber = 1017;
//	SetRect(SMenu[MN_MEM_BOOK].nField[2].rCheakBox,44+35,52+26,85+35,92+26);
	
	// 1��° ����
	SMenu[MN_MEM_BOOK].nField[3].nType = FT_MEM_MAGIC_ICON;
	SMenu[MN_MEM_BOOK].nField[3].nWillDo=1;
	SMenu[MN_MEM_BOOK].nField[3].x=44+35;
	SMenu[MN_MEM_BOOK].nField[3].y=52+26;
	SetRect(SMenu[MN_MEM_BOOK].nField[3].rCheakBox,44+35,52+26,85+35,92+26);

	// 2��° ����
	SMenu[MN_MEM_BOOK].nField[4].nType = FT_MEM_MAGIC_ICON;
	SMenu[MN_MEM_BOOK].nField[4].nWillDo=2;
	SMenu[MN_MEM_BOOK].nField[4].x=107+35;
	SMenu[MN_MEM_BOOK].nField[4].y=52+26;
	SetRect(SMenu[MN_MEM_BOOK].nField[4].rCheakBox,107+35,52+26,148+35,92+26);

	// 3��° ����
	SMenu[MN_MEM_BOOK].nField[5].nType = FT_MEM_MAGIC_ICON;
	SMenu[MN_MEM_BOOK].nField[5].nWillDo=3;
	SMenu[MN_MEM_BOOK].nField[5].x=44+35;
	SMenu[MN_MEM_BOOK].nField[5].y=97+26;
	SetRect(SMenu[MN_MEM_BOOK].nField[5].rCheakBox,44+35,97+26,85+35,137+26);

	// 4��° ����
	SMenu[MN_MEM_BOOK].nField[6].nType = FT_MEM_MAGIC_ICON;
	SMenu[MN_MEM_BOOK].nField[6].nWillDo=4;
	SMenu[MN_MEM_BOOK].nField[6].x=107+35;
	SMenu[MN_MEM_BOOK].nField[6].y=97+26;
	SetRect(SMenu[MN_MEM_BOOK].nField[6].rCheakBox,107+35,97+26,148+35,137+26);

	// 5��° ����
	SMenu[MN_MEM_BOOK].nField[7].nType = FT_MEM_MAGIC_ICON;
	SMenu[MN_MEM_BOOK].nField[7].nWillDo=5;
	SMenu[MN_MEM_BOOK].nField[7].x=44+35;
	SMenu[MN_MEM_BOOK].nField[7].y=144+26;
	SetRect(SMenu[MN_MEM_BOOK].nField[7].rCheakBox,44+35,144+26,85+35,184+26);

	// 6��° ����
	SMenu[MN_MEM_BOOK].nField[8].nType = FT_MEM_MAGIC_ICON;
	SMenu[MN_MEM_BOOK].nField[8].nWillDo=6;
	SMenu[MN_MEM_BOOK].nField[8].x=107+35;
	SMenu[MN_MEM_BOOK].nField[8].y=144+26;
	SetRect(SMenu[MN_MEM_BOOK].nField[8].rCheakBox,107+35,144+26,148+35,183+26);

	// 7��° ����
	SMenu[MN_MEM_BOOK].nField[9].nType = FT_MEM_MAGIC_ICON;
	SMenu[MN_MEM_BOOK].nField[9].nWillDo=7;
	SMenu[MN_MEM_BOOK].nField[9].x=213+35;
	SMenu[MN_MEM_BOOK].nField[9].y=52+26;
	SetRect(SMenu[MN_MEM_BOOK].nField[9].rCheakBox,213+35,52+26,254+35,92+26);

	// 8��° ����
	SMenu[MN_MEM_BOOK].nField[10].nType = FT_MEM_MAGIC_ICON;
	SMenu[MN_MEM_BOOK].nField[10].nWillDo=8;
	SMenu[MN_MEM_BOOK].nField[10].x=277+35;
	SMenu[MN_MEM_BOOK].nField[10].y=52+26;
	SetRect(SMenu[MN_MEM_BOOK].nField[10].rCheakBox,277+35,52+26,318+35,92+26);

	// 9��° ����
	SMenu[MN_MEM_BOOK].nField[11].nType = FT_MEM_MAGIC_ICON;
	SMenu[MN_MEM_BOOK].nField[11].nWillDo=9;
	SMenu[MN_MEM_BOOK].nField[11].x=213+35;
	SMenu[MN_MEM_BOOK].nField[11].y=97+26;
	SetRect(SMenu[MN_MEM_BOOK].nField[11].rCheakBox,213+35,97+26,254+35,137+26);

	// 10��° ����
	SMenu[MN_MEM_BOOK].nField[12].nType = FT_MEM_MAGIC_ICON;
	SMenu[MN_MEM_BOOK].nField[12].nWillDo=10;
	SMenu[MN_MEM_BOOK].nField[12].x=277+35;
	SMenu[MN_MEM_BOOK].nField[12].y=97+26;
	SetRect(SMenu[MN_MEM_BOOK].nField[12].rCheakBox,277+35,97+26,318+35,137+26);

	// ���� ��� (�迭)// ����
	SMenu[MN_MEM_BOOK].nField[13].nType = FT_DATA_PUT;
	SMenu[MN_MEM_BOOK].nField[13].x=96+35;
	SMenu[MN_MEM_BOOK].nField[13].y=33+26;
	SMenu[MN_MEM_BOOK].nField[13].nImageNumber=DP_MAGIC_CLASS;

	// ���� ��� (�迭)// ������
	SMenu[MN_MEM_BOOK].nField[14].nType = FT_DATA_PUT;
	SMenu[MN_MEM_BOOK].nField[14].x=265+35;
	SMenu[MN_MEM_BOOK].nField[14].y=33+26;
	SMenu[MN_MEM_BOOK].nField[14].nImageNumber=DP_MAGIC_CLASS;

	// exit
	SMenu[MN_MEM_BOOK].nField[15].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_MEM_BOOK].nField[15].nSpecialWillDo = SWD_SMALLMENU_END;
	SMenu[MN_MEM_BOOK].nField[15].x=-30+35;
	SMenu[MN_MEM_BOOK].nField[15].y=3+26;
	SMenu[MN_MEM_BOOK].nField[15].nRectImage=1018;
	SMenu[MN_MEM_BOOK].nField[15].nImageNumber=1019;
	SetRect(SMenu[MN_MEM_BOOK].nField[15].rCheakBox,-30+35,3+26,12+35,37+26);

	// memorize button
	SMenu[MN_MEM_BOOK].nField[16].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_MEM_BOOK].nField[16].nSpecialWillDo = SWD_MEMORIZE_BUTTON;
	SMenu[MN_MEM_BOOK].nField[16].x=201+35;
	SMenu[MN_MEM_BOOK].nField[16].y=165+26;
	SMenu[MN_MEM_BOOK].nField[16].nRectImage=1020;
	SMenu[MN_MEM_BOOK].nField[16].nImageNumber=1021;
	SetRect(SMenu[MN_MEM_BOOK].nField[16].rCheakBox,201+35,165+26,264+35,175+26);
	
	// �޸������ ����Ʈ ���
	SMenu[MN_MEM_BOOK].nField[17].nType = FT_DATA_PUT;
	SMenu[MN_MEM_BOOK].nField[17].x=202+35;
	SMenu[MN_MEM_BOOK].nField[17].y=145+26;
	SMenu[MN_MEM_BOOK].nField[17].nImageNumber=DP_MEMORIZE_POINT;
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Awake menu �޴�

	SMenu[MN_AWAKE].x=SCREEN_WIDTH-50;
	SMenu[MN_AWAKE].y=300+GABY_SCREEN*2;
	SMenu[MN_AWAKE].nFieldCount = 1;
	SMenu[MN_AWAKE].nImageNumber = 0;

	// Awake ��ư
	SMenu[MN_AWAKE].nField[0].nType=FT_AWAKE_BUTTON;
	SMenu[MN_AWAKE].nField[0].x=0;
	SMenu[MN_AWAKE].nField[0].y=0;
	SMenu[MN_AWAKE].nField[0].nRectImage=10003;				// awake button
	SMenu[MN_AWAKE].nField[0].nImageNumber=10004;			// break rest
	SetRect(SMenu[MN_AWAKE].nField[0].rCheakBox,0,0,50,50);

///////////////////////////////////////////////////////////////////////////////////////////////////////
// ���� �Խ��� ��������..
	SMenu[MN_PUBLIC_BBS].x=450;
	SMenu[MN_PUBLIC_BBS].y=50;
	SMenu[MN_PUBLIC_BBS].nFieldCount = 2;
	SMenu[MN_PUBLIC_BBS].nImageNumber = 0;		
	SMenu[MN_PUBLIC_BBS].nImageType = PUBLIC_BBS;

	// ���뺸��
	SMenu[MN_PUBLIC_BBS].nField[0].nType=FT_PUBLIC_BBS;
	SMenu[MN_PUBLIC_BBS].nField[0].x=29;
	SMenu[MN_PUBLIC_BBS].nField[0].y=45;
	SMenu[MN_PUBLIC_BBS].nField[0].nWillDo=247;

	// ������ ���� �޴�	 && 	// exit
	SMenu[MN_PUBLIC_BBS].nField[1].nType=FT_PUBLIC_BBS_NEXT;
	SMenu[MN_PUBLIC_BBS].nField[1].x=122;
	SMenu[MN_PUBLIC_BBS].nField[1].y=287;
	SMenu[MN_PUBLIC_BBS].nField[1].nWillDo=247;
	SMenu[MN_PUBLIC_BBS].nField[1].nRectImage=1;	// �⺻ ����..
	SMenu[MN_PUBLIC_BBS].nField[1].nImageNumber=2;
	SMenu[MN_PUBLIC_BBS].nField[1].nImageType=PUBLIC_BBS;
	SetRect(SMenu[MN_PUBLIC_BBS].nField[1].rCheakBox,122,287,122+74,287+95);


///////////////////////////////////////////////////////////////////////////////////////////////////////
// ����� �޴�

	SMenu[MN_HUNTER_MAIN].x=450;
	SMenu[MN_HUNTER_MAIN].y=50;
	SMenu[MN_HUNTER_MAIN].nFieldCount = 4;
	SMenu[MN_HUNTER_MAIN].nImageNumber = 0;		
	SMenu[MN_HUNTER_MAIN].nImageType = HUNTER_MENU;

	// delay
	SMenu[MN_HUNTER_MAIN].nField[0].nType=FT_DELAY_BUTTON;		// �޴� �պκп� ��ġ�ϸ鼭 �ð��� ���� �ʾ������ ������ ������. work ������ �̿��Ѵ�.
	SMenu[MN_HUNTER_MAIN].nField[0].nWillDo=5;					// �ð�, 

	// ����� ���
	SMenu[MN_HUNTER_MAIN].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_HUNTER_MAIN].nField[1].nSpecialWillDo=SWD_FOCUS_WILLDO;
	SMenu[MN_HUNTER_MAIN].nField[1].nWillDo=MN_HUNTER_REGIST;
	SMenu[MN_HUNTER_MAIN].nField[1].x=35;
	SMenu[MN_HUNTER_MAIN].nField[1].y=81;
	SMenu[MN_HUNTER_MAIN].nField[1].nRectImage=1;	
	SMenu[MN_HUNTER_MAIN].nField[1].nImageNumber=2;
	SMenu[MN_HUNTER_MAIN].nField[1].nImageType=HUNTER_MENU;
	SetRect(SMenu[MN_HUNTER_MAIN].nField[1].rCheakBox,33,80,180,112);

	// ����� ����Ʈ
	SMenu[MN_HUNTER_MAIN].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_HUNTER_MAIN].nField[2].nSpecialWillDo=SWD_FOCUS_WILLDO;
	SMenu[MN_HUNTER_MAIN].nField[2].nWillDo=MN_HUNTER_LIST;
	SMenu[MN_HUNTER_MAIN].nField[2].x=35;
	SMenu[MN_HUNTER_MAIN].nField[2].y=139;
	SMenu[MN_HUNTER_MAIN].nField[2].nRectImage=3;	
	SMenu[MN_HUNTER_MAIN].nField[2].nImageNumber=4;
	SMenu[MN_HUNTER_MAIN].nField[2].nImageType=HUNTER_MENU;
	SetRect(SMenu[MN_HUNTER_MAIN].nField[2].rCheakBox,33,137,180,169);

	// DONE ��ư
	SMenu[MN_HUNTER_MAIN].nField[3].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_HUNTER_MAIN].nField[3].nSpecialWillDo = SWD_SMALLMENU_END;
	SMenu[MN_HUNTER_MAIN].nField[3].x=73;
	SMenu[MN_HUNTER_MAIN].nField[3].y=196;
	SMenu[MN_HUNTER_MAIN].nField[3].nRectImage=5;	
	SMenu[MN_HUNTER_MAIN].nField[3].nImageNumber=6;
	SMenu[MN_HUNTER_MAIN].nField[3].nImageType=HUNTER_MENU;
	SetRect(SMenu[MN_HUNTER_MAIN].nField[3].rCheakBox,72,195,144,222);



///////////////////////////////////////////////////////////////////////////////////////////////////////
// ����� ��� �޴�
	SMenu[MN_HUNTER_REGIST].x=450;
	SMenu[MN_HUNTER_REGIST].y=50;
	SMenu[MN_HUNTER_REGIST].nFieldCount = 5;
	SMenu[MN_HUNTER_REGIST].nImageNumber = 7;
	SMenu[MN_HUNTER_REGIST].nImageType = HUNTER_MENU;

	// ok 
	SMenu[MN_HUNTER_REGIST].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_HUNTER_REGIST].nField[0].nSpecialWillDo=SWD_HUNTER_REGIST_OK;
	SMenu[MN_HUNTER_REGIST].nField[0].x=27;
	SMenu[MN_HUNTER_REGIST].nField[0].y=279;
	SMenu[MN_HUNTER_REGIST].nField[0].nRectImage=8;	
	SMenu[MN_HUNTER_REGIST].nField[0].nImageNumber=9;
	SMenu[MN_HUNTER_REGIST].nField[0].nImageType=HUNTER_MENU;
	SetRect(SMenu[MN_HUNTER_REGIST].nField[0].rCheakBox,27,279,93,300);

	// cancel
	SMenu[MN_HUNTER_REGIST].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_HUNTER_REGIST].nField[1].nSpecialWillDo=SWD_SUBMENU_BACK;
	SMenu[MN_HUNTER_REGIST].nField[1].x=158;
	SMenu[MN_HUNTER_REGIST].nField[1].y=279;
	SMenu[MN_HUNTER_REGIST].nField[1].nRectImage=10;	
	SMenu[MN_HUNTER_REGIST].nField[1].nImageNumber=11;
	SMenu[MN_HUNTER_REGIST].nField[1].nImageType=HUNTER_MENU;
	SetRect(SMenu[MN_HUNTER_REGIST].nField[1].rCheakBox,158,279,224,300);

	// view �̸�
	SMenu[MN_HUNTER_REGIST].nField[2].nType=FT_HUNTER_REGIST;
	SMenu[MN_HUNTER_REGIST].nField[2].x=111;
	SMenu[MN_HUNTER_REGIST].nField[2].y=56;
	SMenu[MN_HUNTER_REGIST].nField[2].nWillDo = 1;			// �̸�
	SMenu[MN_HUNTER_REGIST].nField[2].nRectImage = 100;		
	SetRect(SMenu[MN_HUNTER_REGIST].nField[2].rCheakBox,106,54,224,72);

	// view �ݾ�
	SMenu[MN_HUNTER_REGIST].nField[3].nType=FT_HUNTER_REGIST;
	SMenu[MN_HUNTER_REGIST].nField[3].x=111;
	SMenu[MN_HUNTER_REGIST].nField[3].y=111;
	SMenu[MN_HUNTER_REGIST].nField[3].nWillDo = 2;			// �ݾ�
	SMenu[MN_HUNTER_REGIST].nField[3].nRectImage = 80;		
	SetRect(SMenu[MN_HUNTER_REGIST].nField[3].rCheakBox,107,104,200,136);

	// view ��������
	SMenu[MN_HUNTER_REGIST].nField[4].nType=FT_HUNTER_REGIST;
	SMenu[MN_HUNTER_REGIST].nField[4].x=24;
	SMenu[MN_HUNTER_REGIST].nField[4].y=177;
	SMenu[MN_HUNTER_REGIST].nField[4].nWillDo = 3;			// ��������
	SMenu[MN_HUNTER_REGIST].nField[4].nRectImage = 200;		
	SetRect(SMenu[MN_HUNTER_REGIST].nField[4].rCheakBox,19,172,205,258);


///////////////////////////////////////////////////////////////////////////////////////////////////////
// ����� ���� �޴�
	SMenu[MN_HUNTER_LIST].x=450;
	SMenu[MN_HUNTER_LIST].y=50;
	SMenu[MN_HUNTER_LIST].nFieldCount = 5;
	SMenu[MN_HUNTER_LIST].nImageNumber = 20;		
	SMenu[MN_HUNTER_LIST].nImageType = HUNTER_MENU;

	// accept
	SMenu[MN_HUNTER_LIST].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_HUNTER_LIST].nField[0].nSpecialWillDo=SWD_HUNTER_LIST_OK;		// �� ��Ͽ� ��� �Ѵ�.
	SMenu[MN_HUNTER_LIST].nField[0].x=49;

#if defined(TAIWAN_LOCALIZING_ ) || defined( CHINA_LOCALIZING_ ) || defined (HONGKONG_LOCALIZING_)//010730 lms
	SMenu[MN_HUNTER_LIST].nField[0].y=292;
#else
	SMenu[MN_HUNTER_LIST].nField[0].y=291;
#endif
	SMenu[MN_HUNTER_LIST].nField[0].nRectImage=21;	
	SMenu[MN_HUNTER_LIST].nField[0].nImageNumber=22;
	SMenu[MN_HUNTER_LIST].nField[0].nImageType=HUNTER_MENU;
	SetRect(SMenu[MN_HUNTER_LIST].nField[0].rCheakBox,49,290,121,311);

	// cancel
	SMenu[MN_HUNTER_LIST].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_HUNTER_LIST].nField[1].nSpecialWillDo=SWD_SUBMENU_BACK;
	SMenu[MN_HUNTER_LIST].nField[1].x=126;
#if defined(TAIWAN_LOCALIZING_ ) || defined( CHINA_LOCALIZING_ ) || defined (HONGKONG_LOCALIZING_)//010730 lms
	SMenu[MN_HUNTER_LIST].nField[1].y=292;
#else
	SMenu[MN_HUNTER_LIST].nField[1].y=291;
#endif
	SMenu[MN_HUNTER_LIST].nField[1].nRectImage=23;	
	SMenu[MN_HUNTER_LIST].nField[1].nImageNumber=24;
	SMenu[MN_HUNTER_LIST].nField[1].nImageType=HUNTER_MENU;
	SetRect(SMenu[MN_HUNTER_LIST].nField[1].rCheakBox,126,291,198,312);

	// <- prev
	SMenu[MN_HUNTER_LIST].nField[2].nType=FT_HUNTER_LIST_SCROLL;
	SMenu[MN_HUNTER_LIST].nField[2].nWillDo=0;	// prev
	SMenu[MN_HUNTER_LIST].nField[2].x=11;
	SMenu[MN_HUNTER_LIST].nField[2].y=292;
	SMenu[MN_HUNTER_LIST].nField[2].nRectImage=25;	
	SMenu[MN_HUNTER_LIST].nField[2].nImageNumber=26;
	SMenu[MN_HUNTER_LIST].nField[2].nImageType=HUNTER_MENU;
	SetRect(SMenu[MN_HUNTER_LIST].nField[2].rCheakBox,12,292,45,312);

	// next ->
	SMenu[MN_HUNTER_LIST].nField[3].nType=FT_HUNTER_LIST_SCROLL;
	SMenu[MN_HUNTER_LIST].nField[3].nWillDo=1;	// next
	SMenu[MN_HUNTER_LIST].nField[3].x=203;
	SMenu[MN_HUNTER_LIST].nField[3].y=292;
	SMenu[MN_HUNTER_LIST].nField[3].nRectImage=27;	
	SMenu[MN_HUNTER_LIST].nField[3].nImageNumber=28;
	SMenu[MN_HUNTER_LIST].nField[3].nImageType=HUNTER_MENU;
	SetRect(SMenu[MN_HUNTER_LIST].nField[3].rCheakBox,203,292,236,312);

	// view
	SMenu[MN_HUNTER_LIST].nField[4].nType=FT_HUNTER_LIST;
	SMenu[MN_HUNTER_LIST].nField[4].x=85;
	SMenu[MN_HUNTER_LIST].nField[4].y=183;

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
// ���� ���幮 ��� �Խ��� �޴�
	SMenu[MN_NATION_BBS].x=177+240;
	SMenu[MN_NATION_BBS].y=65+60;
	SMenu[MN_NATION_BBS].nImageNumber = 0;
	SMenu[MN_NATION_BBS].nFieldCount = 5;
	SMenu[MN_NATION_BBS].nImageType = NATION_BBS_MENU;

	// ���뺸��
	//SMenu[MN_NATION_BBS].nField[0].nType=FT_PUBLIC_BBS;
	SMenu[MN_NATION_BBS].nField[0].nType=FT_NATION_BBS;
	SMenu[MN_NATION_BBS].nField[0].x=47;
	SMenu[MN_NATION_BBS].nField[0].y=19;
	SMenu[MN_NATION_BBS].nField[0].nRectImage=330;
	SMenu[MN_NATION_BBS].nField[0].nImageNumber=140;		// �۾� ��� ��Ű�� ���� ����
	SMenu[MN_NATION_BBS].nField[0].nImageType=NATION_BBS_MENU;

	// ok button
	SMenu[MN_NATION_BBS].nField[1].nType=FT_NATION_BUTTON;
	SMenu[MN_NATION_BBS].nField[1].x=48+6;
	SMenu[MN_NATION_BBS].nField[1].y=199+4;
	SMenu[MN_NATION_BBS].nField[1].nWillDo=0;
	SMenu[MN_NATION_BBS].nField[1].nRectImage=6;
	SMenu[MN_NATION_BBS].nField[1].nImageNumber=5;
	SMenu[MN_NATION_BBS].nField[1].nImageType=NATION_BBS_MENU;
	SetRect(SMenu[MN_NATION_BBS].nField[1].rCheakBox,56,205,118,236);

	// previous
	SMenu[MN_NATION_BBS].nField[2].nType=FT_NATION_BUTTON;
	SMenu[MN_NATION_BBS].nField[2].x=136+12;
	SMenu[MN_NATION_BBS].nField[2].y=199+5;
	SMenu[MN_NATION_BBS].nField[2].nWillDo=1;
	SMenu[MN_NATION_BBS].nField[2].nRectImage=8;	
	SMenu[MN_NATION_BBS].nField[2].nImageNumber=7;
	SMenu[MN_NATION_BBS].nField[2].nImageType=NATION_BBS_MENU;
	SetRect(SMenu[MN_NATION_BBS].nField[2].rCheakBox,147,205,207,233);

	// next
	SMenu[MN_NATION_BBS].nField[3].nType=FT_NATION_BUTTON;
	SMenu[MN_NATION_BBS].nField[3].x=231+9;
	SMenu[MN_NATION_BBS].nField[3].y=199+4;
	SMenu[MN_NATION_BBS].nField[3].nWillDo=2;
	SMenu[MN_NATION_BBS].nField[3].nRectImage=4;	
	SMenu[MN_NATION_BBS].nField[3].nImageNumber=3;
	SMenu[MN_NATION_BBS].nField[3].nImageType=NATION_BBS_MENU;
	SetRect(SMenu[MN_NATION_BBS].nField[3].rCheakBox,242,207,304,233);

	// exit
	SMenu[MN_NATION_BBS].nField[4].nType=FT_NATION_BUTTON;
	SMenu[MN_NATION_BBS].nField[4].x=319+13;
	SMenu[MN_NATION_BBS].nField[4].y=199+3;
	SMenu[MN_NATION_BBS].nField[4].nWillDo=3;
	SMenu[MN_NATION_BBS].nField[4].nRectImage=2;	
	SMenu[MN_NATION_BBS].nField[4].nImageNumber=1;
	SMenu[MN_NATION_BBS].nField[4].nImageType=NATION_BBS_MENU;
	SetRect(SMenu[MN_NATION_BBS].nField[4].rCheakBox,336,206,392,233);

//////////////////////////////////////////////////////////////////////////////////////////////////
//	�ó����� ���÷���		
	SMenu[MN_SCENARIO].x=0;
	SMenu[MN_SCENARIO].y=0;
	SMenu[MN_SCENARIO].nImageNumber=0;
	SMenu[MN_SCENARIO].nFieldCount=1;

	SMenu[MN_SCENARIO].nField[0].nType = FT_DISPLAY_SCENARIO;
	SetRect(SMenu[MN_SCENARIO].nField[0].rCheakBox,0,0,SCREEN_WIDTH, SCREEN_HEIGHT);

////////////////////////////////////////////////////////////////////////////////////////////////////
// �ĺ� ��� �޴�
	SMenu[MN_VOTE_REGIST].x=271;
	SMenu[MN_VOTE_REGIST].y=50;
	SMenu[MN_VOTE_REGIST].nImageNumber=13;
	SMenu[MN_VOTE_REGIST].nImageType=VOTE_MENU;
	SMenu[MN_VOTE_REGIST].nFieldCount=3;

	// ����� ��� ( ���� ��Ÿ �̹��� ��� )
	SMenu[MN_VOTE_REGIST].nField[0].nType = FT_VOTE_REGIST;
	SMenu[MN_VOTE_REGIST].nField[0].x=10;
	SMenu[MN_VOTE_REGIST].nField[0].y=38;
	SMenu[MN_VOTE_REGIST].nField[0].nImageNumber=4;		// ���� ����
	SMenu[MN_VOTE_REGIST].nField[0].nRectImage=5;		// ȭ��ǥ ����� (9,222), (208,222)
	SMenu[MN_VOTE_REGIST].nField[0].nWillDo=9;		// ok ��ư ��� 52, 223
	SMenu[MN_VOTE_REGIST].nField[0].nImageType=VOTE_MENU;

	// vote ��ǥ�ϱ�
	SMenu[MN_VOTE_REGIST].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_VOTE_REGIST].nField[1].nSpecialWillDo=SWD_VOTER_REGIST_OK;
	SMenu[MN_VOTE_REGIST].nField[1].x=52;
	SMenu[MN_VOTE_REGIST].nField[1].y=223;
	SMenu[MN_VOTE_REGIST].nField[1].nRectImage=10;
	SMenu[MN_VOTE_REGIST].nField[1].nImageNumber=8;
	SMenu[MN_VOTE_REGIST].nField[1].nImageType=VOTE_MENU;
	SetRect(SMenu[MN_VOTE_REGIST].nField[1].rCheakBox,52,223,126,245);

	// EXIT
	SMenu[MN_VOTE_REGIST].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_VOTE_REGIST].nField[2].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_VOTE_REGIST].nField[2].x=131;
	SMenu[MN_VOTE_REGIST].nField[2].y=223;
	SMenu[MN_VOTE_REGIST].nField[2].nRectImage=7;	
	SMenu[MN_VOTE_REGIST].nField[2].nImageNumber=6;
	SMenu[MN_VOTE_REGIST].nField[2].nImageType=VOTE_MENU;
	SetRect(SMenu[MN_VOTE_REGIST].nField[2].rCheakBox,131,223,205,245);


////////////////////////////////////////////////////////////////////////////////////////////////////
// ��ǥ �޴�
	SMenu[MN_VOTE_SUPPORT].x=271;
	SMenu[MN_VOTE_SUPPORT].y=50;
	SMenu[MN_VOTE_SUPPORT].nImageNumber=13;
	SMenu[MN_VOTE_SUPPORT].nImageType=VOTE_MENU;
	SMenu[MN_VOTE_SUPPORT].nFieldCount=5;

	// ����� ��� ( ���� ��Ÿ �̹��� ��� )
	SMenu[MN_VOTE_SUPPORT].nField[0].nType = FT_VOTE_SUPPORT;
	SMenu[MN_VOTE_SUPPORT].nField[0].x=10;
	SMenu[MN_VOTE_SUPPORT].nField[0].y=38;
	SMenu[MN_VOTE_SUPPORT].nField[0].nImageNumber=14;		// ���� ���
	SMenu[MN_VOTE_SUPPORT].nField[0].nImageType=VOTE_MENU;

	// ok
	SMenu[MN_VOTE_SUPPORT].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_VOTE_SUPPORT].nField[1].nSpecialWillDo=SWD_VOTER_SUPPORT_OK;		// ��ǥ �ϴ°�
	SMenu[MN_VOTE_SUPPORT].nField[1].x=52;
	SMenu[MN_VOTE_SUPPORT].nField[1].y=223;
	SMenu[MN_VOTE_SUPPORT].nField[1].nRectImage=12;
	SMenu[MN_VOTE_SUPPORT].nField[1].nImageNumber=11;
	SMenu[MN_VOTE_SUPPORT].nField[1].nImageType=VOTE_MENU;
	SetRect(SMenu[MN_VOTE_SUPPORT].nField[1].rCheakBox,52,223,126,245);

	// EXIT
	SMenu[MN_VOTE_SUPPORT].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_VOTE_SUPPORT].nField[2].nSpecialWillDo=SWD_SUBMENU_BACK;		// ��ǥ ���� // ���� ��Ȳ �ѷ��ش�.
	SMenu[MN_VOTE_SUPPORT].nField[2].x=131;
	SMenu[MN_VOTE_SUPPORT].nField[2].y=223;
	SMenu[MN_VOTE_SUPPORT].nField[2].nRectImage=7;	
	SMenu[MN_VOTE_SUPPORT].nField[2].nImageNumber=6;
	SMenu[MN_VOTE_SUPPORT].nField[2].nImageType=VOTE_MENU;
	SetRect(SMenu[MN_VOTE_SUPPORT].nField[2].rCheakBox,131,223,205,245);

	// <-- ��ũ��
	SMenu[MN_VOTE_SUPPORT].nField[3].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_VOTE_SUPPORT].nField[3].nSpecialWillDo=SWD_VOTER_SUPPORT_SCROLL;
	SMenu[MN_VOTE_SUPPORT].nField[3].nWillDo=LEFT;
	SMenu[MN_VOTE_SUPPORT].nField[3].x=11;
	SMenu[MN_VOTE_SUPPORT].nField[3].y=223;
	SMenu[MN_VOTE_SUPPORT].nField[3].nRectImage=3;
	SMenu[MN_VOTE_SUPPORT].nField[3].nImageNumber=2;
	SMenu[MN_VOTE_SUPPORT].nField[3].nImageType=VOTE_MENU;
	SetRect(SMenu[MN_VOTE_SUPPORT].nField[3].rCheakBox,9,223,46,245);

	// --> ��ũ��
	SMenu[MN_VOTE_SUPPORT].nField[4].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_VOTE_SUPPORT].nField[4].nSpecialWillDo=SWD_VOTER_SUPPORT_SCROLL;		// ��ǥ ���� // ���� ��Ȳ �ѷ��ش�.
	SMenu[MN_VOTE_SUPPORT].nField[4].nWillDo=RIGHT;
	SMenu[MN_VOTE_SUPPORT].nField[4].x=209;
	SMenu[MN_VOTE_SUPPORT].nField[4].y=223;
	SMenu[MN_VOTE_SUPPORT].nField[4].nRectImage=1;	
	SMenu[MN_VOTE_SUPPORT].nField[4].nImageNumber=0;
	SMenu[MN_VOTE_SUPPORT].nField[4].nImageType=VOTE_MENU;
	SetRect(SMenu[MN_VOTE_SUPPORT].nField[4].rCheakBox,207,223,244,245);
////////////////////////////////////////////////////////////////////////////////////////////
// ������ �޴�
	SMenu[MN_SALVATION].x=200+GABX_SCREEN;
	SMenu[MN_SALVATION].y=75;//+GABY_SCREEN;
	SMenu[MN_SALVATION].nImageNumber=16;
	SMenu[MN_SALVATION].nImageType=SALVATION_PCX;
	SMenu[MN_SALVATION].key=6;				// ���������� �� ���μ�
	SMenu[MN_SALVATION].nFieldCount=10;

	// ĳ���� ���ϸ��̼�
	//SMenu[MN_SALVATION].nField[0].nType=FT_ANIMATION;
	SMenu[MN_SALVATION].nField[0].x=200;
	SMenu[MN_SALVATION].nField[0].y=50;
	SMenu[MN_SALVATION].nField[0].nSHideNomalStart=8;		// delay count
	SMenu[MN_SALVATION].nField[0].nImageNumber=0;
	SMenu[MN_SALVATION].nField[0].nImageType=SALVATION_PCX_ANI;

	// �ؽ�Ʈ ���
	SMenu[MN_SALVATION].nField[1].nType=FT_SCROLL_TEXT_PUT;
	SMenu[MN_SALVATION].nField[1].x=35;
	SMenu[MN_SALVATION].nField[1].y=69;
	SMenu[MN_SALVATION].nField[1].nRectImage=159;
	strcpy(SMenu[MN_SALVATION].nField[1].temp, lan->OutputMessage(3,201));//lsw
	SMenu[MN_SALVATION].nField[1].m_lpTemp = SMenu[MN_SALVATION].nField[1].temp;


	// �ؽ�Ʈ ��ũ��
	SMenu[MN_SALVATION].nField[2].nType=FT_SCROLL_BUTTON;
	SMenu[MN_SALVATION].nField[2].nWillDo = SCROLL_UP;
	SMenu[MN_SALVATION].nField[2].x=207;
	SMenu[MN_SALVATION].nField[2].y=65;
	SMenu[MN_SALVATION].nField[2].nShideNomalPlus=2;		// ������ ��ư x, y ��ǥ gab
	SMenu[MN_SALVATION].nField[2].nSHideNomalCount=2;		// ����
	SMenu[MN_SALVATION].nField[2].nRectImage=1;				// ����
	SMenu[MN_SALVATION].nField[2].nImageNumber=0;			// ���� ��ư
	SMenu[MN_SALVATION].nField[2].nImageType=SALVATION_PCX;
	SetRect(SMenu[MN_SALVATION].nField[2].rCheakBox,197,65,218,97);

	// �ؽ�Ʈ ��ũ��
	SMenu[MN_SALVATION].nField[3].nType=FT_SCROLL_BUTTON;
	SMenu[MN_SALVATION].nField[3].nWillDo = SCROLL_DOWN;
	SMenu[MN_SALVATION].nField[3].x=207;
	SMenu[MN_SALVATION].nField[3].y=140;
	SMenu[MN_SALVATION].nField[3].nShideNomalPlus=2;		// ������ ��ư x, y ��ǥ gab
	SMenu[MN_SALVATION].nField[3].nSHideNomalCount=5;		// ����
	SMenu[MN_SALVATION].nField[3].nRectImage=4;				// ����
	SMenu[MN_SALVATION].nField[3].nImageNumber=3;			// ���� ��ư
	SMenu[MN_SALVATION].nField[3].nImageType=SALVATION_PCX;
	SetRect(SMenu[MN_SALVATION].nField[3].rCheakBox,197,140,218,172);

	// OK
	SMenu[MN_SALVATION].nField[4].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_SALVATION].nField[4].nSpecialWillDo=SWD_SALVATION_OK;
	SMenu[MN_SALVATION].nField[4].x=32;
	SMenu[MN_SALVATION].nField[4].y=291;
	SMenu[MN_SALVATION].nField[4].nRectImage=15;
	SMenu[MN_SALVATION].nField[4].nImageNumber=14;
	SMenu[MN_SALVATION].nField[4].nImageType=SALVATION_PCX;
	SetRect(SMenu[MN_SALVATION].nField[4].rCheakBox,32,289,98,310);

	// CANCEL
	SMenu[MN_SALVATION].nField[5].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_SALVATION].nField[5].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_SALVATION].nField[5].x=150;
	SMenu[MN_SALVATION].nField[5].y=291;
	SMenu[MN_SALVATION].nField[5].nRectImage=12;
	SMenu[MN_SALVATION].nField[5].nImageNumber=11;
	SMenu[MN_SALVATION].nField[5].nImageType=SALVATION_PCX;
	SetRect(SMenu[MN_SALVATION].nField[5].rCheakBox,150,289,216,310);

	// scroll butoon	// up
	SMenu[MN_SALVATION].nField[6].nType=FT_DO;
	SMenu[MN_SALVATION].nField[6].nWillDo = DO_BANK_SCROLL_UP;
//	SMenu[MN_SALVATION].nField[6].nType=FT_SALVATION_MONEY_SCROLL;
//	SMenu[MN_SALVATION].nField[6].nWillDo=SCROLL_UP;
	SMenu[MN_SALVATION].nField[6].x=102;
	SMenu[MN_SALVATION].nField[6].y=214;
	SMenu[MN_SALVATION].nField[6].nRectImage=8;
	SMenu[MN_SALVATION].nField[6].nImageNumber=7;
	SMenu[MN_SALVATION].nField[6].nImageType=SALVATION_PCX;
	SetRect(SMenu[MN_SALVATION].nField[6].rCheakBox,96+6,207+6,111+6,219+6);
						// down
	SMenu[MN_SALVATION].nField[7].nType=FT_DO;
	SMenu[MN_SALVATION].nField[7].nWillDo = DO_BANK_SCROLL_DOWN;
//	SMenu[MN_SALVATION].nField[7].nType=FT_SALVATION_MONEY_SCROLL;
//	SMenu[MN_SALVATION].nField[7].nWillDo=SCROLL_DOWN;
	SMenu[MN_SALVATION].nField[7].x=102;
	SMenu[MN_SALVATION].nField[7].y=228;
	SMenu[MN_SALVATION].nField[7].nRectImage=10;
	SMenu[MN_SALVATION].nField[7].nImageNumber=9;
	SMenu[MN_SALVATION].nField[7].nImageType=SALVATION_PCX;
	SetRect(SMenu[MN_SALVATION].nField[7].rCheakBox,96+6,221+6,111+6,234+6);

	// ����� �� ���
	SMenu[MN_SALVATION].nField[8].nType=FT_SALVATION_MONEY;
	SMenu[MN_SALVATION].nField[8].x=113+89+4;
	SMenu[MN_SALVATION].nField[8].y=215+6;

	// �ѱ�� �ݾ� ���
	SMenu[MN_SALVATION].nField[9].nType=FT_DATA_PUT;
	SMenu[MN_SALVATION].nField[9].nWillDo=89;
	SMenu[MN_SALVATION].nField[9].x=113+4;
	SMenu[MN_SALVATION].nField[9].y=250+6;
	SMenu[MN_SALVATION].nField[9].nImageNumber=DP_MONEY2;
	SMenu[MN_SALVATION].nField[9].nValue = &TotalSalvation;

////////////////////////////////////////////////////////////////////////////////////////////
// ��� �޴�  : ��Ʈ ��Ʈ�� �ش�.
	SMenu[MN_SALVATION2].x= CENTER_X;
	SMenu[MN_SALVATION2].y=175;
	SMenu[MN_SALVATION2].nImageNumber=13;
	SMenu[MN_SALVATION2].nImageType=SALVATION_PCX;
	SMenu[MN_SALVATION2].key=6;				// ���������� �� ���μ�
	SMenu[MN_SALVATION2].nFieldCount=12;

	// ���� ����ֱ�
	SMenu[MN_SALVATION2].nField[0].nType=FT_DATA_PUT;
	SMenu[MN_SALVATION2].nField[0].x=125;
	SMenu[MN_SALVATION2].nField[0].y=33;
	SMenu[MN_SALVATION2].nField[0].nImageNumber=DP_TITLE_PUT;
	SMenu[MN_SALVATION2].nField[0].nWillDo=RGB(255, 255, 255);
	strcpy(SMenu[MN_SALVATION2].nField[0].temp, kein_GetMenuString( 82 ) );

	// �߰� ������ ����ֱ�
	SMenu[MN_SALVATION2].nField[1].nType=FT_NOMAL_PUT;
	SMenu[MN_SALVATION2].nField[1].x=21;
	SMenu[MN_SALVATION2].nField[1].y=201;
	SMenu[MN_SALVATION2].nField[1].nImageNumber=6;
	SMenu[MN_SALVATION2].nField[1].nImageType=SALVATION_PCX;

	// �ؽ�Ʈ ���
	SMenu[MN_SALVATION2].nField[2].nType=FT_SCROLL_TEXT_PUT;
	SMenu[MN_SALVATION2].nField[2].x=35;
	SMenu[MN_SALVATION2].nField[2].y=69;
	SMenu[MN_SALVATION2].nField[2].nWillDo = RGB16( 255, 255, 255);
	SMenu[MN_SALVATION2].nField[2].nSpecialWillDo = 0;		// CheakType �� 1�� �ٲ� �ٽô� �ʱ�ȭ ��ƾ���� �ȵ�� ���� �Ѵ�.
	SMenu[MN_SALVATION2].nField[2].nRectImage=159;
	strcpy(SMenu[MN_SALVATION2].nField[2].temp, kein_GetMenuString(36) );
	SMenu[MN_SALVATION2].nField[2].m_lpTemp = SMenu[MN_SALVATION2].nField[2].temp;

	// �ؽ�Ʈ ��ũ��
	SMenu[MN_SALVATION2].nField[3].nType=FT_SCROLL_BUTTON;
	SMenu[MN_SALVATION2].nField[3].nWillDo = SCROLL_UP;
	SMenu[MN_SALVATION2].nField[3].x=207;
	SMenu[MN_SALVATION2].nField[3].y=65;
	SMenu[MN_SALVATION2].nField[3].nShideNomalPlus=2;		// ������ ��ư x, y ��ǥ gab
	SMenu[MN_SALVATION2].nField[3].nSHideNomalCount=2;		// ����
	SMenu[MN_SALVATION2].nField[3].nRectImage=1;				// ����
	SMenu[MN_SALVATION2].nField[3].nImageNumber=0;			// ���� ��ư
	SMenu[MN_SALVATION2].nField[3].nImageType=SALVATION_PCX;
	SetRect(SMenu[MN_SALVATION2].nField[3].rCheakBox,197,65,218,97);

	// �ؽ�Ʈ ��ũ��
	SMenu[MN_SALVATION2].nField[4].nType=FT_SCROLL_BUTTON;
	SMenu[MN_SALVATION2].nField[4].nWillDo = SCROLL_DOWN;
	SMenu[MN_SALVATION2].nField[4].x=207;
	SMenu[MN_SALVATION2].nField[4].y=140;
	SMenu[MN_SALVATION2].nField[4].nShideNomalPlus=2;		// ������ ��ư x, y ��ǥ gab
	SMenu[MN_SALVATION2].nField[4].nSHideNomalCount=5;		// ����
	SMenu[MN_SALVATION2].nField[4].nRectImage=4;				// ����
	SMenu[MN_SALVATION2].nField[4].nImageNumber=3;			// ���� ��ư
	SMenu[MN_SALVATION2].nField[4].nImageType=SALVATION_PCX;
	SetRect(SMenu[MN_SALVATION2].nField[4].rCheakBox,197,140,218,172);

	// �ؽ�Ʈ ���
	SMenu[MN_SALVATION2].nField[5].nType=FT_CHECK_SALVATION;
	SMenu[MN_SALVATION2].nField[5].x=40;
	SMenu[MN_SALVATION2].nField[5].y=173;
	SMenu[MN_SALVATION2].nField[5].nWillDo = RGB16( 255, 255, 255);
	SMenu[MN_SALVATION2].nField[5].nRectImage=159;

	// OK
	SMenu[MN_SALVATION2].nField[6].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_SALVATION2].nField[6].nSpecialWillDo=SWD_SALVATION_OK;
	SMenu[MN_SALVATION2].nField[6].x=32;
	SMenu[MN_SALVATION2].nField[6].y=289;
	SMenu[MN_SALVATION2].nField[6].nRectImage=15;
	SMenu[MN_SALVATION2].nField[6].nImageNumber=14;
	SMenu[MN_SALVATION2].nField[6].nImageType=SALVATION_PCX;
	SetRect(SMenu[MN_SALVATION2].nField[6].rCheakBox,32,289,98,310);

	// CANCEL
	SMenu[MN_SALVATION2].nField[7].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_SALVATION2].nField[7].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_SALVATION2].nField[7].x=150;
	SMenu[MN_SALVATION2].nField[7].y=289;
	SMenu[MN_SALVATION2].nField[7].nRectImage=12;
	SMenu[MN_SALVATION2].nField[7].nImageNumber=11;
	SMenu[MN_SALVATION2].nField[7].nImageType=SALVATION_PCX;
	SetRect(SMenu[MN_SALVATION2].nField[7].rCheakBox,150,289,216,310);

	// scroll butoon	// up
	SMenu[MN_SALVATION2].nField[8].nType=FT_DO;
	SMenu[MN_SALVATION2].nField[8].nWillDo = DO_BANK_SCROLL_UP;
	SMenu[MN_SALVATION2].nField[8].x=113;
	SMenu[MN_SALVATION2].nField[8].y=214-7;
	SMenu[MN_SALVATION2].nField[8].nRectImage=8;
	SMenu[MN_SALVATION2].nField[8].nImageNumber=7;
	SMenu[MN_SALVATION2].nField[8].nImageType=SALVATION_PCX;
	SetRect(SMenu[MN_SALVATION2].nField[8].rCheakBox,113,205,128,221);
						// down
	SMenu[MN_SALVATION2].nField[9].nType=FT_DO;
	SMenu[MN_SALVATION2].nField[9].nWillDo = DO_BANK_SCROLL_DOWN;
	SMenu[MN_SALVATION2].nField[9].x=113;
	SMenu[MN_SALVATION2].nField[9].y=228-7;
	SMenu[MN_SALVATION2].nField[9].nRectImage=10;
	SMenu[MN_SALVATION2].nField[9].nImageNumber=9;
	SMenu[MN_SALVATION2].nField[9].nImageType=SALVATION_PCX;
	SetRect(SMenu[MN_SALVATION2].nField[9].rCheakBox,113,219,128,236);

	// ����� �� ���
	SMenu[MN_SALVATION2].nField[10].nType=FT_SALVATION_MONEY;
	SMenu[MN_SALVATION2].nField[10].x=113+89;
	SMenu[MN_SALVATION2].nField[10].y=215;

	// �ѱ�� �ݾ� ���
	SMenu[MN_SALVATION2].nField[11].nType=FT_DATA_PUT;
	SMenu[MN_SALVATION2].nField[11].nWillDo=89;
	SMenu[MN_SALVATION2].nField[11].x=113;
	SMenu[MN_SALVATION2].nField[11].y=250;
	SMenu[MN_SALVATION2].nField[11].nImageNumber=DP_MONEY2;
	SMenu[MN_SALVATION2].nField[11].nValue = &TotalSalvation;


////////////////////////////////////////////////////////////////////////////////////////////
// 
	/*
	SMenu[MN_TEAM_BATTLE_INFO].x=193+GABX_SCREEN;
	SMenu[MN_TEAM_BATTLE_INFO].y=75;//+GABY_SCREEN;
	SMenu[MN_TEAM_BATTLE_INFO].nImageNumber=0;
	SMenu[MN_TEAM_BATTLE_INFO].nImageType=TEAM_BATTLE;
	SMenu[MN_TEAM_BATTLE_INFO].nFieldCount=5;

	// ���� ĳ���� �̸� ���
	SMenu[MN_TEAM_BATTLE_INFO].nField[0].nType=FT_TEMBATLE_INFO;
	SMenu[MN_TEAM_BATTLE_INFO].nField[0].nWillDo=0;
	SMenu[MN_TEAM_BATTLE_INFO].nField[0].x=35;
	SMenu[MN_TEAM_BATTLE_INFO].nField[0].y=185;

	SMenu[MN_TEAM_BATTLE_INFO].nField[1].nType=FT_TEMBATLE_INFO;
	SMenu[MN_TEAM_BATTLE_INFO].nField[1].nWillDo=1;
	SMenu[MN_TEAM_BATTLE_INFO].nField[1].x=162;
	SMenu[MN_TEAM_BATTLE_INFO].nField[1].y=185;

	SMenu[MN_TEAM_BATTLE_INFO].nField[2].nType=FT_TEMBATLE_INFO;
	SMenu[MN_TEAM_BATTLE_INFO].nField[2].nWillDo=2;
	SMenu[MN_TEAM_BATTLE_INFO].nField[2].x=284;
	SMenu[MN_TEAM_BATTLE_INFO].nField[2].y=185;

	SMenu[MN_TEAM_BATTLE_INFO].nField[3].nType=FT_TEMBATLE_INFO;
	SMenu[MN_TEAM_BATTLE_INFO].nField[3].nWillDo=3;
	SMenu[MN_TEAM_BATTLE_INFO].nField[3].x=417;
	SMenu[MN_TEAM_BATTLE_INFO].nField[3].y=185;

	// ���� ����ϱ�
	SMenu[MN_TEAM_BATTLE_INFO].nField[4].nType=FT_TEMBATLE_ADD;
	SMenu[MN_TEAM_BATTLE_INFO].nField[4].nWillDo=0;
	SMenu[MN_TEAM_BATTLE_INFO].nField[4].x=17;
	SMenu[MN_TEAM_BATTLE_INFO].nField[4].y=185;
	SetRect(SMenu[MN_TEAM_BATTLE_INFO].nField[4].rCheakBox,35,31,127,47);

*/
///////////////////////////////////////////////////////////////////////////////////////////
// ȭ��ȯ
	SMenu[MN_CHANGE_MONEY].x=200+GABX_SCREEN;
	SMenu[MN_CHANGE_MONEY].y=75;//+GABY_SCREEN;
	SMenu[MN_CHANGE_MONEY].nImageNumber=13;
	SMenu[MN_CHANGE_MONEY].nImageType=SALVATION_PCX;
	SMenu[MN_CHANGE_MONEY].key=6;				// ���������� �� ���μ�
	SMenu[MN_CHANGE_MONEY].nFieldCount=11;

	// �߰� ������ ����ֱ�
	SMenu[MN_CHANGE_MONEY].nField[0].nType=FT_NOMAL_PUT;
	SMenu[MN_CHANGE_MONEY].nField[0].x=21;
	SMenu[MN_CHANGE_MONEY].nField[0].y=201;
	SMenu[MN_CHANGE_MONEY].nField[0].nImageNumber=17;
	SMenu[MN_CHANGE_MONEY].nField[0].nImageType=SALVATION_PCX;
	
	// �ؽ�Ʈ ���
	SMenu[MN_CHANGE_MONEY].nField[1].nType=FT_SCROLL_TEXT_PUT;
	SMenu[MN_CHANGE_MONEY].nField[1].x=35;
	SMenu[MN_CHANGE_MONEY].nField[1].y=69;
	SMenu[MN_CHANGE_MONEY].nField[1].nWillDo = RGB16( 255, 255, 255);
	SMenu[MN_CHANGE_MONEY].nField[1].nRectImage=159;
	strcpy(SMenu[MN_CHANGE_MONEY].nField[1].temp, lan->OutputMessage(3,202));//lsw
	SMenu[MN_CHANGE_MONEY].nField[1].m_lpTemp = SMenu[MN_CHANGE_MONEY].nField[1].temp;

	// �ؽ�Ʈ ��ũ��
	SMenu[MN_CHANGE_MONEY].nField[2].nType=FT_SCROLL_BUTTON;
	SMenu[MN_CHANGE_MONEY].nField[2].nWillDo = SCROLL_UP;
	SMenu[MN_CHANGE_MONEY].nField[2].x=207;
	SMenu[MN_CHANGE_MONEY].nField[2].y=65;
	SMenu[MN_CHANGE_MONEY].nField[2].nShideNomalPlus=2;		// ������ ��ư x, y ��ǥ gab
	SMenu[MN_CHANGE_MONEY].nField[2].nSHideNomalCount=2;		// ����
	SMenu[MN_CHANGE_MONEY].nField[2].nRectImage=1;				// ����
	SMenu[MN_CHANGE_MONEY].nField[2].nImageNumber=0;			// ���� ��ư
	SMenu[MN_CHANGE_MONEY].nField[2].nImageType=SALVATION_PCX;
	SetRect(SMenu[MN_CHANGE_MONEY].nField[2].rCheakBox,197,65,218,97);

	// �ؽ�Ʈ ��ũ��
	SMenu[MN_CHANGE_MONEY].nField[3].nType=FT_SCROLL_BUTTON;
	SMenu[MN_CHANGE_MONEY].nField[3].nWillDo = SCROLL_DOWN;
	SMenu[MN_CHANGE_MONEY].nField[3].x=207;
	SMenu[MN_CHANGE_MONEY].nField[3].y=140;
	SMenu[MN_CHANGE_MONEY].nField[3].nShideNomalPlus=2;		// ������ ��ư x, y ��ǥ gab
	SMenu[MN_CHANGE_MONEY].nField[3].nSHideNomalCount=5;		// ����
	SMenu[MN_CHANGE_MONEY].nField[3].nRectImage=4;				// ����
	SMenu[MN_CHANGE_MONEY].nField[3].nImageNumber=3;			// ���� ��ư
	SMenu[MN_CHANGE_MONEY].nField[3].nImageType=SALVATION_PCX;
	SetRect(SMenu[MN_CHANGE_MONEY].nField[3].rCheakBox,197,140,218,172);

	// OK
	SMenu[MN_CHANGE_MONEY].nField[4].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_CHANGE_MONEY].nField[4].nSpecialWillDo=SWD_CHANGE_MONEY_OK;
	SMenu[MN_CHANGE_MONEY].nField[4].x=32;
	SMenu[MN_CHANGE_MONEY].nField[4].y=289;
	SMenu[MN_CHANGE_MONEY].nField[4].nRectImage=15;
	SMenu[MN_CHANGE_MONEY].nField[4].nImageNumber=14;
	SMenu[MN_CHANGE_MONEY].nField[4].nImageType=SALVATION_PCX;
	SetRect(SMenu[MN_CHANGE_MONEY].nField[4].rCheakBox,32,289,98,310);

	// CANCEL
	SMenu[MN_CHANGE_MONEY].nField[5].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_CHANGE_MONEY].nField[5].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_CHANGE_MONEY].nField[5].x=150;
	SMenu[MN_CHANGE_MONEY].nField[5].y=289;
	SMenu[MN_CHANGE_MONEY].nField[5].nRectImage=12;
	SMenu[MN_CHANGE_MONEY].nField[5].nImageNumber=11;
	SMenu[MN_CHANGE_MONEY].nField[5].nImageType=SALVATION_PCX;
	SetRect(SMenu[MN_CHANGE_MONEY].nField[5].rCheakBox,150,289,216,310);

	// scroll butoon	// up
	SMenu[MN_CHANGE_MONEY].nField[6].nType=FT_DO;
	SMenu[MN_CHANGE_MONEY].nField[6].nWillDo = DO_BANK_SCROLL_UP;
//	SMenu[MN_CHANGE_MONEY].nField[6].nType=FT_SALVATION_MONEY_SCROLL;
//	SMenu[MN_CHANGE_MONEY].nField[6].nWillDo=SCROLL_UP;
	SMenu[MN_CHANGE_MONEY].nField[6].x=102-6;
	SMenu[MN_CHANGE_MONEY].nField[6].y=214-5;
	SMenu[MN_CHANGE_MONEY].nField[6].nRectImage=8;
	SMenu[MN_CHANGE_MONEY].nField[6].nImageNumber=7;
	SMenu[MN_CHANGE_MONEY].nField[6].nImageType=SALVATION_PCX;
	SetRect(SMenu[MN_CHANGE_MONEY].nField[6].rCheakBox,96+6,207+6,111+6,219+6);
						// down
	SMenu[MN_CHANGE_MONEY].nField[7].nType=FT_DO;
	SMenu[MN_CHANGE_MONEY].nField[7].nWillDo = DO_BANK_SCROLL_DOWN;
//	SMenu[MN_CHANGE_MONEY].nField[7].nType=FT_SALVATION_MONEY_SCROLL;
//	SMenu[MN_CHANGE_MONEY].nField[7].nWillDo=SCROLL_DOWN;
	SMenu[MN_CHANGE_MONEY].nField[7].x=102-6;
	SMenu[MN_CHANGE_MONEY].nField[7].y=228-5;
	SMenu[MN_CHANGE_MONEY].nField[7].nRectImage=10;
	SMenu[MN_CHANGE_MONEY].nField[7].nImageNumber=9;
	SMenu[MN_CHANGE_MONEY].nField[7].nImageType=SALVATION_PCX;
	SetRect(SMenu[MN_CHANGE_MONEY].nField[7].rCheakBox,96+6,221+6,111+6,234+6);

	// �ٲ� ��, �ٲ��� �� ���
	SMenu[MN_CHANGE_MONEY].nField[8].nType=FT_CHANGE_MONEY;			// �� �ٲٱ� ����
	SMenu[MN_CHANGE_MONEY].nField[8].x=113+89-2;
	SMenu[MN_CHANGE_MONEY].nField[8].y=215+1;

	SMenu[MN_CHANGE_MONEY].nField[9].x=113+89-2;
	SMenu[MN_CHANGE_MONEY].nField[9].y=250+1;
	
	// ���� ����ֱ�
	SMenu[MN_CHANGE_MONEY].nField[10].nType=FT_DATA_PUT;
	SMenu[MN_CHANGE_MONEY].nField[10].x=125;
	SMenu[MN_CHANGE_MONEY].nField[10].y=33;
	SMenu[MN_CHANGE_MONEY].nField[10].nImageNumber=DP_TITLE_PUT;
	SMenu[MN_CHANGE_MONEY].nField[10].nWillDo=RGB(255, 255, 255);
	strcpy(SMenu[MN_CHANGE_MONEY].nField[10].temp, lan->OutputMessage(3,203));//lsw

///////////////////////////////////////////////////////////////////////////////////////////
//  �κ� ����

	SMenu[MN_SMALL_TIPS].nImageNumber=0;
	SMenu[MN_SMALL_TIPS].nFieldCount=10;

	// view smalltips
	SMenu[MN_SMALL_TIPS].nField[0].nType=FT_SMALL_TIPS_BUTTON;
	SMenu[MN_SMALL_TIPS].nField[0].nWillDo=3;

	// end button
	SMenu[MN_SMALL_TIPS].nField[1].nType=FT_SMALL_TIPS_BUTTON;
	SMenu[MN_SMALL_TIPS].nField[1].nWillDo=0;
	SetRect(SMenu[MN_SMALL_TIPS].nField[1].rCheakBox,430, 0, 450, 20);
	strcpy(SMenu[MN_SMALL_TIPS].nField[1].temp, "x" );

	// <-- button
	SMenu[MN_SMALL_TIPS].nField[2].nType=FT_SMALL_TIPS_BUTTON;
	SMenu[MN_SMALL_TIPS].nField[2].nWillDo=1;
	SetRect(SMenu[MN_SMALL_TIPS].nField[2].rCheakBox,360,220,400,240);
	strcpy(SMenu[MN_SMALL_TIPS].nField[2].temp, "<<" );

	// --> button
	SMenu[MN_SMALL_TIPS].nField[3].nType=FT_SMALL_TIPS_BUTTON;
	SMenu[MN_SMALL_TIPS].nField[3].nWillDo=2;
	SetRect(SMenu[MN_SMALL_TIPS].nField[3].rCheakBox,410,220, 450,240);
	strcpy(SMenu[MN_SMALL_TIPS].nField[3].temp, ">>" );

////////////////////////////////////////////////////////////////////////////////////
// EFFECT TOOL
	SMenu[MN_EFFECT_TOOL].x=0;
	SMenu[MN_EFFECT_TOOL].y=0;
	SMenu[MN_EFFECT_TOOL].nImageNumber=0;
	SMenu[MN_EFFECT_TOOL].nFieldCount=2;

	// �޴� �����ϱ�
	SMenu[MN_EFFECT_TOOL].nField[0].nType=FT_VIEW_EFFECT_TOOL;
	SMenu[MN_EFFECT_TOOL].nField[0].nWillDo=0;
	SetRect(SMenu[MN_EFFECT_TOOL].nField[0].rCheakBox,0,0,160,40);

	// effect ���� ��ư
	SMenu[MN_EFFECT_TOOL].nField[1].nType=FT_VIEW_EFFECT_TOOL;
	SMenu[MN_EFFECT_TOOL].nField[1].x=0;
	SMenu[MN_EFFECT_TOOL].nField[1].y=0;
	SMenu[MN_EFFECT_TOOL].nField[1].nWillDo=1;
	SetRect(SMenu[MN_EFFECT_TOOL].nField[1].rCheakBox,180,10, 220,30);
	strcpy(SMenu[MN_EFFECT_TOOL].nField[1].temp, "Ȯ��" );



///////////////////////////////////////////////////////////////////////////////////////////
	SMenu[MN_LADDER_RANK].x=170;
	SMenu[MN_LADDER_RANK].y=20;
	SMenu[MN_LADDER_RANK].nImageNumber=25;
	SMenu[MN_LADDER_RANK].nImageType=GUILD_WAIT;
	SMenu[MN_LADDER_RANK].nFieldCount=6;

	// ok ��ư
	SMenu[MN_LADDER_RANK].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_LADDER_RANK].nField[0].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_LADDER_RANK].nField[0].x=193;
	SMenu[MN_LADDER_RANK].nField[0].y=366;
	SMenu[MN_LADDER_RANK].nField[0].nRectImage=533;
	SMenu[MN_LADDER_RANK].nField[0].nImageNumber=534;
	SetRect(SMenu[MN_LADDER_RANK].nField[0].rCheakBox,193,366,263,393);

	// view rank
	SMenu[MN_LADDER_RANK].nField[1].nType=FT_VIEW_LADDER_RANK;		// ���� �ʵ尪�� �����ϴ� ���� �ִ�.
	SMenu[MN_LADDER_RANK].nField[1].x=191;
	SMenu[MN_LADDER_RANK].nField[1].y=364;
	SMenu[MN_LADDER_RANK].nField[1].nSHideNomalCount=3;
	SMenu[MN_LADDER_RANK].nField[1].nSHideNomalStart=0;
	SMenu[MN_LADDER_RANK].nField[1].nShideNomalPlus=1;

	// up
	SMenu[MN_LADDER_RANK].nField[2].nType=FT_HIDE_DSCROLL_AUTO_PUT;
	SMenu[MN_LADDER_RANK].nField[2].nWillDo=1;
	SMenu[MN_LADDER_RANK].nField[2].x=84;
	SMenu[MN_LADDER_RANK].nField[2].y=336;
	SMenu[MN_LADDER_RANK].nField[2].nRectImage=673;
	SMenu[MN_LADDER_RANK].nField[2].nImageNumber=674;	
	SetRect(SMenu[MN_LADDER_RANK].nField[2].rCheakBox,84,336,116,356);

	// down
	SMenu[MN_LADDER_RANK].nField[3].nType=FT_HIDE_ISCROLL_AUTO_PUT;
	SMenu[MN_LADDER_RANK].nField[3].nWillDo=1;
	SMenu[MN_LADDER_RANK].nField[3].x=119;
	SMenu[MN_LADDER_RANK].nField[3].y=336;
	SMenu[MN_LADDER_RANK].nField[3].nRectImage=675;
	SMenu[MN_LADDER_RANK].nField[3].nImageNumber=676;
	SetRect(SMenu[MN_LADDER_RANK].nField[3].rCheakBox,119,336,151,356);

	// search ��ư
	SMenu[MN_LADDER_RANK].nField[4].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_LADDER_RANK].nField[4].nSpecialWillDo=SWD_SEARCH_LADDER_RANK;
	SMenu[MN_LADDER_RANK].nField[4].nWillDo=1;
	SMenu[MN_LADDER_RANK].nField[4].x=304;
	SMenu[MN_LADDER_RANK].nField[4].y=334;
	SMenu[MN_LADDER_RANK].nField[4].nRectImage=27;
	SMenu[MN_LADDER_RANK].nField[4].nImageNumber=26;
	SMenu[MN_LADDER_RANK].nField[4].nImageType =GUILD_WAIT;
	SetRect(SMenu[MN_LADDER_RANK].nField[4].rCheakBox,304,334,388,356);
	

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
// ���� �޴� ���� ( ���̼��� )
	SMenu[MN_NATION_MAIN_VA].x=CENTER_X;
	SMenu[MN_NATION_MAIN_VA].y=24;
	SMenu[MN_NATION_MAIN_VA].nImageNumber=24;
	SMenu[MN_NATION_MAIN_VA].nImageType=NATION_BBS_MENU;
	SMenu[MN_NATION_MAIN_VA].nFieldCount=6;

	// Exit Button 
	SMenu[MN_NATION_MAIN_VA].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_MAIN_VA].nField[0].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_NATION_MAIN_VA].nField[0].x=69;
	SMenu[MN_NATION_MAIN_VA].nField[0].y=246;
	SMenu[MN_NATION_MAIN_VA].nField[0].nRectImage=607;
	SMenu[MN_NATION_MAIN_VA].nField[0].nImageNumber=608;
	SetRect(SMenu[MN_NATION_MAIN_VA].nField[0].rCheakBox,69,246,141,273);

	// �㰡��
	SMenu[MN_NATION_MAIN_VA].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_MAIN_VA].nField[1].nSpecialWillDo=SWD_SMALL_MENU_WITH_SWD_TO_WORK;
	SMenu[MN_NATION_MAIN_VA].nField[1].nWillDo = MN_VISA_MENU;
	SMenu[MN_NATION_MAIN_VA].nField[1].nSHideNomalNumber = N_VYSEUS;
	SMenu[MN_NATION_MAIN_VA].nField[1].x=31;
	SMenu[MN_NATION_MAIN_VA].nField[1].y=109;
	SMenu[MN_NATION_MAIN_VA].nField[1].nRectImage=21;
	SMenu[MN_NATION_MAIN_VA].nField[1].nImageNumber=20;
	SMenu[MN_NATION_MAIN_VA].nField[1].nImageType=NATION_BBS_MENU;
	SetRect(SMenu[MN_NATION_MAIN_VA].nField[1].rCheakBox,31,109,180,131);

	// ����
	SMenu[MN_NATION_MAIN_VA].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_MAIN_VA].nField[2].nSpecialWillDo=SWD_SORRY;
	SMenu[MN_NATION_MAIN_VA].nField[2].x=31;
	SMenu[MN_NATION_MAIN_VA].nField[2].y=137;
	SMenu[MN_NATION_MAIN_VA].nField[2].nRectImage=16;
	SMenu[MN_NATION_MAIN_VA].nField[2].nImageNumber=15;
	SMenu[MN_NATION_MAIN_VA].nField[2].nImageType = NATION_BBS_MENU;
	SetRect(SMenu[MN_NATION_MAIN_VA].nField[2].rCheakBox,31,137,180,158);

	// �峳
	SMenu[MN_NATION_MAIN_VA].nField[3].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_MAIN_VA].nField[3].nSpecialWillDo = SWD_SORRY;
	SMenu[MN_NATION_MAIN_VA].nField[3].x=31;
	SMenu[MN_NATION_MAIN_VA].nField[3].y=165;
	SMenu[MN_NATION_MAIN_VA].nField[3].nRectImage=23;
	SMenu[MN_NATION_MAIN_VA].nField[3].nImageNumber=22;
	SMenu[MN_NATION_MAIN_VA].nField[3].nImageType = NATION_BBS_MENU;
	SetRect(SMenu[MN_NATION_MAIN_VA].nField[3].rCheakBox,31,165,180,187);

	// ��ȯ
	SMenu[MN_NATION_MAIN_VA].nField[4].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_MAIN_VA].nField[4].nSpecialWillDo = SWD_SORRY;
	SMenu[MN_NATION_MAIN_VA].nField[4].x=31;
	SMenu[MN_NATION_MAIN_VA].nField[4].y=192;
	SMenu[MN_NATION_MAIN_VA].nField[4].nRectImage=14;
	SMenu[MN_NATION_MAIN_VA].nField[4].nImageNumber=13;
	SMenu[MN_NATION_MAIN_VA].nField[4].nImageType = NATION_BBS_MENU;
	SetRect(SMenu[MN_NATION_MAIN_VA].nField[4].rCheakBox,31,193,180,215);

	// ���
	SMenu[MN_NATION_MAIN_VA].nField[5].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_MAIN_VA].nField[5].nSpecialWillDo=SWD_SORRY;
	SMenu[MN_NATION_MAIN_VA].nField[5].x=31;
	SMenu[MN_NATION_MAIN_VA].nField[5].y=219;
	SMenu[MN_NATION_MAIN_VA].nField[5].nRectImage=18;
	SMenu[MN_NATION_MAIN_VA].nField[5].nImageNumber=17;
	SMenu[MN_NATION_MAIN_VA].nField[5].nImageType = NATION_BBS_MENU;
	SetRect(SMenu[MN_NATION_MAIN_VA].nField[5].rCheakBox,31,219,180,241);

///////////////////////////////////////////////////////////////////////////////////////////
// ���� �޴� ���� ( ������ )
	memcpy( &SMenu[MN_NATION_MAIN_ZY], &SMenu[MN_NATION_MAIN_VA], sizeof( SMENU ) );
	SMenu[MN_NATION_MAIN_ZY].nField[1].nSHideNomalNumber = N_ZYPERN;

///////////////////////////////////////////////////////////////////////////////////////////
// ���� �޴� ���� ( �Ͻ� )
	memcpy( &SMenu[MN_NATION_MAIN_ILL], &SMenu[MN_NATION_MAIN_VA], sizeof( SMENU ) );
	SMenu[MN_NATION_MAIN_ILL].nField[1].nSHideNomalNumber = N_YILSE;

///////////////////////////////////////////////////////////////////////////////////////////
// �㰡�� ��� �޴�
	SMenu[MN_VISA_MENU].x=CENTER_X;
	SMenu[MN_VISA_MENU].y=74;
	SMenu[MN_VISA_MENU].nImageNumber=19;
	SMenu[MN_VISA_MENU].nImageType=NATION_BBS_MENU;
	SMenu[MN_VISA_MENU].nFieldCount=7;

	// ���� �����ֱ�
	SMenu[MN_VISA_MENU].nField[0].nType=FT_VISA;
	SMenu[MN_VISA_MENU].nField[0].x=24;
	SMenu[MN_VISA_MENU].nField[0].y=44;

	// OK Button 
	SMenu[MN_VISA_MENU].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_VISA_MENU].nField[1].nSpecialWillDo=SWD_BUY_VISA;		// ���� ���
	SMenu[MN_VISA_MENU].nField[1].x=21;
	SMenu[MN_VISA_MENU].nField[1].y=210;
	SMenu[MN_VISA_MENU].nField[1].nRectImage=677;
	SMenu[MN_VISA_MENU].nField[1].nImageNumber=678;
	SetRect(SMenu[MN_VISA_MENU].nField[1].rCheakBox,22,210,56,244);

	// cancel
	SMenu[MN_VISA_MENU].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_VISA_MENU].nField[2].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_VISA_MENU].nField[2].x=160;
	SMenu[MN_VISA_MENU].nField[2].y=210;
	SMenu[MN_VISA_MENU].nField[2].nRectImage=679;
	SMenu[MN_VISA_MENU].nField[2].nImageNumber=680;
	SetRect(SMenu[MN_VISA_MENU].nField[2].rCheakBox,161,210,195,244);

	// 0
	SMenu[MN_VISA_MENU].nField[3].nType=FT_SELECT_VISA;
	SMenu[MN_VISA_MENU].nField[3].nWillDo=0;
	SetRect(SMenu[MN_VISA_MENU].nField[3].rCheakBox,22,43,53,73);

	// 1
	SMenu[MN_VISA_MENU].nField[4].nType=FT_SELECT_VISA;
	SMenu[MN_VISA_MENU].nField[4].nWillDo=1;
	SetRect(SMenu[MN_VISA_MENU].nField[4].rCheakBox,22,85,53,115);

	// 2
	SMenu[MN_VISA_MENU].nField[5].nType=FT_SELECT_VISA;
	SMenu[MN_VISA_MENU].nField[5].nWillDo=2;
	SetRect(SMenu[MN_VISA_MENU].nField[5].rCheakBox,22,126,53,156);

	// 3
	SMenu[MN_VISA_MENU].nField[6].nType=FT_SELECT_VISA;
	SMenu[MN_VISA_MENU].nField[6].nWillDo=3;
	SetRect(SMenu[MN_VISA_MENU].nField[6].rCheakBox,22,168,53,198);

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
// ������ �޴� 
	SMenu[MN_SCALE].x=-1;
	SMenu[MN_SCALE].y=150;
	SMenu[MN_SCALE].nImageNumber=0;
	SMenu[MN_SCALE].nImageType=MAIN_ETC3;
	SMenu[MN_SCALE].key=7;				// ���������� �� ���μ�
	SMenu[MN_SCALE].nFieldCount=6;
	
	//OK
	SMenu[MN_SCALE].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_SCALE].nField[0].nSpecialWillDo=SWD_DEFAULT_MESSAGEBOX_OK;
	SMenu[MN_SCALE].nField[0].x=121;
	SMenu[MN_SCALE].nField[0].y=194;
	SMenu[MN_SCALE].nField[0].nRectImage=819;
	SMenu[MN_SCALE].nField[0].nImageNumber=820;
	SetRect(SMenu[MN_SCALE].nField[0].rCheakBox,119,193,187,214);

	// �ؽ�Ʈ ���
	SMenu[MN_SCALE].nField[1].nType=FT_SCROLL_TEXT_PUT;
	SMenu[MN_SCALE].nField[1].x=35;
	SMenu[MN_SCALE].nField[1].y=40;
	SMenu[MN_SCALE].nField[1].nWillDo = RGB16( 255, 255, 255);
	SMenu[MN_SCALE].nField[1].nSpecialWillDo = 1;		// CheakType �� 1�� �ٲ� �ٽô� �ʱ�ȭ ��ƾ���� �ȵ�� ���� �Ѵ�.
	SMenu[MN_SCALE].nField[1].nRectImage=210;
//	SMenu[MN_SCALE].nField[1].m_lpTemp = g_szScale; // LoadDefaultStringByMenu() ���� ó��

	// �ؽ�Ʈ ��ũ��
	SMenu[MN_SCALE].nField[2].nType=FT_SCROLL_BUTTON;
	SMenu[MN_SCALE].nField[2].nWillDo = SCROLL_UP;
	SMenu[MN_SCALE].nField[2].x=257;
	SMenu[MN_SCALE].nField[2].y=45;
	SMenu[MN_SCALE].nField[2].nShideNomalPlus=2;		// ������ ��ư x, y ��ǥ gab
	SMenu[MN_SCALE].nField[2].nSHideNomalCount=2;		// ����
	SMenu[MN_SCALE].nField[2].nRectImage=1;				// ����
	SMenu[MN_SCALE].nField[2].nImageNumber=0;			// ���� ��ư
	SMenu[MN_SCALE].nField[2].nImageType=SALVATION_PCX;
	SetRect(SMenu[MN_SCALE].nField[2].rCheakBox,257,45,287,77);

	// �ؽ�Ʈ ��ũ��
	SMenu[MN_SCALE].nField[3].nType=FT_SCROLL_BUTTON;
	SMenu[MN_SCALE].nField[3].nWillDo = SCROLL_DOWN;
	SMenu[MN_SCALE].nField[3].x=257;
	SMenu[MN_SCALE].nField[3].y=140;
	SMenu[MN_SCALE].nField[3].nShideNomalPlus=2;		// ������ ��ư x, y ��ǥ gab
	SMenu[MN_SCALE].nField[3].nSHideNomalCount=5;		// ����
	SMenu[MN_SCALE].nField[3].nRectImage=4;				// ����
	SMenu[MN_SCALE].nField[3].nImageNumber=3;			// ���� ��ư
	SMenu[MN_SCALE].nField[3].nImageType=SALVATION_PCX;
	SetRect(SMenu[MN_SCALE].nField[3].rCheakBox,257,140,287,172);

	// �����
	SMenu[MN_SCALE].nField[4].nType=FT_NOMAL_PUT;
	SMenu[MN_SCALE].nField[4].x=0;
	SMenu[MN_SCALE].nField[4].y=-50;
	SMenu[MN_SCALE].nField[4].nImageNumber=1;
	SMenu[MN_SCALE].nField[4].nImageType=MAIN_ETC3;

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
// ��� �޴� 
	SMenu[MN_GUILD_MAIN].x=CENTER_X;
	SMenu[MN_GUILD_MAIN].y=75;
	SMenu[MN_GUILD_MAIN].nImageNumber=0;
	SMenu[MN_GUILD_MAIN].nImageType=GUILD_MENU_IMG;
	SMenu[MN_GUILD_MAIN].nFieldCount=6;

	// Exit Button 
	SMenu[MN_GUILD_MAIN].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GUILD_MAIN].nField[0].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_GUILD_MAIN].nField[0].x=62;
	SMenu[MN_GUILD_MAIN].nField[0].y=248;
	SMenu[MN_GUILD_MAIN].nField[0].nRectImage=12;
	SMenu[MN_GUILD_MAIN].nField[0].nImageNumber=11;
	SMenu[MN_GUILD_MAIN].nField[0].nImageType=GUILD_MENU_IMG;
	SetRect(SMenu[MN_GUILD_MAIN].nField[0].rCheakBox,62,248,144,271);

	// ��� �Ұ�
	SMenu[MN_GUILD_MAIN].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GUILD_MAIN].nField[1].nSpecialWillDo=SWD_CALLMENU;//021007 lsw 
	SMenu[MN_GUILD_MAIN].nField[1].nWillDo = MN_GUILD_INTRODUCTION;
	SMenu[MN_GUILD_MAIN].nField[1].x=40;
	SMenu[MN_GUILD_MAIN].nField[1].y=81;
	SMenu[MN_GUILD_MAIN].nField[1].nRectImage=2;
	SMenu[MN_GUILD_MAIN].nField[1].nImageNumber=1;
	SMenu[MN_GUILD_MAIN].nField[1].nImageType=GUILD_MENU_IMG;
	SetRect(SMenu[MN_GUILD_MAIN].nField[1].rCheakBox,40,81,170,104);

	// ��� ��û ���
	SMenu[MN_GUILD_MAIN].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GUILD_MAIN].nField[2].nSpecialWillDo=SWD_CALL_GUILD_EXPLAIN;
	SMenu[MN_GUILD_MAIN].nField[2].nWillDo=GE_REGIST_GUILD_MENUAL;
	SMenu[MN_GUILD_MAIN].nField[2].x=40;
	SMenu[MN_GUILD_MAIN].nField[2].y=113;
	SMenu[MN_GUILD_MAIN].nField[2].nRectImage=4;
	SMenu[MN_GUILD_MAIN].nField[2].nImageNumber=3;
	SMenu[MN_GUILD_MAIN].nField[2].nImageType=GUILD_MENU_IMG;
	SetRect(SMenu[MN_GUILD_MAIN].nField[2].rCheakBox,40,113,169,136);

	// ���� ��� ��û
	SMenu[MN_GUILD_MAIN].nField[3].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GUILD_MAIN].nField[3].nSpecialWillDo = SWD_CALL_GUILD_EXPLAIN;
	SMenu[MN_GUILD_MAIN].nField[3].nWillDo = GE_REGIST_IMSI_GUILD;
	SMenu[MN_GUILD_MAIN].nField[3].x=40;
	SMenu[MN_GUILD_MAIN].nField[3].y=145;
	SMenu[MN_GUILD_MAIN].nField[3].nRectImage=8;
	SMenu[MN_GUILD_MAIN].nField[3].nImageNumber=7;
	SMenu[MN_GUILD_MAIN].nField[3].nImageType=GUILD_MENU_IMG;
	SetRect(SMenu[MN_GUILD_MAIN].nField[3].rCheakBox,40,145,170,168);

	// ���� ��� ��û
	SMenu[MN_GUILD_MAIN].nField[4].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GUILD_MAIN].nField[4].nSpecialWillDo=SWD_CALL_GUILD_EXPLAIN;
	SMenu[MN_GUILD_MAIN].nField[4].nWillDo=GE_REGIST_GUILD;
	SMenu[MN_GUILD_MAIN].nField[4].x=40;
	SMenu[MN_GUILD_MAIN].nField[4].y=178;
	SMenu[MN_GUILD_MAIN].nField[4].nRectImage=10;
	SMenu[MN_GUILD_MAIN].nField[4].nImageNumber=9;
	SMenu[MN_GUILD_MAIN].nField[4].nImageType=GUILD_MENU_IMG;
	SetRect(SMenu[MN_GUILD_MAIN].nField[4].rCheakBox,40,178,170,201);

	// ��� ���� ����
	SMenu[MN_GUILD_MAIN].nField[5].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GUILD_MAIN].nField[5].nSpecialWillDo=SWD_CALL_GUILD_CHANGE;
	SMenu[MN_GUILD_MAIN].nField[5].x=40;
	SMenu[MN_GUILD_MAIN].nField[5].y=211;
	SMenu[MN_GUILD_MAIN].nField[5].nRectImage=6;
	SMenu[MN_GUILD_MAIN].nField[5].nImageNumber=5;
	SMenu[MN_GUILD_MAIN].nField[5].nImageType=GUILD_MENU_IMG;
	SetRect(SMenu[MN_GUILD_MAIN].nField[5].rCheakBox,40,211,170,234);

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
// ��� ���� �޴� (  ������ ���� �Ŵ��� ǥ���Ѵ�. )
	SMenu[MN_GUILD_EXPLAIN].x=CENTER_X;
	SMenu[MN_GUILD_EXPLAIN].y=75;
	SMenu[MN_GUILD_EXPLAIN].nImageNumber=32;
	SMenu[MN_GUILD_EXPLAIN].nImageType=GUILD_MENU_IMG;
	SMenu[MN_GUILD_EXPLAIN].nFieldCount=11;
	SMenu[MN_GUILD_EXPLAIN].key2=5;		// �ִ� �ټ� 

	// ���� ���
	SMenu[MN_GUILD_EXPLAIN].nField[0].nType=FT_NOMAL_PUT_CENTER;
	SMenu[MN_GUILD_EXPLAIN].nField[0].x=69;
	SMenu[MN_GUILD_EXPLAIN].nField[0].y=28;
	//SMenu[MN_GUILD_EXPLAIN].nField[0].nImageNumber=;		// �� ��Ȳ�� ���缭
	SMenu[MN_GUILD_EXPLAIN].nField[0].nImageType=GUILD_MENU_IMG;		// �� ��Ȳ�� ���缭

	// YES ��ư ���
	SMenu[MN_GUILD_EXPLAIN].nField[1].nType=FT_NOMAL_PUT;
	SMenu[MN_GUILD_EXPLAIN].nField[1].x=26;
	SMenu[MN_GUILD_EXPLAIN].nField[1].y=231;
	SMenu[MN_GUILD_EXPLAIN].nField[1].nImageNumber=36;
	SMenu[MN_GUILD_EXPLAIN].nField[1].nImageType=GUILD_MENU_IMG;

	// no ��ư ���
	SMenu[MN_GUILD_EXPLAIN].nField[2].nType=FT_NOMAL_PUT;
	SMenu[MN_GUILD_EXPLAIN].nField[2].x=260;
	SMenu[MN_GUILD_EXPLAIN].nField[2].y=231;
	SMenu[MN_GUILD_EXPLAIN].nField[2].nImageNumber=33;
	SMenu[MN_GUILD_EXPLAIN].nField[2].nImageType=GUILD_MENU_IMG;

	// done ��ư ���
	SMenu[MN_GUILD_EXPLAIN].nField[3].nType=FT_NOMAL_PUT;
	SMenu[MN_GUILD_EXPLAIN].nField[3].x=141;
	SMenu[MN_GUILD_EXPLAIN].nField[3].y=231;
	SMenu[MN_GUILD_EXPLAIN].nField[3].nImageNumber=29;
	SMenu[MN_GUILD_EXPLAIN].nField[3].nImageType=GUILD_MENU_IMG;

	// YES Button 
	SMenu[MN_GUILD_EXPLAIN].nField[4].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GUILD_EXPLAIN].nField[4].nSpecialWillDo=SWD_GUILD_EXPLAIN_YES;
	SMenu[MN_GUILD_EXPLAIN].nField[4].x=26;
	SMenu[MN_GUILD_EXPLAIN].nField[4].y=231;
	SMenu[MN_GUILD_EXPLAIN].nField[4].nRectImage=38;
	SMenu[MN_GUILD_EXPLAIN].nField[4].nImageNumber=37;
	SMenu[MN_GUILD_EXPLAIN].nField[4].nImageType=GUILD_MENU_IMG;
	SetRect(SMenu[MN_GUILD_EXPLAIN].nField[4].rCheakBox,26,231,100,255);

	// NO Button 
	SMenu[MN_GUILD_EXPLAIN].nField[5].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GUILD_EXPLAIN].nField[5].nSpecialWillDo=SWD_SUBMENU_BACK;//021007 lsw
	SMenu[MN_GUILD_EXPLAIN].nField[5].x=260;
	SMenu[MN_GUILD_EXPLAIN].nField[5].y=231;
	SMenu[MN_GUILD_EXPLAIN].nField[5].nRectImage=35;
	SMenu[MN_GUILD_EXPLAIN].nField[5].nImageNumber=34;
	SMenu[MN_GUILD_EXPLAIN].nField[5].nImageType=GUILD_MENU_IMG;
	SetRect(SMenu[MN_GUILD_EXPLAIN].nField[5].rCheakBox,260,231,313,255);

	// Done Button 
	SMenu[MN_GUILD_EXPLAIN].nField[6].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GUILD_EXPLAIN].nField[6].nSpecialWillDo=SWD_SUBMENU_BACK;//021007 lsw
	SMenu[MN_GUILD_EXPLAIN].nField[6].x=141;
	SMenu[MN_GUILD_EXPLAIN].nField[6].y=231;
	SMenu[MN_GUILD_EXPLAIN].nField[6].nRectImage=31;
	SMenu[MN_GUILD_EXPLAIN].nField[6].nImageNumber=30;
	SMenu[MN_GUILD_EXPLAIN].nField[6].nImageType=GUILD_MENU_IMG;
	SetRect(SMenu[MN_GUILD_EXPLAIN].nField[6].rCheakBox,141,231,215,255);

	// �ؽ�Ʈ ��ũ��
	SMenu[MN_GUILD_EXPLAIN].nField[7].nType=FT_SCROLL_BUTTON;
	SMenu[MN_GUILD_EXPLAIN].nField[7].nWillDo = SCROLL_UP;
	SMenu[MN_GUILD_EXPLAIN].nField[7].x=315;
	SMenu[MN_GUILD_EXPLAIN].nField[7].y=78;
	SMenu[MN_GUILD_EXPLAIN].nField[7].nShideNomalPlus=2;		// ������ ��ư x, y ��ǥ gab
	SMenu[MN_GUILD_EXPLAIN].nField[7].nSHideNomalCount=2;		// ����
	SMenu[MN_GUILD_EXPLAIN].nField[7].nRectImage=1;				// ����
	SMenu[MN_GUILD_EXPLAIN].nField[7].nImageNumber=0;			// ���� ��ư
	SMenu[MN_GUILD_EXPLAIN].nField[7].nImageType=SALVATION_PCX;
	SetRect(SMenu[MN_GUILD_EXPLAIN].nField[7].rCheakBox,315,78,333,108);

	// �ؽ�Ʈ ��ũ��
	SMenu[MN_GUILD_EXPLAIN].nField[8].nType=FT_SCROLL_BUTTON;
	SMenu[MN_GUILD_EXPLAIN].nField[8].nWillDo = SCROLL_DOWN;
	SMenu[MN_GUILD_EXPLAIN].nField[8].x=315;
	SMenu[MN_GUILD_EXPLAIN].nField[8].y=180;
	SMenu[MN_GUILD_EXPLAIN].nField[8].nShideNomalPlus=2;		// ������ ��ư x, y ��ǥ gab
	SMenu[MN_GUILD_EXPLAIN].nField[8].nSHideNomalCount=5;		// ����
	SMenu[MN_GUILD_EXPLAIN].nField[8].nRectImage=4;				// ����
	SMenu[MN_GUILD_EXPLAIN].nField[8].nImageNumber=3;			// ���� ��ư
	SMenu[MN_GUILD_EXPLAIN].nField[8].nImageType=SALVATION_PCX;
	SetRect(SMenu[MN_GUILD_EXPLAIN].nField[8].rCheakBox,315,180,333,211);

	// ���� ���
	SMenu[MN_GUILD_EXPLAIN].nField[9].nType=FT_VIEW_TEXT_BY_HTML;
	SMenu[MN_GUILD_EXPLAIN].nField[9].x=40;
	SMenu[MN_GUILD_EXPLAIN].nField[9].y=102;

	SMenu[MN_GUILD_EXPLAIN].nField[10].nType=FT_INPUT_GUILD_MASTER;
	SMenu[MN_GUILD_EXPLAIN].nField[10].x=40;
	SMenu[MN_GUILD_EXPLAIN].nField[10].y=184;
	SMenu[MN_GUILD_EXPLAIN].nField[10].nImageNumber=47;
	SMenu[MN_GUILD_EXPLAIN].nField[10].nImageType=GUILD_MENU_IMG;
	SetRect(SMenu[MN_GUILD_EXPLAIN].nField[10].rCheakBox,167,182,303,201);



	
///////////////////////////////////////////////////////////////////////////////////////////	
///////////////////////////////////////////////////////////////////////////////////////////	
// ���� ��� ��� �޴�
	SMenu[MN_GUILD_REGIST_IMSI].x=CENTER_X;
	SMenu[MN_GUILD_REGIST_IMSI].y= 45;
	SMenu[MN_GUILD_REGIST_IMSI].nFieldCount = 9;
	SMenu[MN_GUILD_REGIST_IMSI].nImageNumber = 51;		// ����
	SMenu[MN_GUILD_REGIST_IMSI].nImageType = GUILD_MENU_IMG;

	// OK ��ư ������ ( ������ ������. )
	SMenu[MN_GUILD_REGIST_IMSI].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GUILD_REGIST_IMSI].nField[0].nSpecialWillDo=SWD_REGIST_GUILD_IMSI_SEND;
	SMenu[MN_GUILD_REGIST_IMSI].nField[0].x=27;
	SMenu[MN_GUILD_REGIST_IMSI].nField[0].y=311;
	SMenu[MN_GUILD_REGIST_IMSI].nField[0].nRectImage=53;
	SMenu[MN_GUILD_REGIST_IMSI].nField[0].nImageNumber=52;
	SMenu[MN_GUILD_REGIST_IMSI].nField[0].nImageType=GUILD_MENU_IMG;
	SetRect(SMenu[MN_GUILD_REGIST_IMSI].nField[0].rCheakBox,26,311,134,336);

	// CANCEL ��ư ������
	SMenu[MN_GUILD_REGIST_IMSI].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GUILD_REGIST_IMSI].nField[1].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_GUILD_REGIST_IMSI].nField[1].x=209;
	SMenu[MN_GUILD_REGIST_IMSI].nField[1].y=311;
	SMenu[MN_GUILD_REGIST_IMSI].nField[1].nRectImage=50;
	SMenu[MN_GUILD_REGIST_IMSI].nField[1].nImageNumber=49;
	SMenu[MN_GUILD_REGIST_IMSI].nField[1].nImageType=GUILD_MENU_IMG;
	SetRect(SMenu[MN_GUILD_REGIST_IMSI].nField[1].rCheakBox,208,312,316,337);

	// ���� �Է�
	SMenu[MN_GUILD_REGIST_IMSI].nField[2].nType= FT_MAIL_WRITE;
	SMenu[MN_GUILD_REGIST_IMSI].nField[2].nWillDo = 1;
	SMenu[MN_GUILD_REGIST_IMSI].nField[2].x=116;
	SMenu[MN_GUILD_REGIST_IMSI].nField[2].y=26;
	SMenu[MN_GUILD_REGIST_IMSI].nField[2].nRectImage=180;
	SetRect(SMenu[MN_GUILD_REGIST_IMSI].nField[2].rCheakBox,116,26,251,38);

	// ��� ������ �̸��ֱ�
	SMenu[MN_GUILD_REGIST_IMSI].nField[3].nType= FT_DATA_PUT;
	SMenu[MN_GUILD_REGIST_IMSI].nField[3].x=118;
	SMenu[MN_GUILD_REGIST_IMSI].nField[3].y=59;
	SMenu[MN_GUILD_REGIST_IMSI].nField[3].nImageNumber=DP_NAME;

	// �ӽ� ��ũ ��� �ֱ� ( ����� �ؽ�Ʈ ��� )
	SMenu[MN_GUILD_REGIST_IMSI].nField[4].nType= FT_DATA_PUT;
	SMenu[MN_GUILD_REGIST_IMSI].nField[4].x=293;
	SMenu[MN_GUILD_REGIST_IMSI].nField[4].y=41;
	SMenu[MN_GUILD_REGIST_IMSI].nField[4].nWillDo=RGB(255, 255, 255);
	SMenu[MN_GUILD_REGIST_IMSI].nField[4].nImageNumber=DP_TITLE_PUT;
	strcpy( SMenu[MN_GUILD_REGIST_IMSI].nField[4].temp, "GUILD" );

	// ��� �Ұ� ����
	SMenu[MN_GUILD_REGIST_IMSI].nField[5].nType= FT_MAIL_WRITE;	
	SMenu[MN_GUILD_REGIST_IMSI].nField[5].nWillDo = 3;
	SMenu[MN_GUILD_REGIST_IMSI].nField[5].x=30;
	SMenu[MN_GUILD_REGIST_IMSI].nField[5].y=137;
	SMenu[MN_GUILD_REGIST_IMSI].nField[5].nRectImage=268;
	SetRect(SMenu[MN_GUILD_REGIST_IMSI].nField[5].rCheakBox,30,137,300,300);



/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
// ��å ��Ϲ� ���� �޴�
	SMenu[MN_GUILD_REGIST_DEGREE].x=CENTER_X;
	SMenu[MN_GUILD_REGIST_DEGREE].y= 45;
	SMenu[MN_GUILD_REGIST_DEGREE].nFieldCount = 9;
	SMenu[MN_GUILD_REGIST_DEGREE].nImageNumber = 56;		// ����
	SMenu[MN_GUILD_REGIST_DEGREE].nImageType = GUILD_MENU_IMG;

	// 
	SMenu[MN_GUILD_REGIST_DEGREE].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GUILD_REGIST_DEGREE].nField[0].nSpecialWillDo=SWD_GUILD_DEGREE_OK;
	SMenu[MN_GUILD_REGIST_DEGREE].nField[0].nWillDo=GE_REGIST_GUILD_MARK;
	SMenu[MN_GUILD_REGIST_DEGREE].nField[0].x=35;
	SMenu[MN_GUILD_REGIST_DEGREE].nField[0].y=231;
	SMenu[MN_GUILD_REGIST_DEGREE].nField[0].nRectImage=58;
	SMenu[MN_GUILD_REGIST_DEGREE].nField[0].nImageNumber=57;
	SMenu[MN_GUILD_REGIST_DEGREE].nField[0].nImageType=GUILD_MENU_IMG;
	SetRect(SMenu[MN_GUILD_REGIST_DEGREE].nField[0].rCheakBox,35,231,117,263);

	// CANCEL ��ư ������
	SMenu[MN_GUILD_REGIST_DEGREE].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GUILD_REGIST_DEGREE].nField[1].nSpecialWillDo=SWD_SUBMENU_BACK;//021007 lsw
	SMenu[MN_GUILD_REGIST_DEGREE].nField[1].x=153;
	SMenu[MN_GUILD_REGIST_DEGREE].nField[1].y=231;
	SMenu[MN_GUILD_REGIST_DEGREE].nField[1].nRectImage=55;
	SMenu[MN_GUILD_REGIST_DEGREE].nField[1].nImageNumber=54;
	SMenu[MN_GUILD_REGIST_DEGREE].nField[1].nImageType=GUILD_MENU_IMG;
	SetRect(SMenu[MN_GUILD_REGIST_DEGREE].nField[1].rCheakBox,153,231,235,263);

	// �����å1 �Է�
	SMenu[MN_GUILD_REGIST_DEGREE].nField[2].nType= FT_GUILD_DEGREE;
	SMenu[MN_GUILD_REGIST_DEGREE].nField[2].nWillDo = 0;
	SMenu[MN_GUILD_REGIST_DEGREE].nField[2].x=116;
	SMenu[MN_GUILD_REGIST_DEGREE].nField[2].y=70;
	SMenu[MN_GUILD_REGIST_DEGREE].nField[2].nRectImage=180;
	SetRect(SMenu[MN_GUILD_REGIST_DEGREE].nField[2].rCheakBox,114,68,249,82);

	// �����å2 �Է�
	SMenu[MN_GUILD_REGIST_DEGREE].nField[3].nType= FT_GUILD_DEGREE;
	SMenu[MN_GUILD_REGIST_DEGREE].nField[3].nWillDo = 1;
	SMenu[MN_GUILD_REGIST_DEGREE].nField[3].x=116;
	SMenu[MN_GUILD_REGIST_DEGREE].nField[3].y=102;
	SMenu[MN_GUILD_REGIST_DEGREE].nField[3].nRectImage=180;
	SetRect(SMenu[MN_GUILD_REGIST_DEGREE].nField[3].rCheakBox,114,100,249,114);

	// �����å3 �Է�
	SMenu[MN_GUILD_REGIST_DEGREE].nField[4].nType= FT_GUILD_DEGREE;
	SMenu[MN_GUILD_REGIST_DEGREE].nField[4].nWillDo = 2;
	SMenu[MN_GUILD_REGIST_DEGREE].nField[4].x=116;
	SMenu[MN_GUILD_REGIST_DEGREE].nField[4].y=134;
	SMenu[MN_GUILD_REGIST_DEGREE].nField[4].nRectImage=180;
	SetRect(SMenu[MN_GUILD_REGIST_DEGREE].nField[4].rCheakBox,114,132,249,146);

	// �����å4 �Է�
	SMenu[MN_GUILD_REGIST_DEGREE].nField[5].nType= FT_GUILD_DEGREE;
	SMenu[MN_GUILD_REGIST_DEGREE].nField[5].nWillDo = 3;
	SMenu[MN_GUILD_REGIST_DEGREE].nField[5].x=116;
	SMenu[MN_GUILD_REGIST_DEGREE].nField[5].y=166;
	SMenu[MN_GUILD_REGIST_DEGREE].nField[5].nRectImage=180;
	SetRect(SMenu[MN_GUILD_REGIST_DEGREE].nField[5].rCheakBox,114,164,249,178);

	// �����å5 �Է�
	SMenu[MN_GUILD_REGIST_DEGREE].nField[6].nType= FT_GUILD_DEGREE;
	SMenu[MN_GUILD_REGIST_DEGREE].nField[6].nWillDo = 4;
	SMenu[MN_GUILD_REGIST_DEGREE].nField[6].x=116;
	SMenu[MN_GUILD_REGIST_DEGREE].nField[6].y=198;
	SMenu[MN_GUILD_REGIST_DEGREE].nField[6].nRectImage=180;
	SetRect(SMenu[MN_GUILD_REGIST_DEGREE].nField[6].rCheakBox,114,196,249,210);

/////////////////////////////////////////////////////////////////////////////////////////////////////	
/////////////////////////////////////////////////////////////////////////////////////////////////////
// ���� ��� ���		// �ʵ带 �ٲܷ��� CallGuildInfoMenu() �� �ٲ�� �Ѵ�.
	SMenu[MN_GUILD_REGIST].x=CENTER_X;
	SMenu[MN_GUILD_REGIST].y= 45;
	SMenu[MN_GUILD_REGIST].nFieldCount = 9;
	SMenu[MN_GUILD_REGIST].nImageNumber = 51;		// ����
	SMenu[MN_GUILD_REGIST].nImageType = GUILD_MENU_IMG;

	// OK ��ư ������ ( ������ ������. )
	SMenu[MN_GUILD_REGIST].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GUILD_REGIST].nField[0].nSpecialWillDo=SWD_REGIST_GUILD_SEND;
	SMenu[MN_GUILD_REGIST].nField[0].x=27;
	SMenu[MN_GUILD_REGIST].nField[0].y=311;
	SMenu[MN_GUILD_REGIST].nField[0].nRectImage=53;
	SMenu[MN_GUILD_REGIST].nField[0].nImageNumber=52;
	SMenu[MN_GUILD_REGIST].nField[0].nImageType=GUILD_MENU_IMG;
	SetRect(SMenu[MN_GUILD_REGIST].nField[0].rCheakBox,26,311,134,336);

	// CANCEL ��ư ������
	SMenu[MN_GUILD_REGIST].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GUILD_REGIST].nField[1].nSpecialWillDo=SWD_CALL_OKCANCEL_BOX;
	SMenu[MN_GUILD_REGIST].nField[1].x=209;
	SMenu[MN_GUILD_REGIST].nField[1].y=311;
	SMenu[MN_GUILD_REGIST].nField[1].nRectImage=50;
	SMenu[MN_GUILD_REGIST].nField[1].nImageNumber=49;
	SMenu[MN_GUILD_REGIST].nField[1].nImageType=GUILD_MENU_IMG;
	SetRect(SMenu[MN_GUILD_REGIST].nField[1].rCheakBox,208,312,316,337);
	strcpy( SMenu[MN_GUILD_REGIST].nField[1].temp, kein_GetMenuString( 71 ) );

	// ���� ���
	SMenu[MN_GUILD_REGIST].nField[2].nType= FT_DATA_PUT;
	SMenu[MN_GUILD_REGIST].nField[2].x=116;
	SMenu[MN_GUILD_REGIST].nField[2].y=27;
	SMenu[MN_GUILD_REGIST].nField[2].nRectImage=180;
	SMenu[MN_GUILD_REGIST].nField[2].nImageNumber=DP_DATAPUT_DEFAULT_STRING;

	// ��� ������ �̸��ֱ�
	SMenu[MN_GUILD_REGIST].nField[3].nType= FT_DATA_PUT;
	SMenu[MN_GUILD_REGIST].nField[3].x=118;
	SMenu[MN_GUILD_REGIST].nField[3].y=59;
	SMenu[MN_GUILD_REGIST].nField[2].nRectImage=180;
	SMenu[MN_GUILD_REGIST].nField[3].nImageNumber=DP_DATAPUT_DEFAULT_STRING;

	// ��� ��ũ ��� �ֱ�
	SMenu[MN_GUILD_REGIST].nField[4].nType= FT_NORMAL_PUT_CENTER2;
	SMenu[MN_GUILD_REGIST].nField[4].x=295;
	SMenu[MN_GUILD_REGIST].nField[4].y=47;

	// ��� �Ұ� ���
	SMenu[MN_GUILD_REGIST].nField[5].nType= FT_DATA_PUT;	
	SMenu[MN_GUILD_REGIST].nField[5].x=30;
	SMenu[MN_GUILD_REGIST].nField[5].y=137;
	SMenu[MN_GUILD_REGIST].nField[5].nRectImage=180;
	SMenu[MN_GUILD_REGIST].nField[5].nImageNumber=DP_DATAPUT_DEFAULT_STRING;
	SetRect(SMenu[MN_GUILD_REGIST].nField[5].rCheakBox,30,137,300,300);

////////////////////////////////////////////////////////////////////////////////////////////////
// ��� �Ұ� �޴� ( key ������ ���� �Ŵ��� ǥ���Ѵ�. )
	SMenu[MN_GUILD_INTRODUCTION].x=CENTER_X;
	SMenu[MN_GUILD_INTRODUCTION].y=75;
	SMenu[MN_GUILD_INTRODUCTION].nImageNumber=32;
	SMenu[MN_GUILD_INTRODUCTION].nImageType=GUILD_MENU_IMG;
	SMenu[MN_GUILD_INTRODUCTION].nFieldCount=10;

	// ���� ���
	SMenu[MN_GUILD_INTRODUCTION].nField[0].nType=FT_GUILD_INTRODUCTION;
	SMenu[MN_GUILD_INTRODUCTION].nField[0].x=36;
	SMenu[MN_GUILD_INTRODUCTION].nField[0].y=90;
	SMenu[MN_GUILD_INTRODUCTION].nField[0].nRectImage=270;
	SetRect(SMenu[MN_GUILD_INTRODUCTION].nField[0].rCheakBox,36,84,311,205);

	// ���� ���
	SMenu[MN_GUILD_INTRODUCTION].nField[1].nType=FT_NOMAL_PUT_CENTER;
	SMenu[MN_GUILD_INTRODUCTION].nField[1].x=69;
	SMenu[MN_GUILD_INTRODUCTION].nField[1].y=28;
	//SMenu[MN_GUILD_INTRODUCTION].nField[1].nImageNumber=;		// �� ���� ���缭
	SMenu[MN_GUILD_INTRODUCTION].nField[1].nImageType=GUILD_MENU_IMG;		// �� ��Ȳ�� ���缭

	// done ��ư ���
	SMenu[MN_GUILD_INTRODUCTION].nField[2].nType=FT_NOMAL_PUT;
	SMenu[MN_GUILD_INTRODUCTION].nField[2].x=141;
	SMenu[MN_GUILD_INTRODUCTION].nField[2].y=231;
	SMenu[MN_GUILD_INTRODUCTION].nField[2].nImageNumber=29;
	SMenu[MN_GUILD_INTRODUCTION].nField[2].nImageType=GUILD_MENU_IMG;

	// Done Button 
	SMenu[MN_GUILD_INTRODUCTION].nField[3].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GUILD_INTRODUCTION].nField[3].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_GUILD_INTRODUCTION].nField[3].x=141;
	SMenu[MN_GUILD_INTRODUCTION].nField[3].y=231;
	SMenu[MN_GUILD_INTRODUCTION].nField[3].nRectImage=31;
	SMenu[MN_GUILD_INTRODUCTION].nField[3].nImageNumber=30;
	SMenu[MN_GUILD_INTRODUCTION].nField[3].nImageType=GUILD_MENU_IMG;
	SetRect(SMenu[MN_GUILD_INTRODUCTION].nField[3].rCheakBox,141,231,215,255);

	// �ؽ�Ʈ ��ũ��
	SMenu[MN_GUILD_INTRODUCTION].nField[4].nType=FT_SCROLL_BUTTON;
	SMenu[MN_GUILD_INTRODUCTION].nField[4].nWillDo = SCROLL_UP;
	SMenu[MN_GUILD_INTRODUCTION].nField[4].x=315;
	SMenu[MN_GUILD_INTRODUCTION].nField[4].y=78;
	SMenu[MN_GUILD_INTRODUCTION].nField[4].nShideNomalPlus=2;		// ������ ��ư x, y ��ǥ gab
	SMenu[MN_GUILD_INTRODUCTION].nField[4].nSHideNomalCount=2;		// ����
	SMenu[MN_GUILD_INTRODUCTION].nField[4].nRectImage=1;				// ����
	SMenu[MN_GUILD_INTRODUCTION].nField[4].nImageNumber=0;			// ���� ��ư
	SMenu[MN_GUILD_INTRODUCTION].nField[4].nImageType=SALVATION_PCX;
	SetRect(SMenu[MN_GUILD_INTRODUCTION].nField[4].rCheakBox,316,80,333,108);

	// �ؽ�Ʈ ��ũ��
	SMenu[MN_GUILD_INTRODUCTION].nField[5].nType=FT_SCROLL_BUTTON;
	SMenu[MN_GUILD_INTRODUCTION].nField[5].nWillDo = SCROLL_DOWN;
	SMenu[MN_GUILD_INTRODUCTION].nField[5].x=315;
	SMenu[MN_GUILD_INTRODUCTION].nField[5].y=180;
	SMenu[MN_GUILD_INTRODUCTION].nField[5].nShideNomalPlus=2;		// ������ ��ư x, y ��ǥ gab
	SMenu[MN_GUILD_INTRODUCTION].nField[5].nSHideNomalCount=5;		// ����
	SMenu[MN_GUILD_INTRODUCTION].nField[5].nRectImage=4;				// ����
	SMenu[MN_GUILD_INTRODUCTION].nField[5].nImageNumber=3;			// ���� ��ư
	SMenu[MN_GUILD_INTRODUCTION].nField[5].nImageType=SALVATION_PCX;
	SetRect(SMenu[MN_GUILD_INTRODUCTION].nField[5].rCheakBox,316,183,333,211);

	
////////////////////////////////////////////////////////////////////////////////////////////////
// ��� �޴� 
	SMenu[MN_GUILD_CHANGE_INFO].x=CENTER_X;
	SMenu[MN_GUILD_CHANGE_INFO].y=75;
	SMenu[MN_GUILD_CHANGE_INFO].nImageNumber=0;
	SMenu[MN_GUILD_CHANGE_INFO].nImageType=GUILD_MENU_IMG;
	SMenu[MN_GUILD_CHANGE_INFO].nFieldCount=7;

	// ��� ��ũ ����
	SMenu[MN_GUILD_CHANGE_INFO].nField[1].nType=FT_PUT_BUTTON;
	SMenu[MN_GUILD_CHANGE_INFO].nField[1].nSpecialWillDo=SWD_CALL_GUILD_EXPLAIN;
	SMenu[MN_GUILD_CHANGE_INFO].nField[1].nWillDo = GM_CHANGE_INFO_MENUAL;
	SMenu[MN_GUILD_CHANGE_INFO].nField[1].x=40;
	SMenu[MN_GUILD_CHANGE_INFO].nField[1].y=81;
	SMenu[MN_GUILD_CHANGE_INFO].nField[1].nSHideNomalCount=16;		// ���� ��ư
	SMenu[MN_GUILD_CHANGE_INFO].nField[1].nRectImage=18;
	SMenu[MN_GUILD_CHANGE_INFO].nField[1].nImageNumber=17;
	SMenu[MN_GUILD_CHANGE_INFO].nField[1].nImageType=GUILD_MENU_IMG;
	SetRect(SMenu[MN_GUILD_CHANGE_INFO].nField[1].rCheakBox,40,81,170,104);

	// ��� ��å ����
	SMenu[MN_GUILD_CHANGE_INFO].nField[2].nType=FT_PUT_BUTTON;
	SMenu[MN_GUILD_CHANGE_INFO].nField[2].nSpecialWillDo=SWD_CALL_GUILD_CHANGE_DEGREE;
	SMenu[MN_GUILD_CHANGE_INFO].nField[2].x=40;
	SMenu[MN_GUILD_CHANGE_INFO].nField[2].y=113;
	SMenu[MN_GUILD_CHANGE_INFO].nField[2].nSHideNomalCount=22;
	SMenu[MN_GUILD_CHANGE_INFO].nField[2].nRectImage=24;
	SMenu[MN_GUILD_CHANGE_INFO].nField[2].nImageNumber=23;
	SMenu[MN_GUILD_CHANGE_INFO].nField[2].nImageType=GUILD_MENU_IMG;
	SetRect(SMenu[MN_GUILD_CHANGE_INFO].nField[2].rCheakBox,40,113,169,136);

	// ��� ����
	SMenu[MN_GUILD_CHANGE_INFO].nField[3].nType=FT_PUT_BUTTON;
	SMenu[MN_GUILD_CHANGE_INFO].nField[3].nSpecialWillDo = SWD_CALL_OKCANCEL_BOX;
	SMenu[MN_GUILD_CHANGE_INFO].nField[3].x=40;
	SMenu[MN_GUILD_CHANGE_INFO].nField[3].y=145;
	SMenu[MN_GUILD_CHANGE_INFO].nField[3].nSHideNomalCount=19;
	SMenu[MN_GUILD_CHANGE_INFO].nField[3].nRectImage=21;
	SMenu[MN_GUILD_CHANGE_INFO].nField[3].nImageNumber=20;
	SMenu[MN_GUILD_CHANGE_INFO].nField[3].nImageType=GUILD_MENU_IMG;
	SetRect(SMenu[MN_GUILD_CHANGE_INFO].nField[3].rCheakBox,40,145,170,168);
	strcpy( SMenu[MN_GUILD_CHANGE_INFO].nField[3].temp, kein_GetMenuString( 73 ) );
	
	// ��� ������ ����
	SMenu[MN_GUILD_CHANGE_INFO].nField[4].nType=FT_PUT_BUTTON;
	SMenu[MN_GUILD_CHANGE_INFO].nField[4].nSpecialWillDo=SWD_CALL_OKCANCEL_BOX_ETC;
	SMenu[MN_GUILD_CHANGE_INFO].nField[4].x=40;
	SMenu[MN_GUILD_CHANGE_INFO].nField[4].y=178;
	SMenu[MN_GUILD_CHANGE_INFO].nField[4].nSHideNomalCount=13;
	SMenu[MN_GUILD_CHANGE_INFO].nField[4].nRectImage=15;
	SMenu[MN_GUILD_CHANGE_INFO].nField[4].nImageNumber=14;
	SMenu[MN_GUILD_CHANGE_INFO].nField[4].nImageType=GUILD_MENU_IMG;
	SMenu[MN_GUILD_CHANGE_INFO].nField[4].m_pFunction = CallChangeMaster;
	SetRect(SMenu[MN_GUILD_CHANGE_INFO].nField[4].rCheakBox,40,178,170,201);
	strcpy( SMenu[MN_GUILD_CHANGE_INFO].nField[4].temp, kein_GetMenuString( 137 ) );

	// ��� Ż��
	SMenu[MN_GUILD_CHANGE_INFO].nField[5].nType=FT_PUT_BUTTON;
	SMenu[MN_GUILD_CHANGE_INFO].nField[5].nSpecialWillDo=SWD_CALL_OKCANCEL_BOX;
	SMenu[MN_GUILD_CHANGE_INFO].nField[5].x=40;
	SMenu[MN_GUILD_CHANGE_INFO].nField[5].y=211;
	SMenu[MN_GUILD_CHANGE_INFO].nField[5].nSHideNomalCount=25;
	SMenu[MN_GUILD_CHANGE_INFO].nField[5].nRectImage=27;
	SMenu[MN_GUILD_CHANGE_INFO].nField[5].nImageNumber=26;
	SMenu[MN_GUILD_CHANGE_INFO].nField[5].nImageType=GUILD_MENU_IMG;
	SetRect(SMenu[MN_GUILD_CHANGE_INFO].nField[5].rCheakBox,40,211,170,234);
	strcpy( SMenu[MN_GUILD_CHANGE_INFO].nField[5].temp, kein_GetMenuString( 74 ) );

	// Exit Button 
	SMenu[MN_GUILD_CHANGE_INFO].nField[6].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GUILD_CHANGE_INFO].nField[6].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_GUILD_CHANGE_INFO].nField[6].x=62;
	SMenu[MN_GUILD_CHANGE_INFO].nField[6].y=248;
	SMenu[MN_GUILD_CHANGE_INFO].nField[6].nRectImage=12;
	SMenu[MN_GUILD_CHANGE_INFO].nField[6].nImageNumber=11;
	SMenu[MN_GUILD_CHANGE_INFO].nField[6].nImageType=GUILD_MENU_IMG;
	SetRect(SMenu[MN_GUILD_CHANGE_INFO].nField[6].rCheakBox,62,248,144,271);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
// ��� Ŭ���� �������̽�
	SMenu[MN_DUAL_MAIN].x = CENTER_X;
	SMenu[MN_DUAL_MAIN].y = 50;
	SMenu[MN_DUAL_MAIN].nImageNumber = 2;
	SMenu[MN_DUAL_MAIN].nImageType = DUAL_IMG;
	SMenu[MN_DUAL_MAIN].nFieldCount = 10;

	// ��� Ŭ���� ���� ó�� 
	//SMenu[MN_DUAL_MAIN].nField[0].nType=FT_DUAL_MAIN;

	// �� Ŭ������ ��ư
	// ������ �ɷ�
	SMenu[MN_DUAL_MAIN].nField[0].nType=FT_CHECK_COMBO2;
	SMenu[MN_DUAL_MAIN].nField[0].nWillDo = WARRIOR;
	SMenu[MN_DUAL_MAIN].nField[0].x = 121;
	SMenu[MN_DUAL_MAIN].nField[0].y = 105;
	SMenu[MN_DUAL_MAIN].nField[0].nSHideNomalCount = 2;
	SMenu[MN_DUAL_MAIN].nField[0].nSHideNomalStart = 6;
	SMenu[MN_DUAL_MAIN].nField[0].nRectImage=12;
	SMenu[MN_DUAL_MAIN].nField[0].nImageNumber=12;
	SMenu[MN_DUAL_MAIN].nField[0].nImageType=DUAL_IMG;
	SMenu[MN_DUAL_MAIN].nField[0].nValue=&SMenu[MN_DUAL_MAIN].work;
	SetRect(SMenu[MN_DUAL_MAIN].nField[0].rCheakBox,
	SMenu[MN_DUAL_MAIN].nField[0].x,SMenu[MN_DUAL_MAIN].nField[0].y,
	SMenu[MN_DUAL_MAIN].nField[0].x+80,SMenu[MN_DUAL_MAIN].nField[0].y+50);
	// OK Button 
  // �����Ǵɷ�
	SMenu[MN_DUAL_MAIN].nField[1].nType=FT_CHECK_COMBO2;
	SMenu[MN_DUAL_MAIN].nField[1].nWillDo=THIEF;
  SMenu[MN_DUAL_MAIN].nField[1].x = 34;
	SMenu[MN_DUAL_MAIN].nField[1].y = 201;
  SMenu[MN_DUAL_MAIN].nField[1].nSHideNomalCount = 2;
	SMenu[MN_DUAL_MAIN].nField[1].nSHideNomalStart = 6;
	SMenu[MN_DUAL_MAIN].nField[1].nRectImage=12;
	SMenu[MN_DUAL_MAIN].nField[1].nImageNumber=12;
	SMenu[MN_DUAL_MAIN].nField[1].nImageType=DUAL_IMG;
	SMenu[MN_DUAL_MAIN].nField[1].nValue=&SMenu[MN_DUAL_MAIN].work;
	SetRect(SMenu[MN_DUAL_MAIN].nField[1].rCheakBox,
	SMenu[MN_DUAL_MAIN].nField[1].x,SMenu[MN_DUAL_MAIN].nField[1].y,
	SMenu[MN_DUAL_MAIN].nField[1].x+80,SMenu[MN_DUAL_MAIN].nField[1].y+50);
	// OK Button 
	// �ü��� �ɷ�
	SMenu[MN_DUAL_MAIN].nField[2].nType=FT_CHECK_COMBO2;
	SMenu[MN_DUAL_MAIN].nField[2].nWillDo=ARCHER;
  SMenu[MN_DUAL_MAIN].nField[2].x = 52;
	SMenu[MN_DUAL_MAIN].nField[2].y = 317;  
	SMenu[MN_DUAL_MAIN].nField[2].nSHideNomalCount = 2;
	SMenu[MN_DUAL_MAIN].nField[2].nSHideNomalStart = 6;
	SMenu[MN_DUAL_MAIN].nField[2].nRectImage=12;
	SMenu[MN_DUAL_MAIN].nField[2].nImageNumber=12;
	SMenu[MN_DUAL_MAIN].nField[2].nImageType=DUAL_IMG;
	SMenu[MN_DUAL_MAIN].nField[2].nValue=&SMenu[MN_DUAL_MAIN].work;
	SetRect(SMenu[MN_DUAL_MAIN].nField[2].rCheakBox,
	SMenu[MN_DUAL_MAIN].nField[2].x,SMenu[MN_DUAL_MAIN].nField[2].y,
	SMenu[MN_DUAL_MAIN].nField[2].x+80,SMenu[MN_DUAL_MAIN].nField[2].y+50);
	// OK Button 
  // �������� �ɷ�
	SMenu[MN_DUAL_MAIN].nField[3].nType=FT_CHECK_COMBO2;
	SMenu[MN_DUAL_MAIN].nField[3].nWillDo=WIZARD;
	SMenu[MN_DUAL_MAIN].nField[3].x = 182;
	SMenu[MN_DUAL_MAIN].nField[3].y = 317;
	SMenu[MN_DUAL_MAIN].nField[3].nSHideNomalCount = 2;
	SMenu[MN_DUAL_MAIN].nField[3].nSHideNomalStart = 6;
	SMenu[MN_DUAL_MAIN].nField[3].nRectImage=12;
	SMenu[MN_DUAL_MAIN].nField[3].nImageNumber=12;
	SMenu[MN_DUAL_MAIN].nField[3].nImageType=DUAL_IMG;
	SMenu[MN_DUAL_MAIN].nField[3].nValue=&SMenu[MN_DUAL_MAIN].work;
  SetRect(SMenu[MN_DUAL_MAIN].nField[3].rCheakBox,
	SMenu[MN_DUAL_MAIN].nField[3].x,SMenu[MN_DUAL_MAIN].nField[3].y,
	SMenu[MN_DUAL_MAIN].nField[3].x+80,SMenu[MN_DUAL_MAIN].nField[3].y+50);
	// OK Button 
	// �������� �ɷ�
	SMenu[MN_DUAL_MAIN].nField[4].nType=FT_CHECK_COMBO2;
	SMenu[MN_DUAL_MAIN].nField[4].nWillDo=PRIEST;
	SMenu[MN_DUAL_MAIN].nField[4].x = 201;
	SMenu[MN_DUAL_MAIN].nField[4].y = 201;
	SMenu[MN_DUAL_MAIN].nField[4].nSHideNomalCount = 2;
	SMenu[MN_DUAL_MAIN].nField[4].nSHideNomalStart = 6;
	SMenu[MN_DUAL_MAIN].nField[4].nRectImage=12;
	SMenu[MN_DUAL_MAIN].nField[4].nImageNumber=12;
	SMenu[MN_DUAL_MAIN].nField[4].nImageType=DUAL_IMG;
	SMenu[MN_DUAL_MAIN].nField[4].nValue=&SMenu[MN_DUAL_MAIN].work;

  SetRect(SMenu[MN_DUAL_MAIN].nField[4].rCheakBox,
	SMenu[MN_DUAL_MAIN].nField[4].x,SMenu[MN_DUAL_MAIN].nField[4].y,
	SMenu[MN_DUAL_MAIN].nField[4].x+80,SMenu[MN_DUAL_MAIN].nField[4].y+50);
	// OK Button 
	SMenu[MN_DUAL_MAIN].nField[5].nType=FT_SHOW_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_DUAL_MAIN].nField[5].nSpecialWillDo=SWD_DUAL_OK;
	SMenu[MN_DUAL_MAIN].nField[5].x = 310;
	SMenu[MN_DUAL_MAIN].nField[5].y = 321;
	SMenu[MN_DUAL_MAIN].nField[5].nRectImage=7;
	SMenu[MN_DUAL_MAIN].nField[5].nImageNumber=6;
	SMenu[MN_DUAL_MAIN].nField[5].nImageType=DUAL_IMG;

  SetRect(SMenu[MN_DUAL_MAIN].nField[5].rCheakBox,
	SMenu[MN_DUAL_MAIN].nField[5].x,SMenu[MN_DUAL_MAIN].nField[5].y,
	SMenu[MN_DUAL_MAIN].nField[5].x+200,SMenu[MN_DUAL_MAIN].nField[5].y+50);

  // �ؽ�Ʈ ��ũ��
  SMenu[MN_DUAL_MAIN].nField[6].nType = FT_DATA_PUT;
	SMenu[MN_DUAL_MAIN].nField[6].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_DUAL_MAIN].nField[6].x = 320;
	SMenu[MN_DUAL_MAIN].nField[6].y = 60;
	SMenu[MN_DUAL_MAIN].nField[6].nRectImage = 200;
	strcpy(SMenu[MN_DUAL_MAIN].nField[6].temp,  " ���� ���� �ε����Դϴ�..\n NOW LOADING... ... ��ø� ��ٷ� �ֽʽÿ� ���� ��ư�� Ŭ�� �Ͻʽÿ�  " );


////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
// ����Ƽ �ʱ�ȭ
	SMenu[MN_INIT_ABILITY].x = CENTER_X;
	SMenu[MN_INIT_ABILITY].y = 100;
	SMenu[MN_INIT_ABILITY].nImageNumber = 0;
	SMenu[MN_INIT_ABILITY].nImageType = DUAL_IMG;
	SMenu[MN_INIT_ABILITY].nFieldCount = 2;
  // OK Button 
	SMenu[MN_INIT_ABILITY].nField[1].nType=FT_SHOW_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_INIT_ABILITY].nField[1].nSpecialWillDo=SWD_INIT_ABILITY_OK;
	SMenu[MN_INIT_ABILITY].nField[1].x = 19;
	SMenu[MN_INIT_ABILITY].nField[1].y = 171;
	SMenu[MN_INIT_ABILITY].nField[1].nRectImage=5;
	SMenu[MN_INIT_ABILITY].nField[1].nImageNumber=3;
	SMenu[MN_INIT_ABILITY].nField[1].nImageType=DUAL_IMG;

  SetRect(SMenu[MN_INIT_ABILITY].nField[1].rCheakBox,
	SMenu[MN_INIT_ABILITY].nField[1].x,SMenu[MN_INIT_ABILITY].nField[1].y,
	SMenu[MN_INIT_ABILITY].nField[1].x+300,SMenu[MN_INIT_ABILITY].nField[1].y+50);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
// �� �޴�
	SMenu[MN_RUNE].x=CENTER_X;
	SMenu[MN_RUNE].y=35;
	SMenu[MN_RUNE].nImageNumber=0;
	SMenu[MN_RUNE].nImageType=RUNE_IMG;
	SMenu[MN_RUNE].nFieldCount=8;
	SMenu[MN_RUNE].key=10;

	// ���� �� ��ư ���
	SMenu[MN_RUNE].nField[0].nType=FT_VIEW_RUNE_LEFT;
	SMenu[MN_RUNE].nField[0].nSpecialWillDo=SWD_CALL_GUILD_EXPLAIN;
	SMenu[MN_RUNE].nField[0].nWillDo = GM_CHANGE_INFO_MENUAL;
	SMenu[MN_RUNE].nField[0].nSHideNomalCount=4;
	SMenu[MN_RUNE].nField[0].nSHideNomalStart=0;
	SMenu[MN_RUNE].nField[0].nShideNomalPlus=1;
	SMenu[MN_RUNE].nField[0].nImageType=RUNE_IMG;
	SetRect(SMenu[MN_RUNE].nField[0].rCheakBox,58, 168, 263, 302);

	// ������ �� ����
	SMenu[MN_RUNE].nField[1].nType=FT_VIEW_RUNE_RIGHT;
	//SMenu[MN_RUNE].nField[1].nSpecialWillDo=SWD_CALL_GUILD_CHANGE_DEGREE;
	SMenu[MN_RUNE].nField[1].x=321;
	SMenu[MN_RUNE].nField[1].y=57;
	SMenu[MN_RUNE].nField[1].nImageType=RUNE_IMG;

	// Done Button
	SMenu[MN_RUNE].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_RUNE].nField[2].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_RUNE].nField[2].x=377;
	SMenu[MN_RUNE].nField[2].y=336;
	SMenu[MN_RUNE].nField[2].nRectImage=66;
	SMenu[MN_RUNE].nField[2].nImageNumber=65;
	SMenu[MN_RUNE].nField[2].nImageType=RUNE_IMG;
	SetRect(SMenu[MN_RUNE].nField[2].rCheakBox,377,336,377+76,336+27);

	// �ؽ�Ʈ ���
	SMenu[MN_RUNE].nField[3].nType=FT_SCROLL_TEXT_PUT;
	SMenu[MN_RUNE].nField[3].x=321;
	SMenu[MN_RUNE].nField[3].y=121;
	SMenu[MN_RUNE].nField[3].nWillDo = RGB16( 255, 255, 255);
	SMenu[MN_RUNE].nField[3].nSpecialWillDo = 1;// CheakType �� 1�� �ٲ� �ٽô� �ʱ�ȭ ��ƾ���� �ȵ�� ���� �Ѵ�.
	SMenu[MN_RUNE].nField[3].nRectImage=190;

	// �ؽ�Ʈ ��ũ��
	SMenu[MN_RUNE].nField[4].nType=FT_SCROLL_BUTTON;
	SMenu[MN_RUNE].nField[4].nWillDo = SCROLL_UP;
	SMenu[MN_RUNE].nField[4].x=478;
	SMenu[MN_RUNE].nField[4].y=307;
	SMenu[MN_RUNE].nField[4].nSHideNomalCount=57;		// ����
	SMenu[MN_RUNE].nField[4].nRectImage=58;				// ����
	SMenu[MN_RUNE].nField[4].nImageNumber=58;			// ���� ��ư
	SMenu[MN_RUNE].nField[4].nImageType=RUNE_IMG;
	SetRect(SMenu[MN_RUNE].nField[4].rCheakBox,478,307,478+19,307+19);

	// �ؽ�Ʈ ��ũ��
	SMenu[MN_RUNE].nField[5].nType=FT_SCROLL_BUTTON;
	SMenu[MN_RUNE].nField[5].nWillDo = SCROLL_DOWN;
	SMenu[MN_RUNE].nField[5].x=499;
	SMenu[MN_RUNE].nField[5].y=307;
	SMenu[MN_RUNE].nField[5].nSHideNomalCount=59;		// ����
	SMenu[MN_RUNE].nField[5].nRectImage=60;				// ����
	SMenu[MN_RUNE].nField[5].nImageNumber=60;			// ���� ��ư
	SMenu[MN_RUNE].nField[5].nImageType=RUNE_IMG;
	SetRect(SMenu[MN_RUNE].nField[5].rCheakBox,499,307,499+19,307+19);

	// �ؽ�Ʈ ��ũ��
	SMenu[MN_RUNE].nField[6].nType=FT_SCROLL_BUTTON2;
	SMenu[MN_RUNE].nField[6].nWillDo = SCROLL_UP;
	SMenu[MN_RUNE].nField[6].nSpecialWillDo = 0;		// ������ ���� �ʵ��ȣ
	SMenu[MN_RUNE].nField[6].x=254;
	SMenu[MN_RUNE].nField[6].y=336;
	SMenu[MN_RUNE].nField[6].nSHideNomalCount=55;		// ����
	SMenu[MN_RUNE].nField[6].nRectImage=56;				// ����
	SMenu[MN_RUNE].nField[6].nImageNumber=56;			// ���� ��ư
	SMenu[MN_RUNE].nField[6].nImageType=RUNE_IMG;
	SetRect(SMenu[MN_RUNE].nField[6].rCheakBox,254,336,254+17,336+17);

	// �ؽ�Ʈ ��ũ��
	SMenu[MN_RUNE].nField[7].nType=FT_SCROLL_BUTTON2;
	SMenu[MN_RUNE].nField[7].nWillDo = SCROLL_DOWN;
	SMenu[MN_RUNE].nField[7].nSpecialWillDo = 0;		// ������ ���� �ʵ��ȣ
	SMenu[MN_RUNE].nField[7].x=280;
	SMenu[MN_RUNE].nField[7].y=336;
	SMenu[MN_RUNE].nField[7].nSHideNomalCount=53;		// ����
	SMenu[MN_RUNE].nField[7].nRectImage=54;				// ����
	SMenu[MN_RUNE].nField[7].nImageNumber=54;			// ���� ��ư
	SMenu[MN_RUNE].nField[7].nImageType=RUNE_IMG;
	SetRect(SMenu[MN_RUNE].nField[7].rCheakBox,280,336,280+17,336+17);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
// ���� �޴�
	SMenu[MN_SKIN].x = CENTER_X;
	SMenu[MN_SKIN].y = 100;
	SMenu[MN_SKIN].nImageNumber = 6;
	SMenu[MN_SKIN].nImageType = SKIN_MENU;
	SMenu[MN_SKIN].nFieldCount = 3;

	SMenu[MN_SKIN].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_SKIN].nField[0].nSpecialWillDo=SWD_CALL_SKIN_MENU;
	SMenu[MN_SKIN].nField[0].nWillDo=0;
	SMenu[MN_SKIN].nField[0].x = 42;
	SMenu[MN_SKIN].nField[0].y = 55;
	SMenu[MN_SKIN].nField[0].nRectImage=17;
	SMenu[MN_SKIN].nField[0].nImageNumber=16;
	SMenu[MN_SKIN].nField[0].nImageType=SKIN_MENU;
	SetRect(SMenu[MN_SKIN].nField[0].rCheakBox,
		SMenu[MN_SKIN].nField[0].x,SMenu[MN_SKIN].nField[0].y,
		SMenu[MN_SKIN].nField[0].x+130,SMenu[MN_SKIN].nField[0].y+27);

	SMenu[MN_SKIN].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_SKIN].nField[1].nSpecialWillDo=SWD_CALL_SKIN_MENU;
	SMenu[MN_SKIN].nField[1].nWillDo=1;
	SMenu[MN_SKIN].nField[1].x = 42;
	SMenu[MN_SKIN].nField[1].y = 86;
	SMenu[MN_SKIN].nField[1].nRectImage=13;
	SMenu[MN_SKIN].nField[1].nImageNumber=11;
	SMenu[MN_SKIN].nField[1].nImageType=SKIN_MENU;
	SetRect(SMenu[MN_SKIN].nField[1].rCheakBox,
		SMenu[MN_SKIN].nField[1].x,SMenu[MN_SKIN].nField[1].y,
		SMenu[MN_SKIN].nField[1].x+130,SMenu[MN_SKIN].nField[1].y+27);

	// Done Button 
	SMenu[MN_SKIN].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_SKIN].nField[2].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_SKIN].nField[2].x=69;
	SMenu[MN_SKIN].nField[2].y=130;
	SMenu[MN_SKIN].nField[2].nRectImage=8;
	SMenu[MN_SKIN].nField[2].nImageNumber=7;
	SMenu[MN_SKIN].nField[2].nImageType=SKIN_MENU;
	SetRect(SMenu[MN_SKIN].nField[2].rCheakBox,69,130,69+76,130+27);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
// �׺� �������̽�
	SMenu[MN_GAMBLE].x=0;
	SMenu[MN_GAMBLE].y=0;
	SMenu[MN_GAMBLE].nImageNumber= 0;
	SMenu[MN_GAMBLE].nImageType = GAMBLE_IMG;
	SMenu[MN_GAMBLE].nFieldCount = 5;
	
	// gamble button
	SMenu[MN_GAMBLE].nField[0].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT ;
	SMenu[MN_GAMBLE].nField[0].nSpecialWillDo=SWD_GABLE_ITEM;
	SMenu[MN_GAMBLE].nField[0].x=17;
	SMenu[MN_GAMBLE].nField[0].y=371;
	SMenu[MN_GAMBLE].nField[0].nRectImage=9;
	SMenu[MN_GAMBLE].nField[0].nImageNumber=10;
	SMenu[MN_GAMBLE].nField[0].nImageType = GAMBLE_IMG;
	SetRect(SMenu[MN_GAMBLE].nField[0].rCheakBox,17,371,17+131,371+46);

	// CANCEL button
	SMenu[MN_GAMBLE].nField[1].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT ;
	SMenu[MN_GAMBLE].nField[1].nSpecialWillDo=SWD_ITEM_MAKE_CANCEL;
	SMenu[MN_GAMBLE].nField[1].x=166;
	SMenu[MN_GAMBLE].nField[1].y=371;
	SMenu[MN_GAMBLE].nField[1].nRectImage=11;
	SMenu[MN_GAMBLE].nField[1].nImageNumber=12;
	SMenu[MN_GAMBLE].nField[1].nImageType = GAMBLE_IMG;
	SetRect(SMenu[MN_GAMBLE].nField[1].rCheakBox,166,371,166+131,371+46);

	// �귿 �̹��� ���
	SMenu[MN_GAMBLE].nField[2].nType = FT_ROULETTE;
	SMenu[MN_GAMBLE].nField[2].nSpecialWillDo = 100;
	SMenu[MN_GAMBLE].nField[2].nImageType = GAMBLE_IMG;

	// item ���Ƽ� ��� 
	SMenu[MN_GAMBLE].nField[4].nType = FT_MAKE_ITEM;
	SMenu[MN_GAMBLE].nField[4].nImageType = GAMBLE_IMG;
	SetRect(SMenu[MN_GAMBLE].nField[4].rCheakBox,68,139,244,329);

	SMenu[MN_GAMBLE].nField[3].nType = FT_GAMBLE_TODAY;
	SMenu[MN_GAMBLE].nField[3].nImageType = GAMBLE_IMG;
	x = SMenu[MN_GAMBLE].nField[3].x=41;
	y = SMenu[MN_GAMBLE].nField[3].y=63;
	SetRect(SMenu[MN_GAMBLE].nField[3].rCheakBox,x,y,x+30,y+30);



////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
// ��� ��� ����Ʈ �޴�
	SMenu[MN_GUILD_MEMBER_LIST].x=CENTER_X;
	SMenu[MN_GUILD_MEMBER_LIST].y=10;
	SMenu[MN_GUILD_MEMBER_LIST].nImageNumber= 9;
	SMenu[MN_GUILD_MEMBER_LIST].nImageType = IIT_GUILD_MEMBER_LIST;
	SMenu[MN_GUILD_MEMBER_LIST].nFieldCount = 7;
	
	// ��� ���� ȭ��
	SMenu[MN_GUILD_MEMBER_LIST].nField[0].nType = FT_GUILD_MEMBER_LIST_DEGREE_TAB;
	SMenu[MN_GUILD_MEMBER_LIST].nField[0].x=0;
	SMenu[MN_GUILD_MEMBER_LIST].nField[0].y=67;
	SMenu[MN_GUILD_MEMBER_LIST].nField[0].nImageType = IIT_GUILD_MEMBER_LIST;
	SetRect(SMenu[MN_GUILD_MEMBER_LIST].nField[0].rCheakBox,20,89,435,121);

	// CANCEL button
	SMenu[MN_GUILD_MEMBER_LIST].nField[1].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT ;
	SMenu[MN_GUILD_MEMBER_LIST].nField[1].nSpecialWillDo=SWD_SMALLMENU_END;
	x=SMenu[MN_GUILD_MEMBER_LIST].nField[1].x=358;
	y=SMenu[MN_GUILD_MEMBER_LIST].nField[1].y=384;
	SMenu[MN_GUILD_MEMBER_LIST].nField[1].nRectImage=16;
	SMenu[MN_GUILD_MEMBER_LIST].nField[1].nImageNumber=15;
	SMenu[MN_GUILD_MEMBER_LIST].nField[1].nImageType = IIT_GUILD_MEMBER_LIST;
	SetRect(SMenu[MN_GUILD_MEMBER_LIST].nField[1].rCheakBox,x, y, x+71, y+22);

	// �ؽ�Ʈ ��ũ��
	SMenu[MN_GUILD_MEMBER_LIST].nField[2].nType=FT_SCROLL_BUTTON;
	SMenu[MN_GUILD_MEMBER_LIST].nField[2].nWillDo = SCROLL_UP;
	x = SMenu[MN_GUILD_MEMBER_LIST].nField[2].x=395; //400;
	y = SMenu[MN_GUILD_MEMBER_LIST].nField[2].y=197;// 175;//020725 lsw
	SMenu[MN_GUILD_MEMBER_LIST].nField[2].nShideNomalPlus=2;		// ������ ��ư x, y ��ǥ gab
	SMenu[MN_GUILD_MEMBER_LIST].nField[2].nSHideNomalCount=2;		// ����
	SMenu[MN_GUILD_MEMBER_LIST].nField[2].nRectImage=1;				// ����
	SMenu[MN_GUILD_MEMBER_LIST].nField[2].nImageNumber=0;			// ���� ��ư
	SMenu[MN_GUILD_MEMBER_LIST].nField[2].nImageType=SALVATION_PCX;
	SetRect(SMenu[MN_GUILD_MEMBER_LIST].nField[2].rCheakBox,x,y,x+17,y+28 );

	// �ؽ�Ʈ ��ũ��
	SMenu[MN_GUILD_MEMBER_LIST].nField[3].nType=FT_SCROLL_BUTTON;
	SMenu[MN_GUILD_MEMBER_LIST].nField[3].nWillDo = SCROLL_DOWN;
	x = SMenu[MN_GUILD_MEMBER_LIST].nField[3].x=395;// 400;
	y = SMenu[MN_GUILD_MEMBER_LIST].nField[3].y=327;// 305;
	SMenu[MN_GUILD_MEMBER_LIST].nField[3].nShideNomalPlus=2;		// ������ ��ư x, y ��ǥ gab
	SMenu[MN_GUILD_MEMBER_LIST].nField[3].nSHideNomalCount=5;		// ����
	SMenu[MN_GUILD_MEMBER_LIST].nField[3].nRectImage=4;				// ����
	SMenu[MN_GUILD_MEMBER_LIST].nField[3].nImageNumber=3;			// ���� ��ư
	SMenu[MN_GUILD_MEMBER_LIST].nField[3].nImageType=SALVATION_PCX;
	SetRect(SMenu[MN_GUILD_MEMBER_LIST].nField[3].rCheakBox,x,y,x+17,y+28);

	// ���� ����
	SMenu[MN_GUILD_MEMBER_LIST].nField[4].nType=FT_GUILD_PUBLIC_NOTICE;
	SMenu[MN_GUILD_MEMBER_LIST].nField[4].nWillDo = 0;
	x=SMenu[MN_GUILD_MEMBER_LIST].nField[4].x=33+3;
	y=SMenu[MN_GUILD_MEMBER_LIST].nField[4].y=345+29;
	SMenu[MN_GUILD_MEMBER_LIST].nField[4].nRectImage=4;				// ����
	SMenu[MN_GUILD_MEMBER_LIST].nField[4].nImageNumber=3;			// ���� ��ư
	SMenu[MN_GUILD_MEMBER_LIST].nField[4].nImageType=IIT_GUILD_MEMBER_LIST;
	SetRect(SMenu[MN_GUILD_MEMBER_LIST].nField[4].rCheakBox,x, y, x+30, y+20);

	// ���� ���
	SMenu[MN_GUILD_MEMBER_LIST].nField[5].nType=FT_GUILD_PUBLIC_NOTICE;
	SMenu[MN_GUILD_MEMBER_LIST].nField[5].nWillDo = 1;
	x=SMenu[MN_GUILD_MEMBER_LIST].nField[5].x=66+8;
	y=SMenu[MN_GUILD_MEMBER_LIST].nField[5].y=345+29;
	SMenu[MN_GUILD_MEMBER_LIST].nField[5].nRectImage=1;				// ����
	SMenu[MN_GUILD_MEMBER_LIST].nField[5].nImageNumber=2;			// ���� ��ư
	SMenu[MN_GUILD_MEMBER_LIST].nField[5].nImageType=IIT_GUILD_MEMBER_LIST;
	SetRect(SMenu[MN_GUILD_MEMBER_LIST].nField[5].rCheakBox,x, y, x+30, y+20);

	// ���� ���
	SMenu[MN_GUILD_MEMBER_LIST].nField[6].nType=FT_GUILD_PUBLIC_NOTICE;
	SMenu[MN_GUILD_MEMBER_LIST].nField[6].nWillDo = 2;
	x=SMenu[MN_GUILD_MEMBER_LIST].nField[6].x=100+13;
	y=SMenu[MN_GUILD_MEMBER_LIST].nField[6].y=345+29;
	SMenu[MN_GUILD_MEMBER_LIST].nField[6].nRectImage=6;				// ����
	SMenu[MN_GUILD_MEMBER_LIST].nField[6].nImageNumber=7;			// ���� ��ư
	SMenu[MN_GUILD_MEMBER_LIST].nField[6].nImageType=IIT_GUILD_MEMBER_LIST;
	SetRect(SMenu[MN_GUILD_MEMBER_LIST].nField[6].rCheakBox,x, y, x+30, y+20);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ģ�� �ý���
	SMenu[MN_FRIEND_MENU].x=RIGHT_X;
	SMenu[MN_FRIEND_MENU].y=0;
	SMenu[MN_FRIEND_MENU].nImageNumber= 1;
	SMenu[MN_FRIEND_MENU].nImageType = IIT_FRIEND;
	SMenu[MN_FRIEND_MENU].nFieldCount = 5;
	
	// ģ�� ��� �����ֱ�
	SMenu[MN_FRIEND_MENU].nField[0].nType = FT_FRIEND_NAME;
	SMenu[MN_FRIEND_MENU].nField[0].x=25;
	SMenu[MN_FRIEND_MENU].nField[0].y=55;
	SetRect(SMenu[MN_FRIEND_MENU].nField[0].rCheakBox,18,45,194,282);

	// CANCEL button
	SMenu[MN_FRIEND_MENU].nField[1].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT ;
	SMenu[MN_FRIEND_MENU].nField[1].nSpecialWillDo=SWD_SMALLMENU_CLOSE;
	x=SMenu[MN_FRIEND_MENU].nField[1].x=117;
	y=SMenu[MN_FRIEND_MENU].nField[1].y=291;
	SMenu[MN_FRIEND_MENU].nField[1].nRectImage=7;
	SMenu[MN_FRIEND_MENU].nField[1].nImageNumber=8;
	SMenu[MN_FRIEND_MENU].nField[1].nImageType = IIT_FRIEND;
	SetRect(SMenu[MN_FRIEND_MENU].nField[1].rCheakBox,x, y, x+76, y+27);

	// ��� button
	SMenu[MN_FRIEND_MENU].nField[2].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT ;
	SMenu[MN_FRIEND_MENU].nField[2].nSpecialWillDo=SWD_CALLMENU;
	SMenu[MN_FRIEND_MENU].nField[2].nWillDo=MN_FRIEND_REGIST;
	x=SMenu[MN_FRIEND_MENU].nField[2].x=21;
	y=SMenu[MN_FRIEND_MENU].nField[2].y=291;
	SMenu[MN_FRIEND_MENU].nField[2].nRectImage=5;
	SMenu[MN_FRIEND_MENU].nField[2].nImageNumber=6;
	SMenu[MN_FRIEND_MENU].nField[2].nImageType = IIT_FRIEND;
	//SetRect(SMenu[MN_FRIEND_MENU].nField[2].rCheakBox,x, y, x+76, y+27);

	// �ؽ�Ʈ ��ũ��
	SMenu[MN_FRIEND_MENU].nField[3].nType=FT_SCROLL_BUTTON;
	SMenu[MN_FRIEND_MENU].nField[3].nWillDo = SCROLL_UP;
	x=SMenu[MN_FRIEND_MENU].nField[3].x=147;
	y=SMenu[MN_FRIEND_MENU].nField[3].y=260;
	SMenu[MN_FRIEND_MENU].nField[3].nSHideNomalCount=15;	// ����
	SMenu[MN_FRIEND_MENU].nField[3].nRectImage=16;			// ����
	SMenu[MN_FRIEND_MENU].nField[3].nImageNumber=15;		// ���� ��ư
	SMenu[MN_FRIEND_MENU].nField[3].nImageType=IIT_FRIEND;
	SetRect(SMenu[MN_FRIEND_MENU].nField[3].rCheakBox,x,y,x+17, y+17);

	// �ؽ�Ʈ ��ũ��
	SMenu[MN_FRIEND_MENU].nField[4].nType=FT_SCROLL_BUTTON;
	SMenu[MN_FRIEND_MENU].nField[4].nWillDo = SCROLL_DOWN;
	x=SMenu[MN_FRIEND_MENU].nField[4].x=166;
	y=SMenu[MN_FRIEND_MENU].nField[4].y=260;
	SMenu[MN_FRIEND_MENU].nField[4].nSHideNomalCount=17;		// ����
	SMenu[MN_FRIEND_MENU].nField[4].nRectImage=18;				// ����
	SMenu[MN_FRIEND_MENU].nField[4].nImageNumber=17;			// ���� ��ư
	SMenu[MN_FRIEND_MENU].nField[4].nImageType=IIT_FRIEND;
	SetRect(SMenu[MN_FRIEND_MENU].nField[4].rCheakBox,x,y,x+17,y+17);



////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ģ�� ��� �޴�
	SMenu[MN_FRIEND_REGIST].x=CENTER_X;
	SMenu[MN_FRIEND_REGIST].y=100;
	SMenu[MN_FRIEND_REGIST].nImageNumber= 0;
	SMenu[MN_FRIEND_REGIST].nImageType = IIT_FRIEND;
	SMenu[MN_FRIEND_REGIST].nFieldCount = 5;
	
	// �޼��� ���
	SMenu[MN_FRIEND_REGIST].nField[0].nType=FT_DATA_PUT;
	SMenu[MN_FRIEND_REGIST].nField[0].x=33;
	SMenu[MN_FRIEND_REGIST].nField[0].y=60;
	SMenu[MN_FRIEND_REGIST].nField[0].nWillDo=RGB16( 10, 10, 10 );
	SMenu[MN_FRIEND_REGIST].nField[0].nImageNumber=DP_RECT_STRING_PUT;
	SMenu[MN_FRIEND_REGIST].nField[0].nRectImage=160;
	sprintf(SMenu[MN_FRIEND_REGIST].nField[0].temp, "%s", kein_GetMenuString( 98 ) );//lsw

	// �̸� �Է�
	SMenu[MN_FRIEND_REGIST].nField[1].nType=FT_FRIEND_REGIST_NAME;
	SMenu[MN_FRIEND_REGIST].nField[1].x=35;
	SMenu[MN_FRIEND_REGIST].nField[1].y=153;
	sprintf(SMenu[MN_FRIEND_REGIST].nField[1].temp, "%s", kein_GetMenuString( 99 ) );//lsw

	// CANCEL button
	SMenu[MN_FRIEND_REGIST].nField[2].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT ;
	SMenu[MN_FRIEND_REGIST].nField[2].nSpecialWillDo=SWD_SUBMENU_BACK;//021007 lsw
	x=SMenu[MN_FRIEND_REGIST].nField[2].x=137;
	y=SMenu[MN_FRIEND_REGIST].nField[2].y=182;
	SMenu[MN_FRIEND_REGIST].nField[2].nRectImage=13;
	SMenu[MN_FRIEND_REGIST].nField[2].nImageNumber=14;
	SMenu[MN_FRIEND_REGIST].nField[2].nImageType = IIT_FRIEND;
	SetRect(SMenu[MN_FRIEND_REGIST].nField[2].rCheakBox,x, y, x+54, y+22);

	// OK button
	SMenu[MN_FRIEND_REGIST].nField[3].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT ;
	SMenu[MN_FRIEND_REGIST].nField[3].nSpecialWillDo=SWD_REGIST_FRIEND;
	x=SMenu[MN_FRIEND_REGIST].nField[3].x=29;
	y=SMenu[MN_FRIEND_REGIST].nField[3].y=182;
	SMenu[MN_FRIEND_REGIST].nField[3].nRectImage=9;
	SMenu[MN_FRIEND_REGIST].nField[3].nImageNumber=10;
	SMenu[MN_FRIEND_REGIST].nField[3].nImageType = IIT_FRIEND;
	SetRect(SMenu[MN_FRIEND_REGIST].nField[3].rCheakBox,x, y, x+54, y+22);

	// DELETE button
	SMenu[MN_FRIEND_REGIST].nField[4].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT ;
	SMenu[MN_FRIEND_REGIST].nField[4].nSpecialWillDo=SWD_DELETE_FRIEND;
	x=SMenu[MN_FRIEND_REGIST].nField[4].x=83;
	y=SMenu[MN_FRIEND_REGIST].nField[4].y=182;
	SMenu[MN_FRIEND_REGIST].nField[4].nRectImage=11;
	SMenu[MN_FRIEND_REGIST].nField[4].nImageNumber=12;
	SMenu[MN_FRIEND_REGIST].nField[4].nImageType = IIT_FRIEND;
	SetRect(SMenu[MN_FRIEND_REGIST].nField[4].rCheakBox,x, y, x+54, y+22);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
// ��Ż �޴�
	SMenu[MN_POTAL_MENU].x=CENTER_X;
	SMenu[MN_POTAL_MENU].y=50;
	SMenu[MN_POTAL_MENU].nImageNumber= 7;
	SMenu[MN_POTAL_MENU].nImageType = IIT_POTAL;
	SMenu[MN_POTAL_MENU].nFieldCount = 6;
	SMenu[MN_POTAL_MENU].key = 8;		// �� ������ �� ����
	
	// �̵� ���� �����ֱ�
	SMenu[MN_POTAL_MENU].nField[0].nType = FT_POTAL_MAP;

	// CANCEL button
	SMenu[MN_POTAL_MENU].nField[1].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT ;
	SMenu[MN_POTAL_MENU].nField[1].nSpecialWillDo=SWD_SMALLMENU_CLOSE;
	x=SMenu[MN_POTAL_MENU].nField[1].x=191;
	y=SMenu[MN_POTAL_MENU].nField[1].y=305;
	SMenu[MN_POTAL_MENU].nField[1].nRectImage=5;
	SMenu[MN_POTAL_MENU].nField[1].nImageNumber=6;
	SMenu[MN_POTAL_MENU].nField[1].nImageType = IIT_POTAL;
	SetRect(SMenu[MN_POTAL_MENU].nField[1].rCheakBox,x, y, x+76, y+27);

	// �̵� button
	SMenu[MN_POTAL_MENU].nField[2].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT ;
	SMenu[MN_POTAL_MENU].nField[2].nSpecialWillDo=SWD_POTAL_MENU;
	SMenu[MN_POTAL_MENU].nField[2].nWillDo=MN_POTAL_SELECT;
	x=SMenu[MN_POTAL_MENU].nField[2].x=50;
	y=SMenu[MN_POTAL_MENU].nField[2].y=305;
	SMenu[MN_POTAL_MENU].nField[2].nRectImage=2;
	SMenu[MN_POTAL_MENU].nField[2].nImageNumber=3;
	SMenu[MN_POTAL_MENU].nField[2].nImageType = IIT_POTAL;
	SetRect(SMenu[MN_POTAL_MENU].nField[2].rCheakBox,x, y, x+76, y+27);

	// �ؽ�Ʈ ��ũ��
	SMenu[MN_POTAL_MENU].nField[3].nType=FT_SCROLL_BUTTON;
	SMenu[MN_POTAL_MENU].nField[3].nWillDo = SCROLL_UP;
	x=SMenu[MN_POTAL_MENU].nField[3].x=240;
	y=SMenu[MN_POTAL_MENU].nField[3].y=263;
	SMenu[MN_POTAL_MENU].nField[3].nSHideNomalCount=20;	// ����
	SMenu[MN_POTAL_MENU].nField[3].nRectImage=21;			// ����
	SMenu[MN_POTAL_MENU].nField[3].nImageNumber=20;		// ���� ��ư
	SMenu[MN_POTAL_MENU].nField[3].nImageType=IIT_POTAL;
	SetRect(SMenu[MN_POTAL_MENU].nField[3].rCheakBox,x,y,x+17, y+17);

	// �ؽ�Ʈ ��ũ��
	SMenu[MN_POTAL_MENU].nField[4].nType=FT_SCROLL_BUTTON;
	SMenu[MN_POTAL_MENU].nField[4].nWillDo = SCROLL_DOWN;
	x=SMenu[MN_POTAL_MENU].nField[4].x=260;
	y=SMenu[MN_POTAL_MENU].nField[4].y=263;
	SMenu[MN_POTAL_MENU].nField[4].nSHideNomalCount=22;		// ����
	SMenu[MN_POTAL_MENU].nField[4].nRectImage=23;				// ����
	SMenu[MN_POTAL_MENU].nField[4].nImageNumber=22;			// ���� ��ư
	SMenu[MN_POTAL_MENU].nField[4].nImageType=IIT_POTAL;
	SetRect(SMenu[MN_POTAL_MENU].nField[4].rCheakBox,x,y,x+17,y+17);

	// ���� �� �����ֱ�
	SMenu[MN_POTAL_MENU].nField[5].nType = FT_POTAL_SELECT;
	SMenu[MN_POTAL_MENU].nField[5].x=24;
	SMenu[MN_POTAL_MENU].nField[5].y=102;
	SMenu[MN_POTAL_MENU].nField[5].nImageNumber=0;
	SMenu[MN_POTAL_MENU].nField[5].nImageType=IIT_POTAL;
	SetRect(SMenu[MN_POTAL_MENU].nField[5].rCheakBox, 20, 102, 288, 260);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
// ��Ż �޴�
	SMenu[MN_POTAL_SELECT].x=CENTER_X;
	SMenu[MN_POTAL_SELECT].y=50;
	SMenu[MN_POTAL_SELECT].nImageNumber= 14;
	SMenu[MN_POTAL_SELECT].nImageType = IIT_POTAL;
	SMenu[MN_POTAL_SELECT].nFieldCount = 5;
	SMenu[MN_POTAL_SELECT].key = 5;
	
	// �̵� ���� �����ֱ�
	SMenu[MN_POTAL_SELECT].nField[0].nType = FT_POTAL_SELECT_PUT;
	x=SMenu[MN_POTAL_SELECT].nField[0].x=34;
	y=SMenu[MN_POTAL_SELECT].nField[0].y=64;
	SMenu[MN_POTAL_SELECT].nField[0].nImageNumber=15;
	SMenu[MN_POTAL_SELECT].nField[0].nImageType=IIT_POTAL;

	// CANCEL button
	SMenu[MN_POTAL_SELECT].nField[1].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT ;
	SMenu[MN_POTAL_SELECT].nField[1].nSpecialWillDo=SWD_SUBMENU_BACK;//021007 lsw
	x=SMenu[MN_POTAL_SELECT].nField[1].x=189;
	y=SMenu[MN_POTAL_SELECT].nField[1].y=304;
	SMenu[MN_POTAL_SELECT].nField[1].nRectImage=12;
	SMenu[MN_POTAL_SELECT].nField[1].nImageNumber=13;
	SMenu[MN_POTAL_SELECT].nField[1].nImageType = IIT_POTAL;
	SetRect(SMenu[MN_POTAL_SELECT].nField[1].rCheakBox,x, y, x+76, y+27);

	// �̵� button
	SMenu[MN_POTAL_SELECT].nField[2].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT ;
	SMenu[MN_POTAL_SELECT].nField[2].nSpecialWillDo=SWD_GOTO_POTAL;
	x=SMenu[MN_POTAL_SELECT].nField[2].x=53;
	y=SMenu[MN_POTAL_SELECT].nField[2].y=304;
	SMenu[MN_POTAL_SELECT].nField[2].nRectImage=9;
	SMenu[MN_POTAL_SELECT].nField[2].nImageNumber=10;
	SMenu[MN_POTAL_SELECT].nField[2].nImageType = IIT_POTAL;
	SetRect(SMenu[MN_POTAL_SELECT].nField[2].rCheakBox,x, y, x+76, y+27);

	// �ؽ�Ʈ ��ũ��
	SMenu[MN_POTAL_SELECT].nField[3].nType=FT_SCROLL_BUTTON;
	SMenu[MN_POTAL_SELECT].nField[3].nWillDo = SCROLL_UP;
	x=SMenu[MN_POTAL_SELECT].nField[3].x=270;
	y=SMenu[MN_POTAL_SELECT].nField[3].y=73;
	SMenu[MN_POTAL_SELECT].nField[3].nSHideNomalCount=20;	// ����
	SMenu[MN_POTAL_SELECT].nField[3].nRectImage=21;			// ����
	SMenu[MN_POTAL_SELECT].nField[3].nImageNumber=20;		// ���� ��ư
	SMenu[MN_POTAL_SELECT].nField[3].nImageType=IIT_POTAL;
	SetRect(SMenu[MN_POTAL_SELECT].nField[3].rCheakBox,x,y,x+17, y+17);

	// �ؽ�Ʈ ��ũ��
	SMenu[MN_POTAL_SELECT].nField[4].nType=FT_SCROLL_BUTTON;
	SMenu[MN_POTAL_SELECT].nField[4].nWillDo = SCROLL_DOWN;
	x=SMenu[MN_POTAL_SELECT].nField[4].x=270;
	y=SMenu[MN_POTAL_SELECT].nField[4].y=103;
	SMenu[MN_POTAL_SELECT].nField[4].nSHideNomalCount=22;		// ����
	SMenu[MN_POTAL_SELECT].nField[4].nRectImage=23;				// ����
	SMenu[MN_POTAL_SELECT].nField[4].nImageNumber=22;			// ���� ��ư
	SMenu[MN_POTAL_SELECT].nField[4].nImageType=IIT_POTAL;
	SetRect(SMenu[MN_POTAL_SELECT].nField[4].rCheakBox,x,y,x+17,y+17);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
// ��� ������ �������� �޴�
	SMenu[MN_GUILD_ITEM_GET].x=150;
	SMenu[MN_GUILD_ITEM_GET].y=50;
	SMenu[MN_GUILD_ITEM_GET].nImageNumber= 7;
	SMenu[MN_GUILD_ITEM_GET].nImageType = IIT_GUILD_ITEM;
	SMenu[MN_GUILD_ITEM_GET].nFieldCount = 6;
	SMenu[MN_GUILD_ITEM_GET].key = 15;
	
	// ���� button
	SMenu[MN_GUILD_ITEM_GET].nField[0].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GUILD_ITEM_GET].nField[0].nSpecialWillDo=SWD_SMALL_MENU_CLOSE_CALL_FUCTION;
	x=SMenu[MN_GUILD_ITEM_GET].nField[0].x=24;
	y=SMenu[MN_GUILD_ITEM_GET].nField[0].y=116;
	SMenu[MN_GUILD_ITEM_GET].nField[0].nRectImage=8;
	SMenu[MN_GUILD_ITEM_GET].nField[0].nImageNumber=9;
	SMenu[MN_GUILD_ITEM_GET].nField[0].nImageType = IIT_GUILD_ITEM;
	SMenu[MN_GUILD_ITEM_GET].nField[0].m_pFunction = CallGuildItemPut;
	SetRect(SMenu[MN_GUILD_ITEM_GET].nField[0].rCheakBox,x, y, x+76, y+27);

	// Done button
	SMenu[MN_GUILD_ITEM_GET].nField[1].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GUILD_ITEM_GET].nField[1].nSpecialWillDo=SWD_SUBMENU_BACK;//021007 lsw
	x=SMenu[MN_GUILD_ITEM_GET].nField[1].x=124;
	y=SMenu[MN_GUILD_ITEM_GET].nField[1].y=116;
	SMenu[MN_GUILD_ITEM_GET].nField[1].nRectImage=11;
	SMenu[MN_GUILD_ITEM_GET].nField[1].nImageNumber=10;
	SMenu[MN_GUILD_ITEM_GET].nField[1].nImageType = IIT_GUILD_ITEM;
	SetRect(SMenu[MN_GUILD_ITEM_GET].nField[1].rCheakBox,x, y, x+76, y+27);

	// ��� ��ũ�� ���� �����ֱ�
	SMenu[MN_GUILD_ITEM_GET].nField[2].nType = FT_GUILD_ITEM_GET_ETC;
	x=SMenu[MN_GUILD_ITEM_GET].nField[2].x=31;
	y=SMenu[MN_GUILD_ITEM_GET].nField[2].y=60;
	SMenu[MN_GUILD_ITEM_GET].nField[2].nRectImage=9;
	SMenu[MN_GUILD_ITEM_GET].nField[2].nImageNumber=10;
	SMenu[MN_GUILD_ITEM_GET].nField[2].nImageType = IIT_GUILD_ITEM;
	//SetRect(SMenu[MN_GUILD_ITEM_GET].nField[2].rCheakBox,x, y, x+76, y+27);

	// ������ �����ֱ�
	SMenu[MN_GUILD_ITEM_GET].nField[3].nType=FT_GUILD_ITEM_GET;
	x=SMenu[MN_GUILD_ITEM_GET].nField[3].x=10;
	y=SMenu[MN_GUILD_ITEM_GET].nField[3].y=153;
	SetRect(SMenu[MN_GUILD_ITEM_GET].nField[3].rCheakBox,9,150,185,256);

	// �ؽ�Ʈ ��ũ��
	SMenu[MN_GUILD_ITEM_GET].nField[4].nType=FT_SCROLL_BUTTON;
	SMenu[MN_GUILD_ITEM_GET].nField[4].nWillDo = SCROLL_UP;
	x=SMenu[MN_GUILD_ITEM_GET].nField[4].x=191;
	y=SMenu[MN_GUILD_ITEM_GET].nField[4].y=161;
	SMenu[MN_GUILD_ITEM_GET].nField[4].nSHideNomalCount=58;	// ����
	SMenu[MN_GUILD_ITEM_GET].nField[4].nRectImage=57;			// ����
	SMenu[MN_GUILD_ITEM_GET].nField[4].nImageNumber=57;		// ���� ��ư
	SMenu[MN_GUILD_ITEM_GET].nField[4].nImageType=RUNE_IMG;
	SetRect(SMenu[MN_GUILD_ITEM_GET].nField[4].rCheakBox,x,y,x+17, y+17);

	// �ؽ�Ʈ ��ũ��
	SMenu[MN_GUILD_ITEM_GET].nField[5].nType=FT_SCROLL_BUTTON;
	SMenu[MN_GUILD_ITEM_GET].nField[5].nWillDo = SCROLL_DOWN;
	x=SMenu[MN_GUILD_ITEM_GET].nField[5].x=191;
	y=SMenu[MN_GUILD_ITEM_GET].nField[5].y=231;
	SMenu[MN_GUILD_ITEM_GET].nField[5].nSHideNomalCount=60;		// ����
	SMenu[MN_GUILD_ITEM_GET].nField[5].nRectImage=59;				// ����
	SMenu[MN_GUILD_ITEM_GET].nField[5].nImageNumber=59;			// ���� ��ư
	SMenu[MN_GUILD_ITEM_GET].nField[5].nImageType=RUNE_IMG;
	SetRect(SMenu[MN_GUILD_ITEM_GET].nField[5].rCheakBox,x,y,x+17,y+17);



////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
// ��� ������ �����ϱ� �޴�
	SMenu[MN_GUILD_ITEM_PUT].x=150;
	SMenu[MN_GUILD_ITEM_PUT].y=50;
	SMenu[MN_GUILD_ITEM_PUT].nImageNumber= 5;
	SMenu[MN_GUILD_ITEM_PUT].nImageType = IIT_GUILD_ITEM;
	SMenu[MN_GUILD_ITEM_PUT].nFieldCount = 5;
	SMenu[MN_GUILD_ITEM_PUT].key = 5;
	
	// OK button
	SMenu[MN_GUILD_ITEM_PUT].nField[0].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GUILD_ITEM_PUT].nField[0].nSpecialWillDo=SWD_PUT_GUILD_ITEM;
	x=SMenu[MN_GUILD_ITEM_PUT].nField[0].x=24;
	y=SMenu[MN_GUILD_ITEM_PUT].nField[0].y=221;
	SMenu[MN_GUILD_ITEM_PUT].nField[0].nRectImage=12;
	SMenu[MN_GUILD_ITEM_PUT].nField[0].nImageNumber=13;
	SMenu[MN_GUILD_ITEM_PUT].nField[0].nImageType = IIT_GUILD_ITEM;
	SetRect(SMenu[MN_GUILD_ITEM_PUT].nField[0].rCheakBox,x, y, x+76, y+27);

	// CANCEL button
	SMenu[MN_GUILD_ITEM_PUT].nField[1].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GUILD_ITEM_PUT].nField[1].nSpecialWillDo=SWD_SMALL_MENU_CLOSE_CALL_FUCTION;
	x=SMenu[MN_GUILD_ITEM_PUT].nField[1].x=123;
	y=SMenu[MN_GUILD_ITEM_PUT].nField[1].y=221;
	SMenu[MN_GUILD_ITEM_PUT].nField[1].nRectImage=14;
	SMenu[MN_GUILD_ITEM_PUT].nField[1].nImageNumber=15;
	SMenu[MN_GUILD_ITEM_PUT].nField[1].nImageType = IIT_GUILD_ITEM;
	SMenu[MN_GUILD_ITEM_PUT].nField[1].m_pFunction = CallGetGuildItem;
	SetRect(SMenu[MN_GUILD_ITEM_PUT].nField[1].rCheakBox,x, y, x+76, y+27);


	SMenu[MN_GUILD_ITEM_PUT].nField[2].nType=FT_DATA_PUT;
	SMenu[MN_GUILD_ITEM_PUT].nField[2].x=28;
	SMenu[MN_GUILD_ITEM_PUT].nField[2].y=58;
	SMenu[MN_GUILD_ITEM_PUT].nField[2].nWillDo=RGB16( 200, 200, 200 );
	SMenu[MN_GUILD_ITEM_PUT].nField[2].nImageNumber=DP_RECT_STRING_PUT;
	SMenu[MN_GUILD_ITEM_PUT].nField[2].nRectImage=170;
	sprintf(SMenu[MN_GUILD_ITEM_PUT].nField[2].temp, "%s", kein_GetMenuString( 130 ) );
	
	// ���� �뷮 �����ֱ�
	SMenu[MN_GUILD_ITEM_PUT].nField[3].nType=FT_GUILD_ITEM_PUT_BLANK;
	x=SMenu[MN_GUILD_ITEM_PUT].nField[3].x=28+170;
	y=SMenu[MN_GUILD_ITEM_PUT].nField[3].y=58+18;
	SMenu[MN_GUILD_ITEM_PUT].nField[3].nWillDo=RGB16( 200, 200, 200 );
	SMenu[MN_GUILD_ITEM_PUT].nField[3].nRectImage=170;

	// ������ �÷� ����
	SMenu[MN_GUILD_ITEM_PUT].nField[4].nType=FT_GUILD_ITEM_PUT;
	x=SMenu[MN_GUILD_ITEM_PUT].nField[4].x=100;
	y=SMenu[MN_GUILD_ITEM_PUT].nField[4].y=145;
	SetRect(SMenu[MN_GUILD_ITEM_PUT].nField[4].rCheakBox,17,111,207, 200);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
// ���� ����Ʈ, ��� â�� �б� �޴�
	SMenu[MN_GUILD_ITEM_OR_MEMBER_LIST].x=CENTER_X;
	SMenu[MN_GUILD_ITEM_OR_MEMBER_LIST].y=50;
	SMenu[MN_GUILD_ITEM_OR_MEMBER_LIST].nImageNumber= 0;
	SMenu[MN_GUILD_ITEM_OR_MEMBER_LIST].nImageType = IIT_GUILD_ITEM;
	SMenu[MN_GUILD_ITEM_OR_MEMBER_LIST].nFieldCount = 3;
	
	// call member_list menu
	SMenu[MN_GUILD_ITEM_OR_MEMBER_LIST].nField[0].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GUILD_ITEM_OR_MEMBER_LIST].nField[0].nSpecialWillDo = SWD_SMALL_MENU_CLOSE_CALL_FUCTION;
	x=SMenu[MN_GUILD_ITEM_OR_MEMBER_LIST].nField[0].x=25;
	y=SMenu[MN_GUILD_ITEM_OR_MEMBER_LIST].nField[0].y=63;
	SMenu[MN_GUILD_ITEM_OR_MEMBER_LIST].nField[0].nRectImage=3;
	SMenu[MN_GUILD_ITEM_OR_MEMBER_LIST].nField[0].nImageNumber=4;
	SMenu[MN_GUILD_ITEM_OR_MEMBER_LIST].nField[0].nImageType = IIT_GUILD_ITEM;
	SMenu[MN_GUILD_ITEM_OR_MEMBER_LIST].nField[0].m_pFunction = CallGuildMemberListMenu;
	SetRect(SMenu[MN_GUILD_ITEM_OR_MEMBER_LIST].nField[0].rCheakBox,x, y, x+178, y+59);
	
	// call guild_item_get menu 
	SMenu[MN_GUILD_ITEM_OR_MEMBER_LIST].nField[1].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GUILD_ITEM_OR_MEMBER_LIST].nField[1].nSpecialWillDo = SWD_SMALL_MENU_CLOSE_CALL_FUCTION;
	x=SMenu[MN_GUILD_ITEM_OR_MEMBER_LIST].nField[1].x=25;
	y=SMenu[MN_GUILD_ITEM_OR_MEMBER_LIST].nField[1].y=134;
	SMenu[MN_GUILD_ITEM_OR_MEMBER_LIST].nField[1].nRectImage=1;
	SMenu[MN_GUILD_ITEM_OR_MEMBER_LIST].nField[1].nImageNumber=2;
	SMenu[MN_GUILD_ITEM_OR_MEMBER_LIST].nField[1].nImageType = IIT_GUILD_ITEM;
	SMenu[MN_GUILD_ITEM_OR_MEMBER_LIST].nField[1].m_pFunction = CallGetGuildItem;
	SetRect(SMenu[MN_GUILD_ITEM_OR_MEMBER_LIST].nField[1].rCheakBox,x, y, x+178, y+59);

	// done
	SMenu[MN_GUILD_ITEM_OR_MEMBER_LIST].nField[2].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GUILD_ITEM_OR_MEMBER_LIST].nField[2].nSpecialWillDo=SWD_SMALLMENU_END;
	x=SMenu[MN_GUILD_ITEM_OR_MEMBER_LIST].nField[2].x=74;
	y=SMenu[MN_GUILD_ITEM_OR_MEMBER_LIST].nField[2].y=213;
	SMenu[MN_GUILD_ITEM_OR_MEMBER_LIST].nField[2].nRectImage=11;
	SMenu[MN_GUILD_ITEM_OR_MEMBER_LIST].nField[2].nImageNumber=10;
	SMenu[MN_GUILD_ITEM_OR_MEMBER_LIST].nField[2].nImageType = IIT_GUILD_ITEM;
	SetRect(SMenu[MN_GUILD_ITEM_OR_MEMBER_LIST].nField[2].rCheakBox,x, y, x+76, y+27);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
// Ư�� ������ �ؽ�Ʈ�� ����ϴ� ok cancel �ڽ�
	
	SMenu[MN_OKCANCEL_FILE_TEXT].x=GABX_SCREEN;
	SMenu[MN_OKCANCEL_FILE_TEXT].y=10;
	SMenu[MN_OKCANCEL_FILE_TEXT].nImageNumber=0;
	SMenu[MN_OKCANCEL_FILE_TEXT].nFieldCount=5;

	//OK
	SMenu[MN_OKCANCEL_FILE_TEXT].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_OKCANCEL_FILE_TEXT].nField[0].nSpecialWillDo=SWD_OKCANCEL_OK;
	SMenu[MN_OKCANCEL_FILE_TEXT].nField[0].x=210+193;
	SMenu[MN_OKCANCEL_FILE_TEXT].nField[0].y=20+76;
	SMenu[MN_OKCANCEL_FILE_TEXT].nField[0].nRectImage=646;
	SMenu[MN_OKCANCEL_FILE_TEXT].nField[0].nImageNumber=647;
	SetRect(SMenu[MN_OKCANCEL_FILE_TEXT].nField[0].rCheakBox,208+193,20+76,238+193,47+76);

	//CANCEL
	SMenu[MN_OKCANCEL_FILE_TEXT].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_OKCANCEL_FILE_TEXT].nField[1].nSpecialWillDo=SWD_OKCANCEL_CANCEL;
	SMenu[MN_OKCANCEL_FILE_TEXT].nField[1].x=210+193;
	SMenu[MN_OKCANCEL_FILE_TEXT].nField[1].y=64+76;
	SMenu[MN_OKCANCEL_FILE_TEXT].nField[1].nRectImage=648;
	SMenu[MN_OKCANCEL_FILE_TEXT].nField[1].nImageNumber=649;
	SetRect(SMenu[MN_OKCANCEL_FILE_TEXT].nField[1].rCheakBox,208+193,64+76,238+193,96+76);

	//Message
	SMenu[MN_OKCANCEL_FILE_TEXT].nField[2].nType=FT_DATA_PUT;
	SMenu[MN_OKCANCEL_FILE_TEXT].nField[2].x=32+190;
	SMenu[MN_OKCANCEL_FILE_TEXT].nField[2].y=104;
	SMenu[MN_OKCANCEL_FILE_TEXT].nField[2].nImageNumber=DP_RECT_STRING_PUT;
	SMenu[MN_OKCANCEL_FILE_TEXT].nField[2].nRectImage=153;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	
	// Ability Redistribution Menu
	SMenu[MN_ABILITY_REDISTRIBUTION].x=225;				// LTS LOCALWAR BUG
	SMenu[MN_ABILITY_REDISTRIBUTION].y=50;
	SMenu[MN_ABILITY_REDISTRIBUTION].nImageNumber=0;
	SMenu[MN_ABILITY_REDISTRIBUTION].nImageType=ABILITY_REDISTRIBUTION_IMG;
	SMenu[MN_ABILITY_REDISTRIBUTION].nFieldCount=3;

	// Select button
	SMenu[MN_ABILITY_REDISTRIBUTION].nField[0].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT ;
	SMenu[MN_ABILITY_REDISTRIBUTION].nField[0].nSpecialWillDo=SWD_ABILITY_REDISTRIBUTION;
	SMenu[MN_ABILITY_REDISTRIBUTION].nField[0].x=53;
	SMenu[MN_ABILITY_REDISTRIBUTION].nField[0].y=304;
	SMenu[MN_ABILITY_REDISTRIBUTION].nField[0].nRectImage=1;
	SMenu[MN_ABILITY_REDISTRIBUTION].nField[0].nImageNumber=2;
	SMenu[MN_ABILITY_REDISTRIBUTION].nField[0].nImageType = ABILITY_REDISTRIBUTION_IMG;
	SetRect(SMenu[MN_ABILITY_REDISTRIBUTION].nField[0].rCheakBox,
		SMenu[MN_ABILITY_REDISTRIBUTION].nField[0].x,
		SMenu[MN_ABILITY_REDISTRIBUTION].nField[0].y,
		SMenu[MN_ABILITY_REDISTRIBUTION].nField[0].x+80,
		SMenu[MN_ABILITY_REDISTRIBUTION].nField[0].y+30
		);

	// CANCEL button
	SMenu[MN_ABILITY_REDISTRIBUTION].nField[1].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT ;
	SMenu[MN_ABILITY_REDISTRIBUTION].nField[1].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_ABILITY_REDISTRIBUTION].nField[1].x=189;
	SMenu[MN_ABILITY_REDISTRIBUTION].nField[1].y=304;
	SMenu[MN_ABILITY_REDISTRIBUTION].nField[1].nRectImage=3;
	SMenu[MN_ABILITY_REDISTRIBUTION].nField[1].nImageNumber=4;
	SMenu[MN_ABILITY_REDISTRIBUTION].nField[1].nImageType = ABILITY_REDISTRIBUTION_IMG;
	SetRect(SMenu[MN_ABILITY_REDISTRIBUTION].nField[1].rCheakBox,
		SMenu[MN_ABILITY_REDISTRIBUTION].nField[1].x,
		SMenu[MN_ABILITY_REDISTRIBUTION].nField[1].y,
		SMenu[MN_ABILITY_REDISTRIBUTION].nField[1].x+80,
		SMenu[MN_ABILITY_REDISTRIBUTION].nField[1].y+30);

	SMenu[MN_ABILITY_REDISTRIBUTION].nField[2].nType = FT_DATA_PUT;
	SMenu[MN_ABILITY_REDISTRIBUTION].nField[2].nImageNumber = DP_RECT_STRING_PUT_CUSTOM;
	SMenu[MN_ABILITY_REDISTRIBUTION].nField[2].x = 35;
	SMenu[MN_ABILITY_REDISTRIBUTION].nField[2].y = 65;
	SMenu[MN_ABILITY_REDISTRIBUTION].nField[2].nRectImage = 260;
	strcpy(SMenu[MN_ABILITY_REDISTRIBUTION].nField[2].temp,lan->OutputMessage(0,326));//  "����Ƽ�� ��й� �Ҽ� �ֽ��ϴ�.\n\n ����Ƽ ��й� �ݾ� : 10��ũ��\n\n ����Ƽ ��й迡 ����� �ݾ��� ���࿡ ����Ǿ��� �ݾ��� ����մϴ�. " );

	// LocalWarJoin Menu						// LTS NEW LOCALWAR__________________________    ⼭     
	SMenu[MN_LOCALWAR_JOIN].x = 140;
	SMenu[MN_LOCALWAR_JOIN].y = 15;
	SMenu[MN_LOCALWAR_JOIN].nImageNumber = 0;
	SMenu[MN_LOCALWAR_JOIN].nImageType = LOCALWAR_JOIN_IMG;
	SMenu[MN_LOCALWAR_JOIN].nFieldCount = 13;

	SMenu[MN_LOCALWAR_JOIN].nField[0].nType = FT_LOCALWAR_JOIN;
	SMenu[MN_LOCALWAR_JOIN].nField[0].nWillDo = 0;
	SMenu[MN_LOCALWAR_JOIN].nField[0].x = 260;
	SMenu[MN_LOCALWAR_JOIN].nField[0].y = 92;
	SMenu[MN_LOCALWAR_JOIN].nField[0].nRectImage = 1;
	SMenu[MN_LOCALWAR_JOIN].nField[0].nImageNumber = 1;
	SMenu[MN_LOCALWAR_JOIN].nField[0].nImageType = LOCALWAR_JOIN_IMG;
	SetRect(SMenu[MN_LOCALWAR_JOIN].nField[0].rCheakBox,
		SMenu[MN_LOCALWAR_JOIN].nField[0].x,
		SMenu[MN_LOCALWAR_JOIN].nField[0].y,
		SMenu[MN_LOCALWAR_JOIN].nField[0].x + 80,
		SMenu[MN_LOCALWAR_JOIN].nField[0].y + 30
	);

	SMenu[MN_LOCALWAR_JOIN].nField[1].nType = FT_LOCALWAR_JOIN;
	SMenu[MN_LOCALWAR_JOIN].nField[1].nWillDo = 0;
	SMenu[MN_LOCALWAR_JOIN].nField[1].x = 96;
	SMenu[MN_LOCALWAR_JOIN].nField[1].y = 126;
	SMenu[MN_LOCALWAR_JOIN].nField[1].nRectImage = 2;
	SMenu[MN_LOCALWAR_JOIN].nField[1].nImageNumber = 2;
	SMenu[MN_LOCALWAR_JOIN].nField[1].nImageType = LOCALWAR_JOIN_IMG;
	SetRect(SMenu[MN_LOCALWAR_JOIN].nField[1].rCheakBox,
		SMenu[MN_LOCALWAR_JOIN].nField[1].x,
		SMenu[MN_LOCALWAR_JOIN].nField[1].y,
		SMenu[MN_LOCALWAR_JOIN].nField[1].x + 80,
		SMenu[MN_LOCALWAR_JOIN].nField[1].y + 30);

	SMenu[MN_LOCALWAR_JOIN].nField[2].nType = FT_LOCALWAR_JOIN;
	SMenu[MN_LOCALWAR_JOIN].nField[2].nWillDo = 0;
	SMenu[MN_LOCALWAR_JOIN].nField[2].x = 222;
	SMenu[MN_LOCALWAR_JOIN].nField[2].y = 224;
	SMenu[MN_LOCALWAR_JOIN].nField[2].nRectImage = 3;
	SMenu[MN_LOCALWAR_JOIN].nField[2].nImageNumber = 3;
	SMenu[MN_LOCALWAR_JOIN].nField[2].nImageType = LOCALWAR_JOIN_IMG;
	SetRect(SMenu[MN_LOCALWAR_JOIN].nField[2].rCheakBox,
		SMenu[MN_LOCALWAR_JOIN].nField[2].x,
		SMenu[MN_LOCALWAR_JOIN].nField[2].y,
		SMenu[MN_LOCALWAR_JOIN].nField[2].x + 80,
		SMenu[MN_LOCALWAR_JOIN].nField[2].y + 30);

	SMenu[MN_LOCALWAR_JOIN].nField[3].nType = FT_LOCALWAR_JOIN;
	SMenu[MN_LOCALWAR_JOIN].nField[3].nWillDo = 0;
	SMenu[MN_LOCALWAR_JOIN].nField[3].x = 346;
	SMenu[MN_LOCALWAR_JOIN].nField[3].y = 156;
	SMenu[MN_LOCALWAR_JOIN].nField[3].nRectImage = 4;
	SMenu[MN_LOCALWAR_JOIN].nField[3].nImageNumber = 4;
	SMenu[MN_LOCALWAR_JOIN].nField[3].nImageType = LOCALWAR_JOIN_IMG;
	SetRect(SMenu[MN_LOCALWAR_JOIN].nField[3].rCheakBox,
		SMenu[MN_LOCALWAR_JOIN].nField[3].x,
		SMenu[MN_LOCALWAR_JOIN].nField[3].y,
		SMenu[MN_LOCALWAR_JOIN].nField[3].x + 80,
		SMenu[MN_LOCALWAR_JOIN].nField[3].y + 30);

	SMenu[MN_LOCALWAR_JOIN].nField[4].nType = FT_LOCALWAR_JOIN;
	SMenu[MN_LOCALWAR_JOIN].nField[4].nWillDo = 0;
	SMenu[MN_LOCALWAR_JOIN].nField[4].x = 42;
	SMenu[MN_LOCALWAR_JOIN].nField[4].y = 146;
	SMenu[MN_LOCALWAR_JOIN].nField[4].nRectImage = 5;
	SMenu[MN_LOCALWAR_JOIN].nField[4].nImageNumber = 5;
	SMenu[MN_LOCALWAR_JOIN].nField[4].nImageType = LOCALWAR_JOIN_IMG;
	SetRect(SMenu[MN_LOCALWAR_JOIN].nField[4].rCheakBox,
		SMenu[MN_LOCALWAR_JOIN].nField[4].x,
		SMenu[MN_LOCALWAR_JOIN].nField[4].y,
		SMenu[MN_LOCALWAR_JOIN].nField[4].x + 80,
		SMenu[MN_LOCALWAR_JOIN].nField[4].y + 30);

	SMenu[MN_LOCALWAR_JOIN].nField[5].nType = FT_LOCALWAR_JOIN;
	SMenu[MN_LOCALWAR_JOIN].nField[5].nWillDo = 0;
	SMenu[MN_LOCALWAR_JOIN].nField[5].x = 237;
	SMenu[MN_LOCALWAR_JOIN].nField[5].y = 267;
	SMenu[MN_LOCALWAR_JOIN].nField[5].nRectImage = 6;
	SMenu[MN_LOCALWAR_JOIN].nField[5].nImageNumber = 6;
	SMenu[MN_LOCALWAR_JOIN].nField[5].nImageType = LOCALWAR_JOIN_IMG;
	SetRect(SMenu[MN_LOCALWAR_JOIN].nField[5].rCheakBox,
		SMenu[MN_LOCALWAR_JOIN].nField[5].x,
		SMenu[MN_LOCALWAR_JOIN].nField[5].y,
		SMenu[MN_LOCALWAR_JOIN].nField[5].x + 80,
		SMenu[MN_LOCALWAR_JOIN].nField[5].y + 30);

	SMenu[MN_LOCALWAR_JOIN].nField[6].nType = FT_LOCALWAR_JOIN;
	SMenu[MN_LOCALWAR_JOIN].nField[6].nWillDo = 0;
	SMenu[MN_LOCALWAR_JOIN].nField[6].x = 394;
	SMenu[MN_LOCALWAR_JOIN].nField[6].y = 217;
	SMenu[MN_LOCALWAR_JOIN].nField[6].nRectImage = 7;
	SMenu[MN_LOCALWAR_JOIN].nField[6].nImageNumber = 7;
	SMenu[MN_LOCALWAR_JOIN].nField[6].nImageType = LOCALWAR_JOIN_IMG;
	SetRect(SMenu[MN_LOCALWAR_JOIN].nField[6].rCheakBox,
		SMenu[MN_LOCALWAR_JOIN].nField[6].x,
		SMenu[MN_LOCALWAR_JOIN].nField[6].y,
		SMenu[MN_LOCALWAR_JOIN].nField[6].x + 80,
		SMenu[MN_LOCALWAR_JOIN].nField[6].y + 30);

	SMenu[MN_LOCALWAR_JOIN].nField[7].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_LOCALWAR_JOIN].nField[7].nSpecialWillDo = SWD_LOCALWAR_JOIN;
	SMenu[MN_LOCALWAR_JOIN].nField[7].x = 88;
	SMenu[MN_LOCALWAR_JOIN].nField[7].y = 402;
	SMenu[MN_LOCALWAR_JOIN].nField[7].nRectImage = 29;
	SMenu[MN_LOCALWAR_JOIN].nField[7].nImageNumber = 30;
	SMenu[MN_LOCALWAR_JOIN].nField[7].nImageType = LOCALWAR_JOIN_IMG;
	SetRect(SMenu[MN_LOCALWAR_JOIN].nField[7].rCheakBox,
		SMenu[MN_LOCALWAR_JOIN].nField[7].x,
		SMenu[MN_LOCALWAR_JOIN].nField[7].y,
		SMenu[MN_LOCALWAR_JOIN].nField[7].x + 80,
		SMenu[MN_LOCALWAR_JOIN].nField[7].y + 30);

	SMenu[MN_LOCALWAR_JOIN].nField[8].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_LOCALWAR_JOIN].nField[8].nSpecialWillDo = SWD_LOCALWAR_END;
	SMenu[MN_LOCALWAR_JOIN].nField[8].x = 344;
	SMenu[MN_LOCALWAR_JOIN].nField[8].y = 402;
	SMenu[MN_LOCALWAR_JOIN].nField[8].nRectImage = 32;
	SMenu[MN_LOCALWAR_JOIN].nField[8].nImageNumber = 33;
	SMenu[MN_LOCALWAR_JOIN].nField[8].nImageType = LOCALWAR_JOIN_IMG;
	SetRect(SMenu[MN_LOCALWAR_JOIN].nField[8].rCheakBox,
		SMenu[MN_LOCALWAR_JOIN].nField[8].x,
		SMenu[MN_LOCALWAR_JOIN].nField[8].y,
		SMenu[MN_LOCALWAR_JOIN].nField[8].x + 80,
		SMenu[MN_LOCALWAR_JOIN].nField[8].y + 30);

	// BY Point String
	SMenu[MN_LOCALWAR_JOIN].nField[9].nType = FT_DATA_PUT;
	SMenu[MN_LOCALWAR_JOIN].nField[9].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_LOCALWAR_JOIN].nField[9].x = 130;
	SMenu[MN_LOCALWAR_JOIN].nField[9].y = 217;
	SMenu[MN_LOCALWAR_JOIN].nField[9].nRectImage = 300;
	strcpy(SMenu[MN_LOCALWAR_JOIN].nField[9].temp, "0000");

	// ZY Point String
	SMenu[MN_LOCALWAR_JOIN].nField[10].nType = FT_DATA_PUT;
	SMenu[MN_LOCALWAR_JOIN].nField[10].nImageNumber = DP_RECT_STRING_PUT_CUSTOM;
	SMenu[MN_LOCALWAR_JOIN].nField[10].x = 290;
	SMenu[MN_LOCALWAR_JOIN].nField[10].y = 327;
	SMenu[MN_LOCALWAR_JOIN].nField[10].nRectImage = 300;
	strcpy(SMenu[MN_LOCALWAR_JOIN].nField[10].temp, "0000");

	// YL Point String
	SMenu[MN_LOCALWAR_JOIN].nField[11].nType = FT_DATA_PUT;
	SMenu[MN_LOCALWAR_JOIN].nField[11].nImageNumber = DP_RECT_STRING_PUT_CUSTOM;
	SMenu[MN_LOCALWAR_JOIN].nField[11].x = 430;
	SMenu[MN_LOCALWAR_JOIN].nField[11].y = 137;
	SMenu[MN_LOCALWAR_JOIN].nField[11].nRectImage = 300;
	strcpy(SMenu[MN_LOCALWAR_JOIN].nField[11].temp, "0000");

	SMenu[MN_LOCALWAR_JOIN].nField[12].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;		// LTS NEW LOCALWAR
	SMenu[MN_LOCALWAR_JOIN].nField[12].nSpecialWillDo = SWD_LOCALWAR_MAPMOVE;
	SMenu[MN_LOCALWAR_JOIN].nField[12].x = 217;
	SMenu[MN_LOCALWAR_JOIN].nField[12].y = 402;
	SMenu[MN_LOCALWAR_JOIN].nField[12].nRectImage = 34;
	SMenu[MN_LOCALWAR_JOIN].nField[12].nImageNumber = 35;
	SMenu[MN_LOCALWAR_JOIN].nField[12].nImageType = LOCALWAR_JOIN_IMG;
	SetRect(SMenu[MN_LOCALWAR_JOIN].nField[12].rCheakBox,
		SMenu[MN_LOCALWAR_JOIN].nField[12].x,
		SMenu[MN_LOCALWAR_JOIN].nField[12].y,
		SMenu[MN_LOCALWAR_JOIN].nField[12].x + 80,
		SMenu[MN_LOCALWAR_JOIN].nField[12].y + 30);		// LTS NEW LOCALWAR__________________________        

	//                   ̽ 
	SMenu[MN_LOCALWAR_OK].x = 240;													// LTS LOCALWAR MENU
	SMenu[MN_LOCALWAR_OK].y = 55;
	SMenu[MN_LOCALWAR_OK].nImageNumber = 0;
	SMenu[MN_LOCALWAR_OK].work = 0;					//  ⺻   ÷     ޼   
	SMenu[MN_LOCALWAR_OK].nImageType = LOCALWAR_OK_IMG;
	SMenu[MN_LOCALWAR_OK].nFieldCount = 4;

	// OK   ư
	SMenu[MN_LOCALWAR_OK].nField[0].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_LOCALWAR_OK].nField[0].nSpecialWillDo = SWD_SMALLMENU_END;
	SMenu[MN_LOCALWAR_OK].nField[0].x = 123;
	SMenu[MN_LOCALWAR_OK].nField[0].y = 305;
	SMenu[MN_LOCALWAR_OK].nField[0].nRectImage = 2;
	SMenu[MN_LOCALWAR_OK].nField[0].nImageNumber = 1;
	SMenu[MN_LOCALWAR_OK].nField[0].nImageType = LOCALWAR_OK_IMG;
	SetRect(SMenu[MN_LOCALWAR_OK].nField[0].rCheakBox, SMenu[MN_LOCALWAR_OK].nField[0].x, SMenu[MN_LOCALWAR_OK].nField[0].y,
		SMenu[MN_LOCALWAR_OK].nField[0].x + 100, SMenu[MN_LOCALWAR_OK].nField[0].y + 30);

	// <   ư
	SMenu[MN_LOCALWAR_OK].nField[1].nType = FT_SHOW_SPECIAL_WILLDO_AUTO_PUT;		// LTS LOCALWAR MENU
	SMenu[MN_LOCALWAR_OK].nField[1].nSpecialWillDo = SWD_LW_DONE_PREV;
	SMenu[MN_LOCALWAR_OK].nField[1].x = 272;
	SMenu[MN_LOCALWAR_OK].nField[1].y = 70;
	SMenu[MN_LOCALWAR_OK].nField[1].nRectImage = 4;
	SMenu[MN_LOCALWAR_OK].nField[1].nImageNumber = 3;
	SMenu[MN_LOCALWAR_OK].nField[1].nImageType = LOCALWAR_OK_IMG;
	SetRect(SMenu[MN_LOCALWAR_OK].nField[1].rCheakBox, SMenu[MN_LOCALWAR_OK].nField[1].x, SMenu[MN_LOCALWAR_OK].nField[1].y,
		SMenu[MN_LOCALWAR_OK].nField[1].x + 20, SMenu[MN_LOCALWAR_OK].nField[1].y + 20);

	// >   ư
	SMenu[MN_LOCALWAR_OK].nField[2].nType = FT_SHOW_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_LOCALWAR_OK].nField[2].nSpecialWillDo = SWD_LW_DONE_NEXT;
	SMenu[MN_LOCALWAR_OK].nField[2].x = 272;
	SMenu[MN_LOCALWAR_OK].nField[2].y = 255;
	SMenu[MN_LOCALWAR_OK].nField[2].nRectImage = 6;
	SMenu[MN_LOCALWAR_OK].nField[2].nImageNumber = 5;
	SMenu[MN_LOCALWAR_OK].nField[2].nImageType = LOCALWAR_OK_IMG;
	SetRect(SMenu[MN_LOCALWAR_OK].nField[2].rCheakBox, SMenu[MN_LOCALWAR_OK].nField[2].x, SMenu[MN_LOCALWAR_OK].nField[2].y,
		SMenu[MN_LOCALWAR_OK].nField[2].x + 20, SMenu[MN_LOCALWAR_OK].nField[2].y + 20);

	//     //020815 lsw
	SMenu[MN_LOCALWAR_OK].nField[3].nType = FT_DATA_PUT;
	SMenu[MN_LOCALWAR_OK].nField[3].nImageNumber = DP_RECT_STRING_PUT_CUSTOM;
	SMenu[MN_LOCALWAR_OK].nField[3].x = 40;
	SMenu[MN_LOCALWAR_OK].nField[3].y = 60;
	SMenu[MN_LOCALWAR_OK].nField[3].nRectImage = 230;
	SMenu[MN_LOCALWAR_OK].nField[3].nImageType = 0;
	strcpy(SMenu[MN_LOCALWAR_OK].nField[3].temp, "");

	// 020115 LTS
	SMenu[MN_EVENT_LOCALWAR_OK].x = 225;				// LTS LOCALWAR BUG
	SMenu[MN_EVENT_LOCALWAR_OK].y = 50;
	SMenu[MN_EVENT_LOCALWAR_OK].nImageNumber = 0;
	SMenu[MN_EVENT_LOCALWAR_OK].nImageType = EVENT_LOCALWAR_OK_IMG;
	SMenu[MN_EVENT_LOCALWAR_OK].nFieldCount = 3;

	SMenu[MN_EVENT_LOCALWAR_OK].nField[0].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_EVENT_LOCALWAR_OK].nField[0].nSpecialWillDo = SWD_SMALLMENU_END;
	SMenu[MN_EVENT_LOCALWAR_OK].nField[0].x = 177;
	SMenu[MN_EVENT_LOCALWAR_OK].nField[0].y = 305;
	SMenu[MN_EVENT_LOCALWAR_OK].nField[0].nRectImage = 2;
	SMenu[MN_EVENT_LOCALWAR_OK].nField[0].nImageNumber = 1;
	SMenu[MN_EVENT_LOCALWAR_OK].nField[0].nImageType = EVENT_LOCALWAR_OK_IMG;
	SetRect(SMenu[MN_EVENT_LOCALWAR_OK].nField[0].rCheakBox,
		SMenu[MN_EVENT_LOCALWAR_OK].nField[0].x,
		SMenu[MN_EVENT_LOCALWAR_OK].nField[0].y,
		SMenu[MN_EVENT_LOCALWAR_OK].nField[0].x + 80,
		SMenu[MN_EVENT_LOCALWAR_OK].nField[0].y + 30
	);

	SMenu[MN_EVENT_LOCALWAR_OK].nField[1].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_EVENT_LOCALWAR_OK].nField[1].nSpecialWillDo = SWD_EVENT_LOCALWAR_JOIN;
	SMenu[MN_EVENT_LOCALWAR_OK].nField[1].x = 65;
	SMenu[MN_EVENT_LOCALWAR_OK].nField[1].y = 305;
	SMenu[MN_EVENT_LOCALWAR_OK].nField[1].nRectImage = 4;
	SMenu[MN_EVENT_LOCALWAR_OK].nField[1].nImageNumber = 3;
	SMenu[MN_EVENT_LOCALWAR_OK].nField[1].nImageType = EVENT_LOCALWAR_OK_IMG;
	SetRect(SMenu[MN_EVENT_LOCALWAR_OK].nField[1].rCheakBox,
		SMenu[MN_EVENT_LOCALWAR_OK].nField[1].x,
		SMenu[MN_EVENT_LOCALWAR_OK].nField[1].y,
		SMenu[MN_EVENT_LOCALWAR_OK].nField[1].x + 80,
		SMenu[MN_EVENT_LOCALWAR_OK].nField[1].y + 30);

	SMenu[MN_EVENT_LOCALWAR_OK].nField[2].nType = FT_DATA_PUT;
	SMenu[MN_EVENT_LOCALWAR_OK].nField[2].nImageNumber = DP_RECT_STRING_PUT_CUSTOM;
	SMenu[MN_EVENT_LOCALWAR_OK].nField[2].x = 40;
	SMenu[MN_EVENT_LOCALWAR_OK].nField[2].y = 65;
	SMenu[MN_EVENT_LOCALWAR_OK].nField[2].nRectImage = 250;
	strcpy(SMenu[MN_EVENT_LOCALWAR_OK].nField[2].temp, lan->OutputMessage(0, 360));		// LTS BUG
	// 020115 LTS


	// 020115 LTS
	SMenu[MN_EVENT_LOCALWAR_MOVE].x = 225;				// LTS LOCALWAR BUG
	SMenu[MN_EVENT_LOCALWAR_MOVE].y = 50;
	SMenu[MN_EVENT_LOCALWAR_MOVE].nImageNumber = 0;
	SMenu[MN_EVENT_LOCALWAR_MOVE].nImageType = EVENT_LOCALWAR_OK_IMG;
	SMenu[MN_EVENT_LOCALWAR_MOVE].nFieldCount = 3;

	SMenu[MN_EVENT_LOCALWAR_MOVE].nField[0].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_EVENT_LOCALWAR_MOVE].nField[0].nSpecialWillDo = SWD_SMALLMENU_END;
	SMenu[MN_EVENT_LOCALWAR_MOVE].nField[0].x = 177;
	SMenu[MN_EVENT_LOCALWAR_MOVE].nField[0].y = 305;
	SMenu[MN_EVENT_LOCALWAR_MOVE].nField[0].nRectImage = 2;
	SMenu[MN_EVENT_LOCALWAR_MOVE].nField[0].nImageNumber = 1;
	SMenu[MN_EVENT_LOCALWAR_MOVE].nField[0].nImageType = EVENT_LOCALWAR_MOVE_IMG;
	SetRect(SMenu[MN_EVENT_LOCALWAR_MOVE].nField[0].rCheakBox,
		SMenu[MN_EVENT_LOCALWAR_MOVE].nField[0].x,
		SMenu[MN_EVENT_LOCALWAR_MOVE].nField[0].y,
		SMenu[MN_EVENT_LOCALWAR_MOVE].nField[0].x + 80,
		SMenu[MN_EVENT_LOCALWAR_MOVE].nField[0].y + 30
	);

	SMenu[MN_EVENT_LOCALWAR_MOVE].nField[1].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_EVENT_LOCALWAR_MOVE].nField[1].nSpecialWillDo = SWD_EVENT_LOCALWAR_MOVE;
	SMenu[MN_EVENT_LOCALWAR_MOVE].nField[1].x = 65;
	SMenu[MN_EVENT_LOCALWAR_MOVE].nField[1].y = 305;
	SMenu[MN_EVENT_LOCALWAR_MOVE].nField[1].nRectImage = 4;
	SMenu[MN_EVENT_LOCALWAR_MOVE].nField[1].nImageNumber = 3;
	SMenu[MN_EVENT_LOCALWAR_MOVE].nField[1].nImageType = EVENT_LOCALWAR_MOVE_IMG;
	SetRect(SMenu[MN_EVENT_LOCALWAR_MOVE].nField[1].rCheakBox,
		SMenu[MN_EVENT_LOCALWAR_MOVE].nField[1].x,
		SMenu[MN_EVENT_LOCALWAR_MOVE].nField[1].y,
		SMenu[MN_EVENT_LOCALWAR_MOVE].nField[1].x + 80,
		SMenu[MN_EVENT_LOCALWAR_MOVE].nField[1].y + 30);

	SMenu[MN_EVENT_LOCALWAR_MOVE].nField[2].nType = FT_DATA_PUT;
	SMenu[MN_EVENT_LOCALWAR_MOVE].nField[2].nImageNumber = DP_RECT_STRING_PUT_CUSTOM;
	SMenu[MN_EVENT_LOCALWAR_MOVE].nField[2].x = 40;
	SMenu[MN_EVENT_LOCALWAR_MOVE].nField[2].y = 65;
	SMenu[MN_EVENT_LOCALWAR_MOVE].nField[2].nRectImage = 250;
	strcpy(SMenu[MN_EVENT_LOCALWAR_MOVE].nField[2].temp, lan->OutputMessage(0, 361));		// LTS BUG


////////////////////////////////////////////////////////////////////////////////////////////////////////////////	

///////////////////////////////////////////////////////////////////////////////////////////
// html ������ text ��� �ϴ� �޼��� �ڽ�
	SMenu[MN_HTML_MESSAGEBOX].x=290;
	SMenu[MN_HTML_MESSAGEBOX].y=50;
	SMenu[MN_HTML_MESSAGEBOX].nImageNumber=13;
	SMenu[MN_HTML_MESSAGEBOX].nImageType=SMALL_ETC;
	SMenu[MN_HTML_MESSAGEBOX].nFieldCount=4;
	SMenu[MN_HTML_MESSAGEBOX].key2=8;
	
	//OK
	SMenu[MN_HTML_MESSAGEBOX].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_HTML_MESSAGEBOX].nField[0].nSpecialWillDo=SWD_DEFAULT_MESSAGEBOX_OK;
	SMenu[MN_HTML_MESSAGEBOX].nField[0].x=64;
	SMenu[MN_HTML_MESSAGEBOX].nField[0].y=252;
	SMenu[MN_HTML_MESSAGEBOX].nField[0].nRectImage=12;
	SMenu[MN_HTML_MESSAGEBOX].nField[0].nImageNumber=11;
	SMenu[MN_HTML_MESSAGEBOX].nField[0].nImageType=SMALL_ETC;
	SetRect(SMenu[MN_HTML_MESSAGEBOX].nField[0].rCheakBox,65,252,145,273);

	// �ؽ�Ʈ ��ũ��
	SMenu[MN_HTML_MESSAGEBOX].nField[1].nType=FT_SCROLL_BUTTON;
	SMenu[MN_HTML_MESSAGEBOX].nField[1].nWillDo = SCROLL_UP;
	x=SMenu[MN_HTML_MESSAGEBOX].nField[1].x=165;
	y=SMenu[MN_HTML_MESSAGEBOX].nField[1].y=88;
	SMenu[MN_HTML_MESSAGEBOX].nField[1].nShideNomalPlus=2;		// ������ ��ư x, y ��ǥ gab
	SMenu[MN_HTML_MESSAGEBOX].nField[1].nSHideNomalCount=2;		// ����
	SMenu[MN_HTML_MESSAGEBOX].nField[1].nRectImage=1;				// ����
	SMenu[MN_HTML_MESSAGEBOX].nField[1].nImageNumber=0;			// ���� ��ư
	SMenu[MN_HTML_MESSAGEBOX].nField[1].nImageType=SALVATION_PCX;
	SetRect(SMenu[MN_HTML_MESSAGEBOX].nField[1].rCheakBox,x,y,x+18,y+30);

	// �ؽ�Ʈ ��ũ��
	SMenu[MN_HTML_MESSAGEBOX].nField[2].nType=FT_SCROLL_BUTTON;
	SMenu[MN_HTML_MESSAGEBOX].nField[2].nWillDo = SCROLL_DOWN;
	x=SMenu[MN_HTML_MESSAGEBOX].nField[2].x=165;
	y=SMenu[MN_HTML_MESSAGEBOX].nField[2].y=202;
	SMenu[MN_HTML_MESSAGEBOX].nField[2].nShideNomalPlus=2;		// ������ ��ư x, y ��ǥ gab
	SMenu[MN_HTML_MESSAGEBOX].nField[2].nSHideNomalCount=5;		// ����
	SMenu[MN_HTML_MESSAGEBOX].nField[2].nRectImage=4;				// ����
	SMenu[MN_HTML_MESSAGEBOX].nField[2].nImageNumber=3;			// ���� ��ư
	SMenu[MN_HTML_MESSAGEBOX].nField[2].nImageType=SALVATION_PCX;
	SetRect(SMenu[MN_HTML_MESSAGEBOX].nField[2].rCheakBox,x,y,x+18,y+30);

	// ���� ���
	SMenu[MN_HTML_MESSAGEBOX].nField[3].nType=FT_VIEW_TEXT_BY_HTML;
	SMenu[MN_HTML_MESSAGEBOX].nField[3].x=40;
	SMenu[MN_HTML_MESSAGEBOX].nField[3].y=102;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
// ���� ��� ������ �޴�
	
	SMenu[MN_NATION_ITEM].x=CENTER_X;
	SMenu[MN_NATION_ITEM].y=10;
	SMenu[MN_NATION_ITEM].nImageNumber=0;
	SMenu[MN_NATION_ITEM].nImageType=10000;
	SMenu[MN_NATION_ITEM].nFieldCount=4;

	//YES
	SMenu[MN_NATION_ITEM].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_ITEM].nField[0].nSpecialWillDo=SWD_NATION_ITEM_OK;
	x = SMenu[MN_NATION_ITEM].nField[0].x=40;
	y = SMenu[MN_NATION_ITEM].nField[0].y=308;
	SMenu[MN_NATION_ITEM].nField[0].nRectImage=3;
	SMenu[MN_NATION_ITEM].nField[0].nImageNumber=4;
	SMenu[MN_NATION_ITEM].nField[0].nImageType=10000;
	SetRect(SMenu[MN_NATION_ITEM].nField[0].rCheakBox,x,y,x+76,y+27);

	//NO
	SMenu[MN_NATION_ITEM].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_ITEM].nField[1].nSpecialWillDo=SWD_SMALLMENU_END;
	x = SMenu[MN_NATION_ITEM].nField[1].x=135;
	y = SMenu[MN_NATION_ITEM].nField[1].y=308;
	SMenu[MN_NATION_ITEM].nField[1].nRectImage=1;
	SMenu[MN_NATION_ITEM].nField[1].nImageNumber=2;
	SMenu[MN_NATION_ITEM].nField[1].nImageType=10000;
	SetRect(SMenu[MN_NATION_ITEM].nField[1].rCheakBox,x,y,x+76,y+27);

	SMenu[MN_NATION_ITEM].nField[2].nType=FT_DATA_PUT;
	SMenu[MN_NATION_ITEM].nField[2].x=35;
	SMenu[MN_NATION_ITEM].nField[2].y=113;
	SMenu[MN_NATION_ITEM].nField[2].nImageNumber=DP_RECT_STRING_PUT;
	SMenu[MN_NATION_ITEM].nField[2].nRectImage=188;
	strcpy( SMenu[MN_NATION_ITEM].nField[2].temp, lan->OutputMessage(7,172) );

	SMenu[MN_NATION_ITEM].nField[3].nType=FT_NATION_ITEM;
	SMenu[MN_NATION_ITEM].nField[3].x=39;
	SMenu[MN_NATION_ITEM].nField[3].y=70;
	SetRect(SMenu[MN_NATION_ITEM].nField[3].rCheakBox,21, 51, 58, 87);
	

////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
// ���� ��� ������ �޴�
	SMenu[MN_GUILD_HOUSE].x=CENTER_X;
	SMenu[MN_GUILD_HOUSE].y=50;
	SMenu[MN_GUILD_HOUSE].nImageNumber=0;
	SMenu[MN_GUILD_HOUSE].nImageType=10001;
	SMenu[MN_GUILD_HOUSE].nFieldCount=6;
	SMenu[MN_GUILD_HOUSE].key=8;

	//����
	SMenu[MN_GUILD_HOUSE].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GUILD_HOUSE].nField[0].nSpecialWillDo=SWD_GUILD_HOUSE_OK;
	x = SMenu[MN_GUILD_HOUSE].nField[0].x=45;
	y = SMenu[MN_GUILD_HOUSE].nField[0].y=272;
	SMenu[MN_GUILD_HOUSE].nField[0].nRectImage=1;
	SMenu[MN_GUILD_HOUSE].nField[0].nImageNumber=2;
	SMenu[MN_GUILD_HOUSE].nField[0].nImageType=10001;
	SetRect(SMenu[MN_GUILD_HOUSE].nField[0].rCheakBox,x,y,x+76,y+27);

	//���
	SMenu[MN_GUILD_HOUSE].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GUILD_HOUSE].nField[1].nSpecialWillDo=SWD_SMALLMENU_END;
	x = SMenu[MN_GUILD_HOUSE].nField[1].x=191;
	y = SMenu[MN_GUILD_HOUSE].nField[1].y=272;
	SMenu[MN_GUILD_HOUSE].nField[1].nRectImage=3;
	SMenu[MN_GUILD_HOUSE].nField[1].nImageNumber=4;
	SMenu[MN_GUILD_HOUSE].nField[1].nImageType=10001;
	SetRect(SMenu[MN_GUILD_HOUSE].nField[1].rCheakBox,x,y,x+76,y+27);

	//SMenu[MN_GUILD_HOUSE].nField[2].nType=FT_DATA_PUT;
	SMenu[MN_GUILD_HOUSE].nField[2].x=35;
	SMenu[MN_GUILD_HOUSE].nField[2].y=113;
	SMenu[MN_GUILD_HOUSE].nField[2].nImageNumber=DP_RECT_STRING_PUT;
	SMenu[MN_GUILD_HOUSE].nField[2].nRectImage=188;
	strcpy( SMenu[MN_GUILD_HOUSE].nField[2].temp, lan->OutputMessage(7,172) );

	SMenu[MN_GUILD_HOUSE].nField[3].nType=FT_GUILD_HOUSE;
	SMenu[MN_GUILD_HOUSE].nField[3].x=0;
	SMenu[MN_GUILD_HOUSE].nField[3].y=87;
	SMenu[MN_GUILD_HOUSE].nField[3].nImageType=10001;
	SetRect(SMenu[MN_GUILD_HOUSE].nField[3].rCheakBox,23, 53, 297, 255 );

	// �ؽ�Ʈ ��ũ��
	SMenu[MN_GUILD_HOUSE].nField[4].nType=FT_SCROLL_BUTTON;
	SMenu[MN_GUILD_HOUSE].nField[4].nWillDo = SCROLL_UP;
	x=SMenu[MN_GUILD_HOUSE].nField[4].x=265;
	y=SMenu[MN_GUILD_HOUSE].nField[4].y=233;
	SMenu[MN_GUILD_HOUSE].nField[4].nSHideNomalCount=58;	// ����
	SMenu[MN_GUILD_HOUSE].nField[4].nRectImage=57;			// ����
	SMenu[MN_GUILD_HOUSE].nField[4].nImageNumber=57;		// ���� ��ư
	SMenu[MN_GUILD_HOUSE].nField[4].nImageType=RUNE_IMG;
	SetRect(SMenu[MN_GUILD_HOUSE].nField[4].rCheakBox,x,y,x+17, y+17);

	// �ؽ�Ʈ ��ũ��
	SMenu[MN_GUILD_HOUSE].nField[5].nType=FT_SCROLL_BUTTON;
	SMenu[MN_GUILD_HOUSE].nField[5].nWillDo = SCROLL_DOWN;
	x=SMenu[MN_GUILD_HOUSE].nField[5].x=243;
	y=SMenu[MN_GUILD_HOUSE].nField[5].y=233;
	SMenu[MN_GUILD_HOUSE].nField[5].nSHideNomalCount=60;		// ����
	SMenu[MN_GUILD_HOUSE].nField[5].nRectImage=59;				// ����
	SMenu[MN_GUILD_HOUSE].nField[5].nImageNumber=59;			// ���� ��ư
	SMenu[MN_GUILD_HOUSE].nField[5].nImageType=RUNE_IMG;
	SetRect(SMenu[MN_GUILD_HOUSE].nField[5].rCheakBox,x,y,x+17,y+17);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
// ������ ��� �Ͽ콺 ���� �޴�
	SMenu[MN_GUILD_HOUSE_INFO].x=CENTER_X;
	SMenu[MN_GUILD_HOUSE_INFO].y=50;
	SMenu[MN_GUILD_HOUSE_INFO].nImageNumber=10;
	SMenu[MN_GUILD_HOUSE_INFO].nImageType=10001;
	SMenu[MN_GUILD_HOUSE_INFO].nFieldCount=6;

	//��û
	SMenu[MN_GUILD_HOUSE_INFO].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GUILD_HOUSE_INFO].nField[0].nSpecialWillDo=SWD_GUILD_HOUSE_BUY;
	x = SMenu[MN_GUILD_HOUSE_INFO].nField[0].x=53;
	y = SMenu[MN_GUILD_HOUSE_INFO].nField[0].y=304;
	SMenu[MN_GUILD_HOUSE_INFO].nField[0].nRectImage=6;
	SMenu[MN_GUILD_HOUSE_INFO].nField[0].nImageNumber=7;
	SMenu[MN_GUILD_HOUSE_INFO].nField[0].nImageType=10001;
	SetRect(SMenu[MN_GUILD_HOUSE_INFO].nField[0].rCheakBox,x,y,x+76,y+27);

	//���
	SMenu[MN_GUILD_HOUSE_INFO].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_GUILD_HOUSE_INFO].nField[1].nSpecialWillDo=SWD_SMALLMENU_END;
	x = SMenu[MN_GUILD_HOUSE_INFO].nField[1].x=189;
	y = SMenu[MN_GUILD_HOUSE_INFO].nField[1].y=304;
	SMenu[MN_GUILD_HOUSE_INFO].nField[1].nRectImage=3;
	SMenu[MN_GUILD_HOUSE_INFO].nField[1].nImageNumber=4;
	SMenu[MN_GUILD_HOUSE_INFO].nField[1].nImageType=10001;
	SetRect(SMenu[MN_GUILD_HOUSE_INFO].nField[1].rCheakBox,x,y,x+76,y+27);

	SMenu[MN_GUILD_HOUSE_INFO].nField[3].nType=FT_GUILD_HOUSE_INFO;
	SMenu[MN_GUILD_HOUSE_INFO].nField[3].x=0;
	SMenu[MN_GUILD_HOUSE_INFO].nField[3].y=87;
	SMenu[MN_GUILD_HOUSE_INFO].nField[3].nImageType=10001;
	SetRect(SMenu[MN_GUILD_HOUSE_INFO].nField[3].rCheakBox,23, 53, 297, 255 );

	{
	const int iMnIndex	= MN_WAR_START;
	const int iImg		= WAR_START_IMG;
	int iFtNum = 0;

	SMenuReg(iMnIndex, CENTER_X, 65, iImg,0,15);	//300 ¥�� �������̽� �̴ϱ�//�����̶� 0 0 �̴�
	
	// 
	SMenuFTReg(iMnIndex,iFtNum++,FT_NOMAL_PUT, 0,		// ���� 
			20, 52, 80, 30, iImg, 4, 4);
	SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SMALLMENU_END, //��ư 
			363, 310, 109, 29, iImg, 6, 7);
	SMenuFTReg(iMnIndex,iFtNum++,FT_NOMAL_PUT, 0,		// �����
			188, 125, 280, 200, iImg, 9, 9);
	SMenuFTReg(iMnIndex,iFtNum++,FT_NOMAL_PUT, 0,		// ȭ��ǥ
			116, 200, 180, 300, iImg, 12, 12);
	SMenuFTReg(iMnIndex,iFtNum++,FT_NOMAL_PUT, 0,		// ������
			55, 120, 180, 200, iImg,10,10);
	strcpy(SMenu[iMnIndex].nField[iFtNum].temp,  "������ ���۵Ǿ����ϴ�." );		// ����
	SMenuFTReg(iMnIndex,iFtNum++,FT_DATA_PUT, 0, 
			330, 70, 80, 30, 0, 170, DP_RECT_STRING_PUT);
	}

	{
	const int iMnIndex	= MN_WAR_MOVE;
	const int iImg		= WAR_MOVE_IMG;
	int iFtNum = 0;

	SMenuReg(iMnIndex, CENTER_X, 65, iImg,0,15);	//300 ¥�� �������̽� �̴ϱ�//�����̶� 0 0 �̴�
	
	// 
	SMenuFTReg(iMnIndex,iFtNum++,FT_NOMAL_PUT, 0, 
			18, 40, 80, 30, iImg, 1, 1);

	SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SMALLMENU_END, 
			154, 301, 110, 30, iImg, 9, 10);		// ��� ��ư

	SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_WAR_MOVE, 
			14, 301, 110, 30, iImg, 6, 7);			// �̵���ư

	SMenuFTReg(iMnIndex,iFtNum++,FT_NOMAL_PUT, 0, 
			184, 207, 80, 30, iImg, 12, 12);				// ������ ������
	SMenuFTReg(iMnIndex,iFtNum++,FT_NOMAL_PUT, 0, 
			65, 92, 80, 30, iImg, 13, 13);					// ����� ���̼��� 
	SMenuFTReg(iMnIndex,iFtNum++,FT_NOMAL_PUT, 0, 
			135, 157, 80, 30, iImg, 11, 11);					// ȭ��ǥ
	}

	{
	const int iMnIndex	= MN_WAR_STATUS;
	const int iImg		= WAR_STATUS_IMG;
	int iFtNum = 0;
	SMenuReg(iMnIndex, CENTER_X, 65, iImg,0,20);	//300 ¥�� �������̽� �̴ϱ�//�����̶� 0 0 �̴�
	SMenuFTReg(iMnIndex,iFtNum++,FT_NOMAL_PUT, 0, 
								172, 7, 342, 34, iImg, 18, 18);				// ������ ���� 
	SMenuFTReg(iMnIndex,iFtNum++,FT_NOMAL_PUT, 0, 
								20, 46, 80, 30, iImg, 3, 3);				// ������ �׸�
	
	SMenuFTReg(iMnIndex,iFtNum++,FT_NOMAL_PUT, 0, 
								0, 0, 80, 30, iImg, 13, 13);				// ���� ��ġ

	SMenuFTReg(iMnIndex,iFtNum++,FT_NOMAL_PUT, 0, 
								 78, 95, 80, 30, iImg,  4,  4);				// �������� 
	SMenuFTReg(iMnIndex,iFtNum++,FT_NOMAL_PUT, 0, 
								 78, 131, 80, 30, iImg,  4,  4);				// 1����
	SMenuFTReg(iMnIndex,iFtNum++,FT_NOMAL_PUT, 0, 
								115, 95, 80, 30, iImg, 4, 4);					// 2����
	SMenuFTReg(iMnIndex,iFtNum++,FT_NOMAL_PUT, 0, 
								115, 131, 80, 30, iImg, 5, 5);				// 3����
	SMenuFTReg(iMnIndex,iFtNum++,FT_NOMAL_PUT, 0, 
								197, 95, 80, 30, iImg, 4, 4);				// ��������
	SMenuFTReg(iMnIndex,iFtNum++,FT_NOMAL_PUT, 0, 
								199, 131, 80, 30, iImg, 4, 4);				// 1����
	SMenuFTReg(iMnIndex,iFtNum++,FT_NOMAL_PUT, 0, 
								236, 95, 80, 30, iImg, 4, 4);				// 2����
	SMenuFTReg(iMnIndex,iFtNum++,FT_NOMAL_PUT, 0, 
								236, 131, 80, 30, iImg, 5, 5);				// 3����

	strcpy(SMenu[iMnIndex].nField[iFtNum].temp,  "�����ð�" );		// ����
	SMenuFTReg(iMnIndex,iFtNum++,FT_DATA_PUT, 0, 
			417, 73, 80, 30, 0, 170, DP_RECT_STRING_PUT);
	strcpy(SMenu[iMnIndex].nField[iFtNum].temp,  "���̼���" );		// ����
	SMenuFTReg(iMnIndex,iFtNum++,FT_DATA_PUT, 0, 
			417,162, 80, 30, 0, 170, DP_RECT_STRING_PUT);
	strcpy(SMenu[iMnIndex].nField[iFtNum].temp,  "������" );		// ����
	SMenuFTReg(iMnIndex,iFtNum++,FT_DATA_PUT, 0, 
			417,255, 80, 30, 0, 170, DP_RECT_STRING_PUT);

	SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SMALLMENU_END, // Done��ư
			356, 302, 76, 27, iImg, 2, 1);		// ��� ��ư

	//<! BBD 040311
	SMenuFTReg(iMnIndex,iFtNum++,FT_NOMAL_PUT, 0, 
								326,158, 77, 116, iImg, 21, 21);				// ���̼��� & ������ ����

	//> BBD 040311

//	SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_WAR_MOVE,			// �������� ��ư
//			432, 270, 82, 23, iImg, 6, 5);			// �̵���ư

	}


	// 020701 YGI
////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
// MN_SCENARIO_TRAP_INFO
	SMenuReg(MN_SCENARIO_TRAP_INFO, CENTER_X, 40,SCENARIO_IMG,1,6);//021007 lsw
	//OK		01// 6�� ¥�� Done
//	SMenuFTReg(MN_SCENARIO_TRAP_INFO, 1, FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, 
//						SWD_SMALLMENU_END, 212, 259, 283-212, 283-259,10022, 5, 6);
	//CANCEL	02
	SMenuFTReg(MN_SCENARIO_TRAP_INFO, 2, FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, 
						SWD_SMALLMENU_END, 163, 256, 228-163, 275-256,SCENARIO_IMG, 2, 3);//021007 lsw

	SMenuFTReg(MN_SCENARIO_TRAP_INFO, 3, FT_SCENARIO_TRAP_INFO, 
						0, 299, 259, 0, 0,SCENARIO_IMG, 0, 0);//021007 lsw

////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
// MN_SCENARIO_TRAP_KEY
	SMenuReg(MN_SCENARIO_TRAP_KEY, CENTER_X, 40,SCENARIO_IMG,0,6);//021007 lsw
	//OK		01// 6�� ¥�� Done
//	SMenuFTReg(MN_SCENARIO_TRAP_KEY, 1, FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, 
//						SWD_SMALLMENU_END, 212, 259, 283-212, 283-259,10022, 5, 6);
	//CANCEL	02
	SMenuFTReg(MN_SCENARIO_TRAP_KEY, 2, FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, 
						SWD_SMALLMENU_END, 163, 253, 228-163, 275-253,SCENARIO_IMG, 2, 3);

	SMenuFTReg(MN_SCENARIO_TRAP_KEY, 3, FT_SCENARIO_TRAP_KEY, 
						0, 299, 259, 0, 0,SCENARIO_IMG, 0, 0);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////	

////////////////////////////////////////////////////////////////////////////////////////////////////////////////	


	lsw_menu_set();// 010907 LTS

////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	kh_menu_set();		// ���� �۾�







}


void CommonMenuSet()
{
	// 020701 YGI
	int x, y;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//SYSTEM�޴����� LOGOUT �޴�

	SMenu[MN_SYSTEM_LOGOUT].x=193+GABX_SCREEN+240;
	SMenu[MN_SYSTEM_LOGOUT].y=75;//+GABY_SCREEN;
	SMenu[MN_SYSTEM_LOGOUT].nImageNumber=645;
	SMenu[MN_SYSTEM_LOGOUT].nFieldCount=4;
	SMenu[MN_SYSTEM_LOGOUT].CheakType=0;

	//OK
	SMenu[MN_SYSTEM_LOGOUT].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_SYSTEM_LOGOUT].nField[0].nSpecialWillDo=SWD_QUIT;
	SMenu[MN_SYSTEM_LOGOUT].nField[0].fLButtonDown = false;
	SMenu[MN_SYSTEM_LOGOUT].nField[0].fCheakFlag = false;
	SMenu[MN_SYSTEM_LOGOUT].nField[0].x=210;
	SMenu[MN_SYSTEM_LOGOUT].nField[0].y=20;
	SMenu[MN_SYSTEM_LOGOUT].nField[0].nRectImage=646;
	SMenu[MN_SYSTEM_LOGOUT].nField[0].nImageNumber=647;
	SetRect(SMenu[MN_SYSTEM_LOGOUT].nField[0].rCheakBox,208,17,238,47);

	//CANCEL
	SMenu[MN_SYSTEM_LOGOUT].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_SYSTEM_LOGOUT].nField[1].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_SYSTEM_LOGOUT].nField[1].x=210;
	SMenu[MN_SYSTEM_LOGOUT].nField[1].y=64;
	SMenu[MN_SYSTEM_LOGOUT].nField[1].nRectImage=648;
	SMenu[MN_SYSTEM_LOGOUT].nField[1].nImageNumber=649;
	SetRect(SMenu[MN_SYSTEM_LOGOUT].nField[1].rCheakBox,208,66,238,96);

	//Message
	SMenu[MN_SYSTEM_LOGOUT].nField[2].nType=FT_DATA_PUT;
	SMenu[MN_SYSTEM_LOGOUT].nField[2].x=34;
	SMenu[MN_SYSTEM_LOGOUT].nField[2].y=34;
	SMenu[MN_SYSTEM_LOGOUT].nField[2].nImageNumber=DP_RECT_STRING_PUT;
	SMenu[MN_SYSTEM_LOGOUT].nField[2].nRectImage=147;
	sprintf(SMenu[MN_SYSTEM_LOGOUT].nField[2].temp,"%s", lan->OutputMessage(3,204));//lsw

	// ����Ű
	SMenu[MN_SYSTEM_LOGOUT].nField[3].nType=FT_FIRST_QUICK_KEY;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// YESNO �޴�      // CallYesNo() �Լ��� �̿��Ͽ� ��ǥ���� �־��ش�.
	//���� �޴� ����
	SMenu[MN_YESNO].nImageNumber=242;
	SMenu[MN_YESNO].nFieldCount=2;
	SMenu[MN_YESNO].CheakType=0;

	SMenu[MN_YESNO].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_PUT;
	SMenu[MN_YESNO].nField[0].nSpecialWillDo=SWD_YESNO_YES;
	SMenu[MN_YESNO].nField[0].x=25;
	SMenu[MN_YESNO].nField[0].y=15;
	SMenu[MN_YESNO].nField[0].fCheakFlag=FALSE;
	SMenu[MN_YESNO].nField[0].fLButtonDown=FALSE;
	SMenu[MN_YESNO].nField[0].nImageNumber=243;
	SetRect(SMenu[MN_YESNO].nField[0].rCheakBox,25,15,81,36);

	SMenu[MN_YESNO].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_PUT;
	SMenu[MN_YESNO].nField[1].nSpecialWillDo=SWD_YESNO_NO;
	SMenu[MN_YESNO].nField[1].x=32;
	SMenu[MN_YESNO].nField[1].y=43;
	SMenu[MN_YESNO].nField[1].fCheakFlag=FALSE;
	SMenu[MN_YESNO].nField[1].fLButtonDown=FALSE;
	SMenu[MN_YESNO].nField[1].nImageNumber=244;
	SetRect(SMenu[MN_YESNO].nField[1].rCheakBox,35,43,75,65);
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
//			�޽��� �ڽ�

	SMenu[MN_MESSAGEBOX_BIG].nImageNumber=485;
	SMenu[MN_MESSAGEBOX_BIG].nFieldCount=1;

//	SMenu[MN_MESSAGEBOX_BIG].nField[0].nType=FT_DO;
//	SMenu[MN_MESSAGEBOX_BIG].nField[0].nWillDo=DO_TEXT_PUT;
//	SMenu[MN_MESSAGEBOX_BIG].nField[0].temp
	
	
	SMenu[MN_MESSAGEBOX_SMALL].nImageNumber=484;
	SMenu[MN_MESSAGEBOX_SMALL].nFieldCount=1;

//	SMenu[MN_MESSAGEBOX_SMALL].nField[0].nType=FT_DO;
//	SMenu[MN_MESSAGEBOX_SMALL].nField[0].nWillDo=DO_TEXT_PUT;


/////////////////////////////////////////////////////////////////////////////////////////
// OK, CACEL �޸� �޽��� �ڽ�
	
	SMenu[MN_OKCANCEL_MESSAGEBOX].x=GABX_SCREEN;
	SMenu[MN_OKCANCEL_MESSAGEBOX].y=0;
	SMenu[MN_OKCANCEL_MESSAGEBOX].nImageNumber=0;
	SMenu[MN_OKCANCEL_MESSAGEBOX].nFieldCount=5;

//	SMenu[MN_OKCANCEL_MESSAGEBOX].nField[0].nType=FT_NOMAL_PUT_FX;
//	SMenu[MN_OKCANCEL_MESSAGEBOX].nField[0].x=111;
//	SMenu[MN_OKCANCEL_MESSAGEBOX].nField[0].y=0;
//	SMenu[MN_OKCANCEL_MESSAGEBOX].nField[0].nImageNumber=15;
	
	SMenu[MN_OKCANCEL_MESSAGEBOX].nField[1].nType=FT_NOMAL_PUT;
	SMenu[MN_OKCANCEL_MESSAGEBOX].nField[1].x=193;
	SMenu[MN_OKCANCEL_MESSAGEBOX].nField[1].y=76;
	SMenu[MN_OKCANCEL_MESSAGEBOX].nField[1].nImageNumber=645;

	//OK
	SMenu[MN_OKCANCEL_MESSAGEBOX].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_OKCANCEL_MESSAGEBOX].nField[2].nSpecialWillDo=SWD_OKCANCEL_OK;
	SMenu[MN_OKCANCEL_MESSAGEBOX].nField[2].x=210+193;
	SMenu[MN_OKCANCEL_MESSAGEBOX].nField[2].y=20+76;
	SMenu[MN_OKCANCEL_MESSAGEBOX].nField[2].nRectImage=646;
	SMenu[MN_OKCANCEL_MESSAGEBOX].nField[2].nImageNumber=647;
	SetRect(SMenu[MN_OKCANCEL_MESSAGEBOX].nField[2].rCheakBox,208+193,20+76,238+193,47+76);

	//CANCEL
	SMenu[MN_OKCANCEL_MESSAGEBOX].nField[3].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_OKCANCEL_MESSAGEBOX].nField[3].nSpecialWillDo=SWD_OKCANCEL_CANCEL;
	SMenu[MN_OKCANCEL_MESSAGEBOX].nField[3].x=210+193;
	SMenu[MN_OKCANCEL_MESSAGEBOX].nField[3].y=64+76;
	SMenu[MN_OKCANCEL_MESSAGEBOX].nField[3].nRectImage=648;
	SMenu[MN_OKCANCEL_MESSAGEBOX].nField[3].nImageNumber=649;
	SetRect(SMenu[MN_OKCANCEL_MESSAGEBOX].nField[3].rCheakBox,208+193,64+76,238+193,96+76);

	//Message
	SMenu[MN_OKCANCEL_MESSAGEBOX].nField[4].nType=FT_DATA_PUT;
	SMenu[MN_OKCANCEL_MESSAGEBOX].nField[4].x=32+190;
	SMenu[MN_OKCANCEL_MESSAGEBOX].nField[4].y=104;
	SMenu[MN_OKCANCEL_MESSAGEBOX].nField[4].nImageNumber=DP_RECT_STRING_PUT;
	SMenu[MN_OKCANCEL_MESSAGEBOX].nField[4].nRectImage=153;
//	sprintf(SMenu[MN_OKCANCEL_MESSAGEBOX].nField[4].temp,"������ Logout �Ͻðڽ��ϱ�?");


/////////////////////////////////////////////////////////////////////////////////////////
// OK �޸� �޽��� �ڽ�
	
	SMenu[MN_OK_MESSAGEBOX].x=GABY_SCREEN;
	SMenu[MN_OK_MESSAGEBOX].y=0;
	SMenu[MN_OK_MESSAGEBOX].nImageNumber=0;
	SMenu[MN_OK_MESSAGEBOX].nFieldCount=5;

//	SMenu[MN_OK_MESSAGEBOX].nField[0].nType=FT_NOMAL_PUT_FX;
//	SMenu[MN_OK_MESSAGEBOX].nField[0].x=111;
//	SMenu[MN_OK_MESSAGEBOX].nField[0].y=0;
//	SMenu[MN_OK_MESSAGEBOX].nField[0].nImageNumber=15;
	
	SMenu[MN_OK_MESSAGEBOX].nField[1].nType=FT_NOMAL_PUT;
	SMenu[MN_OK_MESSAGEBOX].nField[1].x=193;
	SMenu[MN_OK_MESSAGEBOX].nField[1].y=76;
	SMenu[MN_OK_MESSAGEBOX].nField[1].nImageNumber=645;

	// ok
	SMenu[MN_OK_MESSAGEBOX].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_OK_MESSAGEBOX].nField[2].nSpecialWillDo=SWD_ONLY_OK;
	SMenu[MN_OK_MESSAGEBOX].nField[2].x=210+193;
	SMenu[MN_OK_MESSAGEBOX].nField[2].y=20+76;
	SMenu[MN_OK_MESSAGEBOX].nField[2].nRectImage=646;
	SMenu[MN_OK_MESSAGEBOX].nField[2].nImageNumber=647;
	SetRect(SMenu[MN_OK_MESSAGEBOX].nField[2].rCheakBox,210+193,20+76,246+193,55+76);

	//Message
	SMenu[MN_OK_MESSAGEBOX].nField[3].nType=FT_DATA_PUT;
	SMenu[MN_OK_MESSAGEBOX].nField[3].x=32+187;
	SMenu[MN_OK_MESSAGEBOX].nField[3].y=104;
	SMenu[MN_OK_MESSAGEBOX].nField[3].nImageNumber=DP_RECT_STRING_PUT;
	SMenu[MN_OK_MESSAGEBOX].nField[3].nRectImage=163;
//	sprintf(SMenu[MN_OK_MESSAGEBOX].nField[3].temp,"������ Logout �Ͻðڽ��ϱ�?");

	// cancel ����
	SMenu[MN_OK_MESSAGEBOX].nField[4].nType=FT_NOMAL_PUT;
	SMenu[MN_OK_MESSAGEBOX].nField[4].x=210+193;
	SMenu[MN_OK_MESSAGEBOX].nField[4].y=64+76;
	SMenu[MN_OK_MESSAGEBOX].nField[4].nImageNumber=706;

////////////////////////////////////////////////////////////////////////////////////////
// ��ũ��Ʈ���� ����ϴ� �޽��� �ڽ�
	// 020701 YGI

	SMenu[MN_DEFAULT_MESSAGEBOX].x=290;
	SMenu[MN_DEFAULT_MESSAGEBOX].y=50;
	SMenu[MN_DEFAULT_MESSAGEBOX].nImageNumber=13;
	SMenu[MN_DEFAULT_MESSAGEBOX].nImageType=SMALL_ETC;
	SMenu[MN_DEFAULT_MESSAGEBOX].nFieldCount=4;
	SMenu[MN_DEFAULT_MESSAGEBOX].key=6;
	
	//OK
	SMenu[MN_DEFAULT_MESSAGEBOX].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_DEFAULT_MESSAGEBOX].nField[0].nSpecialWillDo=SWD_DEFAULT_MESSAGEBOX_OK;
	SMenu[MN_DEFAULT_MESSAGEBOX].nField[0].x=64;
	SMenu[MN_DEFAULT_MESSAGEBOX].nField[0].y=252;
	SMenu[MN_DEFAULT_MESSAGEBOX].nField[0].nRectImage=12;
	SMenu[MN_DEFAULT_MESSAGEBOX].nField[0].nImageNumber=11;
	SMenu[MN_DEFAULT_MESSAGEBOX].nField[0].nImageType=SMALL_ETC;
	SetRect(SMenu[MN_DEFAULT_MESSAGEBOX].nField[0].rCheakBox,65,252,145,273);

	// �ؽ�Ʈ ��ũ��
	SMenu[MN_DEFAULT_MESSAGEBOX].nField[1].nType=FT_SCROLL_BUTTON;
	SMenu[MN_DEFAULT_MESSAGEBOX].nField[1].nWillDo = SCROLL_UP;
	x=SMenu[MN_DEFAULT_MESSAGEBOX].nField[1].x=165;
	y=SMenu[MN_DEFAULT_MESSAGEBOX].nField[1].y=88;
	SMenu[MN_DEFAULT_MESSAGEBOX].nField[1].nShideNomalPlus=2;		// ������ ��ư x, y ��ǥ gab
	SMenu[MN_DEFAULT_MESSAGEBOX].nField[1].nSHideNomalCount=2;		// ����
	SMenu[MN_DEFAULT_MESSAGEBOX].nField[1].nRectImage=1;				// ����
	SMenu[MN_DEFAULT_MESSAGEBOX].nField[1].nImageNumber=0;			// ���� ��ư
	SMenu[MN_DEFAULT_MESSAGEBOX].nField[1].nImageType=SALVATION_PCX;
	SetRect(SMenu[MN_DEFAULT_MESSAGEBOX].nField[1].rCheakBox,x,y,x+18,y+30);

	// �ؽ�Ʈ ��ũ��
	SMenu[MN_DEFAULT_MESSAGEBOX].nField[2].nType=FT_SCROLL_BUTTON;
	SMenu[MN_DEFAULT_MESSAGEBOX].nField[2].nWillDo = SCROLL_DOWN;
	x=SMenu[MN_DEFAULT_MESSAGEBOX].nField[2].x=165;
	y=SMenu[MN_DEFAULT_MESSAGEBOX].nField[2].y=202;
	SMenu[MN_DEFAULT_MESSAGEBOX].nField[2].nShideNomalPlus=2;		// ������ ��ư x, y ��ǥ gab
	SMenu[MN_DEFAULT_MESSAGEBOX].nField[2].nSHideNomalCount=5;		// ����
	SMenu[MN_DEFAULT_MESSAGEBOX].nField[2].nRectImage=4;				// ����
	SMenu[MN_DEFAULT_MESSAGEBOX].nField[2].nImageNumber=3;			// ���� ��ư
	SMenu[MN_DEFAULT_MESSAGEBOX].nField[2].nImageType=SALVATION_PCX;
	SetRect(SMenu[MN_DEFAULT_MESSAGEBOX].nField[2].rCheakBox,x,y,x+18,y+30);

	//Message
	SMenu[MN_DEFAULT_MESSAGEBOX].nField[3].nType=FT_SCROLL_TEXT_PUT;
	SMenu[MN_DEFAULT_MESSAGEBOX].nField[3].x=35;
	SMenu[MN_DEFAULT_MESSAGEBOX].nField[3].y=98;
	SMenu[MN_DEFAULT_MESSAGEBOX].nField[3].nSpecialWillDo = 1;
	SMenu[MN_DEFAULT_MESSAGEBOX].nField[3].nRectImage=130;
	SMenu[MN_DEFAULT_MESSAGEBOX].nField[3].nWillDo = RGB16( 255, 255, 255);
	SMenu[MN_DEFAULT_MESSAGEBOX].nField[3].m_lpTemp = SMenu[MN_DEFAULT_MESSAGEBOX].nField[3].temp;
////////////////////////////////////////////////////////////////////////////////////////


	// acer4
	SMenu[MN_DEFAULT_MESSAGEBOX2].x=290;
	SMenu[MN_DEFAULT_MESSAGEBOX2].y=50;
	SMenu[MN_DEFAULT_MESSAGEBOX2].nImageNumber=13;
	SMenu[MN_DEFAULT_MESSAGEBOX2].nImageType=SMALL_ETC;
	SMenu[MN_DEFAULT_MESSAGEBOX2].nFieldCount=2;
	
	//OK
	SMenu[MN_DEFAULT_MESSAGEBOX2].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_DEFAULT_MESSAGEBOX2].nField[0].nSpecialWillDo=SWD_DEFAULT_MESSAGEBOX_OK;
	SMenu[MN_DEFAULT_MESSAGEBOX2].nField[0].x=64;
	SMenu[MN_DEFAULT_MESSAGEBOX2].nField[0].y=252;
	SMenu[MN_DEFAULT_MESSAGEBOX2].nField[0].nRectImage=12;
	SMenu[MN_DEFAULT_MESSAGEBOX2].nField[0].nImageNumber=11;
	SMenu[MN_DEFAULT_MESSAGEBOX2].nField[0].nImageType=SMALL_ETC;
	SetRect(SMenu[MN_DEFAULT_MESSAGEBOX2].nField[0].rCheakBox,65,252,145,273);

	//Message
	SMenu[MN_DEFAULT_MESSAGEBOX2].nField[1].nType=FT_RETURN_FUCTION;

}
