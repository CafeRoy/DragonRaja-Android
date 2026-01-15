#include "StdAfx.h"
#include <SDL.h>
#include "MobileBridge.h"
// 這裡必須包含你能接觸到主角 (Hero) 的標頭檔
#include "dragon.h" // 假設這是你的全域變數定義處
#include "map.h"
#include "gameproc.h"
#include "Menu.h"
#include "SmallMenuSet.h"
// 引用外部的全域變數 (Legacy Game 通常都有這個)
extern LPCHARACTER Hero; // 你的主角指標
extern void SendAttackPacket(); // 或者遊戲原本的攻擊函式
// 增加一個全域或靜態變數來標記「正在使用搖桿」
bool g_IsJoystickMoving = false;

// =========================================================
// 1. 處理移動 (這通常最難，因為要模擬持續按著)
// =========================================================
// 我們不要用 PushEvent，直接改寫遊戲的「輸入狀態變數」

// 假設遊戲原本是用 g_bKeyTable[] 或類似的變數來判斷是否按著方向鍵
// 如果找不到，我們可以用一個比較暴力的方法：直接改 Hero 的目標
extern bool keysJustPressed[SDL_NUM_SCANCODES]; // 假設遊戲全域按鍵表

LPCHARACTER FindCharacterNearby(bool isNpc)
{
    if (!Hero) return nullptr;

    LPCHARACTER ch = Hero->lpNext;

    // 用來儲存目前找到的「最佳人選」
    LPCHARACTER closestTarget = nullptr;

    // 設定搜尋上限：5 * TILE_SIZE
    // 技巧：為了效能，我們用「距離的平方」來比較，這樣就不需要用 sqrt 開根號
    long maxDist = 25 * TILE_SIZE;
    long minDistanceSq = maxDist * maxDist; // 這裡存的是 (5*TILE_SIZE)^2

    while (ch != NULL)
    {
        // -------------------------------------------------
        // 在這裡加入過濾條件，例如：不能是死人、必須是 NPC 等
        if(isNpc && !ch->IsNpc()) { ch = ch->lpNext; continue; }
        if (ch->hp<=0) { ch = ch->lpNext; continue; }
        if (ch == Hero)   { ch = ch->lpNext; continue; }
        // -------------------------------------------------

        // 1. 計算 X 與 Y 的差值
        long dx = ch->x - Hero->x;
        long dy = ch->y - Hero->y;

        // 2. 計算距離的平方 (a^2 + b^2 = c^2)
        // 使用 long long 防止溢出 (如果地圖座標很大的話)
        long long currentDistSq = (long long)dx * dx + (long long)dy * dy;

        // 3. 核心邏輯：
        // 如果這個目標的距離 < 目前紀錄的最小距離 (初始值是 5格的平方)
        if (currentDistSq < minDistanceSq)
        {
            // 更新最小距離標準 (這樣下一個必須比這個更近才會被選中)
            minDistanceSq = currentDistSq;

            // 記錄這個人是目前最近的
            closestTarget = ch;
        }

        ch = ch->lpNext;
    }

    // 迴圈結束後，這裡回傳的一定是「範圍內」且「最近」的那一個
    // 如果都沒有人小於 5 格，這裡會回傳 NULL
    return closestTarget;
}
// 搜尋範圍內最近的物品，並設定給 CursorCheckedItem
bool Mobile_SearchClosestItem()
{
    LPITEMGROUND t = g_Item;
    LPITEMGROUND bestItem = NULL;

    // 8 格距離 (1格=32像素) -> 256像素
    // 使用平方距離比較以避免開根號，效能較好
    // 256 * 256 = 65536
    const long SEARCH_RANGE_SQ = 65536;

    long minDistanceSq = 99999999; // 先設一個很大的初始距離

    // 確保主角存在
    if (Hero == NULL) return false;

    while( t != NULL )
    {
        // 1. 基本過濾：如果是門、無效的Sprite、或者已開啟的箱子，直接跳過
        if (t->type == ITEMTYPE_DOOR) { t = t->next; continue; }

        // 2. 陷阱過濾 (自動拾取通常不撿陷阱，除非你有拆除技能)
        // 參考原本代碼的 attr 檢查
        if (t->attr[IATTR_ATTR] & (IA2_TRAPED0 | IA2_TRAPED1)) {
            // 如果你希望有拆陷阱技能時也自動選取，這裡要補上原本的 SkillNo 判斷
            // 為了安全，預設先跳過陷阱
            t = t->next;
            continue;
        }

        // 3. 已開啟的箱子跳過
        if ((t->attr[IATTR_ATTR] & IA2_BOX) && (t->attr[IATTR_ATTR] & IA2_OPENED)) {
            t = t->next;
            continue;
        }

        // 4. 物品權限過濾 (不是你的或隊伍的，且還沒過保護期)
        // t->host 檢查，如果需要可以加上

        // 5. 計算距離 (使用邏輯座標 t->x, t->y)
        long distSq = (Hero->x - t->x)*(Hero->x - t->x) + (Hero->y - t->y)*(Hero->y - t->y);

        // 6. 判斷是否在範圍內，且比目前找到的更近
        if (distSq < SEARCH_RANGE_SQ && distSq < minDistanceSq)
        {
            minDistanceSq = distSq;
            bestItem = t;
        }

        t = t->next;
    }

    // 找到了最近的物品
    if (bestItem != NULL)
    {
        // 這行就是你需要的：將目標設為這個物品
        CursorCheckedItem = bestItem;
        return true; // 回傳 true 代表有找到東西
    }

    // 沒找到任何東西
    CursorCheckedItem = NULL;
    return false;
}

