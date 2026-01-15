#include "StdAfx.h"
#include "CimGui.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl2.h"
#include "imgui/imgui_impl_sdlrenderer2.h"
#include <unordered_set>
#include <algorithm>
#include "MobileBridge.h"
#include "map.h"
#include "SDLRenderer.h"
#include "gameproc.h"

// 外部函式宣告
extern void SendBlockAttack();
extern void SendBlockMagic();
extern void SendBlockMove();
extern void SendBlockSkill();

// =================================================================================
// 渲染函式 (保持原本邏輯)
// =================================================================================
void CimGui::MyImGui_ImplSDLRenderer2_RenderDrawData(ImDrawData* draw_data, SDL_Renderer* renderer)
{
    if (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f) return;

    float rsx = 1.0f; float rsy = 1.0f;
    SDL_RenderGetScale(renderer, &rsx, &rsy);
    ImVec2 render_scale;
    render_scale.x = (rsx == 1.0f) ? draw_data->FramebufferScale.x : 1.0f;
    render_scale.y = (rsy == 1.0f) ? draw_data->FramebufferScale.y : 1.0f;

    int fb_width = (int)(draw_data->DisplaySize.x * render_scale.x);
    int fb_height = (int)(draw_data->DisplaySize.y * render_scale.y);
    if (fb_width == 0 || fb_height == 0) return;

    if (draw_data->Textures != nullptr)
        for (ImTextureData* tex : *draw_data->Textures)
            if (tex->Status != ImTextureStatus_OK)
                ImGui_ImplSDLRenderer2_UpdateTexture(tex);

    SDL_Rect original_clip_rect;
    bool original_clip_enabled = (SDL_RenderIsClipEnabled(renderer) == SDL_TRUE);
    SDL_RenderGetClipRect(renderer, &original_clip_rect);
    SDL_BlendMode original_blend_mode;
    SDL_GetRenderDrawBlendMode(renderer, &original_blend_mode);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    ImVec2 clip_off = draw_data->DisplayPos;
    ImVec2 clip_scale = ImVec2(1.0f, 1.0f);

    for (const ImDrawList* draw_list : draw_data->CmdLists)
    {
        const ImDrawVert* vtx_buffer = draw_list->VtxBuffer.Data;
        const ImDrawIdx* idx_buffer = draw_list->IdxBuffer.Data;

        for (int cmd_i = 0; cmd_i < draw_list->CmdBuffer.Size; cmd_i++)
        {
            const ImDrawCmd* pcmd = &draw_list->CmdBuffer[cmd_i];
            if (pcmd->UserCallback) {
                if (pcmd->UserCallback == ImDrawCallback_ResetRenderState) {
                    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                } else {
                    pcmd->UserCallback(draw_list, pcmd);
                }
            } else {
                ImVec2 clip_min((pcmd->ClipRect.x - clip_off.x) * clip_scale.x, (pcmd->ClipRect.y - clip_off.y) * clip_scale.y);
                ImVec2 clip_max((pcmd->ClipRect.z - clip_off.x) * clip_scale.x, (pcmd->ClipRect.w - clip_off.y) * clip_scale.y);
                if (clip_min.x < 0.0f) { clip_min.x = 0.0f; }
                if (clip_min.y < 0.0f) { clip_min.y = 0.0f; }
                if (clip_max.x > (float)fb_width) { clip_max.x = (float)fb_width; }
                if (clip_max.y > (float)fb_height) { clip_max.y = (float)fb_height; }
                if (clip_max.x <= clip_min.x || clip_max.y <= clip_min.y) continue;

                SDL_Rect r = { (int)(clip_min.x), (int)(clip_min.y), (int)(clip_max.x - clip_min.x), (int)(clip_max.y - clip_min.y) };
                SDL_RenderSetClipRect(renderer, &r);

                const float* xy = (const float*)(const void*)((const char*)(vtx_buffer + pcmd->VtxOffset) + offsetof(ImDrawVert, pos));
                const float* uv = (const float*)(const void*)((const char*)(vtx_buffer + pcmd->VtxOffset) + offsetof(ImDrawVert, uv));
#if SDL_VERSION_ATLEAST(2,0,19)
                const SDL_Color* color = (const SDL_Color*)(const void*)((const char*)(vtx_buffer + pcmd->VtxOffset) + offsetof(ImDrawVert, col));
#else
                const int* color = (const int*)(const void*)((const char*)(vtx_buffer + pcmd->VtxOffset) + offsetof(ImDrawVert, col));
#endif
                SDL_Texture* tex = (SDL_Texture*)pcmd->GetTexID();
                SDL_RenderGeometryRaw(renderer, tex, xy, (int)sizeof(ImDrawVert), color, (int)sizeof(ImDrawVert), uv, (int)sizeof(ImDrawVert), draw_list->VtxBuffer.Size - pcmd->VtxOffset, idx_buffer + pcmd->IdxOffset, pcmd->ElemCount, sizeof(ImDrawIdx));
            }
        }
    }
    SDL_RenderSetClipRect(renderer, original_clip_enabled ? &original_clip_rect : nullptr);
    SDL_SetRenderDrawBlendMode(renderer, original_blend_mode);
}

