#include "stdafx.h"
#include "Hangul.h"
#include "dragon.h"
#include "Menu.h"
#include "Hong_Sprite.h"
#include "SmallMenuSet.h"
#include "stdio.h"
#include "directsound.h"
#include "Char.h"
#include "SmallMenu.h"
#include "Hong_Sub.h"
#include "Effect.h"
#include "map.h"
#include "tool.h"
#include "MenuSet.h"
#include "skill.h"
#include "MenuNetwork.h"
#include "MouseCursor.h"
#include "CharDataTable.h"
#include "gameproc.h"
#include "kh_cpp.h"
#include "skill_lsw.h"
#include "menudef.h"
#include "menudef2.h"
#include "TextOutMgr.h"
#include "Auction.h"
#include "item.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib> // for atoi
#define DIVIDE_ITEM_UPGRDEABLE_MIN 2000
#define DIVIDE_ITEM_UPGRDEABLE_MAX 5000

//#define LIGHT_GM_CLIENT //Eleval 01/06/09

t_SkillMakeItemInfo	skillmakeiteminfo;
extern POS			IP_base;
extern DWORD g_ClientTimeNoGap;
extern void ConvertSpecialWord(char *txt,const char *delTarget,const char AfterConvert);
t_CombatSkill CombatSkill[MAX_COMBAT_SKILL_STRUCT] = {0,};//���� ��ų Doning �޴����� ���°�// 0,0 -> ����� ��ų ��ȣ 1,2,3->�׷� 0 �ڸ��� �Ⱦ��� �ȿ��� ��ų ������ ���� �ȴ�
t_CombatSkillExplain CombatSkillExplain[MAX_COMBAT_SKILL_EXPLAIN];//������ų �����
t_CombatSkillGroupExplain CombatSkillGroupExplain[MAX_COMBAT_SKILL_GROUP_EXPLAIN];//������ų �����
int gm_make_item_result_needitem[2];
MakeItem gm_make_item_list[MAX_UPGRADE_RARE]; //GM�� ���� �������� ����Ʈ
MakeItem gm_make_item_result;	//GM�� ���� �����
bool LOCK_ITEM = false;
//MakeItem aMRMakeSrc[3];

extern int iGM_Makeable_Item_List[300];
t_raregroup			RareGroup[5][MAX_RARE_GROUP];//0 �� ����׷�// 1�� ��� �׷�
t_ItemFunction		ItemFunction[MAX_ITEM_FUNCTION];
t_rareprobability	RareProbability[MAX_RARE_PROBABILITY];
t_ItemLegendSub     LegendSubBonus[MAX_ITEM_LEGEND_SUB];
ITEM_LEGEND			g_aItemLegend[MAX_ITEM_LEGEND];//021113 lsw
ITEM_LEGEND_BONUS	g_aLegendBonus[MAX_ITEM_LEGEND_BONUS];

bool CheckBy2ndGM_This_Item_Upgradeable( ItemAttr ia ,const int iType)//���� �� �������� ���׷��̵� ��ų�� �ִ°����ؼ� �̰� ����� �ֳ��� ����
{
	RareMain gmattr;
	CItem *t;
	t = ItemUnit(ia.item_no);
	const short int item_no   =  ia.item_no;

	if(!t)//�߸��� �������̶��..
	{
		return false;
	}

	if(!item_no) {return false;}//������ �ѹ��� ������

	const RareMain *pInRare = (RareMain*)(&ia.attr[IATTR_RARE_MAIN]);
	if(pInRare->soksung2 || pInRare->soksung3)
	{
		AddCurrentStatusMessage(FONT_COLOR_MAX,lan->OutputMessage(2,880) );
		return false;
	}

	const int iUpgradeType = ItemMutantKind[t->GetItemKind()].upgrade_type - 1;//020701 lsw -1 �켭 ���� �϶�
	if(0 > iUpgradeType ) {return false;}//���� �ȵǴ°ǵ� �Ѥ�;;
	
	if (item_no != 1046 && item_no != 1047)
	{
		if (DIVIDE_ITEM == t->GetRButton())//������ ���ٸ� ���� üũ�� �ؾ� �Ѵ�.
		{
			if (ia.attr[IATTR_MUCH]<DIVIDE_ITEM_UPGRDEABLE_MIN || ia.attr[IATTR_MUCH]>DIVIDE_ITEM_UPGRDEABLE_MAX)
			{
				AddCurrentStatusMessage(FONT_COLOR_MAX, lan->OutputMessage(2, 266), DIVIDE_ITEM_UPGRDEABLE_MIN, DIVIDE_ITEM_UPGRDEABLE_MAX);
				return false;
			}
		}
	}
	else {
		// 1046������ ��������� ��������� ��������� ���������.
		if (ia.attr[IATTR_MUCH] > 1)
		{
			AddCurrentStatusMessage(FONT_COLOR_MAX, lan->OutputMessage(0, 35));
			return false;
		}
	}

	if ( !(ia.attr[IATTR_RARE_MAIN]) )//�Ӽ��� ������ ���׷��̵� �Ұ�
	{	
		AddCurrentStatusMessage(FONT_COLOR_MAX,lan->OutputMessage(2,511));
		return false;
	}
	
	memcpy(&gmattr,&ia.attr[IATTR_RARE_MAIN],sizeof(int));

	if( gmattr.grade > 16)
	{
		AddCurrentStatusMessage(FONT_COLOR_MAX,lan->OutputMessage(2,512));
		return false;
	}

	if(iType)//�⼺�� ��Ƽ���� ����� ���� 
	{
		if( !ItemGrade[gmattr.grade].iAble)
		{
			AddCurrentStatusMessage(FONT_COLOR_MAX,lan->OutputMessage(2,512));
			return false;
		}
	}
	else//GM�� ���׷��̵� ����
	{
		if( !ItemGrade[gmattr.grade+1].iAble)
		{
			AddCurrentStatusMessage(FONT_COLOR_MAX,lan->OutputMessage(2,512));
			return false;
		}
	}
	if( gmattr.iHighLevel)//021119 lsw
	{
		AddCurrentStatusMessage(FONT_COLOR_MAX,lan->OutputMessage(2,519));//����������� �ƴմϴ�
		return false;
	}

	for( int i=0; i<SCharacterData.ItemIdCount; i++ )
	{
		if( item_no == iGM_Makeable_Item_List[i] )
		{
			gm_make_item_result.item_no		= HandItemAttr.item_no;
			gm_make_item_result.attr		= HandItemAttr;

			gm_make_item_result_needitem[0]  =  ItemMutantKind[GetItemAttr( item_no, ITEM_KIND)].AddItem[0]/100;//020505 lsw
			gm_make_item_result_needitem[1]  =  ItemMutantKind[GetItemAttr( item_no, ITEM_KIND)].AddItem[1]/100;
			return true;//���׷��̵忡 ������ �ڽ��� ��ų�� �´� ������
		}
	}
	AddCurrentStatusMessage(FONT_COLOR_MAX,lan->OutputMessage(2,513));
	return false;
}

const int MAX_CORRECT_RARE = 5;
const int Convert[2][MAX_CORRECT_RARE]=
{
	{RARE_POWER,		RARE_POWER,		RARE_POWER,		RARE_VIGOR,			RARE_POWER},
	{RARE_VIGOR,		RARE_VIGOR,		RARE_VIGOR,		RARE_PIERCING,		RARE_PIERCING}
};//020823 lsw
bool CorrectRareKindJustKind(const int iResultSok, const int iTargetSok)
{
	if(iResultSok == iTargetSok)
	{
		return true;
	}
	else
	{
		for(int i = 0; i < MAX_CORRECT_RARE; i++)
		{
			if(	Convert [0][i] == iResultSok)
			{
				if(	Convert [1][i] == iTargetSok)
				{
					return true;
				}
			}
			else if(	Convert [1][i] == iResultSok)
			{
				if(	Convert [0][i] == iTargetSok)
				{
					return true;
				}
			}
		}
	}
	return false;
}
bool CorrectRareKind(const int iResultAttr,const int iCompareAttr)
{
	RareMain *result = ((LPRareMain)(&iResultAttr));
	RareMain *compare = ((LPRareMain)(&iCompareAttr));

	const int iResultSok = result->soksung1;
	const int iTargetSok = compare->soksung1;
	
	if( result->grade != compare->grade){return false;}
	if( result->iHighLevel != compare->iHighLevel){return false;}
	if( result->soksung2 != compare->soksung2){return false;}
	if( result->soksung3 != compare->soksung3){return false;}
	if( result->IsDynamicRare != compare->IsDynamicRare){return false;}

	for(int i = 0; i < MAX_CORRECT_RARE; i++)
	{
		if(	Convert [0][i] == iResultSok)
		{
			if(	Convert [1][i] == iTargetSok)
			{
				return true;
			}
		}
		else if(	Convert [1][i] == iResultSok)
		{
			if(	Convert [0][i] == iTargetSok)
			{
				return true;
			}
		}
	}
	return false;
}

bool CheckBy2ndGM_Insert_to_GMItemList(ItemAttr ia, const int iType)
{
	const int item_no = gm_make_item_result.item_no;//���׷��̵� �ΰ� �������� ���

	CItem *t;
	t = ItemUnit(ia.item_no);

	if(!t)
	{
		return false;
	}

	if( !ia.item_no )	
	{	AddCurrentStatusMessage(FONT_COLOR_MAX,lan->OutputMessage(2,514) );
		return false;
	}

	const RareMain *pInRare = (RareMain*)(&ia.attr[IATTR_RARE_MAIN]);
	if(pInRare->soksung2 || pInRare->soksung3)
	{
		AddCurrentStatusMessage(FONT_COLOR_MAX,lan->OutputMessage(2,880) );
		return false;
	}

	if (ia.item_no != 1046 && ia.item_no != 1047) {
		if (DIVIDE_ITEM == t->GetRButton())
		{
			if (ia.attr[IATTR_MUCH]<DIVIDE_ITEM_UPGRDEABLE_MIN || ia.attr[IATTR_MUCH]>DIVIDE_ITEM_UPGRDEABLE_MAX)
			{
				AddCurrentStatusMessage(FONT_COLOR_MAX, lan->OutputMessage(2, 266), DIVIDE_ITEM_UPGRDEABLE_MIN, DIVIDE_ITEM_UPGRDEABLE_MAX);
				return false;
			}
		}
	}
	else {
		if (ia.attr[IATTR_MUCH] > 1)
		{
			AddCurrentStatusMessage(FONT_COLOR_MAX, lan->OutputMessage(0, 35));
			return false;
		}
	}
	
	if(iType)//�⼺�� ����
	{
		if( !ia.attr[IATTR_RARE_MAIN] )	//�Ӽ��� ����
		{
			AddCurrentStatusMessage(FONT_COLOR_MAX,lan->OutputMessage(2,516));
			return false;
		}

		for(int ct = 0; ct <2; ct++)//������ �ڸ��� ����( Pos üũ )
		{
			if(!memcmp(&g_MultiRareSrc.MultiRareSrc[ct].pos , &IP_base,sizeof(POS)))
			{
				AddCurrentStatusMessage(FONT_COLOR_MAX,lan->OutputMessage(2,518));
				return false;	
			}
			
			const RareMain *pRare = (RareMain*)(&g_MultiRareSrc.MultiRareSrc[ct].attr.attr[IATTR_RARE_MAIN] );
			
			if(pInRare->soksung1)
			{
				if(MAX_DOUBLE_RARE_GRADE < pInRare->grade + pRare->grade)//021212 lsw
				{
					AddCurrentStatusMessage(FONT_COLOR_MAX, lan->OutputMessage(2,881));
					return false;
				}

				if(pRare->soksung1 == pInRare->soksung1)
				{
					AddCurrentStatusMessage(FONT_COLOR_MAX, lan->OutputMessage(2,890));
					return false;
				}
				else//
				{
					if(CorrectRareKindJustKind(pRare->soksung1, pInRare->soksung1))//�ٸ����� �ұ��ϵ� �����ǰ�
					{
						return false;
					}
				}
			}
			else
			{
				AddCurrentStatusMessage(FONT_COLOR_MAX,lan->OutputMessage(2,511));//��� �ƴ��ݾ�
				return false;
			}
			const int iTargetNo = g_MultiRareSrc.MultiRareSrc[ct].attr.item_no;
			if(iTargetNo)
			{
				if(ia.item_no != iTargetNo)
				{
					AddCurrentStatusMessage(FONT_COLOR_MAX,lan->OutputMessage(2,515));
					return false;
				}
			}
		}
		return true;
	}
	else //������ ���� 
	{
		if(ItemMutantKind[GetItemAttr( item_no, ITEM_KIND)].AddItem[0]/100 == ia.item_no)//�ʿ� ������ 1�� �̳�?
		{
			return true;
		}

		if(ItemMutantKind[GetItemAttr( item_no, ITEM_KIND)].AddItem[1]/100 == ia.item_no)//�ʿ� ������ 2�� �̳�?
		{
			return true;
		}
	
		if( !ia.attr[IATTR_RARE_MAIN] )	//�Ӽ��� ����
		{
			AddCurrentStatusMessage(FONT_COLOR_MAX,lan->OutputMessage(2,516));
			return false;
		}

		for(int ct = 0; ct <20; ct++)//������ �ڸ��� ����( Pos üũ )
		{
			if(!memcmp(&gm_make_item_list[ct].pos , &IP_base,sizeof(POS)))
			{
				AddCurrentStatusMessage(FONT_COLOR_MAX,lan->OutputMessage(2,518));
				return false;	
			}
		}
		// ��� �����۰� �ƴ� �������� ���� ����.
		if(((RareMain*)&ia.attr[IATTR_RARE_MAIN])->IsDynamicRare)//�� if���� �Լ��� �������� ����ؾ� �մϴ�.
		{	//��� ������ �κ�
			if(gm_make_item_result.attr.item_no != ia.item_no)
			{
				AddCurrentStatusMessage(FONT_COLOR_MAX,lan->OutputMessage(2,515));
				return false;
			}

			if( gm_make_item_result.attr.attr[IATTR_RARE_MAIN] == ia.attr[IATTR_RARE_MAIN] )//�Ӽ��� ����..
			{
				
			}
			else//��Ƽ ����� �ȴ�
			{//�Ӽ� �ٸ��ų�
				return false;
			}
		}
		else
		{
			if(gm_make_item_result.attr.item_no != ia.item_no)
			{
				AddCurrentStatusMessage(FONT_COLOR_MAX,lan->OutputMessage(2,515));
				return false;
			}
			if( gm_make_item_result.attr.attr[IATTR_RARE_MAIN] != ia.attr[IATTR_RARE_MAIN] )//�Ӽ��� �ٸ���..
			{
				if(CorrectRareKind(gm_make_item_result.attr.attr[IATTR_RARE_MAIN],ia.attr[IATTR_RARE_MAIN]))
				{
					//true�� ���ϵǸ� ������
				}
				else
				{
					AddCurrentStatusMessage(FONT_COLOR_MAX,lan->OutputMessage(2,517));
					return false;
				}
			}
		}
		return true;
	}
	return false;
}
//010708 lsw
int Call_GM_2ND_Skill_Menu()
{
	CloseAllMenu();

	fRMoveMenu = MM_ITEM_OPEN;		// ���� if���� ������ ����
	nItemOther = MN_GM_2ND_SKILL;
	y_MenuFocus = 0;

	ZeroMemory( &gm_make_item_result, sizeof(MakeItem) );	//����Ʈ �ʱ�ȭ
	ZeroMemory(  gm_make_item_list	, sizeof(MakeItem[MAX_UPGRADE_RARE]) );
	LOCK_ITEM = false;
	return 1;
}

