#include "MobileInputManager.h"
#include "SDLRenderer.h"
#include "Chat.h"
// 請在此處 include 你的全域變數標頭檔，例如:
// #include "GlobalData.h" 
// #include "ChatManager.h"
// #include "Menu.h"

MobileInputManager& MobileInputManager::Instance() {
    static MobileInputManager instance;
    return instance;
}

MobileInputManager::MobileInputManager() {
    m_scrollAccumulator = 0.0f;
    m_isRightClickActive = false;
}

// ---------------------------------------------------------
// 工具函式
// ---------------------------------------------------------
void MobileInputManager::GetFingerPixelPos(float normalizedX, float normalizedY, int& outX, int& outY) {
    int w, h;
    SDL_GetWindowSize(g_SDLInfo.window, &w, &h);
    outX = (int)(normalizedX * w);
    outY = (int)(normalizedY * h);
}

void MobileInputManager::SendSimulatedMouseEvent(Uint32 type, Uint8 button, int x, int y) {
    SDL_Event event = { 0 };
    event.type = type;
    event.button.button = button;
    event.button.state = (type == SDL_MOUSEBUTTONDOWN) ? SDL_PRESSED : SDL_RELEASED;
    event.button.clicks = 1;
    event.button.x = x;
    event.button.y = y;
    MouseProcess(&event); // 呼叫全域的滑鼠處理函式
}

void MobileInputManager::SendSimulatedMouseWheel(int y) {
    // 這裡直接呼叫 ChatMgr，或者構造 SDL 事件
    if (y != 0) {
        ChatMgr.HandleMouseWheel(y);
    }
}

void MobileInputManager::SendSimulatedMouseMotion(int x, int y) {
    SDL_Event event = { 0 };
    event.type = SDL_MOUSEMOTION;
    event.motion.state = SDL_PRESSED;
    event.motion.x = x;
    event.motion.y = y;
    MouseProcess(&event);
}

// ---------------------------------------------------------
// 判定邏輯
// ---------------------------------------------------------
bool MobileInputManager::IsTouchInsideChatWindow(int x, int y) {
    // 範例邏輯，請根據實際情況修改
    SDL_Rect chatRect = { SMenu[MN_CHATTING].x, SMenu[MN_CHATTING].y, 200, 150 };
    return (x >= chatRect.x && x <= chatRect.x + chatRect.w &&
            y >= chatRect.y && y <= chatRect.y + chatRect.h);
}

bool MobileInputManager::IsTouchOnMagicBar(int x, int y) {
    // 範例邏輯
    SDL_Rect magicBarRect = { SMenu[MN_QUICK_MAGIC_PLUS].x, SMenu[MN_QUICK_MAGIC_PLUS].y, 44 * 12, 44 };
    return (x >= magicBarRect.x && x <= magicBarRect.x + magicBarRect.w &&
            y >= magicBarRect.y && y <= magicBarRect.y + magicBarRect.h);
}

// ---------------------------------------------------------
// 生命週期處理
// ---------------------------------------------------------
void MobileInputManager::HandleAppLifecycle(SDL_Event* e) {
    switch (e->type) {
        case SDL_APP_WILLENTERBACKGROUND:
            g_audioManager.SetMasterVolume(0);
            break;
        case SDL_APP_DIDENTERFOREGROUND:
            g_audioManager.SetMasterVolume(-1);
            break;
        case SDL_APP_TERMINATING:
            // SaveMemorize(); // 緊急存檔
            break;
    }
}

// ---------------------------------------------------------
// 聊天室捲動 (單指)
// ---------------------------------------------------------
void MobileInputManager::HandleChatScrolling(SDL_Event* e) {
    switch (e->type) {
        case SDL_FINGERDOWN:
            m_scrollAccumulator = 0.0f;
            break;

        case SDL_FINGERMOTION:
        {
            int x, y;
            GetFingerPixelPos(e->tfinger.x, e->tfinger.y, x, y);

            // 條件：單指 + 在聊天視窗內
            if (SDL_GetNumTouchFingers(e->tfinger.touchId) == 1 &&
                IsTouchInsideChatWindow(x, y))
            {
                m_scrollAccumulator += (e->tfinger.dy * SCROLL_SENSITIVITY);

                // 向上滾動
                while (m_scrollAccumulator >= 1.0f) {
                    SendSimulatedMouseWheel(1);
                    m_scrollAccumulator -= 1.0f;
                }
                // 向下滾動
                while (m_scrollAccumulator <= -1.0f) {
                    SendSimulatedMouseWheel(-1);
                    m_scrollAccumulator += 1.0f;
                }
            }
        }
            break;

        case SDL_FINGERUP:
            m_scrollAccumulator = 0.0f;
            break;
    }
}

// ---------------------------------------------------------
// 雙指輸入 (右鍵模擬)
// ---------------------------------------------------------
void MobileInputManager::HandleTwoFingerInput(SDL_Event* e) {
    int x, y;
    GetFingerPixelPos(e->tfinger.x, e->tfinger.y, x, y);
    int numFingers = SDL_GetNumTouchFingers(e->tfinger.touchId);

    if (e->type == SDL_FINGERDOWN) {
        if (numFingers == 2) {
            // A. UI 操作 (魔法欄) - 點擊 (左鍵)
            if (IsTouchOnMagicBar(x, y)) {
                SendSimulatedMouseEvent(SDL_MOUSEBUTTONDOWN, SDL_BUTTON_LEFT, x, y);
                SendSimulatedMouseEvent(SDL_MOUSEBUTTONUP, SDL_BUTTON_LEFT, x, y);
            }
                // B. 戰場操作 - 模擬右鍵 (支援長按)
            else {
                if (!m_isRightClickActive) {
                    SendSimulatedMouseEvent(SDL_MOUSEBUTTONDOWN, SDL_BUTTON_RIGHT, x, y);
                    m_isRightClickActive = true;
                }
            }
        }
    }
    else if (e->type == SDL_FINGERMOTION) {
        // 如果正在右鍵拖曳 (例如施法瞄準)
        if (m_isRightClickActive) {
            SendSimulatedMouseMotion(x, y);
        }
    }
    else if (e->type == SDL_FINGERUP) {
        // 只要手指離開導致少於2根，且正在右鍵狀態，就彈起
        if (m_isRightClickActive && numFingers < 2) {
            SendSimulatedMouseEvent(SDL_MOUSEBUTTONUP, SDL_BUTTON_RIGHT, x, y);
            m_isRightClickActive = false;
        }
    }
}

// ---------------------------------------------------------
// 公開入口
// ---------------------------------------------------------
void MobileInputManager::HandleMobileEvents(SDL_Event* e) {
    // 1. 生命週期
    HandleAppLifecycle(e);

    // 2. 觸控相關
    switch (e->type) {
        case SDL_FINGERDOWN:
        case SDL_FINGERMOTION:
        case SDL_FINGERUP:
            // A. 聊天室滾動 (單指)
            HandleChatScrolling(e);
            // B. 右鍵模擬 (雙指)
            HandleTwoFingerInput(e);
            break;
    }
}