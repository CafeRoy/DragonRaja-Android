#include <cstdint>
#include <algorithm> // 用於 std::min 和 std::max
#include <functional>
/**
 * @file rendering_functions_cpp_final.h
 * @brief C++ 渲染函式庫 (完整最終版 V8)
 *
 * 這個版本包含了所有原始組合語言繪圖函式的 C++ 替代品，
 * 修正了所有已知的座標計算、裁剪邏輯和顏色混合演算法錯誤。
 * 現在它是一個功能完整的、可用於生產的渲染函式庫。
 */


extern uint16_t	SCREEN_WIDTH;
extern uint16_t SCREEN_HEIGHT;
extern uint16_t  GAME_SCREEN_XSIZE;
extern uint16_t  GAME_SCREEN_YSIZE;

#ifndef TILE_SIZE
#define TILE_SIZE 32 // 假設一個圖塊是 32x32 像素
#endif

struct Spr {
    short xl, yl;
    short ox, oy;
    unsigned int size;
    char* img;
};
// --- 核心繪圖引擎 (C++ 版本) ---

template<typename BlendOp>
void PutCmprsImgCliping_Generic_CPP(int x, int y, Spr* sp, void* dest, int destPitch, int lx, int rx, int ty, int by, BlendOp blend_op)
{
    if (!sp || !sp->img || by <= 0 || sp->size <= 0) return;

    const uint8_t* src_base = (const uint8_t*)sp->img; // 【記錄基底指標】
    const uint8_t* src_ptr = (const uint8_t*)sp->img;
    uint8_t* dest_base_ptr = (uint8_t*)dest;

    for (int i = 0; i < ty; ++i) {

        // 檢查 1: 在讀取 0xFFFF 之前，確保 src_ptr 仍在邊界內
        if ((src_ptr - src_base) + sizeof(uint16_t) > sp->size) {
            // LogError("RLE row skip error: Pointer out of bounds before 0xFFFF check. sp=%p", sp);
            return; // 數據損毀，放棄繪製
        }

        while (*reinterpret_cast<const uint16_t*>(src_ptr) != 0xFFFF) {
            // 檢查 2: 在讀取 skip/copy header (4 bytes) 之前，確保 src_ptr 仍在邊界內
            if ((src_ptr - src_base) + 4 > sp->size) {
                // LogError("RLE row skip error: Pointer out of bounds before reading header. sp=%p", sp);
                return; // 數據損毀，放棄繪製
            }
            uint16_t copy_bytes = *reinterpret_cast<const uint16_t*>(src_ptr + 2);

            // 檢查 3: 驗證 copy_bytes 的值是否會導致下一次讀取溢位
            // (我們甚至可以加入 copy_bytes > destPitch 的檢查，但邊界檢查更根本)
            if ((src_ptr - src_base) + 4 + copy_bytes > sp->size) {
                // LogError("RLE row skip error: Corrupted copy_bytes value (%u). sp=%p", (unsigned int)copy_bytes, sp);
                return; // 數據損毀，放棄繪製
            }
            src_ptr += 4 + copy_bytes; // 安全地移動指標

            // 檢查 4: 移動後，再次檢查是否溢位 (為了下一次 while 迴圈的 0xFFFF 檢查)
            if ((src_ptr - src_base) + sizeof(uint16_t) > sp->size) {
                // LogError("RLE row skip error: Pointer out of bounds after advancing. sp=%p", sp);
                return; // 數據損毀，放棄繪製
            }
            //src_ptr += 4 + *reinterpret_cast<const uint16_t*>(src_ptr + 2);
        }
        src_ptr += 2;
    }
    int lx_bytes = lx * 2;
    int rx_bytes = (lx + rx) * 2;
    for (int i = 0; i < by; ++i) {
        int dest_y = y + i;
        uint8_t* dest_screen_line_start = dest_base_ptr + dest_y * destPitch;
        int current_x_bytes_in_sprite = 0;
        while (true) {
            uint16_t skip_bytes = *reinterpret_cast<const uint16_t*>(src_ptr);
            if (skip_bytes == 0xFFFF) { src_ptr += 2; break; }
            uint16_t copy_bytes = *reinterpret_cast<const uint16_t*>(src_ptr + 2);

            // --- 最佳防護 (如果你有 sp->size) ---
            // 理想情況下，你還應該用 sp->size 來檢查
            if (((const uint8_t*)src_ptr - (const uint8_t*)sp->img) + 4 + copy_bytes > sp->size)
            {
                // LogError("Sprite RLE data exceeds buffer size!");
                return;
            }
            // --- 【防護罩結束】---

            src_ptr += 4;
            int block_start_byte = current_x_bytes_in_sprite + skip_bytes;
            int block_end_byte = block_start_byte + copy_bytes;
            int draw_start_byte_in_sprite = std::max(block_start_byte, lx_bytes);
            int draw_end_byte_in_sprite = std::min(block_end_byte, rx_bytes);
            if (draw_start_byte_in_sprite < draw_end_byte_in_sprite) {
                int dest_start_byte_on_screen = x * 2 + (draw_start_byte_in_sprite - lx_bytes);
                int src_offset_in_block = draw_start_byte_in_sprite - block_start_byte;
                int bytes_to_draw = draw_end_byte_in_sprite - draw_start_byte_in_sprite;
                const uint8_t* s = src_ptr + src_offset_in_block;
                uint8_t* d = dest_screen_line_start + dest_start_byte_on_screen;
                for (int k = 0; k < bytes_to_draw; k += 2) {
                    uint16_t s_pixel = *reinterpret_cast<const uint16_t*>(s + k);
                    uint16_t* d_pixel_ptr = reinterpret_cast<uint16_t*>(d + k);
                    blend_op(s_pixel, d_pixel_ptr);
                }
            }
            src_ptr += copy_bytes;
            current_x_bytes_in_sprite += skip_bytes + copy_bytes;
        }
    }
}

