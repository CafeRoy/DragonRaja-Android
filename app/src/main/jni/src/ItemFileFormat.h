#ifndef _ITEM_FILE_FORMAT_H_
#define _ITEM_FILE_FORMAT_H_

#include "StdAfx.h"

#ifndef ITEM_NAME_MAX
#define ITEM_NAME_MAX 31
#endif

// ★ 強制 1 Byte 對齊，由我們手動控制 Padding
#pragma pack(push, 1)

// =================================================================
// 1. 基底類別 CItem (修正為 20 Bytes)
// =================================================================
struct CItem_File
{
    unsigned int _vptr; // 32-bit vptr (4 bytes)

    int curr_value;
    int curr_value_sell;
    int curr_value_repair;
};

// =================================================================
// 2. 各個子類別的檔案格式
// =================================================================

// --- PLANT ---
struct CItem_Plant_File : public CItem_File
{
    int Item_id;
    char Name[ITEM_NAME_MAX];     // 31
    char Han_Name[ITEM_NAME_MAX]; // 31

    // ★★★ [修正 2] 補上 2 Bytes 對齊 Padding ★★★
    // 31 + 31 = 62。為了讓下一個 int lv 對齊 4 bytes，中間必有 2 bytes 間隙
    char _align_pad[2];

    int lv;
    int Image_Number;
    int hp;
    int ap;
    int mp;
    int nutrition1;
    int nutrition2;
    int nutrition3;
    int anti_poison;
    int anti_paralysis;
    int anti_magic;
    int anti_fire;
    int anti_ice;
    int anti_electric;
    int Time_maintain;
    int cure_disease1;
    int cure_disease2;
    int cure_disease3;
    int cure_disease4;
    int cure_disease5;
    int take_disease1;
    int take_disease2;
    int take_disease3;
    int take_disease4;
    int take_disease5;
    int weight;
    int circulation_limit;
    int max_duration;
    int Maximum;
    int object;
    int wear_able;
    int base_value;
    int base_store;
    int store_const;
    int store_plus_const;
    int Mouse_MR;
    int Mouse_MRD;
    int Mouse_MR_Effect;
    int Mouse_Mr_Wave;
    int Item_kind;
};

// --- MINERAL ---
struct CItem_Mineral_File : public CItem_File
{
    int Item_id;
    char Name[ITEM_NAME_MAX];
    char Han_Name[ITEM_NAME_MAX];

    char _align_pad[2]; // ★ 修正對齊

    int lv;
    int Image_Number;
    int weight;
    int object;
    int wear_able;
    int base_value;
    int base_store;
    int store_const;
    int store_plus_const;
    int Mouse_MR;
    int Mouse_MRD;
    int Mouse_MR_Effect;
    int Mouse_Mr_Wave;
    int Item_kind;
};

// --- HERB ---
struct CItem_Herb_File : public CItem_File
{
    int Item_id;
    char Name[ITEM_NAME_MAX];
    char Han_Name[ITEM_NAME_MAX];

    char _align_pad[2]; // ★ 修正對齊

    int lv;
    int Image_Number;
    int hp;
    int ap;
    int mp;
    int nutrition1;
    int nutrition2;
    int nutrition3;
    int anti_poison;
    int anti_paralysis;
    int anti_magic;
    int anti_fire;
    int anti_ice;
    int anti_electric;
    int Time_maintain;
    int cure_disease1;
    int cure_disease2;
    int cure_disease3;
    int cure_disease4;
    int cure_disease5;
    int cure_disease6;
    int take_disease1;
    int take_disease2;
    int take_disease3;
    int take_disease4;
    int take_disease5;
    int take_disease6;
    int Change_str;
    int Change_dex;
    int Change_con;
    int Change_wis;
    int Change_int;
    int Change_cha;
    int Change_Movp;
    int Change_end;
    int Change_mor;
    int Change_luc;
    int Change_WS;
    int Change_PS;
    int Time_maintain2;
    int Cure_poison;
    int Cure_stone_paralysis;
    int Cure_Mad;
    int Cure_sluggish;
    int Cure_Active;
    int ChangeNatural2;
    int Time_maintain3;
    int weight;
    int circulation_limit;
    int object;
    int wear_able;
    int base_value;
    int base_store;
    int store_const;
    int store_plus_const;
    int Mouse_MR;
    int Mouse_MRD;
    int Mouse_MR_Effect;
    int Mouse_Mr_Wave;
    int Item_kind;
};