int Call_Skill_Master_Menu()
{
	CloseAllMenu();

	fRMoveMenu = MM_ITEM_OPEN;		// ���� if���� ������ ����
	nItemOther = MN_SKILL_MASTER_MAKE;
	y_MenuFocus = 0;
	
	ZeroMemory( &g_MultiRareSrc, sizeof(MULTIRARESRC) );	//����Ʈ �ʱ�ȭ
	return 1;
}

int Call_MERCHANT_QUEST_MENU(const int iGmQuestIndex)
{
	::SetGMQUESTMenu(iGmQuestIndex);	
	::CallSmallMenu(NM_MERCHANT_QUEST);
	return 0;
}

int Call_EXCHANGE_BOND_MONEY_Menu()//021126 lsw
{
	Auction.ClearBondItem();
	CloseAllMenu();	
	fRMoveMenu = MM_ITEM_OPEN;
	nItemOther = MN_EXCHANGE_BOND_MONEY;
	y_MenuFocus = 0;
	SetChatMode(CM_COMMAND);
	return 1;
}

int Call_MERCHANT_REGISTER_Menu()
{
	Auction.ClearSellItem();
	CloseAllMenu();	
	fRMoveMenu = MM_ITEM_OPEN;
	nItemOther = MN_MERCHANT_REGISTER;
	y_MenuFocus = 0;
	//�Ǹ� ǰ�� Ŭ���� �غ�
	SetChatMode(CM_COMMAND);
	return 1;
}

void Send2NDGMMAKE(int skillno)
{
	ItemAttr *t = ::GetItemByPos(	gm_make_item_result.pos );
//	if(!t->item_no) return;
	if( !gm_make_item_result.item_no)
	{
		AddCurrentStatusMessage(FONT_COLOR_MAX,lan->OutputMessage(2,504));
		return;
	}

	t_packet p;	
	p.h.header.type = CMD_SKILL_RARE_UPGRADE;
	p.h.header.size = sizeof( t_SkillRareUpgrade );
	p.u.SkillRareUpgrade.iSkillNo		= skillno;
	p.u.SkillRareUpgrade.SourceItem		= gm_make_item_result.attr;

	for (int ti = 0; ti <MAX_UPGRADE_RARE ; ti++)
	{
		p.u.SkillRareUpgrade.pos[ti] = gm_make_item_list[ti].pos;
		
		DeleteItem( gm_make_item_list[ti].pos ) ;//����â�� ��� ������
		memset( &gm_make_item_list[ti], 0, sizeof( MakeItem ) );
	}

	QueuePacket( &p, 1 );

	memset( &gm_make_item_result, 0, sizeof( MakeItem ) );
	return;
}

void SendSkillMasterMake(int skillno)
{
	ItemAttr *t = ::GetItemByPos(	gm_make_item_result.pos );
	if( !gm_make_item_result.item_no)
	{
		AddCurrentStatusMessage(FONT_COLOR_MAX,lan->OutputMessage(2,504));
		return;
	}

	t_packet p;	
	p.h.header.type = CMD_SKILL_RARE_UPGRADE;
	p.h.header.size = sizeof( t_SkillRareUpgrade );
	p.u.SkillRareUpgrade.iSkillNo		= skillno;
	p.u.SkillRareUpgrade.SourceItem		= gm_make_item_result.attr;

	for (int ti = 0; ti <MAX_UPGRADE_RARE ; ti++)
	{
		p.u.SkillRareUpgrade.pos[ti] = gm_make_item_list[ti].pos;
		
		DeleteItem( gm_make_item_list[ti].pos ) ;//����â�� ��� ������
		memset( &gm_make_item_list[ti], 0, sizeof( MakeItem ) );
	}

	QueuePacket( &p, 1 );

	memset( &gm_make_item_result, 0, sizeof( MakeItem ) );
	return;
}

void Recv2NDGMMAKEResult(t_SkillRareUpgradeResult *p)
{
	SkillStatus	= SKILL_OFF;
	
	p->SourceItem;//�̰Ÿ� �̿� �ؼ� ���⿡ ���ʽ��� ����
	//���� ���̽� ���Ⱑ�� �ϵ��� ����
	SMenu[MN_GM_2ND_SKILL].nField[4].nWillDo = p->iSuccess;
	
	SkillStatus				= SKILL_OFF;
	nItemOther = 0 ;
	SkillSettingClear( );
	
	//011030 lsw
	// SmallMenuClose(); ��ư ������ ������ �ϱ�
	return;
}

//011030 lsw >
DWORD g_skillmaketime = 0;
//011030 lsw <

void Send2NDSKILLMAKE(int iTryRare, int iRareGroup)
{
	//���� �������� ��� �������� ������
	POS s;
	
	memset(&s,0,sizeof(POS));
	
	s.p1 = iTryRare;
	s.p2 = iRareGroup;//rared 
	s.p3 = skillmakeiteminfo.MakeMuch;
	
	g_skillmaketime = (SDL_GetTicks()+skillmakeiteminfo.MakeMuch*1500);
	
	SendSkillInfo( SkillNo, Mox, Moy, SKILLTARGETTYPE_TABLE_, item_select_result.item_no, s);
	//011025 >
	skillmakeiteminfo.MakeMuch = 0;
	SMenu[MN_ITEM_MAKE_SELECT].nField[10].nWillDo	=0;
	CloseAllMenu();
	//011025 <
	return;
}
// ============================================================================
// 1. Rare Group (稀有物品群組)
// ============================================================================

void ReadRareGroup(char * buf, int i)
{
    char *token;

    token = strtok(buf, "\t\n\r");
    if( token == NULL ) return;

    int iIndex = atoi(token);

    // [安全修正] 邊界檢查
    if (iIndex < 0 || iIndex >= MAX_RARE_GROUP) {
        SDL_Log("Error: ReadRareGroup Index out of bounds: %d", iIndex);
        return;
    }

    RareGroup[0][iIndex].no = iIndex;

    // Helper lambda to safely copy string and eat whitespace
    auto SafeCopy = [](char* dest, char* src, size_t size) {
        strncpy(dest, src, size - 1);
        dest[size - 1] = '\0';
        EatRearWhiteChar(dest);
    };

    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; SafeCopy(RareGroup[0][iIndex].name, token, 39);

    // Group 0
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; SafeCopy(RareGroup[0][iIndex].rareset[0].rare, token, 39);
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; RareGroup[0][iIndex].rareset[0].rare_num       = atoi(token);
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; RareGroup[0][iIndex].rareset[0].rare_rate   = atoi(token);

    // Group 1
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; SafeCopy(RareGroup[0][iIndex].rareset[1].rare, token, 39);
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; RareGroup[0][iIndex].rareset[1].rare_num       = atoi(token);
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; RareGroup[0][iIndex].rareset[1].rare_rate   = atoi(token);

    // Group 2
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; SafeCopy(RareGroup[0][iIndex].rareset[2].rare, token, 39);
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; RareGroup[0][iIndex].rareset[2].rare_num       = atoi(token);
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; RareGroup[0][iIndex].rareset[2].rare_rate   = atoi(token);

    // Group 3
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; SafeCopy(RareGroup[0][iIndex].rareset[3].rare, token, 39);
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; RareGroup[0][iIndex].rareset[3].rare_num       = atoi(token);
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; RareGroup[0][iIndex].rareset[3].rare_rate   = atoi(token);

    // Group 4
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; SafeCopy(RareGroup[0][iIndex].rareset[4].rare, token, 39);
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; RareGroup[0][iIndex].rareset[4].rare_num       = atoi(token);
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; RareGroup[0][iIndex].rareset[4].rare_rate   = atoi(token);

    // Group 5
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; SafeCopy(RareGroup[0][iIndex].rareset[5].rare, token, 39);
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; RareGroup[0][iIndex].rareset[5].rare_num       = atoi(token);
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; RareGroup[0][iIndex].rareset[5].rare_rate   = atoi(token);

    // Group 6
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; SafeCopy(RareGroup[0][iIndex].rareset[6].rare, token, 39);
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; RareGroup[0][iIndex].rareset[6].rare_num       = atoi(token);
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; RareGroup[0][iIndex].rareset[6].rare_rate   = atoi(token);

    // Group 7
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; SafeCopy(RareGroup[0][iIndex].rareset[7].rare, token, 39);
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; RareGroup[0][iIndex].rareset[7].rare_num       = atoi(token);
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; RareGroup[0][iIndex].rareset[7].rare_rate   = atoi(token);

    // Group Buffers
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; memcpy(RareGroup[0][iIndex].group_buffer_1  ,token, 39);
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; memcpy(RareGroup[0][iIndex].group_buffer_2  ,token, 39);
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; memcpy(RareGroup[0][iIndex].group_buffer_3  ,token, 39);

    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; RareGroup[0][iIndex].group_buffer_4    = atoi(token);
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; RareGroup[0][iIndex].group_buffer_5    = atoi(token);
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; RareGroup[0][iIndex].group_buffer_6    = atoi(token);
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; RareGroup[0][iIndex].group_buffer_7    = atoi(token);
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; RareGroup[0][iIndex].group_buffer_8    = atoi(token);
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; RareGroup[0][iIndex].group_buffer_9    = atoi(token);
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; RareGroup[0][iIndex].group_buffer_10   = atoi(token);
}

bool LoadRareGroup()
{
    // [Android] 路徑修正
    const char* filenameBin = "Itemtable/rare_group.bin";

    // ---------------------------------------------------------
    // 優先讀取 BIN
    // ---------------------------------------------------------
    SDL_RWops* fp = SDL_RWFromFile(filenameBin, "rb");
    if (fp)
    {
        // 讀取整個陣列
        // 注意：原代碼是用 RareGroup[0]，表示這是二維陣列的一列
        SDL_RWread(fp, RareGroup[0], sizeof(t_raregroup) * MAX_RARE_GROUP, 1);
        SDL_RWclose(fp);
        return true;
    }

    // ---------------------------------------------------------
    // 讀取 TXT (僅限 PC Debug)
    // ---------------------------------------------------------
#if defined(_DEBUG) && !defined(__ANDROID__)
    const char* filenameTxt = "itemtable/rare_group.txt";
    fp = SDL_RWFromFile(filenameTxt, "rb");
    if (fp)
    {
       Sint64 size = SDL_RWsize(fp);
       std::string content; content.resize(size);
       SDL_RWread(fp, &content[0], size, 1);
       SDL_RWclose(fp);

       std::stringstream ss(content);
       std::string line;
       char buf[1024];
       int i = 0;

       while(std::getline(ss, line)) {
          if (line.empty() || line[0] == ';') continue;
          if (line.back() == '\r') line.pop_back();
          if (i > MAX_RARE_GROUP - 1) break;

          strncpy(buf, line.c_str(), 1023);
          buf[1023] = '\0';

          ReadRareGroup(buf, i);
          i++;
       }

       // 寫入 BIN
       SDL_RWops* fpOut = SDL_RWFromFile(filenameBin, "wb");
       if (fpOut) {
          SDL_RWwrite(fpOut, RareGroup[0], sizeof(t_raregroup) * MAX_RARE_GROUP, 1);
          SDL_RWclose(fpOut);
       }
       return true;
    }
#endif

    SDL_Log("Error: Failed to load RareGroup (Bin not found)");
    return false;
}