template<typename BlendOp>
void PutCmprsImgNOTCliping_Generic_CPP(int x, int y, Spr* sp, void* dest, int destPitch, BlendOp blend_op)
{
    if (!sp || !sp->img || sp->yl <= 0 || sp->size <=0) return;
    const uint8_t* src_ptr = (const uint8_t*)sp->img;
    uint8_t* dest_base_ptr = (uint8_t*)dest;
    for (int i = 0; i < sp->yl; ++i) {
        uint8_t* dest_draw_ptr = dest_base_ptr + (y + i) * destPitch + x * 2;
        while (true) {
            uint16_t skip_bytes = *reinterpret_cast<const uint16_t*>(src_ptr);
            if (skip_bytes == 0xFFFF) { src_ptr += 2; break; }
            uint16_t copy_bytes = *reinterpret_cast<const uint16_t*>(src_ptr + 2);
            // --- 【防護罩 V2.1 - 修正版】---
            // 檢查1：copy_bytes 是否是一個「合理」的值？
            // 我們使用 destPitch (螢幕一行的總位元組) 作為最合理的上限。
            // 一個 RLE 片段的位元組長度(copy_bytes) 絕不應該
            // 超過螢幕一整行的位元組長度(destPitch)。

            // (copy_bytes 是 uint16_t (0-65535)，destPitch 是 int (e.g., 3840)，
            //  這樣比較是安全且有效的)
            if (copy_bytes > destPitch)
            {
                // 偵測到嚴重的資料損毀！
                // 這幾乎可以 100% 確定是由懸空指標引起的。
                // LogError("Sprite data corruption! sp=%p, copy_bytes=%u, destPitch=%d", 
                //          sp, (unsigned int)copy_bytes, destPitch);
                return; // 直接放棄繪製這個 Sprite，函式返回，防止崩潰。
            }

            // --- 最佳防護 (如果你有 sp->size) ---
            // 理想情況下，你還應該用 sp->size 來檢查
            if ( ((const uint8_t*)src_ptr - (const uint8_t*)sp->img) + 4 + copy_bytes > sp->size )
            {
                 // LogError("Sprite RLE data exceeds buffer size!");
                 return;
            }
            // --- 【防護罩結束】---
            src_ptr += 4;
            dest_draw_ptr += skip_bytes;
            for (int k = 0; k < copy_bytes; k += 2) {
                uint16_t s_pixel = *reinterpret_cast<const uint16_t*>(src_ptr + k);
                uint16_t* d_pixel_ptr = reinterpret_cast<uint16_t*>(dest_draw_ptr + k);
                blend_op(s_pixel, d_pixel_ptr);
            }
            src_ptr += copy_bytes;
            dest_draw_ptr += copy_bytes;
        }
    }
}

// ======================================================================
// 繪圖函式實現
// ======================================================================

// --- Opaque (不透明直接複製) ---
void PutCmprsImgCliping_CPP(int x, int y, Spr* sp, void* dest, int destPitch, int lx, int rx, int ty, int by) {
    PutCmprsImgCliping_Generic_CPP(x, y, sp, dest, destPitch, lx, rx, ty, by,
        [](uint16_t s_p, uint16_t* d_p_ptr) { *d_p_ptr = s_p; });
}
void PutCmprsImgNOTCliping_CPP(int x, int y, Spr* sp, void* dest, int destPitch) {
    PutCmprsImgNOTCliping_Generic_CPP(x, y, sp, dest, destPitch,
        [](uint16_t s_p, uint16_t* d_p_ptr) { *d_p_ptr = s_p; });
}

// --- OneColor (單色填充) ---
void PutCmprsImgClipingOneColor_CPP(int x, int y, Spr* sp, void* dest, int destPitch, int lx, int rx, int ty, int by, uint16_t color) {
    PutCmprsImgCliping_Generic_CPP(x, y, sp, dest, destPitch, lx, rx, ty, by,
        [=](uint16_t, uint16_t* d_p_ptr) { *d_p_ptr = color; });
}
void PutCmprsImgNOTClipingOneColor_CPP(int x, int y, Spr* sp, void* dest, int destPitch, uint16_t color) {
    PutCmprsImgNOTCliping_Generic_CPP(x, y, sp, dest, destPitch,
        [=](uint16_t, uint16_t* d_p_ptr) { *d_p_ptr = color; });
}

// --- Void (像素左移特效) ---
void PutCmprsImgVoidCliping_CPP(int x, int y, Spr* sp, void* dest, int destPitch, int lx, int rx, int ty, int by) {
    PutCmprsImgCliping_Generic_CPP(x, y, sp, dest, destPitch, lx, rx, ty, by,
        [](uint16_t, uint16_t* d_p_ptr) { *(d_p_ptr - 1) = *d_p_ptr; });
}
void PutCmprsImgVoidNOTCliping_CPP(int x, int y, Spr* sp, void* dest, int destPitch) {
    PutCmprsImgNOTCliping_Generic_CPP(x, y, sp, dest, destPitch,
        [](uint16_t, uint16_t* d_p_ptr) { *(d_p_ptr - 1) = *d_p_ptr; });
}