// --- COOK ---
struct CItem_Cook_File : public CItem_File
{
    int Item_id;
    char Name[ITEM_NAME_MAX];
    char Han_Name[ITEM_NAME_MAX];

    char _align_pad[2]; // ★ 修正對齊

    int lv;
    int Image_Number;
    int hp;
    int ap;
    int mp;
    int nutrition1;
    int nutrition2;
    int nutrition3;
    int anti_poison;
    int anti_paralysis;
    int anti_magic;
    int anti_fire;
    int anti_ice;
    int anti_electric;
    int Time_maintain;
    int cure_disease1;
    int cure_disease2;
    int cure_disease3;
    int cure_disease4;
    int cure_disease5;
    int cure_disease6;
    int take_disease1;
    int take_disease2;
    int take_disease3;
    int take_disease4;
    int take_disease5;
    int take_disease6;
    int Change_str;
    int Change_dex;
    int Change_con;
    int Change_wis;
    int Change_int;
    int Change_cha;
    int Change_end;
    int Change_Movp;
    int Change_mor;
    int Change_luc;
    int Change_WS;
    int Change_Ps;
    int Time_maintain2;
    int weight;
    int circulation_limit;
    int object;
    int wear_able;
    int base_value;
    int base_store;
    int store_const;
    int store_plus_const;
    int Mouse_MR;
    int Mouse_MRD;
    int Mouse_MR_Effect;
    int Mouse_Mr_Wave;
    int Use_Junk;
    int Item_kind;
};

// --- POTION ---
struct CItem_Potion_File : public CItem_File
{
    int Item_id;
    char Name[ITEM_NAME_MAX];
    char Han_Name[ITEM_NAME_MAX];

    char _align_pad[2]; // ★ 修正對齊

    int lv;
    int Image_Number;
    int hp;
    int ap;
    int mp;
    int nutrition1;
    int nutrition2;
    int nutrition3;
    int anti_poison;
    int anti_paralysis;
    int anti_magic;
    int anti_fire;
    int anti_ice;
    int anti_electric;
    int Time_maintain;
    int cure_disease1;
    int cure_disease2;
    int cure_disease3;
    int cure_disease4;
    int cure_disease5;
    int cure_disease6;
    int take_disease1;
    int take_disease2;
    int take_disease3;
    int take_disease4;
    int take_disease5;
    int take_disease6;
    int Change_str;
    int Change_dex;
    int Change_con;
    int Change_wis;
    int Change_int;
    int Change_cha;
    int Change_end;
    int Change_movp;
    int Change_mor;
    int Change_luc;
    int Change_WS;
    int Change_PS;
    int Time_maintain2;
    int Cure_poison;
    int Cure_stone_paralysis;
    int Cure_Mad;
    int Cure_sluggish;
    int Cure_Active;
    int ChangeNatural2;
    int Time_maintain3;
    int weight;
    int circulation_limit;
    int object;
    int wear_able;
    int base_value;
    int base_store;
    int store_const;
    int store_plus_const;
    int Mouse_MR;
    int Mouse_MRD;
    int Mouse_MR_Effect;
    int Mouse_Mr_Wave;
    int Use_Junk;
    int Item_kind;
};

// --- TOOL ---
struct CItem_Tool_File : public CItem_File
{
    int Item_id;
    char Name[ITEM_NAME_MAX];
    char Han_Name[ITEM_NAME_MAX];

    char _align_pad[2]; // ★ 修正對齊

    int lv;
    int Image_Number;
    int Repairable;
    int Repair_min;
    int Repair_max;
    int Repair_Skill2;
    int Repair_Skill2_min;
    int Repair_Res1;
    int Amt_of_Repair_Res1;
    int Repair_Res2;
    int Amt_of_Repair_Res2;
    int Repair_Res3;
    int Amt_of_Repair_Res3;
    int Repair_Tool;
    int Amt_of_abrasion;
    int Change_str;
    int Change_dex;
    int Change_con;
    int Change_wis;
    int Change_int;
    int Change_cha;
    int Change_end;
    int Change_movp;
    int Change_mor;
    int Change_luc;
    int Change_ws;
    int Change_ps;
    int Change_mag;
    int Change_dev;
    int shape_woman;
    int shape_man;
    int success_rate;
    int abrasion_per_turn;
    int weight;
    int max_duration;
    int object;
    int wear_able;
    int base_value;
    int base_store;
    int store_const;
    int store_plus_const;
    int Mouse_MR;
    int Mouse_MRD;
    int Mouse_MR_Effect;
    int Mouse_Mr_Wave;
    int Item_kind;
};