// ============================================================================
// 2. Rare Probability (稀有掉落機率)
// ============================================================================

void ReadRareProbability(char * buf, int i)
{
    char *token;
    int *TargetPointer;

    // [安全修正] 邊界檢查
    if (i < 0 || i >= MAX_RARE_PROBABILITY) return;

    TargetPointer = &RareProbability[i].num;

    token = strtok(buf, "\t\n\r");
    if( token == NULL ) return;
    (*TargetPointer) = atoi(token);
    TargetPointer++;

    // 讀取後續的 18 個數值
    for (int j = 0; j < 18; j++)
    {
        token = strtok(NULL, "\t\n\r");
        if( token == NULL ) return;
        (*TargetPointer) = atoi(token);
        TargetPointer++;
    }
}

bool LoadRareProbability()
{
    // [Android] 路徑修正
    const char* filenameBin = "Itemtable/rare_probability.bin";

    // ---------------------------------------------------------
    // 優先讀取 BIN
    // ---------------------------------------------------------
    SDL_RWops* fp = SDL_RWFromFile(filenameBin, "rb");
    if (fp)
    {
        SDL_RWread(fp, RareProbability, sizeof(t_rareprobability) * MAX_RARE_PROBABILITY, 1);
        SDL_RWclose(fp);
        return true;
    }

    // ---------------------------------------------------------
    // 讀取 TXT (僅限 PC Debug)
    // ---------------------------------------------------------
#if defined(_DEBUG) && !defined(__ANDROID__)
    const char* filenameTxt = "itemtable/rare_probability.txt";
    fp = SDL_RWFromFile(filenameTxt, "rb");
    if (fp)
    {
       Sint64 size = SDL_RWsize(fp);
       std::string content; content.resize(size);
       SDL_RWread(fp, &content[0], size, 1);
       SDL_RWclose(fp);

       std::stringstream ss(content);
       std::string line;
       char buf[1024];
       int i = 0;

       while(std::getline(ss, line)) {
          if (line.empty() || line[0] == ';') continue;
          if (line.back() == '\r') line.pop_back();
          if (i > MAX_RARE_PROBABILITY - 1) break;

          strncpy(buf, line.c_str(), 1023);
          buf[1023] = '\0';

          ReadRareProbability(buf, i);
          i++;
       }

       // 寫入 BIN
       SDL_RWops* fpOut = SDL_RWFromFile(filenameBin, "wb");
       if (fpOut) {
          SDL_RWwrite(fpOut, RareProbability, sizeof(t_rareprobability) * MAX_RARE_PROBABILITY, 1);
          SDL_RWclose(fpOut);
       }
       return true;
    }
#endif

    SDL_Log("Error: Failed to load RareProbability (Bin not found)");
    return false;
}
void RecvCMD_RARE_UPGRADE_BBS(t_packet *p)
{
//#ifdef	KOREA_LOCALIZING_
	ItemAttr item;
	char name[20];
	memcpy ( name,p->u.RareUpgradeBBS.name,20);
	item = p->u.RareUpgradeBBS.Item;
	RareMain Soksung;
	memcpy(&Soksung,&item.attr[IATTR_RARE_MAIN],4);
	//011025 lsw >
	char itemname[300];
	
	strcpy(itemname,GetItemName(item.item_no));
	
	EatRearWhiteChar(itemname);
	//011030 lsw >
	MP3(67);

	if (Soksung.soksung1 && Soksung.soksung2) {
		p->u.RareUpgradeBBS.Santa ? 
			    AddCurrentStatusMessage(FONT_COLOR_RARE_MAIN, lan->OutputMessage(0, 89), name, itemname, Soksung.grade):
				AddCurrentStatusMessage(FONT_COLOR_RARE_MAIN, lan->OutputMessage(2, 506), name, itemname, Soksung.grade);
	}
	else {
		if (!Soksung.IsDynamicRare)//020626 lsw
		{
			AddCurrentStatusMessage(FONT_COLOR_RARE_MAIN, lan->OutputMessage(2, 503), name, ItemRare[Soksung.soksung1].LocalName, itemname, Soksung.grade);
		}
		else
		{
			AddCurrentStatusMessage(FONT_COLOR_RARE_MAIN, lan->OutputMessage(2, 503), name, ItemFunction[Soksung.soksung1].NameAdd, itemname, Soksung.grade);
		}
	}
	//011030 lsw <
//#endif
	return;
}

int GetSkillResorceAmount(int &r1count ,int &r2count ,int &r3count ,int &r4count)
{
	int join_no = item_select_result.join_no;		// ������� �ϴ� �������� ���° �������ΰ�...

	CItem  *rit1= ItemUnit(item_join[join_no].resource1);;
	CItem  *rit2= ItemUnit(item_join[join_no].resource2);;
	CItem  *rit3= ItemUnit(item_join[join_no].resource3);;
	CItem  *rit4= ItemUnit(item_join[join_no].resource4);;

	int r1amount= item_join[join_no].resource1_amount;	
	int r2amount= item_join[join_no].resource2_amount;	
	int r3amount= item_join[join_no].resource3_amount;	
	int r4amount= item_join[join_no].resource4_amount;	

	int r1= -1;
	int r2= -1;
	int r3= -1;
	int r4= -1;

	int jj=0;
	int kk=0;
	int ii=0;

	ItemAttr Item ; 
	const int ino1 = item_join[join_no].resource1;
	const int ino2 = item_join[join_no].resource2;
	const int ino3 = item_join[join_no].resource3;
	const int ino4 = item_join[join_no].resource4;

	for(jj=0;jj<3;jj++)
	{
		for(kk=0;kk<3;kk++) 
		{
			for(ii=0;ii<8;ii++)  //�κ��丮â�� ���� �ִ� ũ��� 3
			{   
				Item = InvItemAttr[jj][kk][ii];
				if( rit1 )
				{
					if(ino1 == Item.item_no)
					{
						if(DIVIDE_ITEM == rit1->GetRButton()){r1count += Item.attr[IATTR_MUCH];}
						else{r1count++;}
					}

				}
				if( rit2 )
				{
					if(ino2 == Item.item_no)
					{
						if(DIVIDE_ITEM == rit2->GetRButton()){r2count += Item.attr[IATTR_MUCH];}
						else{r2count++;}
					}
				}
				if( rit3 )
				{
					if(ino3 == Item.item_no)
					{
						if(DIVIDE_ITEM==rit3->GetRButton()){r3count += Item.attr[IATTR_MUCH];}
						else{r3count++;}
					}
				}
				if( rit4 )
				{
					if(ino4 == Item.item_no)
					{
						if(DIVIDE_ITEM==rit4->GetRButton()){r4count += Item.attr[IATTR_MUCH];}
						else{r4count++;}
					}
				}
			}
		}
	}
	return 0;
}

void RecvCMD_ADD_EFFECT(t_packet *p)
{
	const	WORD	wEffectNo		=p->u.AddEffect.wEffectNo;	// ���� ������ ��ȣ
	const	WORD	idTarget		=p->u.AddEffect.idTarget;	// ������ ����� �����
	const	DWORD	dwDuration		=p->u.AddEffect.wDuration*1000;	// �����ð�
	LPCHARACTER	ch = ReturnCharacterPoint(idTarget);
	if(ch)
	{
		InsertMagic(ch,ch,wEffectNo,0,ch->x,ch->y,ch->x,ch->y,0,0,0,g_ClientTime+dwDuration);
	}
}

// ============================================================================
// 1. 解析函式 (邏輯保持不變，但在使用 iIndex 前加入安全檢查)
// ============================================================================
void ReadItemFunctionTable(char * buf, int i)
{
    char *token;

    token = strtok(buf, "\t\n\r");
    if( token == NULL ) return;

    // 取得索引 ID
    int iIndex = atoi(token);

    // [安全修正] 防止陣列越界導致崩潰
    if (iIndex < 0 || iIndex >= MAX_ITEM_FUNCTION) {
        SDL_Log("Error: ItemFunctionTable Index out of bounds: %d", iIndex);
        return;
    }

    ItemFunction[i].iNo = iIndex; // 這行原本的邏輯似乎是把讀到的 ID 存回第 i 格，但也用 ID 當索引存下面的資料
    // 注意：原本代碼是 ItemFunction[i].iNo = iIndex; 但下面用 ItemFunction[iIndex]...
    // 這意味著 txt 檔裡的順序 (i) 和 ID (iIndex) 可能不一致。這裡保留原邏輯。

    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return;
    // 建議用 strncpy 防止溢位，但這裡保留 memcpy 以維持原行為，但在後面補 \0 安全
    memcpy(ItemFunction[iIndex].Name, token, 39); ItemFunction[iIndex].Name[39] = 0;
    EatRearWhiteChar( ItemFunction[iIndex].Name );

    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return;
    memcpy(ItemFunction[iIndex].NameAdd, token, 39); ItemFunction[iIndex].NameAdd[39] = 0;
    EatRearWhiteChar( ItemFunction[iIndex].NameAdd );

    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return;
    memcpy(ItemFunction[iIndex].Exp, token, 49); ItemFunction[iIndex].Exp[49] = 0;
    EatRearWhiteChar( ItemFunction[iIndex].Exp );

    token = strtok(NULL,"\t\n\r"); if( token == NULL )    return; ItemFunction[iIndex].iExpMark   = atoi(token);
    token = strtok(NULL,"\t\n\r"); if( token == NULL )    return; ItemFunction[iIndex].iEffectNo  = atoi(token);
    token = strtok(NULL,"\t\n\r"); if( token == NULL )    return; ItemFunction[iIndex].iUpgradeAble   = atoi(token);
    token = strtok(NULL,"\t\n\r"); if( token == NULL )    return; ItemFunction[iIndex].iScrollNo  = atoi(token);

    token = strtok(NULL,"\t\n\r"); if( token == NULL )    return; ItemFunction[iIndex].iMakeRand[0]   = atoi(token);
    token = strtok(NULL,"\t\n\r"); if( token == NULL )    return; ItemFunction[iIndex].iMakeRand[1]   = atoi(token);
    token = strtok(NULL,"\t\n\r"); if( token == NULL )    return; ItemFunction[iIndex].iMakeRand[2]   = atoi(token);

    for(int x = 0; x<17;x++)
    {
        token = strtok(NULL,"\t\n\r"); if( token == NULL )    return; ItemFunction[iIndex].GRADE[x]   = atoi(token);
    }

    for(int y = 0; y<17;y++)
    {
        token = strtok(NULL,"\t\n\r"); if( token == NULL )    return; ItemFunction[iIndex].iTime[y]   = atoi(token);
    }

    token = strtok(NULL,"\t\n\r"); if( token == NULL )    return; ItemFunction[iIndex].iBuffer1      = atoi(token);
    token = strtok(NULL,"\t\n\r"); if( token == NULL )    return; ItemFunction[iIndex].iBuffer2      = atoi(token);
}

