#include "StdAfx.h"
#include <SDL.h>
#include "MobileBridge.h"
// 這裡必須包含你能接觸到主角 (Hero) 的標頭檔
#include "dragon.h" // 假設這是你的全域變數定義處
#include "map.h"
#include "gameproc.h"
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

LPCHARACTER FindCharacterNearby()
{
    if (!Hero) return nullptr;

    LPCHARACTER ch = Hero->lpNext;

    // 用來儲存目前找到的「最佳人選」
    LPCHARACTER closestTarget = nullptr;

    // 設定搜尋上限：5 * TILE_SIZE
    // 技巧：為了效能，我們用「距離的平方」來比較，這樣就不需要用 sqrt 開根號
    long maxDist = 10 * TILE_SIZE;
    long minDistanceSq = maxDist * maxDist; // 這裡存的是 (5*TILE_SIZE)^2

    while (ch != NULL)
    {
        // -------------------------------------------------
        // 在這裡加入過濾條件，例如：不能是死人、必須是 NPC 等
        if(!ch->IsNpc()) { ch = ch->lpNext; continue; }
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

    const int margin = (y > 0.0f) ? 100:200;
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

    YouCanHeroActionProc = 1; YouCanViewTipsCheck = 1; g_nLButtonState = STATE_BUTTON_PRESSED;	 LButtonDownIng = 1;
    real_move=1;
    // 4. 執行移動
    DoHeroMove(1, targetX, targetY);
    //DoLButtonDown();
    real_move=0;
    g_nLButtonState = STATE_BUTTON_RELEASED; LButtonDownIng = 0; YouCanHeroActionProc = 0;
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

    LPCHARACTER target = FindCharacterNearby();
    if(!target) return;

    int targetX = target->x;
    int targetY = target->y;
    Mox = targetX;
    Moy = targetY;
    g_pointMouseX = targetX - Mapx;
    g_pointMouseY = targetY - Mapy-8;
    // 3. 更新其他依賴邏輯座標的變數
    g_pointMouse.x = g_pointMouseX;
    g_pointMouse.y = g_pointMouseY;

    YouCanHeroActionProc = 1; YouCanViewTipsCheck = 1; g_nLButtonState = STATE_BUTTON_PRESSED;	 LButtonDownIng = 1;

    // 4. 執行移動
    //DoHeroMove(1, targetX, targetY);
    DoLButtonDown();
    g_nLButtonState = STATE_BUTTON_RELEASED; LButtonDownIng = 0; YouCanHeroActionProc = 0;
}

void Mobile_DoRightButton(int skillSlot) {
    if (!Hero) return;

    g_nRButtonState = STATE_BUTTON_PRESSED;	 RButtonDownIng = 1;
    // 假設 skillSlot = 1 代表 F1
    // 你可以去查原本遊戲按下 F1 時呼叫了什麼
    // 通常是 SelectSkill 或者是 CastSkill
    DoRButtonDown();

    g_nRButtonState = STATE_BUTTON_RELEASED; RButtonDownIng = 0;
    // Hero->UseSkill(skillSlot); 
    // 或者
    // Network::SendSkillPacket(skillSlot);
}

void Mobile_UsePotion(int quickSlot) {
    // 喝水通常是 ItemManager 管的
    // ItemManager::Instance().UseQuickSlot(quickSlot);

    POS pos_s;
    SetItemPos(QUICK, quickSlot, &pos_s);
    ItemAttr& potion = QuickItemAttr[quickSlot];
    UseItemByRbutton(pos_s, potion);
}
