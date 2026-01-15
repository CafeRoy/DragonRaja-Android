#include "StdAfx.h"
#include "Hangul.h"
#include "dragon.h"
#include "Menu.h"
#include "Hong_Sprite.h"
#include "SmallMenuSet.h"
#include <cstdio>
#include "directsound.h"
#include "Char.h"
#include "SmallMenu.h"
#include "Hong_Sub.h"
#include "Effect.h"
#include "map.h"
#include "Tool.h"
#include "MenuSet.h"
#include "Skill.h"
#include "MenuNetwork.h"
#include "MouseCursor.h"
#include "CharDataTable.h"
#include "gameproc.h"
#include "kh_cpp.h"
#include "NationSys.h"
#include "nationsupport.h"
#include "menudef2.h"
#include "skill_lsw.h"
#include "LineCommand.h"
#include "MagicSys.h"
#include "CombatSys.h"
#include "Chat.h"
#include "TextOutMgr.h"
#include "auction.h"
#include "HunterSystem.h"
#include "exchangeMgr.h"
#include "MailMgr.h"
#include "ArenaManager.h"
#include "DualMgr.h"
extern void SetItemPos(int type, int p1, int p2, int p3, POS* IP_set);
extern int GetSkillMother(int kind, int skill_mother[], int max);
extern void SWD_LOGON_Clicked(const int nMenuIndex);
const int HPMPBarHeight = 91;//HP MP ������ ����
const int SPBarWidth	= 118;//HP MP ������ ����
const int EXPBarWidth	= 223;//EXP ������ ����

extern POS			IP_base;
extern int CraftItem;
extern int refresh_magery_time;
extern int g_LocalWarBegin;			// 011215 LTS
extern bool g_bWarBegin;			// 011215 LTS
extern int g_iScenarioOn;
void DisplayTacGauge(SMENU *SubMenu,int j, const int x, const int y, const int iMax, const int iNow, const int iLv,int refresh,int first_exp);
void CallWisperList(const bool bOpen);
void ExplainAuctionItem( int x, int y, ItemAttr *olg_item ,const bool bIsDurDraw);//020821-2 lsw
extern MakeItem make_item_result;
extern void SendNationPacket(int type);//010730 lsw
extern bool isNationWarfieldServer();
extern int CheckNationStatus(int StatusNo);
extern int WarResultAniCount;
extern RareMain Soksungattr_gm;
extern MakeItem gm_make_item_result;
extern int gm_make_item_result_needitem[2];
extern MakeItem gm_make_item_list[MAX_UPGRADE_RARE];
extern bool LOCK_ITEM;
extern bool DoButtonCheckOfMenu_lsw(int i , int j);
static int iClickPointX=g_pointMouseX, iClickPointY = g_pointMouseY;
extern SMENU SMenu[MAX_MENU];
extern SHN SHideNomal[MAX_SHN][MAX_SHN_FIELD];
extern Spr		god_spr[GOD_MENU_IMG_MAX];		// ���� �޴� 
extern void SetRect(RECT &rect,int left, int top, int right, int bottom);
extern void JustMsg(char *s, ... );
extern void DisplayMailIconExplain(const int iX, const int iY,const int iMailType);
int WarResultAniCount = 0;
const int extra_quickmemory_start_index = 8;

DWORD Convert16to32(const WORD wColor)
{
	return RGB( (wColor>>11 & 0x1f) << 3, 
				(wColor>>6  & 0x1f) << 3,
				(wColor     & 0x1f) << 3);
}

void SMenuReg(int iMainType,int x, int y , int iImageType,int iImageNumber, int iFieldCount,int iCheakType,int iTemp,int iWork,int iKey )
{
	if( iMainType <= 0 || iMainType >= MAX_MENU)
	{
		JustMsg("InitMenu Failed");
		return;
	} 

	SMENU *pMenu = &SMenu[iMainType];
	pMenu->x			=	x;
	pMenu->y			=	y;
	pMenu->nImageNumber	=	iImageNumber;
	pMenu->nImageType	=	iImageType;
	pMenu->nFieldCount	=	iFieldCount;
	pMenu->CheakType	=	iCheakType;
	pMenu->nTemp		=	iTemp;				//�� �ܰ� �޴� ��ȣ�� ������ �ִ�.
	pMenu->work			=	iWork;				//���� ���� �� �ӽ� ����
	pMenu->key			=	iKey;				//����Ű ������ ��...
}

void SMenuFTReg(const int iMainType,const int iFieldNo,const int iButtonType,const int iSpecailWillDo,
						const int iX, const int iY,const int iWidth, const int iHeight,
						const int iImageType, const int iRectImage, const int iImageNumber,const int iWillDo)
{
	if( iMainType <= 0 || iMainType >= MAX_MENU)
	{
		JustMsg("InitMenu Failed");
		return;
	} 
	SMENU *pMenu	= &SMenu[iMainType];
	if( iFieldNo >= pMenu->nFieldCount)
	{
		JustMsg("Field Reg Failed %d %d",iMainType,iFieldNo);
		return;
	}
	SFIELD *pField	= &pMenu->nField[iFieldNo];

	pField->nType = iButtonType;
	pField->nSpecialWillDo=iSpecailWillDo;
	pField->x = iX;
	pField->y = iY;
	pField->nRectImage = iRectImage;
	pField->nImageNumber = iImageNumber;
	pField->nImageType = iImageType;
	pField->nWillDo = iWillDo;
	::SetRect(pField->rCheakBox, iX, iY, iX+iWidth, iY+iHeight);
	
	return;
}

void lsw_menu_set()
{
///////////////////////////////////////////////////////////////////////////////////////////	
//010708 lsw 010730 
//���� ������
	{
	::SMenuReg(MN_NATION_BATTLE_BBS,CENTER_X, 100,NATION_BATTLE_BBS_IMG,0,10);

	//���Ｓ�� NATION_NOW			OK		����â �����.
	SMenu[MN_NATION_BATTLE_BBS].nField[0].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_BATTLE_BBS].nField[0].nSpecialWillDo=SWD_NATION_DECSRIPT;
	SMenu[MN_NATION_BATTLE_BBS].nField[0].x = 42;
	SMenu[MN_NATION_BATTLE_BBS].nField[0].y = 54;
	SMenu[MN_NATION_BATTLE_BBS].nField[0].nRectImage = 12;
	SMenu[MN_NATION_BATTLE_BBS].nField[0].nImageNumber = 11;
	SMenu[MN_NATION_BATTLE_BBS].nField[0].nImageType = NATION_BATTLE_BBS_IMG;
	::SetRect(SMenu[MN_NATION_BATTLE_BBS].nField[0].rCheakBox,
	SMenu[MN_NATION_BATTLE_BBS].nField[0].x,SMenu[MN_NATION_BATTLE_BBS].nField[0].y,
	SMenu[MN_NATION_BATTLE_BBS].nField[0].x+110,SMenu[MN_NATION_BATTLE_BBS].nField[0].y+30);

	//�����Ȳ NATION_NOW			OK
	SMenu[MN_NATION_BATTLE_BBS].nField[2].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_BATTLE_BBS].nField[2].nSpecialWillDo=SWD_NATION_NOW;
	SMenu[MN_NATION_BATTLE_BBS].nField[2].nWillDo=MN_NATION_NOW;
	SMenu[MN_NATION_BATTLE_BBS].nField[2].x = 42;
	SMenu[MN_NATION_BATTLE_BBS].nField[2].y = 84;
	SMenu[MN_NATION_BATTLE_BBS].nField[2].nRectImage = 10;
	SMenu[MN_NATION_BATTLE_BBS].nField[2].nImageNumber = 9;
	SMenu[MN_NATION_BATTLE_BBS].nField[2].nImageType = NATION_BATTLE_BBS_IMG;
	::SetRect(SMenu[MN_NATION_BATTLE_BBS].nField[2].rCheakBox,
	SMenu[MN_NATION_BATTLE_BBS].nField[2].x,SMenu[MN_NATION_BATTLE_BBS].nField[2].y,
	SMenu[MN_NATION_BATTLE_BBS].nField[2].x+110,SMenu[MN_NATION_BATTLE_BBS].nField[2].y+30);

	//�������� NATION JOIN				OK
	SMenu[MN_NATION_BATTLE_BBS].nField[3].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_BATTLE_BBS].nField[3].nSpecialWillDo=SWD_NATION_JOIN;
	SMenu[MN_NATION_BATTLE_BBS].nField[3].nWillDo=MN_NATION_JOIN;
	SMenu[MN_NATION_BATTLE_BBS].nField[3].x = 42;
	SMenu[MN_NATION_BATTLE_BBS].nField[3].y = 114;
	SMenu[MN_NATION_BATTLE_BBS].nField[3].nRectImage = 14;
	SMenu[MN_NATION_BATTLE_BBS].nField[3].nImageNumber = 13;
	SMenu[MN_NATION_BATTLE_BBS].nField[3].nImageType = NATION_BATTLE_BBS_IMG;
	::SetRect(SMenu[MN_NATION_BATTLE_BBS].nField[3].rCheakBox,
	SMenu[MN_NATION_BATTLE_BBS].nField[3].x,SMenu[MN_NATION_BATTLE_BBS].nField[3].y,
	SMenu[MN_NATION_BATTLE_BBS].nField[3].x+110,SMenu[MN_NATION_BATTLE_BBS].nField[3].y+30);

	//�δ� NATION BUDAE					OK
	SMenu[MN_NATION_BATTLE_BBS].nField[4].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_BATTLE_BBS].nField[4].nSpecialWillDo=SWD_NATION_BUDAE;
	SMenu[MN_NATION_BATTLE_BBS].nField[4].nWillDo=MN_NATION_BUDAE;
	SMenu[MN_NATION_BATTLE_BBS].nField[4].x = 42;
	SMenu[MN_NATION_BATTLE_BBS].nField[4].y = 144;
	SMenu[MN_NATION_BATTLE_BBS].nField[4].nRectImage = 4;
	SMenu[MN_NATION_BATTLE_BBS].nField[4].nImageNumber = 3;
	SMenu[MN_NATION_BATTLE_BBS].nField[4].nImageType = NATION_BATTLE_BBS_IMG;
	::SetRect(SMenu[MN_NATION_BATTLE_BBS].nField[4].rCheakBox,
	SMenu[MN_NATION_BATTLE_BBS].nField[4].x,SMenu[MN_NATION_BATTLE_BBS].nField[4].y,
	SMenu[MN_NATION_BATTLE_BBS].nField[4].x+110,SMenu[MN_NATION_BATTLE_BBS].nField[4].y+30);

	//�뺴			OK
	SMenu[MN_NATION_BATTLE_BBS].nField[5].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_BATTLE_BBS].nField[5].nSpecialWillDo=SWD_NATION_GUEST;
	SMenu[MN_NATION_BATTLE_BBS].nField[5].nWillDo=MN_NATION_GUEST;
	SMenu[MN_NATION_BATTLE_BBS].nField[5].x = 42;
	SMenu[MN_NATION_BATTLE_BBS].nField[5].y = 174;
	SMenu[MN_NATION_BATTLE_BBS].nField[5].nRectImage = 6;
	SMenu[MN_NATION_BATTLE_BBS].nField[5].nImageNumber = 5;
	SMenu[MN_NATION_BATTLE_BBS].nField[5].nImageType = NATION_BATTLE_BBS_IMG;
	::SetRect(SMenu[MN_NATION_BATTLE_BBS].nField[5].rCheakBox,
	SMenu[MN_NATION_BATTLE_BBS].nField[5].x,SMenu[MN_NATION_BATTLE_BBS].nField[5].y,
	SMenu[MN_NATION_BATTLE_BBS].nField[5].x+110,SMenu[MN_NATION_BATTLE_BBS].nField[5].y+30);

	//������ �̵�			OK
	SMenu[MN_NATION_BATTLE_BBS].nField[6].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_BATTLE_BBS].nField[6].nSpecialWillDo=SWD_NATION_GOTO_WAR_MAP;
	SMenu[MN_NATION_BATTLE_BBS].nField[6].nWillDo=MN_NATION_GOTO_WAR_MAP;
	SMenu[MN_NATION_BATTLE_BBS].nField[6].x = 42;
	SMenu[MN_NATION_BATTLE_BBS].nField[6].y = 234;
	SMenu[MN_NATION_BATTLE_BBS].nField[6].nRectImage = 16;
	SMenu[MN_NATION_BATTLE_BBS].nField[6].nImageNumber = 15;
	SMenu[MN_NATION_BATTLE_BBS].nField[6].nImageType = NATION_BATTLE_BBS_IMG;
	::SetRect(SMenu[MN_NATION_BATTLE_BBS].nField[6].rCheakBox,
	SMenu[MN_NATION_BATTLE_BBS].nField[6].x,SMenu[MN_NATION_BATTLE_BBS].nField[6].y,
	SMenu[MN_NATION_BATTLE_BBS].nField[6].x+110,SMenu[MN_NATION_BATTLE_BBS].nField[6].y+30);

	//���� ����			
	SMenu[MN_NATION_BATTLE_BBS].nField[7].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_BATTLE_BBS].nField[7].nSpecialWillDo=SWD_NATION_LADDER;
	SMenu[MN_NATION_BATTLE_BBS].nField[7].nWillDo=MN_NATION_LADDER;
	SMenu[MN_NATION_BATTLE_BBS].nField[7].x = 42;
	SMenu[MN_NATION_BATTLE_BBS].nField[7].y = 264;
	SMenu[MN_NATION_BATTLE_BBS].nField[7].nRectImage = 8;
	SMenu[MN_NATION_BATTLE_BBS].nField[7].nImageNumber = 7;
	SMenu[MN_NATION_BATTLE_BBS].nField[7].nImageType = NATION_BATTLE_BBS_IMG;
	::SetRect(SMenu[MN_NATION_BATTLE_BBS].nField[7].rCheakBox,
	SMenu[MN_NATION_BATTLE_BBS].nField[7].x,SMenu[MN_NATION_BATTLE_BBS].nField[7].y,
	SMenu[MN_NATION_BATTLE_BBS].nField[7].x+110,SMenu[MN_NATION_BATTLE_BBS].nField[7].y+30);

	//exit ��ư
	SMenu[MN_NATION_BATTLE_BBS].nField[8].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_BATTLE_BBS].nField[8].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_NATION_BATTLE_BBS].nField[8].x = 69;
	SMenu[MN_NATION_BATTLE_BBS].nField[8].y =307;
	SMenu[MN_NATION_BATTLE_BBS].nField[8].nRectImage = 18;
	SMenu[MN_NATION_BATTLE_BBS].nField[8].nImageNumber = 17;
	SMenu[MN_NATION_BATTLE_BBS].nField[8].nImageType = NATION_BATTLE_BBS_IMG;
	::SetRect(SMenu[MN_NATION_BATTLE_BBS].nField[8].rCheakBox,
	SMenu[MN_NATION_BATTLE_BBS].nField[8].x,SMenu[MN_NATION_BATTLE_BBS].nField[8].y,
	SMenu[MN_NATION_BATTLE_BBS].nField[8].x+110,SMenu[MN_NATION_BATTLE_BBS].nField[8].y+30);

	//���� ���� ����
	SMenu[MN_NATION_BATTLE_BBS].nField[9].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_BATTLE_BBS].nField[9].nSpecialWillDo=SWD_NATION_WEAPON_BUY;
	SMenu[MN_NATION_BATTLE_BBS].nField[9].x = 42;
	SMenu[MN_NATION_BATTLE_BBS].nField[9].y =204;
	SMenu[MN_NATION_BATTLE_BBS].nField[9].nRectImage = 2;
	SMenu[MN_NATION_BATTLE_BBS].nField[9].nImageNumber = 1;
	SMenu[MN_NATION_BATTLE_BBS].nField[9].nImageType = NATION_BATTLE_BBS_IMG;
	::SetRect(SMenu[MN_NATION_BATTLE_BBS].nField[9].rCheakBox,
	SMenu[MN_NATION_BATTLE_BBS].nField[9].x,SMenu[MN_NATION_BATTLE_BBS].nField[9].y,
	SMenu[MN_NATION_BATTLE_BBS].nField[9].x+110,SMenu[MN_NATION_BATTLE_BBS].nField[9].y+30);


	/*	//������ ���� NATION_INFO
	SMenu[MN_NATION_BATTLE_BBS].nField[4].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_BATTLE_BBS].nField[4].nSpecialWillDo=SWD_NATION_INFO;
	SMenu[MN_NATION_BATTLE_BBS].nField[4].nWillDo=MN_NATION_INFO;
	SMenu[MN_NATION_BATTLE_BBS].nField[4].x = 20;
	SMenu[MN_NATION_BATTLE_BBS].nField[4].y = 70;
	SMenu[MN_NATION_BATTLE_BBS].nField[4].nRectImage = 9;
	SMenu[MN_NATION_BATTLE_BBS].nField[4].nImageNumber = 10;
	SMenu[MN_NATION_BATTLE_BBS].nField[4].nImageType = NATION_BATTLE_BBS_IMG;
	::SetRect(SMenu[MN_NATION_BATTLE_BBS].nField[4].rCheakBox,
	SMenu[MN_NATION_BATTLE_BBS].nField[4].x,SMenu[MN_NATION_BATTLE_BBS].nField[4].y,
	SMenu[MN_NATION_BATTLE_BBS].nField[4].x+130,SMenu[MN_NATION_BATTLE_BBS].nField[4].y+30);
	*/
}

/////////////////////////////////////////////////
//���ݷη� ���� �ϰڴ�
//������ ���ݷ� ����	
	{
	::SMenuReg(MN_NATION_INFO,CENTER_X, 100,NATION_INFO_IMG,0,8);
	
	SMenu[MN_NATION_INFO].nField[0].nType = FT_NOMAL_PUT;
	SMenu[MN_NATION_INFO].nField[0].x = 21;
	SMenu[MN_NATION_INFO].nField[0].y = 46;
	SMenu[MN_NATION_INFO].nField[0].nImageNumber = 5;
	SMenu[MN_NATION_INFO].nField[0].nImageType = NATION_INFO_IMG;
	
	//1 �� ����
	SMenu[MN_NATION_INFO].nField[1].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_INFO].nField[1].nSpecialWillDo=SWD_NATION_INFO_SELECT;
	SMenu[MN_NATION_INFO].nField[1].nWillDo=1;
	SMenu[MN_NATION_INFO].nField[1].x = 18+175;
	SMenu[MN_NATION_INFO].nField[1].y = 27+221;
	SMenu[MN_NATION_INFO].nField[1].nRectImage = 1;
	SMenu[MN_NATION_INFO].nField[1].nImageNumber = 1;
	SMenu[MN_NATION_INFO].nField[1].nImageType = NATION_INFO_IMG;
	::SetRect(SMenu[MN_NATION_INFO].nField[1].rCheakBox,SMenu[MN_NATION_INFO].nField[1].x - 10 ,SMenu[MN_NATION_INFO].nField[1].y-10,
		SMenu[MN_NATION_INFO].nField[1].x+24,SMenu[MN_NATION_INFO].nField[1].y+40);

	//2 �� ����
	SMenu[MN_NATION_INFO].nField[2].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_INFO].nField[2].nSpecialWillDo=SWD_NATION_INFO_SELECT;
	SMenu[MN_NATION_INFO].nField[2].nWillDo=2; //nWillDo �� �ִ� ������ ���ݷθ� ����� 
	SMenu[MN_NATION_INFO].nField[2].x = 18+116;
	SMenu[MN_NATION_INFO].nField[2].y = 27+34;
	SMenu[MN_NATION_INFO].nField[2].nRectImage = 1;
	SMenu[MN_NATION_INFO].nField[2].nImageNumber = 1;
	SMenu[MN_NATION_INFO].nField[2].nImageType = NATION_INFO_IMG;
	::SetRect(SMenu[MN_NATION_INFO].nField[2].rCheakBox,SMenu[MN_NATION_INFO].nField[2].x - 10 ,SMenu[MN_NATION_INFO].nField[2].y-10,
		SMenu[MN_NATION_INFO].nField[2].x+24,SMenu[MN_NATION_INFO].nField[2].y+40);

	//3 �� ����
	SMenu[MN_NATION_INFO].nField[3].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_INFO].nField[3].nSpecialWillDo=SWD_NATION_INFO_SELECT;
	SMenu[MN_NATION_INFO].nField[3].nWillDo=3;
	SMenu[MN_NATION_INFO].nField[3].x = 18+49;
	SMenu[MN_NATION_INFO].nField[3].y = 27+81;
	SMenu[MN_NATION_INFO].nField[3].nRectImage = 1;
	SMenu[MN_NATION_INFO].nField[3].nImageNumber = 1;
	SMenu[MN_NATION_INFO].nField[3].nImageType = NATION_INFO_IMG;
	::SetRect(SMenu[MN_NATION_INFO].nField[3].rCheakBox,SMenu[MN_NATION_INFO].nField[3].x - 10 ,SMenu[MN_NATION_INFO].nField[3].y-10,
		SMenu[MN_NATION_INFO].nField[3].x+24,SMenu[MN_NATION_INFO].nField[3].y+40);

	//4 �� ����
	SMenu[MN_NATION_INFO].nField[4].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_INFO].nField[4].nSpecialWillDo=SWD_NATION_INFO_SELECT;
	SMenu[MN_NATION_INFO].nField[4].nWillDo=4;
	SMenu[MN_NATION_INFO].nField[4].x = 18+49;
	SMenu[MN_NATION_INFO].nField[4].y = 27+81;
	SMenu[MN_NATION_INFO].nField[4].nRectImage = 1;
	SMenu[MN_NATION_INFO].nField[4].nImageNumber = 1;
	SMenu[MN_NATION_INFO].nField[4].nImageType = NATION_INFO_IMG;
	::SetRect(SMenu[MN_NATION_INFO].nField[4].rCheakBox,SMenu[MN_NATION_INFO].nField[4].x - 10 ,SMenu[MN_NATION_INFO].nField[4].y-10,
		SMenu[MN_NATION_INFO].nField[4].x+24,SMenu[MN_NATION_INFO].nField[4].y+40);


//�ݱ�
	SMenu[MN_NATION_INFO].nField[5].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_INFO].nField[5].nSpecialWillDo=SWD_SUBMENU_BACK;
	SMenu[MN_NATION_INFO].nField[5].x = 384;
	SMenu[MN_NATION_INFO].nField[5].y = 302;
	SMenu[MN_NATION_INFO].nField[5].nRectImage = 3;
	SMenu[MN_NATION_INFO].nField[5].nImageNumber = 2;
	SMenu[MN_NATION_INFO].nField[5].nImageType = NATION_INFO_IMG;
	::SetRect(SMenu[MN_NATION_INFO].nField[5].rCheakBox,SMenu[MN_NATION_INFO].nField[5].x,SMenu[MN_NATION_INFO].nField[5].y,
		SMenu[MN_NATION_INFO].nField[5].x+100,SMenu[MN_NATION_INFO].nField[5].y+30);

	SMenu[MN_NATION_INFO].nField[6].nType = FT_DATA_PUT;
	SMenu[MN_NATION_INFO].nField[6].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_INFO].nField[6].x = 340;
	SMenu[MN_NATION_INFO].nField[6].y = 64;
	SMenu[MN_NATION_INFO].nField[6].nRectImage = 150;
	strcpy(SMenu[MN_NATION_INFO].nField[6].temp, lan->OutputMessage(0,145) );

	// ������ �� �ִ� ����
	SMenu[MN_NATION_INFO].nField[7].nType = FT_NOMAL_PUT_BY_NWILLDO;
	SMenu[MN_NATION_INFO].nField[7].nWillDo=0;
	SMenu[MN_NATION_INFO].nField[7].x = 18+136;
	SMenu[MN_NATION_INFO].nField[7].y = 27+208;
	SMenu[MN_NATION_INFO].nField[7].nRectImage = 1;
	SMenu[MN_NATION_INFO].nField[7].nImageNumber = 1;
	SMenu[MN_NATION_INFO].nField[7].nImageType = NATION_INFO_IMG;


	}

	//���� ����
	{
	::SMenuReg(MN_NATION_WAR_START,CENTER_X, 100,NATION_WAR_START_IMG,0,10);

	//����
	SMenu[MN_NATION_WAR_START].nField[0].nType = FT_NOMAL_PUT;
	SMenu[MN_NATION_WAR_START].nField[0].x = 21;
	SMenu[MN_NATION_WAR_START].nField[0].y = 46;
	SMenu[MN_NATION_WAR_START].nField[0].nImageNumber = 1;
	SMenu[MN_NATION_WAR_START].nField[0].nImageType = NATION_WAR_START_IMG;

	//���ݷ�
	SMenu[MN_NATION_WAR_START].nField[1].nType = FT_NOMAL_PUT_BY_NWILLDO;
	SMenu[MN_NATION_WAR_START].nField[1].x = 21;
	SMenu[MN_NATION_WAR_START].nField[1].y = 46;
	SMenu[MN_NATION_WAR_START].nField[1].nImageNumber = 1;
	SMenu[MN_NATION_WAR_START].nField[1].nWillDo = 0;
	SMenu[MN_NATION_WAR_START].nField[1].nImageType = NATION_WAR_START_IMG;

	//���ݷ�
	SMenu[MN_NATION_WAR_START].nField[2].nType = FT_NOMAL_PUT_BY_NWILLDO;
	SMenu[MN_NATION_WAR_START].nField[2].x = 21;
	SMenu[MN_NATION_WAR_START].nField[2].y = 46;
	SMenu[MN_NATION_WAR_START].nField[2].nWillDo = 0;
	SMenu[MN_NATION_WAR_START].nField[2].nImageNumber = 1;
	SMenu[MN_NATION_WAR_START].nField[2].nImageType = NATION_WAR_START_IMG;

	//���ݷ�
	SMenu[MN_NATION_WAR_START].nField[3].nType = FT_NOMAL_PUT_BY_NWILLDO;
	SMenu[MN_NATION_WAR_START].nField[3].x = 21;
	SMenu[MN_NATION_WAR_START].nField[3].y = 46;
	SMenu[MN_NATION_WAR_START].nField[3].nWillDo = 0;
	SMenu[MN_NATION_WAR_START].nField[3].nImageNumber = 1;
	SMenu[MN_NATION_WAR_START].nField[3].nImageType = NATION_WAR_START_IMG;

	//���ݷ�
	SMenu[MN_NATION_WAR_START].nField[4].nType = FT_NOMAL_PUT_BY_NWILLDO;
	SMenu[MN_NATION_WAR_START].nField[4].x = 21;
	SMenu[MN_NATION_WAR_START].nField[4].y = 46;
	SMenu[MN_NATION_WAR_START].nField[4].nWillDo = 0;
	SMenu[MN_NATION_WAR_START].nField[4].nImageNumber = 1;
	SMenu[MN_NATION_WAR_START].nField[4].nImageType = NATION_WAR_START_IMG;


	//�����
	SMenu[MN_NATION_WAR_START].nField[5].nType = FT_DATA_PUT;
	SMenu[MN_NATION_WAR_START].nField[5].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_WAR_START].nField[5].x = 340;
	SMenu[MN_NATION_WAR_START].nField[5].y = 64;
	SMenu[MN_NATION_WAR_START].nField[5].nRectImage = 150;
//010927 lsw
//	strcpy(SMenu[MN_NATION_WAR_START].nField[4].temp, lan->OutputMessage(0,145) );
	
	//�ݱ�
	SMenu[MN_NATION_WAR_START].nField[6].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_WAR_START].nField[6].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_NATION_WAR_START].nField[6].x = 384;
	SMenu[MN_NATION_WAR_START].nField[6].y = 302;
	SMenu[MN_NATION_WAR_START].nField[6].nRectImage = 3;
	SMenu[MN_NATION_WAR_START].nField[6].nImageNumber = 2;
	SMenu[MN_NATION_WAR_START].nField[6].nImageType = NATION_WAR_START_IMG;
	::SetRect(SMenu[MN_NATION_WAR_START].nField[6].rCheakBox,SMenu[MN_NATION_WAR_START].nField[6].x,SMenu[MN_NATION_WAR_START].nField[6].y,
		SMenu[MN_NATION_WAR_START].nField[6].x+100,SMenu[MN_NATION_WAR_START].nField[6].y+30);

	//������� ���ݷ�
	SMenu[MN_NATION_WAR_START].nField[7].nType = FT_NOMAL_PUT_BY_NWILLDO;
	SMenu[MN_NATION_WAR_START].nField[7].x = 21;
	SMenu[MN_NATION_WAR_START].nField[7].y = 46;
	SMenu[MN_NATION_WAR_START].nField[7].nWillDo = 0;
	SMenu[MN_NATION_WAR_START].nField[7].nImageNumber = 1;
	SMenu[MN_NATION_WAR_START].nField[7].nImageType = NATION_WAR_START_IMG;

	}
//��ǥ �Ϸ�
////////////////////////////////////////////////////////////////////////
//��ɰ� ��ǥ
	{
	::SMenuReg(MN_NATION_VOTE,CENTER_X, 100,NATION_VOTE_IMG,0,20);
		
	//�޴� ����
	SMenu[MN_NATION_VOTE].nField[0].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_VOTE].nField[0].nSpecialWillDo=SWD_SUBMENU_BACK;
	SMenu[MN_NATION_VOTE].nField[0].x = 121;
	SMenu[MN_NATION_VOTE].nField[0].y = 305;
	SMenu[MN_NATION_VOTE].nField[0].nRectImage = 6;
	SMenu[MN_NATION_VOTE].nField[0].nImageNumber = 5;
	SMenu[MN_NATION_VOTE].nField[0].nImageType = NATION_VOTE_IMG;
	::SetRect(SMenu[MN_NATION_VOTE].nField[0].rCheakBox,SMenu[MN_NATION_VOTE].nField[0].x,SMenu[MN_NATION_VOTE].nField[0].y,
		SMenu[MN_NATION_VOTE].nField[0].x+50,SMenu[MN_NATION_VOTE].nField[0].y+30);
	
	//���� �ϱ� gg�ƴ� ��.��
	SMenu[MN_NATION_VOTE].nField[1].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_VOTE].nField[1].nSpecialWillDo=SWD_NATION_VOTE_SELECT_YOU;
	SMenu[MN_NATION_VOTE].nField[1].x = 118;
	SMenu[MN_NATION_VOTE].nField[1].y = 260;
	SMenu[MN_NATION_VOTE].nField[1].nRectImage = 4;
	SMenu[MN_NATION_VOTE].nField[1].nImageNumber = 3;
	SMenu[MN_NATION_VOTE].nField[1].nImageType = NATION_VOTE_IMG;
	::SetRect(SMenu[MN_NATION_VOTE].nField[1].rCheakBox,SMenu[MN_NATION_VOTE].nField[1].x,SMenu[MN_NATION_VOTE].nField[1].y,
		SMenu[MN_NATION_VOTE].nField[1].x+35,SMenu[MN_NATION_VOTE].nField[1].y+30);

	//��� �ϱ� ( ���� ��ɰ� �ҷ�)
	SMenu[MN_NATION_VOTE].nField[2].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_VOTE].nField[2].nSpecialWillDo=SWD_NATION_VOTE_I_AM_HERO;
	SMenu[MN_NATION_VOTE].nField[2].x = 49;
	SMenu[MN_NATION_VOTE].nField[2].y = 260;
	SMenu[MN_NATION_VOTE].nField[2].nRectImage = 2;
	SMenu[MN_NATION_VOTE].nField[2].nImageNumber = 1;
	SMenu[MN_NATION_VOTE].nField[2].nImageType = NATION_VOTE_IMG;
	::SetRect(SMenu[MN_NATION_VOTE].nField[2].rCheakBox,SMenu[MN_NATION_VOTE].nField[2].x,SMenu[MN_NATION_VOTE].nField[2].y,
		SMenu[MN_NATION_VOTE].nField[2].x+50,SMenu[MN_NATION_VOTE].nField[2].y+50);

	//����
	SMenu[MN_NATION_VOTE].nField[3].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_VOTE].nField[3].nSpecialWillDo=SWD_NATION_VOTE_PREV_REFRESH;
	SMenu[MN_NATION_VOTE].nField[3].nWillDo	=	-1;
	SMenu[MN_NATION_VOTE].nField[3].x = 39;
	SMenu[MN_NATION_VOTE].nField[3].y = 113;
	SMenu[MN_NATION_VOTE].nField[3].nRectImage = 8;
	SMenu[MN_NATION_VOTE].nField[3].nImageNumber = 7;
	SMenu[MN_NATION_VOTE].nField[3].nImageType = NATION_VOTE_IMG;
	::SetRect(SMenu[MN_NATION_VOTE].nField[3].rCheakBox,SMenu[MN_NATION_VOTE].nField[3].x,SMenu[MN_NATION_VOTE].nField[3].y,
		SMenu[MN_NATION_VOTE].nField[3].x+50,SMenu[MN_NATION_VOTE].nField[3].y+50);
	//����
	SMenu[MN_NATION_VOTE].nField[4].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_VOTE].nField[4].nSpecialWillDo=SWD_NATION_VOTE_NEXT_REFRESH;
	SMenu[MN_NATION_VOTE].nField[4].nWillDo=	1;
	SMenu[MN_NATION_VOTE].nField[4].x = 151;
	SMenu[MN_NATION_VOTE].nField[4].y = 113;
	SMenu[MN_NATION_VOTE].nField[4].nRectImage = 10;
	SMenu[MN_NATION_VOTE].nField[4].nImageNumber = 9;
	SMenu[MN_NATION_VOTE].nField[4].nImageType = NATION_VOTE_IMG;
	::SetRect(SMenu[MN_NATION_VOTE].nField[4].rCheakBox,SMenu[MN_NATION_VOTE].nField[4].x,SMenu[MN_NATION_VOTE].nField[4].y,
		SMenu[MN_NATION_VOTE].nField[4].x+50,SMenu[MN_NATION_VOTE].nField[4].y+50);

	//���� �� ����
	SMenu[MN_NATION_VOTE].nField[5].nType = FT_DATA_PUT;
	SMenu[MN_NATION_VOTE].nField[5].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_VOTE].nField[5].x = 70;
	SMenu[MN_NATION_VOTE].nField[5].y = 165;
	SMenu[MN_NATION_VOTE].nField[5].nRectImage = 150;
	SMenu[MN_NATION_VOTE].nField[5].nImageType = NATION_VOTE_IMG;
	::SetRect(SMenu[MN_NATION_VOTE].nField[5].rCheakBox,SMenu[MN_NATION_VOTE].nField[5].x,SMenu[MN_NATION_VOTE].nField[5].y,
		SMenu[MN_NATION_VOTE].nField[5].x+100,SMenu[MN_NATION_VOTE].nField[5].y+30);
	strcpy(SMenu[MN_NATION_VOTE].nField[5].temp,  "1��° ĭ" );

	SMenu[MN_NATION_VOTE].nField[6].nType = FT_DATA_PUT;
	SMenu[MN_NATION_VOTE].nField[6].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_VOTE].nField[6].x = 70;
	SMenu[MN_NATION_VOTE].nField[6].y = 182;
	SMenu[MN_NATION_VOTE].nField[6].nRectImage = 150;
	SMenu[MN_NATION_VOTE].nField[6].nImageType = NATION_VOTE_IMG;
	::SetRect(SMenu[MN_NATION_VOTE].nField[6].rCheakBox,SMenu[MN_NATION_VOTE].nField[6].x,SMenu[MN_NATION_VOTE].nField[6].y,
		SMenu[MN_NATION_VOTE].nField[6].x+100,SMenu[MN_NATION_VOTE].nField[6].y+30);
	strcpy(SMenu[MN_NATION_VOTE].nField[6].temp,  "2��° ĭ" );

		SMenu[MN_NATION_VOTE].nField[7].nType = FT_DATA_PUT;
	SMenu[MN_NATION_VOTE].nField[7].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_VOTE].nField[7].x = 70;
	SMenu[MN_NATION_VOTE].nField[7].y = 200;
	SMenu[MN_NATION_VOTE].nField[7].nRectImage = 150;
	SMenu[MN_NATION_VOTE].nField[7].nImageType = NATION_VOTE_IMG;
	::SetRect(SMenu[MN_NATION_VOTE].nField[7].rCheakBox,SMenu[MN_NATION_VOTE].nField[7].x,SMenu[MN_NATION_VOTE].nField[7].y,
		SMenu[MN_NATION_VOTE].nField[7].x+100,SMenu[MN_NATION_VOTE].nField[7].y+30);
	strcpy(SMenu[MN_NATION_VOTE].nField[7].temp,  "3��° ĭ" );

	SMenu[MN_NATION_VOTE].nField[8].nType = FT_DATA_PUT;
	SMenu[MN_NATION_VOTE].nField[8].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_VOTE].nField[8].x = 103;
	SMenu[MN_NATION_VOTE].nField[8].y = 221;
	SMenu[MN_NATION_VOTE].nField[8].nRectImage = 150;
	SMenu[MN_NATION_VOTE].nField[8].nImageType = NATION_VOTE_IMG;
	::SetRect(SMenu[MN_NATION_VOTE].nField[8].rCheakBox,SMenu[MN_NATION_VOTE].nField[8].x,SMenu[MN_NATION_VOTE].nField[8].y,
		SMenu[MN_NATION_VOTE].nField[8].x+100,SMenu[MN_NATION_VOTE].nField[8].y+30);
	strcpy(SMenu[MN_NATION_VOTE].nField[8].temp,  "FAME" );

	SMenu[MN_NATION_VOTE].nField[9].nType = FT_DATA_PUT;
	SMenu[MN_NATION_VOTE].nField[9].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_VOTE].nField[9].x = 103;
	SMenu[MN_NATION_VOTE].nField[9].y = 237;
	SMenu[MN_NATION_VOTE].nField[9].nRectImage = 150;
	SMenu[MN_NATION_VOTE].nField[9].nImageType = NATION_VOTE_IMG;
	::SetRect(SMenu[MN_NATION_VOTE].nField[9].rCheakBox,SMenu[MN_NATION_VOTE].nField[9].x,SMenu[MN_NATION_VOTE].nField[9].y,
		SMenu[MN_NATION_VOTE].nField[9].x+100,SMenu[MN_NATION_VOTE].nField[9].y+30);
	strcpy(SMenu[MN_NATION_VOTE].nField[9].temp,  "LADDER" );

	//��ǥ ����� ��.
	SMenu[MN_NATION_VOTE].nField[10].nType = FT_DATA_PUT;
	SMenu[MN_NATION_VOTE].nField[10].nImageNumber = DP_FACE_PUT;
	SMenu[MN_NATION_VOTE].nField[10].nImageType = 0;
	SMenu[MN_NATION_VOTE].nField[10].nRectImage = 0;
	SMenu[MN_NATION_VOTE].nField[10].x = 79;
	SMenu[MN_NATION_VOTE].nField[10].y = 88;

	SMenu[MN_NATION_VOTE].nField[11].nType = FT_DATA_PUT;
	SMenu[MN_NATION_VOTE].nField[11].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_VOTE].nField[11].x = 210;
	SMenu[MN_NATION_VOTE].nField[11].y = 100;
	SMenu[MN_NATION_VOTE].nField[11].nRectImage = 150;
	::SetRect(SMenu[MN_NATION_VOTE].nField[11].rCheakBox,SMenu[MN_NATION_VOTE].nField[11].x,SMenu[MN_NATION_VOTE].nField[11].y,
		SMenu[MN_NATION_VOTE].nField[11].x+100,SMenu[MN_NATION_VOTE].nField[11].y+30);
	strcpy(SMenu[MN_NATION_VOTE].nField[11].temp,  "�ֵ� ����Ʈ1" );

	SMenu[MN_NATION_VOTE].nField[12].nType = FT_DATA_PUT;
	SMenu[MN_NATION_VOTE].nField[12].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_VOTE].nField[12].x = 210;
	SMenu[MN_NATION_VOTE].nField[12].y = 130;
	SMenu[MN_NATION_VOTE].nField[12].nRectImage = 150;
	::SetRect(SMenu[MN_NATION_VOTE].nField[12].rCheakBox,SMenu[MN_NATION_VOTE].nField[12].x,SMenu[MN_NATION_VOTE].nField[12].y,
		SMenu[MN_NATION_VOTE].nField[12].x+100,SMenu[MN_NATION_VOTE].nField[12].y+30);
	strcpy(SMenu[MN_NATION_VOTE].nField[12].temp,  "�ֵ� ����Ʈ1" );


	SMenu[MN_NATION_VOTE].nField[13].nType = FT_DATA_PUT;
	SMenu[MN_NATION_VOTE].nField[13].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_VOTE].nField[13].x = 210;
	SMenu[MN_NATION_VOTE].nField[13].y = 160;
	SMenu[MN_NATION_VOTE].nField[13].nRectImage = 150;
	::SetRect(SMenu[MN_NATION_VOTE].nField[13].rCheakBox,SMenu[MN_NATION_VOTE].nField[13].x,SMenu[MN_NATION_VOTE].nField[13].y,
		SMenu[MN_NATION_VOTE].nField[13].x+100,SMenu[MN_NATION_VOTE].nField[13].y+30);
	strcpy(SMenu[MN_NATION_VOTE].nField[13].temp,  "�ֵ� ����Ʈ1" );


	SMenu[MN_NATION_VOTE].nField[14].nType = FT_DATA_PUT;
	SMenu[MN_NATION_VOTE].nField[14].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_VOTE].nField[14].x = 210;
	SMenu[MN_NATION_VOTE].nField[14].y = 190;
	SMenu[MN_NATION_VOTE].nField[14].nRectImage = 150;
	::SetRect(SMenu[MN_NATION_VOTE].nField[14].rCheakBox,SMenu[MN_NATION_VOTE].nField[14].x,SMenu[MN_NATION_VOTE].nField[14].y,
		SMenu[MN_NATION_VOTE].nField[14].x+100,SMenu[MN_NATION_VOTE].nField[14].y+30);
	strcpy(SMenu[MN_NATION_VOTE].nField[14].temp,  "�ֵ� ����Ʈ1" );

	SMenu[MN_NATION_VOTE].nField[15].nType = FT_DATA_PUT;
	SMenu[MN_NATION_VOTE].nField[15].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_VOTE].nField[15].x = 210;
	SMenu[MN_NATION_VOTE].nField[15].y = 220;
	SMenu[MN_NATION_VOTE].nField[15].nRectImage = 150;
	::SetRect(SMenu[MN_NATION_VOTE].nField[15].rCheakBox,SMenu[MN_NATION_VOTE].nField[15].x,SMenu[MN_NATION_VOTE].nField[15].y,
		SMenu[MN_NATION_VOTE].nField[15].x+100,SMenu[MN_NATION_VOTE].nField[15].y+30);
	strcpy(SMenu[MN_NATION_VOTE].nField[15].temp,  "�ֵ� ����Ʈ1" );

	SMenu[MN_NATION_VOTE].nField[16].nType = FT_DATA_PUT;
	SMenu[MN_NATION_VOTE].nField[16].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_VOTE].nField[16].x = 210;
	SMenu[MN_NATION_VOTE].nField[16].y = 250;
	SMenu[MN_NATION_VOTE].nField[16].nRectImage = 150;
	::SetRect(SMenu[MN_NATION_VOTE].nField[16].rCheakBox,SMenu[MN_NATION_VOTE].nField[16].x,SMenu[MN_NATION_VOTE].nField[16].y,
		SMenu[MN_NATION_VOTE].nField[16].x+100,SMenu[MN_NATION_VOTE].nField[16].y+30);
	strcpy(SMenu[MN_NATION_VOTE].nField[16].temp,  "�ֵ� ����Ʈ1" );

	}
//�δ�
////////////////////////////////////////////////////////////////////////
//�δ� ���
	{
	::SMenuReg(MN_NATION_BUDAE,CENTER_X, 100,NATION_BUDAE_IMG,0,20);

	//�ݱ� ��ư
	SMenu[MN_NATION_BUDAE].nField[0].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_BUDAE].nField[0].nSpecialWillDo=SWD_SUBMENU_BACK;
	SMenu[MN_NATION_BUDAE].nField[0].x = 121;
	SMenu[MN_NATION_BUDAE].nField[0].y = 305;
	SMenu[MN_NATION_BUDAE].nField[0].nRectImage = 10;
	SMenu[MN_NATION_BUDAE].nField[0].nImageNumber = 9;
	SMenu[MN_NATION_BUDAE].nField[0].nImageType = NATION_BUDAE_IMG;
	::SetRect(SMenu[MN_NATION_BUDAE].nField[0].rCheakBox,SMenu[MN_NATION_BUDAE].nField[0].x,SMenu[MN_NATION_BUDAE].nField[0].y,
		SMenu[MN_NATION_BUDAE].nField[0].x+70,SMenu[MN_NATION_BUDAE].nField[0].y+25);

	//��ɰ� ��ǥ
	SMenu[MN_NATION_BUDAE].nField[1].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_BUDAE].nField[1].nSpecialWillDo=SWD_NATION_VOTE;
	SMenu[MN_NATION_BUDAE].nField[1].nWillDo=MN_NATION_VOTE;
	SMenu[MN_NATION_BUDAE].nField[1].x = 169;
	SMenu[MN_NATION_BUDAE].nField[1].y = 45;
	SMenu[MN_NATION_BUDAE].nField[1].nRectImage = 12;
	SMenu[MN_NATION_BUDAE].nField[1].nImageNumber = 11;
	SMenu[MN_NATION_BUDAE].nField[1].nImageType = NATION_BUDAE_IMG;
	::SetRect(SMenu[MN_NATION_BUDAE].nField[1].rCheakBox,SMenu[MN_NATION_BUDAE].nField[1].x,SMenu[MN_NATION_BUDAE].nField[1].y,
		SMenu[MN_NATION_BUDAE].nField[1].x+70,SMenu[MN_NATION_BUDAE].nField[1].y+25);
	
	//���ݷ� ����
	SMenu[MN_NATION_BUDAE].nField[2].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_BUDAE].nField[2].nSpecialWillDo=SWD_NATION_ATTACK_WAY_POINT;
	SMenu[MN_NATION_BUDAE].nField[2].nWillDo=MN_NATION_INFO;
	SMenu[MN_NATION_BUDAE].nField[2].x = 42;
	SMenu[MN_NATION_BUDAE].nField[2].y = 211;
	SMenu[MN_NATION_BUDAE].nField[2].nRectImage = 13;
	SMenu[MN_NATION_BUDAE].nField[2].nImageNumber = 14;
	SMenu[MN_NATION_BUDAE].nField[2].nImageType = NATION_BUDAE_IMG;
	::SetRect(SMenu[MN_NATION_BUDAE].nField[2].rCheakBox,SMenu[MN_NATION_BUDAE].nField[2].x,SMenu[MN_NATION_BUDAE].nField[2].y,
		SMenu[MN_NATION_BUDAE].nField[2].x+70,SMenu[MN_NATION_BUDAE].nField[2].y+25);

	//����
	SMenu[MN_NATION_BUDAE].nField[3].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_BUDAE].nField[3].nSpecialWillDo=SWD_NATION_BUDAE_PREV_REFRESH;
	SMenu[MN_NATION_BUDAE].nField[3].nWillDo=MN_NATION_INFO;
	SMenu[MN_NATION_BUDAE].nField[3].x = 38;
	SMenu[MN_NATION_BUDAE].nField[3].y = 237;
	SMenu[MN_NATION_BUDAE].nField[3].nRectImage = 8;
	SMenu[MN_NATION_BUDAE].nField[3].nImageNumber = 7;
	SMenu[MN_NATION_BUDAE].nField[3].nImageType = NATION_BUDAE_IMG;
	::SetRect(SMenu[MN_NATION_BUDAE].nField[3].rCheakBox,SMenu[MN_NATION_BUDAE].nField[3].x,SMenu[MN_NATION_BUDAE].nField[3].y,
		SMenu[MN_NATION_BUDAE].nField[3].x+40,SMenu[MN_NATION_BUDAE].nField[3].y+25);
	//����
	SMenu[MN_NATION_BUDAE].nField[4].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_BUDAE].nField[4].nSpecialWillDo=SWD_NATION_BUDAE_NEXT_REFRESH;
	SMenu[MN_NATION_BUDAE].nField[4].nWillDo=MN_NATION_INFO;
	SMenu[MN_NATION_BUDAE].nField[4].x = 95;
	SMenu[MN_NATION_BUDAE].nField[4].y = 237;
	SMenu[MN_NATION_BUDAE].nField[4].nRectImage = 6;
	SMenu[MN_NATION_BUDAE].nField[4].nImageNumber = 5;
	SMenu[MN_NATION_BUDAE].nField[4].nImageType = NATION_BUDAE_IMG;
	::SetRect(SMenu[MN_NATION_BUDAE].nField[4].rCheakBox,SMenu[MN_NATION_BUDAE].nField[4].x,SMenu[MN_NATION_BUDAE].nField[4].y,
		SMenu[MN_NATION_BUDAE].nField[4].x+40,SMenu[MN_NATION_BUDAE].nField[4].y+25);
	//���
	SMenu[MN_NATION_BUDAE].nField[5].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_BUDAE].nField[5].nSpecialWillDo=SWD_NATION_BUDAE_JOIN;
	SMenu[MN_NATION_BUDAE].nField[5].nWillDo=MN_NATION_INFO;
	SMenu[MN_NATION_BUDAE].nField[5].x = 55;
	SMenu[MN_NATION_BUDAE].nField[5].y = 265;
	SMenu[MN_NATION_BUDAE].nField[5].nRectImage = 4;
	SMenu[MN_NATION_BUDAE].nField[5].nImageNumber = 3;
	SMenu[MN_NATION_BUDAE].nField[5].nImageType = NATION_BUDAE_IMG;
	::SetRect(SMenu[MN_NATION_BUDAE].nField[5].rCheakBox,SMenu[MN_NATION_BUDAE].nField[5].x,SMenu[MN_NATION_BUDAE].nField[5].y,
		SMenu[MN_NATION_BUDAE].nField[5].x+70,SMenu[MN_NATION_BUDAE].nField[5].y+25);

	//���� �� ��
	SMenu[MN_NATION_BUDAE].nField[6].nType = FT_DATA_PUT;
	SMenu[MN_NATION_BUDAE].nField[6].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_BUDAE].nField[6].x = 69;
	SMenu[MN_NATION_BUDAE].nField[6].y = 112;
	SMenu[MN_NATION_BUDAE].nField[6].nRectImage = 150;
	SMenu[MN_NATION_BUDAE].nField[6].nImageType = NATION_BUDAE_IMG;
	::SetRect(SMenu[MN_NATION_BUDAE].nField[6].rCheakBox,SMenu[MN_NATION_BUDAE].nField[6].x,SMenu[MN_NATION_BUDAE].nField[6].y,
		SMenu[MN_NATION_BUDAE].nField[6].x+100,SMenu[MN_NATION_BUDAE].nField[6].y+30);
	strcpy(SMenu[MN_NATION_BUDAE].nField[6].temp,  "�δ��ȣ" );
	
	SMenu[MN_NATION_BUDAE].nField[7].nType = FT_NOMAL_PUT;
	SMenu[MN_NATION_BUDAE].nField[7].nImageNumber = 28;
	SMenu[MN_NATION_BUDAE].nField[7].x = 35;
	SMenu[MN_NATION_BUDAE].nField[7].y = 91;
	SMenu[MN_NATION_BUDAE].nField[7].nImageType = NATION_NOW_IMG;
		
	SMenu[MN_NATION_BUDAE].nField[8].nType = FT_DATA_PUT;
	SMenu[MN_NATION_BUDAE].nField[8].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_BUDAE].nField[8].x = 35;
	SMenu[MN_NATION_BUDAE].nField[8].y = 130;
	SMenu[MN_NATION_BUDAE].nField[8].nRectImage = 150;
	SMenu[MN_NATION_BUDAE].nField[8].nImageType = NATION_BUDAE_IMG;
	::SetRect(SMenu[MN_NATION_BUDAE].nField[8].rCheakBox,SMenu[MN_NATION_BUDAE].nField[8].x,SMenu[MN_NATION_BUDAE].nField[8].y,
		SMenu[MN_NATION_BUDAE].nField[8].x+100,SMenu[MN_NATION_BUDAE].nField[8].y+30);
	strcpy(SMenu[MN_NATION_BUDAE].nField[8].temp,  "�δ����̸�" );

	SMenu[MN_NATION_BUDAE].nField[9].nType = FT_DATA_PUT;
	SMenu[MN_NATION_BUDAE].nField[9].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_BUDAE].nField[9].x = 88;
	SMenu[MN_NATION_BUDAE].nField[9].y = 94;
	SMenu[MN_NATION_BUDAE].nField[9].nRectImage = 150;
	SMenu[MN_NATION_BUDAE].nField[9].nImageType = NATION_BUDAE_IMG;
	::SetRect(SMenu[MN_NATION_BUDAE].nField[9].rCheakBox,SMenu[MN_NATION_BUDAE].nField[9].x,SMenu[MN_NATION_BUDAE].nField[9].y,
		SMenu[MN_NATION_BUDAE].nField[9].x+100,SMenu[MN_NATION_BUDAE].nField[9].y+30);

	SMenu[MN_NATION_BUDAE].nField[11].nType = FT_DATA_PUT;
	SMenu[MN_NATION_BUDAE].nField[11].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_BUDAE].nField[11].x = 71;
	SMenu[MN_NATION_BUDAE].nField[11].y = 111;
	SMenu[MN_NATION_BUDAE].nField[11].nRectImage = 150;
	SMenu[MN_NATION_BUDAE].nField[11].nImageType = NATION_BUDAE_IMG;
	::SetRect(SMenu[MN_NATION_BUDAE].nField[11].rCheakBox,SMenu[MN_NATION_BUDAE].nField[11].x,SMenu[MN_NATION_BUDAE].nField[11].y,
		SMenu[MN_NATION_BUDAE].nField[11].x+100,SMenu[MN_NATION_BUDAE].nField[11].y+30);
//	strcpy(SMenu[MN_NATION_BUDAE].nField[11].temp,  "�� 1 �δ�" );//�δ��
/////////////////////////////
//�ٸ� �δ� ����
	SMenu[MN_NATION_BUDAE].nField[12].nType = FT_DATA_PUT;
	SMenu[MN_NATION_BUDAE].nField[12].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_BUDAE].nField[12].x = 153;
	SMenu[MN_NATION_BUDAE].nField[12].y = 87;
	SMenu[MN_NATION_BUDAE].nField[12].nRectImage = 150;
	SMenu[MN_NATION_BUDAE].nField[12].nImageType = NATION_BUDAE_IMG;
	::SetRect(SMenu[MN_NATION_BUDAE].nField[12].rCheakBox,SMenu[MN_NATION_BUDAE].nField[12].x,SMenu[MN_NATION_BUDAE].nField[12].y,
		SMenu[MN_NATION_BUDAE].nField[12].x+100,SMenu[MN_NATION_BUDAE].nField[12].y+30);
	strcpy(SMenu[MN_NATION_BUDAE].nField[12].temp,  "�� 1 �δ�      %d/50" );


	SMenu[MN_NATION_BUDAE].nField[13].nType = FT_DATA_PUT;
	SMenu[MN_NATION_BUDAE].nField[13].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_BUDAE].nField[13].x = 153;
	SMenu[MN_NATION_BUDAE].nField[13].y = 105;
	SMenu[MN_NATION_BUDAE].nField[13].nRectImage = 150;
	SMenu[MN_NATION_BUDAE].nField[13].nImageType = NATION_BUDAE_IMG;
	::SetRect(SMenu[MN_NATION_BUDAE].nField[13].rCheakBox,SMenu[MN_NATION_BUDAE].nField[13].x,SMenu[MN_NATION_BUDAE].nField[13].y,
		SMenu[MN_NATION_BUDAE].nField[13].x+100,SMenu[MN_NATION_BUDAE].nField[13].y+30);
	strcpy(SMenu[MN_NATION_BUDAE].nField[13].temp,  "�� 2 �δ�      %d/40" );


	SMenu[MN_NATION_BUDAE].nField[14].nType = FT_DATA_PUT;
	SMenu[MN_NATION_BUDAE].nField[14].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_BUDAE].nField[14].x = 153;
	SMenu[MN_NATION_BUDAE].nField[14].y = 125;
	SMenu[MN_NATION_BUDAE].nField[14].nRectImage = 150;
	SMenu[MN_NATION_BUDAE].nField[14].nImageType = NATION_BUDAE_IMG;
	::SetRect(SMenu[MN_NATION_BUDAE].nField[14].rCheakBox,SMenu[MN_NATION_BUDAE].nField[14].x,SMenu[MN_NATION_BUDAE].nField[14].y,
		SMenu[MN_NATION_BUDAE].nField[14].x+100,SMenu[MN_NATION_BUDAE].nField[14].y+30);
	strcpy(SMenu[MN_NATION_BUDAE].nField[14].temp,  "�� 3 �δ�      %d/30" );


	SMenu[MN_NATION_BUDAE].nField[15].nType = FT_DATA_PUT;
	SMenu[MN_NATION_BUDAE].nField[15].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_BUDAE].nField[15].x = 153;
	SMenu[MN_NATION_BUDAE].nField[15].y = 144;
	SMenu[MN_NATION_BUDAE].nField[15].nRectImage = 150;
	SMenu[MN_NATION_BUDAE].nField[15].nImageType = NATION_BUDAE_IMG;
	::SetRect(SMenu[MN_NATION_BUDAE].nField[15].rCheakBox,SMenu[MN_NATION_BUDAE].nField[15].x,SMenu[MN_NATION_BUDAE].nField[15].y,
		SMenu[MN_NATION_BUDAE].nField[15].x+100,SMenu[MN_NATION_BUDAE].nField[15].y+30);
	strcpy(SMenu[MN_NATION_BUDAE].nField[15].temp,  "�� 4 �δ�      %d/20" );

	SMenu[MN_NATION_BUDAE].nField[16].nType = FT_DATA_PUT;
	SMenu[MN_NATION_BUDAE].nField[16].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_BUDAE].nField[16].x = 61;
	SMenu[MN_NATION_BUDAE].nField[16].y = 51;
	SMenu[MN_NATION_BUDAE].nField[16].nRectImage = 150;
	SMenu[MN_NATION_BUDAE].nField[16].nImageType = NATION_BUDAE_IMG;
	::SetRect(SMenu[MN_NATION_BUDAE].nField[16].rCheakBox,SMenu[MN_NATION_BUDAE].nField[16].x,SMenu[MN_NATION_BUDAE].nField[16].y,
		SMenu[MN_NATION_BUDAE].nField[16].x+100,SMenu[MN_NATION_BUDAE].nField[16].y+30);
	strcpy(SMenu[MN_NATION_BUDAE].nField[16].temp,  "��ɰ�." );

	SMenu[MN_NATION_BUDAE].nField[17].nType = FT_DATA_PUT;
	SMenu[MN_NATION_BUDAE].nField[17].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_BUDAE].nField[17].x = 33;
	SMenu[MN_NATION_BUDAE].nField[17].y = 148+3;
	SMenu[MN_NATION_BUDAE].nField[17].nRectImage = 150;
	SMenu[MN_NATION_BUDAE].nField[17].nImageType = NATION_BUDAE_IMG;
	::SetRect(SMenu[MN_NATION_BUDAE].nField[17].rCheakBox,SMenu[MN_NATION_BUDAE].nField[17].x,SMenu[MN_NATION_BUDAE].nField[17].y,
		SMenu[MN_NATION_BUDAE].nField[17].x+100,SMenu[MN_NATION_BUDAE].nField[17].y+30);
	strcpy(SMenu[MN_NATION_BUDAE].nField[17].temp,  "���� �Ӽ�." );


	SMenu[MN_NATION_BUDAE].nField[18].nType = FT_DATA_PUT;
	SMenu[MN_NATION_BUDAE].nField[18].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_BUDAE].nField[18].x = 33;
	SMenu[MN_NATION_BUDAE].nField[18].y = 166+3;
	SMenu[MN_NATION_BUDAE].nField[18].nRectImage = 150;
	SMenu[MN_NATION_BUDAE].nField[18].nImageType = NATION_BUDAE_IMG;
	::SetRect(SMenu[MN_NATION_BUDAE].nField[18].rCheakBox,SMenu[MN_NATION_BUDAE].nField[18].x,SMenu[MN_NATION_BUDAE].nField[18].y,
		SMenu[MN_NATION_BUDAE].nField[18].x+100,SMenu[MN_NATION_BUDAE].nField[18].y+30);
	strcpy(SMenu[MN_NATION_BUDAE].nField[18].temp,  "���� �Ӽ�" );

	SMenu[MN_NATION_BUDAE].nField[19].nType = FT_DATA_PUT;
	SMenu[MN_NATION_BUDAE].nField[19].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_BUDAE].nField[19].x = 33;
	SMenu[MN_NATION_BUDAE].nField[19].y = 184+3;
	SMenu[MN_NATION_BUDAE].nField[19].nRectImage = 150;
	SMenu[MN_NATION_BUDAE].nField[19].nImageType = NATION_BUDAE_IMG;
	::SetRect(SMenu[MN_NATION_BUDAE].nField[19].rCheakBox,SMenu[MN_NATION_BUDAE].nField[19].x,SMenu[MN_NATION_BUDAE].nField[19].y,
		SMenu[MN_NATION_BUDAE].nField[19].x+100,SMenu[MN_NATION_BUDAE].nField[19].y+30);
	strcpy(SMenu[MN_NATION_BUDAE].nField[19].temp,  "���� �Ӽ�" );

	//�δ� ��ũ ���� �� �̹����� �Ǿ�����
//	SMenu[MN_NATION_BUDAE].nField[11].nType = FT_DATA_PUT;
//	SMenu[MN_NATION_BUDAE].nField[11].nImageNumber = DP_FACEIMAGE;
//	SMenu[MN_NATION_BUDAE].nField[11].x = 34;
//	SMenu[MN_NATION_BUDAE].nField[11].y = 92;

	}
////////////////////////////////////////////////////////////////////////
//	������ ��Ȳ 
	{
	::SMenuReg(MN_NATION_NOW,CENTER_X, 100,NATION_NOW_IMG,0,49);

	//�ݱ�
	SMenu[MN_NATION_NOW].nField[0].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_NOW].nField[0].nSpecialWillDo=SWD_SUBMENU_BACK;
	SMenu[MN_NATION_NOW].nField[0].x = 379;
	SMenu[MN_NATION_NOW].nField[0].y = 302;
	SMenu[MN_NATION_NOW].nField[0].nRectImage = 6;
	SMenu[MN_NATION_NOW].nField[0].nImageNumber = 5;
	SMenu[MN_NATION_NOW].nField[0].nImageType = NATION_NOW_IMG;
	::SetRect(SMenu[MN_NATION_NOW].nField[0].rCheakBox,SMenu[MN_NATION_NOW].nField[0].x,SMenu[MN_NATION_NOW].nField[0].y,
	SMenu[MN_NATION_NOW].nField[0].x+100,SMenu[MN_NATION_NOW].nField[0].y+25);

	//��������
	SMenu[MN_NATION_NOW].nField[1].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_NOW].nField[1].nSpecialWillDo=SWD_NATION_NOW_REFRESH;
	SMenu[MN_NATION_NOW].nField[1].x = 432;
	SMenu[MN_NATION_NOW].nField[1].y = 270;
	SMenu[MN_NATION_NOW].nField[1].nRectImage = 8;
	SMenu[MN_NATION_NOW].nField[1].nImageNumber = 7;
	SMenu[MN_NATION_NOW].nField[1].nImageType = NATION_NOW_IMG;
	::SetRect(SMenu[MN_NATION_NOW].nField[1].rCheakBox,SMenu[MN_NATION_NOW].nField[1].x ,SMenu[MN_NATION_NOW].nField[1].y,
	SMenu[MN_NATION_NOW].nField[1].x+80 ,SMenu[MN_NATION_NOW].nField[1].y+30);

	//�δ����
	SMenu[MN_NATION_NOW].nField[2].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_NOW].nField[2].nSpecialWillDo=SWD_NATION_NOW_BUDAE;
	SMenu[MN_NATION_NOW].nField[2].x = 320;
	SMenu[MN_NATION_NOW].nField[2].y = 270;
	SMenu[MN_NATION_NOW].nField[2].nRectImage = 2;
	SMenu[MN_NATION_NOW].nField[2].nImageNumber = 1;
	SMenu[MN_NATION_NOW].nField[2].nImageType = NATION_NOW_IMG;
	::SetRect(SMenu[MN_NATION_NOW].nField[2].rCheakBox,SMenu[MN_NATION_NOW].nField[2].x ,SMenu[MN_NATION_NOW].nField[2].y,
	SMenu[MN_NATION_NOW].nField[2].x+80 ,SMenu[MN_NATION_NOW].nField[2].y+30);

	//��� �̹��� 3�� ��ȣ�� �ٲ�����.
	SMenu[MN_NATION_NOW].nField[3].nImageType = NATION_NOW_IMG;
	SMenu[MN_NATION_NOW].nField[3].nType = FT_NOMAL_PUT;
	SMenu[MN_NATION_NOW].nField[3].x = 20;
	SMenu[MN_NATION_NOW].nField[3].y = 46;
	SMenu[MN_NATION_NOW].nField[3].nImageNumber = 9;
	
	//���� 1�� ( 1���� ���ο� ������ ��)(�׸� ������ 2��
	SMenu[MN_NATION_NOW].nField[4].nType = FT_NOMAL_PUT;
	SMenu[MN_NATION_NOW].nField[4].x = 187;
	SMenu[MN_NATION_NOW].nField[4].y = 0;
	SMenu[MN_NATION_NOW].nField[4].nImageType = NATION_NOW_IMG;
	SMenu[MN_NATION_NOW].nField[4].nImageNumber = 3;//�Ǵ� 4

	
	// ���� �ð� 1��
	SMenu[MN_NATION_NOW].nField[5].nType = FT_DATA_PUT;
	SMenu[MN_NATION_NOW].nField[5].nImageNumber = DP_REMAIN_TIME_PUT;//�ð� ǲ���� �ٲ���
	SMenu[MN_NATION_NOW].nField[5].x = 415;
	SMenu[MN_NATION_NOW].nField[5].y = 48;
	SMenu[MN_NATION_NOW].nField[5].nRectImage = 150;
	SMenu[MN_NATION_NOW].nField[5].nImageType = NATION_NOW_IMG;

	// �Ʊ� ���� 1��
	SMenu[MN_NATION_NOW].nField[6].nType = FT_DATA_PUT;
	SMenu[MN_NATION_NOW].nField[6].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_NOW].nField[6].x = 406;
	SMenu[MN_NATION_NOW].nField[6].y = 66;
	strcpy(SMenu[MN_NATION_NOW].nField[6].temp,  "�Ʊ� ����" );

	// ���� ���� 1��
	SMenu[MN_NATION_NOW].nField[7].nType = FT_DATA_PUT;
	SMenu[MN_NATION_NOW].nField[7].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_NOW].nField[7].x = 406;
	SMenu[MN_NATION_NOW].nField[7].y = 86;
	strcpy(SMenu[MN_NATION_NOW].nField[7].temp,  "���� ����" );

	// ���� 12 ��
	SMenu[MN_NATION_NOW].nField[8].nType = FT_DATA_PUT;
	SMenu[MN_NATION_NOW].nField[8].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_NOW].nField[8].x = 369;
	SMenu[MN_NATION_NOW].nField[8].y = 119;
	strcpy(SMenu[MN_NATION_NOW].nField[8].temp,  "���� 2 " );

	SMenu[MN_NATION_NOW].nField[9].nType = FT_DATA_PUT;
	SMenu[MN_NATION_NOW].nField[9].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_NOW].nField[9].x = 369;
	SMenu[MN_NATION_NOW].nField[9].y = 138;
	strcpy(SMenu[MN_NATION_NOW].nField[9].temp,  "���� 3 " );

	SMenu[MN_NATION_NOW].nField[10].nType = FT_DATA_PUT;
	SMenu[MN_NATION_NOW].nField[10].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_NOW].nField[10].x = 332;
	SMenu[MN_NATION_NOW].nField[10].y = 156;
	strcpy(SMenu[MN_NATION_NOW].nField[10].temp,  "���� 4 " );

	SMenu[MN_NATION_NOW].nField[11].nType = FT_DATA_PUT;
	SMenu[MN_NATION_NOW].nField[11].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_NOW].nField[11].x = 460;
	SMenu[MN_NATION_NOW].nField[11].y = 119;
	strcpy(SMenu[MN_NATION_NOW].nField[11].temp,  "���� 5 " );

	SMenu[MN_NATION_NOW].nField[12].nType = FT_DATA_PUT;
	SMenu[MN_NATION_NOW].nField[12].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_NOW].nField[12].x = 460;
	SMenu[MN_NATION_NOW].nField[12].y = 138;
	strcpy(SMenu[MN_NATION_NOW].nField[12].temp,  "���� 6 " );

	SMenu[MN_NATION_NOW].nField[13].nType = FT_DATA_PUT;
	SMenu[MN_NATION_NOW].nField[13].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_NOW].nField[13].x = 423;
	SMenu[MN_NATION_NOW].nField[13].y = 156;
	strcpy(SMenu[MN_NATION_NOW].nField[13].temp,  "���� 7 " );

	SMenu[MN_NATION_NOW].nField[14].nType = FT_DATA_PUT;
	SMenu[MN_NATION_NOW].nField[14].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_NOW].nField[14].x = 369;
	SMenu[MN_NATION_NOW].nField[14].y = 189;
	strcpy(SMenu[MN_NATION_NOW].nField[14].temp,  "���� 8 " );

	SMenu[MN_NATION_NOW].nField[15].nType = FT_DATA_PUT;
	SMenu[MN_NATION_NOW].nField[15].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_NOW].nField[15].x = 369;
	SMenu[MN_NATION_NOW].nField[15].y = 208;
	strcpy(SMenu[MN_NATION_NOW].nField[15].temp,  "���� 9 " );

	SMenu[MN_NATION_NOW].nField[16].nType = FT_DATA_PUT;
	SMenu[MN_NATION_NOW].nField[16].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_NOW].nField[16].x = 332;
	SMenu[MN_NATION_NOW].nField[16].y = 226;
	strcpy(SMenu[MN_NATION_NOW].nField[16].temp,  "���� 10 " );

	SMenu[MN_NATION_NOW].nField[17].nType = FT_DATA_PUT;
	SMenu[MN_NATION_NOW].nField[17].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_NOW].nField[17].x = 460;
	SMenu[MN_NATION_NOW].nField[17].y = 189;
	strcpy(SMenu[MN_NATION_NOW].nField[17].temp,  "���� 11 " );

	SMenu[MN_NATION_NOW].nField[18].nType = FT_DATA_PUT;
	SMenu[MN_NATION_NOW].nField[18].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_NOW].nField[18].x = 460;
	SMenu[MN_NATION_NOW].nField[18].y = 208;
	strcpy(SMenu[MN_NATION_NOW].nField[18].temp,  "���� 12 " );

	SMenu[MN_NATION_NOW].nField[19].nType = FT_DATA_PUT;
	SMenu[MN_NATION_NOW].nField[19].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_NOW].nField[19].x = 423;
	SMenu[MN_NATION_NOW].nField[19].y = 226;
	strcpy(SMenu[MN_NATION_NOW].nField[19].temp,  "���� 1 " );
	
	// �δ� ��ũ 4�� 
	SMenu[MN_NATION_NOW].nField[20].nImageType = NATION_NOW_IMG;
	SMenu[MN_NATION_NOW].nField[20].nType = FT_NOMAL_PUT_BY_NWILLDO_CUSTOM;
	SMenu[MN_NATION_NOW].nField[20].x = 331;
	SMenu[MN_NATION_NOW].nField[20].y = 119;
	SMenu[MN_NATION_NOW].nField[20].nImageNumber = 21;
 
	SMenu[MN_NATION_NOW].nField[21].nImageType = NATION_NOW_IMG;
	SMenu[MN_NATION_NOW].nField[21].nType = FT_NOMAL_PUT_BY_NWILLDO_CUSTOM;
	SMenu[MN_NATION_NOW].nField[21].x = 423;
	SMenu[MN_NATION_NOW].nField[21].y = 119;
	SMenu[MN_NATION_NOW].nField[21].nImageNumber = 22;

	SMenu[MN_NATION_NOW].nField[22].nImageType = NATION_NOW_IMG;
	SMenu[MN_NATION_NOW].nField[22].nType = FT_NOMAL_PUT_BY_NWILLDO_CUSTOM;
	SMenu[MN_NATION_NOW].nField[22].x = 331;
	SMenu[MN_NATION_NOW].nField[22].y = 189;
	SMenu[MN_NATION_NOW].nField[22].nImageNumber = 23;

	SMenu[MN_NATION_NOW].nField[23].nImageType = NATION_NOW_IMG;
	SMenu[MN_NATION_NOW].nField[23].nType = FT_NOMAL_PUT_BY_NWILLDO_CUSTOM;
	SMenu[MN_NATION_NOW].nField[23].x = 423;
	SMenu[MN_NATION_NOW].nField[23].y = 189;
	SMenu[MN_NATION_NOW].nField[23].nImageNumber = 24;
	
	// ���� 4��
	SMenu[MN_NATION_NOW].nField[24].nImageType = NATION_NOW_IMG;
	SMenu[MN_NATION_NOW].nField[24].nType = FT_NOMAL_PUT_BY_NWILLDO;
	SMenu[MN_NATION_NOW].nField[24].x = 19;
	SMenu[MN_NATION_NOW].nField[24].y = 44;
	SMenu[MN_NATION_NOW].nField[24].nImageNumber =13;

	SMenu[MN_NATION_NOW].nField[25].nImageType = NATION_NOW_IMG;
	SMenu[MN_NATION_NOW].nField[25].nType = FT_NOMAL_PUT_BY_NWILLDO;
	SMenu[MN_NATION_NOW].nField[25].x = 19;
	SMenu[MN_NATION_NOW].nField[25].y = 44;
	SMenu[MN_NATION_NOW].nField[25].nImageNumber =13;
	
	SMenu[MN_NATION_NOW].nField[26].nImageType = NATION_NOW_IMG;
	SMenu[MN_NATION_NOW].nField[26].nType = FT_NOMAL_PUT_BY_NWILLDO;
	SMenu[MN_NATION_NOW].nField[26].x = 19;
	SMenu[MN_NATION_NOW].nField[26].y = 44;
	SMenu[MN_NATION_NOW].nField[26].nImageNumber =13;

	SMenu[MN_NATION_NOW].nField[27].nImageType = NATION_NOW_IMG;
	SMenu[MN_NATION_NOW].nField[27].nType = FT_NOMAL_PUT_BY_NWILLDO;
	SMenu[MN_NATION_NOW].nField[27].x = 19;
	SMenu[MN_NATION_NOW].nField[27].y = 44;
	SMenu[MN_NATION_NOW].nField[27].nImageNumber =13;

	//��輮 5�� 
	SMenu[MN_NATION_NOW].nField[28].nImageType = NATION_NOW_IMG;
	SMenu[MN_NATION_NOW].nField[28].nType = FT_NOMAL_PUT_BY_NWILLDO;
	SMenu[MN_NATION_NOW].nField[28].x = 19;
	SMenu[MN_NATION_NOW].nField[28].y = 44;
	SMenu[MN_NATION_NOW].nField[28].nImageNumber =12;

	SMenu[MN_NATION_NOW].nField[29].nImageType = NATION_NOW_IMG;
	SMenu[MN_NATION_NOW].nField[29].nType = FT_NOMAL_PUT_BY_NWILLDO;
	SMenu[MN_NATION_NOW].nField[29].x = 19;
	SMenu[MN_NATION_NOW].nField[29].y = 44;
	SMenu[MN_NATION_NOW].nField[29].nImageNumber =12;

	SMenu[MN_NATION_NOW].nField[30].nImageType = NATION_NOW_IMG;
	SMenu[MN_NATION_NOW].nField[30].nType = FT_NOMAL_PUT_BY_NWILLDO;
	SMenu[MN_NATION_NOW].nField[30].x = 19;
	SMenu[MN_NATION_NOW].nField[30].y = 44;
	SMenu[MN_NATION_NOW].nField[30].nImageNumber =12;

	SMenu[MN_NATION_NOW].nField[31].nImageType = NATION_NOW_IMG;
	SMenu[MN_NATION_NOW].nField[31].nType = FT_NOMAL_PUT_BY_NWILLDO;
	SMenu[MN_NATION_NOW].nField[31].x = 19;
	SMenu[MN_NATION_NOW].nField[31].y = 44;
	SMenu[MN_NATION_NOW].nField[31].nImageNumber =12;
	
	SMenu[MN_NATION_NOW].nField[32].nImageType = NATION_NOW_IMG;
	SMenu[MN_NATION_NOW].nField[32].nType = FT_NOMAL_PUT_BY_NWILLDO;
	SMenu[MN_NATION_NOW].nField[32].x = 19;
	SMenu[MN_NATION_NOW].nField[32].y = 44;
	SMenu[MN_NATION_NOW].nField[32].nImageNumber =12;

	//�ڱ� ��ġ ��Ÿ�� //010928 lsw
	SMenu[MN_NATION_NOW].nField[33].nImageType = NATION_NOW_IMG;
	SMenu[MN_NATION_NOW].nField[33].nType = FT_NOMAL_PUT_BY_NWILLDO;
	SMenu[MN_NATION_NOW].nField[33].x = 0;
	SMenu[MN_NATION_NOW].nField[33].y = 0;
	SMenu[MN_NATION_NOW].nField[33].nWillDo = 1;
	SMenu[MN_NATION_NOW].nField[33].nImageNumber =33;

	}
////////////////////////////////////////////////////////////////////////
//���� ����
	{
	::SMenuReg(MN_NATION_LADDER,CENTER_X, 0,NATION_LADDER_IMG,0,2);
	
	SMenu[MN_NATION_LADDER].nField[0].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_LADDER].nField[0].nSpecialWillDo=SWD_SUBMENU_BACK;
	SMenu[MN_NATION_LADDER].nField[0].x = 30;
	SMenu[MN_NATION_LADDER].nField[0].y = 200;
	SMenu[MN_NATION_LADDER].nField[0].nRectImage = 1;
	SMenu[MN_NATION_LADDER].nField[0].nImageNumber = 1;
	SMenu[MN_NATION_LADDER].nField[0].nImageType = NATION_LADDER_IMG;
	::SetRect(SMenu[MN_NATION_LADDER].nField[0].rCheakBox,30,200,110,230);

	}
	
////////////////////////////////////////////////////////////////////////	
// ���� ���� ��ǥ 
	{

	::SMenuReg(MN_NATION_JOIN,CENTER_X, 100,NATION_JOIN_IMG,0,9);
	
	//�ݱ�
	SMenu[MN_NATION_JOIN].nField[0].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_JOIN].nField[0].nSpecialWillDo=SWD_SUBMENU_BACK;
	SMenu[MN_NATION_JOIN].nField[0].x = 176;
	SMenu[MN_NATION_JOIN].nField[0].y = 305;
	SMenu[MN_NATION_JOIN].nField[0].nRectImage = 5;
	SMenu[MN_NATION_JOIN].nField[0].nImageNumber = 4;
	SMenu[MN_NATION_JOIN].nField[0].nImageType = NATION_JOIN_IMG;
	::SetRect(SMenu[MN_NATION_JOIN].nField[0].rCheakBox,SMenu[MN_NATION_JOIN].nField[0].x,SMenu[MN_NATION_JOIN].nField[0].y,
		SMenu[MN_NATION_JOIN].nField[0].x+100,SMenu[MN_NATION_JOIN].nField[0].y+30);

	//���� ���� �ϱ� (join)
	SMenu[MN_NATION_JOIN].nField[1].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_JOIN].nField[1].nSpecialWillDo=SWD_NATION_JOIN_JOIN_WAR;
	SMenu[MN_NATION_JOIN].nField[1].x = 66;
	SMenu[MN_NATION_JOIN].nField[1].y = 305;
	SMenu[MN_NATION_JOIN].nField[1].nRectImage = 7;
	SMenu[MN_NATION_JOIN].nField[1].nImageNumber = 6;
	SMenu[MN_NATION_JOIN].nField[1].nImageType = NATION_JOIN_IMG;
	::SetRect(SMenu[MN_NATION_JOIN].nField[1].rCheakBox,SMenu[MN_NATION_JOIN].nField[1].x,SMenu[MN_NATION_JOIN].nField[1].y,
		SMenu[MN_NATION_JOIN].nField[1].x+100,SMenu[MN_NATION_JOIN].nField[1].y+30);
	
	//�׸�
	SMenu[MN_NATION_JOIN].nField[2].nType = FT_NOMAL_PUT;
	SMenu[MN_NATION_JOIN].nField[2].x = 22;
	SMenu[MN_NATION_JOIN].nField[2].y = 51;
	SMenu[MN_NATION_JOIN].nField[2].nImageNumber = 1;
	SMenu[MN_NATION_JOIN].nField[2].nImageType = NATION_JOIN_IMG;

	SMenu[MN_NATION_JOIN].nField[3].nType = FT_DATA_PUT;
	SMenu[MN_NATION_JOIN].nField[3].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_JOIN].nField[3].x = 37;
	SMenu[MN_NATION_JOIN].nField[3].y = 95;
	SMenu[MN_NATION_JOIN].nField[3].nRectImage = 150;
	//010927 lsw
//	strcpy(SMenu[MN_NATION_JOIN].nField[3].temp,  lan->OutputMessage(0,142) );
	
	//�׷���
	SMenu[MN_NATION_JOIN].nField[4].nType = FT_DATA_PUT;
	SMenu[MN_NATION_JOIN].nField[4].nImageNumber = DP_NATION_JOIN_STATUS_BAR;
	SMenu[MN_NATION_JOIN].nField[4].x = 238;
	SMenu[MN_NATION_JOIN].nField[4].y = 275;
	SMenu[MN_NATION_JOIN].nField[4].nRectImage = 8;
	SMenu[MN_NATION_JOIN].nField[4].nWillDo = 0;
	SMenu[MN_NATION_JOIN].nField[4].nImageType = NATION_JOIN_IMG;

	//���� �ο� ����
	SMenu[MN_NATION_JOIN].nField[5].nType = FT_DATA_PUT;
	SMenu[MN_NATION_JOIN].nField[5].nImageNumber = DP_STRING_PUT;
	SMenu[MN_NATION_JOIN].nField[5].x = 274;
	SMenu[MN_NATION_JOIN].nField[5].y = 280;
	SMenu[MN_NATION_JOIN].nField[5].nImageType = NATION_JOIN_IMG;


}
	
	

//////////////////////////////////////////////////////////////////
//010730 lsw �뺴 ����
	{
	::SMenuReg(MN_NATION_GUEST,CENTER_X, 100,NATION_GUEST_IMG,0,4);

	//�ݱ�
	SMenu[MN_NATION_GUEST].nField[0].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_GUEST].nField[0].nSpecialWillDo=SWD_SUBMENU_BACK;
	SMenu[MN_NATION_GUEST].nField[0].x = 70;
	SMenu[MN_NATION_GUEST].nField[0].y = 164;
	SMenu[MN_NATION_GUEST].nField[0].nRectImage = 2;
	SMenu[MN_NATION_GUEST].nField[0].nImageNumber = 1;
	SMenu[MN_NATION_GUEST].nField[0].nImageType = NATION_GUEST_IMG;
	::SetRect(SMenu[MN_NATION_GUEST].nField[0].rCheakBox,SMenu[MN_NATION_GUEST].nField[0].x,SMenu[MN_NATION_GUEST].nField[0].y,
		SMenu[MN_NATION_GUEST].nField[0].x+100,SMenu[MN_NATION_GUEST].nField[0].y+30);
	
	//�뺴 �δ� ���
	SMenu[MN_NATION_GUEST].nField[1].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_GUEST].nField[1].nSpecialWillDo=SWD_NATION_GUEST_BUDAE;
	SMenu[MN_NATION_GUEST].nField[1].x = 43;
	SMenu[MN_NATION_GUEST].nField[1].y = 54;
	SMenu[MN_NATION_GUEST].nField[1].nRectImage = 6;
	SMenu[MN_NATION_GUEST].nField[1].nImageNumber = 5;
	SMenu[MN_NATION_GUEST].nField[1].nImageType = NATION_GUEST_IMG;
	::SetRect(SMenu[MN_NATION_GUEST].nField[1].rCheakBox,SMenu[MN_NATION_GUEST].nField[1].x,SMenu[MN_NATION_GUEST].nField[1].y,
		SMenu[MN_NATION_GUEST].nField[1].x+100,SMenu[MN_NATION_GUEST].nField[1].y+30);

	//��������
	SMenu[MN_NATION_GUEST].nField[2].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_GUEST].nField[2].nSpecialWillDo=SWD_NATION_GUEST_JOIN;
	SMenu[MN_NATION_GUEST].nField[2].nWillDo=1;
	SMenu[MN_NATION_GUEST].nField[2].x = 43;
	SMenu[MN_NATION_GUEST].nField[2].y = 84;
	SMenu[MN_NATION_GUEST].nField[2].nRectImage = 4;
	SMenu[MN_NATION_GUEST].nField[2].nImageNumber = 3;
	SMenu[MN_NATION_GUEST].nField[2].nImageType = NATION_GUEST_IMG;
	::SetRect(SMenu[MN_NATION_GUEST].nField[2].rCheakBox,SMenu[MN_NATION_GUEST].nField[2].x,SMenu[MN_NATION_GUEST].nField[2].y,
		SMenu[MN_NATION_GUEST].nField[2].x+100,SMenu[MN_NATION_GUEST].nField[2].y+30);

	//������ ����
	SMenu[MN_NATION_GUEST].nField[3].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_GUEST].nField[3].nSpecialWillDo=SWD_NATION_GUEST_JOIN;
	SMenu[MN_NATION_GUEST].nField[3].nWillDo=2;
	SMenu[MN_NATION_GUEST].nField[3].x = 43;
	SMenu[MN_NATION_GUEST].nField[3].y = 114;
	SMenu[MN_NATION_GUEST].nField[3].nRectImage = 8;
	SMenu[MN_NATION_GUEST].nField[3].nImageNumber = 7;
	SMenu[MN_NATION_GUEST].nField[3].nImageType = NATION_GUEST_IMG;
	::SetRect(SMenu[MN_NATION_GUEST].nField[3].rCheakBox,SMenu[MN_NATION_GUEST].nField[3].x,SMenu[MN_NATION_GUEST].nField[3].y,
		SMenu[MN_NATION_GUEST].nField[3].x+100,SMenu[MN_NATION_GUEST].nField[3].y+30);

	}
	

//////////////////////////////////////////////////////////////////////////////////////////
//�����ͷ� �̵�
	{
	::SMenuReg(MN_NATION_GOTO_WAR_MAP,CENTER_X, 150,NATION_GOTO_WAR_MAP_IMG,0,6);
	
	//�ݱ�
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[0].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[0].nSpecialWillDo=SWD_SUBMENU_BACK;
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[0].x = 70;
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[0].y = 164;
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[0].nRectImage = 8;
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[0].nImageNumber = 7;
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[0].nImageType = NATION_GOTO_WAR_MAP_IMG;
	::SetRect(SMenu[MN_NATION_GOTO_WAR_MAP].nField[0].rCheakBox,SMenu[MN_NATION_GOTO_WAR_MAP].nField[0].x,SMenu[MN_NATION_GOTO_WAR_MAP].nField[0].y,
		SMenu[MN_NATION_GOTO_WAR_MAP].nField[0].x+100,SMenu[MN_NATION_GOTO_WAR_MAP].nField[0].y+30);
	//������ 1
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[1].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[1].nSpecialWillDo=SWD_NATION_GOTO_WAR_MAP_TARGET;
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[1].nWillDo=1;
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[1].x = 43;
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[1].y = 54;
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[1].nRectImage = 2;
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[1].nImageNumber = 1;
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[1].nImageType = NATION_GOTO_WAR_MAP_IMG;
	::SetRect(SMenu[MN_NATION_GOTO_WAR_MAP].nField[1].rCheakBox,SMenu[MN_NATION_GOTO_WAR_MAP].nField[1].x,SMenu[MN_NATION_GOTO_WAR_MAP].nField[1].y,
		SMenu[MN_NATION_GOTO_WAR_MAP].nField[1].x+100,SMenu[MN_NATION_GOTO_WAR_MAP].nField[1].y+30);
	//������ 2
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[2].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[2].nSpecialWillDo=SWD_NATION_GOTO_WAR_MAP_TARGET;
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[2].nWillDo=2;
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[2].x = 43;
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[2].y = 84;
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[2].nRectImage = 4;
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[2].nImageNumber = 3;
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[2].nImageType = NATION_GOTO_WAR_MAP_IMG;
	::SetRect(SMenu[MN_NATION_GOTO_WAR_MAP].nField[2].rCheakBox,SMenu[MN_NATION_GOTO_WAR_MAP].nField[2].x,SMenu[MN_NATION_GOTO_WAR_MAP].nField[2].y,
		SMenu[MN_NATION_GOTO_WAR_MAP].nField[2].x+100,SMenu[MN_NATION_GOTO_WAR_MAP].nField[2].y+30);

	//������ 3
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[3].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[3].nSpecialWillDo=SWD_NATION_GOTO_WAR_MAP_TARGET;
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[3].nWillDo=3;
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[3].x = 43;
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[3].y = 114;
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[3].nRectImage = 6;
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[3].nImageNumber = 5;
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[3].nImageType = NATION_GOTO_WAR_MAP_IMG;
	::SetRect(SMenu[MN_NATION_GOTO_WAR_MAP].nField[3].rCheakBox,SMenu[MN_NATION_GOTO_WAR_MAP].nField[3].x,SMenu[MN_NATION_GOTO_WAR_MAP].nField[3].y,
		SMenu[MN_NATION_GOTO_WAR_MAP].nField[3].x+100,SMenu[MN_NATION_GOTO_WAR_MAP].nField[3].y+30);
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	//�����ͷ� �̵�
	//������ �޼��� �ڽ�(?) yes no
	{
	::SMenuReg(MN_NATION_MSGBOX, CENTER_X, 50, NATION_MSGBOX_IMG,0,3);

	//���
	SMenu[MN_NATION_MSGBOX].nField[0].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_MSGBOX].nField[0].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_NATION_MSGBOX].nField[0].x = 177;
	SMenu[MN_NATION_MSGBOX].nField[0].y = 305;
	SMenu[MN_NATION_MSGBOX].nField[0].nRectImage = 2;
	SMenu[MN_NATION_MSGBOX].nField[0].nImageNumber = 1;
	SMenu[MN_NATION_MSGBOX].nField[0].nImageType = NATION_MSGBOX_IMG;
	::SetRect(SMenu[MN_NATION_MSGBOX].nField[0].rCheakBox,SMenu[MN_NATION_MSGBOX].nField[0].x,SMenu[MN_NATION_MSGBOX].nField[0].y,
		SMenu[MN_NATION_MSGBOX].nField[0].x+100,SMenu[MN_NATION_MSGBOX].nField[0].y+30);
	
	//����
	SMenu[MN_NATION_MSGBOX].nField[1].nType =			FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_MSGBOX].nField[1].nSpecialWillDo=	SWD_NATION_MSGBOX_OK;
	SMenu[MN_NATION_MSGBOX].nField[1].nWillDo=0;
	SMenu[MN_NATION_MSGBOX].nField[1].x = 65;
	SMenu[MN_NATION_MSGBOX].nField[1].y = 305;
	SMenu[MN_NATION_MSGBOX].nField[1].nRectImage = 4;
	SMenu[MN_NATION_MSGBOX].nField[1].nImageNumber = 3;
	SMenu[MN_NATION_MSGBOX].nField[1].nImageType = NATION_MSGBOX_IMG;
	::SetRect(SMenu[MN_NATION_MSGBOX].nField[1].rCheakBox,SMenu[MN_NATION_MSGBOX].nField[1].x,SMenu[MN_NATION_MSGBOX].nField[1].y,
		SMenu[MN_NATION_MSGBOX].nField[1].x+100,SMenu[MN_NATION_MSGBOX].nField[1].y+30);

	//�����
	SMenu[MN_NATION_MSGBOX].nField[2].nType = FT_DATA_PUT;
	SMenu[MN_NATION_MSGBOX].nField[2].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_MSGBOX].nField[2].x = 40;
	SMenu[MN_NATION_MSGBOX].nField[2].y = 60;
	SMenu[MN_NATION_MSGBOX].nField[2].nRectImage = 220;
	SMenu[MN_NATION_MSGBOX].nField[2].nImageType = NATION_MSGBOX_IMG;
	::SetRect(SMenu[MN_NATION_MSGBOX].nField[2].rCheakBox,SMenu[MN_NATION_MSGBOX].nField[0].x,SMenu[MN_NATION_MSGBOX].nField[0].y,
		SMenu[MN_NATION_MSGBOX].nField[2].x+100,SMenu[MN_NATION_MSGBOX].nField[0].y+30);
	strcpy(SMenu[MN_NATION_MSGBOX].nField[2].temp,  "�� ũũũ.. ��.��;; �������������K~" );
	}
////////////////////////////////////////////////////////////////////
// �� �޼���(DONE)
	{
	::SMenuReg(MN_NATION_DONEBOX, CENTER_X, 100, NATION_DONEBOX_IMG,0,11);
	
	//���
	SMenu[MN_NATION_DONEBOX].nField[0].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_DONEBOX].nField[0].nSpecialWillDo=SWD_SUBMENU_BACK;
	SMenu[MN_NATION_DONEBOX].nField[0].x = 123;
	SMenu[MN_NATION_DONEBOX].nField[0].y = 305;
	SMenu[MN_NATION_DONEBOX].nField[0].nRectImage = 2;
	SMenu[MN_NATION_DONEBOX].nField[0].nImageNumber = 1;
	SMenu[MN_NATION_DONEBOX].nField[0].nImageType = NATION_DONEBOX_IMG;
	::SetRect(SMenu[MN_NATION_DONEBOX].nField[0].rCheakBox,SMenu[MN_NATION_DONEBOX].nField[0].x,SMenu[MN_NATION_DONEBOX].nField[0].y,
		SMenu[MN_NATION_DONEBOX].nField[0].x+100,SMenu[MN_NATION_DONEBOX].nField[0].y+30);
	
	//�����
	SMenu[MN_NATION_DONEBOX].nField[1].nType = FT_DATA_PUT;
	SMenu[MN_NATION_DONEBOX].nField[1].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_DONEBOX].nField[1].x = 40;
	SMenu[MN_NATION_DONEBOX].nField[1].y = 60;
	SMenu[MN_NATION_DONEBOX].nField[1].nRectImage = 250;
	SMenu[MN_NATION_DONEBOX].nField[1].nImageType = NATION_DONEBOX_IMG;
	strcpy(SMenu[MN_NATION_DONEBOX].nField[1].temp,  "���� �ε���~�Դϴ� ���̿� �°�.." );
	}

	//������ ��Ƽ��..
	{
	::SMenuReg(MN_NATION_TODAY_NOTICE,CENTER_X,100, NATION_TODAY_NOTICE_IMG, 0,15);
	
	SMenu[MN_NATION_TODAY_NOTICE].nField[0].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_TODAY_NOTICE].nField[0].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_NATION_TODAY_NOTICE].nField[0].x = 121;
	SMenu[MN_NATION_TODAY_NOTICE].nField[0].y = 305;
	SMenu[MN_NATION_TODAY_NOTICE].nField[0].nRectImage = 2;
	SMenu[MN_NATION_TODAY_NOTICE].nField[0].nImageNumber = 1;
	SMenu[MN_NATION_TODAY_NOTICE].nField[0].nImageType = NATION_TODAY_NOTICE_IMG;
	::SetRect(SMenu[MN_NATION_TODAY_NOTICE].nField[0].rCheakBox,SMenu[MN_NATION_TODAY_NOTICE].nField[0].x,SMenu[MN_NATION_TODAY_NOTICE].nField[0].y,
		SMenu[MN_NATION_TODAY_NOTICE].nField[0].x+100,SMenu[MN_NATION_TODAY_NOTICE].nField[0].y+30);
	
	//1�� �׸� �ٴڿ� �򸮴°�
	SMenu[MN_NATION_TODAY_NOTICE].nField[1].nType = FT_SHOW_SPECIAL_WILLDO_AUTO_PUT	;
	SMenu[MN_NATION_TODAY_NOTICE].nField[1].nSpecialWillDo=SWD_NATION_TODAY_NOTICE_IMAGE;
	SMenu[MN_NATION_TODAY_NOTICE].nField[1].nImageNumber	= 9;
	SMenu[MN_NATION_TODAY_NOTICE].nField[1].nRectImage		= 10;
	SMenu[MN_NATION_TODAY_NOTICE].nField[1].x = 23;
	SMenu[MN_NATION_TODAY_NOTICE].nField[1].y = 55;
	SMenu[MN_NATION_TODAY_NOTICE].nField[1].nImageType = NATION_TODAY_NOTICE_IMG;
	::SetRect(SMenu[MN_NATION_TODAY_NOTICE].nField[1].rCheakBox,SMenu[MN_NATION_TODAY_NOTICE].nField[1].x,SMenu[MN_NATION_TODAY_NOTICE].nField[1].y,
		SMenu[MN_NATION_TODAY_NOTICE].nField[1].x+80,SMenu[MN_NATION_TODAY_NOTICE].nField[1].y+55);
	
	//2�� �׸� �ٴڿ� �򸮴°�
	SMenu[MN_NATION_TODAY_NOTICE].nField[2].nType = FT_SHOW_SPECIAL_WILLDO_AUTO_PUT	;
	SMenu[MN_NATION_TODAY_NOTICE].nField[2].nSpecialWillDo=SWD_NATION_TODAY_NOTICE_IMAGE;
	SMenu[MN_NATION_TODAY_NOTICE].nField[2].nImageNumber	= 9;
	SMenu[MN_NATION_TODAY_NOTICE].nField[2].nRectImage		= 10;
	SMenu[MN_NATION_TODAY_NOTICE].nField[2].x = 120;
	SMenu[MN_NATION_TODAY_NOTICE].nField[2].y = 55;
	SMenu[MN_NATION_TODAY_NOTICE].nField[2].nImageType = NATION_TODAY_NOTICE_IMG;
	::SetRect(SMenu[MN_NATION_TODAY_NOTICE].nField[2].rCheakBox,SMenu[MN_NATION_TODAY_NOTICE].nField[2].x,SMenu[MN_NATION_TODAY_NOTICE].nField[2].y,
		SMenu[MN_NATION_TODAY_NOTICE].nField[2].x+80,SMenu[MN_NATION_TODAY_NOTICE].nField[2].y+55);
	
	//3�� �׸� �ٴڿ� �򸮴°�
	SMenu[MN_NATION_TODAY_NOTICE].nField[3].nType = FT_SHOW_SPECIAL_WILLDO_AUTO_PUT	;
	SMenu[MN_NATION_TODAY_NOTICE].nField[3].nSpecialWillDo=SWD_NATION_TODAY_NOTICE_IMAGE;
	SMenu[MN_NATION_TODAY_NOTICE].nField[3].nImageNumber	= 9;
	SMenu[MN_NATION_TODAY_NOTICE].nField[3].nRectImage		= 10;
	SMenu[MN_NATION_TODAY_NOTICE].nField[3].x = 217;
	SMenu[MN_NATION_TODAY_NOTICE].nField[3].y = 55;
	SMenu[MN_NATION_TODAY_NOTICE].nField[3].nImageType = NATION_TODAY_NOTICE_IMG;
	::SetRect(SMenu[MN_NATION_TODAY_NOTICE].nField[3].rCheakBox,SMenu[MN_NATION_TODAY_NOTICE].nField[3].x,SMenu[MN_NATION_TODAY_NOTICE].nField[3].y,
		SMenu[MN_NATION_TODAY_NOTICE].nField[3].x+80,SMenu[MN_NATION_TODAY_NOTICE].nField[3].y+55);
	
	//���� 1 2 3 
	SMenu[MN_NATION_TODAY_NOTICE].nField[4].nType = FT_DATA_PUT;
	SMenu[MN_NATION_TODAY_NOTICE].nField[4].nImageNumber = DP_STRING_PUT;
	SMenu[MN_NATION_TODAY_NOTICE].nField[4].x = 63;
	SMenu[MN_NATION_TODAY_NOTICE].nField[4].y = 117;
	SMenu[MN_NATION_TODAY_NOTICE].nField[4].nRectImage = 200;
	
	SMenu[MN_NATION_TODAY_NOTICE].nField[5].nType = FT_DATA_PUT;
	SMenu[MN_NATION_TODAY_NOTICE].nField[5].nImageNumber = DP_STRING_PUT;
	SMenu[MN_NATION_TODAY_NOTICE].nField[5].x = 160;
	SMenu[MN_NATION_TODAY_NOTICE].nField[5].y = 117;
	SMenu[MN_NATION_TODAY_NOTICE].nField[5].nRectImage = 200;
	
	SMenu[MN_NATION_TODAY_NOTICE].nField[6].nType = FT_DATA_PUT;
	SMenu[MN_NATION_TODAY_NOTICE].nField[6].nImageNumber = DP_STRING_PUT;
	SMenu[MN_NATION_TODAY_NOTICE].nField[6].x = 257;
	SMenu[MN_NATION_TODAY_NOTICE].nField[6].y = 117;
	SMenu[MN_NATION_TODAY_NOTICE].nField[6].nRectImage = 200;
	
	//�����
	SMenu[MN_NATION_TODAY_NOTICE].nField[7].nType = FT_DATA_PUT;
	SMenu[MN_NATION_TODAY_NOTICE].nField[7].nImageNumber = DP_RECT_STRING_PUT_CUSTOM;
	SMenu[MN_NATION_TODAY_NOTICE].nField[7].x = 33;
	SMenu[MN_NATION_TODAY_NOTICE].nField[7].y = 148;
	SMenu[MN_NATION_TODAY_NOTICE].nField[7].nRectImage = 260;
	SMenu[MN_NATION_TODAY_NOTICE].nField[7].nImageType = 15;
	strcpy(SMenu[MN_NATION_TODAY_NOTICE].nField[7].temp,  " " );
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	//�������ǰ���� ������
	{
	::SMenuReg(MN_NATION_WAR_RESULT, CENTER_X, 30, NATION_WAR_RESULT_IMG,0,11);//�¸�
//	::SMenuReg(MN_NATION_WAR_RESULT, 150, 100, NATION_WAR_RESULT_IMG,4,3);//�й�

	SMenu[MN_NATION_WAR_RESULT].nField[0].nType = FT_DATA_PUT;
	SMenu[MN_NATION_WAR_RESULT].nField[0].nImageNumber = DP_NATION_WAR_RESULT_ANIMATION;
	SMenu[MN_NATION_WAR_RESULT].nField[0].x = 180;
	SMenu[MN_NATION_WAR_RESULT].nField[0].y = 250;
	SMenu[MN_NATION_WAR_RESULT].nField[0].nImageType = 0;	//�̰峪 ����
	SMenu[MN_NATION_WAR_RESULT].nField[0].nSpecialWillDo= 0;//���� ���� ����( FF >> �� 2 REW << �� -2
	SMenu[MN_NATION_WAR_RESULT].nField[0].nWillDo		= 0;//���� ������
	SMenu[MN_NATION_WAR_RESULT].nField[0].nRectImage	= 0;//������ ����

	//���̼��� �����
	SMenu[MN_NATION_WAR_RESULT].nField[1].nType = FT_DATA_PUT;
	SMenu[MN_NATION_WAR_RESULT].nField[1].nImageNumber = DP_NATION_WAR_RESULT_BAR;
	SMenu[MN_NATION_WAR_RESULT].nField[1].x = 516;		// BBD 040313
	SMenu[MN_NATION_WAR_RESULT].nField[1].y = 98;
	SMenu[MN_NATION_WAR_RESULT].nField[1].nWillDo = 100;
	SMenu[MN_NATION_WAR_RESULT].nField[1].nSpecialWillDo = 0;
	SMenu[MN_NATION_WAR_RESULT].nField[1].nRectImage = 9;
	::SetRect(SMenu[MN_NATION_WAR_RESULT].nField[1].rCheakBox,SMenu[MN_NATION_WAR_RESULT].nField[0].x,SMenu[MN_NATION_WAR_RESULT].nField[0].y,
		SMenu[MN_NATION_WAR_RESULT].nField[1].x+100,SMenu[MN_NATION_WAR_RESULT].nField[0].y+30);
	
	//������ �����
	SMenu[MN_NATION_WAR_RESULT].nField[2].nType = FT_DATA_PUT;
	SMenu[MN_NATION_WAR_RESULT].nField[2].nImageNumber = DP_NATION_WAR_RESULT_BAR;
	SMenu[MN_NATION_WAR_RESULT].nField[2].x = 516;		// BBD 040313
	SMenu[MN_NATION_WAR_RESULT].nField[2].y = 139;
	SMenu[MN_NATION_WAR_RESULT].nField[2].nWillDo = 100;
	SMenu[MN_NATION_WAR_RESULT].nField[2].nSpecialWillDo = 0;
	SMenu[MN_NATION_WAR_RESULT].nField[2].nRectImage = 8;
	::SetRect(SMenu[MN_NATION_WAR_RESULT].nField[2].rCheakBox,SMenu[MN_NATION_WAR_RESULT].nField[0].x,SMenu[MN_NATION_WAR_RESULT].nField[0].y,
		SMenu[MN_NATION_WAR_RESULT].nField[2].x+100,SMenu[MN_NATION_WAR_RESULT].nField[0].y+30);
	
	//���̼��� ������
	SMenu[MN_NATION_WAR_RESULT].nField[3].nType = FT_DATA_PUT;
	SMenu[MN_NATION_WAR_RESULT].nField[3].nImageNumber = DP_NATION_WAR_RESULT_BAR;
	SMenu[MN_NATION_WAR_RESULT].nField[3].x = 516;		// BBD 040313
	SMenu[MN_NATION_WAR_RESULT].nField[3].y = 183;
	SMenu[MN_NATION_WAR_RESULT].nField[3].nWillDo = 100;
	SMenu[MN_NATION_WAR_RESULT].nField[3].nSpecialWillDo = 0;
	SMenu[MN_NATION_WAR_RESULT].nField[3].nRectImage = 9;
	::SetRect(SMenu[MN_NATION_WAR_RESULT].nField[3].rCheakBox,SMenu[MN_NATION_WAR_RESULT].nField[0].x,SMenu[MN_NATION_WAR_RESULT].nField[0].y,
		SMenu[MN_NATION_WAR_RESULT].nField[3].x+100,SMenu[MN_NATION_WAR_RESULT].nField[0].y+30);
	
	//���̼��� - �Ͻ� ������
	SMenu[MN_NATION_WAR_RESULT].nField[4].nType = FT_DATA_PUT;
	SMenu[MN_NATION_WAR_RESULT].nField[4].nImageNumber = DP_NATION_WAR_RESULT_BAR;
	SMenu[MN_NATION_WAR_RESULT].nField[4].x = 516;		// BBD 040313
	SMenu[MN_NATION_WAR_RESULT].nField[4].y = 228;
	SMenu[MN_NATION_WAR_RESULT].nField[4].nWillDo = 100;
	SMenu[MN_NATION_WAR_RESULT].nField[4].nSpecialWillDo = 0;
	SMenu[MN_NATION_WAR_RESULT].nField[4].nRectImage = 13;
	::SetRect(SMenu[MN_NATION_WAR_RESULT].nField[4].rCheakBox,SMenu[MN_NATION_WAR_RESULT].nField[0].x,SMenu[MN_NATION_WAR_RESULT].nField[0].y,
		SMenu[MN_NATION_WAR_RESULT].nField[4].x+100,SMenu[MN_NATION_WAR_RESULT].nField[0].y+30);

	//������ ������
	SMenu[MN_NATION_WAR_RESULT].nField[5].nType = FT_DATA_PUT;
	SMenu[MN_NATION_WAR_RESULT].nField[5].nImageNumber = DP_NATION_WAR_RESULT_BAR;
	SMenu[MN_NATION_WAR_RESULT].nField[5].x = 516;		// BBD 040313
	SMenu[MN_NATION_WAR_RESULT].nField[5].y = 270;
	SMenu[MN_NATION_WAR_RESULT].nField[5].nWillDo = 100;
	SMenu[MN_NATION_WAR_RESULT].nField[5].nSpecialWillDo = 0;
	SMenu[MN_NATION_WAR_RESULT].nField[5].nRectImage = 8;
	::SetRect(SMenu[MN_NATION_WAR_RESULT].nField[5].rCheakBox,SMenu[MN_NATION_WAR_RESULT].nField[0].x,SMenu[MN_NATION_WAR_RESULT].nField[0].y,
		SMenu[MN_NATION_WAR_RESULT].nField[5].x+100,SMenu[MN_NATION_WAR_RESULT].nField[0].y+30);

	//������ - �Ͻ� ������
	SMenu[MN_NATION_WAR_RESULT].nField[6].nType = FT_DATA_PUT;
	SMenu[MN_NATION_WAR_RESULT].nField[6].nImageNumber = DP_NATION_WAR_RESULT_BAR;
	SMenu[MN_NATION_WAR_RESULT].nField[6].x = 516;		// BBD 040313
	SMenu[MN_NATION_WAR_RESULT].nField[6].y = 312;
	SMenu[MN_NATION_WAR_RESULT].nField[6].nWillDo = 100;
	SMenu[MN_NATION_WAR_RESULT].nField[6].nSpecialWillDo = 0;
	SMenu[MN_NATION_WAR_RESULT].nField[6].nRectImage = 12;
	::SetRect(SMenu[MN_NATION_WAR_RESULT].nField[6].rCheakBox,SMenu[MN_NATION_WAR_RESULT].nField[0].x,SMenu[MN_NATION_WAR_RESULT].nField[0].y,
		SMenu[MN_NATION_WAR_RESULT].nField[6].x+100,SMenu[MN_NATION_WAR_RESULT].nField[0].y+30);

	//�¸����� �̸� ( ��濡 ������ ���� )
	SMenu[MN_NATION_WAR_RESULT].nField[7].nType = FT_NOMAL_PUT;
	SMenu[MN_NATION_WAR_RESULT].nField[7].nImageNumber = 1;
	SMenu[MN_NATION_WAR_RESULT].nField[7].nImageType = NATION_WAR_RESULT_IMG;

	//Done
	SMenu[MN_NATION_WAR_RESULT].nField[8].nType = FT_SHOW_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_WAR_RESULT].nField[8].nSpecialWillDo=SWD_NATION_WAR_RESULT;
	SMenu[MN_NATION_WAR_RESULT].nField[8].x = 294;
	SMenu[MN_NATION_WAR_RESULT].nField[8].y = 349;
	SMenu[MN_NATION_WAR_RESULT].nField[8].nRectImage = 6;
	SMenu[MN_NATION_WAR_RESULT].nField[8].nImageNumber = 5;
	SMenu[MN_NATION_WAR_RESULT].nField[8].nImageType = NATION_WAR_RESULT_IMG;
	::SetRect(SMenu[MN_NATION_WAR_RESULT].nField[8].rCheakBox,SMenu[MN_NATION_WAR_RESULT].nField[8].x,SMenu[MN_NATION_WAR_RESULT].nField[8].y,
		SMenu[MN_NATION_WAR_RESULT].nField[8].x+200,SMenu[MN_NATION_WAR_RESULT].nField[8].y+50);

	//<! BBD 040311 ������ ���� �̹���
	SMenu[MN_NATION_WAR_RESULT].nField[9].nType = FT_NOMAL_PUT;
	SMenu[MN_NATION_WAR_RESULT].nField[9].x = 380;
	SMenu[MN_NATION_WAR_RESULT].nField[9].y = 84;
	SMenu[MN_NATION_WAR_RESULT].nField[9].nImageNumber = 19;
	SMenu[MN_NATION_WAR_RESULT].nField[9].nImageType = NATION_WAR_RESULT_IMG;
	//> BBD 040311 ������ ���� �̹���

	}
	//011010 lsw >
	{
	const int iMnIndex	= MN_TUTORIAL_DISPLAY;
	const int iImg		= TUTORIAL_DISPLAY_IMG;
	::SMenuReg(iMnIndex, CENTER_X, 30, TUTORIAL_DISPLAY_IMG,0,2);
	//��ư
	::SMenuFTReg(iMnIndex, 0, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, SWD_SMALLMENU_END, 
		0, 0, 200, 50,TUTORIAL_DISPLAY_TMP_IMG, 2, 0);
	}
	//011010 lsw <
	{//011012 lsw 
	::SMenuReg(MN_ITEM_MAKE_SELECT, CENTER_X, 20+GABY_SCREEN, ITEM_MAKE_RARE_SELECT_IMG,0,20);

	// ok, cancel  Button 
	SMenu[MN_ITEM_MAKE_SELECT].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_ITEM_MAKE_SELECT].nField[0].nSpecialWillDo=SWD_ITEM_MAKE_SELECT_OK;
	SMenu[MN_ITEM_MAKE_SELECT].nField[0].x=26;
	SMenu[MN_ITEM_MAKE_SELECT].nField[0].y=270;
	SMenu[MN_ITEM_MAKE_SELECT].nField[0].nRectImage=294;
	SMenu[MN_ITEM_MAKE_SELECT].nField[0].nImageNumber=295;
	::SetRect(SMenu[MN_ITEM_MAKE_SELECT].nField[0].rCheakBox,26,270,66 ,310);

	SMenu[MN_ITEM_MAKE_SELECT].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_ITEM_MAKE_SELECT].nField[1].nSpecialWillDo=SWD_SKILL_CANCEL;
	SMenu[MN_ITEM_MAKE_SELECT].nField[1].x=180;
	SMenu[MN_ITEM_MAKE_SELECT].nField[1].y=270;
	SMenu[MN_ITEM_MAKE_SELECT].nField[1].nRectImage=292;
	SMenu[MN_ITEM_MAKE_SELECT].nField[1].nImageNumber=293;
	::SetRect(SMenu[MN_ITEM_MAKE_SELECT].nField[1].rCheakBox,180,270,220,310);

	// ������ ȭ�� ���
	//011030 lsw >
	SMenu[MN_ITEM_MAKE_SELECT].nField[2].nType=FT_ITEM_MAKE_SELECT;
	SMenu[MN_ITEM_MAKE_SELECT].nField[2].x=32;
	SMenu[MN_ITEM_MAKE_SELECT].nField[2].y=51;
	SMenu[MN_ITEM_MAKE_SELECT].nField[2].nSHideNomalCount=100;
	SMenu[MN_ITEM_MAKE_SELECT].nField[2].nSHideNomalStart=0;
	SMenu[MN_ITEM_MAKE_SELECT].nField[2].nShideNomalPlus=1;
	::SetRect(SMenu[MN_ITEM_MAKE_SELECT].nField[2].rCheakBox,
		SMenu[MN_ITEM_MAKE_SELECT].nField[2].x,SMenu[MN_ITEM_MAKE_SELECT].nField[2].y
		,SMenu[MN_ITEM_MAKE_SELECT].nField[2].x+210,SMenu[MN_ITEM_MAKE_SELECT].nField[2].y+150);
	//011030 lsw <
	// scroll button
	SMenu[MN_ITEM_MAKE_SELECT].nField[3].nType=FT_HIDE_DSCROLL_AUTO_PUT;
	SMenu[MN_ITEM_MAKE_SELECT].nField[3].nWillDo=2;
	SMenu[MN_ITEM_MAKE_SELECT].nField[3].x=171;
	SMenu[MN_ITEM_MAKE_SELECT].nField[3].y=117;
	SMenu[MN_ITEM_MAKE_SELECT].nField[3].nRectImage=641;
	SMenu[MN_ITEM_MAKE_SELECT].nField[3].nImageNumber=642;
	::SetRect(SMenu[MN_ITEM_MAKE_SELECT].nField[3].rCheakBox,
		SMenu[MN_ITEM_MAKE_SELECT].nField[3].x,
		SMenu[MN_ITEM_MAKE_SELECT].nField[3].y,
		SMenu[MN_ITEM_MAKE_SELECT].nField[3].x+30,
		SMenu[MN_ITEM_MAKE_SELECT].nField[3].y+20);
	
	// down
	SMenu[MN_ITEM_MAKE_SELECT].nField[4].nType=FT_HIDE_ISCROLL_AUTO_PUT;
	SMenu[MN_ITEM_MAKE_SELECT].nField[4].nWillDo=2;
	SMenu[MN_ITEM_MAKE_SELECT].nField[4].x=200;
	SMenu[MN_ITEM_MAKE_SELECT].nField[4].y=117;
	SMenu[MN_ITEM_MAKE_SELECT].nField[4].nRectImage=643;
	SMenu[MN_ITEM_MAKE_SELECT].nField[4].nImageNumber=644;
	::SetRect(SMenu[MN_ITEM_MAKE_SELECT].nField[4].rCheakBox,
		SMenu[MN_ITEM_MAKE_SELECT].nField[4].x,
		SMenu[MN_ITEM_MAKE_SELECT].nField[4].y,
		SMenu[MN_ITEM_MAKE_SELECT].nField[4].x+30,
		SMenu[MN_ITEM_MAKE_SELECT].nField[4].y+20);

	// ���� ����	
	SMenu[MN_ITEM_MAKE_SELECT].nField[5].nType=FT_NOMAL_PUT;
	SMenu[MN_ITEM_MAKE_SELECT].nField[5].x=80;
	SMenu[MN_ITEM_MAKE_SELECT].nField[5].y=6;
	SMenu[MN_ITEM_MAKE_SELECT].nField[5].nImageNumber=640;		//��Ȳ�� ���� �ٲ�

	// ���� ������ ���
	SMenu[MN_ITEM_MAKE_SELECT].nField[6].nType=FT_DATA_PUT;
	SMenu[MN_ITEM_MAKE_SELECT].nField[6].x=30;
	SMenu[MN_ITEM_MAKE_SELECT].nField[6].y=135;
	SMenu[MN_ITEM_MAKE_SELECT].nField[6].nRectImage=155;
	SMenu[MN_ITEM_MAKE_SELECT].nField[6].nImageNumber=DP_SELECT_SELECT;
	
	//���� ��ư
	SMenu[MN_ITEM_MAKE_SELECT].nField[7].nType=			FT_SHOW_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_ITEM_MAKE_SELECT].nField[7].nSpecialWillDo=SWD_ITEM_MAKE_SELECT_RARE;
	SMenu[MN_ITEM_MAKE_SELECT].nField[7].x=160;
	SMenu[MN_ITEM_MAKE_SELECT].nField[7].y=233;
	SMenu[MN_ITEM_MAKE_SELECT].nField[7].nRectImage=5;
	SMenu[MN_ITEM_MAKE_SELECT].nField[7].nImageNumber=5;
	SMenu[MN_ITEM_MAKE_SELECT].nField[7].nImageType = ITEM_MAKE_RARE_SELECT_IMG;
	::SetRect(SMenu[MN_ITEM_MAKE_SELECT].nField[7].rCheakBox,
		SMenu[MN_ITEM_MAKE_SELECT].nField[7].x,SMenu[MN_ITEM_MAKE_SELECT].nField[7].y
		,SMenu[MN_ITEM_MAKE_SELECT].nField[7].x+80,SMenu[MN_ITEM_MAKE_SELECT].nField[7].y+30);

	// scroll button ���� ��� ��ư
	SMenu[MN_ITEM_MAKE_SELECT].nField[8].nType=FT_MAKE_RARE_MORE;
	SMenu[MN_ITEM_MAKE_SELECT].nField[8].nWillDo=1;
	SMenu[MN_ITEM_MAKE_SELECT].nField[8].x=114;
	SMenu[MN_ITEM_MAKE_SELECT].nField[8].y=268;
	SMenu[MN_ITEM_MAKE_SELECT].nField[8].nRectImage=642;
	SMenu[MN_ITEM_MAKE_SELECT].nField[8].nImageNumber=641;
	::SetRect(SMenu[MN_ITEM_MAKE_SELECT].nField[8].rCheakBox,
		SMenu[MN_ITEM_MAKE_SELECT].nField[8].x,	SMenu[MN_ITEM_MAKE_SELECT].nField[8].y,
		SMenu[MN_ITEM_MAKE_SELECT].nField[8].x+30,	SMenu[MN_ITEM_MAKE_SELECT].nField[8].y+16);
	// down
	SMenu[MN_ITEM_MAKE_SELECT].nField[9].nType=FT_MAKE_RARE_MORE;
	SMenu[MN_ITEM_MAKE_SELECT].nField[9].nWillDo=-1;
	SMenu[MN_ITEM_MAKE_SELECT].nField[9].x=142;
	SMenu[MN_ITEM_MAKE_SELECT].nField[9].y=268;
	SMenu[MN_ITEM_MAKE_SELECT].nField[9].nRectImage=644;
	SMenu[MN_ITEM_MAKE_SELECT].nField[9].nImageNumber=643;
	::SetRect(SMenu[MN_ITEM_MAKE_SELECT].nField[9].rCheakBox,
	SMenu[MN_ITEM_MAKE_SELECT].nField[9].x,	SMenu[MN_ITEM_MAKE_SELECT].nField[9].y,
	SMenu[MN_ITEM_MAKE_SELECT].nField[9].x+30,	SMenu[MN_ITEM_MAKE_SELECT].nField[9].y+16);

	
	//���� ���� �� �÷���
	SMenu[MN_ITEM_MAKE_SELECT].nField[10].nType = FT_DATA_PUT;
	SMenu[MN_ITEM_MAKE_SELECT].nField[10].nImageNumber = DP_RECT_STRING_PUT;//011218 lsw
	SMenu[MN_ITEM_MAKE_SELECT].nField[10].x = 90;
	SMenu[MN_ITEM_MAKE_SELECT].nField[10].y = 270;
	SMenu[MN_ITEM_MAKE_SELECT].nField[10].nRectImage = 150;
	::SetRect(SMenu[MN_ITEM_MAKE_SELECT].nField[10].rCheakBox,SMenu[MN_ITEM_MAKE_SELECT].nField[10].x,SMenu[MN_ITEM_MAKE_SELECT].nField[10].y,
	SMenu[MN_ITEM_MAKE_SELECT].nField[10].x+100,SMenu[MN_ITEM_MAKE_SELECT].nField[10].y+30);
	strcpy(SMenu[MN_ITEM_MAKE_SELECT].nField[10].temp,  "0" );

	// Max
	SMenu[MN_ITEM_MAKE_SELECT].nField[11].nType=FT_MAKE_RARE_MORE;
	SMenu[MN_ITEM_MAKE_SELECT].nField[11].nWillDo=0;
	SMenu[MN_ITEM_MAKE_SELECT].nField[11].x=100;
	SMenu[MN_ITEM_MAKE_SELECT].nField[11].y=289;
	SMenu[MN_ITEM_MAKE_SELECT].nField[11].nRectImage=2;
	SMenu[MN_ITEM_MAKE_SELECT].nField[11].nImageNumber=1;
	SMenu[MN_ITEM_MAKE_SELECT].nField[11].nImageType = ITEM_MAKE_RARE_SELECT_IMG;
	::SetRect(SMenu[MN_ITEM_MAKE_SELECT].nField[11].rCheakBox,
	SMenu[MN_ITEM_MAKE_SELECT].nField[11].x,	SMenu[MN_ITEM_MAKE_SELECT].nField[11].y,
	SMenu[MN_ITEM_MAKE_SELECT].nField[11].x+40,	SMenu[MN_ITEM_MAKE_SELECT].nField[11].y+25);


	SMenu[MN_ITEM_MAKE_SELECT].nField[12].nType = FT_DATA_PUT;
	SMenu[MN_ITEM_MAKE_SELECT].nField[12].nImageNumber = DP_RARE_SUCCESS_RATE_DISPLAY_NO_TOTAL;
	SMenu[MN_ITEM_MAKE_SELECT].nField[12].x = 25;
	SMenu[MN_ITEM_MAKE_SELECT].nField[12].y = 240;
	SMenu[MN_ITEM_MAKE_SELECT].nField[12].nRectImage = 150;
	strcpy(SMenu[MN_ITEM_MAKE_SELECT].nField[12].temp,  "3��° ĭ" );
	}
	

	{//020214 lsw
	::SMenuReg(MN_ITEM_MAKE_RARE_SELECT, CENTER_X, 30, ITEM_MAKE_RARE_SELECT_IMG,6,30);
	
	//0���� �ȹٲ�� ���� �϶�// ok, 
	::SMenuFTReg(MN_ITEM_MAKE_RARE_SELECT, 0, FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, 
						SWD_ITEM_MAKE_SELECT_RARE_OK, 26+45, 270, 40, 40, 0, 294, 295,0);
	//cancel
	::SMenuFTReg(MN_ITEM_MAKE_RARE_SELECT, 1, FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, 
						SWD_ITEM_MAKE_SELECT_RARE_CANCEL, 180+45, 270, 40, 40, 0, 292, 293,0);

	::SMenuFTReg(MN_ITEM_MAKE_RARE_SELECT, 2, FT_RARE_GROUP, 0, 25, 60, 100, 22, ITEM_MAKE_RARE_SELECT_IMG, 9, 4,1);
	strcpy(SMenu[MN_ITEM_MAKE_RARE_SELECT].nField[2].temp,  "" );
	
	::SMenuFTReg(MN_ITEM_MAKE_RARE_SELECT, 3, FT_RARE_GROUP, 0, 25, 90, 100, 22, ITEM_MAKE_RARE_SELECT_IMG, 9, 4,2);
	strcpy(SMenu[MN_ITEM_MAKE_RARE_SELECT].nField[3].temp,  "" );

	::SMenuFTReg(MN_ITEM_MAKE_RARE_SELECT, 4, FT_RARE_GROUP, 0, 25, 120, 100, 22, ITEM_MAKE_RARE_SELECT_IMG, 9, 4,3);
	strcpy(SMenu[MN_ITEM_MAKE_RARE_SELECT].nField[4].temp,  "" );

	::SMenuFTReg(MN_ITEM_MAKE_RARE_SELECT, 5, FT_RARE_GROUP, 0, 25, 150, 100, 22, ITEM_MAKE_RARE_SELECT_IMG, 9, 4,4);
	strcpy(SMenu[MN_ITEM_MAKE_RARE_SELECT].nField[5].temp,  "" );

	::SMenuFTReg(MN_ITEM_MAKE_RARE_SELECT, 6, FT_RARE_GROUP, 0, 25, 180, 100, 22, ITEM_MAKE_RARE_SELECT_IMG, 9, 4,5);
	strcpy(SMenu[MN_ITEM_MAKE_RARE_SELECT].nField[6].temp,  "" );

	//��ũ�� ����
	::SMenuFTReg(MN_ITEM_MAKE_RARE_SELECT, 7, FT_RARE_GROUP_SCROLL, 0, 116, 200, 25, 30, ITEM_MAKE_RARE_SELECT_IMG, 7, 7,-1);
	//��ũ�� �Ʒ���
	::SMenuFTReg(MN_ITEM_MAKE_RARE_SELECT, 8, FT_RARE_GROUP_SCROLL, 0, 135, 200, 25, 30, ITEM_MAKE_RARE_SELECT_IMG, 8, 8,1);

	::SMenuFTReg(MN_ITEM_MAKE_RARE_SELECT, 10, FT_DATA_PUT, 0, 25, 243, 25, 30, 0, 150, DP_RARE_SUCCESS_RATE_DISPLAY);
	// 0
	::SMenuFTReg(MN_ITEM_MAKE_RARE_SELECT, 16, FT_RARE_GROUP_MEMBER_DISPLAY, 0, 190, 60, 80, 11, 0, 200, DP_RARE_SUCCESS_RATE_DISPLAY);
	strcpy(SMenu[MN_ITEM_MAKE_RARE_SELECT].nField[16].temp,  "0" );

	::SMenuFTReg(MN_ITEM_MAKE_RARE_SELECT, 17, FT_RARE_GROUP_MEMBER_DISPLAY, 0, 190, 80, 80, 11, 0, 200, DP_RARE_SUCCESS_RATE_DISPLAY);
	strcpy(SMenu[MN_ITEM_MAKE_RARE_SELECT].nField[17].temp,  "1" );

	::SMenuFTReg(MN_ITEM_MAKE_RARE_SELECT, 18, FT_RARE_GROUP_MEMBER_DISPLAY, 0, 190, 60+40, 80, 11, 0, 200, DP_RARE_SUCCESS_RATE_DISPLAY);
	strcpy(SMenu[MN_ITEM_MAKE_RARE_SELECT].nField[18].temp,  "2" );

	::SMenuFTReg(MN_ITEM_MAKE_RARE_SELECT, 19, FT_RARE_GROUP_MEMBER_DISPLAY, 0, 190, 60+60, 80, 11, 0, 200, DP_RARE_SUCCESS_RATE_DISPLAY);
	strcpy(SMenu[MN_ITEM_MAKE_RARE_SELECT].nField[19].temp,  "3" );

	::SMenuFTReg(MN_ITEM_MAKE_RARE_SELECT, 20, FT_RARE_GROUP_MEMBER_DISPLAY, 0, 190, 60+80, 80, 11, 0, 200, DP_RARE_SUCCESS_RATE_DISPLAY);
	strcpy(SMenu[MN_ITEM_MAKE_RARE_SELECT].nField[20].temp,  "4" );

	::SMenuFTReg(MN_ITEM_MAKE_RARE_SELECT, 21, FT_RARE_GROUP_MEMBER_DISPLAY, 0, 190, 60+100, 80, 11, 0, 200, DP_RARE_SUCCESS_RATE_DISPLAY);
	strcpy(SMenu[MN_ITEM_MAKE_RARE_SELECT].nField[21].temp,  "5" );
	
	::SMenuFTReg(MN_ITEM_MAKE_RARE_SELECT, 22, FT_RARE_GROUP_MEMBER_DISPLAY, 0, 190, 60+120, 80, 11, 0, 200, DP_RARE_SUCCESS_RATE_DISPLAY);
	strcpy(SMenu[MN_ITEM_MAKE_RARE_SELECT].nField[22].temp,  "6" );
	
	::SMenuFTReg(MN_ITEM_MAKE_RARE_SELECT, 23, FT_RARE_GROUP_MEMBER_DISPLAY, 0, 190, 60+140, 80, 11, 0, 200, DP_RARE_SUCCESS_RATE_DISPLAY);
	strcpy(SMenu[MN_ITEM_MAKE_RARE_SELECT].nField[23].temp,  "7" );

	}
	{
	::SMenuReg(MN_COMBAT_SKILL_POINT, CENTER_X, 40,COMBAT_SKILL_POINT_IMG,0,30);//iWork �� ���õ� �׷��� ��Ÿ����
	//OK		01// 6�� ¥�� Done
	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 1, FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, 
						SWD_SMALLMENU_END, 118, 360, 83, 35,COMBAT_SKILL_POINT_IMG, 43, 42);
//	//CANCEL	02
//	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 2, FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, 
//						SWD_SMALLMENU_END, 184, 358, 73, 35,COMBAT_SKILL_POINT_IMG, 4, 6);
	//DONE		03//�ΰ� ¥�� OK ��ư
	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 3, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, 
						SWD_CSP_GROUP_SELECT_DONE, 130, 350, 73, 30,COMBAT_SKILL_POINT_IMG,	45, 44);
	//������ų �׷�1 00
	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 4, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, //��ư 4 , 5, 6 �� ������ �ʰ� �Ұ� CalcCombatSkillPointGroup(j-3)
						SWD_CSP_GROUP_ICON, 37, 70, 73, 35,COMBAT_SKILL_POINT_IMG, 15, 15,1);
	//������ų �׷�2 00	124.70
	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 5, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, 
						SWD_CSP_GROUP_ICON, 124, 70, 73, 35,COMBAT_SKILL_POINT_IMG, 17, 18,2);
	//������ų �׷�3 00	211.70
	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 6, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, 
						SWD_CSP_GROUP_ICON, 211, 70, 73, 35,COMBAT_SKILL_POINT_IMG, 19, 20,3);
	//������ų������1 01	46.154
	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 7, FT_CS_ICON, 
						SWD_SC_ICON_POINT, 46, 154, 42, 42, COMBAT_SKILL_ICON_IMG, 15, 16);
	//������ų������2 01	141.135
	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 8, FT_CS_ICON, 
						SWD_SC_ICON_POINT, 242, 154, 42, 42, COMBAT_SKILL_ICON_IMG, 15, 16);
	//������ų������3 01	242.154
	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 9, FT_CS_ICON, 
						SWD_SC_ICON_POINT, 68, 234, 42, 42, COMBAT_SKILL_ICON_IMG, 15, 16);
	//������ų������4 01	215.234
	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 10, FT_CS_ICON, 
						SWD_SC_ICON_POINT, 215, 234, 42, 42, COMBAT_SKILL_ICON_IMG, 15, 16);
	//������ų������5 01	142.305
	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 11, FT_CS_ICON, 
						SWD_SC_ICON_POINT, 142, 305, 42, 42, COMBAT_SKILL_ICON_IMG, 15, 16);
	//������ų������6 01	68.234
	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 12, FT_CS_ICON, 
						SWD_SC_ICON_POINT, 141, 135, 42, 42, COMBAT_SKILL_ICON_IMG, 15, 16);
	//�׷� ���� ��ų1	02	//nWillDo ���� �׷� Ÿ�� 1Ÿ�� 2Ÿ��
	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 13, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, 
						SWD_CSP_GROUP_SELECT, 29, 120, 127, 229,COMBAT_SKILL_POINT_IMG, 34,33,1);
	//�׷� ���� ��ų2	02
	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 14, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, 
						SWD_CSP_GROUP_SELECT, 160, 120, 127, 229,COMBAT_SKILL_POINT_IMG, 35,32,2);
	//����Ʈ ��			01//�ѱ� ������ ���� �ϴ� ��ư�� ����� Rect �� �̿� �Ͽ� 1 �ʵ�� ó��
	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 15, FT_CSP_POINT_UP, 
						10, 250, 310, 38, 34,COMBAT_SKILL_POINT_IMG, 31, 30);
//////////////////////�׷� ���÷���
	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 16, FT_CS_GROUP_EXPLAIN, 
						0, 84-10, 134+3, 42, 42,COMBAT_SKILL_ICON_IMG, 31, 30,1);

	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 17, FT_CS_GROUP_EXPLAIN, //�׷� ���� 
						0, 193+10, 134+3, 42, 42,COMBAT_SKILL_ICON_IMG, 31, 30,2);
///////////////////// ���� ��ų ����â
	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 18, FT_CS_EXPLAIN, //��ų ��ü ����
						0, 40, 200, 90, 20,COMBAT_SKILL_POINT_IMG, 31, 30);
	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 19, FT_CS_EXPLAIN, //��ų ��ü ����
						0, 40, 220+2, 90, 20,COMBAT_SKILL_POINT_IMG, 31, 30);
	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 20, FT_CS_EXPLAIN, //��ų ��ü ����
						0, 40, 240+4, 90, 20,COMBAT_SKILL_POINT_IMG, 31, 30);
	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 21, FT_CS_EXPLAIN, //��ų ��ü ����
						0, 40, 260+6, 90, 20,COMBAT_SKILL_POINT_IMG, 31, 30);
	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 22, FT_CS_EXPLAIN, //��ų ��ü ����
						0, 40, 280+8, 90, 20,COMBAT_SKILL_POINT_IMG, 31, 30);
	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 23, FT_CS_EXPLAIN, //��ų ��ü ����
						0, 40, 300+10, 90, 20,COMBAT_SKILL_POINT_IMG, 31, 30);
///////////////////// ������ ��ų ����â
	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 24, FT_CS_EXPLAIN, //��ų ��ü ����
						0, 169, 200, 90, 20,COMBAT_SKILL_POINT_IMG, 31, 30);
	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 25, FT_CS_EXPLAIN, //��ų ��ü ����
						0, 169, 220+2, 90, 20,COMBAT_SKILL_POINT_IMG, 31, 30);
	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 26, FT_CS_EXPLAIN, //��ų ��ü ����
						0, 169, 240+4, 90, 20,COMBAT_SKILL_POINT_IMG, 31, 30);
	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 27, FT_CS_EXPLAIN, //��ų ��ü ����
						0, 169, 260+6, 90, 20,COMBAT_SKILL_POINT_IMG, 31, 30);
	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 28, FT_CS_EXPLAIN, //��ų ��ü ����
						0, 169, 280+8, 90, 20,COMBAT_SKILL_POINT_IMG, 31, 30);
	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 29, FT_CS_EXPLAIN, //��ų ��ü ����
						0, 169, 300+10, 90, 20,COMBAT_SKILL_POINT_IMG, 31, 30);
	}
	{ 
	SMenuReg(MN_COMBAT_SKILL_DOING, (SCREEN_WIDTH / 2) + 400 - 317, SCREEN_HEIGHT-239, 0, 0, 30);//Todo

	//300 ¥�� �������̽� �̴ϱ�//�����̶� 0 0 �̴�
	//���� ����� ��ų
	::SMenuFTReg(MN_COMBAT_SKILL_DOING, 1, FT_CSD_MAIN_ICON,
						0, 275, 89, 42, 42,COMBAT_SKILL_ICON_IMG, 72, 72);
//������ �� ��� �׸�
//	::SMenuFTReg(MN_COMBAT_SKILL_DOING, 2, FT_NOMAL_PUT, 
//						0, 275, 0, 0, 0,COMBAT_SKILL_ICON_IMG, 85, 85);
	//������ �� CP COUNT
	::SMenuFTReg(MN_COMBAT_SKILL_DOING, 3, FT_CSD_POINT_COUNT_BAR,
						0, 239, 131, 0, 0,COMBAT_SKILL_ICON_IMG, 87, 87);
	//������ �� CP MAX
	::SMenuFTReg(MN_COMBAT_SKILL_DOING, 4, FT_CSD_POINT_LINE_BAR, 
						0, 269, 131, 0, 0,COMBAT_SKILL_ICON_IMG, 84, 84);
	//����1-1 //nWillDo�� ������ų �ε����� ��´�
	::SMenuFTReg(MN_COMBAT_SKILL_DOING, 5, FT_CSD_ICON,
						SWD_SC_ICON_DOING, 225, 0, 0, 0,COMBAT_SKILL_ICON_IMG,0,0);
	//					SWD_SC_ICON_DOING, 0, 0, 42, 42,COMBAT_SKILL_ICON_IMG,0,0);
	//����1-2
	::SMenuFTReg(MN_COMBAT_SKILL_DOING, 6, FT_CSD_ICON,
						SWD_SC_ICON_DOING, 180, 0, 0, 0,COMBAT_SKILL_ICON_IMG,0,0);
	//					SWD_SC_ICON_DOING, 0, 0, 42, 42,COMBAT_SKILL_ICON_IMG,0,0);
	//����1-3
	::SMenuFTReg(MN_COMBAT_SKILL_DOING, 7, FT_CSD_ICON, 
						SWD_SC_ICON_DOING, 135, 0, 0, 0,COMBAT_SKILL_ICON_IMG,0,0);
	//					SWD_SC_ICON_DOING, 0, 0, 42, 42,COMBAT_SKILL_ICON_IMG,0,0);
	//����1-4
	::SMenuFTReg(MN_COMBAT_SKILL_DOING, 8, FT_CSD_ICON, 
						SWD_SC_ICON_DOING, 90, 0, 0, 0,COMBAT_SKILL_ICON_IMG,0,0);
	//					SWD_SC_ICON_DOING, 0, 0, 42, 42,COMBAT_SKILL_ICON_IMG,0,0);
	//����1-5
	::SMenuFTReg(MN_COMBAT_SKILL_DOING, 9, FT_CSD_ICON, 
						SWD_SC_ICON_DOING, 45, 0, 0, 0,COMBAT_SKILL_ICON_IMG,0,0);
	//					SWD_SC_ICON_DOING, 0, 0, 42, 42,COMBAT_SKILL_ICON_IMG,0,0);
	//����1-6
	::SMenuFTReg(MN_COMBAT_SKILL_DOING, 10, FT_CSD_ICON, 
						SWD_SC_ICON_DOING, 0, 0, 0, 0,COMBAT_SKILL_ICON_IMG,0,0);
	//					SWD_SC_ICON_DOING, 0, 0, 42, 42,COMBAT_SKILL_ICON_IMG,0,0);
	//����2-1
	::SMenuFTReg(MN_COMBAT_SKILL_DOING, 11, FT_CSD_ICON, 
						SWD_SC_ICON_DOING, 225, 45, 0, 0,COMBAT_SKILL_ICON_IMG,0,0);
	//					SWD_SC_ICON_DOING, 0, 0, 42, 42,COMBAT_SKILL_ICON_IMG,0,0);
	//����2-2
	::SMenuFTReg(MN_COMBAT_SKILL_DOING, 12, FT_CSD_ICON, 
						SWD_SC_ICON_DOING, 180, 45, 0, 0,COMBAT_SKILL_ICON_IMG,0,0);
	//					SWD_SC_ICON_DOING, 0, 0, 42, 42,COMBAT_SKILL_ICON_IMG,0,0);
	//����2-3
	::SMenuFTReg(MN_COMBAT_SKILL_DOING, 13, FT_CSD_ICON, 
						SWD_SC_ICON_DOING, 135, 45, 0, 0,COMBAT_SKILL_ICON_IMG,0,0);
	//					SWD_SC_ICON_DOING, 0, 0, 42, 42,COMBAT_SKILL_ICON_IMG,0,0);
	//����2-4
	::SMenuFTReg(MN_COMBAT_SKILL_DOING, 14, FT_CSD_ICON, 
						SWD_SC_ICON_DOING, 90, 45, 0, 0,COMBAT_SKILL_ICON_IMG,0,0);
	//					SWD_SC_ICON_DOING, 0, 0, 42, 42,COMBAT_SKILL_ICON_IMG,0,0);						
	//����2-5
	::SMenuFTReg(MN_COMBAT_SKILL_DOING, 15, FT_CSD_ICON, 
						SWD_SC_ICON_DOING, 45, 45, 0, 0,COMBAT_SKILL_ICON_IMG,0,0);
	//					SWD_SC_ICON_DOING, 0, 0, 42, 42,COMBAT_SKILL_ICON_IMG,0,0);
	//����2-6
	::SMenuFTReg(MN_COMBAT_SKILL_DOING, 16, FT_CSD_ICON, 
						SWD_SC_ICON_DOING, 0, 45, 0, 0,COMBAT_SKILL_ICON_IMG,0,0);
	//					SWD_SC_ICON_DOING, 0, 0, 42, 42,COMBAT_SKILL_ICON_IMG,0,0);
	//����3-1
	::SMenuFTReg(MN_COMBAT_SKILL_DOING, 17, FT_CSD_ICON, 
						SWD_SC_ICON_DOING, 225, 90, 0, 0,COMBAT_SKILL_ICON_IMG,0,0);
	//					SWD_SC_ICON_DOING, 0, 0, 42, 42,COMBAT_SKILL_ICON_IMG,0,0);
	//����3-2
	::SMenuFTReg(MN_COMBAT_SKILL_DOING, 18, FT_CSD_ICON, 
						SWD_SC_ICON_DOING, 180, 90, 0, 0,COMBAT_SKILL_ICON_IMG,0,0);
	//					SWD_SC_ICON_DOING, 0, 0, 42, 42,COMBAT_SKILL_ICON_IMG,0,0);
	//����3-3
	::SMenuFTReg(MN_COMBAT_SKILL_DOING, 19, FT_CSD_ICON, 
						SWD_SC_ICON_DOING, 135, 90, 0, 0,COMBAT_SKILL_ICON_IMG,0,0);
	//					SWD_SC_ICON_DOING, 0, 0, 42, 42,COMBAT_SKILL_ICON_IMG,0,0);
	//����3-4
	::SMenuFTReg(MN_COMBAT_SKILL_DOING, 20, FT_CSD_ICON, 
						SWD_SC_ICON_DOING, 90, 90, 0, 0,COMBAT_SKILL_ICON_IMG,0,0);
	//					SWD_SC_ICON_DOING, 0, 0, 42, 42,COMBAT_SKILL_ICON_IMG,0,0);
	//����3-5
	::SMenuFTReg(MN_COMBAT_SKILL_DOING, 21, FT_CSD_ICON, 
						SWD_SC_ICON_DOING, 45, 90, 0, 0,COMBAT_SKILL_ICON_IMG,0,0);
	//					SWD_SC_ICON_DOING, 0, 0, 42, 42,COMBAT_SKILL_ICON_IMG,0,0);
	//����3-6
	::SMenuFTReg(MN_COMBAT_SKILL_DOING, 22, FT_CSD_ICON, 
						SWD_SC_ICON_DOING, 0, 90, 0, 0,COMBAT_SKILL_ICON_IMG,0,0);
	//					SWD_SC_ICON_DOING, 0, 0, 42, 42,COMBAT_SKILL_ICON_IMG,0,0);
	}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	�ð� ǥ�� �޴�
//010910 lsw
	{
//	::SMenuReg(MN_TIME_DISPLAY_MENU, 600, 452, MAININTERFACE_IMG,79,8);	//300 ¥�� �������̽� �̴ϱ�//�����̶� 0 0 �̴�
	::SMenuReg(MN_TIME_DISPLAY_MENU, (SCREEN_WIDTH-219)/2-10, 0, 0,0,8);	//300 ¥�� �������̽� �̴ϱ�//�����̶� 0 0 �̴�
//������		//�̸�
//������ų		//����  ��ǥ
	// ����
	::SMenuFTReg(MN_TIME_DISPLAY_MENU,0,FT_ALPHA_PUT,0,0, 0,0, 0, MAININTERFACE_IMG, 79, 79);
	// ���� �� ��ǥ ���
	::SMenuFTReg(MN_TIME_DISPLAY_MENU,1,FT_XY_DISPLAY		, 0, 137, 5, 0, 0, 0, 0,0,0);
	::SMenuFTReg(MN_TIME_DISPLAY_MENU,2,FT_NATION_DISPLAY	, 0, 30, 5, 0, 0, 0, 0,0);
	}
	{//��������
		// ���, ��ü���� �޴�
		::SMenuReg(MN_MERCHANT_MAIN, CENTER_X, 75, MERCHANT_IMG,1,6);	//300 ¥�� �������̽� �̴ϱ�//�����̶� 0 0 �̴�
		// Exit Button 
		::SMenuFTReg(MN_MERCHANT_MAIN,0,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SMALLMENU_END, 
			49, 235, 158-49, 264-235, MERCHANT_IMG, 15, 16);
		// ���� �޴� �θ���
		::SMenuFTReg(MN_MERCHANT_MAIN,1,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_CALLMENU,
			31, 54, 178-31, 87-54, MERCHANT_IMG, 3, 4,MN_MERCHANT_BBS);// ���� �޴�
//<! BBD 040401	acer���� ��û�� ���� �븸�α� �ְʹ��� ������ ���ϰ� �Ѵ�.
#ifdef TAIWAN_LOCALIZING_
		//�ŷ���ǰ ����
		::SMenuFTReg(MN_MERCHANT_MAIN,2,FT_NONE, SWD_CALLMENU,
			31, 98, 178-31, 131-98, MERCHANT_IMG, 6, 7,MN_MERCHANT_FIND);//�˻��޴��� ����
		//�ŷ���ǰ ���
		::SMenuFTReg(MN_MERCHANT_MAIN,3,FT_NONE, SWD_SEND_GET_MERCHANT_SELL_LIST,
			31, 142, 178-31, 175-142, MERCHANT_IMG, 9, 10,MN_MERCHANT_SELL_INFO);//�ڽ��� �Ĵ� ��ǰ�� ������ �޾ƿ����� �Ѵ�
		//�ŷ���ǰ ã��
		::SMenuFTReg(MN_MERCHANT_MAIN,4,FT_NONE, SWD_SEND_GET_MERCHANT_RESULT_LIST,
			31, 186, 178-31, 219-186, MERCHANT_IMG, 12, 13,MN_MERCHANT_RESULT_TAKE);//������� ������ �Ѵ�
#else
		//�ŷ���ǰ ����
		::SMenuFTReg(MN_MERCHANT_MAIN,2,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_CALLMENU,
			31, 98, 178-31, 131-98, MERCHANT_IMG, 6, 7,MN_MERCHANT_FIND);//�˻��޴��� ����
		//�ŷ���ǰ ���
		::SMenuFTReg(MN_MERCHANT_MAIN,3,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SEND_GET_MERCHANT_SELL_LIST,
			31, 142, 178-31, 175-142, MERCHANT_IMG, 9, 10,MN_MERCHANT_SELL_INFO);//�ڽ��� �Ĵ� ��ǰ�� ������ �޾ƿ����� �Ѵ�
		//�ŷ���ǰ ã��
		::SMenuFTReg(MN_MERCHANT_MAIN,4,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SEND_GET_MERCHANT_RESULT_LIST,
			31, 186, 178-31, 219-186, MERCHANT_IMG, 12, 13,MN_MERCHANT_RESULT_TAKE);//������� ������ �Ѵ�
#endif		
//> BBD 040401	acer���� ��û�� ���� �븸�α� �ְʹ��� ������ ���ϰ� �Ѵ�.
	}
	///////////////////////////////////////////////////////////////////////////////////////////
	{// ���� �޴�
	int gab_x = 7;
	const int iMnIndex	= MN_MERCHANT_BBS;
	const int iImg		= MERCHANT_IMG;

	::SMenuReg(iMnIndex, CENTER_X, 75, iImg,0,10);	//300 ¥�� �������̽� �̴ϱ�//�����̶� 0 0 �̴�

	::SMenuFTReg(iMnIndex,0,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_MN_MERCHANT_BBS_OK, // ���� ������
		20, 317, 110, 30, iImg, 18, 19,0);
	
	::SMenuFTReg(iMnIndex,1,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SUBMENU_BACK, // cancel
		148, 317, 110, 30, iImg, 21, 22,0);
	
//	SMenu[iMnIndex].nField[2].nValue=&SMenu[iMnIndex].key;
//	::SMenuFTReg(iMnIndex,2,FT_CHECK_COMBO, SWD_SUBMENU_BACK, // Check item
//		26+gab_x, 53+gab_x, 116, 22, 0, 0, 0,0);
	
	SMenu[iMnIndex].key =1;
//	SMenu[iMnIndex].nField[3].nValue=&SMenu[iMnIndex].key;
//	::SMenuFTReg(iMnIndex,3,FT_CHECK_COMBO, SWD_SUBMENU_BACK, // Check bbs
//		26+gab_x, 142+gab_x, 116, 22, 0, 0, 0,1);
	
	SMenu[iMnIndex].nField[4].nValue=&SMenu[iMnIndex].work;
	::SMenuFTReg(iMnIndex,4,FT_CHECK_COMBO, SWD_SUBMENU_BACK, // Check 1 map
		67+40+gab_x, 237-4+gab_x, 116, 22, 0, 0, 0,0);
	
	SMenu[iMnIndex].nField[5].nValue=&SMenu[iMnIndex].work;
	::SMenuFTReg(iMnIndex,5,FT_CHECK_COMBO, SWD_SUBMENU_BACK, // Check all map
		135+41+gab_x, 237-4+gab_x, 116, 22, 0, 0, 0,1);

	::SMenuFTReg(iMnIndex,6,FT_VIEW_BBS_MONEY, 0, // view money	// ������ ����	// key// ���� �������� �밡
		77, 273, 0, 0, 0, 77, 0,0);

	::SMenuFTReg(iMnIndex,7,FT_WRITE_BBS, 0, // ���� ���� ���
		45, 90, 130, 50, 0, 182, 3,0);//nImageNumber= 3;// �ִ� �ټ�

//	::SMenuFTReg(iMnIndex,8,FT_WRITE_BBS_ITEM_NAME, 0, // ������ ���� ���	// �̸�
//		40, 83, 170, 20, 0, 175, 3,0);//nImageNumber= 3;// �ִ� �ټ�

//	::SMenuFTReg(iMnIndex,9,FT_WRITE_BBS_ITEM_MONEY, 0, // ������ ���� ���	// ����
//		40, 113, 150, 20, 0, 140, 3,0);//nImageNumber= 3;// �ִ� �ټ�
	}

//<soto-����
	{
	//�ŷ���ǰ ����(FIND) ���������� 1��° ��ư
		const int iMnIndex	= MN_MERCHANT_FIND;
		const int iImg		= MERCHANT_FINDER_IMG;
		int iFtNum = 0;
		::SMenuReg(iMnIndex, CENTER_X, 75, iImg,0,26);	//300 ¥�� �������̽� �̴ϱ�//�����̶� 0 0 �̴�
		
		
		// Ok Button
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SEND_GET_MERCHANT_BUY_LIST,//�̰� ���� �� ���� �Ŀ� Send �� ������
			56, 252, 110, 30, iImg, 1, 3);
		// Cancel Button
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_CALLMENU,
			313, 252, 110, 30, iImg, 4, 6,MN_MERCHANT_MAIN);
		
		//�����
		strcpy(SMenu[iMnIndex].nField[iFtNum].temp,lan->OutputMessage(2,750));//�ӽ�
		::SMenuFTReg(iMnIndex,iFtNum++,FT_DATA_PUT,0,
			270,70, 200, 220, 16, 180, DP_RECT_STRING_PUT_CUSTOM);// ����

		//Ű���� �Է�
		::SMenuFTReg(iMnIndex,iFtNum++,FT_WRITE_MERCHANT, SWD_NONE,
			86,61,130,40, iImg, 180, 0,1);

		//�����̸� �Է�
		::SMenuFTReg(iMnIndex,iFtNum++,FT_WRITE_MERCHANT, SWD_NONE,
			86,61 + 64,130,40, iImg, 180, 0,2);

		//�Ⱓ �ؽ�Ʈ ���
		::SMenuFTReg(iMnIndex,iFtNum++,FT_DATA_PUT, 0,
			86,61 + 128,130,40, iImg, 0, DP_AUCTION_DATE);

		//�Ⱓ ���� ��ư.
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_AUCTION_UPDOWN_BTN,
			226,184,  30, 30, iImg, 8, 9,0);

		//�Ⱓ �Ʒ��� ��ư
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_AUCTION_UPDOWN_BTN,
			226,211, 30, 30, iImg, 10, 11,1);

		//Ű���� �Է�üũ.
		::SMenuFTReg(iMnIndex,iFtNum++,FT_CHECK_MERCHANT_FIND,0,
			222,64,34,34,iImg,7,7,0);
		//�����̸� �Է� üũ.
		::SMenuFTReg(iMnIndex,iFtNum++,FT_CHECK_MERCHANT_FIND,1,
			222,64 + 65,34,34,iImg,7,7,0);		
/*		
		::SMenuFTReg(iMnIndex,iFtNum++,FT_DATA_PUT, 0,
			90,65+10,  98, 32, iImg, 0, DP_AUCTION_FK_RARE);
		::SMenuFTReg(iMnIndex,iFtNum++,FT_DATA_PUT, 0,
			90,129+10, 98, 32, iImg, 0, DP_AUCTION_FK_LV);
		::SMenuFTReg(iMnIndex,iFtNum++,FT_DATA_PUT, 0,
			90,194+10, 98, 32, iImg, 0, DP_AUCTION_FK_TACTIC);
		::SMenuFTReg(iMnIndex,iFtNum++,FT_DATA_PUT, 0,
			90,258+10, 98, 32, iImg, 0, DP_AUCTION_FK_WEAR);
		
		//�˻��� ����Ʈ//nWillDo �� �˻��� ���� 1.Ŭ���� 2.�Ӽ�(����) 3.���� 4.��ƽ 5.������
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_AUCTION_FIND_KEY_RARE_SET,
			188,66,  30, 30, iImg, 8, 9,0);
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_AUCTION_FIND_KEY_RARE_SET,
			221,66, 30, 30, iImg, 11, 12,1);

		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_AUCTION_FIND_KEY_LV_SET,
			188,130, 30, 30, iImg, 8, 9,0);
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_AUCTION_FIND_KEY_LV_SET,
			221,130, 30, 30, iImg, 11, 12,1);

		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_AUCTION_FIND_KEY_TACTIC_SET,
			188,195, 30, 30, iImg, 8, 9,0);
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_AUCTION_FIND_KEY_TACTIC_SET,
			221,195, 30, 30, iImg, 11, 12,1);

		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_AUCTION_FIND_KEY_WEAR_SET,
			188,259, 30, 30, iImg, 8, 9,0);
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_AUCTION_FIND_KEY_WEAR_SET,
			221,259, 30, 30, iImg, 11, 12,1);
*/
	}
//>soto-����

	{//�����ڰ� ���� ���� �Ǹ����� ��ǰ ����Ʈ
		const int iMnIndex	= MN_MERCHANT_BUY;
		const int iImg		= MERCHANT_BUY_IMG;
		int iFtNum = 0;
		::SMenuReg(iMnIndex, CENTER_X, 50, iImg,0,15);//�ʵ�� 10���� �ʿ��մϴ�.
		//���,
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SMALLMENU_END, //030127 lsw
			416, 364, 150, 30, iImg, 5, 6,MN_MERCHANT_MAIN);
		//����,
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SEND_MERCHANT_BUY_THIS_ITEM,//�̰� ���� �� ���� �Ŀ� Send �� ������
			94, 364, 150, 30, iImg, 2, 3);
		//��ũ�� ��
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SEND_MERCHANT_BUY_LIST_SCROLL_UP, 
			624, 95, 25, 40, iImg, 8, 9,MN_MERCHANT_MAIN);
		//��ũ�� �ٿ�
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SEND_MERCHANT_BUY_LIST_SCROLL_DOWN, 
			624, 270, 25, 40, iImg, 11, 12,MN_MERCHANT_MAIN);
		//üũ�ڽ�1 (�����۵� ����ϴ�)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_BUY_ITEM_CHECK, 0, 
			15, 82, 20, 20, iImg, 14, 14,0);
		//üũ�ڽ�2 (�����۵� ����ϴ�)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_BUY_ITEM_CHECK, 1, 
			15, 128, 20, 20, iImg, 14, 14,0);
		//üũ�ڽ�3 (�����۵� ����ϴ�)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_BUY_ITEM_CHECK, 2, 
			15, 174, 20, 20, iImg, 14, 14,0);
		//üũ�ڽ�4 (�����۵� ����ϴ�)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_BUY_ITEM_CHECK, 3, 
			15, 220, 20, 20, iImg, 14, 14,0);
		//üũ�ڽ�5 (�����۵� ����ϴ�)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_BUY_ITEM_CHECK, 4, 
			15, 266, 20, 20, iImg, 14, 14,0);
		//üũ�ڽ�6 (�����۵� ����ϴ�)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_BUY_ITEM_CHECK, 5, 
			15, 312, 20, 20, iImg, 14, 14,0);
	}
	{//�ŷ���ǰ���(���θ� ���� �޴�)//�ǸŹ�ǰ ��ü ����
		const int iMnIndex	= MN_MERCHANT_SELL_INFO;
		const int iImg		= MERCHANT_SELL_INFO_IMG;
		int iFtNum = 0;
		::SMenuReg(iMnIndex, CENTER_X, 75, iImg,0,15);//�ʵ�� 10���� �ʿ��մϴ�.
		//���,
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_CALLMENU, 
			367, 364, 110, 30, iImg, 8, 9,MN_MERCHANT_MAIN);
		//������,
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SEND_MERCHANT_SELL_ITEM_DELETE,//�̰� ���� �� ���� �Ŀ� Send �� ������
			55, 364, 110, 30, iImg, 2, 3);
		//�����۵��,
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_CALL_MERCHANT_REGISTER,//�̰� ���� �� ���� �Ŀ� Send �� ������
			211, 364, 110, 30, iImg, 5, 6);
		//��ũ�� ��
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SEND_MERCHANT_SELL_LIST_SCROLL_UP,
			494, 95, 25, 40, iImg, 11, 12,MN_MERCHANT_MAIN);
		//��ũ�� �ٿ�
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SEND_MERCHANT_SELL_LIST_SCROLL_DOWN,
			494, 270, 25, 40, iImg, 14, 15,MN_MERCHANT_MAIN);
		////key�� �ε��� work �� ������ ��ȣ
		//üũ�ڽ�1 (�����۵� ����ϴ�)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_SELL_ITEM_CHECK, 0, 
			15, 82, 20, 20, iImg, 17, 16,0);
		//üũ�ڽ�2 (�����۵� ����ϴ�)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_SELL_ITEM_CHECK, 1, 
			15, 128, 20, 20, iImg, 17, 16,0);
		//üũ�ڽ�3 (�����۵� ����ϴ�)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_SELL_ITEM_CHECK, 2, 
			15, 174, 20, 20, iImg, 17, 16,0);
		//üũ�ڽ�4 (�����۵� ����ϴ�)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_SELL_ITEM_CHECK, 3, 
			15, 220, 20, 20, iImg, 17, 16,0);
		//üũ�ڽ�5 (�����۵� ����ϴ�)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_SELL_ITEM_CHECK, 4, 
			15, 266, 20, 20, iImg, 17, 16,0);
		//üũ�ڽ�6 (�����۵� ����ϴ�)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_SELL_ITEM_CHECK, 5, 
			15, 312, 20, 20, iImg, 17, 16,0);
	}
	{//�Ǹ����� ������ ���
		const int iMnIndex	= MN_MERCHANT_REGISTER;
		const int iImg		= MERCHANT_SELL_REGISTER_IMG;
		int iFtNum = 0;
		::SMenuReg(iMnIndex, 50, 100, iImg,0,15);//�ʵ�� 10���� �ʿ��մϴ�.
		//�ݱ�
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_CALLSMALLMENU, 
			263,239,110,30, iImg, 5, 6,MN_MERCHANT_SELL_INFO);
		//�Ź�ǰ���(OK)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SEND_MERCHANT_SELL_ITEM_REGISTER, 
			45,239,110,30, iImg, 2, 3,MN_MERCHANT_MAIN);
		//������ �����
		::SMenuFTReg(iMnIndex,iFtNum,FT_MERCHANT_SELL_ITEM_DROP_ZONE, 0, 
			93+12,99+15,0,0, iImg, 0, 0,MN_MERCHANT_MAIN);
		::SetRect(SMenu[iMnIndex].nField[iFtNum++].rCheakBox,35,78,35+153,78+80);//������ �ڿ� ����
		
		//�޺��ڽ�(�Ʒ��� ���� ȭ��ǥ)(���Է� ���� �� �ְ� �Ұ�
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_MERCHANT_SELL_TYPE_SET, 
			153,183, 30,30, iImg, 8, 9,1);			
		//�޺��ڽ�(�Ʒ��� ���� ȭ��ǥ)(���Է� ���� �� �ְ� �Ұ�
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_MERCHANT_SELL_TYPE_SET, 
			184,183, 30,30, iImg, 12, 13,1);			
		//������ ����
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_SELL_ITEM_EXPLAIN, 0, 
			217,65,182,101, iImg, 14, 15,MN_MERCHANT_MAIN);
		//���� �Է�
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_SELL_ITEM_VALUE_INPUT, 0, 
			217,185,182,101, iImg, 14, 15,MN_MERCHANT_MAIN);
	}
	{//�ŷ������ ã��(TAKE)
		const int iMnIndex	= MN_MERCHANT_RESULT_TAKE;
		const int iImg		= MERCHANT_RESULT_TAKE_IMG;
		int iFtNum = 0;
		::SMenuReg(iMnIndex, CENTER_X, 50, iImg,0,15);//�ʵ�� 10���� �ʿ��մϴ�.
		//�ݱ�
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_CALLSMALLMENU, 
			227,364,110,30, iImg, 2, 3,MN_MERCHANT_MAIN);

		//����� ã��
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SEND_MERCHANT_RESULT_ITEM_TAKE, 
			42,364,110,30, iImg, 13, 14,MN_MERCHANT_MAIN);

		//��ũ�� ��
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SEND_MERCHANT_RESULT_LIST_SCROLL_UP, 
			341,95,26,43, iImg, 5, 6);

		//��ũ�� �ٿ�
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SEND_MERCHANT_RESULT_LIST_SCROLL_DOWN, 
			341,270,26,43, iImg, 8, 9);	

		//üũ�ڽ�1 (�����۵� ����ϴ�)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_RESULT_ITEM_CHECK, 0, 
			15, 82, 20, 20, iImg, 11, 11,0);
		//üũ�ڽ�2 (�����۵� ����ϴ�)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_RESULT_ITEM_CHECK, 1, 
			15, 128, 20, 20, iImg, 11, 11,0);
		//üũ�ڽ�3 (�����۵� ����ϴ�)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_RESULT_ITEM_CHECK, 2, 
			15, 174, 20, 20, iImg, 11, 11,0);
		//üũ�ڽ�4 (�����۵� ����ϴ�)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_RESULT_ITEM_CHECK, 3, 
			15, 220, 20, 20, iImg, 11, 11,0);
		//üũ�ڽ�5 (�����۵� ����ϴ�)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_RESULT_ITEM_CHECK, 4, 
			15, 266, 20, 20, iImg, 11, 11,0);
		//üũ�ڽ�6 (�����۵� ����ϴ�)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_RESULT_ITEM_CHECK, 5, 
			15, 312, 20, 20, iImg, 11, 11,0);
	}
	{//�����ŷ� �ϱ�
		const int iMnIndex	= MN_MERCHANT_DIRECT_EXCHAGE_LIST;
		const int iImg		= MERCHANT_DIRECT_EXCHANGE_IMG;
		int iFtNum = 0;
		::SMenuReg(iMnIndex, CENTER_X, 50, iImg,0,15);//�ʵ�� 10���� �ʿ��մϴ�.
		//�ݱ�
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SMALLMENU_END, 
			313,364,152,30, iImg, 5, 6);
		//�����۱���
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SEND_MERCHANT_BUY_THIS_ITEM, 
			67,364,152,30, iImg, 2, 3,MN_MERCHANT_MAIN);
		//��ũ�� ��
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SEND_MERCHANT_DIRECT_LIST_SCROLL_UP, 
			494,95, 26,43, iImg, 8, 9);
		//��ũ�� �ٿ�
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SEND_MERCHANT_DIRECT_LIST_SCROLL_DOWN, 
			494,270, 26,43, iImg, 12, 13);	
		//üũ�ڽ�1 (�����۵� ����ϴ�)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_DIRECT_BUY_ITEM_CHECK, 0, 
			15, 82, 20, 20, iImg, 14, 14,0);
		//üũ�ڽ�2 (�����۵� ����ϴ�)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_DIRECT_BUY_ITEM_CHECK, 1, 
			15, 128, 20, 20, iImg, 14, 14,0);
		//üũ�ڽ�3 (�����۵� ����ϴ�)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_DIRECT_BUY_ITEM_CHECK, 2, 
			15, 174, 20, 20, iImg, 14, 14,0);
		//üũ�ڽ�4 (�����۵� ����ϴ�)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_DIRECT_BUY_ITEM_CHECK, 3, 
			15, 220, 20, 20, iImg, 14, 14,0);
		//üũ�ڽ�5 (�����۵� ����ϴ�)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_DIRECT_BUY_ITEM_CHECK, 4, 
			15, 266, 20, 20, iImg, 14, 14,0);
		//üũ�ڽ�6 (�����۵� ����ϴ�)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_DIRECT_BUY_ITEM_CHECK, 5, 
			15, 312, 20, 20, iImg, 14, 14,0);
	}
//<! BBD 040303		// Auction ��ϵ� ����ã�� �޴�
	{
		const int iMnIndex	= MN_MERCHANT_BACKUP_MAIN;
		const int iImg		= MERCHANT_RESULT_TAKE_IMG;
		int iFtNum = 0;
		::SMenuReg(iMnIndex, CENTER_X, 50, iImg,0,15);//�ʵ�� 10���� �ʿ��մϴ�.

		// ��Ŷ ������ --> ��Ŷ ���� �� ���� �����Ѵ�.
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_BACKUP_GET_LIST, SWD_NONE, 
			100,364,110,30, iImg, 2, 3, iMnIndex);

		//�ݱ�
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SMALLMENU_END, 
			227,364,110,30, iImg, 2, 3);

		//����� ã��
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SEND_MERCHANT_BACKUP_ITEM_TAKE, 
			42,364,110,30, iImg, 13, 14,MN_MERCHANT_MAIN);

		//��ũ�� ��
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SEND_MERCHANT_BACKUP_LIST_SCROLL_UP, 
			341,95,26,43, iImg, 5, 6);

		//��ũ�� �ٿ�
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SEND_MERCHANT_BACKUP_LIST_SCROLL_DOWN, 
			341,270,26,43, iImg, 8, 9);	

		//üũ�ڽ�1 (�����۵� ����ϴ�)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_RESULT_ITEM_CHECK, 0, 
			15, 82, 20, 20, iImg, 11, 11,0);
		//üũ�ڽ�2 (�����۵� ����ϴ�)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_RESULT_ITEM_CHECK, 1, 
			15, 128, 20, 20, iImg, 11, 11,0);
		//üũ�ڽ�3 (�����۵� ����ϴ�)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_RESULT_ITEM_CHECK, 2, 
			15, 174, 20, 20, iImg, 11, 11,0);
		//üũ�ڽ�4 (�����۵� ����ϴ�)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_RESULT_ITEM_CHECK, 3, 
			15, 220, 20, 20, iImg, 11, 11,0);
		//üũ�ڽ�5 (�����۵� ����ϴ�)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_RESULT_ITEM_CHECK, 4, 
			15, 266, 20, 20, iImg, 11, 11,0);
		//üũ�ڽ�6 (�����۵� ����ϴ�)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_RESULT_ITEM_CHECK, 5, 
			15, 312, 20, 20, iImg, 11, 11,0);
	}
//> BBD 040303		// Auction ��ϵ� ����ã�� �޴�
	{//	�������� ������ ��ȯâ
		//�ʵ� �ѹ� �ȹٲ�� ���� �Ͻÿ�!
		//LEFT RIGHT �� �ݵ�� ��ġ ������ �ʽ��ϴ�. 
		//���� �ø� �ڸ��� �ݵ�� ������ ���̵� �Դϴ�.
		//�������ӿ��� �ұ��ϰ� LEFT�� ��� �� �͵� �ֽ��ϴ�.
		const int iMnIndex	= MN_EXCHANGE_ITEM;
		const int iImg		= EXCHANGE_IMG;
		int iFtNum = 0;
		::SMenuReg(iMnIndex, 30, 70, iImg,0,6);//�ʵ�� 10���� �ʿ��մϴ�.
		// text ���
		::SMenuFTReg(iMnIndex,0,FT_EXCHANGE_ITEM_ETC, SWD_CALLMENU, 
			0,54, 164,34, iImg, 0, 0);//0
		// �����۰� check ǥ��
		::SMenuFTReg(iMnIndex,1,FT_EXCHANGE_ITEM, 0,
			236,99,174,139, iImg, 6, 6,RIGHT);//1	//�����ʿ� �ֽ��ϴ�//���� �ִ���
		
		::SMenuFTReg(iMnIndex,2,FT_EXCHANGE_ITEM, 0, 
			26,99,174,139, iImg, 5, 5,LEFT);//2	//���ʿ� �ֽ��ϴ�//���� �޴���
			
		::SMenuFTReg(iMnIndex,3,FT_EXCHANGE_ITEM_CHECK,0, //3 �ʵ� �ѹ� �Ȳ��̰� ����
			17,258,25,25, iImg, 4, 4,LEFT);// üũ�ڽ�//������ Ŭ���ϴ� ���̵�
			
		::SMenuFTReg(iMnIndex,4,FT_EXCHANGE_ITEM_CHECK,0, //4 �ʵ� �ѹ� �Ȳ��̰� ����
			401,258,25,25, iImg, 4,4, RIGHT);// üũ�ڽ�//���� Ŭ�� �ϴ� ���̵�
		//Cancel
		::SMenuFTReg(iMnIndex,5,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_ITEM_EXCHANGE_CANCEL, 
			166,296,110,30, iImg, 2, 3);
	}
	{// 1:1 ���� �޴�
		const int iMnIndex	= MN_MAN_TO_MAN;
		const int iImg		= MAN_TO_MAN_IMG;
		int iFtNum = 0;

		::SMenuReg(iMnIndex, 50, 50, iImg,0,7);//�ʵ�� 10���� �ʿ��մϴ�.
		//�̸� ���
		::SMenuFTReg(iMnIndex,VS_LEFT_DISPLAY_FIELD_NO,FT_MAN_TO_MAN_INFO_DISPLAY, 0, 
			29,102, 174,139, iImg, 0, 0,LEFT);//0
		//�̸� ���
		::SMenuFTReg(iMnIndex,VS_RIGHT_DISPLAY_FIELD_NO,FT_MAN_TO_MAN_INFO_DISPLAY, 0, 
			239,102, 174,139, iImg, 0, 0,RIGHT);//0
		//üũ ��ư//����
		::SMenuFTReg(iMnIndex,3,FT_MAN_TO_MAN_CHECK, 1, 
			49,260, 0,0, iImg, 4, 4,LEFT);//0
		//üũ ��ư
		::SMenuFTReg(iMnIndex,4,FT_MAN_TO_MAN_CHECK, 2, 
			375,260, 20,20, iImg, 4, 4,RIGHT);//0`
		// cancel
		::SMenuFTReg(iMnIndex,5,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_MAN_TO_MAN_CANCEL, 
			166,296, 110,30, iImg, 2, 3);//0
	}
	{// �������� ����
		const int iMnIndex	= MN_GUILD_ADMIN;
		const int iImg		= GUILD_ADMIN;
		int iFtNum = 0;
		::SMenuReg(iMnIndex, CENTER_X, 75, iImg,0,4);//�ʵ�� 10���� �ʿ��մϴ�.
		// exit	
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SMALLMENU_END, 
			49,235, 110,30, iImg, 11, 12);
		//������
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_CALLMENU, 
			23,63,164,34, iImg, 2, 3,MN_GUILD_MAIN);
		// grand master // gm ���
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_CALLMENU, 
			23,120,164,34, iImg, 5, 6,MN_GM_LIST);
		//4�� ¥�� �������̽� �Ǵ� ���θ޴��� �θ���
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_CALL_MERCHANT_OR_SKILL_MASTER, 
			23,175, 164,34, iImg, 8, 9,GM_QUEST_MERCHANT);
	}
	{//gm ����Ʈ�� EXPLAIN �������� ������ ����
		const int iMnIndex	= MN_GM_ITEM_EXPLAIN;
		const int iImg		= GUILD_LIST;
		int iFtNum = 0;
		// �׷��� ������ ��� ���� ������ ����
		::SMenuReg(iMnIndex, CENTER_X, 34, iImg,7,5);//�ʵ�� 10���� �ʿ��մϴ�.
		// ������ ����
		::SMenuFTReg(iMnIndex,iFtNum++,FT_GM_ITEM_EXPLAIN, 0, 
			60,109, 100, 15, iImg, 0, 0,155);//0
		//OK
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SUBMENU_BACK, 
			27,325, 101-27, 348-325, iImg, 8, 9);//0
		// CANCEL
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SUBMENU_BACK, 
			173,325, 247-173, 348-325, iImg, 5, 6);//0
		// up  scroll 
		::SMenuFTReg(iMnIndex,iFtNum++,FT_NO_CHECK, SWD_GUILD_EXPLAIN_SCROLL, 
			218,263, 239-218, 275-263, 0, 235, 235,LEFT);//0
		// down  scroll 
		::SMenuFTReg(iMnIndex,iFtNum++,FT_NO_CHECK, SWD_GUILD_EXPLAIN_SCROLL, 
			218,278, 239-218, 275-278, 0, 235, 235,LEFT);//0
	}
	{//gm ����Ʈ�� �䱸�ϴ� �������̽�
		const int iMnIndex	= MN_GM_LIST;
		const int iImg		= GUILD_LIST;
		int iFtNum = 0;
		::SMenuReg(iMnIndex, CENTER_X, 34, iImg,7,8);//�ʵ�� 10���� �ʿ��մϴ�.

		// �׷��� ������ ���� ���� ����Ʈ
		::SMenuFTReg(iMnIndex,iFtNum++,FT_GM_LIST, 0, 
			112,312,0, 0, iImg, 0, 4);//0// ��ũ�� ��ư�� ��� �ش�.
		// OK
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_GM_REGIST, 
			27,325,101-27, 348-325, iImg, 8, 9);//1�� �ʵ�
		// CANCEL
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SUBMENU_BACK, 
			173,325,247-173, 348-325, iImg, 5, 6);//2�� �ʵ�
		// explain
		::SMenuFTReg(iMnIndex,iFtNum++,FT_NO_CHECK, 1, 
			185,178,51, 17, GUILD_ADMIN_ETC, 1, 2,MN_GM_ITEM_EXPLAIN);
		// explain
		::SMenuFTReg(iMnIndex,iFtNum++,FT_NO_CHECK, 2, 
			185,198,51, 17, GUILD_ADMIN_ETC, 1, 2,MN_GM_ITEM_EXPLAIN);
		// explain
		::SMenuFTReg(iMnIndex,iFtNum++,FT_NO_CHECK, 3, 
			185,218,51, 17, GUILD_ADMIN_ETC, 1, 2,MN_GM_ITEM_EXPLAIN);
		// up  scroll 
		::SMenuFTReg(iMnIndex,iFtNum++,FT_NO_CHECK, SWD_GM_LIST_SCROLL, 
			115,315,25, 20, iImg, 2, 3,LEFT);
		// down  scroll 
		::SMenuFTReg(iMnIndex,iFtNum++,FT_NO_CHECK, SWD_GM_LIST_SCROLL, 
			115,336,25, 20, iImg, 0, 1,RIGHT);
	}
	{//�⼺ �����	��ư 4�� ¥��
		const int iMnIndex	= MN_SKILL_MASTER_LIST;
		const int iImg		= SKILL_MASTER_KIND_SELECT_IMG;
		int iFtNum = 0;
		::SMenuReg(iMnIndex, CENTER_X, 34, iImg,0,8);//�ʵ�� 10���� �ʿ��մϴ�.

		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_CALL_SKILL_MASTER_QUEST, 
			30,53, 200,34, iImg, 2, 3,GM_QUEST_BLACKSMITH_MASTER);//��������
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_CALL_SKILL_MASTER_QUEST, 
			30,106, 200,34, iImg, 5, 6,GM_QUEST_CARPENTER_MASTER);//���
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_CALL_SKILL_MASTER_QUEST, 
			30,159, 200,34, iImg, 8, 9,GM_QUEST_BOWCRAFT_MASTER);//Ȱ
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_CALL_SKILL_MASTER_QUEST, 
			30,212, 200,34, iImg, 11, 12,GM_QUEST_TAILOR_MASTER);//���
		// Done
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SUBMENU_BACK, 
			74,260,110,30, iImg, 14, 15);
	}
	{	
		const int iMnIndex	= MN_GM_2ND_SKILL;
		const int iImg		= GM_2ND_SKILL_IMG;
		int iFtNum = 0;
		::SMenuReg(iMnIndex, 150, 70, iImg,0,25,0);

		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_GM_2ND_OK, 
			19,323,92,30, iImg, 2, 3);//OK
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_GM_2ND_CANCEL, 
			198,323,92,30, iImg, 5, 6);//CANCEL
		//���м�:  24,196    262/108
		//���� ������(Ÿ��)(���� ��ܾ����� ���)// �̰� �ؼ� ���ڤ�.�� �б⿡ �־� ����
		//���⿡ ���� ��µ� ��� �ִ�.
		::SMenuFTReg(iMnIndex,iFtNum++,FT_DATA_PUT, 0, 
			24,79,0,0, iImg,0,DP_GM_2ND_SKILL_TARGET_ITEM_PUT);//CANCEL
		//��� �ø� ��.
		::SMenuFTReg(iMnIndex,iFtNum++,FT_GM_2ND_SKILL_MAKE_ITEM, 0, 
			175,59,104,87, iImg, 0, 21);//CANCEL
		//������ �����
		::SMenuFTReg(iMnIndex,iFtNum++,FT_DATA_PUT, 0, 
			72+2,55+4,0,0, iImg, DP_RARE_UPGRADE_RESULT, 21);//CANCEL
	}
	{//�⼺ ��ų
		const int iMnIndex	= MN_SKILL_MASTER_MAKE;
		const int iImg		= SKILL_MASTER_MAKE_IMG;
		int iFtNum = 0;
		::SMenuReg(iMnIndex, 80, 30, iImg,0,10,0);
		
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SKILL_MASTER_MULTIRARE_OK, 
			59,404,92,30, iImg, 2, 3);//OK

		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_GM_2ND_CANCEL, 
			251,404,92,30, iImg, 5, 6);//CANCEL
		
		::SMenuFTReg(iMnIndex,iFtNum++,FT_DATA_PUT, 0, 
			24,82,182,87, iImg,0,DP_SKILL_MASTER_RESULT_ITEM);
		//��� �ø� ��.
		::SMenuFTReg(iMnIndex,iFtNum++,FT_SKILL_MASTER_MAKE_ITEM, 0, 
			228,59,129,103, iImg, 0, 21);
	}
	::SetGMQUESTMenu(GM_QUEST_MERCHANT);
	{//�� ��ȯ �ϱ�(BOND_MONEY)
		const int iMnIndex	= MN_EXCHANGE_BOND_MONEY;
		const int iImg		= EXCHANGE_BOND_MONEY_IMG;
		int iFtNum = 0;
		::SMenuReg(iMnIndex, 150, 30, iImg,0,10,0);
		//CANCEL
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SMALLMENU_END, 
			148,317,110,30, iImg, 5, 6);
		//OK
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_EXCHANGE_BOND_MONEY_OK, 
			20,317,110,30, iImg, 2, 3);
		//���� â
		strcpy(SMenu[iMnIndex].nField[iFtNum].temp, lan->OutputMessage(4,561) );
		::SMenuFTReg(iMnIndex,iFtNum++,FT_DATA_PUT, 0, 
			34,64,0,0, 15,200,DP_RECT_STRING_PUT_CUSTOM);
		//������ ���� ����� 
		::SMenuFTReg(iMnIndex,iFtNum++,FT_EXCHANGE_BOND_MONEY_ITEM, 0, 
			41,218,194,68, iImg, 0, 0);//CANCEL
	}
	
	{//���� �޴�
		const int iMnIndex	= MN_MAIL_MENU;
		const int iImg		= MAIL_MENU;

		::SMenuReg(iMnIndex, 400, 0, iImg,0,12,0);
		//���� ������ (������ ����Ʈ�� ��û �ϸ� ������ ���� ���� �ؼ� ���� �Ѵ�)
		::SMenuFTReg(iMnIndex,0, FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_CALL_MAILMENU, 
			20,61,73,19, iImg, 49, 49, MVT_RECV_LIST);//��ġ �ȹٲ�� ���� (0��)

		//���� ������
		::SMenuFTReg(iMnIndex,1, FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_CALL_MAILMENU, 
			98,61,72,19, iImg, 50, 50, MVT_DELETE_LIST);//��ġ �ȹٲ�� ���� (1��)

		//���� ����
		::SMenuFTReg(iMnIndex,2,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_MAIL_REPAIR, 
			186,442 ,78,30, iImg, 51, 52,MVT_DELETE_LIST);

		int iFtNum = 3;
		//ALL(��� ����)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_MAIL_SELECT_ALL, 
			23,86,24,24, iImg, 24, 25);
		
		//Zone(üũ��ư, �̸�(���Ͼ���), ��������(�б�), �ð�)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MAIL_LIST, SWD_NONE, 
			26,112,380,400,iImg);
		
		//��ũ�� ��ư ��
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_MAIL_SCROLL, 
			155,402,26,25, iImg, 30, 31,SCROLL_DOWN);
		//��ũ�� ��ư ��
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_MAIL_SCROLL, 
			219,402,26,25, iImg, 32, 33,SCROLL_UP);
		//���� ������
		::SMenuFTReg(iMnIndex,iFtNum++,FT_DATA_PUT, SWD_NONE, 
			198, 410,78,30, iImg, 0, DP_NOW_VIEW_MAIL_PAGE);
		
		//���� ����
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_CALL_MAIL_WRITE, 
			18,442 ,78,30, iImg, 4, 5,0);
		//����
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_MAIL_DELETE, 
			102,442 ,78,30, iImg, 6, 7,0);
		//Cancel
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SMALLMENU_END, 
			304,442 ,78,30, iImg, 10, 11);
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	���� ������ �޴�
	{
		const int iMnIndex	= MN_MAIL_WRITE;
		const int iImg		= MAIL_MENU;
		int iFtNum = 0;

		::SMenuReg(iMnIndex, 400, 0, iImg,3,20,0);

		//CANCEL
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_CALL_MAILMENU, 
			304,442,78,30, iImg, 10, 11,MVT_RECV_LIST);
		
		//�� ��ũ�� (����Ÿ��)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_TEMP_MAIL_TYPE_SCROLL, 
			94,86,26,25, iImg, 30, 31,SCROLL_UP);
		//�� ��ũ�� (����Ÿ��)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_TEMP_MAIL_TYPE_SCROLL, 
			152,86,26,25, iImg, 32, 33,SCROLL_DOWN);

		//ģ�� ����Ʈ
//		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_CALL_FRIEND_MENU, 
//			297,114,78,30, iImg, 22, 23);

		//�޴� ��� �Է�
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MAIL_WRITE, SWD_NONE, 
			98,122,190,20, iImg, 180, 0,1);
		//����
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MAIL_WRITE, SWD_NONE, 
			98,152,270,20, iImg, 180, 0,2);
		//����
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MAIL_WRITE, SWD_NONE, 
			30,178,320,250, iImg, 315, 0,3);
		//�� ��ũ��

		//�Ʒ� ��ũ��

		//������
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_MAIL_WRITE_SEND, 
			18,442,78,30, iImg, 16, 17);

		//�ٽþ���
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_MAIL_WRITE_DELETE, 
			102,442,78,30, iImg, 18, 19);

		//����Ÿ�� ������ //���� �� �������Դϴ�.
		::SMenuFTReg(iMnIndex,iFtNum++,FT_DATA_PUT,SWD_NONE, 
			126,90,26,25, iImg, 0, DP_TEMP_MAIL_ICON,0);
	}
/////////////////////////////////////////////////////////////////////////////////////////////////////
	{
		const int iMnIndex	= MN_MAIL_READ;
		const int iImg		= MAIL_MENU;
		int iFtNum = 0;

		::SMenuReg(iMnIndex, 400, 0, iImg,2,15,0);
		
		//CANCEL
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_CALL_MAILMENU, 
			304,442,78,30, iImg, 10, 11,MVT_RECV_LIST);
		
		//ģ���ε��
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_MAIL_SENDER_REGIST_FRIEND, 
			209,115,78,30, iImg, 20, 21,1);

		//������
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MAIL_READ, SWD_NONE, 
			98,124,100,20, iImg, 180, 0,1);
		//����
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MAIL_READ, SWD_NONE, 
			98,152,100,20, iImg, 180, 0,2);
		//����
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MAIL_READ, SWD_NONE, 
			30,177,100,20, iImg, 315, 0,3);
		//��¥,
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MAIL_READ, SWD_NONE, 
			250,92,100,20, iImg, 180, 0,4);
		
		//�����
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_MAIL_DELETE, 
			18,442 ,78,30, iImg, 12, 13,1);

		//����
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_CALL_MAIL_WRITE, 
			102,442 ,78,30, iImg, 14, 15,1);

		//PC�� ����
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_MAIL_SAVE, 
			186,442 ,78,30, iImg, 8, 9);
		
		//����Ÿ�� ������ //���� �� �������Դϴ�.
		::SMenuFTReg(iMnIndex,iFtNum++,FT_DATA_PUT,SWD_NONE, 
			97,89,26,25, iImg, 0, DP_TEMP_MAIL_ICON,0);
	}
	{
////////////////////////////////////////////////////////////////////////////////////////////
// �Ʒ���
		const int iMnIndex	= MN_ARENA_LOBBY;
		const int iImg		= ARENA_IMG;
		int iFtNum = 0;

		::SMenuReg(iMnIndex, 287, 0, iImg,0,25,0);

		// RED ���
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_BATTLE_REGIST, 
			18,82,36,26, iImg, 8, 9,ATI_RED_TEAM);

		// blue ���
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_BATTLE_REGIST, 
			300,82,36,26, iImg, 8, 9,ATI_BLUE_TEAM);

		// ����
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_FIGHTMAP_WAIT_MEMBER, 
			152,82 ,50,26, iImg, 22, 23,0);
		
		//���� ���ð�.
		::SMenuFTReg(iMnIndex,iFtNum++,FT_DATA_PUT, SWD_NONE, 
			470,195 ,50,26, iImg, 0, DP_ARENA_WAIT_TIME);

		//R �·�
		::SMenuFTReg(iMnIndex,iFtNum++,FT_DATA_PUT, SWD_NONE, 
			110,90,50,26, iImg, 0, DP_ARENA_TEAM_WIN_RATE,ATI_RED_TEAM);
		//B �·�
		::SMenuFTReg(iMnIndex,iFtNum++,FT_DATA_PUT, SWD_NONE, 
			260,90,50,26, iImg, 0, DP_ARENA_TEAM_WIN_RATE,ATI_BLUE_TEAM);

		//R �ѹ��þ�
		::SMenuFTReg(iMnIndex,iFtNum++,FT_DATA_PUT, SWD_NONE, 
			360,86 ,50,26, iImg, 0, DP_BETTING_MONEY,ATI_RED_TEAM);
		//B �ѹ��þ�
		::SMenuFTReg(iMnIndex,iFtNum++,FT_DATA_PUT, SWD_NONE, 
			360,110 ,50,26, iImg, 0, DP_BETTING_MONEY,ATI_BLUE_TEAM);

		//�� ���þ�
		::SMenuFTReg(iMnIndex,iFtNum++,FT_DATA_PUT, SWD_NONE, 
			360, 143 , 50, 26, iImg, 0, DP_MY_BETTING_MONEY, 0);
		
		//�� ����
		::SMenuFTReg(iMnIndex,iFtNum++,FT_DATA_PUT, SWD_NONE, 
			360, 167 , 50, 26, iImg, 0, DP_MY_BETTING_MONEY, 1);

		// �������� ���� �����ֱ�		// red//R ���� ����
		SMenu[iMnIndex].nField[iFtNum].nSHideNomalCount=1;
		SMenu[iMnIndex].nField[iFtNum].nSHideNomalStart=0;
		SMenu[iMnIndex].nField[iFtNum].nShideNomalPlus=1;
		::SMenuFTReg(iMnIndex,iFtNum++,FT_VIEW_BATTLE_MEMBER, SWD_NONE, 
			22,120,100,100, iImg, 0, 0,ATI_RED_TEAM);
		
		// �������� ���� �����ֱ�		// blue//B ���� ���� 
		SMenu[iMnIndex].nField[iFtNum].nSHideNomalCount=1;
		SMenu[iMnIndex].nField[iFtNum].nSHideNomalStart=0;
		SMenu[iMnIndex].nField[iFtNum].nShideNomalPlus=1;
		::SMenuFTReg(iMnIndex,iFtNum++,FT_VIEW_BATTLE_MEMBER, SWD_NONE, 
			194,120,100,100, iImg, 0, 0,ATI_BLUE_TEAM);

		// BATTLE�� �����Ѵ�.//��ŸƮ k
		::SMenuFTReg(iMnIndex,iFtNum++,FT_FIGHTMAP_START, SWD_FIGHTMAP_WAIT_MEMBER, 
			67,342 ,110,30, iImg, 2, 3,0);

		//�ݰ� ������//Cancel k
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_FIGHT_WAIT_EXIT, //
			335,342 ,110,30, iImg, 5, 6,0);

		//����� ��� k
		::SMenuFTReg(iMnIndex,iFtNum++,FT_VIEW_BATTLE_WAIT_MEMBER, SWD_NONE, 
			22,258 ,78,30, iImg, 0, 0,0);

		//����� ��ũ�� ->
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_NONE, 
			467,253 ,30,30, iImg, 17, 18,SCROLL_UP);

		//����� ��ũ�� <-
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_NONE, 
			467,302 ,30,30, iImg, 20, 21,SCROLL_DOWN);
		//����
		::SMenuFTReg(iMnIndex,iFtNum++, FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_CALL_ARENA_OBSERVER_MENU, 
			364,214 ,50,26, iImg, 11, 12,MN_OBSERVER_MODE);

		//����
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_CALL_ARENA_BETTING_MENU, 
			364+74,214 ,50,26, iImg, 14, 15,MN_BETTING);

/*		// red  ->
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_DSCROLL_AUTO_PUT, SWD_NONE, 
			186,442 ,78,30, iImg, 8, 9,18);

		// red  <-
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_ISCROLL_AUTO_PUT, SWD_NONE, 
			186,442 ,78,30, iImg, 8, 9,18);

		// blue ->
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_DSCROLL_AUTO_PUT, SWD_NONE, 
			186,442 ,78,30, iImg, 8, 9,19);
		
		// blue <-
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_ISCROLL_AUTO_PUT, SWD_NONE, 
			186,442 ,78,30, iImg, 8, 9,19);
*/	}
	{//���� ���
		const int iMnIndex	= MN_OBSERVER_MODE;
		const int iImg		= OBSERVER_IMG;
		int iFtNum = 0;
		::SMenuReg(iMnIndex, CENTER_X, 100, iImg,0,10,0);
		//�ȳ���
		::SMenuFTReg(iMnIndex,iFtNum,FT_DATA_PUT,0,
			24,65,0,0, iImg,200, DP_RECT_STRING_PUT);
		::strcpy(SMenu[iMnIndex].nField[iFtNum++].temp, lan->OutputMessage(3,281) );

		//OK
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_OBSERVER_TEAM_SELECT, 
			25,197,78,30, iImg, 3, 4);
		//Cancel
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SMALLMENU_END, 
			142,197,78,30, iImg, 6, 7);
		//üũ �ڽ�
		::SMenuFTReg(iMnIndex,iFtNum++,FT_SELECT_OBSERVER_TARGET_TEAM, SWD_NONE, 
			82,160,18,18, iImg, 8, 8,ATI_RED_TEAM);
		//üũ �ڽ�
		::SMenuFTReg(iMnIndex,iFtNum++,FT_SELECT_OBSERVER_TARGET_TEAM, SWD_NONE, 
			210,160,18,18, iImg, 8, 8,ATI_BLUE_TEAM);
	}
	{//���� �޴� 
		const int iMnIndex	= MN_BETTING;
		const int iImg		= OBSERVER_IMG;
		int iFtNum = 0;
		::SMenuReg(iMnIndex, CENTER_X, 100, iImg,1,10,0);
		//�ȳ���
		::SMenuFTReg(iMnIndex,iFtNum,FT_DATA_PUT,0,
			25,130,0,0, 16,230, DP_RECT_STRING_PUT_CUSTOM,FONT_COLOR_SOLID_RED);
		::strcpy(SMenu[iMnIndex].nField[iFtNum++].temp, lan->OutputMessage(3,280) );
		
		//OK
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SEND_MY_ARENA_BETTING_MONEY, 
			224,242 ,42,30, iImg, 11, 12);
		//Done
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SMALLMENU_END, 
			103,289 ,75,30, iImg, 14, 15);
		//���൷
		::SMenuFTReg(iMnIndex,iFtNum++,FT_DATA_PUT,	0,
			58+40,72,0,0,iImg,0,DP_BANK_TOTAL);

		//�� ���� �ɷ��ִ� �ݾ�
		::SMenuFTReg(iMnIndex,iFtNum++,FT_DATA_PUT, SWD_NONE, 	
			30+90, 103, 50, 26, iImg, 0, DP_MY_BETTING_MONEY,0);

		//üũ �ڽ�
		::SMenuFTReg(iMnIndex,iFtNum++,FT_SELECT_BETTING_TARGET_TEAM, SWD_NONE, 
			85,195,25,25, iImg, 8, 8,ATI_RED_TEAM);
		//üũ �ڽ�
		::SMenuFTReg(iMnIndex,iFtNum++,FT_SELECT_BETTING_TARGET_TEAM, SWD_NONE, 
			85,222,25,25, iImg, 8, 8,ATI_BLUE_TEAM);

		//R �ѹ��þ�
		::SMenuFTReg(iMnIndex,iFtNum++,FT_DATA_PUT, SWD_NONE, 
			30+50,195 ,50,26, iImg, 0, DP_BETTING_MONEY,ATI_RED_TEAM);
		//B �ѹ��þ�
		::SMenuFTReg(iMnIndex,iFtNum++,FT_DATA_PUT, SWD_NONE, 
			30+50,222 ,50,26, iImg, 0, DP_BETTING_MONEY,ATI_BLUE_TEAM);

		//�� ���þ� �Է�
		::SMenuFTReg(iMnIndex,iFtNum++,FT_ARENA_BETTING, SWD_NONE, 	
			30, 252, 50, 26, iImg, 180, 0, 0);
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	revival menu �޴�
	{
		const int iMnIndex	= MN_REVIVAL;
		const int iImg		= OBSERVER_IMG;
		int iFtNum = 0;
		::SMenuReg(iMnIndex, 400-80, 48, iImg,16,2,0);
		// revival ��ư
		::SMenuFTReg(iMnIndex,iFtNum++,FT_REVIVAL_BUTTON, SWD_NONE, 
			0,0,80,30, iImg, 17, 18);
	}

}

void SetGMQUESTMenu(const int iGmQuestIndex)
{//���� ����Ʈ OK �ϱ�(������ �� �ʿ��ϰ� �ϴ°� //021108 lsw
	int iBgImgNo = 0;
	switch(iGmQuestIndex)
	{
	case GM_QUEST_MERCHANT://����         
		{
			iBgImgNo = 1;
		}break;
	case GM_QUEST_BLACKSMITH_MASTER:	//�������� �⼺
	case GM_QUEST_CARPENTER_MASTER:	//��� �⼺    
	case GM_QUEST_BOWCRAFT_MASTER:	//Ȱ���ۻ� �⼺
	case GM_QUEST_TAILOR_MASTER	:	//��ܻ� �⼺
		{
			iBgImgNo = 0;
		}break;
	}//switch(iResult)
	
	const int iMnIndex	= NM_MERCHANT_QUEST;
	const int iImg		= SKILL_MASTER_QUEST_IMG;
	int iFtNum = 0;

	::SMenuReg(iMnIndex, CENTER_X, 34, iImg,iBgImgNo,8,0,0,iGmQuestIndex);//work �� ���� �Ǿ�� ���.
	//OK
	::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_GM_REGIST_2, 
		27-3,325-3,101-27, 348-325, iImg, 3, 4);
	// CANCEL
	::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SUBMENU_BACK, 
		173-3,325-3,247-173, 348-325, iImg, 6, 7);
	// �׷��� ������ ���� ���� ����Ʈ//���÷���
	::SMenuFTReg(iMnIndex,iFtNum++,FT_DISPLAY_GM_REGIST, 0, 
		112,312,0, 0, iImg, 0, 4,GM_QUEST_MERCHANT);//0// ��ũ�� ��ư�� ��� �ش�.
}

void StartMapSelectMenuSet(const int iNation)//�� ������ ������ �޴��� ���� �Ѵ�
{	
	::SMenuReg(MN_START_MAP, 0, 0, NEW_START_MAP_IMG,0,17);
	// connect game
	::SMenuFTReg(MN_START_MAP,0,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT,SWD_CONNECT,
						568, 617,216, 67, NEW_START_MAP_IMG, 13, 13,MN_LODING);
	// recent logout	
	::SMenuFTReg(MN_START_MAP,1,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT,SWD_CONNECT_RECENT,
						782, 617,216, 67, NEW_START_MAP_IMG, 13, 13,MN_LODING);
	// back button( 4�� ���� ȭ������ ���ư� )
	::SMenuFTReg(MN_START_MAP,9,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT,SWD_RE_LOGON,
						355, 617,216, 67, NEW_START_MAP_IMG, 13, 13,MN_SELECT_CHARACTER);
	// ���õǾ��� ���� ��� �����ֱ�
	SMenu[MN_START_MAP].nField[15].nType=FT_SELECT_CITY;
	SMenu[MN_START_MAP].nField[15].x=933;
	SMenu[MN_START_MAP].nField[15].y=593;
	SMenu[MN_START_MAP].nField[15].nImageNumber=0;
	SMenu[MN_START_MAP].nField[15].nImageType=START_MAP;

	// ���� �����ϴ� ȿ�� �����ֱ� ( ����Ʈ )
	SMenu[MN_START_MAP].nField[3].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT_2;
	SMenu[MN_START_MAP].nField[3].nSpecialWillDo=SWD_WILLDO_WORK;
	SMenu[MN_START_MAP].nField[3].nWillDo=SC_HELTERNT_VILLAGE;
	//SMenu[MN_START_MAP].nField[3].nWillDo=SC_HELTERNT_CASTLE;
	SMenu[MN_START_MAP].nField[3].x=344;
	SMenu[MN_START_MAP].nField[3].y=189;
	SMenu[MN_START_MAP].nField[3].nRectImage=7;
	SMenu[MN_START_MAP].nField[3].nImageNumber=7;
	SMenu[MN_START_MAP].nField[3].nSHideNomalCount=17;
	SMenu[MN_START_MAP].nField[3].nShideNomalPlus=3;
	SMenu[MN_START_MAP].nField[3].nImageType=START_MAP;
	::SetRect(SMenu[MN_START_MAP].nField[3].rCheakBox,364,209,444,289);

	// ���� �����ϴ� ȿ�� �����ֱ� ( ���̼��� ���� )
#ifndef CHINA_LOCALIZING_
	SMenu[MN_START_MAP].nField[4].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT_2;
#endif
	SMenu[MN_START_MAP].nField[4].nSpecialWillDo=SWD_WILLDO_WORK;
	SMenu[MN_START_MAP].nField[4].nWillDo=SC_BAISUS_IMPEL;
	SMenu[MN_START_MAP].nField[4].x=600;
	SMenu[MN_START_MAP].nField[4].y=290;
	SMenu[MN_START_MAP].nField[4].nRectImage=8;
	SMenu[MN_START_MAP].nField[4].nImageNumber=8;
	SMenu[MN_START_MAP].nField[4].nSHideNomalCount=18;
	SMenu[MN_START_MAP].nField[4].nShideNomalPlus=3;
	SMenu[MN_START_MAP].nField[4].nImageType=START_MAP;
	::SetRect(SMenu[MN_START_MAP].nField[4].rCheakBox,620,310,700,390);

	// ���� �����ϴ� ȿ�� �����ֱ� ( ���ʽ� )
	SMenu[MN_START_MAP].nField[5].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT_2;
	SMenu[MN_START_MAP].nField[5].nSpecialWillDo=SWD_WILLDO_WORK;
	SMenu[MN_START_MAP].nField[5].nWillDo=SC_RENUS;
	SMenu[MN_START_MAP].nField[5].x=462;
	SMenu[MN_START_MAP].nField[5].y=234;
	SMenu[MN_START_MAP].nField[5].nRectImage=9;
	SMenu[MN_START_MAP].nField[5].nImageNumber=9;
	SMenu[MN_START_MAP].nField[5].nSHideNomalCount=19;
	SMenu[MN_START_MAP].nField[5].nShideNomalPlus=3;
	SMenu[MN_START_MAP].nField[5].nImageType=START_MAP;
	::SetRect(SMenu[MN_START_MAP].nField[5].rCheakBox,482,244,542,324);

	// ���� �����ϴ� ȿ�� �����ֱ� ( �ٶ�ź )
	SMenu[MN_START_MAP].nField[6].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT_2;
	SMenu[MN_START_MAP].nField[6].nSpecialWillDo=SWD_WILLDO_WORK;
	SMenu[MN_START_MAP].nField[6].nWillDo=SC_BARANTAN;
	SMenu[MN_START_MAP].nField[6].x=775;
	SMenu[MN_START_MAP].nField[6].y=300;
	SMenu[MN_START_MAP].nField[6].nRectImage=5;
	SMenu[MN_START_MAP].nField[6].nImageNumber=5;
	SMenu[MN_START_MAP].nField[6].nSHideNomalCount=6;
	SMenu[MN_START_MAP].nField[6].nImageType=START_MAP;
	::SetRect(SMenu[MN_START_MAP].nField[6].rCheakBox,795,320,875,400);

	// ���� �����ϴ� ȿ�� �����ֱ� ( �̶󹫽��� )
#ifndef CHINA_LOCALIZING_
	SMenu[MN_START_MAP].nField[7].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT_2;
#endif
	SMenu[MN_START_MAP].nField[7].nSpecialWillDo=SWD_WILLDO_WORK;
	SMenu[MN_START_MAP].nField[7].nWillDo=SC_IRAMUS;
	SMenu[MN_START_MAP].nField[7].x=524;
	SMenu[MN_START_MAP].nField[7].y=255;
	SMenu[MN_START_MAP].nField[7].nRectImage=11;
	SMenu[MN_START_MAP].nField[7].nImageNumber=11;
	SMenu[MN_START_MAP].nField[7].nSHideNomalCount=21;
	SMenu[MN_START_MAP].nField[7].nShideNomalPlus=3;
	SMenu[MN_START_MAP].nField[7].nImageType=START_MAP;
	::SetRect(SMenu[MN_START_MAP].nField[7].rCheakBox,544,265,604,345);

	// ���� �����ϴ� ȿ�� �����ֱ� ( �������� )
#ifndef CHINA_LOCALIZING_
	SMenu[MN_START_MAP].nField[8].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT_2;
#endif
	SMenu[MN_START_MAP].nField[8].nSpecialWillDo=SWD_WILLDO_WORK;
	SMenu[MN_START_MAP].nField[8].nWillDo=SC_DIPH_UBEN;
	SMenu[MN_START_MAP].nField[8].x=456;
	SMenu[MN_START_MAP].nField[8].y=469;
	SMenu[MN_START_MAP].nField[8].nRectImage=12;
	SMenu[MN_START_MAP].nField[8].nImageNumber=12;
	SMenu[MN_START_MAP].nField[8].nSHideNomalCount=22;
	SMenu[MN_START_MAP].nField[8].nShideNomalPlus=3;
	SMenu[MN_START_MAP].nField[8].nImageType=START_MAP;
	SetRect(SMenu[MN_START_MAP].nField[8].rCheakBox,456,469,536,549);
/*
	// ���� �����ϴ� ȿ�� �����ֱ� ( ����Ʈ ���� )
	SMenu[MN_START_MAP].nField[10].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT_2;
	SMenu[MN_START_MAP].nField[10].nSpecialWillDo=SWD_WILLDO_WORK;
	SMenu[MN_START_MAP].nField[10].nWillDo=SC_HELTERNT_VILLAGE;
	SMenu[MN_START_MAP].nField[10].x=75;
	SMenu[MN_START_MAP].nField[10].y=155;
	SMenu[MN_START_MAP].nField[10].nRectImage=25;
	SMenu[MN_START_MAP].nField[10].nImageNumber=25;
	SMenu[MN_START_MAP].nField[10].nImageType=START_MAP;
	::SetRect(SMenu[MN_START_MAP].nField[10].rCheakBox,120,154,154,176);

	// ���� �����ϴ� ȿ�� �����ֱ� ( �޴��� �� )
	SMenu[MN_START_MAP].nField[11].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT_2;
	SMenu[MN_START_MAP].nField[11].nSpecialWillDo=SWD_WILLDO_WORK;
	SMenu[MN_START_MAP].nField[11].nWillDo=SC_HUDAIN_RIVER;
	SMenu[MN_START_MAP].nField[11].x=46;
	SMenu[MN_START_MAP].nField[11].y=204;
	SMenu[MN_START_MAP].nField[11].nRectImage=26;
	SMenu[MN_START_MAP].nField[11].nImageNumber=26;
	SMenu[MN_START_MAP].nField[11].nImageType=START_MAP;
	::SetRect(SMenu[MN_START_MAP].nField[11].rCheakBox,70,185,110,207);

	// ���� �����ϴ� ȿ�� �����ֱ� ( ȸ����� )
	SMenu[MN_START_MAP].nField[12].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT_2;
	SMenu[MN_START_MAP].nField[12].nSpecialWillDo=SWD_WILLDO_WORK;
	SMenu[MN_START_MAP].nField[12].nWillDo=SC_GRAY_MOUNTAIN;
	SMenu[MN_START_MAP].nField[12].x=239;
	SMenu[MN_START_MAP].nField[12].y=61;
	SMenu[MN_START_MAP].nField[12].nRectImage=27;
	SMenu[MN_START_MAP].nField[12].nImageNumber=27;
	SMenu[MN_START_MAP].nField[12].nImageType=START_MAP;
	::SetRect(SMenu[MN_START_MAP].nField[12].rCheakBox,230,50,337,90);
*/
	// ���� �����ϴ� ȿ�� �����ֱ� ( Ÿ�̳� )
#ifndef CHINA_LOCALIZING_
	SMenu[MN_START_MAP].nField[13].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT_2;
#endif
	SMenu[MN_START_MAP].nField[13].nSpecialWillDo=SWD_WILLDO_WORK;
	SMenu[MN_START_MAP].nField[13].nWillDo=SC_TYREN;
	SMenu[MN_START_MAP].nField[13].x=379;
	SMenu[MN_START_MAP].nField[13].y=419;
	SMenu[MN_START_MAP].nField[13].nRectImage=10;
	SMenu[MN_START_MAP].nField[13].nImageNumber=10;
	SMenu[MN_START_MAP].nField[13].nSHideNomalCount=28;
	SMenu[MN_START_MAP].nField[13].nShideNomalPlus=3;
	SMenu[MN_START_MAP].nField[13].nImageType=START_MAP;
    SetRect(SMenu[MN_START_MAP].nField[13].rCheakBox, 379, 419, 459, 499);
	
}

void StartMenuSet2()
{
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////�α��� �޴�/////////////////////////////////////////////////
	{
	::CallLoginMenu();
	::SMenuReg(MN_LOGIN, 0, 0, MAINSTART_LOGIN_IMG,0,10);	//300 ¥�� �������̽� �̴ϱ�//�����̶� 0 0 �̴�
	
	// ����Ʈ �ڽ� ���� �� ���̸� �Դٰ���.....
	// ����Ʈ �ڽ��� �۾��� ��¸�..SMenu[MN_LOGIN].x + 532, SMenu[MN_LOGIN].y + 485
	::SMenuFTReg(MN_LOGIN, 3, FT_SPECIAL_WILLDO, SWD_IDEDITBOX, 532,485, 274, 27, -1, 0, 0 ,0);
	//::SMenuFTReg(MN_LOGIN, 5, FT_DATA_PUT, 0, 365,362 ,193, 15,MAINSTART_LOGIN_IMG, 0,DP_IDEDITBOX,0);
	
	::SMenuFTReg(MN_LOGIN, 4, FT_SPECIAL_WILLDO, SWD_PASSEDITBOX, 532,540, 274, 27, -1, 0, 0 ,0);
	//::SMenuFTReg(MN_LOGIN, 6, FT_DATA_PUT, 0, 365,401, 193, 15, 0, 0, DP_PASSEDITBOX,0);

	//�ʵ� ��ȣ �ٲ��� ���� ����Ű �ȸ��� �α��� �ϱ�
	::SMenuFTReg(MN_LOGIN,0,FT_SPECIAL_WILLDO,SWD_LOGON,532,585,128,27,-1,0,0,MN_SELECT_CHARACTER);//�α���
	//SHOW CREDITS��ư ���ý� ����
//	::SMenuFTReg(MN_LOGIN,7,FT_HIDE_WILLDO_PUT,0,326,464,157,27,MAINSTART_LOGIN_IMG,2,2,MN_CREDIT);
	//INTRO��ư ���ý� �ٿ��
//	::SMenuFTReg(MN_LOGIN,8,FT_HIDE_WILLDO_PUT,0,364,495,75,27,MAINSTART_LOGIN_IMG,3,3,MN_INFO);
	//EXIT��ư ���ý� 
	::SMenuFTReg(MN_LOGIN,1,FT_SPECIAL_WILLDO,SWD_QUIT,679,585,128,27,-1,0,0,0);
	//NewId��ư ���ý�
//	::SMenuFTReg(MN_LOGIN,9,FT_SPECIAL_WILLDO,SWD_NEW_ID,352,432,100,30,MAINSTART_LOGIN_IMG,5,5,0);
	}
//////////////////////////////////////////////////////////////////////////////////////
	//���� ���� �޴�
	//���θ޴��ٿ��
	{
	SMenu[MN_MAINSTART_START].x=293;
	SMenu[MN_MAINSTART_START].y=224;
	SMenu[MN_MAINSTART_START].nImageNumber=9;
	SMenu[MN_MAINSTART_START].nImageType=START_ETC;
	SMenu[MN_MAINSTART_START].nFieldCount=5;
	SMenu[MN_MAINSTART_START].CheakType=2;

	//LOCATION��ư ���ý� �ٿ��	�����ʿ� ����Ʈ �޴� ����
	SMenu[MN_MAINSTART_START].nField[0].nType=FT_NO_CHECK;  // �ʵ� ��ȣ ���� ������ �ϴ� �κ� ����
	SMenu[MN_MAINSTART_START].nField[0].nSpecialWillDo=SWD_CALL_OTHER;
	SMenu[MN_MAINSTART_START].nField[0].nWillDo=MN_LOCATION;
	SMenu[MN_MAINSTART_START].nField[0].x=29;
	SMenu[MN_MAINSTART_START].nField[0].y=29;
	SMenu[MN_MAINSTART_START].nField[0].nRectImage=831;
	SMenu[MN_MAINSTART_START].nField[0].nImageNumber=31;

	//SELECT��ư ���ý� �ٿ��,		���ʿ� �����̼� �޴� ����
	SMenu[MN_MAINSTART_START].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_MAINSTART_START].nField[1].nSpecialWillDo=SWD_CALL_OTHER;   // �ʵ� ��ȣ ���� ������ �ϴ� �κ� ����
	SMenu[MN_MAINSTART_START].nField[1].nWillDo=MN_SELECT;
	SMenu[MN_MAINSTART_START].nField[1].x=114;
	SMenu[MN_MAINSTART_START].nField[1].y=28;
	SMenu[MN_MAINSTART_START].nField[1].nRectImage=10;
	SMenu[MN_MAINSTART_START].nField[1].nImageNumber=11;
	SMenu[MN_MAINSTART_START].nField[1].nImageType=START_ETC;

	//BACK��ư ���ý� �ٿ��,  �ʱ� �޴��� ���ư���
	SMenu[MN_MAINSTART_START].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_MAINSTART_START].nField[2].nSpecialWillDo=SWD_LOGON;
	SMenu[MN_MAINSTART_START].nField[2].nWillDo=MN_SELECT_CHARACTER;
	SMenu[MN_MAINSTART_START].nField[2].x=113;
	SMenu[MN_MAINSTART_START].nField[2].y=115;
	SMenu[MN_MAINSTART_START].nField[2].nRectImage=14;
	SMenu[MN_MAINSTART_START].nField[2].nImageNumber=15;
	SMenu[MN_MAINSTART_START].nField[2].nImageType=START_ETC;

	//����Ʈ ��ư ���ý� �ٿ��
	SMenu[MN_MAINSTART_START].nField[3].nType=FT_NO_CHECK;
//	SMenu[MN_MAINSTART_START].nField[3].nSpecialWillDo=FT_HIDE_WILLDO_AUTO_PUT;
	SMenu[MN_MAINSTART_START].nField[3].nWillDo=MN_LODING;
	SMenu[MN_MAINSTART_START].nField[3].x=74;
	SMenu[MN_MAINSTART_START].nField[3].y=74;
	SMenu[MN_MAINSTART_START].nField[3].nRectImage=16;
	SMenu[MN_MAINSTART_START].nField[3].nImageNumber=17;
	SMenu[MN_MAINSTART_START].nField[3].nImageType=START_ETC;

	// Recent ��ư ������ ���
	SMenu[MN_MAINSTART_START].nField[4].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_MAINSTART_START].nField[4].nSpecialWillDo=SWD_CONNECT_RECENT;
	SMenu[MN_MAINSTART_START].nField[4].nWillDo=MN_LODING;
	SMenu[MN_MAINSTART_START].nField[4].x=29;
	SMenu[MN_MAINSTART_START].nField[4].y=115;
	SMenu[MN_MAINSTART_START].nField[4].nRectImage=12;
	SMenu[MN_MAINSTART_START].nField[4].nImageNumber=13;
	SMenu[MN_MAINSTART_START].nField[4].nImageType=START_ETC;



	}
	{
	//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//	�ι� ����
	::SMenuReg(MN_SELECT_CHARACTER, 0, 0, SELECT_CHARACTER_IMG,0,12);
	
	::SMenuFTReg(MN_SELECT_CHARACTER,0,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT,SWD_RESET_JOB,
						339, 571,200, 62, SELECT_CHARACTER_IMG, 6, 1);
	
	::SMenuFTReg(MN_SELECT_CHARACTER,3,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT,SWD_CHARACTER_DEL_ADD,
						540, 571,200, 62, SELECT_CHARACTER_IMG, 5, 2);

	::SMenuFTReg(MN_SELECT_CHARACTER,1,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT,SWD_IDPASS,//ID Pass�� ��ȯ
						740, 571,200, 62, SELECT_CHARACTER_IMG, 4, 3);
	
	::SMenuFTReg(MN_SELECT_CHARACTER,2,FT_NO_CHECK,0,
						540, 571,200, 62, SELECT_CHARACTER_IMG, 11, 11);//���� �ʵ� Makeĳ���� �� ���ϴ� �κ�

	//select 1 box
	SMenu[MN_SELECT_CHARACTER].nField[4].nType=FT_NOMAL_PUT;
	SMenu[MN_SELECT_CHARACTER].nField[4].nSpecialWillDo= SWD_CHARACTER_SELECT;
	SMenu[MN_SELECT_CHARACTER].nField[4].nWillDo = 0;
	SMenu[MN_SELECT_CHARACTER].nField[4].x=283;
	SMenu[MN_SELECT_CHARACTER].nField[4].y=85;
	SMenu[MN_SELECT_CHARACTER].nField[4].nRectImage= 30;
	SMenu[MN_SELECT_CHARACTER].nField[4].nImageNumber= 30;
	SMenu[MN_SELECT_CHARACTER].nField[4].nImageType= START_ETC;
	::SetRect(SMenu[MN_SELECT_CHARACTER].nField[4].rCheakBox,283, 85,445,309);

	//select 2 box
	SMenu[MN_SELECT_CHARACTER].nField[5].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_SELECT_CHARACTER].nField[5].nSpecialWillDo=SWD_CHARACTER_SELECT;
	SMenu[MN_SELECT_CHARACTER].nField[5].nWillDo= 1;
	SMenu[MN_SELECT_CHARACTER].nField[5].x=283;
	SMenu[MN_SELECT_CHARACTER].nField[5].y=319;
	SMenu[MN_SELECT_CHARACTER].nField[5].nRectImage= 30;
	SMenu[MN_SELECT_CHARACTER].nField[5].nImageNumber= 30;
	SMenu[MN_SELECT_CHARACTER].nField[5].nImageType= START_ETC;
	::SetRect(SMenu[MN_SELECT_CHARACTER].nField[5].rCheakBox,283,319,445,543);

	//select 3 box
	SMenu[MN_SELECT_CHARACTER].nField[6].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_SELECT_CHARACTER].nField[6].nSpecialWillDo= SWD_CHARACTER_SELECT;
	SMenu[MN_SELECT_CHARACTER].nField[6].nWillDo= 2;
	SMenu[MN_SELECT_CHARACTER].nField[6].x=841;
	SMenu[MN_SELECT_CHARACTER].nField[6].y=85;
	SMenu[MN_SELECT_CHARACTER].nField[6].nRectImage= 30;
	SMenu[MN_SELECT_CHARACTER].nField[6].nImageNumber= 30;
	SMenu[MN_SELECT_CHARACTER].nField[6].nImageType= START_ETC;
	::SetRect(SMenu[MN_SELECT_CHARACTER].nField[6].rCheakBox,841,85,1003,309);

	//select 4 box
	SMenu[MN_SELECT_CHARACTER].nField[7].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_SELECT_CHARACTER].nField[7].nSpecialWillDo= SWD_CHARACTER_SELECT;
	SMenu[MN_SELECT_CHARACTER].nField[7].nWillDo= 3;
	SMenu[MN_SELECT_CHARACTER].nField[7].x=841;
	SMenu[MN_SELECT_CHARACTER].nField[7].y=319;
	SMenu[MN_SELECT_CHARACTER].nField[7].nRectImage= 30;
	SMenu[MN_SELECT_CHARACTER].nField[7].nImageNumber= 30;
	SMenu[MN_SELECT_CHARACTER].nField[7].nImageType= START_ETC;
	::SetRect(SMenu[MN_SELECT_CHARACTER].nField[7].rCheakBox,841,319,1003,543);

	//1 box - name field
	SMenu[MN_SELECT_CHARACTER].nField[8].nType=FT_DATA_PUT;
	SMenu[MN_SELECT_CHARACTER].nField[8].nWillDo= 0;
	SMenu[MN_SELECT_CHARACTER].nField[8].x=60;
	SMenu[MN_SELECT_CHARACTER].nField[8].y=53;
	SMenu[MN_SELECT_CHARACTER].nField[8].nImageNumber= DP_NAME_SELECT;

	//2 box - name field
	SMenu[MN_SELECT_CHARACTER].nField[9].nType=FT_DATA_PUT;
	SMenu[MN_SELECT_CHARACTER].nField[9].nWillDo= 1;
	SMenu[MN_SELECT_CHARACTER].nField[9].x=60;
	SMenu[MN_SELECT_CHARACTER].nField[9].y=286;
	SMenu[MN_SELECT_CHARACTER].nField[9].nImageNumber= DP_NAME_SELECT;
	
	//3 box - name field
	SMenu[MN_SELECT_CHARACTER].nField[10].nType=FT_DATA_PUT;
	SMenu[MN_SELECT_CHARACTER].nField[10].nWillDo= 2;
	SMenu[MN_SELECT_CHARACTER].nField[10].x=617;
	SMenu[MN_SELECT_CHARACTER].nField[10].y=53;
	SMenu[MN_SELECT_CHARACTER].nField[10].nImageNumber= DP_NAME_SELECT;

	//4 box - name field
	SMenu[MN_SELECT_CHARACTER].nField[11].nType=FT_DATA_PUT;
	SMenu[MN_SELECT_CHARACTER].nField[11].nWillDo= 3;
	SMenu[MN_SELECT_CHARACTER].nField[11].x=617;
	SMenu[MN_SELECT_CHARACTER].nField[11].y=286;
	SMenu[MN_SELECT_CHARACTER].nField[11].nImageNumber= DP_NAME_SELECT;

	}
	{// ###
	///////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////�ε�ȭ��///////////////////////////////////////////////////////
	// �ε��κп��� �������ٰ� �ö󰡰� �� ���� ���� �������̽� ������� �Ѿ��.
		::SMenuReg(MN_LODING, 0, 0, CONNECTING_IMG,0,2);
/*
		SMenu[MN_LODING].x=0;
		SMenu[MN_LODING].y=0;
	//	SMenu[MN_LODING].nImageNumber=210;
		SMenu[MN_LODING].nImageNumber=20;
		SMenu[MN_LODING].nImageType=START_ETC;
		SMenu[MN_LODING].nFieldCount=2;
		SMenu[MN_LODING].CheakType=0;
*/
		::SMenuFTReg(MN_LODING,0,FT_GAUGE,0, 183+240, 84+60,0, 0, CONNECTING_IMG, 1, 16);
/*
		SMenu[MN_LODING].nField[0].nType=FT_GAUGE;
		SMenu[MN_LODING].nField[0].nSpecialWillDo = 391+GABX_SCREEN;
*/
		SMenu[MN_LODING].nField[1].nType=FT_LODING_LIGHT;
		SMenu[MN_LODING].nField[1].x=255+GABX_SCREEN;
		SMenu[MN_LODING].nField[1].y=290+GABY_SCREEN-7;
		SMenu[MN_LODING].nField[1].nRectImage=930;		// ������ ȭ��
		SMenu[MN_LODING].nField[1].nImageNumber=906;
		SMenu[MN_LODING].nField[1].nSHideNomalCount=906;
	}
	{
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ���� ���� ��� ����â
	::SMenuReg(MN_CREDIT, 0, 0, SHOW_CREDIT_IMG,0,2);
/*
	SMenu[MN_CREDIT].x=0;
	SMenu[MN_CREDIT].y=0;
	SMenu[MN_CREDIT].nImageNumber=29;
	SMenu[MN_CREDIT].nImageType=START_ETC;
	SMenu[MN_CREDIT].nFieldCount=2;
*/
/*	SMenu[MN_CREDIT].nField[0].nType = FT_NOMAL_PUT;
	SMenu[MN_CREDIT].nField[0].x=258;
	SMenu[MN_CREDIT].nField[0].y=106;
	SMenu[MN_CREDIT].nField[0].nImageNumber = 28;
	SMenu[MN_CREDIT].nField[0].nImageType = START_ETC;
*/
	SMenu[MN_CREDIT].nField[1].nType = FT_CREDIT_PUT;
	SMenu[MN_CREDIT].nField[1].nWillDo = 288;
	SMenu[MN_CREDIT].nField[1].x=234;
	SMenu[MN_CREDIT].nField[1].y=180;
	SMenu[MN_CREDIT].nField[1].nRectImage= 288;
	}
	StartMapSelectMenuSet(NW_BY);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	{
// ���� ���� �޴�
	::SMenuReg(MN_SELECT_SERVER, 293, 224, SELECT_SERVER,0,8);
	SMenu[MN_SELECT_SERVER].work = CheckSelectServer();//020828 lsw ���� ��Ʈ ����
	//293 224
	::SMenuFTReg(MN_SELECT_SERVER,0,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT,SWD_SMALL_MENU,
						74, 194,100,30, SELECT_SERVER, 6,7, MN_SYSTEM_LOGOUT);
	
//	::SMenuFTReg(MN_SELECT_SERVER,0,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT,SWD_SMALL_MENU,
//						74+293, 194+224,100,30, SELECT_SERVER, 6,7, MN_SYSTEM_LOGOUT);

	//��ũ�� ��ư ��		// work ���� key���� �̿��Ѵ�.
	SMenu[MN_SELECT_SERVER].nField[1].nType=FT_SELECT_SERVER_SCROLL;
	SMenu[MN_SELECT_SERVER].nField[1].nWillDo=SCROLL_UP;
	SMenu[MN_SELECT_SERVER].nField[1].x=197;
	SMenu[MN_SELECT_SERVER].nField[1].y=29;
	SMenu[MN_SELECT_SERVER].nField[1].nSpecialWillDo = 1;		// ���� ���� ��ư
	SMenu[MN_SELECT_SERVER].nField[1].nRectImage=4;
	SMenu[MN_SELECT_SERVER].nField[1].nImageNumber=5;
	SMenu[MN_SELECT_SERVER].nField[1].nImageType=SELECT_SERVER;
	::SetRect(SMenu[MN_SELECT_SERVER].nField[1].rCheakBox,197,29, 216,58);

	//��ũ�� ��ư �Ʒ�
	SMenu[MN_SELECT_SERVER].nField[2].nType=FT_SELECT_SERVER_SCROLL;
	SMenu[MN_SELECT_SERVER].nField[2].nWillDo=SCROLL_DOWN;
	SMenu[MN_SELECT_SERVER].nField[2].x=197;
	SMenu[MN_SELECT_SERVER].nField[2].y=166;
	SMenu[MN_SELECT_SERVER].nField[2].nSpecialWillDo = 8;		// ���� ���� ��ư
	SMenu[MN_SELECT_SERVER].nField[2].nRectImage=2;
	SMenu[MN_SELECT_SERVER].nField[2].nImageNumber=3;
	SMenu[MN_SELECT_SERVER].nField[2].nImageType=SELECT_SERVER;
	::SetRect(SMenu[MN_SELECT_SERVER].nField[2].rCheakBox,197,166,216,195);

	int y = 10;
    // ���� ����Ʈ //1
	SMenu[MN_SELECT_SERVER].nField[3].nType=FT_SELECT_SERVER_BUTTON;
	SMenu[MN_SELECT_SERVER].nField[3].nWillDo=0;
	SMenu[MN_SELECT_SERVER].nField[3].x=111;
	SMenu[MN_SELECT_SERVER].nField[3].y=44+y;
	::SetRect(SMenu[MN_SELECT_SERVER].nField[3].rCheakBox,29,44+y,179,64+y);//020828 lsw
	
	// 2
	SMenu[MN_SELECT_SERVER].nField[4].nType=FT_SELECT_SERVER_BUTTON;
	SMenu[MN_SELECT_SERVER].nField[4].nWillDo=1;
	SMenu[MN_SELECT_SERVER].nField[4].x=111;
	SMenu[MN_SELECT_SERVER].nField[4].y=69+y;
	::SetRect(SMenu[MN_SELECT_SERVER].nField[4].rCheakBox,29,69+y,179,89+y);//020828 lsw

	// 3
	SMenu[MN_SELECT_SERVER].nField[5].nType=FT_SELECT_SERVER_BUTTON;
	SMenu[MN_SELECT_SERVER].nField[5].nWillDo=2;
	SMenu[MN_SELECT_SERVER].nField[5].x=111;
	SMenu[MN_SELECT_SERVER].nField[5].y=94+y;
	::SetRect(SMenu[MN_SELECT_SERVER].nField[5].rCheakBox,29,94+y,179,114+y);//020828 lsw

	// 4
	SMenu[MN_SELECT_SERVER].nField[6].nType=FT_SELECT_SERVER_BUTTON;
	SMenu[MN_SELECT_SERVER].nField[6].nWillDo=3;
	SMenu[MN_SELECT_SERVER].nField[6].x=111;
	SMenu[MN_SELECT_SERVER].nField[6].y=119+y;
	::SetRect(SMenu[MN_SELECT_SERVER].nField[6].rCheakBox,29,119+y,179,139+y);//020828 lsw

	// 5
	SMenu[MN_SELECT_SERVER].nField[7].nType=FT_SELECT_SERVER_BUTTON;
	SMenu[MN_SELECT_SERVER].nField[7].nWillDo=4;
	SMenu[MN_SELECT_SERVER].nField[7].x=111;
	SMenu[MN_SELECT_SERVER].nField[7].y=144+y;
	::SetRect(SMenu[MN_SELECT_SERVER].nField[7].rCheakBox,29,144+y,179,164+y);//020828 lsw
	}	

	{//020802 lsw 
//	::SMenuReg(MN_TIME_DISPLAY_MENU, 600, 452, MAININTERFACE_IMG,79,8);	//300 ¥�� �������̽� �̴ϱ�//�����̶� 0 0 �̴�
	::SMenuReg(MN_DEL_CHAR_COMFORM, -1, 40, DEL_CHAR_COMFORM_IMG,0,8);	//300 ¥�� �������̽� �̴ϱ�//�����̶� 0 0 �̴�
	::SMenuFTReg(MN_DEL_CHAR_COMFORM,1,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_DELETE_CHARATER,52, 298, 80, 30, DEL_CHAR_COMFORM_IMG, 1,2);
	::SMenuFTReg(MN_DEL_CHAR_COMFORM,2,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_DELETE_CHARATER_CLOSE,188, 298, 80, 30, DEL_CHAR_COMFORM_IMG, 3,4);
	::SMenuFTReg(MN_DEL_CHAR_COMFORM,3,FT_INPUT_DEL_CHAR_COMFORM_KEY_VIEW	, 0, 81, 241, 0, 0, 0, 0,0);
	}
	{//020815-2 lsw 
	::SMenuReg(MN_SELECT_TUTORIAL_LOGIN, -1, 100, SELECT_TUTORIAL_LOGIN_IMG,0,8);	//300 ¥�� �������̽� �̴ϱ�//�����̶� 0 0 �̴�
		

#ifdef TAIWAN_LOCALIZING_
	::SMenuFTReg(MN_SELECT_TUTORIAL_LOGIN,1,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SELECT_TUTORIAL_OK,18	, 138, 80, 30,	SELECT_TUTORIAL_LOGIN_IMG, 4,3);////OK	�ش� ��ư ����.//���� ������ �� ��ư�� ���� �մϴ�//1���� �Ժη� �ٲ��� ����
	::SMenuFTReg(MN_SELECT_TUTORIAL_LOGIN,2,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_RE_LOGON,101, 138, 80, 30,	SELECT_TUTORIAL_LOGIN_IMG, 7,6,MN_SELECT_CHARACTER);//Cancel ĳ�� ����ȭ������ �̵�

	::SMenuFTReg(MN_SELECT_TUTORIAL_LOGIN,3,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SET_LOGIN_TARGET, 31, 60 , 130, 25,	SELECT_TUTORIAL_LOGIN_IMG, 1,1,SC_SCHOLIUM2);//�ʺ��ڻ����(�ʺ��� ����ͷ� �̵�
	::SMenuFTReg(MN_SELECT_TUTORIAL_LOGIN,4,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SET_LOGIN_TARGET, 31, 91, 130, 25,	SELECT_TUTORIAL_LOGIN_IMG, 1,1,-1);//������ ����(�޴� �ݴ°� ymenuFocus �� ���� ��� �Ѵ�//����7 �̻�
#else
	::SMenuFTReg(MN_SELECT_TUTORIAL_LOGIN,1,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SELECT_TUTORIAL_OK,18	, 162, 80, 30,	SELECT_TUTORIAL_LOGIN_IMG, 4,3);////OK	�ش� ��ư ����.//���� ������ �� ��ư�� ���� �մϴ�//1���� �Ժη� �ٲ��� ����
	::SMenuFTReg(MN_SELECT_TUTORIAL_LOGIN,2,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_RE_LOGON,101, 162, 80, 30,	SELECT_TUTORIAL_LOGIN_IMG, 7,6,MN_SELECT_CHARACTER);//Cancel ĳ�� ����ȭ������ �̵�

	//::SMenuFTReg(MN_SELECT_TUTORIAL_LOGIN,3,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SET_LOGIN_TARGET, 31, 57 , 130, 25,	SELECT_TUTORIAL_LOGIN_IMG, 1,1,SC_SCHOLIUM);//�Թ� ���̵�(Ʃ�丮�� ������ �̵�
	::SMenuFTReg(MN_SELECT_TUTORIAL_LOGIN,4,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SET_LOGIN_TARGET, 31, 86 , 130, 25,	SELECT_TUTORIAL_LOGIN_IMG, 1,1,SC_SCHOLIUM2);//�ʺ��ڻ����(�ʺ��� ����ͷ� �̵�
	::SMenuFTReg(MN_SELECT_TUTORIAL_LOGIN,5,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SET_LOGIN_TARGET, 31, 117, 130, 25,	SELECT_TUTORIAL_LOGIN_IMG, 1,1,-1);//������ ����(�޴� �ݴ°� ymenuFocus �� ���� ��� �Ѵ�//����7 �̻�
#endif
	}
}
  
extern const int MAX_START;
extern int ClosemenuWhenLogin[];
void StartMenuSubProcessType2(SMENU *SubMenu,int i, int j,int k)
{
	switch(SubMenu->nField[j].nType)  //������Ұ� � Ÿ������?
	{
	case FT_GAUGE:
		{
			const int iResult = LoadingGameData();

			switch( iResult )
			{
			case -1 :
			case -2 :
				{
					::CallLoginMenu();
					// SMenu[MN_SELECT_CHARACTER].bActive=true;
					SMenu[MN_LODING].bActive=false;
					CallOkCancelMessageBox(  MN_LODING, 0,0, lan->OutputMessage(6,58), 0 );//lsw
					return;
				}break;
			case -3 :	
				{
					::CallLoginMenu();
					// SMenu[MN_SELECT_CHARACTER].bActive=true;
					SMenu[MN_LODING].bActive=false;
					CallOkCancelMessageBox(  MN_LODING, 0,0, lan->OutputMessage(6,59), 0 );//  lsw
					return;
				}break;
			case LGDS_END:
				{
					for(int x=0;x<MAX_START;x++) SMenu[ClosemenuWhenLogin[x]].bActive=FALSE;		//��� â�� �ݴ´�.
					SMenu[MN_MAININTERFACE].bActive=TRUE;
                    //SetChatMode(CM_COMMAND);
				}break;
			}

			const int iMax = SubMenu->nField[j].nImageNumber;
			const int iImageNo = (iResult*iMax) /LGDS_END ;

			FieldTypeNomalPut(	SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y,
								SubMenu->nField[j].nRectImage+iImageNo,SubMenu->nField[j].nImageType);
		}break;	
	case FT_ANIMATION2 : //020515 lsw 
		{
			break;//�켱 �۵� ����
			const int iAniMaxNo	= SubMenu->nField[j].nSpecialWillDo;
			const int iImageNo	= ++SubMenu->nField[j].nImageNumber;
			if(iImageNo >= iAniMaxNo)
			{
				SubMenu->nField[j].nImageNumber =0;
			}
			FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x,SubMenu->nField[j].y,SubMenu->nField[j].nImageNumber,SubMenu->nField[j].nImageType);//���ϸ��̼� ���
		}break;
	case FT_CREDIT_PUT : 
		{
			bool go = false;
			//if(g_aCurrentKeys[DIK_ESCAPE] & 0x80 ) go = true;
			static bool count = false;
			if( LButtonDown ) 
			{
				count = true;
			}
			else if( count ) go = true;

			if( go )
			{
				count = false;
				//g_aCurrentKeys[DIK_ESCAPE] = 0;
				SubMenu->bActive = false;
				::CallLoginMenu();
			}
		 }break;
	case FT_LODING_LIGHT :	
		{
/*			static bool turn = false;
			static bool img_count = true;
			int &img = SubMenu->nField[j].nSHideNomalCount;
			int &img2 = SubMenu->nField[j].nSHideNomalStart;

			if( img >= SubMenu->nField[j].nRectImage ) 
			{
				img = SubMenu->nField[j].nRectImage;
				turn = true;
			}
			else if( img <= SubMenu->nField[j].nImageNumber ) 
			{
				img = SubMenu->nField[j].nImageNumber;
				turn = false;
			}

			FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, img);
			if( img_count ) 
			{
				if( turn )	img--;
				else img++;
				
				img2++;
			}
//			img_count= !img_count;

			int kia_start = 10022;		// ���� ��� ��ȣ
//			int kia_start2 = 10027;
			FieldTypeNomalPut(0, 0, 387+GABX_SCREEN, 0+GABY_SCREEN, kia_start + img2%5);
//			FieldTypeNomalPut(0, 0, 162+GABX_SCREEN, 0+GABY_SCREEN, kia_start2 + img2%5);
			
///*
			if( img >= 906 && img < 911) SubMenu->nField[j-1].nImageNumber = 211;
			else if( img >= 911 && img < 915) SubMenu->nField[j-1].nImageNumber = 468;
			else if( img >= 915 && img < 920) SubMenu->nField[j-1].nImageNumber = 469;
			else if( img >= 920 && img < 924) SubMenu->nField[j-1].nImageNumber = 470;
			else if( img >= 924 && img < 929) SubMenu->nField[j-1].nImageNumber = 471;
			else if( img >= 929 && img < 934) SubMenu->nField[j-1].nImageNumber = 472;
* /			else SubMenu->nField[j-1].nImageNumber = 473;
			
		*/}	break;	

		case FT_INPUT_DEL_CHAR_COMFORM_KEY_VIEW://020820 lsw
		{
			char szSecretKeyCode[51] = {0,};//020820 lsw
			char szSecretKeyCodeTemp[51] = {0,};//020901 lsw
			EWndMgr.GetTxt( HWND_1, szSecretKeyCode, 51 );//020820 lsw

			static int cursor = 0 ;
			Hcolor( FONT_COLOR_WHITE );
			
			if(EWndMgr.IsFocus(HWND_1))//Ŀ���� ���⼭ ���
			{
				const int iPassLen = strlen(szSecretKeyCode);//��ǥ ó��
			//	for(int temp=0; temp < iPassLen; temp++) 
				const int iDisplayLen = (iPassLen>24)?24:iPassLen;
				for(int temp=0; iDisplayLen > temp; temp++) //24�ڸ� ����//021014 lsw
				{
					szSecretKeyCodeTemp[temp]='*'; 
					szSecretKeyCodeTemp[temp+1] = '\0';
				}
				if((cursor%=12) < 6){Hprint2( SubMenu->x+SubMenu->nField[j].x+2 , SubMenu->y+SubMenu->nField[j].y+4+2, g_DestBackBuf, "%s_",szSecretKeyCodeTemp );}
				else {	Hprint2( SubMenu->x+SubMenu->nField[j].x+2 , SubMenu->y+SubMenu->nField[j].y+4+2, g_DestBackBuf, "%s",szSecretKeyCodeTemp );}
			}
			cursor++;
			//���� ������ ���̴� ��ƾ,
			//��Ŀ�� ������ �ٽ� �������� ��ƾ
		}break;
	}
	return;
}

void MenuSet2() // ʵ带 ߰Ҷ ׾  üũ  ʾƾ  ʵ IsNoCheckFieldByMain() Լ ߰Ѵ.
{
	{
		int AddVert = 4; //  . //soto-1.02 4
		int AddHorz = 6; //  . //soto-1.02 6

		//::SMenuReg(MN_ITEM, 480, 0, INVENTORY_IMG,0,25,0);
		::SMenuReg(MN_ITEM, 480, 0, INVENTORY_IMG, 0, 26, 0); // soto-040411
		//ڽ ݱ
		::SMenuFTReg(MN_ITEM, 1, FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_RMENUCLOSE, 238 + AddHorz, 319 + AddVert, 70, 30, INVENTORY_IMG, 6, 7);
		//κ丮â ũ   ʵ
		//PAGE_NUMBER__  2 Դϴ 2 ʵ  
		::SMenuFTReg(MN_ITEM, PAGE_NUMBER__, FT_HIDE_NOMAL_GROUP_PUT, 0, 0 + AddHorz, 0 + AddVert, 0, 0, 0, 0, 0);
		// SMenu[MN_ITEM].nField[2].nType=FT_HIDE_NOMAL_GROUP_PUT;
		SMenu[MN_ITEM].nField[PAGE_NUMBER__].nSHideNomalNumber = HN_INVENTORY;
		SMenu[MN_ITEM].nField[PAGE_NUMBER__].nSHideNomalCount = 3;
		SMenu[MN_ITEM].nField[PAGE_NUMBER__].nSHideNomalStart = 0;
		SMenu[MN_ITEM].nField[PAGE_NUMBER__].nShideNomalPlus = 1;

		// 콺 ̺Ʈ üũϴ κ丮 ʵ
		::SMenuFTReg(MN_ITEM, 3, FT_INVENTORY, 0, 22 + AddHorz, 200 + AddVert, 35 * 8, 35 * 3, 0, 0);

		::SMenuFTReg(MN_ITEM, 4, FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_BANK2_ITEM, 7 + AddHorz, 319 + AddVert, 70, 30, INVENTORY_IMG, 3, 4);
		//κ丮â ũѿ ʵ
		::SMenuFTReg(MN_ITEM, 5, FT_HIDE_ISCROLL_AUTO_PUT, 0, 162 + AddHorz, 320 + AddVert, 40, 30, INVENTORY_IMG, 9, 10, 2);
		//κ丮â ũѿ ʵ
		::SMenuFTReg(MN_ITEM, 6, FT_HIDE_DSCROLL_AUTO_PUT, 0, 126 + AddHorz, 320 + AddVert, 40, 30, INVENTORY_IMG, 12, 13, 2);

		//  ȭ // SMenu[MN_ITEM].nField[7].nImageNumber=120; //menu.cpp ̹ ٲٴ ƾ .
		::SMenuFTReg(MN_ITEM, 7, FT_NOMAL_PUT, 0, 126 + AddHorz, 42 + AddVert, 0, 0, INVENTORY_IMG, 1, 1, 0);

		//   ȭ鿡  ϱ //nWillDo   Ǵϱ  . 8~15   ϴ κ ֽϴ.
		::SMenuFTReg(MN_ITEM, 8, FT_ARMOR_INVENTORY, WEAR_RIGHT_HAND, 152 + AddHorz, 64 + AddVert, 40, 40, INVENTORY_IMG, 0, 0, WT_WEAPON);
		SMenu[MN_ITEM].nField[8].nImageNumber = 865;
		::SMenuFTReg(MN_ITEM, 9, FT_ARMOR_INVENTORY, WEAR_HEAD, 255 + AddHorz, 66 + AddVert, 40, 40, INVENTORY_IMG, 0, 0, WT_HELMET);
		SMenu[MN_ITEM].nField[9].nImageNumber = 865;
		::SMenuFTReg(MN_ITEM, 10, FT_ARMOR_INVENTORY, WEAR_BODY, 208 + AddHorz, 109 + AddVert, 40, 40, INVENTORY_IMG, 0, 0, WT_ARMOR);
		SMenu[MN_ITEM].nField[10].nImageNumber = 865;
		::SMenuFTReg(MN_ITEM, 11, FT_ARMOR_INVENTORY, WEAR_LEFT_HAND, 257 + AddHorz, 152 + AddVert, 40, 40, INVENTORY_IMG, 0, 0, WT_SHIELD);
		SMenu[MN_ITEM].nField[11].nImageNumber = 865;
		::SMenuFTReg(MN_ITEM, 12, FT_ARMOR_INVENTORY, WEAR_UNIQUE, 27 + AddHorz, 67 + AddVert, 40, 40, INVENTORY_IMG, 0, 0, WT_UNIQUE1);
		SMenu[MN_ITEM].nField[12].nImageNumber = 865;
		::SMenuFTReg(MN_ITEM, 13, FT_ARMOR_INVENTORY, WEAR_UNIQUE, 63 + AddHorz, 67 + AddVert, 40, 40, INVENTORY_IMG, 0, 0, WT_UNIQUE2);
		SMenu[MN_ITEM].nField[13].nImageNumber = 865;
		::SMenuFTReg(MN_ITEM, 14, FT_ARMOR_INVENTORY, WEAR_NECK, 20 + AddHorz, 127 + AddVert, 40, 40, INVENTORY_IMG, 0, 0, WT_NECK);
		SMenu[MN_ITEM].nField[14].nImageNumber = 865;
		::SMenuFTReg(MN_ITEM, 15, FT_ARMOR_INVENTORY, WEAR_SHOES, 67 + AddHorz, 127 + AddVert, 40, 40, INVENTORY_IMG, 0, 0, WT_SHOES);
		SMenu[MN_ITEM].nField[15].nImageNumber = 865;
		::SMenuFTReg(MN_ITEM, 16, FT_DATA_PUT, 0, 152 + AddHorz, 64 + AddVert, 40, 40, INVENTORY_IMG, 0, DP_ARMOR_INVENTORY, WT_WEAPON);
		::SMenuFTReg(MN_ITEM, 17, FT_DATA_PUT, 0, 255 + AddHorz, 66 + AddVert, 40, 40, INVENTORY_IMG, 0, DP_ARMOR_INVENTORY, WT_HELMET);
		::SMenuFTReg(MN_ITEM, 18, FT_DATA_PUT, 0, 208 + AddHorz, 109 + AddVert, 40, 40, INVENTORY_IMG, 0, DP_ARMOR_INVENTORY, WT_ARMOR);
		::SMenuFTReg(MN_ITEM, 19, FT_DATA_PUT, 0, 257 + AddHorz, 152 + AddVert, 40, 40, INVENTORY_IMG, 0, DP_ARMOR_INVENTORY, WT_SHIELD);
		::SMenuFTReg(MN_ITEM, 20, FT_DATA_PUT, 0, 27 + AddHorz, 67 + AddVert, 40, 40, INVENTORY_IMG, 0, DP_ARMOR_INVENTORY, WT_UNIQUE1);
		::SMenuFTReg(MN_ITEM, 21, FT_DATA_PUT, 0, 63 + AddHorz, 67 + AddVert, 40, 40, INVENTORY_IMG, 0, DP_ARMOR_INVENTORY, WT_UNIQUE2);
		::SMenuFTReg(MN_ITEM, 22, FT_DATA_PUT, 0, 20 + AddHorz, 127 + AddVert, 40, 40, INVENTORY_IMG, 0, DP_ARMOR_INVENTORY, WT_NECK);
		::SMenuFTReg(MN_ITEM, 23, FT_DATA_PUT, 0, 67 + AddHorz, 127 + AddVert, 40, 40, INVENTORY_IMG, 0, DP_ARMOR_INVENTORY, WT_SHOES);

		//654,170 , 480 =
		::SMenuFTReg(MN_ITEM, 24, FT_NONE, 0, 134 + 10, 140 + 2, 40, 40, 0, 0, 0, 0); //soto-030513
		//̰ ǥ 
		/* 켱 Ƶд
		::SMenuFTReg(MN_ITEM, 24, FT_CANDLE_INVENTORY, WEAR_SHOES, 138, 134, 40,40,INVENTORY_IMG,0,0,WT_SHOES);
		SMenu[MN_ITEM].nField[24].nImageNumber=865;
		::SMenuFTReg(MN_ITEM, 25, FT_DATA_PUT, 0, 138, 134, 40,40,INVENTORY_IMG,0,DP_ARMOR_INVENTORY,WT_SHOES);
		*/
	}
	//----------------------------------------------------------------------------------------------------
	//ͽ ޴
	{
		::SMenuReg(MN_STATUS, 0, 0, STATUS_IMG, 0, 27, 0);
		// thai2 YGI
		::SMenuFTReg(MN_STATUS, 0, FT_NOMAL_PUT, 0, 4, 323, 75, 23, STATUS_IMG, 17, 17); // CSD-030324 :  ׸  °
		::SMenuFTReg(MN_STATUS, 4, FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_MAININTERFACE_ABILITY, 4, 323, 75, 23, STATUS_IMG, 18, 19); // CSD-030324
		// ƽ ޴ θ
		::SMenuFTReg(MN_STATUS, 23, FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_STATUS_TACTICS, 82, 323, 75, 23, STATUS_IMG, 8, 9);
		//ڽ ݱ
		::SMenuFTReg(MN_STATUS, 1, FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_LMENUCLOSE, 238, 323, 75, 23, STATUS_IMG, 14, 15);
		//161 32 ɼ ư//ý
		::SMenuFTReg(MN_STATUS, 26, FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_MENU_OPEN, 160, 323, 75, 23, STATUS_IMG, 11, 12, MN_SYSTEM);
		//ĳ 
		::SMenuFTReg(MN_STATUS, 12, FT_DATA_PUT, 0, 16, 44, 0, 0, STATUS_IMG, 0, DP_FACEIMAGE);
		//ĳ ̸ 
		::SMenuFTReg(MN_STATUS, 5, FT_DATA_PUT, 0, 140, 54, 0, 0, STATUS_IMG, 0, DP_NAME_CENTER);
		//Ŭ Ÿ 
		::SMenuFTReg(MN_STATUS, 2, FT_DATA_PUT, 0, 260, 54, 0, 0, STATUS_IMG, 0, DP_CLASS);
		// Ÿ 
		::SMenuFTReg(MN_STATUS, 3, FT_DATA_PUT, 0, 90, 135, 0, 0, STATUS_IMG, 0, DP_JOB);
		// ǥϱ // -> ٲ
		::SMenuFTReg(MN_STATUS, 13, FT_DATA_PUT, 0, 224, 135, 0, 0, STATUS_IMG, 0, DP_MONEY);
		//ġ Ÿ  
		// ::SMenuFTReg(MN_STATUS,4,FT_DATA_PUT,0,191,95,0,0,STATUS_IMG,0,DP_ARIGEMENT);
		//ĳ  
		::SMenuFTReg(MN_STATUS, 8, FT_DATA_PUT, 0, 96, 85, 0, 0, STATUS_IMG, 0, DP_LEVEL);
		//ĳ ġ 
		::SMenuFTReg(MN_STATUS, 9, FT_DATA_PUT, 0, 118, 86, 0, 0, STATUS_IMG, 0, DP_EXP);
		// ġ 
		::SMenuFTReg(MN_STATUS, 22, FT_DATA_PUT, 0, 209, 86, 0, 0, STATUS_IMG, 0, DP_EXP_NEXT);
		//ĳ HP life
		::SMenuFTReg(MN_STATUS, 10, FT_DATA_PUT, 0, 75, 172, 0, 0, STATUS_IMG, 0, DP_HP);
		//  
		::SMenuFTReg(MN_STATUS, 21, FT_DATA_PUT, 0, 237, 172, 0, 0, STATUS_IMG, 0, DP_FAME);
		//ĳ MP mana
		::SMenuFTReg(MN_STATUS, 11, FT_DATA_PUT, 0, 75, 198, 0, 0, STATUS_IMG, 0, DP_MP);
		//BEAT 
		::SMenuFTReg(MN_STATUS, 19, FT_DATA_PUT, 0, 237, 198, 0, 0, STATUS_IMG, 0, DP_HEAT);
		//HEALTH 
		::SMenuFTReg(MN_STATUS, 17, FT_DATA_PUT, 0, 75, 224, 0, 0, STATUS_IMG, 0, DP_HEALTH);
		//AC 
		::SMenuFTReg(MN_STATUS, 20, FT_DATA_PUT, 0, 237, 224, 0, 0, STATUS_IMG, 0, DP_AC);
		//TACTICE 
		::SMenuFTReg(MN_STATUS, 15, FT_DATA_PUT, 0, 75, 250, 0, 0, STATUS_IMG, 0, DP_TACTICE);
		//DAMAGE  //wsps  ٲ ( dp_wsps )
		::SMenuFTReg(MN_STATUS, 14, FT_DATA_PUT, 0, 237, 250, 0, 0, STATUS_IMG, 0, DP_DAMAGE);
		// ϴ Ű žӽ 
		// ::SMenuFTReg(MN_STATUS,24,FT_BELIEVE_GOD,0,90,285,0,0,MAIN_ETC,0,1);//020515 lsw
		//MOV  // ..
		// ::SMenuFTReg(MN_STATUS,18,FT_DATA_PUT,0,223,207,0,0,STATUS_IMG,0,DP_MOV);
		// 90 285
		// 226 285
	}
	//----------------------------------------------------------------------------------------------------
	//Ƽ ޴
	{
		::SMenuReg(MN_ABILITY, 0, 0, STATUS_IMG, 1, 39, 0);
		//ͽ ޴ ٿ
		//4 82 238
		::SMenuFTReg(MN_ABILITY, 0, FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_MAININTERFACE_ABILITYTOSTATUS, 4, 323, 75, 23, STATUS_IMG, 5, 6);
		// ƽ ޴ θ
		::SMenuFTReg(MN_ABILITY, 2, FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_STATUS_TACTICS, 82, 323, 75, 23, STATUS_IMG, 8, 9);
		//DONE
		::SMenuFTReg(MN_ABILITY, 1, FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_LMENUCLOSE, 238, 323, 75, 23, STATUS_IMG, 14, 15);
		//ý ޴ 
		::SMenuFTReg(MN_ABILITY, 38, FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_MENU_OPEN, 160, 323, 75, 23, STATUS_IMG, 11, 12, MN_SYSTEM);
		::SMenuFTReg(MN_ABILITY, 3, FT_DATA_PUT, 0, 253, 97+25, 75, 23, STATUS_IMG, 0, DP_ABILITY, MOR);
		::SMenuFTReg(MN_ABILITY, 4, FT_DATA_PUT, 0, 97, 98, 0, 0, STATUS_IMG, 0, DP_ABILITY, STR);
		::SMenuFTReg(MN_ABILITY, 5, FT_DATA_PUT, 0, 97, 98 + 25, 0, 0, STATUS_IMG, 0, DP_ABILITY, DEX);
		::SMenuFTReg(MN_ABILITY, 6, FT_DATA_PUT, 0, 97, 98 + 25 * 2, 0, 0, STATUS_IMG, 0, DP_ABILITY, CON);
		::SMenuFTReg(MN_ABILITY, 7, FT_DATA_PUT, 0, 97, 98 + 25 * 3, 0, 0, STATUS_IMG, 0, DP_ABILITY, WIS);
		::SMenuFTReg(MN_ABILITY, 8, FT_DATA_PUT, 0, 97, 98 + 25 * 4, 0, 0, STATUS_IMG, 0, DP_ABILITY, INT_);
		::SMenuFTReg(MN_ABILITY, 9, FT_DATA_PUT, 0, 97, 98 + 25 * 5, 0, 0, STATUS_IMG, 0, DP_ABILITY, MOVP);
		::SMenuFTReg(MN_ABILITY, 10, FT_DATA_PUT, 0, 97, 98 + 25 * 6, 0, 0, STATUS_IMG, 0, DP_ABILITY, ENDU);
		::SMenuFTReg(MN_ABILITY, 11, FT_DATA_PUT, 0, 97, 98 + 25 * 7, 0, 0, STATUS_IMG, 0, DP_ABILITY, CHA);
		::SMenuFTReg(MN_ABILITY, 12, FT_DATA_PUT, 0, 97, 98 + 25 * 8, 0, 0, STATUS_IMG, 0, DP_ABILITY, LUCK);
		::SMenuFTReg(MN_ABILITY, 22, FT_DATA_PUT, 0, 253, 97, 75, 23, STATUS_IMG, 0, DP_ABILITY, WSPS);

		::SMenuFTReg(MN_ABILITY, 26 + 0, FT_NO_CHECK, 20, 135, 94 + (0) * 25, 30, 15, STATUS_IMG, 21, 22, STR);
		::SMenuFTReg(MN_ABILITY, 26 + 1, FT_NO_CHECK, 20, 135, 94 + (1) * 25, 30, 15, STATUS_IMG, 21, 22, DEX);
		::SMenuFTReg(MN_ABILITY, 26 + 2, FT_NO_CHECK, 20, 135, 94 + (2) * 25, 30, 15, STATUS_IMG, 21, 22, CON);
		::SMenuFTReg(MN_ABILITY, 26 + 3, FT_NO_CHECK, 20, 135, 94 + (3) * 25, 30, 15, STATUS_IMG, 21, 22, WIS);
		::SMenuFTReg(MN_ABILITY, 26 + 4, FT_NO_CHECK, 20, 135, 94 + (4) * 25, 30, 15, STATUS_IMG, 21, 22, INT_);
		::SMenuFTReg(MN_ABILITY, 26 + 5, FT_NO_CHECK, 20, 135, 94 + (5) * 25, 30, 15, STATUS_IMG, 21, 22, MOVP);
		::SMenuFTReg(MN_ABILITY, 26 + 6, FT_NO_CHECK, 20, 135, 94 + (6) * 25, 30, 15, STATUS_IMG, 21, 22, ENDU);
		::SMenuFTReg(MN_ABILITY, 26 + 7, FT_NO_CHECK, 20, 135, 94 + (7) * 25, 30, 15, STATUS_IMG, 21, 22, CHA);
		::SMenuFTReg(MN_ABILITY, 26 + 8, FT_NO_CHECK, 20, 135, 94 + (8) * 25, 30, 15, STATUS_IMG, 21, 22, LUCK);
		::SMenuFTReg(MN_ABILITY, 26 + 9, FT_NO_CHECK, 20, 289, 94 + (0) * 25, 30, 15, STATUS_IMG, 21, 22, WSPS);
		::SMenuFTReg(MN_ABILITY, 26 + 10, FT_NO_CHECK, 20, 289, 94 + (1) * 25, 30, 15, STATUS_IMG, 21, 22, MOR);
		// ::SMenuFTReg(MN_ABILITY,12, FT_DATA_PUT,0,134,312,75,23,STATUS_IMG,0,DP_ABILITY,MOR);

		//̸ 
		::SMenuFTReg(MN_ABILITY, 14, FT_DATA_PUT, 0, 105, 47, 0, 0, STATUS_IMG, 0, DP_NAME_CENTER);
		//Ŭ Ÿ 
		::SMenuFTReg(MN_ABILITY, 15, FT_DATA_PUT, 0, 245, 47, 0, 0, STATUS_IMG, 0, DP_CLASS);
		// 
		::SMenuFTReg(MN_ABILITY, 16, FT_DATA_PUT, 0, 97, 72, 0, 0, STATUS_IMG, 0, DP_LEVEL);
		//ĳ ġ 
		::SMenuFTReg(MN_ABILITY, 17, FT_DATA_PUT, 0, 118, 72, 0, 0, STATUS_IMG, 0, DP_EXP);
		::SMenuFTReg(MN_ABILITY, 18, FT_DATA_PUT, 0, 209, 72, 0, 0, STATUS_IMG, 0, DP_EXP_NEXT);

		// ڵ ƮĿ  ...// ֹ,  ֹ WS PS ׸ °   ֽϴ. SMenu[MN_ABILITY].nField[21]  ã ˴ϴ
		::SMenuFTReg(MN_ABILITY, 21, FT_NO_CHECK, 0, 166, 93, 0, 0, STATUS_IMG, 24, 24);
		//׷ 
		::SMenuFTReg(MN_ABILITY, 24, FT_DATA_PUT, 0, 250, 136, 0, 0, STATUS_IMG, 24, DP_RESIST);
		// SMenu[MN_ABILITY].nField[24].nSHideNomalCount=4;
		// SMenu[MN_ABILITY].nField[24].nSHideNomalStart=0;
		// SMenu[MN_ABILITY].nField[24].nShideNomalPlus=1;

		::SMenuFTReg(MN_ABILITY, 25, FT_DATA_PUT, 0, 243, 296, 0, 0, STATUS_IMG, 23, DP_DATAPUT);
		SMenu[MN_ABILITY].nField[25].nValue = &SCharacterData.LvUpPoint;
	}
	{
		//----------------------------------------------------------------------------------------------------
		// ƽ ޴ ٿ
		::SMenuReg(MN_TACTICS, 0, 0, STATUS_IMG, 2, 10, 0);
		//ͽ ޴ ٿ
		//4 82 238
		::SMenuFTReg(MN_TACTICS, 0, FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_TACTICS_STATUS, 4, 323, 75, 23, STATUS_IMG, 5, 6);
		//Ƽ ޴ ٿ
		::SMenuFTReg(MN_TACTICS, 1, FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_TACTICS_ABILITY, 82, 323, 75, 23, STATUS_IMG, 18, 19);
		//DONE
		::SMenuFTReg(MN_TACTICS, 2, FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_LMENUCLOSE, 238, 323, 75, 23, STATUS_IMG, 14, 15);

		::SMenuFTReg(MN_TACTICS, 3, FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_MENU_OPEN, 160, 323, 75, 23, STATUS_IMG, 11, 12, MN_SYSTEM);
		//15 259
		::SMenuFTReg(MN_TACTICS, 4, FT_DATA_PUT, 0, 137, 57, 0, 0, STATUS_IMG, 0, DP_TACTICS_DATA, 39);

		::SMenuFTReg(MN_TACTICS, 6, FT_EXPLAIN_MENU, 0, 114, 21, 75, 23, STATUS_IMG, 0, 0, EXPLAIN_TACTICS_MAN); // ڴ EXPLAIN_TACTICS_WOMAN;//init ޴
		//SMenu[MN_TACTICS].nField[6//ݸ  ̶ ׸ ////15 260 //15 236//::SMenuFTReg(MN_TACTICS,6 ã 
		::SMenuFTReg(MN_TACTICS, 5, FT_NONE, 0, 137, 57, 0, 0, STATUS_IMG, 25, 25, 0);
	}
	{
		//----------------------------------------------------------------------------------------------------
		// Ƽ ޴
		::SMenuReg(MN_PARTY, 480, 0, PARTY_IMG, 0, 15, 0);
		// RELEASE button
		::SMenuFTReg(MN_PARTY, 0, FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_PARTY_RELEASE, 236, 195, 80, 30, PARTY_IMG, 11, 12, 12);
		//ڽ ݱ
		::SMenuFTReg(MN_PARTY, 1, FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_RMENUCLOSE, 236, 298, 80, 30, PARTY_IMG, 14, 15);
		::SMenuFTReg(MN_PARTY, 13, FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_PARTY_ALL_DEL, 236, 246, 80, 30, PARTY_IMG, 17, 18);
		// RELATION ư 
		::SMenuFTReg(MN_PARTY, 2, FT_PUT_BUTTON, SWD_PARTY_BUTTON, 18, 42, 60, 25, PARTY_IMG, 2, 3, PS_EXP);
		SMenu[MN_PARTY].nField[2].nSHideNomalNumber = 12;
		SMenu[MN_PARTY].nField[2].nSHideNomalCount = 2; //ҹư
		// PARTY ư 
		::SMenuFTReg(MN_PARTY, 3, FT_PUT_BUTTON, SWD_PARTY_BUTTON, 117, 42, 60, 25, PARTY_IMG, 5, 6, PS_TACTIC);
		SMenu[MN_PARTY].nField[3].nSHideNomalNumber = 12;
		SMenu[MN_PARTY].nField[3].nSHideNomalCount = 5; //ҹư

		// EMPROYMENT ư 
		::SMenuFTReg(MN_PARTY, 4, FT_PUT_BUTTON, SWD_PARTY_BUTTON, 216, 42, 60, 25, PARTY_IMG, 8, 9, PS_MAGIC_TAC);
		SMenu[MN_PARTY].nField[4].nSHideNomalNumber = 12;
		SMenu[MN_PARTY].nField[4].nSHideNomalCount = 8; //ҹư
		//ٸ Ƽ  :
		//  ̸   ʵ ڿ  ´. 1
		// ٸ ĳͿ 谡 ִٸ nSpecialWillDo üũ(true) д // origin ǥ ̹ ´.
		//1
		::SMenuFTReg(MN_PARTY, 6, FT_PARTY, 12, 55, 217, 0, 0, 0, 468, 468, 0);
		::SetRect(SMenu[MN_PARTY].nField[6].rCheakBox, 25, 182, 85, 252); // üũ Ÿ ٸ
		// 2
		::SMenuFTReg(MN_PARTY, 7, FT_PARTY, 12, 124, 217, 0, 0, 0, 469, 469, 1);
		::SetRect(SMenu[MN_PARTY].nField[7].rCheakBox, 94, 182, 154, 252);
		// 3
		::SMenuFTReg(MN_PARTY, 8, FT_PARTY, 12, 193, 217, 0, 0, 0, 470, 470, 2);
		::SetRect(SMenu[MN_PARTY].nField[8].rCheakBox, 163, 182, 233, 252);
		//4
		::SMenuFTReg(MN_PARTY, 9, FT_PARTY, 12, 55, 297, 0, 0, 0, 471, 471, 3);
		::SetRect(SMenu[MN_PARTY].nField[9].rCheakBox, 25, 262, 85, 332);
		//5
		::SMenuFTReg(MN_PARTY, 10, FT_PARTY, 12, 124, 297, 0, 0, 0, 472, 472, 4);
		::SetRect(SMenu[MN_PARTY].nField[10].rCheakBox, 94, 262, 154, 332);
		//6
		::SMenuFTReg(MN_PARTY, 11, FT_PARTY, 12, 193, 297, 0, 0, 0, 473, 473, 5);
		::SetRect(SMenu[MN_PARTY].nField[11].rCheakBox, 163, 262, 233, 332);
		//   Ŭ, 󱼰 ų ֱ
		::SMenuFTReg(MN_PARTY, 12, FT_PARTY_FACE, 0, 80, 124, 0, 0, 0, 0, 0, 0);
	}

	//----------------------------------------------------------------------------------------------------
	// ̽ ޴
	{
	SMenuReg(MN_MAININTERFACE, (SCREEN_WIDTH / 2) - 400, SCREEN_HEIGHT - 110, MAININTERFACE_IMG, 0, 50, 0);


		// ޴ ٿ OK
		SMenu[MN_MAININTERFACE].bActive = TRUE;
		//ͽ ޴ ٿ OK
		::SMenuFTReg(MN_MAININTERFACE, STATUS_FIELD, FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_MAININTERFACE_STATUS, 194, 67, 40, 40, MAININTERFACE_IMG, 30, 31);
		//SP    OK
		::SMenuFTReg(MN_MAININTERFACE, 1, FT_DATA_PUT, 0, 68, 41, 0, 0, MAININTERFACE_IMG, 5, DP_HEALTH_GAUGE);
		//̸Ƽ
		::SMenuFTReg(MN_MAININTERFACE, 2, FT_DO, 0, 66, 12, 30, 20, MAININTERFACE_IMG, 3, 4, DO_FACE_MENU);
		//ǥ ׸ ٿ
		::SMenuFTReg(MN_MAININTERFACE, 3, FT_DATA_PUT, 0, 66, 12, 30, 20, MAININTERFACE_IMG, 4, DP_FACE, 0);
		//HP ٿ OK
		::SMenuFTReg(MN_MAININTERFACE, 4, FT_DATA_PUT, 0, 0, 2, 0, 0, HP_IMG, 2, DP_HP_BAR);
		//MP ٿ OK
		::SMenuFTReg(MN_MAININTERFACE, 5, FT_DATA_PUT, 0, 723, 2, 0, 0, MANA_IMG, 2, DP_MP_BAR); //MANA_IMG DIVINE_IMG ڵ
		//̸ ٿ
		::SMenuFTReg(MN_MAININTERFACE, 6, FT_DATA_PUT, 0, 267, 46, 0, 0, 0, 0, DP_NAME_CENTER);
		//ġ 
		::SMenuFTReg(MN_MAININTERFACE, 7, FT_DATA_PUT, 0, 397, 46, 0, 0, MAININTERFACE_IMG, 28, DP_MAIN_LEVEL);
		// ٿ
		::SMenuFTReg(MN_MAININTERFACE, 8, FT_DATA_PUT, 0, 350, 46, 0, 0, MAININTERFACE_IMG, 28, DP_LEVEL_IN_TIME_MENU);
		//콺 üũϴ   ޴ ٿ
		::SMenuFTReg(MN_MAININTERFACE, 11, FT_ACCEL_INVENTORY, 0, 461, 69, 33 * 5, 35, 0, 0, 0);
		// MAP
		::SMenuFTReg(MN_MAININTERFACE, MAP_FIELD, FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_INFO_FLAG_SET, 673, 83, 66, 23, MAININTERFACE_IMG, 53, 54, IF_MAP);
		//TodayNotice ư
		::SMenuFTReg(MN_MAININTERFACE, TODAY_NOTICE_FIELD, FT_TODAY_NOTICE, 0, 674, 28, 33, 33, MAININTERFACE_IMG, 55, 56, 0);
		// battle ٲ ư  //acer7
		::SMenuFTReg(MN_MAININTERFACE, PEACE_BATTLE_FIELD, FT_PEACE_BATTLE_PUT, 0, 673, 59, 66, 23, MAININTERFACE_IMG, 53, 54); //50 51 ǽ 59 60 Ʋ 62 63 
		//ų ޴ 
		::SMenuFTReg(MN_MAININTERFACE, COMBAT_SKILL_POINT_FIELD, FT_MAIN_MENU, SWD_CALL_COMBAT_SKILL_POINT, 369, 67, 40, 40, MAININTERFACE_IMG, 42, 43); //020515 lsw
		// ޴ 
		::SMenuFTReg(MN_MAININTERFACE, MAGIC_FIELD, FT_MAIN_MENU, SWD_MAININTERFACE_MAGIC, 326, 67, 40, 40, MAININTERFACE_IMG, 38, 40);
		// ޴ 
		::SMenuFTReg(MN_MAININTERFACE, INVENTORY_FIELD, FT_MAIN_MENU, SWD_MAININTERFACE_ITEM, 237, 67, 40, 40, MAININTERFACE_IMG, 33, 34);
		//Ƽ ޴ 
		::SMenuFTReg(MN_MAININTERFACE, PARTY_FIELD, FT_MAIN_MENU, SWD_MAININTERFACE_PARTY, 282, 67, 40, 40, MAININTERFACE_IMG, 36, 37);
		// ý ޴ 
		::SMenuFTReg(MN_MAININTERFACE, OPTION_FIELD, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, SWD_MENU_OPEN, 638, 75, 31, 31, MAININTERFACE_IMG, 47, 47, MN_SYSTEM);
		//ų ޴ 
		::SMenuFTReg(MN_MAININTERFACE, SKILL_FIELD, FT_MAIN_MENU, SWD_MAININTERFACE_SKILL, 414, 67, 40, 40, MAININTERFACE_IMG, 45, 46, MN_QUICK_SKILL);
		// ޴
		::SMenuFTReg(MN_MAININTERFACE, NATION_BBS_FIELD, FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SMALL_MENU, 701, 21, 40, 36, MAININTERFACE_IMG, 65, 66, MN_NATION_BATTLE_BBS);
		::SMenuFTReg(MN_MAININTERFACE, HOT_KEY_FIELD, FT_HOTKEY_ICON, 0, HOTKEY_ICON_POS_X, HOTKEY_ICON_POS_Y, 40, 40, COMBAT_SKILL_ICON_IMG, 90, 90, 0); //ų ִ  
		///////////////////// ä /////////////////////
		// Է¶
		::SMenuFTReg(MN_MAININTERFACE, 31, FT_DO, 0, 103, 4, 0, 0, MAININTERFACE_IMG, 67, 68, DO_CHATTING_EDITBOX);
		//  ޴ 
		::SMenuFTReg(MN_MAININTERFACE, MAIL_FIELD, FT_MAININTERFACE_MAIL_CALL, SWD_NONE,
			638, 42, 31, 31, MAININTERFACE_IMG, 48, 48, MVT_RECV_LIST);
		//ǹڽ
		::SMenuFTReg(MN_MAININTERFACE, 38, FT_POTION_BOX, 0, 790, -20, 0, 0, 0, 0, 0, 0);
		//ƽ ÷
		::SMenuFTReg(MN_MAININTERFACE, 39, FT_DISPLAY_MAGIC_EXP, 0, 66, 53, 30, 30, MAININTERFACE_IMG, 16, 27);
		::SMenuFTReg(MN_MAININTERFACE, 40, FT_DISPLAY_TAC_EXP, 0, 66, 72, 30, 30, MAININTERFACE_IMG, 16, 27);

		::SMenuFTReg(MN_MAININTERFACE, 41, FT_DISPLAY_SKILL_EXP, 0, 66, 91, 30, 30, MAININTERFACE_IMG, 16, 27);

		// Ӹ  â//42 MunuEdit  //ز ư
		::SMenuFTReg(MN_MAININTERFACE, 42, FT_SELECT_WISPER_TARGET, 0, 145, 6, 15, 15, MAININTERFACE_IMG, 76, 75, 0);

		//ä,  ä //SMenu[MN_MAININTERFACE].nField[44].nWillDo 44 ٲܶ   ..
		//nWillDo 0 ̸  , 1 ̸  //⼭ Ŀ  ù ư  /SMenu/ƮӸ ִ ư
		::SMenuFTReg(MN_MAININTERFACE, 44, FT_SELECT_CHAT_TYPE, 0, 609, 7, 30, 25, MAININTERFACE_IMG, 72, 71, 73);

		::SMenuFTReg(MN_MAININTERFACE, 45, FT_NOMAL_PUT, 0, 66, 52, 0, 0, MAININTERFACE_IMG, 96, 96); // ׸  °
		//ģ ޴
		::SMenuFTReg(MN_MAININTERFACE, FRIEND_FIELD, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, SWD_CALL_FRIEND_MENU, 100, 17, 30, 20, MAININTERFACE_IMG, 97, 97); //ģ ư

		//Lock Button nWillDo 0 --> UnLock 1 -> lock
		::SMenuFTReg(MN_MAININTERFACE, 47, FT_CHAT_LOCK, 0, 639, 7, 30, 35, MAININTERFACE_IMG, 69, 70, 1); //Lock Button//nWillDo 1 ̸ Lock 

		::SMenuFTReg(MN_MAININTERFACE, 9, FT_HUNTER_VERSUS_DISPLAY, 0, 639, -200, 30, 35, MAININTERFACE_IMG, 69, 70, 1); //021001 lsw
	}
	{
		//----------------------------------------------------------------------------------------------------
		//  ޴  ޴
		SMenu[MN_MAIN_EXPLAIN].x = 0;
		SMenu[MN_MAIN_EXPLAIN].y = 371 + GABY_SCREEN * 2;
		SMenu[MN_MAIN_EXPLAIN].CheakType = 0;
		SMenu[MN_MAIN_EXPLAIN].nFieldCount = 1;

		SMenu[MN_MAIN_EXPLAIN].nField[0].nType = FT_EXPLAIN_MENU;
		SMenu[MN_MAIN_EXPLAIN].nField[0].nWillDo = EXPLAIN_MAININTERFACE;
	}
	{
		//----------------------------------------------------------------------------------------------------
		// ǥ  ޴ ̸Ƽ ޴
		//θ޴ ٿ
		/* 031021  س ̸Ƽ
		::SMenuReg(MN_FACE, 98, 360+GABY_SCREEN*2, NEW_EMOTE_IMG,0,9,0);
		//ǥ
		::SMenuFTReg(MN_FACE,0,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,8+0, 8+0,30,30, NEW_EMOTE_IMG,2,2);
		::SMenuFTReg(MN_FACE,1,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,8+40, 8+0,30,30, NEW_EMOTE_IMG,3,3);
		::SMenuFTReg(MN_FACE,2,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,8+40*2,8+0,30,30, NEW_EMOTE_IMG,4,4);
		::SMenuFTReg(MN_FACE,3,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,8+40*3,8+0,30,30, NEW_EMOTE_IMG,5,5);
		::SMenuFTReg(MN_FACE,4,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,4+40*4,8+0,30,30, NEW_EMOTE_IMG,6,6);//̹
		::SMenuFTReg(MN_FACE,5,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,8+40*5,8+0,30,30, NEW_EMOTE_IMG,7,7);
		::SMenuFTReg(MN_FACE,6,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,8+40*6,8+0,30,30, NEW_EMOTE_IMG,8,8);
		::SMenuFTReg(MN_FACE,7,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,8+40*7,8+0,30,30, NEW_EMOTE_IMG,9,9);

		::SMenuFTReg(MN_FACE,8,FT_SPECIAL_WILLDO,SWD_UNMOUSEFOCUS,0,0,0,0,0,0,0);
		*/
		//<< 031021 kyo
		::SMenuReg(MN_FACE, 98, 260 + GABY_SCREEN * 2, NEW_EMOTE_IMG, 0, 34, 0);

		::SMenuFTReg(MN_FACE, 0, FT_NOMAL_PUT, SWD_NONE, -10, -1, 30, 30, NEW_EMOTE_IMG, 1, 1); // BBD 040413
		//<! BBD 040112 ̼ ̸Ƽ 
		int acer = 0;
#ifdef TAIWAN_LOCALIZING_
		acer = 1;
#endif
#ifdef HONGKONG_LOCALIZING_
		acer = 1;
#endif
#ifdef CHINA_LOCALIZING_
		acer = 1;
#endif

		if (acer) //̼ ̸Ƽ ޴ Ѹ ǥ ٸ
		{
			::SMenuFTReg(MN_FACE, 1, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, SWD_FACE_CHOOSE, 6 + 0, 8 + 0, 30, 30, NEW_EMOTE_IMG, 2, 2);
			::SMenuFTReg(MN_FACE, 2, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, SWD_FACE_CHOOSE, 6 + 41, 8 + 0, 30, 30, NEW_EMOTE_IMG, 3, 3);
			::SMenuFTReg(MN_FACE, 3, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, SWD_FACE_CHOOSE, 6 + 41 * 2, 8 + 0, 30, 30, NEW_EMOTE_IMG, 4, 4);
			::SMenuFTReg(MN_FACE, 4, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, SWD_FACE_CHOOSE, 6 + 41 * 3, 8 + 0, 30, 30, NEW_EMOTE_IMG, 5, 5);
			::SMenuFTReg(MN_FACE, 5, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, SWD_FACE_CHOOSE, 41 * 4, 8 + 0, 30, 30, NEW_EMOTE_IMG, 6, 6);
			::SMenuFTReg(MN_FACE, 6, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, SWD_FACE_CHOOSE, 6 + 41 * 5, 8 + 0, 30, 30, NEW_EMOTE_IMG, 7, 7);
			::SMenuFTReg(MN_FACE, 7, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, SWD_FACE_CHOOSE, 6 + 41 * 6, 8 + 0, 30, 30, NEW_EMOTE_IMG, 8, 8);
			::SMenuFTReg(MN_FACE, 8, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, SWD_FACE_CHOOSE, 6 + 41 * 7, 8 + 0, 30, 30, NEW_EMOTE_IMG, 9, 9);
		}
		else
		{
			::SMenuFTReg(MN_FACE, 1, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, SWD_FACE_CHOOSE, 2, 2, 30, 30, NEW_EMOTE_IMG, 2, 2);
			::SMenuFTReg(MN_FACE, 2, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, SWD_FACE_CHOOSE, 45, 3, 30, 30, NEW_EMOTE_IMG, 3, 3);
			::SMenuFTReg(MN_FACE, 3, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, SWD_FACE_CHOOSE, 85, 4, 30, 30, NEW_EMOTE_IMG, 4, 4);
			::SMenuFTReg(MN_FACE, 4, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, SWD_FACE_CHOOSE, 125, 4, 30, 30, NEW_EMOTE_IMG, 5, 5);
			::SMenuFTReg(MN_FACE, 5, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, SWD_FACE_CHOOSE, 165, 5, 30, 30, NEW_EMOTE_IMG, 6, 6);
			::SMenuFTReg(MN_FACE, 6, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, SWD_FACE_CHOOSE, 205, 3, 30, 30, NEW_EMOTE_IMG, 7, 7);
			::SMenuFTReg(MN_FACE, 7, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, SWD_FACE_CHOOSE, 250, 4, 30, 30, NEW_EMOTE_IMG, 8, 8);
			::SMenuFTReg(MN_FACE, 8, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, SWD_FACE_CHOOSE, 288, 5, 30, 30, NEW_EMOTE_IMG, 9, 9);
		}
		//> BBD 040112 ̼ ̸Ƽ 
		::SMenuFTReg(MN_FACE, 9, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, SWD_FACE_CHOOSE, 331, 3, 30, 30, NEW_EMOTE_IMG, 10, 10);
		::SMenuFTReg(MN_FACE, 10, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, SWD_FACE_CHOOSE, 370, 4, 30, 30, NEW_EMOTE_IMG, 11, 11);

		::SMenuFTReg(MN_FACE, 11, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, SWD_FACE_CHOOSE, 0, 45, 30, 30, NEW_EMOTE_IMG, 12, 12);
		::SMenuFTReg(MN_FACE, 12, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, SWD_FACE_CHOOSE, 45, 48, 30, 30, NEW_EMOTE_IMG, 13, 13);
		::SMenuFTReg(MN_FACE, 13, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, SWD_FACE_CHOOSE, 81, 43, 30, 30, NEW_EMOTE_IMG, 14, 14);
		::SMenuFTReg(MN_FACE, 14, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, SWD_FACE_CHOOSE, 124, 41, 30, 30, NEW_EMOTE_IMG, 15, 15);
		::SMenuFTReg(MN_FACE, 15, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, SWD_FACE_CHOOSE, 165, 45, 30, 30, NEW_EMOTE_IMG, 16, 16);
		::SMenuFTReg(MN_FACE, 16, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, SWD_FACE_CHOOSE, 205, 45, 30, 30, NEW_EMOTE_IMG, 17, 17);
		::SMenuFTReg(MN_FACE, 17, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, SWD_FACE_CHOOSE, 246, 48, 30, 30, NEW_EMOTE_IMG, 18, 18);
		::SMenuFTReg(MN_FACE, 18, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, SWD_FACE_CHOOSE, 286, 42, 30, 30, NEW_EMOTE_IMG, 19, 19);
		::SMenuFTReg(MN_FACE, 19, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, SWD_FACE_CHOOSE, 331, 44, 30, 30, NEW_EMOTE_IMG, 20, 20);
		::SMenuFTReg(MN_FACE, 20, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, SWD_FACE_CHOOSE, 367, 42, 30, 30, NEW_EMOTE_IMG, 21, 21);
		::SMenuFTReg(MN_FACE, 21, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, SWD_FACE_CHOOSE, 6, 90, 30, 30, NEW_EMOTE_IMG, 22, 22); // finito added the old emotions 030707
		::SMenuFTReg(MN_FACE, 22, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, SWD_FACE_CHOOSE, 47, 90, 30, 30, NEW_EMOTE_IMG, 23, 23);
		::SMenuFTReg(MN_FACE, 23, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, SWD_FACE_CHOOSE, 88, 90, 30, 30, NEW_EMOTE_IMG, 24, 24);
		::SMenuFTReg(MN_FACE, 24, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, SWD_FACE_CHOOSE, 129, 90, 30, 30, NEW_EMOTE_IMG, 25, 25);
		::SMenuFTReg(MN_FACE, 25, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, SWD_FACE_CHOOSE, 167, 90, 30, 30, NEW_EMOTE_IMG, 26, 26);
		::SMenuFTReg(MN_FACE, 26, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, SWD_FACE_CHOOSE, 211, 90, 30, 30, NEW_EMOTE_IMG, 27, 27);
		::SMenuFTReg(MN_FACE, 27, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, SWD_FACE_CHOOSE, 251, 90, 30, 30, NEW_EMOTE_IMG, 28, 28);
		::SMenuFTReg(MN_FACE, 28, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, SWD_FACE_CHOOSE, 293, 90, 30, 30, NEW_EMOTE_IMG, 29, 29);
		::SMenuFTReg(MN_FACE, 29, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, SWD_FACE_CHOOSE, 331, 87, 30, 30, NEW_EMOTE_IMG, 30, 30); //Eleval 22/06/09 - New Emotes
		::SMenuFTReg(MN_FACE, 30, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, SWD_FACE_CHOOSE, 370, 87, 30, 30, NEW_EMOTE_IMG, 31, 31);
		::SMenuFTReg(MN_FACE, 31, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, SWD_FACE_CHOOSE, 412, 5, 30, 30, NEW_EMOTE_IMG, 32, 32);
		::SMenuFTReg(MN_FACE, 32, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, SWD_FACE_CHOOSE, 412, 45, 30, 30, NEW_EMOTE_IMG, 33, 33);
		::SMenuFTReg(MN_FACE, 33, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, SWD_FACE_CHOOSE, 412, 88, 30, 30, NEW_EMOTE_IMG, 34, 34);
		//>> 031012 kyo
	}
	{
		//----------------------------------------------------------------------------------------------------
		//äŸ 
		//θ޴ ٿ
		::SMenuReg(MN_WISPER_LIST, (393), (608), 0, 0, 15, 0);
		//ǥ 
		::SMenuFTReg(MN_WISPER_LIST, CHAT_TARGET_NORMAL, FT_SELECT_WISPER, CHATTYPE_NORMAL, 8, 8 - 0, 130, 18, MAININTERFACE_IMG, 78, 77, CHAT_TARGET_NORMAL);
		strcpy(SMenu[MN_WISPER_LIST].nField[CHAT_TARGET_NORMAL].temp, lan->OutputMessage(2, 625)); //ä

		::SMenuFTReg(MN_WISPER_LIST, CHAT_TARGET_GUILD, FT_SELECT_WISPER, CHATTYPE_GUILD, 8, 8 - 18, 130, 18, MAININTERFACE_IMG, 78, 77, CHAT_TARGET_GUILD);
		strcpy(SMenu[MN_WISPER_LIST].nField[CHAT_TARGET_GUILD].temp, lan->OutputMessage(2, 626)); //ä

		::SMenuFTReg(MN_WISPER_LIST, CHAT_TARGET_PARTY, FT_SELECT_WISPER, CHATTYPE_PARTY, 8, 8 - 18 * 2, 130, 18, MAININTERFACE_IMG, 78, 77, CHAT_TARGET_PARTY);
		strcpy(SMenu[MN_WISPER_LIST].nField[CHAT_TARGET_PARTY].temp, lan->OutputMessage(2, 628)); //ä

		::SMenuFTReg(MN_WISPER_LIST, SELECT_WISPER_MODE, FT_SELECT_WISPER, CHATTYPE_WHISPER, 8, 8 - 18 * 3 , 130, 18, MAININTERFACE_IMG, 78, 77, SELECT_WISPER_MODE);
		strcpy(SMenu[MN_WISPER_LIST].nField[SELECT_WISPER_MODE].temp, lan->OutputMessage(2, 627)); //ӼӸ Է
		::SMenuFTReg(MN_WISPER_LIST, CHAT_TARGET_WISPER1, FT_SELECT_WISPER, CHATTYPE_WHISPER, 8 , 8 - 18 * 4 , 130, 18, MAININTERFACE_IMG, 78, 77, CHAT_TARGET_WISPER1);
		::SMenuFTReg(MN_WISPER_LIST, CHAT_TARGET_WISPER2, FT_SELECT_WISPER, CHATTYPE_WHISPER, 8 , 8 - 18 * 5 , 130, 18, MAININTERFACE_IMG, 78, 77, CHAT_TARGET_WISPER2);
		::SMenuFTReg(MN_WISPER_LIST, CHAT_TARGET_WISPER3, FT_SELECT_WISPER, CHATTYPE_WHISPER, 8 , 8 - 18 * 6 , 130, 18, MAININTERFACE_IMG, 78, 77, CHAT_TARGET_WISPER3);
		::SMenuFTReg(MN_WISPER_LIST, CHAT_TARGET_WISPER4, FT_SELECT_WISPER, CHATTYPE_WHISPER, 8 , 8 - 18 * 7 , 130, 18, MAININTERFACE_IMG, 78, 77, CHAT_TARGET_WISPER4);
		::SMenuFTReg(MN_WISPER_LIST, CHAT_TARGET_WISPER5, FT_SELECT_WISPER, CHATTYPE_WHISPER, 8 , 8 - 18 * 8 , 130, 18, MAININTERFACE_IMG, 78, 77, CHAT_TARGET_WISPER5);
		
	}

	// MN_DISPLAY_SKILL_EXP// 3 

	//----------------------------------------------------------------------------------------------------
	//̽ ý ޴
	{
		::SMenuReg(MN_SYSTEM, 215 + GABX_SCREEN+240, 57 + GABY_SCREEN+60, NEW_SYSTEM_IMG, 0, 7, 0);
		//޴ 
		/*
		::SMenuFTReg(MN_SYSTEM,1,FT_HIDE_NOMAL_GROUP_PUT,SWD_SMALLMENU_END,66,188,60,30,NEW_SYSTEM_IMG,1,1);
		SMenu[MN_SYSTEM].nField[1].nSHideNomalNumber=HN_SYSTEM_TEXT;
		SMenu[MN_SYSTEM].nField[1].nSHideNomalCount=3;
		SMenu[MN_SYSTEM].nField[1].nSHideNomalStart=0;
		SMenu[MN_SYSTEM].nField[1].nShideNomalPlus=3;
		*/
		// 
		::SMenuFTReg(MN_SYSTEM, 5, FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_MOVE_TO_HOMETOWN, 27, 46, 100, 25, NEW_SYSTEM_IMG, 1, 1, MN_SYSTEM_LOGOUT);
		//option 簢
		::SMenuFTReg(MN_SYSTEM, 2, FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_FOCUS_WILLDO, 27, 70, 100, 25, NEW_SYSTEM_IMG, 1, 1, MN_SYSTEM_OPTION);
		//help 簢
		::SMenuFTReg(MN_SYSTEM, 3, FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_HELP, 27, 94, 100, 25, NEW_SYSTEM_IMG, 1, 1);
		//logout 簢
		::SMenuFTReg(MN_SYSTEM, 4, FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_FOCUS_WILLDO, 27, 118, 100, 25, NEW_SYSTEM_IMG, 1, 1, MN_SYSTEM_LOGOUT);
		//exit
		::SMenuFTReg(MN_SYSTEM, 0, FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SMALLMENU_END, 49, 152, 60, 30, NEW_SYSTEM_IMG, 2, 3);
	}
	{
		::SMenuReg(MN_GHOST_TO_ALIVE, 215 + GABX_SCREEN, 57 + GABY_SCREEN, NEW_GHOST_TO_ALIVE_IMG, 0, 4, 0);
		::SMenuFTReg(MN_GHOST_TO_ALIVE, 0, FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_MOVE_TO_HOMETOWN_WITH_ALIVE, 22, 23, 153, 25, NEW_GHOST_TO_ALIVE_IMG, 1, 1);
		::SMenuFTReg(MN_GHOST_TO_ALIVE, 1, FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, 0, 22, 48, 153, 25, NEW_GHOST_TO_ALIVE_IMG, 1, 1);
	}
	{ //021001 lsw
		// ::SMenuReg(MN_CHATTING, 10, 355, 0,0,10,0);
		::SMenuReg(MN_CHATTING, 0, 321, NEW_CHAT_BACK_IMG, 0, 10, 0);
		//巡  //巡 ׻  ʵ忩 Ѵ
		::SMenuFTReg(MN_CHATTING, 9, FT_DRAG_ZONE, 0, 3, 5, 19, 20, NEW_CHAT_BACK_IMG, 1, 2);
		//ư
		::SMenuFTReg(MN_CHATTING, 1, FT_CHAT_VIEW_SET, CHATTYPE_NORMAL, 3, 24, 28, 20, NEW_CHAT_BACK_IMG, 3, 4, 1);
		::SMenuFTReg(MN_CHATTING, 2, FT_CHAT_VIEW_SET, CHATTYPE_WHISPER, 3, 45 + 4, 28, 20, NEW_CHAT_BACK_IMG, 5, 6, 1);
		::SMenuFTReg(MN_CHATTING, 3, FT_CHAT_VIEW_SET, CHATTYPE_GUILD, 3, 65 + 9, 28, 20, NEW_CHAT_BACK_IMG, 7, 8, 1);
		::SMenuFTReg(MN_CHATTING, 4, FT_CHAT_VIEW_SET, CHATTYPE_PARTY, 3, 85 + 14, 28, 20, NEW_CHAT_BACK_IMG, 13, 12, 1); // Finito for party chat icon an stuff
		//ȭ 
		::SMenuFTReg(MN_CHATTING, 6, FT_DO, 0, 22, -30, 0, 0, NEW_CHAT_BACK_IMG, 0, 0, DO_CHATTING_VIEW); // Finito for party chat icon an stuff
		// #define FT_CHAT_SCROLL 121 // äý ũ ư
	}
	//----------------------------------------------------------------------------------------------------
	//   ϰ ִ ޴
	{
		SMenu[MN_QUICK_MAGIC_PLUS].bActive = true;
		SMenu[MN_QUICK_MAGIC_PLUS].x = 200;
		SMenu[MN_QUICK_MAGIC_PLUS].y = SCREEN_HEIGHT-106; // 694 - 50;
		SMenu[MN_QUICK_MAGIC_PLUS].nFieldCount = 2;

		//   ߰ (    Ű )
		SMenu[MN_QUICK_MAGIC_PLUS].nField[0].nType = FT_QUICK_MAGIC_PLUS;
		SMenu[MN_QUICK_MAGIC_PLUS].nField[0].x = 0; // Ŀ  ٸ...
		SMenu[MN_QUICK_MAGIC_PLUS].nField[0].y = -54;
		SMenu[MN_QUICK_MAGIC_PLUS].nField[0].nImageType = MAIN_ETC;

		// Ŭ   ϸ   ̸ ֱ
		SMenu[MN_QUICK_MAGIC_PLUS].nField[1].nType = FT_USED_MAGIC_EXPLAIN;
		SMenu[MN_QUICK_MAGIC_PLUS].nField[1].x = 0; // Ŀ  ٸ...
		SMenu[MN_QUICK_MAGIC_PLUS].nField[1].y = -54;
		SMenu[MN_QUICK_MAGIC_PLUS].nField[1].nImageType = MAIN_ETC;

		//----------------------------------------------------------------------------------------------------
		//   ũѿ پ ִ ð ޴ Ű
		SMenu[MN_TIME_DISPLAY_ON_OFF].bActive = true;
		SMenu[MN_TIME_DISPLAY_ON_OFF].nImageNumber = 22;
		SMenu[MN_TIME_DISPLAY_ON_OFF].nImageType = MAIN_ETC;
		SMenu[MN_TIME_DISPLAY_ON_OFF].nFieldCount = 1;

		//   ߰ (    Ű )
		SMenu[MN_TIME_DISPLAY_ON_OFF].nField[0].nType = FT_TIME_DISPLAY_ON_OFF;
		SMenu[MN_TIME_DISPLAY_ON_OFF].nField[0].x = 5;
		SMenu[MN_TIME_DISPLAY_ON_OFF].nField[0].y = 3;
		SMenu[MN_TIME_DISPLAY_ON_OFF].nField[0].nImageNumber = 14;
		SMenu[MN_TIME_DISPLAY_ON_OFF].nField[0].nImageType = MAIN_ETC;
		::SetRect(SMenu[MN_TIME_DISPLAY_ON_OFF].nField[0].rCheakBox, 4, 0, 32, 30);

		//----------------------------------------------------------------------------------------------------
		//   迭 ũ ޴
		SMenu[MN_QUICK_MAGIC_SCROLL].x = 461; // 461;
		SMenu[MN_QUICK_MAGIC_SCROLL].y = SCREEN_HEIGHT-159; // 641;
		SMenu[MN_QUICK_MAGIC_SCROLL].nImageNumber = 21;
		SMenu[MN_QUICK_MAGIC_SCROLL].nImageType = MAIN_ETC;
		SMenu[MN_QUICK_MAGIC_SCROLL].nFieldCount = 2;

		SMenu[MN_QUICK_MAGIC_SCROLL].nField[0].nType = FT_QUICK_MAGIC_ARRAY_SCROLL;
		SMenu[MN_QUICK_MAGIC_SCROLL].nField[0].x = 5;
		SMenu[MN_QUICK_MAGIC_SCROLL].nField[0].y = 5;
		SMenu[MN_QUICK_MAGIC_SCROLL].nField[0].nWillDo = 0; // 0: up ư
		SMenu[MN_QUICK_MAGIC_SCROLL].nField[0].nSHideNomalCount = 5;
		SMenu[MN_QUICK_MAGIC_SCROLL].nField[0].nRectImage = 18;
		SMenu[MN_QUICK_MAGIC_SCROLL].nField[0].nImageNumber = 19;
		SMenu[MN_QUICK_MAGIC_SCROLL].nField[0].nImageType = MAIN_ETC;
		::SetRect(SMenu[MN_QUICK_MAGIC_SCROLL].nField[0].rCheakBox, 3, 3, 23, 20);

		SMenu[MN_QUICK_MAGIC_SCROLL].nField[1].nType = FT_QUICK_MAGIC_ARRAY_SCROLL;
		SMenu[MN_QUICK_MAGIC_SCROLL].nField[1].x = 5;
		SMenu[MN_QUICK_MAGIC_SCROLL].nField[1].y = 34;
		SMenu[MN_QUICK_MAGIC_SCROLL].nField[1].nWillDo = 1; // 1: down ư
		SMenu[MN_QUICK_MAGIC_SCROLL].nField[1].nSHideNomalCount = 5;
		SMenu[MN_QUICK_MAGIC_SCROLL].nField[1].nRectImage = 17;
		SMenu[MN_QUICK_MAGIC_SCROLL].nField[1].nImageNumber = 20;
		SMenu[MN_QUICK_MAGIC_SCROLL].nField[1].nImageType = MAIN_ETC;
		::SetRect(SMenu[MN_QUICK_MAGIC_SCROLL].nField[1].rCheakBox, 3, 32, 23, 49);
	}
	//----------------------------------------------------------------------------------------------------
	// ų ޴
	{ //yǥ äâ  ٲϴ.
		const int iMnIndex = MN_QUICK_SKILL;
		const int iImg = SKILL_MENU;
		int iFtNum = 0;

		::SMenuReg(iMnIndex, CENTER_X, 426, iImg, 0, 5);
		// ų  ȭ 
		::SMenuFTReg(iMnIndex, iFtNum++, FT_QUICK_SKILL_PUT, 3,
			46, 22, 35, 35, iImg, 0, 3);
		// scroll up -- 
		::SMenuFTReg(iMnIndex, iFtNum++, FT_QUICK_SKILL_SCROLL, 0,
			7, 10, 10, 15, iImg, 0, 1, 0);
		// scroll down -- 
		::SMenuFTReg(iMnIndex, iFtNum++, FT_QUICK_SKILL_SCROLL, 0,
			26, 10, 10, 15, iImg, 0, 2, 1);
		// scroll left -- 
		::SMenuFTReg(iMnIndex, iFtNum++, FT_QUICK_SKILL_SCROLL_LR, 0,
			0, 34, 24, 32, iImg, 0, 6, 0);
		// scroll right -- 
		::SMenuFTReg(iMnIndex, iFtNum++, FT_QUICK_SKILL_SCROLL_LR, 0,
			22, 34, 24, 32, iImg, 0, 9, 1);
	}
}

void lsw_DoRButtonDownOfMenu(const int i,const int j)//���ư Ŭ��
{
	switch(SMenu[i].nField[j].nType)
	{
  	case FT_CSD_MAIN_ICON://��Ŭ���� Ŭ����
		{ 
			::CSDMainIconClear();
		}break;
	  case FT_CSD_ICON:
		{
			switch(SMenu[i].nField[j].nSpecialWillDo)
			{
			case SWD_SC_ICON_DOING:
				{
					SetSCDMainAndOtherClear(SMenu[i].nField[1].nWillDo);
				}break;
			}
		}break;
	default :
		break;
	}
}

void lsw_DoLButtonDownOfMenu(const int i,const int j)
{	//��ư�� ������ ������ ȣ�� ��
	SMENU *SubMenu	= &SMenu[i];

	int &fRcMouse	= SubMenu->nField[j].fRectMouse;
	int &fLBDown	= SubMenu->nField[j].fLButtonDown;

	int &iImgType	= SubMenu->nField[j].nImageType;
	int &iRcImg		= SubMenu->nField[j].nRectImage;
	int &iImgNo		= SubMenu->nField[j].nImageNumber;

	int &iFieldX	= SubMenu->nField[j].x;
	int &iFieldY	= SubMenu->nField[j].y;
	
	int &iMainX		= SubMenu->x;
	int &iMainY		= SubMenu->y;
	int &iWork		= SubMenu->work;

	int &iSwd		= SubMenu->nField[j].nSpecialWillDo;

	int &nWillDo	=SubMenu->nField[j].nWillDo;
	const int iType = SubMenu->nField[j].nType;

	switch( iType )
	{
	case FT_ACCEL_INVENTORY :
		{
			const int ItemX = ( g_pointMouseX - (SMenu[i].x + SMenu[i].nField[j].x) ) / 33;
			if( 0 > ItemX || 5<= ItemX)
			{
				break;
			}
			if( g_MouseItemType )	
			{
				break;
			}
			//���� �θ��丮 â�� �������� ������ ��´�. 
			if( QuickItemAttr[ItemX].item_no )
			{
				if( IsExchangeNow() )
				{
					int a;
					for(a=0; a<MAX_EXCHANGE_ITEM; a++)
					{
						if( item_give[a].item.item_attr.item_no && item_give[a].item.item_pos.type == QUICK && item_give[a].item.item_pos.p3 == ItemX )
							break;
					}
					if( a != MAX_EXCHANGE_ITEM ) break;
				}

				ItemSoundOfGetItem( QuickItemAttr[ItemX].item_no );
				g_MouseItemType = 1;
				g_MouseItemNumber = QuickItemAttr[ItemX].item_no;
				HandItemAttr = QuickItemAttr[ItemX];
				DeleteItem( &QuickItemAttr[ItemX]);

				SetItemPos(QUICK, ItemX, &IP_base);
				POS pos_t;
				SetItemPos(HAND, &pos_t);
				SendMoveItem( HandItemAttr.item_no, IP_base, pos_t );
			}
		 }break;
	case FT_CANDLE_INVENTORY://����
		{
			const int ItemX = 5;
			if( g_MouseItemType )	
			{
				break;
			}
			//���� �θ��丮 â�� �������� ������ ��´�. 
			if( QuickItemAttr[ItemX].item_no )
			{
				if( IsExchangeNow() )
				{
					int a;
					for(a=0; a<MAX_EXCHANGE_ITEM; a++)
					{
						if( item_give[a].item.item_attr.item_no && item_give[a].item.item_pos.type == QUICK && item_give[a].item.item_pos.p3 == ItemX )
							break;
					}
					if( a != MAX_EXCHANGE_ITEM ) break;
				}

				ItemSoundOfGetItem( QuickItemAttr[ItemX].item_no );
				g_MouseItemType = 1;
				g_MouseItemNumber = QuickItemAttr[ItemX].item_no;
				HandItemAttr = QuickItemAttr[ItemX];
				DeleteItem( &QuickItemAttr[ItemX]);

				SetItemPos(QUICK, ItemX, &IP_base);
				POS pos_t;
				SetItemPos(HAND, &pos_t);
				SendMoveItem( HandItemAttr.item_no, IP_base, pos_t );
			}
		 }break;
	case FT_EXCHANGE_ITEM : 
		{
			if(!IsExchangeNow())
			{
				break;
			}
			int iSideInfo = SMenu[i].nField[j].nWillDo;//LEFT? OR RIGHT
			::DeleteItemAtExchangeGiveItemList(iSideInfo);
		}break;
	case FT_GM_2ND_SKILL_MAKE_ITEM:	
		{//011008 lsw  >�۾����� ������ ������ ������. �ƹ��͵� �������� ��ǥ���� ����
			if( g_MouseItemType ) 
			{//�տ� �� ��� �ִ�.
				break;
			}
			int a;
			for( a=0; a<MAX_UPGRADE_RARE; a++ )
			{//���� ��ȣ�� �ְ� ���콺 ���� üũ�� �Ǿ���
				if( gm_make_item_list[a].item_no 
				&&	MouseInRectCheak( SMenu[i].x, SMenu[i].y, gm_make_item_list[a].rect ) )
				{
					g_MouseItemType = 1;//���콺�� �������� �ٿ� �ش�
					g_MouseItemNumber = gm_make_item_list[a].item_no;
					HandItemAttr = gm_make_item_list[a].attr;//
					IP_base = gm_make_item_list[a].pos;

					POS pos_t;
					SetItemPos(HAND, &pos_t);
					SendMoveItem( HandItemAttr.item_no, IP_base, pos_t );
					memset( &gm_make_item_list[a], 0, sizeof( MakeItem ) );
					DeleteItem( &InvItemAttr[ IP_base.p1 ][ IP_base.p2 ][ IP_base.p3 ] );
					break;
				}
			}
			int temp =0;

			for( a=0; a<MAX_UPGRADE_RARE; a++ )
			{
				if(gm_make_item_list[a].item_no)
				{
					temp++;
				}
			}
			
			if(!temp)
			{
				ZeroMemory(&gm_make_item_result,sizeof(MakeItem));
				ZeroMemory(&Soksungattr_gm,sizeof(int)); 
			}
		}break;
	case FT_SKILL_MASTER_MAKE_ITEM://021111 lsw
		{
			if( g_MouseItemType ) 
			{//�տ� �� ��� �ִ�.
				break;
			}
			for( int a=0; a<2; a++ )
			{
				const ItemAttr Item = g_MultiRareSrc.MultiRareSrc[a].attr ;
				if( Item.item_no
				&& MouseInRectCheak( SMenu[i].x, SMenu[i].y, g_MultiRareSrc.MultiRareSrc[a].rect ) )
				{
					g_MouseItemType = 1;
					g_MouseItemNumber = Item.item_no;
					HandItemAttr = Item;
					IP_base = g_MultiRareSrc.MultiRareSrc[a].pos;

					POS pos_t;
					SetItemPos(HAND, &pos_t);
					SendMoveItem( HandItemAttr.item_no, IP_base, pos_t );
					memset( &g_MultiRareSrc.MultiRareSrc[a], 0, sizeof( MakeItem ) );
					DeleteItem( &InvItemAttr[ IP_base.p1 ][ IP_base.p2 ][ IP_base.p3 ] );
					break;
				}
			}
		}break;
	case FT_MERCHANT_SELL_ITEM_DROP_ZONE://�÷����� �ٽ� �ݴ� �κ�
		{//021126 lsw
			if( g_MouseItemType ) 
			{//�տ� �� ��� �ִ�.
				break;
			}
			if(fRcMouse)//���콺�� �� �϶�
			{
				g_MouseItemType = 1;//���콺�� �������� �ٿ� �ش�
				const ItemAttr *item = Auction.GetSellItemAttr();//������ Pos �ޱ�
				if(!item){break;}//�������� ������ �ȵ���.

				g_MouseItemNumber = item->item_no;
				HandItemAttr = *item;//
				IP_base = Auction.GetSellItemPos();

				POS pos_t;
				::SetItemPos(HAND, &pos_t);
				::SendMoveItem( HandItemAttr.item_no, IP_base, pos_t );
				Auction.ClearSellItem();
				::DeleteItem( &InvItemAttr[ IP_base.p1 ][ IP_base.p2 ][ IP_base.p3 ] );
			}//if(fRcMouse)//���콺�� �� �϶�
		}break;
	case FT_EXCHANGE_BOND_MONEY_ITEM:
		{
			if( g_MouseItemType ) 
			{//�տ� �� ��� �ִ�.
				break;
			}
			if(fRcMouse)//���콺�� �� �϶�
			{
				g_MouseItemType = 1;
				const ItemAttr *pItem = Auction.GetBondItemAttr();
				if(!pItem){break;}//�������� ������ �ȵ���.

				g_MouseItemNumber = pItem->item_no;
				HandItemAttr = *pItem;//
				IP_base = Auction.GetBondItemPos();

				POS pos_t;
				::SetItemPos(HAND, &pos_t);
				::SendMoveItem( HandItemAttr.item_no, IP_base, pos_t );
				Auction.ClearBondItem();
				::DeleteItem( &InvItemAttr[ IP_base.p1 ][ IP_base.p2 ][ IP_base.p3 ] );
			}
		}break;
	default:break;
	}
}

void MenuSubProcessType_FT_DO(SMENU *SubMenu, int j)//020515 lsw
{
	int &iFieldX = SubMenu->nField[j].x;
	int &iFieldY = SubMenu->nField[j].y;
	int &iMainX = SubMenu->x;
	int &iMainY = SubMenu->y;
	int &iImgNo =SubMenu->nField[j].nImageNumber;

	switch(SubMenu->nField[j].nWillDo)
	{
	case DO_CHATTING_VIEW :	
		{
			ChatMgr.DisplayData( iMainX+iFieldX , iMainY+iFieldY );//020702 lsw
		}break;
	case DO_CHATTING_EDITBOX :	
		{
			if( !IsChatBoxActive() ) //ä�� ��尡 �ƴϸ� �ֵ��� �ݰ� �����.
			{
				SetChatMode(CM_COMMAND);//��Ŀ���� ���� ��������
				CallWisperList(false);				
				EWndMgr.hideInputBox(HWND_CHAT_INPUT);
				EWndMgr.hideInputBox(HWND_CHAT_TARGET_INPUT);
				break;
			}
			else//ä�� ���� ���
			{
				SubMenu->nField[44].nType = FT_SELECT_CHAT_TYPE;
				SubMenu->nField[47].nType = FT_CHAT_LOCK;
				if (!EWndMgr.checkInputBoxVisibility(HWND_CHAT_INPUT)) EWndMgr.showInputBox(HWND_CHAT_INPUT);
				//char temp[MAX_CHAT_STRING_]={""};
				//char wisper_target[23]={""};
				//EWndMgr.GetTxt( HWND_CHAT_TARGET_INPUT, wisper_target, 20 );
				//EWndMgr.GetTxt( HWND_CHAT_INPUT, temp, 52 );//020530 lsw
				std::string wisper_target = EWndMgr.GetTxt(HWND_CHAT_TARGET_INPUT);
				
				/*if (!EWndMgr.IsFocus(HWND_CHAT_INPUT) &&
					!EWndMgr.IsFocus(HWND_CHAT_TARGET_INPUT) )
				{//�̻��ѵ��� ��Ŀ���� ������ �P��´� �⺻�� ä�� �Է�â����
					SetFocus2(HWND_CHAT_INPUT);	//021001 lsw					
				}*/
				//�̸�ĭ�� ���� ������ ��Ŀ���� �̸� �Է����� ��� ���´�

				FieldTypeNomalPutFx3(iMainX, iMainY, iFieldX, iFieldY, iImgNo, SubMenu->nField[j].nImageType, 18, 1);
				FieldTypeNomalPutFx3(iMainX + 58, iMainY + 4, iFieldX, iFieldY, SubMenu->nField[j].nRectImage, SubMenu->nField[j].nImageType, 15, 1);

				Hcolor(FONT_COLOR_WHITE);

				const int SendType = GetChatTarget();

				if (SendType >= 3)//wisper chat
				{
					if (wisper_target.empty())
					{
						EWndMgr.showInputBox(HWND_CHAT_TARGET_INPUT);
					}
				}
				else
				{
					//EWndMgr.hideInputBox(HWND_CHAT_TARGET_INPUT);
					Hprint2(iMainX + iFieldX + 58 + 2, iMainY + iFieldY + 4 + 2, g_DestBackBuf, "%s", SMenu[MN_WISPER_LIST].nField[SendType].temp);
				}


				if( isKeyDown(SDL_SCANCODE_ESCAPE) ) 
				{
					SetChatMode(CM_COMMAND);
				}
			}
		}break;
	default:
		break;
	}
}

void lsw_MenuSubProcessType(SMENU *SubMenu,const int j)
{
	int &fRcMouse	= SubMenu->nField[j].fRectMouse;
	int &fLBDown	= SubMenu->nField[j].fLButtonDown;

	int &iImgType	= SubMenu->nField[j].nImageType;
	int &iRcImg		= SubMenu->nField[j].nRectImage;
	int &iImgNo		= SubMenu->nField[j].nImageNumber;

	int &iFieldX	= SubMenu->nField[j].x;
	int &iFieldY	= SubMenu->nField[j].y;
	
	int &iMainX		= SubMenu->x;
	int &iMainY		= SubMenu->y;
	int &iWork		= SubMenu->work;

	int &iSwd		= SubMenu->nField[j].nSpecialWillDo;

	int &nWillDo	=SubMenu->nField[j].nWillDo;
	const int iType = SubMenu->nField[j].nType;
	switch(iType)
	{
	case	FT_HUNTER_VERSUS_DISPLAY:
	{	///////////////////////////////////////////////
		if(my_hunter_list) my_hunter_list->print2( 700, 50 );		// ����� ��� �����ֱ�
		///////////////////////// ������� ���� ���� ����
		if( ::IsRightWindowOpen() ) break;
		if( danger_by_hunter )
		{
			int color = danger_by_hunter % 2;
			if( color )
				Hcolor( FONT_COLOR_PLUS );
			else Hcolor( FONT_COLOR_NAME );
			
			Hprint2( 700, 50, g_DestBackBuf, "DANGER" );
			danger_by_hunter -- ;
			if( danger_by_hunter < 0 ) danger_by_hunter = 0;
			
		}
		static int bCheck = -1;
		bCheck++;
		///////////////////////////////////////////
		//  1:1 ���� ������ �˸���
		if( man_to_man_start )
		{
			if( man_to_man_start < 80 )
			{
				static int img_max = 0;
				if( !img_max )
				{
					Spr *temp_spr;
					int j;
					for( j=0; j<START_FIGHT_IMG_MAX; j++ )
					{
						temp_spr = GetSprOfMenu( START_FIGHT_MAP, j );
						if(!temp_spr || !temp_spr->img ) break;
					}
					img_max = j;
				}
				static int ct = 0;

				if( ct==0 ) MP3( SN_FIGHT_START_READY );
				if( ct==13 ) MP3( SN_FIGHT_START_FIGHT );

				Spr *s1 = GetSprOfMenu( START_FIGHT_MAP, ct );
				if( !s1 ) break;

				PutCompressedImageFX( SCREEN_WIDTH/2, 200, s1, 19, 2 );

				ct++;
				if( ct >= img_max ) 
				{
					man_to_man_start=0;
					ct=0;
					if( g_FightMapStart ) OpenFightMapSignMenuOpen( fight_map_live_man );		// ������ ����ֱ�..
				}
			}
			else
				man_to_man_start --;
		}

		if( fight_map_stop )
		{
			if( fight_map_stop < 80 )
			{
				static int ct = 0;
				if( ct==0 ) MP3( SN_FIGHT_STOP );
				Spr *s = GetSprOfMenu( STOP_FIGHT_MAP, ct );
				if( s && s->img )
				{
					PutCompressedImageFX( SCREEN_WIDTH/2, 200, s, 17, 2 );
					//PutCompressedImage( SCREEN_WIDTH/2, 200, s);//, 19, 2 );
					if( bCheck%2 )ct++;
				}
				else
				{
					fight_map_stop=0;
					ct=0;
					//OpenFightMapSignMenuOpen( fight_map_live_man );		// ������ ����ֱ�..
					MenuSoundClose( MN_FIGHT_MAP_SIGN );
				}
			}
			else fight_map_stop--;
		}
		else
		{
			static int animation=0;
			if( g_fight_win > 0 )
			{													
				static int ct = 0;
				if( ct==0 ) MP3( 2211 );
				if( ct == 44 ) MP3( 2204 );
				if( ct == 60  ) MP3( SN_FIGHT_WIN );

				Spr *s1 = GetSprOfMenu( FIGHT_MAP_WIN, ct );
				Spr *s2 = GetSprOfMenu( FIGHT_MAP_WIN, ct+1 );

				if( s1 && s2 && s1->img && s2->img )
				{
					PutCompressedImage( SCREEN_WIDTH/2, 200, s1 );
					PutCompressedImageFX( SCREEN_WIDTH/2, 200, s2, 17, 2 );

					static int flag = 1;
					if( ct > 10 )
					{
						if( animation < 4 )
						{
							if( ct==40 || ct==10 )
							{
								flag = !flag;
								animation++;
							}
							if( flag ) ct += 2;
							else ct -= 2;
						}
						else ct+=2;
					}
					else
					{
						flag = true;
						ct += 2;
						animation++;
					}
				}
				else
				{														
					g_fight_win=0;
					ct=0;
					animation = 0;
				}
			}
			if( g_fight_lose > 0 )
			{
				static int ct = 0;
				//if( ct==0 ) MP3( SN_FIGHT_LOSE );
				if( ct==22 || ct==32 ) MP3( 302 );
				if( ct == 70 ) MP3( 2203 );
				if( ct== 84 ) MP3( SN_FIGHT_LOSE );

				Spr *s1 = GetSprOfMenu( FIGHT_MAP_LOSE, ct );
				Spr *s2 = GetSprOfMenu( FIGHT_MAP_LOSE, ct+1 );

				if( !s1 || !s2 ) 
				{
					g_fight_lose=0;
					ct=0;
					break;
				}
				else
				{
					if( s1->img && s2->img )
					{
						PutCompressedImage( SCREEN_WIDTH/2, 300, s1 );
						PutCompressedImageFX( SCREEN_WIDTH/2, 300, s2, 17, 2 );
						if( bCheck%2 )ct+=2;
					}
					else
					{
						g_fight_lose=0;
						ct=0;
					}
				}
			}
		}
		if( g_RemainTimeDelay )
		{
			static int cn = 0;
			int div = (5-g_RemainTimeType-1)*10;		// �ð� ����Ʈ�� �Ѳ����� ���ε� �Ǿ� �ֱ� ������
			
			if( !(cn%10) ) MP3( SN_FIGHT_TIME );
			
			Spr *s1 = GetSprOfMenu( FIGHT_MAP_TIME, div+cn%10 );
			Spr *s2 = GetSprOfMenu( FIGHT_MAP_TIME, div+cn%10+1 );
			if( !s1 || !s2 ) 
			{
				break;
			}
			else
			{
				if( s1->img && s2->img )
				{
					PutCompressedImage( SCREEN_WIDTH/2+(SCREEN_WIDTH-640)/2, SCREEN_HEIGHT/2-(SCREEN_HEIGHT-480)/2, s1 );
					PutCompressedImageFX( SCREEN_WIDTH/2+(SCREEN_WIDTH-640)/2, SCREEN_HEIGHT/2-(SCREEN_HEIGHT-480)/2, s2, 17, 2 );
					if( bCheck%2 )cn+=2;
				}
			}
			g_RemainTimeDelay--;
			if( !g_RemainTimeDelay ) cn = 0;
		}
	}break;
case FT_RARE_GROUP_MEMBER_DISPLAY://020214 lsw
	{
		if(fRcMouse)//���콺 ���� �Ǿ�����
		{
			if(iImgNo)//�÷��� �� �̰� 
			{
				Hcolor( FONT_COLOR_RARE_MAIN );
				RectTextPut(iMainX+iFieldX,iMainY+iFieldY,iRcImg,SubMenu->nField[j].temp); 
				const int iX	= iMainX+iFieldX+100;
				const int iY	= iMainY+iFieldY-20;
				const int iRareIndex = iImgNo;
				DrawRareExplain(iX,iY,iRareIndex,SubMenu->key);
			}
		}
		else
		{
			Hcolor( FONT_COLOR_WHITE );
			RectTextPut(iMainX+iFieldX,iMainY+iFieldY,iRcImg,SubMenu->nField[j].temp); 
		}
	}break;
case 	FT_CS_ICON://�������� 3,4�� ����
	{
		if(fRcMouse)//���콺 ���� �Ǿ�����
		{
			FieldTypeNomalPut(iMainX, iMainY, iFieldX,iFieldY,iRcImg, iImgType);
		//	DrawCombatSkillExplain(Hero->x-Mapx+20,Hero->y-Mapy-80, nWillDo);
		//	DrawCombatSkillExplain(iMainX-8,325, nWillDo);
			DrawCombatSkillExplain(303,130, nWillDo);
		}
		else
		{
			FieldTypeNomalPut(iMainX, iMainY, iFieldX,iFieldY,iImgNo, iImgType);
		}
		const int iCSIndex = nWillDo;
		if(iCSIndex)
		{
			const int iSkillLv = CombatSkill[iCSIndex].iSkillLv;
			if(iSkillLv >0 )//�����̶�� ��´�
			{
				const int iLvImageNo = 79 + iSkillLv - 1;//79�� �̹��� ���� ����
				FieldTypeNomalPutFx3(iMainX, iMainY, iFieldX,iFieldY,iLvImageNo, iImgType,5,5);
				FieldTypeNomalPutFx3(iMainX, iMainY, iFieldX,iFieldY,iLvImageNo, iImgType,5,2);
			}
		}
		
	}break;
case 	FT_CSD_ICON://�������� 3,4�� ����
	{
		const int iCSIndex = nWillDo;
		if(iCSIndex)
		{
			if(fRcMouse)//���콺 ���� �Ǿ�����
			{
				//FieldTypeNomalPut(iMainX, iMainY, iFieldX,iFieldY,SkillIdToCombatSkillIconImage(CombatSkill[iCSIndex].iSkillNo,false), iImgType);
                DrawCombatSkillExplain(Hero->x-Mapx+20,Hero->y-Mapy-80, nWillDo);
			}
			else
			{

				FieldTypeNomalPut(iMainX, iMainY, iFieldX,iFieldY,SkillIdToCombatSkillIconImage(CombatSkill[iCSIndex].iSkillNo,true), iImgType);
			}
			const int iLvImageNo = 79 + CombatSkill[iCSIndex].iSkillLv - 1 ;//79�� �̹��� ���� ����
			FieldTypeNomalPutFx3(iMainX, iMainY, iFieldX,iFieldY,iLvImageNo, iImgType,5,5);
			FieldTypeNomalPutFx3(iMainX, iMainY, iFieldX,iFieldY,iLvImageNo, iImgType,5,2);
			DisplayQuickMemory(QM_COMBATSKILL,iCSIndex,iMainX+iFieldX+15,iMainY+iFieldY+15);
			if( fRcMouse )
			{
				SetQuickMemoryByKeyInput(QM_COMBATSKILL,iCSIndex);
			}
		}
	}break;
case 	FT_CSD_MAIN_ICON:
	{
	 FieldTypeNomalPutFx3(iMainX, iMainY, iFieldX+3,iFieldY+3,72, iImgType,5,5);//����Ʈ�� ����ִ� ��ư
	 FieldTypeNomalPutFx3(iMainX, iMainY, iFieldX,iFieldY,86, iImgType,5,5);//����Ʈ�� ����ִ� ��ư
	 FieldTypeNomalPut(iMainX, iMainY, iFieldX,iFieldY,86, iImgType);//����Ʈ�� ����ִ� ��ư			
		const int iCSIndex = nWillDo;
		if(iCSIndex)
		{
			const int iImageNo = SkillIdToCombatSkillIconImage(CombatSkill[iCSIndex].iSkillNo,false);
			FieldTypeNomalPut(iMainX, iMainY, iFieldX,iFieldY,iImageNo, iImgType);
			{//���� ��� ��
			const int iLvImageNo = 79 + CombatSkill[iCSIndex].iSkillLv - 1 ;//79�� �̹��� ���� ����
			FieldTypeNomalPutFx3(iMainX, iMainY, iFieldX,iFieldY,iLvImageNo, iImgType,5,5);
			FieldTypeNomalPutFx3(iMainX, iMainY, iFieldX,iFieldY,iLvImageNo, iImgType,5,2);
			}
			DisplayQuickMemory(QM_COMBATSKILL,iCSIndex,iMainX+iFieldX+18,iMainY+iFieldY+18);
			if(fRcMouse)//���콺 ���� �Ǿ�����
			{
				DrawCombatSkillExplain(Hero->x-Mapx+20,Hero->y-Mapy-80, nWillDo);
				SetQuickMemoryByKeyInput(QM_COMBATSKILL,iCSIndex);
			}
		}
	}break;

case 	FT_CSP_POINT_UP:
	{
		if(fRcMouse)
		{
			if(fLBDown)//��ư Ŭ�� �̺�Ʈ
			{
				FieldTypeNomalPut(iMainX, iMainY, iFieldX, iFieldY, iRcImg, iImgType );
			}
			else
			{
				FieldTypeNomalPut   (iMainX, iMainY, iFieldX, iFieldY , iImgNo, iImgType );
				FieldTypeNomalPutFx3(iMainX, iMainY, iFieldX, iFieldY , iImgNo, iImgType, 3, 2 );
			}
		}
		else
		{
			FieldTypeNomalPut(iMainX, iMainY, iFieldX, iFieldY, iImgNo, iImgType );
		}			
		SetHangulAlign(TA_CENTER);
		Hprint2( iMainX+iFieldX-27 ,iMainY+iFieldY+19, g_DestBackBuf,"%d",iCSReservPoint);  //�ٰ����� 15�̴�
	}break;
case 	FT_CSD_POINT_COUNT_BAR:
	{
		if(SubMenu->nField[1].nWillDo)//����� ��ų�� ������ ���
		{
	//		FieldTypeNomalPutFx3(iMainX, iMainY, iFieldX,iFieldY,85, COMBAT_SKILL_ICON_IMG,5 ,6);//���� ���
	//		FieldTypeNomalPutFx3(iMainX, iMainY, iFieldX,iFieldY,85, COMBAT_SKILL_ICON_IMG,2 ,4);//���� ���
			//��� �κ� �ֱ�
			FieldTypeNomalPut(iMainX, iMainY, iFieldX,iFieldY,85, COMBAT_SKILL_ICON_IMG);//���� ���
			const int iMax = nWillDo			= iCSPCMax;
			const int iNow = iSwd	= iCSPCNow;
			if(!iMax || !iNow ) {break;}

			const int iBarHeight = 88;
			const int iTarget = static_cast<int>(((float)iNow) / ((float)iMax) * ((float)iBarHeight)) ;
			
			for(int iCount = 0; iCount < iTarget; iCount++)//011031 lsw
			{
				FieldTypeNomalPutFx3(iMainX, iMainY, 
					iFieldX+23,
					iFieldY+iBarHeight-iCount+14,
					iImgNo,
					iImgType,17,1);
			}
			SetHangulAlign(TA_CENTER);
			Hcolor( FONT_COLOR_WHITE );
			Hprint2( iMainX+iFieldX+33,iMainY+iFieldY+iBarHeight+4, g_DestBackBuf,"%d",iNow);
		}
	}break;
case 	FT_CSD_POINT_LINE_BAR:
	{
		if(SubMenu->nField[1].nWillDo)//����� ��ų�� ������ ���
		{
		//	FieldTypeNomalPutFx3(iMainX, iMainY, iFieldX,iFieldY,85, COMBAT_SKILL_ICON_IMG,5,6);//���� ���
		//	FieldTypeNomalPut(iMainX, iMainY, iFieldX,iFieldY,85, COMBAT_SKILL_ICON_IMG);//���� ���
			//��� �κ� �ֱ�
			const int iMax = nWillDo			= iCSPMax;
			const int iNow = iSwd	= iCSPNow;
			if(!iMax || !iNow ) {break;}

			const int iBarHeight = 88;
			const int iTarget = static_cast<int>(((float)iNow) / ((float)iMax) * ((float)iBarHeight));
			
			for(int iCount = 0; iCount < iTarget; iCount++)//011031 lsw
			{
				FieldTypeNomalPutFx3(iMainX, iMainY, 
					iFieldX+23,
					iFieldY+iBarHeight-iCount+14,//-3,
					iImgNo,
					iImgType,17,1);
			}
			SetHangulAlign(TA_CENTER);
			Hcolor( FONT_COLOR_WHITE );
			Hprint2( iMainX+iFieldX+33,iMainY+iFieldY+iBarHeight+4, g_DestBackBuf,"%d",iNow);
		}
	}break;
case	FT_CS_GROUP_EXPLAIN:
	{	//	iWork;//�׷� �ѹ�		//	nWillDo;//�׷� Ÿ��
		const int iImageNo = 73 + (iWork-1)*2 +(nWillDo-1);//���� �׸��� 73 �� ���� ����
//			const int x = iMainX+iFieldX;
//			const int y = iMainY+iFieldY;
		FieldTypeNomalPut(iMainX, iMainY, iFieldX,iFieldY,iImageNo, COMBAT_SKILL_ICON_IMG);//���� ���
		
//			Hcolor( FONT_COLOR_YELLOW );	
//			SetHangulAlign(TA_CENTER);
//			Hprint2(x+21,y+50,g_DestBackBuf,"<%s>",CombatSkillGroupExplain[(iWork-1)*2 +(nWillDo)].LocalName);
		
		if(fRcMouse)//���콺 �� �϶�
		{
			DrawCombatSkillGroupExplain(563,130, iWork,nWillDo);
		}
	}break;
case	FT_CS_EXPLAIN:
	{
		int iCSkillIndex = CombatSkill[nWillDo].iSkillNo - iCSCovertSkillID;
		if(fRcMouse)//���콺 �� �϶�
		{
			Hcolor( FONT_COLOR_PLUS );	
			Hprint2(iMainX +iFieldX,iMainY+ iFieldY,g_DestBackBuf,CombatSkillExplain[iCSkillIndex].EngName);
			DrawCombatSkillExplain(303,130, nWillDo);
		}
		else
		{
			Hcolor( FONT_COLOR_WHITE );	
			Hprint2(iMainX +iFieldX,iMainY+ iFieldY,g_DestBackBuf,CombatSkillExplain[iCSkillIndex].EngName);
		}
	}break;
case FT_PEACE_BATTLE_PUT :
	{//49 50 51 �ǽ� 58 59 60 ��Ʋ 61 62 63 ������
		// battle mode
		const int param = main_interface.data[IF_BATTLE_PEACE];
		int nomal[] = { 49, 58, 61 };
		int light[] = { 50, 59, 62 };
		int down[] =  { 51, 60, 63 };

		int *image_num = nomal;//�⺻ ���� �׸�
		
		if( fRcMouse ) //�ö����
		{
			if( fLBDown )
			{
				image_num = down;
			}
			else
			{
				image_num = light;
			}
		}
		
		FieldTypeNomalPut(iMainX, iMainY, iFieldX, iFieldY, 
			image_num[param], iImgType );
	}break;
	case FT_DISPLAY_TAC_EXP:
		{
			
			int first_exp=0, exp, lv, next;
			int refresh = 0;
			static int old_tactics = SCharacterData.tatics;
			if( old_tactics != SCharacterData.tatics ) 
			{
				refresh = 1;
				old_tactics = SCharacterData.tatics;
			}
			exp = SCharacterData.TacticsExp[SCharacterData.tatics-88];

			int olg_lv = SCharacterData.SkillId[SCharacterData.tatics];
			if( olg_lv )
				first_exp = NPC_Lev_Ref[olg_lv].nMinExp;
			else olg_lv = 0;

			next = SCharacterData.TacticsExpNext[SCharacterData.tatics-88]; 
			if( !next ) next = 1;
			lv = olg_lv;		// tactic�� +1�� ǥ���� �ش�.
			DisplayTacGauge(SubMenu,j,iMainX+iFieldX,iMainY+iFieldY,next,exp,lv,refresh,first_exp);
			
			SetHangulAlign( TA_LEFT );
			Hprint2( iMainX+iFieldX+25, iMainY+iFieldY+2, g_DestBackBuf, "%s", explain_tactics[SCharacterData.tatics-88] );
			// Magic EXP Percentage JHDR
			const int iNow = exp - first_exp;
			const int iMax = next - first_exp;
			const float fExpRate = (float(iNow)) / (float(iMax));
			int Per = (int)(fExpRate * 100);
			HprintBold(iMainX + iFieldX + 76, iMainY + iFieldY + 1,
				Convert16to32(FONT_COLOR_WHITE), Convert16to32(FONT_COLOR_BLACK),
				"%d%%", Per);
		}break;
	case FT_DISPLAY_MAGIC_EXP:
		{
			int refresh = 0;
			
			int first_exp, exp, lv, next;
			int tac = (SCharacterData.nCharacterData[SPELL] == PRIEST_SPELL) ? ORISON : MAGERY ;
			exp = SCharacterData.TacticsExp[tac];

			int olg_lv = SCharacterData.SkillId[tac+88];
			if( olg_lv )
				first_exp = NPC_Lev_Ref[olg_lv].nMinExp;
			else first_exp = 0;

			next = SCharacterData.TacticsExpNext[tac]; 
			if( !next ) next = 1;
			lv = olg_lv;
			if( refresh_magery_time <= 0 )
			{
				CallServer( CMD_REQ_MAGERY );  //������ ���������� ���� ��...
				refresh_magery_time = 100;
			}
			else refresh_magery_time--;
			DisplayTacGauge(SubMenu,j,iMainX+iFieldX,iMainY+iFieldY,next,exp,lv,refresh,first_exp);
			SetHangulAlign( TA_LEFT );
			Hprint2( iMainX+iFieldX+25, iMainY+iFieldY+2, g_DestBackBuf, "%s", explain_tactics[tac] );

			// Magic EXP Percentage JHDR
			const int iNow = exp - first_exp;
			const int iMax = next - first_exp;
			const float fExpRate = (float(iNow)) / (float(iMax));
			int Per = (int)(fExpRate * 100);
			HprintBold(iMainX + iFieldX + 76, iMainY + iFieldY + 1,
				Convert16to32(FONT_COLOR_WHITE), Convert16to32(FONT_COLOR_BLACK),
				"%d%%", Per);

		}break;
	case FT_DISPLAY_SKILL_EXP:
		{
			int refresh = 0;
			
			int first_exp, exp, lv, next;
			int job;
			if( !before_skill_no )
			{
				GetSkillMother( g_GmMain[Hero->job].skill, &job, 1 );
			}
			else job = before_skill_no;
			
			static int old = job;
			if( old != job ) SubMenu->CheakType = 0;
			if(job<0 || job> MAX_SKILL) break;
			//011012 lsw >
			exp = SCharacterData.nJobSkill[skill[job].inclusive].skillexp % 10000;
			lv =  SCharacterData.nJobSkill[skill[job].inclusive].skillexp / 10000;
			//011012 lsw <

			next = 10000;
			first_exp = 0;
			DisplayTacGauge(SubMenu,j,iMainX+iFieldX,iMainY+iFieldY,next,exp,lv,refresh,first_exp);
			SetHangulAlign( TA_LEFT );
			Hprint2( iMainX+iFieldX+25, iMainY+iFieldY+2, g_DestBackBuf, "%s", skill[job].han_name );
			// Magic EXP Percentage JHDR
			const int iNow = exp - first_exp;
			const int iMax = next - first_exp;
			const float fExpRate = (float(iNow)) / (float(iMax));
			int Per = (int)(fExpRate * 100);
			HprintBold(iMainX + iFieldX + 76, iMainY + iFieldY + 1,
				Convert16to32(FONT_COLOR_WHITE), Convert16to32(FONT_COLOR_BLACK),
				"%d%%", Per);
		}break;
	case FT_EXPLAIN_MENU :
		{
			if( !SubMenu->CheakType )
			{
				SubMenu->CheakType = 1;
				if( SubMenu->nField[j+1].nType != FT_EXPLAIN_MENU )		// ���� �ʵ� Ÿ���� ������찡 �ƴϸ� ����Ÿ�� �о� �´�.
					SetMenuExplain( nWillDo, SubMenu->Id, j );
			}

			if( fRcMouse )
			{	//if( !bSoundOn ) { MP3( SN_TOUCH ); bSoundOn = true; }
				ExplainMenuSet( SubMenu->nField[j].temp );
			}
		}break;
	case FT_NATION_DISPLAY :
		{
			SetHangulAlign(TA_CENTER);
			Hcolor( NationColor16[MapInfo[MapNumber].nation] );
			Hprint2(iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf, "%s", NationName[MapInfo[MapNumber].nation]);
			/*int num = 651 + g_SDLInfo.day_of_week;
			if (num == 651) num = 658;
			Hcolor(RGB16(215, 215, 145));
			Hprint2(iMainX + iFieldX, iMainY + iFieldY + 20, g_DestBackBuf, "%s", lan->OutputMessage(0, num));*/
		}break;
	//011004 lsw
	case FT_XY_DISPLAY :
		{	//< CSD-030509
			const int mapx = g_Map.x + g_pointMouseX / TILE_SIZE;
			const int mapy = g_Map.y + g_pointMouseY / TILE_SIZE;
			SetHangulAlign(TA_CENTER);
			Hcolor( RGB16(215,215,145) );
			Hprint2(iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf, "%s", MapNumberName);
            Hprint2(iMainX+iFieldX, iMainY+iFieldY+20, g_DestBackBuf, "%3d , %3d", mapx, mapy);
			//Hprint2(iMainX + iFieldX, iMainY + iFieldY+20, g_DestBackBuf, "%s", g_SDLInfo.serverTime.c_str() );
			break;
		}	//> CSD-030509
	case FT_WILLDO_WORK :
		{
 			if (iWork==1)
			{
				FieldTypeNomalPut(iMainX,iMainY,38,3,4 , TIME_MENU );
			}
			else 
			{
				FieldTypeNomalPut(iMainX,iMainY,38,3,3 , TIME_MENU );
			}
		 }break;
	case FT_MAININTERFACE_MAIL_CALL:
		{
			if(g_MailMgr.GetExistNotRead())//������ ���� ����
			{
				static bool bBlend	= true;
				static int iAlpha =10;

				if(	bBlend ) 
				{	iAlpha += 2;	
					if( iAlpha > 10 ) 
					{
						bBlend = !bBlend;
					}
				}
				else
				{
					iAlpha -=1;	
					if( iAlpha < 5 ) 
					{
						bBlend = !bBlend;
					}
				}
				FieldTypeNomalPutFx3(iMainX,iMainY,iFieldX,iFieldY, iImgNo, iImgType,iAlpha,2);
				static int count = 0, index =0;
				if (index >= 5) index = 0;
				Spr* s= g_SDLInfo.GetSpriteFromCache("./data/bird.dat", index);//雀仔飛
				if (count++ % 3 == 0) index++;
				PutCompressedImage(SCREEN_WIDTH -100, 100, s);
			}
			else
			{
				FieldTypeNomalPut(iMainX,iMainY,iFieldX,iFieldY, iImgNo , iImgType);
			}

		}break;
//<! BBD 040303
	case FT_MERCHANT_BACKUP_GET_LIST : 
		{

			if( !SubMenu->CheakType )
			{
				SubMenu->CheakType = 1;
				// ��Ŷ ����
				Auction.SendCMD_MERCHANT_BACKUP_LIST_REQUEST();//�Ǹ����� ����� �������� �޾ƿ�
			}
			break;
		}
//> BBD 040303
	case FT_TODAY_NOTICE:
		{	
			if (g_LocalWarBegin || g_bWarBegin || g_iScenarioOn || nWillDo > 0)//�̺�Ʈ ����
			{
				static bool bBlend	= true;
				static int iAlpha =10;

				if(	bBlend ) 
				{	iAlpha += 2;	
					if( iAlpha > 10 ) 
					{
						bBlend = !bBlend;
					}
				}
				else
				{
					iAlpha -=1;	
					if( iAlpha < 5 ) 
					{
						bBlend = !bBlend;
					}
				}
				FieldTypeNomalPutFx3(iMainX,iMainY,iFieldX,iFieldY, iImgNo, iImgType,iAlpha,2);
			}
			else
			{
				FieldTypeNomalPutFx3(iMainX,iMainY,iFieldX,iFieldY, iImgNo , iImgType,15,1);
			}
		}break;
	case FT_ALPHA_PUT:
		{
			FieldTypeNomalPutFx3(iMainX,iMainY,iFieldX,iFieldY, iImgNo, iImgType,15,1);
			FieldTypeNomalPutFx3(iMainX, iMainY+20, iFieldX, iFieldY, iImgNo, iImgType, 15, 1);
		}break;
	case FT_DRAG_ZONE:
		{
			int &iOn = nWillDo;
			if(iOn)
			{
				const int iGab = 10;
				const int iTargetX = g_pointMouseX;
				const int iTargetY = g_pointMouseY;
				if(0+iGab < iTargetX && GAME_SCREEN_XSIZE-iGab > iTargetX )
				{
					iMainX = iTargetX-iGab;
				}
				if(0+iGab < iTargetY &&	GAME_SCREEN_YSIZE-iGab > iTargetY )
				{
					iMainY = iTargetY-iGab;
				}
			}
		}break; //�극��ũ �����ϴ� �׸��� �Ʒ� �ֵ� ��ġ �ٲ��� ������
	case FT_CHAT_VIEW_SET:
		{
			if(ChatMgr.GetViewChat(iSwd))
			{
				FieldTypeNomalPut(iMainX,iMainY,iFieldX,iFieldY, iImgNo , iImgType);
			}
			else
			{
				FieldTypeNomalPut(iMainX,iMainY,iFieldX,iFieldY, iRcImg , iImgType);
			}
		}break;
	case FT_CHAT_LOCK:
		{
			if(nWillDo)
			{
				FieldTypeNomalPut(iMainX,iMainY,iFieldX,iFieldY, iImgNo , iImgType);
			}
			else
			{
				FieldTypeNomalPut(iMainX,iMainY,iFieldX,iFieldY, iRcImg , iImgType);
			}
		}break;				
	case FT_SELECT_CHAT_TYPE:
		{
			const int SendType = GetChatTarget();
			switch(SendType)
			{
			case 0:
				{	
					EWndMgr.SetTxt( HWND_CHAT_TARGET_INPUT, SMenu[MN_WISPER_LIST].nField[SendType].temp );//021001 lsw
					
					FieldTypeNomalPut(iMainX,iMainY,iFieldX,iFieldY, iImgNo , iImgType);
				}break;
			case 1:
				{	
					EWndMgr.SetTxt( HWND_CHAT_TARGET_INPUT, SMenu[MN_WISPER_LIST].nField[SendType].temp );//021001 lsw
					
					FieldTypeNomalPut(iMainX,iMainY,iFieldX,iFieldY, iRcImg , iImgType);
				}break;
			case 2://�ӼӸ� ��� ����
				{
					EWndMgr.SetTxt( HWND_CHAT_TARGET_INPUT, SMenu[MN_WISPER_LIST].nField[SendType].temp );//021001 lsw
					
					// Finito added image number manually
					FieldTypeNomalPut(iMainX,iMainY,iFieldX,iFieldY, 100 , iImgType);				
				}break;
			case 3:// Finito
				{
					FieldTypeNomalPut(iMainX,iMainY,iFieldX,iFieldY, nWillDo, iImgType);
				}break;
			default:
				{
					EWndMgr.SetTxt( HWND_CHAT_TARGET_INPUT, SMenu[MN_WISPER_LIST].nField[SendType].temp );//021001 lsw
					
					FieldTypeNomalPut(iMainX,iMainY,iFieldX,iFieldY, nWillDo, iImgType);
					
				}break;
			}

			//if (SendType != 3) EWndMgr.showInputBox(HWND_CHAT_INPUT);
		}break;
	case FT_SELECT_WISPER:
		{
			if(!IsAbleChatTargetName(nWillDo))
			{
				break;
			}
			FieldTypeNomalPutFx3(iMainX,iMainY,iFieldX,iFieldY, iImgNo , iImgType,15,1);//�⺻ ������ ���
			Hprint2(iMainX+iFieldX+2, iMainY+iFieldY+4, g_DestBackBuf,"%s",SubMenu->nField[j].temp);//020505 lsw
			const int TargetCh = nWillDo;
			if(TargetCh == iWork || fRcMouse)
			{
				FieldTypeNomalPut(iMainX+2,iMainY+2,iFieldX,iFieldY, iRcImg , iImgType);
			}
		}break;
	case FT_SELECT_WISPER_TARGET:
		{
		//	FieldTypeNomalPut(iMainX+2,iMainY+2,iFieldX,iFieldY, iRcImg , iImgType);
		}break;
	case FT_HOTKEY_ICON ://020701 lsw
		{
			FieldTypeNomalPut(iMainX, iMainY, iFieldX,iFieldY,iImgNo, iImgType);//����Ʈ�� ����ִ� ��ư
			const int iNo = HotKeyMemory.iNo;
			if (!iNo|| strcmp(Hero->name,HotKeyMemory.OwnerName))//���� �̸��� �ƴ϶��
			{
				break;
			}
			const int iX = iMainX+iFieldX+21;
			const int iY = iMainY+iFieldY+21;//SetMagicOn �� ���� ���� ��� +21

			switch(HotKeyMemory.iType)
			{
			case QM_MAGIC:
				{
					if( iNo )//���� �ѹ��� �ְ�
					{
						if(!RectImageOn) 
						{
							PutMagicIcon(iX,iY, iNo);	// ���
						}
					}
					else 
					{
						break;
					}
					if( RectImageOn && !MagicOnFlag.type )	
					{
						PutMagicIcon(iX , iY, iNo);
						static int ct=1;
						static bool ct_flag = 0;

						PutMagicIcon(iX , iY, iNo, ct+2);

						if(ct_flag) {ct++;}
						else {ct--;}

						ct %= 9 ;
						if( ct == 0 ){ ct_flag = 1;}
						if( ct == 8 ){ ct_flag = 0;}
					}
					if( fRcMouse || fLBDown || SubMenu->nField[j].fRButtonDown )	
					{
						CheckMagicExplain( HotKeyMemory.iNo );
						PutMagicIcon(iX, iY, iNo, 8);		// ���� �׸�
					}
				}break;
			case QM_SKILL:
				{
					if( iNo )//��ȣ�� �ְ�
					{
						PutSkillIcon(iX, iY, iNo);	// ���
					}
					else 
					{
						break;
					}

					if( fRcMouse && !g_MouseItemType )//���콺�� �ö� ����
					{
						CheckSkillExplain( iNo );
					}
					if( fLBDown || SkillOn == 1)//��ų�� ������ �̶��
					{
						PutSkillIcon(iX, iY, iNo, 8);		// ���� �׸�
					}
				}break;
			case QM_COMBATSKILL:
				{
					
				}break;
			}
			SetHangulAlign(TA_CENTER);
		//	Hcolor(FONT_COLOR_YELLOW);
			HprintBold( iX-10, iY-4, FONT_COLOR_SOLID_YELLOW, FONT_COLOR_BLACK, "'/'" );
		}break;
	case FT_QUICK_MAGIC_PLUS:
	{
		int magic_x = 0;
		const int offset_x = SCREEN_WIDTH / 2 - 400 + 189; //409;
		const int offset_x2 = offset_x-3;
		const int offset_x3 = offset_x + ONE_MAGIC_RECT;
		// --- 進入 FT_QUICK_MAGIC_PLUS 時的基準值日誌 ---
		//SDL_Log("DEBUG: FT_QUICK_MAGIC_PLUS - iMainX (Enter) = %d, iMainY (Enter) = %d", iMainX, iMainY);
		//SDL_Log("DEBUG: FT_QUICK_MAGIC_PLUS - scroll_start (magic_plus_start) = %d", magic_plus_start); // 新增：打印 scroll_start 的值
		// --------------------------------------------------
		int showmagic = SHOW_MAGIC;

		// 在繪製前，檢查 magic_plus_start 的值是否有效
	// 這是為了防止法術總數減少時，起始點還停留在舊的位置
	//
		if (magic_plus_count <= showmagic)
		{
			// 如果法術總數連一行都不到，則起始點必須是 0
			magic_plus_start = 0;
		}
		else
		{
			// 如果法術總數超過一行，
			// 則檢查起始點是否超過了「最後一頁」的起始位置
			int max_start_index = magic_plus_count - showmagic;
			if (magic_plus_start > max_start_index)
			{
				magic_plus_start = max_start_index;
			}
		}

		RECT& rect = SubMenu->nField[j].rCheakBox;
		switch (magic_plus_count)		// ׵     
		{
		case 0:
		case 1:
		{
			FieldTypeNomalPut(iMainX, iMainY, offset_x2, -52, 15, MAIN_ETC);
			FieldTypeNomalPut(iMainX, iMainY, offset_x, -52, 16, MAIN_ETC);
			magic_x = offset_x;

			// --- SetRect 之前的參數和之後的 rect 值日誌 (Case 0/1) ---
			//SDL_Log("DEBUG: Case 0/1 - magic_x = %d, iMainX = %d, iMainY = %d", magic_x, iMainX, iMainY);
			// 讓 rect 儲存相對於菜單基準點的偏移量，以配合 MouseInRectCheak
			::SetRect(rect, magic_x, -52, magic_x + ONE_MAGIC_RECT, 0);
			//SDL_Log("DEBUG: Case 0/1 - rect {left: %d, top: %d, right: %d, bottom: %d}", rect.left, rect.top, rect.right, rect.bottom);
			// ----------------------------------------------------
			//SMenu[MN_QUICK_MAGIC_SCROLL].x = magic_x - 22;
			SMenu[MN_QUICK_MAGIC_SCROLL].bActive = false;
			SMenu[MN_TIME_DISPLAY_ON_OFF].x = iMainX + offset_x3;
			SMenu[MN_TIME_DISPLAY_ON_OFF].y = iMainY - 52;
		}break;

		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
		{
			int gabx = (ONE_MAGIC_RECT/2) * (magic_plus_count - 1);
			FieldTypeNomalPut(iMainX, iMainY, offset_x2 - gabx, -52, 15, MAIN_ETC);
			int i;
			for (i = 0; i < magic_plus_count; i++)
				FieldTypeNomalPut(iMainX, iMainY, offset_x - gabx + ONE_MAGIC_RECT * i, -52, 16, MAIN_ETC);
			magic_x = offset_x - gabx;

			// --- SetRect 之前的參數和之後的 rect 值日誌 (Case 2-5) ---
			//SDL_Log("DEBUG: Case 2-5 - magic_x = %d, iMainX = %d, iMainY = %d, gabx = %d, last_i = %d", magic_x, iMainX, iMainY, gabx, i);
			// 讓 rect 儲存相對於菜單基準點的偏移量，以配合 MouseInRectCheak
			::SetRect(rect, magic_x, -52, magic_x + i * ONE_MAGIC_RECT, 0);
			//SDL_Log("DEBUG: Case 2-5 - rect {left: %d, top: %d, right: %d, bottom: %d}", rect.left, rect.top, rect.right, rect.bottom);
			// ----------------------------------------------------
			//SMenu[MN_QUICK_MAGIC_SCROLL].x = magic_x -22;
			SMenu[MN_QUICK_MAGIC_SCROLL].bActive = false;
			SMenu[MN_TIME_DISPLAY_ON_OFF].x = iMainX + offset_x3 + gabx;
			SMenu[MN_TIME_DISPLAY_ON_OFF].y = iMainY - 52;
		}break;

		default:
		{
			FieldTypeNomalPut(iMainX, iMainY, offset_x2 - (ONE_MAGIC_RECT / 2) * (showmagic-1), -52, 15, MAIN_ETC);
			for (int i = 0; i < showmagic; i++)
			{
				FieldTypeNomalPut(iMainX, iMainY, offset_x - (ONE_MAGIC_RECT / 2) * (showmagic-1) + ONE_MAGIC_RECT * i, -52, 16, MAIN_ETC);
			}
			magic_x = offset_x - (ONE_MAGIC_RECT / 2) * (showmagic-1);

			// --- SetRect 之前的參數和之後的 rect 值日誌 (Default Case) ---
			//SDL_Log("DEBUG: Default Case - magic_x = %d, iMainX = %d, iMainY = %d", magic_x, iMainX, iMainY);
			// 讓 rect 儲存相對於菜單基準點的偏移量，以配合 MouseInRectCheak
			::SetRect(rect, magic_x, -52, magic_x + (showmagic) *ONE_MAGIC_RECT, 0);
			//SDL_Log("DEBUG: Default Case - rect {left: %d, top: %d, right: %d, bottom: %d}", rect.left, rect.top, rect.right, rect.bottom);
			// ----------------------------------------------------
			SMenu[MN_QUICK_MAGIC_SCROLL].x = offset_x - ONE_MAGIC_RECT * 1 - (ONE_MAGIC_RECT/2);
			SMenu[MN_QUICK_MAGIC_SCROLL].bActive = true;
			SMenu[MN_TIME_DISPLAY_ON_OFF].x = iMainX + offset_x3 + (ONE_MAGIC_RECT / 2) * (showmagic-1);
			SMenu[MN_TIME_DISPLAY_ON_OFF].y = iMainY - 52;

		}break;
		}

		int& scroll_max = SubMenu->nField[j].nSHideNomalCount;
		scroll_max = magic_plus_count;

		const int scroll_start = magic_plus_start;
		// 保持 for 迴圈條件修正
		for (int i = scroll_start, count = 0; i < scroll_max && count < showmagic; i++, count++)
		{
			if (i >= MAX_MAGIC || i < 0) {
				//SDL_Log("DEBUG: Display Loop Break - i out of bounds: %d", i);
				break;
			}
			if (magic_plus[i])
			{
				// --- 顯示時的魔法 ID 和索引日誌 ---
				//SDL_Log("DEBUG: Displaying Slot %d (Array Index %d) with Magic ID: %d", count, i, magic_plus[i]);
				// ----------------------------------

				PutMagicIcon(iMainX + magic_x + 44 * count + 22, iMainY - 52 + 27, magic_plus[i]);
				DisplayQuickMemory(QM_MAGIC, magic_plus[i], iMainX + magic_x + 44 * count + 16, iMainY - 52 + 22);
			}
			else
			{
				//SDL_Log("DEBUG: Display Loop Break - magic_plus[%d] is null/false, stopping display.", i);
				break;
			}
		}

		if (fRcMouse || fLBDown)
		{
			// --- 滑鼠點擊時的基礎日誌 ---
			//SDL_Log("DEBUG: Mouse Click - g_pointMouseX = %d, g_pointMouseY = %d", g_pointMouseX, g_pointMouseY);
			//SDL_Log("DEBUG: Mouse Click - rCheakBox {left: %d, top: %d, right: %d, bottom: %d} (Relative)",
				//SubMenu->nField[j].rCheakBox.left, SubMenu->nField[j].rCheakBox.top,
				//SubMenu->nField[j].rCheakBox.right, SubMenu->nField[j].rCheakBox.bottom);
			// -----------------------------

			// 關鍵修正：將 rCheakBox.left 轉換為絕對座標後再進行計算
			const int absolute_rect_left = iMainX + SubMenu->nField[j].rCheakBox.left;
			const int x_count = (g_pointMouseX - absolute_rect_left) / 44;
			const int x_check = (g_pointMouseX - absolute_rect_left) % 44;

			// --- x_count 和 x_check 的計算結果日誌 ---
			//SDL_Log("DEBUG: Mouse Click - Calculated x_count = %d, x_check = %d (relative to absolute_rect_left: %d)",
				//x_count, x_check, absolute_rect_left);
			// ------------------------------------------

			// 你可能需要根據實際顯示情況調整這個 x_check 的範圍
			// 如果 x_check = 0 或 1 是有效點擊，需要調整條件。
			if (x_check < 2 || x_check > 41) {
				//SDL_Log("DEBUG: Mouse Click - Invalid x_check (%d), breaking.", x_check);
				return;
			}

			// --- 點擊後嘗試使用的魔法 ID 和索引日誌 ---
			// 這裡確保 magic_plus 陣列索引不越界，雖然理論上 x_count 不會導致越界
			if ((scroll_start + x_count) < MAX_MAGIC && (scroll_start + x_count) >= 0 && magic_plus[scroll_start + x_count])
			{
				//SDL_Log("DEBUG: Attempting to use Magic ID: %d from Array Index: %d (scroll_start: %d + x_count: %d)",
					//magic_plus[scroll_start + x_count], scroll_start + x_count, scroll_start, x_count);

				PutMagicIcon(iMainX + magic_x + 44 * x_count + 22, iMainY - 52 + 27, magic_plus[scroll_start + x_count], 8, 2);
				CheckMagicExplain(magic_plus[scroll_start + x_count]);
			}
			else {
				//SDL_Log("DEBUG: Clicked slot is empty or out of bounds. Final Index: %d (scroll_start: %d + x_count: %d)",
					//scroll_start + x_count, scroll_start, x_count);
			}
			// 即使槽位空，如果需要，也可以設定快速記憶
			SetQuickMemoryByKeyInput(QM_MAGIC, magic_plus[scroll_start + x_count]);
		}
	}
	break;
	case	FT_RARE_GROUP:
		{	//
			const int iRareGroup = iWork + nWillDo;
			const int iIsDynamicRare = SMenu[MN_ITEM_MAKE_RARE_SELECT].key;
			if( 0 > iRareGroup || MAX_RARE_GROUP <= iRareGroup)
			{
				break;
			}
			if(RareGroup[iIsDynamicRare][iRareGroup].name[0])
			{
				Hprint2( iMainX+iFieldX+30, iMainY+iFieldY, g_DestBackBuf , "%s", RareGroup[iIsDynamicRare][iRareGroup].name);
				FieldTypeNomalPut(iMainX, iMainY, iFieldX, iFieldY, iImgNo, iImgType);

				if( SMenu[MN_ITEM_MAKE_RARE_SELECT].nField[0].nWillDo == iRareGroup)
				{
					FieldTypeNomalPut(iMainX, iMainY, iFieldX, iFieldY, iRcImg, iImgType);
				}
			}
		}break;
	case FT_MERCHANT_SELL_ITEM_DROP_ZONE:
		{
			ItemAttr *item = Auction.GetSellItemAttr();//������ Pos �ޱ�
			if(!item){break;}//������ ������ ����
			PutItemIcon(iMainX	+	iFieldX,//+	iFieldX, 
						iMainY	+	iFieldY,//+	iFieldY,
						item->item_no,30,1 );
		}break;
	case FT_MERCHANT_SELL_ITEM_EXPLAIN:
		{
			ItemAttr *item = Auction.GetSellItemAttr();//������ Pos �ޱ�
			if(!item){return;}
			ExplainAuctionItem(iMainX+iFieldX,iMainY+iFieldY,	item,true);
			//�������̸�, ������ ������, ����, ����, �䱸�ɷ�, ��ƽ��
		}break;
	case FT_MERCHANT_SELL_ITEM_VALUE_INPUT://登記物品時輸入金額
		{//���ڸ� �Է� �ǰԲ�
			const int iX = iMainX+iFieldX;
			const int iY = iMainY+iFieldY;
			//char szTemp[11] = {0,};
			EWndMgr.showInputBox(HWND_REGISTER_ITEM_SELL);
			std::string szTemp = EWndMgr.GetTxt( HWND_REGISTER_ITEM_SELL);
			const int iSellValue = ::atol( szTemp.c_str() );
			//EWndMgr.SetTxt(HWND_REGISTER_ITEM_SELL,szTemp);//021001 lsw

			if(iSellValue)
			{
				TxtOut.RcTXTOutNoBG(iX - 150, iY+7, 100,0,lan->OutputMessage(4,540));//021126 lsw
				Auction.SetSellValue(iSellValue);
			}
			else
			{
				//TxtOut.RcTXTOutNoBG(iX - 150, iY+7, 100,0,lan->OutputMessage(4,541));//021126 lsw
			}


			TxtOut.RcTXTOutNoBG(iX +30, iY+7, 100,0,"%u",iSellValue);
		}break;
	case FT_MERCHANT_RESULT_ITEM_CHECK:
	case FT_MERCHANT_DIRECT_BUY_ITEM_CHECK:
	case FT_MERCHANT_BUY_ITEM_CHECK:
	case FT_MERCHANT_SELL_ITEM_CHECK:
		{
			int iIndex = 0;
			SEARCHRESULT *SR = NULL;
			switch(iType)
			{
			case FT_MERCHANT_RESULT_ITEM_CHECK:
				{
					iIndex = Auction.GetTakeItemIndex();
                    auto List = Auction.GetSearchList();
					SR = &List.ResultList[iSwd + Auction.GetResultViewPage()*MAX_VIEW_ABLE_PER_PAGE ];
				}break;
			case FT_MERCHANT_DIRECT_BUY_ITEM_CHECK:
			case FT_MERCHANT_BUY_ITEM_CHECK:
				{
					iIndex = Auction.GetBuyItemIndex();
                    auto List = Auction.GetSearchList();
					SR = &List.ResultList[iSwd + Auction.GetBuyerViewPage()*MAX_VIEW_ABLE_PER_PAGE ];
				}break;
			case FT_MERCHANT_SELL_ITEM_CHECK:
				{
					iIndex = Auction.GetDeleteItemIndex();
                    auto List = Auction.GetSearchList();
					SR = &List.ResultList[iSwd + Auction.GetSellerViewPage()*MAX_VIEW_ABLE_PER_PAGE ];
				}break;
			}		
			

			const int iX = iMainX+iFieldX;
			const int iY = iMainY+iFieldY;
			
			nWillDo = SR->iIndex;
			
			ItemAttr *pItem = &SR->m_ResultItem;	if(!pItem){break;}
			const int iItemNo = pItem->item_no;
			CItem *t = ItemUnit(iItemNo);			if(!t){break;}

			if(iItemNo)
			{
				char szTemp[MAX_PATH] ={0,};
				if(DIVIDE_ITEM == t->GetRButton())
				{
					sprintf( szTemp, "%d", pItem->attr[IATTR_MUCH] );
				}
				else
				{
					WORD d_max=0, d_curr=0;
					GetItemDuration( *pItem, d_curr, d_max );
					sprintf( szTemp, "%d/%d", d_curr/10, d_max/10 );
				}

				switch(iType)
				{
				case FT_MERCHANT_RESULT_ITEM_CHECK:
					{
						ExplainAuctionItem(iX+78,iY+5,pItem,false);//������ ����
						Hcolor(FONT_COLOR_WHITE);
						if(!strcmp(Hero->name,SR->szBuyerName))
						{
							PutItemIcon(iX+50 ,iY+20,iItemNo);	
							TxtOut.RcTXTOutNoBG(iX+257,iY+8,50,0,szTemp);//������
						}
						else
						{
							if(!strcmp(Hero->name,SR->szSellerName))
							{
								TxtOut.RcTXTOutNoBG(iX +38, iY+7, 30,0,lan->OutputMessage(4,551));//030103 lsw
								TxtOut.RcTXTOutNoBG(iX +257, iY+7, 50,0,"%d",SR->iSellValue);
							}
							else
							{
								TxtOut.RcTXTOutNoBG(iX +38, iY+7, 30,0,"Error");
							}
						}
						if(	iX+35+35 > g_pointMouseX&& iX+35 < g_pointMouseX
						&&	iY+35 > g_pointMouseY	&& iY < g_pointMouseY)
						{
							ExplainItemAttr = *pItem;
						}
					}break;
				case FT_MERCHANT_DIRECT_BUY_ITEM_CHECK:
					{
						ExplainAuctionItem(iX+70,iY+5,pItem,false);//������ ����

						Hcolor(FONT_COLOR_WHITE);//021121 lsw
						TxtOut.RcTXTOutNoBG(iX+393-130,iY+8,50,0,szTemp);//������
						Hcolor(FONT_COLOR_WHITE);	
						const int iSellValue = SR->iSellValue;
						if(iSellValue)//�Ǹűݾ��� ������ 
						{
							TxtOut.RcTXTOutNoBG(iX+460-130,iY+15,60,0,lan->OutputMessage(4,540));//������ �̸�
							TxtOut.RcTXTOutNoBG(iX+532-130,iY+7,60,0,"%d",iSellValue);//������ �̸�
						}
						else//�����ŷ����
						{
							TxtOut.RcTXTOutNoBG(iX+460-130,iY+15,60,0,lan->OutputMessage(4,541));//������ �̸�
						}
						PutItemIcon(iX+42 ,iY+20,iItemNo);	//������ ������
						if(	iX+25+35 > g_pointMouseX&& iX+25 < g_pointMouseX
						&&	iY+35 > g_pointMouseY	&& iY < g_pointMouseY)
						{
							ExplainItemAttr = *pItem;
						}
					}break;
				case FT_MERCHANT_BUY_ITEM_CHECK:
					{
						ExplainAuctionItem(iX+70,iY+5,pItem,false);//������ ����
					
						Hcolor(FONT_COLOR_WHITE);//021121 lsw
						TxtOut.RcTXTOutNoBG(iX+260,iY+15,200,0,SR->szSellerName);//�Ǹ��� �̸�
						TxtOut.RcTXTOutNoBG(iX+393,iY+8,50,0,szTemp);//������	
						Hcolor(FONT_COLOR_WHITE);
						const int iSellValue = SR->iSellValue;
						if(iSellValue)//�Ǹűݾ��� ������ 
						{
							TxtOut.RcTXTOutNoBG(iX+460,iY+15,60,0,lan->OutputMessage(4,540));//������ �̸�
							TxtOut.RcTXTOutNoBG(iX+532,iY+7,60,0,"%d",iSellValue);//������ �̸�
						}
						else//�����ŷ����
						{
							TxtOut.RcTXTOutNoBG(iX+460,iY+15,60,0,lan->OutputMessage(4,541));//������ �̸�
							TxtOut.RcTXTOutNoBG(iX+532,iY+7,60,0,"%d",iSellValue);//������ �̸�
						}
						PutItemIcon(iX+42 ,iY+20,iItemNo);	//������ ������
						if(	iX+25+35 > g_pointMouseX&& iX+25 < g_pointMouseX
						&&	iY+35 > g_pointMouseY	&& iY < g_pointMouseY)
						{
							ExplainItemAttr = *pItem;
						}
					}break;
				case FT_MERCHANT_SELL_ITEM_CHECK:
					{
						if(iIndex )//üũ��ư
						{	if(iIndex == nWillDo )
							{
								FieldTypeNomalPut(iMainX+3, iMainY+3, iFieldX, iFieldY, iRcImg, iImgType);
							}
						}
						
						ExplainAuctionItem(iX+70,iY+5,pItem,false);//������ ����
						Hcolor(FONT_COLOR_WHITE);
						TxtOut.RcTXTOutNoBG(iX+260,iY+8,50,0,szTemp);//������	
						const int iSellValue = SR->iSellValue;
						if(iSellValue)//�Ǹűݾ��� ������ 
						{
							TxtOut.RcTXTOutNoBG(iX+322,iY+15,60,0,lan->OutputMessage(4,540));//������ �̸�
							TxtOut.RcTXTOutNoBG(iX+402,iY+7,60,0,"%d",iSellValue);//������ �̸�
						}
						else//�����ŷ����
						{
							TxtOut.RcTXTOutNoBG(iX+322,iY+15,60,0,lan->OutputMessage(4,541));//������ �̸�
						}
						PutItemIcon(iX+42 ,iY+20,iItemNo);	//������ ������
						if(	iX+25+35 > g_pointMouseX&& iX+25 < g_pointMouseX
						&&	iY+35 > g_pointMouseY	&& iY < g_pointMouseY)
						{
							ExplainItemAttr = *pItem;
						}
					}break;
				}

				if(iIndex && iIndex == nWillDo )//���� �������� üũ ��ư �����°�
				{//üũǥ��
					switch(iType)
					{
					case FT_MERCHANT_RESULT_ITEM_CHECK:
						{
							FieldTypeNomalPut(iX+4, iY, 3, 4, iRcImg, iImgType);
						}break;
					case FT_MERCHANT_DIRECT_BUY_ITEM_CHECK:
					case FT_MERCHANT_BUY_ITEM_CHECK:
					case FT_MERCHANT_SELL_ITEM_CHECK:
						{
							FieldTypeNomalPut(iX, iY, 3, 4, iRcImg, iImgType);
						}break;
					}
				}
				//���콺 �� �Ǹ� ������ ���� ���÷���
				
			}//if(iItemNo)
			else
			{
				Hcolor(FONT_COLOR_WHITE);
				switch(iType)
				{
				case FT_MERCHANT_RESULT_ITEM_CHECK:
					{
						TxtOut.RcTXTOutNoBG(iX +38, iY+7, 30,0,lan->OutputMessage(4,550));
					}break;
				case FT_MERCHANT_DIRECT_BUY_ITEM_CHECK:
				case FT_MERCHANT_BUY_ITEM_CHECK:
				case FT_MERCHANT_SELL_ITEM_CHECK:
					{
						TxtOut.RcTXTOutNoBG(iX +30, iY+7, 30,0,lan->OutputMessage(4,550));
					}break;
				}				
			}
		}break;
	case FT_EXCHANGE_ITEM : 
		{
		//	COMMENT ::FieldTypeNomalPut(iMainX, iMainY, iFieldX, iFieldY, iImgNo, iImgType);
			switch(ExchangeMgr.GetExchangeType())
			{
			case EXCHANGE_TYPE_MERCHANT_DIRECT:
				{//021121
					if(LEFT == nWillDo)
					{
						if(ExchangeMgr.m_bIsIAmMerchant)//���� �����̸� ���ʿ��� �������� ����(LEFT --> ���� �����̰� Right �� �Ѥ�;..
						{
							::FieldTypeNomalPut(iMainX, iMainY, iFieldX, iFieldY, iImgNo,iImgType );//�����ŷ��� ��� �ٴڿ� ���
						}
					}
					else//�������ε� ���� �����̸� �Ѹ���
					{
						if(!ExchangeMgr.m_bIsIAmMerchant)
						{
							::FieldTypeNomalPut(iMainX, iMainY, iFieldX, iFieldY, iImgNo,iImgType );//�����ŷ��� ��� �ٴڿ� ���
						}
					}
					//������ ��� ��ǰ�̴ϱ� �˾Ƽ�ó�� �սô� �Ѥ�;. ���Ŀ���
					::DisplayExchangeItemList(nWillDo,  fRcMouse);					
				}break;
			default:
				{
					::DisplayExchangeItemList(nWillDo,  fRcMouse);	break;
				}break;
			}//switch(ExchangeMgr.GetExchangeType())
		}break;
	case FT_EXCHANGE_ITEM_CHECK : 
		{
			const bool bIsOkMySide = ExchangeMgr.IsMySideConformStateOk();
			const bool bIsOkOtherSide = ExchangeMgr.IsOtherSideConformStateOk();//021030 lsw

			switch(nWillDo)
			{
			case LEFT:
				{
					if(bIsOkOtherSide)
					{
						::FieldTypeNomalPut(iMainX, iMainY, iFieldX+5, iFieldY+5, iImgNo ,iImgType);
					}
				}break;
			case RIGHT:
				{
					if(bIsOkMySide)
					{
						::FieldTypeNomalPut(iMainX, iMainY, iFieldX+5, iFieldY+5, iImgNo ,iImgType);
					}
				}break;
			}
		}break;
	case FT_EXCHANGE_ITEM_ETC:	
		{	// �κ� â �̿��� �޴��� �� ���ֹ�����.
			if( ::IsLeftWindowOpen() ) 	{::LeftClose();}
			Hcolor( FONT_COLOR_NAME );
			SetHangulAlign( TA_CENTER );			
			::Hprint2( iMainX+306+112/2,iMainY+65, g_DestBackBuf, "%s", SCharacterData.sCharacterName );//�ڽ� �̸�
			::Hprint2( iMainX+23+112/2,iMainY+65, g_DestBackBuf, "%s", ExchangeMgr.GetExchangeTargetName() );//��ȯ����� �̸�
		}break;
	case FT_MAN_TO_MAN_INFO_DISPLAY:
		{
			if( ::IsLeftWindowOpen() ) 	{::LeftClose();}
			char *target_name = SubMenu->nField[j].temp;//Ÿ���� ������ ���� �̱� ����
			LPCHARACTER ch = ::ExistHe( target_name );

			int iLineCt =0;
			const int iLineGab =20;
			const int iGabX =10;
			const int iGabY =5;
			const int iX = iMainX+iFieldX+iGabX;
			const int iY = iMainY+iFieldY+iGabY;

			Hcolor( FONT_COLOR_WHITE );
			SetHangulAlign( TA_CENTER );

			if(LEFT== nWillDo)
			{
				if( !ch ) // �������� �ʴ´ٸ�..
				{
					::SendManToManRefusal( SubMenu->key );
					::AddCurrentStatusMessage( 250, 100, 100, lan->OutputMessage(3,50));
					::MenuSoundClose( SubMenu->Id );
					break;
				}
				::Hprint2( iMainX+iFieldX+50,iMainY+64, g_DestBackBuf, "%s", ch->name );//�ڸ� ����
			}
			else
			{//if(!ch)break; �� �޾� ��� ������ �������� ���� Hero�� ������ �׳� ƨ����
				::Hprint2( iMainX+iFieldX+125,iMainY+64, g_DestBackBuf, "%s", ch->name );//�ڸ� ����
			}
			::SetHangulAlign( TA_LEFT );
			
			::PutLvNameByColor(iX,iY+iLineCt*iLineGab,ch);
			iLineCt++;

			::Hprint2( iX,iY+iLineCt*iLineGab, g_DestBackBuf, "Level : %d",ch->lv );
			iLineCt++;

			::Hprint2( iX,iY+iLineCt*iLineGab, g_DestBackBuf, "HP : %d",ch->_HpMax );
			iLineCt++;//���� ��°� �ȵ�
		}break;
	case FT_MAN_TO_MAN_CHECK :
		{
			const int iGabX=3,iGabY=3;
			if( nWillDo == LEFT ) 
			{
				FieldTypeNomalPut(iMainX+iGabX, iMainY+iGabY, iFieldX, iFieldY, iImgNo, iImgType );
			}
			else
			{
				if( iWork == 1 )		// ���� ��û�� �޾�������.
				{
					if( fRcMouse || fLBDown )
					{
						FieldTypeNomalPut(iMainX+iGabX, iMainY+iGabY, iFieldX, iFieldY, iImgNo, iImgType);
					}
				}
			}
		}break;
	case FT_GM_LIST : 
		{
			int do_search = 0;		//  explain��ư�� ���� ���� �ϴ���
			if( !SubMenu->CheakType )
			{
				SubMenu->CheakType = 1;
				SetGMskill();
				do_search = 1;
				for( int a=0; a<5; a++ ) //3~7 �ʵ� �켱 �ʵ�Ÿ�� ������
				{
					SubMenu->nField[3+a].nType=FT_NO_CHECK;//�ʵ� ���
				}
			}
			if( !GM_temp_list_count ) 
			{
				Hcolor( FONT_COLOR_NUMBER );
				::RectTextPut(iMainX+59, iMainY+165, 157, lan->OutputMessage(7,89) );//��� �� �� ����
				break;
			}

			static int ct = iWork;

			if( ct != iWork )		// ��ũ�� ���� ���
			{
				do_search = 1;
				ct = iWork;
			}
			// 1.���� gm���� ����ؾ��� ��ų ����					// GM_temp_list[ct]
			// 2. �� ��ų�� �ʿ��� ������ �� ����Ʈ ����			// g_GmMain
			int gm_list = GM_temp_list[ct];

			GM_QUEST *pGmMain = &g_GmMain[gm_list];

			if( do_search )
			{
				for( int a=0; a<MAX_GM_NEED_ITEM_COUNT; a++ )
				{
					const int iItemNo = pGmMain->need_item[a];
					if( iItemNo )
					{
						int para = g_GM_Item.SearchById( iItemNo );
						if( para == -1 )
						{
							SubMenu->nField[3+a].nType=FT_NO_CHECK;
						}
						else 
						{
							SubMenu->nField[3+a].nSHideNomalCount=0;
							SubMenu->nField[3+a].nType=FT_GM_ITEM_EXPLAIN_BUTTON;
						}
					}
					else
					{
						SubMenu->nField[3+a].nType=FT_NO_CHECK;
					}
				}
			}

			::DisplayGmRegistRequest(gm_list,iMainX,iMainY);

			if( GM_temp_list_count > 1 )		// ��ũ�� ��ư ��� �ش�.
			{
				FieldTypeNomalPut(iMainX, iMainY, iFieldX, iFieldY, iImgNo, iImgType);
				if( ct-1 < 0 )
				{
					SubMenu->nField[6].nType=FT_NOMAL_PUT;
				}
				else 
				{
					SubMenu->nField[6].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
				}

				if( ct+1 >= GM_temp_list_count ) 
				{
					SubMenu->nField[7].nType=FT_NOMAL_PUT;
				}
				else 
				{
					SubMenu->nField[7].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
				}
			}
			else 
			{
				SubMenu->nField[6].nType=FT_NO_CHECK;
				SubMenu->nField[7].nType=FT_NO_CHECK;
			}
		}break;
	case FT_DISPLAY_GM_REGIST:
		{
			::DisplayGmRegistRequest(iWork,iMainX,iMainY);
		}break;
	case FT_GM_ITEM_EXPLAIN_BUTTON:
		{
			int &ct= SubMenu->nField[j].nSHideNomalCount;
			ct++;
			if(ct < 15)
			{
				FieldTypeNomalPut(iMainX, iMainY, iFieldX, iFieldY, iRcImg, iImgType);
			}
			if(ct > 30)	ct = 0;
			if(fRcMouse || fLBDown)
				FieldTypeNomalPut(iMainX, iMainY, iFieldX, iFieldY, iImgNo, iImgType);
		}break;
	case FT_GM_ITEM_EXPLAIN : 
		{
			int width = SubMenu->nField[j].nWillDo;		// �� ��� ���� 
			int para = SubMenu->key;		// ���ܰ� �޴����� ������ g_GM_Item ���� ����� �Ķ����
			int &scroll = iWork;		// ��ũ���ϰ�� �� ��ũ�� ��ȣ
			int &page_max = SubMenu->nField[0].nSHideNomalCount;		// �� ���������� ������ ����
			if( SubMenu->CheakType == 0 )
			{
				SubMenu->CheakType = 1;
				int line = GetLineRectTextPut( width, g_GM_Item.GetExplain(para) );
				page_max = (line-1)/8 +1 ;
				scroll = 0;
			}
			if( page_max > 1 )
			{
				static int count = 0;
				if( scroll-1 >= 0 )		// ȭ��ǥ ���� ����̱�
				{
					SubMenu->nField[3].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
					if( (count%14) < 7  )
						FieldTypeNomalPut(iMainX, iMainY, SubMenu->nField[3].x, SubMenu->nField[3].y, SubMenu->nField[3].nImageNumber, SubMenu->nField[3].nImageType);
				}
				else SubMenu->nField[3].nType=FT_NO_CHECK;

				if( scroll+1 < page_max )		// ȭ��ǥ �Ʒ��� ����̱�
				{
					SubMenu->nField[4].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
					if( (count%14) < 7  )
						FieldTypeNomalPut(iMainX, iMainY, SubMenu->nField[4].x, SubMenu->nField[4].y, SubMenu->nField[4].nImageNumber, SubMenu->nField[4].nImageType);
				}
				else SubMenu->nField[4].nType=FT_NO_CHECK;

				count++;
			}

			Hcolor( FONT_COLOR_NAME );
			SetHangulAlign( TA_CENTER );
			int gaby = 0;
			Hprint2( iMainX+137, iMainY+127+gaby*18, g_DestBackBuf, "%s", g_GM_Item.GetName(para) );gaby++;
			SetHangulAlign( TA_LEFT);
			Hcolor( FONT_COLOR_NUMBER );
			
			SkipRectTextPut( iMainX+iFieldX, iMainY+127+gaby*18, width, scroll*8, g_GM_Item.GetExplain(para), 8 );
		}break;
	case FT_QUICK_SKILL_PUT : //工作技能圖示
		{
			int ret = 0;
			if( !SubMenu->CheakType ) ret = 1;		// ��ó�� �����Ѵٸ�...
			
			int x = iMainX+iFieldX;
			int y = iMainY+iFieldY;

			int old_start = SubMenu->nField[j].nSHideNomalStart;
			int old_max = SubMenu->nField[j].nSHideNomalCount;
			static int old_start2 = old_start; 

			static int work = iWork;
			static int scroll_max[4] = {0,};
			static int scroll_count[4] = {0,};
			if( !SubMenu->CheakType || work != iWork || old_start2 != old_start )
			{
				scroll_count[work] = old_start;
				old_start2 = old_start;
				work = iWork;
				SubMenu->CheakType = 1;
				scroll_max[work] = SetQuickSkill( skill_index, scroll_count[work], work );
			}

			int &start = SubMenu->nField[j].nSHideNomalStart;
			int &max = SubMenu->nField[j].nSHideNomalCount;
			start = scroll_count[work];
			max = scroll_max[work];

			FieldTypeNomalPut(iMainX, iMainY,51,2, iWork+12, SKILL_MENU );		// ���� ������ ���
			if( !max ) return;

			FieldTypeNomalPut(0,0,x,y, 3, SKILL_MENU );		// ����
			int count = 0;
			int gabx = 0;
			int size = 0;

			while( count < 5 && skill_index[ count ] )
			{
				Spr *s = GetSprOfMenu( SKILL_MENU, 4);
				if( !s ) continue;
				size = s->xl;
				gabx = size*count;
				FieldTypeNomalPut(0,0,x+4+gabx,y, 4, SKILL_MENU );		// ���...
				PutSkillIcon( x+4+gabx+20, y+22, skill_index[ count ] );
				DisplayQuickMemory(QM_SKILL,skill_index[ count ], x+4+gabx+20-8,y+22-4);//020610 lsw ���� ����
				count++;
				gabx = s->xl*count;
			}
			FieldTypeNomalPut(0,0,x+4+gabx,y, 5, SKILL_MENU );		// ������
			static int old_gabx = gabx;
			if( ret || old_gabx != gabx )
			{
				::SetRect(SubMenu->nField[j].rCheakBox,iFieldX,iFieldY,iFieldX+3+gabx,iFieldY+44);//021111 lsw
				old_gabx = gabx;
			}
			///////////////////////////////////////////////////////////////////////////
			if( SubMenu->nField[j].fRectMouse || SubMenu->nField[j].fLButtonDown )
			{
				if( size )
				{
					int ItemX = ( g_pointMouseX - (x+4) )/size;
					PutSkillIcon( x+4+size*ItemX+20, y+22, skill_index[ ItemX ], 8, 2 );
					CheckSkillExplain( skill_index[ ItemX ] );
				}
			}
			////////////////////////////////////////////////////////////////////////////
			if( max >= 5 )
			{
				SubMenu->nField[iSwd].nType = FT_QUICK_SKILL_SCROLL_LR;
				SubMenu->nField[iSwd+1].nType = FT_QUICK_SKILL_SCROLL_LR;
			}
			else 
			{
				SubMenu->nField[iSwd].nType = FT_NO_CHECK;
				SubMenu->nField[iSwd+1].nType = FT_NO_CHECK;
			}
			if( SubMenu->nField[j].fRectMouse )
			{
				int x = iMainX+iFieldX;
				int ItemX = ( g_pointMouseX - (x+4) )/38;
				int s = skill_index[ItemX];
				
				if( !s || !skill[s].num ) break;
				SetQuickMemoryByKeyInput(QM_SKILL,s);
			}
		}break;	
	case FT_GM_2ND_SKILL_MAKE_ITEM : 
		{
			for( int a=0; a<MAX_UPGRADE_RARE; a++ )
			{
				if( gm_make_item_list[a].item_no ) 
					PutItemIcon(	SubMenu->x	+	gm_make_item_list[a].x	,//+	SubMenu->nField[j].x, 
									SubMenu->y	+	gm_make_item_list[a].y	,//+	SubMenu->nField[j].y,
									gm_make_item_list[a].item_no,30,1 );
			}
			break;
		}
	case FT_SKILL_MASTER_MAKE_ITEM://021111 lsw
		{
			for( int a=0; a<2; a++ )
			{
				const ItemAttr Item = g_MultiRareSrc.MultiRareSrc[a].attr ;
				const int iItemNo = Item.item_no;
				if( iItemNo ) 
				{
					PutItemIcon(	iMainX	+	g_MultiRareSrc.MultiRareSrc[a].x	,//+	SubMenu->nField[j].x, 
									iMainY	+	g_MultiRareSrc.MultiRareSrc[a].y	,//+	SubMenu->nField[j].y,
									iItemNo,30,1 );
				}
			}
		}break;
	case FT_EXCHANGE_BOND_MONEY_ITEM:
		{
			const ItemAttr *pItem = Auction.GetBondItemAttr() ;
			if(pItem)
			{
				const int iItemNo = pItem->item_no;
				if( iItemNo ) 
				{
					::PutItemIcon(	iMainX	+	iFieldX+100,
									iMainY	+	iFieldY+30,
									iItemNo,30,1 );
				}

			}
		}break;
	case FT_MAIL_READ : 
		{
			std::string text;
			char szTime[MAX_PATH]={0,};
			switch( nWillDo )
			{
			case 1: 
				{
					text = g_MailMgr.GetTempMailSender();
				}break;
			case 2: 
				{
					text = g_MailMgr.GetTempMailTitle(); 
				}break;
			case 3: 
				{
					text =g_MailMgr.GetTempMailBody(); 
				}break; 
			case 4: 
				{
					g_MailMgr.GetTempMailDate(true,szTime); 
					text = szTime;
				}break; 
			default:{}break;
			}

			// 031110 YGI
			RectTextPut( iMainX+iFieldX, iMainY+iFieldY, SubMenu->nField[j].nRectImage, const_cast<char*>(text.c_str()) , 15, 15 ); // CSD-030723
		}break;
	case FT_MAIL_WRITE:
		{	
			int willdo = SubMenu->nField[j].nWillDo;
			int &count = SubMenu->nField[j].nSHideNomalCount;

			if (SubMenu->Id == MN_GUILD_REGIST_IMSI && !EWndMgr.checkInputBoxVisibility(HWND_GUILD_NAME)) {
				EWndMgr.showInputBox(HWND_GUILD_INTRO);
				EWndMgr.showInputBox(HWND_GUILD_NAME);
			}

			/*if( !SubMenu->CheakType )
			{
				SubMenu->CheakType = 1;

				
				SubMenu->work = 1;
			}

			char text[2048] = {0,};
			int max = 0;
			int iHWndIndex = 0;//021001 lsw
			HWND hWnd = EWndMgr.GetHWnd(iHWndIndex);
			switch( willdo )
			{
				case 1: iHWndIndex = HWND_MAIL1; max = 20;   break;
				case 2: iHWndIndex = HWND_MAIL2; max = 29;  break;
				case 3: iHWndIndex = HWND_MAIL3; max = 1000; break;
				default: break;
			}

			EWndMgr.GetTxt(iHWndIndex, text, max );
//			EWndMgr.SetTxt(iHWndIndex,text);//021001 lsw

			if( SubMenu->work == willdo )
				SetFocus2( iHWndIndex );

			ChangeString( text, '\r', ' ');
			//RectTextPut(iFieldX+iMainX, iFieldY+iMainY,iRcImg, text, 15, 15);
			TextOutInRect(text, iFieldX+iMainX, iFieldY+iMainY,iRcImg, 15);

			if( SubMenu->work == willdo )
			{
				
				count++;
			}*/

		}break;
	case FT_MAIL_LIST:
		{	//Ÿ��, �������, ����, ��¥
			for(int i = 0; MAX_MAIL_PAGE > i; i++)
			{
				if(!g_MailMgr.GetMailIndex(i))//�ε��� �� ���� ���̸� ����
				{
					continue;
				}
				
				const WORD wDefaultColor = (g_MailMgr.GetMailIsRead(i)?FONT_COLOR_DEFAULT:FONT_COLOR_WHITE);
				
				Hcolor(wDefaultColor);
				const int iX = iMainX+iFieldX;
				const int iY = iMainY+iFieldY+ CMailMgr::LIST_LINE_GAB*i +6;//���� ����

				//���õ� ��ũ
				const bool bIsSelect = g_MailMgr.IsSelected(i);
				if(bIsSelect)
				{
					::Hprint2(iX+6,iY-1,g_DestBackBuf,"X");
				}

				//������
				const int iMailType = g_MailMgr.GetMailType(i);
				const int iMailIconNo = g_MailMgr.GetMailIconNo(iMailType);
				::FieldTypeNomalPut(iX+25,iY-4,0,0,iMailIconNo,iImgType);

				if( ::BoxAndDotCrash( iX+25, iY-4, 20, 20, g_pointMouseX, g_pointMouseY ) )
				{
					::DisplayMailIconExplain(iMainX-150,iMainY+10,iMailType);
				}

				//�������
				const std::string szSender = g_MailMgr.GetMailSender(i);

				if( BoxAndDotCrash( iX+53,iY-3, 100, 18, g_pointMouseX, g_pointMouseY ) )
				{
					Hcolor(FONT_COLOR_BLIGHT_BLUE);
				}
				else
				{
					Hcolor(wDefaultColor);
				}

				::Hprint2(iX+53,iY,g_DestBackBuf, const_cast<char*>(szSender.c_str()));

				//����
				if( BoxAndDotCrash( iX+162,iY-3, 130, 18, g_pointMouseX, g_pointMouseY ) )
				{
					::Hcolor(FONT_COLOR_RED);
				}
				else
				{
					::Hcolor(wDefaultColor);
				}

				std::string szTitle = g_MailMgr.GetMailTitle(i);
				
				::Hprint2(iX+162,iY,g_DestBackBuf, const_cast<char*>(szTitle.c_str()));

				Hcolor(wDefaultColor);
				char szTime[MAX_PATH]= {0,};
				g_MailMgr.GetMailDate(i,false,szTime);
				::Hprint2(iX+299,iY,g_DestBackBuf, "%s", szTime);
			}
		}break;
	case FT_SELECT_BETTING_TARGET_TEAM:
		{
			const int iTarget = GetArenaBettingTarget();
			if(nWillDo == iTarget)
			{
				::FieldTypeNomalPut(iMainX, iMainY, iFieldX, iFieldY, iImgNo, iImgType);
			}
		}break;
	case FT_SELECT_OBSERVER_TARGET_TEAM:
		{	//< CSD-030521
			if (g_pArenaManager->IsEnableGambling(Hero->id))
			{
				const int iTarget = GetArenaObserverTarget();
			
				if (nWillDo == iTarget)
				{
					::FieldTypeNomalPut(iMainX, iMainY, iFieldX, iFieldY, iImgNo, iImgType);
				}
			}

			break;
		}	//> CSD-030521
	case FT_BATTLE_WAIT : 
		{
		}break;
	case FT_FIGHTMAP_START: 
		{	//< CSD-030521
			if (!g_pArenaManager->IsLeader(Hero->id))
			{
				break;
			}

			if(SubMenu->nField[j].fLButtonDown)
			{
				FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType);
			}
			else if(SubMenu->nField[j].fRectMouse)
			{
				FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nRectImage, SubMenu->nField[j].nImageType);
			}									
			else
			{
				int &count = SubMenu->nField[j].nSHideNomalCount;
				count++;
				if( count > 3 )
				{
					FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nRectImage, SubMenu->nField[j].nImageType);
					if( count > 6 ) count = 0;
				}										
			}

			break;
		}	//> CSD-030521
	case FT_VIEW_BATTLE_MEMBER: 
		{	//< CSD-030521
			int gab = SubMenu->nField[j].nSHideNomalStart;		// ��ũ��
			int &max = SubMenu->nField[j].nSHideNomalCount;

			CArenaTeam* pTeam = g_pArenaManager->GetArenaTeam(nWillDo);
			
			if (pTeam == NULL)
			{
				break;
			}

			max = pTeam->GetMemberCount()/4 + 1;
			
			CArenaTeam::LIST_MEMBER vtTeam = pTeam->GetMemberSet();
			int nCount = 0;

			for (CArenaTeam::ITOR_MEMBER i = vtTeam.begin(); i != vtTeam.end(); ++i)
			{
				LPCHARACTER pMember = ::ReturnCharacterPoint(*i);
	
				if (pMember == NULL)
				{
					continue;
				}

				char* name = pMember->name;
				const int iPutY = iMainY + iFieldY + nCount*18;

				if (g_pArenaManager->IsLeader(*i))
				{
					Hcolor(FONT_COLOR_NAME);
				}
				else
				{
					Hcolor(FONT_COLOR_NUMBER);
				}

				RectTextPut( iMainX+iFieldX, iPutY, 75, name, 18, 1 );

				if (SubMenu->nField[j].fRectMouse && BoxAndDotCrash(iMainX+iFieldX, iPutY, 100, 20, g_pointMouseX, g_pointMouseY))
				{	//< CSD-030520
					/*
					if( g_FightMapMember.m_dwTotalId[nWillDo][i] )
					{
						//g_ViewFightMapCharInfo = true;
						ViewFightMapCharInfo( 
							g_FightMapMember.m_Name[nWillDo][i], 
							g_FightMapMember.m_dwTotalId[nWillDo][i],
							iMainX-150,
							iMainY+iFieldY );
					}
					*/
				}	//> CSD-030520

				if (++nCount >= 7)
				{
					break;
				}
			}

			break;
		}	//> CSD-030521
	case FT_VIEW_BATTLE_WAIT_MEMBER: 
		{	//< CSD-030521
			int ct = 0;
			CArenaLobby::LIST_MEMBER ltLobby =  g_pArenaManager->GetLobbyInfo()->GetLobbyMemberSet();

			for (CArenaLobby::ITOR_MEMBER i = ltLobby.begin(); i != ltLobby.end(); ++i)
			{
				LPCHARACTER pMember = ::ReturnCharacterPoint(*i);
	
				if (pMember == NULL)
				{
					continue;
				}

				char* pName = pMember->name;

				CArenaTeam* pTeam = g_pArenaManager->GetObserveTeam(*i);

				if (pTeam == NULL)
				{
					::Hcolor(FONT_COLOR_WHITE);
				}
				else
				{
					switch (pTeam->GetTeamNumber())
					{
					case 0:
						{
							::Hcolor(FONT_COLOR_SOLID_RED);
							break;
						}
					case 1:
						{
							::Hcolor(FONT_COLOR_SOLID_BLUE);
							break;
						}
					}
				}

				::Hprint2(iMainX+iFieldX+(ct%4)*100, iMainY+iFieldY+(ct/4)*18, g_DestBackBuf, "%s", pName);
				++ct;
			}

			break;
		 }	//> CSD-030521
	case FT_ARENA_BETTING:
		{
		if (!EWndMgr.checkInputBoxVisibility(HWND_3)) {
			EWndMgr.showInputBox(HWND_3);
		}
		char temp[20] = { 0, };
		DWORD dwMoney = 0;
		std::string money = EWndMgr.GetTxt(HWND_3);
		//EatRearWhiteChar(temp);
		try {
			dwMoney = atol(money.c_str());
		}
		catch (...)
		{
			break;
		}
			bool bIsForce = false;
			if(SCharacterData.BankMoney < dwMoney)
			{
				dwMoney = SCharacterData.BankMoney;
				bIsForce = true;
			}
			
			if( bIsForce || CheckNumber( temp, strlen( temp ) ) )
			{	
				sprintf( temp, "%d", dwMoney );
				EWndMgr.SetTxt(HWND_3,temp);//021001 lsw
			}

			::PrintMoney( SubMenu, j, dwMoney, SubMenu->nField[j].nRectImage );
	   }break;
	default: break;
	}
}

void lsw_FT_DATA_PUT(SMENU *SubMenu ,int j)
{
	int i = 0;
	int k = 0;
	int nWeapon_Type;													//���� ���� �Ǵ��� ���� ���� (����, ����, ���, ����)
	BOOL flag=TRUE; int & ability=nWeapon_Type;							// ���ΰ� �ɷ�ġ ����� ���� �ӽ� ����

	int &iMainX = SubMenu->x;
	int &iMainY = SubMenu->y;
	int &iFieldX = SubMenu->nField[j].x;
	int &iFieldY = SubMenu->nField[j].y;
	
	int &nWillDo = SubMenu->nField[j].nWillDo;
	int &iImgType = SubMenu->nField[j].nImageType;

	int &nRcImg = SubMenu->nField[j].nRectImage;

	switch(SubMenu->nField[j].nImageNumber)
	{
	case DP_RECT_STRING_PUT_CUSTOM ://�� ���� ���°� //011218 lsw
		{
			const int iHeight= iImgType;//�̹��� Ÿ���� �� ����
			const int iColor = nWillDo;
			if( iColor )
			{
				Hcolor( iColor );
			}
			(iHeight)?
				RectTextPut(iMainX+iFieldX,iMainY+iFieldY,nRcImg,SubMenu->nField[j].temp,iHeight): 
				RectTextPut(iMainX+iFieldX,iMainY+iFieldY,nRcImg,SubMenu->nField[j].temp); 
			break;
		}
	//011008 lsw >
	case DP_GM_2ND_SKILL_TARGET_ITEM_PUT://單G
		{
			int iLineGab = 15;
			if(!gm_make_item_result.item_no){break;}
	//			::PutItemIcon(iMainX+iFieldX ,iMainY+iFieldY+ 10, gm_make_item_result.item_no);	
			memcpy(&Soksungattr_gm,&gm_make_item_result.attr.attr[IATTR_RARE_MAIN],sizeof(int));
			int count=0;
			CItem *item = ItemUnit( gm_make_item_result.attr );

			Hcolor( FONT_COLOR_RARE_MAIN );
			
			Hprint2(iMainX+34, iMainY+90 + count * iLineGab, g_DestBackBuf,"%s",(Soksungattr_gm.IsDynamicRare)? ItemFunction[Soksungattr_gm.soksung1].NameAdd:ItemRare[Soksungattr_gm.soksung1].LocalName);//020505 lsw
			count++;

			Hprint2(iMainX+34, iMainY+90 + count * iLineGab, g_DestBackBuf,"%d Grade",Soksungattr_gm.grade+1 );
			count++;
            if (system_info.lang==0)
			HprintBold( iMainX+34,iMainY+90+count* iLineGab, FONT_COLOR_NAME, RGB16( 0,0,0 ),"%s",item->GetItemName());
            else
			HprintBold( iMainX+34,iMainY+90+count* iLineGab, FONT_COLOR_NAME, RGB16( 0,0,0 ),"%s",item->GetItemHanName());
            
			
			count = 0;
			// x�� ���� ����
			//�䱸 ������  x/1;
			//��� ������  x/ 100%�Ǵ� ����;
			int ineedItemCount1 = 0;
			int ineedItemCount2 = 0;
			int iTargetItemCount= 0;
			const int iResultItemKind = GetItemAttr( gm_make_item_result.item_no, ITEM_KIND);
			if( MAX_ITEM_MUTANT_KIND <= iResultItemKind || 0 > iResultItemKind )
			{
				break;
			}

			const int iNeedItemNo1 = ItemMutantKind[iResultItemKind].AddItem[0]/100;
			const int iNeedItemNo2 = ItemMutantKind[iResultItemKind].AddItem[1]/100;//020505 lsw
			
			for ( int xx = 0 ; xx < MAX_UPGRADE_RARE ; xx++)
			{
				if(iNeedItemNo1 && (iNeedItemNo1  == gm_make_item_list[xx].item_no  ))
				{
					ineedItemCount1++;
				}
				else if(iNeedItemNo2 && ( iNeedItemNo2  == gm_make_item_list[xx].item_no) )
				{
					ineedItemCount2++;
				}
				else if(gm_make_item_list[xx].item_no == gm_make_item_result.item_no)
				{
					iTargetItemCount++;
				}
			}

			const int iUpgradeType = ItemMutantKind[item->GetItemKind()].upgrade_type - 1;//020505 lsw -1 �켭 ���� �϶�
			if(0 > iUpgradeType ) {break;}//���� �ȵǴ°ǵ� �Ѥ�;;
			
			const int iGrade		= Soksungattr_gm.grade+1;

			int iPerpectMuch = ItemGrade[iGrade].ResourceAmount[1]; // ResourceAmount[iUpgradeType];
			int iPerpectMuch2 = ItemGrade[iGrade].ResourceAmount[0];
			if (iTargetItemCount > ItemGrade[iGrade].ResourceAmount[1])
			{
				break;
			}

			float fSuccessRate =0.000;
			Hcolor( FONT_COLOR_WHITE );
			Hprint2(iMainX+34, iMainY+205 + count * iLineGab, g_DestBackBuf,"%s",  GetItemHanName(iNeedItemNo1));
			if(ineedItemCount1< 0 ||  ineedItemCount1 > 1){  Hcolor( FONT_COLOR_PLUS );	}
			Hprint2(iMainX+34, iMainY+205 + count * iLineGab, g_DestBackBuf,"                        %d/1",ineedItemCount1);

			count++;
			if( (iNeedItemNo1 != iNeedItemNo2) && iNeedItemNo2)
			{
				Hcolor( FONT_COLOR_WHITE );
				Hprint2(iMainX+34, iMainY+205 + count * iLineGab, g_DestBackBuf,"%s",  GetItemHanName(iNeedItemNo2));
				if(ineedItemCount2< 0 ||  ineedItemCount2 > 1){   Hcolor( FONT_COLOR_PLUS );	}
				Hprint2(iMainX+34, iMainY+205 + count * iLineGab, g_DestBackBuf,"                        %d/1",ineedItemCount2);
				count++;
			}
			Hcolor( FONT_COLOR_RARE_MAIN );
			
			Hprint2(iMainX+34, iMainY+205 + count * iLineGab, g_DestBackBuf,"%s ", (Soksungattr_gm.IsDynamicRare)?ItemFunction[Soksungattr_gm.soksung1].NameAdd:ItemRare[Soksungattr_gm.soksung1].LocalName);
			Hprint2(iMainX+34, iMainY+205 + count * iLineGab, g_DestBackBuf,"                    %d Grade", Soksungattr_gm.grade);
			count++;

			Hcolor( FONT_COLOR_WHITE );
			if (system_info.lang==0)
			Hprint2(iMainX+34, iMainY+205 + count * iLineGab, g_DestBackBuf,"%s",item->GetItemName() );
			else
			Hprint2(iMainX+34, iMainY+205 + count * iLineGab, g_DestBackBuf,"%s",item->GetItemHanName() );
			if( iTargetItemCount > iPerpectMuch2 || iTargetItemCount == 0)
			{
				Hcolor( FONT_COLOR_PLUS );
			}
			Hprint2(iMainX+34, iMainY+205 + count * iLineGab, g_DestBackBuf,"                        %d/%d",iTargetItemCount,iPerpectMuch);
			count++;

			Hcolor( FONT_COLOR_WHITE );
			Hprint2(iMainX+34, iMainY+138 , g_DestBackBuf,"SuccessRate");

			if ( ineedItemCount1  && iNeedItemNo1)//�䱸 �������� ���� �ϰ� ������ ������ �ִ�
			{
				fSuccessRate = float(iTargetItemCount) / float(iPerpectMuch2);

				if((int)fSuccessRate && iNeedItemNo2)
				{
					if( ineedItemCount2 )//�ι�° �䱸ġ�� ���� �Ѵ�
					{
						fSuccessRate = float(iTargetItemCount) / float(iPerpectMuch2);
					}
					else
					{
						fSuccessRate = 0;
					}
				}
			}
			if( (fSuccessRate*100) < 50)
			{
				Hcolor( FONT_COLOR_PLUS );
				SMenu[MN_GM_2ND_SKILL].nField[0].nType = 0;
			}
			else
			{
				Hcolor( FONT_COLOR_YELLOW );
				SMenu[MN_GM_2ND_SKILL].nField[0].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
			}
			//成功率計算
			Hprint2(iMainX+79, iMainY+138, g_DestBackBuf,"             %3d%%",int(fSuccessRate*100));
		}break;//case DP_GM_2ND_SKILL_TARGET_ITEM_PUT:
	case	DP_SKILL_MASTER_RESULT_ITEM://���м��� ���� ��//雙G
		{	//����� ������ �̴� ��ƾ//����� �������� ������ �̸��� ��� ����
			const int iLGab = 18;
			
			ItemAttr Item1= g_MultiRareSrc.MultiRareSrc[0].attr;
			ItemAttr Item2= g_MultiRareSrc.MultiRareSrc[1].attr;
			
			if(!Item1.item_no ||!Item2.item_no){break;}//������ ��ȣ�� �ݵ���־�� �ϰ�
			if(Item1.item_no != Item2.item_no){break;}//������ ��ȣ�� ���ƾ���

			const RareMain *pTempRare1 =  (RareMain *)&Item1.attr[IATTR_RARE_MAIN];
			const RareMain *pTempRare2 =  (RareMain *)&Item2.attr[IATTR_RARE_MAIN];
			if(	pTempRare1->IsDynamicRare
			||	pTempRare2->IsDynamicRare
			||	pTempRare1->iHighLevel
			||	pTempRare2->iHighLevel){break;}//���̳� ��� ������ ��� �Ƚ���.. Ʋ���� �ŵ�
			
			RareMain RareResult;
			{//����� ����
				RareResult.grade = (pTempRare1->grade+pTempRare2->grade)/2;
				RareResult.soksung1=pTempRare1->soksung1;
				RareResult.soksung2=pTempRare2->soksung1;
				RareResult.soksung3=0;
				RareResult.iHighLevel = 0;
				RareResult.IsDynamicRare = false;
			}

			const RareMain *pRare = &RareResult;
			const int iGrade = pRare->grade;
			const int iWidth = TXT_BG_NORMAL_WIDTH;//021111 lsw

			CItem *item = ItemUnit( gm_make_item_result.attr );
			Hcolor( FONT_COLOR_RARE_MAIN );
			if(!aIMR[iGrade].iAble)
			{	//�ȵ�
				break;
			}
			int iCt= 0;
			::Hprint2(iMainX+34, iMainY+104 + iCt * iLGab, g_DestBackBuf,"%s",(pRare->IsDynamicRare)? ItemFunction[pRare->soksung1].NameAdd:ItemRare[pRare->soksung1].LocalName);//020505 lsw
			::Hprint2(iMainX+34, iMainY+104 + iCt * iLGab, g_DestBackBuf,"                     %d Grade",iGrade );
			iCt++;
			
			::Hprint2(iMainX+34, iMainY+104 + iCt * iLGab, g_DestBackBuf,"%s",(pRare->IsDynamicRare)? ItemFunction[pRare->soksung2].NameAdd:ItemRare[pRare->soksung2].LocalName);//020505 lsw
			::Hprint2(iMainX+34, iMainY+104 + iCt * iLGab, g_DestBackBuf,"                     %d Grade",iGrade );
			iCt++;
			if(system_info.lang==0)
			HprintBold( iMainX+34,iMainY+104+iCt* iLGab, FONT_COLOR_NAME, RGB16( 0,0,0 ),"%s",item->GetItemName());
			else
			HprintBold( iMainX+34,iMainY+104+iCt* iLGab, FONT_COLOR_NAME, RGB16( 0,0,0 ),"%s",item->GetItemHanName());
			
			iCt=0;

			const int iX = iMainX+iFieldX+5;
			const int iY = iMainY+iFieldY+150;

			bool bIsFail = false;

			if(Item1.item_no)
			{
				::Hprint2(iX, iY+ iCt * iLGab, g_DestBackBuf,"%s",(pTempRare1->IsDynamicRare)? ItemFunction[pTempRare1->soksung1].NameAdd:ItemRare[pTempRare1->soksung1].LocalName);//020505 lsw
				::Hprint2(iX, iY+ iCt * iLGab, g_DestBackBuf,"                     %d Grade",pTempRare1->grade );
				iCt++;
				if(system_info.lang==0)
				HprintBold( iX,iY+iCt* iLGab, FONT_COLOR_NAME, RGB16( 0,0,0 ),"%s",item->GetItemName());
				else
				HprintBold( iX,iY+iCt* iLGab, FONT_COLOR_NAME, RGB16( 0,0,0 ),"%s",item->GetItemHanName());
				iCt++;
			}
			else
			{
				bIsFail = true;
			}

			if(Item2.item_no)
			{
				::Hprint2(iX, iY+ iCt * iLGab, g_DestBackBuf,"%s",(pTempRare2->IsDynamicRare)? ItemFunction[pTempRare2->soksung1].NameAdd:ItemRare[pTempRare2->soksung1].LocalName);//020505 lsw
				::Hprint2(iX, iY+ iCt * iLGab, g_DestBackBuf,"                     %d Grade",pTempRare2->grade );
				iCt++;
			    if(system_info.lang==0)
				HprintBold( iX,iY+iCt* iLGab, FONT_COLOR_NAME, RGB16( 0,0,0 ),"%s",item->GetItemName());
			    else
				HprintBold( iX,iY+iCt* iLGab, FONT_COLOR_NAME, RGB16( 0,0,0 ),"%s",item->GetItemHanName());
				iCt++;
			}
			else
			{
				bIsFail = true;
			}
			
			for(int xx= 0;MAX_IMR_FIELD_CT>xx;xx++)
			{
				int iNationAddCt = 0;
				switch(Hero->name_status.nation)
				{
				case NW_BY:{if(xx == 0){ iNationAddCt = aIMR[iGrade].iAddCt;}}break;
				case NW_ZY:{if(xx == 1){ iNationAddCt = aIMR[iGrade].iAddCt;}}break;
				case NW_YL:{if(xx == 2){ iNationAddCt = aIMR[iGrade].iAddCt;}}break;
				}
				const int iNeedItemNo = aIMR[iGrade].aItemNo[xx];
				const int iNeedItemCt = aIMR[iGrade].aItemCt[xx]+iNationAddCt;
				if(iNeedItemNo && iNeedItemCt)
				{
					CItem *t = ItemUnit( iNeedItemNo );
					if(!t){continue;}
					const int iNowCt = ::CheckInventory(iNeedItemNo,iNeedItemCt);//���� ���� ������ ���� ����� �ִϱ� -���� ���� �� �ִ�
					int iRCT=0;
					if(system_info.lang==0)
					iRCT = TxtOut.RcTXTOutNoBG(iX,iY+iLGab*iCt,iWidth,0,"%s %d/%d",t->GetItemName(),iNowCt+iNeedItemCt,iNeedItemCt);
					else
					iRCT = TxtOut.RcTXTOutNoBG(iX,iY+iLGab*iCt,iWidth,0,"%s %d/%d",t->GetItemHanName(),iNowCt+iNeedItemCt,iNeedItemCt);
					iCt+=iRCT;
					if(0 > iNowCt)//��ᰡ ���ڶ�� ����
					{
						bIsFail = true;
					}
				}
			}			
			
			int Foundbook3 = SearchItemCount(1053);
			const int Foundbook1 = SearchItemCount(1051);
			if (Foundbook3 > 3) Foundbook3 = 3;
			const int ExtraSuccess = Foundbook3 * 5;
			const int Santa_Claus = SearchItemCount(10191);
			const bool SantaEventItem = Santa_Claus > 0 ? true : false;
			const int precent = 45;//♾️
			Hcolor( FONT_COLOR_WHITE );
			Hprint2(iMainX+34, iMainY+155 , g_DestBackBuf,"SuccessRate");
			if(bIsFail)
			{
				Hcolor( FONT_COLOR_PLUS );
				Hprint2(iMainX+79, iMainY+155, g_DestBackBuf,"             %3d",0);
			}
			else
			{
				Hcolor( FONT_COLOR_YELLOW );
				if (SantaEventItem) {
					Hprint2(iMainX + 79, iMainY + 155, g_DestBackBuf, "             ♾️+%3d%%", ExtraSuccess);
				}
				else {
					Hprint2(iMainX + 79, iMainY + 155, g_DestBackBuf, "             %3d+%3d%%", precent, ExtraSuccess);
				}
			}
			if (Foundbook3)
			{
				Hcolor(FONT_COLOR_PLUS);
				Hprint2(iX, iY + iLGab * iCt, g_DestBackBuf, lan->OutputMessage(2,810), ExtraSuccess);
			}
			else {
				Hcolor(FONT_COLOR_PLUS);
				Hprint2(iX, iY + iLGab * iCt, g_DestBackBuf, lan->OutputMessage(2, 812), ExtraSuccess);
			}

			if(Foundbook1)
			{
				Hcolor(FONT_COLOR_PLUS);
				Hprint2(iX, iY + iLGab * iCt + iLGab, g_DestBackBuf, lan->OutputMessage(2, 811));
			}
			else {
				Hcolor(FONT_COLOR_PLUS);
				Hprint2(iX, iY + iLGab * iCt + iLGab, g_DestBackBuf, lan->OutputMessage(2, 813));
			}

			if (SantaEventItem) {
				Hcolor(FONT_COLOR_RED);
				Hprint2(170, 225, g_DestBackBuf, lan->OutputMessage(0, 88));
			}
			else if (LOCK_ITEM)
			{
				Hcolor(FONT_COLOR_RED);
				Hprint2(170, 225, g_DestBackBuf, lan->OutputMessage(0, 80));
			}

		}break;//case	DP_SKILL_MASTER_RESULT_ITEM://�⼺�� ����� ������ ���÷���
	case	DP_NATION_WAR_RESULT_BAR:
		{
		//	if( nWillDo==0 )
		//	{	break;		}

			if( nWillDo > SubMenu->nField[j].nSpecialWillDo)
			{
	//				SubMenu->nField[j].nSpecialWillDo++;
				SubMenu->nField[j].nSpecialWillDo+=(rand()%10+1);//011031 lsw 
			}
			else
			{
				SubMenu->nField[j].nSpecialWillDo = nWillDo;
			}

			// 500 ����ؿ�
			float max = ( 250.000 * float(SubMenu->nField[j].nSpecialWillDo) / 3000.000 );
			for(int iCount = 0; iCount < std::min( (int)max, 250 ) ; iCount++)//011031 lsw
			{
				FieldTypeNomalPutFx3(iMainX, iMainY, iFieldX+iCount,iFieldY,nRcImg, NATION_WAR_RESULT_IMG, 6, 1);
			}
			Hprint2(iMainX+iFieldX, iMainY+iFieldY+3, g_DestBackBuf,"%d",SubMenu->nField[j].nSpecialWillDo);
		}break;
	case DP_NATION_WAR_RESULT_ANIMATION:
		{
			Spr *s1;
			Spr *s2;

			int iType = 	iImgType;

			int *iAniCount;
			int *iLoofStartFrame ;
			int *iLoofEndFrame	;
			int *iLoofOKFlag	;
			
			static int delayTime = 1;//delayTime�� 0�� �Ǹ� �ȵȴ�
			static int delayTimeValue = 0;

			iAniCount		=	&WarResultAniCount;
			iLoofStartFrame =	&nWillDo;			//������ ������
			iLoofEndFrame	= 	&nRcImg;		//������ ����
			iLoofOKFlag		=	&SubMenu->nField[j].nSpecialWillDo;	//���� ���� ����

			//���� ���ƶ� üũ�� ������ ������ �׳� �����Ѵ�
			//�ƽ�ġ ���� ���� ���� üũ�� �� �ְ� ���� �����Ӱ� ������ �������� ���Ѵ�
			//�÷��״� ������ ���� 2 �� �ְ� �ڷ� ���� -2 �� �ش�

			if( (*iLoofOKFlag) )//�÷��װ� ���� ������
			{
				if ( !(  delayTimeValue % delayTime) )
				{
					(*iAniCount) += (*iLoofOKFlag);
					delayTimeValue = 1;
				}

				delayTimeValue++;

				if( (*iAniCount) < (*iLoofStartFrame))
				{
					(*iLoofOKFlag) = 2;
				}
				else if( (*iAniCount) > (*iLoofEndFrame) )
				{
					(*iLoofOKFlag) = -2;
				}
			}
			else//�÷��� �������� (�ʱ�)
			{
				if(iType == 0)//�¸� �ÿ���
				{
					if( (*iAniCount) > 92 )//ī��Ʈ�²�! ¦���� �����
					{
						(*iLoofOKFlag)  = 2;
						(*iAniCount)	=	(*iLoofStartFrame)		= 94;
						(*iLoofEndFrame)		= 104;
						delayTime = 2;
					}
					iFieldX = 180;
					iFieldY = 250;
				}
				else
				{
					if( (*iAniCount) > 84 )//ī��Ʈ�²�! ¦���� �����
					{
						(*iLoofOKFlag)  = 2;
						(*iAniCount)	=	(*iLoofStartFrame)		= 48;
						(*iLoofEndFrame)		= 54;
						delayTime = 3;
					}
					iFieldX = 180;
					iFieldY = 300;
				}
				(*iAniCount)+=2;
			}
			//���� �ִϸ��̼� ����Ÿ ����
			if( iType == 0 )
			{
				if( (*iAniCount)==0 || (*iAniCount)==104) MP3( 2211 );
				if( (*iAniCount) == 44 ) MP3( 2204 );
				if( (*iAniCount) == 60  ) MP3( SN_FIGHT_WIN );
				s1 = GetSprOfMenu( FIGHT_MAP_WIN, (*iAniCount));
				s2 = GetSprOfMenu( FIGHT_MAP_WIN, (*iAniCount)+1);
			}
			else
			{
				if( (*iAniCount)==22 || (*iAniCount)==32 ) MP3( 302 );
				if( (*iAniCount) == 70 ) MP3( 2203 );
				if( (*iAniCount)== 84 ) MP3( SN_FIGHT_LOSE );
				s1 = GetSprOfMenu( FIGHT_MAP_LOSE, (*iAniCount));
				s2 = GetSprOfMenu( FIGHT_MAP_LOSE, (*iAniCount)+1);
			}

			//���
			if( s1 && s1->img ){PutCompressedImage	( iMainX+iFieldX, iMainX+iFieldY, s1 );}
			if( s2 && s2->img ){PutCompressedImageFX( iMainX+iFieldX, iMainX+iFieldY, s2, 17, 2 );}	
		}break;
	case DP_RARE_UPGRADE_RESULT:
		{
			if(nWillDo==100)
			{
				AddCurrentStatusMessage(255,255,255,lan->OutputMessage(2,510));
			//	AddCurrentStatusMessage(255,255,255,lan->OutputMessage(2,257));//011031 lsw
				nWillDo = 0;
				CloseAllMenu();	
			}
			
			else if( nWillDo < 100 && nWillDo > 0) 
			{
				//011030 lsw >
				AddCurrentStatusMessage(255,255,255,lan->OutputMessage(2,223));
		//		AddCurrentStatusMessage(255,255,255,lan->OutputMessage(2,253));//011031 lsw
				nWillDo = 0;
				//011030 lsw <
				CloseAllMenu();	
			}
		}break;
	//011008 lsw <
	case	DP_NATION_JOIN_STATUS_BAR:
		{
			float max = ( 150.000 * float(nWillDo) / 200.000 );
			
			for(i = 0; i < max; i++)//���� ���� �Ǵ°�
			{
				 FieldTypeNomalPutFx3(iMainX, iMainY, iFieldX,iFieldY-i,nRcImg, NATION_JOIN_IMG, 15, 1);
				 if(i > 150 ) break;
			}
		}
		break;
	case DP_REMAIN_TIME_PUT:
		{
			static int NationTime = SDL_GetTicks();

			if(	SDL_GetTicks() - NationTime> 1000)
			{
				NationTime = SDL_GetTicks();
				if(nRcImg > 0)
				{
				nRcImg --;
				}
			}

			int RemainTime  = nRcImg; 

			DWORD temp =static_cast<DWORD>(RemainTime % 3600);
			Hprint2(iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf,"%2d:%2d:%2d",char(RemainTime / 3600),char(temp / 60),char(temp % 60));
		}break;
	case	DP_RARE_SUCCESS_RATE_DISPLAY_NO_TOTAL://普通做物品介面顯示函式
		{
			int JoinNo = item_select_result.join_no;
			
			int sno = SkillTable[SkillNo].MotherSkillType;
			//011031 lsw>
			if( JoinNo && SkillNo && sno && item_select_result.item_no)
			{
				int myskillLv = SCharacterData.nJobSkill[sno].skillexp/10000;
				int itemLv = item_join[JoinNo].skill1_minimum;
				
				CItem *t = ItemUnit(item_select_result.item_no);
				if(!t) break;
				int iFuncItem = 0;
				switch(item_select_result.item_no/1000)
				{
				case PLANT			:
					{
						iFuncItem = ((CItem_Plant*)t)->cure_disease1;
					}break;
				case HERB			:
					{
						iFuncItem = ((CItem_Herb*)t)->cure_disease1;
					//	cure_disease1
					}break;
				case COOK			:
					{
						iFuncItem = ((CItem_Cook*)t)->cure_disease1;
					//	cure_disease1
					}break;
				case POTION			:
					{
						iFuncItem = ((CItem_Potion*)t)->cure_disease1;
					//	cure_disease1
					}break;
				case MINERAL		:
				case TOOL			:
				case WEAPON			:
				case DISPOSABLE		:
				case ARMOR			:
				case ACCESSORY		:
				case ETC			:
				default:break;
				}
				if(iFuncItem)
				{
					Hprint2(iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf,
					"%s", ItemFunction[iFuncItem].NameAdd);
					break;
				}

				int itemKind = 	ItemMutantKind[t->GetItemKind()].upgrade_type-1;// -1 �� ������ ������׷��̵� �Ұ�
				
				if(itemKind > 3 || itemKind < 0)
				{
					Hprint2(iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf,
					"%s", lan->OutputMessage(2,521));
				}
				else
				{
					const int ExtraProbability = RareProbability[myskillLv - itemLv].bonus_suc / 10;
					const int baseProbability = SCharacterData.nJobSkill[sno].rare / 10;

					int r = baseProbability + ExtraProbability;
					if( r > 100 )
					{	r = 100;	}
					if( r < 0	)
					{	r = 0;		}
					Hcolor( FONT_COLOR_WHITE );
					Hprint2(iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf,
					lan->OutputMessage(2,520),
						baseProbability,
						baseProbability %10,
						ExtraProbability
					);

					Hcolor( FONT_COLOR_YELLOW );
					Hprint2(iMainX+iFieldX+100, iMainY+iFieldY, g_DestBackBuf,
					"%3d ", ExtraProbability);
					Hcolor( FONT_COLOR_WHITE );
				}
			}
		}break;

	case	DP_RARE_SUCCESS_RATE_DISPLAY://整單G 面板顯示
		{
			int JoinNo = item_select_result.join_no;
			
			int sno = SkillTable[SkillNo].MotherSkillType;

			if( JoinNo && SkillNo && sno)
			{
				int myskillLv = SCharacterData.nJobSkill[sno].skillexp/10000;
				int itemLv = item_join[JoinNo].skill1_minimum;
				
				CItem *t = ItemUnit(item_select_result.item_no/1000,item_select_result.item_no%1000);
				if(!t) break;

				int itemKind = 	ItemMutantKind[t->GetItemKind()].upgrade_type-1;// -1 �� ������ ������׷��̵� �Ұ�
				
				if(itemKind > 3 || itemKind < 0)
				{
					Hprint2(iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf,
					"%s", lan->OutputMessage(2,254));
				}
				else
				{
					t_SkillExp2 *dayexp ;
					dayexp = (t_SkillExp2*)&SCharacterData.nJobSkill[sno-14];

					if( dayexp->makecount > RareProbability[myskillLv].max_rare_day)
					{
						dayexp->makecount  = RareProbability[myskillLv].max_rare_day;
					}
					//011030 lsw >
					int r = SCharacterData.nJobSkill[sno].rare/10+RareProbability[myskillLv - itemLv].bonus_suc/10;
					if( r > 100 )
					{	r = 100;	}
					if( r < 0	)
					{	r = 0;		}
					//011110 lsw
					if(dayexp->makecount < RareProbability[myskillLv].max_rare_day )
					{
						Hprint2(iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf,
						lan->OutputMessage(2,255),
						//rare_probability[myskillLv - itemLv].exp_rare_suc[itemKind], 
						SCharacterData.nJobSkill[sno].rare/10,
						SCharacterData.nJobSkill[sno].rare%10,
						RareProbability[myskillLv - itemLv].bonus_suc/10, 
						RareProbability[myskillLv - itemLv].bonus_suc%10, 
						r,
						dayexp->makecount,
						RareProbability[myskillLv].max_rare_day
						);
					}
					else
					{
						Hprint2(iMainX+iFieldX+100, iMainY+iFieldY, g_DestBackBuf,
						lan->OutputMessage(2,265),
						//rare_probability[myskillLv - itemLv].exp_rare_suc[itemKind], 
						SCharacterData.nJobSkill[sno].rare/10,
						SCharacterData.nJobSkill[sno].rare%10,
						RareProbability[myskillLv - itemLv].bonus_suc/10, 
						RareProbability[myskillLv - itemLv].bonus_suc%10, 
						r
						);
					}

					if (LOCK_ITEM) 
					{
						Hcolor(FONT_COLOR_RED);
						Hprint2(170, 225, g_DestBackBuf, lan->OutputMessage(0, 80));
					}
					//011030 lsw <
				}
			}
		}
		break;
	case DP_HEALTH_GAUGE :	
		{
			if( SCharacterData.nCharacterMAXSP > 0 )
			{	
				const int iMax = SCharacterData.nCharacterMAXSP;
				const int iNow = SCharacterData.nCharacterSP;

				int iTarget = static_cast<int>(((float)iNow) / ((float)iMax) * ((float)SPBarWidth)); 
				if( SPBarWidth < iTarget){iTarget = SPBarWidth;}
				for(int iCount = 0; iCount < iTarget; iCount++)//011031 lsw
				{
					FieldTypeNomalPut(iMainX, iMainY, iFieldX+iCount, iFieldY, 							
						nRcImg, iImgType);
				}
			}
		}break;
	case DP_HP_BAR :	
		{
			//int realiNow = 0;
			if( SCharacterData.nCharacterMAXHP > 0 )
			{
				const int iMax = SCharacterData.nCharacterMAXHP;
				const int iNow = /*(Hero->hp > iMax) ? iMax : Hero->hp;*/SCharacterData.nCharacterHP/*Hero->hp*/; //Eleval 18/08/09 - HP BUG
	
				int iTarget = static_cast<int>(((float)iNow) / ((float)iMax) * ((float)HPMPBarHeight)); 
				if( HPMPBarHeight < iTarget){iTarget = HPMPBarHeight;}
				FieldTypeNomalPut(iMainX, iMainY, iFieldX, iFieldY,
						0, iImgType);
				for(int iCount = 0; iCount < iTarget; iCount++)//011031 lsw
				{
					FieldTypeNomalPutFx3(iMainX+4, iMainY+HPMPBarHeight+17, iFieldX, iFieldY-iCount,
						nRcImg, iImgType,13,1);
				}					
				FieldTypeNomalPut(iMainX, iMainY,iFieldX, iFieldY,
						1, iImgType);

				//realiNow = iNow;
			}
			SetHangulAlign( TA_CENTER );
			Hcolor( FONT_COLOR_WHITE );
            int hp = SCharacterData.nCharacterHP;
			Hprint2( iMainX+iFieldX+30, iMainY+iFieldY+HPMPBarHeight, g_DestBackBuf, "%d",  hp); //test from eleval for HP BUG
		}break;
	case DP_HP :
		{	
			int hp = SCharacterData.nCharacterHP/*Hero->hp*/; //Eleval 18/08/09 - HP Bug
			if( hp > SCharacterData.nCharacterMAXHP ) hp = SCharacterData.nCharacterMAXHP;
			SetHangulAlign( TA_RIGHT ); 
//				Hprint2( iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf,"%d/%d",hp, SCharacterData.nCharacterMAXHP);
			Hprint2( iMainX+iFieldX+58, iMainY+iFieldY, g_DestBackBuf,"%d", SCharacterData.nCharacterMAXHP);
			break;
		}
	case DP_MP_BAR :	
		{
			if( SCharacterData.nCharacterMAXMP > 0 )
			{
				const int iMax = SCharacterData.nCharacterMAXMP;
				const int iNow = SCharacterData.nCharacterMP;
				
				int iTarget = static_cast<int>(((float)iNow) / ((float)iMax) * ((float)HPMPBarHeight));
				if( HPMPBarHeight < iTarget){iTarget = HPMPBarHeight;}
				FieldTypeNomalPut(iMainX, iMainY, iFieldX, iFieldY,0, DIVINE_IMG);
				if( SCharacterData.nCharacterData[SPELL] == PRIEST_SPELL )//����� �϶�
				{
					for(int iCount = 0; iCount < iTarget; iCount++)//011031 lsw
					{
						FieldTypeNomalPutFx3(iMainX+21, iMainY+HPMPBarHeight+17, iFieldX, iFieldY-iCount,nRcImg, DIVINE_IMG,13,1);
					}					
				}
				else 
				{
					for(int iCount = 0; iCount < iTarget; iCount++)//011031 lsw
					{
						FieldTypeNomalPutFx3(iMainX+21, iMainY+HPMPBarHeight+17, iFieldX, iFieldY-iCount,nRcImg, MANA_IMG,13,1);
					}					
				}
				FieldTypeNomalPut(iMainX, iMainY,iFieldX, iFieldY,1, DIVINE_IMG);
			}
			SetHangulAlign( TA_CENTER );
			Hcolor( FONT_COLOR_WHITE );
			Hprint2( iMainX+iFieldX+47, iMainY+iFieldY+HPMPBarHeight, g_DestBackBuf, "%d",  SCharacterData.nCharacterMP);
		}break;
	case DP_MP :	
		{
		//	if( SCharacterData.nCharacterData[SPELL] == PRIEST_SPELL )
		//	{
		//		FieldTypeNomalPut( iMainX, iMainY, 10, 196, 23, MAIN_ETC );
			//	}

			SetHangulAlign( TA_RIGHT ); 
				//Hprint2( iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf,"%d/%d",SCharacterData.nCharacterMP, SCharacterData.nCharacterMAXMP);
			Hprint2( iMainX+iFieldX+58, iMainY+iFieldY, g_DestBackBuf,"%d",SCharacterData.nCharacterMAXMP);
		}break;


	case DP_MAIN_LEVEL : 
		{
			const int before_exp = NPC_Lev_Ref[SCharacterData.nLevel].nMinExp;
			if( SCharacterData.nExpNext-before_exp> 0 )
			{
				const int iMax = NPC_Lev_Ref[SCharacterData.nLevel].nMaxExp - before_exp;//���� ����ġ �ѷ� - ���� ����ġ �ѷ�
				const int iNow = std::max(static_cast<int>(SCharacterData.nExp-before_exp),0);

				const float fExpRate= ((float)iNow) / ((float)iMax);
				const int iTarget = std::min((int)(fExpRate * EXPBarWidth), EXPBarWidth);
				for(int iCount = 0; iCount < iTarget; iCount++)//011031 lsw
				{
					::FieldTypeNomalPut(iMainX, iMainY, iFieldX+iCount, iFieldY, nRcImg, iImgType );
				}
				
				
				::Hcolor(ConvertColor(200,200,9));
				if(SCharacterData.nLevel <= 100)//100 Level ���� ����ġ ����
				{
					::SetHangulAlign( TA_RIGHT );
					::HprintBold( iMainX+iFieldX + EXPBarWidth/2, iMainY + iFieldY-1,
						Convert16to32(FONT_COLOR_EXP_PERCENT),Convert16to32(FONT_COLOR_BLACK),
						"%d", SCharacterData.nExp );
				}
				::SetHangulAlign( TA_LEFT );
				int Per = (int)(fExpRate*100);
			//	if(0> Per){Per += 100; Per *= -1;}
				::HprintBold( iMainX+iFieldX+EXPBarWidth - 23, iMainY + iFieldY,
						Convert16to32(FONT_COLOR_EXP_PERCENT),Convert16to32(FONT_COLOR_BLACK),
						"%d%%", Per);
//							"%d%%", __max(0,(int)(fExpRate*100)) );
			}
		 }break;
	
	case DP_FACE :	//020515 lsw
		{
			FieldTypeNomalPut(iMainX, iMainY, iFieldX, iFieldY,nRcImg,iImgType ); //ǥ�� �׸��� 255�� ���ͽ��� 
		}break;
	case DP_FACEIMAGE :	// �� �̹��� ���
		{
			PutCharImage(iMainX+iFieldX, iMainY+iFieldY, SCharacterData.nCharacterData[FACE]); 
		}break;
	case DP_FACE_PUT : //011001 lsw
		{
			if(nWillDo)
			{
				 PutCharImage( iMainX+iFieldX, iMainY+iFieldY, nRcImg, 6, FS_SMALL,2, iImgType );
			}
		}
		break;
	case DP_LEVEL_IN_TIME_MENU:
		{
			SetHangulAlign( TA_LEFT );
			//::Hcolor( Lv_Color[SCharacterData.nLevel-1].GetRGB16() );
			::HprintBold( iMainX+iFieldX, iMainY+iFieldY, 
						Convert16to32(FONT_COLOR_WHITE),Convert16to32(FONT_COLOR_BLACK),
						"Lv.%d", SCharacterData.nLevel );
		}break;
	case DP_LEVEL :	
		{
		//	SetHangulAlign( TA_RIGHT );
			::Hcolor( Lv_Color[SCharacterData.nLevel-1].GetRGB16() );
			Hprint2( iMainX+iFieldX+3, iMainY+iFieldY-1, g_DestBackBuf,"%d",SCharacterData.nLevel);
		}break;
	case DP_EXP :	
		{
			if( SCharacterData.nLevel <= 100 || GetSysInfo( SI_GAME_MAKE_MODE ) )
			{
				SetHangulAlign( TA_RIGHT );
				static DWORD old_exp = SCharacterData.nExp;
				static DWORD old_exp2 = SCharacterData.nExp;
				static int plus = 0;

				if( !SubMenu->CheakType )	old_exp = SCharacterData.nExp;

				if( old_exp < SCharacterData.nExp )
				{
					bool refresh_plus = false;
					if( old_exp2 < SCharacterData.nExp ) refresh_plus = true;
					old_exp2 = SCharacterData.nExp;

					if( !plus || refresh_plus )
					{
						plus = (int )((SCharacterData.nExp - old_exp)/30.0f);
						if( !plus ) plus = 1;
					}
					
					Hprint2( iMainX+iFieldX+88, iMainY+iFieldY-2, g_DestBackBuf,"%u",old_exp);
					old_exp += plus;
					if( old_exp > SCharacterData.nExp ) old_exp = SCharacterData.nExp;
				}
				else
				{
					Hprint2( iMainX+iFieldX+88, iMainY+iFieldY-2, g_DestBackBuf,"%u",SCharacterData.nExp);
					plus = 0;
				}
			}
			else//100 �� �̻�
			{
				const int nWidth = 88;
				DWORD exp = SCharacterData.nExp;
				DWORD expNext = SCharacterData.nExpNext;
				if( expNext )
				{
					int unit = expNext/10;
					int type = exp/unit;		// ���° �ΰ�..
					if( type >= 1 )
					{
						for( int i=0; i<nWidth; i++ )
						{
							FieldTypeNomalPut( iMainX+i, iMainY, iFieldX, iFieldY, 0, IIT_EXP_GAUGE );
						}
					}
					int count = int((float)(exp-unit*type)*nWidth/(unit));
					if(count > nWidth)
					{
						count = nWidth;
					}
				}
			}
		}break;
	case DP_EXP_NEXT :	
		{
			if( SCharacterData.nLevel <= 100 )
			{
				SetHangulAlign( TA_RIGHT );
				Hprint2( iMainX+iFieldX+88, iMainY+iFieldY-2, g_DestBackBuf,"%u",SCharacterData.nExpNext);
				break;
			}
			else
			{
				const int nWidth = 88;
				DWORD exp = SCharacterData.nExp;
				DWORD expNext = SCharacterData.nExpNext;
				if( expNext )
				{
					int count = (int)((float)exp*nWidth/expNext);
					if(count > nWidth)
					{
						count = nWidth;
					}
#ifdef _DEBUG // finito 060507
					if( GetSysInfo( SI_NOT_CONNECT_MODE ) )
					{
						SCharacterData.nExp += 500000;
					}
#endif
					for( int i=0; i<count; i++ )
					{
						FieldTypeNomalPut( iMainX+i, iMainY, iFieldX, iFieldY, 0, IIT_EXP_GAUGE );
					}
				}
			}
		}break;
	case DP_DAMAGE :	
		{
			SetHangulAlign( TA_RIGHT ); 
			Hprint2( iMainX+iFieldX+58, iMainY+iFieldY, g_DestBackBuf,"%d", SCharacterData.nCharacterAbility[DAMAGE] );
		}break;
	case DP_HEALTH :	
		{
			SetHangulAlign( TA_RIGHT ); 
			Hprint2( iMainX+iFieldX+58, iMainY+iFieldY, g_DestBackBuf,"%d/%d",SCharacterData.nCharacterSP, SCharacterData.nCharacterMAXSP);
		}break;
		
	case DP_TACTICS_DATA :	
		{
			if( !SubMenu->CheakType )	// ��ƽ�� â Ȱ��ȭ �ٷ� ����
			{
				//SubMenu->CheakType = 1;		//FT_EXPLAIN_MENU���� �ʱ�ȭ ���ش�.
				ReqTacSkillExp( );		// ��ġ ���� 
				SubMenu->work = refresh_magery_time;
			}

			if( refresh_magery_time <= 0 ) 
			{
				CallServer( CMD_REQ_MAGERY );  //������ ���������� ���� ��...
				refresh_magery_time = 100;
			}
			else refresh_magery_time--;

			int x = iMainX+iFieldX;
			int y = iMainY+iFieldY;
			int tactic_para[][13] = 
			{ 
				{ CRAPPLE, SWORD, ARCHERY, PARRYING, MACE, PIERCE, WHIRL, HURL, MAGERY, D_SWORD, D_MACE, }, 
				{ CRAPPLE, SWORD, ARCHERY, FENCING, MACE, PIERCE, WHIRL, HURL,PARRYING, MAGERY, }, 

			};
			int max;
			int first_param = 0;
			if( SCharacterData.nCharacterData[GENDER] == MALE )
			{
				if( SCharacterData.nCharacterData[SPELL] == WIZARD_SPELL )
				{
			//	#if defined (TAIWAN_LOCALIZING_) || defined (HONGKONG_LOCALIZING_)//010730 lms �븸 magery �����
			//		FieldTypeNomalPut( iMainX, iMainY, 10, 216, nWillDo, iImgType );	// magery �� ���� �����
			//	#else
			//		FieldTypeNomalPut( iMainX, iMainY, 11, 220, nWillDo, iImgType );	// magery �� ���� �����
			//	#endif
					tactic_para[0][8] = MAGERY;
				}
				else tactic_para[0][8] =  ORISON;

				first_param = 0;
				max = 11;
			}
			else
			{
				if( SCharacterData.nCharacterData[SPELL] == WIZARD_SPELL )
				{
					tactic_para[1][9] = MAGERY;//���ݸ������
				//	FieldTypeNomalPut( iMainX, iMainY, 10, 244, nWillDo, iImgType );	// magery �� ���� �����
				}
				else tactic_para[1][9] =  ORISON;

				first_param = 1;
				max = 10;
			}
			int lv, exp, exp_next;
			const int nWidth1 = 69;
			const int nWidth2 = 69;

			for( int a = 0; a<max; a++ )
			{
#ifdef _DEBUG // finito 060507
				if( GetSysInfo( SI_NOT_CONNECT_MODE ) )
				{
					Hero->aStepInfo[a + TAC_INDEX] = 1;
					SCharacterData.TacticsExp[tactic_para[first_param][a]] += 100000;
					RecvTacSkillExp( (t_char_info_tac_skillexp *)SCharacterData.TacticsExp );
				}
#endif
				lv = SCharacterData.SkillId[tactic_para[first_param][a]+88]+1;
				exp = SCharacterData.TacticsExp[tactic_para[first_param][a]];
				exp_next = SCharacterData.TacticsExpNext[tactic_para[first_param][a]];
			//	Hprint2( x, y+gabx[first_param][a], g_DestBackBuf,"%d", lv );
				Hprint2( x, y+a*23, g_DestBackBuf,"%d", lv );
				if( lv > 101 )
				{
					if( exp_next )
					{
						int unit = exp_next/10;
						int type = exp/unit;		// ���° �ΰ�..
						if( type >= 1 )
						{
							for( int i=0; i<nWidth1; i++ )
							{
								FieldTypeNomalPut( iMainX+23+i, iMainY+a*23, iFieldX, iFieldY, type, IIT_EXP_GAUGE );
							}
						}
						int count = int((float)(exp-unit*type)*nWidth1/(unit));
						for( int i=0; i<count; i++ )
						{
							FieldTypeNomalPut( iMainX+23+i, iMainY+a*23, iFieldX, iFieldY, (type+1), IIT_EXP_GAUGE );
						}
					}

					if( exp_next )
					{
						int count = (int)((float)exp*nWidth2/exp_next);
						for( int i=0; i<count; i++ )
						{
						//	FieldTypeNomalPut( iMainX+97+i, iMainY+gabx[first_param][a]-1, iFieldX, iFieldY, 0, IIT_EXP_GAUGE );//
							FieldTypeNomalPut( iMainX+96+i, iMainY+a*23, iFieldX, iFieldY, 0, IIT_EXP_GAUGE );//
						}
					}
				}

				if( lv <= 101 || GetSysInfo( SI_GAME_MAKE_MODE ) )//100 ���ϴ�
				{
					Hprint2( x+25, y+a*23, g_DestBackBuf,"%u", exp );		//���÷��� ���ش�
					Hprint2( x+98, y+a*23, g_DestBackBuf,"%u", exp_next );	//���÷��� ���ش�
				}
			}
			break;
		}
	 //��ġ�� ���
	case DP_ARIGEMENT :	
		{
			Hcolor( FONT_COLOR_DEFAULT );
			Hprint2( iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf,"%s",SHideNomal[HN_MAKECHARACTER_ARIGEMENT_TEXT][SCharacterData.nCharacterData[ARIGEMENT]].temp);
		}break;
	case DP_NAME_CENTER :		//�̸� ���
		{
			Hcolor( FONT_COLOR_NAME );
			SetHangulAlign(TA_CENTER);
			Hprint2( iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf,"%s", SCharacterData.sCharacterName );
		}break;
	case DP_NAME :		//�̸� ���
		{
			Hcolor( FONT_COLOR_NAME );
			Hprint2( iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf,"%s", SCharacterData.sCharacterName );
		}break;

	case DP_CLASS :	//Ŭ���� ���
		{
			Hcolor( FONT_COLOR_DEFAULT );
			SetHangulAlign(TA_CENTER);
			Hprint2( iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf,"%s",SHideNomal[HN_MAKECHARACTER_CLASS_TEXT][SCharacterData.nCharacterData[CLASS]].temp );
		}break;

	case DP_SPELL :		//���� ���
		{
			Hcolor( FONT_COLOR_DEFAULT );
			if(SCharacterData.nCharacterData[SPELL] == 0)
			{
				Hprint( iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf,"WIZARD");
			}
			else if(SCharacterData.nCharacterData[SPELL] == 1)
			{
				Hprint( iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf,"PRIEST");
			}
		}break;
		 
	case DP_TACTICS :	//��ƽ�� ���
		{
			Hcolor( FONT_COLOR_DEFAULT );
			if(SCharacterData.nCharacterData[GENDER]==0) 
			{
				Hprint2( iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf,"%s",SHideNomal[HN_MAKECHARACTER_TACTICS_MAN_TEXT][SCharacterData.nCharacterData[TACTICS_MAN]].temp  );
			}
			else	
			{
				Hprint2( iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf,"%s",SHideNomal[HN_MAKECHARACTER_TACTICS_WOMAN_TEXT][SCharacterData.nCharacterData[TACTICS_WOMAN]].temp  );
			}
		}break;
		 
	case DP_JOB :		//���� ���
		{
			Hcolor( FONT_COLOR_DEFAULT );
			Hprint2( iMainX+iFieldX, iMainY+iFieldY-1, g_DestBackBuf,"%s",SHideNomal[HN_MAKECHARACTER_JOB_TEXT][SCharacterData.nCharacterData[JOB]-20].temp  );
		}break;
	case DP_ABILITY : 	
		{
		AbilityOpenWhenLevelUp();		// 溯漣機縑 婦溼罹... 蟾晦
		const int ability = nWillDo;
		const int basic = SCharacterData.nCharacterAbility[ability]; //+SCharacterData.EachUp[ability];	// eachup擎 溯漣機 檣蒂 賑陽虜 霤褻
		const int plus = Hero_Ability[ability];

		/*if (basic == g_mgrDual.GetAbility(ability))
		{
		Hcolor(FONT_COLOR_MAX);
		}//020707 lsw

		/if( plus > basic )
		{
		Hcolor( FONT_COLOR_SPECIAL );
		Hprint2( iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf,"%d", plus );
		}
		else if( plus < basic )
		{
		Hcolor( FONT_COLOR_PLUS );
		Hprint2( iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf,"%d", plus );
		}
		else
		{
		Hprint2( iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf,"%d", plus );
		}*/

		Hcolor(FONT_COLOR_NUMBER);
		if (plus < basic)
		{
			Hcolor(FONT_COLOR_PLUS);//計算完裝備後小於本身點數
		}
		if (plus > basic)
		{
			Hcolor(FONT_COLOR_SPECIAL);//計算完裝備後大於本身點數
		}
		if (plus >= g_mgrDual.GetAbility(ability)) //計算完 點數與上限相等
		{
			Hcolor(FONT_COLOR_MAX);
		}
		Hprint2(iMainX + iFieldX, iMainY + iFieldY, g_DestBackBuf, "%d", plus);

		}break;
						
	case DP_RESIST : 
		{
			for( int a=0; a<MAX_RESIST_TYPE; a++)
			{
				SetHangulAlign( TA_RIGHT );
				::Hprint2( iMainX+iFieldX+31, iMainY+iFieldY+a*25+10, g_DestBackBuf,"%d", 
							SCharacterData.naBasicResist[a]+ 
							SCharacterData.naExtentionResist[a]);
			}
		 }break;

	case DP_WEIGHT	:
		{
			Hcolor( FONT_COLOR_SPECIAL );
			if(SubMenu->Id == MN_ITEM)
			{
				if( !Hero_Ability[STR] )		// ���� �� ����!
				{
					CallOkCancelMessageBox( SubMenu->Id, 0,0,lan->OutputMessage(6,63), 0 );//010215 lsw
					//CallServer(CMD_RESEND_CHARLIST);
					break;
				}
				float rate;	// ���� ��� �ִ� ���Կ� ������ �� ���Կ��� ��
				int max = Hero_Ability[STR] * 100;		// ��� �ִ� ���� �ƽ�
				/*2000 + STR* 35 + ����* 20*/
				rate =   SCharacterData.nWeight / (float)max * 100.0f;
				if( rate > 150.0f ) rate = 150.0f;
				if( rate < 0 ) rate = 150.0f;
				float width_rate = 80 / 150.0f;	
				if( rate != 150 )
				{
					for( int start=(int)(rate*width_rate); start<80; start++ )		// rate*width_rate =>���� �Ÿ�
					{
						if( rate < 100 ) 
								FieldTypeNomalPut(iMainX+start, iMainY, iFieldX, iFieldY, nRcImg );
						else	FieldTypeNomalPut(iMainX+start, iMainY, iFieldX, iFieldY, SubMenu->nField[j].nSHideNomalCount );
					}
				}
				SetHangulAlign( TA_RIGHT );//020720 lsw
#ifdef _DEBUG // finito 060507
				if( GetSysInfo( SI_GAME_MAKE_MODE ) )
				{
					Hprint2( 50, 100, g_DestBackBuf,"<%d>", SCharacterData.nWeight );
					Hprint2( 50, 120, g_DestBackBuf,"<%d>", (int)rate );
				}
#endif
				FieldTypeNomalPut(iMainX, iMainY, 124+20, 35+3, 31, MAIN_ETC );		// ���� Ȱ �׸�
				FieldTypeNomalPut(iMainX, iMainY, 124+144, 35+100, 32, MAIN_ETC );		// ���п� ȭ�� �̹���
			}
			else if(SubMenu->Id == MN_BANK_CUSTODY)	
			{
				int weight;
				weight = GetBankItemWeight();
				SetHangulAlign( TA_RIGHT );//020720 lsw
				Hprint2( iMainX+iFieldX+142, iMainY+iFieldY, g_DestBackBuf,"%d", weight);
			}
		}break;

	case DP_ARMOR_INVENTORY :	//顯示背包裝備圖
		{	
			nWeapon_Type = nWillDo; 
			if(EquipItemAttr[nWeapon_Type].item_no)
			{
				PutItemIcon( iMainX+iFieldX+15 , iMainY+iFieldY+15, EquipItemAttr[nWeapon_Type].item_no );
			}
			else if( EquipItemAttr[WT_WEAPON].item_no													// ���⸦ ��� �ִµ�
				&& (nWeapon_Type == WT_SHIELD)															// �����ڸ��� �ƹ��͵� ����
				&& ( GetItemAttr( EquipItemAttr[WT_WEAPON].item_no, WEAR_ABLE ) == WEAR_TWO_HAND )	)	// �� ����Ⱑ ��հ��ε�
			{
				int kind = GetItemAttr( EquipItemAttr[WT_WEAPON].item_no, ITEM_KIND );
				if( kind == IK_SHORT_BOW || kind == IK_LONG_BOW )
				{
					static int ct;
					if( ct%10 ) 
					{
						PutItemIcon( iMainX+iFieldX+15 , iMainY+iFieldY+15, 7005, 0, 2, 1 );
					}
					ct++;
				}
				else PutItemIcon( iMainX+iFieldX+15 , iMainY+iFieldY+15, EquipItemAttr[WT_WEAPON].item_no, 8, 2 );
			}
		}break;

	case DP_SELECT_ITEM	:	
		{
			if( nWillDo == ITEM_BUY )
			{
				if( SubMenu->work )
				{
					PutItemIcon( iMainX+iFieldX+15, iMainY+iFieldY+15, SubMenu->work );
					PutItemIcon( iMainX+iFieldX+15, iMainY+iFieldY+15, SubMenu->work,8,2 );
					Hcolor( FONT_COLOR_NAME );
					SetHangulAlign( TA_CENTER );
					Hprint2( iMainX+iFieldX+95, iMainY+iFieldY+10, g_DestBackBuf, "%s", GetItemName( SubMenu->work ) );
					ItemExplain( SubMenu->work );
					
					int t = GetItemAttr(SubMenu->work, VALUE);		// ������ ó�� 0�ΰ��� ���� ����...
					if( t )
					{
						SetHangulAlign( TA_RIGHT ); 
						Hcolor( FONT_COLOR_NUMBER );
						int price = GetItemAttr(SubMenu->work, VALUE);
//															if( GetItemAttr( SubMenu->work, DO_RBUTTON ) == DIVIDE_ITEM )
//																price *= GetItemAttr( SubMenu->work, DURATION );
						Hprint2( iMainX+91+57, iMainY+231, g_DestBackBuf,"%d" , price );
					}
				}
			}
			else if( nWillDo == ITEM_SELL )
			{
				if( !SubMenu->CheakType )		// ó�� �� �޴��� �ҷ� ���� ���
				{
					SubMenu->work = -1;
					SubMenu->CheakType = 1;
					break;
				}
				if(SubMenu->work == -1 ) break;
				int selelct = SubMenu->work;
				int item = aSellBuyItemAttr[selelct].item_no;
				int item_pos = aSellBuyItemPos[selelct];		// �κ��丮������ ������ ��ġ 
//													int item_pos = SubMenu->work;
				int i_a = (item_pos & 0x00000060) >> 5 ;
				int i_b = (item_pos & 0x00000018) >> 3 ;
				int i_c = (item_pos & 0x00000007) ;
				ItemAttr t = InvItemAttr[i_a][i_b][i_c];
				if( item )
				{
					PutItemIcon( iMainX+iFieldX+15, iMainY+iFieldY+15, item );
					PutItemIcon( iMainX+iFieldX+15, iMainY+iFieldY+15, item,8,2 );
					Hcolor( FONT_COLOR_NAME );
					SetHangulAlign( TA_CENTER );
					Hprint2( iMainX+iFieldX+95, iMainY+iFieldY+10, g_DestBackBuf, "%s", GetItemName( item ) );
					ItemExplain(  t );
					SetHangulAlign( TA_RIGHT ); 
					DWORD price = GetItemValueSell( t );
//														if( price == 0 ) break;		// �� ����� ���´�.
					Hcolor( FONT_COLOR_NUMBER );
					Hprint2( iMainX+91+57, iMainY+231, g_DestBackBuf,"%d", price );
				}
			}
			else if( nWillDo == ITEM_REPAIR )
			{
				if( !SubMenu->CheakType )		// ó�� �� �޴��� �ҷ� ���� ���
				{
					SubMenu->work = -1;
					SubMenu->CheakType = 1;
					break;
				}
				if(SubMenu->work == -1 ) break;
				int select = SubMenu->work;
				int item = aSellBuyItemAttr[select].item_no;		
				int item_pos = aSellBuyItemPos[select];		// �κ��丮������ ������ ��ġ 
				int i_a = (item_pos & 0x00000060) >> 5 ;
				int i_b = (item_pos & 0x00000018) >> 3 ;
				int i_c = (item_pos & 0x00000007) ;
				ItemAttr t = InvItemAttr[i_a][i_b][i_c];

				if( item )
				{
					PutItemIcon( iMainX+iFieldX+15, iMainY+iFieldY+15, item );
					PutItemIcon( iMainX+iFieldX+15, iMainY+iFieldY+15, item,8,2 );
					Hcolor( FONT_COLOR_NAME );
					SetHangulAlign( TA_CENTER );
					Hprint2( iMainX+iFieldX+95, iMainY+iFieldY, g_DestBackBuf, "%s", GetItemName( item ) );

					Hcolor( FONT_COLOR_NUMBER );
					WORD d_max, d_curr; 
					GetItemDuration( t, d_curr, d_max );
					Hprint2( iMainX+iFieldX+95, iMainY+iFieldY+15, g_DestBackBuf, "%d / %d", d_curr/10, d_max/10 );

					int price=1;
					if( d_max )
					{
						//price = (int)( GetItemAttr(t.item_no, VALUE)/d_max*(d_max - d_curr)*DUR_RATIO +.5 );
						price = GetItemAttr(t.item_no, VALUE);
						price = (int)( (float)price/d_max*(d_max - d_curr)*DUR_RATIO +.5 );//;+((RareMain*)(&t.attr[IATTR_RARE_MAIN]))->grade*1000000;//020505 lsw

						if(t.attr[IATTR_RARE_MAIN])
						{	
							const int iRepairPoint = (int)( (float)((d_max - d_curr))*RARE_DUR_RATIO +.5); // ���� ��� �� �Ǵ� 
							const int iRareGrade  = ((LPRareMain)(&t.attr[IATTR_RARE_MAIN]))->grade;
							price += (iRepairPoint/2) * ((iRareGrade*iRareGrade)+10) * 100	; // ���� ��� �� �Ǵ� //020815 lsw
						}
						if( !price ) price = 1;
					}
					SetHangulAlign( TA_RIGHT ); //020705 lsw
					Hprint2( iMainX+91+57, iMainY+231, g_DestBackBuf,"%d", price );
					ItemExplain( t );
				}
			}
			else if( nWillDo == ITEM_BUY_MALL )
			{
				if( SubMenu->work )
				{
					PutItemIcon( iMainX+iFieldX+15, iMainY+iFieldY+15, SubMenu->work );
					PutItemIcon( iMainX+iFieldX+15, iMainY+iFieldY+15, SubMenu->work,8,2 );
					Hcolor( FONT_COLOR_NAME );
					SetHangulAlign( TA_CENTER );
					Hprint2( iMainX+iFieldX+95, iMainY+iFieldY+10, g_DestBackBuf, "%s", GetItemName( SubMenu->work ) );
					ItemExplain( SubMenu->work );
					
					int t = GetItemAttr(SubMenu->work, RAJA_EMBLEM);		// ������ ó�� 0�ΰ��� ���� ����...
					if( t )
					{
						SetHangulAlign( TA_RIGHT ); 
						Hcolor( FONT_COLOR_NUMBER );
						int price = GetItemAttr(SubMenu->work, RAJA_EMBLEM);
//															if( GetItemAttr( SubMenu->work, DO_RBUTTON ) == DIVIDE_ITEM )
//																price *= GetItemAttr( SubMenu->work, DURATION );
						Hprint2( iMainX+148, iMainY+326, g_DestBackBuf,"%d" , price );
					}
				}
			}
	}break;

	case DP_ARIGEMENT_BAR : 
		{
			MP3( SN_MAGIC_BALL );
			switch(SCharacterData.nCharacterData[ARIGEMENT])
			{
				case 0 :
				case 1 :	FieldTypeNomalPut(iMainX, iMainY, 157, 178, 310); //������� �̹��� ��ȣ�� 310
				case 2 :	FieldTypeNomalPut(iMainX, iMainY, 157, 200, 310); //������� �̹��� ��ȣ�� 310
				case 3 :	FieldTypeNomalPut(iMainX, iMainY, 157, 221, 310); //������� �̹��� ��ȣ�� 310
				case 4 :
				case 5 :	FieldTypeNomalPut(iMainX, iMainY, 157, 250, 310); //������� �̹��� ��ȣ�� 310
				case 6 :	FieldTypeNomalPut(iMainX, iMainY, 157, 272, 310); //������� �̹��� ��ȣ�� 310
				case 7 :	FieldTypeNomalPut(iMainX, iMainY, 157, 295, 310); //������� �̹��� ��ȣ�� 310
				case 8 :break;
			}
		}break;

	case DP_GUILD :		
		{
			Hcolor( FONT_COLOR_DEFAULT );
			Hprint2( iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf,"%s",SCharacterData.nGuild);
		}break;
	case DP_MONEY :		
		{
			SetHangulAlign( TA_RIGHT ); 
			Hprint2( iMainX+iFieldX+58, iMainY+iFieldY, g_DestBackBuf,"%d", SCharacterData.nMoney);
		}break;
	case DP_FAME :		
		{
			SetHangulAlign( TA_RIGHT ); 
			Hprint2( iMainX+iFieldX+58, iMainY+iFieldY, g_DestBackBuf,"%d", SCharacterData.nCharacterAbility[FAME]);
			break;
			//						case DP_WSPS :		if(flag) { ability=WSPS; flag=FALSE; }
			if(flag) { ability=AC; flag=FALSE; }
			SetHangulAlign( TA_RIGHT ); 
			Hprint2( iMainX+iFieldX+58, iMainY+iFieldY, g_DestBackBuf,"%d", Hero_Ability[ability]);
		}break;
	case DP_TACTICE :	
		{
			SetHangulAlign( TA_RIGHT ); 
			if( SCharacterData.tatics != -1 )
			{
				Hprint2( iMainX+iFieldX+58, iMainY+iFieldY, g_DestBackBuf,"%d", SCharacterData.SkillId[SCharacterData.tatics]+1 );
			}
			else 
			{
				Hprint2( iMainX+iFieldX+58, iMainY+iFieldY, g_DestBackBuf,"%d", 0 );
			}
		}break;
	case DP_HEAT :	
		{
			SetHangulAlign( TA_RIGHT ); 
			Hprint2( iMainX+iFieldX+57, iMainY+iFieldY, g_DestBackBuf,"%d", SCharacterData.beat); 
		}break;
	case DP_AC	:
		{
			SetHangulAlign( TA_RIGHT ); 
			Hprint2( iMainX+iFieldX+57, iMainY+iFieldY, g_DestBackBuf,"%d", SCharacterData.ac	); 
			break;
		}
	case DP_MOV	:		
		{
			SetHangulAlign( TA_RIGHT );
			Hprint2( iMainX+iFieldX+57, iMainY+iFieldY, g_DestBackBuf,"%d", SCharacterData.mov); 
		}break;

	case DP_MONEY2 :	
		{
			PrintMoney( SubMenu, j, (DWORD)(*SubMenu->nField[j].nValue)+nBankTemp, nWillDo );
		}break;
	case DP_SKILL_ICON :
		{
			for(int a=0;a<2;a++) 
			{
				for(int b=0;b<4;b++)  
				{                          
					if(SkillInventory[nWillDo][a][b])
					{
						PutSkillIcon(iMainX+iFieldX+17+41*b , iMainY+iFieldY+17+a*36, SkillInventory[nWillDo][a][b]);
					}
/*														else if( SkillInventory2[nWillDo][a][b] )
					{
						PutSkillIcon(iMainX+iFieldX+17+41*b , iMainY+iFieldY+17+a*36, SkillInventory2[nWillDo][a][b], 15, 1);
					}*/
				}
			}
		}break;

	case DP_BANK_CURRENT_PAY :	
		{
			PrintMoney(	SubMenu, j, GetCurrentBankItemPay(), 124 ); 
			break;
		}
	case DP_BANK_PAY :		
		{
			PrintMoney(	SubMenu, j, GetCurrentBankItemPay(), 124 ); 
		}break;
	case DP_LOAN_TOTAL :	
		{
			PrintMoney(	SubMenu, j, SCharacterData.LoanTotal, 105 ); 
		}break;
	case DP_LOAN_ABLE : 	
		{
			if( SCharacterData.LoanTotal > SCharacterData.LastLoan ) 
			{
				PrintMoney( SubMenu, j, SCharacterData.LoanTotal - SCharacterData.LastLoan, 105 );
			}
			else 
			{
				PrintMoney( SubMenu, j, 0, 105 ); 
			}
		}break;
	case DP_BANK_SAVE :		
		{
			PrintMoney( SubMenu, j, SCharacterData.BankMoney, 153 ); 
		}break;

	case DP_LAST_LOAN:		
		{
			PrintMoney(	SubMenu, j, SCharacterData.LastLoan, 105 ); break;
		}
		
	case DP_TEMP_MONEY :
		{
			if( !SubMenu->CheakType )
			{
				SubMenu->CheakType = 1;
				SetChatMode(CM_COMMAND);
				EWndMgr.ClearAllTxt();//021001 lsw
			}
			long long max = 1000000000;		
			switch( SubMenu->Id )
			{
				case MN_BANK_DEPOSIT	: 
					{
						if( SCharacterData.BankMoney > 4000000000 )
						{
							max = 0;
						}
						else
						{
							max = std::min( static_cast<uint32_t >(4000000000 - SCharacterData.BankMoney), SCharacterData.nMoney );
						}
						break;
					}
				case MN_BANK_DEFRAYAL	: max = SCharacterData.BankMoney; break;
				//case MN_BANK_LOAN		: max = MyBank.GetLoanAble(); break;
				case MN_BANK_REPAYMENT	: 
										if( SCharacterData.LastLoan > SCharacterData.nMoney)
											max = SCharacterData.LastLoan;
										else
											max = SCharacterData.nMoney;
										break;
			}
			//021001 lsw	
			if (!EWndMgr.checkInputBoxVisibility(HWND_3)) {
				EWndMgr.showInputBox(HWND_3);
			}
			char temp[20] = { 0, };
			std::string money = EWndMgr.GetTxt(HWND_3);
			//EatRearWhiteChar(temp);
			try {
				nBankTemp = atol(money.c_str());
			}
			catch (...)
			{
				break;
			}
			int refresh = 0;
			if( nBankTemp<0)
			{
				nBankTemp = 0;
				refresh = 1;
			}
			if( (DWORD)nBankTemp>max )		//// 020925 YGI
			{
				nBankTemp = max;
				refresh = 1;
			}

			if( refresh || CheckNumber( temp, strlen( temp ) ) )
			{	
				sprintf( temp, "%u", nBankTemp );
				EWndMgr.SetTxt(HWND_3, temp);//021001 lsw //�����쿡 �ؽ�Ʈ�� ���� �մϴ�.
			}
			PrintMoney( SubMenu, j, nBankTemp, 153 ); 
		}break;
	case DP_BANK_TOTAL :
		{
			if(SubMenu->Id == MN_BANK_DEPOSIT)
			{
				PrintMoney( SubMenu, j, SCharacterData.BankMoney+nBankTemp, 153 );
			}
			else 
			{
				PrintMoney( SubMenu, j, SCharacterData.BankMoney-nBankTemp, 153 );
			}
			break;
		}
	case DP_DAY_TEXT :	
		{
			PrintDate( SubMenu, j, g_GameInfo.timeGame, 100 ); 
		}break;
	case DP_CREDIT_TEXT :
		{
			Hcolor( RGB16(255, 255, 255) );
			SetHangulAlign( TA_CENTER ); 
			int lenght = 50;
			Spr *s = GetSprOfMenu( SubMenu->nImageType, SubMenu->nImageNumber );
			if( s ) lenght= s->xl / 2;
			Hprint2( iMainX+lenght, iMainY+iFieldY, g_DestBackBuf, "Believable" );
		 }break;

	case DP_REPAYMENT_MONEY :	
		{
			PrintMoney(	SubMenu, j, SCharacterData.LastLoan, 105 ); 
		}break;
	case DP_REPAYMENT_DATE :	
		{
			PrintDate(SubMenu, j, SCharacterData.LastLoan_Time, 100); 
		}break;
	case DP_CHARACTER_MONEY :	
		{
			PrintMoney(	SubMenu, j, SCharacterData.nMoney, nWillDo); 
		}break;  //ĳ���Ͱ� ������ �ִ� ���� ��´�. �������� ����
	case DP_CHARACTER_RAJA_EMBLEM :	
		{
			PrintMoney(	SubMenu, j, GetItemMuchByMuchItem(10057), nWillDo); 
		}break;
	case DP_AUCTION_FK_RARE://���θ޴����� �˻��⿡ ���� �ѱ� ���
		{//021014 lsw
//			const int iValue = Auction.GetFKRareType();
//			int iType = 2, iNum = 0;
//			switch(iValue)
//			{
//			case NORMAL_ITEM:	{iNum = 711; }break;
//			case RARE_ITEM	:	{iNum = 712; }break;
//			case HIGH_ITEM	:	{iNum = 713; }break;
//			case DYNAMIC_RARE_ITEM:	{iNum = 714; }break;
//			case SET_ITEM	:	{iNum = 715; }break;
//			default:{return;}break;
//			}
//			Hprint2( iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf, lan->OutputMessage(iType, iNum ));
		}break;
	case DP_AUCTION_FK_LV:
		{
//			const int iValue = Auction.GetFKLevel();
//			Hprint2( iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf, "%d~%d", iValue*10, (iValue+1)*10);
		}break;
	case DP_AUCTION_FK_TACTIC:
		{
//			const int iValue = Auction.GetFKTacticType();
//			Hprint2( iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf, ::GetTacticName(iValue) );
		}break;
	case DP_AUCTION_FK_WEAR:
		{
//			const int iValue = Auction.GetFKWearType();
//			int iType = 2, iNum = 0;
//			switch(iValue)
//			{
//			case WEAR_ANYTHING	:{iNum = 720;}break;
//			case WEAR_ONLY_INVEN:{iNum = 721;}break;
//			case WEAR_RIGHT_HAND:{iNum = 722;}break;
//			case WEAR_LEFT_HAND	:{iNum = 723;}break;
//			case WEAR_HEAD		:{iNum = 724;}break;
//			case WEAR_BODY		:{iNum = 725;}break;
//			case WEAR_NECK		:{iNum = 726;}break;
//			case WEAR_SHOES		:{iNum = 727;}break;
//			case WEAR_UNIQUE	:{iNum = 728;}break;
//			case WEAR_BELT		:{iNum = 729;}break;
//			case WEAR_TWO_HAND	:{iNum = 730;}break;
//			default:{return;}break;
//			}
//			::Hprint2( iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf, lan->OutputMessage(iType, iNum));
		}break;
	case DP_BETTING_MONEY:
		{
			DWORD dwMoney = ::GetTeamBettingMoney(nWillDo);
			::PrintMoney(SubMenu,j,dwMoney,130);
		}break;
	case DP_ARENA_TEAM_WIN_RATE:
		{	//< CSD-030520
			const float fWinRate = ::GetTeamWinRate(nWillDo);
			::Hprint2( iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf, "%3.1f%%",fWinRate);

			//檢查是否佔用隊伍(隊長)太久時間
			if (g_pArenaManager->CheckInTeamTimePeroid(g_curr_time)) {
				g_pArenaManager->SendLeaveTeam();
				g_pArenaManager->SendLeaveLobby();
				g_BattleStatusView = 0;
				SmallMenuClose();
				g_pArenaManager->Reset();
				FighMapReset();
			}
			break;
		}	//> CSD-030520
	case DP_MY_BETTING_MONEY:
		{
			switch(nWillDo)
			{
			case 0://���ñ�
				{
					DWORD dwMoney = ::GetMyBettingMoney();
					::PrintMoney(SubMenu,j,dwMoney,130);
				}break;
			case 1://����
				{
					DWORD dwMoney = ::GetMyBettingResult();
					::PrintMoney(SubMenu,j,dwMoney,130);
				}break;
			}
		}break;
	case DP_ARENA_WAIT_TIME:
		{
			::SetHangulAlign(TA_CENTER);
			const DWORD dwTime = ::GetAreanWaitTime();
			::Hprint2( iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf, "%d",dwTime );
		}break;
	case DP_TEMP_MAIL_ICON:
		{
			const int iMailType = g_MailMgr.GetTempMailType();
			const int iMailIconNo = g_MailMgr.GetMailIconNo(iMailType);
			::FieldTypeNomalPut(iMainX+iFieldX,iMainY+iFieldY,0,0,iMailIconNo,iImgType);
			
			if( ::BoxAndDotCrash( iMainX+iFieldX,iMainY+iFieldY, 20, 20, g_pointMouseX, g_pointMouseY ) )
			{
				const int iX = iMainX+iFieldX+30, iY = iMainY+iFieldY;
				::DisplayMailIconExplain(iX,iY,iMailType);
			}
		}break;
	case DP_NOW_VIEW_MAIL_PAGE:
		{
			::Hcolor(FONT_COLOR_WHITE);
			::SetHangulAlign(TA_CENTER);
			TxtOut.RcTXTOutNoBG(iMainX+iFieldX,iMainY+iFieldY,150,1,"%d/5",g_MailMgr.GetNowViewPageNo()+1 );
		}break;
	}
	return;
}

bool DoButtonCheckOfMenu_lsw( int i, int j )
{
	SMENU *SubMenu = &SMenu[i];

	int &nWillDo= SubMenu->nField[j].nWillDo;
	int &iSwd = SubMenu->nField[j].nSpecialWillDo;

	int &work	= SubMenu->work;

	int &iMainX = SubMenu->x;
	int &iMainY = SubMenu->y;
	
	int &iFieldX = SubMenu->nField[j].x;
	int &iFieldY = SubMenu->nField[j].y;
	
	switch(SubMenu->nField[j].nType)
	{
	case	FT_RARE_GROUP:
		{	//
			const int iRareGroup = work + nWillDo;

			if( 0 > iRareGroup || MAX_RARE_GROUP <= iRareGroup)
			{
				break;
			}
			
			SMenu[MN_ITEM_MAKE_RARE_SELECT].nField[0].nWillDo=iRareGroup;//���� �׷� ��

			const int iIsDynamicRare = SMenu[MN_ITEM_MAKE_RARE_SELECT].key;
			for ( int count = 0; count < 8 ; count++)
			{
				if(!iIsDynamicRare)//����ƽ ����
				{
					int tempRareNum = RareGroup[iIsDynamicRare][iRareGroup].rareset[count].rare_num;
					if(  tempRareNum )
					{
						strcpy(SubMenu->nField[count+16].temp,ItemRare[tempRareNum].LocalName);//020505 lsw
						SubMenu->nField[count+16].nImageNumber = tempRareNum;
					}
					else
					{
						strcpy(SubMenu->nField[count+16].temp," ");
						SubMenu->nField[count+16].nImageNumber = 0;
					}
				}
				else
				{
					int tempRareNum = RareGroup[iIsDynamicRare][iRareGroup].rareset[count].rare_num;
					if(  tempRareNum )
					{
						strcpy(SubMenu->nField[count+16].temp,ItemFunction[tempRareNum].NameAdd);//020505 lsw
						SubMenu->nField[count+16].nImageNumber = tempRareNum;
					}
					else
					{
						strcpy(SubMenu->nField[count+16].temp," ");
						SubMenu->nField[count+16].nImageNumber = 0;
					}
				}
				
			}
		}break;
  case	FT_RARE_GROUP_SCROLL://startpoint ���� �־ ��ũ��
		{
			const int UpDown = nWillDo;
			if( UpDown == 1)//DOWN ������
			{//011025 lsw >// 1 ���� ���� �ϰ� 	//0 �� ���� �ȳ����ϱ� -1 �ѹ� �� ���ְ�
				const int iRareGroup = work + SMenu[MN_ITEM_MAKE_RARE_SELECT].nField[6].nWillDo;
				const int iIsDynamicRare = SMenu[MN_ITEM_MAKE_RARE_SELECT].key;

				if(0 > iRareGroup || MAX_RARE_GROUP-1 <= iRareGroup){break;}//MAX_RARE_GROUP-1 �迭�� ������ ������ �κ�

				if(!RareGroup[iIsDynamicRare][iRareGroup+4].name[0])
				{	
					break;
				}
			}
			else if(UpDown == -1 )//UP
			{
				if( SMenu[MN_ITEM_MAKE_RARE_SELECT].work <=0 )
				{
					SMenu[MN_ITEM_MAKE_RARE_SELECT].work =0;
					break;
				}
			}
			SMenu[MN_ITEM_MAKE_RARE_SELECT].work += UpDown;//�̰� ��ũ�Ѷ����� �������� ������
		}break;
	//011013 lsw >
	case FT_MAKE_RARE_MORE:
		{	
			int join_no = item_select_result.join_no;		// ������� �ϴ� �������� ���° �������ΰ�...
			
			CItem  *rit1;
			CItem  *rit2;
			CItem  *rit3;
			CItem  *rit4;

			rit1 = ItemUnit(item_join[join_no].resource1);
			rit2 = ItemUnit(item_join[join_no].resource2);
			rit3 = ItemUnit(item_join[join_no].resource3);
			rit4 = ItemUnit(item_join[join_no].resource4);
			
			int r1amount= item_join[join_no].resource1_amount;	
			int r2amount= item_join[join_no].resource2_amount;	
			int r3amount= item_join[join_no].resource3_amount;	
			int r4amount= item_join[join_no].resource4_amount;	

			int r1count=0;
			int r2count=0;
			int r3count=0;
			int r4count=0;
			
			int r1= -1;
			int r2= -1;
			int r3= -1;
			int r4= -1;
			int *MakeMuch = 0;

			MakeMuch = &skillmakeiteminfo.MakeMuch;
			//011030 lsw >
			GetSkillResorceAmount(r1count,r2count,r3count,r4count);
			//011030 lsw <
			if( r1amount ) 				{r1 =r1count /r1amount;}
			if( r2amount ) 				{r2 =r2count /r2amount;}
			if( r3amount ) 				{r3 =r3count /r3amount;}
			if( r4amount ) 				{r4 =r4count /r4amount;}
			
			if(r1 != -1)				{	*MakeMuch = r1;	}
			if(r2 != -1)				{	*MakeMuch = std::min(*MakeMuch,r2);}
			if(r3 != -1)				{	*MakeMuch = std::min(*MakeMuch,r3);}
			if(r4 != -1)				{	*MakeMuch = std::min(*MakeMuch,r4);}

			int AddValue = nWillDo;

			int *now = &SMenu[MN_ITEM_MAKE_SELECT].nField[10].nWillDo;
			
			if( AddValue )
			{
				*now += AddValue;
				if( *now < 0 )
				{	*now = 0;	}
				if(*MakeMuch < *now)
				{	*now= *MakeMuch;	}
			}
			else
			{
				*now= *MakeMuch;
			}
			*MakeMuch = *now;
			//011022 lsw > �ӽ÷� 10���̻� �Ұ������� ����
			if(*MakeMuch > 10 )
			{
				//011030 lsw >
				AddCurrentStatusMessage(255,255,255,lan->OutputMessage(2,501));
				//011030 lsw <
				*MakeMuch = 10;
				*now = 10;
			}
			//011022 lsw >
			sprintf(SMenu[MN_ITEM_MAKE_SELECT].nField[10].temp,"%d ",*MakeMuch);
		}break;
	case 	FT_CS_ICON:
		{
			switch(iSwd)
			{
			case SWD_SC_ICON_POINT:
				{
					SetCSPIconImageANDnWillDo(work,SubMenu->nField[3].nWillDo);//Done��ư�� ���� ȣ�� �Ǿ��ٸ� ������ ������ �ؾ� �ϱ� �����̴�
					SubMenu->nField[j].nImageNumber = SubMenu->nField[j].nRectImage;//���õ� �̹��� �Ѹ��� 
					SubMenu->nField[15].nWillDo = nWillDo;//����Ƽ �� ��ư�� ��ų ��ȣ �ش�
#ifdef _DEBUG
					if( GetSysInfo(SI_GAME_MAKE_MODE ) )	// 020925 YGI
					AddCurrentStatusMessage(0,255,255,"������ų ����Ʈ �����ܿ� Ŭ�� �ϼ̽��ϴ� ");
#endif
				}break;
			}
		}break;

	case FT_CSD_ICON:
		{
			switch(iSwd)
			{
			case SWD_SC_ICON_DOING:
				{
					if (nWillDo == 0)
					{
						CSDMainIconClear();
					}
					const int nSkill = nWillDo + 214;
					
					if (GetCombat()->IsEnable(nSkill))
					{	//< CSD-TW-030606
						g_mgrBattle.SendCombatSelect(nSkill);
					}	//> CSD-TW-030606
#ifdef _DEBUG
					if( GetSysInfo(SI_GAME_MAKE_MODE ) )	// 020925 YGI
					AddCurrentStatusMessage(0,255,255,"技能選中了");
#endif
				}break;
			}
		}break;

	case 	FT_CSD_MAIN_ICON:
		{
#ifdef _DEBUG
			if( GetSysInfo(SI_GAME_MAKE_MODE ) )	// 020925 YGI
			AddCurrentStatusMessage(0,255,255,"點擊了技能主圖");
#endif
			InitCombatSkill();
			SetCSDIconRectAndnWillDo();
		}break;
	case FT_CSP_POINT_UP:
		{
			SendCSLvUp(nWillDo);
		}break;
	case FT_PEACE_BATTLE_PUT :
		{
			if (Hero->nCurrentAction!=17)	// LTS SITDOWN				// ���� ���� �⺻ ��ȭ���� 
			{
				switch(GetBattlePeace())
				{
				case PEACE_MODE://��ȭ�����
					{
						SetBattlePeace( BATTLE_MODE );//���� ���� �ٲ��ְ�
						AddCurrentStatusMessage(FONT_COLOR_RED, lan->OutputMessage(0,2) );//�������� ��ȯ�մϴ�.
					}break;
				case BATTLE_MODE://���������
					{
						MP3(SN_FIGHT );
						SetBattlePeace( PK_MODE );//PK���� �ٲ� �ְ�
					//	AddCurrentStatusMessage( 255,255,255, kein_GetMenuString(96) );	���ڿ� ������ �ȵǴ� ����Դϴ�.
					}break;
				case PK_MODE://������ ����� 
					{
						MP3(SN_FIGHT );
						SetBattlePeace( PEACE_MODE );//��ȭ���� �ٲ�����
						AddCurrentStatusMessage(FONT_COLOR_WHITE, lan->OutputMessage(0,1) );//��ȭ���� ��ȯ�մϴ�.
					}break;
				default:
					{
					}break;
				}
			}
		}break;
	case FT_SELECT_WISPER_TARGET:
		{
			bool bOn = SMenu[MN_WISPER_LIST].bActive;//���� �־��ٸ� 
			CallWisperList(!bOn);//�ݰ� 
			if(bOn)//������ ���� . �� �Ʊ� ���� �־����ϱ� 
			{
				SetChatTarget(CHAT_TARGET_NORMAL);
			}
		}break;
	case FT_SELECT_WISPER:
		{
			switch(iSwd)
			{
			case CHATTYPE_NORMAL	:
				{
					SetChatTarget(nWillDo);
					
				}break;
			case CHATTYPE_GUILD		:
				{
					SetChatTarget(nWillDo);
					
				}break;
			case CHATTYPE_PARTY		:
				{
					SetChatTarget(nWillDo);
					
				}break;
			case CHATTYPE_WHISPER	:
				{
					if(IsAbleChatTargetName(nWillDo))//�̸��� �ִٸ�//������ ���� �Ǿ�� �ȵȴ�(�ƹ��ϵ� ���� �ʴ� ��ư�̴�)
					{	//�̸��� �ִٸ� �̸��� ������ �ְ� 
						SetChatTarget(nWillDo);
						if(GetChatTarget() == SELECT_WISPER_MODE)//���̸� ����
						{
							SetChatTarget(SELECT_WISPER_MODE);
						}
						else//���̸� ������ �ƴ϶��
						{
							EWndMgr.SetTxt( HWND_CHAT_TARGET_INPUT, SubMenu->nField[j].temp );//021001 lsw
							
						}
					}
					else
					{
						work = 0;	//�븻 ä��
					}
				}break;
			}
			CallWisperList(false);//�ݱ�� ����
		}break;
	case FT_CHAT_LOCK:
		{
			if(!nWillDo)
			{
				nWillDo = 1;
				AddCurrentStatusMessage(FONT_COLOR_BLIGHT_GREEN2,lan->OutputMessage(0,609));//020705 lsw
			}
			else
			{
				nWillDo = 0;
				AddCurrentStatusMessage(FONT_COLOR_BLIGHT_GREEN,lan->OutputMessage(0,610));//020705 lsw
			}			
		}break;
	case FT_SELECT_CHAT_TYPE:
		{//nWillDo�� ����
			switch(GetChatTarget())
			{
			case 2:
				{
					SetChatTarget(CHAT_TARGET_NORMAL);
				}break;
			case 1: //MN_WISPER_LIST�� ���� ?���°� �ٸ���
				{
					SetChatTarget(CHAT_TARGET_PARTY);//���� ä�� ���� ��ȯ
				}break;
			case 0:
				{
					SetChatTarget(CHAT_TARGET_GUILD);//��� ä������ ��ȯ
				}break;

			}
		}break;
	case	FT_CHAT_VIEW_SET:
		{
			const int iChatType = iSwd;
			const int iOn = ChatMgr.GetViewChat(iChatType);
			ChatMgr.SetViewChat(iChatType,!iOn);//021001 lsw
		}break;
	case	FT_DRAG_ZONE:
		{	
			if (Hero->peacests != 0) {
				AddCurrentStatusMessage(FONT_COLOR_NORMAL, "only in peace mode");
				break;
			}
			
			if(nWillDo)
			{
				nWillDo = 0;
			}
			else
			{
				nWillDo = 1;
			}
		}break;

	case FT_HOTKEY_ICON://020701 lsw
		{
			const int iNo = HotKeyMemory.iNo;
			if(!iNo || strcmp(Hero->name,HotKeyMemory.OwnerName)) //�ڱ� �̸� �ƴϸ� ���� �ȵȴ�
			{
				break;
			}

			switch(HotKeyMemory.iType)
			{
			case QM_MAGIC:
				{
					const int nMagic = g_mgrBattle.Convert(iNo);
					if (GetMagic()->IsEnable(nMagic))
					{	//< CSD-TW-030606
						RectImageOn = true; 
						MagicOnFlag.SetMagicOn(iNo, iMainX+iFieldX+15, iMainY+iFieldY+15, 0);
						g_mgrBattle.SendMagicSelect(iNo);
					}	//> CSD-TW-030606
				}break;
			case QM_SKILL:
				{
					if( !iNo || !skill[iNo].num) break;
					MP3( SN_SKILL );

					if( SelectSkill(iNo)==1 ) //��ų�� ��� ���̶��
					{	// ��ų ���		KHS
						SkillSettingClear( );
						break;
					}
					else 									
					{
						SkillOn = 1;
						SelectSkill( iNo );
						y_MenuFocus = 0;
						CanMakeItemListSet( iNo );
					}
				}break;
			case QM_COMBATSKILL:
				{
				}break;
			}
		}break;					  
	case FT_MERCHANT_RESULT_ITEM_CHECK: 
		{
			if(nWillDo)//�޾� �;���
			{
				if(nWillDo == Auction.GetTakeItemIndex())
				{
					Auction.SetTakeItemIndex(0);
				}
				else
				{
					Auction.SetTakeItemIndex(nWillDo);
				}
			}
		}break;
	case FT_MERCHANT_DIRECT_BUY_ITEM_CHECK:
	case FT_MERCHANT_BUY_ITEM_CHECK:
		{
			if(nWillDo)
			{
				if(nWillDo == Auction.GetBuyItemIndex() )//������ �� ���ȳ� �Ѥ�;.
				{
					Auction.SetBuyItemIndex(0);
				}
				else
				{
					Auction.SetBuyItemIndex(nWillDo);
				}
			}
		}break;
	case FT_MERCHANT_SELL_ITEM_CHECK:
		{
			if(nWillDo)
			{
				if(nWillDo == Auction.GetDeleteItemIndex())//������ �� ���ȳ� �Ѥ�;.
				{
					Auction.SetDeleteItemIndex(0);
				}
				else
				{
					Auction.SetDeleteItemIndex(nWillDo);
				}
			}
		}break;
	case FT_EXCHANGE_ITEM_CHECK : 
		{
			if( nWillDo == RIGHT ) // Only Right Side
			{
				const bool bMySideOk	= ExchangeMgr.IsMySideConformStateOk();
				const bool bOtherSideOk	= ExchangeMgr.IsOtherSideConformStateOk();
				if(!bMySideOk)
				{
					ExchangeMgr.SetMySideConformState(true);// Change My side
					ExchangeMgr.SendExchangeStateSign(true);// Send Now State

					if( bMySideOk && bOtherSideOk )	// �ŷ� ����!
					{
						ExchangeMgr.SendExchangeResult();
					}
				}
				else//030108 lsw
				{
					ExchangeMgr.SetOtherSideConformState(false);
					ExchangeMgr.SetMySideConformState(false);// Change My side
					ExchangeMgr.SendExchangeStateSign(false);// Send Now State
				}
			}
		}break;
	case FT_MAN_TO_MAN_CHECK :
		{
			if( RIGHT == nWillDo && work == 1 )	// ������ ������ ��û�ߴ�.
			{
				// ���� ����... ������ ������.
				::SendManToManStart( SubMenu->key );
				SubMenu->bActive = false;
			}
		}break;
	case FT_EXCHANGE_ITEM : 
		{
			if(!IsExchangeNow())
			{
				break;
			}
			if( nWillDo == RIGHT )	// ���� �� ������
			{
				if( g_MouseItemType == 1 )//�������� ��� �ִٸ�
				{
					CItem* i = ItemUnit(HandItemAttr.item_no); // finito 040707 check if item is noechange an therefore can't drop
					if (!i) break; 

					if (i->GetItemKind() == IK_NOEXCHANGE) 
					{
						Kein_PutMessage( KM_FAIL, kein_GetMenuString(245) );
						break;
					}

					int a;
					for( a=0; a<MAX_EXCHANGE_ITEM; a++ )
					{
						if( !item_give[a].item.item_attr.item_no )
						{
							if( IP_base.type == EQUIP )
							{
								int a=0, b=0, c=0;
								if( ::SearchEmpty(a, b, c) )
								{
									::SetItemPos( INV, a, b, c, &IP_base );
								}
								else break;
							}
							else if( IP_base.type != QUICK && IP_base.type != INV )		// �׿��� ��ġ������ �������� �޾����� �ʴ´�.
							{
								break;
							}

							::PutMouseItemCheck();		// ������ �������� �ǵ��� ���´�.
							::SetExchageItemData(a,*GetItemByPos(IP_base),IP_base,true);//021121 lsw
							break;
						}
					}
					if(a ==MAX_EXCHANGE_ITEM)
					{
						MP3( SN_WARNING );
					}
				}
				else
				{
					MP3( SN_WARNING );
				}
			}
		}break;
			//011004 lsw >
	case FT_GM_2ND_SKILL_MAKE_ITEM : //單G
		{//�÷����� ���� //�ڵ忡 �������� �ִ°�?. -> ���µ����콺 ���ϸ� ����Ʈ�� ����(������) ���� 
			int ct=0;
			if( g_MouseItemType != 1) 
			{
				for( int a=0; a<MAX_UPGRADE_RARE; a++ )
				{
					if( gm_make_item_list[a].item_no && MouseInRectCheak( SMenu[i].x, SMenu[i].y, gm_make_item_list[a].rect ) )
					{
						memset( &gm_make_item_list[a], 0, sizeof( MakeItem ) );
						break;
					}
				}
				break;
			}

			int blankpos  = -1;
			for( int a=0; a < MAX_UPGRADE_RARE ; a++ )
			{
				if( gm_make_item_list[a].item_no ) 
				{
					ct++;//����ִ� ������ ����
				}
				else
				{
					blankpos = a;
				}
			}//010708 lsw ī��Ʈ ����
			
			if( ct>=MAX_UPGRADE_RARE) break; // �� á��. 

			//ó�� ��� ���� ó���� ��� �ü� �ִ� ���������� Ȯ��(������ �ѹ�, �Ӽ� �ִ°�)�� �߰�
			if( IP_base.type != INV ) 
			{
				AddCurrentStatusMessage(FONT_COLOR_WHITE , lan->OutputMessage(2,530));//�κ��丮 ������ ���� �ʼ�..
				break;
			}
			if( ct == 0 )//�ƹ��͵� ������ ����� ���� �ϰ� 0���� �ִ´�
			{	 
				if( !CheckBy2ndGM_This_Item_Upgradeable(HandItemAttr,0))
				{
					break;//�߰� �Ұ���
				}
			}
			else
			{
				if(!CheckBy2ndGM_Insert_to_GMItemList(HandItemAttr,0))//ó�� �������� �ƴϸ�
					break;//�߰� �Ұ���
			}
			if ((HandItemAttr.attr[IATTR_ATTR] & IA2_LOCK_ITEM) == IA2_LOCK_ITEM) {

				AddCurrentStatusMessage(FONT_COLOR_WHITE, lan->OutputMessage(0, 80));
				LOCK_ITEM = true;
			}

			gm_make_item_list[blankpos].item_no = HandItemAttr.item_no;
			gm_make_item_list[blankpos].x = g_pointMouseX-SMenu[i].x;
			gm_make_item_list[blankpos].y = g_pointMouseY-SMenu[i].y;
			::SetRect( &gm_make_item_list[blankpos].rect, gm_make_item_list[blankpos].x-15,gm_make_item_list[blankpos].y-15,gm_make_item_list[blankpos].x+15,gm_make_item_list[blankpos].y+15 );
			gm_make_item_list[blankpos].attr = HandItemAttr;
			gm_make_item_list[blankpos].pos = IP_base;		// �κ��丮 ���� ��Ҹ� ����Ѵ�.
		}break;
	case FT_SKILL_MASTER_MAKE_ITEM://雙G
		{//�÷����� ���� //�ڵ忡 �������� �ִ°�?. -> ���µ����콺 ���ϸ� ����Ʈ�� ����(������) ���� 
			int ct=0;
			if( g_MouseItemType != 1) 
			{
				for( int a=0; a<2; a++ )
				{
					const ItemAttr Item = g_MultiRareSrc.MultiRareSrc[a].attr;
					if( Item.item_no
					&& 	MouseInRectCheak( SMenu[i].x, SMenu[i].y, g_MultiRareSrc.MultiRareSrc[a].rect ) )
					{
						memset( &g_MultiRareSrc.MultiRareSrc[a], 0, sizeof( MakeItem ) );
						break;
					}
				}
				break;
			}

			int blankpos  = -1;
			for( int a=0; a < 2 ; a++ )
			{
				const ItemAttr Item = g_MultiRareSrc.MultiRareSrc[a].attr;
				if( Item.item_no ) 
				{
					ct++;//����ִ� ������ ����
				}
				else
				{
					blankpos = a;
				}
			}//�� �� �ִ� �ڸ� üũ
			
			if( ct>=2)
			{
				break; // �� á��. 
			}

			//ó�� ��� ���� ó���� ��� �ü� �ִ� ���������� Ȯ��(������ �ѹ�, �Ӽ� �ִ°�)�� �߰�
			if( IP_base.type != INV ) 
			{
				AddCurrentStatusMessage(FONT_COLOR_WHITE , lan->OutputMessage(2,530));//�κ��丮 ������ ���� �ʼ�..
				break;
			}
			if( ct == 0 )
			{	 
				if( !CheckBy2ndGM_This_Item_Upgradeable(HandItemAttr,1))
				{//���� ���� ���� �Ҷ� ��Ƽ�� ���� ���� �ִ�
					break;//�߰� �Ұ���
				}
			}
			if(	ct == 1 )//��� üũ�� �ϴ¹�
			{	 
				if( !CheckBy2ndGM_Insert_to_GMItemList(HandItemAttr,1))
				{//���� ���� ���� �Ҷ� ��Ƽ�� ���� ���� �ִ�
					break;//�߰� �Ұ���
				}
			}

			if ((HandItemAttr.attr[IATTR_ATTR] & IA2_LOCK_ITEM) == IA2_LOCK_ITEM) {

				AddCurrentStatusMessage(FONT_COLOR_WHITE, lan->OutputMessage(0, 80));
				LOCK_ITEM = true;
			}

			g_MultiRareSrc.MultiRareSrc[blankpos].item_no	= HandItemAttr.item_no;
			g_MultiRareSrc.MultiRareSrc[blankpos].x			= g_pointMouseX-SMenu[i].x;
			g_MultiRareSrc.MultiRareSrc[blankpos].y			= g_pointMouseY-SMenu[i].y;
			::SetRect( &g_MultiRareSrc.MultiRareSrc[blankpos].rect,
				g_MultiRareSrc.MultiRareSrc[blankpos].x-15,
				g_MultiRareSrc.MultiRareSrc[blankpos].y-15,
				g_MultiRareSrc.MultiRareSrc[blankpos].x+15,
				g_MultiRareSrc.MultiRareSrc[blankpos].y+15 );
			g_MultiRareSrc.MultiRareSrc[blankpos].attr		= HandItemAttr;
			g_MultiRareSrc.MultiRareSrc[blankpos].pos		= IP_base;		// �κ��丮 ���� ��Ҹ� ����Ѵ�.

		}break;
	case	FT_MERCHANT_SELL_ITEM_DROP_ZONE: 
		{//�÷����� ���� 
			if( g_MouseItemType != 1) //�ڵ忡 �������� �ִ°�?. -> ���µ����콺 ���ϸ� ����Ʈ�� ����(������) ���� 
			{
				break;
			}
			Auction.SetSellItem(IP_base,HandItemAttr);
		}break;
	case FT_EXCHANGE_BOND_MONEY_ITEM:
		{//������ �ø�
			if( g_MouseItemType != 1) //�ڵ忡 �������� �ִ°�?. -> ���µ����콺 ���ϸ� ����Ʈ�� ����(������) ���� 
			{
				break;
			}
			switch(HandItemAttr.item_no)
			{
			case BOND_MONEY_1000:
			case BOND_MONEY_500:
			case BOND_MONEY_100:
			case BOND_MONEY_50:
			case BOND_MONEY_10:
			case BOND_MONEY_5:
				{
					Auction.SetBondItem(IP_base,HandItemAttr);
				}break;
			default:
				{
					::AddCurrentStatusMessage( FONT_COLOR_SOLID_YELLOW, lan->OutputMessage(4,560));
				}break;
			}
		}break;
	case FT_HUNTER_REGIST :
	case FT_MAIL_WRITE :
		{
			SMenu[i].work = SMenu[i].nField[j].nWillDo;
		}break;
	case FT_FRIEND_NAME: // 好友名單
	{
		// 1. 取得分頁資訊
		// 注意：這裡假設 SubMenu->work 存儲了頁面狀態 (page, page_max)
		int& click = g_Menu_Variable.m_FriendGrup->click;
		int& page_status = SubMenu->work;
		WORD current_page;
		WORD max_pages;
		GetWORDOfDWORD((DWORD)page_status, current_page, max_pages);

		// 2. 定義常量
		const int MAX_FRIENDS = 30;     // 好友列表總上限
		const int LINES_PER_PAGE = 10;  // 每頁顯示的行數

		// 3. 初始化計數器
		// real_count: 總共處理過的好友數量 (用於分頁跳過)
		// display_count: 當前頁面已顯示的好友數量
		int total_processed_count = 0;
		int current_page_display_count = 0;

		// 計算當前頁面應該跳過多少好友
		const int SKIP_COUNT = current_page * LINES_PER_PAGE;

		// 4. 優先處理登入好友
		// 第一次迴圈只處理登入好友 (login == true)
		// 處理順序：登入 -> 未登入 (保持原邏輯)

		// (A) 處理登入的好友
		for (int i = 0; i < MAX_FRIENDS; ++i)
		{
			// 如果當前頁面已滿，則跳出迴圈
			if (current_page_display_count >= LINES_PER_PAGE) break;

			int is_logged_in = 0;
			char* friend_name = g_Menu_Variable.m_FriendGrup->GetFriend(i, is_logged_in);

			// 檢查好友名字是否有效 且 必須是登入狀態
			if (!friend_name[0] || !is_logged_in) continue;

			// --- 分頁邏輯 ---
			// 判斷是否需要跳過 (如果 total_processed_count < SKIP_COUNT)
			if (total_processed_count < SKIP_COUNT)
			{
				total_processed_count++;
				continue;
			}

			// --- 點擊處理邏輯 (僅處理登入好友的點擊) ---
			// 檢查區域 2:  寫郵件
			char* clicked_name_mail = g_Menu_Variable.m_FriendGrup->CheckRect(i, 2, 0, 0);
			if (clicked_name_mail)
			{
				SmallMenuClose();
				::Call_MAIL_WRITE_MENU(clicked_name_mail);
				return true;
			}

			// 檢查區域 1: 密語
			char* clicked_name_whisper = g_Menu_Variable.m_FriendGrup->CheckRect(i, 1, 0, 0);
			if (clicked_name_whisper)
			{
				SendWisperByLineCommand(clicked_name_whisper, "");
				return true;
			}

			char* clicked_name_delete = g_Menu_Variable.m_FriendGrup->CheckRect(i, 3, 0, 0);
			if (clicked_name_delete)
			{
				++click;
				if (click >= 3) {
					click = 0;
					k_delete_friend data;
					strcpy(data.name1, Hero->name);
					strcpy(data.name2, clicked_name_delete);
					SendDirectDB(CMD_DELETE_FRIEND, &data, sizeof(k_delete_friend));
					g_Menu_Variable.m_FriendGrup->DeleteName(i);
					AddCurrentStatusMessage(FONT_COLOR_NORMAL, lan->OutputMessage(0, 96), clicked_name_delete);
				}
				return true;
			}
			// 朋友已顯示在當前頁面上
			current_page_display_count++;
			total_processed_count++;
		}

		// (B) 處理未登入的好友 (重新開始計數，但 total_processed_count 不重設)
		for (int i = 0; i < MAX_FRIENDS; ++i)
		{
			// 如果當前頁面已滿，則跳出迴圈
			if (current_page_display_count >= LINES_PER_PAGE) break;

			int is_logged_in = 0;
			char* friend_name = g_Menu_Variable.m_FriendGrup->GetFriend(i, is_logged_in);

			// 檢查好友名字是否有效 且 必須是未登入狀態
			if (!friend_name[0] || is_logged_in) continue;

			// --- 分頁邏輯 ---
			// 判斷是否需要跳過
			if (total_processed_count < SKIP_COUNT)
			{
				total_processed_count++;
				continue;
			}

			// --- 點擊處理邏輯 (僅處理未登入好友的點擊) ---
			// 檢查區域 2: 寫郵件
			char* clicked_name_mail = g_Menu_Variable.m_FriendGrup->CheckRect(i, 2, 0, 0);
			if (clicked_name_mail)
			{
				SmallMenuClose();
				::Call_MAIL_WRITE_MENU(clicked_name_mail);
				return true;
			}

			char* clicked_name_delete = g_Menu_Variable.m_FriendGrup->CheckRect(i, 3, 0, 0);
			if (clicked_name_delete)
			{
				++click;
				if (click >= 3) {
					click = 0;
					k_delete_friend data;
					strcpy(data.name1, Hero->name);
					strcpy(data.name2, clicked_name_delete);
					SendDirectDB(CMD_DELETE_FRIEND, &data, sizeof(k_delete_friend));
					g_Menu_Variable.m_FriendGrup->DeleteName(i);
					AddCurrentStatusMessage(FONT_COLOR_NORMAL, lan->OutputMessage(0, 96), clicked_name_delete);
				}
				return true;
			}
			// 未登入好友沒有密語選項 (原程式碼只檢查了區域 2)

			// 朋友已顯示在當前頁面上
			current_page_display_count++;
			total_processed_count++;
		}

	} break;
	case FT_MAIL_LIST://Ŭ���ÿ�
		{	//Ÿ��, �������, ����, ��¥
			for(int i = 0; MAX_MAIL_PAGE > i; i++)
			{
				const int iIndex = g_MailMgr.GetMailIndex(i);

				if(!iIndex)//�ε��� �� ���� ���̸� ����
				{
					continue;
				}
				
				const int iX = iMainX+iFieldX;
				const int iY = iMainY+iFieldY+ CMailMgr::LIST_LINE_GAB*i+6;//���� ����

				//���õ� ��ũ
				const bool bIsSelect = g_MailMgr.IsSelected(i);
				if( BoxAndDotCrash( iX+6,iY-1, 17, 17, g_pointMouseX, g_pointMouseY ) )
				{
					g_MailMgr.SetSelected(i,!bIsSelect);
				}

				//�������
				if( BoxAndDotCrash( iX+53,iY-3, 100, 18, g_pointMouseX, g_pointMouseY ) )
				{
					std::string szSender = g_MailMgr.GetMailSender(i);
					::Call_MAIL_WRITE_MENU(const_cast<char*>(szSender.c_str()));
				}

				//����
				if( BoxAndDotCrash( iX+162,iY-3, 130, 18, g_pointMouseX, g_pointMouseY ) )
				{
					g_MailMgr.RequestBody(iIndex);
					::CloseAllMenu();
				}
			}
		}break;
	case FT_SELECT_OBSERVER_TARGET_TEAM:
		{
			SetArenaObserverTarget(nWillDo);
		}break;
	case FT_SELECT_BETTING_TARGET_TEAM:
		{
			::SetArenaBettingTarget(nWillDo);
		}break;
	case FT_TODAY_NOTICE:
		{	
			if (SMenu[i].nField[j].nWillDo > 0 )
			{
				SMenu[i].nField[j].nWillDo--;
			}
			if(g_iScenarioOn){g_iScenarioOn =0;}
			CloseAllMenu();
			WarMenuNoticeOpen();//011013 lsw
			CallServer( CMD_GET_SCENARIO_INFO );
		}break;
	case FT_MAININTERFACE_MAIL_CALL://MailMenu Call
		{
			g_MailMgr.SendReqMailList(nWillDo,0);
		}break;
	default:
		{
			return false;
		}break;
	}
	return true;
}



void kh_menu_set( )
{
/////////////////////////////////////////////////////////////////////////////////////
// ����Ʈ �޴�
	SMenu[MN_QUEST].nImageType= QUEST_MENU;
	SMenu[MN_QUEST].x= 132+GABX_SCREEN;
	SMenu[MN_QUEST].y= 100;			
	SMenu[MN_QUEST].nFieldCount	= 4;
	SMenu[MN_QUEST].nImageNumber = 1;


	// ���� å�� �ѱ��
	SMenu[MN_QUEST].nField[0].nImageType = QUEST_MENU;
	SMenu[MN_QUEST].nField[0].nType = FT_QUEST_SCROLL;			// ó���ϴ� �� MenuSubProcessType() �׸��� ��� �ֵ��� �ϴ°�, DoButtonCheckOfMenu() ���콺 Ŭ�� ����..
	SMenu[MN_QUEST].nField[0].nWillDo = 0;		// left
	//SMenu[MN_QUEST].nField[0].nType = FT_DSCROLL_MEM_BOOK;
	SMenu[MN_QUEST].nField[0].x=39;
	SMenu[MN_QUEST].nField[0].y=64;
	SMenu[MN_QUEST].nField[0].nRectImage=5;
	SMenu[MN_QUEST].nField[0].nImageNumber=5;
	::SetRect(SMenu[MN_QUEST].nField[0].rCheakBox,39,64,39+167,64+107);

	// ������ å�� �ѱ��
	SMenu[MN_QUEST].nField[1].nImageType = QUEST_MENU;
	SMenu[MN_QUEST].nField[1].nType = FT_QUEST_SCROLL;			// ó���ϴ� �� MenuSubProcessType() �׸��� ��� �ֵ��� �ϴ°�, DoButtonCheckOfMenu() ���콺 Ŭ�� ����.. 
	SMenu[MN_QUEST].nField[1].nWillDo = 1;		// right
	//SMenu[MN_QUEST].nField[1].nType = FT_ISCROLL_MEM_BOOK;
	SMenu[MN_QUEST].nField[1].x=330;
	SMenu[MN_QUEST].nField[1].y=193;
	SMenu[MN_QUEST].nField[1].nRectImage=4;
	SMenu[MN_QUEST].nField[1].nImageNumber=4;
	::SetRect(SMenu[MN_QUEST].nField[1].rCheakBox,330,193,330+35,193+15);

	// go index
	SMenu[MN_QUEST].nField[2].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_QUEST].nField[2].nSpecialWillDo = SWD_SUBMENU_BACK;	
	SMenu[MN_QUEST].nField[2].x=310;
	SMenu[MN_QUEST].nField[2].y=17;
	SMenu[MN_QUEST].nField[2].nRectImage=3;
	SMenu[MN_QUEST].nField[2].nImageNumber=3;
	SMenu[MN_QUEST].nField[2].nImageType=QUEST_MENU;
	::SetRect(SMenu[MN_QUEST].nField[2].rCheakBox,310,17,368,35);

	// EXIT	
	SMenu[MN_QUEST].nField[3].nImageType=QUEST_MENU;
	SMenu[MN_QUEST].nField[3].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_QUEST].nField[3].nSpecialWillDo = SWD_SMALLMENU_END;	
	SMenu[MN_QUEST].nField[3].x=0;
	SMenu[MN_QUEST].nField[3].y=19;
	SMenu[MN_QUEST].nField[3].nRectImage=2;
	SMenu[MN_QUEST].nField[3].nImageNumber=2;
	::SetRect(SMenu[MN_QUEST].nField[3].rCheakBox,0,19,42,54);
	// ���� ��� (�迭)// ����
	/*
	SMenu[MN_QUEST].nField[3].nType = FT_QUEST_SCROLL;
	SMenu[MN_QUEST].nField[3].x=96+35;
	SMenu[MN_QUEST].nField[3].y=33+26+80;
	SMenu[MN_QUEST].nField[3].nImageNumber=DP_MAGIC_CLASS;

	// ���� ��� (�迭)// ������
	SMenu[MN_QUEST].nField[4].nType = FT_QUEST_SCROLL;
	SMenu[MN_QUEST].nField[4].x=265+35-5;
	SMenu[MN_QUEST].nField[4].y=33+26+80;
	SMenu[MN_QUEST].nField[4].nImageNumber=DP_MAGIC_CLASS;
*/

//
// ����Ʈ ��� ����ϱ�
//
/////////////////////////////////////////////////////////////////	
	SMenu[MN_QUEST_INDEX].nImageType= QUEST_MENU;
	SMenu[MN_QUEST_INDEX].x= 132+GABX_SCREEN;
	SMenu[MN_QUEST_INDEX].y= 100;			
	SMenu[MN_QUEST_INDEX].nFieldCount = 5;
	SMenu[MN_QUEST_INDEX].nImageNumber = 0;
	SMenu[MN_QUEST_INDEX].key = 14;		// ���������� ��°����� �ε��� �ִ밪

	
	SMenu[MN_QUEST_INDEX].nField[0].nType = FT_QUEST_INDEX;
	SMenu[MN_QUEST_INDEX].nField[0].nWillDo = 0;	
	SMenu[MN_QUEST_INDEX].nField[0].x=33;
	SMenu[MN_QUEST_INDEX].nField[0].y=64;
	SMenu[MN_QUEST_INDEX].nField[0].nRectImage=268;
	::SetRect(SMenu[MN_QUEST_INDEX].nField[0].rCheakBox,0,40,200,193);

	SMenu[MN_QUEST_INDEX].nField[1].nType = FT_QUEST_INDEX;
	SMenu[MN_QUEST_INDEX].nField[1].nWillDo = 1;	
	SMenu[MN_QUEST_INDEX].nField[1].x=33;
	SMenu[MN_QUEST_INDEX].nField[1].y=64;
	SMenu[MN_QUEST_INDEX].nField[1].nRectImage=268;
	::SetRect(SMenu[MN_QUEST_INDEX].nField[1].rCheakBox,200,40,400,193);

	
	//���� å�� �ѱ�� 
	SMenu[MN_QUEST_INDEX].nField[2].nImageType = QUEST_MENU;
	SMenu[MN_QUEST_INDEX].nField[2].nType = FT_QUEST_INDEX_SCROLL;			// ó���ϴ� �� MenuSubProcessType() �׸��� ��� �ֵ��� �ϴ°�, DoButtonCheckOfMenu() ���콺 Ŭ�� ����..
	SMenu[MN_QUEST_INDEX].nField[2].nWillDo = 0;		// left
	//SMenu[MN_QUEST_INDEX].nField[2].nType = FT_DSCROLL_MEM_BOOK;
	SMenu[MN_QUEST_INDEX].nField[2].x=51;
	SMenu[MN_QUEST_INDEX].nField[2].y=193;
	SMenu[MN_QUEST_INDEX].nField[2].nRectImage=5;
	SMenu[MN_QUEST_INDEX].nField[2].nImageNumber=5;
	::SetRect(SMenu[MN_QUEST_INDEX].nField[2].rCheakBox,51,193,50+35,193+15);

	// ������ å�� �ѱ��
	SMenu[MN_QUEST_INDEX].nField[3].nImageType = QUEST_MENU;
	SMenu[MN_QUEST_INDEX].nField[3].nType = FT_QUEST_INDEX_SCROLL;			// ó���ϴ� �� MenuSubProcessType() �׸��� ��� �ֵ��� �ϴ°�, DoButtonCheckOfMenu() ���콺 Ŭ�� ����.. 
	SMenu[MN_QUEST_INDEX].nField[3].nWillDo = 1;		// right
	//SMenu[MN_QUEST_INDEX].nField[3].nType = FT_ISCROLL_MEM_BOOK;
	SMenu[MN_QUEST_INDEX].nField[3].x=330;
	SMenu[MN_QUEST_INDEX].nField[3].y=193;
	SMenu[MN_QUEST_INDEX].nField[3].nRectImage=4;
	SMenu[MN_QUEST_INDEX].nField[3].nImageNumber=4;
	::SetRect(SMenu[MN_QUEST_INDEX].nField[3].rCheakBox,330,193,330+35,193+15);

	// EXIT	
	SMenu[MN_QUEST_INDEX].nField[4].nImageType=QUEST_MENU;
	SMenu[MN_QUEST_INDEX].nField[4].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_QUEST_INDEX].nField[4].nSpecialWillDo = SWD_SMALLMENU_END;	
	SMenu[MN_QUEST_INDEX].nField[4].x=0;
	SMenu[MN_QUEST_INDEX].nField[4].y=19;
	SMenu[MN_QUEST_INDEX].nField[4].nRectImage=2;
	SMenu[MN_QUEST_INDEX].nField[4].nImageNumber=2;
	::SetRect(SMenu[MN_QUEST_INDEX].nField[4].rCheakBox,0,19,42,54);
	
	
//------------------------------------------------------------------------------------------------
// ��� �޴� 
//------------------------------------------------------------------------------------------------
// ����޴� 
	SMenu[MN_FAME].x= 132+GABX_SCREEN;
	SMenu[MN_FAME].y= 50;			
	SMenu[MN_FAME].nFieldCount = 6;
	SMenu[MN_FAME].nImageType= FAME_MENU;
	SMenu[MN_FAME].nImageNumber = 0;

////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------------------------
// ĳ���� ���󺯰� ���� ���̽� 
//------------------------------------------------------------------------------------------------

	//���� �޴� �ٿ��
	SMenu[MN_CHARACTER_COLOR_CHANGE].x=CENTER_X;
	SMenu[MN_CHARACTER_COLOR_CHANGE].y=100;
	SMenu[MN_CHARACTER_COLOR_CHANGE].nImageNumber=15;
	SMenu[MN_CHARACTER_COLOR_CHANGE].nImageType= SKIN_MENU;
	SMenu[MN_CHARACTER_COLOR_CHANGE].nFieldCount=8; 
// 	SMenu[MN_CHARACTER_COLOR_CHANGE].key=2;			// 1:body, 0:skin

	// �����
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[1].nType=FT_SKIN_MONEY;
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[1].x=45;
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[1].y=190;
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[1].nImageNumber=14;
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[1].nImageType=SKIN_MENU;
	::SetRect(SMenu[MN_CHARACTER_COLOR_CHANGE].nField[1].rCheakBox,97,199, 167, 212 );

	// ok button
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[2].nSpecialWillDo=SWD_CHARACTER_COLOR_CHANGE;
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[2].x=43;
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[2].y=232;
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[2].nRectImage=10;
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[2].nImageNumber=9;
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[2].nImageType=SKIN_MENU;
	::SetRect(SMenu[MN_CHARACTER_COLOR_CHANGE].nField[2].rCheakBox,43,232,43+76,232+27);

	// cancel button
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[3].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[3].nSpecialWillDo=SWD_SUBMENU_BACK;//021007 lsw
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[3].x=140;
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[3].y=232;
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[3].nRectImage=5;
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[3].nImageNumber=4;
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[3].nImageType=SKIN_MENU;
	::SetRect(SMenu[MN_CHARACTER_COLOR_CHANGE].nField[3].rCheakBox,140,232,140+76,232+27);

	// RGB �� �����ϱ� 
	//R
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[4].nType=FT_COLOR_CHANGE_GAUGE;
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[4].x=204;
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[4].y=78;
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[4].nImageNumber=401;
	::SetRect(SMenu[MN_CHARACTER_COLOR_CHANGE].nField[4].rCheakBox,55,76,193,90);

	//G
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[5].nType=FT_COLOR_CHANGE_GAUGE;
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[5].x=204;
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[5].y=115;
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[5].nImageNumber=486;
	::SetRect(SMenu[MN_CHARACTER_COLOR_CHANGE].nField[5].rCheakBox,55,113,193,127);

	//B
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[6].nType=FT_COLOR_CHANGE_GAUGE;
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[6].x=204;
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[6].y=150;
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[6].nImageNumber=503;
	::SetRect(SMenu[MN_CHARACTER_COLOR_CHANGE].nField[6].rCheakBox,55,148,193,162);

	// Mantle dye
	SMenu[MN_MANTLE_COLOR_CHANGE].x=CENTER_X;
	SMenu[MN_MANTLE_COLOR_CHANGE].y=100;
	SMenu[MN_MANTLE_COLOR_CHANGE].nImageNumber=0;
	SMenu[MN_MANTLE_COLOR_CHANGE].nImageType= MANTLEDYE_IMG;
	SMenu[MN_MANTLE_COLOR_CHANGE].nFieldCount=7; 
// 	SMenu[MN_CHARACTER_COLOR_CHANGE].key=2;			// 1:body, 0:skin

	// ok button
	SMenu[MN_MANTLE_COLOR_CHANGE].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_MANTLE_COLOR_CHANGE].nField[1].nSpecialWillDo=SWD_MANTLE_COLOR_CHANGE;
	SMenu[MN_MANTLE_COLOR_CHANGE].nField[1].x=43;
	SMenu[MN_MANTLE_COLOR_CHANGE].nField[1].y=232;
	SMenu[MN_MANTLE_COLOR_CHANGE].nField[1].nRectImage=8;
	SMenu[MN_MANTLE_COLOR_CHANGE].nField[1].nImageNumber=7;
	SMenu[MN_MANTLE_COLOR_CHANGE].nField[1].nImageType=MANTLEDYE_IMG;
	::SetRect(SMenu[MN_MANTLE_COLOR_CHANGE].nField[1].rCheakBox,43,232,43+76,232+27);

	// cancel button
	SMenu[MN_MANTLE_COLOR_CHANGE].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_MANTLE_COLOR_CHANGE].nField[2].nSpecialWillDo=SWD_MANTLEDYE_CANCEL;//021007 lsw
	SMenu[MN_MANTLE_COLOR_CHANGE].nField[2].x=140;
	SMenu[MN_MANTLE_COLOR_CHANGE].nField[2].y=232;
	SMenu[MN_MANTLE_COLOR_CHANGE].nField[2].nRectImage=6;
	SMenu[MN_MANTLE_COLOR_CHANGE].nField[2].nImageNumber=5;
	SMenu[MN_MANTLE_COLOR_CHANGE].nField[2].nImageType=MANTLEDYE_IMG;
	::SetRect(SMenu[MN_MANTLE_COLOR_CHANGE].nField[2].rCheakBox,140,232,140+76,232+27);

	// RGB �� �����ϱ� 
	//R
	SMenu[MN_MANTLE_COLOR_CHANGE].nField[3].nType=FT_MANTLE_COLOR_CHANGE_GAUGE;
	SMenu[MN_MANTLE_COLOR_CHANGE].nField[3].x=204;
	SMenu[MN_MANTLE_COLOR_CHANGE].nField[3].y=78;
	SMenu[MN_MANTLE_COLOR_CHANGE].nField[3].nImageNumber=401;
	::SetRect(SMenu[MN_MANTLE_COLOR_CHANGE].nField[3].rCheakBox,55,76,193,90);

	//G
	SMenu[MN_MANTLE_COLOR_CHANGE].nField[4].nType=FT_MANTLE_COLOR_CHANGE_GAUGE;
	SMenu[MN_MANTLE_COLOR_CHANGE].nField[4].x=204;
	SMenu[MN_MANTLE_COLOR_CHANGE].nField[4].y=115;
	SMenu[MN_MANTLE_COLOR_CHANGE].nField[4].nImageNumber=486;
	::SetRect(SMenu[MN_MANTLE_COLOR_CHANGE].nField[4].rCheakBox,55,113,193,127);

	//B
	SMenu[MN_MANTLE_COLOR_CHANGE].nField[5].nType=FT_MANTLE_COLOR_CHANGE_GAUGE;
	SMenu[MN_MANTLE_COLOR_CHANGE].nField[5].x=204;
	SMenu[MN_MANTLE_COLOR_CHANGE].nField[5].y=150;
	SMenu[MN_MANTLE_COLOR_CHANGE].nField[5].nImageNumber=503;
	::SetRect(SMenu[MN_MANTLE_COLOR_CHANGE].nField[5].rCheakBox,55,148,193,162);

	// Item mall reece
	SMenu[MN_ITEM_MALL].x=345+GABX_SCREEN;
	SMenu[MN_ITEM_MALL].y=20;
	SMenu[MN_ITEM_MALL].nImageNumber= 0;
	SMenu[MN_ITEM_MALL].nImageType = ITEMMALL_IMG;
	SMenu[MN_ITEM_MALL].nFieldCount = 7;
	SMenu[MN_ITEM_MALL].key = 15;
	SMenu[MN_ITEM_MALL].CheakType = 0;

	// Buy button
	SMenu[MN_ITEM_MALL].nField[0].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_ITEM_MALL].nField[0].nSpecialWillDo=SWD_MALL_ITEM_BUY;
	SMenu[MN_ITEM_MALL].nField[0].x=16;
	SMenu[MN_ITEM_MALL].nField[0].y=352;
	SMenu[MN_ITEM_MALL].nField[0].nRectImage=1;
	SMenu[MN_ITEM_MALL].nField[0].nImageNumber=2;
	SMenu[MN_ITEM_MALL].nField[0].nImageType = ITEMMALL_IMG;
	SetRect(SMenu[MN_ITEM_MALL].nField[0].rCheakBox,16, 352, 16+76, 352+27);

	// Close button
	SMenu[MN_ITEM_MALL].nField[1].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_ITEM_MALL].nField[1].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_ITEM_MALL].nField[1].x=136;
	SMenu[MN_ITEM_MALL].nField[1].y=352;
	SMenu[MN_ITEM_MALL].nField[1].nRectImage=3;
	SMenu[MN_ITEM_MALL].nField[1].nImageNumber=4;
	SMenu[MN_ITEM_MALL].nField[1].nImageType = ITEMMALL_IMG;
	SetRect(SMenu[MN_ITEM_MALL].nField[1].rCheakBox,136, 352, 136+76, 352+27);

	// Select item
	SMenu[MN_ITEM_MALL].nField[2].nType=FT_MALL_ITEM;
	SMenu[MN_ITEM_MALL].nField[2].nSHideNomalCount=5;
	SMenu[MN_ITEM_MALL].nField[2].nSHideNomalStart=0;
	SMenu[MN_ITEM_MALL].nField[2].nShideNomalPlus=1;	
	SMenu[MN_ITEM_MALL].nField[2].x=14;
	SMenu[MN_ITEM_MALL].nField[2].y=122;
	SetRect(SMenu[MN_ITEM_MALL].nField[2].rCheakBox,13,119,189,225);

	// Shows the currently selected item
	SMenu[MN_ITEM_MALL].nField[3].nType=FT_DATA_PUT;
	SMenu[MN_ITEM_MALL].nField[3].nWillDo=ITEM_BUY_MALL;
	SMenu[MN_ITEM_MALL].nField[3].x=43;
	SMenu[MN_ITEM_MALL].nField[3].y=242;
	SMenu[MN_ITEM_MALL].nField[3].nImageNumber=DP_SELECT_ITEM;

	// Scroll up
	SMenu[MN_ITEM_MALL].nField[4].nType=FT_HIDE_DSCROLL_AUTO_PUT;
	SMenu[MN_ITEM_MALL].nField[4].nWillDo = 2;
	SMenu[MN_ITEM_MALL].nField[4].x=193;
	SMenu[MN_ITEM_MALL].nField[4].y=129;
	SMenu[MN_ITEM_MALL].nField[4].nRectImage=57;		
	SMenu[MN_ITEM_MALL].nField[4].nImageNumber=57;		
	SMenu[MN_ITEM_MALL].nField[4].nImageType=RUNE_IMG;
	SetRect(SMenu[MN_ITEM_MALL].nField[4].rCheakBox,193, 129, 193+17, 129+17);

	// Scroll down
	SMenu[MN_ITEM_MALL].nField[5].nType=FT_HIDE_ISCROLL_AUTO_PUT;
	SMenu[MN_ITEM_MALL].nField[5].nWillDo = 2;
	SMenu[MN_ITEM_MALL].nField[5].x=193;
	SMenu[MN_ITEM_MALL].nField[5].y=199;
	SMenu[MN_ITEM_MALL].nField[5].nRectImage=59;			
	SMenu[MN_ITEM_MALL].nField[5].nImageNumber=59;			
	SMenu[MN_ITEM_MALL].nField[5].nImageType=RUNE_IMG;
	SetRect(SMenu[MN_ITEM_MALL].nField[5].rCheakBox,193, 199, 193+17, 199+17);

	SMenu[MN_ITEM_MALL].nField[6].nType=FT_DATA_PUT;
	SMenu[MN_ITEM_MALL].nField[6].nWillDo=74;
	SMenu[MN_ITEM_MALL].nField[6].x=74;
	SMenu[MN_ITEM_MALL].nField[6].y=298;
	SMenu[MN_ITEM_MALL].nField[6].nImageNumber=DP_CHARACTER_RAJA_EMBLEM;
}

bool IsSameItemNoANDPos(const int &iItemNoSrc ,const int &iItemNoDest, const POS &pos,const int iType, const int iFirst, const int iSecond, const int iThird)
{//�̰Ŷ� ��ġ�ϸ� ���÷��� ����
	if(iItemNoSrc == iItemNoDest
	&&	pos.type == iType
	&&	pos.p1 ==	iFirst
	&&	pos.p2 ==	iSecond
	&&	pos.p3 ==	iThird)
	{
		return true;
	}
	return false;
}

void ButtonDownAtInventory(const int i, const int j)
{
	const int ItemX = ( g_pointMouseX - (SMenu[i].x + 19) ) / 35;
	const int ItemY = ( g_pointMouseY - (SMenu[i].y + 210) ) / 35;
	if( ItemY >= 3 || ItemY < 0 || ItemX >= 8 || ItemX < 0 ) 
	{
		return;
	}
	
	const int ItemX2 = ( StateMousePoint.x - (SMenu[i].x + 19) ) / 35;
	const int ItemY2 = ( StateMousePoint.y - (SMenu[i].y + 210) ) / 35;
	if( ItemX2 != ItemX || ItemY2 != ItemY2 ) 
	{
		return;
	}
																	
	const int xCheck = ( g_pointMouseX - (SMenu[i].x + 19) ) % 35;			//��輱�� Ŭ���� ����
	const int yCheck = ( g_pointMouseY - (SMenu[i].y + 210) ) % 35;
	if( (xCheck < 2) || (xCheck > 30) || (yCheck < 2) || (yCheck > 30 ) )  
	{
		return;	
	}

	const int first = SMenu[i].nField[PAGE_NUMBER__].nSHideNomalStart;
	const int second = ItemY;
	const int third = ItemX;
	ItemAttr &item = InvItemAttr[first][second][third];

	if( g_MouseItemType )	
	{
		return;
	}

	if( SMenu[MN_GAMBLE].bActive )	// �۾��밡 Ȱ��ȭ ���� ��� �۾��뿡 ���� �ִ� �������� ���õǾ����� �ʴ´�.
	{
		int a;
		for(a=0; a<20; a++)
		{
			const POS &tempPos = make_item_list[a].pos;
			const ITEMATTR *pItem = &make_item_list[a].attr;
			if(::IsSameItemNoANDPos(item.item_no ,pItem->item_no, tempPos,INV,first, second, third))
			{
				return;
			}
		}
		if( a != 20 ) 
		{
			return;
		}
	}
	//010708 lsw
	if( SMenu[MN_GM_2ND_SKILL].bActive )	// GM ���۾��밡 Ȱ��ȭ ���� ��� �۾��뿡 ���� �ִ� �������� ���õǾ����� �ʴ´�.
	{
		int a;
		for(a=0; a<MAX_UPGRADE_RARE; a++)
		{
			const POS &tempPos = gm_make_item_list[a].pos;
			const ITEMATTR *pItem = &gm_make_item_list[a].attr;
			if(::IsSameItemNoANDPos(item.item_no ,pItem->item_no, tempPos,INV,first, second, third))
			{
				return;
			}
		}
		if( a != 20 ) 
		{
			return;
		}
	}
	if( SMenu[MN_SKILL_MASTER_MAKE].bActive )
	{
		int a;
		for( a=0; a<2; a++ )
		{
			const POS &tempPos = g_MultiRareSrc.MultiRareSrc[a].pos;
			const ITEMATTR *pItem = &g_MultiRareSrc.MultiRareSrc[a].attr;
			if(::IsSameItemNoANDPos(item.item_no ,pItem->item_no, tempPos,INV,first, second, third))
			{
				return;
			}
		}
		if( a!=2 ) 
		{
			return;
		}
	}
	if(SMenu[MN_MERCHANT_REGISTER].bActive)//021014 lsw
	{
		const POS &tempPos = Auction.GetSellItemPos();//�Ȱڴٰ� ���Ѱ� ��Ӱ� ����
		const ITEMATTR *pItem = Auction.GetSellItemAttr();
		if(pItem)
		{
			if(::IsSameItemNoANDPos(item.item_no ,pItem->item_no, tempPos,INV,first, second, third))
			{
				return;
			}
		}
	}
	if( IsExchangeNow() )
	{
		int a;
		for(a=0; a<MAX_EXCHANGE_ITEM; a++)
		{
			const POS &tempPos = item_give[a].item.item_pos;
			const ITEMATTR *pItem = &item_give[a].item.item_attr;
			if(::IsSameItemNoANDPos(item.item_no ,pItem->item_no, tempPos,INV,first, second, third))
			{
				return;
			}
		}
		if( a != MAX_EXCHANGE_ITEM ) 
		{
			return;
		}
	}

	if(SMenu[MN_EXCHANGE_BOND_MONEY].bActive)
	{
		const POS tempPos = Auction.GetBondItemPos();
		const ITEMATTR *pItem = Auction.GetBondItemAttr();
		if(pItem)
		{
			if(::IsSameItemNoANDPos(item.item_no ,pItem->item_no, tempPos,INV,first, second, third))
			{
				return;
			}
		}
	}

	if( item.item_no )
	{
		const int iItemNo = item.item_no;
		::ItemSoundOfGetItem(iItemNo);
		g_MouseItemType=1;
		g_MouseItemNumber = iItemNo;

		HandItemAttr = item;
		::SetItemPos(INV, first,ItemY,ItemX, &IP_base);
		POS pos_t;
		::SetItemPos(HAND, &pos_t);
		::DeleteItem( &item );
		::SendMoveItem( iItemNo, IP_base, pos_t );
	}
	return;
}

void	DisplayInventoryMenu(SMENU *SubMenu ,int j)
{
	int AddVert = 4; // ���� ������. //soto-1.02 4
	int	AddHorz = 6; // ���� ������. //soto-1.02 6
	
	int i=0, k=0;
	int first_para = SubMenu->nField[ PAGE_NUMBER__].nSHideNomalStart;

	
	for(k=0;k<3;k++) 
	{
		for(i=0;i<8;i++)  //�κ��丮â�� ���� �ִ� ũ��� 3
		{	//nField[2]�� ��ũ���� ���� ���� �ʵ��̴�//PAGE_NUMBER__
			const ItemAttr &put_item = InvItemAttr[first_para][k][i];
			if( put_item.item_no )
			{
				int put=1;
				if( SMenu[MN_GAMBLE].bActive )
				{
					for( int a=0; a<20; a++ )
					{
						const POS &pos		= make_item_list[a].pos;
						const int iItemNo	= make_item_list[a].item_no;
						if(::IsSameItemNoANDPos(put_item.item_no ,iItemNo, pos,INV,first_para, k, i))//������ ��ο���
						{
							::PutItemIcon(SubMenu->x+13+22+35*i + AddHorz, SubMenu->y+217+35*k + AddVert, put_item.item_no,15,1);
							put=0;
							break;
						}
					}
					if( put ) 
					{
						::PutItemIcon(SubMenu->x+13+22+35*i  + AddHorz, SubMenu->y+217+35*k + AddVert, put_item.item_no);
					}
				}
				else if( IsExchangeNow() )
				{
					for( int a=0; a<MAX_EXCHANGE_ITEM; a++ )
					{
						const POS &pos		= item_give[a].item.item_pos;
						const int iItemNo	= item_give[a].item.item_attr.item_no;
						if(::IsSameItemNoANDPos(put_item.item_no ,iItemNo, pos,INV,first_para, k, i))//������ ��ο���
						{
							::PutItemIcon(SubMenu->x+13+22+35*i  + AddHorz, SubMenu->y+217+35*k + AddVert, put_item.item_no,15,1);
							put=0;
							break;
						}
					}
					if( put ) 
					{
						::PutItemIcon(SubMenu->x+13+22+35*i  + AddHorz, SubMenu->y+217+35*k + AddVert, put_item.item_no);
					}
				}
				else if( SMenu[MN_GM_2ND_SKILL].bActive )//011001 lsw 2�� ���� ����
				{
					for( int a=0; a<MAX_UPGRADE_RARE; a++ )
					{
						const POS &pos = gm_make_item_list[a].pos;
						const int iItemNo = gm_make_item_list[a].item_no;
						if(::IsSameItemNoANDPos(put_item.item_no ,iItemNo, pos,INV,first_para, k, i))//������ ��ο���
						{
							::PutItemIcon(SubMenu->x+13+22+35*i  + AddHorz, SubMenu->y+217+35*k + AddVert, put_item.item_no,15,1);
							put=0;
							break;
						}
					}
					if( put )
					{
						
						if( !gm_make_item_result.item_no)//������� �ƹ��͵� ������ ����� �ִ� ������ ����Ʈ�� �ش�
						{
							::PutItemIcon(SubMenu->x+13+22+35*i  + AddHorz, SubMenu->y+217+35*k + AddVert, put_item.item_no);
						}
						else//��� ���� �������� ������� �ɼ� �ִ� ���̵鸸 ��� ��´�
						{
							if(	put_item.item_no == gm_make_item_result.item_no
							||	put_item.item_no == gm_make_item_result_needitem[0]
							||	put_item.item_no == gm_make_item_result_needitem[1])
							{
								::PutItemIcon(SubMenu->x+13+22+35*i  + AddHorz, SubMenu->y+217+35*k + AddVert, put_item.item_no);
							}
							else
							{
								::PutItemIcon(SubMenu->x+13+22+35*i  + AddHorz, SubMenu->y+217+35*k + AddVert, put_item.item_no, 15, 1);
							}
						}
					}
				}
				else if( SMenu[MN_SKILL_MASTER_MAKE].bActive )
				{
					for( int a=0; a<2; a++ )
					{
						const POS &pos = g_MultiRareSrc.MultiRareSrc[a].pos;
						const int iItemNo = g_MultiRareSrc.MultiRareSrc[a].item_no;
						if(::IsSameItemNoANDPos(put_item.item_no ,iItemNo, pos,INV,first_para, k, i))//������ ��ο���
						{
							::PutItemIcon(SubMenu->x+13+22+35*i  + AddHorz, SubMenu->y+217+35*k + AddVert, put_item.item_no,15,1);
							put=0;
							break;
						}
					}
					if(put)
					{
						::PutItemIcon(SubMenu->x+13+22+35*i  + AddHorz, SubMenu->y+217+35*k + AddVert, put_item.item_no);
					}
				}
				else if(SMenu[MN_MERCHANT_REGISTER].bActive)//021014 lsw
				{
					const POS &pos = Auction.GetSellItemPos();//�Ȱڴٰ� ���Ѱ� ��Ӱ� ����
					ItemAttr *pItem = Auction.GetSellItemAttr();
					if(pItem)
					{
						if(::IsSameItemNoANDPos(put_item.item_no ,pItem->item_no, pos,INV,first_para, k, i))//������ ��ο���
						{
							::PutItemIcon(SubMenu->x+13+22+35*i  + AddHorz, SubMenu->y+217+35*k + AddVert, put_item.item_no,15,1);
						}
						else
						{
							::PutItemIcon(SubMenu->x+13+22+35*i  + AddHorz, SubMenu->y+217+35*k + AddVert, put_item.item_no);
						}
					}
					else
					{
						::PutItemIcon(SubMenu->x+13+22+35*i  + AddHorz, SubMenu->y+217+35*k + AddVert, put_item.item_no);
					}
				}
				else if(SMenu[MN_EXCHANGE_BOND_MONEY].bActive)//021126 lsw
				{
					const POS &pos = Auction.GetBondItemPos();//�Ȱڴٰ� ���Ѱ� ��Ӱ� ����
					ItemAttr *pItem = Auction.GetBondItemAttr();
					if(pItem)
					{
						if(::IsSameItemNoANDPos(put_item.item_no ,pItem->item_no, pos,INV,first_para, k, i))//������ ��ο���
						{
							::PutItemIcon(SubMenu->x+13+22+35*i  + AddHorz, SubMenu->y+217+35*k + AddVert, put_item.item_no,15,1);
						}
						else
						{
							::PutItemIcon(SubMenu->x+13+22+35*i  + AddHorz, SubMenu->y+217+35*k + AddVert, put_item.item_no);
						}
					}
					else
					{
						::PutItemIcon(SubMenu->x+13+22+35*i  + AddHorz, SubMenu->y+217+35*k + AddVert, put_item.item_no);
					}

				}
				else //�̰͵� ���͵� �ƴ� ���¶��
				{
					::PutItemIcon(SubMenu->x+13+22+35*i  + AddHorz, SubMenu->y+217+35*k + AddVert, put_item.item_no);
					CraftItem = CheckAndCraftVertical(first_para, -1);
					/*if (CraftItem >= 0) {
						SDL_Log("CraftItem =%d", CraftItem);
					}*/
				}
			}
		}
	}
	
	if(	(SubMenu->nField[j].fRectMouse || SubMenu->nField[j].fRButtonDown)
	&& !g_MouseItemType )
	{	//[3][3][8] �̱� ������ �̷��� �˴ϴ� y��ǥ�� ���
		const int ItemX = ( g_pointMouseY - (SubMenu->y + SubMenu->nField[j].y) ) / 35;
		const int ItemY = ( g_pointMouseX - (SubMenu->x + SubMenu->nField[j].x) ) / 35;
		if( ItemY >= 8 || ItemY < 0 || ItemX >= 3 || ItemX < 0 ) 
		{
			return;
		}
																			
		const int xCheck = ( g_pointMouseX - (SubMenu->x + SubMenu->nField[j].x) ) % 35;
		const int yCheck = ( g_pointMouseY - (SubMenu->y + SubMenu->nField[j].y) ) % 35;
		if( (xCheck < 2) || (xCheck > 30) || (yCheck < 2) || (yCheck > 30 ) )  
		{
			return;
		}

		const ItemAttr &item= InvItemAttr[first_para][ItemX][ItemY];
		if( LButtonDown || !item.item_no ) 
		{
			return;
		}

		if( SMenu[MN_GAMBLE].bActive )				// ������ ����ũ â�� ���� �ö� ������ ���õ��� �ʴ´�.
		{
			for( i=0; i<20; i++ )
			{
				const POS &pos = make_item_list[i].pos ;
				const ItemAttr &Item = make_item_list[i].attr;
				if( Item.item_no 
				&& pos.type == INV
				&& pos.p1 == first_para
				&& pos.p2 == ItemX 
				&& pos.p3 == ItemY )
				{
					ItemExplain(item);
					break;
				}
			}
			if( i!=20 ) 
			{
				return;
			}
		}
		else if( IsExchangeNow() )		// ������ ��ȯâ�� �÷��� ������
		{
			for( i=0; i<MAX_EXCHANGE_ITEM; i++ )
			{
				const POS &pos = item_give[i].item.item_pos ;
				const ItemAttr &Item = item_give[i].item.item_attr;
				if( Item.item_no 
				&& pos.type == INV
				&& pos.p1 == first_para
				&& pos.p2 == ItemX 
				&& pos.p3 == ItemY )
				{
					ItemExplain(item);
					break;
				}
			}
			if( i!=20 )
			{
				return;
			}
		}

		if( SMenu[MN_GM_2ND_SKILL].bActive )				// ������ ����ũ â�� ���� �ö� ������ ���õ��� �ʴ´�.
		{
			for( i=0; i<MAX_UPGRADE_RARE; i++ )
			{
				const POS &pos = gm_make_item_list[i].pos;
				const ItemAttr &Item = gm_make_item_list[i].attr;
				if( Item.item_no 
				&& pos.type == INV
				&& pos.p1 == first_para
				&& pos.p2 == ItemX 
				&& pos.p3 == ItemY ) 
				{
					return;
				}
			}
			if( i!=20 ) 
			{
				return;
			}
		}
		if( SMenu[MN_SKILL_MASTER_MAKE].bActive )
		{
			int a;
			for( a=0; a<2; a++ )
			{
				const POS &pos = g_MultiRareSrc.MultiRareSrc[a].pos;
				const ItemAttr &Item = g_MultiRareSrc.MultiRareSrc[a].attr;
				if( Item.item_no 
				&& pos.type == INV
				&& pos.p1 == first_para
				&& pos.p2 == ItemX 
				&& pos.p3 == ItemY ) 
				{
					return;
				}
			}
			if( a!=2 ) 
			{
				return;
			}
		}
		if(SMenu[MN_MERCHANT_REGISTER].bActive)//021014 lsw
		{
			const POS pos = Auction.GetSellItemPos();//�Ȱڴٰ� ���Ѱ� ��Ӱ� ����
			if(pos.type == INV
			&& pos.p1 ==first_para
			&& pos.p2 == ItemX
			&& pos.p3 ==ItemY)
			{//�ƿ� ������ �ʴ´�.. ������ �Ѥ�;..
				return;
			}
		}
		// 040105 YGI
		CItem *t = ItemUnit( item );
		if( t && t->GetItemKind() == IK_TREASURE_MAP )
		{
			CTreasureMapItem *attr = (CTreasureMapItem *)&item.attr[IATTR_TREASURE_MAP];
			POS pos;
			SetItemPos( INV, first_para, ItemX, ItemY, &pos );
			SendGetTreasureMapDetail( &pos );
		}
		//////////////////////////////
		::ItemExplain(item);																		// ������ ����
		::PutItemIcon(SubMenu->x+13+22+35*ItemY + AddHorz, SubMenu->y+217+35*ItemX + AddVert, item.item_no, 8);	// ��� ǥ��

		int type= item.item_no/1000;
		if( type == POTION )				// ������ ��� ���ϸ��̼�
		{
			::PutItemIconAni( SubMenu->x+13+22+35*ItemY + AddHorz, SubMenu->y+217+35*ItemX + AddVert, item.item_no );
		}
	}
	return;
}

void DisplayQuickMenu(SMENU *SubMenu ,int j)
{
	const int gabx = SubMenu->nField[j].x;
	const int gaby = SubMenu->nField[j].y;
	int i=0, k=0 ;
	for(k=0;k<5;k++) //���� �κ��丮â�� ���� �ִ� ũ��� 5 �� �Ǿ����ϴ�.
	{
		if(QuickItemAttr[k].item_no )
		{
			if( IsExchangeNow() )
			{
				int put = 1;
				for( int a=0; a<MAX_EXCHANGE_ITEM; a++ )
				{
					if(item_give[a].item.item_attr.item_no && item_give[a].item.item_pos.type==QUICK && item_give[a].item.item_pos.p3==k )
					{
						PutItemIcon(SubMenu->x+15+gabx +33*k , SubMenu->y+16+gaby, QuickItemAttr[k].item_no, 15, 1 );
						put=0;
						break;
					}
				}
				if( put ) 
				{
					PutItemIcon(SubMenu->x+15+gabx +33*k , SubMenu->y+16+gaby, QuickItemAttr[k].item_no);
				}
			}
			else 
			{
				PutItemIcon(SubMenu->x+15+gabx +33*k , SubMenu->y+16+gaby, QuickItemAttr[k].item_no);
			}
		}										
	}	//for�� ��
	
	const int ItemX = ( g_pointMouseX - (SubMenu->x + SubMenu->nField[j].x) ) / 33;
	if( 0 > ItemX || 5 <= ItemX)//5���� ���÷���(���콺 ���� üũ
	{
		return;
	}
	if( !QuickItemAttr[ItemX].item_no ) 
	{
		return;
	}

	if( SMenu[MN_GAMBLE].bActive )				// ������ ����ũ â�� ���� �ö� ������ ���õ��� �ʴ´�.
	{
		for( i=0; i<20; i++ )
		{
			if( make_item_list[i].item_no && make_item_list[i].pos.type == QUICK && make_item_list[i].pos.p3 == ItemX )
			{
				ItemExplain( QuickItemAttr[ItemX] );
				break;
			}
		}
		if( i!=20 ) 
		{
			return;
		}
	}
	else if( IsExchangeNow() )		// ������ ��ȯâ�� �÷��� ������
	{
		for( i=0; i<MAX_EXCHANGE_ITEM; i++ )
		{
			if( item_give[i].item.item_attr.item_no && item_give[i].item.item_pos.type == QUICK && item_give[i].item.item_pos.p3 == ItemX ) 
			{
				ItemExplain( QuickItemAttr[ItemX] );
				break;
			}
		}
		if( i!=MAX_EXCHANGE_ITEM ) 
		{
			return;
		}
	}
	else if( SMenu[MN_GM_2ND_SKILL].bActive )				// ������ ����ũ â�� ���� �ö� ������ ���õ��� �ʴ´�.
	{
		for( i=0; i<MAX_UPGRADE_RARE; i++ )
		{
			if( gm_make_item_list[i].item_no && gm_make_item_list[i].pos.type == QUICK && gm_make_item_list[i].pos.p3 == ItemX )
			{
				ItemExplain( QuickItemAttr[ItemX] );
				break;
			}
		}
		if( i!=20 )
		{
			return;
		}
	}

	if( !g_MouseItemType && (SubMenu->nField[j].fRectMouse || SubMenu->nField[j].fRButtonDown) )
	{
//										if( !bSoundOn ) { MP3( SN_TOUCH ); bSoundOn = true; }

		if( !LButtonDown ) ItemExplain(QuickItemAttr[ItemX]);
		PutItemIcon(SubMenu->x+15+gabx +33*ItemX , SubMenu->y+16+gaby, QuickItemAttr[ItemX].item_no, 8);

		int type= QuickItemAttr[ItemX].item_no/1000;
		if( type == POTION )				// ������ ��� ���ϸ��̼�
		{
			PutItemIconAni( SubMenu->x+15+gabx +33*ItemX , SubMenu->y+16+gaby, QuickItemAttr[ItemX].item_no);
		}
	}
	return;
}

void DisplayCandleMenu(SMENU *SubMenu ,int j)
{
	const int gabx = SubMenu->nField[j].x;
	const int gaby = SubMenu->nField[j].y;
	int i=0, k=0 ;
	if(QuickItemAttr[5].item_no )
	{
		if( IsExchangeNow() )
		{
			int put = 1;
			for( int a=0; a<MAX_EXCHANGE_ITEM; a++ )
			{
				if(item_give[a].item.item_attr.item_no && item_give[a].item.item_pos.type==QUICK && item_give[a].item.item_pos.p3==k )
				{
					PutItemIcon(SubMenu->x+15+gabx , SubMenu->y+16+gaby, QuickItemAttr[k].item_no, 15, 1 );
					put=0;
					break;
				}
			}
			if( put ) PutItemIcon(SubMenu->x+15+gabx  , SubMenu->y+16+gaby, QuickItemAttr[k].item_no);
		}
		else 
		{
			PutItemIcon(SubMenu->x+15+gabx  , SubMenu->y+16+gaby, QuickItemAttr[k].item_no);
		}
	}										
	
	const int ItemX = 5;
	
	if( !QuickItemAttr[ItemX].item_no ) 
	{
		return;
	}

	if( SMenu[MN_GAMBLE].bActive )				// ������ ����ũ â�� ���� �ö� ������ ���õ��� �ʴ´�.
	{
		for( i=0; i<20; i++ )
		{
			if( make_item_list[i].item_no && make_item_list[i].pos.type == QUICK && make_item_list[i].pos.p3 == ItemX )
			{
				ItemExplain( QuickItemAttr[ItemX] );
				break;
			}
		}
		if( i!=20 ) 
		{
			return;
		}
	}
	else if( IsExchangeNow() )		// ������ ��ȯâ�� �÷��� ������
	{
		for( i=0; i<MAX_EXCHANGE_ITEM; i++ )
		{
			if( item_give[i].item.item_attr.item_no && item_give[i].item.item_pos.type == QUICK && item_give[i].item.item_pos.p3 == ItemX ) 
			{
				ItemExplain( QuickItemAttr[ItemX] );
				break;
			}
		}
		if( i!=MAX_EXCHANGE_ITEM ) 
		{
			return;
		}
	}
	else if( SMenu[MN_GM_2ND_SKILL].bActive )				// ������ ����ũ â�� ���� �ö� ������ ���õ��� �ʴ´�.
	{
		for( i=0; i<MAX_UPGRADE_RARE; i++ )
		{
			if( gm_make_item_list[i].item_no && gm_make_item_list[i].pos.type == QUICK && gm_make_item_list[i].pos.p3 == ItemX )
			{
				ItemExplain( QuickItemAttr[ItemX] );
				break;
			}
		}
		if( i!=20 )
		{
			return;
		}
	}

	if( !g_MouseItemType && (SubMenu->nField[j].fRectMouse || SubMenu->nField[j].fRButtonDown) )
	{
//										if( !bSoundOn ) { MP3( SN_TOUCH ); bSoundOn = true; }

		if( !LButtonDown ) ItemExplain(QuickItemAttr[ItemX]);
		PutItemIcon(SubMenu->x+15+gabx +33*ItemX , SubMenu->y+16+gaby, QuickItemAttr[ItemX].item_no, 8);

		int type= QuickItemAttr[ItemX].item_no/1000;
		if( type == POTION )				// ������ ��� ���ϸ��̼�
		{
			PutItemIconAni( SubMenu->x+15+gabx +33*ItemX , SubMenu->y+16+gaby, QuickItemAttr[ItemX].item_no);
		}
	}
	return;
}

void DisplayTacGauge(SMENU *SubMenu, int j, const int x, const int y, const int iMax, const int iNow, const int iLv,int refresh,int first_exp)
{
	int &old_exp = SubMenu->nField[j].nSHideNomalCount;
	int &old_exp2 = SubMenu->nField[j].nSHideNomalStart;
	int &plus = SubMenu->nField[j].nShideNomalPlus;
//	int &old_exp_for_100 = SubMenu->nField[j].nWillDo;		// 100 �� �̻��� ����ġ �����ֱ⸦ ���Ͽ�
//	int &point_count = SubMenu->nField[j].nSpecialWillDo;
	
	if( refresh || !SubMenu->CheakType )
	{
		SubMenu->CheakType = 1;
		old_exp = iNow;
		//old_exp_for_100 = old_exp = exp;
		old_exp2 = iNow;
		plus = 0;
		//point_count = 0;
	}
	if( old_exp < iNow )
	{
		bool refresh_plus = false;
		if( old_exp2 < iNow ) refresh_plus = true;
		old_exp2 = iNow;

		if( !plus || refresh_plus )
		{
			plus = (int )((iNow - old_exp)/30.0f);
			if( !plus ) plus = 1;
		}											
		old_exp += plus;
		if( old_exp > iNow ) old_exp = iNow;
	}
	else
	{
		old_exp = iNow;
		plus = 0;
	}

	if( 0 < iMax-first_exp  )
	{
		int exp_rate = (int)( (double)(old_exp-first_exp)*118.00 / (double)(iMax-first_exp) );		// 118�� ����� ������ �� ����
		if( exp_rate >= 0 )
		{
		//	for( int i=0; i< 118 - exp_rate; i++ )
			for( int i=0; i< exp_rate; i++ )
			{
				FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x+i, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType );
			}
		}
	}

	int add_point = 0;
	if( SubMenu->nField[j].nType != FT_DISPLAY_SKILL_EXP ) 
	{//��ų�� �ƴϸ� 1 ���ؼ� ���÷���
		add_point = 1;
	}
	Hcolor( 190, 250, 190 );
	SetHangulAlign(TA_RIGHT);
	Hprint2( SubMenu->x+SubMenu->nField[j].x+88+30, SubMenu->y+SubMenu->nField[j].y+2, g_DestBackBuf,"%3d",iLv+add_point);
	return;//����ġ ��ü ���� �����
}

void DisplayEmote(const int x,const int y,const int iSmileNo)//020515 lsw
{
	const int ImageNo = iSmileNo+1;
	if(34 < ImageNo ||1 > ImageNo) //Eleval 22/06/09 - Put 34 for 5 new smileys
	{
		return;
	}
	FieldTypeNomalPut(x, y, 0, 0, ImageNo , NEW_EMOTE_IMG); // 031021 kyo
}

static bool bOn = false;

void CallWisperList(const bool bOpen)
{//�Ѱ� ������ ���� ó�� �Ұ� ������ ����� �Ұ�
	SMenu[MN_WISPER_LIST].bActive = bOpen;
}

void ClearQuickMemory()
{
	memset( SaveMemory, 0, sizeof(QUICKMEMORY) * MAX_MAGIC_MEMORY );
}

void CheckDuplicateQuickMemory(const int iType, const int iNo, const int iBuffer)
{
	for(int i = 0; i<MAX_MAGIC_MEMORY;i++)
	{
		if((iType==SaveMemory[i].iType ) &&	(iNo == SaveMemory[i].iNo))
		{
			SaveMemory[i].iType = 0;
			SaveMemory[i].iNo = 0;//�ߺ� �����
		}
	}
}

void SetQuickMemoryByKeyInput(const int iType, const int iNo, const int iBuffer)
{
	if(!isKeyPressed(SDL_SCANCODE_LCTRL))
	{
		return;//��Ʈ��Ű �ȴ����� �ȵ�
	}	

	for (int i = SDL_SCANCODE_F1; i <= SDL_SCANCODE_F8; i++)
	{
		SDL_Scancode key = static_cast<SDL_Scancode>(i);
		if (isKeyPressed(key)) {
			const int index = key - SDL_SCANCODE_F1;
			CheckDuplicateQuickMemory(iType, iNo, 0);
			SaveMemory[index].SetQuickMemory(Hero->name, iType, iNo);
		}
	}

	for (int i = SDL_SCANCODE_6; i <= SDL_SCANCODE_0; i++)
	{
		SDL_Scancode key = static_cast<SDL_Scancode>(i);
		if (isKeyPressed(key)) {
			const int index = key - SDL_SCANCODE_6 + extra_quickmemory_start_index;
			CheckDuplicateQuickMemory(iType, iNo, 0);
			SaveMemory[index].SetQuickMemory(Hero->name, iType, iNo);
		}
	}
	/*if (isKeyPressed(SDL_SCANCODE_F1)) { CheckDuplicateQuickMemory(iType, iNo, 0); SaveMemory[0].SetQuickMemory(Hero->name, iType, iNo); }
	if(isKeyPressed(SDL_SCANCODE_F2))	{CheckDuplicateQuickMemory(iType,iNo,0);SaveMemory[1].SetQuickMemory(Hero->name,iType,iNo);}
	if(isKeyPressed(SDL_SCANCODE_F3))	{CheckDuplicateQuickMemory(iType,iNo,0);SaveMemory[2].SetQuickMemory(Hero->name,iType,iNo);}
	if(isKeyPressed(SDL_SCANCODE_F4))	{CheckDuplicateQuickMemory(iType,iNo,0);SaveMemory[3].SetQuickMemory(Hero->name,iType,iNo);}
	if(isKeyPressed(SDL_SCANCODE_F5))	{CheckDuplicateQuickMemory(iType,iNo,0);SaveMemory[4].SetQuickMemory(Hero->name,iType,iNo);}
	if(isKeyPressed(SDL_SCANCODE_F6))	{CheckDuplicateQuickMemory(iType,iNo,0);SaveMemory[5].SetQuickMemory(Hero->name,iType,iNo);}
	if(isKeyPressed(SDL_SCANCODE_F7))	{CheckDuplicateQuickMemory(iType,iNo,0);SaveMemory[6].SetQuickMemory(Hero->name,iType,iNo);}
	if(isKeyPressed(SDL_SCANCODE_F8))	{CheckDuplicateQuickMemory(iType,iNo,0);SaveMemory[7].SetQuickMemory(Hero->name,iType,iNo);}
	*/
	SaveMemorize();
}

void DisplayQuickMemory(const int iType, const int iTargetNo, const int iX, const int iY)
{
	for(int i = 0;i < MAX_MAGIC_MEMORY; i++)
	{
		if( iTargetNo == SaveMemory[i].iNo && iType == SaveMemory[i].iType)//���� �� �ִ� ��Ȳ
		{
			
			if(!strcmp(SaveMemory[i].OwnerName,Hero->name))//�̸��� ���ƾ� ����.
			{
				if (i <= 7)
					HprintBold(iX, iY, FONT_COLOR_SOLID_YELLOW, FONT_COLOR_BLACK, "F%d", i + 1);
				else if(i==12)
					HprintBold(iX, iY, FONT_COLOR_SOLID_YELLOW, FONT_COLOR_BLACK, "0");
				else HprintBold(iX, iY, FONT_COLOR_SOLID_YELLOW, FONT_COLOR_BLACK, "%d", i - 2);
			}
		}
	}	
	return;
}

void DoQuickmemory(const int iQMNo)
{
	if( isKeyDown(SDL_SCANCODE_LCTRL) || isKeyDown(SDL_SCANCODE_RCTRL))
	{
		return;//030611 lsw ��Ʈ��Ű ������ ���޸� ���� �ȵ�
	}

	if( 0 > iQMNo || MAX_MAGIC_MEMORY <= iQMNo)
	{
		return;
	}

	const int iNo = SaveMemory[iQMNo].iNo;
	const int iType = SaveMemory[iQMNo].iType;
	switch(iType)
	{
	case QM_MAGIC:
		{
			const int nMagic = g_mgrBattle.Convert(iNo);
			if (GetMagic()->IsEnable(nMagic))
			{	//< CSD-TW-030606
				g_mgrBattle.SendMagicSelect(iNo);
				RectImageOn = true;
				MagicOnFlag.SetMagicOn(iNo);
			}	//> CSD-TW-030606
		}break;
	case QM_SKILL:
		{
//			if(SKILLTARGETTYPE_2ND_GM  == SkillTable[ iNo ].Howto)//soto-030903 �ϴ� GM ��ų�� ����.
//				break;

			int para = 1;			
			if( skill[iNo].series==1 ) 
			{
				para = 0;	
			}
			if(SelectSkill( iNo )==1)
			{
				break;
			}
			HotKeyMemory.SetQuickMemory(Hero->name,iType,iNo);//020701 lsw

			SkillOn = 0;
			CanMakeItemListSet( iNo );		// ����� �ִ� ������ ����
		}break;
	case QM_COMBATSKILL:
		{
			const int nNo = CombatSkill[iNo].iSkillNo+100;

			if (GetCombat()->IsEnable(nNo))
			{
				g_mgrBattle.SendCombatSelect(nNo);
				RectImageOn = true;
				MagicOnFlag.SetMagicOn(nNo);
			}

      break;
		}
	default:
		{
		}break;
	}
	return;	
}

void DoQuickmemoryByKeyInput(SDL_Scancode iInputKey,bool bIsDikCheck)
{
	if(Hero)
	{
		switch (Hero->viewtype)
		{	//< CSD-030515
		case VIEWTYPE_GHOST_:
		case VIEWTYPE_OBSERVE_:
			{
				return;
			}
		}	//> CSD-030515
	}

	if (isKeyDown(iInputKey)) {
		if(iInputKey>= SDL_SCANCODE_F1 && iInputKey <= SDL_SCANCODE_F8)
			DoQuickmemory(iInputKey - SDL_SCANCODE_F1);
		else
		{
			if (iInputKey >= SDL_SCANCODE_6 && iInputKey <= SDL_SCANCODE_0)
				DoQuickmemory(iInputKey - SDL_SCANCODE_6 + extra_quickmemory_start_index);
		}
	}
	/*if (isKeyDown(SDL_SCANCODE_F2)) { DoQuickmemory(1); }
	if (isKeyDown(SDL_SCANCODE_F3)) { DoQuickmemory(2); }
	if (isKeyDown(SDL_SCANCODE_F4)) { DoQuickmemory(3); }
	if (isKeyDown(SDL_SCANCODE_F5)) { DoQuickmemory(4); }
	if (isKeyDown(SDL_SCANCODE_F6)) { DoQuickmemory(5); }
	if (isKeyDown(SDL_SCANCODE_F7)) { DoQuickmemory(6); }
	if (isKeyDown(SDL_SCANCODE_F8)) { DoQuickmemory(7); }*/
}

int CheckLegendSetComplete(const int iLegendGroupNo, const int iLegendGroupIndex)//�Էµ� ������ ��Ʈ�� �� �𿴴°�
{
	int aLegendGNo[ITEM_EQUIP_MAX] = {0,};
	int aLegendGIndex[ITEM_EQUIP_MAX] = {0,};
	int i;
	for(i = 0; i <ITEM_EQUIP_MAX; i++)
	{
		const LPRareMain pRare = (LPRareMain)&EquipItemAttr[i].attr[IATTR_RARE_MAIN];
		if( H_LV_LEGEND_ITEM != pRare->iHighLevel){continue;}//�����尡 �ƴ� ���
		aLegendGNo[i]	= pRare->soksung2;
		aLegendGIndex[i]= pRare->soksung3;
	}

	for(i = 0; i <ITEM_EQUIP_MAX; i++)//üũ�ؼ� ���� �ִ°� ������ ������ �̶��
	{
		const int iGNo		= aLegendGNo[i];
		const int iGIndex	= aLegendGIndex[i];
		if(!iGNo || !iGIndex){continue;}
		const int iNeeds = g_aItemLegend[iGNo].aSub[iGIndex].iNeeds;
		int iLegendResult = 0;
		for(int aa = 0; aa <ITEM_EQUIP_MAX; aa++)//	needs üũ�� �ٽ� 8���� �˻��ؼ� ��Ʈ�� �Ǿ����� ����
		{
			if( aLegendGNo[aa]		== iGNo	//�� ���Ⱑ iGNo�� ����
			&&	aLegendGIndex[aa]	== iGIndex)//�ε����� ��ġ �Ѵٸ�
			{
				int iCmpKey = 0;
				switch(aa)
				{
				case WT_WEAPON:	{iCmpKey = LEGEND_NEED_WEAPON;}break;
				case WT_SHIELD:	{iCmpKey = LEGEND_NEED_SHIELD;}break;
				case WT_HELMET:	{iCmpKey = LEGEND_NEED_HELMET;}break;
				case WT_ARMOR:	{iCmpKey = LEGEND_NEED_ARMOR;}break;
				case WT_NECK:	{iCmpKey = LEGEND_NEED_NECK;}break;
				case WT_SHOES:	{iCmpKey = LEGEND_NEED_SHOES;}break;
				case WT_UNIQUE1:{iCmpKey = LEGEND_NEED_UNIQUE;}break;
				case WT_UNIQUE2:{iCmpKey = LEGEND_NEED_UNIQUE;}break;
				}
				if( iNeeds & iCmpKey)//���� �ϵ尡 aa�� �䱸 �ϸ�
				{
					iLegendResult |= iCmpKey;//�����忡 ������ �ߴ�
					aLegendGNo[aa] = 0;//�ߺ� üũ�� �ȵǰ� �ϱ� ���ؼ� �����
					aLegendGIndex[aa] = 0;//�ߺ� üũ�� �ȵǰ� �ϱ� ���ؼ� �����
				}
			}
		}//for(int aa = 0; aa <ITEM_EQUIP_MAX; aa++)
		if(iNeeds == iLegendResult)
		{//�� á��.. ��.
			if(	iLegendGroupNo == iGNo 
			&&	iLegendGroupIndex == iGIndex)
			{//���� ã�� ��Ʈ��
				return 1;//��Ʈ �� ��Ҿ�
			}
		}//(iNeeds == iLegendResult)
	}//for(i = 0; i <ITEM_EQUIP_MAX; i++)//üũ�ؼ� ���� �ִ°� ������ ������ �̶��
	return 0;
}

void DisplayRareExplainSub(const int x,const int y,int &iCount , int &iReultLineCount, int &iBlk,const int iLGab, const int  iWidth,const RareMain *pRareValue,const int ct,const WORD wForceColor)
{
	int iIndex = 0;
	switch(ct)
	{
	case 0:{iIndex = pRareValue->soksung1;}break;
	case 1:{iIndex = pRareValue->soksung2;}break;
	case 2:{iIndex = pRareValue->soksung3;}break;
	default:{return;}break;
	}
	if(!iIndex){return;}

	if(wForceColor)	{::Hcolor(wForceColor);}
	else{::Hcolor(FONT_COLOR_RARE_DISPLAY);}

	if(pRareValue->IsDynamicRare)
	{
		t_ItemFunction *pIF = &ItemFunction[iIndex];
		switch(pIF->iExpMark%10)
		{
		case 0:
			{
				iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCount,iWidth,5,"%s  ",pIF->Exp);//�ڿ� ���� ��
			}break;
		case 1:
			{
				iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCount,iWidth,5,"%s   %d",pIF->Exp, pIF->GRADE[pRareValue->grade]);//�ڿ� ���� ��
			}break;
		case 2:
			{
				iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCount,iWidth,5,"%s   %d%%",pIF->Exp, pIF->GRADE[pRareValue->grade]);//�ڿ� ���� ��
			}break;
		case 3:
			{
				iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCount,iWidth,5,"%s   %dSec",pIF->Exp, pIF->GRADE[pRareValue->grade]);//�ڿ� ���� ��
			}break;
		}
		switch(ItemFunction[iIndex].iExpMark/10)
		{
		case 1://1�̸� +
			{
				::Hcolor(FONT_COLOR_RARE_PLUS_POINT);//��ȣ��	
				::Hprint2(x+5,y+iBlk+iLGab*iCount,g_DestBackBuf,"%s +",pIF->Exp);// 1 : name
			}break;
		case 2://2�̸� -
			{
				::Hcolor(FONT_COLOR_RARE_MINUS_POINT);//��ȣ��	
				::Hprint2(x+5,y+iBlk+iLGab*iCount,g_DestBackBuf,"%s -",pIF->Exp);// 1 : name
			}break;
		}
		
		if(wForceColor)	{::Hcolor(wForceColor);}
		else{::Hcolor(FONT_COLOR_RARE_DISPLAY);}
		
		iCount += iReultLineCount;//020801 lsw
		iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCount,iWidth,5,lan->OutputMessage(0,40),pIF->iTime[pRareValue->grade]);//�����
		iCount += iReultLineCount;
	}//if(pRareValue->IsDynamicRare)
	else
	{
		t_ItemRare *pIR = &ItemRare[iIndex];
		switch(pIR->iExpMark%10)
		{
		case 0:
			{
				iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCount,iWidth,5,"%s  ",pIR->ExplainAdd);//�ڿ� ���� ��
			}break;
		case 1:
			{
				iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCount,iWidth,5,"%s   %d",pIR->ExplainAdd, pIR->GRADE[pRareValue->grade]);//�ڿ� ���� ��
			}break;
		case 2:
			{
				iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCount,iWidth,5,"%s   %d%%",pIR->ExplainAdd, pIR->GRADE[pRareValue->grade]);//�ڿ� ���� ��
			}break;
		case 3:
			{
				iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCount,iWidth,5,"%s   %dSec",pIR->ExplainAdd, pIR->GRADE[pRareValue->grade]);//�ڿ� ���� ��
			}break;
		}
		switch(pIR->iExpMark/10)
		{
		case 1://1�̸� +
			{
				::Hcolor(FONT_COLOR_RARE_PLUS_POINT);//��ȣ��	
				::Hprint2(x+5,y+iBlk+iLGab*iCount,g_DestBackBuf,"%s +",pIR->ExplainAdd);// 1 : name
			}break;
		case 2://2�̸� -
			{
				::Hcolor(FONT_COLOR_RARE_MINUS_POINT);//��ȣ��	
				::Hprint2(x+5,y+iBlk+iLGab*iCount,g_DestBackBuf,"%s -",pIR->ExplainAdd);// 1 : name
			}break;
		}

		switch(pRareValue->iHighLevel)//���� ���� ��
		{
		case H_LV_NO_HIGH_ITEM:
			{
				::Hcolor(FONT_COLOR_RARE_DISPLAY);
			}break;
		case H_LV_HIGH_ITEM:
			{
				::Hcolor(FONT_COLOR_HIGH_RARE_DISPLAY);
			}break;
		case H_LV_LEGEND_ITEM:
			{
				if(wForceColor)	{::Hcolor(wForceColor);}
				else{::Hcolor(FONT_COLOR_RARE_DISPLAY);}
			}break;
		default:
			{
			}break;
		}
		::Hprint2(x+5,y+iBlk+iLGab*iCount,g_DestBackBuf,"%s", pIR->ExplainAdd);//�����
		iCount += iReultLineCount;
	}
}

void DisplayRareExplain(const int x,const int y,int &iCount , int &iReultLineCount, int &iBlk,const int iLGab, const int  iWidth,const RareMain *pRareValue)
{
	switch(pRareValue->iHighLevel)
	{
	case H_LV_NO_HIGH_ITEM:
		{
			::Hcolor(FONT_COLOR_RARE_MAIN);
			iReultLineCount = TxtOut.RcTXTOutB(x,y+iBlk+iLGab*iCount,iWidth,0,lan->OutputMessage(2,790));// 1 : name
			iCount += iReultLineCount;
			for(int ct = 0; 3 > ct; ct++)
			{
				::DisplayRareExplainSub(x,y,iCount, iReultLineCount,iBlk,iLGab,iWidth,pRareValue,ct);
			}
		}break;
	case H_LV_HIGH_ITEM:
		{
			Hcolor( FONT_COLOR_HIGH_RARE_MAIN );
			iReultLineCount = TxtOut.RcTXTOutB(x,y+iBlk+iLGab*iCount,iWidth,0,lan->OutputMessage(2,791));// 1 : name
			iCount += iReultLineCount;
			::DisplayRareExplainSub(x,y,iCount, iReultLineCount,iBlk,iLGab,iWidth,pRareValue,0);//1���� ǥ����
		}break;
	case H_LV_LEGEND_ITEM:
		{
			::Hcolor( FONT_COLOR_HIGH_RARE_MAIN );
			iReultLineCount = TxtOut.RcTXTOutB(x,y+iBlk+iLGab*iCount,iWidth,0,lan->OutputMessage(2,792));// 1 : name
			iCount += iReultLineCount;
			::DisplayRareExplainSub(x,y,iCount, iReultLineCount,iBlk,iLGab,iWidth,pRareValue,0);//�⺻ �ϳ���
			
			::Hcolor( FONT_COLOR_HIGH_RARE_MAIN );
			iReultLineCount = TxtOut.RcTXTOutB(x,y+iBlk+iLGab*iCount,iWidth,0,lan->OutputMessage(2,793));// 1 : name
			iCount += iReultLineCount;

			WORD wForceColor =0;

			if(CheckLegendSetComplete(pRareValue->soksung2, pRareValue->soksung3))//�Էµ� ������ ��Ʈ�� �� �𿴴°�
			{
				wForceColor =0;
			}
			else
			{
				wForceColor =FONT_COLOR_RED;
			}
			
			for(int xx = 0;MAX_ITEM_LEGEND_SUB_BONUS_MAX>xx;xx++)
			{
				for(int yy = 0;MAX_ITEM_LEGEND_BONUS_KIND>yy;yy++)
				{
					RareMain TempRare = {0,};
					TempRare.grade		= g_aLegendBonus[g_aItemLegend[pRareValue->soksung2].aSub[pRareValue->soksung3].aiBonus[xx]].aiGrade[yy];
					TempRare.soksung1	= g_aLegendBonus[g_aItemLegend[pRareValue->soksung2].aSub[pRareValue->soksung3].aiBonus[xx]].aiKind[yy];
					TempRare.iHighLevel = pRareValue->iHighLevel;
					
					::DisplayRareExplainSub(x,y,iCount, iReultLineCount,iBlk,iLGab,iWidth,&TempRare,0,wForceColor);
				}
			}
		}break;
	default:
		{
			return;
		}break;
	}
}

void CallLoginMenu(const bool bForceLoginActive)
{//021021 lsw
	::Release( &connections );	// �ʱ�ȭ
	::Initialize();
	//::Sleep(1000);
#ifdef _DEBUG
	#ifdef KOREA_LOCALIZING_
	if(!bForceLoginActive)//true�� ������ ��������� �Ȱ��� ����
	{
		// ini�� üũ�ؼ� ���� ������ 
		const int count = CheckSelectServer();
		//if( GetSysInfo( SI_GAME_MAKE_MODE ) ) count = 0;
		
		if( count )
		{	// ���� ���� �޴��� ����.
			SMenu[MN_SELECT_SERVER].bActive = true;
			SMenu[MN_SELECT_SERVER].work = count;
			SMenu[MN_SELECT_SERVER].nFieldCount = 3+count;
			if( SMenu[MN_SELECT_SERVER].nFieldCount > 8 ) 
			{
				SMenu[MN_SELECT_SERVER].nFieldCount = 8;
			}
			for( int i=0; i<count && i<5; i++ )
			{
				SMenu[MN_SELECT_SERVER].nField[3+i].nType = FT_SELECT_SERVER_BUTTON;
			}
		}
	return;
	}
#endif
#endif

	sId = "";
	sPassword = "";
	//EWndMgr.ClearAllTxt();//021001 lsw
	SMenu[MN_LOGIN].bActive=true;
	EWndMgr.showInputBox(HWND_2);
	EWndMgr.showInputBox(HWND_1);
	SMenu[MN_SELECT_CHARACTER].bActive=false;
	//id_password = true;
}

void SWD_LOGON_Clicked(const int nMenuIndex)
{ 
	while (g_GameInfo.AutoLogin < 100) {
		g_GameInfo.AutoLogin++;
		SDL_Delay(60);
	}

	g_bLogOnFlag = false;//010909 lsw �������� �α׿� �÷��׸� �׿� �ش�
	int ret =0;
	sId = EWndMgr.GetTxt(HWND_1);
	sPassword = EWndMgr.GetTxt(HWND_2);
	if (sId.empty() || sPassword.empty()) return;

	/*ret = ConnectProxyServer(&connections);
	if (ret != 1) {
		CallOkCancelMessageBox(nMenuIndex, 0, 0, lan->OutputMessage(5, 71));
		return;
	}*/

	ret = CheckAgentServer( &connections, sId, sPassword );
	
	if( ret ==1 )
	{
#ifdef _DEBUG // finito 060507
		if( GetSysInfo( SI_NOT_CONNECT_MODE ) )
		{
			SMenu[MN_SELECT_CHARACTER].bActive=true;
			SMenu[MN_LOGIN].bActive=false;//020515 lsw
			SCharSource = LoadCh[character_active];
		}
#endif
	}
	else 
	{
		switch( ret )
		{
			case -1: ::CallOkCancelMessageBox( nMenuIndex, 0,0, lan->OutputMessage(5,71),  0 );	break;//lsw
			case -3: ::CallOkCancelMessageBox( nMenuIndex, 0,0, lan->OutputMessage(5,75),  0 );	break;
			case -4: ::CallOkCancelMessageBox( nMenuIndex, 0,0, lan->OutputMessage(5,76),  0 );	break;//lsw
			case -5: ::CallOkCancelMessageBox( nMenuIndex, 0,0, lan->OutputMessage(5,77),  0 );	break;//lsw

			case -6: ::CallOkCancelMessageBox( nMenuIndex, 0,0, lan->OutputMessage(5,78),  0 );	break;//lsw
			case -7: ::CallOkCancelMessageBox( nMenuIndex, 0,0, lan->OutputMessage(5,79),  0 );	break;
			case -8: ::CallOkCancelMessageBox( nMenuIndex, 0,0, lan->OutputMessage(5,80),  0 );	break;//lsw
			case -9: ::CallOkCancelMessageBox( nMenuIndex, 0,0, lan->OutputMessage(5,84),  0 );	break;
			case -18: ::CallOkCancelMessageBox(nMenuIndex, 0, 0, lan->OutputMessage(5, 101), 0);	break;
			case -10: ::CallOkCancelMessageBox( nMenuIndex, 0,0, lan->OutputMessage(5,85), 0 );	break;//lsw
			case -11: ::CallOkCancelMessageBox( nMenuIndex, 0,0, lan->OutputMessage(3,205),0 );	break;
			default : ::CallOkCancelMessageBox( nMenuIndex, 0,0, lan->OutputMessage(5,86), 0 );	break;//lsw
		}						
		MP3( SN_WARNING );
	}
}

void CallDelCharComform()
{
	EWndMgr.ClearAllTxt();//021001 lsw
	y_MenuFocus = MN_DEL_CHAR_COMFORM;
	SMenu[MN_DEL_CHAR_COMFORM].bActive=true;
	EWndMgr.showInputBox(HWND_DELETE_CHAR_KEY);
}

void CallSelectLoginTutorialMenu()// 020815-2 lsw
{
	y_MenuFocus = MN_SELECT_TUTORIAL_LOGIN;
	SMenu[MN_SELECT_TUTORIAL_LOGIN].bActive=true;
}

void ExplainAuctionItem( int x, int y, ItemAttr *olg_item ,const bool bIsDurDraw)//020821-2 lsw
{
	const int trans = 20;
	char temp_str[255] = {0,};
	const int item_type = olg_item->item_no/1000;
	const RareMain *pRareValue = (RareMain*)(&olg_item->attr[IATTR_RARE_MAIN]);
	
	CItem *item = ItemUnit( *olg_item );
	if( !item ) return;
	if( !olg_item->item_no ) return;

	const int kind = item->GetItemKind();

	SetHangulAlign(TA_LEFT);
	const int iWidth = TXT_BG_NORMAL_WIDTH;//021001 lsw
	
	int iCt = 0;
	int iReultLineCount = 0;
	
	int iBlk = 0;
	int iReultBlankSize = 0;
	
	const int iLGab = TxtOut.GetLineGab();
	const int iBlkSize = TxtOut.GetBlankGabSize();
	
	ExplainItemName(x , y, olg_item,iCt, iBlk, false);
#ifdef _DEBUG // finito 060507
	if( GetSysInfo( SI_GAME_MAKE_MODE ) )//������ �̸� ���
	{
		iReultLineCount = TxtOut.RcTXTOutNoBG(x,y+iBlk+iLGab*iCt,iWidth,0,"%s     ( %d )",item->GetItemHanName(), olg_item->item_no);
		iCt += iReultLineCount;
	}
#endif
	if(!bIsDurDraw){return;}
	const int rate = item->GetItemWeight();
	if( rate )
	{
		sprintf( temp_str, lan->OutputMessage(7,153), rate );
		if( item->GetRButton() == DIVIDE_ITEM )
		{
			sprintf( temp_str, lan->OutputMessage(7,154), temp_str, olg_item->attr[IATTR_MUCH] );
		}
		else 
		{
			WORD d_max, d_curr;
			GetItemDuration( *olg_item, d_curr, d_max );
			sprintf( temp_str, lan->OutputMessage(7,155), temp_str, d_curr/10, d_max/10 );
		}
	}
	else
	{
		if( item->GetRButton() == DIVIDE_ITEM )
		{
			if( kind == IK_MONEY )
				sprintf( temp_str, "%d CELL", olg_item->attr[IATTR_MUCH] );
			if( kind == IK_NEW_MONEY )
				sprintf( temp_str, "%d CRIT", olg_item->attr[IATTR_MUCH] );
			else
				sprintf( temp_str, lan->OutputMessage(7,156), olg_item->attr[IATTR_MUCH] );
		}
		else 
		{
			WORD d_max, d_curr;
			GetItemDuration( *olg_item, d_curr, d_max );
			sprintf( temp_str, lan->OutputMessage(7,157), d_curr/10, d_max/10 );
		}
	}

	Hcolor( FONT_COLOR_NUMBER );
	iReultLineCount = TxtOut.RcTXTOutNoBG(x,y+iBlk+iLGab*iCt,iWidth,0,temp_str);
	iCt += iReultLineCount;

}

char* GetTacticName(const int iTacticKind)
{
	if( TACTICS_MAX<iTacticKind || TACTICS_START> iTacticKind){return "Error";}

	int iType = 0, iNum = 0;
	switch(iTacticKind)
	{
	case TACTICS_START:					{ iType = 2,iNum = 740;	}break;//��ƽ�䱸����
	case TACTICS_Crapple:				{ iType = 9,iNum = 451;	}break;
	case TACTICS_swordmanship:			{ iType = 9,iNum = 452;	}break;
	case TACTICS_Archery:				{ iType = 9,iNum = 453;	}break;
	case TACTICS_Fencing:				{ iType = 9,iNum = 454;	}break;
	case TACTICS_Mace_fighting:			{ iType = 9,iNum = 455;	}break;
	case TACTICS_Pierce:				{ iType = 9,iNum = 456;	}break;
	case TACTICS_Whirl:					{ iType = 9,iNum = 457;	}break;
	case TACTICS_Hurl:					{ iType = 9,iNum = 458;	}break;
	case TACTICS_Parrying:				{ iType = 9,iNum = 459;	}break;
	case TACTICS_Double_Swordmanship:	{ iType = 9,iNum = 460;	}break;
	case TACTICS_Double_MaceFighting:	{ iType = 9,iNum = 461;	}break;
	case TACTICS_Magery:				{ iType = 9,iNum = 462;	}break;
	case TACTICS_Orison:				{ iType = 9,iNum = 463;	}break;
	default:
		{
			{return "Error";}
		}break;
	}

	return lan->OutputMessage(iType, iNum );
}

bool CanMoveItemToMenu(const int iFieldType)
{	//�������� �巡�� �ؼ� ���� �� �ִ� �ʵ�
	switch(iFieldType)
	{//021030 lsw
	case FT_INVENTORY :
	case FT_ACCEL_INVENTORY :
	case FT_ARMOR_INVENTORY :
	case FT_BANK_INVENTORY :
	case FT_BANK2_INVENTORY:
	case FT_PARTY:
	case FT_MAKE_ITEM :
	case FT_PARTY_FACE_PUT :
	case FT_GUILD_ITEM_PUT:
	case FT_GM_2ND_SKILL_MAKE_ITEM	:
	case FT_SKILL_MASTER_MAKE_ITEM://021111 lsw
	case FT_EXCHANGE_BOND_MONEY_ITEM:
	case FT_MERCHANT_SELL_ITEM_DROP_ZONE:
	case FT_EXCHANGE_ITEM  :
		{
			return true;
		}break;
	default:
		{
			return false;
		}break;
	}
	return false;
}

void DisplayGmRegistRequest(const int iGmQuestIndex, const int iMainX, const int iMainY)
{
	GM_QUEST *pGmMain = &g_GmMain[iGmQuestIndex];

	Hcolor( FONT_COLOR_NAME );
	::SetHangulAlign( TA_CENTER );
	int iLineCt = 0;
	::Hprint2( iMainX+137, iMainY+127+iLineCt*18, g_DestBackBuf, "%s", pGmMain->name );
	iLineCt++;		// ���� �̸� ���

	::Hcolor( RGB16( 200, 50, 200 ) );
	::SetHangulAlign( TA_LEFT );

	#ifdef CHANGE_MONEY_
	::Hprint2( iMainX+45, iMainY+135+iLineCt*20, g_DestBackBuf, lan->OutputMessage(7,90), pGmMain->need_money );
	#else
	::Hprint2( iMainX+45, iMainY+135+iLineCt*20, g_DestBackBuf, lan->OutputMessage(7,91), pGmMain->need_money );
	#endif

	iLineCt++;
	for( int a=0; a<MAX_GM_NEED_ITEM_COUNT; a++ )
	{
		::Hcolor( RGB16( 200, 250, 200 ) );
		const int iItemNo = pGmMain->need_item[a];
		if( iItemNo )
		{
			::Hprint2( iMainX+45, iMainY+140+iLineCt*20, g_DestBackBuf, lan->OutputMessage(7,92), GetItemName(iItemNo), pGmMain->need_quantity[a]);
			iLineCt++;
		}
	}
}

void DisplayMailIconExplain(const int iX, const int iY,const int iMailType)
{
	const int iWidth = TXT_BG_SHORT_WIDTH;//021001 lsw
	int iCount = 0;
	int iReultLineCount = 0;
	
	int iBlankGab = 0;
	int iReultBlankSize = 0;
	
	const int iLineGab = TxtOut.GetLineGab();
	const int iBlankGabSize = TxtOut.GetBlankGabSize();

	TxtOut.Init(COLOR_BLACK,TXT_BG_SHORT_WIDTH,20);
	TxtOut.PutTextHeaderBGImg(iX,iY+iBlankGab+iLineGab*iCount,iWidth);

	int iKrLanNo = 0;
	switch(iMailType)
	{
	case CMailMgr::NORMAL_MAIL:				{iKrLanNo = 960;}break;
	case CMailMgr::GUILD_MAIL:				{iKrLanNo = 961;}break;
	case CMailMgr::GAMEMASTER_MAIL:			{iKrLanNo = 962;}break;
	case CMailMgr::NORMAL_MAIL_REPLY:		{iKrLanNo = 963;}break;
	case CMailMgr::GAMEMASTER_MAIL_REPLY:	{iKrLanNo = 964;}break;
	default:								{iKrLanNo = 960;}break;
	}

	iReultLineCount = TxtOut.RcTXTOut(iX,iY+iBlankGab+iLineGab*iCount,iWidth,1,lan->OutputMessage(2,iKrLanNo));
	iCount += iReultLineCount;
	TxtOut.PutTextTailBGImg(iX,iY+iBlankGab+iLineGab*iCount,iWidth);
}