// =================================================================================
// 單例與初始化
// =================================================================================
CimGui& CimGui::Instance() {
    static CimGui instance;
    return instance;
}

CimGui::CimGui() {
    m_mapMoveContext = { 0, 0, 0, "" };
}

CimGui::~CimGui() {
    Shutdown();
}

void CimGui::Shutdown() {
    if (m_renderer) {
        ImGui_ImplSDLRenderer2_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
        m_renderer = nullptr;
    }
}

bool CimGui::Init(SDL_Window* window, SDL_Renderer* renderer) {
    if (!window || !renderer) return false;

    m_window = window;
    m_renderer = renderer;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.MouseDragThreshold = 50.0f;

    float scaleFactor = 1.0f;
    ImGui::GetStyle().ScaleAllSizes(scaleFactor);
    io.FontGlobalScale = scaleFactor;

    SDL_SetHint(SDL_HINT_TOUCH_MOUSE_EVENTS, "1");
    SDL_SetHint(SDL_HINT_MOUSE_TOUCH_EVENTS, "0");

    ImGui::StyleColorsDark();

    // --- [載入字型] ---
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

    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    return true;
}

// =================================================================================
// ★★★ [核心修改] ProcessEvent：多點觸控分流 ★★★
// =================================================================================
bool CimGui::ProcessEvent(SDL_Event* event)
{
    if(event->type == MY_CUSTOM_EVENT_CODE) return false;

    // 定義攔截區域常數 (邏輯座標 1280x720)
    // X: 400, Y: 154, W: 530, H: 45
    // 範圍: X[400 ~ 930], Y[154 ~ 199]
    const float BLOCK_X_MIN = 390.0f;
    const float BLOCK_X_MAX = BLOCK_X_MIN + 530.0f; // 930.0f
    const float BLOCK_Y_MIN = 566.0f;
    const float BLOCK_Y_MAX = BLOCK_Y_MIN + 45.0f;  // 199.0f

    // -------------------------------------------------------------------------
    // 1. 優先處理「手指」事件 (SDL_FINGER*)
    // -------------------------------------------------------------------------
    if (event->type == SDL_FINGERDOWN || event->type == SDL_FINGERMOTION || event->type == SDL_FINGERUP) {
        // 1. 計算手指在 1280x720 邏輯空間的座標
        float logicW = MobileUILayout::LOGIC_W;
        float logicH = MobileUILayout::LOGIC_H;
        float touchX = event->tfinger.x * logicW;
        float touchY = event->tfinger.y * logicH;

        // ★★★ [新增功能] 強制攔截特定區域 (400, 154, 530, 45) ★★★
        // 只要手指落在這裡，直接吃掉消息，不讓遊戲處理
        if (touchX >= BLOCK_X_MIN && touchX <= BLOCK_X_MAX &&
            touchY >= BLOCK_Y_MIN && touchY <= BLOCK_Y_MAX)
        {
            // 如果你希望 ImGui 也不要處理這裡，就在這裡 return true
            // 如果你希望 ImGui 可以處理(例如這區域有按鈕)但遊戲不能處理，也是 return true
            // 因為 ProcessEvent return true 代表「攔截，不傳給遊戲引擎」
            if(m_hero)  m_hero->Mobile_RealMove=false;
        }
        else{
            if(m_hero)  m_hero->Mobile_RealMove=true;
        }

        // 2. 判斷搖桿 (原有的邏輯)
        ImVec4 joy = MobileUILayout::GetJoyRect();
        float buffer = 50.0f;

        if (event->type == SDL_FINGERDOWN) {
            if (touchX >= joy.x - buffer && touchX <= joy.x + joy.z + buffer &&
                touchY >= joy.y - buffer && touchY <= joy.y + joy.w + buffer)
            {
                m_joyFingerId = event->tfinger.fingerId;
                m_joyCurrentPos = ImVec2(event->tfinger.x, event->tfinger.y);
                m_isJoyTouching = true;
                return true;
            }
        }
        else if (event->type == SDL_FINGERMOTION) {
            if (m_isJoyTouching && event->tfinger.fingerId == m_joyFingerId) {
                m_joyCurrentPos = ImVec2(event->tfinger.x, event->tfinger.y);
                return true;
            }
        }
        else if (event->type == SDL_FINGERUP) {
            if (m_isJoyTouching && event->tfinger.fingerId == m_joyFingerId) {
                m_isJoyTouching = false;
                m_joyFingerId = -1;
                return true;
            }
        }
    }

    // -------------------------------------------------------------------------
    // 2. ImGui 處理
    // -------------------------------------------------------------------------
    ImGui_ImplSDL2_ProcessEvent(event);
    ImGuiIO& io = ImGui::GetIO();

    if (io.WantCaptureKeyboard) {
        if (event->type == SDL_KEYDOWN || event->type == SDL_KEYUP ||
            event->type == SDL_TEXTINPUT || event->type == SDL_TEXTEDITING) {
            return true;
        }
    }

    // ★ 建議加入這行：通用的 ImGui 攔截 ★
    // 如果滑鼠/手指正在操作任何 ImGui 的視窗(例如 GM 面板)，就直接攔截
    // 這樣你就不用為 GM 面板手動寫座標判斷了
    if (io.WantCaptureMouse) {
        // 但要注意：因為我們上面已經手動處理了 Joystick 的 Finger 事件
        // 這裡攔截主要是為了防止「右手點擊按鈕或面板」時穿透到遊戲
        return true;
    }

    // -------------------------------------------------------------------------
    // 3. 滑鼠輸入攔截 (Mouse Input)
    // -------------------------------------------------------------------------
    if (event->type == SDL_MOUSEBUTTONDOWN || event->type == SDL_MOUSEMOTION ||
        event->type == SDL_MOUSEBUTTONUP || event->type == SDL_MOUSEWHEEL)
    {
        float mouseX = (float)event->button.x;
        float mouseY = (float)event->button.y;
        float realW = io.DisplaySize.x;
        float realH = io.DisplaySize.y;
        if (realW <= 0 || realH <= 0) return false;

        float logicMouseX = mouseX / (realW / MobileUILayout::LOGIC_W);
        float logicMouseY = mouseY / (realH / MobileUILayout::LOGIC_H);

        // ★★★ [新增功能] 強制攔截特定區域 (針對滑鼠) ★★★
        if (logicMouseX >= BLOCK_X_MIN && logicMouseX <= BLOCK_X_MAX &&
            logicMouseY >= BLOCK_Y_MIN && logicMouseY <= BLOCK_Y_MAX)
        {
            if(m_hero)  m_hero->Mobile_RealMove=false; // 吃掉消息
        }
        else{
            if(m_hero)  m_hero->Mobile_RealMove=true;
        }

        // 按鈕區攔截 (原有的)
        ImVec4 btn = MobileUILayout::GetBtnRect();
        if (logicMouseX >= btn.x && logicMouseX <= (btn.x + btn.z) &&
            logicMouseY >= btn.y && logicMouseY <= (btn.y + btn.w)) {
            return true;
        }

        // 搖桿區攔截 (原有的)
        ImVec4 joy = MobileUILayout::GetJoyRect();
        if (logicMouseX >= joy.x && logicMouseX <= (joy.x + joy.z) &&
            logicMouseY >= joy.y && logicMouseY <= (joy.y + joy.w)) {
            return true;
        }
    }

    return false;
}

