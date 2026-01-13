#include "StdAfx.h"
#include <sstream>
#include <filesystem> // C++17 for path handling
#include <numeric>    // For std::accumulate
#include <iostream>   // For error messages
#include "Hangul.h"
#include "dragon.h"
#include "SmallMenuSet.h"
#include "Menu.h"
#include "SmallMenu.h"
#include "Hong_Sprite.h"
#include <cstdio>
#include "convert565to555.h" //todo useless
#include "Hong_Sub.h"
#include "Tool.h"
#include "MenuSet.h"
//#include "MenuDef.h"			// 010904 LTS
#include "ItemTable.h"
#include "Skill.h"
#include "Effect.h"
#include "NationSys.h"
#include "gameproc.h"
#include "MenuSet.h"
#include "SymbolMgr_Client.h" // soto-1.02
#include "ItemFileFormat.h"
//#define LIGHT_GM_CLIENT //Added by eleval 01/06/09 for light gm client
void CheckAbility();
CSymbolMgr_Client	g_SymBolMgr; // soto-1.02 reece check out furth

t_ItemMutantKind ItemMutantKind[MAX_ITEM_MUTANT_KIND];
t_ItemGrade ItemGrade[MAX_ITEM_GRADE];
t_ItemRare ItemRare[MAX_ITEM_RARE];
RareMain   itemtime[MAX_ITEM_GRADE];
extern bool IsEquipAbleCheckAbleDay(int start, int end);//011215 lsw
extern void	SendReCallItemFromServer(int type);
extern void DeleteItemByItemNumber(int item);
//011025 lsw >
extern DWORD g_skillmaketime;
//011025 lsw <
extern	ItemAttr QuickItemAttr[7];
//extern void RectTextPut(int x, int y, int nWidth, char *sSource);
//extern struct		InterfaceFlag main_interface;

Spr		Icon1[ITEM_MAX][10];
Spr		Icon2[ITEM_MAX][10];
Spr		Icon3[ITEM_MAX][10];
Spr		Icon4[ITEM_MAX][10];
Spr		Icon5[ITEM_MAX][10];
Spr		Icon6[ITEM_MAX][10];
Spr		Icon7[ITEM_MAX][10];
Spr		Icon8[ITEM_MAX][10];
Spr		Icon9[ITEM_MAX][10];
Spr		Icon10[ITEM_MAX][10];
Spr		Icon11[ITEM_MAX][10];
Spr		Icon12[ITEM_MAX][10];
Spr		Icon13[ITEM_MAX][10];


int Num_Of_CItem_Plant;
int Num_Of_CItem_Mineral;
int Num_Of_CItem_Herb;
int Num_Of_CItem_Cook;
int Num_Of_CItem_Potion;
int Num_Of_CItem_Tool;
int Num_Of_CItem_Weapon;
int Num_Of_CItem_Disposable;
int Num_Of_CItem_Armor;
int Num_Of_CItem_Accessory;
int Num_Of_CItem_Etc;

struct CItem_List Item_Ref;			// ������//reece
//CItem_Plant *ItemPlant ;

//void GetName(char* temp, char* name, int n=0);
bool ReadMagicIconImageNotSameAsSpell(int type, int num);
bool ReadMagicIconImage(int type, int num);
bool ReadSkillIcon(int num);
int GetItemAttr(int item_id, int attr);

void CalcGuildToSkill();	// ��ų�� �迭 ���� �з�

int menu_to_set[] = { 2, 1, 5, 3, 12, 9, 6, 8, 4, 7, 11 };		// �޴� ��ȣ�� ������ ���� ���̺�� �ִ� ��ȣ�� ��ġ��Ų��.
char g_MagicName[MAX_MAGIC_][31]={0};
int n_MagicBagic[2][12][15] =
{ //< CSD-020702
	{
		{0, },
		{0, },
		{0, },
		{0, },
		{0, },
		{0, },
		{0, },
		{0, },
		{0, },
		{0, },
		{0, },
		{0, },
	},
	{
		{0, },
		{0, },
		{0, },
		{0, },
		{0, },
		{0, },
		{0, },
		{0, },
		{0, },
		{0, },
		{0, },
		{0, },
	}
};//> CSD-020702

/*###########################################################################
			������ Ŭ���� ��� �Լ� �� ������ ���� �Լ�
###########################################################################*/
void GetName(char* temp, char* name, FILE* fp, int n = 0)
{
	char* source;
	source = name;

	int i = 0;
	int max = ITEM_NAME_MAX;

	if (n) if (n != ITEM_NAME_MAX + 9) max = n - 9;

	while (*(temp + i))
	{
		if (*(temp + i) == '<') break;
		i++;
	}
	i++;
	while (*(temp + i))
	{
		if (*(temp + i) == '>')
		{
			*source = '\0';
			fseek(fp, -max - 7 + i, SEEK_CUR);
			return;
		}
		*source = *(temp + i);
		source++;
		i++;
	}
}


int ReadItemTableFunction(char* text, int& Item_id, char* Name, char* Han_Name, int* item_value, int max)
{
	char* token;

	token = strtok(text, "\t\n");
	Item_id = atoi(token);

	token = strtok(NULL, "\t\n");
	EatRearWhiteChar(token);
	strcpy(Name, token);

	token = strtok(NULL, "\t\n");
	EatRearWhiteChar(token);
	strcpy(Han_Name, token);


	for (int i = 0; i < max; i++)
	{
		token = strtok(NULL, "\t\n");
		*item_value = atoi(token);
		item_value++;
	}
	return 1;
}

int CItem_Plant::ReadItemTable(char* temp)
{
	int max = ITEM_Plant_MAX - 3;
	return ReadItemTableFunction(temp, Item_id, Name, Han_Name, &lv, max);
}

int CItem_Mineral::ReadItemTable(char* temp)
{
	int max = ITEM_Mineral_MAX - 3;
	return ReadItemTableFunction(temp, Item_id, Name, Han_Name, &lv, max);
}

int CItem_Herb::ReadItemTable(char* temp)
{
	int max = ITEM_Herb_MAX - 3;
	return ReadItemTableFunction(temp, Item_id, Name, Han_Name, &lv, max);
}

int CItem_Cook::ReadItemTable(char* temp)
{
	int max = ITEM_Cook_MAX - 3;
	return ReadItemTableFunction(temp, Item_id, Name, Han_Name, &lv, max);
}

int CItem_Potion::ReadItemTable(char* temp)
{
	int max = ITEM_Potion_MAX - 3;
	return ReadItemTableFunction(temp, Item_id, Name, Han_Name, &lv, max);
}

int CItem_Tool::ReadItemTable(char* temp)
{
	int max = ITEM_Tool_MAX - 3;
	return ReadItemTableFunction(temp, Item_id, Name, Han_Name, &lv, max);
}

int CItem_Weapon::ReadItemTable(char* temp)
{
	int max = ITEM_Weapon_MAX - 3;
	return ReadItemTableFunction(temp, Item_id, Name, Han_Name, &lv, max);
}

int CItem_Disposable::ReadItemTable(char* temp)
{
	int max = ITEM_Disposable_MAX - 3;
	return ReadItemTableFunction(temp, Item_id, Name, Han_Name, &lv, max);
}

int CItem_Armor::ReadItemTable(char* temp)
{
	int max = ITEM_Armor_MAX - 3;
	return ReadItemTableFunction(temp, Item_id, Name, Han_Name, &lv, max);
}

int CItem_Accessory::ReadItemTable(char* temp)
{
	int max = ITEM_Accessory_MAX - 3;
	return ReadItemTableFunction(temp, Item_id, Name, Han_Name, &lv, max);
}

int CItem_Etc::ReadItemTable(char* temp)
{
	int max = ITEM_Etc_MAX - 3;
	return ReadItemTableFunction(temp, Item_id, Name, Han_Name, &lv, max);
}


/*
CItem_Plant* Plant = NULL;
CItem_Mineral* Mineral = NULL;
CItem_Herb* Herb = NULL;
CItem_Cook* Cook = NULL;
CItem_Potion* Potion = NULL;
CItem_Tool* Tool = NULL;
CItem_Weapon* Weapon = NULL;
CItem_Disposable* Disposable = NULL;
CItem_Armor* Armor = NULL;
CItem_Accessory* Accessory = NULL;
CItem_Etc* Etc = NULL;
*/

std::vector<CItem_Plant> Plant;
std::vector<CItem_Mineral> Mineral;
std::vector<CItem_Herb> Herb;
std::vector<CItem_Cook> Cook;
std::vector<CItem_Potion> Potion;
std::vector<CItem_Tool> Tool;
std::vector<CItem_Weapon> Weapon;
std::vector<CItem_Disposable> Disposable;
std::vector<CItem_Armor> Armor;
std::vector<CItem_Accessory> Accessory;
std::vector<CItem_Etc> Etc;

//############################ ������ �ε� ##############################
extern int YesOrNo(char* s, char* title);
extern BOOL  ShowErrorMessage(std::string msg);


void FreeItemTable()
{/*
	SAFE_DELETE_ARRAY(Plant);
	SAFE_DELETE_ARRAY(Mineral);
	SAFE_DELETE_ARRAY(Herb);
	SAFE_DELETE_ARRAY(Cook);
	SAFE_DELETE_ARRAY(Potion);
	SAFE_DELETE_ARRAY(Tool);
	SAFE_DELETE_ARRAY(Weapon);
	SAFE_DELETE_ARRAY(Disposable);
	SAFE_DELETE_ARRAY(Armor);
	SAFE_DELETE_ARRAY(Accessory);
	SAFE_DELETE_ARRAY(Etc);*/
}


/////////////////////////////////////////////
// ������ ���̺�(bin) �о���� Ư���� ���
int GetLineItemByItemTableBin(char* filename)
{
    int line = 0;

    // [Android] 1. 路徑修正：移除開頭的 ./ (如果有的話)
    std::string path = filename;
    if (path.find("./") == 0) path = path.substr(2);

    // [Android] 2. 使用 SDL_RWops 開啟檔案
    // 這能確保讀取 APK 內的 Assets 或者是存儲空間的檔案
    SDL_RWops* fp = SDL_RWFromFile(path.c_str(), "rb");

    if (fp)
    {
        // [Android] 3. 讀取數據
        // 參數順序: (context, ptr, size, maxnum)
        SDL_RWread(fp, &line, sizeof(int), 1);

        // [Android] 4. 關閉檔案
        SDL_RWclose(fp);
    }
    else
    {
        // 選用：如果要除錯，可以印出讀取失敗的 Log
        SDL_Log("GetLineItemByItemTableBin Failed: %s", path.c_str());
    }

    return line;
}

template <typename T_GameItem, typename T_FileItem, typename T_Container>
bool ReadBinaryItemBatch(SDL_RWops* fp, CHSEL_STREAM& ItemDe, T_Container& container, int count, int maxAttr, int typeIndex)
{
    // 計算差異 (子類別數據大小)
    // 假設 FileItem 是 [Base(16) + Data], GameItem 是 [Base(24) + Data]
    const int DATA_SIZE = sizeof(T_FileItem) - sizeof(CItem_File);
    //SDL_Log("T_GameItem=%zu sizeof(T_FileItem)=%zu sizeof(CItem_File)=%zu ", sizeof(T_GameItem), sizeof(T_FileItem), sizeof(CItem_File));
    for (int ct = 0; ct < count; ct++) {
        T_FileItem temp_file;

        // 1. 讀取 (使用正確的檔案大小)
        if (SDL_RWread(fp, &temp_file, sizeof(T_FileItem), 1) != 1) {
            SDL_Log("Read Error: Type %d, Index %d", typeIndex, ct);
            return false;
        }

        // 2. 解密
        ItemDe.Decrypt(reinterpret_cast<char*>(&temp_file), sizeof(T_FileItem));

        // 3. CRC 檢查
        int crc1 = 0;
        const int crc2 = ItemDe.GetCRCConvertInt();
        if (SDL_RWread(fp, &crc1, sizeof(int), 1) != 1) return false;

        if (crc1 != crc2) {
            SDL_Log("CRC Error: Type %d, Index %d", typeIndex, ct);
            return false;
        }

        // 4. ★ 轉換 (Convert) ★
        T_GameItem& game_item = container[ct];

        // A. 複製基底類別 CItem 的變數 (跳過 vptr)
        game_item.curr_value        = temp_file.curr_value;
        game_item.curr_value_sell   = temp_file.curr_value_sell;
        game_item.curr_value_repair = temp_file.curr_value_repair;

        // 來源 (File)：跳過 CItem_File (它是我們手動定義的，固定 20 bytes)
        char* pSrc = (char*)&temp_file + sizeof(CItem_File);

        // 目的 (Game)：不要用 sizeof(CItem)，改用真實數據偏移量
        // vptr (指標大小) + 3個 int (12 bytes)
        const int BASE_DATA_OFFSET = sizeof(void*) + (3 * sizeof(int));

        char* pDest = (char*)&game_item + BASE_DATA_OFFSET;

        // 計算要複製的長度
        // 檔案結構總長 - 檔案Header長度 = 剩餘數據長度
        const int COPY_SIZE = sizeof(T_FileItem) - sizeof(CItem_File);

        memcpy(pDest, pSrc, COPY_SIZE);

        // 5. 初始化
        container[ct].CopyItemClass(&game_item, maxAttr);
    }
    return true;
}

// ============================================================================
// 主函式
// ============================================================================
void ItemRead()
{
    // 步驟 1: 獲取每個物品類型的數量
    struct ItemTypeInfo {
        int& count_ref;
        const char* id;
    };
    ItemTypeInfo item_types[] = {
            {Num_Of_CItem_Plant, "0"}, {Num_Of_CItem_Mineral, "1"}, {Num_Of_CItem_Herb, "2"},
            {Num_Of_CItem_Cook, "3"}, {Num_Of_CItem_Potion, "4"}, {Num_Of_CItem_Tool, "5"},
            {Num_Of_CItem_Weapon, "6"}, {Num_Of_CItem_Disposable, "7"}, {Num_Of_CItem_Armor, "8"},
            {Num_Of_CItem_Accessory, "9"}, {Num_Of_CItem_Etc, "10"}
    };

    for (auto& type_info : item_types) {
        char filename_txt[256], filename_bin[256];
        // [Android] 路徑修正：移除開頭的 ./
        snprintf(filename_txt, sizeof(filename_txt), "Itemtable/itemtable%s.txt", type_info.id);
        snprintf(filename_bin, sizeof(filename_bin), "Itemtable/itemtable%s.bin", type_info.id);

        // 注意：GetTextLine 和 GetLineItemByItemTableBin 也需要進入函式內部修改為 SDL 版本
        // 這裡假設您已經改好了這兩個函式
#ifdef _DEBUG
        // 在 Android 上即使是 Debug 模式，通常也沒有 txt 檔，建議優先讀 bin
       #ifdef __ANDROID__
           type_info.count_ref = GetLineItemByItemTableBin(filename_bin);
       #else
           type_info.count_ref = GetTextLine(filename_txt);
           if (!type_info.count_ref) type_info.count_ref = GetLineItemByItemTableBin(filename_bin);
       #endif
#else
        type_info.count_ref = GetLineItemByItemTableBin(filename_bin);
#endif
    }

    if (!Num_Of_CItem_Plant || !Num_Of_CItem_Mineral || !Num_Of_CItem_Herb || !Num_Of_CItem_Cook
        || !Num_Of_CItem_Potion || !Num_Of_CItem_Tool || !Num_Of_CItem_Weapon || !Num_Of_CItem_Disposable
        || !Num_Of_CItem_Armor || !Num_Of_CItem_Accessory || !Num_Of_CItem_Etc)
    {
        JustMsg(lan->OutputMessage(6, 20));
        return;
    }

    // 賦值提前 (保持您原本的修正)
    Item_Ref.nItem[0] = Num_Of_CItem_Plant;
    Item_Ref.nItem[1] = Num_Of_CItem_Mineral;
    Item_Ref.nItem[2] = Num_Of_CItem_Herb;
    Item_Ref.nItem[3] = Num_Of_CItem_Cook;
    Item_Ref.nItem[4] = Num_Of_CItem_Potion;
    Item_Ref.nItem[5] = Num_Of_CItem_Tool;
    Item_Ref.nItem[6] = Num_Of_CItem_Weapon;
    Item_Ref.nItem[7] = Num_Of_CItem_Disposable;
    Item_Ref.nItem[8] = Num_Of_CItem_Armor;
    Item_Ref.nItem[9] = Num_Of_CItem_Accessory;
    Item_Ref.nItem[10] = Num_Of_CItem_Etc;

    // 步驟 2: Resize (保持不變)
    Plant.resize(Num_Of_CItem_Plant);
    Mineral.resize(Num_Of_CItem_Mineral);
    Herb.resize(Num_Of_CItem_Herb);
    Cook.resize(Num_Of_CItem_Cook);
    Potion.resize(Num_Of_CItem_Potion);
    Tool.resize(Num_Of_CItem_Tool);
    Weapon.resize(Num_Of_CItem_Weapon);
    Disposable.resize(Num_Of_CItem_Disposable);
    Armor.resize(Num_Of_CItem_Armor);
    Accessory.resize(Num_Of_CItem_Accessory);
    Etc.resize(Num_Of_CItem_Etc);

    const int ItemAttrMax[11] = {
            ITEM_Plant_MAX, ITEM_Mineral_MAX, ITEM_Herb_MAX, ITEM_Cook_MAX, ITEM_Potion_MAX, ITEM_Tool_MAX,
            ITEM_Weapon_MAX, ITEM_Disposable_MAX, ITEM_Armor_MAX, ITEM_Accessory_MAX, ITEM_Etc_MAX,
    };

    // 步驟 3: 核心讀取邏輯
    for (int i = 0; i < Num_Of_Item_Type; i++)
    {
        char filename[MAX_FILENAME], filenameBin[MAX_FILENAME];
        // [Android] 路徑修正
        snprintf(filename, MAX_FILENAME, "Itemtable/itemtable%d.txt", i);
        snprintf(filenameBin, MAX_FILENAME, "Itemtable/itemtable%d.bin", i);

        // 標記是否已經成功從 Binary 讀取
        bool bLoadedFromBin = false;

        // -----------------------------------------------------------
        // 優先讀取 Binary (.bin) [RELEASE 模式 或 Android 強制]
        // -----------------------------------------------------------
        // 在 Android 上我們強制優先讀取 .bin，因為無法生成它
#if defined(_DEBUG) && !defined(__ANDROID__)
        bool tryTextFirst = true;
#else
        bool tryTextFirst = false;
#endif

        if (!tryTextFirst)
        {
            SDL_RWops* fp = SDL_RWFromFile(filenameBin, "rb");
            if (fp)
            {
                CHSEL_STREAM ItemDe;
                HSEL_INITIAL deinit;
                int iLineNum = 0;
                SDL_RWread(fp, &iLineNum, sizeof(int), 1);
                SDL_RWread(fp, &deinit, sizeof(HSEL_INITIAL), 1);

                if (!ItemDe.Initial(deinit)) {
                    SDL_Log("ItemTable make bin Error");
                    SDL_RWclose(fp);
                    continue;
                }

                int iNumOfItemTable = 0;
                SDL_RWread(fp, &iNumOfItemTable, sizeof(int), 1);
                ItemDe.Decrypt(reinterpret_cast<char*>(&iNumOfItemTable), sizeof(int));

                if (iLineNum != iNumOfItemTable) {
                    SDL_Log("ItemTable Read Error (Line Num mismatch) %d", i);
                    SDL_RWclose(fp);
                    continue;
                }

                int iCRC1 = 0;
                const int iCRC2 = ItemDe.GetCRCConvertInt();
                SDL_RWread(fp, &iCRC1, sizeof(int), 1);
                if (iCRC2 != iCRC1) {
                    SDL_Log("ItemTable Read Error (Header CRC mismatch) %d", i);
                    SDL_RWclose(fp);
                    continue;
                }

                // 使用 Template 函式處理大量重複邏輯
                bool res = false;
                switch (i)
                {
                    case PLANT:
                        res = ReadBinaryItemBatch<CItem_Plant, CItem_Plant_File>(fp, ItemDe, Plant, iNumOfItemTable, ItemAttrMax[i], i);
                        break;
                    case MINERAL:
                        res = ReadBinaryItemBatch<CItem_Mineral, CItem_Mineral_File>(fp, ItemDe, Mineral, iNumOfItemTable, ItemAttrMax[i], i);
                        break;
                    case HERB:
                        res = ReadBinaryItemBatch<CItem_Herb, CItem_Herb_File>(fp, ItemDe, Herb, iNumOfItemTable, ItemAttrMax[i], i);
                        break;
                    case COOK:
                        res = ReadBinaryItemBatch<CItem_Cook, CItem_Cook_File>(fp, ItemDe, Cook, iNumOfItemTable, ItemAttrMax[i], i);
                        break;
                    case POTION:
                        res = ReadBinaryItemBatch<CItem_Potion, CItem_Potion_File>(fp, ItemDe, Potion, iNumOfItemTable, ItemAttrMax[i], i);
                        break;
                    case TOOL:
                        res = ReadBinaryItemBatch<CItem_Tool, CItem_Tool_File>(fp, ItemDe, Tool, iNumOfItemTable, ItemAttrMax[i], i);
                        break;
                    case WEAPON:
                        res = ReadBinaryItemBatch<CItem_Weapon, CItem_Weapon_File>(fp, ItemDe, Weapon, iNumOfItemTable, ItemAttrMax[i], i);
                        break;
                    case DISPOSABLE:
                        res = ReadBinaryItemBatch<CItem_Disposable, CItem_Disposable_File>(fp, ItemDe, Disposable, iNumOfItemTable, ItemAttrMax[i], i);
                        break;
                    case ARMOR:
                        res = ReadBinaryItemBatch<CItem_Armor, CItem_Armor_File>(fp, ItemDe, Armor, iNumOfItemTable, ItemAttrMax[i], i);
                        break;
                    case ACCESSORY:
                        res = ReadBinaryItemBatch<CItem_Accessory, CItem_Accessory_File>(fp, ItemDe, Accessory, iNumOfItemTable, ItemAttrMax[i], i);
                        break;
                    case ETC:
                        res = ReadBinaryItemBatch<CItem_Etc, CItem_Etc_File>(fp, ItemDe, Etc, iNumOfItemTable, ItemAttrMax[i], i);
                        break;
                }

                SDL_RWclose(fp);
                if(res) bLoadedFromBin = true;
            }
        }

        // -----------------------------------------------------------
        // 讀取 Text (.txt) [僅限 PC DEBUG 模式]
        // -----------------------------------------------------------
#if defined(_DEBUG) && !defined(__ANDROID__)
        // 如果沒有讀取 Bin (或是 PC Debug 模式強制讀 txt)，則讀取 txt
       if (!bLoadedFromBin)
       {
           SDL_RWops* fp = SDL_RWFromFile(filename, "rb");
           if (fp && (Item_Ref.nItem[i] > 0))
           {
               // 讀取全部內容到字串
               Sint64 fileSize = SDL_RWsize(fp);
               std::string content;
               content.resize(fileSize);
               SDL_RWread(fp, &content[0], fileSize, 1);
               SDL_RWclose(fp);

               std::stringstream ss(content);
               std::string line;
               char temp[MAX_STRING_];

               // 逐行解析
               while(std::getline(ss, line))
               {
                    if (line.empty()) continue;
                    // 移除 \r
                    if (line.back() == '\r') line.pop_back();

                    strncpy(temp, line.c_str(), MAX_STRING_);

                    // 這裡的 strtok 邏輯稍微危險，因為 strtok 會破壞 temp
                    // 建議使用 stringstream 解析 ID
                    char bufForId[MAX_STRING_];
                    strcpy(bufForId, temp);

                    char* token = strtok(bufForId, "\t\n");
                    if (token) {
                        int item_id = atoi(token);

                        // 根據類型呼叫 ReadItemTable
                        // 這裡為了保留原結構，還是得 switch，但已經比原本乾淨
                        switch (i)
                        {
                            case PLANT:     if (item_id >= 0 && item_id < Plant.size()) Plant[item_id].ReadItemTable(temp); break;
                            case MINERAL:   if (item_id >= 0 && item_id < Mineral.size()) Mineral[item_id].ReadItemTable(temp); break;
                            case HERB:      if (item_id >= 0 && item_id < Herb.size()) Herb[item_id].ReadItemTable(temp); break;
                            case COOK:      if (item_id >= 0 && item_id < Cook.size()) Cook[item_id].ReadItemTable(temp); break;
                            case POTION:    if (item_id >= 0 && item_id < Potion.size()) Potion[item_id].ReadItemTable(temp); break;
                            case TOOL:      if (item_id >= 0 && item_id < Tool.size()) Tool[item_id].ReadItemTable(temp); break;
                            case WEAPON:    if (item_id >= 0 && item_id < Weapon.size()) Weapon[item_id].ReadItemTable(temp); break;
                            case DISPOSABLE:if (item_id >= 0 && item_id < Disposable.size()) Disposable[item_id].ReadItemTable(temp); break;
                            case ARMOR:     if (item_id >= 0 && item_id < Armor.size()) Armor[item_id].ReadItemTable(temp); break;
                            case ACCESSORY: if (item_id >= 0 && item_id < Accessory.size()) Accessory[item_id].ReadItemTable(temp); break;
                            case ETC:       if (item_id >= 0 && item_id < Etc.size()) Etc[item_id].ReadItemTable(temp); break;
                        }
                    }
               }

               // 【關鍵修改】在 Android 上我們不執行「寫入 .bin」的操作
               // 因為 APK 是唯讀的。生成 .bin 的工作應該在 PC 端完成。
               // 這裡保留原代碼結構，但只在 Windows 執行

               // ... (此處省略寫入 .bin 的代碼，因為 Android 不需要且不能跑) ...
               // 如果您需要在 PC 上生成 bin，請保留原本的寫入邏輯，但加上 #ifndef __ANDROID__
           }
       }
#endif
    } // End Loop

    // 步驟 4: 指標設定 (不變)
    for (int z = 0; z < Plant.size(); z++) { Item_Ref.Item_type[0][z] = &Plant[z]; }
    for (int z = 0; z < Mineral.size(); z++) { Item_Ref.Item_type[1][z] = &Mineral[z]; }
    for (int z = 0; z < Herb.size(); z++) { Item_Ref.Item_type[2][z] = &Herb[z]; }
    for (int z = 0; z < Cook.size(); z++) { Item_Ref.Item_type[3][z] = &Cook[z]; }
    for (int z = 0; z < Potion.size(); z++) { Item_Ref.Item_type[4][z] = &Potion[z]; }
    for (int z = 0; z < Tool.size(); z++) { Item_Ref.Item_type[5][z] = &Tool[z]; }
    for (int z = 0; z < Weapon.size(); z++) { Item_Ref.Item_type[6][z] = &Weapon[z]; }
    for (int z = 0; z < Disposable.size(); z++) { Item_Ref.Item_type[7][z] = &Disposable[z]; }
    for (int z = 0; z < Armor.size(); z++) { Item_Ref.Item_type[8][z] = &Armor[z]; }
    for (int z = 0; z < Accessory.size(); z++) { Item_Ref.Item_type[9][z] = &Accessory[z]; }
    for (int z = 0; z < Etc.size(); z++) { Item_Ref.Item_type[10][z] = &Etc[z]; }
}

int GetItemNumber(int item_number, int& type, int& num)
{
	type = item_number / 1000;
	num = item_number % 1000;

	if ((type < 0) || (type > 10)) return 0;
	//	if( (num<0) || (num>= Item_Ref.nItem[type]) ) return 0;

	return 1;
}