// --- WEAPON ---
struct CItem_Weapon_File : public CItem_File
{
    int Item_id;
    char Name[ITEM_NAME_MAX];
    char Han_Name[ITEM_NAME_MAX];

    char _align_pad[2]; // ★ 修正對齊

    int lv;
    int Image_Number;
    int Repairable;
    int Repair_min;
    int Repair_max;
    int Repair_Skill2;
    int Repair_Skill2_min;
    int Repair_Res1;
    int Amt_of_Repair_Res1;
    int Repair_Res2;
    int Amt_of_Repair_Res2;
    int Repair_Res3;
    int Amt_of_Repair_Res3;
    int Repair_Tool;
    int Amt_of_abrasion;
    int Need2_lv;
    int Need2_str;
    int Need2_dex;
    int Need2_con;
    int Need2_wis;
    int Need2_int;
    int Need2_cha;
    int Need2_end;
    int Need2_Movp;
    int Need2_mor;
    int Need2_luc;
    int Need2_ws;
    int Need2_ps;
    int Need2_fame;
    int Need2_rank;
    int Need2_gender;
    int Need2_race;
    int Need2_min_age;
    int Need2_max_age;
    int Need3_lv;
    int Need3_str;
    int Need3_dex;
    int Need3_con;
    int Need3_wis;
    int Need3_int;
    int Need3_cha;
    int Need3_end;
    int Need3_Movp;
    int Need3_mor;
    int Need3_luc;
    int Need3_ws;
    int Need3_ps;
    int Need3_fame;
    int Need3_rank;
    int Need3_gender;
    int Need3_race;
    int Need3_min_age;
    int Need3_max_age;
    int Class_Warrior;
    int Class_Archer;
    int Class_Thief;
    int Class_Wizard;
    int Class_Cleric;
    int Change_str;
    int Change_dex;
    int Change_con;
    int Change_wis;
    int Change_int;
    int Change_cha;
    int Change_end;
    int Change_Movp;
    int Change_mor;
    int Change_luc;
    int Change_ws;
    int Change_ps;
    int Change_mag;
    int Change_dev;
    int casting_Time;
    int Change_Life_max;
    int Change_mana_max;
    int Change_Divine_Max;
    int Change_health_max;
    int Increase_Poisoning;
    int Increase_Stone;
    int Increase_Magic;
    int Increase_Fire;
    int Increase_Ice;
    int Increase_lightning;
    int imunity_Cure_1;
    int Imunity_Cure_2;
    int Imunity_Cure_3;
    int Imunity_Cure_4;
    int Imunity_Cure_5;
    int Imunity_Cure_6;
    int SkillEffect;
    int shape_woman;
    int shape_man;
    int Skill_Ability;
    int Damage;
    int Hit_rate;
    int Defense_rate;
    int ArorDH;
    int nRange;
    int nWeaponSpeed;
    int effect;
    int weight;
    int max_duration;
    int object;
    int wear_able;
    int base_value;
    int base_store;
    int store_const;
    int store_plus_const;
    int Item_kind;
    int Mouse_MR;
    int Mouse_MRD;
    int Mouse_MR_Effect;
    int Mouse_Mr_Wave;
    int kind_of_magic;
    int Incr_damage;
};

// --- DISPOSABLE ---
struct CItem_Disposable_File : public CItem_File
{
    int Item_id;
    char Name[ITEM_NAME_MAX];
    char Han_Name[ITEM_NAME_MAX];

    char _align_pad[2]; // ★ 修正對齊

    int lv;
    int Image_Number;
    int Need3_lv;
    int Need3_str;
    int Need3_dex;
    int Need3_con;
    int Need3_wis;
    int Need3_int;
    int Need3_cha;
    int Need3_end;
    int Need3_Movp;
    int Need3_mor;
    int Need3_luc;
    int Need3_ws;
    int Need3_ps;
    int Need3_fame;
    int Need3_rank;
    int Need3_gender;
    int Need3_race;
    int Need3_min_age;
    int Need3_max_age;
    int Class_Warrior;
    int Class_Archer;
    int Class_Thief;
    int Class_Wizard;
    int Class_Cleric;
    int shape_woman;
    int shape_man;
    int Skill_Ability;
    int Damage;
    int Hit_rate;
    int fire_speed;
    int nRange;
    int damage_rage;
    int Through;
    int nWeaponSpeed;
    int effect_num;
    int weight;
    int object;
    int wear_able;
    int base_value;
    int base_store;
    int store_const;
    int store_plus_const;
    int Mouse_MR;
    int Mouse_MRD;
    int Mouse_MR_Effect;
    int Mouse_Mr_Wave;
    int Quantity;
    int Item_kind;
};

