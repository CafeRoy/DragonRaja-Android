/*****************************************************************************\
* Copyright (c), Future Entertainment World / Seoul, Republic of Korea        *
* All Rights Reserved.                                                        *
*                                                                             *
* This document contains proprietary and confidential information.  No        *
* parts of this document or the computer program it embodies may be in        *
* any way copied, duplicated, reproduced, translated into a different         *
* programming language, or distributed to any person, company, or             *
* corporation without the prior written consent of Future Entertainment World *
\*****************************************************************************/



#include "stdafx.h"
#include <Math.h>
#include <stdio.h>
#include "Dragon.h"
#include "GameProc.h"
#include "Char.h"
#include "Hong_Sprite.h"
#include "Map.h"
#include "Hangul.h"
#include "Hong_Sub.h"
#include "Tool.h"
#include "MouseCursor.h"
#include "Effect.h"
#include "Hong_Light.h"
#include"CRenderMethod.h"


extern CHARACTERLIST		g_CharacterList;
int LightT[LIGHT_ROW][LIGHT_COL] = { 0, };
int LightTableX = 0;
int LightTableY = 0;

int DayLightControl = 31;
int always_lightOn = 0;
unsigned char* BrightSprBuf[21][2] = { 0, };

int   LightCount = 0;
LIGHT Light[LIGHT_MAX_] = { 0, };
LIGHT g_OutLight[MAX_OUTLIGHT_] = { 0, };

//unsigned char BrightBuf[(SCREEN_WIDTH / 2) * (SCREEN_HEIGHT / 2)] = {0};
unsigned char BrightBuf[(960) * (540)] = {0};
WORD rgbTable[32 * 65536] = { 0, };

/*void MakergbTable(void)
{
	WORD c;
	BYTE j;

	WORD i;
	int  t;

	if( _PixelInfo.BitMaskR == 0x7C00 )	// 555
	{
		for( t = 0, i = 0 ; t < 65536 ; t++, i ++)
		{
			for( j = 0 ; j < 32 ; j ++)
			{
			_asm {
					xor		eax,eax;
					mov		ax,i;
					ror		eax,10;
					shl		ax,11;
					rol		eax,5;
					shl		ax,6;
					rol		eax,5;

					xor     ebx, ebx;
					mov     bl, j;
					imul	eax,ebx;
					shr		eax,5;
					and		eax,0x03e0fc1f;

					ror		eax,5;
					shr		ax,6;
					ror		eax,5;
					shr		ax,11;
					rol		eax,10;

					mov		c,ax;
			}
				rgbTable[j * 65536 + i] = c;
			}
		}
	}
	else								// 565
	{
		for( t = 0, i = 0 ; t < 65536 ; t++, i ++)
		{
			for( j = 0 ; j < 32 ; j ++)
			{
			_asm {
					xor		eax,eax;
					mov		ax,i;
					ror		eax,11;
					shl		ax,10;
					rol		eax,6;
					shl		ax,5;
					rol		eax,5;

					xor     ebx, ebx;
					mov     bl, j;
					imul	eax,ebx;
					shr		eax,5;
					and		eax,0x03e0fc1f;

					ror		eax,5;
					shr		ax,5;
					ror		eax,6;
					shr		ax,10;
					rol		eax,11;

					mov		c,ax;
			}
				rgbTable[j * 65536 + i] = c;
			}
		}
	}
}*/

