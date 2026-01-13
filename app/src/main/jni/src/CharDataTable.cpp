/*****************************************************************************\
* Copyright (c), Future Entertainment World / Seoul, Republic of Korea        *
* All Rights Reserved.                                                        *
*                                                                             *
* This document contains proprietary and confidential information.  No        *
* parts of this document or the computer program it embodies may be in        *
* any way copied, duplicated, reproduced, translated into a different         *
* programming language, or distributed to any person, company, or             *
* corporation without the prior written consent of Future Entertainment World *
\*****************************************************************************/

#include "StdAfx.h"


#include "dragon.h"
#include "char.h"
#include "Hong_Sub.h"
#include "directsound.h"
#include "CharDataTable.h"
#include "Horse.h"										// LTS HOSERIDER



extern CHARACTERLIST		g_CharacterList;
extern cHorseRider			g_HorseRider;				// LTS HORSERIDER


#define WAVE_ANIMATION  0

extern	ItemAttr EquipItemAttr[ITEM_EQUIP_MAX];
NPCLev_to_Exp		NPC_Lev_Ref[Num_Of_NPC_Lev] ;


////////////////////////////////////////////////////////////////////////////////////////////////
//
//	User Function...
//

///////////////// 0807 lkh �߰� ////////////////
int ReturnWaveHeroAnimation( LPCHARACTER ch, int wave )
{
  switch ( ch->nCurrentAction )
  {			
    case ACTION_BATTLE_NONE_TWOHAND	://�����⺻ 2( ��� )
    case ACTION_BATTLE_NONE			://�����⺻
	case ACTION_NONE				://�⺻����
	case ACTION_DIE					://�ױ�
	case ACTION_MAGIC_BEFORE		://��������
	case ACTION_RUN					://�ٱ�
	case ACTION_BATTLE_WALK_OWNHAND	://�����ȱ�
	case ACTION_BATTLE_WALK_TWOHAND	://�����ȱ� 2 ( ��տ� �� )
	case ACTION_BASIC_WALK			://�⺻�ȱ�


	case ACTION_ATTACK_ONESWORD1	://�Ѽհ���
	case ACTION_ATTACK_ONESWORD2	://�Ѽհ���2
	case ACTION_ATTACK_BONG_DOWN	://��, ���, ����( ����ġ�� )
	case ACTION_ATTACK_BONG_JJIRKI	://â, ��, ����( ��� )
	case ACTION_ATTACK_BOW			://Ȱ���	
	case ACTION_ATTACK_PENSWORD2	://����ڼ�
	case ACTION_ATTACK_PENSING1		://��̰��� 1
	case ACTION_ATTACK_PENSING2		://��̰��� 2
	case ACTION_THROW				://���������
	case ACTION_ATTACK_BONG_ROUND	://��, â, ���, ����( �ֵθ��� )
			
			return AttackRangeTable[ch->sprno][ ch->accessory[ 2]][8 + ch->sex];
		
		break;


#define WAV_ARMOR_IRON_HITED		214
#define WAV_ARMOR_ROBE_HITED		215
#define WAV_ARMOR_SKIN_HITED		216

//	214       �谩��Ÿ��.wav
//	215       �κ갩��Ÿ��.wav
//	216       �ǻ�Ÿ��.wav 

//	217       ������Ÿ��.wav
//	218       ����Ÿ��.wav
//	219       ����Ÿ��.wav
//	220       ��Ÿ��.wav

//	221       �����.wav
//	222       ��������.wav
	case ACTION_ATTACKED			://�±�  // ####�Ƹ� ������ ���� Ÿ�ݽ� ���� �Ҹ�...
		// #### 
		if( wave == 1 )
		{
			switch( ch->accessory[0] )
			{
			case 3 :  
			case 5 :  
			case 7 :  
			case 13:  
			case 15:  return WAV_ARMOR_IRON_HITED;
				
				
			case 1 :  
			case 9 :  
			case 11:  
			case 17:  
			case 19:  
			case 21:  if( Random(2) )
						return WAV_ARMOR_ROBE_HITED; 

			default :  return 216; //     �ǻ�Ÿ��.wav 
			}
		}
		else 
		{	
			if( ch->sex == 0 )
			{
				// #### ���� '��'�Ҹ��� �ִ´�. 
			}
			else 
			{
				// #### ���� '��'�Ҹ��� �ִ´�. 
			}
		}
		break;

	case ACTION_MAGIC_CASTING		://����ĳ����
	case ACTION_SITDOWN				://�ʱ�
	case ACTION_HORSE				://��Ÿ��
		break;
  }

	return wave;
}


int ReturnWaveNPCAnimation( LPCHARACTER ch, int wave )
{	
	switch ( ch->nCurrentAction )
	{
	case MON1_NONE			:	//�⺻����
	case MON1_DIE			:	// �ױ�....
	case MON1_ATTACKED		:	//�±�	
	case MON1_BATTLE_ATTACK :	// �Ѽհ���
	case MON1_CASTING_MAGIC :		// 08��������		// LTS DRAGON 
	case MON1_BATTLE_ATTACK2 :	// 09���� 2
	case MON1_BATTLE_ATTACK3 :	// 10���� 3
	case MON1_JUST_ACTION :		// 11������ ���� 	
	break;
	} // switch..
	
	return wave;
}	

/*	
	MON1_NONE = 0,		// �⺻����
	MON1_BATTLE_WALK,	// �⺻�ȱ�
	MON1_ATTACKED,		// �±�
	MON1_DIE,			// �ױ�....
	MON1_BATTLE_ATTACK  // �Ѽհ���
*/