///////////////////////////////////////////////////////////////////////////////////////
void LoadItemIconData(Spr* s, int num)
{
    SDL_RWops* fp = nullptr;
    char filename[MAX_FILENAME];

    // [Android] 1. 路徑處理：移除 ./
    sprintf(filename, "Itemdata/%07d.CSP", num);

    // [Android] 2. 開啟檔案
    fp = SDL_RWFromFile(filename, "rb");

    if (fp)
    {
        // 讀取 Header
        SDL_RWread(fp, &s->xl, sizeof(short), 1);
        SDL_RWread(fp, &s->yl, sizeof(short), 1);
        SDL_RWread(fp, &s->ox, sizeof(short), 1);
        SDL_RWread(fp, &s->oy, sizeof(short), 1);
        SDL_RWread(fp, &s->size, sizeof(unsigned int), 1);

        dw_TotalImageSize += s->size;

        MemFree(s->img);

        // 安全檢查：避免分配 0 bytes
        if (s->size > 0) {
            MemAlloc(s->img, s->size);
            // 讀取圖片數據
            SDL_RWread(fp, s->img, 1, s->size);

            convert565to555(s); // todo finished
        } else {
            s->img = nullptr;
        }

        SDL_RWclose(fp);

        // [Android] 3. 成功讀取後才建立紋理
        g_SDLInfo.CreateTextureFromSingleSpr_CPP(filename, s);
    }
    else
    {
        // [Android] 4. 失敗時的 Fallback 邏輯
        if (num != 0) {
            // 如果不是讀取 0 號失敗，則嘗試讀取 0 號 (預設圖示)
            // SDL_Log("LoadItemIconData: Icon %d not found, fallback to 0.", num);
            LoadItemIconData(s, 0);
        }
        else {
            // 如果連 0 號都讀不到，代表資源嚴重缺失，停止遞迴避免當機
            SDL_Log("LoadItemIconData Critical Error: Default icon (0) not found!");
            s->img = nullptr;
            s->size = 0;
        }
    }
}
// 簡單的配方查詢函數
// 輸入: 三個相同的物品編號 (item_no)
// 輸出: 合成後的 ItemAttr 結構
int GetCraftingResult(short int inputItemNo)
{
	/*if (inputItemNo / 1000 != ACCESSORY) {
		return 0;
	}*/

	CItem* it = ItemUnit(inputItemNo);
	if (!it) {
		return 0;
	}

	return IsCraftEquip(inputItemNo) ? inputItemNo : 0;

	// 假設: 物品編號 100 是「木棒」
	/*if (inputItemNo == 100) {
		// 合成結果：物品編號 200 是「長木板」
		return ItemAttr{ 200, {0, 0, 0, 0, 0, 0} };
	}
	// 假設: 物品編號 300 是「礦石」
	else if (inputItemNo == 300) {
		// 合成結果：物品編號 400 是「錠」
		// 可以設定合成後的新屬性
		return ItemAttr{ 400, {10, 0, 0, 0, 0, 0} };
	}

	// 如果沒有配方對應，返回一個空的 ItemAttr
	return 0;*/
}


/**
 * 檢查並執行 InvItemAttr 陣列中的三格垂直合成
 * @param InvItemAttr 遊戲背包的三維陣列
 * @return true 如果有任何合成發生，否則為 false
 */
int CheckAndCraftVertical(int page, int column)
{
	int craft_successful = -1;
	ItemAttr item = { 0 };
	if(column < 0)
	{
		// 2. 遍歷每一列 (Column)
		for (int c = 0; c < 8; ++c)
		{
			// 3. 檢查這一列的 3 個垂直格子 (Row 0, 1, 2)
			const ItemAttr& item0 = InvItemAttr[page][0][c]; // R0 (上)
			const ItemAttr& item1 = InvItemAttr[page][1][c]; // R1 (中)
			const ItemAttr& item2 = InvItemAttr[page][2][c]; // R2 (下)

			// 條件 A: 三個格子都必須有物品 (item_no 不為 EMPTY_ITEM_NO)
			if (item0.item_no != 0 &&
				item1.item_no != 0 &&
				item2.item_no != 0)
			{

				// 條件 B: 三個物品的類型 (item_no) 必須相同
				if (item0.item_no == item1.item_no &&
					item1.item_no == item2.item_no)
				{

					// 條件 C: 檢查是否有配方對應
					ItemAttr result_item;
					result_item.item_no = GetCraftingResult(item0.item_no);

					if (result_item.item_no != 0)
					{
						craft_successful = c;
					}
				}
			}
		}
	}
	else {
		// 3. 檢查這一列的 3 個垂直格子 (Row 0, 1, 2)
		const ItemAttr& item0 = InvItemAttr[page][0][column]; // R0 (上)
		const ItemAttr& item1 = InvItemAttr[page][1][column]; // R1 (中)
		const ItemAttr& item2 = InvItemAttr[page][2][column]; // R2 (下)

		// 條件 A: 三個格子都必須有物品 (item_no 不為 EMPTY_ITEM_NO)
		if (item0.item_no != 0 &&
			item1.item_no != 0 &&
			item2.item_no != 0)
		{

			// 條件 B: 三個物品的類型 (item_no) 必須相同
			if (item0.item_no == item1.item_no &&
				item1.item_no == item2.item_no)
			{

				// 條件 C: 檢查是否有配方對應
				ItemAttr result_item;
				result_item.item_no = GetCraftingResult(item0.item_no);

				if (result_item.item_no != 0)
				{
					craft_successful = column;
				}
			}
		}
	}

	return craft_successful;
}

//------------------------------------------------------------------------
//	������ �׸� ����ִ� �Լ� ( ���ϸ��̼ǵ� ���� )
//------------------------------------------------------------------------
void PutItemIcon(int x, int y, int item_number, int delay, int effect, int is_not_equip)
{
	if (!item_number) return;
	int image;					// �������� ������ �ִ� �̹���
	int kind;						// ������ �׸� Ÿ��
	int index;

	GetItemNumber(item_number, kind, index);
	CItem* t = ItemUnit(kind, index);
	if (!t)
	{
		DeleteItemByItemNumber(item_number);
		return;
	}

	image = t->GetImageNumber();
	if (!GetItemNumber((int)image / 100, kind, index))
	{
#ifdef _DEBUG // finito 060507
		if (GetSysInfo(SI_GAME_MAKE_MODE))
		{
			JustMsg(lan->OutputMessage(6, 81), item_number);//010215 lsw
			exit(0);
		}
		return;
#endif
	}
	if (!index)		// ������ �׸��� 0�̸�
	{
		DeleteItemByItemNumber(item_number);
		return;
	}
	Spr* s = nullptr;

	//if (GetSysInfo(SI_GAME_MAKE_MODE))
	if (index >= ITEM_MAX)
	{
		JustMsg(lan->OutputMessage(6, 69), item_number);//010215 lsw
		return;
	}	
	switch (kind)
	{
	case 0:	s = &Icon1[index][0]; break;
	case 1:	s = &Icon2[index][0]; break;
	case 2:	s = &Icon3[index][0]; break;
	case 3:	s = &Icon4[index][0]; break;
	case 4:	s = &Icon5[index][0]; break;
	case 5:	s = &Icon6[index][0]; break;
	case 6:	s = &Icon7[index][0]; break;
	case 7:	s = &Icon8[index][0]; break;
	case 8:	s = &Icon9[index][0]; break;
	case 9:	s = &Icon10[index][0]; break;
	case 10:	s = &Icon11[index][0]; break;
	case 11:	s = &Icon12[index][0]; break;
	case 12:	s = &Icon13[index][0]; break;


	default:	JustMsg("if(!s->img) : Error ");		break;
	}
	if (s && !s->img)
	{
		LoadItemIconData(s, image);
	}
	if (delay) PutCompressedImageFX(x, y, s, delay, effect);
	else
	{
		PutCompressedImage(x, y, s);
		/*if( GetItemAttrWeapon(item_number) && is_not_equip==0 ) PutCompressedImage(x, y, s);
		else
		{
			PutCompressedImageBlendRGB( x, y, s, RGB( 0,0,200 ) );
			PutCompressedImageFX( x, y, s, 10, 2);
		}*/
	}
}

void PutItemIconAni(int x, int y, int item_number)
{
	int image;					// �������� ������ �ִ� �̹���
	int kind;						// ������ �׸� Ÿ��
	int index;

	static int ct = 0;
	static int flag = 0;

	GetItemNumber(item_number, kind, index);

	CItem* t = ItemUnit(kind, index);
	if (!t) return;
	image = t->GetImageNumber();

	image += ct;

	GetItemNumber((int)image / 100, kind, index);

	Spr* s = nullptr;

	//if (GetSysInfo(SI_GAME_MAKE_MODE))
		if (index >= ITEM_MAX)
		{
			JustMsg(lan->OutputMessage(6, 69), item_number);//010215 lsw
			return;
		}
	switch (kind)
	{
	case 0:	s = &Icon1[index][ct]; break;
	case 1:	s = &Icon2[index][ct]; break;
	case 2:	s = &Icon3[index][ct]; break;
	case 3:	s = &Icon4[index][ct]; break;
	case 4:	s = &Icon5[index][ct]; break;
	case 5:	s = &Icon6[index][ct]; break;
	case 6:	s = &Icon7[index][ct]; break;
	case 7:	s = &Icon8[index][ct]; break;
	case 8:	s = &Icon9[index][ct]; break;
	case 9:	s = &Icon10[index][ct]; break;
	case 10:	s = &Icon11[index][ct]; break;
	case 11:	s = &Icon12[index][ct]; break;
	case 12:	s = &Icon13[index][ct]; break;

	default:	JustMsg("if(!s->img) : Error ");		break;
	}
	if (s && !s->img)
	{
		LoadItemIconData(s, image);
	}
	PutCompressedImage(x, y, s);
	flag++;
	if (!(flag % 3)) ct++;
	ct %= 4;
}


char explain_tactics[13][20] = { 0,/*	"�ָ԰���"		,
									"�Ѽհ�"		,
									"Ȱ���"		 ,
									"��̰�"		,
									"ö�����"		,
									"â���"		,
									"��������"		,
									"��˴�����"		,
									"���й��"		,
									"��հ�"		,
									"��յ���"		,
									"���ݸ������"	,
									"��ȣ�Ź����"	,
									*/
};



////////////////////////////////////////////////////////////////////////
char* GetItemName(int item_id)
{
	int type = item_id;
	int num = item_id;
	GetItemNumber(item_id, type, num);

	CItem* t = ItemUnit(type, num);
	if (!t) return temp_menu_string;

	if(system_info.lang ==0)
		return t->GetItemName();
	else 
		return t->GetItemHanName();
}

char* GetItemHanName(int item_id)
{
	int type = item_id;
	int num = item_id;
	GetItemNumber(item_id, type, num);

	CItem* t = ItemUnit(type, num);
	if (t) return t->GetItemHanName();
	else return NULL;
}


int GetItemAttr2(int gender, int item_id, int attr)
{
	int type;
	int num;

	if (item_id == 0) return 0;
	GetItemNumber(item_id, type, num);

	int is_man = gender;

	CItem* t = ItemUnit(type, num);
	if (!t) return 0;

	switch (attr)
	{
	case WEAR_ABLE:	return t->GetItemClass();
	case WEIGHT:	return t->GetItemWeight();
	case VALUE:	if (!t->curr_value) ReqItemValue(item_id);
		return t->curr_value;

		///		case VALUE_SELL	:	if( !t->curr_value_sell ) ReqItemValue_Sell(item_id);
		//							return t->curr_value_sell;

	case VALUE_REPAIR:	return t->curr_value_repair;
	case DURATION:	return t->GetItemDuration();
	case ITEMSHOW:	return t->GetItemShow(is_man);
	case EQUIPABLE:	return t->is_equipable();
	case DO_RBUTTON:	return t->GetRButton();
	case USE_JUNK: 	return t->GetUseJunk();
	case RBUTTON_WAVE: return t->GetRButtonSound();
	case ITEM_KIND:	return t->GetItemKind();
	case ITEM_LIMIT:	return t->GetLimit();
	case IMAGE_NUMBER: return t->GetImageNumber();
	case REPAIR_ABLE:	return t->GetRepairAble();
	case OBJECT_IMAGE: return t->GetObjectNumber();
	case ITEM_COLOR:  return t->GetItemColor();		// 011018 KHS ����
	case RAJA_EMBLEM: // Gets the price (amount of raja emblems) of the item in the item mall
		if (!t->curr_value) ReqMallItemPrice(item_id);
		return t->curr_value;
	}
	return 0;
}

int GetItemAttr(int item_id, int attr)
{
	return GetItemAttr2(SCharacterData.nCharacterData[GENDER], item_id, attr);
}

bool IsCraftEquip(int item_no) {
	
	bool bRet = ((GetItemAttr(item_no, WEAR_ABLE) == WEAR_NECK) || 
				(GetItemAttr(item_no, WEAR_ABLE) == WEAR_UNIQUE));
	return bRet;
}

bool CheckLegendItem(ItemAttr* item) {

	LPRareMain rare = (LPRareMain)&item->attr[IATTR_RARE_MAIN];
	return (rare->iHighLevel == H_LV_LEGEND_ITEM);
}

int GetItemValueBuy(int item_id)
{
	int type = item_id;
	int num = item_id;
	GetItemNumber(item_id, type, num);
	CItem* t = ItemUnit(type, num);
	if (t) return t->GetItemValue();
	else return 0;

}
std::list<int> special_sell_items = { 10188 };
int GetItemValueSell(ItemAttr item) {
  int type;
  int num;
  GetItemNumber(item.item_no, type, num);

  CItem* t = ItemUnit(type, num);
  if (t) {
    int price = 0;
    const int base = t->curr_value;

	if (special_sell_items.end() != std::find(special_sell_items.begin(), special_sell_items.end(), item.item_no))
		return static_cast<int>(base * 0.35);

    if (type == DISPOSABLE) {
      int much = t->GetQuantity();
      if (!much) much = 1;
      price = (int)(((float)base * 0.008 + 0.5) * (float)item.attr[IATTR_MUCH] /
                    (float)much);
      return price;
    } else if (GetItemAttr(item.item_no, DO_RBUTTON) == DIVIDE_ITEM) {
      int much = t->GetItemDuration();
      if (!much) much = 1;
      price = (int)(((float)base * 0.008 + 0.5) * (float)item.attr[IATTR_MUCH] /
                    (float)much);
      return price;
    }

    WORD d_curr, d_max;
    GetItemDuration(item, d_curr, d_max);
    WORD o_d_max = t->GetItemDuration();
    if (d_max < o_d_max) d_max = o_d_max;

    if (d_max) {
      if ((item.attr[IATTR_LIMIT] - g_curr_time) > 0) {
        price = (int)((base) * ((float)d_curr / (float)d_max * .008) + .5);
        return price;
      }
      price = (int)((base) * ((float)d_curr / (float)d_max * .008) + .5);
      return price;
    }
  }

  return 0;
}

void SetItemPos(int type, POS* IP_set)
{
	IP_set->type = (char)type;
	IP_set->p1 = 0;
	IP_set->p2 = 0;
	IP_set->p3 = 0;
}

void SetItemPos(int type, int p, POS* IP_set)
{
	IP_set->type = (char)type;
	IP_set->p1 = 0;
	IP_set->p2 = 0;
	IP_set->p3 = (short int)p;
}

void SetItemPos(int type, int p1, int p2, int p3, POS* IP_set)
{
	IP_set->type = (char)type;
	IP_set->p1 = (short int)p1;
	IP_set->p2 = (short int)p2;
	IP_set->p3 = (short int)p3;
}

//extern int		aItemInventory[3][3][8];
//extern int		aAccelInventory[6];
//extern int		aArmor_Inventory[8];

//extern ItemAttr InvItemAttr[3][3][8];
extern ItemAttr EquipItemAttr[ITEM_EQUIP_MAX];
//extern ItemAttr QuickItemAttr[7];

//extern int WearAble[9];


void SendMoveItem(int item_no, POS s, POS t, bool sound)
{
	t_packet	packet;

	if (s.type == HAND && t.type == HAND) return;
    if (s.type == t.type &&
        s.p1 == t.p1 &&
        s.p2 == t.p2 &&
        s.p3 == t.p3)
    {
        return; // 來源與目標相同，直接不處理
    }


	packet.h.header.type = CMD_ITEM_EXCHANGE;
	packet.h.header.size = sizeof(t_item_exchange);

	packet.u.item_exchange.item_no = item_no;
	packet.u.item_exchange.pos_s = s;
	packet.u.item_exchange.pos_t = t;

	QueuePacket(&packet, 1);

	if (sound)
	{
		if (t.type == HAND) ItemSoundOfGetItem(item_no);
		else if (t.type == EQUIP || t.type == INV) EquipSound(item_no);
		else if (s.type == HAND) DropItemSound(item_no);
	}

	int type = WearAble[GetItemAttr(item_no, WEAR_ABLE)];
	ChangeEquip(Hero, EquipItemAttr, type);
}


void RecvChrItemInfo_0(t_chr_item_info0* i)
{//021022 lsw
	int a = i->ar0;
	int b = i->ar1;
	int ct = (a * 3) + b;

	for (int c = 0; c < 8; c++)
	{
		InvItemAttr[a][b][c] = i->inv[c];
	}
	ct++;
	if (!g_StartMenuOn)
	{
		if (ct == 9)
		{
			SendReCallItemFromServer(CMD_RESEND_ITEM_EQUIP);
			ct = 0;
		}
	}
	else
	{
		if (ct == 9)
		{
			ReqMagicInfo();
			ct = 0;
		}
		else
		{
			ReqItemInfo2(ct);
		}
	}
}

void RecvChrItemInfo_1(t_chr_item_info1* i)
{	//< CSD-031001
	const WORD idWeaponItem = i->equip[WT_WEAPON].item_no;

	if (idWeaponItem / 1000 == 6)
	{	//< CSD-031101
		CItem_Weapon* pWeapon = static_cast<CItem_Weapon*>(ItemUnit(idWeaponItem));

		if (pWeapon != NULL)
		{
			const int nEffect = pWeapon->effect;

			if (nEffect > 0)
			{
				LoadHeroClothAccessoryDataOne(Hero->sprno, nEffect);
			}

			switch (Hero->sprno)
			{
			case 0: LoadHeroClothAccessoryDataOne(Hero->sprno, pWeapon->shape_woman); break;
			case 1: LoadHeroClothAccessoryDataOne(Hero->sprno, pWeapon->shape_man); break;
			}
		}
	}	//> CSD-031101

	Hero->idWeaponItem = idWeaponItem;
	//> CSD-031001
	for (int a = 0; a < 8; ++a)
	{
		EquipItemAttr[a] = i->equip[a];
		//		CallItemVirtualFunction( VF_EQUIP_ITEM, EquipItemAttr[a].item_no );
	}

	CheckAbility();

	g_SokSungFlag = 0;			// �Ӽ� üũ

	for (int t = 0; t < 8; ++t)
	{
		if (EquipItemAttr[t].attr[IATTR_RARE_MAIN])
		{
			g_SokSungFlag = 1;
			break;
		}
	}

	if (!g_StartMenuOn)
	{
		SendReCallItemFromServer(CMD_RESEND_ITEM_QUICK);
	}
	else
	{
		ReqItemInfo2(0);
	}
	// 011018 KHS ����
	// Hero�� ������ Item���� ó���Ұ��� ���⼭ �Ѵ�. 
	Hero->mantle = EquipItemAttr[WT_NECK].item_no;
	if (EquipItemAttr[WT_NECK].attr[IATTR_RESERVED1] > 0)
	{
		int color = EquipItemAttr[WT_NECK].attr[IATTR_RESERVED1];
		int r, g, b;
		r = (color >> 16);
		color -= (r << 16);
		g = (color >> 8);
		color -= (g << 8);
		b = color;
		Hero->mantlecolor = ReturnBlendRGB(r, g, b);
	}
	else
	{
		Hero->mantlecolor = GetItemAttr(Hero->mantle, ITEM_COLOR);
	}


	if (Hero->mantle)
	{
		int shape = GetItemAttr2(Hero->sprno, Hero->mantle, ITEMSHOW);
		LoadHeroClothAccessoryDataOne(Hero->sprno, shape);
		
	}
	// 011030 KHS JJING
	const int jjing = ((RareMain*)&EquipItemAttr[WT_ARMOR].attr[IATTR_RARE_MAIN])->grade;

	if (jjing >= 5)
	{
		Hero->jjing = jjing;
		LoadHeroClothAccessoryDataOne(Hero->sprno, GetItemAttr(EquipItemAttr[WT_ARMOR].item_no, ITEMSHOW) + 200);
	}
}

void RecvChrItemInfo_2(t_chr_item_info2* i)
{
	int a;

	for (a = 0; a < 6; a++)
	{
		QuickItemAttr[a] = i->quick[a];
	}

	//soto-1.02
	if (QuickItemAttr[5].item_no)
	{
		g_SymBolMgr.SetDualItem(QuickItemAttr[5].item_no, QuickItemAttr[5].attr[0]);
	}
	//soto-1.02

	if (g_StartMenuOn) ReqItemInfo1();
}

void RecvItemEquip(t_chr_status_info* chr)
{
	if (chr->fStr) { SCharacterData.nCharacterAbility[STR] = chr->Str; }
	if (chr->fCon) { SCharacterData.nCharacterAbility[CON] = chr->Con; }
	if (chr->fDex) { SCharacterData.nCharacterAbility[DEX] = chr->Dex; }
	if (chr->fWis) { SCharacterData.nCharacterAbility[WIS] = chr->Wis; }
	if (chr->fInt) { SCharacterData.nCharacterAbility[INT_] = chr->Int; }
	if (chr->fMoveP) { SCharacterData.nCharacterAbility[MOVP] = chr->MoveP; }
	if (chr->fChar) { SCharacterData.nCharacterAbility[CHA] = chr->Char; }
	if (chr->fEndu) { SCharacterData.nCharacterAbility[ENDU] = chr->Endu; }
	if (chr->fHealth) { SCharacterData.nCharacterAbility[HEALTH] = chr->Health; }
	if (chr->fMoral) { SCharacterData.nCharacterAbility[MOR] = chr->Moral; }
	if (chr->fLuck) { SCharacterData.nCharacterAbility[LUCK] = chr->Luck; }
	if (chr->fHp) { SCharacterData.nCharacterHP = chr->Hp; }
	if (chr->fAp) { SCharacterData.nCharacterSP = chr->Ap; }
	if (chr->fMp) { SCharacterData.nCharacterMP = chr->Mp; }
	if (chr->fAc) { SCharacterData.nCharacterAbility[AC] = chr->Ac; }

	if (chr->fDamage) { SCharacterData.nCharacterAbility[DAMAGE] = chr->Dam_Min * 1000 + chr->Dam_Max; }
	if (chr->fHit) { SCharacterData.nCharacterAbility[HIT] = chr->Hit; }
	if (chr->fWsPs) { SCharacterData.nCharacterAbility[WSPS] = chr->WsPs; }


	//  ���ϸ��̼� ��ȣ��  old �����Ϳ� ���Ͽ� �ٸ��� ������ ������ �����ش�.
	//	SendAnimationInfo( );
	//
}


//############################ ���� ������ ���̺� �ε�� �̹��� �ε�  ##########################//
CMagic magic[MAX_MAGIC_];
CMagic magic2[MAX_MAGIC_];


Spr		magic_spr[MAGIC_SPR_MAX];
Spr		magic_sprNotSameAsSpell[MAGIC_SPR_MAX];

void CMagic::LoadExplain(int num, FILE* fp)
{
	char temp[MAX_STRING_];
	char* token;

	fgets(temp, MAX_STRING_, fp);

	token = strtok(temp, "\t\n");
	token = strtok(NULL, "\t\n");
	token = strtok(NULL, "\t\n");
	if (token)	SetExplain(token);
}

// 輔助：讀取整個文字檔
static bool ReadAllText(SDL_RWops* fp, std::string& outStr) {
    if (!fp) return false;
    Sint64 size = SDL_RWsize(fp);
    if (size <= 0) return false;
    outStr.resize(size);
    SDL_RWseek(fp, 0, RW_SEEK_SET);
    SDL_RWread(fp, &outStr[0], size, 1);
    return true;
}

// 輔助：修正路徑 (移除 ./)
static std::string FixPath(const std::string& path) {
    if (path.find("./") == 0) return path.substr(2);
    return path;
}

// ----------------------------------------------------------------------------
// 函式 1: processMagicFile (讀取對立職業魔法)
// ----------------------------------------------------------------------------
void processMagicFile(const std::string& filenameBin, int load) {
    // [Android] 1. 路徑處理
    std::string path = FixPath(filenameBin);

    // [Android] 2. 開啟檔案
    SDL_RWops* ifs = SDL_RWFromFile(path.c_str(), "rb");

    // 檢查文件是否成功打開
    if (!ifs) {
        CheckSumError = -1;
        SDL_Log("processMagicFile Error 1: Cannot open %s", path.c_str());
        // JustMsg("load not same as spell failed! 1");
        return;
    }

    char crc1 = 0, crc2 = 0;

    // 讀取 crc1
    if (SDL_RWread(ifs, &crc1, sizeof(char), 1) != 1) {
        CheckSumError = -1;
        SDL_Log("processMagicFile Error 2: Read CRC1 failed");
        SDL_RWclose(ifs);
        return;
    }

    // 讀取 magic 數據
    // magic2 必須是已經分配好的全域變數或靜態陣列
    if (SDL_RWread(ifs, magic2, sizeof(CMagic), MAX_MAGIC_) != MAX_MAGIC_) {
        CheckSumError = -1;
        SDL_Log("processMagicFile Error 3: Read Magic Data failed");
        SDL_RWclose(ifs);
        return;
    }

    // 讀取 crc2
    if (SDL_RWread(ifs, &crc2, sizeof(char), 1) != 1) {
        CheckSumError = -1;
        SDL_Log("processMagicFile Error 4: Read CRC2 failed");
        SDL_RWclose(ifs);
        return;
    }

    SDL_RWclose(ifs);

    // 計算 CRC
    // [Android] 為了確保跨平台一致性，明確使用 signed char (假設原版是在 Windows 預設設定下編譯)
    // 如果 Android 上 CRC 錯誤，請嘗試改用 unsigned char
    char* data_ptr = reinterpret_cast<char*>(magic2);
    size_t total_size = sizeof(CMagic) * MAX_MAGIC_;

    char calculatedCrc = 0;
    for (size_t i = 0; i < total_size; ++i) {
        calculatedCrc += data_ptr[i];
    }

    // 進行 CRC 校驗
    if (calculatedCrc != crc1 || calculatedCrc != crc2) {
        CheckSumError = -1;
        SDL_Log("processMagicFile Error 5: CRC Mismatch (File:%d, Calc:%d)", crc1, calculatedCrc);
        // JustMsg("load not same as spell failed! 5");
    }
    else {
        // CRC 校驗成功
        for (int i = 0; i < MAX_MAGIC_; ++i) {
            if (magic2[i].GetMagicNum()) {
                ReadMagicIconImageNotSameAsSpell(load, magic2[i].GetImage());
            }
        }
    }
}