void MakergbTable() {
	SDL_Log("DEBUG: Entering MakergbTable_C_Version...");

	if (g_SDLInfo.cpuRenderSurface == NULL || g_SDLInfo.cpuRenderSurface->format == NULL) {
		SDL_Log("Error: MakergbTable_C_Version: cpuRenderSurface or its format is NULL!");
		return;
	}

	SDL_PixelFormat* format = g_SDLInfo.cpuRenderSurface->format;

	SDL_PixelFormat* sourceFormat = SDL_AllocFormat(SDL_PIXELFORMAT_RGB565);
	if (sourceFormat == NULL) {
		SDL_Log("Error: Failed to allocate SDL_PIXELFORMAT_RGB565 format for source pixels!");
		return;
	}

	SDL_Log("DEBUG: Starting outer loop (originalPixelValue)...");
	for (unsigned int originalPixelValue = 0; originalPixelValue < 65536; originalPixelValue++) {
		// 每隔一定數量打印一次，避免過多輸出導致卡頓
		if (originalPixelValue % 10000 == 0) {
			SDL_Log("DEBUG: originalPixelValue = %u", originalPixelValue);
		}

		Uint8 r_orig, g_orig, b_orig;
		// 如果卡在這裡，請特別留意這行
		SDL_GetRGB(originalPixelValue, sourceFormat, &r_orig, &g_orig, &b_orig);

		for (BYTE brightness = 0; brightness < 32; brightness++) {
			// 這個內層迴圈通常不會導致卡頓，除非外層迴圈本身就有問題
			float lightFactor = (float)brightness / 31.0f;

			Uint8 r_final = (Uint8)(r_orig * lightFactor);
			Uint8 g_final = (Uint8)(g_orig * lightFactor);
			Uint8 b_final = (Uint8)(b_orig * lightFactor);

			if (r_final > 255) r_final = 255;
			if (g_final > 255) g_final = 255;
			if (b_final > 255) b_final = 255;

			// 如果卡在這裡，請特別留意這行
			rgbTable[brightness * 65536 + originalPixelValue] = (WORD)SDL_MapRGB(format, r_final, g_final, b_final);
		}
	}

	SDL_FreeFormat(sourceFormat);
	SDL_Log("DEBUG: MakergbTable_C_Version completed successfully."); // 添加完成訊息
}

/**
 * @brief 將光照圖效果應用到目標畫布上 (直接運算版本)。
 * 這是對原始組合語言函式 PutMapBright 的直接、功能對等的 C++ 翻譯。
 *
 * 這個版本採用直接的數學運算來調整像素亮度。它一次處理兩個像素
 * (一個 32-bit DWORD)，並使用一個與螢幕等寬的光照圖。
 *
 * @param dest 指向目標畫布記憶體的指標 (格式應為 16-bit RGB565)。
 * @param alpha 指向一個 8-bit 光照圖記憶體的指標。此光照圖的解析度
 * 應與目標畫布相同 (GAME_SCREEN_XSIZE * GAME_SCREEN_YSIZE)。
 */
void PutMapBright_CPP(void* dest, void* alpha)
{
	// 1. 將通用指標轉換為我們需要的具體型別
	uint8_t* lightmap = static_cast<uint8_t*>(alpha);
	uint8_t* dest_buffer = static_cast<uint8_t*>(dest);

	// 2. 計算目標畫布的 Pitch (一行所佔的位元組數)
	const int dest_pitch = GAME_SCREEN_XSIZE * 2; // 假設每個像素 2 位元組

	// 3. 主迴圈：一次處理螢幕上的一行
	for (int y = 0; y < GAME_SCREEN_YSIZE; ++y) {

		// 取得指向光照圖和目標畫布中當前行的指標
		uint8_t* lightmap_row_ptr = lightmap + y * GAME_SCREEN_XSIZE;
		uint32_t* dest_row_ptr = reinterpret_cast<uint32_t*>(dest_buffer + y * dest_pitch);

		// 4. 內層迴圈：一次處理螢幕上的兩個像素 (一個 32-bit DWORD)
		// 原始 ASM 的迴圈計數器有誤，這裡修正為正確的邏輯
		for (int x = 0; x < GAME_SCREEN_XSIZE / 2; ++x) {

			// 讀取包含兩個像素的 32-bit 值
			uint32_t two_pixels = dest_row_ptr[x];

			// 讀取對應的亮度值
			// 注意：原始 ASM 每次迴圈只讀取一個亮度值，但處理兩個像素。
			// 這可能是一個 bug 或一種特殊的處理方式。我們這裡忠實重現。
			uint8_t brightness = lightmap_row_ptr[x]; // 或者 lightmap_row_ptr[x*2] ? 根據實際效果調整

			// 執行原始 ASM 中的核心演算法
			// 這是一種快速但有損精度的亮度調整技巧
			two_pixels >>= 5;
			two_pixels *= brightness;

			// 將處理後的 32-bit 值寫回
			dest_row_ptr[x] = two_pixels;
		}
	}
}


