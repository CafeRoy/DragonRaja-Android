#include "StdAfx.h" // 如果您的專案依賴它，請保留；否則建議移除並只 include 需要的
#include "CimGui.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl2.h"
#include "imgui/imgui_impl_sdlrenderer2.h"
#include <unordered_set>
#include <algorithm> // for std::remove_if
#include "MobileBridge.h"
// 假設這些是您的遊戲專用標頭檔
#include "map.h"
#include "SDLRenderer.h"
#include "gameproc.h"
// #include "PacketDefinition.h" // 假設這裡定義了 t_packet 等結構

// 外部函式宣告 (為了讓編譯器知道它們存在)
extern void SendBlockAttack();
extern void SendBlockMagic();
extern void SendBlockMove();
extern void SendBlockSkill();
// extern void SendBlockAntiTeam(); // 暫時註解，因為您代碼裡註解了

// 這是修改版的渲染函式，專門配合 SDL_RenderSetLogicalSize 使用
// 這是修改後的自定義渲染函式
// 修正了：
// 1. 移除了編譯不過的內部函式呼叫
// 2. 移除了會導致 ImGui 位置錯亂的 Viewport 重置程式碼
void CimGui::MyImGui_ImplSDLRenderer2_RenderDrawData(ImDrawData* draw_data, SDL_Renderer* renderer)
{
    // 檢查是否有東西要畫
    if (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f)
        return;

    float rsx = 1.0f;
    float rsy = 1.0f;
    SDL_RenderGetScale(renderer, &rsx, &rsy);
    ImVec2 render_scale;
    render_scale.x = (rsx == 1.0f) ? draw_data->FramebufferScale.x : 1.0f;
    render_scale.y = (rsy == 1.0f) ? draw_data->FramebufferScale.y : 1.0f;

    // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
    int fb_width = (int)(draw_data->DisplaySize.x * render_scale.x);
    int fb_height = (int)(draw_data->DisplaySize.y * render_scale.y);
    if (fb_width == 0 || fb_height == 0)
        return;

    // Catch up with texture updates. Most of the times, the list will have 1 element with an OK status, aka nothing to do.
    // (This almost always points to ImGui::GetPlatformIO().Textures[] but is part of ImDrawData to allow overriding or disabling texture updates).
    if (draw_data->Textures != nullptr)
        for (ImTextureData* tex : *draw_data->Textures)
            if (tex->Status != ImTextureStatus_OK)
                ImGui_ImplSDLRenderer2_UpdateTexture(tex);

    // [備份狀態]
    SDL_Rect original_clip_rect;
    bool original_clip_enabled = (SDL_RenderIsClipEnabled(renderer) == SDL_TRUE);
    SDL_RenderGetClipRect(renderer, &original_clip_rect);

    // [設定狀態]
    SDL_BlendMode original_blend_mode;
    SDL_GetRenderDrawBlendMode(renderer, &original_blend_mode);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // 這裡我們不改 Viewport/Scale，沿用遊戲設定

    ImVec2 clip_off = draw_data->DisplayPos;
    ImVec2 clip_scale = ImVec2(1.0f, 1.0f); // 假設 ImGui 邏輯座標 = SDL 邏輯座標

    for (const ImDrawList* draw_list : draw_data->CmdLists)
    {
        const ImDrawVert* vtx_buffer = draw_list->VtxBuffer.Data;
        const ImDrawIdx* idx_buffer = draw_list->IdxBuffer.Data;

        for (int cmd_i = 0; cmd_i < draw_list->CmdBuffer.Size; cmd_i++)
        {
            const ImDrawCmd* pcmd = &draw_list->CmdBuffer[cmd_i];
            if (pcmd->UserCallback)
            {
                if (pcmd->UserCallback == ImDrawCallback_ResetRenderState) {
                    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                } else {
                    pcmd->UserCallback(draw_list, pcmd);
                }
            }
            else
            {
                // Project scissor/clipping rectangles into framebuffer space
                ImVec2 clip_min((pcmd->ClipRect.x - clip_off.x) * clip_scale.x, (pcmd->ClipRect.y - clip_off.y) * clip_scale.y);
                ImVec2 clip_max((pcmd->ClipRect.z - clip_off.x) * clip_scale.x, (pcmd->ClipRect.w - clip_off.y) * clip_scale.y);
                if (clip_min.x < 0.0f) { clip_min.x = 0.0f; }
                if (clip_min.y < 0.0f) { clip_min.y = 0.0f; }
                if (clip_max.x > (float)fb_width) { clip_max.x = (float)fb_width; }
                if (clip_max.y > (float)fb_height) { clip_max.y = (float)fb_height; }
                if (clip_max.x <= clip_min.x || clip_max.y <= clip_min.y)
                    continue;

                SDL_Rect r = { (int)(clip_min.x), (int)(clip_min.y), (int)(clip_max.x - clip_min.x), (int)(clip_max.y - clip_min.y) };
                SDL_RenderSetClipRect(renderer, &r);

                const float* xy = (const float*)(const void*)((const char*)(vtx_buffer + pcmd->VtxOffset) + offsetof(ImDrawVert, pos));
                const float* uv = (const float*)(const void*)((const char*)(vtx_buffer + pcmd->VtxOffset) + offsetof(ImDrawVert, uv));
#if SDL_VERSION_ATLEAST(2,0,19)
                const SDL_Color* color = (const SDL_Color*)(const void*)((const char*)(vtx_buffer + pcmd->VtxOffset) + offsetof(ImDrawVert, col)); // SDL 2.0.19+
#else
                const int* color = (const int*)(const void*)((const char*)(vtx_buffer + pcmd->VtxOffset) + offsetof(ImDrawVert, col)); // SDL 2.0.17 and 2.0.18
#endif

                // Bind texture, Draw
                SDL_Texture* tex = (SDL_Texture*)pcmd->GetTexID();
                SDL_RenderGeometryRaw(renderer, tex,
                                      xy, (int)sizeof(ImDrawVert),
                                      color, (int)sizeof(ImDrawVert),
                                      uv, (int)sizeof(ImDrawVert),
                                      draw_list->VtxBuffer.Size - pcmd->VtxOffset,
                                      idx_buffer + pcmd->IdxOffset, pcmd->ElemCount, sizeof(ImDrawIdx));
            }
        }
    }

    // [還原狀態]
    SDL_RenderSetClipRect(renderer, original_clip_enabled ? &original_clip_rect : nullptr);
    SDL_SetRenderDrawBlendMode(renderer, original_blend_mode);
}