// ============================================================================
// 2. 讀取函式 (Android 優先讀 Bin)
// ============================================================================
bool LoadItemFunctionTable()
{
    // [Android] 路徑修正：移除 .\\ 並改用正斜線
    const char* filenameBin = "Itemtable/ItemFunction.bin";

    // ---------------------------------------------------------
    // 優先讀取 BIN (Android 唯一路徑)
    // ---------------------------------------------------------
    SDL_RWops* fp = SDL_RWFromFile(filenameBin, "rb");

    if (fp)
    {
        // 直接讀取整個結構陣列
        SDL_RWread(fp, ItemFunction, sizeof(t_ItemFunction) * MAX_ITEM_FUNCTION, 1);
        SDL_RWclose(fp);
        return true;
    }

    // ---------------------------------------------------------
    // 讀取 TXT (僅限 PC Debug 模式，Android 略過)
    // ---------------------------------------------------------
#if defined(_DEBUG) && !defined(__ANDROID__)
    const char* filenameTxt = "itemtable/ItemFunction.txt";
    fp = SDL_RWFromFile(filenameTxt, "rb"); // 讀取文字檔也用 rb，自行處理換行

    if (fp)
    {
       // 讀取全部內容到記憶體
       Sint64 fileSize = SDL_RWsize(fp);
       std::string content;
       content.resize(fileSize);
       SDL_RWread(fp, &content[0], fileSize, 1);
       SDL_RWclose(fp);

       std::stringstream ss(content);
       std::string line;
       char buf[1024];
       int i = 0;

       while (std::getline(ss, line))
       {
          // 移除 \r
          if (!line.empty() && line.back() == '\r') line.pop_back();

          if (line.empty() || line[0] == ';') continue;
          if (i > MAX_ITEM_FUNCTION - 1) break;

          // 複製到 buffer 供 strtok 使用
          strncpy(buf, line.c_str(), 1023);
          buf[1023] = '\0';

          ReadItemFunctionTable(buf, i);
          i++;
       }

       // 寫入 BIN (僅 PC)
       SDL_RWops* fpOut = SDL_RWFromFile(filenameBin, "wb");
       if (fpOut)
       {
          SDL_RWwrite(fpOut, ItemFunction, sizeof(t_ItemFunction) * MAX_ITEM_FUNCTION, 1);
          SDL_RWclose(fpOut);
       }
       return true;
    }
#endif

    // 如果連 Bin 都讀不到，回傳失敗
    SDL_Log("Error: Failed to load ItemFunctionTable (Bin not found)");
    return false;
}
bool CanViewGhost(CHARACTER *ch)
{
	if(ch->bCanViewGhost)
	{
		if( g_ClientTimeNoGap < ch->dwCanViewGhostTime)
		{
			SDL_Log("CanViewGhost TRUE");
			return true;	
		}
	}
	return false;
}

bool	CanViewMagicTrap(CHARACTER *ch)
{
	if(ch->bCanViewMagicTrap)
	{
		if( g_ClientTimeNoGap < ch->dwCanViewMagicTrapTime )
		{
			return true;	
		}
	}
	return false;
}

bool	CanViewTrap(CHARACTER *ch)
{
	if(ch->bCanViewTrap)
	{
		if( g_ClientTimeNoGap < ch->dwCanViewTrapTime )
		{
			return true;
		}
	}
	return false;
}

bool	CanViewBomb(CHARACTER *ch)//Trap 0
{
	if( ch->bCanViewBomb )
	{
		if( g_ClientTimeNoGap < ch->dwCanViewBombTime)
		{
			return true;
		}
	}
	return false;
}

void InitHeroCanView()
{
	Hero->bCanViewGhost			= 0;
	Hero->bCanViewMagicTrap		= 0;
	Hero->bCanViewBomb			= 0;
	Hero->bCanViewTrap			= 0;
}