/**
 * @brief 將光照圖效果應用到目標畫布上 (查詢表版本)。
 * 這是對原始組合語言函式 PutMapBright2 的直接、功能對等的 C++ 翻譯。
 *
 * 這個版本使用一個巨大的預先計算好的查詢表 (LUT) 來決定最終顏色，
 * 並且光照圖的解析度是螢幕的一半。
 *
 * @param dest 指向目標畫布記憶體的指標 (格式應為 16-bit RGB565)。
 * @param alpha 指向一個 8-bit 光照圖記憶體的指標。此光照圖的解析度
 * 應為目標畫布的一半 (GAME_SCREEN_XSIZE/2 * GAME_SCREEN_YSIZE/2)。
 * @param rgbTable 指向一個巨大的顏色查詢表。
 */
void PutMapBright2_CPP(void* dest, void* alpha)
{
	// 1. 將通用指標轉換為我們需要的具體型別
	uint8_t* lightmap = static_cast<uint8_t*>(alpha);
	uint8_t* dest_buffer = static_cast<uint8_t*>(dest);

	// 2. 計算光照圖的寬度 (一行有多少個亮度值)
	const int lightmap_width = GAME_SCREEN_XSIZE / 2;
	// 計算目標畫布的 Pitch (一行所佔的位元組數)
	const int dest_pitch = GAME_SCREEN_XSIZE * 2; // 假設每個像素 2 位元組

	// 3. 主迴圈：一次處理螢幕上的兩行
	for (int y = 0; y < GAME_SCREEN_YSIZE; y += 2) {
		//HandleRunning(&connections);
		// 取得指向光照圖中當前行的指標
		uint8_t* lightmap_row_ptr = lightmap + (y / 2) * lightmap_width;

		// 取得指向目標畫布中我們要處理的第一行和第二行的指標
		uint16_t* dest_row1_ptr = reinterpret_cast<uint16_t*>(dest_buffer + y * dest_pitch);
		uint16_t* dest_row2_ptr = reinterpret_cast<uint16_t*>(dest_buffer + (y + 1) * dest_pitch);

		// 4. 內層迴圈：一次處理螢幕上的 2x2 像素區塊
		for (int x = 0; x < GAME_SCREEN_XSIZE; x += 2) {

			// 取得對應這個 2x2 區塊的單一亮度值
			uint8_t brightness = lightmap_row_ptr[x / 2];

			// 原始 ASM 邏輯：如果亮度值是 31，則跳過處理。
			if (brightness == 31) {
				continue;
			}

			// --- 處理 2x2 像素區塊 ---

			// 處理左上角的像素 (y, x)
			uint16_t p1_original = dest_row1_ptr[x];
			uint32_t index1 = (static_cast<uint32_t>(brightness) << 16) | p1_original;
			dest_row1_ptr[x] = rgbTable[index1];

			// 處理右上角的像素 (y, x+1)
			uint16_t p2_original = dest_row1_ptr[x + 1];
			uint32_t index2 = (static_cast<uint32_t>(brightness) << 16) | p2_original;
			dest_row1_ptr[x + 1] = rgbTable[index2];

			// 處理左下角的像素 (y+1, x)
			uint16_t p3_original = dest_row2_ptr[x];
			uint32_t index3 = (static_cast<uint32_t>(brightness) << 16) | p3_original;
			dest_row2_ptr[x] = rgbTable[index3];

			// 處理右下角的像素 (y+1, x+1)
			uint16_t p4_original = dest_row2_ptr[x + 1];
			uint32_t index4 = (static_cast<uint32_t>(brightness) << 16) | p4_original;
			dest_row2_ptr[x + 1] = rgbTable[index4];
		}
	}
}