// ----------------------------------------------------------------------------
// 函式 2: ReadMagicTable (主函式)
// ----------------------------------------------------------------------------
bool ReadMagicTable()
{
    // [Android] 變數宣告與初始化
    std::string filename = "magic/";     // 移除 ./
    std::string filenameBin = "magic/";  // 移除 ./
    std::string loadfile = "";
    int load = WIZARD_SPELL;

    // 根據職業與語言決定檔名
    // 注意：這裡使用 string append 而不是 strcat，更安全
    switch (SCharacterData.nCharacterData[SPELL])
    {
        case WIZARD_SPELL:
        {
            if (system_info.lang == 0) {
                filename += "wizard_en.txt";
                filenameBin += "wizard_en.bin";
                load = PRIEST_SPELL;
                loadfile = "magic/priest_en.bin"; // 移除 ./
            } else {
                filename += "wizard_ch.txt";
                filenameBin += "wizard_ch.bin";
                load = PRIEST_SPELL;
                loadfile = "magic/priest_ch.bin"; // 移除 ./
            }
            break;
        }
        case PRIEST_SPELL:
        {
            if (system_info.lang == 0) {
                filename += "priest_en.txt";
                filenameBin += "priest_en.bin";
                load = WIZARD_SPELL;
                loadfile = "magic/wizard_en.bin"; // 移除 ./
            } else {
                filename += "priest_ch.txt";
                filenameBin += "priest_ch.bin";
                load = WIZARD_SPELL;
                loadfile = "magic/wizard_ch.bin"; // 移除 ./
            }
            break;
        }
    }

    // [Android] 嘗試讀取 TXT (開發模式/轉檔)
    SDL_RWops* file = SDL_RWFromFile(filename.c_str(), "rb"); // rb 讀取文字較安全

    if (file) {
        std::string content;
        if (ReadAllText(file, content))
        {
            std::stringstream ss(content);
            std::string line;

            while (std::getline(ss, line)) {
                // 處理換行符號
                if (!line.empty() && line.back() == '\r') line.pop_back();
                if (line.empty() || line[0] == '#' || line[0] == ';') continue;

                std::stringstream lineSS(line);
                std::string token;
                int magic_no;

                // 1. 讀取 Magic Number
                if (!std::getline(lineSS, token, '\t')) continue;
                try {
                    magic_no = std::stoi(token);
                } catch (...) {
                    continue;
                }

                if (magic_no < 0 || magic_no >= MAX_MAGIC_) continue;

                auto& current_magic = magic[magic_no];
                current_magic.SetMagicNum(magic_no);

                // 2. 讀取 Name
                if (std::getline(lineSS, token, '\t')) {
                    current_magic.SetHanName(token.c_str());
                }

                // 3. 讀取 HanName
                if (std::getline(lineSS, token, '\t')) {
                    current_magic.SetName(token.c_str());
                }

                // 4. 讀取 4 個 Data 欄位
                for (int i = 0; i < 4; ++i) {
                    if (std::getline(lineSS, token, '\t')) {
                        try {
                            current_magic.SetData(i, token.empty() ? 0 : std::stoi(token));
                        } catch (...) {
                            current_magic.SetData(i, 0);
                        }
                    }
                }

                // 5. 讀取 Explain
                if (std::getline(lineSS, token, '\t')) {
                    current_magic.SetExplain(token.c_str());
                }
            }
        }
        SDL_RWclose(file);

        // [Android] 寫入 BIN (注意：Assets 是唯讀的，這段代碼在真機上會失敗)
        SDL_RWops* bin_file = SDL_RWFromFile(filenameBin.c_str(), "wb");
        if (bin_file) {
            const char* data_ptr = reinterpret_cast<const char*>(magic);
            const size_t total_size = sizeof(magic);

            // 計算 CRC
            char crc = 0;
            for(size_t i=0; i<total_size; ++i) crc += data_ptr[i];

            SDL_RWwrite(bin_file, &crc, 1, 1);
            SDL_RWwrite(bin_file, data_ptr, 1, total_size);
            SDL_RWwrite(bin_file, &crc, 1, 1);
            SDL_RWclose(bin_file);
            SDL_Log("ReadMagicTable: Converted TXT to BIN: %s", filenameBin.c_str());
        } else {
            SDL_Log("ReadMagicTable: Parsed TXT but cannot write BIN (ReadOnly Assets?)");
        }

        // 載入 Icon
        for (const auto& m : magic) {
            if (m.GetMagicNum() != 0) {
                ReadMagicIconImage(SCharacterData.nCharacterData[SPELL], m.GetImage());
            }
        }
    }
        // [Android] 如果 TXT 不存在，嘗試讀取 BIN
    else
    {
        SDL_RWops* fp = SDL_RWFromFile(filenameBin.c_str(), "rb");
        if (fp)
        {
            char* tt = (char*)magic;
            char crc = 0, crc1, crc2;

            SDL_RWread(fp, &crc1, 1, 1);
            SDL_RWread(fp, magic, sizeof(CMagic), MAX_MAGIC_);
            SDL_RWread(fp, &crc2, 1, 1);
            SDL_RWclose(fp);

            // 計算 CRC
            size_t total_size = sizeof(CMagic) * MAX_MAGIC_;
            for (size_t i = 0; i < total_size; i++)
            {
                crc += tt[i];
            }

            if (crc != crc1 || crc != crc2)
            {
                CheckSumError = -1;
                SDL_Log("ReadMagicTable CheckSumError: %s", filenameBin.c_str());
                // JustMsg(lan->OutputMessage(6, 82));
            }
            else
            {
                for (int i = 0; i < MAX_MAGIC_; i++)
                    if (magic[i].GetMagicNum())
                    {
                        ReadMagicIconImage(SCharacterData.nCharacterData[SPELL], magic[i].GetImage());
                    }
            }
        }
        else return 0; // 兩者都找不到
    }

    // 讀取對立職業魔法
    processMagicFile(loadfile, load);

    // 建立 Atlas
    std::string filenameBin2 = "2" + filenameBin; // 簡單的字串串接
    g_SDLInfo.CreateAtlasFromLoadedData(filenameBin.c_str(), magic_spr, MAGIC_SPR_MAX);
    // 注意：這裡假設 magic_sprNotSameAsSpell 對應 filenameBin2 (這似乎是原代碼的邏輯)
    g_SDLInfo.CreateAtlasFromLoadedData(filenameBin2.c_str(), magic_sprNotSameAsSpell, MAGIC_SPR_MAX);

    return 1;
}

void CMagic::DeleteMagic()
{
	if (!GetMagicNum()) return;

	if (magic_spr[m_data[ImgNum]].img)
	{
		delete magic_spr[m_data[ImgNum]].img;
	}
	magic_spr[m_data[ImgNum]].img = NULL;

	m_iMagicNum = 0;
	SetName("");
	SetHanName("");
	memset(m_data, 0, sizeof(int[MAGIC_DATA_MAX]));
}

// 函式 1
bool ReadMagicIconImageNotSameAsSpell(int type, int num)
{
    char filename[MAX_FILENAME];
    SDL_RWops* fp = nullptr; // 替換 FILE*

    Spr* s = nullptr;

    switch (type)
    {
        // ★注意：請確認你的檔案副檔名真的是大寫 .CSP 嗎？如果是小寫，請這裡也要改成小寫
        case WZD:     sprintf(filename, "./magic/W%03d.CSP", num); break;
        case PRI:     sprintf(filename, "./magic/P%03d.CSP", num); break;
    }

    // [Android] 路徑修正：移除開頭的 ./
    std::string path = filename;
    if (path.find("./") == 0) path = path.substr(2);

    s = &magic_sprNotSameAsSpell[num];

    if (s->img) return 1;

    // [Android] 使用 SDL_RWFromFile
    if ((fp = SDL_RWFromFile(path.c_str(), "rb")))
    {
        SDL_RWread(fp, &s->xl, sizeof(short), 1);
        SDL_RWread(fp, &s->yl, sizeof(short), 1);
        SDL_RWread(fp, &s->ox, sizeof(short), 1);
        SDL_RWread(fp, &s->oy, sizeof(short), 1);
        SDL_RWread(fp, &s->size, sizeof(unsigned int), 1);

        MemFree(s->img);
        MemAlloc(s->img, s->size);

        // 讀取圖片數據
        SDL_RWread(fp, s->img, 1, s->size);

        convert565to555(s); // 保持原樣
        SDL_RWclose(fp);    // 替換 fclose
    }
    else
    {
        // (選用) 加個 Log 方便除錯，如果檔案找不到會顯示
        SDL_Log("ReadMagicIconImageNotSameAsSpell Failed: %s", path.c_str());
        return 0;
    }
    return 1;
}

// 函式 2
bool ReadMagicIconImage(int type, int num)
{
    char filename[MAX_FILENAME];
    SDL_RWops* fp = nullptr; // 替換 FILE*

    Spr* s = nullptr;

    switch (type)
    {
        case WZD:     sprintf(filename, "./magic/W%03d.CSP", num); break;
        case PRI:     sprintf(filename, "./magic/P%03d.CSP", num); break;
    }

    // [Android] 路徑修正
    std::string path = filename;
    if (path.find("./") == 0) path = path.substr(2);

    if (num >= MAGIC_SPR_MAX) {
        SDL_Log("[ERROR] ReadMagicIconImage num >= MAGIC_SPR_MAX");
        // 這裡原本沒有 return，建議加上防呆，或是保持原樣往下走(如果這是預期行為)
    }

    // 防止陣列越界崩潰 (建議加上)
    if (num >= 0 && num < MAGIC_SPR_MAX) {
        s = &magic_spr[num];
    } else {
        return 0;
    }

    if (s->img) return 1;

    // [Android] 使用 SDL_RWFromFile
    if ((fp = SDL_RWFromFile(path.c_str(), "rb")))
    {
        SDL_RWread(fp, &s->xl, sizeof(short), 1);
        SDL_RWread(fp, &s->yl, sizeof(short), 1);
        SDL_RWread(fp, &s->ox, sizeof(short), 1);
        SDL_RWread(fp, &s->oy, sizeof(short), 1);
        SDL_RWread(fp, &s->size, sizeof(unsigned int), 1);

        MemFree(s->img);
        MemAlloc(s->img, s->size);

        // 讀取圖片數據
        SDL_RWread(fp, s->img, 1, s->size);

        convert565to555(s);
        SDL_RWclose(fp);
    }
    else
    {
        SDL_Log("ReadMagicIconImage Failed: %s", path.c_str());
        return 0;
    }
    return 1;
}

void FreeMagicIcon()
{
	for (int i = 0; i < MAGIC_SPR_MAX; i++)
	{
		if (magic_spr[i].img) MemFree(magic_spr[i].img);
		if (magic_sprNotSameAsSpell[i].img) MemFree(magic_sprNotSameAsSpell[i].img);
	}
}

void PutMagicIcon(int x, int y, int num, int flag, int type)
{
	Spr* s;
	//	if( !magic[num].GetImage() ) return;
	s = &magic_spr[magic[num].GetImage()];
	if (flag)
	{
		PutCompressedImageFX(x, y, s, flag, type);
	}
	else
	{
		PutCompressedImage(x, y, s);
	}
}

void PutMagicIconNotSameAsSpell(int x, int y, int num, int flag, int type)
{
	Spr* s;
	//	if( !magic[num].GetImage() ) return;
	s = &magic_sprNotSameAsSpell[magic2[num].GetImage()];
	if (flag)
	{
		PutCompressedImageFX(x, y, s, flag, type);
	}
	else
	{
		PutCompressedImage(x, y, s);
	}
}
////////////////////////////////////////////////////////////////////////////////////////
//
//		���� ���� ��� �޼ҵ� --> ����
//
//
////////////////////////////////////////////////////////////////////////////////////////




//###########################################################################################

/*
extern ItemAttr InvItemAttr[3][3][8];
extern ItemAttr EquipItemAttr[8];
extern ItemAttr QuickItemAttr[7];
extern ItemAttr HandItemAttr;

extern ItemAttr InvAttrBackUp[3][3][8];
extern ItemAttr EquipAttrBackUp[8];
extern ItemAttr QuickAttrBackUp[6];
extern ItemAttr HandAttrBackUp;

extern int aAccelInventory[6];
extern int AccelBackUp[6];

extern int aArmor_Inventory[8];
extern int ArmorBackUp[8];

extern int aItemInventory[3][3][8];
extern int InvBackUp[3][3][8];

extern int HandBackUp;
extern int HandBackUptype;

extern int g_MouseItemNumber;
extern int g_MouseItemType;
*/

void InventoryBackUp()
{
	/*memcpy(InvAttrBackUp, InvItemAttr, sizeof(ItemAttr[3][3][8]));
	memcpy(EquipAttrBackUp, EquipItemAttr, sizeof(ItemAttr[8]));
	memcpy(QuickAttrBackUp, QuickItemAttr, sizeof(ItemAttr[7]));
	HandAttrBackUp = HandItemAttr;

	//	memcpy( AccelBackUp, aAccelInventory, sizeof( int[6] ) );
	//	memcpy( ArmorBackUp, aArmor_Inventory, sizeof( int[8] ) );
	//	memcpy( InvBackUp, aItemInventory, sizeof( int[3][3][8] ) );
	HandBackUp = g_MouseItemNumber;
	HandBackUptype = g_MouseItemType;
*/
}

void InventoryRestore()
{
	// �� ���� �޴´�.
	CallServer(CMD_RESEND_ITEM);
	DeleteItem(&HandItemAttr);
	g_MouseItemType = 0;
	g_MouseItemNumber = 0;

	ChangeEquip(Hero, EquipItemAttr, 0);
}

int Item_Attack_Ani[27][8] =
{
	0,	50,	65,		0,	0,	0,	0,	0,		//��հ�	
	61,	50,	70,		4,	3,	5,	6,	8,		//�Ѽ�Į..
	62,	50,	70,		4,	3,	5,	6,	8,		//�Ѽ�Į..
	63,	50,	70,		4,	3,	5,	6,	8,		//��Į.
	64,	50,	70,		4,	3,	5,	6,	8,		//
	65,	50,	70,		4,	3,	5,	6,	8,		//
	66,	50,	70,		4,	3,	5,	6,	0,		//
	67,	50,	70,		4,	3,	5,	6,	0,		//
	68,	50,	70,		22,	21,	7,	23,	0,		//�ظ�
	69,	50,	70,		4,	3,	5,	6,	0,		//
	70,	50,	70,		4,	3,	7,	8,	23,		//��1.
	71,	50,	70,		4,	3,	7,	6,	23,		//
	72,	70,	90,		22,	21,	7,	6,	23,		//
	73,	70,	90,		22,	21,	7,	2,	30,		//	
	74,	80,	100,	22,	21,	8,	0,	0,		//â1.
	75,	70,	90,		22,	21,	8,	0,	0,		//
	76,	70,	90,		4,	3,	19,	0,	0,		//
	77,	70,	90,		4,	3,	19,	0,	0,		//
	78,	50,	480,	4,	3,	9,	0,	0,		//Ȱ1
	79,	70,	530,	4,	3,	9,	0,	0,		//	
	80,	40,	70,		10,	3,	11,	0,	0,		//....?,,
	81,	40,	70,		10,	3,	11,	0,	0,		//
	82,	40,	70,		10,	3,	11,	0,	0,		//....?,,
	83,	40,	70,		10,	3,	11,	0,	0,		//....?,,
	84,	40,	70,		10,	3,	11,	0,	0,		//....?,,
	85,	40,	70,		10,	3,	11,	0,	0,		//....?,,
	86,	40,	70,		10,	3,	11,	0,	0,		//....?,,
};

void GetItemAnimationNumber(int num, int& a, int& b, int& c, int& d, int& e, int& f, int& g)
{
	if (num)
	{
		a = Item_Attack_Ani[num - 60][1];
		b = Item_Attack_Ani[num - 60][2];
		c = Item_Attack_Ani[num - 60][3];
		d = Item_Attack_Ani[num - 60][4];
		e = Item_Attack_Ani[num - 60][5];
		f = Item_Attack_Ani[num - 60][6];
		g = Item_Attack_Ani[num - 60][7];
	}
	else
	{
		a = Item_Attack_Ani[0][1];
		b = Item_Attack_Ani[0][2];
		c = Item_Attack_Ani[0][3];
		d = Item_Attack_Ani[0][4];
		e = Item_Attack_Ani[0][5];
		f = Item_Attack_Ani[0][6];
		g = Item_Attack_Ani[0][7];
	}
}
// 020620 YGI
void CSkill::ReadSkill(int no, char* buf)
{
	num = no;
	char* token;

#ifdef KOREA_LOCALIZING_
	token = strtok(buf, "\t\n");
	strcpy(name, token);

	token = strtok(NULL, "\t\n");
	strcpy(han_name, token);
#else
	token = strtok(buf, "\t\n");
	strcpy(han_name, token);

	token = strtok(NULL, "\t\n");
	strcpy(name, token);
#endif

	token = strtok(NULL, "\t\n");
	inclusive = atoi(token);

	token = strtok(NULL, "\t\n");
	series = atoi(token);

	token = strtok(NULL, "\t\n");
	money = atoi(token);

	token = strtok(NULL, "\t\n\r");
	skillable = atoi(token);

	token = strtok(NULL, "\t\n");
	strcpy(explain, token);

	static int count = 0;

	int ct = strlen(token);
	if (ct > count) count = ct;

}

CSkill	skill[MAX_SKILL];
Spr		skill_spr[MAX_SKILL];
int		inclusive_to_skill[MAX_SKILL_KIND][8];

bool ReadSkillTable()
{
    // [Android] 2. 路徑處理
    const char* filename = "skill/SKILL.txt";
    const char* filenameBin = "skill/SKILL.bin";

    SDL_RWops* fp = nullptr;

    // =========================================================
    // 模式 A: 嘗試讀取 TXT (開發模式)
    // =========================================================
    fp = SDL_RWFromFile(filename, "rb");
    if (fp)
    {
        std::string content;
        if (ReadAllText(fp, content))
        {
            std::stringstream ss(content);
            std::string line;
            int num = 0;
            char temp[256]; // 用來存 ID
            char buf[512];  // 用來存剩餘參數

            while (std::getline(ss, line) && num < MAX_SKILL)
            {
                // 處理空行與換行符
                if (!line.empty() && line.back() == '\r') line.pop_back();
                if (line.empty()) continue;

                // 簡單解析：取得第一個 token (ID)，以及該行剩餘的部分
                // 對應原碼: fscanf(..., temp) + fgets(..., buf)

                // 1. 讀取 ID
                std::stringstream lineSS(line);
                std::string idStr;
                lineSS >> idStr;

                // 註解檢查
                if (idStr.empty() || idStr[0] == '#' || idStr[0] == ';') continue;

                num = std::stoi(idStr);

                // 2. 取得剩餘字串 (模擬 fgets)
                // fgets 會讀取 "目前位置到行尾"
                // 我們需要從 line 中找到 idStr 結束的位置
                size_t dataPos = line.find(idStr);
                if (dataPos != std::string::npos) {
                    dataPos += idStr.length();
                    // 跳過 ID 後面可能存在的空白
                    // 不過原版 fgets 可能會讀到前面的空白，為了保險起見，我們擷取 ID 之後的所有內容
                    std::string dataPart = line.substr(dataPos);

                    // 複製到 buf (傳給 ReadSkill)
                    strncpy(buf, dataPart.c_str(), 511);
                    buf[511] = '\0';

                    // 呼叫原本的解析函式
                    skill[num].ReadSkill(num, buf);
                    ReadSkillIcon(num);
                }
            }
        }
        SDL_RWclose(fp);

        // [Android] 寫入 BIN 部分 (在真機上會失敗，僅模擬器或 PC 有效)
        // 我們加上檢查，如果寫入失敗也不要崩潰
        /*
        fp = SDL_RWFromFile(filenameBin, "wb");
        if (fp)
        {
           // 強制使用 signed char 以確保 CRC 與 Windows 一致
           signed char* tt = (signed char*)skill;
           signed char crc = 0;
           size_t totalSize = sizeof(CSkill) * MAX_SKILL;

           for (size_t i = 0; i < totalSize; i++)
           {
              crc += *tt;
              tt++;
           }

           SDL_RWwrite(fp, &crc, 1, 1);
           SDL_RWwrite(fp, skill, sizeof(CSkill), MAX_SKILL);
           SDL_RWwrite(fp, &crc, 1, 1);
           SDL_RWclose(fp);
           SDL_Log("ReadSkillTable: TXT converted to BIN successfully.");
        }
        else {
           SDL_Log("ReadSkillTable: Loaded TXT but skipped BIN writing (ReadOnly).");
        }
        */
    }
        // =========================================================
        // 模式 B: 讀取 BIN (發布模式)
        // =========================================================
    else
    {
        fp = SDL_RWFromFile(filenameBin, "rb");
        if (fp)
        {
            // [Android] 3. 使用 signed char 處理 CRC
            // Windows 上 char 預設是 signed，Android 上通常是 unsigned
            // 這會導致 CRC 計算結果不同，一定要強制轉型！
            signed char* tt = (signed char*)skill;
            signed char crc = 0, crc1 = 0, crc2 = 0;

            SDL_RWread(fp, &crc1, 1, 1);
            SDL_RWread(fp, skill, sizeof(CSkill), MAX_SKILL);
            SDL_RWread(fp, &crc2, 1, 1);
            SDL_RWclose(fp);

            size_t totalSize = sizeof(CSkill) * MAX_SKILL;
            for (size_t i = 0; i < totalSize; i++)
            {
                crc += *tt;
                tt++;
            }

            if (crc != crc1 || crc != crc2)
            {
                CheckSumError = -1;
                SDL_Log("ReadSkillTable CheckSumError: %s (Calc:%d, File:%d/%d)", filenameBin, crc, crc1, crc2);
                // JustMsg(lan->OutputMessage(6, 83));
            }
            else
            {
                // 讀取成功，載入 Icon
                for (int i = 0; i < MAX_SKILL; i++)
                {
                    if (skill[i].num)
                    {
                        ReadSkillIcon(i);
                    }
                }
            }
        }
        else return 0; // 找不到任何檔案
    }

    // 建立 Atlas 與後續計算
    g_SDLInfo.CreateAtlasFromLoadedData(filename, skill_spr, MAX_SKILL);
    CalcGuildToSkill();
    ConvertSkillTable();
    return 1;
}

////////////////////////////////////////////////
//
//	SkillMain�� �����.
void ConvertSkillTable()
{
	for (int i = 0; i < MAX_SKILL_; i++)
	{
		skill[i].inclusive = SkillTable[i].MotherSkillType;
		skill[i].money = SkillTable[i].money;
		skill[i].skillable = SkillTable[i].need_exp;
		skill[i].targettype = SkillTable[i].Howto;
	}
}

bool ReadSkillIcon(int num)
{
    char filename[MAX_FILENAME];
    SDL_RWops* fp = nullptr;
    Spr* s;

    // [Android] 1. 路徑處理：移除 ./
    // 例如：skill/s001.CSP
    sprintf(filename, "skill/S%03d.csp", num);

    s = &skill_spr[num];

    // [Android] 2. 開啟檔案
    fp = SDL_RWFromFile(filename, "rb");

    if (fp)
    {
        // [Android] 3. 逐一讀取 Header (安全，無 Struct 對齊問題)
        SDL_RWread(fp, &s->xl, sizeof(short), 1);
        SDL_RWread(fp, &s->yl, sizeof(short), 1);
        SDL_RWread(fp, &s->ox, sizeof(short), 1);
        SDL_RWread(fp, &s->oy, sizeof(short), 1);

        // 讀取資料大小 (確保是 4 bytes)
        SDL_RWread(fp, &s->size, sizeof(unsigned int), 1);

        // [Android] 4. 記憶體管理與讀取
        MemFree(s->img);

        // 安全檢查：防止空檔案或錯誤導致分配 0 或過大記憶體
        if (s->size > 0) {
            MemAlloc(s->img, s->size);

            // 讀取圖片數據
            SDL_RWread(fp, s->img, 1, s->size);

            convert565to555(s); // 維持原邏輯
        } else {
            s->img = nullptr;
        }

        // [Android] 5. 關閉檔案
        SDL_RWclose(fp);
    }
    else
    {
        // 找不到檔案時回傳 false (0)
        // SDL_Log("ReadSkillIcon: Not found %s", filename); // 視需要開啟 Log
        return false;
    }

    // [Android] 6. (選用) 如果需要立即建立紋理，可以取消下行註解
    // 注意：如果在 ReadSkillTable 中會統一建立 Atlas，這裡就不需要建立單張紋理
    // g_SDLInfo.CreateTextureFromSingleSpr_CPP(filename, s);

    return true; // 1
}


void PutSkillIcon(int x, int y, int num, int flag, int flag2)
{
	Spr* s;
	s = &skill_spr[num];
	if (!s->img) return;
	if (flag) PutCompressedImageFX(x, y, s, flag, flag2);
	else PutCompressedImage(x, y, s);
}



#define MAX_SKILL_OF_KIND		20
//
//	Ư�� ��ų������ ���� �ִ� ��� ��ų ��ȣ�� �����´�.
//	para : 33�� ¥�� ��ų ����, �� ��ų ��ȣ�� ��ư� �迭, �� ũ��
//
int GetSkillMother(int kind, int skill_mother[], int max)
{
	memset(skill_mother, 0, sizeof(int) * max);
	int count = 0;
	for (int i = 0; i < MAX_SKILL; i++)
	{
		if (skill[i].num)
		{
			if (skill[i].inclusive == kind)
			{
				skill_mother[count++] = skill[i].num;
				if (count >= max) break;
			}
		}
	}
	return count;
}

/*
int skill_mother[32][2] = {	{1,3},   {3,5},   {5,8},   {8,10},  {10,16}, {16,19}, {19,21}, {21,25}, {25,28}, {28,32},
							{32,34}, {34,35}, {35,38}, {38,42}, {42,43}, {43,44}, {44,45}, {45,48}, {48,50}, {50,53},
							{53,55}, {55,56}, {56,57}, {57,64}, {64,67}, {67,73}, {73,75}, {75,76}, {76,78}, {78,86},
							{86,87}, {87,101}	};
*/
extern int SkillInventory[2][2][4];
extern int SkillInventory2[2][2][4];
void ChangeSkillMenuInventory(int x)
{
	int i, ct;
	memset(SkillInventory, 0, sizeof(int[2][2][4]));
	memset(SkillInventory2, 0, sizeof(int[2][2][4]));

	int skill_mother[MAX_SKILL_OF_KIND];
	int count = GetSkillMother(x, skill_mother, MAX_SKILL_OF_KIND);		// ���� ������ ��ų�� ã�´�.

	for (ct = 0; ct < count; ct++)
	{
		i = skill_mother[ct];
		if (SCharacterData.SkillId[i]) SkillInventory[0][ct / 4][ct % 4] = i;
		else SkillInventory2[0][ct / 4][ct % 4] = i;
	}

	count = GetSkillMother(x + 1, skill_mother, MAX_SKILL_OF_KIND);		// ���� ������ ��ų�� ã�´�.
	for (ct = 0; ct < count; ct++)
	{
		i = skill_mother[ct];
		if (SCharacterData.SkillId[i]) SkillInventory[1][ct / 4][ct % 4] = i;
		else SkillInventory2[1][ct / 4][ct % 4] = i;
	}
}

bool	IsEquipAbleItemKind(const int iItemKind)
{
	if (IK_FALG == iItemKind)
	{
		CItem* t = ItemUnit(EquipItemAttr[WT_SHOES].item_no);
		if (!t)
		{
			return false;
		}
		const int iCmpItemKind = t->GetItemKind();
		if (IK_SADDLE != iCmpItemKind)
		{
			return false;
		}
	}
	return true;
}

bool	IsEquipAbleAbilityCheck(const int iNeedAbility, const int iMyAbility)
{
	if ((!iNeedAbility) || (iNeedAbility <= iMyAbility))
	{
		return true;
	}
	return false;
}

bool	IsEquipAbleCheckWisInt(const int iNeedWis, const int iNeedInt, const int iMyWis, const int iMyInt)
{
	if (iNeedWis && (!iNeedInt))//Wis�� �䱸 �ϸ�
	{
		return IsEquipAbleAbilityCheck(iNeedWis, iMyWis);
	}
	if ((!iNeedWis) && iNeedInt)//Int�� �䱸 �ϸ�
	{
		return IsEquipAbleAbilityCheck(iNeedInt, iMyInt);
	}
	if ((!iNeedWis) && (!iNeedInt))//�Ѵ� �ʿ� ���� �ʴ�
	{
		return true;
	}
	if (iNeedWis && iNeedInt)//�Ѵ� �䱸 �ϸ�
	{
		if (IsEquipAbleAbilityCheck(iNeedWis, iMyWis) || IsEquipAbleAbilityCheck(iNeedInt, iMyInt))//�Ѵ� ���� �ؾ� ��
		{
			return true;
		}
	}
	return false;
}

