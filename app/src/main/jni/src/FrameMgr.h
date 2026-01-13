#pragma once // 防止重複包含

#include <string>
#include <chrono>
#include "SDL.h" // 確保包含了 SDL 標頭
#include "SDL_render.h" // For rendererInfo

class CFrameMgr {
    const int NORMAL_RATE = 100;
    const int TARGET_RATE = 144;
public:
    CFrameMgr();

    // 在遊戲初始化時呼叫一次，設定目標邏輯更新率
    void Init(float targetUPS);

    // 在 GameProc 迴圈開始時呼叫一次，用來更新所有內部計時器
    void UpdateTimers();

    // GameProc 中 while 迴圈的控制器，判斷是否需要執行一次邏輯更新
    bool ShouldUpdateLogic();

    void LimitRenderFPS(); // 【新增】這將是我們新的手動煞車函式

	int GetMonitorRefreshRate();// 【新增】取得顯示器刷新率的函式

    // Getter 函式 (維持不變)
    std::string PrintRenderInfo();
    int32_t GetLogicUPS();
    int32_t GetRenderFPS();
    float GetDeltaTime();

    // 反作弊或超時偵測 (保留您原有的邏輯)
    bool frameTimecheck();
private:
    
    // --- 核心時間控制變數 ---
    double m_msPerUpdate;      // 每個邏輯更新需要的毫秒數 (例如 1000.0 / 17.0)
    double m_previousTime;     // 上一次更新時間的時間戳
    double m_timeAccumulator;  // 時間累加器 (我們的「時間銀行」)
    float  m_fDeltaTime;       // 儲存計算出的真實 deltaTime (單位：秒)

    // --- 用於計算並顯示 FPS 的變數 ---
    int32_t m_iRenderFPS_Counter;
    int32_t m_iRenderFPS_Display;
    uint32_t m_dwRenderSecondTimer;

    // --- 用於計算並顯示 UPS 的變數 ---
    int32_t m_iLogicUPS_Counter;
    int32_t m_iLogicUPS_Display;
    uint32_t m_dwLogicSecondTimer;

    // --- 反作弊相關 ---
    std::chrono::steady_clock::time_point last_frame_time;

    double m_renderFrameDelay; // 每幀渲染所需的最小毫秒延遲
    double m_frameStartTimestamp;    // 每一幀開始時的時間戳
};

// 讓 g_FrameMgr 成為一個可以在專案中使用的全域實例
extern CFrameMgr g_FrameMgr;
extern SDL_RendererInfo rendererInfo; // 確保這個外部變數有被宣告