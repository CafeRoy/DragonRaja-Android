#if !defined(AFX_SKILL_LSW_H__65C7ED7E_9020_467F_AB76_8528EE975EC8__INCLUDED_)
#define AFX_SKILL_LSW_H__65C7ED7E_9020_467F_AB76_8528EE975EC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_UPGRADE_RARE 20//021113 lsw

extern bool CheckBy2ndGM_Insert_to_GMItemList(ItemAttr ia, const int iType);
extern bool CheckBy2ndGM_This_Item_Upgradeable( ItemAttr ia,const int iType);//���� �� �������� ���׷��̵� ��ų�� �ִ°����ؼ� �̰� ����� �ֳ��� ����
extern void Send2NDSKILLMAKE(int iTryRare, int iRareGroup);


extern bool LoadRareGroup();
extern bool LoadFunctionGroup();
extern bool LoadRareProbability();
extern void ReadRareGroup(char * buf,int i);
extern void ReadRareProbability(char * buf,int i);
extern bool LoadItemFunctionTable();
extern int GetSkillResorceAmount(int &r1count ,int &r2count ,int &r3count ,int &r4count);
extern bool DoButtonCheckOfMenu_SpecialWillDo_lsw(int i, int j);

typedef struct RARESET
{
	char rare[40]	;
	int rare_num	;
	int rare_rate	;
}t_rareset,*LPRARESET;

typedef struct rare_group
{
	int no;
	char name[40];

	t_rareset rareset[8];

	char group_buffer_1[40]	;
	char group_buffer_2[40]	;
	char group_buffer_3[40]	;
	int	 group_buffer_4		;
	int	 group_buffer_5		;
	int	 group_buffer_6		;
	int	 group_buffer_7		;
	int	 group_buffer_8		;
	int	 group_buffer_9		;
	int	 group_buffer_10	;
}t_raregroup,*LPRARE_GROUP;

typedef struct rare_probability
{
	int num				;
	int exp_rare_suc[4]	;
	int max_rare_day	;
	int bonus_suc		;
	int max_suc			;	
	int buffer_1		;
	int buffer_2		;
	int buffer_3		;
	int buffer_4		;
	int buffer_5		;
	int buffer_6		;
	int buffer_7		;
	int buffer_8		;
	int buffer_9		;
	int buffer_10		;
}t_rareprobability,*LPRAREPROBABILITY;

typedef struct SkillMakeItemInfo
{
	int item_no			;
	int join_no			;
	int TryRareType		;
	int RareGroupType	;
	int MakeMuch		;
}t_SkillMakeItemInfo,*LPSKILLMAKEITEMINFO; 

extern t_SkillMakeItemInfo	skillmakeiteminfo;

#define MAX_RARE_GROUP			18//020515 lsw
#define MAX_RARE_PROBABILITY	110

extern t_raregroup			RareGroup[5][MAX_RARE_GROUP];//0 �� ����׷�// 1�� ��� �׷�
extern t_rareprobability	RareProbability[MAX_RARE_PROBABILITY];

#define CMD_ADD_EFFECT	9115
typedef struct ADD_EFFECT
{
	WORD wEffectNo;	// ���� ������ ��ȣ
	WORD idTarget;	// ������ ����� �����
	WORD wDuration;	// �����ð�
}t_AddEffect,*LPADDEFFECT;

#define MAX_ITEM_FUNCTION	100
typedef struct item_function
{
	int		iNo;
	char	Name[40];
	char	NameAdd[40];
	char	Exp[50];//�߰� ����
	int		iExpMark;//020530 lsw
	int		iEffectNo;
	int		iUpgradeAble;
	int		iScrollNo;
	int		iMakeRand[3];
	int		GRADE[17];
	int		iTime[17];
	int		iBuffer1;
	int		iBuffer2;
}t_ItemFunction,*LPITEMFUNCTION;
extern t_ItemFunction ItemFunction[MAX_ITEM_FUNCTION];