void Mobile_ApplyDazeEffect(int* inputX, int* inputY)
{
    // 1. 安全檢查
    if (!Hero) return;

    // 2. 檢查是否處於「混亂 (Daze)」狀態
    // 如果沒有混亂，直接返回，不做任何修改，保留原本的 x, y
    if (GetIndexOfCondition(SCharacterData.condition) != CC_DAZE)
    {
        return;
    }

    // ========================================================
    // 以下邏輯完全複製自原版 MouseClickPosition
    // 只是把 Mox 換成了 *inputX，把 Moy 換成了 *inputY
    // ========================================================

    int distance_X = 0;
    int temp_Dist_X = 0;
    int distance_Y = 0;
    int temp_Dist_Y = 0;

    // 計算目標點與主角的距離
    temp_Dist_X = abs(Hero->x - *inputX);
    temp_Dist_Y = abs(Hero->y - *inputY);

    if (temp_Dist_X < 32) temp_Dist_X = 32;
    if (temp_Dist_Y < 32) temp_Dist_Y = 32;

    // 計算隨機偏移量
    distance_X = (temp_Dist_X * 2) + ((rand() % temp_Dist_X / 2) - (temp_Dist_X / 4));
    distance_Y = (temp_Dist_Y * 2) + ((rand() % temp_Dist_Y / 2) - (temp_Dist_Y / 4));

    // 隨機決定偏移方向
    switch (rand() % 4)
    {
        case 0: // x 軸偏移
            if (Hero->x > *inputX)
                *inputX += distance_X;
            else
                *inputX -= distance_X;
            break;

        case 1: // y 軸偏移
            if (Hero->y > *inputY)
                *inputY += distance_Y;
            else
                *inputY -= distance_Y;
            break;

        case 2: // x,y 雙軸偏移
            if (Hero->x > *inputX)
                *inputX += distance_X;
            else
                *inputX -= distance_X;

            if (Hero->y > *inputY)
                *inputY += distance_Y;
            else
                *inputY -= distance_Y;
            break;

        case 3: // 完全隨機跳動
            *inputX = rand() % distance_X - distance_X / 2;
            *inputY = rand() % distance_Y - distance_Y / 2;
            break;
    }
}