// --- Trans (特殊半透明) ---
void PutCmprsImgClipingTrans565_CPP(int x, int y, Spr* sp, void* dest, int destPitch, int lx, int rx, int ty, int by, uint16_t MSK) {
    PutCmprsImgCliping_Generic_CPP(x, y, sp, dest, destPitch, lx, rx, ty, by,
        [](uint16_t s_p, uint16_t* d_p_ptr) {
            *d_p_ptr = (((s_p & 0xF7DE) + (*d_p_ptr & 0xF7DE)) >> 1);
        });
}
void PutCmprsImgNOTClipingTrans565_CPP(int x, int y, Spr* sp, void* dest, int destPitch, uint16_t MSK) {
    PutCmprsImgNOTCliping_Generic_CPP(x, y, sp, dest, destPitch,
        [](uint16_t s_p, uint16_t* d_p_ptr) {
            *d_p_ptr = (((s_p & 0xE71C) + (*d_p_ptr & 0xE71C)) >> 2);
        });
}

// --- Blend (標準 Alpha 混合) ---
void PutCmprsImgClipingBlend565_CPP(int x, int y, Spr* sp, void* dest, int destPitch, int lx, int rx, int ty, int by, int alpha) {
    alpha = std::min(32, std::max(0, alpha)); const int inv_alpha = 32 - alpha;
    PutCmprsImgCliping_Generic_CPP(x, y, sp, dest, destPitch, lx, rx, ty, by,
        [=](uint16_t s_p, uint16_t* d_p_ptr) {
            int s_r = (s_p >> 11) & 0x1F, s_g = (s_p >> 5) & 0x3F, s_b = s_p & 0x1F;
            int d_r = (*d_p_ptr >> 11) & 0x1F, d_g = (*d_p_ptr >> 5) & 0x3F, d_b = *d_p_ptr & 0x1F;
            *d_p_ptr = (((s_r * alpha + d_r * inv_alpha) >> 5) << 11) | (((s_g * alpha + d_g * inv_alpha) >> 5) << 5) | ((s_b * alpha + d_b * inv_alpha) >> 5);
        });
}
void PutCmprsImgNOTClipingBlend565_CPP(int x, int y, Spr* sp, void* dest, int destPitch, int alpha) {
    alpha = std::min(32, std::max(0, alpha)); const int inv_alpha = 32 - alpha;
    PutCmprsImgNOTCliping_Generic_CPP(x, y, sp, dest, destPitch,
        [=](uint16_t s_p, uint16_t* d_p_ptr) {
            int s_r = (s_p >> 11) & 0x1F, s_g = (s_p >> 5) & 0x3F, s_b = s_p & 0x1F;
            int d_r = (*d_p_ptr >> 11) & 0x1F, d_g = (*d_p_ptr >> 5) & 0x3F, d_b = *d_p_ptr & 0x1F;
            *d_p_ptr = (((s_r * alpha + d_r * inv_alpha) >> 5) << 11) | (((s_g * alpha + d_g * inv_alpha) >> 5) << 5) | ((s_b * alpha + d_b * inv_alpha) >> 5);
        });
}

// --- Add (加色混合) ---
void PutCmprsImgClipingAdd565_CPP(int x, int y, Spr* sp, void* dest, int destPitch, int lx, int rx, int ty, int by, int alpha) {
    PutCmprsImgCliping_Generic_CPP(x, y, sp, dest, destPitch, lx, rx, ty, by,
        [=](uint16_t s_p, uint16_t* d_p_ptr) {
            int s_r = (s_p >> 11) & 0x1F, s_g = (s_p >> 5) & 0x3F, s_b = s_p & 0x1F;
            int d_r = (*d_p_ptr >> 11) & 0x1F, d_g = (*d_p_ptr >> 5) & 0x3F, d_b = *d_p_ptr & 0x1F;
            s_r = (s_r * alpha) >> 4; s_g = (s_g * alpha) >> 4; s_b = (s_b * alpha) >> 4;
            int r = s_r + d_r, g = s_g + d_g, b = s_b + d_b;
            if (r > 31) r = 31; if (g > 63) g = 63; if (b > 31) b = 31;
            *d_p_ptr = (r << 11) | (g << 5) | b;
        });
}
void PutCmprsImgNOTClipingAdd565_CPP(int x, int y, Spr* sp, void* dest, int destPitch, int alpha) {
    PutCmprsImgNOTCliping_Generic_CPP(x, y, sp, dest, destPitch,
        [=](uint16_t s_p, uint16_t* d_p_ptr) {
            int s_r = (s_p >> 11) & 0x1F, s_g = (s_p >> 5) & 0x3F, s_b = s_p & 0x1F;
            int d_r = (*d_p_ptr >> 11) & 0x1F, d_g = (*d_p_ptr >> 5) & 0x3F, d_b = *d_p_ptr & 0x1F;
            s_r = (s_r * alpha) >> 4; s_g = (s_g * alpha) >> 4; s_b = (s_b * alpha) >> 4;
            int r = s_r + d_r, g = s_g + d_g, b = s_b + d_b;
            if (r > 31) r = 31; if (g > 63) g = 63; if (b > 31) b = 31;
            *d_p_ptr = (r << 11) | (g << 5) | b;
        });
}

