// ===================================================================================
//
//  【實作教學】裝備染色與材質重塑 - 第 2 步
//
//  說明：
//  這份程式碼將教您如何建立顏色漸層（我們的「調色盤」），以及一個
//  核心的「染色機」函式 (`CreateRecoloredSurface`)。
//  這個函式會接收一個原始的 Surface 和一個調色盤，並返回一個被完全
//  重新上色的、煥然一新的新 Surface。
//
// ===================================================================================
#include <SDL.h>
#include <vector> // 如果需要，可以包含其他標頭檔
#include <array>
#include <algorithm>
#include "SDLRenderer.h"
// -----------------------------------------------------------------------------------
// 1. 建立您的「調色盤」(漸層定義)
// -----------------------------------------------------------------------------------
SDL_Color myColor = {255, 100, 50, 255};  // 一個橘紅色
SDL_Color black = {0, 0, 0, 255};
SDL_Color white = {255, 255, 255, 255};
SDL_Color transparent = {0, 0, 0, 0};  // 注意：透明度設為0
SDL_Color voidColor = {255, 0, 255};

// 使用 std::array 來定義地圖的值
using GradientArray = std::array<SDL_Color, 256>;
std::map<DWORD, GradientArray> g_GradientCache;

// 在您的全域變數區，宣告幾個用來儲存漸層顏色的陣列
// 一個陣列包含 256 種顏色，代表從最暗到最亮的完整顏色方案
GradientArray g_fireGradient;
GradientArray g_iceGradient;
GradientArray g_shadowGradient;  // 我們可以多定義幾種，以備後用
GradientArray g_holyGradient;
GradientArray g_natureGradient;
GradientArray g_lightningGradient;
GradientArray g_poisonGradient;
GradientArray g_voidGradient;


 /**
 * @brief 一個輔助函式，用來生成兩種顏色之間的平滑漸層
 * @param start 漸層的起始顏色 (對應亮度 0)
 * @param end 漸層的結束顏色 (對應亮度 255)
 * @param gradient_array 指向要填充的 SDL_Color[256] 陣列的指標
 */
void CreateGradient(SDL_Color start, SDL_Color end, SDL_Color* gradient_array)
{
    if (!gradient_array) return;

    for (int i = 0; i < 256; ++i) {
        // t 是一個從 0.0 到 1.0 的比例值
        float t = i / 255.0f;

        // 使用線性插值 (Lerp) 來計算中間顏色
        gradient_array[i].r = (Uint8)(start.r * (1.0f - t) + end.r * t);
        gradient_array[i].g = (Uint8)(start.g * (1.0f - t) + end.g * t);
        gradient_array[i].b = (Uint8)(start.b * (1.0f - t) + end.b * t);
        gradient_array[i].a = 255; // Alpha 值設為不透明
    }
}

/**
 * @brief 在遊戲初始化時，呼叫這個函式來生成所有我們需要的漸層
 */
void InitGradients()
{
    // 生成一個從暗紅到亮黃的「獄火」漸層
    CreateGradient({ 80, 10, 0 }, { 255, 255, 150 }, g_fireGradient.data());

    // 生成一個從深藍到亮青的「冰霜」漸層
    CreateGradient({10, 20, 100}, {200, 255, 255}, g_iceGradient.data());

    // 生成一個從深紫到灰白的「暗影」漸層
    CreateGradient({40, 0, 60}, {220, 220, 220}, g_shadowGradient.data());

    //1. 神聖 / 天堂 (Holy / Heavenly)
    CreateGradient({139, 101, 8}, {255, 255, 224}, g_holyGradient.data());

    //自然 / 生命 (Nature / Life)
    CreateGradient({0, 100, 0}, {124, 202, 0}, g_natureGradient.data());

    CreateGradient({25, 25, 112}, {135, 206, 250}, g_lightningGradient.data());

    CreateGradient({85, 107, 47}, {127, 255, 0}, g_poisonGradient.data());

    CreateGradient({75, 0, 130}, {255, 0, 255}, g_voidGradient.data());

    
    g_GradientCache.try_emplace(1, g_fireGradient);
    g_GradientCache.try_emplace(2, g_iceGradient);
    g_GradientCache.try_emplace(3, g_shadowGradient);
    g_GradientCache.try_emplace(4, g_holyGradient);
    g_GradientCache.try_emplace(5, g_natureGradient);
    g_GradientCache.try_emplace(6, g_lightningGradient);
    g_GradientCache.try_emplace(7, g_poisonGradient);
    g_GradientCache.try_emplace(8, g_voidGradient);

}

SDL_Color DWORD_565_To_SDLColor(DWORD color_565) {
  SDL_Color sdl_color;

  // 將 32 位元的 DWORD 轉換為 16 位元的數值
  Uint16 c = (Uint16)color_565;

  // 提取 565 格式的各個分量
  // R: 高 5 位元
  Uint8 r5 = (c >> 11) & 0x1F;
  // G: 中 6 位元
  Uint8 g6 = (c >> 5) & 0x3F;
  // B: 低 5 位元
  Uint8 b5 = c & 0x1F;

  // 將 5 位元擴展到 8 位元 (R 和 B)
  // 透過複製最高位元到最低位元來填補空缺
  sdl_color.r = (r5 << 3) | (r5 >> 2);
  sdl_color.b = (b5 << 3) | (b5 >> 2);

  // 將 6 位元擴展到 8 位元 (G)
  sdl_color.g = (g6 << 2) | (g6 >> 4);

  // 由於 565 格式不包含 Alpha，我們將其設為完全不透明
  sdl_color.a = 255;

  return sdl_color;
}