void Mobile_OnJoystickMove(float x, float y) {
    if (!Hero) return;
    g_IsJoystickMoving = true;

    const Uint32 MOVE_DELAY = 350;

    static Uint32 dwLastMoveTime = 0;
    Uint32 dwCurrentTime = SDL_GetTicks();

    // 如果距離上次移動還不到指定時間，就直接跳出，不執行移動
    if (dwCurrentTime - dwLastMoveTime < MOVE_DELAY) {
        return;
    }

    // 更新最後移動時間
    dwLastMoveTime = dwCurrentTime;

    const int margin = (y > 0.0f) ? 150:200;
    // 1. 計算目標世界座標
    int targetX = Hero->x + (int)(x * margin);
    int targetY = Hero->y + (int)(y * margin);

    // 2. 應用混亂效果
    Mobile_ApplyDazeEffect(&targetX, &targetY);

    // ============================================================
    // ★★★ 關鍵修正：同步全域滑鼠變數 (騙過 DoHeroMove) ★★★
    // ============================================================
    // 強制把 Mox, Moy 設定為我們計算出來的目標點
    // 這樣遊戲引擎會以為「滑鼠現在就在搖桿指的地方」

    /*Mox = targetX;
    Moy = targetY;
    g_pointMouseX = targetX - Mapx;
    g_pointMouseY = targetY - Mapy;
    // 3. 更新其他依賴邏輯座標的變數
    g_pointMouse.x = g_pointMouseX;
    g_pointMouse.y = g_pointMouseY;*/

    // 如果遊戲還有用到「螢幕座標變數」 (g_pointMouseX)，也要同步
    // 公式：螢幕座標 = 世界座標 - 地圖偏移量
    //g_pointMouseX = targetX - Mapx;
    //g_pointMouseY = targetY - Mapy;
    // ============================================================

    // 3. 更新按鍵狀態 (模擬鍵盤，給動畫系統用)
    keysJustPressed[SDL_SCANCODE_UP] = false;
    keysJustPressed[SDL_SCANCODE_DOWN] = false;
    keysJustPressed[SDL_SCANCODE_LEFT] = false;
    keysJustPressed[SDL_SCANCODE_RIGHT] = false;

    if (fabs(y) > fabs(x)) {
        if (y < -0.3f) keysJustPressed[SDL_SCANCODE_UP] = true;
        if (y > 0.3f)  keysJustPressed[SDL_SCANCODE_DOWN] = true;
    } else {
        if (x < -0.3f) keysJustPressed[SDL_SCANCODE_LEFT] = true;
        if (x > 0.3f)  keysJustPressed[SDL_SCANCODE_RIGHT] = true;
    }

    real_move= YouCanHeroActionProc = 1;
    //YouCanViewTipsCheck = 1; g_nLButtonState = STATE_BUTTON_PRESSED;	 LButtonDownIng = 1;
    // 4. 執行移動
    DoHeroMove(1, targetX, targetY);
    //DoLButtonDown();
    //g_nLButtonState = STATE_BUTTON_RELEASED; LButtonDownIng = 0;
    real_move = YouCanHeroActionProc = 0;
    // 5. 鏡頭跟隨
    g_Map.tox = (Hero->visual_x / 32) - (GAME_SCREEN_XSIZE / 32 / 2);
    g_Map.toy = (Hero->visual_y / 32) - (GAME_SCREEN_YSIZE / 32 / 2);
    ;
    if (g_Map.tox < 0) g_Map.tox = 0;
    if (g_Map.toy < 0) g_Map.toy = 0;
}

void Mobile_OnJoystickStop() {

    g_IsJoystickMoving = false;
    // 釋放所有方向鍵
    keysJustPressed[SDL_SCANCODE_UP] = false;
    keysJustPressed[SDL_SCANCODE_DOWN] = false;
    keysJustPressed[SDL_SCANCODE_LEFT] = false;
    keysJustPressed[SDL_SCANCODE_RIGHT] = false;
    g_nLButtonState = STATE_BUTTON_RELEASED; LButtonDownIng = 0; YouCanHeroActionProc = 0;
}

// =========================================================
// 2. 處理動作 (直接呼叫函式)
// =========================================================
void Mobile_DoLeftButton() {
    if (!Hero) return;

    bool find_npc = true;
    LPCHARACTER target = FindCharacterNearby(find_npc);
    if(!target) return;

    int targetX = target->x;
    int targetY = target->y;
    Mox = targetX;
    Moy = targetY;
    g_pointMouseX = targetX - Mapx;
    g_pointMouseY = targetY - Mapy-10;
    // 3. 更新其他依賴邏輯座標的變數
    g_pointMouse.x = g_pointMouseX;
    g_pointMouse.y = g_pointMouseY;

    YouCanHeroActionProc = 1; YouCanViewTipsCheck = 1; g_nLButtonState = STATE_BUTTON_PRESSED;	 LButtonDownIng = 1;
    DoLButtonDown();
    g_nLButtonState = STATE_BUTTON_RELEASED; LButtonDownIng = 0; YouCanHeroActionProc = 0;
}