// --- ARMOR ---
struct CItem_Armor_File : public CItem_File
{
    int Item_id;
    char Name[ITEM_NAME_MAX];
    char Han_Name[ITEM_NAME_MAX];

    char _align_pad[2]; // ★ 修正對齊

    int lv;
    int Image_Number;
    int Repairable;
    int Repair_min;
    int Repair_max;
    int Repair_Skill2;
    int Repair_Skill2_min;
    int Repair_Res1;
    int Amt_of_Repair_Res1;
    int Repair_Res2;
    int Amt_of_Repair_Res2;
    int Repair_Res3;
    int Amt_of_Repair_Res3;
    int Repair_Tool;
    int Amt_of_abrasion;
    int Need2_lv;
    int Need2_str;
    int Need2_dex;
    int Need2_con;
    int Need2_wis;
    int Need2_int;
    int Need2_cha;
    int Need2_end;
    int Need2_Movp;
    int Need2_mor;
    int Need2_luc;
    int Need2_ws;
    int Need2_ps;
    int Need2_fame;
    int Need2_rank;
    int Need2_gender;
    int Need2_race;
    int Need2_min_age;
    int Need2_max_age;
    int Need3_lv;
    int Need3_str;
    int Need3_dex;
    int Need3_con;
    int Need3_wis;
    int Need3_int;
    int Need3_cha;
    int Need3_end;
    int Need3_Movp;
    int Need3_mor;
    int Need3_luc;
    int Need3_ws;
    int Need3_ps;
    int Need3_fame;
    int Need3_rank;
    int Need3_gender;
    int Need3_race;
    int Need3_min_age;
    int Need3_max_age;
    int Class_Warrior;
    int Class_Archer;
    int Class_Thief;
    int Class_Wizard;
    int Class_Cleric;
    int Change_str;
    int Change_dex;
    int Change_con;
    int Change_wis;
    int Change_int;
    int Change_cha;
    int Change_end;
    int Change_Movp;
    int Change_mor;
    int Change_luc;
    int Change_ws;
    int Change_ps;
    int Change_mag;
    int Change_dev;
    int casting_Time;
    int Change_Life_max;
    int Change_mana_max;
    int Change_Divine_Max;
    int Change_health_max;
    int Increase_Poisoning;
    int Increase_Stone;
    int Increase_Magic;
    int Increase_Fire;
    int Increase_Ice;
    int Increase_lightning;
    int imunity_Cure_1;
    int Imunity_Cure_2;
    int Imunity_Cure_3;
    int Imunity_Cure_4;
    int Imunity_Cure_5;
    int Imunity_Cure_6;
    int SkillEffect;
    int shape_woman;
    int shape_man;
    int Skill_Ability;
    int Damage;
    int Hit_rate;
    int Defense_rate;
    int AR_or_DH;
    int Defense_power;
    int body;
    int arms;
    int head;
    int leg;
    int neck;
    int hand;
    int weight;
    int max_duration;
    int object;
    int wear_able;
    int base_value;
    int base_store;
    int store_const;
    int store_plus_const;
    int Mouse_MR;
    int Mouse_MRD;
    int Mouse_MR_Effect;
    int Mouse_Mr_Wave;
    int Item_kind;
    int kind_of_magic;
    int Incr_damage;
};

// --- ACCESSORY ---
struct CItem_Accessory_File : public CItem_File
{
    int Item_id;
    char Name[ITEM_NAME_MAX];
    char Han_Name[ITEM_NAME_MAX];

    char _align_pad[2]; // ★ 修正對齊