bool	IsEquipAbleCheckWsPs(const int iNeedWs, const int iNeedPs, const int iMyWsPs, const int iSpellType)
{
	switch (iSpellType)
	{
	case WIZARD_SPELL:
	{
		if (IsEquipAbleAbilityCheck(iNeedWs, iMyWsPs))
		{
			return true;
		}
	}break;
	case PRIEST_SPELL:
	{
		if (IsEquipAbleAbilityCheck(iNeedPs, iMyWsPs))
		{
			return true;
		}
	}break;
	default:
	{
	}break;
	}
	return false;
}
bool	IsEquipAbleCheckGender(const int iNeedGender, const int iMyGender)
{
	switch (iNeedGender)
	{
	case 0:
	{
		return true;
	}break;
	case 1:
	{
		if (iMyGender == MALE)
		{
			return true;
		}
	}break;
	case 2:
	{
		if (iMyGender == FEMALE)
		{
			return true;
		}
	}break;
	default:
	{
		return false;
	}break;
	}
	return false;
}
bool	IsEquipAbleCheckMouseMRD(const int iNeedMouseMRD, const int iMyMouseMRD)// ��ƽüũ
{
	if ((iNeedMouseMRD <= 1) || (iNeedMouseMRD <= iMyMouseMRD))
	{
		return true;
	}
	return false;
}

inline bool IsEquipAbleCheckNation(const int iAbleNation, const CHARACTER* ch)//000  100 ���� 10 ���� 1 �Ͻ�
{
	switch (ch->name_status.nation)
	{
	case NW_BY:
	{
		if ((iAbleNation % 1000) / 100)
		{
			return true;
		}
	}break;
	case NW_ZY:
	{
		if ((iAbleNation % 100) / 10)
		{
			return true;
		}
	}break;
	case NW_YL:
	{
		if ((iAbleNation % 10) / 1)
		{
			return true;
		}
	}break;
	default:
	{
		return false;
	}break;
	}
	return false;
}

 bool IsEquipAbleCheckLv(const int iAbleLv, const CHARACTER* ch)
{
	if (!iAbleLv) { return true; }//���� 
	if (iAbleLv <= ch->lv)
	{
		return true;
	}
	return false;
}

 bool IsEquipAbleCheckFame(const int iAbleFame, const CHARACTER* ch)
{
	if (!iAbleFame) { return true; }//���� 
	if (iAbleFame <= ch->fame)
	{
		return true;
	}
	return false;
}

bool	IsEquipAbleDualStep(const int iDualStep, const CHARACTER* ch)
{
	return ((iDualStep <= ch->aStepInfo[CLS_STEP]) ? true : false);
}

bool	IsEquipAbleClass(const int iWarrior, const int iThief,
	const int iArcher, const int iWizard,
	const int iPriest, const CHARACTER* ch)
{
	switch (ch->class_type)
	{
	case WARRIOR:
	{
		if (!iWarrior) { return false; }
	}break;
	case THIEF:
	{
		if (!iThief) { return false; }
	}break;
	case ARCHER:
	{
		if (!iArcher) { return false; }
	}break;
	case WIZARD:
	{
		if (!iWizard) { return false; }
	}break;
	case PRIEST:
	{
		if (!iPriest) { return false; }
	}break;
	default:
	{
		return false;
	}break;
	}
	return true;
}

//################################## ���� ���� �Ǵ� �Լ� #########################################
int CItem_Weapon::is_equipable()
{
	if (!IsEquipAbleAbilityCheck(Need3_str, Hero_Ability[STR])) { return 0; }
	if (!IsEquipAbleAbilityCheck(Need3_con, Hero_Ability[CON])) { return 0; }
	if (!IsEquipAbleAbilityCheck(Need3_dex, Hero_Ability[DEX])) { return 0; }
	if (!IsEquipAbleAbilityCheck(Need3_end, Hero_Ability[ENDU])) { return 0; }
	if (!IsEquipAbleAbilityCheck(Need3_cha, Hero_Ability[CHA])) { return 0; }
	if (!IsEquipAbleAbilityCheck(Need3_mor, Hero_Ability[MOR])) { return 0; }
	if (!IsEquipAbleAbilityCheck(Need3_Movp, Hero_Ability[MOVP])) { return 0; }
	if (!IsEquipAbleAbilityCheck(Need3_luc, Hero_Ability[LUCK])) { return 0; }

	if (!IsEquipAbleCheckMouseMRD(Mouse_MRD, SCharacterData.SkillId[Skill_Ability])) { return 0; }
	if (!IsEquipAbleCheckWisInt(Need3_wis, Need3_int, Hero_Ability[WIS], Hero_Ability[INT_])) { return 0; }
	if (!IsEquipAbleCheckWsPs(Need3_ws, Need3_ps, Hero_Ability[WSPS], Hero->spell)) { return 0; }
	if (!IsEquipAbleCheckGender(Need3_gender, SCharacterData.nCharacterData[GENDER])) { return 0; }
	if (!IsEquipAbleCheckAbleDay(Repair_Skill2_min, Repair_Res1)) { return 0; }
	if (!IsEquipAbleCheckNation(Imunity_Cure_4, Hero)) { return 0; }
	if (!IsEquipAbleCheckLv(Imunity_Cure_5, Hero)) { return 0; }
	if (!IsEquipAbleCheckFame(Imunity_Cure_6, Hero)) { return 0; }
	if (!IsEquipAbleItemKind(GetItemKind())) { return 0; }
	if (!IsEquipAbleDualStep(GetNeedDualStep(), Hero)) { return 0; }
	if (!IsEquipAbleClass(
		GetClassWarriorAble(), GetClassThiefAble(),
		GetClassArcherAble(), GetClassWizardAble(),
		GetClassClericAble(), Hero)) {
		return 0;
	}//021113 lsw
	return 1;
}

int CItem_Tool::is_equipable()
{
	return 1;
}

int CItem_Armor::is_equipable()
{
#ifdef _DEBUG // finito 060507
	if (SysInfo.notconectserver) return 1;
#endif
	if (!IsEquipAbleAbilityCheck(Need3_str, Hero_Ability[STR])) { return 0; }
	if (!IsEquipAbleAbilityCheck(Need3_con, Hero_Ability[CON])) { return 0; }
	if (!IsEquipAbleAbilityCheck(Need3_dex, Hero_Ability[DEX])) { return 0; }
	if (!IsEquipAbleAbilityCheck(Need3_end, Hero_Ability[ENDU])) { return 0; }
	if (!IsEquipAbleAbilityCheck(Need3_cha, Hero_Ability[CHA])) { return 0; }
	if (!IsEquipAbleAbilityCheck(Need3_mor, Hero_Ability[MOR])) { return 0; }
	if (!IsEquipAbleAbilityCheck(Need3_Movp, Hero_Ability[MOVP])) { return 0; }
	if (!IsEquipAbleAbilityCheck(Need3_luc, Hero_Ability[LUCK])) { return 0; }

	if (!IsEquipAbleCheckMouseMRD(Mouse_MRD, SCharacterData.SkillId[Skill_Ability])) { return 0; }
	if (!IsEquipAbleCheckWisInt(Need3_wis, Need3_int, Hero_Ability[WIS], Hero_Ability[INT_])) { return 0; }
	if (!IsEquipAbleCheckWsPs(Need3_ws, Need3_ps, Hero_Ability[WSPS], Hero->spell)) { return 0; }
	if (!IsEquipAbleCheckGender(Need3_gender, SCharacterData.nCharacterData[GENDER])) { return 0; }
	if (!IsEquipAbleCheckAbleDay(Repair_Skill2_min, Repair_Res1)) { return 0; }
	if (!IsEquipAbleCheckNation(Imunity_Cure_4, Hero)) { return 0; }
	if (!IsEquipAbleCheckLv(Imunity_Cure_5, Hero)) { return 0; }
	if (!IsEquipAbleCheckFame(Imunity_Cure_6, Hero)) { return 0; }
	if (!IsEquipAbleItemKind(GetItemKind())) { return 0; }
	if (!IsEquipAbleDualStep(GetNeedDualStep(), Hero)) { return 0; }
	if (!IsEquipAbleClass(
		GetClassWarriorAble(), GetClassThiefAble(),
		GetClassArcherAble(), GetClassWizardAble(),
		GetClassClericAble(), Hero)) {
		return 0;
	}//021113 lsw
	return 1;
}

int CItem_Disposable::is_equipable()
{
#ifdef _DEBUG // finito 060507
	if (SysInfo.notconectserver) return 1;
#endif
	if (!IsEquipAbleAbilityCheck(Need3_str, Hero_Ability[STR])) { return 0; }
	if (!IsEquipAbleAbilityCheck(Need3_con, Hero_Ability[CON])) { return 0; }
	if (!IsEquipAbleAbilityCheck(Need3_dex, Hero_Ability[DEX])) { return 0; }
	if (!IsEquipAbleAbilityCheck(Need3_end, Hero_Ability[ENDU])) { return 0; }
	if (!IsEquipAbleAbilityCheck(Need3_cha, Hero_Ability[CHA])) { return 0; }
	if (!IsEquipAbleAbilityCheck(Need3_mor, Hero_Ability[MOR])) { return 0; }
	if (!IsEquipAbleAbilityCheck(Need3_Movp, Hero_Ability[MOVP])) { return 0; }
	if (!IsEquipAbleAbilityCheck(Need3_luc, Hero_Ability[LUCK])) { return 0; }

	if (!IsEquipAbleCheckMouseMRD(Mouse_MRD, SCharacterData.SkillId[Skill_Ability])) { return 0; }
	if (!IsEquipAbleCheckWisInt(Need3_wis, Need3_int, Hero_Ability[WIS], Hero_Ability[INT_])) { return 0; }
	if (!IsEquipAbleCheckWsPs(Need3_ws, Need3_ps, Hero_Ability[WSPS], Hero->spell)) { return 0; }
	if (!IsEquipAbleCheckGender(Need3_gender, SCharacterData.nCharacterData[GENDER])) { return 0; }
	//	if(!IsEquipAbleCheckAbleDay	(Repair_Skill2_min,Repair_Res1) )		{return 0;}
	//	if(!IsEquipAbleCheckNation(Imunity_Cure_4, Hero))		{return 0;}
	//	if(!IsEquipAbleCheckLv(Imunity_Cure_5, Hero))		{return 0;}
	//	if(!IsEquipAbleCheckFame(Imunity_Cure_6, Hero))		{return 0;}
	if (!IsEquipAbleItemKind(GetItemKind())) { return 0; }
	return 1;
}
int CItem_Accessory::is_equipable()
{
#ifdef _DEBUG // finito 060507
	if (SysInfo.notconectserver) return 1;
#endif
	if (!IsEquipAbleAbilityCheck(Need3_str, Hero_Ability[STR])) { return 0; }
	if (!IsEquipAbleAbilityCheck(Need3_con, Hero_Ability[CON])) { return 0; }
	if (!IsEquipAbleAbilityCheck(Need3_dex, Hero_Ability[DEX])) { return 0; }
	if (!IsEquipAbleAbilityCheck(Need3_end, Hero_Ability[ENDU])) { return 0; }
	if (!IsEquipAbleAbilityCheck(Need3_cha, Hero_Ability[CHA])) { return 0; }
	if (!IsEquipAbleAbilityCheck(Need3_mor, Hero_Ability[MOR])) { return 0; }
	if (!IsEquipAbleAbilityCheck(Need3_Movp, Hero_Ability[MOVP])) { return 0; }
	if (!IsEquipAbleAbilityCheck(Need3_luc, Hero_Ability[LUCK])) { return 0; }

	if (!IsEquipAbleCheckMouseMRD(Mouse_MRD, SCharacterData.SkillId[Skill_Ability])) { return 0; }
	if (!IsEquipAbleCheckWisInt(Need3_wis, Need3_int, Hero_Ability[WIS], Hero_Ability[INT_])) { return 0; }
	if (!IsEquipAbleCheckWsPs(Need3_ws, Need3_ps, Hero_Ability[WSPS], Hero->spell)) { return 0; }
	if (!IsEquipAbleCheckGender(Need3_gender, SCharacterData.nCharacterData[GENDER])) { return 0; }
	if (!IsEquipAbleCheckAbleDay(Repair_Skill2_min, Repair_Res1)) { return 0; }
	if (!IsEquipAbleCheckNation(Imunity_Cure_4, Hero)) { return 0; }
	if (!IsEquipAbleCheckLv(Imunity_Cure_5, Hero)) { return 0; }
	if (!IsEquipAbleCheckFame(Imunity_Cure_6, Hero)) { return 0; }
	if (!IsEquipAbleItemKind(GetItemKind())) { return 0; }
	if (!IsEquipAbleDualStep(GetNeedDualStep(), Hero)) { return 0; }
	if (!IsEquipAbleClass(
		GetClassWarriorAble(), GetClassThiefAble(),
		GetClassArcherAble(), GetClassWizardAble(),
		GetClassClericAble(), Hero)) {
		return 0;
	}//021113 lsw
	return 1;
}

int CItem_Etc::is_equipable()
{
#ifdef _DEBUG // finito 060507
	if (SysInfo.notconectserver) return 1;
#endif
	if (!IsEquipAbleAbilityCheck(Need3_str, Hero_Ability[STR])) { return 0; }
	if (!IsEquipAbleAbilityCheck(Need3_con, Hero_Ability[CON])) { return 0; }
	if (!IsEquipAbleAbilityCheck(Need3_dex, Hero_Ability[DEX])) { return 0; }
	if (!IsEquipAbleAbilityCheck(Need3_end, Hero_Ability[ENDU])) { return 0; }
	if (!IsEquipAbleAbilityCheck(Need3_cha, Hero_Ability[CHA])) { return 0; }
	if (!IsEquipAbleAbilityCheck(Need3_mor, Hero_Ability[MOR])) { return 0; }
	if (!IsEquipAbleAbilityCheck(Need3_Movp, Hero_Ability[MOVP])) { return 0; }
	if (!IsEquipAbleAbilityCheck(Need3_luc, Hero_Ability[LUCK])) { return 0; }

	if (!IsEquipAbleCheckMouseMRD(Mouse_MRD, SCharacterData.SkillId[Skill_Ability])) { return 0; }
	if (!IsEquipAbleCheckWisInt(Need3_wis, Need3_int, Hero_Ability[WIS], Hero_Ability[INT_])) { return 0; }
	if (!IsEquipAbleCheckWsPs(Need3_ws, Need3_ps, Hero_Ability[WSPS], Hero->spell)) { return 0; }
	if (!IsEquipAbleCheckGender(Need3_gender, SCharacterData.nCharacterData[GENDER])) { return 0; }
	if (!IsEquipAbleCheckAbleDay(Repair_Skill2_min, Repair_Res1)) { return 0; }
	if (!IsEquipAbleCheckNation(Imunity_Cure_4, Hero)) { return 0; }
	if (!IsEquipAbleCheckLv(Imunity_Cure_5, Hero)) { return 0; }
	if (!IsEquipAbleCheckFame(Imunity_Cure_6, Hero)) { return 0; }
	if (!IsEquipAbleItemKind(GetItemKind())) { return 0; }
	if (!IsEquipAbleDualStep(GetNeedDualStep(), Hero)) { return 0; }
	if (!IsEquipAbleClass(
		GetClassWarriorAble(), GetClassThiefAble(),
		GetClassArcherAble(), GetClassWizardAble(),
		GetClassClericAble(), Hero)) {
		return 0;
	}//021113 lsw
	return 1;
}

//int equip_able[] = { 
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ������ �Ȱ��� ���� ��ƾ ����
int CItem_Weapon::is_equipable2(int dest_pos)
{
	if (wear_able != WearAble[dest_pos])
	{
		if ((wear_able != WEAR_TWO_HAND)) return NOT;
		if (EquipItemAttr[WT_SHIELD].item_no)
		{
			switch (Item_kind)
			{
			case IK_DUAL_BOW://030102 lsw Dual BOW
			case IK_SHORT_BOW:
			case IK_LONG_BOW:
			{
			}break;
			default:
			{
				return NOT;		// Ȱ�� �ƴѰ��
			}
			}
			CItem* t = ItemUnit(EquipItemAttr[WT_SHIELD]);
			if (t)
			{
				switch (t->GetItemKind())
				{
				case IK_TWO_ARROW:
				case IK_DUAL_ARROW://030102 lsw ARROW
				{
				}break;
				default:
				{
					return NOT;// Ȱ�̱� �ѵ� Ȱ�̿��� ���� ��� �ִ� ��� 
				}break;
				}
			}
		}
		if (dest_pos != WT_WEAPON) return NOT;
	}
	else
	{
		switch (wear_able)
		{
		case WEAR_RIGHT_HAND://WT_WEAPON Position
		{
			CItem* t = ItemUnit(EquipItemAttr[WT_SHIELD]);
			if (t)
			{
				switch (t->GetItemKind())
				{
				case IK_TWO_ARROW:
				case IK_DUAL_ARROW://030102 lsw ARROW
				{
					return NOT;//SHIELD Position in Arrow
				}break;
				}
			}
		}break;
		case WEAR_LEFT_HAND:
		{
			switch (GetItemKind())
			{
			case IK_TWO_ARROW:
			case IK_DUAL_ARROW:
			{
				CItem* t = ItemUnit(EquipItemAttr[WT_WEAPON]);
				if (t)
				{
					switch (t->GetItemKind())
					{
					case IK_DUAL_BOW://030102 lsw Dual BOW
					case IK_SHORT_BOW:
					case IK_LONG_BOW:
					{
					}break;
					default:
					{
						return NOT;
					}break;
					}
				}
			}
			}
		}break;
		}
	}
	return is_equipable();
}

int CItem_Tool::is_equipable2(int dest_pos)
{
	return is_equipable();
}

int CItem_Armor::is_equipable2(int dest_pos)
{
	if (wear_able != WearAble[dest_pos])			// ��ġ�� �� ���� ��� // 0621 YGI
	{
		if ((wear_able != WEAR_TWO_HAND)) return NOT;
		if (EquipItemAttr[WT_SHIELD].item_no) return NOT;
		if (dest_pos != WT_WEAPON) return NOT;
	}

	if (wear_able == WEAR_LEFT_HAND)						// ��հ��� ����Ҷ� ���и� �����ϸ�...
	{
		CItem* t = ItemUnit(EquipItemAttr[WT_WEAPON]);
		if (t && t->GetItemClass() == WEAR_TWO_HAND) return NOT;
	}
	return is_equipable();
}


int CItem_Disposable::is_equipable2(int dest_pos)
{
	if (wear_able != WearAble[dest_pos])			// ��ġ�� �� ���� ��� // 0621 YGI
	{
		if ((wear_able != WEAR_TWO_HAND)) return NOT;		// ��չ��Ⱑ �ƴҶ�
		if (EquipItemAttr[WT_SHIELD].item_no) return NOT;		// �����ڸ��� ���� �ִٸ�
		if (dest_pos != WT_WEAPON) return NOT;				// ��ġ�� ���� �ڸ��� �ƴϸ� NOT�� ����
	}
	else
	{
		if (Item_kind == IK_TWO_ARROW
			|| Item_kind == IK_DUAL_ARROW)					// ��հ��� ����Ҷ� ȭ���� �����ϸ�//030102 lsw Arrow
		{
			CItem* t = ItemUnit(EquipItemAttr[WT_WEAPON]);
			if (t)
			{
				switch (t->GetItemKind())
				{
				case IK_DUAL_BOW://030102 lsw Dual BOW
				case IK_SHORT_BOW:
				case IK_LONG_BOW:
				{
				}break;
				default:
				{
					return NOT;		// Ȱ�� �ƴѰ��
				}
				}
			}
			else
			{
				return NOT;
			}
		}
	}
	return is_equipable();
}

int CItem_Accessory::is_equipable2(int dest_pos)
{
	if (wear_able != WearAble[dest_pos]) return NOT;		// ��ġ�� �� ���� ���
	return is_equipable();
}

int CItem_Etc::is_equipable2(int dest_pos)
{
	if (wear_able != WearAble[dest_pos]) return NOT;		// ��ġ�� �� ���� ���

	return is_equipable();
}
// 0624 YGI////////////////////////////////////////////////////////////////////////////////// �������















//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//###################### ���� ����� #######################


void GetLearnKnowlageInv(int start)			// ���� ����� ��ﶧ ������ ��ų
{
	int a, b, s;
	memset(SkillInventory, 0, sizeof(int[2][2][4]));

	motherSkillImageNo[0] = 636;	// IDENTIFICATION // ����� �ӽ�
	//	motherSkillImageNo[1] = 637;	// HEALING
	SMenu[MN_SKILLGUILD_KNOWLAGE].nField[6].nSHideNomalCount = 1;
	SMenu[MN_SKILLGUILD_KNOWLAGE].nField[6].nSHideNomalStart = start;
	SMenu[MN_SKILLGUILD_KNOWLAGE].nField[5].nType = FT_NOMAL_PUT;	// other button

	int skill_mother[MAX_SKILL_OF_KIND];
	int count = GetSkillMother(ITEM_IDENTIFICATION, skill_mother, MAX_SKILL_OF_KIND);
	s = skill_mother[0]; a = 0; b = 0;
	while (count > 0 && s <= skill_mother[count - 1])
	{
		//011012 lsw
		if (!SCharacterData.SkillId[s] && SCharacterData.nJobSkill[ITEM_IDENTIFICATION].skillexp >= (unsigned int)skill[s].skillable)
		{
			SkillInventory[0][a][b] = s;
			b++;
			if (b > 3) { b = 0; a++; }
		}
		s++;
	}

	if (SkillInventory[0][1][0]) SMenu[MN_SKILLGUILD_KNOWLAGE].nField[2].nSHideNomalCount = 2;	// ���� ������
	else SMenu[MN_SKILLGUILD_KNOWLAGE].nField[2].nSHideNomalCount = 1;

	//	s=skill_mother[IDENTIFICATION][0]; a=0; b=0;
	//	while( s < skill_mother[IDENTIFICATION][1] )
	//	{
	//		if( !SCharacterData.SkillId[s] && SCharacterData.nJobSkill[IDENTIFICATION] >= ( unsigned int )skill[s].skillable) 
	//		{
	//			SkillInventory[1][a][b] = s;
	//			b++;
	//			if( b > 3 ) { b=0; a++; }
	//		}
	//		s++;
	//	}
}

