#pragma once
#include <SDL.h>

class MobileInputManager {
public:
    // 取得單例實體
    static MobileInputManager& Instance();

    // 禁止複製
    MobileInputManager(const MobileInputManager&) = delete;
    MobileInputManager& operator=(const MobileInputManager&) = delete;

    // 主要入口：處理所有手機相關事件
    void HandleMobileEvents(SDL_Event* e);

private:
    MobileInputManager(); // 建構子私有化

    // --- 成員變數 ---
    float m_scrollAccumulator = 0.0f;
    const float SCROLL_SENSITIVITY = 20.0f;
    bool m_isRightClickActive = false; // 記錄雙指右鍵是否正在進行

    // --- 內部輔助函式 ---
    void GetFingerPixelPos(float normalizedX, float normalizedY, int& outX, int& outY);
    bool IsTouchInsideChatWindow(int x, int y);
    bool IsTouchOnMagicBar(int x, int y);

    void HandleAppLifecycle(SDL_Event* e);
    void HandleChatScrolling(SDL_Event* e);
    void HandleTwoFingerInput(SDL_Event* e);

    // 輔助發送滑鼠事件
    void SendSimulatedMouseEvent(Uint32 type, Uint8 button, int x, int y);
    void SendSimulatedMouseWheel(int y);
    void SendSimulatedMouseMotion(int x, int y);
};