    int lv;
    int Image_Number;
    int Repairable;
    int Repair_min;
    int Repair_max;
    int Repair_Skill2;
    int Repair_Skill2_min;
    int Repair_Res1;
    int Amt_of_Repair_Res1;
    int Repair_Res2;
    int Amt_of_Repair_Res2;
    int Repair_Res3;
    int Amt_of_Repair_Res3;
    int Repair_Tool;
    int Amt_of_abrasion;
    int Need2_lv;
    int Need2_str;
    int Need2_dex;
    int Need2_con;
    int Need2_wis;
    int Need2_int;
    int Need2_cha;
    int Need2_end;
    int Need2_Movp;
    int Need2_mor;
    int Need2_luc;
    int Need2_ws;
    int Need2_ps;
    int Need2_fame;
    int Need2_rank;
    int Need2_gender;
    int Need2_race;
    int Need2_min_age;
    int Need2_max_age;
    int Need3_lv;
    int Need3_str;
    int Need3_dex;
    int Need3_con;
    int Need3_wis;
    int Need3_int;
    int Need3_cha;
    int Need3_end;
    int Need3_Movp;
    int Need3_mor;
    int Need3_luc;
    int Need3_ws;
    int Need3_ps;
    int Need3_fame;
    int Need3_rank;
    int Need3_gender;
    int Need3_race;
    int Need3_min_age;
    int Need3_max_age;
    int Class_Warrior;
    int Class_Archer;
    int Class_Thief;
    int Class_Wizard;
    int Class_Cleric;
    int Change_str;
    int Change_dex;
    int Change_con;
    int Change_wis;
    int Change_int;
    int Change_cha;
    int Change_end;
    int Change_Movp;
    int Change_mor;
    int Change_luc;
    int Change_ws;
    int Change_ps;
    int Change_mag;
    int Change_dev;
    int casting_Time;
    int Change_Life_max;
    int Change_mana_max;
    int Change_Divine_Max;
    int Change_health_max;
    int Increase_Poisoning;
    int Increase_Stone;
    int Increase_Magic;
    int Increase_Fire;
    int Increase_Ice;
    int Increase_lightning;
    int imunity_Cure_1;
    int Imunity_Cure_2;
    int Imunity_Cure_3;
    int Imunity_Cure_4;
    int Imunity_Cure_5;
    int Imunity_Cure_6;
    int Skill_effect;
    int Skill_Ability;
    int Damage;
    int Hit_rate;
    int Defense_rate;
    int AR_or_DH;
    int Defense_power;
    int body;
    int arms;
    int head;
    int leg;
    int neck;
    int hand;
    int JobSkill_Farming;
    int JobSkill_Mining;
    int JobSkill_Fishing;
    int JobSkill_Cutdown;
    int JobSkill_Butchering;
    int JobSkill_Herb_digging;
    int JobSkill_Herding;
    int JobSkill_Carpentary;
    int JobSkill_Blacksmithy;
    int JobSkill_Cooking;
    int JobSkill_Tailoring;
    int JobSkill_Bowcraft;
    int JobSkill_Alchemy;
    int JobSkill_Candle;
    int JobSkill_repair;
    int JobSkill_reutil;
    int JobSkill_music;
    int JobSkill_detect;
    int JobSkill_hide;
    int JobSkill_campiing;
    int JobSkill_taming;
    int comm_animal;
    int comm_human;
    int comm_orc;
    int comm_elf;
    int comm_fairy;
    int comm_hobbit;
    int comm_dwarf;
    int comm_dragon;
    int animal_lore;
    int arms_lore;
    int evaluate_int;
    int evaluate_wis;
    int analomy;
    int item_ident;
    int taste_ident;
    int Appraisal;
    int healing;
    int spirituality;
    int poisoning;
    int entrapment;
    int scrooling;
    int cunning;
    int bomb_play;
    int tactice;
    int crapple;
    int sword;
    int archey;
    int fencing;
    int mace;
    int pierce;
    int whid;
    int hurt;
    int parrying;
    int double_sword;
    int double_mace;
    int weight;
    int max_duration;
    int Durationpermin;
    int object;
    int wear_able;
    int base_value;
    int base_store;
    int store_const;
    int store_plus_const;
    int Mouse_MR;
    int Mouse_MRD;
    int Mouse_MR_Effect;
    int Mouse_Mr_Wave;
    int Item_kind;
    int kind_of_magic;
    int Incr_damage;
};

// --- ETC ---
struct CItem_Etc_File : public CItem_File
{
    int Item_id;
    char Name[ITEM_NAME_MAX];
    char Han_Name[ITEM_NAME_MAX];

    char _align_pad[2]; // ★ 修正對齊

