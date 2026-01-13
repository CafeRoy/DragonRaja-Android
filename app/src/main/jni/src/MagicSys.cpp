// MagicSys.cpp: implementation of the CMagicSys class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MagicSys.h"

#include "Char.h"
#include "Map.h"
#include "Effect.h"
#include "Menu.h"		// 030402 kyo
using namespace battle;

extern char tool_ID_EFFECT_TEST;
extern bool IsAttackAble(LPCHARACTER pTarget);
///////////////////////////////////////////////////////////////////////////////
// Global Member
///////////////////////////////////////////////////////////////////////////////
extern char g_MagicName[MAX_MAGIC_][31];
extern	ItemAttr QuickItemAttr[7];
static POINT s_aPlace[8][5] =
{
	{{ 0, -3}, {-2, -3}, { 2, -3}, {-4, -3}, { 4, -3}},
	{{-2, -2}, {-4,  0}, { 0, -4}, {-6,  2}, { 2, -6}},
	{{-3,  0}, {-3,  2}, {-3, -2}, {-3,  4}, {-3, -4}},
	{{-2,  2}, { 0,  4}, {-4,  0}, { 2,  6}, {-6, -2}},
	{{ 0,  3}, {-2,  3}, { 2,  3}, {-4,  3}, { 4,  3}},
	{{ 2,  2}, { 4,  0}, { 0,  4}, { 6, -2}, {-2,  6}},
	{{ 3,  0}, { 3,  2}, { 3, -2}, { 3,  4}, { 3, -4}},
	{{ 2, -2}, { 0, -4}, { 4,  0}, {-2, -6}, { 6,  2}}
};

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CMagicSys::CMagicSys()
{
	
}

CMagicSys::~CMagicSys()
{
	
}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

int CMagicSys::Action(LPCHARACTER pCaster)
{
	return ACTION_NONE;
}

void CMagicSys::Effect(MAGICLIST* pEffect)
{
	LPCHARACTER pCaster = pEffect->lpChar_Own;
	LPCHARACTER pTarget = pEffect->lpChar_Target;
	if (pCaster == NULL || pTarget == NULL)  return;
	
	const int nMagic = pEffect->magicSerial;
	
	if (pEffect->magicSubNum == 8)
	{
		switch (nMagic)
		{
		case VAMPIRE_POISON:   // �����̾��� ��
		case FEATHER_OF_CURSE: // ������ ����
		case CURSE_OF_HELL:    // ���� ����
			{
				EffectLocal(pEffect, g_ClientTime);
				break;
			}
		case OCTA_LIGHTNING:
		case BLOOD_SPRINKLE:
			{
				break;
			}
		default:
			{
				::InsertMagic(pCaster, pTarget,
					pEffect->next_Effect, 9,
					pEffect->x, pEffect->y,
					pEffect->tarx, pEffect->tary);
				break;
			}
		}
	}
	else
	{
		switch (nMagic)
		{
		case GREAT_THUNDER_BOLT: 
			{ 
				EffectDrop(pEffect, 4, g_ClientTime);
				break;
			}
		case GREAT_METEOR:
			{
				EffectStorm(pEffect, g_ClientTime);
				break;
			}
		case FLYING_SPARK: 
			{ // ������ ����ũ
				EffectShoot(pEffect, 3);
				break;
			}
		case FIRE_EXPLOSION: 
			{ // ȭ������     
				EffectMulti(pEffect, 4);
				break;
			}
		case ICE_STORM:
		case FIRE_STORM: 
			{ // ���̾� ����
				EffectStorm(pEffect, g_ClientTime);
				break;
			}
		case SHADOW_OF_EVIL: 
			{ // ���� �׸���
				EffectSpread(pEffect);
				break;
			}
		case BLITZ_SPEAR: 
			{ // ����â
				EffectShoot(pEffect, 5);
				break;
			}
		case LIGHTNING_CLOUD: 
			{ // ���� ����
				EffectRound(pEffect, g_ClientTime);
				break;
			}
		case ICE_BREATH_PRIEST:
		{ // ���� �극��
			EffectBreath(pEffect, 0, -60);
			break;
		}
		case ICE_BREATH: 
			{ // ���� �극��
				EffectBreath(pEffect, 60, 0);
				break;
			}
		case LIGHTNING_BREATH: 
			{ // ���� �극��
				EffectBreath(pEffect, 30, 0);
				break;
			}
		case FLAME_PILLAR:
			{ // ȭ�� ���

                EffectPillar(pEffect, g_ClientTime, 150, true);
				break;
			}
		case FLAME_POUR:
			{
                EffectPillar(pEffect, g_ClientTime, 150, true);
				break;
			}
		case HOLY_RAGE:
			{ 
				EffectDrop(pEffect, 3, g_ClientTime);
				break;
			}
		case FIRE_BREATH:
		case AREA_PARALYSIS:
		case AREA_POISON:
		case AREA_CONFUSION:
		case AREA_SLOW:
		case AREA_BAT_ATTACK:
			{
				break;
			}
		case DEATH_FIRE:	// 030123 kyo
			{
				EffectMulti(pEffect, 5);
				break;
			}			
		case AUTHORITY_OF_WHOARENCHA: 	// 030415 kyo
			{
				EffectSpreadIrregular(pEffect, g_ClientTime, 5, 7, 2 );	//5Ÿ�ϰ��� 2�� 
				break;
			}
		case ICE_BREATH2: 	// 030415 kyo
			{
				EffectPillar(pEffect, g_ClientTime, 50, true); // 030519 kyo
                EffectPillar(pEffect, g_ClientTime, 100, true);
                EffectPillar(pEffect, g_ClientTime, 150, true);
				break;
			}
		case BLOOD_LIGHTNING:	// 030415 kyo
			{
				EffectMulti(pEffect,3);
				break;
			}
		case FLAME_ARROW:
			{	//< CSD-031020
				EffectMulti(pEffect, 5);
				break;
			}	//> CSD-031020
		default:
			{
				::InsertMagic(pCaster, pTarget,
					pEffect->next_Effect,
					pEffect->magicSubNum,
					pEffect->x, pEffect->y,
					pEffect->tarx, pEffect->tary, 
					false, 0, false, 
					pEffect->magic_Runtime);
				break;
			}
		}
	}
}

