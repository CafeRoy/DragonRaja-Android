#ifndef SIMPLE_UPDATER_H
#define SIMPLE_UPDATER_H

#include "SDL.h"
#include <string>
#include <vector>
#include <atomic>
#include <thread>

class SimpleUpdater {
public:
    SimpleUpdater(SDL_Renderer* renderer);
    ~SimpleUpdater();

    std::string baseUrl = "https://www.drefactor.com/packages/";
    //APK 當前版本號
    const std::string CURRENT_APP_VERSION = "1.0";
    // ★★★ 新增：APK 下載頁面 (當需要更新時，會打開這個網頁) ★★★
    const std::string APP_DOWNLOAD_URL = "https://www.drefactor.com/Home/Download";
    // 核心入口：呼叫這個函式，它會卡住主程式，直到更新跑完
    // 回傳 true 代表更新成功/無需更新，可以進入遊戲
    // 回傳 false 代表出錯或玩家取消
    bool Run();

private:
    SDL_Renderer* m_renderer;
    
    // --- 線程溝通變數 (Atomic 保證線程安全) ---
    std::atomic<bool> m_isDone;          // 下載結束沒？
    std::atomic<float> m_progress;       // 當前進度 (0.0 ~ 1.0)
    std::atomic<int> m_totalFiles;       // 總共要下幾個
    std::atomic<int> m_downloadedFiles;  // 已下幾個
    
    // --- 內部函式 ---
    void DrawUI();                       // 畫那個「很簡陋」的介面
    void DownloadThreadFunc();           // 後台下載邏輯

    // 新增檢測函式
    bool CheckAppUpdate();
    
    // 檢查邏輯
    struct UpdateFile {
        std::string name;
        std::string md5;
    };
};

extern std::string GetWritablePath();
#endif