extern bool	CanViewBomb(CHARACTER *ch);//Trap0
extern bool	CanViewTrap(CHARACTER *ch);
extern bool	CanViewMagicTrap(CHARACTER *ch);
extern bool	CanViewGhost(CHARACTER *ch);
extern void InitHeroCanView();//020510 lsw

enum eSkillNoIndex
{
	SKILL_DETECT_GHOST						=1,	
	SKILL_TALK_WITH_GHOST					=2,	
	SKILL_POISONING							=3,	
	SKILL_DETECT_POISON						=4,	
	SKILL_FIND_TRAPS						=5,	
	SKILL_REMOVE_TRAPS						=6,	
	SKILL_ENTRAPMENT						=7,	
//	SKILL_MAGIC_SCROLLING					=8,	
//	SKILL_ABILITY_SCROLLING					=9,	
//	SKILL_EMERGENCY_ALRAM					=10,	
//	SKILL_FIRST_AID							=11,	
//	SKILL_FAST_FOOT							=12,	
//	SKILL_SECRETHEAR						=13,	
//	SKILL_WALLCLIMBING						=14,	
//	SKILL_INTUITION							=15,	
	SKILL_DETECT_BOMB						=16,	
	SKILL_BOMB_IN_BOX						=17,	
	SKILL_REMOVE_BOMB						=18,	
	SKILL_STEALING							=19,	
//	SKILL_LOCKPICKING						=20,	
	SKILL_RECLAIM							=21,	
	SKILL_SOW								=22,	
//	SKILL_CULTIVATE							=23,	
	SKILL_HAVEST							=24,	
//	SKILL_DETECT_MINERAL					=25,	
	SKILL_MINING							=26,	
	SKILL_REFINE							=27,	
	SKILL_FISH_HOOKING						=28,	
//	SKILL_BAITMAKING						=29,	
	SKILL_NETTING							=30,	
//	SKILL_SAILING							=31,	
	SKILL_CUTTING							=32,	
	SKILL_LUMBERING							=33,	
	SKILL_BUTCHERING						=34,	
//	SKILL_DETECT_HERB						=35,	
	SKILL_DIGGING							=36,	
	SKILL_MAKEPOWDER						=37,	
//	SKILL_HERDING							=38,	
//	SKILL_MILKING							=39,	
//	SKILL_MIDWIFERY							=40,	
	SKILL_SHEAR								=41,	
	SKILL_CARPENTRY							=42,	
	SKILL_BLACKSMITHY						=43,	
	SKILL_COOKING							=44,	
	SKILL_CLOTHING							=45,	
//	SKILL_DYEING							=46,	
	SKILL_TAILORING							=47,	
	SKILL_BOWCRAFTING						=48,	
	SKILL_FLETCHING							=49,	
	SKILL_MAKEPOTION						=50,	
	SKILL_MAKEMETAL							=51,	
	SKILL_MAKEBOTTLE						=52,	
	SKILL_CANDLEMAKING						=53,	
//	SKILL_WAXING							=54,	
//	SKILL_REPAIRNG							=55,	
//	SKILL_REUTILIZING						=56,	
//	SKILL_PLAYING							=57,	
//	SKILL_PEACEMAKING						=58,	
//	SKILL_PARALYSIS							=59,	
//	SKILL_HALLUCINATION						=60,	
//	SKILL_PROVOCATION						=61,	
//	SKILL_ENTICEMENT						=62,	
//	SKILL_NOISE								=63,	
//	SKILL_TRACKING							=64,	
	SKILL_SNOOPING							=65,	
//	SKILL_SEARCH							=66,	
	SKILL_MERCHANT							=67,	
	SKILL_BLACKSMITHY_MASTER				=68,	//�������� �⼺	
	SKILL_CARPENTRY_MASTER					=69,	//��� �⼺	
	SKILL_BOWCRAFTING_MASTER				=70,	//Ȱ���ۻ� �⼺	
	SKILL_TAILORING_MASTER					=71,	//��ܻ� �⼺	
//	SKILL_HIDE								=72,	
//	SKILL_CAMPING							=73,	
	SKILL_MAKEFIRE							=74,	
	SKILL_TAMING							=75,	
//	SKILL_COMMUNICATE_WITH_ANIMAL			=76,	
//	SKILL_COMMUNICATE_WITH_OTHER_TRIBE		=77,	
//	SKILL_ANIMAL_LORE						=78,	
//	SKILL_ARMS_LORE							=79,	
	SKILL_EVALUATE_INTELIGENCE				=80,	
	SKILL_EVALUATE_WISDOM					=81,	
	SKILL_ANATOMY_JUDGEMENT					=82,	
	SKILL_ITEM_IDENTIFICATION				=83,	
//	SKILL_TASTE_IDENTIFICATION				=84,	
//	SKILL_APPRAISAL							=85,	
//	SKILL_HEALING							=86,	
	SKILL_TACTICS							=87,
	TACTICS_Crapple							=88,
	TACTICS_swordmanship					=89,
	TACTICS_Archery							=90,
	TACTICS_Fencing							=91,	
	TACTICS_Mace_fighting					=92,
	TACTICS_Pierce							=93,
	TACTICS_Whirl							=94,
	TACTICS_Hurl							=95,
	TACTICS_Parrying						=96,
	TACTICS_Double_Swordmanship				=97,
	TACTICS_Double_MaceFighting				=98,	
	TACTICS_Magery							=99,
	TACTICS_Orison							=100,
//	SKILL_THREAD							=101,	
//	SKILL_COLORING							=102,	
//	SKILL_JUMP								=103,	
//	SKILL_RIDING							=104,	
	SKILL_GHOST_RESURRECTION				=105,	
	SKILL_GM_CARPENTER						=106,	
	SKILL_GM_BLACKSMITH						=107,	
	SKILL_GM_COOK							=108,	
	SKILL_GM_TAILOR							=109,	
	SKILL_GM_BOWCRAFT						=110,	
	SKILL_GM_ALCHEMIST						=111,	
	SKILL_GM_CANDLEMAKER					=112,	
};

