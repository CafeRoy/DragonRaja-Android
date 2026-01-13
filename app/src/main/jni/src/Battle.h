// Battle.h: interface for the CBattle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BATTLE_H__01911661_5F5A_426F_8E7B_2C16589738DD__INCLUDED_)
#define AFX_BATTLE_H__01911661_5F5A_426F_8E7B_2C16589738DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
///////////////////////////////////////////////////////////////////////////////
// ���� ���� ����
namespace battle
{
	enum BATTLE_TYPE
	{	//< CSD-031014
		// ������ ���� 
		PHYSICAL = 0,                  
		// ����
		LIFE_UP = 1,                   // ü��ȸ�� 
		PROTECTION = 2,                // ��ȣ
		SHIELD = 3,                    // ��ȣ��
		DECIPHER = 4,                  // �ص� ����
		HEAL_DISEASE = 5,              // ����ġ��
		GREAT_HEAL = 6,                // ��ü��ȸ��
		SLOW = 10,                     // �ӵ�����
		CONFUSION = 11,                // ȥ��
		INTOXICATION = 12,             // �ߵ�
		STONELY =	13,                // ��ȭ
		ABSOLUTE_PARALYSIS = 14,       // ���븶��
		TOXIC_CLOUD = 15,              // ������  
		DISEASE_INDUCE = 16,           // �������� 
		VAMPIRE_TOUCH = 17,            // �����̾��� �ձ�
		SPIDER_WEB = 19,               // �Ź���  
		MAGICAL_TRAP = 20,             // ����Ʈ��
		ENCHANT_WEAPON = 21,           // ��þƮ����
		TRANSPARENCY = 22,             // �����ֹ�
		NULLITY_MAGIC = 24,            // ������ȿȭ  
		MINOR_AMPLIFY = 25,            // ������
		MAGIC_AMPLIFY = 26,            // ��������
		FIRE_RESIST = 28,              // �ҿ� ���� ����  
		FIRE_ARROW = 29,               // ȭ��ȭ�� 
		FIRE_BALL= 30,                 // ȭ����
		SCORCHER = 31,                 // ȭ����(ȭ�����)
		FIRE_BLADE = 32,		//火刃
		CIRCLE_FLAME= 33,              // ���ȭ��
		FIRE_WALL = 34,                // ȭ����
		FLAME_WAVE = 35,               // �����̴� ȭ����
		APOCALYPSE = 36,               // ������ ��
		ICE_RESIST = 37,               // ���� ����
		ICE_ARROW = 38,                // ����ȭ��
		ICE_BALL = 39,                 // ������ 
		ICE_BLADE = 40,			//冰刃
		FREEZE_5TILE_RADIUS = 41,      // �ݰ� 5Ÿ�� ��	
		ICE_WALL = 42,                 // ������  	
		CIRCLE_ICE = 43,               // �������� ����
		ICE_STORM = 44,                // ������ǳ
		FREEZE_9TILE_RADIUS = 45,      // �ݰ� 9Ÿ�� �󸮱�	
		LIGHT = 46,                    // ��
		ELECT_RESIST = 47,             // ���ݿ� ���� ����
		LIGHTNING_BOLT = 48,           // ����
		LIGHT_BLADE = 49,		//雷刃
		CHAIN_LIGHTNING = 50,          // ���� ����
		CIRCLE_LIGHTNING = 51,         // ���� ���� 
		LIGHTNING = 52,                // ���� ����
		TWINKLE_STAR = 53,             // ��ȣ��
		LIGHTNING_5TILE_RADIUS = 54,   // �ݰ� 5Ÿ�� ����
		LOW_MONSTER_RECALL = 55,       // ���� ���� ��ȯ
		MIDDLE_MONSTER_RECALL = 58,    // �߱� ���� ��ȯ
		SNOWSTORM	= 59,			   // �������� ��ǳ
		ICE_BREATH2	= 60,			   // ���̽� �극��2
		BLOOD_LIGHTNING = 61,		   // ����� ����Ʈ��
		PHASING = 64,                  // ����̵�
		MEMORIZING_LOCATION = 65,      // �̵���ұ��
		TELEPORT = 66,                 // �����̵�
		MULTI_PORT = 67,               // ���������̵�
		TOWN_PORTAL = 68,              // �����̵�
		MAGIC_DETECT = 75,             // ����Ž��
		REMOVE_CURSE = 78,             // ��������
		PROTECT_RANGE_WEAPON = 79,     // ��Ÿ� ����κ��� ��ȣ
		PHEONIX = 80,				   // �һ�
		METEOR_STRIKE = 81,			   // � // 030509 kyo //030506���� 110
		LOST_SIGHT = 82,               // �÷»��   
		SPEED_UP = 83,                 // �ӵ�����
		GREAT_THUNDER_BOLT = 84,       // ������
		GREAT_METEOR = 85,             // �� 
		DRAGON_SCALE = 86,             // �巡���� ���
		DEATH_FIRE = 87,               // �������̾�
		SUMMONING_SKELETON = 88,       // ��������Ʈ ��ȯ
		FLYING_SPARK = 89,             // �ö��� ����ũ
		VAMPIRE_POISON = 90,           // �����̾��� ��
		VAMPIRE_DODGE = 91,            // �����̾� ȸ��
		SUMMONING_UNDEAD = 92,         // �𵥵� ��ȯ
		FEATHER_OF_CURSE = 93,         // ������ ����
		CURSE_OF_HELL = 94,            // ���� ����
		RECALL_FOLLOW = 95,            // ���� ��ȯ
		FIRE_EXPLOSION = 96,           // ȭ������
		FIRE_STORM = 97,               // ���̾��
		SHADOW_OF_EVIL = 98,           // ���� �׸���
		FREEZING = 99,                 // ���� ��
		BLITZ_SPEAR = 100,             // ����â
		LIGHTNING_CLOUD = 101,         // ���� ����
		FIRE_BREATH = 102,             // ȭ�� �극��
		ICE_BREATH = 103,              // ���� �극��
		LIGHTNING_BREATH = 104,        // ���� �극��
		FLAME_PILLAR = 105,            // ȭ�� ���
		FLAME_POUR = 106,              // ȭ�� ��ǳ 
		OCTA_LIGHTNING = 107,          // ��� ����
		BLOOD_SPRINKLE = 108,          // �ǻѸ���
		GAZER = 109,				   // ������
		FLAME_ARROW = 110,             // ȭ��ȭ�� 

