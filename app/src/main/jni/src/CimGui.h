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
#include <functional> // 為了 std::function

// 前向宣告
class CCharacter;
typedef CCharacter* LPCHARACTER;

// 定義 GM 操作的枚舉
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
// 1. 定義 UI 佈局參數
// =========================================================
struct MobileUILayout {
    static constexpr float LOGIC_W = 1280.0f;
    static constexpr float LOGIC_H = 720.0f;
    static constexpr float SCALE   = 1.2f;

    // 左下角搖桿
    static constexpr float JOY_SIZE     = 160.0f * SCALE;
    static constexpr float JOY_MARGIN_X = 30.0f;
    static constexpr float JOY_MARGIN_Y = 50.0f;

    // --- 右下角按鈕區 ---
    static constexpr float BTN_BIG_SIZE   = 80.0f * SCALE;
    static constexpr float BTN_SMALL_SIZE = 50.0f * SCALE;
    static constexpr float BTN_TINY_SIZE  = 40.0f * SCALE;

    static constexpr float SPACING        = 10.0f * SCALE;
    static constexpr float BTN_AREA_W = (BTN_BIG_SIZE * 2) + SPACING;
    static constexpr float BTN_AREA_H = BTN_BIG_SIZE + BTN_SMALL_SIZE + BTN_TINY_SIZE + (SPACING * 2);
    static constexpr float BTN_MARGIN_X = 20.0f;
    static constexpr float BTN_MARGIN_Y = 20.0f;

    // Helper 函式
    static ImVec4 GetJoyRect() { return ImVec4(JOY_MARGIN_X, LOGIC_H - JOY_SIZE - JOY_MARGIN_Y, JOY_SIZE, JOY_SIZE); }
    static ImVec4 GetBtnRect() { return ImVec4(LOGIC_W - BTN_AREA_W - BTN_MARGIN_X, LOGIC_H - BTN_AREA_H - BTN_MARGIN_Y, BTN_AREA_W, BTN_AREA_H); }
};

class CimGui
{
public:
    static CimGui& Instance();
    CimGui(const CimGui&) = delete;
    CimGui& operator=(const CimGui&) = delete;

public:
    bool Init(SDL_Window* window, SDL_Renderer* renderer);
    void Shutdown();
    void NewFrame();
    void Render();
    bool ProcessEvent(SDL_Event* event); // 核心修改點

    void SetInGame(bool state, LPCHARACTER heroPtr = nullptr);
    void Draw();

private:
    CimGui();
    ~CimGui();

#ifdef _DEBUG
    void DrawGmPanel();
    void DrawGmActionsHeader();
    void DrawMapMoveHeader();
    void DrawCharacterManager();
#endif
    void DrawSettingsPanel();
    int  ShowMessageBoxYesNo(const char* title, const char* message);
    LPCHARACTER FindCharacter(const std::string& name);

#ifdef _DEBUG
    void SendPacket_DoAction(const std::vector<std::string>& charNames, int action);
    void SendPacket_MapMove(const std::vector<std::string>& charNames);
    void SendPacket_BBS(const std::string& msg);
    void SendPacket_ExpEvent(short active, int dayMask);
    void SendPacket_GenItem(int itemNumber);
    void SendPacket_ReloadNpc();
    void SendPacket_OxAnswer(int answer);
    void SendPacket_CreateNpc(short npcIndex, short count);
    void SendPacket_BlockFunctions(int actionType);
#endif

public:
    bool m_showGmPanel = true;
    bool m_showSettingsPanel = false;

private:
    SDL_Window* m_window = nullptr;
    SDL_Renderer* m_renderer = nullptr;
    bool            m_inGame = false;
    LPCHARACTER     m_hero = nullptr;
    MapMoveContext  m_mapMoveContext;

    struct GmState {
        int selectedActionIdx = 0;
        int npcIndex = 0;
        int npcCount = 1;
        float teleportX = 0.0f;
        float teleportY = 0.0f;
        int selectedMapIdx = 0;
        char messageBuf[256] = "Hello Server!";
        bool expEventDays[7] = { false };
        std::vector<std::string> destCharNames;
        int sourceSelectedIdx = -1;
        int destSelectedIdx = -1;
    } m_gmState;

public:
    bool m_showMobileUI = true;
    bool MobileCtrlFlag = false;
    int m_btnPageIndex = 0;

private:
    void DrawMobileUI();
    void DrawJoystick(float scale);
    void DrawActionButtons(float scale);

    // ★★★ 新增：按鈕狀態管理結構 (解決長按重覆與互相干擾問題) ★★★
    struct ButtonState {
        bool isLongPressTriggered = false;
    };
    std::unordered_map<std::string, ButtonState> m_btnStates;

    // 進階按鈕函式
    void MobileButtonEx(const char* label, const ImVec2& size, std::function<void()> onShortClick, std::function<void()> onLongPress, float threshold = 0.25f);

    void MyImGui_ImplSDLRenderer2_RenderDrawData(ImDrawData* draw_data, SDL_Renderer* renderer);

    // ★★★ 修改：多點觸控搖桿狀態變數 ★★★
    SDL_FingerID m_joyFingerId = -1; // 記錄哪根手指正在按搖桿
    ImVec2       m_joyCurrentPos;    // 手指目前的 Raw 座標 (0~1)
    bool         m_isJoyTouching = false; // 是否正在觸控搖桿
};

#endif