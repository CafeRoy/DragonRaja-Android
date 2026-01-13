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
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include "MapInfo.h"

#include "Dragon.h"
#include "Char.h"
#include "GameProc.h"
#include "Map.h"
#include "Hangul.h"
#include "Tool.h"

#include "DirectSound.h"

#define __HONG_SPRITE_H__
#include "Hong_Sprite.h"
#include "Hong_Light.h"
#include "Hong_Sub.h"

#include "Convert565to555.h"
#include "MouseCursor.h"
#include "Effect.h"
#include "weather.h"

extern CHARACTERLIST		g_CharacterList;

WORD	AddTableR[128];
WORD	AddTableG[256];
WORD	AddTableB[128];

WORD	SubTableR[64];
WORD	SubTableG[128];
WORD	SubTableB[64];

bool	StartLoadCharSpriteDataThreadFlag;
bool	LoadingCharSpriteData;
bool g_bStopWeatherSystem = false; // CSD-CN-031215
bool g_bOffWeatherSystem = false; // CSD-CN-031217

extern bool g_IsThunderStorm; //Eleval 12/08/09 - Thunderstorm
extern bool bLowRain; //Eleval 12/08/09 - Thunderstorm
extern int last_thunder_time; //Eleval 12/08/09 - Thunderstorm
extern int thunder_wait_time; //Eleval 12/08/09 - Thunderstorm

BOOL		Statue_Weather;

void SDL_CheckColorBitMask()
{
	// 確保 g_SDLInfo.cpuRenderSurface 和其 format 是有效的
	if (g_SDLInfo.cpuRenderSurface == nullptr || g_SDLInfo.cpuRenderSurface->format == nullptr) {
		SDL_Log("Error: cpuRenderSurface or its format is null!");
		return; // 或者執行適當的錯誤處理
	}

	SDL_PixelFormat* format = g_SDLInfo.cpuRenderSurface->format;
	SDL_Log("Surface Pixel Format Name: %s", SDL_GetPixelFormatName(format->format));
	SDL_Log("Surface Bits Per Pixel: %d", format->BitsPerPixel);
	SDL_Log("Surface Rmask: 0x%X, Gmask: 0x%X, Bmask: 0x%X, Amask: 0x%X", format->Rmask, format->Gmask, format->Bmask, format->Amask);
	SDL_Log("Surface Rshift: %d, Gshift: %d, Bshift: %d, Ashift: %d", format->Rshift, format->Gshift, format->Bshift, format->Ashift);
	SDL_Log("Surface Rloss: %d, Gloss: %d, Bloss: %d, Aloss: %d", format->Rloss, format->Gloss, format->Bloss, format->Aloss);

	// --- 將下面的硬編碼替換為從 SDL_PixelFormat 結構中獲取的值 ---

	// 直接從 Surface 的 format 結構體中獲取位元遮罩
	_PixelInfo.BitMaskR = g_SDLInfo.cpuRenderSurface->format->Rmask;
	// _PixelInfo.BitMaskG = g_SDLInfo.cpuRenderSurface->format->Gmask; // 如果你需要Gmask和Bmask也應該這樣獲取
	// _PixelInfo.BitMaskB = g_SDLInfo.cpuRenderSurface->format->Bmask;

	// 直接從 Surface 的 format 結構體中獲取右位移量
	_PixelInfo.ShiftRightR = g_SDLInfo.cpuRenderSurface->format->Rshift;
	_PixelInfo.ShiftRightG = g_SDLInfo.cpuRenderSurface->format->Gshift;
	_PixelInfo.ShiftRightB = g_SDLInfo.cpuRenderSurface->format->Bshift;

	// --- 關於 ShiftLeft 的注意事項 ---
	// SDL 的 SDL_PixelFormat 結構中直接提供了 Rshift, Gshift, Bshift。
	// Rshift 表示將 R 分量從像素值中提取出來後，需要右移多少位才能將其對齊到最低位。
	// 你原來的 ShiftLeft 邏輯看起來像是用於將一個 8-bit R/G/B 值轉換回一個 16-bit 像素中的特定位置。
	// 如果你的目的是這個，你可以根據 BytesPerPixel 和 ShiftRightR 來計算 ShiftLeftR。
	// 例如：
	// _PixelInfo.ShiftLeftR = (g_SDLInfo.cpuRenderSurface->format->BitsPerPixel - g_SDLInfo.cpuRenderSurface->format->Rloss) - _PixelInfo.ShiftRightR;
	// 或者更常見的，用於將 8-bit 值轉換為完整像素值的位移：
	// _PixelInfo.ShiftLeftR = _PixelInfo.ShiftRightR - g_SDLInfo.cpuRenderSurface->format->Rloss; // 如果是從0-255轉為0-31
	// _PixelInfo.ShiftLeftG = _PixelInfo.ShiftRightG - g_SDLInfo.cpuRenderSurface->format->Gloss;
	// _PixelInfo.ShiftLeftB = _PixelInfo.ShiftRightB - g_SDLInfo.cpuRenderSurface->format->Bloss;
	//
	// 然而，最簡單和最推薦的方法是：
	// 如果你的目的是將獨立的 R, G, B (0-255) 值組合成一個像素值，**使用 `SDL_MapRGB()` 函式**。
	// 如果你的目的是從一個像素值中提取獨立的 R, G, B (0-255) 值，**使用 `SDL_GetRGB()` 函式**。
	// 這兩個函式會自動處理所有的位元遮罩、位移和位元損失，讓你不需要手動計算這些 `ShiftLeft` 和 `ShiftRight`。

	// 如果你仍然需要手動計算 ShiftLeft 來做其他操作，請確認其用途和公式。
	// 否則，如果你只是為了轉換 RGB 值，可以直接移除手動設定 ShiftLeft。
	// 以下是保留你原有 ShiftLeft 邏輯的改法，但請注意其準確性取決於你原來的意圖：
	_PixelInfo.ShiftLeftR = (g_SDLInfo.cpuRenderSurface->format->BitsPerPixel - g_SDLInfo.cpuRenderSurface->format->Rloss) - _PixelInfo.ShiftRightR;
	_PixelInfo.ShiftLeftG = (g_SDLInfo.cpuRenderSurface->format->BitsPerPixel - g_SDLInfo.cpuRenderSurface->format->Gloss) - _PixelInfo.ShiftRightG;
	_PixelInfo.ShiftLeftB = (g_SDLInfo.cpuRenderSurface->format->BitsPerPixel - g_SDLInfo.cpuRenderSurface->format->Bloss) - _PixelInfo.ShiftRightB;
	// 或者更直接反映你的原始硬編碼邏輯：
	// _PixelInfo.ShiftLeftR = g_SDLInfo.cpuRenderSurface->format->Bloss + g_SDLInfo.cpuRenderSurface->format->Gloss + g_SDLInfo.cpuRenderSurface->format->Rshift; // 這種計算可能更符合你原始的ShiftLeftB為11的意圖
	// 這個部分需要你根據 `_PixelInfo.ShiftLeftX` 實際的用途來仔細確認。
}
void	InitSpriteTransTable()
{

	SDL_CheckColorBitMask();

	WORD	MaskG;
	if (_PixelInfo.BitMaskR == 0x7c00)
	{
		MaskG = 0x1f;
	}
	else
	{
		MaskG = 0x3f;
	}

	//Make ADD Table
	int i;
	for (i = 0; i < 128; i++) {
		if (i < 32)
		{
			AddTableR[i] = i << _PixelInfo.ShiftRightR;
			AddTableB[i] = i << _PixelInfo.ShiftRightB;
		}
		else
		{
			AddTableR[i] = 0x1f << _PixelInfo.ShiftRightR;
			AddTableB[i] = 0x1f << _PixelInfo.ShiftRightB;
		}
	}
	for (i = 0; i < 256; i++) {
		if (i <= MaskG)
		{
			AddTableG[i] = i << _PixelInfo.ShiftRightG;
		}
		else
		{
			AddTableG[i] = MaskG << _PixelInfo.ShiftRightG;
		}
	}

	//Make SUB Table
	for (i = 0; i < 64; i++) {
		if (i < 32)
		{
			SubTableR[i] = 0;
			SubTableB[i] = 0;
		}
		else
		{
			SubTableR[i] = (i - 32) << _PixelInfo.ShiftRightR;
			SubTableB[i] = (i - 32) << _PixelInfo.ShiftRightB;
		}
		//SubTableG[i] = i << _PixelInfo.ShiftRightG;
		//SubTableg[i] = 0;
	}
	for (i = 0; i < 128; i++) {
		if (i < 64)
		{
			SubTableG[i] = 0;
		}
		else
		{
			SubTableG[i] = (i - 64) << _PixelInfo.ShiftRightG;
		}
	}
}

void PutCompressedImage(int x, int y, Spr* sp) {
  int lx, rx, ty, by, IsC = 0;


  g_SDLInfo.RenderMethod(x, y, sp, PUT_NORMAL);

  if (sp == NULL) return;
  if (sp->img == NULL) return;
  if (!g_DestBackBuf) return;
  
  

  int xl = sp->xl;
  int yl = sp->yl;

  if (xl == 0) return;
  if (yl == 0) return;

  x -= sp->ox;
  y -= sp->oy;

  if (x >= SCREEN_WIDTH)
    return;
  else if (x + xl < 0)
    return;

  if (y >= SCREEN_HEIGHT)
    return;
  else if (y + yl < 0)
    return;

  rx = xl;
  lx = 0;
  ty = 0;
  by = yl;

  if (x + xl >= (SCREEN_WIDTH - 1)) {
    rx -= x + xl - (SCREEN_WIDTH - 1);
    IsC = 1;
  }
  if (x < 0) {
    lx = 0 - x;
    x = 0;
    rx -= lx;
    IsC = 1;
  }

  if (y + yl >= SCREEN_HEIGHT) {
    by -= y + yl - SCREEN_HEIGHT;
    IsC = 1;
  }
  if (y < 0) {
    ty = 0 - y;
    y = 0;
    by -= ty;
    IsC = 1;
  }

  if (sp->img == NULL) return;

  if (IsC) {
    PutCmprsImgCliping_CPP(x, y, sp, g_DestBackBuf, dDxSize, lx, rx + 1, ty, by);
  } else {
    PutCmprsImgNOTCliping_CPP(x, y, sp, g_DestBackBuf, dDxSize);
    //PutCmprsImgNOTCliping(x, y, yl, sp->img, g_DestBackBuf);
  }
  
}
void PutCompressedImageVoid(int x, int y, Spr* sp)
{

	int	lx, rx, ty, by, IsC = 0;
	//g_SDLInfo.RenderMethod(x, y, sp, PUT_VOID);
	if (sp == NULL) return;
	if (sp->img == NULL) return;
	if (!g_DestBackBuf) return;
	

	int xl = sp->xl;
	int yl = sp->yl;

	if (xl == 0) return;
	if (yl == 0) return;

	x -= sp->ox;
	y -= sp->oy;


	if (x >= (SCREEN_WIDTH - 2))	return;
	else if (x + xl < 0)			return;

	if (y >= SCREEN_HEIGHT)		return;
	else if (y + yl < 0)			return;

	rx = xl;
	lx = 0;
	ty = 0;
	by = yl;

	if (x + xl >= (SCREEN_WIDTH - 2))
	{
		rx -= x + xl - (SCREEN_WIDTH - 2);
		IsC = 1;
	}
	if (x < 0)
	{
		lx = 0 - x;
		x = 0;
		rx -= lx;
		IsC = 1;
	}

	if (y + yl >= SCREEN_HEIGHT)
	{
		by -= y + yl - SCREEN_HEIGHT;
		IsC = 1;
	}
	if (y < 0)
	{
		ty = 0 - y;
		y = 0;
		by -= ty;
		IsC = 1;
	}

	if (IsC)
		PutCmprsImgVoidCliping_CPP(x, y, sp, g_DestBackBuf,dDxSize, lx, rx, ty, by);
	else
        PutCmprsImgVoidNOTCliping_CPP(x, y, sp, g_DestBackBuf, dDxSize);

}