///////////////////////////// SoundUp lkh ���� ///////////////////////////////////
int ReturnPCAutoWave(LPCHARACTER	lpChar, int type)		//PC(lpChar->ȿ���� ��ü/��� ĳ���� | type->0:����/1:��Ÿ����)
{
	int wave_Num =300;
	int	weapon_Type = IK_ONE_SWORD;
	int	attack_Pattern;			//�Ǽ�:0/�Ѽհ�:1/â:2/��,������:3/������:4/�Ѽ�ö��:5/��յ���:6/���:7/�Ѽպ�:8/��:9/������:10/��Ÿ:11

	// type -> 0:������ 1:Ÿ����
	//int sound		= lpChar->ȿ���� ��ü;

	CItem *weapon	= ItemUnit( EquipItemAttr[WT_WEAPON] );

	if(weapon)
		weapon_Type = weapon->GetItemKind();

	///////////////////////////// Ÿ���� ////////////////////////////////
	if(type)
	{
		LPCHARACTER			attacker=ReturnCharacterPoint(lpChar->attacker_ID);
		if(attacker==NULL)	return 0;

		//////////////////////////// npc�� ���� ���� �з� /////////////////////////////////
		switch(attacker->sprno)
		{
			case 7:		//���̷���
			case 8:		//����
			case 18:	//Ű ���� ����
			case 19:	//Ű ū ����
			case 20:	//�׶��� ����
			case 57:	//���������
			case 58:	//��Ƽ
			case 59:	//ȭ����
			case 62:	//��ũ2
			case 68:	//ȣ��Ʈ��
			case 69:	//ȣ��Ʈ��
			case 78:	//������
			case 63:	//������
			case 64:	//������
				attack_Pattern=1;		//�Ѽհ��ֵѱ�
				break;
			case 10:	//������
			case 44:	//����������
				attack_Pattern=2;		//â �迭
				break;
			case 13:	//Ʈ��
				attack_Pattern=3;		//��/������ �迭
				break;
			case 17:	//Ÿ�̹�
			case 24:	//������
			case 39:	//�ϱ޹��
				attack_Pattern=4;		//������
				break;
			case 37:	//��������
				attack_Pattern=5;		//�Ѽ�ö��
				break;
			case 75:	//������ ����
			case 61:	//ī��������		
				attack_Pattern=6;		//��յ���
				break;
			case 40:	//�ǽ����Ǿ�
				attack_Pattern=7;		//��˴�����
				break;
			case 65:
			case 66:
				attack_Pattern=8;		//�Ѽպ�
				break;
			case 60:	//�ù��Ͼ�
			case 74:	//���尣/����� ����
				attack_Pattern=9;		//�� �迭
				break;
			case 12:
			case 43:
			case 45:
			case 70:
				attack_Pattern=0;		//�Ǽ��ֵѱ�
				break;
			case 55:	//�ҵ�
			case 9:		//���νĹ�
			case 29:	//��
			case 47:	//�������ΰ�
			case 2:		//����
			case 33:	//����
			case 32:	//�˵���
				attack_Pattern=10;		//������
				break;
			default:
				attack_Pattern=11;		//��Ÿ
				break;
		}
		
		BOOL	shield_Use=0;		//���� ���� ����
		BOOL	armor_Use=0;		//�� ���� ����
		
		CItem *shield = ItemUnit( EquipItemAttr[WT_SHIELD] );
		if( shield && lpChar->peacests==1 )	shield_Use = TRUE;		//��������� ���
		
		CItem *armor = ItemUnit( EquipItemAttr[WT_ARMOR] );
		if( armor )		armor_Use = TRUE;

		int		aw = attacker->accessory[2];

		//���и� ��� ���� ��� 25% Ȯ���� ���� Ÿ���� ó��
		if( shield_Use && rand()%101<=25 )
		{
			switch(lpChar->accessory[3])	//���� Ÿ��
			{
			case 91:		//��������
			{
				// ������ ���ݵ��ۿ� ����
				if(attacker->nCurrentAction == ACTION_ATTACK_ONESWORD1 || attack_Pattern == 1 || attack_Pattern == 8 )
				{
					if( aw == 61 || aw == 62 || aw == 71 || attack_Pattern == 1 )
						wave_Num=337;		//�Ѽ�Į
					else if( aw == 66 || aw == 67 || attack_Pattern == 8 )
						wave_Num=335;		//�Ѽպ�
					else 
						wave_Num=334;		//ö��
				}

				else if(attacker->nCurrentAction == ACTION_ATTACK_ONESWORD2)
				{
					switch( aw )
					{
						case 61:			//�Ѽ�Į
						case 62:			
						case 66:			//�Ѽյ���
						case 67:
							wave_Num=337;
							break;
						default:
							wave_Num=334;
							break;
					}
				}

				else if(attacker->nCurrentAction == ACTION_ATTACK_BONG_DOWN || attack_Pattern == 4 || attack_Pattern == 9 )
				{
					if( aw == 72 || aw == 73 || aw == 76 || aw == 77 || attack_Pattern == 4 || attack_Pattern == 9 )
						wave_Num=334;
					
					else if( aw == 68 || aw == 69 || attack_Pattern == 6 )		//��պ�
						wave_Num=336+rand()%2;
					
					else if( aw == 70 || attack_Pattern == 5 )					//�Ѽ�ö��
						wave_Num=337;
				}

				else if(attacker->nCurrentAction == ACTION_ATTACK_BONG_JJIRKI || attacker->nCurrentAction == ACTION_ATTACK_BOW ||
					attacker->nCurrentAction == ACTION_THROW || attacker->nCurrentAction == ACTION_ATTACK_PENSING1 || attack_Pattern == 7 || attack_Pattern == 2)
					wave_Num=335;
				
				
				else if(attacker->nCurrentAction == ACTION_ATTACK_PENSING2)			//  12 ����:��̰��� 2 (������ �Ʒ��� ���� ���)	����:��� ��� ���
				{
					if(attacker->sprno)		//����
						wave_Num=335;
					else					//����
						wave_Num=336;
				}

				else if(attacker->nCurrentAction == ACTION_ATTACK_BONG_ROUND)		//  23 ����:��,â,��� ������ �ֵθ���				����:3��Ÿ ���
				{
					if(attacker->sprno)		//����
						wave_Num=335;
					else					//����
						wave_Num=334;
				}
				
				else	wave_Num=335;
			}
			break;

			case 89:			//���׹���
				wave_Num=330;
				break;
			
			default:			//�ݼӹ���
			{
				if(attacker->nCurrentAction==ACTION_ATTACK_ONESWORD1 || attack_Pattern == 1)
					wave_Num=331;
				else if(attacker->nCurrentAction==ACTION_ATTACK_ONESWORD2 || attack_Pattern == 5 || attack_Pattern == 8 )
				{
					if( aw == 61 || aw == 62 || aw == 71 || aw == 66 || aw == 67 || attack_Pattern == 5 || attack_Pattern == 8 )
						wave_Num=331;
					else
						wave_Num=332;
				}
				else if(attacker->nCurrentAction==ACTION_ATTACK_BONG_DOWN || attack_Pattern == 9 || attack_Pattern == 4 || attack_Pattern == 6 )
				{
					if( aw == 72 || aw == 73 || aw == 70 || attack_Pattern == 9 )
						wave_Num=331;
					else if( aw == 76 || aw == 77 || attack_Pattern == 4 )
						wave_Num=337;
					else if( aw == 68 || aw == 69 || attack_Pattern == 6 )
						wave_Num=332;
				}
				else if(attacker->nCurrentAction==ACTION_ATTACK_BONG_JJIRKI || attacker->nCurrentAction==ACTION_ATTACK_BOW ||
					attacker->nCurrentAction==ACTION_THROW || attacker->nCurrentAction==ACTION_ATTACK_PENSING1 || attack_Pattern == 7 || attack_Pattern == 2)
					wave_Num=333;
				else if(attacker->nCurrentAction==ACTION_ATTACK_PENSING2)
				{
					if(attacker->sprno)		//����
						wave_Num=331;
					else					//����
						wave_Num=333;
				}
				else if(attacker->nCurrentAction==ACTION_ATTACK_BONG_ROUND)
				{
					if(attacker->sprno)		//����
						wave_Num=331;
					else
					{
						if( aw == 72 || aw == 73 )
							wave_Num=331;
						else
							wave_Num=337;
					}
				}
				else	wave_Num=332;
			}
			break;
			}		//end switch

		}
		
		else			//���и� ��� ���� ���� ���(����Ÿ����)
		{
			if( armor && armor->GetItemKind() == IK_IRON_ARMOR )			//�ݼӰ���	// 0929 YGI
			{
				if(attacker->type==SPRITETYPE_CHARACTER)
				{
					switch(attacker->nCurrentAction)		//������ ���⿡ ���� �������� �Ҹ� ����
					{
					case ACTION_ATTACK_ONESWORD1:		//   5 �Ѽհ��� �ܹ�
						wave_Num=300+rand()%2;
						break;

					case ACTION_ATTACK_ONESWORD2:		//   6 �Ѽհ��� 2��Ÿ
						switch( attacker->accessory[2] )
						{
							case 61:			//�Ѽ�Į
							case 62:
							case 66:			//�Ѽյ���
							case 67:
								wave_Num=300+rand()%2;
								break;
							default:
								wave_Num=302;
								break;
						}
						break;

					case ACTION_ATTACK_BONG_DOWN:		//   7 ��, ���,  ����( ��� ����ġ��-�Ӹ������� �Ʒ��� )
						switch( attacker->accessory[2] )
						{
							case 72:			//	��
							case 73:
							case 76:			//	������
							case 77:
								wave_Num=303;
								break;
							case 68:		//	��յ���
							case 69:
								wave_Num=302;
								break;
							case 70:		//	���ö��
								wave_Num=305;
								break;
						}
						break;

					case ACTION_ATTACK_BONG_JJIRKI:		//   8 â �� ��( �Ѽ� ���-��� ������ )
					case ACTION_ATTACK_BOW:				//   9 Ȱ���
					case ACTION_THROW:					//  19 ���������
					case ACTION_ATTACK_PENSING1:			//  11 ����:��̰��� 1 (��� �ѹ� ���)			����:��� ������ �밢������ ����
						wave_Num=304;
						break;

					case ACTION_ATTACK_PENSING2:			//  12 ����:��̰��� 2 (������ �Ʒ��� ���� ���)	����:��� ��� ���
						wave_Num=301;
						break;

					case ACTION_ATTACK_BONG_ROUND:		//  23 ����:��,â,��� ������ �ֵθ���				����:3��Ÿ ���
						if(lpChar->sprno)		//����
							wave_Num=304;
						else					//����
							wave_Num=305;
						break;

					default:
						wave_Num=303;
						break;
					}
				}
				else		//���ڰ� npc�� ���
				{
					//�Ǽ�:0/�Ѽհ�:1/â:2/��,������:3/������:4/�Ѽ�ö��:5/��յ���:6/���:7/�Ѽպ�:8/��:9/������:10/��Ÿ:11
					switch(attack_Pattern)
					{
					case 0:
						wave_Num=303;
						break;
					case 1:
						wave_Num=300+rand()%2;
						break;
					case 2:
						wave_Num=304;
						break;
					case 3:
						wave_Num=302;
						break;
					case 4:
						wave_Num=305;
						break;
					case 5:
						wave_Num=305;
						break;
					case 6:
						wave_Num=302;
						break;
					case 7:
						wave_Num=304;
						break;
					case 8:
						wave_Num=300+rand()%2;
						break;
					case 9:
						wave_Num=303;
						break;
					case 10:
						//wave_Num=������;
						break;
					case 11:
						//wave_Num=��Ÿ��;
						break;
					}
				}

			}
			else			//�ǻ�,����,õ,�κ�
			{
				if(attacker->type==SPRITETYPE_CHARACTER)
				{
					switch(attacker->nCurrentAction)		//������ ���⿡ ���� �������� �Ҹ� ����
					{
					case ACTION_ATTACK_ONESWORD1:		//   5 �Ѽհ��� �ܹ�
						switch( attacker->accessory[2] )
						{
							case 61:			//�Ѽ�Į
							case 62:
								wave_Num=307;
								break;
							case 66:
							case 67:
								wave_Num=308;
								break;
							default:
								wave_Num=310;
								break;
						}
						break;

					case ACTION_ATTACK_ONESWORD2:		//   6 �Ѽհ��� 2��Ÿ
						switch( attacker->accessory[2] )
						{
							case 61:			//�Ѽ�Į
							case 62:
							case 66:			//�Ѽյ���
							case 67:
								wave_Num=307;
								break;
							default:
								wave_Num=318;
								break;
						}
						break;

					case ACTION_ATTACK_BONG_DOWN:		//   7 ��, ���,  ����( ��� ����ġ��-�Ӹ������� �Ʒ��� )
						switch( attacker->accessory[2] )
						{
							case 72:			//	��
							case 73:
							case 76:			//	������
							case 77:
								wave_Num=316;
								break;
							case 68:		//	��յ���
							case 69:
								wave_Num=308;
								break;
							case 70:		//	���ö��
								wave_Num=316;
								break;
						}
						break;

					case ACTION_ATTACK_BONG_JJIRKI:		//   8 â �� ��( �Ѽ� ���-��� ������ )
						wave_Num=304;
						break;

					case ACTION_ATTACK_BOW:				//   9 Ȱ���
					case ACTION_THROW:					//  19 ���������
						wave_Num=312;
						break;

					case ACTION_ATTACK_PENSING1:		//  11 ����:��̰��� 1 (��� �ѹ� ���)			����:��� ������ �밢������ ����
						if(lpChar->sprno)		//����
							wave_Num=319;
						else
							wave_Num=312;
						break;
					
					case ACTION_ATTACK_PENSING2:		//  12 ����:��̰��� 2 (������ �Ʒ��� ���� ���)	����:��� ��� ���
						if(lpChar->sprno)		//����
							wave_Num=319;
						else
							wave_Num=320;
						break;

					case ACTION_ATTACK_BONG_ROUND:		//  23 ����:��,â,��� ������ �ֵθ���				����:3��Ÿ ���
						if(lpChar->sprno)		//����
							wave_Num=306;
						else					//����
							wave_Num=317;
						break;

					default:
						wave_Num=315;
						break;
					}
				}
				else		//���ڰ� npc�� ���
				{
					//�Ǽ�:0/�Ѽհ�:1/â:2/��,������:3/������:4/�Ѽ�ö��:5/��յ���:6/���:7/�Ѽպ�:8/��:9/������:10/��Ÿ:11
					switch(attack_Pattern)
					{
					case 0:
						wave_Num=315;
						break;
					case 1:
						wave_Num=307;
						break;
					case 2:
						wave_Num=304;
						break;
					case 3:
						wave_Num=318;
						break;
					case 4:
						wave_Num=317;
						break;
					case 5:
						wave_Num=316;
						break;
					case 6:
						wave_Num=308;
						break;
					case 7:
						wave_Num=312;
						break;
					case 8:
						wave_Num=307;
						break;
					case 9:
						wave_Num=316;
						break;
					case 10:
						//wave_Num=������;
						break;
					case 11:
						//wave_Num=��Ÿ��;
						break;
					}
				}
				//wave_Num=315;
			}
		}
	}

	//////////////////////////// ������ /////////////////////////////
	else		//����(����)
	{
		if( !weapon )
		{	
			wave_Num = 219;	//���ָ�	(SOUND_NONEHAND)
			return wave_Num;
		}

		switch(lpChar->nCurrentAction)
		{
		case ACTION_ATTACK_ONESWORD1:		//   5 �Ѽհ��� �ܹ�
			switch( weapon_Type )
			{
				case IK_ONE_SWORD:			//�Ѽ�Į
					wave_Num=200+rand()%2;
					break;
				case IK_ONE_AXE:			//�Ѽյ���
					wave_Num=202+rand()%2;
					break;
				case IK_ONE_MACE:			//�Ѽ�ö��
					wave_Num=204;
					break;
				case IK_GON:				//�Ѽ�ä��
					wave_Num=207;
					break;
			}
			break;

		case ACTION_ATTACK_ONESWORD2:		//   6 �Ѽհ��� 2��Ÿ
			switch( weapon_Type )
			{
				case IK_ONE_SWORD:			//�Ѽ�Į
					wave_Num=200+(lpChar->odd_Even%2);
					lpChar->odd_Even++;
					break;
				case IK_ONE_AXE:			//�Ѽյ���
					wave_Num=202+(lpChar->odd_Even%2);
					lpChar->odd_Even++;
					break;
				case IK_GON:				//�Ѽ�ä��
					wave_Num=207+(lpChar->odd_Even%2);
					lpChar->odd_Even++;
					break;
			}
			break;

		case ACTION_ATTACK_BONG_DOWN:		//   7 ��, ���,  ����( ��� ����ġ��-�Ӹ������� �Ʒ��� )
			switch( weapon_Type )
			{
				case IK_WHIRL:			//	��
					wave_Num=209;
					break;
				case IK_STAFE:			//	������
					wave_Num=210;
					break;
				case IK_TWO_AXE:		//	��յ���
					wave_Num=211;
					break;
				case IK_TWO_MACE:		//	���ö��
					wave_Num=212;
					break;
			}
			break;

		case ACTION_ATTACK_BONG_JJIRKI:		//   8 â �� ��( �Ѽ� ���-��� ������ )
			wave_Num=213;
			break;

		case ACTION_ATTACK_BOW:				//   9 Ȱ���
			wave_Num=214;
			break;

		case ACTION_THROW:					//  19 ���������
			wave_Num=216;
			break;

		case ACTION_ATTACK_PENSING1:			//  11 ����:��̰��� 1 (��� �ѹ� ���)			����:��� ������ �밢������ ����
			if(lpChar->sprno)		//����
				wave_Num=215;
			else					//����
				wave_Num=217;
			break;

		case ACTION_ATTACK_PENSING2:			//  12 ����:��̰��� 2 (������ �Ʒ��� ���� ���)	����:��� ��� ���
			if(lpChar->sprno)		//����
				wave_Num=215;
			else					//����
				wave_Num=217;
			break;

		case ACTION_ATTACK_BONG_ROUND:		//  23 ����:��,â,��� ������ �ֵθ���				����:3��Ÿ ���
			if(lpChar->sprno)		//����
				wave_Num=209;
			else					//����
				wave_Num=218;
			break;

		default:
			wave_Num=219;
			break;
		}
	}


	if( wave_Num == 67 )
	{
		//_asm nop;
	}

	return wave_Num;
}

