// CombatSys.cpp: implementation of the CCombatSys class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CombatSys.h"

#include "Char.h"
#include "Map.h"
#include "Menu.h"
#include "Effect.h"
#include "gameproc.h"
using namespace battle;

extern int g_CurrCharStatus;
extern char tool_ID_EFFECT_TEST;
extern bool IsAttackAble(LPCHARACTER pTarget);
extern void DoLButtonDown();

static bool g_aDirection[8] =
{
	false, true,  true,  true,
		true,  false, false, false
};

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CCombatSys::CCombatSys()
{
	
}

CCombatSys::~CCombatSys()
{
	
}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

int CCombatSys::Action(LPCHARACTER pCaster)
{ 
	//const int nCombat = pCaster->nActiveCombat; //Thralas
	
	switch(pCaster->nActiveCombat.GetDecrypted())//switch (nCombat)
	{
    case LIGHTNING_BOOM:    return ACTION_ATTACK_BONG_DOWN;
    case THUNDER_BLOW:      return ACTION_ATTACK_ONESWORD2;
    case LIGHTNING_SHOCK:   return ACTION_ATTACK_BONG_JJIRKI;
    case THUNDER_STRIKE:    return ACTION_ATTACK_ONESWORD1;
    case GROUND_ATTACK:     return ACTION_ATTACK_BONG_DOWN;
    case BLOOD_WILL:        return ACTION_ATTACK_ONESWORD1; 
    case SWORD_N_ROSES:     return ACTION_ATTACK_ONESWORD2;
    case HORN_OF_ICEBERG:   return ACTION_ATTACK_BONG_JJIRKI;
    case CHARGING:          return ACTION_ATTACK_BONG_DOWN;
    case DOUBLE_ATTACK:     return ACTION_ATTACK_ONESWORD2;
    case DARK_BLADE:        return ACTION_ATTACK_ONESWORD1;
    case LIGHTNING_EXTREME: return ACTION_ATTACK_BONG_ROUND;	// 030513 kyo
    case EARTH_EXTREME:     return ACTION_ATTACK_BONG_DOWN;
    case DARK_EXTREME:      return ACTION_ATTACK_BONG_ROUND;
    case WIND_EXTREME:      return ACTION_ATTACK_BONG_ROUND;
	}
	
	return ACTION_NONE;
} 