// --- Sub (減色混合) ---
void PutCmprsImgClipingSub565_CPP(int x, int y, Spr* sp, void* dest, int destPitch, int lx, int rx, int ty, int by, int alpha) {
    PutCmprsImgCliping_Generic_CPP(x, y, sp, dest, destPitch, lx, rx, ty, by,
        [=](uint16_t s_p, uint16_t* d_p_ptr) {
            int s_r = (s_p >> 11) & 0x1F, s_g = (s_p >> 5) & 0x3F, s_b = s_p & 0x1F;
            int d_r = (*d_p_ptr >> 11) & 0x1F, d_g = (*d_p_ptr >> 5) & 0x3F, d_b = *d_p_ptr & 0x1F;
            s_r = (s_r * alpha) >> 4; s_g = (s_g * alpha) >> 4; s_b = (s_b * alpha) >> 4;
            int r = d_r - s_r, g = d_g - s_g, b = d_b - s_b;
            if (r < 0) r = 0; if (g < 0) g = 0; if (b < 0) b = 0;
            *d_p_ptr = (r << 11) | (g << 5) | b;
        });
}
void PutCmprsImgNOTClipingSub565_CPP(int x, int y, Spr* sp, void* dest, int destPitch, int alpha) {
    PutCmprsImgNOTCliping_Generic_CPP(x, y, sp, dest, destPitch,
        [=](uint16_t s_p, uint16_t* d_p_ptr) {
            int s_r = (s_p >> 11) & 0x1F, s_g = (s_p >> 5) & 0x3F, s_b = s_p & 0x1F;
            int d_r = (*d_p_ptr >> 11) & 0x1F, d_g = (*d_p_ptr >> 5) & 0x3F, d_b = *d_p_ptr & 0x1F;
            s_r = (s_r * alpha) >> 4; s_g = (s_g * alpha) >> 4; s_b = (s_b * alpha) >> 4;
            int r = d_r - s_r, g = d_g - s_g, b = d_b - s_b;
            if (r < 0) r = 0; if (g < 0) g = 0; if (b < 0) b = 0;
            *d_p_ptr = (r << 11) | (g << 5) | b;
        });
}

// --- BlendRGB (顏色過濾) ---
void PutCmprsImgClipingBlendRGB565_CPP(int x, int y, Spr* sp, void* dest, int destPitch, int lx, int rx, int ty, int by, uint32_t RGB) {
    int f_r = (RGB >> 16) & 0xFF, f_g = (RGB >> 8) & 0xFF, f_b = RGB & 0xFF;
    PutCmprsImgCliping_Generic_CPP(x, y, sp, dest, destPitch, lx, rx, ty, by,
        [=](uint16_t s_p, uint16_t* d_p_ptr) {
            int s_r = (s_p >> 11) & 0x1F, s_g = (s_p >> 5) & 0x3F, s_b = s_p & 0x1F;
            *d_p_ptr = ((s_r & (f_r >> 3)) << 11) | ((s_g & (f_g >> 2)) << 5) | (s_b & (f_b >> 3));
        });
}
void PutCmprsImgNOTClipingBlendRGB565_CPP(int x, int y, Spr* sp, void* dest, int destPitch, uint32_t RGB) {
    int f_r = (RGB >> 16) & 0xFF, f_g = (RGB >> 8) & 0xFF, f_b = RGB & 0xFF;
    PutCmprsImgNOTCliping_Generic_CPP(x, y, sp, dest, destPitch,
        [=](uint16_t s_p, uint16_t* d_p_ptr) {
            int s_r = (s_p >> 11) & 0x1F, s_g = (s_p >> 5) & 0x3F, s_b = s_p & 0x1F;
            *d_p_ptr = ((s_r & (f_r >> 3)) << 11) | ((s_g & (f_g >> 2)) << 5) | (s_b & (f_b >> 3));
        });
}

// --- HBlend (半透明混合) ---
void PutCmprsImgClipingHBlend565_CPP(int x, int y, Spr* sp, void* dest, int destPitch, int lx, int rx, int ty, int by) {
    PutCmprsImgCliping_Generic_CPP(x, y, sp, dest, destPitch, lx, rx, ty, by,
        [](uint16_t s_p, uint16_t* d_p_ptr) { *d_p_ptr = (((s_p & 0xF7DE) + (*d_p_ptr & 0xF7DE)) >> 1); });
}
void PutCmprsImgNOTClipingHBlend565_CPP(int x, int y, Spr* sp, void* dest, int destPitch) {
    PutCmprsImgNOTCliping_Generic_CPP(x, y, sp, dest, destPitch,
        [](uint16_t s_p, uint16_t* d_p_ptr) { *d_p_ptr = (((s_p & 0xF7DE) + (*d_p_ptr & 0xF7DE)) >> 1); });
}