bool CMagicSys::Ready(LPCHARACTER pCaster, int nMagic, int nReady)
{	
	switch (pCaster->nCurrentAction)
	{
	case ACTION_ATTACK_BOW:
	case ACTION_THROW:
	case ACTION_SITDOWN:
		{
			return false;
		}
	}
	// �������� ������ ���� ���� ĳ������ ������ ���
	if (g_lpML == NULL || pCaster->ready_Magic == 0)
	{
		if (Hero == pCaster)
		{
			OUTPUT(MK_NORMAL, lan->OutputMessage(1, 14), g_MagicName[nMagic]);
		}
		
		pCaster->excute_MagicNum.PutEncrypted(nMagic);  		      // �غ����� ������ ������ȣ�� ����
		pCaster->ready_Magic = 1;					            // ĳ���� ���� ���� ����
		pCaster->start_Casting  = g_ClientTime + nReady + 3000;
		// �����迭�� ��ȣ�迭�� ������� �� �����ð� �� ������ų ��� �Ұ��� ����
		if (IsContinue(nMagic))
		{
			if (pCaster->continue_RunMagic < g_ClientTime + g_lpMT[nMagic].continue_Time*1000)
			{ 
				pCaster->continue_RunMagic = g_ClientTime + g_lpMT[nMagic].continue_Time*1000;
			} 
		}
		
		pCaster->end_Casting = g_ClientTime + nReady;	// ĳ���� �Ϸ�ð� ����
		// ĳ���� ����
		switch (pCaster->spell)
		{
		case WIZARD_SPELL: 
			{	// �������� ���
				::InsertMagic(pCaster, pCaster, 403, 0, 0, 0, 0, 0); 
				break; 
			}
		case PRIEST_SPELL:	
			{	// �������� ���
				::InsertMagic(pCaster, pCaster, 404, 0, 0, 0, 0, 0);
				break;	
			}
		}
		
		return true;
	}	
	
	if (Hero == pCaster)
	{
		OUTPUT(MK_WARNING, lan->OutputMessage(1, 13));
	}
	
	return false;
}	

bool CMagicSys::Apply(LPCHARACTER pCaster, LPCHARACTER pTarget, int nMagic, int nX, int nY, bool bFail)
{
	if (nMagic != RELIEF_AUTHORITY)	// ������ ���� �ƴϰ�
	{
		if (!IsAttackAble(pTarget))	// �׾� �ְų� ���� ����� ��� 
		{		
			goto MAGIC_FIAL;			// ���� ����
		}
	}
	
	if (pTarget == NULL) pTarget = pCaster;
	// ĳ������ 1ȸ ���� �����ε� ������ ������� �Ҷ� blocking
	if (g_lpMT[nMagic].excute_Type == 1 && pCaster->ready_Magic == 3)
	{
		OUTPUT(MK_WARNING, lan->OutputMessage(1, 15));
		goto MAGIC_FIAL;
	}
	
	if (Hero == pCaster)
	{	//���� Ŭ������Ʈ�� �ܿ� MP�� ������ ����ϴµ� �ʿ��� MP���� ���� ���� ���� ��� 
		if (SCharacterData.nCharacterHP > 0 && SCharacterData.nCharacterMP >= g_lpMT[nMagic].exhaust_MP)
		{
			SCharacterData.nCharacterMP -= g_lpMT[nMagic].exhaust_MP;
			if (SCharacterData.nCharacterMP < 0)  SCharacterData.nCharacterMP = 0;
			pCaster->mp -= g_lpMT[nMagic].exhaust_MP;
			if (pCaster->mp < 0) Hero->mp = 0;
		}
		else 
		{
			switch (SCharacterData.nCharacterData[SPELL])
			{
			case WIZARD_SPELL:
				{	// ��������
					OUTPUT(MK_SHORTAGE, lan->OutputMessage(1, 9));
					break;
				}
			case PRIEST_SPELL:
				{	// �����ڶ��
					OUTPUT(MK_SHORTAGE, lan->OutputMessage(1, 10));
					break;
				}
			}
			
			return false;
		}
	}
	// �����ڰ� ĳ������ ������ ���� �߻� �غ�Ǿ� ������ �Ǵ� �� �׽�Ʈ�� ���� �޴����ý�		
	if ((g_lpMT[nMagic].excute_Type == 0 && pCaster->ready_Magic == 3) || 
		pCaster->ready_Magic == 2 || tool_ID_EFFECT_TEST == 1)
	{
		DIRECTION dir = pCaster->direction;
		
		if ((g_lpMT[nMagic].appoint_Type == 1 && pCaster == pTarget) || g_lpMT[nMagic].appoint_Type == 0)
		{
			dir = pCaster->direction;
		}
		else
		{
			::ChangeDirection(&dir, pCaster->x, pCaster->y, nX, nY); // �ش� Ŭ���������� ���� ��ȯ
		}
		
		switch (pCaster->sprno)
		{ 
		case 83: // �����̾�
		case 86: // ����̵�
			{
				::CharDoAni(pCaster, dir , MON1_BATTLE_MAGIC);
				break;
			}
		default:
			{
				::CharDoAni(pCaster, dir , pCaster->nCurrentAction);
				break;
			}
		}
		
		if (!AddEffect(pCaster, pTarget, nMagic, nX, nY, bFail))
		{
			OUTPUT(MK_WARNING, lan->OutputMessage(1, 29));	
			goto MAGIC_FIAL;
		}
	}
	
	if (Hero == pCaster || Hero == pTarget)
	{
		if (bFail) 
		{	// ������ ������ �����
			OUTPUT(MK_NORMAL, lan->OutputMessage(1, 30), g_MagicName[nMagic]);	
			goto MAGIC_FIAL;
		}
		
		switch (nMagic)
		{		
		case LOW_MONSTER_RECALL:
			{
				OUTPUT(MK_NORMAL, lan->OutputMessage(1, 31), pCaster->name);
				break;	
			}
		case MIDDLE_MONSTER_RECALL:
			{
				OUTPUT(MK_NORMAL, lan->OutputMessage(1, 32), pCaster->name);
				break;
			}
		default:
			{
			/*	if (pCaster == pTarget)
				{
					OUTPUT(MK_NORMAL, lan->OutputMessage(1, 33), pCaster->name, g_MagicName[nMagic]);
				}
				else
				{
					OUTPUT(MK_NORMAL, lan->OutputMessage(1, 34), pCaster->name, g_MagicName[nMagic], pTarget->name);
				}
			*/	
				OUTPUT(MK_NORMAL, lan->OutputMessage(1, 34),
                               pCaster->name, pTarget->name, g_MagicName[nMagic]);
				break;		
			}
		}
	}
	// ���� �غ���¿��� ������������ �ܰ�� ���� ����
	if (pCaster->ready_Magic == 2)
	{
		// �����ð��� �����ϴ� ������ ���
		if (g_lpMT[nMagic].continue_Time != 0)	
		{
			pCaster->ready_Magic = 3;	// �������� ����
		}
		// �ѹ� ������ �ѹ� ��� ������ ������ ���
		if (g_lpMT[nMagic].excute_Type == 1)	
		{
			::EndCastMagic(pCaster);	// �������� ���� �غ���¸� �ʱ�ȭ
		}
		
		return true;
	}
	
	OUTPUT(MK_NORMAL, lan->OutputMessage(1, 35));  	
	
MAGIC_FIAL:
	::EndCastMagic(pCaster);	// �������� ���� �غ���¸� �ʱ�ȭ
	return false;
}