/*void PutMapBright(LPVOID dest, LPVOID alpha)
{
	DWORD	k = 1280,j;
	LPVOID	Dtmp = (LPVOID)(dest);

	j = SCREEN_WIDTH;

	_asm{
		push	es;

		push	ds;
		pop		es;

		xor		ebx,ebx;

		xor		eax,eax;
		xor		ecx,ecx;

		mov		cx, GAME_SCREEN_YSIZE

		mov		esi, alpha

	loop_1:
		mov		edi,Dtmp;

		push    cx
		xor		eax,eax;
		mov		cx,SCREEN_WIDTH;	//load data num

loop_alpha:
		xor		eax,eax;
		mov		eax,[edi];
		shr		eax,5;

		mov     bl, byte ptr[ esi];
		inc		esi;
		imul	eax,ebx;

		mov		[edi],eax;

		add		edi,4;
		loop	loop_alpha;

		pop		cx
		dec		cx
		jz      exit_1;

		mov		eax,Dtmp;
		add		eax,k;
		mov		Dtmp,eax;

		jmp		loop_1;

	exit_1:
		pop		es;
	};
}

void PutMapBright2(LPVOID dest, LPVOID alpha)
{
	DWORD	k = (GAME_SCREEN_XSIZE * 2), j;
	LPVOID	Dtmp = (LPVOID)(dest);

	j = SCREEN_WIDTH;

	_asm {
		push	es;

		push	ds;
		pop		es;

		xor ebx, ebx;

		xor eax, eax;
		xor ecx, ecx;

		mov		cx, (GAME_SCREEN_YSIZE / 2)

			mov		esi, alpha

			loop_1 :
		mov		edi, Dtmp;

		push    cx
			mov		cx, (GAME_SCREEN_XSIZE / 2);	//load data num

	loop_alpha:

		movzx   dx, byte ptr[esi];
		cmp     dl, 31
			jz      skip_1;
		shl     edx, 16
			movzx	eax, word ptr[edi + 2];
		add		edx, eax;
		shl		edx, 1;
		mov     ax, word ptr[offset  rgbTable + edx];
		shl		eax, 16


			movzx     dx, byte ptr[esi];
		shl     edx, 16
			mov		bx, [edi];
		add		edx, ebx;
		shl		edx, 1;
		mov     ax, word ptr[offset  rgbTable + edx];

		mov[edi], eax;

	skip_1:
		add		edi, 4;
		inc     esi

			dec		cx;
		jnz 	loop_alpha;

		//--------------

		sub     esi, (GAME_SCREEN_XSIZE / 2)  // 1 ¶óÀÎÀÇ µ¥ÀÌÅ¸¸¦ °¡Áö°í µÎ¹øÂïÀ»°ÍÀÌ±â¶§¹®.

			mov		edx, Dtmp;
		add		edx, k;
		mov		Dtmp, edx;
		mov		edi, Dtmp;

		mov		cx, (GAME_SCREEN_XSIZE / 2);	//load data num

	loop_alpha2:

		movzx   dx, byte ptr[esi];
		cmp     dl, 31
			jz      skip_2;
		xor eax, eax;
		shl     edx, 16
			mov		ax, [edi + 2];
		add		edx, eax;
		shl		edx, 1;
		mov     ax, word ptr[offset  rgbTable + edx];
		shl		eax, 16

			movzx   dx, byte ptr[esi];
		shl     edx, 16
			mov		bx, [edi];
		add		edx, ebx;
		shl		edx, 1;
		mov     ax, word ptr[offset  rgbTable + edx];

		mov[edi], eax;


	skip_2:
		add		edi, 4;
		inc     esi

			dec		cx
			jnz 	loop_alpha2;

		//------------------
		pop		cx
			dec		cx
			jz      exit_1;

		mov		edx, Dtmp;
		add		edx, k;
		mov		Dtmp, edx;

		jmp		loop_1;

	exit_1:
		pop		es;
	};
}*/

