#include "SimpleUpdater.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <sys/stat.h> // 用於 mkdir
#include <sys/types.h>
#include <errno.h>
#include "SDLRenderer.h"

// 引入 SDL 和 JNI
#include "SDL.h"
#include <jni.h>

// ---------------------------------------------------------
// 輔助函式區
// ---------------------------------------------------------

// ---------------------------------------------------------
// 輔助函式：取得可寫入路徑 (跨平台實作)
// ---------------------------------------------------------
std::string GetWritablePath() {
    std::string pathString;

#ifdef __ANDROID__
    // Android 專用：取得外部儲存空間的 App 私有目錄
    // 路徑類似: /storage/emulated/0/Android/data/com.DragonRefactor.dragonraja/files/
    const char* path = SDL_AndroidGetExternalStoragePath();

    // 如果外部儲存不可用 (極少見)，嘗試內部儲存
    if (path == nullptr) {
        path = SDL_AndroidGetInternalStoragePath();
    }
#else
    // Windows/其他平台：取得使用者設定檔目錄 (AppData)
    // 參數1: 組織名, 參數2: App名
    // 路徑類似: C:\Users\User\AppData\Roaming\DragonRaja\Client\
    char* path = SDL_GetPrefPath("DragonRaja", "Client");
    // SDL_GetPrefPath 會自動分配記憶體，用完雖然要 free，
    // 但因為我們馬上轉成 string 且這函式只會在更新時跑，這裡簡化處理直接轉
#endif

    if (path != nullptr) {
        pathString = path;

        // Windows 的 SDL_GetPrefPath 記得要釋放 (Android 的不用，那是靜態字串)
#ifndef __ANDROID__
        SDL_free((void*)path);
#endif
    } else {
        // 萬一真的失敗，回傳空字串或當前目錄
        pathString = "./";
    }

    // ★★★ 關鍵：確保路徑最後有 "/" ★★★
    if (!pathString.empty() && pathString.back() != '/' && pathString.back() != '\\') {
        pathString += "/";
    }

    return pathString;
}

// 1. JNI 下載函式 (呼叫 Java)
static bool Native_DownloadFile(const std::string& url, const std::string& savePath) {
    JNIEnv* env = (JNIEnv*)SDL_AndroidGetJNIEnv();

    // 1. 直接取得 Activity 的實體 (Object)
    jobject activity = (jobject)SDL_AndroidGetActivity();
    if (!activity) {
        SDL_Log("JNI Error: Could not get SDL Activity object.");
        return false;
    }

    // 2. ★★★ 關鍵修改：直接從實體反查類別 ★★★
    // 這比 FindClass 更安全，因為它不需要打字串，也不受 ClassLoader 限制
    jclass activityClass = env->GetObjectClass(activity);

    if (!activityClass) {
        SDL_Log("JNI Error: Could not get class from Activity object.");
        return false;
    }

    // 3. 找到 downloadFileFromUrl 靜態方法
    jmethodID mid = env->GetStaticMethodID(activityClass, "downloadFileFromUrl", "(Ljava/lang/String;Ljava/lang/String;)Z");

    if (!mid) {
        SDL_Log("JNI Error: Could not find downloadFileFromUrl method.");
        // 記得清理
        env->DeleteLocalRef(activityClass);
        env->DeleteLocalRef(activity);
        return false;
    }

    // 4. 準備參數
    jstring jUrl = env->NewStringUTF(url.c_str());
    jstring jPath = env->NewStringUTF(savePath.c_str());

    // 5. 呼叫
    jboolean result = env->CallStaticBooleanMethod(activityClass, mid, jUrl, jPath);

    // 6. 清理記憶體 (Activity Object 是 Global Ref，這裡可以不用 Delete，但 Class 和 String 必須 Delete)
    env->DeleteLocalRef(jUrl);
    env->DeleteLocalRef(jPath);
    env->DeleteLocalRef(activityClass);
    env->DeleteLocalRef(activity); // SDL_AndroidGetActivity 回傳的是 Global Ref，但在某些 SDL 版本需要手動釋放 Local 引用，加上比較保險

    return (result == JNI_TRUE);
}