// 判斷目前的滑鼠座標 (mouseX, mouseY) 是否落在虛擬搖桿或按鈕的範圍內
bool IsInsideMobileUI(int mouseX, int mouseY)
{
    // 假設你的遊戲畫面邏輯大小是 1280x720
    // ★注意：如果你的視窗實際大小不是 1280x720，這裡需要做座標轉換 (例如 mouseX / scaleX)
    float logicW = 1280.0f;
    float logicH = 720.0f;
    float scale = 1.3f;

    // --- 1. 左下角搖桿範圍計算 ---
    float joySize = 220.0f * scale;
    // 搖桿矩形: X=30, Y=底部-JoySize-20, W=JoySize, H=JoySize
    float joyX = 30.0f;
    float joyY = logicH - joySize - 20.0f;

    if (mouseX >= joyX && mouseX <= (joyX + joySize) &&
        mouseY >= joyY && mouseY <= (joyY + joySize)) {
        return true; // 點在搖桿上
    }

    // --- 2. 右下角按鈕範圍計算 ---
    float btnW = 320.0f * scale;
    float btnH = 260.0f * scale;
    // 按鈕矩形: X=右邊界-BtnW-20, Y=底部-BtnH-10
    float btnX = logicW - btnW - 20.0f;
    float btnY = logicH - btnH - 10.0f;

    if (mouseX >= btnX && mouseX <= (btnX + btnW) &&
        mouseY >= btnY && mouseY <= (btnY + btnH)) {
        return true; // 點在按鈕區上
    }

    return false; // 點在空地上
}

// =================================================================================
// 單例實作
// =================================================================================
CimGui& CimGui::Instance() {
    static CimGui instance;
    return instance;
}

CimGui::CimGui() {
    // 初始化成員變數
    m_mapMoveContext = { 0, 0, 0, "" };
}

CimGui::~CimGui() {
    Shutdown();
}

void CimGui::Shutdown() {
//#ifdef _DEBUG
    if (m_renderer) { // 簡單判斷是否已初始化
        ImGui_ImplSDLRenderer2_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
        m_renderer = nullptr;
    }
//#endif
}
// -----------------------------------------------------------------------------
// [初始化函式]
// -----------------------------------------------------------------------------
bool CimGui::Init(SDL_Window* window, SDL_Renderer* renderer) {

    if (!window || !renderer) return false;

    m_window = window;
    m_renderer = renderer;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    // 允許鍵盤導航 (Android 上接藍牙鍵盤時有用)
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // --- [針對手指觸控的優化] ---
    // 加大拖曳判定閾值，解決手指點擊時產生的微小位移導致點擊無效的問題
    io.MouseDragThreshold = 50.0f;

    // 全局縮放 (視情況調整)
    float scaleFactor = 1.0f;
    ImGui::GetStyle().ScaleAllSizes(scaleFactor);
    io.FontGlobalScale = scaleFactor;

    // --- [SDL 觸控提示] ---
    // 建議開啟！這樣 SDL 會自動幫你把 Touch 事件轉成 Mouse 事件
    // 這樣你在 ProcessEvent 就不用處理 SDL_FINGERDOWN 等複雜事件
    SDL_SetHint(SDL_HINT_TOUCH_MOUSE_EVENTS, "1");
    // 關閉合成滑鼠事件 (避免重複觸發)
    SDL_SetHint(SDL_HINT_MOUSE_TOUCH_EVENTS, "0");

    ImGui::StyleColorsDark();

    // --- [載入字型] ---
    //const char* font_path = "ttf/myfont.ttf"; // 請替換成你的變數
    SDL_RWops* rw = SDL_RWFromFile(font_path_chs, "rb");

    if (rw) {
        Sint64 fileSize = SDL_RWsize(rw);
        void* fontData = malloc(fileSize);
        SDL_RWread(rw, fontData, 1, fileSize);
        SDL_RWclose(rw);

        ImFontConfig fontConfig;
        fontConfig.FontDataOwnedByAtlas = true; // 交給 ImGui 管理記憶體

        // 載入中文字型
        float fontSize = 24.0f; // 建議根據 DPI 動態調整
        io.Fonts->AddFontFromMemoryTTF(fontData, (int)fileSize, fontSize, &fontConfig, io.Fonts->GetGlyphRangesChineseFull());

        SDL_Log("INFO: Loaded font: %s", font_path);
    }
    else {
        SDL_Log("WARNING: Failed to load font, using default.");
        io.Fonts->AddFontDefault();
    }

    // 初始化標準後端
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    return true;
}