void CimGui::NewFrame() {
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(1280.0f, 720.0f);
    ImGui::NewFrame();
}

void CimGui::Render() {
    if (!m_inGame) return;
    Draw();
    ImGui::Render();
    MyImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), m_renderer);
}

void CimGui::SetInGame(bool state, LPCHARACTER heroPtr) {
    m_inGame = state;
    m_hero = heroPtr;
    if (!state) m_gmState.destCharNames.clear();
}

void CimGui::Draw() {
    if (m_showGmPanel) {
#ifdef _DEBUG
        DrawGmPanel();
#endif
    } else if (m_showSettingsPanel) {
        DrawSettingsPanel();
    }
    if (m_showMobileUI && m_inGame) {
        DrawMobileUI();
    }
// =========================================================
}

#ifdef _DEBUG
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
} //GMPanel GMMenu
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
#endif

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


void CimGui::DrawSettingsPanel() {}

// =================================================================================
// ★★★ [核心修改] DrawJoystick：狀態驅動 (State-Driven) ★★★
// =================================================================================
void CimGui::DrawJoystick(float scale) {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    ImVec2 winPos = ImGui::GetWindowPos();
    ImVec2 winSize = ImGui::GetWindowSize();
    ImVec2 center = ImVec2(winPos.x + winSize.x * 0.5f, winPos.y + winSize.y * 0.5f);

    float radius_base = 80.0f * scale;
    // float radius_knob = 30.0f * scale; // 沒用到，直接寫死在下面

    ImVec2 knobOffset = ImVec2(0, 0);

    // ★ 關鍵：這裡不再使用 ImGui::InvisibleButton
    // 而是直接讀取由 ProcessEvent 更新的 m_isJoyTouching 與 m_joyCurrentPos

    // 處理移動邏輯
    if (m_isJoyTouching) {
        // 將 Raw 座標 (0~1) 轉回邏輯座標 (1280x720)
        float currentX = m_joyCurrentPos.x * MobileUILayout::LOGIC_W;
        float currentY = m_joyCurrentPos.y * MobileUILayout::LOGIC_H;

        float dx = currentX - center.x;
        float dy = currentY - center.y;

        // 限制圓形範圍
        float len = sqrtf(dx * dx + dy * dy);
        if (len > radius_base) {
            float s = radius_base / len;
            dx *= s; dy *= s;
        }

        knobOffset = ImVec2(dx, dy);

        // 計算歸一化數值 (-1.0 ~ 1.0)
        float inputX = dx / radius_base;
        float inputY = dy / radius_base;

        // ★ 每一幀都發送移動指令，即使手指沒動只要按著就會走
        Mobile_OnJoystickMove(inputX, inputY);
    }
    else {
        // 確保發送一次停止指令
        static bool wasMoving = false;
        // 簡單偵測：如果上一幀還在動，這幀沒動，就送 Stop
        // 這裡我們利用一個 static 變數輔助，或者依賴 Mobile_OnJoystickStop 內部的檢查
        if (wasMoving) {
            Mobile_OnJoystickStop();
            wasMoving = false;
        }
    }

    if (m_isJoyTouching) {
        static bool wasMoving = true; // 標記為正在動
    }

    // 純粹繪圖
    draw_list->AddCircleFilled(center, radius_base, IM_COL32(100, 100, 100, 120));
    draw_list->AddCircle(center, radius_base, IM_COL32(200, 200, 200, 100), 0, 4.0f * scale);

    ImVec2 knobPos = ImVec2(center.x + knobOffset.x, center.y + knobOffset.y);
    ImU32 knobColor = m_isJoyTouching ? IM_COL32(0, 200, 0, 150) : IM_COL32(200, 200, 200, 150);
    draw_list->AddCircleFilled(knobPos, 30.0f * scale, knobColor);
}