//���� ���� �ϴ� �Լ� 
void SetDayLight(const short int nValue)
{
	SDL_Log("DayLightControl [%d]", nValue);
	DayLightControl = nValue;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////// ���� ��ų
/////////////////////////////////////////////////////// ���� ��ų Point

int GetCombatSkillIndexStart(const int iGroupNo,const int iGroupType)
{
	return (iGroupNo-1)*12 +(iGroupType-1)*6 +1;
}

int SkillIdToCombatSkillIconImage(const int iSkilNo,const bool bIsMain)
{
	if(!bIsMain)
	{
		return (iSkilNo-iCSCovertSkillID);//i�� ��ų �ѹ��� iCSCovertSkillID�� ���� �����̴ϱ� iCSCovertSkillID�� �����Ѵ� �̹��� �����ſ� 36 ������ ���ο�Ŷ� +35 �Ѵ�
	}
	
	return (iSkilNo-iCSCovertSkillID)+36;//i�� ��ų �ѹ��� iCSCovertSkillID�� ���� �����̴ϱ� iCSCovertSkillID�� �����Ѵ�
}

int GetKnowCSGroupType(const int iGroupNo)//�׷�ѹ��� ������ ���� �׷�Ÿ���� ��ȯ�Ѵ�. �׷��� �ƹ��͵� ������ 0�� ��ȯ
{
	switch(iGroupNo)
	{
		case 1:		case 2:		case 3:
			break;
		default:
			JustMsg("GetKnowCSGroupType�� �׷찪�� �̻��մϴ� ");
			break;
	}

	// iGroupNo = 1 iGroup1Start = 115 iGroup1End =120 iGroup2Start = 121 iGroup2End = 126
	// iGroupNo = 2 iGroup1Start = 127 iGroup1End =132 iGroup2Start = 133 iGroup2End = 138
	// iGroupNo = 3 iGroup1Start = 139 iGroup1End =144 iGroup2Start = 145 iGroup2End = 150
	const int iGroup1Start	= iCSCovertSkillID+(iGroupNo-1)*12;
	const int iGroup1End	= iGroup1Start+5;	
	const int iGroup2Start	= iGroup1End+1;	
	const int iGroup2End	= iGroup2Start +5;	
	int iGroupTypeSum1 = 0, iGroupTypeSum2 =0 ,i = 0;
	
	for(i = iGroup1Start; i <=iGroup1End; i++)
	{
		int skillid = SCharacterData.SkillId[i];
		iGroupTypeSum1 += skillid;
	}
	for(i = iGroup2Start; i <=iGroup2End; i++)
	{
		int skillid = SCharacterData.SkillId[i];
		iGroupTypeSum2 += skillid;
	}
	//SDL_Log("iGroupNo(%d) skillID(%d) Total(%d)", iGroupNo, i, iGroupTypeSum2);
	/*for (i = 115; i <= 150; i++)
	{
		int j = SCharacterData.SkillId[i];
		SDL_Log("skillID(%d) (%d) Total(%d)", i, j, iGroupTypeSum2);
		iGroupTypeSum2 += j;
	}*/

	if( iGroupTypeSum1 > iGroupTypeSum2)
	{
		return 1;
	}
	if( iGroupTypeSum2 > iGroupTypeSum1)
	{
		return 2;
	}
	return 0;
}

void SetCSPButtonType(const int iSelectedGroupButton)//���� 3�� �ִ� �׷� ��ư�� �����°� �� ��ư�� nWillDo ���� ��ư�� ���� �׷� ���̴�
{//�θ� �׷��� ������ ���� �� �׷�Ÿ���� ���� �ؾ� �ϴ°�?, �̹� ����� ���׷��̵� �ܰ��ΰ��� �̾� ��ư�� �����Ѵ�
//���� ��ư Ȱ��ȭ�� å������	
	switch(GetKnowCSGroupType(iSelectedGroupButton))
	{
	case 0://�׷� ������
		{
			SMenu[MN_COMBAT_SKILL_POINT].nImageNumber	=	9;//�̹��� �ѹ��� �׷�Ÿ�Լ��� �Ҷ� �̹�����
			//OK		01
			SMenu[MN_COMBAT_SKILL_POINT].nField[1].nType = FT_NONE;//��Ȱ��
			//DONE		03
			SMenu[MN_COMBAT_SKILL_POINT].nField[3].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;//Ȱ��
			
			SMenu[MN_COMBAT_SKILL_POINT].nField[7].nType = FT_NONE;//Ȱ��
			SMenu[MN_COMBAT_SKILL_POINT].nField[8].nType = FT_NONE;//Ȱ��
			SMenu[MN_COMBAT_SKILL_POINT].nField[9].nType = FT_NONE;//Ȱ��
			SMenu[MN_COMBAT_SKILL_POINT].nField[10].nType = FT_NONE;//Ȱ��
			SMenu[MN_COMBAT_SKILL_POINT].nField[11].nType = FT_NONE;//Ȱ��
			SMenu[MN_COMBAT_SKILL_POINT].nField[12].nType = FT_NONE;//Ȱ��

			//�׷� ���� ��ų1	02	//nWillDo ���� �׷� Ÿ�� 1Ÿ�� 2Ÿ��
			SMenu[MN_COMBAT_SKILL_POINT].nField[13].nType = FT_SHOW_SPECIAL_WILLDO_AUTO_PUT;//Ȱ��
			//�׷� ���� ��ų2	02
			SMenu[MN_COMBAT_SKILL_POINT].nField[14].nType = FT_SHOW_SPECIAL_WILLDO_AUTO_PUT;//Ȱ��
			//����Ʈ ��			01//�ѱ� ������ ���� �ϴ� ��ư�� ����� Rect �� �̿� �Ͽ� 1 �ʵ�� ó��
			SMenu[MN_COMBAT_SKILL_POINT].nField[15].nType = FT_NONE;//Ȱ��

			SMenu[MN_COMBAT_SKILL_POINT].nField[16].nType  = FT_CS_GROUP_EXPLAIN;
			SMenu[MN_COMBAT_SKILL_POINT].nField[17].nType  = FT_CS_GROUP_EXPLAIN;
			
			SMenu[MN_COMBAT_SKILL_POINT].nField[18].nType  = FT_CS_EXPLAIN;
			SMenu[MN_COMBAT_SKILL_POINT].nField[19].nType  = FT_CS_EXPLAIN;
			SMenu[MN_COMBAT_SKILL_POINT].nField[20].nType  = FT_CS_EXPLAIN;
			SMenu[MN_COMBAT_SKILL_POINT].nField[21].nType  = FT_CS_EXPLAIN;
			SMenu[MN_COMBAT_SKILL_POINT].nField[22].nType  = FT_CS_EXPLAIN;
			SMenu[MN_COMBAT_SKILL_POINT].nField[23].nType  = FT_CS_EXPLAIN;

			SMenu[MN_COMBAT_SKILL_POINT].nField[24].nType  = FT_CS_EXPLAIN;
			SMenu[MN_COMBAT_SKILL_POINT].nField[25].nType  = FT_CS_EXPLAIN;
			SMenu[MN_COMBAT_SKILL_POINT].nField[26].nType  = FT_CS_EXPLAIN;
			SMenu[MN_COMBAT_SKILL_POINT].nField[27].nType  = FT_CS_EXPLAIN;
			SMenu[MN_COMBAT_SKILL_POINT].nField[28].nType  = FT_CS_EXPLAIN;
			SMenu[MN_COMBAT_SKILL_POINT].nField[29].nType  = FT_CS_EXPLAIN;
			}break;
	default://�׷��� �ִ�
		{
			SMenu[MN_COMBAT_SKILL_POINT].nImageNumber	=	0;//�̹��� �ѹ��� �׷�Ÿ�Լ��� �Ҷ� �̹�����
			//OK		01
			SMenu[MN_COMBAT_SKILL_POINT].nField[1].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;//��Ȱ��
			//DONE		03
			SMenu[MN_COMBAT_SKILL_POINT].nField[3].nType = FT_NONE;//Ȱ��
			
			SMenu[MN_COMBAT_SKILL_POINT].nField[7].nType = FT_CS_ICON;//Ȱ��
			SMenu[MN_COMBAT_SKILL_POINT].nField[8].nType = FT_CS_ICON;//Ȱ��
			SMenu[MN_COMBAT_SKILL_POINT].nField[9].nType = FT_CS_ICON;//Ȱ��
			SMenu[MN_COMBAT_SKILL_POINT].nField[10].nType = FT_CS_ICON;//Ȱ��
			SMenu[MN_COMBAT_SKILL_POINT].nField[11].nType = FT_CS_ICON;//Ȱ��
			SMenu[MN_COMBAT_SKILL_POINT].nField[12].nType = FT_CS_ICON;//Ȱ��

			//�׷� ���� ��ų1	02	//nWillDo ���� �׷� Ÿ�� 1Ÿ�� 2Ÿ��
			SMenu[MN_COMBAT_SKILL_POINT].nField[13].nType = FT_NONE;//Ȱ��
			//�׷� ���� ��ų2	02
			SMenu[MN_COMBAT_SKILL_POINT].nField[14].nType = FT_NONE;//Ȱ��
			//����Ʈ ��			01//�ѱ� ������ ���� �ϴ� ��ư�� ����� Rect �� �̿� �Ͽ� 1 �ʵ�� ó��
			SMenu[MN_COMBAT_SKILL_POINT].nField[15].nType = FT_CSP_POINT_UP;//Ȱ��

			SMenu[MN_COMBAT_SKILL_POINT].nField[16].nType  = FT_NONE;
			SMenu[MN_COMBAT_SKILL_POINT].nField[17].nType  = FT_NONE;

			SMenu[MN_COMBAT_SKILL_POINT].nField[18].nType  = FT_NONE;
			SMenu[MN_COMBAT_SKILL_POINT].nField[19].nType  = FT_NONE;
			SMenu[MN_COMBAT_SKILL_POINT].nField[20].nType  = FT_NONE;
			SMenu[MN_COMBAT_SKILL_POINT].nField[21].nType  = FT_NONE;
			SMenu[MN_COMBAT_SKILL_POINT].nField[22].nType  = FT_NONE;
			SMenu[MN_COMBAT_SKILL_POINT].nField[23].nType  = FT_NONE;

			SMenu[MN_COMBAT_SKILL_POINT].nField[24].nType  = FT_NONE;
			SMenu[MN_COMBAT_SKILL_POINT].nField[25].nType  = FT_NONE;
			SMenu[MN_COMBAT_SKILL_POINT].nField[26].nType  = FT_NONE;
			SMenu[MN_COMBAT_SKILL_POINT].nField[27].nType  = FT_NONE;
			SMenu[MN_COMBAT_SKILL_POINT].nField[28].nType  = FT_NONE;
			SMenu[MN_COMBAT_SKILL_POINT].nField[29].nType  = FT_NONE;
		}break;
	}
}

void SetCSPButtonTypeWhenSelectGroupType()
{//��ư�� ���� �Ѵ�
	SMenu[MN_COMBAT_SKILL_POINT].nImageNumber	=	0;//�̹��� �ѹ��� �׷�Ÿ�Լ��� �Ҷ� �̹�����
	//OK		01
	SMenu[MN_COMBAT_SKILL_POINT].nField[1].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;//��Ȱ��
	//CANCEL	02
	SMenu[MN_COMBAT_SKILL_POINT].nField[2].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;//��Ȱ��
	//DONE		03
	SMenu[MN_COMBAT_SKILL_POINT].nField[3].nType = FT_NONE;//Ȱ��
	
	SMenu[MN_COMBAT_SKILL_POINT].nField[7].nType = FT_CS_ICON;//Ȱ��
	SMenu[MN_COMBAT_SKILL_POINT].nField[8].nType = FT_CS_ICON;//Ȱ��
	SMenu[MN_COMBAT_SKILL_POINT].nField[9].nType = FT_CS_ICON;//Ȱ��
	SMenu[MN_COMBAT_SKILL_POINT].nField[10].nType = FT_CS_ICON;//Ȱ��
	SMenu[MN_COMBAT_SKILL_POINT].nField[11].nType = FT_CS_ICON;//Ȱ��
	SMenu[MN_COMBAT_SKILL_POINT].nField[12].nType = FT_CS_ICON;//Ȱ��

	//�׷� ���� ��ų1	02	//nWillDo ���� �׷� Ÿ�� 1Ÿ�� 2Ÿ��
	SMenu[MN_COMBAT_SKILL_POINT].nField[13].nType = FT_NONE;//Ȱ��
	//�׷� ���� ��ų2	02
	SMenu[MN_COMBAT_SKILL_POINT].nField[14].nType = FT_NONE;//Ȱ��
	//����Ʈ ��			01//�ѱ� ������ ���� �ϴ� ��ư�� ����� Rect �� �̿� �Ͽ� 1 �ʵ�� ó��
	SMenu[MN_COMBAT_SKILL_POINT].nField[15].nType = FT_CSP_POINT_UP;//Ȱ��

	SMenu[MN_COMBAT_SKILL_POINT].nField[16].nType = FT_NONE;//Ȱ��
	SMenu[MN_COMBAT_SKILL_POINT].nField[17].nType = FT_NONE;//Ȱ��
	
	SMenu[MN_COMBAT_SKILL_POINT].nField[18].nType = FT_NONE;//Ȱ��
	SMenu[MN_COMBAT_SKILL_POINT].nField[19].nType = FT_NONE;//Ȱ��
	SMenu[MN_COMBAT_SKILL_POINT].nField[20].nType = FT_NONE;//Ȱ��
	SMenu[MN_COMBAT_SKILL_POINT].nField[21].nType = FT_NONE;//Ȱ��
	SMenu[MN_COMBAT_SKILL_POINT].nField[22].nType = FT_NONE;//Ȱ��
	SMenu[MN_COMBAT_SKILL_POINT].nField[23].nType = FT_NONE;//Ȱ��

	SMenu[MN_COMBAT_SKILL_POINT].nField[24].nType = FT_NONE;//Ȱ��
	SMenu[MN_COMBAT_SKILL_POINT].nField[25].nType = FT_NONE;//Ȱ��
	SMenu[MN_COMBAT_SKILL_POINT].nField[26].nType = FT_NONE;//Ȱ��
	SMenu[MN_COMBAT_SKILL_POINT].nField[27].nType = FT_NONE;//Ȱ��
	SMenu[MN_COMBAT_SKILL_POINT].nField[28].nType = FT_NONE;//Ȱ��
	SMenu[MN_COMBAT_SKILL_POINT].nField[29].nType = FT_NONE;//Ȱ��
}

void SetCSPGroupButtonImage(const int iSelected)//�׷� ��ư �̹��� ����
{
	SFIELD *f = &SMenu[MN_COMBAT_SKILL_POINT].nField[4];
	switch(GetKnowCSGroupType(1))
	{ 
	case 1:
		{
			f->nImageNumber	=	12;
			f->nRectImage	=	11;
		}break;
	case 2:
		{
			f->nImageNumber	=	14;
			f->nRectImage	=	13;
		}break;
	case 0:
		{
			f->nImageNumber	=	24;
			f->nRectImage	=	23;
		}break;
	default:
		break;
	}
	f = &SMenu[MN_COMBAT_SKILL_POINT].nField[5];
	switch(GetKnowCSGroupType(2))
	{
	case 1:
		{
			f->nImageNumber	=	16;
			f->nRectImage	=	15;
		}break;
	case 2:
		{
			f->nImageNumber	=	18;
			f->nRectImage	=	17;
		}break;
	case 0:
		{
			f->nImageNumber	=	26;
			f->nRectImage	=	25;
		}break;
	default:
		break;
	}
	f = &SMenu[MN_COMBAT_SKILL_POINT].nField[6];
	switch(GetKnowCSGroupType(3))
	{
	case 1:
		{
			f->nImageNumber	=	20;
			f->nRectImage	=	19;
		}break;
	case 2:
		{
			f->nImageNumber	=	22;
			f->nRectImage	=	21;
		}break;
	case 0:
		{
			f->nImageNumber	=	28;
			f->nRectImage	=	27;
		}break;
	default:
		break;
	}
	
	switch(iSelected)//���õȰ��� ���� ��ư����
	{
	case 1:
		{
			f = &SMenu[MN_COMBAT_SKILL_POINT].nField[4];
			f->nImageNumber	=	f->nRectImage;
		}break;
	case 2:
		{
			f = &SMenu[MN_COMBAT_SKILL_POINT].nField[5];
			f->nImageNumber	=	f->nRectImage;
		}break;
	case 3:
		{
			f = &SMenu[MN_COMBAT_SKILL_POINT].nField[6];
			f->nImageNumber	=	f->nRectImage;
		}break;
	default:
		break;
	}
	//�׷� Ÿ�� ��ư �׸� ����
	SMenu[MN_COMBAT_SKILL_POINT].nField[13].nRectImage = 34;
	SMenu[MN_COMBAT_SKILL_POINT].nField[13].nImageNumber = 33;
	SMenu[MN_COMBAT_SKILL_POINT].nField[14].nRectImage = 35;
	SMenu[MN_COMBAT_SKILL_POINT].nField[14].nImageNumber = 32;
}

void SetCSPIconImageANDnWillDo(const int iGroupNo,const int iForceGroupType)//���õ� �׷쿡 ���� CSP�� ��ư �׸��� nWillDo�� �����Ѵ�
{
	int iGroupType =0;
	if(iForceGroupType)
	{
		iGroupType = iForceGroupType;
	}
	else
	{
		 iGroupType = GetKnowCSGroupType(iGroupNo);
	}
	int iCombatSkillIndex = GetCombatSkillIndexStart(iGroupNo, iGroupType);
	const int iGroup1Start	= iCSCovertSkillID+iCombatSkillIndex-1;//1�� �׷���� �����ϴϱ�
	const int iGroup1End = iGroup1Start+6;
	int iStartField		= 7;//SMenu[MN_COMBAT_SKILL_POINT].nField[7] ���� SC������ �̱� �����̴�
	int i = 0;
	for(i = iGroup1Start; i <iGroup1End; i++)
	{
		SFIELD *f = &SMenu[MN_COMBAT_SKILL_POINT].nField[iStartField];
		if(SCharacterData.SkillId[i])//�ƴ°�
		{
			f->nImageNumber = SkillIdToCombatSkillIconImage(i,true);//i�� ��ų �ѹ��� iCSCovertSkillID�� ���� �����̴ϱ� iCSCovertSkillID�� �����Ѵ�
			f->nRectImage	= SkillIdToCombatSkillIconImage(i,false);//i�� ��ų �ѹ��� iCSCovertSkillID�� ���� �����̴ϱ� iCSCovertSkillID�� �����Ѵ� �̹��� �����ſ� 36 ������ ���ο�Ŷ� +35 �Ѵ�
		}
		else //�𸣴°�
		{
			f->nImageNumber = 73+(iGroupNo-1)*2+(iGroupType-1);
			f->nRectImage	= SkillIdToCombatSkillIconImage(i,false);//i�� ��ų �ѹ��� iCSCovertSkillID�� ���� �����̴ϱ� iCSCovertSkillID�� �����Ѵ� �̹��� �����ſ� 36 ������ ���ο�Ŷ� +35 �Ѵ�
		}
		f->nWillDo = iCombatSkillIndex++;//t_CombatSkill�� �ε��� ���� 1 ���� ���� 0���� Doing�� ���� ������ ��
		iStartField++;
	}

	if( iGroupNo >0)
	{
		for(int xx = 0; xx < 12;xx++)
		{
			const int iStartCSIndex = ((iGroupNo-1) * 12 +1);
			SMenu[MN_COMBAT_SKILL_POINT].nField[18+xx].nWillDo	=iStartCSIndex+xx;//���� �κп� �ø��°� FT_CS_EXPLAIN ��nWillDo ����
		}
	}
}

void CallCombatSkillPointMenu()
{
	if(!SMenu[MN_COMBAT_SKILL_POINT].bActive)
	{
		CloseAllMenu();
		SMenu[MN_COMBAT_SKILL_POINT].bActive = true;	
	}
	else
	{
		SMenu[MN_COMBAT_SKILL_POINT].bActive = false;
	}

	if(0 >= Hero->hp)
	{
		CloseAllMenu();
		return;
	}
	const int iDefaultGroup =1;
	InitCombatSkill();
	SMenu[MN_COMBAT_SKILL_POINT].work = iDefaultGroup;//work�� ���� ���õ� �׷찪�� ����ִ�
	SetCSPButtonType(iDefaultGroup);//���� 3�� �ִ� �׷� ��ư�� �����°� �� ��ư�� nWillDo ���� ��ư�� ���� �׷� ���̴�
	SetCSPGroupButtonImage(iDefaultGroup);//�׷� ��ư �̹����� ���� �Ѵ�//SkillId �� ������ �̴°Ŵ�
	SetCSPIconImageANDnWillDo(iDefaultGroup);//����ü�̽��� �ߴ� 6���� ������ ��ư �̹����� nWillDo ����

//	CallSmallMenu(MN_COMBAT_SKILL_POINT);
}

int iCSReservPoint = 10;
//Ÿ���� ���� ���� �� ����?
void SendCSLvUp(const int iCombatSkillIndex)//iSkillNo(1~6)
{
	if( !iCombatSkillIndex ) 
	{
#ifdef _DEBUG // finito 060507
		if( GetSysInfo(SI_GAME_MAKE_MODE ) )		// 020925 YGI
			AddCurrentStatusMessage(255,255,255,"������ų �ε����� �̻��մϴ� ");
#endif
		return;
	}
	
  if (iCSReservPoint > 0)
  { 
    g_mgrBattle.SendCombatLearn(iCombatSkillIndex + 214, Hero);
  } 
}

void InitCombatSkill()
{
  memset(CombatSkill,0,sizeof(t_CombatSkill)*MAX_COMBAT_SKILL_STRUCT);
	int iCSN = 1;
	for(int i = 1; i < 36+1 ; i++)
	{	//1-6	7-12		//13-18	19-24		//25-30 31-36
		CombatSkill[i].iGroup			=	(i-1)/12 +1		;			//���� ��ų �׷�
		CombatSkill[i].iGroupType		=	(i-1)%12 /6 +1		;		//���� ��ų Ÿ��
		CombatSkill[i].iCombatSkillNo	=	iCSN++										;	//���� ��ų �ѹ�
		CombatSkill[i].iSkillNo			=	iCSCovertSkillID + i - 1				;		//SkillID �� �ѹ�
		CombatSkill[i].iSkillLv			=	SCharacterData.SkillId[CombatSkill[i].iSkillNo];		//���� ��ų ����//SCharacterData.SkilID[iSkillNo]�ϸ� ������ ��
		if(6 < iCSN)
		{
			iCSN=1;
		}
	}

  iCSReservPoint = Hero->aStepInfo[CSP_STEP];
}

/////////////////////////////////////////////////////// ���� ��ų Doing
int iCpMax = 100;
int iCpNow = 50;

int iCpCountMax = 100;
int iCpCountNow = 50;
bool SetCSDFlied(const int iGroupNo, const int iGroupType, const int iStartFlieddNo)
{
	//������ų �׷� Ÿ���� �ִٸ�. CombatSkill�� �˻� �Ͽ�(ex>1~^)������ �ְ� ������ ��Ƽ�� �Ѵ�
	int iFNo = iStartFlieddNo;//�����ʵ� �ѹ� �ʵ带 ���������� ��ġ �Ұ�
	int iCSIndex = GetCombatSkillIndexStart(iGroupNo, iGroupType);
	bool bForceNoneType = false;
	bool iReturnValue = false;//���ϵǴ� ��. �ʵ� �ϳ��� ���� �Ǹ� ���� �ʵ�� �δ�
	if( !iGroupType)
	{
		bForceNoneType = true;
	}
	if (iCSIndex < 0) iCSIndex = 0;
	const int iCSIndexTarget = iCSIndex+6;
	//Ŭ���� �� �ʵ� ��°� ���� ����.

	SMenu[MN_COMBAT_SKILL_DOING].nField[iStartFlieddNo].nWillDo = 0;
	SMenu[MN_COMBAT_SKILL_DOING].nField[iStartFlieddNo+1].nWillDo = 0;
	SMenu[MN_COMBAT_SKILL_DOING].nField[iStartFlieddNo+2].nWillDo = 0;

	SMenu[MN_COMBAT_SKILL_DOING].nField[iStartFlieddNo+3].nWillDo = 0;
	SMenu[MN_COMBAT_SKILL_DOING].nField[iStartFlieddNo+4].nWillDo = 0;
	SMenu[MN_COMBAT_SKILL_DOING].nField[iStartFlieddNo+5].nWillDo = 0;
	
	for(; iCSIndex<iCSIndexTarget ;iCSIndex++)
	{
		SFIELD *f = &SMenu[MN_COMBAT_SKILL_DOING].nField[iFNo];
//    if(	iCSIndex != (iCSIndexTarget-1) 
//		|| 
//			(iGroupNo==3 && iGroupType ==2) )//5���� �ƴϸ� �׸��� 3�� �׷쿡 2�� Ÿ��( �ٶ� �̸� �׸��� )
//		{
		  if(CombatSkill[iCSIndex].iSkillLv&&(bForceNoneType == 0))//���Ŵ�.. 1�� �̻��̴ϱ�
		  {
			  f->nImageNumber = SkillIdToCombatSkillIconImage(CombatSkill[iCSIndex].iSkillNo,true);
			  f->nRectImage	= SkillIdToCombatSkillIconImage(CombatSkill[iCSIndex].iSkillNo,false);
			  SetRect(f->rCheakBox, f->x, f->y, f->x+42, f->y+42);
			  f->nWillDo = iCSIndex;
			  iFNo++;//���� �ʵ��
			  iReturnValue = true;
      }
//    }
		else//���� �ִ� ��ų �ƴϸ� ������ nWillDo�� 0���� ����
		{
			f->nWillDo = 0;
			SetRect(f->rCheakBox, 0, 0, 0, 0);
		}
	}
	return iReturnValue;
}

void SetCSDIconRectAndnWillDo()
{
	int iSkillId = iCSCovertSkillID;
	int iFieldNo = 17;//�ʵ� �ѹ� �б�
	int iGroupNo = 1;

//	( SetCSDFlied(1,GetKnowCSGroupType(1),17) );
//	( SetCSDFlied(2,GetKnowCSGroupType(2),17) );
//	( SetCSDFlied(3,GetKnowCSGroupType(3),5) );

	for ( iGroupNo =1; iGroupNo<4;iGroupNo++)
	{
		if( SetCSDFlied(iGroupNo,GetKnowCSGroupType(iGroupNo),iFieldNo) )
		{
			iFieldNo-=6;
		}
	}
}

void SetSCDMainAndOtherClear(const int iCombatSkillIndex)
{	
	SMenu[MN_COMBAT_SKILL_DOING].nField[1].nWillDo = 0;
	SMenu[MN_COMBAT_SKILL_DOING].work = 0;//�������� ��� ���̴�
	for(int iFNo=5 ; iFNo <=22 ;iFNo++)//�ʵ����� Ŭ���� (Ŭ�� ���� ���)
	{
		SFIELD *f = &SMenu[MN_COMBAT_SKILL_DOING].nField[iFNo];
		f->nWillDo = 0;
		SetRect(f->rCheakBox, 0, 0, 0, 0);//
	}
	if(!iCombatSkillIndex)
	{
		return;
	}//��ų �ε����� �̷��� �ȵ���
	SMenu[MN_COMBAT_SKILL_DOING].nField[1].nWillDo = iCombatSkillIndex;//nWillDo ����
	SMenu[MN_COMBAT_SKILL_DOING].work = 2;//�������� ��� ���̴�
}

bool IsSelectCombat()
{	//< CSD-TW-030606
	return (SMenu[MN_COMBAT_SKILL_DOING].nField[1].nWillDo == 0) ? false:true;
}	//> CSD-TW-030606

void CallCombatSkillDoingMenu()
{ //������ ���ϴ�
	if(!SMenu[MN_COMBAT_SKILL_DOING].bActive)
	{
		SMenu[MN_COMBAT_SKILL_DOING].bActive = true;	
	}
  /*
	else
	{
		SMenu[MN_COMBAT_SKILL_DOING].bActive = true;
	}
	InitCombatSkill();
	SetCSDIconRectAndnWillDo();
	SetSCDMainAndOtherClear(0);
  */
}

int iCSPMax = 0;
int iCSPNow	= 0;

int iCSPCMax = 0;
int iCSPCNow = 0;

void CSDMainIconClear()
{	//< CSD-TW-030627
	g_mgrBattle.SendCombatClear();
}	//> CSD-TW-030627

// 輔助函式：從流中讀取到下一個 Tab 符號
bool ReadToken(std::stringstream& ss, std::string& token_str) {
    if (!std::getline(ss, token_str, '\t')) {
        // 處理流的末尾，確保最後一個 token 也能被正確讀取
        if (ss.eof() && !token_str.empty()) {
            return true;
        }
        return false;
    }
    return true;
}

// ----------------------------------------------------------------------------
// 1. Combat Skill Explain (戰鬥技能說明)
// ----------------------------------------------------------------------------

inline void ReadCombatSkillExplain(const char* buf, int i)
{
    std::stringstream ss(buf);
    std::string token;

    if (buf == nullptr) return;

    // --- 1. iSkillNo ---
    if (!ReadToken(ss, token)) return;
    const int iIndex = i;

    // [安全修正] 邊界檢查
    if (iIndex < 0 || iIndex >= MAX_COMBAT_SKILL_EXPLAIN) return;

    CombatSkillExplain[iIndex].iSkillNo = atoi(token.c_str());

    // --- 2. EngName ---
    if (!ReadToken(ss, token)) return;
    strncpy(CombatSkillExplain[iIndex].EngName, token.c_str(), 30);
    CombatSkillExplain[iIndex].EngName[30] = '\0';
    EatRearWhiteChar(CombatSkillExplain[iIndex].EngName);

    // --- 3. LocalName (Skipped logic) ---
    // 依據您提供的原始註解，這裡原本是被註解掉的，但也許資料流裡有這個欄位
    // 這裡我們假設資料流確實有一個欄位需要跳過，以維持後續欄位對齊
    // if (!ReadToken(ss, token)) return;

    // --- 4. ClassName ---
    if (!ReadToken(ss, token)) return;
    strncpy(CombatSkillExplain[iIndex].ClassName, token.c_str(), 50);
    CombatSkillExplain[iIndex].ClassName[50] = '\0';
    EatRearWhiteChar(CombatSkillExplain[iIndex].ClassName);

    // --- 5. WeaponTactic ---
    if (!ReadToken(ss, token)) return;
    strncpy(CombatSkillExplain[iIndex].WeaponTactic, token.c_str(), 50);
    CombatSkillExplain[iIndex].WeaponTactic[50] = '\0';
    EatRearWhiteChar(CombatSkillExplain[iIndex].WeaponTactic);

    // --- 6-8. iNeedLv[3] ---
    for (int x = 0; x < 3; ++x) {
        if (!ReadToken(ss, token)) return;
        CombatSkillExplain[iIndex].iNeedLv[x] = atoi(token.c_str());
    }

    // --- 9-11. iLvCp[3] ---
    for (int x = 0; x < 3; ++x) {
        if (!ReadToken(ss, token)) return;
        CombatSkillExplain[iIndex].iLvCp[x] = atoi(token.c_str());
    }

    // --- 12-14. MicroExplain[3] ---
    for (int xx = 0; xx < 3; xx++)
    {
        if (!ReadToken(ss, token)) return;
        strncpy(CombatSkillExplain[iIndex].MicroExplain[xx], token.c_str(), 50);
        CombatSkillExplain[iIndex].MicroExplain[xx][50] = '\0';
        EatRearWhiteChar(CombatSkillExplain[iIndex].MicroExplain[xx]);
    }

    // --- 15. Use ---
    if (!ReadToken(ss, token)) return;
    strncpy(CombatSkillExplain[iIndex].Use, token.c_str(), 70);
    CombatSkillExplain[iIndex].Use[70] = '\0';
    EatRearWhiteChar(CombatSkillExplain[iIndex].Use);

    // --- 16. Explain ---
    if (!ReadToken(ss, token)) return;
    strncpy(CombatSkillExplain[iIndex].Explain, token.c_str(), 100);
    CombatSkillExplain[iIndex].Explain[100] = '\0';
    EatRearWhiteChar(CombatSkillExplain[iIndex].Explain);

    // --- 17. Convert ---
    ConvertSpecialWord(CombatSkillExplain[iIndex].Explain, "\\n", '\n');
    ConvertSpecialWord(CombatSkillExplain[iIndex].Explain, "\\t", '\t');
    ConvertSpecialWord(CombatSkillExplain[iIndex].Explain, "\\r", '\r');
}

bool LoadCombatSkillExplain(int lang)
{
    // [Android] 路徑修正
    std::string ext = (lang > 0) ? "ch" : "en";
    char filenameBin[MAX_FILENAME];
    snprintf(filenameBin, MAX_FILENAME, "data/CombatSkillExplain.bin.%s", ext.c_str());

    // ---------------------------------------------------------
    // 優先讀取 BIN
    // ---------------------------------------------------------
    SDL_RWops* fp = SDL_RWFromFile(filenameBin, "rb");
    if (fp)
    {
        SDL_RWread(fp, CombatSkillExplain, sizeof(t_CombatSkillExplain) * MAX_COMBAT_SKILL_EXPLAIN, 1);
        SDL_RWclose(fp);
        return true;
    }

    // ---------------------------------------------------------
    // 讀取 TXT (僅限 PC Debug)
    // ---------------------------------------------------------
#if defined(_DEBUG) && !defined(__ANDROID__)
    const char* filenameTxt = "data/CombatSkillExplain.txt";
    fp = SDL_RWFromFile(filenameTxt, "rb");

    if (fp)
    {
       Sint64 size = SDL_RWsize(fp);
       std::string content; content.resize(size);
       SDL_RWread(fp, &content[0], size, 1);
       SDL_RWclose(fp);

       std::stringstream ss(content);
       std::string line;
       char buf[1024];
       int i = 0;

       while(std::getline(ss, line)) {
          // 移除 \r
          if (!line.empty() && line.back() == '\r') line.pop_back();

          if (line.empty() || line[0] == ';') continue;
          if (i >= MAX_COMBAT_SKILL_EXPLAIN - 1) break;

          strncpy(buf, line.c_str(), 1023);
          buf[1023] = '\0';

          ReadCombatSkillExplain(buf, i);
          i++;
       }

       // 寫入 BIN
       SDL_RWops* fpOut = SDL_RWFromFile(filenameBin, "wb");
       if (fpOut) {
          SDL_RWwrite(fpOut, CombatSkillExplain, sizeof(t_CombatSkillExplain) * MAX_COMBAT_SKILL_EXPLAIN, 1);
          SDL_RWclose(fpOut);
       }
       return true;
    }
#endif

    SDL_Log("Error: Failed to load CombatSkillExplain (Bin not found)");
    return false;
}

// ----------------------------------------------------------------------------
// 2. Combat Skill Group Explain (戰鬥技能群組說明)
// ----------------------------------------------------------------------------

inline void ReadCombatSkillGroupExplain(const char* buf, int i)
{
    std::stringstream ss(buf);
    std::string token;

    if (buf == nullptr) return;

    // --- 1. iNo ---
    if (!ReadToken(ss, token)) return;
    const int iIndex = i;

    if (iIndex < 0 || iIndex >= MAX_COMBAT_SKILL_GROUP_EXPLAIN) return;

    CombatSkillGroupExplain[iIndex].iNo = atoi(token.c_str());

    // --- 2. iGroupNo ---
    if (!ReadToken(ss, token)) return;
    CombatSkillGroupExplain[iIndex].iGroupNo = atoi(token.c_str());

    // --- 3. iUnknown1 (Overwrites iGroupNo) ---
    if (!ReadToken(ss, token)) return;
    CombatSkillGroupExplain[iIndex].iGroupNo = atoi(token.c_str());

    // --- 4. EngName ---
    if (!ReadToken(ss, token)) return;
    strncpy(CombatSkillGroupExplain[iIndex].EngName, token.c_str(), 30);
    CombatSkillGroupExplain[iIndex].EngName[30] = '\0';
    EatRearWhiteChar(CombatSkillGroupExplain[iIndex].EngName);

    // --- 5. LocalName (Skipped field 1) ---
    if (!ReadToken(ss, token)) return;

    // --- 6. iUnknown2 (Skipped field 2) ---
    if (!ReadToken(ss, token)) return;

    // --- 7-12. iSkillId[6] ---
    for (int x = 0; x < 6; ++x) {
        if (!ReadToken(ss, token)) return;
        CombatSkillGroupExplain[iIndex].iSkillId[x] = atoi(token.c_str());
    }

    // --- 13. Explain ---
    if (!ReadToken(ss, token)) return;

    // Remove \r, \n from Explain
    token.erase(std::remove(token.begin(), token.end(), '\r'), token.end());
    token.erase(std::remove(token.begin(), token.end(), '\n'), token.end());

    strncpy(CombatSkillGroupExplain[iIndex].Explain, token.c_str(), 100);
    CombatSkillGroupExplain[iIndex].Explain[100] = '\0';
    EatRearWhiteChar(CombatSkillGroupExplain[iIndex].Explain);

    // --- 14. Convert ---
    ConvertSpecialWord(CombatSkillGroupExplain[iIndex].Explain, "\\n", '\n');
    ConvertSpecialWord(CombatSkillGroupExplain[iIndex].Explain, "\\t", '\t');
    ConvertSpecialWord(CombatSkillGroupExplain[iIndex].Explain, "\\r", '\r');
}

bool LoadCombatSkillGroupExplain()
{
    // [Android] 路徑修正
    const char* filenameBin = "data/CombatSkillGroupExplain.bin";

    // ---------------------------------------------------------
    // 優先讀取 BIN
    // ---------------------------------------------------------
    SDL_RWops* fp = SDL_RWFromFile(filenameBin, "rb");
    if (fp)
    {
        SDL_RWread(fp, CombatSkillGroupExplain, sizeof(t_CombatSkillGroupExplain) * MAX_COMBAT_SKILL_GROUP_EXPLAIN, 1);
        SDL_RWclose(fp);
        return true;
    }

    // ---------------------------------------------------------
    // 讀取 TXT (僅限 PC Debug)
    // ---------------------------------------------------------
#if defined(_DEBUG) && !defined(__ANDROID__)
    const char* filenameTxt = "data/CombatSkillGroupExplain.txt";
    fp = SDL_RWFromFile(filenameTxt, "rb");

    if (fp)
    {
       Sint64 size = SDL_RWsize(fp);
       std::string content; content.resize(size);
       SDL_RWread(fp, &content[0], size, 1);
       SDL_RWclose(fp);

       std::stringstream ss(content);
       std::string line;
       char buf[1024];
       int i = 0;

       while(std::getline(ss, line)) {
          if (!line.empty() && line.back() == '\r') line.pop_back();

          if (line.empty() || line[0] == ';') continue;
          if (i > MAX_COMBAT_SKILL_GROUP_EXPLAIN - 1) break;

          strncpy(buf, line.c_str(), 1023);
          buf[1023] = '\0';

          ReadCombatSkillGroupExplain(buf, i);
          i++;
       }

       // 寫入 BIN
       SDL_RWops* fpOut = SDL_RWFromFile(filenameBin, "wb");
       if (fpOut) {
          SDL_RWwrite(fpOut, CombatSkillGroupExplain, sizeof(t_CombatSkillGroupExplain) * MAX_COMBAT_SKILL_GROUP_EXPLAIN, 1);
          SDL_RWclose(fpOut);
       }
       return true;
    }
#endif

    SDL_Log("Error: Failed to load CombatSkillGroupExplain (Bin not found)");
    return false;
}

void DrawRareExplain(const int iExplainX, const int iExplainY, const int iRareIndex, const int iRareType)
{
	int iCount = 0;
	const int iLineGab = 24;
	const int trans = 20;
	const int iWidth = 130;

	Spr *back_spr = GetSprOfMenu( MAIN_ETC, 55 );
	SetHangulAlign( TA_LEFT);

	char *txt;
	
	switch(iRareType)
	{
	case 0:
		{
			txt = ItemRare[iRareIndex].Explain;
		}break;
	case 2:
	case 3:
	case 4:
		{
			txt = ItemFunction[iRareIndex].Exp;
		}break;
	default:
		{
			return;
		}break;
	}
	
	const int line	= GetLineRectTextPut( iWidth,txt );

	for( int i = 0; i<line; i++ )
	{
		FieldTypeNomalPutFx( iExplainX, iExplainY+iLineGab*(iCount+i), 0,0, 71, 10, 1 );
	}

	Hcolor( FONT_COLOR_YELLOW );
	RectTextPut(iExplainX+8,iExplainY+8,iWidth,txt); 
}

void DrawCombatSkillExplain(const int iExplainX, const int iExplainY, const int iCSIndex)
{
	SetHangulAlign(TA_LEFT);
	const int iWidth = TXT_BG_NORMAL_WIDTH;
	const int SKILL_START = 115;
	const int iCSkillIndex = CombatSkill[iCSIndex].iSkillNo - iCSCovertSkillID;
	int iCount = 0;
	int iReultLineCount = 0;
	
	int iBlankGab = 0;
	int iReultBlankSize = 0;
	
	const int iLineGab = TxtOut.GetLineGab();
	const int iBlankGabSize = TxtOut.GetBlankGabSize();
	
	LPCombatSkillExplain pCSE  = &CombatSkillExplain[iCSkillIndex];

	//����׸�
	TxtOut.Init(COLOR_BLACK,TXT_BG_NORMAL_WIDTH);
	TxtOut.PutTextHeaderBGImg(iExplainX,iExplainY+iBlankGab+iLineGab*iCount,iWidth);

	//������ų �̸�
	Hcolor(FONT_COLOR_RARE_MAIN);
	iReultLineCount = TxtOut.RcTXTOut(iExplainX,iExplainY+iBlankGab+iLineGab*iCount,iWidth,0,lan->OutputMessage(0,411),pCSE->EngName);
	iCount += iReultLineCount;

	iReultBlankSize = TxtOut.RcBlankOut(iExplainX,iExplainY+iBlankGab+iLineGab*iCount,iWidth,1);
	iBlankGab+=iReultBlankSize;

	//Class ���� + //Ŭ���� ���� �̸�
	Hcolor(FONT_COLOR_BLIGHT_GREEN2);
	iReultLineCount = TxtOut.RcTXTOut(iExplainX,iExplainY+iBlankGab+iLineGab*iCount,iWidth,0,lan->OutputMessage(0,421),pCSE->ClassName);
	Hcolor(FONT_COLOR_RARE_MAIN);
	RectTextPut(iExplainX,iExplainY+iBlankGab+iLineGab*iCount,iWidth,lan->OutputMessage(0,420));
	iCount += iReultLineCount;

	//��ƽ ���� �̸�
	Hcolor(FONT_COLOR_BLIGHT_GREEN2);
	iReultLineCount = TxtOut.RcTXTOut(iExplainX,iExplainY+iBlankGab+iLineGab*iCount,iWidth,8,lan->OutputMessage(0,417),pCSE->WeaponTactic);
	iCount += iReultLineCount;
	
	iReultBlankSize = TxtOut.RcBlankOut(iExplainX,iExplainY+iBlankGab+iLineGab*iCount,iWidth,1);
	iBlankGab+=iReultBlankSize;

	//�׳� ����
	Hcolor(FONT_COLOR_WHITE);
	iReultLineCount = TxtOut.RcTXTOut(iExplainX,iExplainY+iBlankGab+iLineGab*iCount,iWidth,0,pCSE->Explain);
	iCount += iReultLineCount;

	iReultBlankSize = TxtOut.RcBlankOut(iExplainX,iExplainY+iBlankGab+iLineGab*iCount,iWidth,1);
	iBlankGab+=iReultBlankSize;

	if(1)//Ŭ���� ���� ���� �ϸ�
	{	//How To Use
	//	Hcolor(FONT_COLOR_RARE_MAIN);
		Hcolor(FONT_COLOR_BLIGHT_ORANGE);
		iReultLineCount = TxtOut.RcTXTOut(iExplainX,iExplainY+iBlankGab+iLineGab*iCount,iWidth,0,lan->OutputMessage(0,418));
		iCount += iReultLineCount;
		//�����
		Hcolor(FONT_COLOR_BLIGHT_ORANGE);
		iReultLineCount = TxtOut.RcTXTOut(iExplainX,iExplainY+iBlankGab+iLineGab*iCount,iWidth,8,pCSE->Use);
		iCount += iReultLineCount;
		
		iReultBlankSize = TxtOut.RcBlankOut(iExplainX,iExplainY+iBlankGab+iLineGab*iCount,iWidth,1);
		iBlankGab+=iReultBlankSize;

		if (0 > iCSkillIndex || iCSkillIndex + SKILL_START >= 200) return;

		const int iSkillLv = SCharacterData.SkillId[iCSkillIndex + SKILL_START];
	
		Hcolor(FONT_COLOR_RARE_MAIN);
		if(iSkillLv)//
		{
			iReultLineCount = TxtOut.RcTXTOut(iExplainX,iExplainY+iBlankGab+iLineGab*iCount,iWidth,0,lan->OutputMessage(0,412),iSkillLv);
			iCount += iReultLineCount;	
			Hcolor(FONT_COLOR_RARE_MAIN);
			iReultLineCount = TxtOut.RcTXTOut(iExplainX,iExplainY+iBlankGab+iLineGab*iCount,iWidth,8,lan->OutputMessage(0,413),pCSE->iLvCp[iSkillLv-1]);
			iCount += iReultLineCount;
			//������� ������ ���� ���
			iReultLineCount = TxtOut.RcTXTOut(iExplainX,iExplainY+iBlankGab+iLineGab*iCount,iWidth,8,pCSE->MicroExplain[iSkillLv-1]);//Micro explain//�迭�̱� ������ iSkillLv+1 �� �ش�
			iCount += iReultLineCount;
			
			iReultBlankSize = TxtOut.RcBlankOut(iExplainX,iExplainY+iBlankGab+iLineGab*iCount,iWidth,1);
			iBlankGab+=iReultBlankSize;
		}
		else//���� �� �������
		{
			iReultLineCount = TxtOut.RcTXTOut(iExplainX,iExplainY+iBlankGab+iLineGab*iCount,iWidth,0,lan->OutputMessage(0,414));
			iCount += iReultLineCount;
				
			iReultBlankSize = TxtOut.RcBlankOut(iExplainX,iExplainY+iBlankGab+iLineGab*iCount,iWidth,1);
			iBlankGab+=iReultBlankSize;
		}

		if(3 > iSkillLv)//���� ���� ���� ����//���������� 3���� �۰� 0 ���� Ŀ�� �Ѵ�// �ֳ� . ���� ���� �̴ϱ�
		{
			//Next Lv
			Hcolor(FONT_COLOR_SOLID_RED);
			iReultLineCount = TxtOut.RcTXTOut(iExplainX,iExplainY+iBlankGab+iLineGab*iCount,iWidth,0,lan->OutputMessage(0,415),iSkillLv+1);//Next Level
			iCount += iReultLineCount;

			//Need Lv
			Hcolor(FONT_COLOR_SOLID_RED);
			iReultLineCount = TxtOut.RcTXTOut(iExplainX,iExplainY+iBlankGab+iLineGab*iCount,iWidth,8,lan->OutputMessage(0,416),pCSE->iNeedLv[iSkillLv]);//Require 
			iCount += iReultLineCount;

			if((iSkillLv+1) > SCharacterData.SkillId[iCSkillIndex])
			Hcolor(FONT_COLOR_SOLID_RED);
			iReultLineCount = TxtOut.RcTXTOut(iExplainX,iExplainY+iBlankGab+iLineGab*iCount,iWidth,8,pCSE->MicroExplain[iSkillLv]);//Micro explain
			iCount += iReultLineCount;

		}
	}
	else //Ŭ���� �ٸ���
	{
		Hcolor(FONT_COLOR_RED);
		iReultLineCount = TxtOut.RcTXTOut(iExplainX,iExplainY+iBlankGab+iLineGab*iCount,iWidth,0,lan->OutputMessage(0,422));
		iCount += iReultLineCount;
	}
	//�����׸�
	TxtOut.PutTextTailBGImg(iExplainX,iExplainY+iBlankGab+iLineGab*iCount,iWidth);
}

void DrawCombatSkillGroupExplain(const int iExplainX, const int iExplainY, const int iGroupNo, const int iGroupType)
{
	SetHangulAlign(TA_LEFT);
	const int iWidth = TXT_BG_SHORT_WIDTH;
	int iIndex = (iGroupNo-1)*2 +(iGroupType);
	int iCount = 0;
	const int iLineGab = TxtOut.GetLineGab();
	int iReultLineCount = 0;
	int iBlankGab = 0;
	const int iBlankGabSize = 3;

	LPCombatSkillGroupExplain pCSGE  = &CombatSkillGroupExplain[iIndex];

	//����׸�
	TxtOut.Init(COLOR_BLACK,TXT_BG_SHORT_WIDTH);
	TxtOut.PutTextHeaderBGImg(iExplainX,iExplainY+iBlankGab+iLineGab*iCount,iWidth);

	Hcolor(FONT_COLOR_GREEN);
	iReultLineCount = TxtOut.RcTXTOut( iExplainX, iExplainY+iLineGab*iCount,iWidth,0, "< %s >", pCSGE->EngName);  //�ٰ����� 15�̴�
	iCount+=iReultLineCount;

	Hcolor(FONT_COLOR_WHITE);
	iReultLineCount = TxtOut.RcTXTOut( iExplainX, iExplainY+iLineGab*iCount, iWidth,0, pCSGE->Explain);
	iCount+=iReultLineCount;

	//����׸�
	TxtOut.PutTextTailBGImg(iExplainX,iExplainY+iBlankGab+iLineGab*iCount,iWidth);
}
void RareGroupSet()
{
    int i1 = 1,i2 = 1,	i3 = 1,	i4 = 1;
    for (;i1 < MAX_RARE_GROUP;i1++)
    {
        const int iGroupSet = RareGroup[1][i1].group_buffer_10;
        switch(iGroupSet)
        {
            case 2:
            {
                RareGroup[iGroupSet][i2] = RareGroup[1][i1];
                i2++;
            }break;
            case 3:
            {
                RareGroup[iGroupSet][i3] = RareGroup[1][i1];
                i3++;
            }break;
            case 4:
            {
                RareGroup[iGroupSet][i4] = RareGroup[1][i1];
                i4++;
            }break;
            default:
                continue;
                break;
        }

    }
}

// ============================================================================
// 1. 解析函式
// ============================================================================
void ReadFunctionGroup(char * buf, int i)
{
    char *token;

    token = strtok(buf, "\t\n\r");
    if( token == NULL ) return;

    int iIndex = atoi(token);

    // [安全修正] 邊界檢查
    if (iIndex < 0 || iIndex >= MAX_RARE_GROUP) {
        SDL_Log("Error: ReadFunctionGroup Index out of bounds: %d", iIndex);
        return;
    }

    RareGroup[1][iIndex].no = iIndex;

    // Helper lambda to safely copy string and eat whitespace
    auto SafeCopy = [](char* dest, char* src, size_t size) {
        strncpy(dest, src, size - 1);
        dest[size - 1] = '\0';
        EatRearWhiteChar(dest);
    };

    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; SafeCopy(RareGroup[1][iIndex].name, token, 39);

    // RareSet 0
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; SafeCopy(RareGroup[1][iIndex].rareset[0].rare, token, 39);
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; RareGroup[1][iIndex].rareset[0].rare_num       = atoi(token);
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; RareGroup[1][iIndex].rareset[0].rare_rate   = atoi(token);

    // RareSet 1
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; SafeCopy(RareGroup[1][iIndex].rareset[1].rare, token, 39);
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; RareGroup[1][iIndex].rareset[1].rare_num       = atoi(token);
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; RareGroup[1][iIndex].rareset[1].rare_rate   = atoi(token);

    // RareSet 2
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; SafeCopy(RareGroup[1][iIndex].rareset[2].rare, token, 39);
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; RareGroup[1][iIndex].rareset[2].rare_num       = atoi(token);
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; RareGroup[1][iIndex].rareset[2].rare_rate   = atoi(token);

    // RareSet 3
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; SafeCopy(RareGroup[1][iIndex].rareset[3].rare, token, 39);
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; RareGroup[1][iIndex].rareset[3].rare_num       = atoi(token);
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; RareGroup[1][iIndex].rareset[3].rare_rate   = atoi(token);

    // RareSet 4
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; SafeCopy(RareGroup[1][iIndex].rareset[4].rare, token, 39);
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; RareGroup[1][iIndex].rareset[4].rare_num       = atoi(token);
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; RareGroup[1][iIndex].rareset[4].rare_rate   = atoi(token);

    // RareSet 5
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; SafeCopy(RareGroup[1][iIndex].rareset[5].rare, token, 39);
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; RareGroup[1][iIndex].rareset[5].rare_num       = atoi(token);
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; RareGroup[1][iIndex].rareset[5].rare_rate   = atoi(token);

    // RareSet 6
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; SafeCopy(RareGroup[1][iIndex].rareset[6].rare, token, 39);
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; RareGroup[1][iIndex].rareset[6].rare_num       = atoi(token);
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; RareGroup[1][iIndex].rareset[6].rare_rate   = atoi(token);

    // RareSet 7
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; SafeCopy(RareGroup[1][iIndex].rareset[7].rare, token, 39);
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; RareGroup[1][iIndex].rareset[7].rare_num       = atoi(token);
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; RareGroup[1][iIndex].rareset[7].rare_rate   = atoi(token);

    // Group Buffers
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; memcpy(RareGroup[1][iIndex].group_buffer_1  ,token, 39);
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; memcpy(RareGroup[1][iIndex].group_buffer_2  ,token, 39);
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; memcpy(RareGroup[1][iIndex].group_buffer_3  ,token, 39);

    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; RareGroup[1][iIndex].group_buffer_4    = atoi(token);
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; RareGroup[1][iIndex].group_buffer_5    = atoi(token);
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; RareGroup[1][iIndex].group_buffer_6    = atoi(token);
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; RareGroup[1][iIndex].group_buffer_7    = atoi(token);
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; RareGroup[1][iIndex].group_buffer_8    = atoi(token);
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; RareGroup[1][iIndex].group_buffer_9    = atoi(token);
    token = strtok(NULL,"\t\n\r"); if( token == NULL ) return; RareGroup[1][iIndex].group_buffer_10   = atoi(token);
}

// ============================================================================
// 2. 讀取函式
// ============================================================================
bool LoadFunctionGroup()
{
    // [Android] 路徑修正
    const char* filenameBin = "Itemtable/function_group.bin";

    // ---------------------------------------------------------
    // 優先讀取 BIN (Android 唯一路徑)
    // ---------------------------------------------------------
    SDL_RWops* fp = SDL_RWFromFile(filenameBin, "rb");
    if (fp)
    {
        // 依照原始邏輯，連續讀取 4 個 Group (1, 2, 3, 4)
        SDL_RWread(fp, RareGroup[1], sizeof(t_raregroup) * MAX_RARE_GROUP, 1);
        SDL_RWread(fp, RareGroup[2], sizeof(t_raregroup) * MAX_RARE_GROUP, 1);
        SDL_RWread(fp, RareGroup[3], sizeof(t_raregroup) * MAX_RARE_GROUP, 1);
        SDL_RWread(fp, RareGroup[4], sizeof(t_raregroup) * MAX_RARE_GROUP, 1);

        SDL_RWclose(fp);
        return true;
    }

    // ---------------------------------------------------------
    // 讀取 TXT (僅限 PC Debug)
    // ---------------------------------------------------------
#if defined(_DEBUG) && !defined(__ANDROID__)
    const char* filenameTxt = "itemtable/function_group.txt";
    fp = SDL_RWFromFile(filenameTxt, "rb");

    if (fp)
    {
       Sint64 size = SDL_RWsize(fp);
       std::string content; content.resize(size);
       SDL_RWread(fp, &content[0], size, 1);
       SDL_RWclose(fp);

       std::stringstream ss(content);
       std::string line;
       char buf[1024];
       int i = 0;

       while(std::getline(ss, line)) {
          // 移除 \r
          if (!line.empty() && line.back() == '\r') line.pop_back();

          if (line.empty() || line[0] == ';') continue;
          if (i > MAX_RARE_GROUP - 1) break;

          strncpy(buf, line.c_str(), 1023);
          buf[1023] = '\0';

          ReadFunctionGroup(buf, i);
          i++;
       }

       // 執行後處理 (原代碼邏輯)
       RareGroupSet();

       // 寫入 BIN (僅 PC)
       // 注意：這裡必須寫入 Group 1, 2, 3, 4
       SDL_RWops* fpOut = SDL_RWFromFile(filenameBin, "wb");
       if (fpOut) {
          SDL_RWwrite(fpOut, RareGroup[1], sizeof(t_raregroup) * MAX_RARE_GROUP, 1);
          SDL_RWwrite(fpOut, RareGroup[2], sizeof(t_raregroup) * MAX_RARE_GROUP, 1);
          SDL_RWwrite(fpOut, RareGroup[3], sizeof(t_raregroup) * MAX_RARE_GROUP, 1);
          SDL_RWwrite(fpOut, RareGroup[4], sizeof(t_raregroup) * MAX_RARE_GROUP, 1);
          SDL_RWclose(fpOut);
       }
       return true;
    }
#endif

    SDL_Log("Error: Failed to load FunctionGroup (Bin not found)");
    return false;
}
bool GetItemDurTxt(ItemAttr *pItem, char* szTxt)
{
	CItem *t = ::ItemUnit(pItem->item_no);
	if(!t){return false;}
	const int iItemWeight	= t->GetItemWeight();
	const int iItemKind		= t->GetItemKind();
	if( iItemWeight )
	{
		sprintf( szTxt, lan->OutputMessage(7,153), iItemWeight );
		if( t->GetRButton() == DIVIDE_ITEM )
		{
			sprintf( szTxt, lan->OutputMessage(7,154), szTxt, pItem->attr[IATTR_MUCH] );
		}
		else 
		{
			WORD d_max=0, d_curr=0;
			GetItemDuration( *pItem, d_curr, d_max );
			sprintf( szTxt, lan->OutputMessage(7,155), szTxt, d_curr/10, d_max/10 );
		}
	}
	else
	{
		if( t->GetRButton() == DIVIDE_ITEM )
		{
			if( iItemKind == IK_MONEY )
				sprintf( szTxt, "%d CELL", pItem->attr[IATTR_MUCH] );
			if( iItemKind == IK_NEW_MONEY )
				sprintf( szTxt, "%d CRIT", pItem->attr[IATTR_MUCH] );
			else
				sprintf( szTxt, lan->OutputMessage(7,156), pItem->attr[IATTR_MUCH] );
		}
		else 
		{
			WORD d_max, d_curr;
			GetItemDuration( *pItem, d_curr, d_max );
			sprintf( szTxt, lan->OutputMessage(7,157), d_curr/10, d_max/10 );
		}
	}
	return true;
}

int CheckMerchantOrSkillMaster()
{//011108 lsw
	t_SkillExp *pExp = SCharacterData.nJobSkill;

	if(!::IsAllGM())//�⺻���� All Gm�̾�� �Ѵ�
	{	
		return 0;
	}

	if(!::IsMerchant())//������ �ƴ϶��
	{
		return GM_QUEST_MERCHANT;//������ �ȵǸ� ������ ���κ��� �Ǿ��
	}

	SkillExp3 *pExp3 = (SkillExp3*)&pExp[SKILL_MASTER];

	if(	pExp3->skillType !=GM_QUEST_BLACKSMITH_MASTER	//�������� �⼺
	&&	pExp3->skillType !=GM_QUEST_CARPENTER_MASTER	//��� �⼺    
	&&	pExp3->skillType !=GM_QUEST_BOWCRAFT_MASTER	//Ȱ���ۻ� �⼺
	&&	pExp3->skillType !=GM_QUEST_TAILOR_MASTER)	//��ܻ� �⼺
	{//�ƹ��͵� �⼺�� �ƴ� ���� ���
		return GM_QUEST_BLACKSMITH_MASTER;	//�������� �⼺
	}
	return 0;
}

extern void FreeTestAnimation();

extern int iSelectedCharacter;
extern void RecvCMD_SELECTED_CHAR_RESULT_CLIENT_BOTH_DB_DEMON(t_packet *p);
void CallMN_START_MAP_Menu()
{
#ifdef _DEBUG
	if( SysInfo.notconectserver )
	{
		t_packet p;
		p.u.Hwoa.rang.SelectedCharResult.iKey = 1;
		RecvCMD_SELECTED_CHAR_RESULT_CLIENT_BOTH_DB_DEMON(&p);
		return;
	}
#endif

	iSelectedCharacter = -1;
	StartPosition = 0;
	SCharacterData = SCharSource;
	FreeTestAnimation();//020530 lsw

/*	t_packet packet;
	packet.h.header.type = CMD_SELECTED_CHAR_RESULT_CLIENT_BOTH_DB_DEMON;
	packet.h.header.size = sizeof(SELECTEDCHARRESULT);
	packet.u.Hwoa.rang.SelectedCharResult.iLv  = SCharacterData.nLevel;
	packet.u.Hwoa.rang.SelectedCharResult.iKey  = 0;
	::memcpy(packet.u.Hwoa.rang.SelectedCharResult.szName,SCharacterData.sCharacterName,MAX_NAME);
	::QueuePacket( &packet, 1 );
	y_MenuFocus=MN_START_MAP; finito 060507 - removed just doesn't seem to do anything useful at all...*/
	t_packet p;
	p.u.Hwoa.rang.SelectedCharResult.iKey = 1;
	RecvCMD_SELECTED_CHAR_RESULT_CLIENT_BOTH_DB_DEMON(&p);
}