//////////////// SoundUp lkh ���� ////////////////
int ReturnNPCAutoWave(LPCHARACTER	lpChar, int type)		//NPC(						"						)
{
	int wave_Num = 200; // DEFAULT..
	int	material;		//�����з�:0->�ݼ�/1->����/2->�ǻ�,����/3->����(����)/4->��,����/5->��,��������
	LPCHARACTER			attacker=ReturnCharacterPoint(lpChar->attacker_ID);
	// ���� ����� ���� ���(�� ���������� ���)
	if(attacker==NULL)		return 0;

	// type -> 0:������ 1:Ÿ����

	//////////////// ���->Ÿ���� /////////////////
	if(type==1)		
	{
		switch(lpChar->sprno)
		{
		// �ݼ� ����
		case 20:
		case 80:
		case 19:
		case 63:
		case 18:
		case 64:
			material = 0;
			break;
		case 92:
		case 93:
		case 94:
		case 95:
			material = 1;
			break;
		case 5:
		case 6:
		case 45:
			material = 3;
			break;
		case 44:
		case 36:
		case 38:
			material = 4;
			break;
		case 7:		//���̷���
		case 48:
		case 3:
		case 40:
		case 70:
		case 35:
		case 52:
			material = 5;
			break;
		default:
			material = 2;
			break;
		}

		switch(material)
		{
		case 0:			//�ݼ�����
			switch(attacker->nCurrentAction)		//������ ���⿡ ���� �������� �Ҹ� ����
			{
			case ACTION_ATTACK_ONESWORD1:		//   5 �Ѽհ��� �ܹ�
				wave_Num = 300+rand()%2;
				break;

			case ACTION_ATTACK_ONESWORD2:
				switch( attacker->accessory[2] )
				{
				case 61:
				case 62:
				case 66:
				case 67:
					wave_Num = 300+rand()%2;
					break;
				default:
					wave_Num = 302;
					break;
				}
				break;

			case ACTION_ATTACK_BONG_DOWN:		//   7 ��, ���,  ����( ��� ����ġ��-�Ӹ������� �Ʒ��� )
				switch( attacker->accessory[2] )
				{
					case 72:			//	��
					case 73:
					case 76:			//	������
					case 77:
						wave_Num=303;
						break;
					case 68:		//	��յ���
					case 69:
						wave_Num=302;
						break;
					default:
						wave_Num=305;
						break;
				}
				break;

			case ACTION_ATTACK_BONG_JJIRKI:		//   8 â �� ��( �Ѽ� ���-��� ������ )
			case ACTION_ATTACK_BOW:				//   9 Ȱ���
				wave_Num=304;
				break;

			case ACTION_THROW:					//  19 ���������
			case ACTION_ATTACK_PENSING1:		//  11 ����:��̰��� 1 (��� �ѹ� ���)			����:��� ������ �밢������ ����
				wave_Num=304;
				break;

			case ACTION_ATTACK_PENSING2:		//  12 ����:��̰��� 2 (������ �Ʒ��� ���� ���)	����:��� ��� ���
				wave_Num=301;
				break;

			case ACTION_ATTACK_BONG_ROUND:		//  23 ����:��,â,��� ������ �ֵθ���				����:3��Ÿ ���
				wave_Num=305;
				break;

			default:
				wave_Num=303;
				break;
			}
			break;
			
		case 1:			//����
			switch(attacker->nCurrentAction)
			{
			case ACTION_ATTACK_ONESWORD1:		//   5 �Ѽհ��� �ܹ�
				switch( attacker->accessory[2] )
				{
					case 61:			//�Ѽ�Į
					case 62:
						wave_Num=335;
						break;
					case 66:
					case 67:
						wave_Num=306;
						break;
					default:
						wave_Num=309;
						break;
				}
				break;

			case ACTION_ATTACK_ONESWORD2:		//   6 �Ѽհ��� 2��Ÿ
				switch( attacker->accessory[2] )
				{
					case 61:			//�Ѽ�Į
					case 62:			
					case 66:			//�Ѽյ���
					case 67:			
						wave_Num=313;
						break;
					default:
						wave_Num=318;
						break;
				}
				break;

			case ACTION_ATTACK_BONG_DOWN:		//   7 ��, ���,  ����( ��� ����ġ��-�Ӹ������� �Ʒ��� )
				switch( attacker->accessory[2] )
				{
					case 72:			//��
					case 73:
					case 76:			//������
					case 77:
						wave_Num=319;
						break;
					case 68:			//��յ���
					case 69:
						wave_Num=313;
						break;
					default:
						wave_Num=309;
						break;
				}
				break;
			case ACTION_ATTACK_BONG_JJIRKI:		//   8 â �� ��( �Ѽ� ���-��� ������ )
			case ACTION_ATTACK_PENSING1:		//  11 ����:��̰��� 1 (��� �ѹ� ���)			����:��� ������ �밢������ ����
				wave_Num = 322;
				break;

			case ACTION_ATTACK_BOW:				//   9 Ȱ���
			case ACTION_THROW:					//  19 ���������
				wave_Num = 338;
				break;
				
			case ACTION_ATTACK_PENSING2:			//  12 ����:��̰��� 2 (������ �Ʒ��� ���� ���)	����:��� ��� ���
				if(attacker->sprno)		//����
					wave_Num=319;
				else					//����
					wave_Num=324;
				break;

			case ACTION_ATTACK_BONG_ROUND:		//  23 ����:��,â,��� ������ �ֵθ���				����:3��Ÿ ���
				wave_Num=334;
				break;

			default:
				wave_Num=335;
				break;
			}
			break;
		
		case 2:			//�ǻ�/����
			switch(attacker->nCurrentAction)
			{
			case ACTION_ATTACK_ONESWORD1:		//   5 �Ѽհ��� �ܹ�
				switch( attacker->accessory[2] )
				{
					case 61:			//�Ѽ�Į
					case 62:
						wave_Num=307;
						break;
					case 66:
					case 67:
						wave_Num=308;
						break;
					default:
						wave_Num=310;
						break;
				}
				break;
			case ACTION_ATTACK_ONESWORD2:		//   6 �Ѽհ��� 2��Ÿ
				switch( attacker->accessory[2] )
				{
					case 61:			//�Ѽ�Į
					case 62:			
					case 66:			//�Ѽյ���
					case 67:			
						wave_Num=307;
						break;
					default:
						wave_Num=318;
						break;
				}
				break;
			case ACTION_ATTACK_BONG_DOWN:		//   7 ��, ���,  ����( ��� ����ġ��-�Ӹ������� �Ʒ��� )
				switch( attacker->accessory[2] )
				{
					case 72:			//��
					case 73:
					case 76:			//������
					case 77:
						wave_Num=316;
						break;
					case 68:			//��յ���
					case 69:
						wave_Num=308;
						break;
					default:
						wave_Num=316;
						break;
				}
				break;
			case ACTION_ATTACK_BONG_JJIRKI:		//   8 â �� ��( �Ѽ� ���-��� ������ )
				wave_Num = 304;
				break;
			case ACTION_ATTACK_PENSING1:		//  11 ����:��̰��� 1 (��� �ѹ� ���)			����:��� ������ �밢������ ����
			case ACTION_ATTACK_BOW:				//   9 Ȱ���
			case ACTION_THROW:					//  19 ���������
				wave_Num = 312;
				break;
			case ACTION_ATTACK_PENSING2:			//  12 ����:��̰��� 2 (������ �Ʒ��� ���� ���)	����:��� ��� ���
				if(attacker->sprno)		//����
					wave_Num=319;
				else					//����
					wave_Num=320;
				break;

			case ACTION_ATTACK_BONG_ROUND:		//  23 ����:��,â,��� ������ �ֵθ���				����:3��Ÿ ���
				wave_Num=317;
				break;

			default:
				wave_Num=315;
				break;
			}
			break;
		
		case 3:			//������/����
			switch(attacker->nCurrentAction)
			{
			case ACTION_ATTACK_ONESWORD1:		//   5 �Ѽհ��� �ܹ�
				switch( attacker->accessory[2] )
				{
					case 61:			//�Ѽ�Į
					case 62:
					case 66:
					case 67:
						wave_Num=325;
						break;
					default:
						wave_Num=326;
						break;
				}
				break;
			case ACTION_ATTACK_ONESWORD2:		//   6 �Ѽհ��� 2��Ÿ
				switch( attacker->accessory[2] )
				{
					case 61:			//�Ѽ�Į
					case 62:			
					case 66:			//�Ѽյ���
					case 67:			
						wave_Num=325;
						break;
					default:
						wave_Num=326;
						break;
				}
				break;
			case ACTION_ATTACK_BONG_DOWN:		//   7 ��, ���,  ����( ��� ����ġ��-�Ӹ������� �Ʒ��� )
					wave_Num=326;
					break;
			case ACTION_ATTACK_BONG_JJIRKI:		//   8 â �� ��( �Ѽ� ���-��� ������ )
			case ACTION_ATTACK_BOW:				//   9 Ȱ���
			case ACTION_THROW:					//  19 ���������
			case ACTION_ATTACK_PENSING1:		//  11 ����:��̰��� 1 (��� �ѹ� ���)			����:��� ������ �밢������ ����
					wave_Num=327;
					break;
			case ACTION_ATTACK_PENSING2:			//  12 ����:��̰��� 2 (������ �Ʒ��� ���� ���)	����:��� ��� ���
				if(attacker->sprno)		//����
					wave_Num=326;
				else					//����
					wave_Num=325;
				break;

			case ACTION_ATTACK_BONG_ROUND:		//  23 ����:��,â,��� ������ �ֵθ���				����:3��Ÿ ���
			default:
				wave_Num=326;
				break;
			}
			break;
		case 4:
			switch(attacker->nCurrentAction)
			{
			case ACTION_ATTACK_ONESWORD1:		//   5 �Ѽհ��� �ܹ�
				switch( attacker->accessory[2] )
				{
					case 61:			//�Ѽ�Į
					case 62:
					case 66:
					case 67:
						wave_Num=314;
						break;
					default:
						wave_Num=309;
						break;
				}
				break;
			case ACTION_ATTACK_ONESWORD2:		//   6 �Ѽհ��� 2��Ÿ
				switch( attacker->accessory[2] )
				{
					case 61:			//�Ѽ�Į
					case 62:			
					case 66:			//�Ѽյ���
					case 67:			
						wave_Num=313;
						break;
					default:
						wave_Num=315;
						break;
				}
				break;
			case ACTION_ATTACK_BONG_DOWN:		//   7 ��, ���,  ����( ��� ����ġ��-�Ӹ������� �Ʒ��� )
				switch( attacker->accessory[2] )
				{
					case 72:			//��
					case 73:
						wave_Num=303;
						break;
					case 76:
					case 77:
					case 68:			//��յ���
					case 69:
						wave_Num=313;
						break;
					default:
						wave_Num=310;
						break;
				}
				break;
					
			case ACTION_ATTACK_BONG_JJIRKI:		//   8 â �� ��( �Ѽ� ���-��� ������ )
				wave_Num=312;
				break;
			case ACTION_ATTACK_BOW:				//   9 Ȱ���
			case ACTION_THROW:					//  19 ���������
				wave_Num=323;
				break;
			case ACTION_ATTACK_PENSING1:		//  11 ����:��̰��� 1 (��� �ѹ� ���)			����:��� ������ �밢������ ����
				wave_Num=323;
				break;
			case ACTION_ATTACK_PENSING2:			//  12 ����:��̰��� 2 (������ �Ʒ��� ���� ���)	����:��� ��� ���
				if(attacker->sprno)		//����
					wave_Num=313;
				else					//����
					wave_Num=318;
				break;
			case ACTION_ATTACK_BONG_ROUND:		//  23 ����:��,â,��� ������ �ֵθ���				����:3��Ÿ ���
				if(attacker->sprno)		//����
					wave_Num=303;
				else
					wave_Num=313;
				break;
			default:
				wave_Num=311;
				break;
			}
			break;
		case 5:
			switch(attacker->nCurrentAction)
			{
			case ACTION_ATTACK_ONESWORD1:		//   5 �Ѽհ��� �ܹ�
				switch( attacker->accessory[2] )
				{
					case 61:			//�Ѽ�Į
					case 62:
					case 66:
					case 67:
						wave_Num=320;
						break;
					default:
						wave_Num=321;
						break;
				}
				break;
			case ACTION_ATTACK_ONESWORD2:		//   6 �Ѽհ��� 2��Ÿ
				switch( attacker->accessory[2] )
				{
					case 61:			//�Ѽ�Į
					case 62:			
					case 66:			//�Ѽյ���
					case 67:			
						wave_Num=322;
						break;
					default:
						wave_Num=321;
						break;
				}
				break;
			case ACTION_ATTACK_BONG_DOWN:		//   7 ��, ���,  ����( ��� ����ġ��-�Ӹ������� �Ʒ��� )
				switch( attacker->accessory[2] )
				{
					case 72:			//��
					case 73:
					case 76:
					case 77:
					case 68:			//��յ���
					case 69:
						wave_Num=322;
						break;
					default:
						wave_Num=321;
						break;
				}
				break;
					
			case ACTION_ATTACK_BONG_JJIRKI:		//   8 â �� ��( �Ѽ� ���-��� ������ )
				wave_Num=323;
				break;
			case ACTION_ATTACK_BOW:				//   9 Ȱ���
			case ACTION_THROW:					//  19 ���������
				wave_Num=324;
				break;
			case ACTION_ATTACK_PENSING1:		//  11 ����:��̰��� 1 (��� �ѹ� ���)			����:��� ������ �밢������ ����
				wave_Num=324;
				break;
			case ACTION_ATTACK_PENSING2:			//  12 ����:��̰��� 2 (������ �Ʒ��� ���� ���)	����:��� ��� ���
				if(attacker->sprno)		//����
					wave_Num=322;
				else					//����
					wave_Num=320;
				break;
			case ACTION_ATTACK_BONG_ROUND:		//  23 ����:��,â,��� ������ �ֵθ���				����:3��Ÿ ���
				if(attacker->sprno)		//����
					wave_Num=320;
				else
					wave_Num=320;
				break;
			default:
				wave_Num=321;
				break;
			}
			break;
		}
	}	
	
	//////////////// ����(������) /////////////////
	else if(type==0)
	{
		switch(lpChar->sprno)
		{
		case 7:		//���̷���
		case 8:		//����
		case 9:		//���νĹ�
		case 18:	//Ű ���� ����
		case 19:	//Ű ū ����
		case 20:	//�׶��� ����
		case 57:	//���������
		case 58:	//��Ƽ
		case 59:	//ȭ����
		case 62:	//��ũ2
		case 68:	//ȣ��Ʈ��
		case 69:	//ȣ��Ʈ��
		case 78:	//������
		case 63:	//������
		case 64:	//������
			wave_Num=200+rand()%2;		//�Ѽհ��ֵѱ�
			break;
		case 10:	//������
		case 44:	//����������
			wave_Num=213;		//â �迭
			break;
		case 13:	//Ʈ��
			wave_Num=207+rand()%2;		//��/������ �迭
			break;
		case 17:	//Ÿ�̹�
		case 24:	//������
		case 39:	//�ϱ޹��
			if(rand()%2)
				wave_Num=210;		//������ �迭
			else
				wave_Num=218;
			break;
		case 37:	//��������
			wave_Num=204;		//�Ѽ�ö��
			break;
		case 75:	//������ ����
		case 61:	//ī��������		
			wave_Num=211;		//��յ���
			break;
		case 40:	//�ǽ����Ǿ�
			wave_Num=216;		//��˴�����
			break;
		case 65:
		case 66:
			wave_Num=202+rand()%2;		//�Ѽպ�
			break;
		case 60:	//�ù��Ͼ�
		case 74:	//���尣/����� ����
			wave_Num=209;		//�� �迭
			break;
		case 12:
		case 43:
		case 45:
		case 70:
			wave_Num=219;		//�Ǽ��ֵѱ�
			break;
		}
	}

	else if(type==2)//���� �ױ⿡ ���� ȿ����
	{
		switch(lpChar->sprno)
		{
		case 7:		//���̷���
			wave_Num=440;		//�μ��ױ�
			break;
		case 37:
		case 38:
		case 44:
			wave_Num=445;		//�������ױ�
			break;
		case 65:	case 66:
			wave_Num=444;
			break;
		case 10:
		case 41:
		case 43:
		case 48:
		case 49:
		case 54:
			wave_Num=443;	//������
			break;
		case 9:		//���νĹ�
			wave_Num=441;	//������
			break;
		default:
			if(rand()%2)
				wave_Num=446;	//���� ������ 1
			else
				wave_Num=447;	//���� ������ 2
		}

	}

	return  wave_Num;
}