// -----------------------------------------------------------------------------
// [事件處理函式]
// -----------------------------------------------------------------------------
// =========================================================
// 2. 消息處理函式 (物理攔截版)
// =========================================================
bool CimGui::ProcessEvent(SDL_Event* event)
{
    if(event->type == MY_CUSTOM_EVENT_CODE) return false;

    // 1. 讓 ImGui 更新內部狀態 (必要，否則 UI 不會動)
    ImGui_ImplSDL2_ProcessEvent(event);
    ImGuiIO& io = ImGui::GetIO();

    // 2. 鍵盤輸入：保持原樣，直接交給 ImGui 判斷
    if (io.WantCaptureKeyboard) {
        if (event->type == SDL_KEYDOWN || event->type == SDL_KEYUP ||
            event->type == SDL_TEXTINPUT || event->type == SDL_TEXTEDITING) {
            return true;
        }
    }

    // 3. 滑鼠輸入：手動物理判定
    if (event->type == SDL_MOUSEBUTTONDOWN ||
        event->type == SDL_MOUSEBUTTONUP ||
        event->type == SDL_MOUSEWHEEL)
    {
        // 取得滑鼠在「真實視窗」中的座標
        float mouseX = (float)event->button.x;
        float mouseY = (float)event->button.y;

        // ★★★ 關鍵：座標轉換 ★★★
        // 因為 IsInsideMobileUI 是基於 1280x720 邏輯座標算的
        // 如果你的視窗現在是 1920x1080，必須把滑鼠座標「縮」回去
        float realW = io.DisplaySize.x; // 目前視窗寬度
        float realH = io.DisplaySize.y; // 目前視窗高度

        // 防止除以 0
        if (realW <= 0 || realH <= 0) return false;

        float scaleX = realW / MobileUILayout::LOGIC_W;
        float scaleY = realH / MobileUILayout::LOGIC_H;

        // 把真實滑鼠座標 轉成 邏輯座標
        float logicMouseX = mouseX / scaleX;
        float logicMouseY = mouseY / scaleY;

        // 開始判定：是否點在搖桿矩形內？
        ImVec4 joy = MobileUILayout::GetJoyRect();
        if (logicMouseX >= joy.x && logicMouseX <= (joy.x + joy.z) &&
            logicMouseY >= joy.y && logicMouseY <= (joy.y + joy.w)) {
            return true; // 攔截！這是點搖桿，遊戲別動
        }

        // 開始判定：是否點在按鈕矩形內？
        ImVec4 btn = MobileUILayout::GetBtnRect();
        if (logicMouseX >= btn.x && logicMouseX <= (btn.x + btn.z) &&
            logicMouseY >= btn.y && logicMouseY <= (btn.y + btn.w)) {
            return true; // 攔截！這是點按鈕，遊戲別動
        }
    }

    // 如果上面都沒攔截，表示點在空地上，回傳 false 給遊戲處理
    return false;
}
void CimGui::NewFrame() {
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGuiIO& io = ImGui::GetIO();
    // 1. 告訴 ImGui 我們的大小是 1280x720 (邏輯大小)
    io.DisplaySize = ImVec2(1280.0f, 720.0f);

    ImGui::NewFrame();
}

void CimGui::Render() {
//#ifdef _DEBUG
    if (!m_inGame) return;

    Draw();
    ImGui::Render();

    // ★★★ 關鍵：直接畫，不要切換模式 ★★★
    // 讓 SDL 負責把我們畫好的 1280x720 介面，連同遊戲畫面一起縮放到全螢幕
    //ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), m_renderer);
    MyImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), m_renderer);
//#endif
}

void CimGui::SetInGame(bool state, LPCHARACTER heroPtr) {
    m_inGame = state;
    m_hero = heroPtr;
    if (!state) {
        // 登出時重置部分狀態
        m_gmState.destCharNames.clear();
    }
}

// =================================================================================
// 主要繪製邏輯
// =================================================================================
void CimGui::Draw() {
    // 只有當開關為 true 時才畫
    if (m_showGmPanel ) {
        //DrawGmPanel();
    }else if(m_showSettingsPanel){
        //DrawSettingsPanel();
    }

    // 2. ★★★ 必須加入這行：畫手機虛擬按鈕 ★★★
    // 只有在遊戲中才顯示 (m_inGame)，或者您想一直顯示也可以
    if (m_showMobileUI && m_inGame) {
        DrawMobileUI();
    }
}

void CimGui::DrawGmPanel() {
    if (!m_hero) return;

    ImGuiIO& io = ImGui::GetIO();

    // 在 Logcat 印出 ImGui 偵測到的螢幕大小
    //SDL_Log("ImGui Screen Size: %.1f x %.1f", io.DisplaySize.x, io.DisplaySize.y);

    ImGui::GetMainViewport()->Pos = ImVec2(0, 0);
    ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH - 450.0f, 0.0f), ImGuiCond_Always);

    ImGui::SetNextWindowSize(ImVec2(450, 450), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin("GM Control Panel", &m_showGmPanel)) {
        ImGui::End();
        return;
    }

    // 1. GM 常用指令區
    if (ImGui::CollapsingHeader("GM Actions", ImGuiTreeNodeFlags_DefaultOpen)) {
        DrawGmActionsHeader();
    }

    // 2. 地圖傳送區
    if (ImGui::CollapsingHeader("Map Teleport", ImGuiTreeNodeFlags_DefaultOpen)) {
        DrawMapMoveHeader();
    }

    // 3. 角色列表管理區 (雙列表)
    if (ImGui::CollapsingHeader("Character Management", ImGuiTreeNodeFlags_DefaultOpen)) {
        DrawCharacterManager();
    }

    ImGui::End();
}