void PutCompressImageApart(int x, int y, Spr* sp, int sx, int sy, int ex, int ey, int alpha)
{

	int		lx=0, rx=0, ty=0, by=0;

	int xl = sp->xl;
	int yl = sp->yl;

	if (sp == NULL) return;
	if (sp->img == NULL) return;
	/*g_SDLInfo.param1 = sx;
	g_SDLInfo.param2 = sy;
	g_SDLInfo.param3 = ex;
	g_SDLInfo.param4 = ey;
	g_SDLInfo.param5 = alpha;
	g_SDLInfo.RenderMethod(x, y, sp, PUT_APART);
	return;*/

	if (xl == 0) return;
	if (yl == 0) return;

	//	x -= sp->ox;
	//	y -= sp->oy;


	if (x >= ex) return;
	else if (x + xl < sx) return;

	if (y >= ey) return;
	else if (y + yl < sy) return;

	rx = xl;
	lx = sx;
	ty = sy;
	by = yl;

	if (x + xl >= ex)
	{
		rx -= x + xl - ex;
	}
	if (x < sx)
	{
		lx = sx - x;
		x = sx;
		rx -= lx;
	}

	if (y + yl >= ey)
	{
		by -= y + yl - ey;
	}
	if (y < sy)
	{
		ty = sy - y;
		y = sy;
		by -= ty;
	}

	if (alpha > 31)
	{
		PutCmprsImgCliping_CPP(x, y, sp, g_DestBackBuf, dDxSize, lx, rx, ty, by);
	}
	else
	{
		PutCmprsImgClipingBlend565_CPP(x, y, sp, g_DestBackBuf, dDxSize, lx, rx, ty, by, alpha);
	}

}


void PutCompressedImageFX(int x, int y, Spr* sp, DWORD alpha, DWORD op)
{
	int		lx, rx, ty, by, IsC = 0;
	
	g_SDLInfo.param[0] = alpha;
	g_SDLInfo.param[1] = op;
	g_SDLInfo.RenderMethod(x, y, sp, PUT_FX);
	if (sp == NULL) return;
	if (sp->img == NULL) return;
	if (!g_DestBackBuf) return;

	int xl = sp->xl;
	int yl = sp->yl;

	if (xl == 0) return;
	if (yl == 0) return;

	x -= sp->ox;
	y -= sp->oy;



	if (x >= SCREEN_WIDTH) return;
	else if (x + xl < 0) return;

	if (y >= SCREEN_HEIGHT) return;
	else if (y + yl < 0) return;

	rx = xl;
	lx = 0;
	ty = 0;
	by = yl;




	if (x + xl >= SCREEN_WIDTH)
	{
		rx -= x + xl - SCREEN_WIDTH;
		IsC = 1;
	}
	if (x < 0)
	{
		lx = 0 - x;
		x = 0;
		rx -= lx;
		IsC = 1;
	}

	if (y + yl >= SCREEN_HEIGHT)
	{
		by -= y + yl - SCREEN_HEIGHT;
		IsC = 1;
	}
	if (y < 0)
	{
		ty = 0 - y;
		y = 0;
		by -= ty;
		IsC = 1;
	}


	if (IsC) {
		switch (op) {
		case 1://alpha
			PutCmprsImgClipingBlend565_CPP(x, y, sp, g_DestBackBuf, dDxSize, lx, rx, ty, by, alpha);
			break;
		case 2://add
			PutCmprsImgClipingAdd565_CPP(x, y, sp, g_DestBackBuf, dDxSize, lx, rx, ty, by, alpha);
			break;
		case 3://sub
			PutCmprsImgClipingSub565_CPP(x, y, sp, g_DestBackBuf, dDxSize, lx, rx, ty, by, alpha);
			break;
		case 4://H alpha
			PutCmprsImgClipingHBlend565_CPP(x, y, sp, g_DestBackBuf, dDxSize, lx, rx, ty, by);
			break;
		case 5://add not put the same colors between dest and source
			PutCmprsImgClipingAddDifer565_CPP(x, y, sp, g_DestBackBuf, dDxSize, lx, rx, ty, by, alpha);
			break;
		case 6://alpha not put the same colors between dest and source
			PutCmprsImgClipingBlendDifer565_CPP(x, y, sp, g_DestBackBuf, dDxSize, lx, rx, ty, by, alpha);
			break;
		}
	}
	else
	{
		switch (op) {
		case 1://alpha
			PutCmprsImgNOTClipingBlend565_CPP(x, y, sp, g_DestBackBuf, dDxSize, alpha);
			break;
		case 2://add
			PutCmprsImgNOTClipingAdd565_CPP(x, y, sp, g_DestBackBuf, dDxSize, alpha);
			break;
		case 3://sub
			PutCmprsImgNOTClipingSub565_CPP(x, y, sp, g_DestBackBuf, dDxSize, alpha);
			break;
		case 4://H alpha
			PutCmprsImgNOTClipingHBlend565_CPP(x, y, sp, g_DestBackBuf, dDxSize);
			break;
		case 5://add  not put the same colors between dest and source
			PutCmprsImgNOTClipingAddDifer565_CPP(x, y, sp, g_DestBackBuf, dDxSize, alpha);
			break;
		case 6://alpha not put the same colors between dest and source
			PutCmprsImgNOTClipingBlendDifer565_CPP(x, y, sp, g_DestBackBuf, dDxSize, alpha);
			break;
		}

	}

}


void PutCompressedImageShadow(int x, int y, Spr* sp)
{

	int	lx, rx, ty, by, IsC = 0;
	//g_SDLInfo.RenderMethod(x, y, sp, PUT_SHADOW);
	
	if (!g_DestBackBuf) return;
	if (sp == NULL) return;
	if (sp->img == NULL) return;

	int xl = sp->xl;
	int yl = sp->yl;

	x -= sp->ox;
	y -= sp->oy;


	if (x >= SCREEN_WIDTH)			return;
	else if (x + xl < 0)	return;
	if (y >= GAME_SCREEN_YSIZE)			return;
	else if (y + yl < 0)	return;

	rx = xl;
	lx = 0;
	ty = 0;
	by = yl;

	if (x + xl >= SCREEN_WIDTH)
	{
		rx -= x + xl - SCREEN_WIDTH;
		IsC = 1;
	}
	if (x < 0)
	{
		lx = 0 - x;
		x = 0;
		rx -= lx;
		IsC = 1;
	}
	if (y + yl >= GAME_SCREEN_YSIZE)
	{
		by -= y + yl - GAME_SCREEN_YSIZE;
		IsC = 1;
	}
	if (y < 0)
	{
		ty = 0 - y;
		y = 0;
		by -= ty;
		IsC = 1;
	}

	if (!IsC) {
        PutCmprsImgNOTClipingShadow565_CPP(x, y, yl, sp->img, g_DestBackBuf, dDxSize);    
		//PutCmprsImgNOTClipingShadow565(x, y, yl, sp->img, g_DestBackBuf);
	}
}