void GetLearnSkillInv(int type, int start)
{
	int a, b, s, ct;
	memset(SkillInventory, 0, sizeof(int[2][2][4]));
	int skill_mother[MAX_SKILL_OF_KIND];
	int count;
	motherSkillImageType[0] = 0;
	motherSkillImageType[1] = 0;

	switch (type)
	{
	case	MN_BLACKSMITH:	motherSkillImageNo[0] = 623;	// MINING
		motherSkillImageNo[1] = 630;	// BLACKSMITHY
		SMenu[MN_SKILLGUILD_SKILL].nField[6].nSHideNomalCount = 2;
		SMenu[MN_SKILLGUILD_SKILL].nField[6].nSHideNomalStart = start;
		SMenu[MN_SKILLGUILD_SKILL].nField[5].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;	// other button

		count = GetSkillMother(MINING, skill_mother, MAX_SKILL_OF_KIND);
		ct = 0; a = 0; b = 0;
		while (count > 0 && ct <= count)
		{
			s = skill_mother[ct];

			if (!skill[s].num) { ct++; continue; }
			//011012 lsw
			if (!SCharacterData.SkillId[s] && SCharacterData.nJobSkill[MINING].skillexp >= (unsigned int)skill[s].skillable)
			{
				SkillInventory[0][a][b] = s;
				b++;
				if (b > 3) { b = 0; a++; }
			}
			ct++;
		}

		if (SkillInventory[0][1][0]) SMenu[MN_SKILLGUILD_SKILL].nField[2].nSHideNomalCount = 2;	// ���� ������
		else SMenu[MN_SKILLGUILD_SKILL].nField[2].nSHideNomalCount = 1;

		count = GetSkillMother(BLACKSMITHY, skill_mother, MAX_SKILL_OF_KIND);
		ct = 0; a = 0; b = 0;
		while (count > 0 && ct <= count)
		{
			s = skill_mother[ct];
			if (!skill[s].num) { ct++; continue; }
			//011012 lsw
			if (!SCharacterData.SkillId[s] && SCharacterData.nJobSkill[BLACKSMITHY].skillexp >= (unsigned int)skill[s].skillable)
			{
				SkillInventory[1][a][b] = s;
				b++;
				if (b > 3) { b = 0; a++; }
			}
			ct++;
		}
		break;
	case	MN_FARM:		motherSkillImageNo[0] = 0;		// FARMING
		SMenu[MN_SKILLGUILD_SKILL].nField[6].nSHideNomalCount = 1;
		SMenu[MN_SKILLGUILD_SKILL].nField[6].nSHideNomalStart = start;
		SMenu[MN_SKILLGUILD_SKILL].nField[5].nType = FT_NOMAL_PUT;	// other button

		count = GetSkillMother(FARMING, skill_mother, MAX_SKILL_OF_KIND);
		ct = 0; a = 0; b = 0;
		while (count > 0 && ct <= count)
		{
			s = skill_mother[ct];
			if (!skill[s].num) { ct++; continue; }
			//011012 lsw
			if (!SCharacterData.SkillId[s] && SCharacterData.nJobSkill[FARMING].skillexp >= (unsigned int)skill[s].skillable)
			{
				SkillInventory[0][a][b] = s;
				b++;
				if (b > 3) { b = 0; a++; }
			}
			ct++;
		}
		if (SkillInventory[0][1][0]) SMenu[MN_SKILLGUILD_SKILL].nField[2].nSHideNomalCount = 2;	// ���� ������
		else SMenu[MN_SKILLGUILD_SKILL].nField[2].nSHideNomalCount = 1;
		break;


	case	MN_RESTAURANT:	motherSkillImageNo[0] = 631;	// COOKING
		SMenu[MN_SKILLGUILD_SKILL].nField[6].nSHideNomalCount = 1;
		SMenu[MN_SKILLGUILD_SKILL].nField[6].nSHideNomalStart = start;
		SMenu[MN_SKILLGUILD_SKILL].nField[5].nType = FT_NOMAL_PUT;	// other button

		count = GetSkillMother(COOKING, skill_mother, MAX_SKILL_OF_KIND);
		ct = 0; a = 0; b = 0;
		while (count > 0 && ct <= count)
		{
			s = skill_mother[ct];
			if (!skill[s].num) { ct++; continue; }
			//011012 lsw
			if (!SCharacterData.SkillId[s] && SCharacterData.nJobSkill[COOKING].skillexp >= (unsigned int)skill[s].skillable)
			{
				SkillInventory[0][a][b] = s;
				b++;
				if (b > 3) { b = 0; a++; }
			}
			ct++;
		}
		if (SkillInventory[0][1][0]) SMenu[MN_SKILLGUILD_SKILL].nField[2].nSHideNomalCount = 2;	// ���� ������
		else SMenu[MN_SKILLGUILD_SKILL].nField[2].nSHideNomalCount = 1;
		break;

	case	MN_HERBSHOP:	motherSkillImageNo[0] = 627;	// HERBDIGGING
		SMenu[MN_SKILLGUILD_SKILL].nField[6].nSHideNomalCount = 1;
		SMenu[MN_SKILLGUILD_SKILL].nField[6].nSHideNomalStart = start;
		SMenu[MN_SKILLGUILD_SKILL].nField[5].nType = FT_NOMAL_PUT;	// other button

		count = GetSkillMother(HERBDIGGING, skill_mother, MAX_SKILL_OF_KIND);
		ct = 0; a = 0; b = 0;
		while (count > 0 && ct <= count)
		{
			s = skill_mother[ct];
			if (!skill[s].num) { ct++; continue; }
			//011012 lsw
			if (!SCharacterData.SkillId[s] && SCharacterData.nJobSkill[HERBDIGGING].skillexp >= (unsigned int)skill[s].skillable)
			{
				SkillInventory[0][a][b] = s;
				b++;
				if (b > 3) { b = 0; a++; }
			}
			ct++;
		}
		if (SkillInventory[0][1][0]) SMenu[MN_SKILLGUILD_SKILL].nField[2].nSHideNomalCount = 2;	// ���� ������
		else SMenu[MN_SKILLGUILD_SKILL].nField[2].nSHideNomalCount = 1;
		break;

	case	MN_SHAMBLES:	motherSkillImageNo[0] = 626;	// BUTCHERING
		SMenu[MN_SKILLGUILD_SKILL].nField[6].nSHideNomalCount = 1;
		SMenu[MN_SKILLGUILD_SKILL].nField[6].nSHideNomalStart = start;
		SMenu[MN_SKILLGUILD_SKILL].nField[5].nType = FT_NOMAL_PUT;	// other button

		count = GetSkillMother(BUTCHERING, skill_mother, MAX_SKILL_OF_KIND);
		ct = 0; a = 0; b = 0;
		while (count > 0 && ct <= count)
		{
			s = skill_mother[ct];
			if (!skill[s].num) { ct++; continue; }
			//011012 lsw
			if (!SCharacterData.SkillId[s] && SCharacterData.nJobSkill[BUTCHERING].skillexp >= (unsigned int)skill[s].skillable)
			{
				SkillInventory[0][a][b] = s;
				b++;
				if (b > 3) { b = 0; a++; }
			}
			ct++;
		}
		if (SkillInventory[0][1][0]) SMenu[MN_SKILLGUILD_SKILL].nField[2].nSHideNomalCount = 2;	// ���� ������
		else SMenu[MN_SKILLGUILD_SKILL].nField[2].nSHideNomalCount = 1;
		break;

	case	MN_CANDLE:	motherSkillImageNo[0] = 635;	// CANDLEMAKING
		SMenu[MN_SKILLGUILD_SKILL].nField[6].nSHideNomalCount = 1;
		SMenu[MN_SKILLGUILD_SKILL].nField[6].nSHideNomalStart = start;
		SMenu[MN_SKILLGUILD_SKILL].nField[5].nType = FT_NOMAL_PUT;	// other button

		count = GetSkillMother(CANDLEMAKING, skill_mother, MAX_SKILL_OF_KIND);
		ct = 0; a = 0; b = 0;
		while (count > 0 && ct <= count)
		{
			s = skill_mother[ct];
			if (!skill[s].num) { ct++; continue; }
			//011012 lsw
			if (!SCharacterData.SkillId[s] && SCharacterData.nJobSkill[CANDLEMAKING].skillexp >= (unsigned int)skill[s].skillable)
			{
				SkillInventory[0][a][b] = s;
				b++;
				if (b > 3) { b = 0; a++; }
			}
			ct++;
		}
		if (SkillInventory[0][1][0]) SMenu[MN_SKILLGUILD_SKILL].nField[2].nSHideNomalCount = 2;	// ���� ������
		else SMenu[MN_SKILLGUILD_SKILL].nField[2].nSHideNomalCount = 1;
		break;

	case	MN_SLOPSHOP:	motherSkillImageNo[0] = 632;	// TAILORING
		SMenu[MN_SKILLGUILD_SKILL].nField[6].nSHideNomalCount = 1;
		SMenu[MN_SKILLGUILD_SKILL].nField[6].nSHideNomalStart = start;
		SMenu[MN_SKILLGUILD_SKILL].nField[5].nType = FT_NOMAL_PUT;	// other button

		count = GetSkillMother(TAILORING, skill_mother, MAX_SKILL_OF_KIND);
		ct = 0; a = 0; b = 0;
		while (count > 0 && ct <= count)
		{
			s = skill_mother[ct];
			if (!skill[s].num) { ct++; continue; }
			//011012 lsw
			if (!SCharacterData.SkillId[s] && SCharacterData.nJobSkill[TAILORING].skillexp >= (unsigned int)skill[s].skillable)
			{
				SkillInventory[0][a][b] = s;
				b++;
				if (b > 3) { b = 0; a++; }
			}
			ct++;
		}
		if (SkillInventory[0][1][0]) SMenu[MN_SKILLGUILD_SKILL].nField[2].nSHideNomalCount = 2;	// ���� ������
		else SMenu[MN_SKILLGUILD_SKILL].nField[2].nSHideNomalCount = 1;
		break;

	case	MN_ALCHEMY:	motherSkillImageNo[0] = 634;	// ALCHEMY
		SMenu[MN_SKILLGUILD_SKILL].nField[6].nSHideNomalCount = 1;
		SMenu[MN_SKILLGUILD_SKILL].nField[6].nSHideNomalStart = start;
		SMenu[MN_SKILLGUILD_SKILL].nField[5].nType = FT_NOMAL_PUT;	// other button

		count = GetSkillMother(ALCHEMING, skill_mother, MAX_SKILL_OF_KIND);
		ct = 0; a = 0; b = 0;
		while (count > 0 && ct <= count)
		{
			s = skill_mother[ct];
			if (!skill[s].num) { ct++; continue; }
			//011012 lsw
			if (!SCharacterData.SkillId[s] && SCharacterData.nJobSkill[ALCHEMING].skillexp >= (unsigned int)skill[s].skillable)
			{
				SkillInventory[0][a][b] = s;
				b++;
				if (b > 3) { b = 0; a++; }
			}
			ct++;
		}
		if (SkillInventory[0][1][0]) SMenu[MN_SKILLGUILD_SKILL].nField[2].nSHideNomalCount = 2;	// ���� ������
		else SMenu[MN_SKILLGUILD_SKILL].nField[2].nSHideNomalCount = 1;
		break;

	case	MN_WOODWORKING:	motherSkillImageNo[0] = 629;	// CARPENTRY
		motherSkillImageNo[1] = 625;	// CUTDOWN
		SMenu[MN_SKILLGUILD_SKILL].nField[6].nSHideNomalCount = 2;
		SMenu[MN_SKILLGUILD_SKILL].nField[6].nSHideNomalStart = start;
		SMenu[MN_SKILLGUILD_SKILL].nField[5].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;	// other button

		count = GetSkillMother(CARPENTRY, skill_mother, MAX_SKILL_OF_KIND);
		ct = 0; a = 0; b = 0;
		while (count > 0 && ct <= count)
		{
			s = skill_mother[ct];
			if (!skill[s].num) { ct++; continue; }
			//011012 lsw
			if (!SCharacterData.SkillId[s] && SCharacterData.nJobSkill[CARPENTRY].skillexp >= (unsigned int)skill[s].skillable)
			{
				SkillInventory[0][a][b] = s;
				b++;
				if (b > 3) { b = 0; a++; }
			}
			ct++;
		}

		if (SkillInventory[0][1][0]) SMenu[MN_SKILLGUILD_SKILL].nField[2].nSHideNomalCount = 2;	// ���� ������
		else SMenu[MN_SKILLGUILD_SKILL].nField[2].nSHideNomalCount = 1;

		count = GetSkillMother(CUTDOWN, skill_mother, MAX_SKILL_OF_KIND);
		ct = 0; a = 0; b = 0;
		while (count > 0 && ct <= count)
		{
			s = skill_mother[ct];
			//011012 lsw
			if (!SCharacterData.SkillId[s] && SCharacterData.nJobSkill[CUTDOWN].skillexp >= (unsigned int)skill[s].skillable)
			{
				SkillInventory[1][a][b] = s;
				b++;
				if (b > 3) { b = 0; a++; }
			}
			ct++;
		}
		break;

	case	MN_BOWCRAFT:	motherSkillImageNo[0] = 633;	// BOWCRAFTING
		motherSkillImageNo[1] = 625;	// CUTDOWN
		SMenu[MN_SKILLGUILD_SKILL].nField[6].nSHideNomalCount = 2;
		SMenu[MN_SKILLGUILD_SKILL].nField[6].nSHideNomalStart = start;
		SMenu[MN_SKILLGUILD_SKILL].nField[5].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;	// other button

		count = GetSkillMother(BOWCRAFTING, skill_mother, MAX_SKILL_OF_KIND);
		ct = 0; a = 0; b = 0;
		while (count > 0 && ct <= count)
		{
			s = skill_mother[ct];
			if (!skill[s].num) { ct++; continue; }
			if (!SCharacterData.SkillId[s] && SCharacterData.nJobSkill[BOWCRAFTING].skillexp >= (unsigned int)skill[s].skillable)
			{
				SkillInventory[0][a][b] = s;
				b++;
				if (b > 3) { b = 0; a++; }
			}
			ct++;
		}

		if (SkillInventory[0][1][0]) SMenu[MN_SKILLGUILD_SKILL].nField[2].nSHideNomalCount = 2;	// ���� ������
		else SMenu[MN_SKILLGUILD_SKILL].nField[2].nSHideNomalCount = 1;

		count = GetSkillMother(CUTDOWN, skill_mother, MAX_SKILL_OF_KIND);
		ct = 0; a = 0; b = 0;
		while (count > 0 && ct <= count)
		{
			s = skill_mother[ct];
			if (!skill[s].num) { ct++; continue; }
			//011012 lsw 
			if (!SCharacterData.SkillId[s] && SCharacterData.nJobSkill[CUTDOWN].skillexp >= (unsigned int)skill[s].skillable)
			{
				SkillInventory[1][a][b] = s;
				b++;
				if (b > 3) { b = 0; a++; }
			}
			ct++;
		}
		break;

	case	MN_SHIPYARD:	motherSkillImageNo[0] = 624;	// FISHING
		SMenu[MN_SKILLGUILD_SKILL].nField[6].nSHideNomalCount = 1;
		SMenu[MN_SKILLGUILD_SKILL].nField[6].nSHideNomalStart = start;
		SMenu[MN_SKILLGUILD_SKILL].nField[5].nType = FT_NOMAL_PUT;	// other button

		count = GetSkillMother(FISHING, skill_mother, MAX_SKILL_OF_KIND);
		ct = 0; a = 0; b = 0;
		while (count > 0 && ct <= count)
		{
			s = skill_mother[ct];
			if (!skill[s].num) { ct++; continue; }
			//011012 lsw
			if (!SCharacterData.SkillId[s] && SCharacterData.nJobSkill[FISHING].skillexp >= (unsigned int)skill[s].skillable)
			{
				SkillInventory[0][a][b] = s;
				b++;
				if (b > 3) { b = 0; a++; }
			}
			ct++;
		}
		if (SkillInventory[0][1][0]) SMenu[MN_SKILLGUILD_SKILL].nField[2].nSHideNomalCount = 2;	// ���� ������
		else SMenu[MN_SKILLGUILD_SKILL].nField[2].nSHideNomalCount = 1;
		break;

	case	MN_ACADEMY:		motherSkillImageNo[0] = 5;	// TAMING 
		motherSkillImageType[0] = SMALL_ETC;

		SMenu[MN_SKILLGUILD_SKILL].nField[6].nSHideNomalCount = 1;
		SMenu[MN_SKILLGUILD_SKILL].nField[6].nSHideNomalStart = start;
		SMenu[MN_SKILLGUILD_SKILL].nField[5].nType = FT_NOMAL_PUT;	// other button

		count = GetSkillMother(TAIMING, skill_mother, MAX_SKILL_OF_KIND);
		skill_mother[count++] = 78;	// ANIMAL_LORE
		ct = 0; a = 0; b = 0;
		while (count > 0 && ct <= count)
		{
			s = skill_mother[ct];
			if (!skill[s].num) { ct++; continue; }
			//011012 lsw
			if (!SCharacterData.SkillId[s] && SCharacterData.nJobSkill[TAIMING].skillexp >= (unsigned int)skill[s].skillable)
			{
				SkillInventory[0][a][b] = s;
				b++;
				if (b > 3) { b = 0; a++; }
			}
			ct++;
		}

		if (SkillInventory[0][1][0]) SMenu[MN_SKILLGUILD_SKILL].nField[2].nSHideNomalCount = 2;	// ���� ������
		else SMenu[MN_SKILLGUILD_SKILL].nField[2].nSHideNomalCount = 1;
		break;
	}
}


void ItemSound(int item)		// �����ۿ� ���� �Ҹ�
{

	int type = GetItemAttr(item, ITEM_KIND);
	switch (type)
	{
	case	IK_DRINK:	MP3(SN_DRINK_BEER); break;
	case	IK_POTION:	MP3(SN_EAT_POTION); break;

	case	IK_HERB:
	case	IK_HERB_POWDER:
	case	IK_SEED:
	case	IK_GRAIN:
	case	IK_FOOD_MASS:
	case	IK_PLATE_FOOD:	MP3(SN_EAT_BREAD); break;

	case	IK_MONEY:
	case	IK_NEW_MONEY:	break;
	default:	break;
	}
}


void DropItemSound(int item)		// ������ ���� ����Ʈ���� �Ҹ�
{
	int type = GetItemAttr(item, ITEM_KIND);
	switch (type)
	{
	case	IK_NEW_MONEY:
	case	IK_MONEY:	MP3(SN_PUT_MONEY); break;
	default:					MP3(SN_DROP_ITEM); break;
	}
}



void ChangeItem(int start)			// ����Ű�� �̿��Ͽ� �������� �ٲ��ش�.
{
	memset(InvItemAttr, 0, sizeof(ItemAttr) * 3 * 3 * 8);
	ItemAttr* item_array;

	int max = Num_Of_CItem_Plant + Num_Of_CItem_Mineral + Num_Of_CItem_Herb + Num_Of_CItem_Cook + Num_Of_CItem_Potion + Num_Of_CItem_Tool
		+ Num_Of_CItem_Weapon + Num_Of_CItem_Disposable + Num_Of_CItem_Armor + Num_Of_CItem_Accessory + Num_Of_CItem_Etc;

	int inv_total = 3 * 3 * 8;
	start *= inv_total;
	item_array = new ItemAttr[max];

	ZeroMemory(item_array, sizeof(ItemAttr) * max);

	int ct = 0, i;

	for (i = 1; i < Num_Of_CItem_Plant; i++) { item_array[ct].item_no = i + PLANT * 1000;  ct++; }
	for (i = 1; i < Num_Of_CItem_Mineral; i++) { item_array[ct].item_no = i + MINERAL * 1000;  ct++; }
	for (i = 1; i < Num_Of_CItem_Herb; i++) { item_array[ct].item_no = i + HERB * 1000;  ct++; }
	for (i = 1; i < Num_Of_CItem_Cook; i++) { item_array[ct].item_no = i + COOK * 1000;  ct++; }
	for (i = 1; i < Num_Of_CItem_Potion; i++) { item_array[ct].item_no = i + POTION * 1000;  ct++; }
	for (i = 1; i < Num_Of_CItem_Tool; i++) { item_array[ct].item_no = i + TOOL * 1000;  ct++; }
	for (i = 1; i < Num_Of_CItem_Weapon; i++) { item_array[ct].item_no = i + WEAPON * 1000;  ct++; }
	for (i = 1; i < Num_Of_CItem_Disposable; i++) { item_array[ct].item_no = i + DISPOSABLE * 1000;  ct++; }
	for (i = 1; i < Num_Of_CItem_Armor; i++) { item_array[ct].item_no = i + ARMOR * 1000;  ct++; }
	for (i = 1; i < Num_Of_CItem_Accessory; i++) { item_array[ct].item_no = i + ACCESSORY * 1000;  ct++; }
	for (i = 1; i < Num_Of_CItem_Etc; i++) { item_array[ct].item_no = i + ETC * 1000;  ct++; }


	int count = 0;
	for (i = start; i < start + inv_total; i++)
	{
		ct = i % max;
		if (item_array[ct].item_no < 0) break;
		if (GetItemAttr(item_array[ct].item_no, IMAGE_NUMBER))
		{
			*((ItemAttr*)InvItemAttr + count) = item_array[ct];
		}
		count++;
	}

	SAFE_DELETE_ARRAY(item_array);
}


void MagicSetting2()		// �ڵ����� �޸������ �Ѵ�.
{

	memset(aMagicItem, 0, sizeof(int[3][4][3][5]));
	for (int a = MAX_MAGIC_ - 1; a >= 1; a--)
	{
		if (SCharacterData.MagicId[a])
		{
			const int iNo = magic[a].GetMagicNum();
			if (iNo)
			{
				magic[iNo].SetArray2();
				InsertMagicQuickArray(iNo);
			}
		}
	}


}
void CMagic::SetArray2()
{
	int exist = false;

	int first = (m_data[MagicClass] - 1) / 4;
	int second = (m_data[MagicClass] - 1) % 4;
	int third = 0;

	for (int b = 0; b < 15; b++)
	{
		if (n_MagicBagic[SCharacterData.nCharacterData[SPELL]][m_data[MagicClass] - 1][b] == GetMagicNum())
		{
			for (int z = 0; z < 15; z++)
				if (!aMagicItem[first][second][z / 5][z % 5])
				{
					third = z;
					exist = true;
					break;
				}
			break;
		}
	}

	if (!exist) return;

	int end = third % 5;
	third /= 5;

	aMagicItem[first][second][third][end] = GetMagicNum();
}


void MagicSetting()		// �ڵ� �޸����� ���� �ʴ´�.
{
	memset(aMagicItem, 0, sizeof(int[3][4][3][5]));
	for (int a = 0; a < MAX_MAGIC_; a++)
	{
		if (SCharacterData.curr_magic[a])
		{
			magic[SCharacterData.curr_magic[a]].SetArray();
		}
		else break;
	}
}

void CMagic::SetArray()
{
	int e = false;

	int first = (m_data[MagicClass] - 1) / 4;
	int second = (m_data[MagicClass] - 1) % 4;
	int third = 0;

	for (int b = 0; b < 3; b++)
	{
		for (int c = 0; c < 5; c++)
		{
			if (!aMagicItem[first][second][b][c])
			{
				aMagicItem[first][second][b][c] = GetMagicNum();
				e = true;
				break;
			}
		}
		if (e) break;
	}
}



/////////////////////////////////////////////////////////////////////////////////
// 
//		�������� ���� ���� ĳ���� �ɷ�ġ ��ȭ��Ű�� ������ ��ö �Լ� 
//
/////////////////////////////////////////////////////////////////////////////////
int CItem_Plant::EatItem()
{
	SCharacter& ch = SCharacterData;
	if (IsHeroDead()) return 0;

	ch.nCharacterSP += ap;
	if (ch.nCharacterSP > ch.nCharacterMAXSP) ch.nCharacterSP = ch.nCharacterMAXSP;

	int* temp = &cure_disease1;
	int i;
	for (i = 0; i < 6; i++)
	{
		if (*(temp + i) && ch.disease[i])
		{
			ch.disease[i] = 0;
		}
	}

	temp = &take_disease1;
	for (i = 0; i < 6; i++)
	{
		if (*(temp + i) && !ch.disease[i])
		{
			// Ȯ���� ������ �ɸ� ���ΰ��� üũ�ϴ� �Լ� ��ƾ ����
			// ch.disease[i] = TakeDisease( disease_type, char_meet, char_talk, char_sex, ob_touch, ob_eat );
			// ch.disease_no_time = 0;
		}
	}

	return 1;
}
int CItem_Herb::EatItem()
{
	SCharacter& ch = SCharacterData;
	if (IsHeroDead()) return 0;

	ch.nCharacterSP += ap;
	if (ch.nCharacterSP > ch.nCharacterMAXSP) ch.nCharacterSP = ch.nCharacterMAXSP;

	int* temp = &cure_disease1;
	int i;
	for (i = 0; i < 6; i++)
	{
		if (*(temp + i) && ch.disease[i])
		{
			ch.disease[i] = 0;
			//			ch.disease_no_time[i] = 10;   10���� �־� �ش�. 
		}
	}

	temp = &take_disease1;
	for (i = 0; i < 6; i++)
	{
		if (*(temp + i) && !ch.disease[i])
		{
			// Ȯ���� ������ �ɸ� ���ΰ��� üũ�ϴ� �Լ� ��ƾ ����
			// ch.disease[i] = TakeDisease( disease_type, char_meet, char_talk, char_sex, ob_touch, ob_eat );
			// ch.disease_no_time = 0;
		}
	}

	if (ch.nAbilityPlusOfPotion[STR] < Change_str)
	{
		ch.nAbilityPlusOfPotion[STR] = Change_str;
	}
	// ch.nAbilityPluseTime[STR] = 10; �ð��� üũ�Ͽ� �� �ð����� �ٲ۴�.

	if (ch.nAbilityPlusOfPotion[DEX] < Change_dex)
	{
		ch.nAbilityPlusOfPotion[DEX] = Change_dex;
	}
	// ch.nAbilityPluseTime[DEX] = 10; �ð��� üũ�Ͽ� �� �ð����� �ٲ۴�.

	if (ch.nAbilityPlusOfPotion[CON] < Change_con)
	{
		ch.nAbilityPlusOfPotion[CON] = Change_con;
	}
	// ch.nAbilityPluseTime[con] = 10; �ð��� üũ�Ͽ� �� �ð����� �ٲ۴�.

	if (ch.nAbilityPlusOfPotion[WIS] < Change_wis)
	{
		ch.nAbilityPlusOfPotion[WIS] = Change_wis;
	}
	// ch.nAbilityPluseTime[WIS] = 10; �ð��� üũ�Ͽ� �� �ð����� �ٲ۴�.

	if (ch.nAbilityPlusOfPotion[INT_] < Change_int)
	{
		ch.nAbilityPlusOfPotion[INT_] = Change_int;
	}
	// ch.nAbilityPluseTime[INT_] = 10; �ð��� üũ�Ͽ� �� �ð����� �ٲ۴�.

	if (ch.nAbilityPlusOfPotion[CHA] < Change_cha)
	{
		ch.nAbilityPlusOfPotion[CHA] = Change_cha;
	}
	// ch.nAbilityPluseTime[CHA] = 10; �ð��� üũ�Ͽ� �� �ð����� �ٲ۴�.

	if (ch.nAbilityPlusOfPotion[ENDU] < Change_end)
	{
		ch.nAbilityPlusOfPotion[ENDU] = Change_end;
	}
	// ch.nAbilityPluseTime[ENDU] = 10; �ð��� üũ�Ͽ� �� �ð����� �ٲ۴�.

	if (ch.nAbilityPlusOfPotion[MOVP] < Change_Movp)
	{
		ch.nAbilityPlusOfPotion[MOVP] = Change_Movp;
	}
	// ch.nAbilityPluseTime[MOVP] = 10; �ð��� üũ�Ͽ� �� �ð����� �ٲ۴�.

	if (ch.nAbilityPlusOfPotion[MOR] < Change_mor)
	{
		ch.nAbilityPlusOfPotion[MOR] = Change_mor;
	}
	// ch.nAbilityPluseTime[MOR] = 10; �ð��� üũ�Ͽ� �� �ð����� �ٲ۴�.

	if (ch.nAbilityPlusOfPotion[LUCK] < Change_luc)
	{
		ch.nAbilityPlusOfPotion[LUCK] = Change_luc;
	}
	// ch.nAbilityPluseTime[LUCK] = 10; �ð��� üũ�Ͽ� �� �ð����� �ٲ۴�.

	if (ch.nCharacterData[SPELL])
	{
		if (ch.nAbilityPlusOfPotion[WSPS] < Change_PS)
		{
			ch.nAbilityPlusOfPotion[WSPS] = Change_PS;
		}
		// ch.nAbilityPluseTime[WSPS] = 10; �ð��� üũ�Ͽ� �� �ð����� �ٲ۴�.
	}
	else
	{
		if (ch.nAbilityPlusOfPotion[WSPS] < Change_WS)
		{
			ch.nAbilityPlusOfPotion[WSPS] = Change_WS;
		}
		// ch.nAbilityPluseTime[WSPS] = 10; �ð��� üũ�Ͽ� �� �ð����� �ٲ۴�.
	}

	return 1;
}

int CItem_Cook::EatItem()
{
	SCharacter& ch = SCharacterData;
	if (IsHeroDead()) return 0;

	ch.nCharacterSP += ap;
	if (ch.nCharacterSP > ch.nCharacterMAXSP) ch.nCharacterSP = ch.nCharacterMAXSP;


	int* temp = &cure_disease1;
	int i;
	for (i = 0; i < 6; i++)
	{
		if (*(temp + i) && ch.disease[i])
		{
			ch.disease[i] = 0;
			//			ch.disease_no_time[i] = 10;   10���� �־� �ش�. 
		}
	}

	temp = &take_disease1;
	for (i = 0; i < 6; i++)
	{
		if (*(temp + i) && !ch.disease[i])
		{
			// Ȯ���� ������ �ɸ� ���ΰ��� üũ�ϴ� �Լ� ��ƾ ����
			// ch.disease[i] = TakeDisease( disease_type, char_meet, char_talk, char_sex, ob_touch, ob_eat );
			// ch.disease_no_time = 0;
		}
	}

	if (ch.nAbilityPlusOfPotion[STR] < Change_str)
	{
		ch.nAbilityPlusOfPotion[STR] = Change_str;
	}
	// ch.nAbilityPluseTime[STR] = 10; �ð��� üũ�Ͽ� �� �ð����� �ٲ۴�.

	if (ch.nAbilityPlusOfPotion[DEX] < Change_dex)
	{
		ch.nAbilityPlusOfPotion[DEX] = Change_dex;
	}
	// ch.nAbilityPluseTime[DEX] = 10; �ð��� üũ�Ͽ� �� �ð����� �ٲ۴�.

	if (ch.nAbilityPlusOfPotion[CON] < Change_con)
	{
		ch.nAbilityPlusOfPotion[CON] = Change_con;
	}
	// ch.nAbilityPluseTime[con] = 10; �ð��� üũ�Ͽ� �� �ð����� �ٲ۴�.

	if (ch.nAbilityPlusOfPotion[WIS] < Change_wis)
	{
		ch.nAbilityPlusOfPotion[WIS] = Change_wis;
	}
	// ch.nAbilityPluseTime[WIS] = 10; �ð��� üũ�Ͽ� �� �ð����� �ٲ۴�.

	if (ch.nAbilityPlusOfPotion[INT_] < Change_int)
	{
		ch.nAbilityPlusOfPotion[INT_] = Change_int;
	}
	// ch.nAbilityPluseTime[INT_] = 10; �ð��� üũ�Ͽ� �� �ð����� �ٲ۴�.

	if (ch.nAbilityPlusOfPotion[CHA] < Change_cha)
	{
		ch.nAbilityPlusOfPotion[CHA] = Change_cha;
	}
	// ch.nAbilityPluseTime[CHA] = 10; �ð��� üũ�Ͽ� �� �ð����� �ٲ۴�.

	if (ch.nAbilityPlusOfPotion[ENDU] < Change_end)
	{
		ch.nAbilityPlusOfPotion[ENDU] = Change_end;
	}
	// ch.nAbilityPluseTime[ENDU] = 10; �ð��� üũ�Ͽ� �� �ð����� �ٲ۴�.

	if (ch.nAbilityPlusOfPotion[MOVP] < Change_Movp)
	{
		ch.nAbilityPlusOfPotion[MOVP] = Change_Movp;
	}
	// ch.nAbilityPluseTime[MOVP] = 10; �ð��� üũ�Ͽ� �� �ð����� �ٲ۴�.

	if (ch.nAbilityPlusOfPotion[MOR] < Change_mor)
	{
		ch.nAbilityPlusOfPotion[MOR] = Change_mor;
	}
	// ch.nAbilityPluseTime[MOR] = 10; �ð��� üũ�Ͽ� �� �ð����� �ٲ۴�.

	if (ch.nAbilityPlusOfPotion[LUCK] < Change_luc)
	{
		ch.nAbilityPlusOfPotion[LUCK] = Change_luc;
	}
	// ch.nAbilityPluseTime[LUCK] = 10; �ð��� üũ�Ͽ� �� �ð����� �ٲ۴�.

	if (ch.nCharacterData[SPELL])
	{
		if (ch.nAbilityPlusOfPotion[WSPS] < Change_Ps)
		{
			ch.nAbilityPlusOfPotion[WSPS] = Change_Ps;
		}
		// ch.nAbilityPluseTime[WSPS] = 10; �ð��� üũ�Ͽ� �� �ð����� �ٲ۴�.
	}
	else
	{
		if (ch.nAbilityPlusOfPotion[WSPS] < Change_WS)
		{
			ch.nAbilityPlusOfPotion[WSPS] = Change_WS;
		}
		// ch.nAbilityPluseTime[WSPS] = 10; �ð��� üũ�Ͽ� �� �ð����� �ٲ۴�.
	}
	return 1;
}
int CItem_Potion::EatItem()
{
	SCharacter& ch = SCharacterData;
	if (IsHeroDead()) return 0;

	ch.nCharacterSP += ap;
	if (ch.nCharacterSP > ch.nCharacterMAXSP) ch.nCharacterSP = ch.nCharacterMAXSP;


	int* temp = &cure_disease1;
	int i;
	for (i = 0; i < 6; i++)
	{
		if (*(temp + i) && ch.disease[i])
		{
			ch.disease[i] = 0;
			//			ch.disease_no_time[i] = 10;   10���� �־� �ش�. 
		}
	}

	temp = &take_disease1;
	for (i = 0; i < 6; i++)
	{
		if (*(temp + i) && !ch.disease[i])
		{
			// Ȯ���� ������ �ɸ� ���ΰ��� üũ�ϴ� �Լ� ��ƾ ����
			// ch.disease[i] = TakeDisease( disease_type, char_meet, char_talk, char_sex, ob_touch, ob_eat );
			// ch.disease_no_time = 0;
		}
	}

	if (ch.nAbilityPlusOfPotion[STR] < Change_str)
	{
		ch.nAbilityPlusOfPotion[STR] = Change_str;
	}
	// ch.nAbilityPluseTime[STR] = 10; �ð��� üũ�Ͽ� �� �ð����� �ٲ۴�.

	if (ch.nAbilityPlusOfPotion[DEX] < Change_dex)
	{
		ch.nAbilityPlusOfPotion[DEX] = Change_dex;
	}
	// ch.nAbilityPluseTime[DEX] = 10; �ð��� üũ�Ͽ� �� �ð����� �ٲ۴�.

	if (ch.nAbilityPlusOfPotion[CON] < Change_con)
	{
		ch.nAbilityPlusOfPotion[CON] = Change_con;
	}
	// ch.nAbilityPluseTime[con] = 10; �ð��� üũ�Ͽ� �� �ð����� �ٲ۴�.

	if (ch.nAbilityPlusOfPotion[WIS] < Change_wis)
	{
		ch.nAbilityPlusOfPotion[WIS] = Change_wis;
	}
	// ch.nAbilityPluseTime[WIS] = 10; �ð��� üũ�Ͽ� �� �ð����� �ٲ۴�.

	if (ch.nAbilityPlusOfPotion[INT_] < Change_int)
	{
		ch.nAbilityPlusOfPotion[INT_] = Change_int;
	}
	// ch.nAbilityPluseTime[INT_] = 10; �ð��� üũ�Ͽ� �� �ð����� �ٲ۴�.

	if (ch.nAbilityPlusOfPotion[CHA] < Change_cha)
	{
		ch.nAbilityPlusOfPotion[CHA] = Change_cha;
	}
	// ch.nAbilityPluseTime[CHA] = 10; �ð��� üũ�Ͽ� �� �ð����� �ٲ۴�.

	if (ch.nAbilityPlusOfPotion[ENDU] < Change_end)
	{
		ch.nAbilityPlusOfPotion[ENDU] = Change_end;
	}
	// ch.nAbilityPluseTime[ENDU] = 10; �ð��� üũ�Ͽ� �� �ð����� �ٲ۴�.

	if (ch.nAbilityPlusOfPotion[MOVP] < Change_movp)
	{
		ch.nAbilityPlusOfPotion[MOVP] = Change_movp;
	}
	// ch.nAbilityPluseTime[MOVP] = 10; �ð��� üũ�Ͽ� �� �ð����� �ٲ۴�.

	if (ch.nAbilityPlusOfPotion[MOR] < Change_mor)
	{
		ch.nAbilityPlusOfPotion[MOR] = Change_mor;
	}
	// ch.nAbilityPluseTime[MOR] = 10; �ð��� üũ�Ͽ� �� �ð����� �ٲ۴�.

	if (ch.nAbilityPlusOfPotion[LUCK] < Change_luc)
	{
		ch.nAbilityPlusOfPotion[LUCK] = Change_luc;
	}
	// ch.nAbilityPluseTime[LUCK] = 10; �ð��� üũ�Ͽ� �� �ð����� �ٲ۴�.

	if (ch.nCharacterData[SPELL])
	{
		if (ch.nAbilityPlusOfPotion[WSPS] < Change_PS)
		{
			ch.nAbilityPlusOfPotion[WSPS] = Change_PS;
		}
		// ch.nAbilityPluseTime[WSPS] = 10; �ð��� üũ�Ͽ� �� �ð����� �ٲ۴�.
	}
	else
	{
		if (ch.nAbilityPlusOfPotion[WSPS] < Change_WS)
		{
			ch.nAbilityPlusOfPotion[WSPS] = Change_WS;
		}
		// ch.nAbilityPluseTime[WSPS] = 10; �ð��� üũ�Ͽ� �� �ð����� �ٲ۴�.
	}

	/*	switch( ch.condition )
		{
			case CC_NORMAL	:	break;
			case CC_POISON	:	if( Cure_poison ) ch.condition = CC_NORMAL; break;
			case CC_STONE	:	if( Cure_stone_paralysis ) ch.condition = CC_NORMAL; break;
			case CC_MAD		:	if( Cure_Mad ) ch.condition = CC_NORMAL; break;
			case CC_SLUG	:	if( Cure_sluggish ) ch.condition = CC_NORMAL; break;
			case CC_ACTIVE	:	if( Cure_Active ) ch.condition = CC_NORMAL; break;
		}
	*/
	if (ChangeNatural2)
	{
		ch.condition = ChangeNatural2;
	}

	return 1;
}