void LoadBrightDataSub(char* filename, unsigned char** buf0, unsigned char** buf1)
{
    // [Android] 1. 路徑修正：移除開頭的 ./
    std::string path = filename;
    if (path.find("./") == 0) path = path.substr(2);

    SDL_RWops* fp = SDL_RWFromFile(path.c_str(), "rb");

    if (!fp) {
        SDL_Log("Error: Failed to open %s", path.c_str());
        return;
    }

    DWORD xl, yl;

    // [Android] 2. Seek 跳過第 1 個 byte
    // SEEK_SET -> RW_SEEK_SET
    SDL_RWseek(fp, 1, RW_SEEK_SET);

    // [Android] 3. 讀取寬高
    SDL_RWread(fp, &xl, sizeof(DWORD), 1);
    SDL_RWread(fp, &yl, sizeof(DWORD), 1);

    // 分配記憶體 (保持原樣)
    // 注意：如果是用 new 分配，請確保 MemAlloc 內部實作正確
    // 這裡加上 nullptr 檢查以防萬一
    MemAlloc(*buf0, (xl * yl) + 8);
    MemAlloc(*buf1, (xl * yl) + 8);

    if (!*buf0 || !*buf1) {
        SDL_Log("Error: Memory allocation failed for BrightData");
        SDL_RWclose(fp);
        return;
    }

    // 寫入 Header 到緩衝區
    *(DWORD*)(*buf0) = xl;
    *(DWORD*)(*buf0 + 4) = yl;

    // [Android] 4. Seek 跳過 8 bytes
    // SEEK_CUR -> RW_SEEK_CUR
    SDL_RWseek(fp, 8, RW_SEEK_CUR);

    // [Android] 5. 讀取像素資料
    // 從 offset 8 開始寫入
    SDL_RWread(fp, *buf0 + 8, xl * yl, 1);

    // 複製到 buf1
    memcpy(*buf1, *buf0, xl * yl + 8);

    BYTE* p0 = (BYTE*)*buf0;
    BYTE* p1 = (BYTE*)*buf1;

    // 處理亮度 (保持原邏輯)
    // 注意：原本的迴圈是 i < xl * yl。
    // 因為資料是從 index 8 開始存，總長度是 xl*yl + 8。
    // 所以這個迴圈會處理到 xl*yl 的位置，也就是會漏掉最後 8 個 byte 的像素資料。
    // 如果這是故意的 (例如最後 8 byte 是 padding)，則維持原樣。
    // 如果這是 bug，正確應該是 i < xl * yl + 8。
    // 這裡為了保持 100% 移植兼容性，我維持原樣。
    for (DWORD i = 8; i < xl * yl; i++)
    {
        *(p0 + i) = *(p0 + i) / 8;
        *(p1 + i) = *(p1 + i) / 9;
    }

    // [Android] 6. 關檔
    SDL_RWclose(fp);
}

void LoadBrightData(void)
{
	char temp[FILENAME_MAX];

	for (int i = 0; i < 21; i++)
	{
		sprintf(temp, "./data/lig%03d.spr", i);
		LoadBrightDataSub(temp, &BrightSprBuf[i][0], &BrightSprBuf[i][1]);
	}
}

/**
 * @brief 將一個局部的光照圖案，以加色混合的方式，繪製到全域光照圖上。
 * 這是對原始組合語言函式 PutBright 的直接、功能對等的 C++ 翻譯。
 *
 * @param x 光照效果在螢幕上的中心 x 座標。
 * @param y 光照效果在螢幕上的中心 y 座標。
 * @param light_data 指向一個包含光照圖案資料的緩衝區。
 * 該緩衝區的前 8 個位元組是寬度和高度。
 * @param bright_buf 指向全域的、解析度為螢幕一半的光照圖 (Lightmap)。
 */
void PutBright_CPP(int x, int y, const uint8_t* light_data, uint8_t* bright_buf)
{
	// 1. 基本的有效性檢查
	if (!light_data || !bright_buf) {
		return;
	}

	// 2. 從資料頭部讀取光照圖案的寬度和高度
	const int light_width = *reinterpret_cast<const int32_t*>(light_data);
	const int light_height = *reinterpret_cast<const int32_t*>(light_data + 4);
	const uint8_t* source_ptr = light_data + 8; // 跳過 8 位元組的頭部

	if (light_width <= 0 || light_height <= 0) {
		return;
	}

	// 3. 座標轉換與校正
	// 將螢幕座標轉換為光照圖座標 (解析度減半)
	x >>= 1;
	y >>= 1;

	// 將座標原點從中心點移到左上角
	x -= (light_width / 2);
	y -= (light_height / 2);

	// 4. 邊界裁剪 (Clipping)
	// 全域光照圖的邊界
	const int lightmap_width = GAME_SCREEN_XSIZE / 2;
	const int lightmap_height = GAME_SCREEN_YSIZE / 2;

	// 計算來源 (light_data) 和目標 (bright_buf) 的有效繪製範圍
	int src_x_start = 0;
	int src_y_start = 0;
	int dest_x_start = x;
	int dest_y_start = y;
	int width_to_draw = light_width;
	int height_to_draw = light_height;

	// 與左邊界比較
	if (dest_x_start < 0) {
		src_x_start = -dest_x_start;
		width_to_draw -= src_x_start;
		dest_x_start = 0;
	}
	// 與上邊界比較
	if (dest_y_start < 0) {
		src_y_start = -dest_y_start;
		height_to_draw -= src_y_start;
		dest_y_start = 0;
	}
	// 與右邊界比較
	if (dest_x_start + width_to_draw > lightmap_width) {
		width_to_draw = lightmap_width - dest_x_start;
	}
	// 與下邊界比較
	if (dest_y_start + height_to_draw > lightmap_height) {
		height_to_draw = lightmap_height - dest_y_start;
	}

	if (width_to_draw <= 0 || height_to_draw <= 0) {
		return; // 完全在可見範圍之外
	}
	//HandleRunning(&connections);
	// 5. 主繪圖迴圈
	for (int i = 0; i < height_to_draw; ++i) {
		// 計算來源和目標當前行的起始指標
		const uint8_t* s_ptr = source_ptr + (src_y_start + i) * light_width + src_x_start;
		uint8_t* d_ptr = bright_buf + (dest_y_start + i) * lightmap_width + dest_x_start;

		for (int j = 0; j < width_to_draw; ++j) {
			// 讀取來源亮度值
			uint8_t source_brightness = s_ptr[j];

			// 原始 ASM 邏輯：如果來源亮度為 0，則不進行任何操作
			if (source_brightness > 0) {
				// 讀取目標亮度值，並進行加法混合
				int new_brightness = d_ptr[j] + source_brightness;

				// 鉗位 (Clamping)，確保結果不超過 31
				if (new_brightness > 31) {
					new_brightness = 31;
				}

				// 將計算後的值寫回目標
				d_ptr[j] = static_cast<uint8_t>(new_brightness);
			}
		}
	}
}

