#include "StdAfx.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "CMessage.h"
#include "Debuglog.h"
#include "SDLRenderer.h"
CMessage* g_cMsg = nullptr;

const std::unordered_map<CMessage::Language, std::string> languageMap = {
    {CMessage::Language::EN, "en"},
    {CMessage::Language::CH, "ch"}
};

CMessage::CMessage(Language lang) : m_currentLanguage(lang) {
    auto it = languageMap.find(m_currentLanguage);
    if (it != languageMap.end()) {
        m_fileName = it->second + "_msg.txt";
        m_filePath = "data" / m_fileName;

        loadMessage();
    }
}

bool CMessage::loadMessage() {
    // 1. 改用 SDL_RWops 開啟檔案 (讀取 APK 內的 assets)
// 注意：Android 路徑不包含 "./"，請確保 m_filePath 是相對路徑 (例如 "Data/msg.txt")
    SDL_RWops* rw = SDL_RWFromFile(m_filePath.c_str(), "rb");

    if (rw == NULL) {
        // 改用 SDL_Log 這樣在 Android Logcat 才看得到錯誤
        SDL_Log("Error: Unable to open file %s. SDL Error: %s", m_filePath.c_str(), SDL_GetError());
        return false;
    }

// 2. 取得檔案大小並讀取內容
    Sint64 fileSize = SDL_RWsize(rw);
    std::string fileContent;

    if (fileSize > 0) {
        fileContent.resize(fileSize);
        SDL_RWread(rw, &fileContent[0], fileSize, 1);
    } else {
        // 檔案是空的或讀取大小失敗
        SDL_RWclose(rw);
        return false;
    }

// 3. 關閉檔案指標
    SDL_RWclose(rw);

// 4. 將讀入的記憶體轉換成 stringstream
// 命名為 'file'，這樣下面原本的程式碼幾乎不用動
    std::stringstream file(fileContent);

// -----------------------------------------------------------
// 以下邏輯保持原樣 (除了把 cerr 改成 SDL_Log 方便除錯)
// -----------------------------------------------------------

    m_messages.clear();

    std::string line;
    int currentType = -1;

    while (std::getline(file, line)) {
        // 移除 Windows 可能殘留的 CR 符號 (\r)
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        if (line.empty()) {
            continue;
        }

        if (line.front() == '[' && line.back() == ']') {
            std::string typeStr = line.substr(1, line.length() - 2);
            try {
                currentType = std::stoi(typeStr);
            }
            catch (const std::exception& e) {
                // 建議把 g_DBGLog 保留，或者也可以用 SDL_Log
                g_DBGLog.Log(LOG_LV1, "Warning: Invalid section header format in file %s ", m_filePath.c_str());
                currentType = -1;
            }
        }
        else if (currentType != -1) {
            size_t colonPos = line.find(':');
            if (colonPos != std::string::npos) {
                std::string numStr = line.substr(0, colonPos);
                std::string msgStr = line.substr(colonPos + 1);

                try {
                    int num = std::stoi(numStr);
                    m_messages[currentType][num] = msgStr;
                }
                catch (const std::exception& e) {
                    g_DBGLog.Log(LOG_LV1, "Warning: Invalid message format in file %s ", m_filePath.c_str());
                }
            }
        }
    }

// 這裡不需要 file.close()，因為 stringstream 會自動釋放
    return !m_messages.empty();
}

// 關鍵修改：回傳 char*，並使用 std::string 的 .data() 方法來取得可修改的指標
char* CMessage::GetTextMessage(int type, int num) {
    auto it_type = m_messages.find(type);
    if (it_type != m_messages.end()) {
        auto it_num = it_type->second.find(num);
        if (it_num != it_type->second.end()) {
            m_buffer = it_num->second; // 複製到內部緩衝區
            return const_cast<char *>(m_buffer.data()); // 回傳緩衝區的非 const 指標
        }
    }

    m_buffer.clear(); // 找不到訊息時清空緩衝區
    return const_cast<char *>(m_buffer.data());
}

// 實作 displayAllMessage 函式
void CMessage::displayAllMessage() const {
    if (m_messages.empty()) {
        // 使用 SDL_Log 替換 std::cout
        SDL_Log("No messages loaded.");
        return;
    }

    // 使用 SDL_Log 替換 std::cout
    SDL_Log("--- Loaded Messages from %s ---", m_filePath.string().c_str());
    for (const auto& pair_type : m_messages) {
        int type = pair_type.first;
        const auto& map_num = pair_type.second;

        // 使用 SDL_Log 替換 std::cout
        SDL_Log("Type [%d]:", type);

        for (const auto& pair_num : map_num) {
            int num = pair_num.first;
            const std::string& msg = pair_num.second;
            // 使用 SDL_Log 替換 std::cout
            SDL_Log("  - Number %d: %s", num, msg.c_str());
        }
    }
    // 使用 SDL_Log 替換 std::cout
    SDL_Log("--- End of Messages ---");
}