void PutCompressedImageLight_CPP(int x, int y, Spr* sp, void* dest,int destPitch, int alpha) {
  

	if (!g_DestBackBuf) return;
	if (!sp || !sp->img) return;

  int xl = sp->xl;
  int yl = sp->yl;

  x -= sp->ox;
  y -= sp->oy;

  // 完整性剔除 (Culling)
  if (x >= SCREEN_WIDTH || (x + xl) <= 0 || y >= SCREEN_HEIGHT ||
      (y + yl) <= 0) {
    return;
  }

  // 裁剪計算 (Clipping)
  int lx = 0, ty = 0;
  int rx_len = xl, by_len = yl;
  bool IsC = false;

  if (x + xl > SCREEN_WIDTH) {
    rx_len = SCREEN_WIDTH - x;
    IsC = true;
  }
  if (x < 0) {
    lx = -x;
    x = 0;
    IsC = true;
  }

  if (y + yl > SCREEN_HEIGHT) {
    by_len = SCREEN_HEIGHT - y;
    IsC = true;
  }
  if (y < 0) {
    ty = -y;
    by_len -= ty;
    y = 0;
    IsC = true;
  }

  if (rx_len <= 0 || by_len <= 0) return;

  // 確保 alpha 值在合理範圍內 (例如 0-16)
  alpha = min(16, max(0, alpha));

  if (IsC) {
    PutCmprsImgClipingLight_CPP(x, y, sp, dest, destPitch, lx, rx_len, ty,by_len, alpha);
  } else {
    PutCmprsImgNOTClipingLight_CPP(x, y, sp, dest, destPitch, alpha);
  }
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
void PutCompressedImageBlendRGB(int x, int y, Spr* sp, DWORD RGB)
{

	int	lx, rx, ty, by, IsC = 0;
//g_SDLInfo.param1 = RGB;
	//g_SDLInfo.RenderMethod(x, y, sp, PUT_BLENDRGB);
	if (sp == NULL) return;
	if (sp->img == NULL) return;
	if (!g_DestBackBuf) return;
	

	int xl = sp->xl;
	int yl = sp->yl;


	x -= sp->ox;
	y -= sp->oy;



	if (x >= SCREEN_WIDTH)			return;
	else if (x + xl < 0)	return;
	if (y >= SCREEN_HEIGHT)			return;
	else if (y + yl < 0)	return;

	rx = xl;
	lx = 0;
	ty = 0;
	by = yl;

	if (x + xl >= SCREEN_WIDTH)
	{
		rx -= x + xl - SCREEN_WIDTH;
		IsC = 1;
	}
	if (x < 0)
	{
		lx = 0 - x;
		x = 0;
		rx -= lx;
		IsC = 1;
	}
	if (y + yl >= SCREEN_HEIGHT)
	{
		by -= y + yl - SCREEN_HEIGHT;
		IsC = 1;
	}
	if (y < 0)
	{
		ty = 0 - y;
		y = 0;
		by -= ty;
		IsC = 1;
	}

	if (IsC)	PutCmprsImgClipingBlendRGB565_CPP(x, y, sp, g_DestBackBuf,dDxSize, lx, rx, ty, by, RGB);
	else 	PutCmprsImgNOTClipingBlendRGB565_CPP(x, y,sp , g_DestBackBuf,dDxSize,RGB);

}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
void PutCompressedImageCharRGB(int x, int y, Spr* sp, DWORD RGB)
{

	int	lx, rx, ty, by, IsC = 0;

	g_SDLInfo.param[0] = RGB;
	g_SDLInfo.RenderMethod(x, y, sp, PUT_CHARRGB);

	if (!g_DestBackBuf) return;
	if (sp == NULL) return;
	if (sp->img == NULL) return;
	int xl = sp->xl;
	int yl = sp->yl;

	x -= sp->ox;
	y -= sp->oy;



	if (x >= SCREEN_WIDTH)			return;
	else if (x + xl < 0)	return;
	if (y >= SCREEN_HEIGHT)			return;
	else if (y + yl < 0)	return;

	rx = xl;
	lx = 0;
	ty = 0;
	by = yl;

	if (x + xl >= SCREEN_WIDTH)
	{
		rx -= x + xl - SCREEN_WIDTH;
		IsC = 1;
	}
	if (x < 0)
	{
		lx = 0 - x;
		x = 0;
		rx -= lx;
		IsC = 1;
	}
	if (y + yl >= SCREEN_HEIGHT)
	{
		by -= y + yl - SCREEN_HEIGHT;
		IsC = 1;
	}
	if (y < 0)
	{
		ty = 0 - y;
		y = 0;
		by -= ty;
		IsC = 1;
	}

	if (IsC)		PutCmprsImgClipingCharRGB565_CPP(x, y, sp, g_DestBackBuf,dDxSize, lx, rx, ty, by, RGB);
	else 		PutCmprsImgNOTClipingCharRGB565_CPP(x, y, sp, g_DestBackBuf,dDxSize, RGB);

}
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@   RGB Diferent...


int tilewavetable[400] = { 2, 2, 2, 2, 4, 4, 4, 4, 6, 6, 6, 6, 8, 8, 8, 8, 8,10,10,10,
10,10, 8, 8, 8, 8, 8, 6, 6, 6, 6, 6, 4, 4, 4, 4, 2, 2, 2, 2,
2, 2, 2, 4, 4, 4, 4, 6, 6, 6, 6, 8, 8, 8, 8, 8,10,10,10,10,
10, 8, 8, 8, 8, 8, 6, 6, 6, 6, 6, 4, 4, 4, 4, 2, 2, 2,
2,2,2,2,4,4,4,4,6,6,6,6,8,8,8,8,8,10,10,10,10,10,8,8,8,8,8,6,6,6,6,6,4,4,4,4,2,2,2,
2,2,2,2,4,4,4,4,6,6,6,6,8,8,8,8,8,10,10,10,10,10,8,8,8,8,8,6,6,6,6,6,4,4,4,4,2,2,2,
2,2,2,2,4,4,4,4,6,6,6,6,8,8,8,8,8,10,10,10,10,10,8,8,8,8,8,6,6,6,6,6,4,4,4,4,2,2,2,
2,2,2,2,4,4,4,4,6,6,6,6,8,8,8,8,8,10,10,10,10,10,8,8,8,8,8,6,6,6,6,6,4,4,4,4,2,2,2,
};

void PutCompressedImageWaveTile(int x, int y, Spr* sp)
{

	int		lx, rx, ty, by, IsC = 0;
	//g_SDLInfo.RenderMethod(x, y, sp, PUT_WAVTILE);
	if (sp == NULL) return;
	if (sp->img == NULL) return;
	if (!g_DestBackBuf) return;
	

	int xl = sp->xl;
	int yl = sp->yl;

	if (xl == 0) return;
	if (yl == 0) return;

	x -= sp->ox;
	y -= sp->oy;


	if (x >= GAME_SCREEN_XSIZE) return;
	else if (x + xl < 0) return;

	if (y >= GAME_SCREEN_YSIZE) return;
	else if (y + yl < 0) return;

	rx = xl;
	lx = 0;
	ty = 0;
	by = yl;



	if (x + xl >= GAME_SCREEN_XSIZE)
	{
		rx -= x + xl - GAME_SCREEN_XSIZE;
		IsC = 1;
	}
	if (x < 0)
	{
		lx = 0 - x;
		x = 0;
		rx -= lx;
		IsC = 1;
	}

	if (y + yl >= GAME_SCREEN_YSIZE)
	{
		by -= y + yl - GAME_SCREEN_YSIZE;
		IsC = 1;
	}
	if (y < 0)
	{
		ty = 0 - y;
		y = 0;
		by -= ty;
		IsC = 1;
	}


	if (IsC)
	{
		PutCmprsImgClipingWaveTile_CPP(x, y, sp, g_DestBackBuf, dDxSize, lx, rx + 1, ty, by);
	}
	else
	{
		PutCmprsImgNOTClipingWaveTile_CPP(x, y, sp, g_DestBackBuf, dDxSize);
	}

}


























//#####################################################################################
//  這是一個簡單的雜湊演算法 (Fowler-Noll-Vo hash function)
//  它會將字串轉換成一個唯一的32位元無符號整數
//  此函式在編譯時就會被執行
uint32_t fnv1a_hash(const std::string& str) {
  const uint32_t fnv_prime = 16777619u;
  uint32_t hash = 2166136261u;
  for (char c : str) {
    hash ^= static_cast<uint32_t>(c);
    hash *= fnv_prime;
  }
  return hash;
}

//#####################################################################################
//#####################################################################################

//  Character Sprite  Loading....

//#####################################################################################
//#####################################################################################
//#####################################################################################
bool readFileIntoVector(const std::string& filename, std::vector<unsigned char>& buffer) {
	std::ifstream file(filename, std::ios::binary | std::ios::ate);
	if (!file) {
		return false;
	}

	std::streamsize file_size = file.tellg();
	file.seekg(0, std::ios::beg);

	buffer.resize(file_size);
	if (!file.read(reinterpret_cast<char*>(buffer.data()), file_size)) {
		return false;
	}
	return true;
}

int LoadSprFile_V2(const std::string& filename, DWORD& len, unsigned char** buf, CharacterSprite* charactersprite)
{
    // [Android] 1. 路徑修正

    if(g_GameInfo.err_file.count(filename)){
        return 0;
    }
    std::string path = filename;
    if (path.find("./") == 0) path = path.substr(2);

    // [Android] 2. 開啟檔案 (支援 APK Assets)
    SDL_RWops* fp = SDL_RWFromFile(path.c_str(), "rb");
    if (!fp) {
        SDL_Log("Error: Could not open file %s", path.c_str());
        g_GameInfo.err_file.try_emplace(filename, 1);
        return 0;
    }

    // 3. 讀取 Sprite 數量 (Header)
    short no = 0;
    if (SDL_RWread(fp, &no, sizeof(short), 1) == 0) {
        SDL_Log("Error: File %s is too small.", path.c_str());
        SDL_RWclose(fp);
        return 0;
    }

    if (no <= 0) {
        SDL_Log("Error: Invalid sprite count (%d) in file %s.", no, path.c_str());
        SDL_RWclose(fp);
        return 0;
    }

    // 4. 跳過 Offset Table
    // 原邏輯跳過 no * 4 bytes
    SDL_RWseek(fp, no * 4, RW_SEEK_CUR);

    // 5. 計算並分配緩衝區
    // 原始公式: file_buffer.size() - (no * 4 * 2) - 2
    // 在這裡我們用 SDL_RWsize 取得檔案總大小
    Sint64 totalFileSize = SDL_RWsize(fp);
    size_t image_data_length = (size_t)(totalFileSize - (no * 4 * 2) - 2);
    len = (DWORD)image_data_length;

    // 清除舊緩衝區 (如果有的話)
    if (*buf) {
        delete[] *buf;
        *buf = nullptr;
    }

    // 分配新緩衝區
    *buf = new (std::nothrow) unsigned char[image_data_length];
    if (!*buf) {
        SDL_Log("Error: Failed to allocate memory (%zu bytes) for file %s.", image_data_length, path.c_str());
        SDL_RWclose(fp);
        return 0;
    }

    DWORD nOff = 0; // Buffer 內的寫入偏移量

    // 6. 讀取 Sprite 資料 (8 方向迴圈)
    // 這裡維持原始邏輯：8 個方向，每個方向有 no/8 張圖
    int spritesPerDir = no / 8;

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < spritesPerDir; j++)
        {
            Spr* s = &charactersprite->sp[i][j];

            // 讀取該區塊的大小 (Block Size)
            uint32_t size_from_file = 0;
            if (SDL_RWread(fp, &size_from_file, sizeof(uint32_t), 1) == 0) {
                SDL_Log("Error: Unexpected EOF reading size for sprite [%d][%d] in %s", i, j, path.c_str());
                break;
            }

            if (size_from_file == 0) {
                s->xl = 0; s->yl = 0; s->ox = 0; s->oy = 0; s->size = 0; s->img = nullptr;
                continue;
            }

            // 讀取 Sprite Header (座標與大小)
            SDL_RWread(fp, &(s->xl), sizeof(short), 1);
            SDL_RWread(fp, &(s->yl), sizeof(short), 1);
            SDL_RWread(fp, &(s->ox), sizeof(short), 1);
            SDL_RWread(fp, &(s->oy), sizeof(short), 1);
            SDL_RWread(fp, &(s->size), sizeof(uint32_t), 1);

            // 邊界檢查：確保不會寫入超過分配的緩衝區
            if (nOff + s->size > image_data_length) {
                SDL_Log("Error: Image data for sprite [%d][%d] exceeds buffer size in %s.", i, j, path.c_str());
                break;
            }

            // 直接將像素資料讀入 buf 的正確位置
            // 這裡取代了原本的 memcpy，效率更高
            size_t readBytes = SDL_RWread(fp, *buf + nOff, 1, s->size);

            if (readBytes != s->size) {
                SDL_Log("Error: Unexpected EOF reading pixel data for sprite [%d][%d] in %s", i, j, path.c_str());
                break;
            }

            // 設定 Spr 結構的 img 指標
            s->img = (char*)*buf + nOff;

            // 推進 Buffer 偏移量
            // 注意：這裡根據原始邏輯，nOff 是由 s->size (純像素大小) 累加的
            // 而不是 size_from_file (Block大小)
            // 但通常在這種格式裡，size_from_file 會等於 sizeof(Header) + s->size
            // 這裡我們只關心像素資料的緊湊排列
            nOff += size_from_file;
        }
    }

    SDL_RWclose(fp);
    return (int)image_data_length;
}