////////////////// 0810 lkh �߰�  ////////////////////
int	ReturnWaveNatural( LPCHARACTER	ch )
{
	int return_Wave = 0;
		
	int	max_count = ch->AnimationTable[ACTION_NONE].nCount;
	
	for(int i=0; i<max_count; i++)
	{
		return_Wave = ch->AnimationTable[ACTION_NONE].wave[i];
		if(return_Wave/1000==8)
			return (return_Wave%1000);
	}
	return 0;
}

////////////////// SoundUp lkh ����(5Typeȿ����ó��) ////////////////////
int ReturnWaveAnimation( LPCHARACTER ch )
{	
	int	return_Wave=0;

	int wave=0;
	if (ch->HorseNo>0)		// LTS HORSERIDER ADD
	{
		if (ch->animationType==ANIMATION_TYPE_MON1) wave= ch->AnimationTable[ ch->nCurrentAction].wave[ ch->nCurrentFrame.GetDecrypted()]; // nCurrentFrame
		else wave=g_HorseRider.GetWaveNo(ch);
	}
	else
	{
		wave= ch->AnimationTable[ch->nCurrentAction].wave[ ch->nCurrentFrame.GetDecrypted()]; //nCurrentFrame
	}

	if( wave == 67 )
	{
		//_asm nop;
	}

	if( wave <= 0 )								return 0;
//	if( wave > MAX_SOUND && wave != 9999 )		return 0;

	/////////////////////////// 40% Ȯ���� �Ҹ��� ////////////////////////
	//if( (wave>=66 && wave<=80) || (wave>=84 && wave<=98) || (wave>=267 && wave<=318)
	//	|| wave==132 || wave==134 || wave==135 || wave==136 ) //���հ� ��� �Ҹ��� ���
	//{
	//	if(rand()%5<2)
	//		return wave;
	//	else
	//		return 0;
	//}
	
	// wave ���̺� �׸��� ���� 9999�� ��� ĳ������ ������ ��� �� NPC������ ���� �˸��� ȿ���� ���
	if(wave == 9999)
	{
		if( ch->type==SPRITETYPE_CHARACTER )									//PC Character
		{
			switch( ch->nCurrentAction )
			{
			case ACTION_ATTACK_ONESWORD1:		//�Ѽհ���1
			case ACTION_ATTACK_ONESWORD2:		//�Ѽհ���2
			case ACTION_ATTACK_BONG_DOWN:		//����ġ�����
			case ACTION_ATTACK_BONG_JJIRKI:		//������
			case ACTION_ATTACK_PENSING1:		//�ҽ̰���1
			case ACTION_ATTACK_PENSING2:		//�ҽ̰���2
			case ACTION_ATTACK_BONG_ROUND:		//�ֵθ���
			case ACTION_ATTACK_BOW:				//Ȱ���
			case ACTION_THROW:					//��˴�����
				return_Wave=ReturnPCAutoWave(ch, 0);	//�ش� ĳ���Ͱ� �����ϰ� �ִ� ���⿡ ���� �ٸ� �Ҹ� ����
				break;
			case ACTION_ATTACKED:				//�±�
				return_Wave=ReturnPCAutoWave(ch, 1);	//�ش� ĳ���Ͱ� �����ϰ� �ִ� ��ȣ���� ���� �ٸ� �Ҹ� ����
				break;
			}
		}
		
		else if( ch->type==SPRITETYPE_NPC || ch->type==SPRITETYPE_MONSTER )		//NPC Character
		{	
			switch( ch->nCurrentAction )
			{
			case MON1_BATTLE_ATTACK:				//�Ѽհ���
				return_Wave=ReturnNPCAutoWave(ch, 0);
				break;
			case MON1_ATTACKED:						//�±�	
				return_Wave=ReturnNPCAutoWave(ch, 1);
				break;
			}
		}
		return return_Wave;
	}

	// 8XXX�� ��� -> ĳ���� �������� ���(Ư�� ���ϸ��̼��� ��� �ݺ��Ǵ� ��� Ư�� ���� ���� �ڿ����� ��µǰ� �ϴ� ��ƾ / XXX->Ư����ȣ)
	if(wave/1000 == 8)
	{
		//int	gap = 5;					//������ �÷��̽� ���ϸ��̼� �ݺ� Ƚ��
		int	wave_Natural = wave%1000;
				
		if(ch->naturalSound_Time < g_curr_time)		//���� �ڿ����� �߻���ų ��ŭ �ð��� ����� ���
		{
			// ����ð����� 13~25�� ���ĸ���
			ch->naturalSound_Time = g_curr_time + 13 + ( rand()%13 );
			if(rand()%3==0)		return	wave_Natural;		//������
			else				return 0;
		}
	}
	
	// 33% Ȯ���� �Ҹ� ���� 7XXX
	if( wave/1000 == 7 )
	{
		if(rand()%3)
			return (wave%1000);
	}

	// �������� 1�� ���:�ش��ȣ ���� ���° ȭ�ϱ��� ���� �÷����ϴ� ���
	if(wave/10000 == 1)					//õ���� ����:���ع�ȣ ���� ���° ȭ�ϱ���, ����� ����:���� ���� ���� ��ȣ
	{											
		BOOL	play_OnOff=TRUE;
		
		// ���ռҸ��� ��¼�� �ѹ�(50% Ȯ��)
		switch( ch->nCurrentAction )
		{
		case ACTION_ATTACK_ONESWORD1:			//   5 �Ѽհ��� �ܹ�
		case ACTION_ATTACK_ONESWORD2:			//   6 �Ѽհ��� 2��Ÿ
		case ACTION_ATTACK_BONG_DOWN:			//   7 ��, ���,  ����( ��� ����ġ��-�Ӹ������� �Ʒ��� )
		case ACTION_ATTACK_BONG_JJIRKI:			//   8 â �� ��( �Ѽ� ���-��� ������ )
		case ACTION_ATTACK_BOW:					//   9 Ȱ���
		case ACTION_ATTACK_PENSING1:			//  11 ����:��̰��� 1 (��� �ѹ� ���)			����:��� ������ �밢������ ����
		case ACTION_ATTACK_PENSING2:			//  12 ����:��̰��� 2 (������ �Ʒ��� ���� ���)	����:��� ��� ���
		case ACTION_THROW:						//  19 ���������
		case ACTION_ATTACK_BONG_ROUND:			//  23 ����:��,â,��� ������ �ֵθ���				����:3��Ÿ ���
			//if(rand()%2)	play_OnOff = FALSE;	//	50% Ȯ�� ����
			break;
		default:
			break;
		}
		
		if(play_OnOff)
		{
			int		wave_Real = (wave%1000);		//���� ���� ��ȣ
			int		conti = (wave%10000)/1000;		//ù��° ���� ��ȣ ���� ���°����
			//acer7
			if( conti )
				wave_Real = wave_Real + rand()%conti;	//���� �÷��̵� ���� ��ȣ
			return wave_Real;
		}
	}
	return wave;
}	
///////////////////////////////////////////////////////////////////////////	
	