void CCombatSys::Effect(MAGICLIST* pEffect)
{
	LPCHARACTER pCaster = pEffect->lpChar_Own;
	LPCHARACTER pTarget = pEffect->lpChar_Target;
	if (pCaster == NULL || pTarget == NULL)  return;
	
	const int nCombat = pEffect->magicSerial;
	
	if (pEffect->magicSubNum == 8)
	{
		switch (nCombat)
		{
		case BLOOD_EARTH:
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
		switch (nCombat)
		{
		case MULTIPLE_FIRE:
			{
				switch (pCaster->nActiveLevel.GetDecrypted())
				{
				case 1:
				case 2: EffectShoot(pEffect, 3); break;
				case 3: EffectShoot(pEffect, 5); break;
				}
				
				break;
			}
		case ICING_BLAST:
			{
				switch (pCaster->nActiveLevel.GetDecrypted())
				{
				case 1:
				case 2: EffectMulti(pEffect, 3); break;
				case 3: EffectMulti(pEffect, 5); break;
				}
				
				break;
			}
		case LIGHTNING_EXTREME:
			{
				//EffectSpread(pEffect);
				EffectSpreadIrregular(pEffect, g_ClientTime, 5, 7, 2 );	//5Ÿ�ϰ��� 2�� // 030328 kyo
				break;
			}
		case EARTH_EXTREME:
			{
				const int nMax = pCaster->nActiveLevel.GetDecrypted() + 1; 
				EffectQuake(pEffect, nMax, g_ClientTime);
				break;
			}
		case FIRE_EXTREME:
		case ICE_EXTREME:
			{
				EffectSpread(pEffect);
				break;
			}
		case DARK_EXTREME:
			{
				const int nMax = pCaster->nActiveLevel.GetDecrypted() + 1; 
				EffectQuake(pEffect, nMax, g_ClientTime);
				break;
			}
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

bool CCombatSys::Ready(LPCHARACTER pCaster, int nCombat, int nReady)
{
	switch (pCaster->nCurrentAction)
	{	//< CSD-030423
	case ACTION_ATTACK_BOW:
	case ACTION_THROW:
	case ACTION_SITDOWN:
		{
			return false;
		}
	}	//> CSD-030423
	// �������� ������ ���� ���� ĳ������ ������ ���
	if (g_lpML == NULL || pCaster->ready_Magic == 0 || pCaster->ready_Magic == 2)
	{
		if (Hero == pCaster)
		{
			const int nNumber = nCombat - 100;
			
			if (nNumber >= 115 && nNumber <= 150)
			{
				OUTPUT(MK_NORMAL, lan->OutputMessage(1, 60), CombatSkillExplain[nNumber-115].EngName);
			}
		}
		
		pCaster->excute_MagicNum.PutEncrypted(nCombat);  		      // �غ����� ������ ������ȣ�� ����
		pCaster->ready_Magic = 1;					            // ĳ���� ���� ���� ����
		pCaster->start_Casting  = g_ClientTime + 3000;
		pCaster->end_Casting = g_ClientTime + nReady;	// ĳ���� �Ϸ�ð� ����
		// ������ų �غ� ����
		const int nType = g_lpMT[nCombat].order_Type;
		const int nCast = 500 + (nType/10) + (nType%10);
		::InsertMagic(pCaster, pCaster, nCast, 0, 0, 0, 0, 0); 
		return true;
	}	
	
	if (Hero == pCaster)
	{
		OUTPUT(MK_WARNING, lan->OutputMessage(1, 59));
	}
	
	return false;
}

bool CCombatSys::Apply(LPCHARACTER pCaster, LPCHARACTER pTarget, int nCombat, int nX, int nY, bool bFail)
{
	if (!IsAttackAble(pTarget))	// �׾� �ְų� ���� ����� ��� 
	{		
		goto COMBAT_FIAL;			// ���� ����
	}
	
	if (pTarget == NULL)  pTarget = pCaster;

	if (Hero == pCaster || Hero == pTarget) // 030516 kyo ��ġ�̵� ���� ������ �������θ� ���´�. 
	{
		const int nNumber = nCombat - 100;
		
		if (nNumber >= 115 && nNumber <= 150)
		{
			if (bFail) 
			{ // ������ ������ �����
				OUTPUT(MK_NORMAL, lan->OutputMessage(1, 62), CombatSkillExplain[nNumber-115].EngName);
				goto COMBAT_FIAL;
			}
			
		/*	if (pCaster == pTarget)
			{
				OUTPUT(MK_NORMAL, lan->OutputMessage(1, 63), pCaster->name, 
					CombatSkillExplain[nNumber].LocalName);
			}
			else
			{
				OUTPUT(MK_NORMAL, lan->OutputMessage(1, 64), pCaster->name, 
					pTarget->name, 
					CombatSkillExplain[nNumber].LocalName);
			}
		*/
				OUTPUT(MK_NORMAL, lan->OutputMessage(1, 63), pCaster->name, 
					CombatSkillExplain[nNumber-115].EngName, pTarget->name);
		}
	}

	// �����ڰ� ĳ������ ������ ���� �߻� �غ�Ǿ� ������ �Ǵ� �� �׽�Ʈ�� ���� �޴����ý�		
	if ((g_lpMT[nCombat].excute_Type == 0 && pCaster->ready_Magic == 3) ||
		pCaster->ready_Magic == 2 || tool_ID_EFFECT_TEST == 1)
	{
		DIRECTION	dir = pCaster->direction;
		
		if ((g_lpMT[nCombat].appoint_Type == 1 && pCaster == pTarget) || g_lpMT[nCombat].appoint_Type == 0)
		{
			dir = pCaster->direction;
		}
		else
		{
			::ChangeDirection(&dir, pCaster->x, pCaster->y, nX, nY); // �ش� Ŭ���������� ���� ��ȯ
		}
		
		switch (nCombat)
		{ 
		case LIGHTNING_SHIELD:
		case BURNING_BLOODY:	// 030415 kyo
		case ICE_SHIELD:
		case LIGHTNING_BOOM:
		case GROUND_ATTACK:
		case THUNDER_BLOW:
		case LIGHTNING_SHOCK:
		case THUNDER_STRIKE:
		case STONE_ARMOR:
		case CRITICAL_HIDING:
		case HORN_OF_ICEBERG:
		case CHARGING:
		case TWISTER:
		case TYBERN_GIFT:
		case ACUQUIRE_DARK:	// 030415 kyo
		case DARK_BLADE:
		case MULTIPLE_FIRE:
		case ICING_BLAST:
		case SIMUNIAN_BOSOM:
		case LIGHTNING_EXTREME: //030513 kyo 
		case EARTH_EXTREME:
		case FIRE_EXTREME:
		case DARK_EXTREME:
		case ICE_EXTREME:
		case WIND_EXTREME:
			{
				::CharDoAni(pCaster, dir, ACTION_BATTLE_NONE);
				break;
			}
		case WHILWIND:
			{	//< CSD-TW-030701
				::CharDoAni(pCaster, dir, ACTION_MAGIC_CASTING);
				break;
			}	//> CSD-TW-030701
		case BLOOD_EARTH:
		case POISONING_NOVA:
			{
				ClearCrash();
				::CharDoAni(pCaster, dir, ACTION_MAGIC_BEFORE);
				break;
			}
		} 
		
		if (!AddEffect(pCaster, pTarget, nCombat, nX, nY, bFail))
		{
			OUTPUT(MK_WARNING, lan->OutputMessage(1, 61));	
			goto COMBAT_FIAL;
		}
	}
	
	// ���� �غ���¿��� ������������ �ܰ�� ���� ����
	if (pCaster->ready_Magic == 2)
	{
		// �����ð��� �����ϴ� ������ ���
		if (g_lpMT[nCombat].continue_Time != 0)	
		{
			pCaster->ready_Magic = 3;	// �������� ����
		}
		// �ѹ� ������ �ѹ� ��� ������ ������ ���
		if (g_lpMT[nCombat].excute_Type == 1)	
		{
			::EndCastMagic(pCaster);	// �������� ���� �غ���¸� �ʱ�ȭ
		}
		
		return true;
	}
	
	if (Hero == pCaster)
	{
		OUTPUT(MK_NORMAL, lan->OutputMessage(1, 65));
	}
	
COMBAT_FIAL:
	::EndCastMagic(pCaster);	// �������� ���� �غ���¸� �ʱ�ȭ
	return false;
}

void CCombatSys::Effect(LPCHARACTER pCaster, LPCHARACTER pTarget)
{
	static int nTemp;
	
	if (pCaster != NULL && pTarget != nullptr)
	{
		switch(pCaster->nActiveCombat.GetDecrypted()) //switch (nCaster)
		{
		case LIGHTNING_BOOM:
			{
				::InsertMagic(pCaster, pTarget, 569, 9, 0, 0, pTarget->x, pTarget->y);
				break;
			}
		case THUNDER_BLOW:
			{
				::InsertMagic(pCaster, pTarget, 577, 9, 0, 0, pTarget->x, pTarget->y);
				break;
			}
		case LIGHTNING_SHOCK:
			{
				::InsertMagic(pCaster, pTarget, 553, 9, 0, 0, pTarget->x, pTarget->y);
				break;
			}
		case THUNDER_STRIKE:
			{
				::InsertMagic(pCaster, pTarget, 552, 4, 0, 0, pTarget->x, pTarget->y);
				break;
			}
		case GROUND_ATTACK:
			{
				::InsertMagic(pCaster, pTarget, 574, 4, 0, 0, pTarget->x, pTarget->y);
				break;
			}
		case BLOOD_WILL:
			{
				::InsertMagic(pCaster, pTarget, 555, 4, 0, 0, pTarget->x, pTarget->y);
				break;
			}
		case SWORD_N_ROSES:
			{
				if (++nTemp%3 == 1)
				{
					::InsertMagic(pCaster, pTarget, 572, 9, 0, 0, pTarget->x, pTarget->y);
				}
				
				break;
			}
		case FLEET_MISSILE:
			{
				InsertMagic(pCaster, pTarget, 575, 0, 0, 0, pTarget->x, pTarget->y);
				break;
			}
		case CONVERTING_ARMOR:
			{
				::InsertMagic(pCaster, pTarget, 29, 0, 0, 0, pTarget->x, pTarget->y);
				break;
			}
		case HORN_OF_ICEBERG:
			{
				::InsertMagic(pCaster, pTarget, 565, 4, 0, 0, pTarget->x, pTarget->y);
				break;
			}
		case CHARGING:
		case DOUBLE_ATTACK:
			{
				::InsertMagic(pCaster, pTarget, 567, 4, 0, 0, pTarget->x, pTarget->y);
				break;
			}
		case CHERROY_SHADE:
			{
				::InsertMagic(pCaster, pTarget, 570, 9, 0, 0, pTarget->x, pTarget->y);
				break;
			}
		case DARK_BLADE:
			{
				::InsertMagic(pCaster, pTarget, 576, 4, 0, 0, pTarget->x, pTarget->y);
				::InsertMagic(pCaster, pTarget, 560, 9, 0, 0, pTarget->x, pTarget->y);
				break;
			}
		case CRITICAL_HIDING:
			{
				::InsertMagic(pCaster, pTarget, 179, 9, 0, 0, pTarget->x, pTarget->y);
				break;
			}
		case MULTIPLE_FIRE:
		case FIRE_EXTREME:
			{
				::InsertMagic(pCaster, pTarget, 513, 0, 0, 0, pTarget->x, pTarget->y);
				break;
			}
		case ICING_BLAST:
		case ICE_EXTREME:
			{
				::InsertMagic(pCaster, pTarget, 514, 0, 0, 0, pTarget->x, pTarget->y);
				break;
			}
		case SHARK_MISSILE:
			{
				switch (pCaster->nCurrentAction)
				{
				case ACTION_ATTACK_BOW:
					{
						InsertMagic(pCaster, pTarget, 38, 0, 0, 0, pTarget->x, pTarget->y);
						
						break;
					}
				case ACTION_THROW:
					{
						::InsertMagic(pCaster, pTarget, 189, 0, 0, 0, pTarget->x, pTarget->y);
						break;
					}
				}
				
				break;
			}
		case LIGHTNING_EXTREME: 
			{
				::InsertMagic(pCaster, pTarget, 587, 9, 0, 0, pTarget->x, pTarget->y); // 030513 kyo
				break;
			}
		case EARTH_EXTREME:
			{
				::InsertMagic(pCaster, pTarget, 586, 9, 0, 0, pTarget->x, pTarget->y);
				break;
			}
		case DARK_EXTREME: 
			{
				::InsertMagic(pCaster, pTarget, 596, 9, 0, 0, pTarget->x, pTarget->y);
				break;
			}
		case WIND_EXTREME:
			{
				::InsertMagic(pCaster, pCaster, 591, 0, 0, 0, pCaster->x, pCaster->y);
				::InsertMagic(pCaster, pTarget, 590, 9, 0, 0, pTarget->x, pTarget->y);
				break;
			}
    }
  }
  
  if (pTarget != NULL)
  {
	  switch(pTarget->nActiveCombat.GetDecrypted()) //switch (nActive)
	  {
      case STONE_ARMOR:
		  {
			  ::InsertMagic(pTarget, pTarget, 573, 9, 0, 0, pTarget->x, pTarget->y);
			  break;
		  }
	  }
	  
	  const int nPassive = pTarget->nPassiveCombat;
	  
	  switch (nPassive)
	  {
      case LIGHTNING_SHIELD:
		  {
			  ::InsertMagic(pTarget, pTarget, 580, 9, 0, 0, pTarget->x, pTarget->y);
			  break;
		  }
      case ICE_SHIELD:
		  {
			  ::InsertMagic(pTarget, pTarget, 584, 9, 0, 0, pTarget->x, pTarget->y);
			  break;
		  }
	  }
  }
}

int CCombatSys::GetCastContinue(int nCombat) const
{
	return g_lpMT[nCombat].cast_Continue;
}

bool CCombatSys::IsEnable(int nCombat) const
{
	// ��ȭ��� ���¿����� ������ų ��� �Ұ�
	if (main_interface.data[IF_BATTLE_PEACE] == 0)  
	{ 
		OUTPUT(MK_NORMAL, lan->OutputMessage(1, 55));
		return false;
	}
	// ĳ���Ͱ� �׾� �ִ� ���
	if (SCharacterData.nCharacterHP <= 0)	
	{
		OUTPUT(MK_WARNING, lan->OutputMessage(1, 66));
		return false;
	}
	// ĳ���Ͱ� �������� �ڰ� �ִ� ���
	if (SCharacterData.sleep != CSC_AWAKE)
	{
		OUTPUT(MK_WARNING, lan->OutputMessage(1, 67));
		return false;
	}
	// �ɾ� �ִ� ���¿��� �������� �Ұ�
	if (Hero->nCurrentAction == 17)
	{
		return false;
	}
	// �ǹ� �ȿ� �ִ� ���
	const int nX = Hero->x>>5;
	const int nY = Hero->y>>5;
	
	if (TileMap[nX][nY].attr_inside == 1)
	{
		OUTPUT(MK_WARNING, lan->OutputMessage(1, 4));
		return false;
	}
	
	return true;
}

///////////////////////////////////////////////////////////////////////////////
// Protected Method
///////////////////////////////////////////////////////////////////////////////

bool CCombatSys::AddEffect(LPCHARACTER pCaster, LPCHARACTER pTarget, int nCombat, int nX, int nY, bool bFail)
{
	const int nEffect = nCombat + 300;
	const bool bPattern = false;
	
	switch (nCombat)
	{
	case LIGHTNING_SHIELD:
    case BURNING_BLOODY: 	// 030415 kyo
    case ICE_SHIELD:
    case THUNDER_BLOW:
    case LIGHTNING_SHOCK:
    case THUNDER_STRIKE:
    case BLOOD_WILL:
    case STONE_ARMOR:
    case SWORD_N_ROSES:
    case MULTIPLE_FIRE:
    case FLEET_MISSILE:
    case CONVERTING_ARMOR:
    case HORN_OF_ICEBERG:
    case ICING_BLAST:
    case CRITICAL_HIDING:
    case DOUBLE_ATTACK:
    case GUST:
    case TYBERN_GIFT:
	case ACUQUIRE_DARK:	// 030415 kyo
    case CHERROY_SHADE:
    case SHARK_MISSILE:
    case DARK_BLADE:
    case SIMUNIAN_BOSOM:
    case LIGHTNING_EXTREME:
    case EARTH_EXTREME:
    case FIRE_EXTREME:
    case DARK_EXTREME:
    case ICE_EXTREME:
    case WIND_EXTREME:
		{
			::InsertMagic(pCaster, pTarget, nEffect, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
    case LIGHTNING_BOOM:
    case BLOOD_EARTH:
    case GROUND_ATTACK:
    case POISONING_NOVA:
    case WHILWIND:
    case TWISTER:
		{ 
			::InsertMagic(pCaster, pTarget, nEffect, 8, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
    case CHARGING:
		{ 
			::InsertMagic(pCaster, pTarget, nEffect, 2, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
    default:
		{
			return false;				
		}
	}												
	
	return true;
}