void Mobile_DoRightButton(int skillSlot) {
    if (!Hero) return;

    if(skillSlot == 1){//對自己施放

        g_nRButtonState = STATE_BUTTON_PRESSED;	 RButtonDownIng = 1;
        LPCHARACTER  old_target = g_GameInfo.lpcharacter;
        int OldMouseMenu = g_MouseInMenu;
        int SelectedType = g_GameInfo.nSelectedSpriteType;
        g_MouseInMenu = 0;
        g_GameInfo.lpcharacter = Hero;g_GameInfo.nSelectedSpriteType = SPRITETYPE_CHARACTER;
        DoRButtonDown();
        g_GameInfo.lpcharacter = old_target;g_GameInfo.nSelectedSpriteType = SelectedType;
        g_MouseInMenu = OldMouseMenu;
        g_nRButtonState = STATE_BUTTON_RELEASED; RButtonDownIng = 0;
        return;
    }


    bool find_npc = true;
    LPCHARACTER target = FindCharacterNearby(find_npc);
    //if(!target) return;

    int magic_no = Hero->excute_MagicNum.GetDecrypted();

    if (target && Hero->ready_Magic == 2 && magic_no < 200) {
        if (IsHarmfulMagic(magic_no)) {

            // 1. 先取得座標並計算距離
            float heroX = (float)Hero->x;
            float heroY = (float)Hero->y;
            float targetX = (float)target->x;
            float targetY = (float)target->y;

            float dx = targetX - heroX;
            float dy = targetY - heroY;
            float dist = sqrt(dx * dx + dy * dy); // 算出真實距離

            // 2. ★★★ 這裡是你要求的新邏輯 ★★★
            // 如果距離小於等於 150，鎖定目標；否則設為 nullptr (只朝方向施法但不鎖定)
            if (dist <= 150.0f) {
                g_GameInfo.nSelectedSpriteType = SPRITETYPE_MONSTER;
                g_GameInfo.lpcharacter = target;
            } else {
                g_GameInfo.nSelectedSpriteType = SPRITETYPE_NONE;
                g_GameInfo.lpcharacter = nullptr;
            }

            // 3. 設定滑鼠座標 (向量歸一化邏輯)
            // 這裡我們設定滑鼠永遠點在距離自己 150 的方向上 (不管是否鎖定目標)
            float castDistance = 150.0f;

            if (dist > 0.001f) {
                // 算出從自己往怪物的方向，延伸 castDistance 的位置
                float offsetX = (dx / dist) * castDistance;
                float offsetY = (dy / dist) * castDistance;

                // Hero 在螢幕上的位置
                float heroScreenX = heroX - Mapx;
                float heroScreenY = heroY - Mapy;

                // 設定滑鼠
                g_pointMouseX = (int)(heroScreenX + offsetX);
                g_pointMouseY = (int)(heroScreenY + offsetY);
            } else {
                // 重疊時的備案
                g_pointMouseX = (int)(heroX - Mapx + 50);
                g_pointMouseY = (int)(heroY - Mapy+20);
            }

            // 4. 更新全域變數
            // Mox/Moy 通常是紀錄點擊的"世界座標"，還是設為目標位置比較保險，或者設為計算出的虛擬點也可以
            // 這裡維持設為目標原始位置，比較符合直覺
            Mox = g_pointMouseX + Mapx;
            Moy = g_pointMouseY + Mapy;
            g_pointMouse.x = g_pointMouseX;
            g_pointMouse.y = g_pointMouseY;
        }
    }

    //這是其他邏輯
    if(g_MouseInMenuThisFrame){
        switch (g_MouseInMenuThisFrame)
        {
            case MN_ITEM:
            {
                DoRButtonCheckOfMenu(MN_ITEM, 3);
            }break;
            case MN_MAININTERFACE:
            {
                DoRButtonCheckOfMenu(MN_MAININTERFACE, 11);
            }break;
        }
        return;
    }

    //執行右鍵
    g_nRButtonState = STATE_BUTTON_PRESSED;	 RButtonDownIng = 1;
    DoRButtonDown();
    g_nRButtonState = STATE_BUTTON_RELEASED; RButtonDownIng = 0;
}

void Mobile_UsePotion(int quickSlot) {
    // 喝水通常是 ItemManager 管的
    // ItemManager::Instance().UseQuickSlot(quickSlot);

    POS pos_s;
    SetItemPos(QUICK, quickSlot, &pos_s);
    ItemAttr& potion = QuickItemAttr[quickSlot];
    UseItemByRbutton(pos_s, potion);
}

void Mobile_DoQuickmemory(SDL_Scancode key)
{
    DoQuickmemory(key - SDL_SCANCODE_F1);
    /*if( isKeyDown(SDL_SCANCODE_LCTRL) || isKeyDown(SDL_SCANCODE_RCTRL))
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
    }*/
    return;
}

void Mobile_SetQuickMemoryByKeyInput(bool CtrlFlag, SDL_Scancode Key)
{
    if(!CtrlFlag){

        Mobile_DoQuickmemory(Key);
        return;
    }

    keysJustPressed[SDL_SCANCODE_LCTRL] = true;
    keysJustPressed[Key] = true;

    SMENU* subMenu = &SMenu[MN_QUICK_MAGIC_PLUS];

    for (int j = 0; j < subMenu->nFieldCount; j++){
        lsw_MenuSubProcessType(subMenu, j);
    }

    subMenu = &SMenu[MN_COMBAT_SKILL_DOING];
    for (int j = 0; j < subMenu->nFieldCount; j++){
        lsw_MenuSubProcessType(subMenu, j);
    }

    keysJustPressed[Key] = false;
    keysJustPressed[SDL_SCANCODE_LCTRL] = false;
    /*if(!isKeyPressed(SDL_SCANCODE_LCTRL))
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
    }*/
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

void Mobile_SendPickItem()
{
    if(Hero && Hero->hp<=0) return;

    if(Mobile_SearchClosestItem())
        CheckEventWith_Item();
}