		AREA_BAT_ATTACK = 111,         // ���� �������
		AREA_PARALYSIS = 112,          // ���� ����
		AREA_POISON = 113,             // ���� �ߵ�
		AREA_CONFUSION = 114,          // ���� ȥ��
		AREA_SLOW = 115,               // ���� �ӵ�����

		RESTORE_AUTHORITY = 151,       // ȸ���ǱǴ�  
		PROTECT_DANGER = 152,          // �������κ��� ��ȣ
		BLESS = 153,                   // ���� �ູ
		RECOVER_POISON = 154,          // �ص��� �Ǵ�
		CURE_DISEASE = 155,            // ����ġ���� �Ǵ�
		MIRACLE_RECOVER = 156,     	   // ��ġ�� ����
		PERFECT_PROTECT = 157,         // ������ ���  
		GREAT_SHIELD = 158,            // ��ȣ�� ����
		CANCEL_DIVINE = 159,           // �ݽŷ� ��ȿȭ
		RECURRENCE_DIVINE = 160,       // ȸ���� �Ǵ�
		PRAY_POWER = 161,              // �⵵�� ����
		HOLY_LIGHT = 162,              // �������� ��  
		ANTHEM = 163,                  // ����
		RELIEF_AUTHORITY = 164,        // ������ �� 
		HOLY_FIRE = 165,               // �������� ��
		GREAT_PRAY = 166,              // ������ �⵵  
		APPRECIATE = 167,              // �ٸ� ������
		INFLUENCE_DIVINE = 168,        // �ſ��� ��ȭ
		VOICE_SOUL = 169,              // ��ȥ�� �Ҹ�
		HOLY_RAGE = 170,               // ������ �г�
		TURN_UNDEAD = 171,             // �� �𵥵�
		CURSE = 172,                   // ���� ���� 
		AUTHORITY_RECOVER_SIGHT = 173, // �÷�ȸ���� �Ǵ�	
		DIVINE_POWER = 174,            // ����� �Ŀ�
		AUTHORITY_LUCK = 175,          // ���� ����� �Ǵ�	
		AUTHORITY_LOST_SIGHT = 177,    // �÷»���� �Ǵ�
		PRAY = 178,	                   // �⵵
		HOLY_CURE = 179,               // �ż��� ġ��
		AUTHORITY_SIMUNIAN = 181,      // �ù��Ͼ��� �Ǵ�
		AUTHORITY_CHARISNUMEN = 182,   // ī���� ������ �Ǵ�
		AUTHORITY_LETHY = 183,         // ��Ƽ�� �Ǵ�
		AUTHORITY_OF_ATHYAS	= 185,     // �ƻ����� �Ǵ�
		AUTHORITY_OF_WHOARENCHA	= 187, // ȭ������ �Ǵ�
		RAGE_STONE = 189,              // ������ �г�
		LIFE_DRAIN = 190,              // ����� Ż��  
		RAGE_BLAST = 191,              // ȭ���� �г�
		RAGE_LIGHT_BALL = 192,         // ������ �г�
		THROW_ARROW = 194,             // ȭ��
		THROW_KNIFE = 195,             // ��
		ICE_BREATH_PRIEST = 196,       // 龍龜之息
		EDELWEROI_SPEAR = 197,			// 艾槍
		// ���� �Ӽ� ������ų
		LIGHTNING_BOOM = 215,          
		THUNDER_BLOW = 216,
		LIGHTNING_SHOCK = 217,
		THUNDER_STRIKE = 218,
		LIGHTNING_SHIELD = 219,
		LIGHTNING_EXTREME = 220,
		// ���� �Ӽ� ������ų
		BLOOD_EARTH = 221,             
		GROUND_ATTACK = 222, 
		BLOOD_WILL = 223,
		STONE_ARMOR = 224,
		SIMUNIAN_BOSOM = 225,
		EARTH_EXTREME = 226,
		// ȭ�� �Ӽ� ������ų 
		SWORD_N_ROSES = 227,
		MULTIPLE_FIRE = 228, 
		FLEET_MISSILE = 229,
		CONVERTING_ARMOR = 230, 
		BURNING_BLOODY = 231,
		FIRE_EXTREME = 232,
		// ��� �Ӽ� ������ų
		POISONING_NOVA = 233,
		CHERROY_SHADE = 234, 
		DARK_BLADE = 235, 
		TYBERN_GIFT = 236,
		ACUQUIRE_DARK = 237,
		DARK_EXTREME = 238,
		// ���� �Ӽ� ������ų
		ICING_BLAST = 239,
		CRITICAL_HIDING = 240, 
		SHARK_MISSILE = 241, 
		HORN_OF_ICEBERG = 242, 
		ICE_SHIELD = 243,
		ICE_EXTREME = 244,
		// �ٶ� �Ӽ� ������ų
		CHARGING = 245,
		DOUBLE_ATTACK = 246, 
		WHILWIND = 247,
		TWISTER = 248, 
		GUST = 249, 
		WIND_EXTREME = 250,
		// �̺�Ʈ
		THROW_SNOW_BALL = 193,
		THROW_WATER_BALL = 661,
		THROW_BIG_GULP = 663,
		// ������ ����Ʈ ȿ��
		THROW_FIRE_ARROW = 665,
		THROW_ICE_KNIFE = 666,
		// �������� ���� ȿ��
		ITEM_EFFECT_FIRE = 605,
		ITEM_EFFECT_ICE = 606,
		ITEM_EFFECT_LIGHTNING = 607
	};	//> CSD-031014