bool checkSpriteFileLength(const std::string& filename, DWORD sprfilelength) {
  // === 程式碼中不會有明文字串，只有它們的雜湊值和對應的檔案長度 ===
  // 這裡的 map 是 static const，只會在第一次呼叫時建立一次。
  static const std::unordered_map<uint32_t, long> fileChecks = {
      {fnv1a_hash("120000"), 1857214}, {fnv1a_hash("121000"), 4861144},
      {fnv1a_hash("015000"), 2279186}, {fnv1a_hash("048000"), 1485521},
      {fnv1a_hash("122000"), 2211052}, {fnv1a_hash("123000"), 1773206}};

  // 取得檔案代碼並計算雜湊值
  const std::string fileCode = filename.substr(7, 6);
  const uint32_t hash_of_fileCode = fnv1a_hash(fileCode);

  // 在 map 中查找
  const auto it = fileChecks.find(hash_of_fileCode);

  // 如果找到了對應的雜湊值
  if (it != fileChecks.end()) {
    const long expectedLength = it->second;

    if (sprfilelength != expectedLength) {
      JustMsg(lan->OutputMessage(6, 51), filename.c_str());
      ExitApplication(EA_LOAD_CHAR_SPR_DATA_SUB);
      return false;  // 代表檢查失敗
    }
  }

  return true;  // 代表檢查成功或無須檢查
}

int LoadCharSpriteData_Sub(char* filename, char** buf, DWORD* length, CharacterSprite* charactersprite, int mode)
{
	FILE* fp;
	short no;
	DWORD sprfilelength, size;
	DWORD nOff = 0;
	int   i = 0, j = 0;
	Spr* s = { 0, };

#ifdef _SDL
    if (*buf != NULL) return 0;
	std::string file(filename);
	LoadSprFile_V2(file, *length, (unsigned char**) buf, charactersprite);
	checkSpriteFileLength(filename, *length);
	g_SDLInfo.CreateOutlinePoints(charactersprite);// Create outline points for character sprite
	g_SDLInfo.CreateAtlasForCharacter(filename, charactersprite, CHARACTER_SPRITE_FRAME_MAX_);
	return 1;
#endif
/*
	fp = Fopen(filename, "rb");
	if (fp == NULL) return 0;

	fread(&no, 2, 1, fp);
	fseek(fp, no * 4, SEEK_CUR);  // ������ ��ŵ
	*length = sprfilelength = _filelength(_fileno(fp)) - no * 4 * 2 - 2;


	if (filename[7] == '1' && filename[8] == '2' && filename[9] == '0' && filename[10] == '0' && filename[11] == '0' && filename[12] == '0') {
		if (sprfilelength != 1857214) {
			JustMsg(lan->OutputMessage(6, 51), filename, i + 1, j);
			ExitApplication(EA_LOAD_CHAR_SPR_DATA_SUB);
			return 0;
		}
	}

	if (filename[7] == '1' && filename[8] == '2' && filename[9] == '1' && filename[10] == '0' && filename[11] == '0' && filename[12] == '0') {
		if (sprfilelength != 4861144) {
			JustMsg(lan->OutputMessage(6, 51), filename, i + 1, j);
			ExitApplication(EA_LOAD_CHAR_SPR_DATA_SUB);
			return 0;
		}
	}

	if (filename[7] == '0' && filename[8] == '1' && filename[9] == '5' && filename[10] == '0' && filename[11] == '0' && filename[12] == '0') {
		if (sprfilelength != 2279186) {
			JustMsg(lan->OutputMessage(6, 51), filename, i + 1, j);
			ExitApplication(EA_LOAD_CHAR_SPR_DATA_SUB);
			return 0;
		}
	}

	if (filename[7] == '0' && filename[8] == '4' && filename[9] == '8' && filename[10] == '0' && filename[11] == '0' && filename[12] == '0') {
		if (sprfilelength != 1485521) {
			JustMsg(lan->OutputMessage(6, 51), filename, i + 1, j);
			ExitApplication(EA_LOAD_CHAR_SPR_DATA_SUB);
			return 0;
		}
	}

	if (filename[7] == '1' && filename[8] == '2' && filename[9] == '2' && filename[10] == '0' && filename[11] == '0' && filename[12] == '0') {
		if (sprfilelength != 2211052) {
			JustMsg(lan->OutputMessage(6, 51), filename, i + 1, j);
			ExitApplication(EA_LOAD_CHAR_SPR_DATA_SUB);
			return 0;
		}
	}


	if (filename[7] == '1' && filename[8] == '2' && filename[9] == '3' && filename[10] == '0' && filename[11] == '0' && filename[12] == '0') {
		if (sprfilelength != 1773206) {
			JustMsg(lan->OutputMessage(6, 51), filename);
			ExitApplication(EA_LOAD_CHAR_SPR_DATA_SUB);
			return 0;
		}
	}

	if (*buf != NULL)  return 0;
	MemFree(*buf);		// 0201 YGI
	MemAlloc(*buf, sprfilelength);

	if (mode == LD_CHAR_SPRITE_ONLY_MEM_ALLOC)
	{
		fclose(fp);
		return 0;
	}


	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < no / 8; j++)
		{
			s = &charactersprite->sp[i][j];
			fread(&size, 4, 1, fp);
			if (size == 0)
			{
				s->xl = 0;
				s->yl = 0;
				s->size = 0;
				s->img = NULL;
				continue;
			}

			fread(&(s->xl), sizeof(short), 1, fp);
			fread(&(s->yl), sizeof(short), 1, fp);
			//s->yl ++;		
			fread(&(s->ox), sizeof(short), 1, fp);
			fread(&(s->oy), sizeof(short), 1, fp);
			fread(&(s->size), sizeof(unsigned int), 1, fp);

			if (nOff + s->size >= sprfilelength)
			{
				//				JustMsg( " [%s]�� ȭ�Ͽ� %d���� %d�� \nspriteȭ�Ͽ� ������ �ִµ� �մϴ�.\n\n Check�Ͽ��ֽʽÿ�.\n �����մϴ�."
				JustMsg(lan->OutputMessage(6, 51),
					filename, i + 1, j);
				ExitApplication(EA_LOAD_CHAR_SPR_DATA_SUB);
			}
			fread(*buf + nOff, s->size, 1, fp);
			s->img = *buf + nOff;

			convert565to555_LoadSprite(s); //Todo finished

			nOff += size;
		}
	}

	fclose(fp);*/
	return sprfilelength;
}





int LoadCharSpriteData(char* filename, char** buf, DWORD* length, CharacterSprite* charactersprite, int mode)
{
	return LoadCharSpriteData_Sub(filename, buf, length, charactersprite, mode);
}




void FreeCharacterSpriteData(char* buf)
{
	if (buf == NULL) return;
	MemFree(buf);
}

void FreeCharacterSpriteDataAll(int s, int e)
{
	for (int i = s; i < e; i++)
	{
		MemFree(CharBuf[i]);
		CharBuf[i] = nullptr;
		CharBufCount[i] = 0;

		for (int j = 0; j < MAX_CHARACTOR_PART_; j++)
		{
			for (int k = 0; k < MAX_CHARACTER_LAYER_; k++)
			{
				MemFree(CharLayerBuf[i][j][k]);
				CharLayerBuf[i][j][k] = nullptr;
				CharLayerBufCount[i][j][k] = 0;
			}
		}
	}
}

void LoadHeroClothAccessoryData(int s, int e)
{
	int i;
	char tempfilename[FILENAME_MAX];

	for (i = s; i <= e; i++)
	{
		sprintf(tempfilename, "./char/000%03d.spr", i);
		LoadCharSpriteData(tempfilename, &CharAccessoryBuf[0][i], &CharAccessoryBufLength[0][i], &CharAccessorySpr[0][i], LD_CHAR_SPRITE_ALL_LOAD);
		sprintf(tempfilename, "./char/001%03d.spr", i);
		LoadCharSpriteData(tempfilename, &CharAccessoryBuf[1][i], &CharAccessoryBufLength[1][i], &CharAccessorySpr[1][i], LD_CHAR_SPRITE_ALL_LOAD);
	}
}
void FreeHeroClothAccessoryDataOne(int sprno, int no)
{
	if (no == 0) return;
	if (sprno > 1) return;
	if (CharAccessoryCount[sprno][no] <= 0) return;

	CharAccessoryCount[sprno][no]--;
	if (CharAccessoryCount[sprno][no] <= 0)
	{
		if (CharAccessoryBuf[sprno][no] == NULL) return;
		delete CharAccessoryBuf[sprno][no];
		CharAccessoryBuf[sprno][no] = NULL;
	}
}
void LoadHeroClothAccessoryDataOne(int sprno, int no)
{
	if (no == 0) return;
	if (sprno > 1) return;
	if (no >= CHARACTER_ACCESSORY_MAX_) return;	// 011018 KHS ����	
	if (CharAccessoryBuf[sprno][no]) return;

	char tempfilename[FILENAME_MAX];
	sprintf(tempfilename, "./char/%03d%03d.spr", sprno, no);

	int ret;

	ret = LoadCharSpriteData(tempfilename, &CharAccessoryBuf[sprno][no], &CharAccessoryBufLength[sprno][no], &CharAccessorySpr[sprno][no], LD_CHAR_SPRITE_ALL_LOAD);
}
void FreeHeroClothAccessoryData(void)
{
	int i;
	for (i = 1; i < 104; i++)
	{
		FreeCharacterSpriteData(CharAccessoryBuf[0][i]);
		CharAccessoryBuf[0][i] = NULL;
		CharAccessoryCount[0][i] = 0;
		FreeCharacterSpriteData(CharAccessoryBuf[1][i]);
		CharAccessoryBuf[1][i] = NULL;
		CharAccessoryCount[1][i] = 0;
	}
}