const int TACTICS_START = SKILL_TACTICS;//�̰� �̸��� �Ǹ� ��������
const int TACTICS_MIN = TACTICS_Crapple;//�̰� �̸��� �Ǹ� ��������
const int TACTICS_MAX = TACTICS_Orison;//�̰� �ʰ��ϸ� ��������

////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct ITEMMUTANTKIND
{
	int		item_kind;
	char	exp_kind[100];
	int		upgrade_type;
	int		AddItem[4];
	int		iBuffer1;
	int		iBuffer2;
	int		iBuffer3;
	int		iBuffer4;
}t_ItemMutantKind,*LPITEM_MUTANT_KIND;
const int MAX_ITEM_MUTANT_KIND = 201;
extern t_ItemMutantKind ItemMutantKind[MAX_ITEM_MUTANT_KIND];
////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct ITEMGRADE
{
	int iGrade;
	int iAble;
	int ResourceAmount[4];
	int iBuffer1;
	int iBuffer2;
	int iBuffer3;
	int iBuffer4;
}t_ItemGrade,*LPITEM_GRADE;
const int MAX_ITEM_GRADE = 18;//MAX_ITEM_GRADE�� avail���� üũ�� ������ �޾� �ü� �ֵ��� �ڵ� �Ұ�
extern t_ItemGrade ItemGrade[MAX_ITEM_GRADE];
////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct ITEMRARE
{
	int		iNo		;
	int		iType	;
	char	EngName[100];
	char	LocalName[100];
	char	Explain[100];
	char	ExplainAdd[100];
	int		iExpMark;// + - ��ȣ ǥ��
	int		iMakeAble;
	int		RaisePro[8];// 0���� DefaultRaisePro
	int		iUpgradeAble;
	int		iAbleNation;//
	int		GRADE[16];//0���� MAX �������� �׷��̵庰 ��
	int		Buffer1;
	int		Buffer2;
	int		Buffer3;
	int		Buffer4;
	int		Buffer5;
	int		Buffer6;
}t_ItemRare,*LPITEMRARE;
const int MAX_STATIC_RARE = 100;
const int MAX_ITEM_RARE = 200;
extern t_ItemRare ItemRare[MAX_ITEM_RARE];
////////////////////////////////////////////////////////////////////////////////////////////////////////