void PutBright(int x, int y, char* light)
{
	int xl, yl;
	int lx, rx, ty, by;
	char* sou;
	int i, j;
	unsigned char* p;

	if (!light) return; // LTS DRAGON BUG
	xl = *(DWORD*)light;
	yl = *(DWORD*)(light + 4);

	x >>= 1;
	y >>= 1;

	light += 8;

	x -= (xl / 2);
	y -= (yl / 2);

	if (x >= (GAME_SCREEN_XSIZE / 2))			return;
	else if (x + xl < 0)	return;
	if (y >= (GAME_SCREEN_YSIZE / 2))			return;
	else if (y + yl < 0)	return;

	rx = xl;
	lx = 0;
	ty = 0;
	by = yl;

	if (x + xl >= (GAME_SCREEN_XSIZE / 2))
	{
		rx -= x + xl - (GAME_SCREEN_XSIZE / 2);
	}
	if (x < 0)
	{
		lx = 0 - x;
		x = 0;
	}
	if (y + yl >= (GAME_SCREEN_YSIZE / 2))
	{
		by -= y + yl - (GAME_SCREEN_YSIZE / 2);
	}
	if (y < 0)
	{
		ty = 0 - y;
		y = 0;
	}

	sou = light + lx + ty * xl;
	for (i = ty; i < by; i++)
	{
		p = BrightBuf + x + y * (GAME_SCREEN_XSIZE / 2);
		for (j = lx; j < rx; j++, p++, sou++)
		{
			if (*sou)
			{
				*(p) += *sou;
				if (*(p) > 31) *(p) = 31;
			}
		}
		sou += (xl - (rx - lx));
		y++;
	}
}



void OutputLight(int x, int y, int lightness, int color)
{
	if (DayLightControl >= 31) return;
	if (LightCount + 1 >= LIGHT_MAX_) return;

	int mx, my;

	mx = x / TILE_SIZE;
	my = y / TILE_SIZE;

	if (TileMap[mx][my].roof && TileMap[mx][my].show_roof == 0)	return;


	for (int i = 0; i < LightCount; i++)
	{
		if (BoxAndDotCrash(Light[i].x - 48, Light[i].y - 48, 96, 96, x, y))
		{
			return;
		}
	}

	//distance_Light = sqrt(pow(Light[i].x - x,2)+pow(Light[i].y - y,2));
	//}
	Light[LightCount].x = x + (rand() % 5 - 2);
	Light[LightCount].y = y + (rand() % 5 - 2);
	Light[LightCount].light = lightness;		//0~16
	Light[LightCount].flag = color;
	LightCount++;
}