// ---------------------------------------------------------------------------------
// GM 指令區塊細節
// ---------------------------------------------------------------------------------
void CimGui::DrawGmActionsHeader() {
    static const std::vector<std::string> actions = {
            "Give Life All", "Make Ghost", "BBS All", "Exp Event", "Create NPC",
            "Select OX", "Block Attack", "Block Magic", "Block Move", "Block Skill",
            "Block Team", "Block All", "Reload NPC", "Gen Item"
    };

    // 下拉選單
    const char* currentAction = actions[m_gmState.selectedActionIdx].c_str();
    if (ImGui::BeginCombo("Action", currentAction)) {
        for (int n = 0; n < actions.size(); n++) {
            bool isSelected = (m_gmState.selectedActionIdx == n);
            if (ImGui::Selectable(actions[n].c_str(), isSelected)) {
                m_gmState.selectedActionIdx = n;
            }
            if (isSelected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    // 根據選擇顯示對應參數輸入框
    int action = m_gmState.selectedActionIdx; // 對應 GmActionType

    if (action == GM_ACTION_BBS_ALL) {
        ImGui::InputText("Message", m_gmState.messageBuf, IM_ARRAYSIZE(m_gmState.messageBuf));
    }
    else if (action == GM_ACTION_EXP_EVENT) {
        const char* days[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
        ImGui::Text("Active Days:");
        for (int i = 0; i < 7; ++i) {
            if (i > 0) ImGui::SameLine();
            ImGui::Checkbox(days[i], &m_gmState.expEventDays[i]);
        }
    }

    // NPC 相關參數 (共用)
    if (action == GM_ACTION_CREATE_NPC || action == GM_ACTION_EXP_EVENT ||
        action == GM_ACTION_SELECT_OX || action == GM_ACTION_GEN_ITEM) {
        ImGui::InputInt("Index/ID", &m_gmState.npcIndex);
        if (action == GM_ACTION_CREATE_NPC) {
            ImGui::SameLine();
            ImGui::InputInt("Count", &m_gmState.npcCount);
        }
    }

    ImGui::Separator();

    // 執行按鈕
    if (ImGui::Button("Execute Action", ImVec2(-1, 0))) { // 寬度填滿
        switch (action) {
            case GM_ACTION_BBS_ALL:
                SendPacket_BBS(m_gmState.messageBuf);
                break;
            case GM_ACTION_EXP_EVENT: {
                int mask = 0;
                for (int i = 0; i < 7; ++i) if (m_gmState.expEventDays[i]) mask |= (1 << i);
                SendPacket_ExpEvent(m_gmState.npcIndex, mask);
                break;
            }
            case GM_ACTION_CREATE_NPC:
                SendPacket_CreateNpc(m_gmState.npcIndex, m_gmState.npcCount);
                break;
            case GM_ACTION_SELECT_OX:
                SendPacket_OxAnswer(m_gmState.npcIndex);
                break;
            case GM_ACTION_GEN_ITEM:
                SendPacket_GenItem(m_gmState.npcIndex);
                break;
            case GM_ACTION_RELOAD_EVENTNPC:
                SendPacket_ReloadNpc();
                break;
                // 封鎖類指令
            case GM_ACTION_BLOCK_ATTACK:
            case GM_ACTION_BLOCK_MAGIC:
            case GM_ACTION_BLOCK_MOVE:
            case GM_ACTION_BLOCK_SKILL:
            case GM_ACTION_BLOCK_TEAM:
            case GM_ACTION_BLOCK_ALL:
                SendPacket_BlockFunctions(action);
                break;
                // 默認：針對選定角色執行的指令 (Give Life, Make Ghost)
            default:
                SendPacket_DoAction(m_gmState.destCharNames, action);
                break;
        }
    }
}

// ---------------------------------------------------------------------------------
// 地圖傳送區塊細節
// ---------------------------------------------------------------------------------
void CimGui::DrawMapMoveHeader() {
    // 假設 MapInfo 是全域變數
    // 建立地圖名稱列表 (為了效能，建議在 Init 或第一次開啟時建立緩存，這裡簡化處理)
    static std::vector<std::pair<int, std::string>> mapList;
    if (mapList.empty()) {
        for (int i = 0; i < MAX_MAP_; i++) {
            if (strlen(MapInfo[i].mapname) > 0) {
                mapList.push_back({ i, MapInfo[i].mapname });
            }
        }
    }

    // 尋找當前選中的地圖名稱
    std::string currentMapName = "Unknown";
    for (const auto& pair : mapList) {
        if (pair.first == m_gmState.selectedMapIdx) {
            currentMapName = pair.second;
            break;
        }
    }

    if (ImGui::BeginCombo("Map", currentMapName.c_str())) {
        for (const auto& pair : mapList) {
            bool isSelected = (m_gmState.selectedMapIdx == pair.first);
            if (ImGui::Selectable(pair.second.c_str(), isSelected)) {
                m_gmState.selectedMapIdx = pair.first;
            }
            if (isSelected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    ImGui::InputFloat("X", &m_gmState.teleportX, 1.0f, 10.0f, "%.1f");
    ImGui::SameLine();
    ImGui::InputFloat("Y", &m_gmState.teleportY, 1.0f, 10.0f, "%.1f");

    if (ImGui::Button("Teleport Players", ImVec2(-1, 0))) {
        // 設定上下文
        m_mapMoveContext.map_index = m_gmState.selectedMapIdx;
        m_mapMoveContext.x = (int)m_gmState.teleportX;
        m_mapMoveContext.y = (int)m_gmState.teleportY;

        if (ShowMessageBoxYesNo("Teleport Confirm", "Move selected players to new map?") == 1) {
            SendPacket_MapMove(m_gmState.destCharNames);
        }
    }
}

// ---------------------------------------------------------------------------------
// 角色管理區塊 (雙列表)
// ---------------------------------------------------------------------------------
void CimGui::DrawCharacterManager() {
    // ... (前面資料準備的部分保持不變) ...
    std::vector<std::string> sourceChars;
    std::unordered_set<std::string> destSet(m_gmState.destCharNames.begin(), m_gmState.destCharNames.end());
    LPCHARACTER ch = m_hero;
    while (ch) {
        if (destSet.find(ch->name) == destSet.end()) {
            sourceChars.push_back(ch->name);
        }
        ch = ch->lpNext;
    }

    // ==========================================================
    // ★★★ 設定列表寬度 ★★★
    // ==========================================================
    float windowWidth = ImGui::GetContentRegionAvail().x;

    // 方法 A：動態寬度 (填滿視窗，扣除中間按鈕區域)
    // 60.0f 是預留給中間按鈕的寬度，您可以加大這個數字讓列表變窄
    float listWidth = (windowWidth - 100.0f) * 0.5f;

    // 方法 B：固定寬度 (如果您想要死板的大小，取消下面註解)
    // float listWidth = 150.0f;

    // --- 左側 (Source) ---
    ImGui::BeginGroup();
    ImGui::Text("Nearby");

    // ★ 加入這行來改變下一個元件(ListBox)的寬度
    ImGui::SetNextItemWidth(listWidth);

    if (ImGui::ListBox("##Src", &m_gmState.sourceSelectedIdx, [](void* data, int idx, const char** out_text) {
        auto& v = *static_cast<std::vector<std::string>*>(data);
        if (idx < 0 || idx >= v.size()) return false;
        *out_text = v[idx].c_str(); return true;
    }, &sourceChars, sourceChars.size(), 10)) {
        // 單擊處理
    }
    // 雙擊加入
    if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0) && m_gmState.sourceSelectedIdx != -1) {
        m_gmState.destCharNames.push_back(sourceChars[m_gmState.sourceSelectedIdx]);
        m_gmState.sourceSelectedIdx = -1;
    }
    ImGui::EndGroup();

    ImGui::SameLine();

    // --- 中間 (Buttons) ---
    // 這邊稍微調整一下游標位置，讓按鈕置中一點
    ImGui::BeginGroup();
    ImGui::Dummy(ImVec2(0, 40));

    // 讓按鈕稍微窄一點，避免太寬
    float buttonWidth = 50.0f;

    if (ImGui::Button(" > ", ImVec2(buttonWidth, 0)) && m_gmState.sourceSelectedIdx != -1) {
        m_gmState.destCharNames.push_back(sourceChars[m_gmState.sourceSelectedIdx]);
        m_gmState.sourceSelectedIdx = -1;
    }
    if (ImGui::Button(" < ", ImVec2(buttonWidth, 0)) && m_gmState.destSelectedIdx != -1) {
        m_gmState.destCharNames.erase(m_gmState.destCharNames.begin() + m_gmState.destSelectedIdx);
        m_gmState.destSelectedIdx = -1;
    }
    ImGui::Dummy(ImVec2(0, 10));
    if (ImGui::Button("All>", ImVec2(buttonWidth, 0))) {
        m_gmState.destCharNames.insert(m_gmState.destCharNames.end(), sourceChars.begin(), sourceChars.end());
    }
    if (ImGui::Button("<All", ImVec2(buttonWidth, 0))) {
        m_gmState.destCharNames.clear();
    }
    ImGui::EndGroup();

    ImGui::SameLine();

    // --- 右側 (Dest) ---
    ImGui::BeginGroup();
    ImGui::Text("Selected");

    // ★ 加入這行來改變下一個元件(ListBox)的寬度
    ImGui::SetNextItemWidth(listWidth);

    if (ImGui::ListBox("##Dst", &m_gmState.destSelectedIdx, [](void* data, int idx, const char** out_text) {
        auto& v = *static_cast<std::vector<std::string>*>(data);
        if (idx < 0 || idx >= v.size()) return false;
        *out_text = v[idx].c_str(); return true;
    }, &m_gmState.destCharNames, m_gmState.destCharNames.size(), 10)) {
    }
    // 雙擊移除
    if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0) && m_gmState.destSelectedIdx != -1) {
        m_gmState.destCharNames.erase(m_gmState.destCharNames.begin() + m_gmState.destSelectedIdx);
        m_gmState.destSelectedIdx = -1;
    }
    ImGui::EndGroup();
}
// =================================================================================
// 輔助與封包發送實作
// =================================================================================

LPCHARACTER CimGui::FindCharacter(const std::string& name) {
    LPCHARACTER ch = m_hero;
    while (ch) {
        if (name == ch->name) return ch;
        ch = ch->lpNext;
    }
    return nullptr;
}

int CimGui::ShowMessageBoxYesNo(const char* title, const char* message) {
    const SDL_MessageBoxButtonData buttons[] = {
            { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 0, "No" },
            { SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 1, "Yes" }
    };
    const SDL_MessageBoxData messageboxdata = {
            SDL_MESSAGEBOX_INFORMATION,
            m_window, // 綁定到主視窗
            title,
            message,
            SDL_arraysize(buttons),
            buttons,
            NULL
    };
    int buttonid;
    if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) return -1;
    return buttonid;
}

// 封包發送邏輯 (從您原本的程式碼整理而來)
void CimGui::SendPacket_DoAction(const std::vector<std::string>& charNames, int action) {
    t_packet p;
    for (const std::string& name : charNames) {
        LPCHARACTER ch = FindCharacter(name);
        if (!ch) continue;

        if (action == GM_ACTION_MAKE_GHOST) {
            if (ch->viewtype == VIEWTYPE_GHOST_) continue;
            p.h.header.type = CMD_MAKE_GHOST;
        }
        else if (action == GM_ACTION_GIVE_LIFE) {
            if (ch->viewtype != VIEWTYPE_GHOST_) continue;
            p.h.header.type = CMD_GIVE_LIFE;
        }
        else {
            continue;
        }

        p.u.make_ghost.id = ch->id;
        p.h.header.size = sizeof(t_give_life);
        QueuePacket(&p, 1);
    }
}

void CimGui::SendPacket_MapMove(const std::vector<std::string>& charNames) {
    t_packet p;
    for (const std::string& name : charNames) {
        LPCHARACTER ch = FindCharacter(name);
        if (!ch) continue;

        p.h.header.type = CMD_DIRECT_MAP_MOVE;
        p.u.kein.direct_map_move.map_number = m_mapMoveContext.map_index;
        p.u.kein.direct_map_move.x = m_mapMoveContext.x;
        p.u.kein.direct_map_move.y = m_mapMoveContext.y;

        memset(p.u.kein.direct_map_move.name, 0, 20);
        strncpy(p.u.kein.direct_map_move.name, ch->name, 19); // 安全拷貝

        p.h.header.size = sizeof(k_direct_map_move);
        QueuePacket(&p, 1);
    }
}

void CimGui::SendPacket_BBS(const std::string& msg) {
    t_packet p;
    p.h.header.type = CMD_BBS;
    strncpy(p.u.server_bbs.bbs, msg.c_str(), MAX_PATH - 1); // 假設 bbs 是 char 陣列
    // p.h.header.size = sizeof(t_server_bbs) - MAX_PATH + msg.length(); // 您的原邏輯
    // 建議檢查 size 計算是否正確，這看起來有點危險
    p.h.header.size = sizeof(t_server_bbs); // 通常固定大小比較安全，除非Server支援變長
    QueuePacket(&p, 1);
}

void CimGui::SendPacket_ExpEvent(short active, int dayMask) {
    t_packet p = {};
    p.h.header.type = CMD_SET_EXP_EVENT;
    p.h.header.size = sizeof(t_set_exp_event);
    p.u.set_exp_event.active = active;
    p.u.set_exp_event.multiplier = 1;
    p.u.set_exp_event.range = 1;

    for (int i = 0; i < 7; i++) {
        p.u.set_exp_event.days[i] = (dayMask & (1 << i)) ? 1 : 0;
    }
    QueuePacket(&p, 1);
}

void CimGui::SendPacket_GenItem(int itemNumber) {
    t_packet p = {};
    p.h.header.type = CMD_GENERATE_ITEM;
    p.u.kein.default_int = itemNumber;
    p.h.header.size = sizeof(int);
    QueuePacket(&p, 1);
}

void CimGui::SendPacket_ReloadNpc() {
    t_packet p;
    p.h.header.type = CMD_EVENT_MONSTER;
    p.h.header.size = 0;
    QueuePacket(&p, 1);
}

void CimGui::SendPacket_OxAnswer(int answer) {
    t_packet p;
    p.h.header.type = CMD_OX_ANSER;
    p.u.kein.default_int = answer;
    p.h.header.size = sizeof(int);
    QueuePacket(&p, 1);
}

void CimGui::SendPacket_CreateNpc(short npcIndex, short count) {
    t_packet p;
    p.h.header.type = CMD_GENERATE_NPC;
    p.h.header.size = sizeof(t_set_exp_event); // 注意：這裡用 t_set_exp_event 結構是否正確？請確認 Server 端定義
    p.u.set_exp_event.active = npcIndex;
    p.u.set_exp_event.multiplier = count;
    p.u.set_exp_event.range = 1;
    memset(p.u.set_exp_event.days, 0, sizeof(p.u.set_exp_event.days));
    p.u.set_exp_event.days[1] = 1; // 預設週一？保留您的邏輯
    QueuePacket(&p, 1);
}

void CimGui::SendPacket_BlockFunctions(int actionType) {
    switch (actionType) {
        case GM_ACTION_BLOCK_ATTACK: SendBlockAttack(); break;
        case GM_ACTION_BLOCK_MAGIC:  SendBlockMagic(); break;
        case GM_ACTION_BLOCK_MOVE:   SendBlockMove(); break;
        case GM_ACTION_BLOCK_SKILL:  SendBlockSkill(); break;
        case GM_ACTION_BLOCK_TEAM:   /*SendBlockAntiTeam();*/ break;
        case GM_ACTION_BLOCK_ALL:
            SendBlockAttack();
            SendBlockMagic();
            //SendBlockAntiTeam();
            SendBlockMove();
            SendBlockSkill();
            break;
    }
}

void CimGui::SendKeyDown(int sdl_scancode) {
    SDL_Event event;
    memset(&event, 0, sizeof(event));

    // 1. 基本設定
    event.type = SDL_KEYDOWN;
    event.key.state = SDL_PRESSED;
    event.key.repeat = 0;

    // 2. ★★★ 關鍵修正：必須填寫 Window ID ★★★
    // 如果沒有填寫這個，SDL_PollEvent 拿出來後，可能因為找不到對應視窗而被丟棄
    if (m_window) {
        event.key.windowID = SDL_GetWindowID(m_window);
    }

    // 3. 補全時間戳記 (有些遊戲邏輯會檢查間隔)
    event.key.timestamp = SDL_GetTicks();

    // 4. 設定 Scancode
    event.key.keysym.scancode = (SDL_Scancode)sdl_scancode;

    // 5. ★ 建議補全 Keycode (sym) ★
    // 雖然您說遊戲只看 scancode，但 SDL 內部轉換有時依賴 sym
    event.key.keysym.sym = SDL_GetKeyFromScancode((SDL_Scancode)sdl_scancode);

    // 6. 推入並檢查結果
    if (SDL_PushEvent(&event) != 1) {
        SDL_Log("Failed to push KeyDown event! Error: %s", SDL_GetError());
    }
}

void CimGui::SendKeyUp(int sdl_scancode) {
    SDL_Event event;
    memset(&event, 0, sizeof(event));

    event.type = SDL_KEYUP;
    event.key.state = SDL_RELEASED;
    event.key.repeat = 0;

    if (m_window) {
        event.key.windowID = SDL_GetWindowID(m_window);
    }
    event.key.timestamp = SDL_GetTicks();
    event.key.keysym.scancode = (SDL_Scancode)sdl_scancode;
    event.key.keysym.sym = SDL_GetKeyFromScancode((SDL_Scancode)sdl_scancode);

    if (SDL_PushEvent(&event) != 1) {
        SDL_Log("Failed to push KeyUp event!");
    }
}
// -----------------------------------------------------------------------------
// [核心函式] 繪製虛擬搖桿
// -----------------------------------------------------------------------------
void CimGui::DrawJoystick(float scale) {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImGuiIO& io = ImGui::GetIO();

    ImVec2 winPos = ImGui::GetWindowPos();
    ImVec2 winSize = ImGui::GetWindowSize();

    // 使用視窗中心
    ImVec2 center = ImVec2(winPos.x + winSize.x * 0.5f, winPos.y + winSize.y * 0.5f);

    // 半徑乘上 scale
    float radius_base = 80.0f * scale;
    float radius_knob = 30.0f * scale;

    // --- (中間觸控計算邏輯保持不變) ---
    ImGui::SetCursorScreenPos(ImVec2(center.x - radius_base, center.y - radius_base));
    ImGui::InvisibleButton("JoystickZone", ImVec2(radius_base * 2, radius_base * 2));

    bool interacting = ImGui::IsItemActive();
    if (interacting) {
        m_isJoystickActive = true;
        ImVec2 touchPos = io.MousePos;
        float dx = touchPos.x - center.x;
        float dy = touchPos.y - center.y;
        float len = sqrtf(dx * dx + dy * dy);
        if (len > radius_base) {
            float s = radius_base / len;
            dx *= s; dy *= s;
        }
        m_joystickKnob = ImVec2(dx, dy);
    } else {
        m_isJoystickActive = false;
        m_joystickKnob = ImVec2(0, 0);
    }
    // --- (中間邏輯結束) ---

    // 繪圖
    draw_list->AddCircleFilled(center, radius_base, IM_COL32(100, 100, 100, 120));
    draw_list->AddCircle(center, radius_base, IM_COL32(200, 200, 200, 100), 0, 4.0f * scale);

    ImVec2 knobPos = ImVec2(center.x + m_joystickKnob.x, center.y + m_joystickKnob.y);
    ImU32 knobColor = m_isJoystickActive ? IM_COL32(0, 200, 0, 100) : IM_COL32(200, 200, 200, 100);
    draw_list->AddCircleFilled(knobPos, radius_knob, knobColor);

    // =========================================================
    // ★★★ 修正部分：改成呼叫 MobileBridge ★★★
    // =========================================================

    // 用一個靜態變數來記錄上一幀是否有移動，確保 Stop 只呼叫一次
    static bool wasMoving = false;

    if (m_isJoystickActive) {
        // 1. 計算歸一化數值 (-1.0 ~ 1.0)
        // dx, dy 是偏移量，除以半徑就是比例
        float inputX = m_joystickKnob.x / radius_base;
        float inputY = m_joystickKnob.y / radius_base;

        // 2. 傳送給橋接器
        // 這裡會直接改寫遊戲內的 g_bKeyTable
        Mobile_OnJoystickMove(inputX, inputY);

        wasMoving = true;
    }
    else {
        // 3. 只有當「剛剛還在動」現在變成「不動」時，才呼叫 Stop
        if (wasMoving) {
            Mobile_OnJoystickStop();
            wasMoving = false;
        }
    }
}
// 輔助函式：畫圓形按鈕並回傳是否點擊
bool RoundButton(const char* label, ImVec2 center, float radius, ImU32 color) {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImGui::SetCursorScreenPos(ImVec2(center.x - radius, center.y - radius));

    // 隱形按鈕
    bool pressed = ImGui::InvisibleButton(label, ImVec2(radius * 2, radius * 2));

    // 互動回饋顏色
    ImU32 finalColor = color;
    if (ImGui::IsItemActive()) finalColor = IM_COL32(255, 0, 0, 200); // 按下變紅
    else if (ImGui::IsItemHovered()) finalColor = IM_COL32(255, 255, 255, 200); // 懸停變亮

    // 畫圓
    draw_list->AddCircleFilled(center, radius, finalColor);

    // 畫文字 (置中)
    ImVec2 textSize = ImGui::CalcTextSize(label);
    draw_list->AddText(ImVec2(center.x - textSize.x * 0.5f, center.y - textSize.y * 0.5f),
                       IM_COL32(0, 0, 0, 255), label);

    return pressed;
}

// -----------------------------------------------------------------------------
// [核心函式] 繪製右下角動作按鈕
// -----------------------------------------------------------------------------
void CimGui::DrawActionButtons(float scale)
{
    float bigSize   = MobileUILayout::BTN_BIG_SIZE;
    float smallSize = MobileUILayout::BTN_SMALL_SIZE;
    float tinySize  = MobileUILayout::BTN_TINY_SIZE; // 新增這行
    float spacing   = MobileUILayout::SPACING;
    ImVec2 areaSize = ImGui::GetWindowSize();

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 100.0f);

    // ------------------------------------------------
    // 1. 最下層 (Row 3): 攻擊與技能
    // ------------------------------------------------
    float row3_Y = areaSize.y - bigSize;

    // ATK (左下)
    ImGui::SetCursorPos(ImVec2(0, row3_Y));
    ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.9f, 0.5f, 0.0f, 0.6f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.6f, 0.1f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(1.0f, 0.7f, 0.2f, 0.9f));
    if (ImGui::Button("ATK", ImVec2(bigSize, bigSize))) {
        Mobile_DoLeftButton();
    }
    ImGui::PopStyleColor(3);

    // Skill (右下)
    ImGui::SetCursorPos(ImVec2(bigSize + spacing, row3_Y));
    ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.5f, 0.5f, 0.5f, 0.6f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.6f, 0.6f, 0.6f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.3f, 0.3f, 0.3f, 0.9f));
    if (ImGui::Button("Skill", ImVec2(bigSize, bigSize))) {
        Mobile_DoRightButton(0);
    }
    ImGui::PopStyleColor(3);


    // ------------------------------------------------
    // 2. 中間層 (Row 2): 藥水
    // ------------------------------------------------
    // Y位置 = 底部 - 大按鈕 - 間隙 - 小按鈕
    float row2_Y = areaSize.y - bigSize - spacing - smallSize;
    float offsetSmall = (bigSize - smallSize) * 0.5f;

    // HP (左中)
    ImGui::SetCursorPos(ImVec2(0 + offsetSmall, row2_Y));
    ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.8f, 0.1f, 0.1f, 0.6f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 0.2f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(1.0f, 0.3f, 0.3f, 0.9f));
    if (ImGui::Button("HP", ImVec2(smallSize, smallSize))) {
        Mobile_UsePotion(0);
    }
    ImGui::PopStyleColor(3);

    // MP (右中)
    ImGui::SetCursorPos(ImVec2(bigSize + spacing + offsetSmall, row2_Y));
    ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.1f, 0.3f, 0.9f, 0.6f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.4f, 1.0f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.0f, 0.2f, 0.7f, 0.9f));
    if (ImGui::Button("MP", ImVec2(smallSize, smallSize))) {
        Mobile_UsePotion(1);
    }
    ImGui::PopStyleColor(3);


    // ------------------------------------------------
    // 3. 最上層 (Row 1): 功能鍵 (CTRL)
    // ------------------------------------------------
    // ★★★ 這是新加的樓層 ★★★
    // 把它放在 MP 的正上方
    // Y位置 = MP的Y - 間隙 - 超小按鈕
    float row1_Y = row2_Y - spacing - tinySize;
    float offsetTiny = (bigSize - tinySize) * 0.5f; // 讓它對齊最下面的大按鈕中心

    // CTRL (右上頂端) - 紫色
    ImGui::SetCursorPos(ImVec2(bigSize + spacing + offsetTiny, row1_Y));

    ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.6f, 0.2f, 0.8f, 0.6f)); // 紫色半透明
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.3f, 0.9f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.4f, 0.0f, 0.6f, 0.9f)); // 按下深紫

    // 如果你希望按住才有效，就要用 IsItemActive 判定
    // 這裡先寫成點擊觸發
    if (ImGui::Button("Ctrl", ImVec2(tinySize, tinySize))) {
        // 觸發 Ctrl 功能
        CheckPC = MobileCtrlFlag = !MobileCtrlFlag;
        if(Hero){
            Hero->DontMoveAttackFlag = MobileCtrlFlag;
            Hero->display_item = MobileCtrlFlag;
        }
    }
    ImGui::PopStyleColor(3);


    ImGui::PopStyleVar();
}
// =========================================================
// 3. 繪圖函式 (同步使用佈局參數)
// =========================================================
void CimGui::DrawMobileUI() {
    if (!m_showMobileUI) return;

    // 這裡我們只負責畫圖，不再需要定義 magic number

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar |
                             ImGuiWindowFlags_NoResize |
                             ImGuiWindowFlags_NoScrollbar |
                             ImGuiWindowFlags_NoCollapse |
                             ImGuiWindowFlags_NoBackground; // 透明窗口

    // --- 畫左下角搖桿 ---
    ImVec4 joy = MobileUILayout::GetJoyRect();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0)); // 去除邊距
    ImGui::SetNextWindowPos(ImVec2(joy.x, joy.y), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(joy.z, joy.w), ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.2f);
    if (ImGui::Begin("JoystickWindow", NULL, flags)) {
        // 為了美觀，手畫一個半透明圓形背景
        ImVec2 p = ImGui::GetWindowPos();
        ImVec2 center = ImVec2(p.x + joy.z * 0.5f, p.y + joy.w * 0.5f);
        ImGui::GetWindowDrawList()->AddCircleFilled(center, joy.z * 0.5f, IM_COL32(0, 0, 0, 100));

        DrawJoystick(MobileUILayout::SCALE);
    }
    ImGui::End();
    ImGui::PopStyleVar();

    // --- 畫右下角按鈕 ---
    ImVec4 btn = MobileUILayout::GetBtnRect();

    ImGui::SetNextWindowPos(ImVec2(btn.x, btn.y), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(btn.z, btn.w), ImGuiCond_Always);

    // 按鈕區也來個圓角矩形背景
    ImGui::SetNextWindowBgAlpha(0.2f);

    if (ImGui::Begin("ButtonsWindow", NULL, flags)) {
        ImVec2 p = ImGui::GetWindowPos();
        ImGui::GetWindowDrawList()->AddRectFilled(
                p,
                ImVec2(p.x + btn.z, p.y + btn.w),
                IM_COL32(0, 0, 0, 80),
                20.0f
        );

        DrawActionButtons(MobileUILayout::SCALE);
    }
    ImGui::End();

    // (DEBUG用) 如果想確認點擊範圍對不對，把下面這行打開，會畫出紅框
    /*
    ImDrawList* fg = ImGui::GetForegroundDrawList();
    fg->AddRect(ImVec2(joy.x, joy.y), ImVec2(joy.x+joy.z, joy.y+joy.w), IM_COL32(255,0,0,255), 2.0f);
    fg->AddRect(ImVec2(btn.x, btn.y), ImVec2(btn.x+btn.z, btn.y+btn.w), IM_COL32(255,0,0,255), 2.0f);
     */
}