bool CMagicSys::IsEnable(int nMagic) const
{
	// ��ȭ��� ���¿����� ���� ��� �Ұ�
	if (main_interface.data[IF_BATTLE_PEACE] == 0)  
	{ // 030402 kyo
		OUTPUT(MK_NORMAL, lan->OutputMessage(1, 41));
		return false;
	}

	// ���� ���� ����� 5�� ��� �� �ٽ� ���� ���� ����
	if(g_ClientTime < Hero->castStartingTime)		
	{
		OUTPUT(MK_WARNING, lan->OutputMessage(1, 36));
		return false;
	}
	// ĳ���Ͱ� �׾� �ִ� ���
	if (SCharacterData.nCharacterHP <= 0)	
	{
		OUTPUT(MK_WARNING, lan->OutputMessage(1, 1));
		return false;
	}
	// ĳ���Ͱ� �������� �ڰ� �ִ� ���
	if (SCharacterData.sleep != CSC_AWAKE)
	{
		OUTPUT(MK_WARNING, lan->OutputMessage(1, 2));
		return false;
	}
	// �ɾ� �ִ� ���¿��� �������� �Ұ�
	if (Hero->nCurrentAction == 17)
	{
		return false;
	}
	/*
	// ������ų ���� ���� ���
	if (Hero->bSelect)
	{
    OUTPUT(MK_WARNING, lan->OutputMessage(1, 89));
    return false;
	}
	// �������� ���� ���
	if (Hero->ready_Magic != 0)
	{
	OUTPUT(MK_WARNING, lan->OutputMessage(1, 3));
	return false;
	}
	*/
	if (Hero->ready_Magic_rare != 0)
	{
		OUTPUT(MK_SHORTAGE, lan->OutputMessage(0, 81));
		return false;
	}
	// �ɷ�ġ �˻�
	if (Hero_Ability[WIS] < g_lpMT[nMagic].require_WP)
	{
		OUTPUT(MK_SHORTAGE, lan->OutputMessage(1, 6));	
		return false;
	}
	
	if (Hero_Ability[INT_] < g_lpMT[nMagic].require_IT)
	{
		OUTPUT(MK_SHORTAGE, lan->OutputMessage(1, 6));	
		return false;			
	}
	
	if (Hero_Ability[WSPS] < g_lpMT[nMagic].point_WS)
	{
		OUTPUT(MK_SHORTAGE, lan->OutputMessage(1, 6));	
		return false;
	}

	if (Hero->aStepInfo[CLS_STEP] < g_lpMT[nMagic].nDualStep)
	{
		OUTPUT(MK_SHORTAGE, lan->OutputMessage(1, 6));	
		return false;
	}
	
	// 031110 YGI
	if (Hero->aStepInfo[CLS_STEP] < g_lpMT[nMagic].nDualStep)		
	{// ���ܰ� 	// 030415 kyo
		OUTPUT(MK_SHORTAGE, lan->OutputMessage(1, 6));	
		return false;
	}

	if(g_lpMT[nMagic].nDualStep > QuickItemAttr[5].attr[0] ) //soto-031110 
	{
		OUTPUT(MK_SHORTAGE, lan->OutputMessage(1, 6));	
		return false;
	}

	// ��ƽ�� ������ �˻�
	switch (SCharacterData.nCharacterData[SPELL])
	{
    case WIZARD_SPELL:
		{ // ��������
			if (SCharacterData.SkillId[MAGERY + 88] + 1 < g_lpMT[nMagic].basic_Magery)
			{
				OUTPUT(MK_SHORTAGE, lan->OutputMessage(1, 7));
				return false;				
			}
			
			if (SCharacterData.nCharacterMP < g_lpMT[nMagic].exhaust_MP)
			{
				OUTPUT(MK_SHORTAGE, lan->OutputMessage(1, 9));
				return false;         				
			}
			
			break;
		}
    case PRIEST_SPELL:
		{ // �����ڶ��
			if (SCharacterData.SkillId[ORISON + 88] + 1 < g_lpMT[nMagic].basic_Magery)		
			{
				OUTPUT(MK_SHORTAGE, lan->OutputMessage(1, 8));
				return false;				
			}
			
			if (SCharacterData.nCharacterMP < g_lpMT[nMagic].exhaust_MP)
			{
				OUTPUT(MK_SHORTAGE, lan->OutputMessage(1, 10));
				return false; 				
			}
			
			break;
		}
	}
	// �ǹ� �ȿ� �ִ� ��� ȸ������ �̿ܿ��� ���� ��� �Ұ�
	if (g_lpMT[nMagic].magic_Type/10 != 1)
	{
		const int nX = (Hero->x>>5);
		const int nY = (Hero->y>>5);
		
		if (TileMap[nX][nY].attr_inside == 1)
		{
			OUTPUT(MK_WARNING, lan->OutputMessage(1, 4));
			return false;
		}
	}
	
	return true;
}