// --- Difer (差異混合) ---
void PutCmprsImgClipingAddDifer565_CPP(int x, int y, Spr* sp, void* dest, int destPitch, int lx, int rx, int ty, int by, int alpha) {
    PutCmprsImgCliping_Generic_CPP(x, y, sp, dest, destPitch, lx, rx, ty, by,
        [=](uint16_t s_p, uint16_t* d_p_ptr) {
            if (s_p != *d_p_ptr) {
                int s_r = (s_p >> 11) & 0x1F, s_g = (s_p >> 5) & 0x3F, s_b = s_p & 0x1F;
                int d_r = (*d_p_ptr >> 11) & 0x1F, d_g = (*d_p_ptr >> 5) & 0x3F, d_b = *d_p_ptr & 0x1F;
                s_r = (s_r * alpha) >> 4; s_g = (s_g * alpha) >> 4; s_b = (s_b * alpha) >> 4;
                int r = s_r + d_r, g = s_g + d_g, b = s_b + d_b;
                if (r > 31) r = 31; if (g > 63) g = 63; if (b > 31) b = 31;
                *d_p_ptr = (r << 11) | (g << 5) | b;
            }
        });
}
void PutCmprsImgNOTClipingAddDifer565_CPP(int x, int y, Spr* sp, void* dest, int destPitch, int alpha) {
    PutCmprsImgNOTCliping_Generic_CPP(x, y, sp, dest, destPitch,
        [=](uint16_t s_p, uint16_t* d_p_ptr) {
            if (s_p != *d_p_ptr) {
                int s_r = (s_p >> 11) & 0x1F, s_g = (s_p >> 5) & 0x3F, s_b = s_p & 0x1F;
                int d_r = (*d_p_ptr >> 11) & 0x1F, d_g = (*d_p_ptr >> 5) & 0x3F, d_b = *d_p_ptr & 0x1F;
                s_r = (s_r * alpha) >> 4; s_g = (s_g * alpha) >> 4; s_b = (s_b * alpha) >> 4;
                int r = s_r + d_r, g = s_g + d_g, b = s_b + d_b;
                if (r > 31) r = 31; if (g > 63) g = 63; if (b > 31) b = 31;
                *d_p_ptr = (r << 11) | (g << 5) | b;
            }
        });
}
void PutCmprsImgClipingBlendDifer565_CPP(int x, int y, Spr* sp, void* dest, int destPitch, int lx, int rx, int ty, int by, int alpha) {
    alpha = std::min(32, std::max(0, alpha)); const int inv_alpha = 32 - alpha;
    PutCmprsImgCliping_Generic_CPP(x, y, sp, dest, destPitch, lx, rx, ty, by,
        [=](uint16_t s_p, uint16_t* d_p_ptr) {
            if (s_p != *d_p_ptr) {
                int s_r = (s_p >> 11) & 0x1F, s_g = (s_p >> 5) & 0x3F, s_b = s_p & 0x1F;
                int d_r = (*d_p_ptr >> 11) & 0x1F, d_g = (*d_p_ptr >> 5) & 0x3F, d_b = *d_p_ptr & 0x1F;
                int r = (s_r * alpha + d_r * inv_alpha) >> 5, g = (s_g * alpha + d_g * inv_alpha) >> 5, b = (s_b * alpha + d_b * inv_alpha) >> 5;
                *d_p_ptr = (r << 11) | (g << 5) | b;
            }
        });
}
void PutCmprsImgNOTClipingBlendDifer565_CPP(int x, int y, Spr* sp, void* dest, int destPitch, int alpha) {
    alpha = std::min(32, std::max(0, alpha)); const int inv_alpha = 32 - alpha;
    PutCmprsImgNOTCliping_Generic_CPP(x, y, sp, dest, destPitch,
        [=](uint16_t s_p, uint16_t* d_p_ptr) {
            if (s_p != *d_p_ptr) {
                int s_r = (s_p >> 11) & 0x1F, s_g = (s_p >> 5) & 0x3F, s_b = s_p & 0x1F;
                int d_r = (*d_p_ptr >> 11) & 0x1F, d_g = (*d_p_ptr >> 5) & 0x3F, d_b = *d_p_ptr & 0x1F;
                int r = (s_r * alpha + d_r * inv_alpha) >> 5, g = (s_g * alpha + d_g * inv_alpha) >> 5, b = (s_b * alpha + d_b * inv_alpha) >> 5;
                *d_p_ptr = (r << 11) | (g << 5) | b;
            }
        });
}

// ======================================================================
// 【新】CharRGB (Alpha 遮罩顏色混合)
// ======================================================================

void PutCmprsImgClipingCharRGB565_CPP(int x, int y, Spr* sp, void* dest,
                                      int destPitch, int lx, int rx, int ty,
                                      int by, uint32_t RGB) {
  // 從傳入的 DWORD (0xRRGGBB) 中分解出 8-bit 的 R, G, B 分量
  const uint8_t r_in = (RGB >> 16) & 0xFF;
  const uint8_t g_in = (RGB >> 8) & 0xFF;
  const uint8_t b_in = RGB & 0xFF;

  // 呼叫你的泛型裁剪函式，並傳入 CharRGB565 的專屬 Lambda 混合操作
  PutCmprsImgCliping_Generic_CPP(x, y, sp, dest, destPitch, lx, rx, ty, by,
                                 [=](uint16_t s_pixel, uint16_t* d_pixel_ptr) {
                                   // s_pixel
                                   // 是來源圖像的像素，我們只使用它的藍色通道作為
                                   // Alpha

                                   // 1. 從來源像素的藍色通道提取 5-bit Alpha
                                   // (範圍 0-31)
                                   uint8_t alpha = s_pixel & 0x1F;

                                   // 2. 根據 Alpha，混合使用者指定的 RGB 顏色
                                   //    使用 31 進行標準化，確保 alpha=31
                                   //    時能得到完整的原色
                                   uint8_t r_out = (r_in * alpha) / 31;
                                   uint8_t g_out = (g_in * alpha) / 31;
                                   uint8_t b_out = (b_in * alpha) / 31;

                                   // 3. 將計算出的 8-bit RGB 顏色打包成 16-bit
                                   // RGB565 格式，並寫入目標
                                   *d_pixel_ptr = ((r_out >> 3) << 11) |
                                                  ((g_out >> 2) << 5) |
                                                  (b_out >> 3);
                                 });
}