///////////////////////////////////////////////////////////////////////////	
int ReturnEffectAnimation( LPCHARACTER ch )		// LTS HORSERIDER ADD
{
	int effect;
	if (ch->HorseNo>0)
	{
		if (ch->animationType==ANIMATION_TYPE_MON1) effect=ch->AnimationTable[ ch->nCurrentAction].effect[ ch->nCurrentFrame.GetDecrypted()];
		else effect=g_HorseRider.GetEffectWaveNo(ch);
	}
	else
	{
		effect=ch->AnimationTable[ ch->nCurrentAction].effect[ ch->nCurrentFrame.GetDecrypted()]; //nCurrentFrame

    if (effect == 3)
    {
      int k =0;
    }
	}
	return effect;
}
	
int ReturnAttackWaveAnimation( LPCHARACTER ch )		// LTS HORSERIDER ADD
{	
	int wave;
	
	if (ch->HorseNo>0)
	{
		if (ch->animationType==ANIMATION_TYPE_MON1) wave=ch->AnimationTable[ ch->nCurrentAction].wave[ ch->nCurrentFrame.GetDecrypted()]; //nCurrentFrame
		else wave=g_HorseRider.GetWaveNo(ch);
	}
	else
	{
		wave=ch->AnimationTable[ ch->nCurrentAction].wave[ ch->nCurrentFrame.GetDecrypted()]; //nCurrentFrame
	}


	return wave;
}	
	