///////////////////////////////////////////////////////////////////////////////
// Protected Method
///////////////////////////////////////////////////////////////////////////////

bool CMagicSys::AddEffect(LPCHARACTER pCaster, LPCHARACTER pTarget, int nMagic, int nX, int nY, bool bFail)
{
	const bool bPattern = false;
	
	switch (nMagic)
	{
		//// ������
	case LIFE_UP: // ü�� ȸ��
		{
			if (pCaster != pTarget)
			{	// ����ڿ��� ���� ����
				::InsertMagic(pCaster, pTarget, nMagic, 8, 0, 0, 0, 0, bPattern, 0, bFail);
			}
			else	
			{ // �����ڿ��� ���� ����
				::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			}
			
			break;
		}
	case PROTECTION: // ��ȣ
		{
			if (pCaster != pTarget)
			{	// ����ڿ��� ���� ����
				::InsertMagic(pCaster, pTarget, nMagic, 8, 0, 0, 0, 0, bPattern, 0, bFail);
			}
			else	
			{ // �����ڿ��� ���� ����
				::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, 0, 0, bPattern, 0, bFail);
			}				
			
			break;
		}
	case SHIELD: // ��ȣ��
		{
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}		
	case DECIPHER: // �ص�����
		{
			::InsertMagic(pCaster, pTarget, nMagic, 8, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
	case HEAL_DISEASE: // ���� ġ��
		{
			if (pCaster != pTarget)
			{	// ����ڿ��� ���� ����
				::InsertMagic(pCaster, pTarget, nMagic, 8, pTarget->x, pTarget->y, 0, 0, bPattern, 0, bFail);
			}
			else
			{ // �����ڿ��� ���� ����				
				::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			}
			
			break;
		}
	case GREAT_HEAL: // ��ü��ȸ��
		{
			if (pCaster != pTarget)
			{	// ����ڿ��� ���� ����
				::InsertMagic(pCaster, pTarget, nMagic, 8, pTarget->x, pTarget->y, 0, 0, bPattern, 0, bFail);	//����ѹ� 8�� ��� ������ �õ�->���ü �߰�/����
			}
			else
			{ // �����ڿ��� ���� ����				
				::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			}
			
			break;
		}			
	case SLOW: // �ӵ�����
		{
			::InsertMagic(pCaster, pTarget, nMagic, 8, 0, 0, 0, 0, bPattern, 0, bFail);
			break;
		}		
	case CONFUSION: // ȥ��		
		{
			::InsertMagic(pCaster, pTarget, nMagic, 8, 0, 0, 0, 0, bPattern, 0, bFail);
			break;
		}		
	case INTOXICATION: // �ߵ�
		{
			::InsertMagic(pCaster, pTarget, nMagic, 8, 0, 0, 0, 0, bPattern, 0, bFail);
			break;
		}		
	case STONELY: // ��ȭ
		{
			::InsertMagic(pCaster, pTarget, nMagic, 8, 0, 0, 0, 0, bPattern, 0, bFail);
			break;
		}
	case ABSOLUTE_PARALYSIS: // ���븶��
		{
			::InsertMagic(pCaster, pTarget, nMagic, 8, 0, 0, 0, 0, bPattern, 0, bFail);
			break;
		}
	case TOXIC_CLOUD: // ������
		{
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
	case DISEASE_INDUCE: // ��������
		{
			::InsertMagic(pCaster, pTarget, nMagic, 8, 0, 0, 0, 0, bPattern, 0, bFail);
			break;
		}
	case VAMPIRE_TOUCH: // �����̾��� �ձ�
		{
			::InsertMagic(pCaster, pTarget, nMagic, 8, 0, 0, 0, 0, bPattern, 0, bFail);
			break;
		}	
	case SPIDER_WEB: // �Ź���
		{
			::InsertMagic(pCaster, pTarget, nMagic, 8, 0, 0, 0, 0, bPattern, 0, bFail);
			break;
		}		
	case MAGICAL_TRAP: // ����Ʈ��
		{
			::InsertMagic(pCaster, pTarget, nMagic, 8, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
	case ENCHANT_WEAPON: // ��üƮ ����
		{
			if (pCaster != pTarget)
			{	// ����ڿ��� ���� ����
				::InsertMagic(pCaster, pTarget, nMagic, 8, pTarget->x, pTarget->y, 0, 0, bPattern, 0, bFail);	//����ѹ� 8�� ��� ������ �õ�->���ü �߰�/����
			}
			else
			{ // �����ڿ��� ���� ����				
				::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			}
			
			break;
		}
	case TRANSPARENCY: // �����ֹ�
		{
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, 0, 0, bPattern, 0, bFail);
			break;
		}
	case NULLITY_MAGIC: // ������ȿȭ
		{
			if (pCaster != pTarget)
			{	// ����ڿ��� ���� ����
				::InsertMagic(pCaster, pTarget, nMagic, 8, 0, 0, pTarget->x, pTarget->y, bPattern, 0, bFail);	//����ѹ� 8�� ��� ������ �õ�->���ü �߰�/����
			}
			else
			{ // �����ڿ��� ���� ����				
				::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			}
			
			break;
		}
	case MINOR_AMPLIFY: // ������
		{
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, 0, 0, bPattern, 0, bFail);
			break;
		}
		
    case MAGIC_AMPLIFY: // ��������
		{
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, 0, 0, bPattern, 0, bFail);
			break;
		}    
	case FIRE_RESIST:
    case ICE_RESIST:
    case ELECT_RESIST:
		{
			::InsertMagic(pCaster, pTarget, nMagic, -1, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}	
	case FIRE_ARROW: // ��ȭ��
		{
			pCaster->bow_Type = 1;
			break;
		}
	case FIRE_BLADE: //火刃
	{
		pCaster->bow_Type = 5;
		break;
	}
	case ICE_BLADE: //冰刃
	{
		pCaster->bow_Type = 6;
		break;
	}
	case LIGHT_BLADE: //雷刃
	{
		pCaster->bow_Type = 7;
		break;
	}
	case FIRE_BALL: // ȭ����
		{
			if (pCaster != pTarget)
			{	// ����ڿ��� ���� ����
				::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, 0, 0, bPattern, 0, bFail);
			}
			else
			{ // �����ڿ��� ���� ����				
				::InsertMagic(pCaster, pTarget, nMagic, 1, 0, 0, nX, nY, bPattern, 0, bFail);
			}
			
			break;
		}	
	case SCORCHER: // ȭ����
		{
			ClearCrash();
			
			if (pCaster != pTarget)
			{	// ����ڿ��� ���� ����
				::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, 0, 0, bPattern, 0, bFail);
			}
			else
			{ // �����ڿ��� ���� ����				
				::InsertMagic(pCaster, pTarget, nMagic, 1, 0, 0, nX, nY, bPattern, 0, bFail);
			}
			
			break;
		}
	case CIRCLE_FLAME: // ���ȭ��
		{
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
	case FIRE_WALL: // ȭ����
		{
			::InsertMagic(pCaster, pTarget, nMagic, 8, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
	case FLAME_WAVE: // �����̴� ȭ���庮
		{
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, nX, nY, bPattern, 0, bFail);	//����ѹ� 8�� ��� ������ �õ�->���ü �߰�/����
			break;
		}		
	case APOCALYPSE: // ������ ��
		{
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
	case ICE_ARROW: // ���� ȭ��
		{
			pCaster->bow_Type = 2;
			break;
		}			
	case ICE_BALL: // ������
		{
			if (pCaster != pTarget)
			{	// ����ڿ��� ���� ����
				::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, 0, 0, bPattern, 0, bFail);
			}
			else
			{ // �����ڿ��� ���� ����				
				::InsertMagic(pCaster, pTarget, nMagic, 1, 0, 0, nX, nY, bPattern, 0, bFail);
			}
			
			break;
		}	
	case FREEZE_5TILE_RADIUS: // �ݰ�5Ÿ�� ��
		{
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0,  nX, nY, bPattern, 0, bFail);
			break;
		}
	case ICE_WALL: // ������
		{
			::InsertMagic(pCaster, pTarget, nMagic, 8, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
	case CIRCLE_ICE: // ���� ��������
		{
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
	case FREEZE_9TILE_RADIUS: // �ݰ� 9Ÿ�� ��
		{
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0,  nX, nY, bPattern, 0, bFail);
			break;
		}
	case LIGHT: // ��
		{
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, 0, 0, bPattern, 0, bFail);
			break;
		}		
	case LIGHTNING_BOLT:
		{
			::InsertMagic(pCaster, pTarget, nMagic, 99, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
    case GREAT_THUNDER_BOLT:
		{
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
	case CHAIN_LIGHTNING: // ���� ���ݰ���
		{
			if (pCaster != pTarget)
			{	// ����ڿ��� ���� ����
				::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, 0, 0, bPattern, 0, bFail);
			}
			else
			{ // �����ڿ��� ���� ����				
				::InsertMagic(pCaster, pTarget, nMagic, 1, 0, 0, nX, nY, bPattern, 0, bFail);
			}
			
			break;
		}
	case CIRCLE_LIGHTNING: // ���� ���ݰ���
		{
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, 0, 0, bPattern, 0, bFail);
			break;
		}
	case LIGHTNING: // ���� ���ݰ���
		{
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
	case TWINKLE_STAR: // ��ȣ��
		{
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, 0, 0, bPattern, 0, bFail);
			break;
		}
	case LIGHTNING_5TILE_RADIUS: // �ݰ�5Ÿ�� ���ݰ���
		{
			::InsertMagic(pCaster, pTarget, nMagic, 99, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
	case LOW_MONSTER_RECALL: // ���� ���� ��ȯ
		{
			::InsertMagic(pCaster, pCaster, nMagic, 8, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
	case MIDDLE_MONSTER_RECALL: // �߱� ���� ��ȯ	
		{
			::InsertMagic(pCaster, pCaster, nMagic, 8, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
	case PHASING: // ����̵�	
		{
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
	case MEMORIZING_LOCATION: // �̵���ұ��
		{
			::InsertMagic(pCaster, pCaster, nMagic, 8, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
	case TELEPORT: // �����̵�
		{
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}		
	case MULTI_PORT: // ���������̵�
		{
			::InsertMagic(pCaster, pCaster, nMagic, 0, 0, 0, 0, 0, bPattern, 0, bFail);
			break;
		}		
	case TOWN_PORTAL: // �����̵�
		{
			::InsertMagic(pCaster, pCaster, nMagic, 0, 0, 0, 0, 0, bPattern, 0, bFail);
			break;
		}
	case MAGIC_DETECT: // ����Ž��
		{
			::InsertMagic(pCaster, pCaster, nMagic, 8, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}		
	case REMOVE_CURSE: // ��������
		{
			::InsertMagic(pCaster, pTarget, nMagic, 8, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}		
	case PROTECT_RANGE_WEAPON: // ��Ÿ�����κ��� ��ȣ
		{
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, 0, 0, bPattern, 0, bFail);
			break;
		}	
	case LOST_SIGHT: // �÷»��
		{
			::InsertMagic(pCaster, pTarget, nMagic, 8, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
	case SPEED_UP: // �ӵ�����
		{
			if (pCaster != pTarget)
			{	// ����ڿ��� ���� ����
				::InsertMagic(pCaster, pTarget, nMagic, 8, 0, 0, 0, 0, bPattern, 0, bFail);	//����ѹ� 8�� ��� ������ �õ�->���ü �߰�/����
			}
			else
			{ // �����ڿ��� ���� ����				
				::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			}
			
			break;
		}
	case DRAGON_SCALE: // �巡����
		{
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, 0, 0, bPattern, 0, bFail);
			break;
		}
    case PHEONIX: // �һ�
		{
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}		
	case METEOR_STRIKE: // ����	
    case GREAT_METEOR:
		{
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}				
		//// ����
    case DEATH_FIRE: // ���� ���̾�
		{
			if (pCaster != pTarget)
			{	// ����ڿ��� ���� ����
				::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, 0, 0, bPattern, 0, bFail);
			}
			else	
			{ // �����ڿ��� ���� ����				
				::InsertMagic(pCaster, pTarget, nMagic, 1, 0, 0, nX, nY, bPattern, 0, bFail);
			}
			
			break;
		}
    case SUMMONING_SKELETON: // ��������Ʈ ��ȯ
		{	//< CSD-TW-030606
			const int nDir = pCaster->direction;
			int nPosX = 0, nPosY = 0;
			
			for (int i = 0; i < g_lpMT[nMagic].nSummonMax; ++i)
			{
				nPosX = pCaster->x + (s_aPlace[nDir][i].x<<5);
				nPosY = pCaster->y - (s_aPlace[nDir][i].y<<5);
				::InsertMagic(pCaster, pTarget, nMagic, 8, nPosX, nPosY, nPosX, nPosY, bPattern, 0, bFail);
			}
			
			break;
		}	//> CSD-TW-030606
	case FLYING_SPARK: // ������ ����ũ
		{ 
			if (pCaster != pTarget)
			{	// ����ڿ��� ���� ����
				::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, 0, 0, bPattern, 0, bFail);
			}
			else	
			{ // �����ڿ��� ���� ����				
				::InsertMagic(pCaster, pTarget, nMagic, 1, 0, 0, nX, nY, bPattern, 0, bFail);
			}
			
			break;
		}
    case VAMPIRE_POISON: // �����̾��� ��
		{ 
			::InsertMagic(pCaster, pTarget, nMagic, 8, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
    case VAMPIRE_DODGE: // �����̾� ȸ��
		{ 
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
	case SUMMONING_UNDEAD: // �𵥵� ��ȯ
		{ 
			const int nDir = pCaster->direction;
			int nPosX = 0, nPosY = 0;
			
			for (int i = 0; i < g_lpMT[nMagic].nSummonMax; ++i)
			{	//< CSD-030314
				nPosX = pCaster->x + (s_aPlace[nDir][i].x<<5);
				nPosY = pCaster->y - (s_aPlace[nDir][i].y<<5);
				::InsertMagic(pCaster, pTarget, nMagic, 8, nPosX, nPosY, nPosX, nPosY, bPattern, 0, bFail);
			}	//> CSD-030314
			
			break;
		}
    case RECALL_FOLLOW: // ���� ��ȯ
		{
			::InsertMagic(pCaster, pTarget, nMagic, 8, nX, nY, nX, nY, bPattern, 0, bFail);
			break;
		}
    case FEATHER_OF_CURSE: // ������ ����
		{ 
			::InsertMagic(pCaster, pTarget, nMagic, 8, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
    case CURSE_OF_HELL: // ��������
		{
			::InsertMagic(pCaster, pTarget, nMagic, 8, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
    case FIRE_EXPLOSION: // ȭ������
		{ 
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
	case FIRE_STORM: // ���̾� ����	
    case ICE_STORM:
		{ 
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
    case SHADOW_OF_EVIL: // ���� �׸���
		{ 
			if (pCaster != pTarget)
			{	// ����ڿ��� ���� ����
				::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, 0, 0, bPattern, 0, bFail);
			}
			else	
			{ // �����ڿ��� ���� ����				
				::InsertMagic(pCaster, pTarget, nMagic, 1, 0, 0, nX, nY, bPattern, 0, bFail);
			}
			
			break;
		}
    case FREEZING: // ������ 
		{ 
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0,  nX, nY, bPattern, 0, bFail);
			break;
		}
	case BLITZ_SPEAR: // ����â
		{ 
			if (pCaster != pTarget)
			{	// ����ڿ��� ���� ����
				::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, 0, 0, bPattern, 0, bFail);
			}
			else	
			{ // �����ڿ��� ���� ����				
				::InsertMagic(pCaster, pTarget, nMagic, 1, 0, 0, nX, nY, bPattern, 0, bFail);
			}
			
			break;
		}
	case LIGHTNING_CLOUD: // ���� ����
		{
			::InsertMagic(pCaster, pCaster, nMagic, 0, 0, 0, 0, 0, bPattern, 0, bFail);
			break;
		}
    case FIRE_BREATH: // ȭ�� �극��
		{
			// 020707 YGI
			//::InsertMagic(pCaster, pCaster, nMagic, 0, 0, 0, -370, 60, bPattern, 0, bFail);
			break;
		}
	case ICE_BREATH_PRIEST:
	{
		InsertMagic(pCaster, pTarget, ICE_BREATH, 0, 0, 0, 0, 0, bPattern, 0, bFail);
		break;
	}break;
	case ICE_BREATH: // ���� �극��
	case LIGHTNING_BREATH: // ���� �극��
		{ //< CSD-021021
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, 0, 0, bPattern, 0, bFail);
			break;
		} //> CSD-021021
	case FLAME_PILLAR: // ȭ�� ���
    case FLAME_POUR: // ȭ�� ��ǳ
		{ 
			ClearCrash();
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0,  nX, nY, bPattern, 0, bFail);
			break;
		}
    case OCTA_LIGHTNING:
    case BLOOD_SPRINKLE:
		{
			::InsertMagic(pCaster, pCaster, nMagic, 8, 0, 0, 0, 0, bPattern, 0, bFail);
			break;
		}
	case FLAME_ARROW:
		{	//< CSD-031020
			::InsertMagic(pCaster, pTarget, nMagic, 1, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}	//> CSD-031020
    case AREA_BAT_ATTACK:
    case AREA_PARALYSIS:
    case AREA_POISON:
    case AREA_CONFUSION:
    case AREA_SLOW:
		{ //< CSD-020707
			::InsertMagic(pCaster, pCaster, nMagic, 0, 0, 0, 0, 0, bPattern, 0, bFail);
			break;
		} //> CSD-020707
		//// ������
	case RESTORE_AUTHORITY:	// ȸ���� �Ǵ�
		{
			if (pCaster != pTarget)
			{	// ����ڿ��� ���� ����
				::InsertMagic(pCaster, pTarget, nMagic + 150, 8, 0, 0, 0, 0, bPattern, 0, bFail);							
			}
			else
			{ // �����ڿ��� ���� ����				
				::InsertMagic(pCaster, pTarget, nMagic + 150, 0, 0, 0, nX, nY, bPattern, 0, bFail);			
			}
			
			break;
		}
	case PROTECT_DANGER: // �������κ��� ��ȣ
		{
			if (pCaster != pTarget)
			{	// ����ڿ��� ���� ����
				::InsertMagic(pCaster, pTarget, nMagic + 150, 8, pTarget->x, pTarget->y, 0, 0, bPattern, 0, bFail);
			}
			else
			{ // �����ڿ��� ���� ����				
				::InsertMagic(pCaster, pTarget, nMagic + 150, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			}
			
			break;
		}
	case BLESS:	// �����ູ
		{
			if (pCaster != pTarget)
			{	// ����ڿ��� ���� ����
				::InsertMagic(pCaster, pTarget, nMagic + 150, 8, pTarget->x, pTarget->y, 0, 0, bPattern, 0, bFail);
			}
			else
			{ // �����ڿ��� ���� ����				
				::InsertMagic(pCaster, pTarget, nMagic + 150, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			}
			
			break;
		}
	case RECOVER_POISON: // �ص��ǱǴ�
		{
			if (pCaster != pTarget)
			{	// ����ڿ��� ���� ����
				::InsertMagic(pCaster, pTarget, nMagic + 150, 8, pTarget->x, pTarget->y, 0, 0, bPattern, 0, bFail);
			}
			else
			{ // �����ڿ��� ���� ����				
				::InsertMagic(pCaster, pTarget, nMagic + 150, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			}
			
			break;
		}
	case CURE_DISEASE:
    case PERFECT_PROTECT:
	case AUTHORITY_LETHY: // CSD-021015
    case CANCEL_DIVINE:
    case RECURRENCE_DIVINE:
    case PRAY_POWER:
    case MIRACLE_RECOVER:
    case GREAT_SHIELD:
    case GREAT_PRAY:
    case AUTHORITY_SIMUNIAN:
    case DIVINE_POWER:
		{
			if (pCaster != pTarget)
			{	// ����ڿ��� ���� ����
				::InsertMagic(pCaster, pTarget, nMagic + 150, 8, pTarget->x, pTarget->y, 0, 0, bPattern, 0, bFail);
			}
			else
			{ // �����ڿ��� ���� ����				
				::InsertMagic(pCaster, pTarget, nMagic + 150, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			}
			
			break;
		}
	case HOLY_LIGHT: // �������� ��
	case AUTHORITY_CHARISNUMEN: // CSD-021015
		{
			::InsertMagic(pCaster, pTarget, nMagic + 150, 0, 0, 0, 0, 0, bPattern, 0, bFail);
			break;
		}
	case ANTHEM:
		{ //< CSD-020706
			::InsertMagic(pCaster, pTarget, nMagic + 150, 8, 0, 0, 0, 0, bPattern, 0, bFail);
			break;
		} //> CSD-020706
	case RELIEF_AUTHORITY: // ������ �Ǵ�
		{
			if (pCaster != pTarget)
			{	// ����ڿ��� ���� ����
				::InsertMagic(pCaster, pTarget, nMagic + 150, 8, pTarget->x, pTarget->y, 0, 0, bPattern, 0, bFail);
			}
			else
			{ // �����ڿ��� ���� ����				
				::InsertMagic(pCaster, pTarget, nMagic + 150, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			}
			
			break;
		}
	case HOLY_FIRE: // Ȧ�� ���̾�
		{
			if (pCaster != pTarget)
			{	// ����ڿ��� ���� ����
				::InsertMagic(pCaster, pTarget, nMagic + 150, 0, 0, 0, 0, 0, bPattern, 0, bFail);
			}
			else	
			{ // �����ڿ��� ���� ����				
				::InsertMagic(pCaster, pTarget, nMagic + 150, 1, 0, 0, nX, nY, bPattern, 0, bFail);
			}
			
			break;
		}
	case INFLUENCE_DIVINE: // �ſ��� ��ȭ
		{
			if (pCaster != pTarget)
			{	// ����ڿ��� ���� ����
				::InsertMagic(pCaster, pTarget, nMagic + 150, 8, pTarget->x, pTarget->y, 0, 0, bPattern, 0, bFail);
			}
			else	
			{ // �����ڿ��� ���� ����				
				::InsertMagic(pCaster, pTarget, nMagic + 150, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			}
			
			break;
		}
	case VOICE_SOUL:
		{
			::InsertMagic(pCaster, pTarget, nMagic + 150, 0, 0, 0, 0, 0, bPattern, 0, bFail);
			break;
		}		
    case HOLY_RAGE:
		{
			::InsertMagic(pCaster, pTarget, nMagic + 150, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
    case APPRECIATE:
    case CURSE:
    case TURN_UNDEAD:
		{
			::InsertMagic(pCaster, pTarget, nMagic + 150, 8, 0, 0, 0, 0, bPattern, 0, bFail);
			break;
		}
	case AUTHORITY_RECOVER_SIGHT: //�÷�ȸ���� �Ǵ�
		{
			if (pCaster != pTarget)
			{	// ����ڿ��� ���� ����
				::InsertMagic(pCaster, pTarget, nMagic + 150, 8, pTarget->x, pTarget->y, 0, 0, bPattern, 0, bFail);
			}
			else	
			{ // �����ڿ��� ���� ����				
				::InsertMagic(pCaster, pTarget, nMagic + 150, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			}
			
			break;
		}
	case AUTHORITY_LUCK: // ���� ����� �Ǵ�
		{   
			if (pCaster != pTarget)
			{	// ����ڿ��� ���� ����
				::InsertMagic(pCaster, pTarget, nMagic + 150, 8, 0, 0, 0, 0, bPattern, 0, bFail);
			}
			else	
			{ // �����ڿ��� ���� ����				
				::InsertMagic(pCaster, pTarget, nMagic + 150, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			}
			
			break;
		}
	case AUTHORITY_LOST_SIGHT: // �÷»���� �Ǵ�
		{
			::InsertMagic(pCaster, pTarget, nMagic + 150, 8, 0, 0, pTarget->x, pTarget->y, bPattern, 0, bFail);
			break;
		}
	case RAGE_STONE: // ������ �г�
		{
			::InsertMagic(pCaster, pTarget, 358, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
    case LIFE_DRAIN: // ����� Ż��
		{
			::InsertMagic(pCaster, pTarget, 365, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}		
	case RAGE_BLAST:	// ȭ���� �г�
		{
			::InsertMagic(pCaster, pTarget, 364, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
	case RAGE_LIGHT_BALL: // ������ �г�
		{
			::InsertMagic(pCaster, pTarget, 366, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}						
		//// ������ ����
	case THROW_ARROW:
		{
			pCaster->bow_Type = 0;
			
			if (Hero != pCaster)
			{
				::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			}
			//�� ����� Ŭ���ϴ³� �ƴϳĿ� ���� ȣ��ο��� bPattern=0; �Ǵ� bPattern=1 ����
			break;
		}
	case THROW_KNIFE:
		{
			pCaster->bow_Type = 3;
			
			if (Hero != pCaster)
			{
				::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			}
			//�� ����� Ŭ���ϴ³� �ƴϳĿ� ���� ȣ��ο��� bPattern=0; �Ǵ� bPattern=1 ����
			//::InsertMagic( scr, dcr,magic_Num, 0, 0, 0, nX, nY, bPattern);
			break;	
		}

	case PRAY:	// 030415 kyo
		{//�߰� ���� �ʿ�
			if(pCaster == pTarget)
				::InsertMagic(pCaster, pTarget, 372, 8, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
	case HOLY_CURE:	// 030415 kyo
		{
			if (pCaster != pTarget)
			{	// ����ڿ��� ���� ����
				::InsertMagic(pCaster, pTarget, 373, 8, pTarget->x, pTarget->y, 0, 0, bPattern, 0, bFail);
			}
			else
			{ // �����ڿ��� ���� ����				
				::InsertMagic(pCaster, pTarget, 373, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			}
			break;
		}
	case AUTHORITY_OF_ATHYAS:	// 030415 kyo
		{
			if(pCaster == pTarget) // 030514 kyo
			{
				::InsertMagic(pCaster, pTarget, 389, 0, 0, 0, nX, nY, bPattern, 0, bFail);			
			}
			break;
		}
	case AUTHORITY_OF_WHOARENCHA:	// 030415 kyo
		{		
			::InsertMagic(pCaster, pTarget, 390, 0, 0, 0, nX, nY, bPattern, 0, bFail);			
			break;
		}
	case SNOWSTORM:	// 030415 kyo
		{
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
	case ICE_BREATH2:	// 030415 kyo
		{
			ClearCrash(); // 030430 kyo 
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
	case BLOOD_LIGHTNING:	// 030415 kyo
		{
			ClearCrash(); // 030516 kyo
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
	case GAZER:	// 030509 kyo
		{
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
	default:
		{
			return false;				
		}
	}												
	
	return true;
}

///////////////////////////////////////////////////////////////////////////////
// Private Method
///////////////////////////////////////////////////////////////////////////////

bool CMagicSys::IsContinue(int nMagic) const
{
	switch (g_lpMT[nMagic].magic_Type/10)
	{
    case 2: // ��ȣ ������ ���
    case 3: // ���� ������ ���
		{
			return true;
		}
	}
	
	return false;
}