void PutCmprsImgNOTClipingCharRGB565_CPP(int x, int y, Spr* sp, void* dest,
                                         int destPitch, uint32_t RGB) {
  // 從傳入的 DWORD (0xRRGGBB) 中分解出 8-bit 的 R, G, B 分量
  const uint8_t r_in = (RGB >> 16) & 0xFF;
  const uint8_t g_in = (RGB >> 8) & 0xFF;
  const uint8_t b_in = RGB & 0xFF;

  // 呼叫你的泛型非裁剪函式，並傳入相同的 Lambda 混合操作
  PutCmprsImgNOTCliping_Generic_CPP(
      x, y, sp, dest, destPitch, [=](uint16_t s_pixel, uint16_t* d_pixel_ptr) {
        // 1. 從來源像素的藍色通道提取 5-bit Alpha (範圍 0-31)
        uint8_t alpha = s_pixel & 0x1F;

        // 2. 根據 Alpha，混合使用者指定的 RGB 顏色
        uint8_t r_out = (r_in * alpha) / 31;
        uint8_t g_out = (g_in * alpha) / 31;
        uint8_t b_out = (b_in * alpha) / 31;

        // 3. 將計算出的 8-bit RGB 顏色打包成 16-bit RGB565 格式，並寫入目標
        *d_pixel_ptr =
            ((r_out >> 3) << 11) | ((g_out >> 2) << 5) | (b_out >> 3);
      });
}


// ======================================================================
// 【新】Light (加色光暈混合)
// ======================================================================

void PutCmprsImgClipingLight_CPP(int x, int y, Spr* sp, void* dest,
                                 int destPitch, int lx, int rx, int ty, int by,
                                 int alpha) {
  // 呼叫你的泛型裁剪函式，並傳入加色混合的 Lambda
  PutCmprsImgCliping_Generic_CPP(
      x, y, sp, dest, destPitch, lx, rx, ty, by,
      [=](uint16_t s_p, uint16_t* d_p_ptr) {
        // 1. 分解來源 (s_p) 和目標 (*d_p_ptr) 像素的 R/G/B 通道
        int s_r = (s_p >> 11) & 0x1F, s_g = (s_p >> 5) & 0x3F, s_b = s_p & 0x1F;
        int d_r = (*d_p_ptr >> 11) & 0x1F, d_g = (*d_p_ptr >> 5) & 0x3F,
            d_b = *d_p_ptr & 0x1F;

        // 2. 根據 alpha (0-16) 調整來源像素的強度
        //    shr 4 等同於除以 16，與 ASM 中的邏輯一致
        s_r = (s_r * alpha) >> 4;
        s_g = (s_g * alpha) >> 4;
        s_b = (s_b * alpha) >> 4;

        // 3. 將調整後的來源顏色與目標顏色相加
        int r = s_r + d_r;
        int g = s_g + d_g;
        int b = s_b + d_b;

        // 4. 處理溢位 (顏色值不能超過最大值)
        if (r > 31) r = 31;
        if (g > 63) g = 63;
        if (b > 31) b = 31;

        // 5. 將結果重新組合並寫回目標
        *d_p_ptr = (r << 11) | (g << 5) | b;
      });
}

void PutCmprsImgNOTClipingLight_CPP(int x, int y, Spr* sp, void* dest,
                                    int destPitch, int alpha) {
  // 呼叫你的泛型非裁剪函式，並傳入相同的 Lambda
  PutCmprsImgNOTCliping_Generic_CPP(
      x, y, sp, dest, destPitch, [=](uint16_t s_p, uint16_t* d_p_ptr) {
        int s_r = (s_p >> 11) & 0x1F, s_g = (s_p >> 5) & 0x3F, s_b = s_p & 0x1F;
        int d_r = (*d_p_ptr >> 11) & 0x1F, d_g = (*d_p_ptr >> 5) & 0x3F,
            d_b = *d_p_ptr & 0x1F;

        s_r = (s_r * alpha) >> 4;
        s_g = (s_g * alpha) >> 4;
        s_b = (s_b * alpha) >> 4;

        int r = s_r + d_r;
        int g = s_g + d_g;
        int b = s_b + d_b;

        if (r > 31) r = 31;
        if (g > 63) g = 63;
        if (b > 31) b = 31;

        *d_p_ptr = (r << 11) | (g << 5) | b;
      });
}

