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

#include "StdAfx.h"
#include "Dragon.h"
#include "parser.h"
#include "hangul.h"
#include "SDLRenderer.h"
int HR, HB, HG;
int HangulAlign;

void Hcolor( WORD color )
{
	HR = (color>>11 & 0x1f) << 3;
	HG = (color>>6  & 0x1f) << 3;
	HB = (color     & 0x1f) << 3;
}


void Hcolor( int r, int g, int b )
{
	HR = r, HG = g, HB = b;
}


void Getcolor( int *r, int *g, int *b )
{
	*r = HR, *g = HG, *b = HB;
}

// 程式碼修正建議：
void Hprint(const int x, const int y, char* destbuf, const char* szStr)
{
	// 計算陰影顏色
	// 將 HR, HG, HB 的值各別右移一位，也就是除以 2，得到一個較暗的顏色
	SDL_Color _bc = { 0, 0, 0, 250 };
	//SDL_Color _bc = { (Uint8)(HR >> 1), (Uint8)(HG >> 1), (Uint8)(HB >> 1), 250 };

	// 如果文字主體是黑色，陰影顏色也會是黑色，這時可以特殊處理
	// 例如：如果 HR, HG, HB 都是 0，將陰影改為白色或灰色
	if (HR == 0 && HG == 0 && HB == 0) {
		_bc.r = 255;  // 或者一個較淺的灰色，例如 128
		_bc.g = 255;
		_bc.b = 255;
	}

	/*SDL_Hprint(x - 1, y, szStr, _bc, HangulAlign);
	SDL_Hprint(x + 1, y, szStr, _bc, HangulAlign);
	SDL_Hprint(x, y - 1, szStr, _bc, HangulAlign);
	SDL_Hprint(x, y + 1, szStr, _bc, HangulAlign);*/
	SDL_HprintOutLine(x, y-1, szStr, _bc, HangulAlign);

	// 繪製文字主體
	SDL_Color _color = { (Uint8)HR, (Uint8)HG, (Uint8)HB };
	SDL_Hprint(x + 1, y, szStr, _color, HangulAlign);
}

void Hprint2( const int x, const int y, char *destbuf, char *s,... )
{//020828 lsw
    char szBuffer[1024]; // 確保緩衝區夠大

    va_list arg;
    va_start(arg, s);
    // 移除 if (arg == NULL) 判斷
    // 改用安全的 vsnprintf
    vsnprintf(szBuffer, sizeof(szBuffer), s, arg);
    va_end(arg);

    szBuffer[sizeof(szBuffer) - 1] = '\0'; // 確保結尾

	const char *szStr = (char *)szBuffer;
	SDL_Color _color = {(Uint8)HR, (Uint8)HG, (Uint8)HB ,255};
	SDL_Color _bc = { 0, 0, 0, 200 };

	if (HR != _bc.r && HG != _bc.g && HB != _bc.b)
	{
		/*SDL_Hprint(x - 1, y, szStr, _bc, HangulAlign);
		SDL_Hprint(x + 1, y, szStr, _bc, HangulAlign);
		SDL_Hprint(x, y - 1, szStr, _bc, HangulAlign);
		SDL_Hprint(x, y + 1, szStr, _bc, HangulAlign);*/
		SDL_HprintOutLine(x, y-1, szStr, _bc, HangulAlign);
	}
	SDL_Hprint(x + 1, y, szStr, _color, HangulAlign);
	

}

void MakeCursorUpDown(char *text, int &newCurPos, bool up)
{
	int xTarget=0,yTarget=0;
	int x,y;
	int width,height;
	char *beginText = text;
	int max = strlen(text);


	width = 315;		/* hardcoded.. If the mail system changes width, need to update this value. */
	height = 15;

	x=0;
	y=0;

	//GetCursorLocation(text,newCurPos,xTarget,yTarget,width,height);

	newCurPos=0;

	while(1)
	{
		if(newCurPos >= max)
			break;
		//GetCursorLocation(text,newCurPos,x,y,width,height);
		if(up)
		{
			if(y >= yTarget - height && x >= xTarget)
			{
				//newCurPos--;					// we stop if the line is good and the X pos is going to be too high next char s
				break;		
			}	
			if(y > yTarget)
			{									// if the Y pos is too high already, we position at the last char of last line.
				newCurPos--;
				break;
			}
		}
		else
		{
			if(y >= yTarget + height && x >= xTarget)
			{
				//newCurPos--;					// we stop if the line is good and the X pos is going to be too high next char s
				break;		
			}	
			if(y >= yTarget + 2*height)
			{									// if the Y pos is too high already, we position at the last char of last line.
				newCurPos--;
				break;
			}
		}
		newCurPos++;
	}
}