/////////////////////////////////////////////////////////////////////////////////
//
// ������ ������ ���� ĳ���� �ɷ�ġ ��ȭ�� ����ϴ� ��� �Լ�
//
/////////////////////////////////////////////////////////////////////////////////
// ���� ����
int CItem_Weapon::EquipItem()
{
	SCharacter& ch = SCharacterData;
	int* plus = ch.nAbilityPlusOfWeapon;

	plus[STR] += Change_str;
	plus[DEX] += Change_dex;
	plus[CON] += Change_con;
	plus[WIS] += Change_wis;
	plus[INT_] += Change_int;
	plus[CHA] += Change_cha;
	plus[ENDU] += Change_end;
	plus[MOVP] += Change_Movp;
	plus[MOR] += Change_mor;
	plus[LUCK] += Change_luc;

	if (ch.nCharacterData[SPELL])	plus[WSPS] += Change_ps;
	else	plus[WSPS] += Change_ws;

	ch.nCharacterMAXHP += Change_Life_max;
	ch.nCharacterHP += Change_Life_max;

	if (ch.nCharacterData[SPELL])
	{
		ch.nCharacterMAXMP += Change_Divine_Max;
		ch.nCharacterMP += Change_Divine_Max;
	}
	else
	{
		ch.nCharacterMAXMP += Change_mana_max;
		ch.nCharacterMP += Change_mana_max;
	}

	ch.nCharacterMAXSP += Change_health_max;
	ch.nCharacterSP += Change_health_max;

	int* temp = &imunity_Cure_1;
	for (int i = 0; i < 6; i++)
	{
		if (*(temp + i) && ch.disease[i])
		{
			ch.disease[i] = 0;
			ch.disease_no_time[i] = 0;
		}
	}
	return 1;
}
/////////////////////////////////////////////////////////////////////////////////
//
// �ʱ� ������ ���� �ɷ�ġ ���
//
/////////////////////////////////////////////////////////////////////////////////
// ���� ����
int CItem_Weapon::CurrEquip()
{
	SCharacter& ch = SCharacterData;
	int* plus = ch.nAbilityPlusOfWeapon;

	plus[STR] += Change_str;
	plus[DEX] += Change_dex;
	plus[CON] += Change_con;
	plus[WIS] += Change_wis;
	plus[INT_] += Change_int;
	plus[CHA] += Change_cha;
	plus[ENDU] += Change_end;
	plus[MOVP] += Change_Movp;
	plus[MOR] += Change_mor;
	plus[LUCK] += Change_luc;

	if (ch.nCharacterData[SPELL])	plus[WSPS] += Change_ps;
	else	plus[WSPS] += Change_ws;
	return 1;
}

//#####################################################
// �Ƹ� ����
int CItem_Armor::EquipItem()
{
	SCharacter& ch = SCharacterData;
	int* plus = ch.nAbilityPlusOfWeapon;

	plus[STR] += Change_str;
	plus[DEX] += Change_dex;
	plus[CON] += Change_con;
	plus[WIS] += Change_wis;
	plus[INT_] += Change_int;
	plus[CHA] += Change_cha;
	plus[ENDU] += Change_end;
	plus[MOVP] += Change_Movp;
	plus[MOR] += Change_mor;
	plus[LUCK] += Change_luc;

	if (ch.nCharacterData[SPELL])	plus[WSPS] += Change_ps;
	else	plus[WSPS] += Change_ws;

	ch.nCharacterMAXHP += Change_Life_max;
	ch.nCharacterHP += Change_Life_max;

	if (ch.nCharacterData[SPELL])
	{
		ch.nCharacterMAXMP += Change_Divine_Max;
		ch.nCharacterMP += Change_Divine_Max;
	}
	else
	{
		ch.nCharacterMAXMP += Change_mana_max;
		ch.nCharacterMP += Change_mana_max;
	}

	ch.nCharacterMAXSP += Change_health_max;
	ch.nCharacterSP += Change_health_max;

	int* temp = &imunity_Cure_1;
	for (int i = 0; i < 6; i++)
	{
		if (*(temp + i) && ch.disease[i])
		{
			ch.disease[i] = 0;
			ch.disease_no_time[i] = 0;
		}
	}
	return 1;
}
//#####################################################
// �Ƹ� ���� ����
int CItem_Armor::CurrEquip()
{
	SCharacter& ch = SCharacterData;
	int* plus = ch.nAbilityPlusOfWeapon;

	plus[STR] += Change_str;
	plus[DEX] += Change_dex;
	plus[CON] += Change_con;
	plus[WIS] += Change_wis;
	plus[INT_] += Change_int;
	plus[CHA] += Change_cha;
	plus[ENDU] += Change_end;
	plus[MOVP] += Change_Movp;
	plus[MOR] += Change_mor;
	plus[LUCK] += Change_luc;

	if (ch.nCharacterData[SPELL])	plus[WSPS] += Change_ps;
	else	plus[WSPS] += Change_ws;
	return 1;
}


//#####################################################
// �Ǽ��縮 ����
int CItem_Accessory::EquipItem()
{
	SCharacter& ch = SCharacterData;
	int* plus = ch.nAbilityPlusOfWeapon;

	plus[STR] += Change_str;
	plus[DEX] += Change_dex;
	plus[CON] += Change_con;
	plus[WIS] += Change_wis;
	plus[INT_] += Change_int;
	plus[CHA] += Change_cha;
	plus[ENDU] += Change_end;
	plus[MOVP] += Change_Movp;
	plus[MOR] += Change_mor;
	plus[LUCK] += Change_luc;


	if (ch.nCharacterData[SPELL])	plus[WSPS] += Change_ps;
	else	plus[WSPS] += Change_ws;

	ch.nCharacterMAXHP += Change_Life_max;
	ch.nCharacterHP += Change_Life_max;

	if (ch.nCharacterData[SPELL])
	{
		ch.nCharacterMAXMP += Change_Divine_Max;
		ch.nCharacterMP += Change_Divine_Max;
	}
	else
	{
		ch.nCharacterMAXMP += Change_mana_max;
		ch.nCharacterMP += Change_mana_max;
	}

	ch.nCharacterMAXSP += Change_health_max;
	ch.nCharacterSP += Change_health_max;

	int* temp = &imunity_Cure_1;
	for (int i = 0; i < 6; i++)
	{
		if (*(temp + i) && ch.disease[i])
		{
			ch.disease[i] = 0;
			ch.disease_no_time[i] = 0;
		}
	}
	return 1;
}

//#####################################################
int CItem_Accessory::CurrEquip()
{
	SCharacter& ch = SCharacterData;
	int* plus = ch.nAbilityPlusOfWeapon;

	plus[STR] += Change_str;
	plus[DEX] += Change_dex;
	plus[CON] += Change_con;
	plus[WIS] += Change_wis;
	plus[INT_] += Change_int;
	plus[CHA] += Change_cha;
	plus[ENDU] += Change_end;
	plus[MOVP] += Change_Movp;
	plus[MOR] += Change_mor;
	plus[LUCK] += Change_luc;


	if (ch.nCharacterData[SPELL])	plus[WSPS] += Change_ps;
	else	plus[WSPS] += Change_ws;
	return 1;
}


//#####################################################
// ETC ����
int CItem_Etc::EquipItem()
{
	SCharacter& ch = SCharacterData;
	int* plus = ch.nAbilityPlusOfWeapon;

	plus[STR] += Change_str;
	plus[DEX] += Change_dex;
	plus[CON] += Change_con;
	plus[WIS] += Change_wis;
	plus[INT_] += Change_int;
	plus[CHA] += Change_cha;
	plus[ENDU] += Change_end;
	//	plus[MOVP] += Change_Movp;
	plus[MOR] += Change_mor;
	plus[LUCK] += Change_luc;


	if (ch.nCharacterData[SPELL])	plus[WSPS] += Change_ps;
	else	plus[WSPS] += Change_ws;

	ch.nCharacterMAXHP += Change_Life_max;
	ch.nCharacterHP += Change_Life_max;

	if (ch.nCharacterData[SPELL])
	{
		ch.nCharacterMAXMP += Change_Divine_Max;
		ch.nCharacterMP += Change_Divine_Max;
	}
	else
	{
		ch.nCharacterMAXMP += Change_mana_max;
		ch.nCharacterMP += Change_mana_max;
	}

	ch.nCharacterMAXSP += Change_health_max;
	ch.nCharacterSP += Change_health_max;

	int* temp = &imunity_Cure_1;
	for (int i = 0; i < 6; i++)
	{
		if (*(temp + i) && ch.disease[i])
		{
			ch.disease[i] = 0;
			ch.disease_no_time[i] = 0;
		}
	}
	return 1;
}

///////////////////////////////////////////////////////////////////////
//   ��Ÿ ������ ���� ����
int CItem_Etc::CurrEquip()
{
	SCharacter& ch = SCharacterData;
	int* plus = ch.nAbilityPlusOfWeapon;

	plus[STR] += Change_str;
	plus[DEX] += Change_dex;
	plus[CON] += Change_con;
	plus[WIS] += Change_wis;
	plus[INT_] += Change_int;
	plus[CHA] += Change_cha;
	plus[ENDU] += Change_end;
	//	plus[MOVP] += Change_Movp;
	plus[MOR] += Change_mor;
	plus[LUCK] += Change_luc;

	if (ch.nCharacterData[SPELL])	plus[WSPS] += Change_ps;
	else	plus[WSPS] += Change_ws;
	return 1;
}


/////////////////////////////////////////////////////////////////////////////////
// ������ ������ ĳ���� �ɷ�ġ ���� üũ 
//
/////////////////////////////////////////////////////////////////////////////////
// ���� ����
int CItem_Weapon::ReleaseItem()
{
	SCharacter& ch = SCharacterData;
	int* plus = ch.nAbilityPlusOfWeapon;

	plus[STR] -= Change_str;
	plus[DEX] -= Change_dex;
	plus[CON] -= Change_con;
	plus[WIS] -= Change_wis;
	plus[INT_] -= Change_int;
	plus[CHA] -= Change_cha;
	plus[ENDU] -= Change_end;
	plus[MOVP] -= Change_Movp;
	plus[MOR] -= Change_mor;
	plus[LUCK] -= Change_luc;

	if (ch.nCharacterData[SPELL])	plus[WSPS] -= Change_ps;
	else	plus[WSPS] -= Change_ws;

	ch.nCharacterMAXHP -= Change_Life_max;
	ch.nCharacterHP -= Change_Life_max;

	if (ch.nCharacterData[SPELL])
	{
		ch.nCharacterMAXMP -= Change_Divine_Max;
		ch.nCharacterMP -= Change_Divine_Max;
	}
	else
	{
		ch.nCharacterMAXMP -= Change_mana_max;
		ch.nCharacterMP -= Change_mana_max;
	}

	ch.nCharacterMAXSP -= Change_health_max;
	ch.nCharacterSP -= Change_health_max;
	return 1;
}

//#####################################################
// ���� ���� ����
int CItem_Armor::ReleaseItem()
{
	SCharacter& ch = SCharacterData;
	int* plus = ch.nAbilityPlusOfWeapon;

	plus[STR] -= Change_str;
	plus[DEX] -= Change_dex;
	plus[CON] -= Change_con;
	plus[WIS] -= Change_wis;
	plus[INT_] -= Change_int;
	plus[CHA] -= Change_cha;
	plus[ENDU] -= Change_end;
	plus[MOVP] -= Change_Movp;
	plus[MOR] -= Change_mor;
	plus[LUCK] -= Change_luc;

	if (ch.nCharacterData[SPELL])	plus[WSPS] -= Change_ps;
	else	plus[WSPS] -= Change_ws;

	ch.nCharacterMAXHP -= Change_Life_max;
	ch.nCharacterHP -= Change_Life_max;

	if (ch.nCharacterData[SPELL])
	{
		ch.nCharacterMAXMP -= Change_Divine_Max;
		ch.nCharacterMP -= Change_Divine_Max;
	}
	else
	{
		ch.nCharacterMAXMP -= Change_mana_max;
		ch.nCharacterMP -= Change_mana_max;
	}

	ch.nCharacterMAXSP -= Change_health_max;
	ch.nCharacterSP -= Change_health_max;
	return 1;
}
//#####################################################
// �Ǽ��縮 ����
int CItem_Accessory::ReleaseItem()
{
	SCharacter& ch = SCharacterData;
	int* plus = ch.nAbilityPlusOfWeapon;

	plus[STR] -= Change_str;
	plus[DEX] -= Change_dex;
	plus[CON] -= Change_con;
	plus[WIS] -= Change_wis;
	plus[INT_] -= Change_int;
	plus[CHA] -= Change_cha;
	plus[ENDU] -= Change_end;
	plus[MOVP] -= Change_Movp;
	plus[MOR] -= Change_mor;
	plus[LUCK] -= Change_luc;


	if (ch.nCharacterData[SPELL])	plus[WSPS] -= Change_ps;
	else	plus[WSPS] -= Change_ws;

	ch.nCharacterMAXHP -= Change_Life_max;
	ch.nCharacterHP -= Change_Life_max;

	if (ch.nCharacterData[SPELL])
	{
		ch.nCharacterMAXMP -= Change_Divine_Max;
		ch.nCharacterMP -= Change_Divine_Max;
	}
	else
	{
		ch.nCharacterMAXMP -= Change_mana_max;
		ch.nCharacterMP -= Change_mana_max;
	}

	ch.nCharacterMAXSP -= Change_health_max;
	ch.nCharacterSP -= Change_health_max;
	return 1;
}

//#####################################################
// ETC ����
int CItem_Etc::ReleaseItem()
{
	SCharacter& ch = SCharacterData;
	int* plus = ch.nAbilityPlusOfWeapon;

	plus[STR] -= Change_str;
	plus[DEX] -= Change_dex;
	plus[CON] -= Change_con;
	plus[WIS] -= Change_wis;
	plus[INT_] -= Change_int;
	plus[CHA] -= Change_cha;
	plus[ENDU] -= Change_end;
	//	plus[MOVP] -= Change_Movp;
	plus[MOR] -= Change_mor;
	plus[LUCK] -= Change_luc;


	if (ch.nCharacterData[SPELL])	plus[WSPS] -= Change_ps;
	else	plus[WSPS] -= Change_ws;

	ch.nCharacterMAXHP -= Change_Life_max;
	ch.nCharacterHP -= Change_Life_max;

	if (ch.nCharacterData[SPELL])
	{
		ch.nCharacterMAXMP -= Change_Divine_Max;
		ch.nCharacterMP -= Change_Divine_Max;
	}
	else
	{
		ch.nCharacterMAXMP -= Change_mana_max;
		ch.nCharacterMP -= Change_mana_max;
	}

	ch.nCharacterMAXSP -= Change_health_max;
	ch.nCharacterSP -= Change_health_max;
	return 1;
}











/////////////////////////////////////////////////////////////////////////
// 
//  ������ ��ö �Լ� ���ϴ� �����Լ�
/////////////////////////////////////////////////////////////////////////

int CallItemVirtualFunction(int function, int item_number)
{
	if (!item_number) return 0;

	int type;
	int number;

	GetItemNumber(item_number, type, number);

	CItem* t = ItemUnit(type, number);
	if (!t) return 0;
	switch (function)
	{
	case VF_EQUIP_ITEM:	return t->EquipItem();
	case VF_RELEASE_ITEM:	return t->ReleaseItem();
	case VF_CURR_EQUIP:	return t->CurrEquip();
	case VF_EAT_ITEM:
	{
		int ret = t->EatItem();
		if (ret)
		{
			if (IsHeroDead()) return 0;
			Hero->hp = SCharacterData.nCharacterHP;
			Hero->mp = SCharacterData.nCharacterMP;
		}
		return ret;
	}break;
	default: return 0;
	}

	return 1;
}


int ChangeItemAttr(ItemAttr* item, int type, int plus, int skill)
{
	int item_type;
	int index;

	GetItemNumber(item->item_no, item_type, index);

	switch (type)
	{
	case CIA_DURATION: {
		WORD dur;
		WORD dur_max;
		dur = LOWORD(item->attr[IATTR_DURATION]);
		dur_max = HIWORD(item->attr[IATTR_DURATION]);

		dur += plus;

		if (dur > dur_max) dur = dur_max;
		else if (dur < 1) return 0; // ������ �ı��Լ� ��...

		item->attr[IATTR_DURATION] = MAKELONG(dur, dur_max);
		break;
	}
	case CIA_VIEW_CHANGE: {
		item->item_no = plus;
		break;
	}
	case CIA_MUCH: {
		;
		item->attr[IATTR_MUCH] += plus;
		CheckAutoDivideMoney(item);
		break;
	}
	}

	return 1;
}

extern char* GetItemName(int nItemno);
CItem_Join	item_join[ITEM_JOIN_MAX];
//////////////////////////////////////////////////////////////////////////////////////////
//
//		���� ���̺� �ε� �ؿ���
//
//////////////////////////////////////////////////////////////////////////////////////////
// ============================================================================
// 1. Item Join Table (合成表)
// ============================================================================

void CItem_Join::ReadItemJoin(char* buf)
{
    char* token;
    int* target;
    token = strtok(buf, "\t\n\r");
    if (token == NULL) return;
    no = atoi(token);

    token = strtok(NULL, "\t\n\r");
    if (token == NULL) return;
    item_id = atoi(token);

    token = strtok(NULL, "\t\n");
    if (token == NULL) return;
    strcpy(item_name, token);

    token = strtok(NULL, "\t\n\r");
    if (token == NULL) return;
    item_amount = atoi(token);

    token = strtok(NULL, "\t\n");
    if (token == NULL) return;
    strcpy(han_name, token);

    target = &need_know;
    int ct = 0;
    while ((token = strtok(NULL, "\t\n\r")) != NULL)
    {
        int t = atoi(token);
        if (!t && (token[0] < '0' || token[0]>'9'))
        {
            strcpy(explain, token);
        }
        else
        {
            // 防止指針越界
            *(target + ct) = t;
        }
        ct++;
    }
}

bool LoadItemJoinTable()
{
    // [Android] 路徑修正
    const char* filenameBin = "Itemtable/ItemJoin.bin";

    // ---------------------------------------------------------
    // 優先讀取 BIN (Android 唯一路徑)
    // ---------------------------------------------------------
    SDL_RWops* fp = SDL_RWFromFile(filenameBin, "rb");

    if (fp)
    {
        char* tt = (char*)item_join;
        char crc = 0, crc1 = 0, crc2 = 0;

        // 讀取 Header CRC
        SDL_RWread(fp, &crc1, 1, 1);

        // 讀取資料
        SDL_RWread(fp, item_join, sizeof(CItem_Join), ITEM_JOIN_MAX);

        // 讀取 Tail CRC
        SDL_RWread(fp, &crc2, 1, 1);

        SDL_RWclose(fp);

        // CRC 計算
        for (int i = 0; i < sizeof(CItem_Join) * ITEM_JOIN_MAX; i++)
        {
            crc += *tt;
            tt++;
        }

        if (crc != crc1 || crc != crc2)
        {
            CheckSumError = -1;
            // 建議使用 SDL_Log
            SDL_Log("Error: ItemJoinTable CRC Check Failed!");
            JustMsg(lan->OutputMessage(6, 84));
        }
        return true;
    }

    // ---------------------------------------------------------
    // 讀取 TXT (僅限 PC Debug，Android 略過)
    // ---------------------------------------------------------
#if defined(_DEBUG) && !defined(__ANDROID__)
    const char* filenameTxt = "itemtable/ItemJoin.txt";
    fp = SDL_RWFromFile(filenameTxt, "rb"); // 即使讀文字檔也用 rb
    if (fp)
    {
        // 讀取全部內容
        Sint64 size = SDL_RWsize(fp);
        std::string content;
        content.resize(size);
        SDL_RWread(fp, &content[0], size, 1);
        SDL_RWclose(fp);

        std::stringstream ss(content);
        std::string line;
        char buf[1024];
        int i = 0;

        while (std::getline(ss, line))
        {
            if (line.empty() || line[0] == ';') continue;
            // 移除 \r
            if (line.back() == '\r') line.pop_back();

            strncpy(buf, line.c_str(), 1023);
            buf[1023] = '\0'; // 安全結尾

            item_join[i].ReadItemJoin(buf);

            // 數值處理
            item_join[i].item_id /= 100;
            item_join[i].resource1 /= 100;
            item_join[i].resource2 /= 100;
            item_join[i].resource3 /= 100;
            item_join[i].resource4 /= 100;
            i++;
        }

        // 寫入 BIN (僅 PC)
        SDL_RWops* fpOut = SDL_RWFromFile(filenameBin, "wb");
        if (fpOut)
        {
            char* tt = (char*)item_join;
            char crc = 0;
            for (int k = 0; k < sizeof(CItem_Join) * ITEM_JOIN_MAX; k++)
            {
                crc += *tt;
                tt++;
            }
            SDL_RWwrite(fpOut, &crc, 1, 1);
            SDL_RWwrite(fpOut, item_join, sizeof(CItem_Join), ITEM_JOIN_MAX);
            SDL_RWwrite(fpOut, &crc, 1, 1);
            SDL_RWclose(fpOut);
        }
        return true;
    }
#endif

    SDL_Log("Error: Failed to load ItemJoinTable (Bin not found)");
    return false;
}

// ============================================================================
// 2. Item Sok Sung Kind (屬性種類表)
// ============================================================================

inline void ReadItemSokSungKind(char* buf, int i)
{
    char* token;
    token = strtok(buf, "\t\n\r");
    if (token == NULL) return;

    int idx = atoi(token);
    // [安全修正] 避免 exit(0)
    if (idx > MAX_ITEM_MUTANT_KIND) {
        SDL_Log("ItemSokSungKind Read Error: Index %d out of bounds", idx);
        return;
    }

    ItemMutantKind[idx].item_kind = atoi(token);

    token = strtok(NULL, "\t\n\r");    if (token == NULL) return;
    strcpy(ItemMutantKind[idx].exp_kind, token);
    EatRearWhiteChar(ItemMutantKind[idx].exp_kind);

    token = strtok(NULL, "\t\n"); if (token == NULL) return;
    ItemMutantKind[idx].upgrade_type = atoi(token);

    for (int Count = 0; Count < 8; Count++)
    {
        token = strtok(NULL, "\t\n"); if (token == NULL) return;
        ItemMutantKind[idx].AddItem[Count] = atoi(token);
    }

    token = strtok(NULL, "\t\n"); if (token == NULL) return;   ItemMutantKind[idx].iBuffer1 = atoi(token);
    token = strtok(NULL, "\t\n"); if (token == NULL) return;   ItemMutantKind[idx].iBuffer2 = atoi(token);
    token = strtok(NULL, "\t\n"); if (token == NULL) return;   ItemMutantKind[idx].iBuffer3 = atoi(token);
    token = strtok(NULL, "\t\n"); if (token == NULL) return;   ItemMutantKind[idx].iBuffer4 = atoi(token);
}