// =================================================================================
// ★★★ [核心修改] MobileButtonEx：使用 Map 記錄狀態 ★★★
// =================================================================================
void CimGui::MobileButtonEx(const char* label, const ImVec2& size, std::function<void()> onShortClick, std::function<void()> onLongPress, float threshold)
{
    // 1. 畫按鈕
    bool clicked = ImGui::Button(label, size);

    // 2. 取得這個按鈕專屬的狀態 (使用 Map，避免不同按鈕共用一個 static 變數)
    ButtonState& state = m_btnStates[label];

    // 3. 偵測按住狀態
    if (ImGui::IsItemActive()) {
        double held_time = ImGui::GetIO().MouseDownDuration[0];

        // 如果「還沒觸發過」且「時間到了」
        if (held_time > threshold && !state.isLongPressTriggered) {
            if (onLongPress) {
                onLongPress();
            }
            state.isLongPressTriggered = true; // 上鎖
        }
    } else {
        // 手放開了，解鎖
        if (!ImGui::IsMouseDown(0) && !ImGui::IsItemHovered()) {
            state.isLongPressTriggered = false;
        }
    }

    // 4. 短按邏輯 (只有在沒觸發長按的情況下才算短按)
    if (clicked && !state.isLongPressTriggered) {
        if (onShortClick) {
            onShortClick();
        }
    }
}