	enum E_DAMAGE_TYPE :BYTE {
		DAMAGE_TYPE_NORMAL,
		DAMAGE_TYPE_CRITICAL,
		DAMAGE_TYPE_PHYSICAL_PIERCING,
		DAMAGE_TYPE_MAGIC_PIERCING,
		DAMAGE_TYPE_BLOCKED,
		DAMAGE_TYPE_IMMUNE,
		DAMAGE_TYPE_RESIST,
		DAMAGE_TYPE_MISS,
		DAMAGE_TYPE_MAX,
	};
};
//
///////////////////////////////////////////////////////////////////////////////
struct USEEFFECTLIST;
struct MAGICLIST;
///////////////////////////////////////////////////////////////////////////////
// ���� ���� Ŭ����
class CBattle  
{
public:
	CBattle();
	virtual ~CBattle();
	
public:
	virtual int Action(LPCHARACTER pCaster) = 0;
	virtual void Effect(MAGICLIST* pEffect) = 0;
	
public:
	void EffectLocal(MAGICLIST* pMagic, DWORD dwNow);
	void EffectRound(MAGICLIST* pMagic, DWORD dwNow);
	void EffectStorm(MAGICLIST* pMagic, DWORD dwNow);
	void EffectSpread(MAGICLIST* pMagic);
	void EffectShoot(MAGICLIST* pMagic, int nCount);
	void EffectMulti(MAGICLIST* pMagic, int nCount);
	void EffectBreath(MAGICLIST* pMagic, int nX, int nY);
	void EffectPillar(MAGICLIST* pMagic, DWORD dwNow, const int nWidth=100, bool IsCkCollision=false); // 030519 kyo
	void EffectQuake(MAGICLIST* pMagic, int nCount, DWORD dwNow);
	void EffectDrop(MAGICLIST* pMagic, int nCount, DWORD dwNow);
	void EffectSpreadIrregular(MAGICLIST* pMagic, DWORD dwNow, const int nTile, const int nRan, int nLoop=2); //	030328 kyo
	
protected:
	/////////////////////////////////////////////////////////////////////////////
	// ����Ʈ ����
	struct EFFECT_INFO
	{
		LPCHARACTER pCaster; // ������
		LPCHARACTER pTarget; // �����
		int         nType;   // �����̳� ��ų��ȣ
		int         nEffect; // ����Ʈ ��ȣ 
		int         nSub;    // ����Ÿ��
		int         nDir;    // ����
		int         nSrcX;   // ���� ��ġ
		int         nSrcY;   
		int         nDstX;   // ��� ��ġ
		int         nDstY;
	};
	//
	/////////////////////////////////////////////////////////////////////////////
	void AddEffect(EFFECT_INFO* pEffect);
	void ClearCrash();
	
	protected:
		void Coordinates(double dAngle, int& rX, int& rY);
};

///////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_BATTLE_H__01911661_5F5A_426F_8E7B_2C16589738DD__INCLUDED_)