    int lv;
    int Image_Number;
    int Repairable;
    int Repair_min;
    int Repair_max;
    int Repair_Skill2;
    int Repair_Skill2_min;
    int Repair_Res1;
    int Amt_of_Repair_Res1;
    int Repair_Res2;
    int Amt_of_Repair_Res2;
    int Repair_Res3;
    int Amt_of_Repair_Res3;
    int Repair_Tool;
    int Amt_of_abrasion;
    int Need2_lv;
    int Need2_str;
    int Need2_dex;
    int Need2_con;
    int Need2_wis;
    int Need2_int;
    int Need2_cha;
    int Need2_end;
    int Need2_Movp;
    int Need2_mor;
    int Need2_luc;
    int Need2_ws;
    int Need2_ps;
    int Need2_Magery;
    int Need2_Orison;
    int Need2_fame;
    int Need2_rank;
    int Need2_gender;
    int Need2_race;
    int Need2_min_age;
    int Need2_max_age;
    int Need3_lv;
    int Need3_str;
    int Need3_dex;
    int Need3_con;
    int Need3_wis;
    int Need3_int;
    int Need3_cha;
    int Need3_end;
    int Need3_Movp;
    int Need3_mor;
    int Need3_luc;
    int Need3_ws;
    int Need3_ps;
    int Need3_fame;
    int Need3_rank;
    int Need3_gender;
    int Need3_race;
    int Need3_min_age;
    int Need3_max_age;
    int Class_Warrior;
    int Class_Archer;
    int Class_Thief;
    int Class_Wizard;
    int Class_Cleric;
    int Change_str;
    int Change_dex;
    int Change_con;
    int Change_wis;
    int Change_int;
    int Change_cha;
    int Change_end;
    int Change_mor;
    int Change_luc;
    int Change_ws;
    int Change_ps;
    int Change_mag;
    int Change_dev;
    int casting_Time;
    int Change_Life_max;
    int Change_mana_max;
    int Change_Divine_Max;
    int Change_health_max;
    int Increase_Poisoning;
    int Increase_Stone;
    int Increase_Magic;
    int Increase_Fire;
    int Increase_Ice;
    int Increase_lightning;
    int imunity_Cure_1;
    int Imunity_Cure_2;
    int Imunity_Cure_3;
    int Imunity_Cure_4;
    int Imunity_Cure_5;
    int Imunity_Cure_6;
    int Skill_effect;
    int Substitution_magic;
    int Skill_Ability;
    int Damage;
    int Hit_rate;
    int Defense_rate;
    int AR_or_DH;
    int Defense_power;
    int body;
    int arms;
    int head;
    int leg;
    int neck;
    int hand;
    int JobSkill_Farming;
    int JobSkill_Mining;
    int JobSkill_Fishing;
    int JobSkill_Cutdown;
    int JobSkill_Butchering;
    int JobSkill_Herb_digging;
    int JobSkill_Herding;
    int JobSkill_Carpentary;
    int JobSkill_Blacksmithy;
    int JobSkill_Cooking;
    int JobSkill_Tailoring;
    int JobSkill_Bowcraft;
    int JobSkill_Alchemy;
    int JobSkill_Candle;
    int JobSkill_repair;
    int JobSkill_reutil;
    int JobSkill_music;
    int JobSkill_detect;
    int JobSkill_hide;
    int JobSkill_campiing;
    int JobSkill_taming;
    int comm_animal;
    int comm_human;
    int comm_orc;
    int comm_elf;
    int comm_fairy;
    int comm_hobbit;
    int comm_dwarf;
    int comm_dragon;
    int animal_lore;
    int arms_lore;
    int evaluate_int;
    int evaluate_wis;
    int analomy;
    int item_ident;
    int taste_ident;
    int Appraisal;
    int healing;
    int spirituality;
    int poisoning;
    int entrapment;
    int scrooling;
    int cunning;
    int bomb_play;
    int tactice;
    int crapple;
    int sword;
    int archey;
    int fencing;
    int mace;
    int pierce;
    int whid;
    int hurt;
    int parrying;
    int double_sword;
    int double_mace;
    int change_music;
    int change_detect;
    int change_hide;
    int change_amping;
    int change_taming;
    int weight;
    int max_duration;
    int object;
    int wear_able;
    int base_value;
    int base_store;
    int store_const;
    int store_plus_const;
    int Item_kind;
    int Mouse_MR;
    int Mouse_MRD;
    int Mouse_MR_Effect;
    int Mouse_Mr_Wave;
    int Use_Junk;
    int kind_of_magic;
    int Incr_damage;
};

#pragma pack(pop)

#endif // _ITEM_FILE_FORMAT_H_