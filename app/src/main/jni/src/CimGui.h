#ifndef CIMGUI_H
#define CIMGUI_H

// ==========================================================
// ★★★ 宏魔法必須加在這裡 (Header File) ★★★
// ==========================================================
// 1. 先把遊戲定義的 MemAlloc / MemFree 暫存起來
#pragma push_macro("MemAlloc")
#pragma push_macro("MemFree")

// 2. 暫時移除定義，讓 ImGui 可以使用它原本的函式名稱
#undef MemAlloc
#undef MemFree

// 3. 這裡才載入 ImGui
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

// 4. 恢復遊戲的宏定義 (讓後面的遊戲代碼恢復正常)
#pragma pop_macro("MemFree")
#pragma pop_macro("MemAlloc")
// ==========================================================

#include <SDL.h>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>


// 前向宣告，避免在這裡 include 太多遊戲邏輯標頭檔
class CCharacter;
typedef CCharacter* LPCHARACTER;

// 定義 GM 操作的枚舉，取代原本的 const int
enum GmActionType {
    GM_ACTION_GIVE_LIFE = 0,
    GM_ACTION_MAKE_GHOST,
    GM_ACTION_BBS_ALL,
    GM_ACTION_EXP_EVENT,
    GM_ACTION_CREATE_NPC,
    GM_ACTION_SELECT_OX,
    GM_ACTION_BLOCK_ATTACK,
    GM_ACTION_BLOCK_MAGIC,
    GM_ACTION_BLOCK_MOVE,
    GM_ACTION_BLOCK_SKILL,
    GM_ACTION_BLOCK_TEAM,
    GM_ACTION_BLOCK_ALL,
    GM_ACTION_RELOAD_EVENTNPC,
    GM_ACTION_GEN_ITEM
};

struct MapMoveContext {
    int x, y;
    int map_index;
    std::string map_name;
};


// =========================================================
// 1. 定義 UI 佈局參數 (放在 .h 類別內 或 .cpp 最上方)
// =========================================================
struct MobileUILayout {
    static constexpr float LOGIC_W = 1280.0f;
    static constexpr float LOGIC_H = 720.0f;
    static constexpr float SCALE   = 1.2f;

    // 左下角搖桿 (保持你剛才改好的設定)
    static constexpr float JOY_SIZE     = 160.0f * SCALE; // 縮小的搖桿
    static constexpr float JOY_MARGIN_X = 30.0f;
    static constexpr float JOY_MARGIN_Y = 50.0f;          // 上移的搖桿

    // --- 右下角按鈕區 ---
    static constexpr float BTN_BIG_SIZE   = 80.0f * SCALE;
    static constexpr float BTN_SMALL_SIZE = 50.0f * SCALE;

    // ★ 新增：超小按鈕 (比 MP 再小一點，設為 50 或 45) ★
    static constexpr float BTN_TINY_SIZE  = 40.0f * SCALE;

    static constexpr float SPACING        = 10.0f * SCALE;

    static constexpr float BTN_AREA_W = (BTN_BIG_SIZE * 2) + SPACING;

    // ★ 修改：總高度變高了 (大 + 小 + 超小 + 兩個間隙) ★
    static constexpr float BTN_AREA_H = BTN_BIG_SIZE + BTN_SMALL_SIZE + BTN_TINY_SIZE + (SPACING * 2);

    static constexpr float BTN_MARGIN_X = 20.0f;
    static constexpr float BTN_MARGIN_Y = 20.0f;

    // Helper 函式不用動
    static ImVec4 GetJoyRect() { return ImVec4(JOY_MARGIN_X, LOGIC_H - JOY_SIZE - JOY_MARGIN_Y, JOY_SIZE, JOY_SIZE); }
    static ImVec4 GetBtnRect() { return ImVec4(LOGIC_W - BTN_AREA_W - BTN_MARGIN_X, LOGIC_H - BTN_AREA_H - BTN_MARGIN_Y, BTN_AREA_W, BTN_AREA_H); }
};
class CimGui
{
public:
    // 單例模式：取得唯一實例
    static CimGui& Instance();