template <typename BlendOp>
void PutCmprsImgCliping_Generic_Extended_CPP(
    int x, int y, Spr* sp, void* dest, int destPitch, int lx, int rx, int ty,
    int by, BlendOp blend_op,
    std::function<int(int)> xOffsetOp = [](int) { return 0; }) {
  if (!sp || !sp->img || by <= 0) return;
  const uint8_t* src_ptr = (const uint8_t*)sp->img;
  uint8_t* dest_base_ptr = (uint8_t*)dest;

  for (int i = 0; i < ty; ++i) {
    while (*reinterpret_cast<const uint16_t*>(src_ptr) != 0xFFFF) {
      src_ptr += 4 + *reinterpret_cast<const uint16_t*>(src_ptr + 2);
    }
    src_ptr += 2;
  }

  int lx_bytes = lx * 2;
  int rx_bytes = (lx + rx) * 2;

  for (int i = 0; i < by; ++i) {
    // --- 核心改動：在這裡應用水平偏移 ---
    int current_wave_offset_pixels = xOffsetOp(y + i);
    int current_x = x + current_wave_offset_pixels;
    // ---

    int dest_y = y + i;
    uint8_t* dest_screen_line_start = dest_base_ptr + dest_y * destPitch;

    int current_x_bytes_in_sprite = 0;
    while (true) {
      uint16_t skip_bytes = *reinterpret_cast<const uint16_t*>(src_ptr);
      if (skip_bytes == 0xFFFF) {
        src_ptr += 2;
        break;
      }
      uint16_t copy_bytes = *reinterpret_cast<const uint16_t*>(src_ptr + 2);
      src_ptr += 4;

      int block_start_byte = current_x_bytes_in_sprite + skip_bytes;
      int block_end_byte = block_start_byte + copy_bytes;

      int draw_start_byte_in_sprite = std::max(block_start_byte, lx_bytes);
      int draw_end_byte_in_sprite = std::min(block_end_byte, rx_bytes);

      if (draw_start_byte_in_sprite < draw_end_byte_in_sprite) {
        // 注意這裡的 dest_start_byte_on_screen，它現在使用 current_x
        int dest_start_byte_on_screen =
            current_x * 2 + (draw_start_byte_in_sprite - lx_bytes);
        int src_offset_in_block = draw_start_byte_in_sprite - block_start_byte;
        int bytes_to_draw = draw_end_byte_in_sprite - draw_start_byte_in_sprite;

        const uint8_t* s = src_ptr + src_offset_in_block;
        uint8_t* d = dest_screen_line_start + dest_start_byte_on_screen;

        for (int k = 0; k < bytes_to_draw; k += 2) {
          uint16_t s_pixel = *reinterpret_cast<const uint16_t*>(s + k);
          uint16_t* d_pixel_ptr = reinterpret_cast<uint16_t*>(d + k);
          blend_op(s_pixel, d_pixel_ptr);
        }
      }
      src_ptr += copy_bytes;
      current_x_bytes_in_sprite += skip_bytes + copy_bytes;
    }
  }
}

template <typename BlendOp>
void PutCmprsImgNOTCliping_Generic_Extended_CPP(
    int x, int y, Spr* sp, void* dest, int destPitch, BlendOp blend_op,
    std::function<int(int)> xOffsetOp = [](int) { return 0; }) {
  if (!sp || !sp->img || sp->yl <= 0) return;
  const uint8_t* src_ptr = (const uint8_t*)sp->img;
  uint8_t* dest_base_ptr = (uint8_t*)dest;

  for (int i = 0; i < sp->yl; ++i) {
    // --- 核心改動：在這裡應用水平偏移 ---
    int current_wave_offset_pixels = xOffsetOp(y + i);
    int current_x = x + current_wave_offset_pixels;
    // ---

    uint8_t* dest_draw_ptr =
        dest_base_ptr + (y + i) * destPitch + current_x * 2;

    while (true) {
      uint16_t skip_bytes = *reinterpret_cast<const uint16_t*>(src_ptr);
      if (skip_bytes == 0xFFFF) {
        src_ptr += 2;
        break;
      }
      uint16_t copy_bytes = *reinterpret_cast<const uint16_t*>(src_ptr + 2);
      src_ptr += 4;

      dest_draw_ptr += skip_bytes;

      for (int k = 0; k < copy_bytes; k += 2) {
        uint16_t s_pixel = *reinterpret_cast<const uint16_t*>(src_ptr + k);
        uint16_t* d_pixel_ptr = reinterpret_cast<uint16_t*>(dest_draw_ptr + k);
        blend_op(s_pixel, d_pixel_ptr);
      }
      src_ptr += copy_bytes;
      dest_draw_ptr += copy_bytes;
    }
  }
}

extern int WaveTileStart;
extern int tilewavetable[400];
const int TILE_WAVE_TABLE_SIZE =400;

void PutCmprsImgClipingWaveTile_CPP(int x, int y, Spr* sp, void* dest,
                                    int destPitch, int lx, int rx, int ty,
                                    int by) {
  // 定義水平偏移的 Lambda
  auto wave_offset_op = [=](int screen_y) {
    int table_index = (WaveTileStart + screen_y) & (TILE_WAVE_TABLE_SIZE - 1);
    return tilewavetable[table_index];
  };
  // 呼叫新的泛型函式，並傳入不透明複製的 Lambda 和水平偏移的 Lambda
  PutCmprsImgCliping_Generic_Extended_CPP(
      x, y, sp, dest, destPitch, lx, rx, ty, by,
      [](uint16_t s_p, uint16_t* d_p_ptr) { *d_p_ptr = s_p; }, wave_offset_op);
}

void PutCmprsImgNOTClipingWaveTile_CPP(int x, int y, Spr* sp, void* dest,
                                       int destPitch) {
  auto wave_offset_op = [=](int screen_y) {
    int table_index = (WaveTileStart + screen_y) & (TILE_WAVE_TABLE_SIZE - 1);
    return tilewavetable[table_index];
  };
  PutCmprsImgNOTCliping_Generic_Extended_CPP(
      x, y, sp, dest, destPitch,
      [](uint16_t s_p, uint16_t* d_p_ptr) { *d_p_ptr = s_p; }, wave_offset_op);
}