void FreeHeroClothAccessoryDataOne_tool(int sprno, int no)
{	//< CSD-030306	
	if (no == 0) return;
	if (sprno > 1) return;
	if (CharAccessoryCount[sprno][no] <= 0) return;

	CharAccessoryCount[sprno][no]--;
	if (CharAccessoryCount[sprno][no] <= 0)
	{
		if (CharAccessoryBuf[sprno][no] == NULL) return;
		delete CharAccessoryBuf[sprno][no];
		CharAccessoryBuf[sprno][no] = NULL;
	}
}	//> CSD-030306		

void LoadHeroClothAccessoryDataOne_tool(int sprno, int no)
{	//< CSD-030306	
	if (no == 0) return;
	if (sprno > 1) return;
	if (no >= CHARACTER_ACCESSORY_MAX_) return;	// 011018 KHS ����	
	if (CharAccessoryBuf[sprno][no]) return;

	char tempfilename[FILENAME_MAX];
	sprintf(tempfilename, "./char_test/%03d%03d.spr", sprno, no);

	int ret;

	ret = LoadCharSpriteData(tempfilename, &CharAccessoryBuf[sprno][no], &CharAccessoryBufLength[sprno][no], &CharAccessorySpr[sprno][no], LD_CHAR_SPRITE_ALL_LOAD);
}	//> CSD-030306

void LoadHeroSpriteData(void)
{
	LoadCharSpriteData("./char/000000.spr", &CharBuf[0], &CharBufLength[0], &CharSpr[0], LD_CHAR_SPRITE_ALL_LOAD);
	LoadCharSpriteData("./char/001000.spr", &CharBuf[1], &CharBufLength[1], &CharSpr[1], LD_CHAR_SPRITE_ALL_LOAD);
	LoadHeroClothAccessoryData(104, 107);
	LoadHeroClothAccessoryDataOne(0, 138);
	LoadHeroClothAccessoryDataOne(1, 138);
	LoadHeroClothAccessoryDataOne(0, 146);
	LoadHeroClothAccessoryDataOne(1, 146);
	LoadHeroClothAccessoryDataOne(0, 147);
	LoadHeroClothAccessoryDataOne(1, 147);
}
void FreeHeroSpriteData(void)
{
	FreeCharacterSpriteData(CharBuf[0]);
	FreeCharacterSpriteData(CharBuf[1]);
}

// ------------------------------------------------------------------------------
// 全域變數 (Global Variables)
// ------------------------------------------------------------------------------

// 使用結構陣列取代原本的 int 陣列
static WeatherParticle g_RainParticles[MAX_RAIN];
static WeatherParticle g_SnowParticles[MAX_SNOW]; // 確保 MAX_SNOW 定義正確
static WeatherParticle g_WindParticles[MAX_WIND];

static int g_RainActiveCount = 0; // rainmax
static int g_SnowActiveCount = 0; // snowmax
static int g_WindActiveCount = 0; // windmax

static int RainNumber = 0;
static int old_RainNumber = 0;
static bool DrawRain = true;

static int SnowNumber = 0;
static int old_SnowNumber = 0;
static bool DrawSnow = true;

static int WindNumber = 0;

// 外部依賴變數 (假設存在)
extern BOOL Statue_Weather;
extern int DayLightControl;
extern bool g_IsThunderStorm;
extern int last_thunder_time;
extern int thunder_wait_time;
extern bool bLowRain;

// ------------------------------------------------------------------------------
// 輔助函式 (Helper Functions)
// ------------------------------------------------------------------------------

// 判斷是否需要繪製 (Culling)
bool IsVisible(int x, int y, int mapX, int mapY) {
    int screenX = x - mapX;
    int screenY = y - mapY;
    // 寬鬆的邊界檢查，避免邊緣閃爍
    return (screenX >= -50 && screenX <= SCREEN_WIDTH + 50 &&
            screenY >= -50 && screenY <= SCREEN_HEIGHT + 50);
}

// ------------------------------------------------------------------------------
// RAIN 系統實作
// ------------------------------------------------------------------------------

void RainStart(int number, int flag) {
    if (DrawRain) RainNumber = number;
    old_RainNumber = number;

    // Sound logic
    int sound_Volume = number / 9; // 保留原本邏輯
    DayLightControl = 20;
}

void ThunderStart(int number, int flag) {
    if (DrawRain) RainNumber = number;
    old_RainNumber = number;

    g_IsThunderStorm = true;
    DayLightControl = 25;
}

void RainStop(void) {
    RainNumber = 0;
    old_RainNumber = RainNumber;
    StopWave(SOUND_RAINY);
    Statue_Weather = 0;
    DayLightControl = 31;
}

void ThunderStop(void) {
    RainNumber = 0;
    old_RainNumber = RainNumber;
    g_IsThunderStorm = false;
    StopWave(SOUND_RAINY);
    Statue_Weather = 0;
    last_thunder_time = -1;
    thunder_wait_time = 0;
    bLowRain = false;
    DayLightControl = 31;
}

void ToggleRainHide(bool flag) {
    DrawRain = !flag;
    RainNumber = flag ? 0 : old_RainNumber;
}

// 計算適合當前解析度的粒子數量
int GetOptimalParticleCount(int inputNumber)
{
    // 如果系統沒開天氣，直接回傳 0
    if (inputNumber == 0) return 0;

    // 1. 計算當前螢幕面積與基準面積的比例
    // 使用 float 避免整數除法導致精度丟失
    // (例如 1920x1080 的面積大約是 1024x768 的 2.6 倍)
    float screenFactor = (float)(SCREEN_WIDTH * SCREEN_HEIGHT) / (float)(BASE_RES_WIDTH * BASE_RES_HEIGHT);

    // 2. 根據輸入的強度 (RainNumber/SnowNumber) 計算基礎數量
    // 原本邏輯：RainNumber 最大通常是 900
    // 我們假設輸入的 inputNumber 是基於「基準解析度」的期望值

    int targetCount = (int)(inputNumber * screenFactor);

    // 3. 安全限制：絕對不能超過我們宣告的陣列大小 (BUFFER)
    // 這裡假設 Rain 和 Snow 的 Buffer 大小一樣
    if (targetCount >= MAX_WEATHER_BUFFER) targetCount = MAX_WEATHER_BUFFER;

    return targetCount;
}

// 為了保持簽名相容，保留此函式，但內部調用新的 Reset
void RainAnimationSub(int i) {
    if (i >= 0 && i < MAX_RAIN) {
        g_RainParticles[i].Reset(WEATHER_RAIN, 0); // Rain 不依賴 MapY 進行初始化判斷
    }
}

void RainAnimationSetting(void) {
    // [修改點] 計算適合當前解析度的目標數量
    int realTargetCount = GetOptimalParticleCount(RainNumber);

    if (realTargetCount == 0 && g_RainActiveCount == 0) return;

    if (realTargetCount != g_RainActiveCount) {
        if (realTargetCount > g_RainActiveCount) {
            int newCount = g_RainActiveCount + 10;
            if (newCount > realTargetCount) newCount = realTargetCount;

            // 注意：這裡確保不要超過我們宣告的 Array 大小
            if (newCount > MAX_RAIN) newCount = MAX_RAIN;

            // 初始化新增粒子
            for (int i = g_RainActiveCount; i < newCount; i++) {
                // 微調：移除了原本重複的 Reset 呼叫，並修正初始化位置
                g_RainParticles[i].x = Random(SCREEN_WIDTH + 200) - 100;
                g_RainParticles[i].y = Random(SCREEN_HEIGHT) - SCREEN_HEIGHT; // 從上方落下
                g_RainParticles[i].endY = Random(680);
                g_RainParticles[i].Reset(WEATHER_RAIN, 0);
            }
            g_RainActiveCount = newCount;
        }
        else {
            g_RainActiveCount -= 10;
            if (g_RainActiveCount < realTargetCount) g_RainActiveCount = realTargetCount;
        }
    }

    for (int i = 0; i < g_RainActiveCount; i++) {
        WeatherParticle& p = g_RainParticles[i];

        if (p.lifeState == 0) {
            // 【刪除位移代碼】不再這裡寫 p.x += ...
            if (p.y > p.endY) {
                p.lifeState = 2; // 切換到觸地狀態
            }
        }
        else {
            // 觸地計時器遞減
            p.lifeState--;
            if (p.lifeState == 0) {
                // 重生邏輯：這屬於「邏輯行為」，留在這裡
                p.x = Random(SCREEN_WIDTH + 200) - 100 + Mapx; // 基於當前位置重生
                p.y = (float)Mapy - Random(SCREEN_HEIGHT);
                p.endY = (float)Mapy + Random(SCREEN_HEIGHT);
                p.Reset(WEATHER_RAIN, 0);
            }
        }
    }
}

void RainAnimationOutput(void) {
    if (g_SnowActiveCount > 0) return; // 互斥顯示

    for (int i = 0; i < g_RainActiveCount; i++) {
        WeatherParticle& p = g_RainParticles[i];

        // 優化: 視錐剔除 (Culling)
        // 假設雨是 Screen Space，不需要減 MapX/MapY (如果不是，請自行減去)
        if (p.x < -50 || p.x > SCREEN_WIDTH + 50 || p.y > SCREEN_HEIGHT + 50) continue;

        if (p.lifeState > 0) {
            // 觸地水花 (87, 86)
            int sprIndex = (p.lifeState == 1) ? 87 : 86;
            PutCompressedImageFX(p.x, p.y, &spr[sprIndex], p.alpha, 2);
        }
        else {
            // 雨滴
            PutCompressedImageFX(p.x, p.y, &spr[SPRITE_RAIN_BASE + p.spriteIdx], p.alpha, 2);
        }
    }
}

// ------------------------------------------------------------------------------
// SNOW 系統實作
// ------------------------------------------------------------------------------
// 雨滴平滑更新
void UpdateRainSmoothly(float dt) {
    // dt * 17.0f 是為了將速度校準回原本 17Hz 時代的體感速度
    float multiplier = dt * 17.0f;

    for (int i = 0; i < g_RainActiveCount; i++) {
        WeatherParticle& p = g_RainParticles[i];
        if (p.lifeState == 0) {
            p.x += p.speedX * multiplier;
            p.y += p.speedY * multiplier;
        }
    }
}

// 雪花平滑更新
void UpdateSnowSmoothly(float dt) {
    float multiplier = dt * 17.0f; // 校準回原本 17Hz 的速度感

    for (int i = 0; i < g_SnowActiveCount; i++) {
        WeatherParticle& p = g_SnowParticles[i];
        if (p.y < p.endY) {
            // 如果是平靜狀態，給一點點隨機擺動，但要乘以 multiplier
            float drift = (p.driftTimer > 0) ? -((p.speedY * 0.5f) + 2.5f) : 0.3f;

            p.x += drift * multiplier;
            p.y += p.speedY * multiplier;
        }
    }
}