// =================================================================================
// ★★★ [核心修改] DrawActionButtons：套用 MobileButtonEx ★★★
// =================================================================================
void CimGui::DrawActionButtons(float scale)
{
    float bigSize   = MobileUILayout::BTN_BIG_SIZE;
    float smallSize = MobileUILayout::BTN_SMALL_SIZE;
    float tinySize  = MobileUILayout::BTN_TINY_SIZE;
    float spacing   = MobileUILayout::SPACING;
    ImVec2 areaSize = ImGui::GetWindowSize();

    // 1. 滑動切換邏輯 (Swipe Logic)
    if (ImGui::IsWindowHovered() && ImGui::IsMouseDragging(0, 20.0f))
    {
        ImVec2 delta = ImGui::GetMouseDragDelta(0);
        if (abs(delta.x) > 50.0f && abs(delta.x) > abs(delta.y)) {
            if (delta.x > 0) m_btnPageIndex = 0;
            else             m_btnPageIndex = 1;
            ImGui::ResetMouseDragDelta(0);
        }
    }

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 100.0f);

    // 定義顏色
    ImVec4 col_Orange = ImVec4(0.9f, 0.5f, 0.0f, 0.6f);
    ImVec4 col_Grey   = ImVec4(0.5f, 0.5f, 0.5f, 0.6f);
    ImVec4 col_Red    = ImVec4(0.8f, 0.1f, 0.1f, 0.6f);
    ImVec4 col_Blue   = ImVec4(0.1f, 0.3f, 0.9f, 0.6f);
    ImVec4 col_Green  = ImVec4(0.1f, 0.6f, 0.1f, 0.6f);
    ImVec4 col_Cyan   = ImVec4(0.1f, 0.6f, 0.6f, 0.6f);
    ImVec4 col_Purple = ImVec4(0.6f, 0.2f, 0.8f, 0.6f);

    // 變數設定
    const char *txt_L="", *txt_R="", *txt_HP="", *txt_MP="", *txt_TopL="", *txt_TopR="";
    ImVec4 col_L, col_R, col_HP, col_MP, col_TopL, col_TopR;

    if (m_btnPageIndex == 0) {
        txt_L = "ATK";   col_L = col_Orange;
        txt_R = "Skill"; col_R = col_Grey;
        txt_HP = "HP";   col_HP = col_Red;
        txt_MP = "MP";   col_MP = col_Blue;
        txt_TopL = "Shift"; col_TopL = col_Cyan;
        txt_TopR = "Ctrl"; col_TopR = col_Purple;
    } else {
        txt_L = "F1";  col_L = col_Green;
        txt_R = "F2";   col_R = col_Cyan;
        txt_HP = "F3";  col_HP = col_Grey;
        txt_MP = "F4";  col_MP = col_Grey;
        txt_TopL = "Pick"; col_TopL = col_Green;
        txt_TopR = "Ctrl"; col_TopR = col_Red;
    }

    float row3_Y = areaSize.y - bigSize;
    float row2_Y = areaSize.y - bigSize - spacing - smallSize;
    float row1_Y = row2_Y - spacing - tinySize;
    float offsetSmall = (bigSize - smallSize) * 0.5f;
    float offsetTiny = (bigSize - tinySize) * 0.5f;

    // --- 左下大鈕 (ATK / Pick) ---
    ImGui::SetCursorPos(ImVec2(0, row3_Y));
    ImGui::PushStyleColor(ImGuiCol_Button, col_L);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(col_L.x+0.1f, col_L.y+0.1f, col_L.z+0.1f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(col_L.x-0.1f, col_L.y-0.1f, col_L.z-0.1f, 0.9f));

    // ★ 使用 MobileButtonEx 取代 ImGui::Button
    MobileButtonEx(txt_L, ImVec2(bigSize, bigSize),
            // 短按
                   [&](){
                       if (m_btnPageIndex == 0) Mobile_DoLeftButton();
                       else                     Mobile_SetQuickMemoryByKeyInput(MobileCtrlFlag,SDL_SCANCODE_F1);
                   },
            // 長按
                   [&](){
                       if (m_btnPageIndex == 0) {
                           SDL_Log("ATK Long Pressed - Auto Attack ON");
                           // Hero->AutoAttack = true;
                       } else {
                           SDL_Log("F1 Long Pressed - Setup");
                       }
                   }
    );
    ImGui::PopStyleColor(3);

    // --- 右下大鈕 (Skill / Sit) ---
    ImGui::SetCursorPos(ImVec2(bigSize + spacing, row3_Y));
    ImGui::PushStyleColor(ImGuiCol_Button, col_R);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(col_R.x+0.1f, col_R.y+0.1f, col_R.z+0.1f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(col_R.x-0.1f, col_R.y-0.1f, col_R.z-0.1f, 0.9f));

    // ★ 使用 MobileButtonEx 取代 ImGui::Button
    MobileButtonEx(txt_R, ImVec2(bigSize, bigSize),
            // 短按
                   [&](){
                       if (m_btnPageIndex == 0) Mobile_DoRightButton(0);
                       else                     Mobile_SetQuickMemoryByKeyInput(MobileCtrlFlag, SDL_SCANCODE_F2);
                   },
            // 長按
                   [&](){
                       if (m_btnPageIndex == 0) {
                           SDL_Log("Skill Long Pressed - Select Skill?");
                           Mobile_DoRightButton(1);
                       } else {
                           SDL_Log("F2 Long Pressed - Setup");
                       }
                   }
    );
    ImGui::PopStyleColor(3);

    // --- (以下小按鈕保持原樣，也可以改成 MobileButtonEx 如果需要) ---
    // 左上小鈕 (HP)
    ImGui::SetCursorPos(ImVec2(0 + offsetSmall, row2_Y));
    ImGui::PushStyleColor(ImGuiCol_Button, col_HP);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(col_HP.x+0.1f, col_HP.y+0.1f, col_HP.z+0.1f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(col_HP.x-0.1f, col_HP.y-0.1f, col_HP.z-0.1f, 0.9f));
    if (ImGui::Button(txt_HP, ImVec2(smallSize, smallSize))) {
        if (m_btnPageIndex == 0) Mobile_UsePotion(0);
        else                     Mobile_SetQuickMemoryByKeyInput(MobileCtrlFlag, SDL_SCANCODE_F3);
    }
    ImGui::PopStyleColor(3);

    // 右上小鈕 (MP)
    ImGui::SetCursorPos(ImVec2(bigSize + spacing + offsetSmall, row2_Y));
    ImGui::PushStyleColor(ImGuiCol_Button, col_MP);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(col_MP.x+0.1f, col_MP.y+0.1f, col_MP.z+0.1f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(col_MP.x-0.1f, col_MP.y-0.1f, col_MP.z-0.1f, 0.9f));
    if (ImGui::Button(txt_MP, ImVec2(smallSize, smallSize))) {
        if (m_btnPageIndex == 0) Mobile_UsePotion(1);
        else                     Mobile_SetQuickMemoryByKeyInput(MobileCtrlFlag, SDL_SCANCODE_F4);
    }
    ImGui::PopStyleColor(3);

    // 左上頂端 (Shift/Pick)
    ImGui::SetCursorPos(ImVec2(0 + offsetTiny, row1_Y));
    ImGui::PushStyleColor(ImGuiCol_Button, col_TopL);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(col_TopL.x+0.1f, col_TopL.y+0.1f, col_TopL.z+0.1f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(col_TopL.x-0.1f, col_TopL.y-0.1f, col_TopL.z-0.1f, 0.9f));
    if (ImGui::Button(txt_TopL, ImVec2(tinySize, tinySize))) {
        if (m_btnPageIndex == 0) { if(m_hero) m_hero->DontMoveAttackFlag = !m_hero->DontMoveAttackFlag; }
        else { Mobile_SendPickItem(); }
    }
    ImGui::PopStyleColor(3);

    // 右上頂端 (Ctrl)
    ImGui::SetCursorPos(ImVec2(bigSize + spacing + offsetTiny, row1_Y));
    ImGui::PushStyleColor(ImGuiCol_Button, col_TopR);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(col_TopR.x+0.1f, col_TopR.y+0.1f, col_TopR.z+0.1f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(col_TopR.x-0.1f, col_TopR.y-0.1f, col_TopR.z-0.1f, 0.9f));
    if (MobileCtrlFlag) {
        ImGui::PopStyleColor(3);
        ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.8f, 0.0f, 1.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 1.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.6f, 0.0f, 0.8f, 1.0f));
    }
    if (ImGui::Button("Ctrl", ImVec2(tinySize, tinySize))) {
        CheckPC = MobileCtrlFlag = !MobileCtrlFlag;
        if(m_hero) m_hero->display_item = MobileCtrlFlag;
    }
    ImGui::PopStyleColor(3);

    ImGui::PopStyleVar();

    // 頁面指示器
    ImDrawList* dl = ImGui::GetWindowDrawList();
    ImVec2 winPos = ImGui::GetWindowPos();
    float dotY = winPos.y + areaSize.y - 5.0f;
    float centerX = winPos.x + areaSize.x * 0.5f;
    dl->AddCircleFilled(ImVec2(centerX - 10, dotY), 4.0f, (m_btnPageIndex == 0) ? IM_COL32(255, 255, 255, 200) : IM_COL32(100, 100, 100, 100));
    dl->AddCircleFilled(ImVec2(centerX + 10, dotY), 4.0f, (m_btnPageIndex == 1) ? IM_COL32(255, 255, 255, 200) : IM_COL32(100, 100, 100, 100));
}