bool LoadItemSokSungKindTable()
{
    // [Android] 路徑修正: 移除 .\\ 並改用斜線
    const char* filenameBin = "Itemtable/ItemSokSungKind.bin";

    // 優先讀取 BIN
    SDL_RWops* fp = SDL_RWFromFile(filenameBin, "rb");
    if (fp)
    {
        SDL_RWread(fp, ItemMutantKind, sizeof(t_ItemMutantKind) * MAX_ITEM_MUTANT_KIND, 1);
        SDL_RWclose(fp);
        return true;
    }

    // 讀取 TXT (僅 PC Debug)
#if defined(_DEBUG) && !defined(__ANDROID__)
    #ifndef LIGHT_GM_CLIENT
    const char* filenameTxt = "itemtable/ItemSokSungKind.txt";
    fp = SDL_RWFromFile(filenameTxt, "rb");
    if (fp)
    {
        Sint64 size = SDL_RWsize(fp);
        std::string content;
        content.resize(size);
        SDL_RWread(fp, &content[0], size, 1);
        SDL_RWclose(fp);

        std::stringstream ss(content);
        std::string line;
        char buf[1024];
        int i = 0;

        while (std::getline(ss, line))
        {
            if (line.empty() || line[0] == ';') continue;
            if (line.back() == '\r') line.pop_back();
            if (i > MAX_ITEM_MUTANT_KIND - 1) break;

            strncpy(buf, line.c_str(), 1023);
            GetItemName(i); // 假設此函式存在
            ReadItemSokSungKind(buf, i);
            i++;
        }

        // 寫入 BIN
        SDL_RWops* fpOut = SDL_RWFromFile(filenameBin, "wb");
        if (fpOut) {
            SDL_RWwrite(fpOut, ItemMutantKind, sizeof(t_ItemMutantKind) * MAX_ITEM_MUTANT_KIND, 1);
            SDL_RWclose(fpOut);
        }
        return true;
    }
#endif
#endif

    return false;
}

// ============================================================================
// 3. Item Sok Sung Grade (屬性等級表)
// ============================================================================

inline void ReadItemSokSungGrade(char* buf, int i)
{
    char* token;
    int* TargetPointer = &ItemGrade[i].iGrade;

    token = strtok(buf, "\t\n\r"); if (token == NULL) return;

    int idx = atoi(token);
    // [安全修正]
    if (idx >= MAX_ITEM_GRADE) {
        SDL_Log("ItemSokSungGrade Read Error: Index out of bounds");
        return;
    }

    (*TargetPointer) = atoi(token);
    TargetPointer++;

    for (int j = 0; j < 10; j++)
    {
        token = strtok(NULL, "\t\n\r"); if (token == NULL) return;
        (*TargetPointer) = atoi(token);
        TargetPointer++;
    }
}

bool LoadItemSokSungGradeTable()
{
    const char* filenameBin = "Itemtable/ItemSokSungGrade.bin";

    // 優先讀取 BIN
    SDL_RWops* fp = SDL_RWFromFile(filenameBin, "rb");
    if (fp)
    {
        SDL_RWread(fp, ItemGrade, sizeof(t_ItemGrade) * MAX_ITEM_GRADE, 1);
        SDL_RWclose(fp);
        return true;
    }

    // 讀取 TXT (僅 PC)
#if defined(_DEBUG) && !defined(__ANDROID__)
    const char* filenameTxt = "itemtable/ItemSokSungGrade.txt";
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

       while (std::getline(ss, line)) {
          if (line.empty() || line[0] == ';') continue;
          if (line.back() == '\r') line.pop_back();
          if (i > MAX_ITEM_GRADE - 1) break;
          strncpy(buf, line.c_str(), 1023);
          ReadItemSokSungGrade(buf, i);
          i++;
       }

       SDL_RWops* fpOut = SDL_RWFromFile(filenameBin, "wb");
       if (fpOut) {
          SDL_RWwrite(fpOut, ItemGrade, sizeof(t_ItemGrade) * MAX_ITEM_GRADE, 1);
          SDL_RWclose(fpOut);
       }
       return true;
    }
#endif
    return false;
}

// ============================================================================
// 4. Item Sok Sung (屬性表 - 包含語言判斷)
// ============================================================================

inline void ReadItemSokSung(char* buf, int i)
{
    char* token;
    token = strtok(buf, "\t\n\r"); if (token == NULL) return;

    int idx = atoi(token);
    // [安全修正]
    if (idx >= MAX_ITEM_RARE) {
        SDL_Log("ItemSokSung Read Error: Index out of bounds");
        return;
    }

    ItemRare[idx].iNo = idx;
    token = strtok(NULL, "\t\n\r");    if (token == NULL) return; ItemRare[idx].iType = atoi(token);
    token = strtok(NULL, "\t\n"); if (token == NULL) return;      strcpy(ItemRare[idx].EngName, token);    EatRearWhiteChar(ItemRare[idx].EngName);
    token = strtok(NULL, "\t\n"); if (token == NULL) return;      strcpy(ItemRare[idx].LocalName, token);  EatRearWhiteChar(ItemRare[idx].LocalName);
    token = strtok(NULL, "\t\n"); if (token == NULL) return;      strcpy(ItemRare[idx].Explain, token);       EatRearWhiteChar(ItemRare[idx].Explain);
    token = strtok(NULL, "\t\n"); if (token == NULL) return;      strcpy(ItemRare[idx].ExplainAdd, token); EatRearWhiteChar(ItemRare[idx].ExplainAdd);
    token = strtok(NULL, "\t\n"); if (token == NULL) return;      ItemRare[idx].iExpMark = atoi(token);
    token = strtok(NULL, "\t\n"); if (token == NULL) return;      ItemRare[idx].iMakeAble = atoi(token);

    for (int Count = 0; Count < 8; Count++) {
        token = strtok(NULL, "\t\n"); if (token == NULL) return;      ItemRare[idx].RaisePro[Count] = atoi(token);
    }
    token = strtok(NULL, "\t\n"); if (token == NULL) return;      ItemRare[idx].iUpgradeAble = atoi(token);
    token = strtok(NULL, "\t\n"); if (token == NULL) return;      ItemRare[idx].iAbleNation = atoi(token);
    for (int Count = 0; Count < 16; Count++) {
        token = strtok(NULL, "\t\n"); if (token == NULL) return;      ItemRare[idx].GRADE[Count] = atoi(token);
    }
    token = strtok(NULL, "\t\n"); if (token == NULL) return;      ItemRare[idx].Buffer1 = atoi(token);
    token = strtok(NULL, "\t\n"); if (token == NULL) return;      ItemRare[idx].Buffer2 = atoi(token);
    token = strtok(NULL, "\t\n"); if (token == NULL) return;      ItemRare[idx].Buffer3 = atoi(token);
    token = strtok(NULL, "\t\n"); if (token == NULL) return;      ItemRare[idx].Buffer4 = atoi(token);
    token = strtok(NULL, "\t\n"); if (token == NULL) return;      ItemRare[idx].Buffer5 = atoi(token);
    token = strtok(NULL, "\t\n"); if (token == NULL) return;      ItemRare[idx].Buffer6 = atoi(token);
}

bool LoadItemSokSungTable()
{
    char filenameBin[MAX_FILENAME]={0};

    // [Android] 路徑修正
    if (system_info.lang == 0) {
        snprintf(filenameBin, MAX_FILENAME, "Itemtable/ItemSokSung.bin_en");
    }
    else {
        snprintf(filenameBin, MAX_FILENAME, "Itemtable/ItemSokSung.bin_ch");
    }

    // 優先讀取 BIN
    SDL_RWops* fp = SDL_RWFromFile(filenameBin, "rb");
    if (fp)
    {
        SDL_RWread(fp, ItemRare, sizeof(t_ItemRare) * MAX_ITEM_RARE, 1);
        SDL_RWclose(fp);
        return true;
    }

    // 讀取 TXT (僅 PC Debug)
#if defined(_DEBUG) && !defined(__ANDROID__)
    #ifndef LIGHT_GM_CLIENT
    const char* filenameTxt = "itemtable/ItemSokSung.txt";
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

        while (std::getline(ss, line)) {
            if (line.empty() || line[0] == ';') continue;
            if (line.back() == '\r') line.pop_back();
            if (i > MAX_ITEM_RARE - 1) break;
            strncpy(buf, line.c_str(), 1023);
            ReadItemSokSung(buf, i);
            i++;
        }

        SDL_RWops* fpOut = SDL_RWFromFile(filenameBin, "wb");
        if (fpOut) {
            SDL_RWwrite(fpOut, ItemRare, sizeof(t_ItemRare) * MAX_ITEM_RARE, 1);
            SDL_RWclose(fpOut);
        }
        return true;
    }
#endif
#endif

    return false;
}

extern SkillMain SkillTable[MAX_SKILL_];
void SkillUseItem(int skill, int menu)
{
	memset(skill_use_item, 0, sizeof(PosItem[5][10]));
	memset(&skill_use_item_result, 0, sizeof(PosItem));

	PutSkillUseItem_skill_use_item(skill);

	MainMenuClose();
	CallMenu(MN_ITEM_SELECT, menu, true);
	//	y_MenuFocus = MN_ITEM_SELECT;
}

//010708 lsw ����� �ִ� gm���� ���
int iGM_Makeable_Item_List[ITEM_JOIN_MAX];
// ���� ��ų
const int iGM_2ND_Mother_Skill[7] = { SKILL_CARPENTRY,	SKILL_BLACKSMITHY,	SKILL_COOKING,	SKILL_TAILORING,	SKILL_BOWCRAFTING,	SKILL_MAKEPOTION,	SKILL_CANDLEMAKING };
const int iSkill_Master_Mother_Skill[4] = { SKILL_BLACKSMITHY,	SKILL_CARPENTRY, SKILL_BOWCRAFTING	,SKILL_TAILORING };
int CanMakeItemListSet(int s, int menu)		// ����� �ִ� ������ ���
{//��ų�� ���� �������̽��� �θ� //021111 lsw
	int iHowTo = SkillTable[s].Howto;
	int count = 0;
	switch (iHowTo)
	{
	case SKILLTARGETTYPE_TABLE_:
	{
		memset(Item_Select, 0, sizeof(ItemJoinSelect[MAX_LEARN_SKILL_PAGE][10]));
		memset(&item_select_result, 0, sizeof(ItemJoinSelect));
		for (int i = 0; i < ITEM_JOIN_MAX; i++)
		{
			if (item_join[i].item_id && GetItemAttr(item_join[i].item_id, IMAGE_NUMBER))
			{
				if (item_join[i].need_skill1 == s)
				{
					if (!FindLearnItem(item_join[i].item_id)) continue;

					Item_Select[count / 10][count % 10].item_no = item_join[i].item_id;
					Item_Select[count / 10][count % 10].skill_min = item_join[i].skill1_minimum;
					Item_Select[count / 10][count % 10].kind = ItemUnit(item_join[i].item_id)->GetItemKind();
					Item_Select[count / 10][count % 10].skillability = ItemUnit(item_join[i].item_id)->GetSkillAbility();
					count++;
				}
			}
		}
		//���ķ�ƾ �ִ°�..
		{
			int i, j;
			int n = count;
			ItemJoinSelect t;
			ItemJoinSelect* a = &Item_Select[0][0];

			for (i = 1; i < n; i++)
			{
				t = a[i];
				j = i;
				while (a[j - 1].item_no > t.item_no && j > 0)//���Ե� �� ã��
				{
					a[j] = a[j - 1];//�ڷ� �ű�
					j--;
				}
				a[j] = t;//����
			}


			for (i = 1; i < n; i++)
			{
				t = a[i];
				j = i;
				while (a[j - 1].skillability > t.skillability && j > 0 && a[j - 1].item_no / 1000 == t.item_no / 1000)//���Ե� �� ã��
				{
					a[j] = a[j - 1];//�ڷ� �ű�
					j--;
				}
				a[j] = t;//����
			}

			for (i = 1; i < n; i++)
			{
				t = a[i];
				j = i;
				while (a[j - 1].kind > t.kind && j > 0 && a[j - 1].kind == t.kind)//���Ե� �� ã��
				{
					a[j] = a[j - 1];//�ڷ� �ű�
					j--;
				}
				a[j] = t;//����
			}

			for (i = 1; i < n; i++)
			{
				t = a[i];
				j = i;
				while (a[j - 1].skill_min > t.skill_min && j > 0 && a[j - 1].kind == t.kind)//���Ե� �� ã��
				{
					a[j] = a[j - 1];//�ڷ� �ű�
					j--;
				}
				a[j] = t;//����
			}

			//	�̰� �Ѥ�;;
				//��ﶧ�� ����� �ȹ���� ������..
				//��ų Kind JOin ���̺�
				//SKill_min
				//������ �ѹ�
				//011111 lsw 12345 -->> �� ����
				//a^=b^=a^=b �ϸ� �ȴ� ^^
				//item Type �޾� ���� �� �ְ�  /1000
				//ī�ε�� ����
				//Ÿ�Ժ��� SKil_min���� ����   /
				//���� SKil_min �� ������ ������ �ѹ��� ����

		}
		if (count)
		{
			MainMenuClose();
			//011025 lsw >
			if (SDL_GetTicks() + 1000 > g_skillmaketime)
			{
				sprintf(SMenu[MN_ITEM_MAKE_SELECT].nField[10].temp, "0");
				CallMenu(MN_ITEM_MAKE_SELECT, menu, true);//011012 lsw
				SMenu[MN_ITEM_MAKE_SELECT].nField[2].nSHideNomalCount = count / 10 + 1;
				g_skillmaketime = 0;
			}
			else
			{
				SkillStatus = SKILL_OFF;
				nItemOther = 0;
				SkillSettingClear();
				//011110 lsw 
				//	char msg[300];
				//	sprintf( msg,lan->OutputMessage(2,256),(g_skillmaketime - SDL_GetTicks())/1000);
					//CallOkCancelMessageBox(FALSE,0,0,msg,0);
				AddCurrentStatusMessage(FONT_COLOR_YELLOW, lan->OutputMessage(2, 256), (g_skillmaketime - SDL_GetTicks()) / 1000);

			}
			//011025 lsw <
		}
		else
		{
			MP3(SN_WARNING);
			CallOkCancelMessageBox(menu, 0, 0, lan->OutputMessage(6, 85), 0);//010215 lsw
			SkillSettingClear();
		}
		return count;
	}break;
	case	SKILLTARGETTYPE_MAP_WITH_ITEM_:
	{
		SkillUseItem(s, menu);
		return 0;
	}break;
	case	SKILLTARGETTYPE_2ND_GM://2ND ��ų ����//���� �� �� �ٲ� ��
	{
		count = 0;
		memset(iGM_Makeable_Item_List, 0, 4 * ITEM_JOIN_MAX);// �ʱ�ȭ 40 �� �³�? 
		if (106 > s || 112 < s) return 0;//106�� ������ gm��ų �������� 106 �̴ϱ� ��.��
		int convert_skill = iGM_2ND_Mother_Skill[s - 106];
		for (int i = 0; i < ITEM_JOIN_MAX; i++)
		{
			if (item_join[i].item_id && GetItemAttr(item_join[i].item_id, IMAGE_NUMBER))
			{
				if (item_join[i].need_skill1 == convert_skill)//��ų�� ��Ī�� �־�� �Ѵ�.)
				{
					if (!FindLearnItem(item_join[i].item_id)) continue;
					if (count > ITEM_JOIN_MAX)//����� �������� 500�� ���� ���Ѵ�.
					{
						MP3(SN_WARNING);
						CallOkCancelMessageBox(menu, 0, 0, lan->OutputMessage(4, 106), 0);//����
						SkillSettingClear();
					}
					iGM_Makeable_Item_List[count] = (item_join[i].item_id);//����� �ִ� ���� ����Ʈ
					count++;
				}
			}
		}
		if (count)
		{
			::MainMenuClose();
			::Call_GM_2ND_Skill_Menu();
		}
		else
		{
			CallOkCancelMessageBox(menu, 0, 0, lan->OutputMessage(4, 107), 0);//����
		}
	}break;
	case	SKILLTARGETTYPE_SKILL_MASTER:
	{

		count = 0;

		if (!::IsMerchant())return count;//soto-HK030928 ������ �ƴϸ� �⼺�� �ƴϴ�.

		memset(iGM_Makeable_Item_List, 0, 4 * ITEM_JOIN_MAX);// �ʱ�ȭ 40 �� �³�? 
		if (s > 72 || s < 68)
		{
			return 0;//skillmain ���̺� ������
		}
		int convert_skill = iSkill_Master_Mother_Skill[s - 68];
		for (int i = 0; i < ITEM_JOIN_MAX; i++)
		{
			if (item_join[i].item_id && GetItemAttr(item_join[i].item_id, IMAGE_NUMBER))
			{
				if (item_join[i].need_skill1 == convert_skill)//��ų�� ��Ī�� �־�� �Ѵ�.)
				{
					if (!FindLearnItem(item_join[i].item_id)) continue;
					if (count > ITEM_JOIN_MAX)//����� �������� 500�� ���� ���Ѵ�.
					{
						MP3(SN_WARNING);
						CallOkCancelMessageBox(menu, 0, 0, lan->OutputMessage(4, 106), 0);//����
						SkillSettingClear();
					}
					iGM_Makeable_Item_List[count] = (item_join[i].item_id);//����� �ִ� ���� ����Ʈ
					count++;
				}
			}
		}
		if (count)
		{
			::MainMenuClose();
			::Call_Skill_Master_Menu();
		}
		else
		{
			CallOkCancelMessageBox(menu, 0, 0, lan->OutputMessage(4, 107), 0);//����
		}
	}break;
	default:
		break;
	}
	return count;
}


void CalcGuildToSkill()
{
	memset(inclusive_to_skill, 0, sizeof(int) * MAX_SKILL_KIND * 8);
	for (int i = 0; i < MAX_SKILL; i++)
	{
		int para = skill[i].inclusive;
		for (int j = 0; j < 8; j++)
		{
			if (!inclusive_to_skill[para][j])
			{
				inclusive_to_skill[para][j] = i;	// ��ų ��ȣ �Է�
				break;
			}
		}
	}
}



ItemAttr* GetItemByPos(POS item)
{
	switch (item.type)
	{
	case	HAND:	return &HandItemAttr;
	case	INV:	return &InvItemAttr[item.p1][item.p2][item.p3];
	case	EQUIP:	return &EquipItemAttr[item.p3];
	case	QUICK:	return &QuickItemAttr[item.p3];
	case	BANK:	return &BankItemAttr[item.p1][item.p2][item.p3];
	case	BANK2:	return &Bank2ItemAttr[item.p1][item.p2][item.p3];
	case	MAKE:	//
	case	OTHER_CH:
	default:
	{
		return NULL;
	}break;
	}
	return NULL;
}



////////////////////////////////////////////////////////////////////////////////
//
//	������ ��ȣ�� Ÿ���� ���� ���� �Ѿ������� �Ǵ����ϴ� �Լ�
//
//		�Ķ���ʹ� Ÿ�԰� ��ȣ�� �Է� �ް�
//		�����ڷ� �����Ѵ�.	�� �� ���� ��� NULL����
//
//
////////////////////////////////////////////////////////////////////////////////
CItem* ItemUnit(int type, int item_no)
{
	if ((type < 0) || (type > 10)) goto ITEM_UNIT_NOT;
	if ((item_no < 0) || (item_no >= Item_Ref.nItem[type])) goto ITEM_UNIT_NOT;
	
	return Item_Ref.Item_type[type][item_no];

ITEM_UNIT_NOT:
	return NULL;
}

CItem* ItemUnit(ItemAttr item)
{
	int type;
	int num;
	GetItemNumber(item.item_no, type, num);

	return ItemUnit(type, num);
}
CItem* ItemUnit(int item_no)
{
	return ItemUnit(item_no / 1000, item_no % 1000);
}

int DeleteItem(ItemAttr* item)
{
	int type, num;
	GetItemNumber(item->item_no, type, num);

	CItem* t = ItemUnit(type, num);
	if (!t) return -1;

	int attr = GetItemAttr(item->item_no, WEAR_ABLE);
	//SDL_Log("attr %d", attr);
	int wt = WearAble[GetItemAttr(item->item_no, WEAR_ABLE)];
	memset(item, 0, sizeof(ItemAttr));
	if (wt) ChangeEquip(Hero, EquipItemAttr, type);

	return 1;
}

int DeleteItem(POS pos)
{
	ItemAttr* t = ::GetItemByPos(pos);
	if (t)	return DeleteItem(t);
	else return 0;
}