void SnowStart(int no, int flag)
{
    // 強制關閉雷雨標記 (Fix: 下雪時不應聽到雷聲)
    g_IsThunderStorm = false;

    // 如果下雪時也不應該有雨聲，建議加上這行：
    StopWave(SOUND_RAINY);

    SnowNumber = 0;

    // 這些是地圖排除邏輯 (保持原樣)
    if (strcmp(MapName, "FIREDUN1") == 0) return;
    if (strcmp(MapName, "ICE-W01") == 0) return;
    if (strcmp(MapName, "MANDUN") == 0) return;

    if (DrawSnow) SnowNumber = no;
    old_SnowNumber = no;
    Statue_Weather = 1;
}

void ToggleSnowHide(bool flag) {
    DrawSnow = !flag;
    SnowNumber = flag ? 0 : old_SnowNumber;
}

void SnowStop(void) {
    SnowNumber = 0;
    old_SnowNumber = SnowNumber;
    Statue_Weather = 0;
}

void SnowAnimationSub(int my, int i) {
    if (i >= 0 && i < MAX_SNOW) {
        g_SnowParticles[i].Reset(WEATHER_SNOW, my);
    }
}

void SnowAnimationSetting(void)
{
    // [修改點 1] 計算當前解析度下，真正需要的「最大數量」
    int realTargetCount = GetOptimalParticleCount(SnowNumber);

    if (realTargetCount == 0 && g_SnowActiveCount == 0) return;

    // [修改點 2] 所有的比較邏輯都改用 realTargetCount
    if (realTargetCount != g_SnowActiveCount) {
        int mapstarty = g_Map.y * 32;
        int mapstartx = g_Map.x * 32;

        if (realTargetCount > g_SnowActiveCount) {
            // 漸增
            // 注意：這裡使用 MAX_SNOW (現在是 4000) 作為硬上限
            int newCount = min(g_SnowActiveCount + 10, MAX_SNOW);

            // 軟上限：不能超過當前解析度需要的數量
            if (newCount > realTargetCount) newCount = realTargetCount;

            for (int i = g_SnowActiveCount; i < newCount; i++) {
                g_SnowParticles[i].x = mapstartx + Random(SCREEN_WIDTH + 200) - 100;
                g_SnowParticles[i].y = mapstarty + Random(100) - 100;
                g_SnowParticles[i].endY = mapstarty + Random(SCREEN_HEIGHT);
                g_SnowParticles[i].Reset(WEATHER_SNOW, mapstarty);
            }
            g_SnowActiveCount = newCount;
        }
        else {
            // 漸減
            g_SnowActiveCount = max(g_SnowActiveCount - 10, realTargetCount);
        }
    }

    for (int i = 0; i < g_SnowActiveCount; i++) {
        WeatherParticle& p = g_SnowParticles[i];

        // 處理風向計時器（這屬於邏輯狀態）
        if (p.driftTimer == 0) {
            if (Random(200) == 0) p.driftTimer = 60 + Random(60);
        }
        else {
            p.driftTimer--;
        }

        // 處理觸地後重生
        if (p.y >= p.endY) {
            if (p.animationPhase == 0) {
                p.animationPhase = 1;
            }
            else {
                p.alpha--;
                if (p.alpha < 0) {
                    // 重生在「當前視覺中心」的周圍
                    p.x = (float)Mapx + Random(SCREEN_WIDTH + 400) - 200;
                    p.y = (float)Mapy - Random(200);
                    p.endY = (float)Mapy + Random(SCREEN_HEIGHT);
                    p.Reset(WEATHER_SNOW, (int)Mapy);
                }
            }
        }
    }
}

void SnowAnimationOutput(void) {
    // 【關鍵】不要使用 g_Map.x * 32，必須使用平滑後的 Mapx
    int camX = Mapx;
    int camY = Mapy;

    for (int i = 0; i < g_SnowActiveCount; i++) {
        WeatherParticle& p = g_SnowParticles[i];

        // 這裡減去平滑相機座標，雪花就會穩穩地「種」在地圖上
        int screenX = (int)p.x - camX;
        int screenY = (int)p.y - camY;

        // 簡單的視錐剔除
        if (screenX < -50 || screenX > SCREEN_WIDTH + 50 ||
            screenY < -50 || screenY > SCREEN_HEIGHT + 50) continue;

        PutCompressedImageFX(screenX, screenY,
                             &spr[SPRITE_SNOW_BASE + p.spriteIdx], p.alpha, 2);
    }
}

// ------------------------------------------------------------------------------
// WIND 系統實作 (修復了大量 Copy-Paste 錯誤)
// ------------------------------------------------------------------------------

void WindStart(int number) {
    WindNumber = number;
}

void WindStop(void) {
    WindNumber = 0;
}

void WindAnimationSub(int my, int i) {
    if (i >= 0 && i < MAX_WIND) {
        g_WindParticles[i].Reset(WEATHER_WIND, my);
    }
}

void WindAnimationSetting(void)
{
    // [修改點 1] 解析度適配
    int realTargetCount = GetOptimalParticleCount(WindNumber);

    if (realTargetCount == 0 && g_WindActiveCount == 0) return;

    // [修改點 2] 數量管理 (修復了原代碼判斷 RainNumber 的錯誤)
    if (realTargetCount != g_WindActiveCount) {
        int mapstarty = g_Map.y * 32;
        int mapstartx = g_Map.x * 32;

        if (realTargetCount > g_WindActiveCount) {
            // 漸增
            int newCount = g_WindActiveCount + 10;
            if (newCount > realTargetCount) newCount = realTargetCount;
            if (newCount > MAX_WIND) newCount = MAX_WIND;

            for (int i = g_WindActiveCount; i < newCount; i++) {
                // 初始化
                g_WindParticles[i].x = mapstartx + Random(SCREEN_WIDTH + 200) - 100;
                g_WindParticles[i].y = mapstarty + Random(100) - 100;
                g_WindParticles[i].endY = mapstarty + Random(SCREEN_HEIGHT);
                g_WindParticles[i].Reset(WEATHER_WIND, mapstarty);
                g_WindParticles[i].active = 1;
            }
            g_WindActiveCount = newCount;
        }
        else {
            // 漸減
            g_WindActiveCount -= 10;
            if (g_WindActiveCount < realTargetCount) g_WindActiveCount = realTargetCount;
        }
    }

    // 風向控制邏輯
    static int windCooldown = 0;
    bool triggerWind = false;

    // 原代碼邏輯：50% 機率設置 cooldown
    if (Random(100) == 50) {
        windCooldown = 40;
    }

    if (windCooldown > 0) {
        windCooldown--;
        if (windCooldown == 0) {
            triggerWind = true;
        }
    }

    int mapstartx = g_Map.x * 32;
    int mapstarty = g_Map.y * 32;

    // 視窗邊界 (用於循環)
    int viewLeft = mapstartx - 150;
    int viewRight = mapstartx + SCREEN_WIDTH + 150;

    for (int i = 0; i < g_WindActiveCount; i++) {
        WeatherParticle& p = g_WindParticles[i];

        // 觸發強風 (原代碼邏輯)
        if (triggerWind) {
            p.driftTimer = Random(30) + 30; // windwind
        }

        if (p.y < p.endY) {
            // --- 移動邏輯 ---
            if (p.driftTimer > 0) { // windwind > 0
                p.driftTimer--;
                p.x -= (Random(10) + 20); // 強風向左吹
            }
            else {
                p.x += (Random(5) - 2);   // 平常隨機飄
            }

            p.y += p.speedY; // 往下落

            // --- [新增] 循環機制 (Wrap Around) ---
            // 這樣落葉/風沙才不會被吹光
            if (p.x < viewLeft) {
                p.x = viewRight;
                p.y = mapstarty + Random(SCREEN_HEIGHT);
                p.endY = mapstarty + Random(SCREEN_HEIGHT);
            }
            else if (p.x > viewRight) {
                p.x = viewLeft;
            }
        }
        else if (p.animationPhase == 0) { // windflag == 0
            // 觸地開始淡出
            p.animationPhase = 1;
            p.alpha += 2;
            if (p.alpha > 15) p.alpha = 15;
        }
        else {
            // 淡出中
            p.alpha--;
            if (p.alpha < 0) {
                // 重生
                p.x = mapstartx + Random(SCREEN_WIDTH + 800) - 400;
                p.y = mapstarty + Random(200) - 200;
                p.endY = mapstarty + Random(680);
                p.animationPhase = 0;
                p.driftTimer = 0;
                p.Reset(WEATHER_WIND, mapstarty);
            }
        }
    }
}

void WindAnimationOutput(void)
{
    int mapstartx = g_Map.x * 32;
    int mapstarty = g_Map.y * 32;

    for (int i = 0; i < g_WindActiveCount; i++) {
        WeatherParticle& p = g_WindParticles[i];

        // [新增] 視錐剔除 (Culling)
        // 如果粒子在畫面外，就不繪製
        // p.x 是世界座標，要減去地圖偏移量才是螢幕座標
        int screenX = p.x - mapstartx;
        int screenY = p.y - mapstarty;

        if (screenX < -50 || screenX > SCREEN_WIDTH + 50 ||
            screenY < -50 || screenY > SCREEN_HEIGHT + 50)
            continue;

        PutCompressedImageFX(screenX, screenY, &spr[SPRITE_WIND_BASE + p.spriteIdx], p.alpha, 2);
    }
}
// ------------------------------------------------------------------------------
// 網路封包與系統整合部分 (保持原樣，僅做變數名稱對應)
// ------------------------------------------------------------------------------

DWORD RainTime;
DWORD RainStartTime, RainEndTime;
int RainAmount;

int InitWeather(void) {
    RainStop();
    SnowStop();
    ThunderStop();
    WindStop();
    return 0;
}

void RecvChangeWeather(t_rm_change_weather* p) {
    if (g_bOffWeatherSystem) {
        return;
    }

    g_bStopWeatherSystem = p->bStopWeather ? true : false;

    switch ((int)p->bWeather) {
        case 0: // 晴天
            RainStop();
            SnowStop();
            ThunderStop();
            Statue_Weather = 0;
            break;
        case 1: // 雨天
        {
            DWORD amount = p->dwAmount;
            SnowStop();
            ThunderStop();
            if (amount > 900) amount = 900;
            RainStart(amount, 0);
            Statue_Weather = 1;
        }
            break;
        case 2: // 雪天
            RainStop();
            ThunderStop();
            SnowStart(p->dwAmount, 0);
            Statue_Weather = 1;
            break;
        case 3: // 雷雨
        {
            DWORD amount = p->dwAmount;
            RainStop();
            ThunderStop();
            if (amount > 900) amount = 900;
            ThunderStart(amount, 0);
            Statue_Weather = 1;
        }
            break;
    }
}

