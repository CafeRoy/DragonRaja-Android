#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <stdexcept>
#include <fstream> // 用於實際的檔案讀取
#include <SDL.h>   // 用於 SDL_Log

// 用於儲存 RGB 顏色的資料結構。
// uint8_t 是一個 8 位元的無號整數，非常適合表示 0-255 的顏色值。

struct Color {
    uint8_t r = 0, g = 0, b = 0;
};

// 用於儲存一段文字及其對應顏色的資料結構。
struct TextSegment {
    std::string text;
    Color color;
};

// 我們將「一行文字」定義為一個或多個 TextSegment 的集合。
// 這允許單行文字擁有多種顏色。
using LineOfText = std::vector<TextSegment>;

using TotalOfLineText = std::vector<LineOfText>;
using RichTextLine = std::vector<TextSegment>;

extern std::vector<RichTextLine> ReadTipsBlock(const std::string& filename, int block_index);
extern std::vector<LineOfText> parser(std::string filename);