// 2. 建立目錄輔助函式 (必須有，不然下載 assets/data/item.tbl 會失敗)
static bool CreateParentDirectories(const std::string& filePath) {
    size_t pos = 0;
    std::string dirPath;
    size_t lastSlash = filePath.find_last_of('/');
    if (lastSlash == std::string::npos) return true;

    std::string path = filePath.substr(0, lastSlash);
    while ((pos = path.find('/', pos)) != std::string::npos) {
        dirPath = path.substr(0, pos++);
        if (dirPath.empty()) continue;
        if (mkdir(dirPath.c_str(), 0777) != 0 && errno != EEXIST) return false;
    }
    if (mkdir(path.c_str(), 0777) != 0 && errno != EEXIST) return false;
    return true;
}

// 3. 讀取整個檔案到 String (用來讀 filelist.txt)
static std::string ReadFileToString(const std::string& path) {
    std::ifstream t(path);
    if (!t.is_open()) return "";
    std::stringstream buffer;
    buffer << t.rdbuf();
    return buffer.str();
}

// 4. 字串切割
static std::vector<std::string> SplitString(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// 5. 取得本地檔案大小
static long GetLocalFileSize(const std::string& path) {
    struct stat stat_buf;
    int rc = stat(path.c_str(), &stat_buf);
    return (rc == 0) ? stat_buf.st_size : -1;
}

// ---------------------------------------------------------
// 類別實作區
// ---------------------------------------------------------

SimpleUpdater::SimpleUpdater(SDL_Renderer* renderer)
        : m_renderer(renderer), m_isDone(false), m_progress(0.0f), m_totalFiles(0), m_downloadedFiles(0)
{
}

SimpleUpdater::~SimpleUpdater() {
}

bool SimpleUpdater::Run() {

    if (CheckAppUpdate()) {
        return false; // 如果需要更新 APK，直接退出 Updater，不進入遊戲
    }

    // 啟動線程
    std::thread worker(&SimpleUpdater::DownloadThreadFunc, this);
    worker.detach();

    SDL_Event e;
    bool running = true;

    while (running && !m_isDone) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) return false;
        }
        DrawUI();
        SDL_Delay(33);
    }
    return true;
}

void SimpleUpdater::DrawUI() {
    // --- A. 清空畫面 --- (保持原樣)
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);

    int w=1280, h=720;
    //SDL_GetRendererOutputSize(m_renderer, &w, &h);

    // --- B. 畫進度條邊框 --- (保持原樣)
    int barW = (int)(w * 0.8f);
    int barH = 50;
    int barX = (w - barW) / 2;
    int barY = (h - barH) / 2;

    SDL_Rect outlineRect = { barX, barY, barW, barH };
    SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(m_renderer, &outlineRect);

    // --- C. 畫綠色進度 --- (保持原樣)
    int padding = 4;
    int progressW = (int)((barW - padding * 2) * m_progress);
    if (progressW > 0) {
        SDL_Rect fillRect = { barX + padding, barY + padding, progressW, barH - padding * 2 };
        SDL_SetRenderDrawColor(m_renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(m_renderer, &fillRect);
    }

    // --- D. 畫文字 "下載中..." (取代原本的黃色方塊) ---

    // 1. 懶加載字體 (只會在第一次執行時載入一次)
    static TTF_Font* s_font = g_SDLInfo.gFont;

    if (s_font) {
        // 2. 製作動態文字：下載中. -> 下載中.. -> 下載中...
        std::string statusText;

        // 判斷狀態：如果總數是 0，代表還在下載清單或比對中，還沒開始下載資源
        if (m_totalFiles == 0) {
            statusText = "正在檢查更新";
        } else {
            // 格式：下載中 (5/100)
            // std::to_string 會把 int 轉成 string
            statusText = "更新檔下載中 (" + std::to_string(m_downloadedFiles) + "/" + std::to_string(m_totalFiles) + ")";
        }

        // 加一點動態點點 (...)
        int dots = (SDL_GetTicks() / 500) % 4;
        for (int i = 0; i < dots; i++) statusText += ".";

        // 3. 渲染文字 (白色)
        SDL_Color color = { 255, 255, 255, 255 };
        SDL_Surface* surface = TTF_RenderUTF8_Blended(s_font, statusText.c_str(), color);

        if (surface) {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface);
            if (texture) {
                // 計算文字位置 (放在進度條上方)
                int textW = surface->w;
                int textH = surface->h;
                SDL_Rect textRect = { barX, barY - textH - 10, textW, textH }; // 進度條上方 10px

                SDL_RenderCopy(m_renderer, texture, NULL, &textRect);

                SDL_DestroyTexture(texture); // 用完馬上丟，防止內存洩漏
            }
            SDL_FreeSurface(surface); // 釋放 Surface
        }
    } else {
        // 如果字體載入失敗 (沒放 font.ttf)，就退回去畫那個黃色方塊
        static int blink = 0;
        blink++;
        if ((blink / 20) % 2 == 0) {
            SDL_Rect dot = { barX + barW + 10, barY + 15, 20, 20 };
            SDL_SetRenderDrawColor(m_renderer, 255, 255, 0, 255);
            SDL_RenderFillRect(m_renderer, &dot);
        }
    }

    SDL_RenderPresent(m_renderer);
}