short int g_RealDate;
void WeatherDataProcess(DWORD value, DWORD another, DWORD rainstart, DWORD rainend, int amount, int temperature)
{
    g_CurrentTime = value;
    g_ServerClientTimeGap = g_CurrentTime - SDL_GetTicks();
    g_ClientTime = SDL_GetTicks() + g_ServerClientTimeGap;
    g_curr_time = another;

    if (Hero == NULL) return;

    time_t lTime;
    time(&lTime);
    struct tm* today = localtime(&lTime);

    // --- 冬季下雪邏輯 ---
    if (today->tm_mon == 11 && (today->tm_mday >= 22 || today->tm_mday <= 29))
    {
        if (MapInfo[MapNumber].rain)
        {
            if (amount)
            {
                // [FIX] 在開始下雪前，確保雨和雷都停掉
                // 如果不加這兩行，當從非雪區進入雪區，或者天氣切換瞬間，
                // 之前的雷雨狀態可能還殘留著。
                if (g_IsThunderStorm || RainNumber > 0) {
                    RainStop();
                    ThunderStop();
                }

                SnowStart(amount, 0);
                Statue_Weather = 1;
            }
            else
            {
                SnowStop();
            }
        }
    }
        // --- 一般雨天邏輯 ---
    else
    {
        RainTime = 0;
        RainStartTime = rainstart;
        RainEndTime = rainend;

        // 如果現在不是冬季時間，確保雪是停的
        if (SnowNumber > 0) SnowStop();

        if (MapInfo[MapNumber].rain)
        {
            if (0 > rainstart && 0 < rainend)
            {
                amount *= 10;
                if (amount > 900) amount = 900;

                RainAmount = amount;
                RainStart(amount, 0);
                Statue_Weather = 1;
            }
            // else { Statue_Weather = 0; }
        }
    }
    g_RealDate = temperature;
}

#define FRAME_PER_SEC		17

// ani�� 1�ʴ� �����̴� 
void CharWalkTime(LPCHARACTER ch, int ani)
{
	double dist = 0.0;
	double framepertime;

	for (int d = 0; d < ch->AnimationTable[ani].nCount; d++) //�ȱ⿡ ���� �ӵ��˻�...
		dist += (double)ch->AnimationTable[ani].dx[d];

	// dist =  �ѹ� Ani�ϴµ� �̵��ϴ� Dot��.
	// ch->AnimationTable[ani].nCount  = Frame��.
	// framepertime = 32dot(1 Tile)�̵��ϴµ� �ɸ��� Frame��.
	// ch->walktime = 32dot(1 Tile)�̵��ϴµ� �ɸ��� mSEC.
	if (dist)
	{
		framepertime = TILE_SIZE * (double)ch->AnimationTable[ani].nCount / dist;
		ch->walktime = (int)(1000 * framepertime / FRAME_PER_SEC);
	}
}


int CharCalcMovP(LPCHARACTER ch)
{
	int ret = 50;
	int sx, sy, ex, ey;
	DWORD towalktime;

	if ((ch->desttime < g_ClientTime) || g_ClientTime == 0) 
		return ret;

	sx = ch->x / TILE_SIZE;
	sy = ch->y / TILE_SIZE;
	ex = ch->path[ch->pathcount - 1][0];
	ey = ch->path[ch->pathcount - 1][1];

	DWORD d = ch->desttime - g_ClientTime;

	if (ch->desttime < g_ClientTime)
	{
		ch->movp.PutEncrypted(ret);
		return ret;
	}

	//如果移動距離太近,則降低移動速度,防止瞬移,由於人物的圖象是 TILE_SIZE=32,參考函式PlusMovePDisTance
	if (ch->pathcount <= 1) ch->pathcount = 2;

	//點擊的目的地太近的話,降低移動速度,防止滑步和瞬移
	int plus = ch->pathcount <= 3 ? -1 : 1;

	towalktime = d / (ch->pathcount + plus);

	if (towalktime)
	{
		ch->movp.PutEncrypted((ch->walktime * 50 / towalktime));
		ret = ch->movp.GetDecrypted();
	}

	if (ret >= 200) { ret = 200;	ch->movp.PutEncrypted(200); }
	if (ret < 25) { ret = 25;		ch->movp.PutEncrypted(25); }
	//SDL_Log("CharCalcMovP: desttime=%d, g_ClientTime=%d", ch->desttime, g_ClientTime);
	//SDL_Log("CharCalcMovP: movp=%d, walktime=%d, towalktime=%d, pathcount=%d", ret, ch->walktime, towalktime, ch->pathcount);
	return ret;
}

// ============================================================================
// 通用實作函式：讀取角色動畫資料 (Android 版)
// ============================================================================
static void LoadCharacterAnimationData_Impl(LPCHARACTER ch)
{
    char filenameBin[FILENAME_MAX];

    // [Android] 1. 路徑修正：移除開頭的 ./
    // 假設 sprno 是 0-999
    snprintf(filenameBin, FILENAME_MAX, "char/%03dAniTable.bin", ch->sprno);

    // [Android] 2. 強制只讀取 BIN
    // 在手機上無法進行 TXT 解析與寫入 BIN 的流程 (APK 唯讀)

    SDL_RWops* fp = SDL_RWFromFile(filenameBin, "rb");

    if (fp)
    {
        char crc1 = 0;
        char crc = 0;

        // 3. 跳過 Garbage Data (20 bytes)
        SDL_RWseek(fp, 20, RW_SEEK_SET);

        // 4. 讀取 CheckSum
        SDL_RWread(fp, &crc1, 1, 1);

        // 5. 讀取動畫表
        // 注意：ch->AnimationTable 是一個陣列或指針，確保它指向有效的記憶體空間
        SDL_RWread(fp, ch->AnimationTable, sizeof(PCANIMATIONTABLE), MAX_ANIMATION);

        SDL_RWclose(fp);

        // 6. 計算 CheckSum 驗證
        char* tt = (char*)ch->AnimationTable;
        for (int i = 0; i < sizeof(PCANIMATIONTABLE) * MAX_ANIMATION; i++)
        {
            crc += *tt;
            tt++;
        }

        if (abs(crc1 - crc))
        {
            SDL_Log("Error: Animation Data CheckSum Failed! File: %s", filenameBin);
            // 只有在非嚴格模式下才忽略錯誤，或者直接設定錯誤標記
            CheckSumError = CHECKSUM_ERROR_ANIMATIONDATA;
        }
    }
    else
    {
        // 檔案讀取失敗
        SDL_Log("Error: Failed to open %s", filenameBin);

        if (GetSysInfo(SI_GAME_MAKE_MODE)) {
            // 僅在開發模式下提示詳細錯誤
            SDL_Log("CheckNumError : ch->sprno = %d ( No File )", ch->sprno);
        }
        CheckSumError = CHECKSUM_ERROR_ANIMATIONDATA;
    }
}

// ============================================================================
// 原始函式介面 (保留以維持相容性)
// ============================================================================

void LoadChacterAnimationData(LPCHARACTER ch)
{
    LoadCharacterAnimationData_Impl(ch);
}

void LoadChacterAnimationData_Tool(LPCHARACTER ch)
{
    LoadCharacterAnimationData_Impl(ch);
}
void LoadClothOrderData(void)
{
    // 定義要讀取的檔案清單結構
    struct ClothFileDef {
        int index;          // 對應 SpriteOrder 的第幾組
        const char* path;   // 檔案路徑
    };

    // [Android] 路徑已移除 ./
    ClothFileDef files[] = {
            {0, "char/00ClothOrderBasic.bin"},
            {1, "char/01ClothOrderBasic.bin"},
            {2, "char/00ClothOrderBow.bin"},
            {3, "char/01ClothOrderBow.bin"}
    };

    for (const auto& file : files)
    {
        SDL_RWops* fp = SDL_RWFromFile(file.path, "rb");

        if (fp)
        {
            // -------------------------------------------------------------
            // 優化寫法：一次讀取整個區塊 (Block Read)
            // -------------------------------------------------------------
            // 原本的邏輯是 8行 * 75列 = 600 bytes
            // 我們直接讀取 600 bytes 到 SpriteOrder[index] 的記憶體位址
            // 這樣只需要 1 次 IO 操作，而不是 600 次

            // 檢查 SpriteOrder[file.index] 是否為連續記憶體 (二維陣列通常是)
            // 這裡假設 SpriteOrder 是 char[4][8][75] 這樣的靜態陣列
            SDL_RWread(fp, SpriteOrder[file.index], 1, 8 * 75);

            /* 如果你擔心記憶體對齊問題，想保留原本的雙層迴圈邏輯，可以這樣寫：
               for (int i = 0; i < 8; i++) {
                   // 一次讀一行 (75 bytes)
                   SDL_RWread(fp, &SpriteOrder[file.index][i][0], 1, 75);
               }
            */

            SDL_RWclose(fp);
            // SDL_Log("Loaded %s", file.path);
        }
        else
        {
            SDL_Log("Error: Failed to open %s", file.path);
        }
    }
}
/*
void PutTileNotCliping(int x, int y, LPVOID Stmp, LPVOID dest)
{
	LPVOID	Dtmp = (LPVOID)((char*)dest + y * dDxSize + (x * 2)); //removes 64bytes

	_asm {
		push	es;

		push	ds;
		pop		es;

		mov     ecx, dDxSize;
		sub     ecx, 64;

		mov		edi, Dtmp;
		mov		esi, Stmp;

		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // subjtracting 64byte from edi =0
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 1
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 2
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 3
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 4
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 5
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 6
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 7
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 8
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 9
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 10
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 11
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 12
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 13
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 14
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 15
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 16
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 17
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 18
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 19
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 20
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 21
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 22
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 23
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 24
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 25
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 26
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 27
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 28
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 29
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 30
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;

		pop		es;
	}
}
*/
void PutTileNotCliping(int x, int y, void* Stmp, void* dest, int pitch)
{
    // 1. 計算目標記憶體起始位置
    // 原理：Base + (Y * Pitch) + (X * 2 bytes_per_pixel)
    uint8_t* pDest = (uint8_t*)dest + (y * pitch) + (x * 2);

    // 2. 來源指標
    uint8_t* pSrc = (uint8_t*)Stmp;

    // 3. 參數定義
    // 原本 Assembly 做了 16 次 movsd (4 bytes), 所以寬度是 64 bytes
    const int ROW_BYTES = 64;

    // 原本 Assembly 展開了 32 次 (高度)
    const int TILE_HEIGHT = 32;

    // 4. 開始搬運 (逐行複製)
    for (int i = 0; i < TILE_HEIGHT; ++i)
    {
        // 複製一行 (64 bytes)
        memcpy(pDest, pSrc, ROW_BYTES);

        // 移動指標到下一行
        pDest += pitch;      // 目標跳到下一條掃描線 (Scanline)
        pSrc += ROW_BYTES;   // 來源是連續的，直接加寬度即可
    }
}

