#include "parser.h"
/**
 * @brief 將包含顏色標籤的單行文字解析成多個區段。
 *
 * 這個函式會掃描一行文字，尋找 "<:color:RRRGGGBBB>Some Text" 這樣的模式。
 * 它會找到每個顏色標籤，解析 RGB 值，並提取其後的文字，
 * 直到下一個標籤或行尾為止。
 *
 * @param line 要解析的字串行。
 * @return 一個 std::vector<TextSegment>，代表解析後的一行。每個元素都是一段帶有對應顏色的文字。
 */

LineOfText parseLine(const std::string& line) {
    LineOfText segments;
    std::string::size_type searchPos = 0;

    const std::string tagPrefix = "<:color:";
    const std::string::size_type tagPrefixLen = tagPrefix.length();

    while (searchPos < line.length()) {
        // 尋找下一個顏色標籤的開頭。
        std::string::size_type tagStart = line.find(tagPrefix, searchPos);

        // 如果在行中找不到更多標籤，就完成解析。
        if (tagStart == std::string::npos) {
            break;
        }

        // 尋找標籤的結尾 '>'。
        std::string::size_type tagEnd = line.find('>', tagStart);
        if (tagEnd == std::string::npos) {
            // 標籤格式錯誤 (沒有結尾的 '>'), 停止解析此行。
            break;
        }

        // --- 從標籤中解析顏色 ---
        Color color; // 如果解析失敗，預設為黑色
        std::string::size_type colorStrPos = tagStart + tagPrefixLen;
        std::string colorStr = line.substr(colorStrPos, tagEnd - colorStrPos);

        // 確保顏色字串正好是 9 位數字。
        if (colorStr.length() == 9) {
            try {
                // substr(pos, len) 提取字串的一部分。
                // stoi 將字串轉換為整數。
                int r = std::stoi(colorStr.substr(0, 3));
                int g = std::stoi(colorStr.substr(3, 3));
                int b = std::stoi(colorStr.substr(6, 3));

                // static_cast 安全地將整數轉換為我們的 uint8_t 型別。
                color = { static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b) };
            }
            catch (const std::exception& e) {
                // 如果 stoi 失敗 (例如，包含非數字字元)，我們將使用預設的黑色，並記錄一條日誌。
                SDL_Log(u8"警告：無法解析顏色 '%s'。錯誤: %s", colorStr.c_str(), e.what());
            }
        }

        // --- 提取文字區段 ---
        // 這段顏色的文字從標籤的 '>' 之後開始。
        // 它結束於下一個標籤的開頭，或該行的結尾。
        std::string::size_type nextTagStart = line.find(tagPrefix, tagEnd + 1);

        std::string text;
        if (nextTagStart == std::string::npos) {
            // 這是此行最後一個標籤，所以文字部分延伸到行尾。
            text = line.substr(tagEnd + 1);
        }
        else {
            // 還有下一個標籤，所以文字在我們標籤的結尾和下一個標籤的開頭之間。
            text = line.substr(tagEnd + 1, nextTagStart - (tagEnd + 1));
        }

        // 將解析出的區段加入到此行的列表中。
        segments.push_back({ text, color });

        // 將搜尋位置移動到下一個標籤的開頭以繼續迴圈。
        // 如果沒有更多標籤，searchPos 將變為 std::string::npos，迴圈將結束。
        searchPos = nextTagStart;
    }

    return segments;
}

// 處理輸入流的函式 (來自檔案)
void processStream(TotalOfLineText& Total, std::istream& inputStream, int& line) {
    std::string currentLine;
    int lineNumber = 1;
    
    while (std::getline(inputStream, currentLine)) {
        //SDL_Log("--- 第 %d 行 ---", lineNumber++);

        // 跳過完全空白或只包含空白字元的行。
        if (currentLine.find_first_not_of(" \t\n\v\f\r") == std::string::npos) {
            //SDL_Log("(空白行)");
            continue;
        }

        // 解析當前行。
        LineOfText parsedLine = parseLine(currentLine);

        if (parsedLine.empty()) {
            //SDL_Log("(此行沒有可辨識的顏色標籤和文字)");
        }
        else {
            // 印出此行找到的每個區段。
            for (const auto& segment : parsedLine) {
                //SDL_Log("顏色: %d %d %d", segment.color.r, segment.color.g, segment.color.b);
                // 將文字放在引號中，以便看清前後的空白字元。
                //SDL_Log("文字: %s", segment.text.c_str());
            }
            Total.push_back(parsedLine);
        }

    }
    line = lineNumber;
}