const int MAX_DOUBLE_RARE_GRADE = 10;//021212 lsw

typedef struct ITEMRAREMAIN
{
	DWORD	grade			: 4;//���
	DWORD	soksung1		: 8;//�Ӽ�1
	DWORD	soksung2		: 8;//�Ӽ�2 ��Ƽ �Ӽ�
	DWORD	soksung3		: 8;//�ð����
	DWORD	iHighLevel		: 3;//1�̸� ���� ������
	DWORD	IsDynamicRare	: 1;//1 �̸� ��� ������ 0 �̸� ���� ������
}RareMain, *LPRareMain;

typedef struct ItemReforgeData
{
	// --- 5-bit 變量：記錄洗鍊次數或等級 (Max 31) ---
	// 用途：記錄單一屬性的洗鍊次數，當值 >= 20 時，表示該屬性洗鍊鎖定
	DWORD	QualityLevel : 6;  // 物品基礎品質或等級 (0-31)
	DWORD	Attr1ReforgeCount : 10;  // 屬性 1 的洗鍊次數 (0-19次，20為鎖定)
	DWORD	Attr3ReforgeCount : 5;  // 屬性 3 的洗鍊次數 (0-19次，20為鎖定)
	DWORD	Attr4ReforgeCount : 5;  // 屬性 4 的洗鍊次數 (0-19次，20為鎖定)
	DWORD	Attr5ReforgeCount : 5;  // 屬性 5 的洗鍊次數 (0-19次，20為鎖定)

	// --- 2-bit 變量 (Max 3) ---
	// 用途：您可以使用 1 bit 作為總鎖定標誌
	DWORD	IsReforgeLocked : 1;  // 總體洗鍊鎖定標誌 (0:可洗, 1:永久鎖定)
} ItemReforgeData, * LPItemReforgeData;

const int MAX_REFORGE_TIMES = 60;
const int REFORGE_ONCE_COST = 933333;
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
const int MAX_COMBAT_SKILL_STRUCT = 40;//0 ���� ������ ���� �������� ������ų ���� ��ŭ 
typedef struct COMBATSKILL
{
	int iGroup;			//���� ��ų �׷�
	int iGroupType;		//���� ��ų Ÿ��
	int iCombatSkillNo;	//���� ��ų �ѹ�
	int iSkillNo;		//SkillID �� �ѹ�
	int iSkillLv;		//���� ��ų ����//SCharacter.SkilID[iSkillNo]�ϸ� ������ ��
}t_CombatSkill,*LPCombatSkill;
extern t_CombatSkill CombatSkill[MAX_COMBAT_SKILL_STRUCT];//���� ��ų Doning �޴����� ���°�// 0,0 -> ����� ��ų ��ȣ 1,2,3->�׷�
extern int iCSPMax;
extern int iCSPNow;

extern int iCSPCMax;
extern int iCSPCNow;
extern int iCSReservPoint;

const int iCSCovertSkillID = 115;

