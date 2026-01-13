#include <stdio.h> // For fprintf
#include <stdbool.h> // For bool
#include <vector>
#include <string>
#include "utf8_helper.h"
#include "utf8.h"
#include "SDLRenderer.h"
// --- UTF-8 輔助函數 ---

// 檢查一個字節是否是 UTF-8 多字節字符的開頭
// UTF-8 字符的起始字節不會是 10xxxxxx (即高兩位不是 10)
inline bool is_utf8_start_byte(unsigned char byte) {
    return (byte & 0xC0) != 0x80;
}

// 計算 UTF-8 字串中的字元數量
// 參數: s - 要計算的 UTF-8 字串
// 返回: 字串中實際的 Unicode 字元數量
int utf8_num_chars(const std::string& s) {
    int num_chars = 0;
    for (size_t i = 0; i < s.length(); ) {
        unsigned char c = s[i];
        if ((c & 0x80) == 0x00) { // 1-byte character (0xxxxxxx)
            i += 1;
        }
        else if ((c & 0xE0) == 0xC0) { // 2-byte character (110xxxxx)
            i += 2;
        }
        else if ((c & 0xF0) == 0xE0) { // 3-byte character (1110xxxx)
            i += 3;
        }
        else if ((c & 0xF8) == 0xF0) { // 4-byte character (11110xxx)
            i += 4;
        }
        else { // Invalid UTF-8 sequence, treat as 1 byte to advance
            i += 1;
            // Optionally: SDL_Log("Warning: Invalid UTF-8 byte sequence detected.");
        }
        num_chars++;
    }
    return num_chars;
}

// 將 UTF-8 字元索引轉換為字串中的位元組索引
// 參數: s - UTF-8 字串
// 參數: char_index - 要查找的字元索引 (從 0 開始)
// 返回: 對應字元的位元組索引。如果 char_index 超出範圍，返回字串的位元組長度。
size_t utf8_char_to_byte_index(const std::string& s, size_t char_index) {
    size_t byte_index = 0;
    size_t current_char_count = 0;
    while (byte_index < s.length() && current_char_count < char_index) {
        unsigned char c = s[byte_index];
        if ((c & 0x80) == 0x00) {
            byte_index += 1;
        }
        else if ((c & 0xE0) == 0xC0) {
            byte_index += 2;
        }
        else if ((c & 0xF0) == 0xE0) {
            byte_index += 3;
        }
        else if ((c & 0xF8) == 0xF0) {
            byte_index += 4;
        }
        else { // Invalid, advance by 1 byte
            byte_index += 1;
        }
        current_char_count++;
    }
    return byte_index;
}

// 安全地截取 UTF-8 子字串
// 參數: s - 原始 UTF-8 字串
// 參數: char_start_index - 子字串的起始字元索引 (從 0 開始)
// 參數: char_length - 子字串的字元數量 (-1 表示到字串末尾)
// 返回: 截取後的 UTF-8 子字串
std::string utf8_substr(const std::string& s, size_t char_start_index, size_t char_length) {
    if (char_start_index >= utf8_num_chars(s)) {
        return ""; // Start index out of bounds
    }

    size_t byte_start = utf8_char_to_byte_index(s, char_start_index);

    if (char_length == -1) { // If length is -1, take to the end
        return s.substr(byte_start);
    }
    else {
        size_t byte_end = utf8_char_to_byte_index(s, char_start_index + char_length);
        return s.substr(byte_start, byte_end - byte_start);
    }
}

// 輔助函式：根據換行符和最大寬度分割文本行（處理自動換行）
// font: 用於測量文本寬度的 TTF_Font*
// maxWidth: 文本行的最大像素寬度
std::vector<std::string> getDisplayLines(const std::string& text, TTF_Font* font, int maxWidth) {
    std::vector<std::string> lines;
    size_t currentReadPos = 0; // 當前在原始文本中讀取的位置（字節索引）

    while (currentReadPos < text.length()) {
        std::string remainingSegment = text.substr(currentReadPos);
        size_t nextNewlinePos = remainingSegment.find('\n');

        std::string currentLogicalLine; // 當前正在處理的邏輯行（直到下一個 '\n' 或文本結束）
        if (nextNewlinePos != std::string::npos) {
            currentLogicalLine = remainingSegment.substr(0, nextNewlinePos);
        }
        else {
            currentLogicalLine = remainingSegment;
        }

        // 對這個邏輯行進行自動換行處理
        std::string textToWrap = currentLogicalLine;
        size_t wrapReadPos = 0; // 在 textToWrap 中的讀取位置（字符索引）

        while (wrapReadPos < utf8_num_chars(textToWrap)) {
            int bestWrapCharCount = 0;

            for (int i = 1; i <= utf8_num_chars(textToWrap) - wrapReadPos; ++i) {
                std::string segment = utf8_substr(textToWrap, wrapReadPos, i);
                int segmentWidth = 0;
                TTF_SizeUTF8_Safe(font, segment.c_str(), &segmentWidth, NULL);

                if (segmentWidth > maxWidth) {
                    bestWrapCharCount = i - 1;
                    break;
                }
                bestWrapCharCount = i;
            }

            if (bestWrapCharCount == 0 && utf8_num_chars(textToWrap) - wrapReadPos > 0) {
                bestWrapCharCount = 1;
            }

            std::string displayLine = utf8_substr(textToWrap, wrapReadPos, bestWrapCharCount);
            lines.push_back(displayLine);

            wrapReadPos += bestWrapCharCount;
        }

        // 更新 currentReadPos 到下一個邏輯行的開始
        if (nextNewlinePos != std::string::npos) {
            currentReadPos += nextNewlinePos + 1; // 跳過當前邏輯行和 '\n'
        }
        else {
            currentReadPos = text.length(); // 已處理到文本結束
        }
    }

    // 確保即使文本為空，也至少有一行
    if (lines.empty()) {
        lines.push_back("");
    }

    return lines;
}


// 檢查一個 Unicode 字元是否為中文字元、英文字母或數字
bool is_valid_char(unsigned int codepoint) {
  // 檢查是否為英文字母或數字 (ASCII 範圍)
  if (codepoint >= 0x00 && codepoint <= 0x7F) {
    return isalnum(static_cast<char>(codepoint));
  }

  // 檢查是否為中文 (CJK 統一漢字區塊)
  // 這裡只檢查最常見的範圍，實際完整的中文範圍更廣
  if (codepoint >= 0x4E00 && codepoint <= 0x9FFF) {
    return true;
  }

  // 如果字元不在上述任何範圍，則無效
  return false;
}

// 驗證整個 UTF-8 字串
bool is_valid_utf8_string(const std::string& str) {
  try {
    // 使用 utfcpp 函式來遍歷字串
    std::string::const_iterator end_it = str.end();
    std::string::const_iterator it = str.begin();

    while (it != end_it) {
      // 安全地獲取下一個字元 (Unicode Code Point)
      unsigned int codepoint = utf8::next(it, end_it);

      // 檢查這個字元是否符合我們的要求
      if (!is_valid_char(codepoint)) {
        return false;  // 發現無效字元，返回 false
      }
    }
  } catch (const utf8::exception& e) {
    // 如果字串本身不是有效的 UTF-8 編碼，也視為無效
    SDL_Log("Invalid UTF-8 sequence: %s" , e.what());
    return false;
  }

  return true;  // 所有字元都有效，返回 true
}