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
static bool Android_DownloadFile(const std::string& url, const std::string& savePath) {
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
    const std::string CURRENT_APP_VERSION = "1.0";

    std::string writablePath = GetWritablePath();
    std::string baseUrl = "https://www.drefactor.com/packages/";

    // 1. 下載 filelist.txt 到暫存檔
    std::string tempFileList = writablePath + "filelist_temp.txt";

    // 使用 JNI 下載
    if (!Android_DownloadFile(baseUrl + "filelist.txt", tempFileList)) {
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
            bool success = Android_DownloadFile(item.url, item.savePath);

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