void CimGui::DrawMobileUI() {
    if (!m_showMobileUI) return;
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground;

    ImVec4 joy = MobileUILayout::GetJoyRect();
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::SetNextWindowPos(ImVec2(joy.x, joy.y), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(joy.z, joy.w), ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.2f);
    if (ImGui::Begin("JoystickWindow", NULL, flags)) {
        ImVec2 p = ImGui::GetWindowPos();
        ImVec2 center = ImVec2(p.x + joy.z * 0.5f, p.y + joy.w * 0.5f);
        ImGui::GetWindowDrawList()->AddCircleFilled(center, joy.z * 0.5f, IM_COL32(0, 0, 0, 100));
        DrawJoystick(MobileUILayout::SCALE);
    }
    ImGui::End();
    ImGui::PopStyleVar();

    ImVec4 btn = MobileUILayout::GetBtnRect();
    ImGui::SetNextWindowPos(ImVec2(btn.x, btn.y), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(btn.z, btn.w), ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.2f);
    if (ImGui::Begin("ButtonsWindow", NULL, flags)) {
        ImVec2 p = ImGui::GetWindowPos();
        ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + btn.z, p.y + btn.w), IM_COL32(0, 0, 0, 80), 20.0f);
        DrawActionButtons(MobileUILayout::SCALE);
    }
    ImGui::End();
}