    // 禁止複製
    CimGui(const CimGui&) = delete;
    CimGui& operator=(const CimGui&) = delete;

public:
    // 初始化與幀循環
    bool Init(SDL_Window* window, SDL_Renderer* renderer);
    void Shutdown(); // 新增：清理資源
    void NewFrame();
    void Render();   // 改名為 Render，語意更精確
    bool ProcessEvent(SDL_Event* event); // 改名為 ProcessEvent

    // 遊戲狀態管理
    void SetInGame(bool state, LPCHARACTER heroPtr = nullptr);

    // UI 繪製入口
    void Draw();

private:
    // 私有建構子
    CimGui();
    ~CimGui();

#ifdef _DEBUG
    // --- GM 面板相關邏輯 (封裝在內部) ---
    void DrawGmPanel();
    void DrawGmActionsHeader();    // GM 指令區塊
    void DrawMapMoveHeader();      // 地圖傳送區塊
    void DrawCharacterManager();   // 角色管理區塊 (雙列表)
#endif
    // --- Release 設定頁面 (未來擴充用) ---
    void DrawSettingsPanel();
    // --- 輔助函式 ---
    int  ShowMessageBoxYesNo(const char* title, const char* message);
    LPCHARACTER FindCharacter(const std::string& name);

    // --- 封包發送函式 (私有化，只供 UI 內部呼叫) ---
    void SendPacket_DoAction(const std::vector<std::string>& charNames, int action);
    void SendPacket_MapMove(const std::vector<std::string>& charNames);
    void SendPacket_BBS(const std::string& msg);
    void SendPacket_ExpEvent(short active, int dayMask);
    void SendPacket_GenItem(int itemNumber);
    void SendPacket_ReloadNpc();
    void SendPacket_OxAnswer(int answer);
    void SendPacket_CreateNpc(short npcIndex, short count);

    // 外部定義的封包函式 (原本是 extern 的)
    void SendPacket_BlockFunctions(int actionType);

public:
    bool m_showGmPanel = true;
    bool m_showSettingsPanel = false;
private:

    SDL_Window* m_window = nullptr;
    SDL_Renderer* m_renderer = nullptr;
    bool            m_inGame = false;
    LPCHARACTER     m_hero = nullptr;
    MapMoveContext  m_mapMoveContext;

    // GM 面板狀態 (使用成員變數而非 static，方便重置)
    struct GmState {
        int selectedActionIdx = 0;
        int npcIndex = 0;
        int npcCount = 1;
        float teleportX = 0.0f;
        float teleportY = 0.0f;
        int selectedMapIdx = 0;
        char messageBuf[256] = "Hello Server!";
        bool expEventDays[7] = { false };

        // 雙列表資料
        std::vector<std::string> destCharNames;
        int sourceSelectedIdx = -1;
        int destSelectedIdx = -1;
    } m_gmState;

    // 在 class CimGui 裡面加入：

public:
    // 開關 Mobile UI
    bool m_showMobileUI = true;
    bool MobileCtrlFlag = false;
    int m_btnPageIndex = 0;
private:
    void DrawMobileUI();        // 主進入點
    void DrawJoystick(float scale);        // 繪製搖桿
    void DrawActionButtons(float scale);   // 繪製技能按鈕

    // 模擬按鍵輸入 (這是溝通 ImGui 與 遊戲核心 的橋樑)
    void SendKeyDown(int sdl_scancode);
    void SendKeyUp(int sdl_scancode);

    void MyImGui_ImplSDLRenderer2_RenderDrawData(ImDrawData* draw_data, SDL_Renderer* renderer);
    // 搖桿的狀態變數
    ImVec2 m_joystickPos;      // 搖桿中心點 (螢幕座標)
    ImVec2 m_joystickKnob;     // 搖桿頭目前的偏移量 (-1.0 ~ 1.0)
    bool   m_isJoystickActive = false;
    int    m_lastDirKey = -1;  // 上一次發送的方向鍵，用來避免重複發送
};

#endif