void PutTileCliping(int x, int y, LPVOID Stmp, LPVOID dest, int lx, int rx, int ty, int by)
{
	int  xl = TILE_SIZE;
	int  yl = TILE_SIZE;
	WORD* p, * sou;
	int  i, j;

	sou = (WORD*)((char*)Stmp + (lx + ty * xl) * 2);
	for (i = ty; i < by; i++)
	{
		p = (WORD*)((char*)dest + x * 2 + y * dDxSize);
		for (j = lx; j < rx; j++, p++, sou++)
			*p = *sou;

		sou += (xl - (rx - lx));
		y++;
	}
}


void PutTile(int x, int y, Spr* s, LPVOID Stmp, LPVOID dest)
{

	g_SDLInfo.RenderMethod(x, y, s, PUT_TILE);

	if (!g_DestBackBuf) return;

	int	lx, rx, ty, by, IsC = 0;
	int xl = TILE_SIZE;
	int yl = TILE_SIZE;

	if (x >= SCREEN_WIDTH)			return;
	else if (x + xl < 0)	return;
	if (y >= SCREEN_HEIGHT)			return;
	else if (y + yl < 0)	return;

	rx = xl;
	lx = 0;
	ty = 0;
	by = yl;

	if (x + xl > SCREEN_WIDTH)
	{
		rx -= x + xl - SCREEN_WIDTH;
		IsC = 1;
	}
	if (x < 0)
	{
		lx = 0 - x;
		x = 0;
		//		rx -= lx;
		IsC = 1;
	}
	if (y + yl > SCREEN_HEIGHT)
	{
		by -= y + yl - SCREEN_HEIGHT;
		IsC = 1;
	}
	if (y < 0)
	{
		ty = 0 - y;
		y = 0;
		//		by -= ty;
		IsC = 1;
	}

	if (IsC)		PutTileCliping(x, y, Stmp, dest, lx, rx, ty, by);
    else PutTileNotCliping(x, y, Stmp, dest, dDxSize);
	//else 		PutCmprsImgNOTCliping_CPP(x, y, s, g_DestBackBuf, dDxSize);
}

// 修改 PutTileNotCliping 函式，新增 alpha 參數
void PutTileNotClipingAlpha(int x, int y, LPVOID Stmp, LPVOID dest, int alpha) {
  WORD* src_ptr = (WORD*)Stmp;
  WORD* dest_ptr_row = (WORD*)((char*)dest + y * dDxSize + x * 2);
  int alphaMapping = 32;
#ifdef _TEXTURE
  alphaMapping = 24;
#endif
  for (int j = 0; j < TILE_SIZE; j++) {
    WORD* dest_ptr = dest_ptr_row;
    for (int i = 0; i < TILE_SIZE; i++) {
      WORD src_pixel = src_ptr[i];
      WORD dest_pixel = dest_ptr[i];

      // 565 格式的像素分解
      int r_src = (src_pixel >> 11) & 0x1F;
      int g_src = (src_pixel >> 5) & 0x3F;
      int b_src = src_pixel & 0x1F;

      int r_dest = (dest_pixel >> 11) & 0x1F;
      int g_dest = (dest_pixel >> 5) & 0x3F;
      int b_dest = dest_pixel & 0x1F;

      // 混合公式： (src * alpha + dest * (32 - alpha)) / 32
      int r_new = ((r_src * alpha) + (r_dest * (alphaMapping - alpha))) >> 5;
      int g_new = ((g_src * alpha) + (g_dest * (alphaMapping - alpha))) >> 5;
      int b_new = ((b_src * alpha) + (b_dest * (alphaMapping - alpha))) >> 5;

      // 組合成新的 565 像素
      dest_ptr[i] = (r_new << 11) | (g_new << 5) | b_new;
    }
    src_ptr += TILE_SIZE;
    dest_ptr_row = (WORD*)((char*)dest_ptr_row + dDxSize);
  }
}

// 修改 PutTileCliping 函式，新增 alpha 參數
void PutTileClipingAlpha(int x, int y, LPVOID Stmp, LPVOID dest, int lx, int rx, int ty, int by, int alpha) {
  int xl = TILE_SIZE;
  WORD* sou;
  WORD* p;
  int i, j;

  sou = (WORD*)((char*)Stmp + (lx + ty * xl) * 2);
  int alphaMapping = 32;
#ifdef _TEXTURE
  alphaMapping = 24;
#endif
  for (i = ty; i < by; i++) {
    p = (WORD*)((char*)dest + y * dDxSize + x * 2);
    for (j = lx; j < rx; j++, p++, sou++) {
      WORD src_pixel = *sou;
      WORD dest_pixel = *p;

      // 565 格式的像素分解
      int r_src = (src_pixel >> 11) & 0x1F;
      int g_src = (src_pixel >> 5) & 0x3F;
      int b_src = src_pixel & 0x1F;

      int r_dest = (dest_pixel >> 11) & 0x1F;
      int g_dest = (dest_pixel >> 5) & 0x3F;
      int b_dest = dest_pixel & 0x1F;

      // 混合公式： (src * alpha + dest * (32 - alpha)) / 32
      int r_new = ((r_src * alpha) + (r_dest * (alphaMapping - alpha))) >> 5;
      int g_new = ((g_src * alpha) + (g_dest * (alphaMapping - alpha))) >> 5;
      int b_new = ((b_src * alpha) + (b_dest * (alphaMapping - alpha))) >> 5;

      // 組合成新的 565 像素
      *p = (r_new << 11) | (g_new << 5) | b_new;
    }
    sou += (xl - (rx - lx));
    y++;
  }
}


void PutTileAlpha(int x, int y, Spr* s, LPVOID Stmp, LPVOID dest, int alpha) {

	g_SDLInfo.param[0] = alpha;
	g_SDLInfo.RenderMethod(x, y, s, PUT_TILE_ALPHA);

	if (!g_DestBackBuf) return;
  int lx, rx, ty, by, IsC = 0;
  int xl = TILE_SIZE;
  int yl = TILE_SIZE;

  if (x >= SCREEN_WIDTH)
    return;
  else if (x + xl < 0)
    return;
  if (y >= SCREEN_HEIGHT)
    return;
  else if (y + yl < 0)
    return;

  rx = xl;
  lx = 0;
  ty = 0;
  by = yl;

  if (x + xl > SCREEN_WIDTH) {
    rx -= x + xl - SCREEN_WIDTH;
    IsC = 1;
  }
  if (x < 0) {
    lx = 0 - x;
    x = 0;
    //		rx -= lx;
    IsC = 1;
  }
  if (y + yl > SCREEN_HEIGHT) {
    by -= y + yl - SCREEN_HEIGHT;
    IsC = 1;
  }
  if (y < 0) {
    ty = 0 - y;
    y = 0;
    IsC = 1;
  }

  if (IsC)
    PutTileClipingAlpha(x, y, Stmp, dest, lx, rx, ty, by, alpha);
  else
    PutTileNotClipingAlpha(x, y, Stmp, dest, alpha);
}



/*WORD RGB16(int r, int g, int b) {
	r >>= 3;
	g >>= 3;
	b >>= 3;
	if (_PixelInfo.BitMaskR != 0x7C00)
	{
		return ((r) << 11) | (g) << 6 | (b);
	}
	return ((r) << 10) | (g) << 5 | (b);
}*/
// 將 24-bit RGB (R:8, G:8, B:8) 轉換為 16-bit RGB565 (R:5, G:6, B:5)
WORD RGB16(int r, int g, int b) {
  // 將 8-bit 顏色值縮放到正確的位元數。
  // 使用位元遮罩 (&) 來確保值不會超過位元限制。
  // R: 8位 -> 5位 (右移3位)
  // G: 8位 -> 6位 (右移2位)
  // B: 8位 -> 5位 (右移3位)
  WORD r5 = (r >> 3) & 0x1F;  // 0x1F = 00011111b
  WORD g6 = (g >> 2) & 0x3F;  // 0x3F = 00111111b
  WORD b5 = (b >> 3) & 0x1F;

  // 透過位移將縮小後的值放在正確的位置並組合
  // R位於位元11-15，G位於位元5-10，B位於位元0-4
  return (r5 << 11) | (g6 << 5) | b5;
}

/*DWORD ReturnBlendRGB(int r, int g, int b) {
	WORD rgb = (WORD)RGB16(r, g, b);
	DWORD RGB;

	if (_PixelInfo.BitMaskR == 0x7C00)
	{
		_asm { xor edx, edx;
		mov		dx, rgb;
		ror		edx, 10;
		shl		dx, 11;
		rol		edx, 5;
		shl		dx, 6;
		rol		edx, 5;
		mov     RGB, edx;
		}
	}
	else
	{
		_asm { xor edx, edx;
		mov		dx, rgb;
		ror		edx, 11;
		shl		dx, 10;
		rol		edx, 6;
		shl		dx, 5;
		rol		edx, 5;
		mov     RGB, edx;
		}
	}
	return RGB;
}*/

DWORD ReturnBlendRGB(int r, int g, int b) {
  // 步驟 1: 將 24-bit RGB 轉換為 16-bit RGB565
  // 這裡我們直接計算 16-bit 值，並將其分解為各個通道
  WORD r5 = (r >> 3) & 0x1F;
  WORD g6 = (g >> 2) & 0x3F;
  WORD b5 = (b >> 3) & 0x1F;

  // 步驟 2: 將 5/6-bit 的值擴展為 8-bit
  // 這樣可以避免在轉換回 32-bit 顏色時，顏色會變暗
  DWORD r8 = (r5 << 3) | (r5 >> 2);
  DWORD g8 = (g6 << 2) | (g6 >> 4);
  DWORD b8 = (b5 << 3) | (b5 >> 2);

  // 步驟 3: 將擴展後的 8-bit RGB 值組合成 32-bit DWORD
  // 格式為 0x00RRGGBB
  // 注意：這裡假設沒有 Alpha 通道
  return (r8 << 16) | (g8 << 8) | b8;
}