/**
 * @brief 從指定的文字檔中，讀取第 n 個區塊的內容，並解析顏色標籤。
 * @param filename 要讀取的檔案路徑。
 * @param block_index 要讀取的區塊索引 (從 0 開始)。
 * @return 一個包含多行 RichTextLine 的 vector。如果檔案不存在或區塊找不到，會拋出例外。
 */
std::vector<RichTextLine> ReadTipsBlock(const std::string& filename, int block_index)
{
    // --- 步驟 1: 打開檔案 ---
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::string err =  u8"錯誤：無法開啟檔案 " + filename;
        SDL_Log("%s", err.c_str());
        return {};
    }

    std::string line;
    int current_block = 0;

    // --- 步驟 2: 尋找目標區塊的起始位置 ---
    while (std::getline(file, line))
    {
        if (!line.empty() && line[0] == ':') {
            if (current_block == block_index) {
                break; // 找到了！
            }
            current_block++;
        }
    }

    // 如果讀到檔案結尾還沒找到，就拋出例外
    if (current_block != block_index) {
        std::string err = u8"錯誤：在檔案中找不到索引為 " + std::to_string(block_index) + u8" 的區塊。";
		SDL_Log("%s", err.c_str());
        return {};
    }

    // --- 步驟 3: 讀取並解析區塊內容 ---
    std::vector<RichTextLine> result_block;
    Color current_color = { 255, 255, 255 }; // 預設顏色為白色

    while (std::getline(file, line))
    {
        // 如果遇到下一個區塊的分隔符，就結束讀取
        if (!line.empty() && line[0] == ':') {
            break;
        }

        RichTextLine rich_line;
        size_t current_pos = 0;

        // --- 內部迴圈：解析一行中的顏色標籤 ---
        while (current_pos < line.length())
        {
            // 尋找下一個顏色標籤的起始位置
            size_t tag_start = line.find("<:color:", current_pos);

            // 情況 A：如果找不到更多標籤了
            if (tag_start == std::string::npos) {
                // 將從目前位置到行尾的所有文字，作為一個區段
                std::string remaining_text = line.substr(current_pos);
                if (!remaining_text.empty()) {
                    rich_line.push_back({ remaining_text, current_color });
                }
                break; // 結束這一行的解析
            }

            // 情況 B：找到了標籤
            // 1. 先將標籤之前的文字，作為一個區段
            std::string text_before_tag = line.substr(current_pos, tag_start - current_pos);
            if (!text_before_tag.empty()) {
                rich_line.push_back({ text_before_tag, current_color });
            }

            // 2. 解析標籤內的顏色
            size_t color_start = tag_start + 8; // 跳過 "<:color:"
            // 檢查標籤格式是否完整
            if (color_start + 9 < line.length() && line[color_start + 9] == '>') {
                std::string color_str = line.substr(color_start, 9);
                try {
                    int r = std::stoi(color_str.substr(0, 3));
                    int g = std::stoi(color_str.substr(3, 3));
                    int b = std::stoi(color_str.substr(6, 3));
                    current_color = { (uint8_t)r, (uint8_t)g, (uint8_t)b };
                }
                catch (const std::exception&) {
                    // 如果顏色碼格式錯誤，忽略這個標籤
                }
                current_pos = color_start + 10; // 更新目前位置，跳過整個標籤
            }
            else {
                // 如果標籤格式不完整，就把它當作普通文字處理
                current_pos = tag_start + 1;
            }
        }

        if (!rich_line.empty()) {
            result_block.push_back(rich_line);
        }
    }

    file.close();
    return result_block;
}


TotalOfLineText parser(std::string filename)
{
    TotalOfLineText Total;

    Total.clear();
    // --- 從真實檔案讀取 ---
    // 將 "your_file.txt" 替換成您的檔案路徑。
    std::ifstream inputFile(filename);
    if (!inputFile) {

        SDL_Log(u8"錯誤: 無法開啟 '%s'。", filename.c_str());
        return Total; // 返回一個錯誤碼
    }

    SDL_Log(u8"正在從檔案 '%s' 讀取...", filename.c_str());
    int line = 0; 
    processStream(Total, inputFile, line);

    return Total;
}
