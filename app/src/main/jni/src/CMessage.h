#ifndef CMESSAGE_H_
#define CMESSAGE_H_

#include <string>
#include <unordered_map>
#include <filesystem>

namespace fs = std::filesystem;

class CMessage {
public:
    enum class Language {
        EN, // 英文
        CH  // 中文
    };

    // 公開建構子，可以直接建立物件
    CMessage(Language lang);

    bool loadMessage();

    // 回傳 char*
    char* GetTextMessage(int type, int num);
    void displayAllMessage() const;
private:
    std::unordered_map<int, std::unordered_map<int, std::string>> m_messages;
    Language m_currentLanguage;

    fs::path m_filePath;
    fs::path m_fileName;

    std::string m_buffer;
};
extern CMessage* g_cMsg;
#endif // CMESSAGE_H_