int ReturnAttackedWaveAnimation( LPCHARACTER ch )	// LTS HORSERIDER ADD
{	
	int wave;
	if (ch->HorseNo>0)
	{
		if (ch->animationType==ANIMATION_TYPE_MON1) wave=ch->AnimationTable[ ch->nCurrentAction].wave[ ch->nCurrentFrame.GetDecrypted()];
		else wave=g_HorseRider.GetWaveNo(ch);
	}
	else
	{
		wave=ch->AnimationTable[ ch->nCurrentAction].wave[ ch->nCurrentFrame.GetDecrypted()]; //nCurrentFrame
	}
	
	return wave;
}

LPCHARACTER  ReturnCharacterID( int id )
{
	LPCHARACTER lpcharacter = 	Hero;
	while( lpcharacter != NULL )
	{
		if( lpcharacter->id == id ) return lpcharacter;
		lpcharacter = lpcharacter->lpNext;
	}

	return NULL;
}
	
LPCHARACTER ReturnCharacterPoint( int id )
{	
	LPCHARACTER lpcharacter = Hero;
	while( lpcharacter != NULL )
	{
		if( lpcharacter->id == id ) return lpcharacter;
		lpcharacter = lpcharacter->lpNext;
	}
	
	return NULL;
}	

XorCrypt PlusMovePDisTance( LPCHARACTER ch, XorCrypt dx )
{	
	XorCrypt ddx, rest;
	ddx.Init(); rest.Init();
	if( dx.GetDecrypted() == 0 )
	{
		ddx.PutEncrypted(0);
		return ddx;
	}

	ddx.PutEncrypted(ch->movp.GetDecrypted() * dx.GetDecrypted() / 50);

	rest.PutEncrypted( ddx.GetDecrypted() * 50 / dx.GetDecrypted() );

	ch->restdx += abs( ch->movp.GetDecrypted() - rest.GetDecrypted() );
	
	if( ch->restdx >= ch->movp.GetDecrypted() )  
	{
		ch->restdx -= ch->movp.GetDecrypted();
		ddx.PutEncrypted(ddx.GetDecrypted()+1);
		//ddx ++;
	}

	//ch->ddx = ddx;
	return ddx;
}	