void	PutCharacterLight(LPCHARACTER	lpCharacter, int diff_Time)
{
    unsigned char* tt;

    if (g_Inside != TileMap[lpCharacter->position.x][lpCharacter->position.y].attr_inside)	return;

    tt = NULL;
    switch (lpCharacter->animationType)
    {
        case ANIMATION_TYPE_MAN:
        case ANIMATION_TYPE_WOMAN:
        {
            if (Hero->viewtype == VIEWTYPE_GHOST_)
            {
                if (lpCharacter->viewtype == VIEWTYPE_GHOST_)	//
                {
                    if (!IsHidenGhost(lpCharacter))//        Ʈ    ƴ϶            ´ .
                    {
                        tt = BrightSprBuf[18 - diff_Time][0];
                    }
                }
                else
                {
                    tt = BrightSprBuf[18 - diff_Time][0];			//      Ghost    Ǹ      Light         ִ .
                }
            }
            else
            {

                if (lpCharacter->viewtype != VIEWTYPE_TRANSPARENCY_ || lpCharacter == Hero)
                {
                    if (lpCharacter->viewtype == VIEWTYPE_GHOST_)	//
                    {
                        if (!IsHidenGhost(lpCharacter) && CanViewGhost(Hero))//020320 lsw
                        {
                            tt = BrightSprBuf[18 - diff_Time][0];		//   밡 Ghost           ִ            ϸ        ִ .
                        }
                    }
                    else											//     ִ
                    {
                        //if (lpCharacter == Hero)		tt = BrightSprBuf[18 - diff_Time][0];	//      Ghsot    ƴϰ    뵵 Ghost    ƴϸ        ִ .
                        //else
                        {
                            if (lpCharacter->sprno > 1)
                            {
                                tt = BrightSprBuf[18 - diff_Time][0];
                            }
                            else
                            {
                                int maxLight = min(18 - diff_Time, 9);
                                tt = BrightSprBuf[maxLight][0];
                            }
                        }


                    }
                }
            }

            break;
        }
        case ANIMATION_TYPE_MON1:
        {
            if (lpCharacter->nRace == HUMAN)
            {	//< CSD-030419
                tt = BrightSprBuf[15][0];
            }	//> CSD-030419

            break;
        }
    }

    if (tt)
    {
        // 【核心修改】：全面改用 visual_x / visual_y 進行渲染座標計算
        // 確保光圈中心與角色圖片的視覺中心完全重合
        int screen_x = (int)lpCharacter->visual_x - Mapx;
        int screen_y = (int)lpCharacter->visual_y - Mapy;

        if (Hero->lostSight)
        {
            if (lpCharacter == Hero)
            {
                // 這裡原代碼有 -32 的偏移，通常是為了將光圈對準腳下或身體中心，請保留
                PutBright_CPP(screen_x, screen_y - 32, tt, BrightBuf);
            }
        }
        else
        {
            if (lpCharacter->nRace == HUMAN)
            {
                // 將邏輯座標 lpCharacter->x 替換為剛才算的平滑 screen_x
                PutBright_CPP(screen_x, screen_y - 32, tt, BrightBuf);
            }
        }
    }
}

void FireflyBright() {
    // ===== 修改後的 MapBright 螢火蟲掛鉤邏輯 =====
    const auto& fireflies = g_fireflySystem.GetParticles();

    // 直接使用全域的平滑相機座標 Mapx/Mapy
    // 這確保了光暈、角色、地圖、特效全部使用同一個「視窗起始點」
    int map_offset_x_for_bright = Mapx;
    int map_offset_y_for_bright = Mapy;

    for (const auto& particle : fireflies) {
        // 1. 亮度計算 (保持不變)
        float brightness_ratio = (sin(particle.lifetime * particle.blink_speed + particle.blink_offset) + 1.0f) / 2.0f;
        const float MAX_LIGHT_LEVEL = 4.0f;
        int light_level = static_cast<int>(brightness_ratio * MAX_LIGHT_LEVEL);

        if (light_level < 2) continue;
        if (light_level > 20) light_level = 20; // 增加邊界保護
        if (!BrightSprBuf[light_level][0]) continue;

        // 2. 【核心修改】將螢火蟲的「世界座標」平滑轉換為「螢幕座標」
        // 因為 particle.world_x 是 float，Mapx 是平滑後的 int
        // 兩者相減能得到最精確的螢幕位置，不會有滑步感
        int screen_x = static_cast<int>(particle.world_x) - map_offset_x_for_bright;
        int screen_y = static_cast<int>(particle.world_y) - map_offset_y_for_bright;

        // 3. 渲染光暈到 BrightBuf (光照層)
        PutBright_CPP(screen_x, screen_y, BrightSprBuf[light_level][0], BrightBuf);
    }
}


