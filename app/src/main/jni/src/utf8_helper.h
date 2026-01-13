#pragma once
#include <SDL_ttf.h>
#include <vector>
#include <string>
#include <cstddef>  // For size_t
size_t utf8_char_to_byte_index(const std::string& s, size_t char_index);
extern std::vector<std::string> getDisplayLines(const std::string& text, TTF_Font* font, int maxWidth);
extern std::string utf8_substr(const std::string& s, size_t char_start_index,
                               size_t char_length = -1);
extern int utf8_num_chars(const std::string& s);
extern inline bool is_utf8_start_byte(unsigned char byte);