// --- Shadow (扭曲陰影) ---
void PutCmprsImgNOTClipingShadow565_CPP(int x, int y, int yl, const void* Stmp, void* dest, uint32_t dDxSize)  // 新增：目的緩衝區的結束位址
{
  // --- Initial Setup (對應 ASM 函式開頭的準備) ---
  // if(SY==0) jz exit_1; (SY 在 ASM 中是 j, 即 yl)
  if (yl <= 0) {
    return;
  }

  x += (yl / 2);
  y += (yl / 2);

  // LPVOID Dtmp = ...; mov edx, Dtmp;
  // Dtmp 對應 edx 暫存器，作為每輪迴圈的「基準行」指標
  uint8_t* Dtmp =
      (uint8_t*)dest + (size_t)y * dDxSize + (size_t)x * sizeof(uint16_t);

  // mov esi, Stmp;
  // Sptr 對應 esi 暫存器，來源資料指標
  const uint16_t* Sptr = (const uint16_t*)Stmp;

  // sk = dDxSize - 2;
  // 預先計算好的行進增量
  const int32_t sk = dDxSize - 2;

  // j = yl;
  // j 對應記憶體中的計數器 j
  int j = yl;

  // --- 主迴圈 (對應 loop_1 到 jnz loop_1 的完整結構) ---
  while (true) {
    // === loop_1: ===
    // mov edi, edx;
    // Dptr 對應 edi 暫存器，是當前區塊的活動指標
    uint16_t* Dptr = (uint16_t*)Dtmp;

    // --- 第一部分：繪圖迴圈 (對應 loop_2 及 loop_TR) ---
    // === loop_2: ===
    while (*Sptr != 0xffff) {  // cmp word ptr[esi], 0xffff; jz exit_2;

      // mov ax, [esi];
      uint16_t skip_bytes = *Sptr;
      // mov cx, [esi + 2];
      uint16_t draw_bytes_total = *(Sptr + 1);
      // add esi, 4;
      Sptr += 2;

      // add edi, eax;
      Dptr = (uint16_t*)((uint8_t*)Dptr + skip_bytes);

      // shr ecx, 1;
      int pixels_to_draw = draw_bytes_total / 2;

      // === loop_TR: ===
      for (int k = 0; k < pixels_to_draw; ++k) {
        // mov ebx, [edi];
        uint16_t dest_pixel = *Dptr;
        // and ebx, 1111011111011110b; shr ebx, 1;
        dest_pixel = (dest_pixel & 0xF7DE) >> 1;
        // mov [edi], bx;
        *Dptr = dest_pixel;

        // add esi, 2;
        Sptr++;
        // add edi, 2;
        Dptr++;
      }
      // jmp loop_2; (由 while 迴圈的結構實現)
    }

    // === exit_2: ===
    Sptr++;  // add esi, 2; (跳過 0xffff)

    Dtmp += sk;    // add edx, sk;  (將基準行指標 edx 移動 dDxSize - 2)
    j--;           // dec j;        (第一次計數器遞減)
    if (j == 0) {  // jz exit_1;
      break;
    }

    // --- 中介步驟 (程式流程從 exit_2 掉下來) ---
    // mov edi, edx;
    // 將活動指標 edi 重設為「已經移動過」的基準行指標
    Dptr = (uint16_t*)Dtmp;

    // --- 第二部分：純跳過迴圈 (對應 loop_22) ---
    // === loop_22: ===
    while (*Sptr != 0xffff) {  // cmp word ptr[esi], 0xffff; jz exit_22;

      // mov ax, [esi];
      uint16_t skip_bytes = *Sptr;
      // mov cx, [esi + 2];
      uint16_t bytes_to_skip_in_source = *(Sptr + 1);
      // add esi, 4;
      Sptr += 2;

      // add edi, eax;
      Dptr = (uint16_t*)((uint8_t*)Dptr + skip_bytes);
      // add esi, ecx;
      Sptr = (const uint16_t*)((const uint8_t*)Sptr + bytes_to_skip_in_source);

      // jmp loop_22; (由 while 迴圈的結構實現)
    }

    // === exit_22: ===
    Sptr++;  // add esi, 2; (跳過 0xffff)

    j--;           // dec j;        (第二次計數器遞減)
    if (j <= 0) {  // jnz loop_1; (如果 j 變為 0 或負數，則跳出)
      break;
    }
    // 如果 j > 0，則繼續 while(true) 的下一輪迴圈
  }
  // === exit_1: ===
  // 函式結束
}
// --- 【新】顏色轉換工具 ---
uint16_t RGB16_CPP(int r, int g, int b) {
    return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
}
uint32_t ReturnBlendRGB_CPP(int r, int g, int b) {
    uint16_t rgb16 = RGB16_CPP(r, g, b);
    uint32_t packed_rgb = 0;
    // 忠實重現原始 ASM 的打包邏輯，即使它有缺陷
    uint32_t temp = rgb16;
    temp = (temp >> 11) | (temp << (32 - 11)); // ror edx, 11
    temp = (temp << 10) & 0xFFFF0000;          // shl dx, 10
    temp = (temp << 6) | (temp >> (32 - 6));   // rol edx, 6
    temp = (temp << 5) & 0xFFFF0000;           // shl dx, 5
    temp = (temp << 5) | (temp >> (32 - 5));   // rol edx, 5
    packed_rgb = temp;
    return packed_rgb;
}