void ReturnCharMoveDistance( LPCHARACTER ch, XorCrypt *dist )
{
	switch( ch->animationType )
	{	
		case ANIMATION_TYPE_TEST  : 
		case ANIMATION_TYPE_MAN   :
		case ANIMATION_TYPE_WOMAN : switch( ch->nCurrentAction )
									{
										case ACTION_BATTLE_WALK_TWOHAND	://�����ȱ� 2 ( ��տ� �� )
										case ACTION_BATTLE_WALK_OWNHAND	://�����ȱ�	
										case ACTION_BASIC_WALK			://�⺻�ȱ�		
											if (ch->HorseNo)										// LTS HORSERIDER
											{
												dist->PutEncrypted(g_HorseRider.GetDistance(ch));
											}
											else
											{
												dist->PutEncrypted(ch->AnimationTable[ ch->nCurrentAction].dx[ ch->nCurrentFrame.GetDecrypted()]);
											}
											*dist = PlusMovePDisTance( ch, *dist );
											return;
										case ACTION_RUN					://�ٱ�	 : �ѱ��� 1.5��..
											if (ch->HorseNo)										// LTS HORSERIDER
											{
												dist->PutEncrypted(g_HorseRider.GetDistance(ch)*150/100);
											}
											else
											{
												dist->PutEncrypted((ch->AnimationTable[ ch->nCurrentAction].dx[ ch->nCurrentFrame.GetDecrypted()])*150/100);
											}
											*dist = PlusMovePDisTance( ch, *dist );
											return;
									}	
			break;					
										
		case ANIMATION_TYPE_MON1  : switch( ch->nCurrentAction )
									{
										case MON1_BATTLE_WALK	://�����ȱ�
												dist->PutEncrypted( ch->AnimationTable[ ch->nCurrentAction].dx[ ch->nCurrentFrame.GetDecrypted()]);
												*dist = PlusMovePDisTance( ch, *dist );
												return;
									}	
			break;
	}	
}		