void SimpleUpdater::DownloadThreadFunc() {

    std::string writablePath = GetWritablePath();

    // 1. 下載 filelist.txt 到暫存檔
    std::string tempFileList = writablePath + "filelist_temp.txt";

    // 使用 JNI 下載
    if (!Native_DownloadFile(baseUrl + "filelist.txt", tempFileList)) {
        SDL_Log("更新清單下載失敗");
        m_isDone = true; // 網路有問題，直接跳過更新進入遊戲
        return;
    }

    // 2. 讀取並解析 filelist
    std::string content = ReadFileToString(tempFileList);

    // ★★★ 新增這段：去除 UTF-8 BOM 頭 ★★★
    // BOM 是三個特定的 Hex: EF BB BF
    if (content.size() >= 3 &&
        (unsigned char)content[0] == 0xEF &&
        (unsigned char)content[1] == 0xBB &&
        (unsigned char)content[2] == 0xBF) {

        // 發現 BOM！把它切掉 (從第 3 個 byte 開始取)
        content = content.substr(3);
        SDL_Log("Detected and removed UTF-8 BOM from filelist.");
    }
    // ★★★ 結束 ★★★

    std::istringstream stream(content);
    std::string line;

    struct DownloadItem {
        std::string url;
        std::string savePath;
        std::string name;
    };
    std::vector<DownloadItem> needDownload;

    while (std::getline(stream, line)) {
        if (line.empty()) continue;
        if (line.back() == '\r') line.pop_back(); // 處理 Windows 換行

        auto parts = SplitString(line, '|');
        if (parts.size() < 2) continue; // 格式錯誤跳過

        std::string relPath = parts[0]; // assets/data/item.tbl
        long remoteSize = std::stol(parts[1]);
        // std::string md5 = parts[2]; // 暫時只比對大小，想更嚴謹可用 MD5

        std::string localPath = writablePath + relPath;

        // 比對邏輯：如果沒檔案 OR 大小不同 => 下載
        if (GetLocalFileSize(localPath) != remoteSize) {
            DownloadItem item;
            item.url = baseUrl + relPath;
            item.savePath = localPath;
            item.name = relPath;
            needDownload.push_back(item);
        }
    }

    // 3. 開始批次下載
    int total = m_totalFiles = (int)needDownload.size();

    if (m_totalFiles == 0) {
        SDL_Log("無需更新");
    } else {
        SDL_Log("發現 %d 個檔案需要更新", total);

        for (const auto& item : needDownload) {
            // ★★★ 重要：建立資料夾 ★★★
            CreateParentDirectories(item.savePath);

            // JNI 下載
            bool success = Native_DownloadFile(item.url, item.savePath);

            if (!success) {
                SDL_Log("下載失敗: %s", item.name.c_str());
                // 這裡可以做失敗重試，或者先忽略
            }

            m_downloadedFiles++;
            m_progress = (float)m_downloadedFiles / (float)m_totalFiles;
        }
    }

    // 清理暫存檔
    remove(tempFileList.c_str());

    m_isDone = true;
}