///////////////////////////////////////////////////
// ��� �ʿ䰡 ���� �������� join ���̺���� ������ 
// ��� �����ۿ� �־� �д�.
void SetLearnItem()
{
	for (int i = 0; i < ITEM_JOIN_MAX; i++)
	{
		if (!item_join[i].need_know && item_join[i].item_id)
		{
			SCharacterData.ItemId[SCharacterData.ItemIdCount++] = item_join[i].item_id;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
//		������ ��ȣ�� ��ų ��ȣ�� ������ ���� ���̺��� �����Ͽ� 
//		���� ���� �ִ� ���������� �ƴ����� �Ǵ��Ѵ�.
//////////////////////////////////////////////////////////////////////////////
bool CanLearnItem(int item, int price, int& join_no)
{
	for (int i = 0; i < ITEM_JOIN_MAX; i++)
	{
		if (item_join[i].item_id == item && item_join[i].tech_price == menu_to_set[price])
		{
			join_no = i;
			return true;
		}
	}
	return false;
}

bool CanLearnItemForExplain(int item)
{
	for (int i = 0; i < ITEM_JOIN_MAX; i++)
	{
		if (item_join[i].item_id == item)
		{
			//011012 lsw
			if ((item_join[i].need_skill1 ? (SCharacterData.SkillId[item_join[i].need_skill1] && SCharacterData.nJobSkill[skill[item_join[i].need_skill1].inclusive].skillexp / 10000 >= (unsigned int)item_join[i].skill1_minimum) : 1)
				&& (item_join[i].need_skill2 ? (SCharacterData.SkillId[item_join[i].need_skill2] && SCharacterData.nJobSkill[skill[item_join[i].need_skill2].inclusive].skillexp / 10000 >= (unsigned int)item_join[i].skill2_minimum) : 1))
			{
				return true;
			}
			else return false;
		}
	}
	return false;
}

bool CanLearnItemByNeedSkill(int join_no)
{
	//011012 lsw
	if ((item_join[join_no].need_skill1 ? (SCharacterData.SkillId[item_join[join_no].need_skill1] && SCharacterData.nJobSkill[skill[item_join[join_no].need_skill1].inclusive].skillexp / 10000 >= (unsigned int)item_join[join_no].skill1_minimum) : 1)
		&& (item_join[join_no].need_skill2 ? (SCharacterData.SkillId[item_join[join_no].need_skill2] && SCharacterData.nJobSkill[skill[item_join[join_no].need_skill2].inclusive].skillexp / 10000 >= (unsigned int)item_join[join_no].skill2_minimum) : 1))
	{
		return true;
	}
	else return false;
}


int GetWeight(ItemAttr item)
{
	int weight = 0;
	int type, num;
	GetItemNumber(item.item_no, type, num);

	CItem* t = ItemUnit(type, num);
	if (!t) return 1;
	weight = t->GetItemWeight();

	if (type == DISPOSABLE)		// ��� ��...
	{
		int dur = t->GetQuantity();
		if (dur > 0) weight *= item.attr[IATTR_MUCH] / dur;
		else weight *= item.attr[IATTR_MUCH];
	}
	else if (t->GetRButton() == DIVIDE_ITEM)
	{
		int dur = t->GetItemDuration();		// ����..
		if (dur > 0)
			weight *= item.attr[IATTR_MUCH] / dur;
		else weight *= item.attr[IATTR_MUCH];
	}
	return weight;
}



ItemAttr Item_Number2Attr(int item)
{
	ItemAttr temp = { 0, };

	int type, num;
	GetItemNumber(item, type, num);

	CItem* t = ItemUnit(type, num);
	if (t)
	{
		WORD dur = GetItemAttr(item, DURATION);
		temp.item_no = item;
		temp.attr[IATTR_DURATION] = MAKELONG(dur, dur);
		int limit = GetItemAttr(item, ITEM_LIMIT);
		if (limit > 0)
		{
			temp.attr[IATTR_LIMIT] = limit + g_curr_time;
		}
	}
	return temp;
}

void GetItemDuration(ItemAttr item, WORD& d_curr, WORD& d_max)
{
	d_curr = LOWORD(item.attr[IATTR_DURATION]);
	d_max = HIWORD(item.attr[IATTR_DURATION]);
}



void DeleteItemByItemNumber(int item)
{
	int i;
	for (i = 0; i < 3; i++)		// inventory
		for (int j = 0; j < 3; j++)
			for (int k = 0; k < 8; k++)
			{
				if (item == InvItemAttr[i][j][k].item_no)
				{
					DeleteItem(&InvItemAttr[i][j][k]);
					return;
				}
			}
	for (i = 0; i < 8; i++)			// equip inv
	{
		if (item == EquipItemAttr[i].item_no)
		{
			DeleteItem(&EquipItemAttr[i]);
			return;
		}
	}

	for (i = 0; i < 6; i++)			// quick inv
	{
		if (item == QuickItemAttr[i].item_no)
		{
			DeleteItem(&QuickItemAttr[i]);
			return;
		}
	}
}

void CItem_Plant::CopyItemClass(CItem_Plant* source, int ct)
{
	Item_id = source->Item_id;
	strcpy(Name, source->Name);
	strcpy(Han_Name, source->Han_Name);
	int* item_value = &lv;
	int* source_item_value = &source->lv;
	for (int i = 0; i < ct - 3; i++)
	{
		*item_value = *source_item_value;
		item_value++;
		source_item_value++;
	}
	CItem::CopyItemClass();
}
void CItem_Mineral::CopyItemClass(CItem_Mineral* source, int ct)
{
	Item_id = source->Item_id;
	strcpy(Name, source->Name);
	strcpy(Han_Name, source->Han_Name);

	int* item_value = &lv;
	int* source_item_value = &source->lv;
	for (int i = 0; i < ct - 3; i++)
	{
		*item_value = *source_item_value;
		item_value++;
		source_item_value++;
	}
	CItem::CopyItemClass();
}
void CItem_Herb::CopyItemClass(CItem_Herb* source, int ct)
{
	Item_id = source->Item_id;
	strcpy(Name, source->Name);
	strcpy(Han_Name, source->Han_Name);

	int* item_value = &lv;
	int* source_item_value = &source->lv;
	for (int i = 0; i < ct - 3; i++)
	{
		*item_value = *source_item_value;
		item_value++;
		source_item_value++;
	}
	CItem::CopyItemClass();
}
void CItem_Cook::CopyItemClass(CItem_Cook* source, int ct)
{
	Item_id = source->Item_id;source->Name[30]=0;
	sprintf(Name, "%s", source->Name);
	sprintf(Han_Name, "%s", source->Han_Name);

	int* item_value = &lv;
	int* source_item_value = &source->lv;
	for (int i = 0; i < ct - 3; i++)
	{
		*item_value = *source_item_value;
		item_value++;
		source_item_value++;
	}
	CItem::CopyItemClass();
}
void CItem_Potion::CopyItemClass(CItem_Potion* source, int ct)
{
	Item_id = source->Item_id;
	strcpy(Name, source->Name);
	strcpy(Han_Name, source->Han_Name);

	int* item_value = &lv;
	int* source_item_value = &source->lv;
	for (int i = 0; i < ct - 3; i++)
	{
		*item_value = *source_item_value;
		item_value++;
		source_item_value++;
	}
	CItem::CopyItemClass();
}
void CItem_Tool::CopyItemClass(CItem_Tool* source, int ct)
{
	Item_id = source->Item_id;
	strcpy(Name, source->Name);
	strcpy(Han_Name, source->Han_Name);

	int* item_value = &lv;
	int* source_item_value = &source->lv;
	for (int i = 0; i < ct - 3; i++)
	{
		*item_value = *source_item_value;
		item_value++;
		source_item_value++;
	}
	CItem::CopyItemClass();
}
void CItem_Weapon::CopyItemClass(CItem_Weapon* source, int ct)
{
	Item_id = source->Item_id;
	strcpy(Name, source->Name);
	strcpy(Han_Name, source->Han_Name);

	int* item_value = &lv;
	int* source_item_value = &source->lv;
	for (int i = 0; i < ct - 3; i++)
	{
		*item_value = *source_item_value;
		item_value++;
		source_item_value++;
	}
	CItem::CopyItemClass();
}
void CItem_Disposable::CopyItemClass(CItem_Disposable* source, int ct)
{
	Item_id = source->Item_id;
	strcpy(Name, source->Name);
	strcpy(Han_Name, source->Han_Name);

	int* item_value = &lv;
	int* source_item_value = &source->lv;
	for (int i = 0; i < ct - 3; i++)
	{
		*item_value = *source_item_value;
		item_value++;
		source_item_value++;
	}
	CItem::CopyItemClass();
}
void CItem_Armor::CopyItemClass(CItem_Armor* source, int ct)
{
	Item_id = source->Item_id;
	strcpy(Name, source->Name);
	strcpy(Han_Name, source->Han_Name);

	int* item_value = &lv;
	int* source_item_value = &source->lv;
	for (int i = 0; i < ct - 3; i++)
	{
		*item_value = *source_item_value;
		item_value++;
		source_item_value++;
	}
	CItem::CopyItemClass();
}
void CItem_Accessory::CopyItemClass(CItem_Accessory* source, int ct)
{
	Item_id = source->Item_id;
	strcpy(Name, source->Name);
	strcpy(Han_Name, source->Han_Name);

	int* item_value = &lv;
	int* source_item_value = &source->lv;
	for (int i = 0; i < ct - 3; i++)
	{
		*item_value = *source_item_value;
		item_value++;
		source_item_value++;
	}
	CItem::CopyItemClass();
}
void CItem_Etc::CopyItemClass(CItem_Etc* source, int ct)
{
	Item_id = source->Item_id;source->Name[30]=0;
	sprintf(Name, "%s", source->Name);
	sprintf(Han_Name, "%s", source->Han_Name);

	int* item_value = &lv;
	int* source_item_value = &source->lv;
	for (int i = 0; i < ct - 3; i++)
	{
		*item_value = *source_item_value;
		item_value++;
		source_item_value++;
	}
	CItem::CopyItemClass();
}


int GetItemEquipSound(int item_no)
{
	int kind = GetItemAttr(item_no, ITEM_KIND);
	switch (kind)
	{
	case IK_ROBE:
	case IK_LEATHER_ARMOR:
	case IK_NORMAL_CLOTHE:  return SN_CLOTHE;
	}

	return 0;
}

void EquipSound(int item_no)
{
	int type = GetItemAttr(item_no, WEAR_ABLE);
	int sound = 0;

	switch (type)
	{
	case WEAR_BELT:
	case WEAR_ANYTHING:
	case WEAR_ONLY_INVEN:	break;
	case WEAR_LEFT_HAND:	MP3(SN_SHIELD); break;
	case WEAR_TWO_HAND:
	case WEAR_RIGHT_HAND:	MP3(SN_WEAPON); break;
	case WEAR_HEAD:	MP3(SN_HELMET); break;
	case WEAR_BODY:	sound = GetItemEquipSound(item_no);
		if (sound) MP3(sound);
		else MP3(SN_ARMOR);
		break;
	case WEAR_NECK:	MP3(SN_NECK); break;
	case WEAR_SHOES:	MP3(SN_SHOES); break;
	case WEAR_UNIQUE:	MP3(SN_UNIQ); break;
	}
	return;
}


void FreeItemIcon()
{
	for (int i = 0; i < ITEM_MAX; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (Icon1[i][j].img) MemFree(Icon1[i][j].img);
			if (Icon2[i][j].img) MemFree(Icon2[i][j].img);
			if (Icon3[i][j].img) MemFree(Icon3[i][j].img);
			if (Icon4[i][j].img) MemFree(Icon4[i][j].img);
			if (Icon5[i][j].img) MemFree(Icon5[i][j].img);
			if (Icon6[i][j].img) MemFree(Icon6[i][j].img);
			if (Icon7[i][j].img) MemFree(Icon7[i][j].img);
			if (Icon8[i][j].img) MemFree(Icon8[i][j].img);
			if (Icon9[i][j].img) MemFree(Icon9[i][j].img);
			if (Icon10[i][j].img) MemFree(Icon10[i][j].img);
			if (Icon11[i][j].img) MemFree(Icon11[i][j].img);
			if (Icon12[i][j].img) MemFree(Icon12[i][j].img);
			if (Icon13[i][j].img) MemFree(Icon13[i][j].img);

		}
	}
}


int GetTotalWeight()
{
	int a, b, c;
	int weight = 0;
	for (a = 0; a < 3; a++)
		for (b = 0; b < 3; b++)
			for (c = 0; c < 8; c++)
				if (InvItemAttr[a][b][c].item_no)
					weight += GetWeight(InvItemAttr[a][b][c]);
	for (a = 0; a < 6; a++)
		if (QuickItemAttr[a].item_no)	weight += GetWeight(QuickItemAttr[a]);
	for (a = 0; a < 8; a++)
		if (EquipItemAttr[a].item_no)	weight += GetWeight(EquipItemAttr[a]);
	if (HandItemAttr.item_no) weight += GetWeight(HandItemAttr);
	return weight;
}

int CalMovpByWeight()
{
	int result = 0;
	int max = Hero_Ability[STR] * 100;		// ��� �ִ� ���� �ƽ�
	if (SCharacterData.nWeight <= max) return Hero_Ability[MOVP];

	if (max)
	{
		float rate = (SCharacterData.nWeight - max) / ((float)max) * 100.0f;
		int move_down = (int)((rate * 2) / 100.0f * Hero_Ability[MOVP]);
		result = Hero_Ability[MOVP] - move_down;
	}
	if (result < 0) result = 0;
	return result;
}


/**
 * @brief 從指定的二進位檔案讀取魔法名稱到全域陣列。
 * @param bin_path 要讀取的二進位檔案路徑。
 * @param name_count 要讀取的名稱數量。
 * @param offset 在 g_MagicName 中開始寫入的索引偏移。
 * @return 成功返回 true，失敗返回 false。
 */
bool ReadNamesFromFile(const std::__fs::filesystem::path& bin_path, int name_count,
                       int offset) {
  // 1. 使用 static_assert 確保 CMagic 的安全性，這點非常重要
  /*static_assert(std::is_trivially_copyable_v<CMagic>,
                "CMagic must be trivially copyable for binary I/O.");*/

  // 2. 使用 ifstream 進行檔案讀取 (RAII，自動關檔)
  std::ifstream file(bin_path, std::ios::binary);
  if (!file) {
    std::cerr << "錯誤: 無法開啟檔案 " << bin_path << std::endl;
    return false;
  }

  // 3. 使用 std::vector 避免堆疊溢位 (Stack Overflow)
  std::vector<CMagic> temp_magic(MAX_MAGIC_);
  char crc1, crc2;

  file.read(&crc1, sizeof(crc1));
  file.read(reinterpret_cast<char*>(temp_magic.data()),
            sizeof(CMagic) * MAX_MAGIC_);
  file.read(&crc2, sizeof(crc2));

  if (file.gcount() == 0 || crc1 != crc2) {
    std::cerr << "錯誤: 檔案讀取不完整或頭尾 CRC 不匹配 " << bin_path
              << std::endl;
    return false;
  }

  // 4. 實現 CRC 校驗，確保資料完整性
  const char* data_ptr = reinterpret_cast<const char*>(temp_magic.data());
  char calculated_crc = std::accumulate(
      data_ptr, data_ptr + sizeof(CMagic) * MAX_MAGIC_, char{0});
  if (calculated_crc != crc1) {
    std::cerr << "錯誤: 資料校驗失敗 " << bin_path << std::endl;
    return false;
  }

  // 5. 根據語言版本，安全地複製名稱
  for (int i = 0; i < name_count; ++i) {
    // 取得對應語言的名稱指標
    const char* source_name = (system_info.lang == 0)
                                  ? temp_magic[i].GetName()
                                  : temp_magic[i].GetHanName();

    if (source_name) {
      // **重要**: 使用 strncpy 取代 strcpy 來防止緩衝區溢位
      strncpy(g_MagicName[offset + i], source_name, 30);
      // 確保字串結尾一定是 null-terminator
      g_MagicName[offset + i][30] = '\0';
    }
  }
  return true;
}

// 主函式現在變得非常簡潔清晰
bool LoadMagicName()  // 建議回傳 bool 表示成功或失敗
{
  const std::string lang_suffix =
      (system_info.lang == 0) ? "_en.bin" : "_ch.bin";

  // 6. 使用 C++17 的 filesystem 庫來建構路徑，取代 sprintf
  std::__fs::filesystem::path wizard_path = "./magic/wizard" + lang_suffix;
  std::__fs::filesystem::path priest_path = "./magic/priest" + lang_suffix;

  // 依序載入 wizard 和 priest 的資料
  if (!ReadNamesFromFile(wizard_path, 150, 0)) {
    return false;
  }
  if (!ReadNamesFromFile(priest_path, 50, 150)) {
    return false;
  }

  return true;
}

int GetItemAttrWeapon(int item)		// ���������� ���߿� ���� ������ �������� Ȯ���ϴ� ��.
{
	int position = GetItemAttr(item, WEAR_ABLE);
	if (position == WEAR_ONLY_INVEN || position == WEAR_ANYTHING || position == WEAR_BELT || item / 1000 == TOOL) return 1;
	return GetItemAttr(item, EQUIPABLE);
}

////////////////////////////////////////////////////////
//
//	������ ���� �Լ�
//
////////////////////////////////////////////////////////
ItemAttr GenerateItem(int item_no)
{
	int type, num;
	GetItemNumber(item_no, type, num);
	ItemAttr item = { 0, };

	CItem* t = ItemUnit(type, num);
	if (t)
	{
		if (!t->GetImageNumber()) return item;
		item.item_no = item_no;


		if (type == DISPOSABLE)
			item.attr[IATTR_DURATION] = t->GetQuantity();
		else if (t->GetRButton() == DIVIDE_ITEM)
		{
			if (item_no == IN_NEW_COINS) {
				item.attr[IATTR_DURATION] = MAX_MONEY;
			}
			else item.attr[IATTR_DURATION] = t->GetItemDuration();
		}
		else item.attr[IATTR_DURATION] = MAKELONG(t->GetItemDuration(), t->GetItemDuration());

		//item.attr[IATTR_LIMIT] = t->GetLimit()*6*3600 + g_curr_time;
	}

	return item;
}


void ItemSoundOfGetItem(int item_no)
{
	int kind = GetItemAttr(item_no, ITEM_KIND);
	switch (kind)
	{
	case IK_NEW_MONEY:
	case IK_MONEY: MP3(SN_GET_MONEY); break;
	default: MP3(SN_GET_ITEM); break;
	}
}

void ItemSoundOfDropItem(int item_no)
{
	int kind = GetItemAttr(item_no, ITEM_KIND);
	switch (kind)
	{
	case IK_NEW_MONEY:
	case IK_MONEY: MP3(SN_PUT_MONEY); break;
	default: MP3(SN_DROP_ITEM); break;
	}
}

void CheckAbility()
{
	::memset(SCharacterData.nAbilityPlusOfWeapon, 0, sizeof(int) * 11);
	for (int i = 0; i < 8; i++)		// ���� �԰� �����ϴ� �����ۿ� ����...
	{
		CallItemVirtualFunction(VF_CURR_EQUIP, EquipItemAttr[i].item_no);
	}
}

/////////////////////////////////////////////////////////////////
// �׷��� �����Ϳ� ����ϱ����� ����ϴ� ������ ���� ���̺�
CItemExplain g_GM_Item;


void CItemExplain::GetItemExplain(int n)
{
    char buf[1024];
    SDL_RWops* fp = nullptr;
    char filename[MAX_FILENAME], filenameBin[MAX_FILENAME];

    int nation;
    if (n) nation = n;
    else
    {
        static int nation_num[] = { 1, 1, 1, 1, 2, 1, 3, 1 };
        // 安全檢查：防止 SCharacterData.nation 越界
        int idx = SCharacterData.nation;
        if (idx < 0 || idx >= sizeof(nation_num)/sizeof(int)) idx = 0;
        nation = nation_num[idx];
    }

    // [Android] 1. 路徑處理：移除 ./
    sprintf(filename, "Itemtable/GMItem%d.txt", nation);
    sprintf(filenameBin, "Itemtable/GMItem%d.bin", nation);

    // ========================================================================
    // 模式 A: 嘗試讀取 TXT (開發/除錯模式)
    // ========================================================================
    fp = SDL_RWFromFile(filename, "rb"); // rb 讀取文字較通用
    if (fp)
    {
        int i = 0;
        int real_count = 0;
        std::string content;

        // 讀取並解析
        if (ReadAllText(fp, content))
        {
            std::stringstream ss(content);
            std::string line;

            while (std::getline(ss, line))
            {
                real_count++;

                // 處理換行符與空行
                if (!line.empty() && line.back() == '\r') line.pop_back();
                if (line.empty()) continue;

                // 註解檢查
                if (line[0] == ';' || line[0] == '#') continue;

                // 將 line 複製到 buf (因為 SetValue 可能會修改字串內容，例如 strtok)
                strncpy(buf, line.c_str(), 1023);
                buf[1023] = '\0';

                int ret = SetValue(i, buf);
                if (ret < 0)
                {
                    if (GetSysInfo(SI_GAME_MAKE_MODE)) {
                        SDL_Log("GetItemExplain Error: File %s Line %d", filename, real_count);
                        // JustMsg(lan->OutputMessage(6, 86), filename, real_count);
                    }
                    continue;
                }
                i++;

                // 安全檢查：防止陣列越界 (假設 m_Item 有最大限制)
                // if (i >= MAX_ITEM_EXPLAIN) break;
            }
        }
        m_nCount = i;
        SDL_RWclose(fp);

        SortingById();

        // [Android] 嘗試寫入 BIN
        // 注意：在真機 Assets 目錄是唯讀的，這裡通常會失敗
        fp = SDL_RWFromFile(filenameBin, "wb");
        if (fp)
        {
            // [關鍵] 強制轉型為 signed char 以配合 Windows 的 CRC 算法
            signed char* tt = (signed char*)m_Item;
            signed char crc = 0;

            size_t totalBytes = sizeof(CItemExplainBasic) * m_nCount;
            for (size_t k = 0; k < totalBytes; k++)
            {
                crc += *tt;
                tt++;
            }

            SDL_RWwrite(fp, &m_nCount, sizeof(int), 1);
            SDL_RWwrite(fp, &crc, 1, 1);
            SDL_RWwrite(fp, m_Item, sizeof(CItemExplainBasic), m_nCount);
            SDL_RWwrite(fp, &crc, 1, 1);
            SDL_RWclose(fp);
            SDL_Log("GetItemExplain: Converted TXT to BIN (%s)", filenameBin);
        }
        else {
            SDL_Log("GetItemExplain: Read TXT but cannot write BIN (Assets are read-only).");
        }
    }
        // ========================================================================
        // 模式 B: 讀取 BIN (發布模式)
        // ========================================================================
    else
    {
        fp = SDL_RWFromFile(filenameBin, "rb");
        if (fp)
        {
            // [關鍵] 這裡也必須使用 signed char，否則 Android 上校驗必失敗
            signed char* tt = (signed char*)m_Item;
            signed char crc = 0, crc1 = 0, crc2 = 0;

            SDL_RWread(fp, &m_nCount, sizeof(int), 1);
            SDL_RWread(fp, &crc1, 1, 1);

            // 安全檢查：m_nCount 是否合理? 防止惡意檔案導致崩潰
            // if (m_nCount > MAX_ITEM_EXPLAIN) { ... handle error ... }

            SDL_RWread(fp, m_Item, sizeof(CItemExplainBasic), m_nCount);
            SDL_RWread(fp, &crc2, 1, 1);
            SDL_RWclose(fp);

            size_t totalBytes = sizeof(CItemExplainBasic) * m_nCount;
            for (size_t k = 0; k < totalBytes; k++)
            {
                crc += *tt;
                tt++;
            }

            if (crc != crc1 || crc != crc2)
            {
                CheckSumError = -1;
                SDL_Log("GetItemExplain CheckSumError: %s (Calc:%d, File:%d/%d)", filenameBin, crc, crc1, crc2);
                // JustMsg(lan->OutputMessage(6, 87));
            }
        }
        else {
            SDL_Log("GetItemExplain: Not found %s or %s", filename, filenameBin);
        }
    }
}


int CItemExplain::SetValue(int para, char* buf)
{
	char* token;

	token = strtok(buf, "\t\n\r");		// �ʿ���� ��ȣ
	if (token == NULL) return 0;

	token = strtok(NULL, "\t\n\r");
	if (token == NULL) return -1;
	m_Item[para].m_nItemId = atoi(token) / 100;

	token = strtok(NULL, "\t\n\r");		// ������ �ȵǴ� �׸�
	if (token == NULL) return -1;
	strcpy(m_Item[para].m_szHanName, token);

	token = strtok(NULL, "\t\n");
	if (token == NULL) return 0;		// �������� �ִ� �׸�
#ifdef _DEBUG // finito 060507
	if (GetSysInfo(SI_GAME_MAKE_MODE))
	{
		int len = strlen(token);
		if (len >= MAX_ITEM_EXPLAIN_MSG)
			JustMsg(lan->OutputMessage(6, 88));//010215 lsw
	}
#endif
	strcpy(m_Item[para].explain, token);

	return 1;
}
void CItemExplain::SortingById()
{
	for (int i = 0; i < m_nCount - 1; i++)
	{
		for (int j = i; j < m_nCount; j++)
		{
			if (m_Item[i].m_nItemId == 0 || m_Item[i].m_nItemId > m_Item[j].m_nItemId)
			{
				CItemExplainBasic temp = m_Item[i];
				m_Item[i] = m_Item[j];
				m_Item[j] = temp;
			}
		}
	}
}

int CItemExplain::SearchById(int id)		// 2�� Ž��
{
	int low = 0, high = m_nCount - 1;
	int mid = 0;
	while (low <= high)
	{
		mid = (low + high) / 2;
		if (m_Item[mid].m_nItemId == id) return mid;
		if (m_Item[mid].m_nItemId < id) low = mid + 1;
		else high = mid - 1;
	}

	return -1;
}

char* CItemExplain::GetName(int para)
{
	if (para < 0 || para >= MAX_ITEM_EXPLAIN_COUNT) return NULL;
	return m_Item[para].m_szHanName;
}

char* CItemExplain::GetExplain(int para)
{
	if (para < 0 || para >= MAX_ITEM_EXPLAIN_COUNT) return NULL;
	return m_Item[para].explain;
}

// Ư�� �������� ������ŭ �ִ��� üũ
// �� ������ ���̸� �����ش�.
int CheckInventory(const int iItemNo, const int iNeedCount)//021111 lsw
{
	int ct = 0;
	CItem* t = ItemUnit(iItemNo);
	if (!t) return -1;
	for (int a = 0; a < 3; a++)
		for (int b = 0; b < 3; b++)
			for (int c = 0; c < 8; c++)
			{
				if (InvItemAttr[a][b][c].item_no == iItemNo)
				{
					if (t->GetRButton() == DIVIDE_ITEM) {
						ct += InvItemAttr[a][b][c].attr[IATTR_MUCH];
					}
					else {
						ct++;
					}
				}
			}

	return ct - iNeedCount;
}


///////////////////////////////////////////////////////////////////
//
void CItem_Weapon::GetItemNeedAbility(int& count, int* type, int* value)
{
	int need[] = { Need3_str, Need3_dex, Need3_con, Need3_wis, Need3_int, Need3_cha, Need3_end, Need3_Movp, Need3_mor, Need3_luc, Need3_ws, };
	if (SCharacterData.nCharacterData[SPELL] == PRIEST_SPELL) need[10] = Need3_ps;

	int t[] = { STR, DEX, CON, WIS, INT_, CHA, ENDU, MOVP, MOR, LUCK, WSPS };
	int max = sizeof(need) / sizeof(int);


	count = 0;
	for (int i = 0; i < max; i++)
	{
		if (need[i])
		{
			type[count] = t[i];
			value[count++] = need[i];
		}
	}
}
void CItem_Armor::GetItemNeedAbility(int& count, int* type, int* value)
{
	int need[] = { Need3_str, Need3_dex, Need3_con, Need3_wis, Need3_int, Need3_cha, Need3_end, Need3_Movp, Need3_mor, Need3_luc, Need3_ws, };
	if (SCharacterData.nCharacterData[SPELL] == PRIEST_SPELL) need[10] = Need3_ps;
	int t[] = { STR, DEX, CON, WIS, INT_, CHA, ENDU, MOVP, MOR, LUCK, WSPS };
	int max = sizeof(need) / sizeof(int);

	count = 0;
	for (int i = 0; i < max; i++)
	{
		if (need[i])
		{
			type[count] = t[i];
			value[count++] = need[i];
		}
	}
}
void CItem_Disposable::GetItemNeedAbility(int& count, int* type, int* value)
{
	int need[] = { Need3_str, Need3_dex, Need3_con, Need3_wis, Need3_int, Need3_cha, Need3_end, Need3_Movp, Need3_mor, Need3_luc, Need3_ws, };
	if (SCharacterData.nCharacterData[SPELL] == PRIEST_SPELL) need[10] = Need3_ps;
	int t[] = { STR, DEX, CON, WIS, INT_, CHA, ENDU, MOVP, MOR, LUCK, WSPS };
	int max = sizeof(need) / sizeof(int);

	count = 0;
	for (int i = 0; i < max; i++)
	{
		if (need[i])
		{
			type[count] = t[i];
			value[count++] = need[i];
		}
	}
}
void CItem_Accessory::GetItemNeedAbility(int& count, int* type, int* value)
{
	int need[] = { Need3_str, Need3_dex, Need3_con, Need3_wis, Need3_int, Need3_cha, Need3_end, Need3_Movp, Need3_mor, Need3_luc, Need3_ws, };
	if (SCharacterData.nCharacterData[SPELL] == PRIEST_SPELL) need[10] = Need3_ps;
	int t[] = { STR, DEX, CON, WIS, INT_, CHA, ENDU, MOVP, MOR, LUCK, WSPS };
	int max = sizeof(need) / sizeof(int);

	count = 0;
	for (int i = 0; i < max; i++)
	{
		if (need[i])
		{
			type[count] = t[i];
			value[count++] = need[i];
		}
	}
}
void CItem_Etc::GetItemNeedAbility(int& count, int* type, int* value)
{
	int need[] = { Need3_str, Need3_dex, Need3_con, Need3_wis, Need3_int, Need3_cha, Need3_end, Need3_Movp, Need3_mor, Need3_luc, Need3_ws, };
	if (SCharacterData.nCharacterData[SPELL] == PRIEST_SPELL) need[10] = Need3_ps;
	int t[] = { STR, DEX, CON, WIS, INT_, CHA, ENDU, MOVP, MOR, LUCK, WSPS };
	int max = sizeof(need) / sizeof(int);

	count = 0;
	for (int i = 0; i < max; i++)
	{
		if (need[i])
		{
			type[count] = t[i];
			value[count++] = need[i];
		}
	}
}


void CItem_Herb::GetItemChangeAbility(int& count, int* type, int* value)
{
	int need[] = { Change_str, Change_dex, Change_con, Change_wis, Change_int, Change_cha, Change_end, Change_Movp, Change_mor, Change_luc, };
	static int max = sizeof(need) / sizeof(int);

	count = 0;
	for (int i = 0; i < max; i++)
	{
		if (need[i])
		{
			type[count] = i;
			value[count++] = need[i];
		}
	}
}
void CItem_Cook::GetItemChangeAbility(int& count, int* type, int* value)
{
	int need[] = { Change_str, Change_dex, Change_con, Change_wis, Change_int, Change_cha, Change_end, Change_Movp, Change_mor, Change_luc, };
	static int max = sizeof(need) / sizeof(int);

	count = 0;
	for (int i = 0; i < max; i++)
	{
		if (need[i])
		{
			type[count] = i;
			value[count++] = need[i];
		}
	}
}
void CItem_Potion::GetItemChangeAbility(int& count, int* type, int* value)
{
	int need[] = { Change_str, Change_dex, Change_con, Change_wis, Change_int, Change_cha, Change_end, Change_movp, Change_mor, Change_luc };
	static int max = sizeof(need) / sizeof(int);

	count = 0;
	for (int i = 0; i < max; i++)
	{
		if (need[i])
		{
			type[count] = i;
			value[count++] = need[i];
		}
	}
}
void CItem_Tool::GetItemChangeAbility(int& count, int* type, int* value)
{
	int need[] = { Change_str, Change_dex, Change_con, Change_wis, Change_int, Change_cha, Change_end, Change_movp, Change_mor, Change_luc, };
	static int max = sizeof(need) / sizeof(int);

	count = 0;
	for (int i = 0; i < max; i++)
	{
		if (need[i])
		{
			type[count] = i;
			value[count++] = need[i];
		}
	}
}
void CItem_Weapon::GetItemChangeAbility(int& count, int* type, int* value)
{
	int need[] = { Change_str, Change_dex, Change_con, Change_wis, Change_int, Change_cha, Change_end, Change_Movp, Change_mor, Change_luc,
		Change_ws, Change_ps, Change_mag, Change_dev, casting_Time, Change_Life_max, Change_mana_max, Change_Divine_Max, Change_health_max, };
	static int max = sizeof(need) / sizeof(int);

	count = 0;
	for (int i = 0; i < max; i++)
	{
		if (need[i])
		{
			type[count] = i;
			value[count++] = need[i];
		}
	}
}
void CItem_Armor::GetItemChangeAbility(int& count, int* type, int* value)
{
	int need[] = { Change_str, Change_dex, Change_con, Change_wis, Change_int, Change_cha, Change_end, Change_Movp, Change_mor, Change_luc,
		Change_ws, Change_ps, Change_mag, Change_dev, casting_Time, Change_Life_max, Change_mana_max, Change_Divine_Max, Change_health_max, };
	static int max = sizeof(need) / sizeof(int);

	count = 0;
	for (int i = 0; i < max; i++)
	{
		if (need[i])
		{
			type[count] = i;
			value[count++] = need[i];
		}
	}
}
void CItem_Etc::GetItemChangeAbility(int& count, int* type, int* value)
{
	int need[] = { Change_str, Change_dex, Change_con, Change_wis, Change_int, Change_cha, Change_end, Change_mor, Change_luc,
		Change_ws, Change_ps, Change_mag, Change_dev, casting_Time, Change_Life_max, Change_mana_max, Change_Divine_Max, Change_health_max, };
	static int max = sizeof(need) / sizeof(int);

	int t[] = { 0, 1, 2, 3, 4, 5, 6, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18 };
	count = 0;
	for (int i = 0; i < max; i++)
	{
		if (need[i])
		{
			type[count] = t[i];
			value[count++] = need[i];
		}
	}
}
void CItem_Accessory::GetItemChangeAbility(int& count, int* type, int* value)
{
	int need[] = { Change_str, Change_dex, Change_con, Change_wis, Change_int, Change_cha, Change_end, Change_Movp, Change_mor, Change_luc,
		Change_ws, Change_ps, Change_mag, Change_dev, casting_Time, Change_Life_max, Change_mana_max, Change_Divine_Max, Change_health_max, };
	static int max = sizeof(need) / sizeof(int);



	count = 0;
	for (int i = 0; i < max; i++)
	{
		if (need[i])
		{
			type[count] = i;
			value[count++] = need[i];
		}
	}
}

int GetAntiMagic_1(int* anti, int* ability)
{
	int count = 0;
	for (int i = 0; i < 6; i++)
	{
		if (*(ability + i))
		{
			anti[count++] = *(ability + i);
		}
	}
	return count;
}

bool EqualItem(const ItemAttr* source, const ItemAttr* target)
{
	/*
	if( source->item_no != target->item_no ) return 0;
	for( int i=0; i<6; i++ )
	{
		if( source->attr[i] != target->attr[i] ) return 0 ;
	}
*/
	if (memcmp(source, target, sizeof(ItemAttr)) == 0) return 1;
	return 0;
}