int IsDead( LPCHARACTER ch )
{		
	if( ch == NULL ) return 1;
	if( ch == Hero ) 
	{	
		return (SCharacterData.nCharacterHP <= 0);
	}	
	else 
	{	
		return ( ch->hp <= 0 );
	}	
		
	return 0;
}		

bool IsAttackAble( LPCHARACTER pTarget )
{
	//Return false if the passed target is a GM, and the player itself is NOT
	if(pTarget->type == SPRITETYPE_CHARACTER && (pTarget->IsCounselor() && !Hero->IsCounselor()))
	{ // �޽��� ��� �ʿ�
		return false;
	}

	if( pTarget->viewtype == VIEWTYPE_GHOST_ )
	{
		return false;
	}
	return true;
}

void LoadLevelExpTable( void )
{
    // [Android 修改] 1. 路徑修正：移除開頭的 ./
    const char* path = "data/levelexp.bin";

    // [Android 修改] 2. 開檔：改用 SDL_RWFromFile
    SDL_RWops* fp = SDL_RWFromFile(path, "rb");

    if( fp )
    {
        char crc;
        char crc1 = 0;

        // [Android 修改] 3. 讀取：改用 SDL_RWread
        // 讀取主要資料區塊
        // 參數順序：(context, ptr, size, maxnum)
        SDL_RWread(fp, NPC_Lev_Ref, sizeof(NPCLev_to_Exp), Num_Of_NPC_Lev);

        // 讀取 1 byte 的 CRC
        SDL_RWread(fp, &crc, 1, 1);

        // [Android 修改] 4. 關檔
        SDL_RWclose(fp);

        // =========================================================
        // 解密與校驗邏輯 (保持原樣)
        // =========================================================
        char *tt = (char *)NPC_Lev_Ref;

        // 注意：這裡直接操作記憶體進行解密
        for( int i = 0 ; i < (int)(sizeof(NPCLev_to_Exp) * Num_Of_NPC_Lev) ; i ++)
        {
            crc1 += *tt;        // 計算解密前的 CheckSum
            *tt -= ( 100 + i ); // 解密運算 (數值位移)
            tt ++;
        }

        if( CheckSumError == 0 )
        {
            CheckSumError = abs( crc1 - crc );
            if( CheckSumError ) {
                // 建議同時印出 Log，因為 Android 上 JustMsg 可能只是一個 Toast
                SDL_Log("Error: LevelUpTable CheckSum Failed! Calc:%d, Read:%d", crc1, crc);
                JustMsg( "LevelUpTable Error" );
            }
        }

        SDL_Log("Info: Loaded LevelExpTable successfully.");
    }
    else
    {
        SDL_Log("Error: Failed to open %s", path);
    }
}

extern void SendAction(int i,int Direction);			// LTS ACTION 
void SendSmileFace( int smileno )
{
	t_packet p;
	
	p.h.header.type = CMD_SMILE_FACE;
	{
		p.u.client_smile_face.smileno = smileno;
	}
	p.h.header.size = sizeof( t_client_smile_face );

	//kyo
	switch(smileno)
	{
	case 3:
	case 4:
	case 6:
	case 7:
	case 21:
	case 27:
		{
			SendAction(ACTION_ANGER,Hero->direction);
		}break;
	case 1:
	case 5:
	case 8:
	case 13:
	case 19:
	case 20:
	case 22:
	case 24:
	case 25:
		{
			SendAction(ACTION_VICTORY,Hero->direction);
		}break;
	}
	

	QueuePacket( &p, 1 );

	Hero->smiledelay = 120;
	Hero->smileno = smileno;
}

 
void RecvSmileFace( int id, int smileno )
{	
	LPCHARACTER ch = ReturnCharacterPoint( id );
	if( ch == NULL ) return;

	if( smileno < 0 || smileno > 29 ) return;

	ch->smiledelay = 120;
	ch->smileno = smileno;
}