void HprintBold( const int x, const int y, const int fc, const int bc, char *s, ... )
{//020828 lsw
    char szBuffer[1024]; // 確保緩衝區夠大

    va_list arg;
    va_start(arg, s);
    // 移除 if (arg == NULL) 判斷
    // 改用安全的 vsnprintf
    vsnprintf(szBuffer, sizeof(szBuffer), s, arg);
    va_end(arg);

    szBuffer[sizeof(szBuffer) - 1] = '\0'; // 確保結尾

	const char * szStr = (char *)szBuffer;

	SDL_Color _fc = { (uint8_t)fc, (uint8_t)(fc >> 8), (uint8_t)(fc >> 16), 255 };

	SDL_Color _bc = { 0,0,0, 200 };

	/*SDL_Hprint(x - 1, y, szStr, _bc, HangulAlign);
	SDL_Hprint(x + 1, y,  szStr, _bc, HangulAlign);
	SDL_Hprint(x, y - 1,  szStr, _bc, HangulAlign);
	SDL_Hprint(x, y + 1,  szStr, _bc, HangulAlign);*/
	SDL_HprintOutLine(x, y-1, szStr, _bc, HangulAlign);

	SDL_HprintBold(x + 1, y, szStr, _fc, HangulAlign);
}


void HprintBoldcolor(int start_x, int start_y, const RichTextLine& line_to_render)
{
	if ( line_to_render.empty()) {
		return;
	}

	// 1. 我們需要一個變數，來追蹤目前畫到哪一個 X 座標了
	int current_x = start_x;

	// 2. 【核心】遍歷這一行中的每一個「顏色區段」
	for (const auto& segment : line_to_render)
	{
		// 如果這個區段的文字是空的，就直接跳過
		if (segment.text.empty()) {
			continue;
		}

		// a. 將我們的 Color 結構，轉換為 SDL 需要的 SDL_Color
		SDL_Color sdl_color = { segment.color.r, segment.color.g, segment.color.b };


		//Hprint(start_x, start_y, nullptr, segment.text.c_str());
		// b. 【關鍵】使用 TTF_RenderText，只為「這一段」文字，建立一個帶有指定顏色的 Surface
		//    我們使用 Blended 以獲得最好的抗鋸齒效果
		const char* clean_str = SanitizeString(segment.text.c_str(), segment.text.length());

		/*SDL_Surface* textSurface = TTF_RenderUTF8_Blended(g_SDLInfo.gFont, clean_str.c_str(), sdl_color);
		if (!textSurface) {
			// 如果建立失敗，就跳過這個區段
			continue;
		}

		// c. 準備要將這段文字貼到目標畫布上的位置
		SDL_Rect destinationRect;
		destinationRect.x = current_x;
		destinationRect.y = start_y;
		destinationRect.w = textSurface->w;
		destinationRect.h = textSurface->h;

		// d. 將剛剛建立的、帶有顏色的文字 Surface，「貼」到我們的目標畫布上
		SDL_BlitSurface(textSurface, NULL, g_SDLInfo.cpuRenderSurface, &destinationRect);*/
        RenderTextWithFallback(current_x,start_y,clean_str,sdl_color, g_SDLInfo.gFont, g_SDLInfo.gEmojiFont );
		// e. 【關鍵】更新 current_x，為下一個區段的繪製做準備
		//current_x += textSurface->w;

		// f. 【重要】釋放我們為這個小區段建立的臨時 Surface，防止記憶體洩漏
		//SDL_FreeSurface(textSurface);
	}
}


void SetHangulAlign( int type )
{
	HangulAlign = type;
}





