const int MAX_COMBAT_SKILL_EXPLAIN = 200;
typedef struct COMBATSKILLEXPLAIN
{
	int		iSkillNo;		//	��ų��ȣ	//�� �ε���
	char	EngName[31];	//	�����̸�	
	//char	LocalName[31];	//	�ѱ��̸�	
	
	char	ClassName[51];	//	���� Ŭ���� ����( �ѱ۷� �Ķ� )
	char	WeaponTactic[51];	//	���� Ŭ���� ����( �ѱ۷� �Ķ� )
	
	int		iNeedLv[3];		//	�䱸����1	
	int		iLvCp[3];			//	����1_CP	

	char	MicroExplain[3][51];//������ ����
	
	char	Use[71];			//���� ����

	char	Explain[101];		//����
}t_CombatSkillExplain,*LPCombatSkillExplain;
extern t_CombatSkillExplain CombatSkillExplain[MAX_COMBAT_SKILL_EXPLAIN];//������ų �����

const int MAX_COMBAT_SKILL_GROUP_EXPLAIN = 10;
typedef struct COMBATSKILLGROUPEXPLAIN
{
	int		iNo;		//	��ų��ȣ	//�� �ε���
	int		iGroupNo;		//	��ų��ȣ	//�� �ε���
	int		iGroupType;		//	��ų��ȣ	//�� �ε���
	char	EngName[30];	//	�����̸�	
	char	LocalName[30];	//	�ѱ��̸�	
	int		iSkillId[6];
	char	Explain[100];		//����
}t_CombatSkillGroupExplain,*LPCombatSkillGroupExplain;
extern t_CombatSkillGroupExplain CombatSkillGroupExplain[MAX_COMBAT_SKILL_GROUP_EXPLAIN];//������ų �����

extern void CallCombatSkillDoingMenu();
extern void CallCombatSkillPointMenu();
extern void SetCSPButtonType(const int iSelectedGroupButton);
extern void SetCSPGroupButtonImage(const int iSelected);//�׷� ��ư �̹��� ����
extern void SetCSPIconImageANDnWillDo(const int iGroupNo,const int iForceGroupType = 0);
extern void SendCSLvUp(const int iCombatSkillIndex);//iSkillNo(1~6)
extern void InitCombatSkill();
extern void SetSCDMainAndOtherClear(const int iCombatSkillIndex);
extern bool IsSelectCombat(); // CSD-TW-030606
extern void CSDMainIconClear(); // CSD-TW-030627
extern int SkillIdToCombatSkillIconImage(const int iSkilNo,const bool bIsMain);
extern void SetCSDIconRectAndnWillDo();
extern void DrawCombatSkillExplain(const int iExplainX, const int iExplainY, const int iCSIndex);
extern void DrawCombatSkillGroupExplain(const int iExplainX, const int iExplainY, const int iGroupNo, const int iGroupType);
extern void SetCSPButtonTypeWhenSelectGroupType();
extern bool LoadCombatSkillExplain(int lang);
extern bool LoadCombatSkillGroupExplain();
extern void DrawRareExplain(const int iExplainX, const int iExplainY, const int iRareIndex,const int iRareType);
extern bool IsEquipAbleCheckNation(const int iAbleNation, const CHARACTER *ch);//000  100 ���� 10 ���� 1 �Ͻ�
extern bool IsEquipAbleCheckLv(const int iAbleLv, const CHARACTER *ch);
extern bool IsEquipAbleCheckFame(const int iAbleFame, const CHARACTER *ch);
extern bool	IsEquipAbleDualStep(const int iDualStep, const CHARACTER *ch);
extern bool	IsEquipAbleClass(const int iWarrior,const int iThief,
						 const int iArcher,const int iWizard,
						 const int iPriest,const CHARACTER *ch);
extern int Call_MERCHANT_QUEST_MENU(const int iGmQuestIndex);//021030 lsw