// ---------------------------------------------------------
// 檢查 APK 主程式是否需要更新
// 回傳 true = 需要更新 (遊戲應停止)
// 回傳 false = 無需更新 (繼續跑資源更新)
// ---------------------------------------------------------
bool SimpleUpdater::CheckAppUpdate() {
    SDL_Log("正在檢查 App 版本...");

    // 1. 下載伺服器上的 app_version.txt
    // 注意：這裡我們偷懶用一下 curl 或者直接讀取
    // 為了方便，我們複用你現有的下載邏輯，先下載到一個暫存檔
    std::string localVersionPath = GetWritablePath() + "app_version_temp.txt";
    std::string remoteUrl = baseUrl + "app_version.txt"; // 假設放在同一層

    // 刪除舊的暫存檔
    remove(localVersionPath.c_str());

    // 呼叫你的 Native_DownloadFile (或 Android_DownloadFile)
    // 這裡要用同步下載 (Blocking)，因為要先檢查完才能繼續
    // 由於你的下載是在 Thread 裡跑的，我們這裡簡單用一個阻塞式下載
    // 如果你之前的下載函式是 Async 的，這裡可能要改一下。
    // 但為了簡單，我們假設你已經有一個簡單的 HTTP GET 函式，或者我們直接用下載隊列的一個變體。

    // 為了最簡單實作，我們這裡發起一個單檔下載請求
    // (這裡假設你之前的 Android_DownloadFile 是阻塞的，或者你有辦法讓它阻塞)
    // 如果沒有，我們這裡手寫一個簡單的 curl 或者利用之前的邏輯

    // ★★★ 為了不改動你現有架構太多，我們用最簡單的 SDL_Net 或直接複用下載邏輯 ★★★
    // 這裡我們假設下載很快，直接用原本的下載函式 (注意：這是在 Main Thread 呼叫還是 Thread?)
    // 最好是在 Run() 的一開始就做。

    if (!Native_DownloadFile(remoteUrl, localVersionPath)) {
        SDL_Log("無法取得伺服器版本資訊，跳過檢查。");
        return false; // 下載失敗，可能是網路問題，讓玩家先進遊戲吧
    }

    // 2. 讀取下載下來的版本號
    std::ifstream file(localVersionPath);
    std::string serverVersionStr;
    if (file.is_open()) {
        std::getline(file, serverVersionStr);
        file.close();
    }

    // 去除可能的換行符號
    serverVersionStr.erase(0, serverVersionStr.find_first_not_of(" \t\n\r"));
    serverVersionStr.erase(serverVersionStr.find_last_not_of(" \t\n\r") + 1);

    if (serverVersionStr.empty()) return false;

    SDL_Log("當前版本: %s | 伺服器版本: %s", CURRENT_APP_VERSION.c_str(), serverVersionStr.c_str());

    // 3. 比對版本 (使用 atof 轉成浮點數比較，簡單粗暴)
    double localVer = std::atof(CURRENT_APP_VERSION.c_str());
    double serverVer = std::atof(serverVersionStr.c_str());

    if (serverVer > localVer) {
        // 4. 需要更新！
        SDL_Log("發現新版本！強制更新。");

        // 彈出提示視窗
        const SDL_MessageBoxButtonData buttons[] = {
                { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "前往下載" },
                { SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 0, "退出" },
        };
        /*const SDL_MessageBoxColorScheme colorScheme = {{
                { 255, 0, 0 }, { 0, 255, 0 }, { 255, 255, 0 }, { 0, 0, 255 }, { 255, 0, 255 }
        }};*/
        const SDL_MessageBoxData messageboxdata = {
                SDL_MESSAGEBOX_INFORMATION,
                NULL,
                "版本更新",
                "發現新的遊戲版本！請前往下載最新安裝包以繼續遊戲。",
                SDL_arraysize(buttons),
                buttons,
                nullptr
        };

        int buttonid;
        if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
            SDL_Log("error displaying message box");
        }

        if (buttonid == 1) {
            // 打開瀏覽器
            SDL_OpenURL(APP_DOWNLOAD_URL.c_str());
        }

        return true; // 告訴主程式需要停止
    }

    return false; // 版本一致，或是 server 版本較舊
}