int MapBright(void)
{
    int i;

    int temp_DayLight = 0;
    int		diff_Time = 0;
    if (!g_DestBackBuf) return 0;
    temp_DayLight = DayLightControl;

    if (always_lightOn == 31) {
        DayLightControl = always_lightOn;
    }
    //////////////////// 0313 lkh  ߰  (        ) //////////////////////////
    if ((Hero->condition == CON_DEATH || Hero->hp == 0) && Hero->lostSight != 0)
    {
        Hero->sight = NORMAL_SIGHT;
        Hero->lostSight = 0;
        //	Hero->lightness		= 0;//010530 lsw
        Hero->lostSightTime = 0;
    }

    if (Hero->lostSightTime)			// þ߻    ð       õǾ
    {
        diff_Time = abs((long)(Hero->lostSightTime - g_curr_time));		//      ð         ̽ð
        if (diff_Time > 15)			diff_Time = 15;				//   ̰  15    ̻  ̸    ӱ⸦ 15    Ѽ
    }

    if (Hero->lostSight)									// þ
    {
        if (Hero->lostSightTime <= g_curr_time)					// þ        ȿ ð
        {
            Hero->lostSight = 0;
            Hero->lostSightTime = 0;
            diff_Time = 0;
            Hero->sight = NORMAL_SIGHT;
        }
        if (diff_Time < 15)										//    ð    15    ̳
        {
            Hero->lostSight = (diff_Time * 2);
            if (Hero->lostSight < 0)	Hero->lostSight = 0;
        }
        temp_DayLight = DayLightControl - Hero->lostSight;		//DayLightControl=>0~31
        if (temp_DayLight < 0)	temp_DayLight = 0;
    }
    else if (DayLightControl >= 31)
        return 0;

    // 2. 初始化光照緩衝區 (每一幀都執行，確保 144Hz 絲滑)
    memset(BrightBuf, temp_DayLight, (GAME_SCREEN_XSIZE / 2) * (GAME_SCREEN_YSIZE / 2));

    // 3. 繪製角色光源 (重點：PutCharacterLight 內部需使用 visual_x)
    LPCHARACTER lpCharacter = Hero;
    while (lpCharacter != NULL)
    {
        PutCharacterLight(lpCharacter, diff_Time);
        lpCharacter = lpCharacter->lpNext;
    }

    // 4. 處理動態外部光源 (g_OutLight)
    for (i = 0; i < MAX_OUTLIGHT_; i++)
    {
        if (g_OutLight[i].light > 0)		// Ҹ                  Light    ü
        {
            if (LightCount + 1 >= LIGHT_MAX_) break;

            Light[LightCount].x = g_OutLight[i].x;
            Light[LightCount].y = g_OutLight[i].y;
            Light[LightCount].light = g_OutLight[i].light;
            Light[LightCount].flag = g_OutLight[i].flag;
            Light[LightCount].frame = g_OutLight[i].frame;
            Light[LightCount].terminateFrame = g_OutLight[i].terminateFrame;

            if (Light[LightCount].frame >= Light[LightCount].terminateFrame)
                g_OutLight[i].light -= 2;
            else
                g_OutLight[i].frame++;

            LightCount++;
        }
        else
        {
            memset(&Light[LightCount], 0, sizeof(LIGHT));
        }
    }

    // 5. 將所有光源繪製到 BrightBuf
    for (i = 0; i < LightCount; i++)
    {
        LIGHT* l = &Light[i];
        if (l->light < 0 || l->light > 20) continue;
        if (!BrightSprBuf[l->light][0]) continue;

        // 直接減去平滑後的 Mapx
        PutBright_CPP(l->x - Mapx, l->y - Mapy, BrightSprBuf[l->light][0], BrightBuf);
    }

    // 6. 處理螢火蟲 (之前已優化)
    FireflyBright();

    // 7. 最終混合輸出
    PutMapBright2_CPP(g_DestBackBuf, BrightBuf);

    return 0;
}