enum eRareType
{
	RARE_POWER				=1,	//	short	nPower				//�������� ������ Damage ����	
	RARE_ACCURATE			=2,	//	char	nAccurate			//������ ���� ������ ���� (Ÿ�ݽ� miss Ȯ�� ����)	
	RARE_WISE				=3,	//	short	nWise				//���� ĳ���� �ð� 00��ŭ ����	
	RARE_BEAR				=4,	//	short	nBear				//ĳ���� ���� �ð� 00��ŭ ����	
	RARE_DISTANT			=5,	//	char	cDistant			//���� ������ �־��� ���� ������ ���� (���� 100%/90%/85%�� ����)	
	RARE_VIGOR				=6,	//	short	nVigor				//�������� ������ Damage % ����	
	RARE_CURSE				=7,	//	short	nCurse				//�ڽ��� ������ ���� �迭 ���� ȿ�� ���� �ð� 00 ����	
	RARE_AID				=8,	//	short	nAid				//�ڽ��� ������ ���� �迭 ���� ȿ�� ���� �ð� 00 ����	
	RARE_MASKING			=9,	//	short	nMasking			//�ڽ��� ������ ��ȣ �迭 ���� ȿ�� ���� �ð� 00 ����	
	RARE_PIERCING			=10,	//	char	cPiercing			//00% Ȯ���� ����� ��ȣ �迭 ������ �����Ͽ� ������ �������� ����	
	RARE_BASIC_RESIST		=11,	//	char	cBasicResist		//00% Ȯ���� ����� ���� ���׷��� ����(�⺻���׷�)�Ͽ� ���� �������� ����	
	RARE_HARDEST			=12,	//	short	nHardest			//ac ����	
	RARE_SMART				=13,	//	char	cSmart				//������ ���� ȸ���� ���� (��밡 ������ Ÿ�ݽ� miss Ȯ�� ����)	
	RARE_RESIST_FREEZE		=14,	//	short	nResist_freeze		//�������׷� ����	
	RARE_RESIST_VENOM		=15,	//	short	nResist_venom		//�����׷� ����	
	RARE_RESIST_LIGHTNING	=16,	//	short	nResist_lightning	//�������׷� ����	
	RARE_RESIST_BLAZE		=17,	//	short	nResist_blaze		//�����׷� ����	
	RARE_RESIST_PARALYSIS	=18,	//	short	nResist_paralysis	//�������׷� ����	
	RARE_RESIST_HOLY		=19,	//	short	nResist_holy		//�Ź����׷� ����	
	RARE_RESIST_ALL			=20,	//	short	nResist_all			//��� ���׷� ����	//������ ���� ��
	RARE_SPEED				=21,	//	short	nSpeed				//�̵� �ӵ� ����	
	RARE_LIFE				=22,	//	short	nLife				//Life max ����	
	RARE_MANA				=23,	//	short	nMana				//Mana max ����	
	RARE_DIVINE				=24,	//	short	nDivine				//Divine max ����	
	RARE_HEALTH				=25,	//	short	nHealth				//Health max ����	
	RARE_BROOD				=26,	//	short	nBrood				//Life max % ����	
	RARE_MIND				=27,	//	short	nMind				//Mana max % ����	
	RARE_SPIRIT				=28,	//	short	nSpirit				//Divine max % ����	
	RARE_YOUTH				=29,	//	short	nYouth				//Health max % ����	
	RARE_FATAL				=30,	//	short	nFatal				//Life 1ȸ �ڵ� ȸ���� ����	
	RARE_MAGIC				=31,	//	short	nMagic				//Mana 1ȸ �ڵ� ȸ���� ����	
	RARE_NATURE				=32,	//	short	nNature				//Divine 1ȸ �ڵ� ȸ���� ����	
	RARE_STRENGTH			=33,	//	short	nStrength			//str ����	
	RARE_DEXTERITY			=34,	//	short	nDexterity			//dex ����	
	RARE_CONSTITUTION		=35,	//	short	nConstitution		//con ����	
	RARE_WISDOM				=36,	//	short	nWisdom				//wis ����	
	RARE_INTELLIGENCE		=37,	//	short	nIntelligence		//int ����	
	RARE_CHARISMA			=38,	//	short	nCharisma			//cha ����	
	RARE_ENDURANCE			=39,	//	short	nEndurance			//endu ����	
	RARE_MOVEMENT			=40,	//	short	nMovement			//movp ����	
	RARE_MORAL				=41,	//	short	nMoral				//mor ����	
	RARE_LUCKY				=42,	//	short	nLucky				//luck ����	
	RARE_WIZARD				=43,	//	short	nWizard				//WS ����	
	RARE_PRIEST				=44,	//	short	nPriest				//PS ����	
	RARE_MAGIC_POWER		=45,	//	short	nMagic_power		//���ڵ� �迭 ���� ������ ����	
	RARE_MAGIC_VIGOR		=46,	//	char	cMagic_vigor		//���ڵ� �迭 ���� ������ 00% ����	
	RARE_MAGIC_STAY			=47,	//	short	nMagic_stay			//���ڵ� �迭 ���� ���� �ð� ����	
	RARE_NATURE_POWER		=48,	//	short	nNature_power		//������Ʈ �迭 ���� ������ ����	
	RARE_NATURE_VIGOR		=49,	//	char	cNature_vigor		//������Ʈ �迭 ���� ������ 00% ����	
	RARE_NATURE_STAY		=50,	//	short	nNature_stay		//������Ʈ �迭 ���� ���ӽð� ����	
	RARE_LIFE_RECOVERY		=51,	//	char	cLife_recovery		//Life �ڵ� ȸ���� �ִ�ġ ���� (��ü�� 00%���� �ڵ� ȸ��)
	RARE_MANA_RECOVERY		=52,	//	char	cMana_recovery		//Mana �ڵ� ȸ���� �ִ�ġ ���� (��ü�� 00%���� �ڵ� ȸ��)
	RARE_CRITICAL			=53,	//	char 	cCritical			//ũ��Ƽ�� ������ ���� Ȯ�� 00% ����	
	RARE_GUARDIAN			=54,	//	short	nGuardian			//ĳ������ ������ ���� ����	
	RARE_EVASION			=55,	//	char	nEvasion			//���� ���� ȸ���� ���� (����� ���� ���� ��ȿȭ Ȯ��)	
	RARE_56					=56,	//		56					//	
	RARE_57					=57,	//		57					//	
	RARE_58					=58,	//		58					//	
	RARE_59					=59,	//		59					//	
	RARE_60					=60,	//		60					//	
	RARE_61					=61,	//		61					//	
	RARE_62					=62,	//		62					//	
	RARE_63					=63,	//		63					//	
	RARE_64					=64,	//		64					//	
	RARE_65					=65,	//		65					//	
	RARE_66					=66,	//		66					//	
	RARE_67					=67,	//		67					//	
	RARE_WITCH				=68,	//	short	nWitch				//Ÿ�� �� ����� Mana�� 00��ŭ �����	
	RARE_DEVIL				=69,	//	char	cDevil				//Ÿ�� �� ����� Mana�� 00% ��ŭ �����	
	RARE_DESIRE				=70,	//	short	nDesire				//Ÿ�� �� ����� Life�� �ΰ������� 00��ŭ �����	
	RARE_ROBBER				=71,	//	char	cRobber				//Ÿ�� �� ����� Life�� �ΰ������� 00% ��ŭ �����	
	RARE_EVIL				=72,	//	short	nEvil				//Ÿ�� �� ����� Divine�� �ΰ������� 00��ŭ �����	
	RARE_SATAN				=73,	//	char	cSatan				//Ÿ�� �� ����� Divine�� �ΰ������� 00% ��ŭ �����	
	RARE_WEARY				=74,	//	short	nWeary				//Ÿ�� �� ����� Health�� �ΰ������� 00��ŭ �����	
	RARE_FATIGUE			=75,	//	char	cFatigue			//Ÿ�� �� ����� Health�� �ΰ������� 00% ��ŭ �����	
	RARE_COUNTERATTACK		=76,	//	short	nCounterattack		//�����ڿ��� 00�� �������� ������	
	RARE_STRIKE				=77,	//	char	cStrike				//�������� ���������� 00%�� ������	
	RARE_78					=78,	//		83					//	
	RARE_SNAG				=79,	//	short	nSnag				//Ÿ�� �� ��븦 00(�ð�)���� �����Ŵ	
	RARE_VERTIGLY			=80,	//	short	nVertigly			//Ÿ�� �� ��븦 00(�ð�)���� ȥ���� �ɸ��� ��	
	RARE_POISON				=81,	//	short	nPoison				//Ÿ�� �� ��븦 00(�ð�)���� �ߵ��� �ɸ��� ��	
	RARE_STIFF				=82,	//	short	nStiff				//Ÿ�� �� ��븦 00(�ð�)���� ��ȭ�� �ɸ��� ��	
	RARE_SLACK				=83,	//	short	nSlack				//Ÿ�� �� ��븦 00(�ð�)���� �ӵ����Ͽ� �ɸ��� ��	
	RARE_84					=84,	//		84					//	
	RARE_GHOST				=85,	//	char	bGhost				//�����ϰ� �Ǹ� ������ �� �� ����	
	RARE_LANDMINE			=86,	//	char	bLandmine			//�Ϲ� Ʈ���� ������ �� ����	
	RARE_TRAPING			=87,	//	char	bTraping			//���� Ʈ���� ������ �� ����	
	RARE_DETECT				=88,	//	char	bDetect				//��� Ʈ���� ������ �� ����	//������ ���� �� 
	RARE_PUNY = 88,	//	char	bDetect				//��� Ʈ���� ������ �� ����	//������ ���� �� 
};