// 產生一個介於 -offset 和 +offset 之間的隨機數
int getRandomOffset(int offset) { return (rand() % (2 * offset + 1)) - offset; }
// 根據基礎顏色和最大偏移量，產生一個近似顏色
SDL_Color createApproximateColor(SDL_Color baseColor, int maxOffset) {
  SDL_Color approxColor;

  // 對每個顏色分量應用隨機偏移
  int newR = baseColor.r + getRandomOffset(maxOffset);
  int newG = baseColor.g + getRandomOffset(maxOffset);
  int newB = baseColor.b + getRandomOffset(maxOffset);

  // 確保顏色值在 0 到 255 的有效範圍內
  approxColor.r = (Uint8)std::min(255, std::max(0, newR));
  approxColor.g = (Uint8)std::min(255, std::max(0, newG));
  approxColor.b = (Uint8)std::min(255, std::max(0, newB));

  // Alpha 值保持不變
  approxColor.a = baseColor.a;

  return approxColor;
}

SDL_Color* getOrCreategGradient(DWORD color) {
  auto it = g_GradientCache.find(color);

  // 步驟 1: 檢查快取
  if (it != g_GradientCache.end()) {
    // 如果找到了，返回 std::array 的底層資料指標
    return it->second.data();
  }

  // 步驟 2: 如果沒找到，建立一個新的漸層
  GradientArray newGradient{};
  SDL_Color start = DWORD_565_To_SDLColor(color);
  //int offset = 30;
  //offset = std::min((start.r+start.g+start.b)/3, offset);
  //SDL_Color end = createApproximateColor(start, offset); // 允許每個分量有最多 ±30 的偏移
  CreateGradient(start, white, newGradient.data());

  // 步驟 3: 插入並返回新建立的漸層指標
  // try_emplace 會嘗試在 map 中插入新元素
  // 它回傳一個 pair，第一個元素是新元素的迭代器，第二個是 bool
  // 值表示是否成功插入
  auto result = g_GradientCache.try_emplace(color, newGradient);

  // result.first 是指向新元素的迭代器
  // result.first->second 是 std::array
  // 返回其底層資料指標
  return result.first->second.data();
}
 /**
 * @brief 接收一個原始 Surface 和一個漸層，返回一個被重新上色的新 Surface
 * @param original_surface 指向原始裝備圖片的 SDL_Surface
 * @param gradient 指向我們預先定義好的 SDL_Color[256] 漸層陣列
 * @return 一個全新的、已經被重新上色的 SDL_Surface*。失敗則返回 nullptr。
 */
SDL_Surface* CreateRecoloredSurface(SDL_Surface* original_surface, const SDL_Color* gradient)
{
    if (!original_surface || !gradient) return nullptr;

    // 為了不修改原始圖片，我們先建立一個副本
    SDL_Surface* recoloredSurface = SDL_ConvertSurface(original_surface, original_surface->format, 0);
    if (!recoloredSurface) return nullptr;

    // 鎖定 Surface 以便安全地修改像素
    SDL_LockSurface(recoloredSurface);

    // 假設您的遊戲是 16-bit 色彩，如果不是，需要將 Uint16* 改為 Uint32*
    Uint16* pixels = (Uint16*)recoloredSurface->pixels;
    Uint32 transparentColorKey32;
    SDL_GetColorKey(recoloredSurface, &transparentColorKey32);
    Uint16 transparentColorKey16 = (Uint16)transparentColorKey32;

    int pixel_count = recoloredSurface->w * recoloredSurface->h;
    for (int i = 0; i < pixel_count; ++i)
    {
        // 如果當前像素不是透明的
        if (pixels[i] != transparentColorKey16)
        {
            // 1. 分解原始像素的 R, G, B 分量
            Uint8 r, g, b;
            SDL_GetRGB(pixels[i], recoloredSurface->format, &r, &g, &b);

            // 2. 計算該像素的「亮度」(0-255)
            //    這是一個標準的加權平均公式，更符合人眼視覺
            int luminance = (int)(r * 0.299 + g * 0.587 + b * 0.114);
            if (luminance > 255) luminance = 255;
            if (luminance < 0) luminance = 0;

            // 3. 使用亮度作為索引，去我們的「調色盤」中取出新顏色
            const SDL_Color& new_color = gradient[luminance];

            // 4. 將新顏色寫回像素陣列
            pixels[i] = SDL_MapRGB(recoloredSurface->format, new_color.r, new_color.g, new_color.b);
        }
    }

    SDL_UnlockSurface(recoloredSurface);
    return recoloredSurface;
}
