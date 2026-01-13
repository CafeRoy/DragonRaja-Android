#include "stdafx.h"
#include "FrameMgr.h"
#include "Hong_Sub.h"
#include "dragon.h"

// 確保這些外部變數有在某個地方被定義
CFrameMgr g_FrameMgr;
// --- 建構子 ---
CFrameMgr::CFrameMgr()
{
    // 將所有成員初始化為 0 或安全的值
    m_msPerUpdate = 1000.0 / 17.0; // 給一個預設值
    m_previousTime = 0.0;
    m_timeAccumulator = 0.0;
    m_fDeltaTime = 0.0f;
    m_iRenderFPS_Counter = 0;
    m_iRenderFPS_Display = 0;
    m_dwRenderSecondTimer = 0;
    m_iLogicUPS_Counter = 0;
    m_iLogicUPS_Display = 0;
    m_dwLogicSecondTimer = 0;
    m_frameStartTimestamp = 0.0;
    m_renderFrameDelay = 0.0;
}

// --- 初始化函式 ---
void CFrameMgr::Init(float targetUPS) {
    m_msPerUpdate = 1000.0 / targetUPS;
    m_previousTime = SDL_GetTicks(); // 初始化時間戳
    m_timeAccumulator = 0.0;

    // 初始化 FPS/UPS 計數器
    m_iRenderFPS_Counter = 0;
    m_iRenderFPS_Display = 0;
    m_dwRenderSecondTimer = SDL_GetTicks() + 1000;
    m_iLogicUPS_Counter = 0;
    m_iLogicUPS_Display = 0;
    m_dwLogicSecondTimer = SDL_GetTicks() + 1000;

    last_frame_time = std::chrono::steady_clock::now();
    m_renderFrameDelay = 1000.0 / static_cast<double>(GetMonitorRefreshRate());// 設定一個渲染幀率上限，例如 144 FPS
}

// --- 更新所有計時器 ---
void CFrameMgr::UpdateTimers() { 
    
    // 【新增這一行到函式的最頂部】
    m_frameStartTimestamp = SDL_GetTicks(); // 在每幀開始時記錄時間

    // 1. 核心時間累加邏輯
    double currentTime = SDL_GetTicks();
   
    double elapsed = currentTime - m_previousTime;
    m_previousTime = currentTime;

    // 限制單次時間增量，防止遊戲暫停過久導致「死亡螺旋」
    if (elapsed > 250.0) {
        elapsed = 250.0;
    }

    m_timeAccumulator += elapsed;
    m_fDeltaTime = static_cast<float>(elapsed / 1000.0);
    g_SDLInfo.AllParticelSystemUpdate(m_fDeltaTime); // 如果需要，可以保留

    // 2. 計算顯示用的 FPS (Render FPS)
    ++m_iRenderFPS_Counter;
    if (SDL_GetTicks() > m_dwRenderSecondTimer) {
        m_iRenderFPS_Display = m_iRenderFPS_Counter;
        m_iRenderFPS_Counter = 0;
        m_dwRenderSecondTimer += 1000;
    }
}

// --- 判斷是否應更新邏輯 ---
bool CFrameMgr::ShouldUpdateLogic() {
    if (m_timeAccumulator >= m_msPerUpdate) {
        m_timeAccumulator -= m_msPerUpdate;

        // 計算顯示用的 UPS (Logic UPS)
        ++m_iLogicUPS_Counter;
        if (SDL_GetTicks() > m_dwLogicSecondTimer) {
            m_iLogicUPS_Display = m_iLogicUPS_Counter;
            m_iLogicUPS_Counter = 0;
            m_dwLogicSecondTimer += 1000;
        }

        // 執行你的反作弊檢查
        frameTimecheck();

        return true;
    }
    return false;
}

// --- 【新增這個全新的函式】 ---
void CFrameMgr::LimitRenderFPS()
{
    // 這就是你舊 EndFrame 裡的核心邏輯
    double frameProcessingTime = SDL_GetTicks() - m_frameStartTimestamp;
    if (frameProcessingTime < m_renderFrameDelay)
    {
        // 如果這一幀處理得太快，就手動延時補足剩下的時間
        SDL_Delay(static_cast<uint32_t>(m_renderFrameDelay - frameProcessingTime));
    }
}
int CFrameMgr::GetMonitorRefreshRate() {
    SDL_DisplayMode mode;
    int displayIndex = 0; // 假設你只關心主螢幕 (display index 0)

    // 取得當前顯示器的顯示模式
    if (SDL_GetCurrentDisplayMode(displayIndex, &mode) != 0) {
        // 如果失敗，返回一個預設值（例如 60）
        SDL_Log("Error getting display mode: %s", SDL_GetError());
        return NORMAL_RATE;
    }
    // SDL_DisplayMode 的 refresh_rate 欄位就是刷新率
    return max(mode.refresh_rate, TARGET_RATE);
}

// --- Getter 函式 ---
int32_t CFrameMgr::GetLogicUPS() { return m_iLogicUPS_Display; }
int32_t CFrameMgr::GetRenderFPS() { return m_iRenderFPS_Display; }
float   CFrameMgr::GetDeltaTime() { return m_fDeltaTime; }


// --- 其他輔助函式 (維持不變) ---
bool CFrameMgr::frameTimecheck() {
    auto current_frame_time = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> elapsed_time = current_frame_time - last_frame_time;

    // 只在非偵錯模式 (#ifndef _DEBUG) 下才執行這段檢查
#ifndef _DEBUG
#ifdef _WIN32
    if (elapsed_time.count() > 16000.0) {
        g_DBGLog.Log(LOG_LV1, "Frame time exceeded threshold: %f ms", elapsed_time.count());
        std::quick_exit(0);
    }
#endif
#endif

    last_frame_time = current_frame_time;
    return true;
}

std::string CFrameMgr::PrintRenderInfo()
{
    static SDL_RendererInfo rendererInfo;

	if (rendererInfo.name) return std::string(rendererInfo.name);

    // 呼叫函式，將 g_renderer 的資訊填充到 rendererInfo 中
    if (SDL_GetRendererInfo(g_SDLInfo.renderer, &rendererInfo) == 0)
    {
        // 成功獲取資訊

        // 1. 打印渲染器的名稱 (例如 "direct3d", "opengl", "software")
        SDL_Log("目前使用的渲染器: %s", rendererInfo.name);

        // 2. 檢查是否為硬體加速 (GPU 渲染)
        if (rendererInfo.flags & SDL_RENDERER_ACCELERATED) {
            SDL_Log("渲染方式: 硬體加速 (GPU)");
        }
        else {
            SDL_Log("渲染方式: 軟體渲染 (CPU)");
        }

        // 3. 檢查是否啟用了 VSync
        if (rendererInfo.flags & SDL_RENDERER_PRESENTVSYNC) {
            SDL_Log("垂直同步 (VSync): 已啟用");
        }
        else {
            SDL_Log("垂直同步 (VSync): 未啟用");
        }
        if(rendererInfo.name) return std::string(rendererInfo.name);
    }
    return std::string("");
}