enum eHighLv
{
	H_LV_NO_HIGH_ITEM = 0,
	H_LV_HIGH_ITEM = 1,
	H_LV_LEGEND_ITEM = 2,
};

const int MAX_ITEM_LEGEND_SUB_BONUS_MAX = 4;
typedef struct ITEM_LEGEND_SUB
{
	int iGroupIndex;
	int iGroupMax;
	char szName[20];
	int iNeeds;
	int iWARHand2;
	int iWALHand3;
	int iWAHead4;
	int iWABody5;
	int iWANeck6;
	int iWAShoe7;
	int iWAUnique8;
	int iWATwoHand10;
	int aiBonus[MAX_ITEM_LEGEND_SUB_BONUS_MAX];
}t_ItemLegendSub, * LPITEMLEGENDSUB;

const int MAX_ITEM_LEGEND_SUB = 10;
const int MAX_ITEM_LEGEND = 30;

typedef struct ITEM_LEGEND
{
	int iNo;
	int iGroupNo;
	int iGroupIndexMax;
	t_ItemLegendSub aSub[MAX_ITEM_LEGEND_SUB];
}t_ItemLegend, * LPITEMLEGEND;


const int MAX_ITEM_LEGEND_BONUS	= 1000; 
const int MAX_ITEM_LEGEND_BONUS_KIND	= 6; 
typedef struct ITEM_LEGEND_BONUS
{
	int iNo;
	char szName[20];
	int aiKind[MAX_ITEM_LEGEND_BONUS_KIND];
	int aiGrade[MAX_ITEM_LEGEND_BONUS_KIND];
}t_ItemLegendBonus,*LPITEMLEGENDBONUS;

extern ITEM_LEGEND			g_aItemLegend[MAX_ITEM_LEGEND];
extern ITEM_LEGEND_BONUS	g_aLegendBonus[MAX_ITEM_LEGEND_BONUS];

extern void SetGMQUESTMenu(const int iGmQuestIndex);
extern int CheckMerchantOrSkillMaster();
extern int skill_index[5];
extern void SendCMD_SKILL_MASTER_MULTI_RARE_MAKE(const int iSkillNo);

extern void CallMN_START_MAP_Menu();
#endif // !defined(AFX_SKILL_LSW_H__65C7ED7E_9020_467F_AB76_8528EE975EC8__INCLUDED_)

