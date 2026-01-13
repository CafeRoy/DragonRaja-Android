// TextOutMgr.cpp: implementation of the CTextOutMgr class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Hangul.h"
#include "dragon.h"
#include "Menu.h"
#include "Hong_Sprite.h"
#include "SmallMenuSet.h"
#include "stdio.h"
#include "directsound.h"
#include "Char.h"
#include "SmallMenu.h"
#include "Hong_Sub.h"
#include "Effect.h"
#include "map.h"
#include "tool.h"
#include "MenuSet.h"
#include "skill.h"
#include "Item.h"
#include "MenuNetwork.h"
#include "MouseCursor.h"
#include "CharDataTable.h"
#include "gameproc.h"
#include "kh_cpp.h"
#include "skill_lsw.h"
#include "MenuDef.h"
#include "menudef2.h"
#include "TextOutMgr.h"
#include "Chat.h"
#include "SDLRenderer.h"
#include "utf8.h"
#include "SDLRenderer.h"
extern int HR, HB, HG;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CTextOutMgr TxtOut ;
CTextOutMgr::CTextOutMgr()
{
	m_iBlankGabSize = 3;
	m_iLineGab		= DEFAULT_LINE_GAB;
	SetAlphaValue(23);
	m_iBlendType	= 1;
	
	Init(COLOR_BLACK,TXT_BG_NORMAL_WIDTH);
}

void CTextOutMgr::SetAlphaValue(const int iValue)
{
	m_iAlpha	= iValue;
}

int CTextOutMgr::SetBgImgColor(const int iColor)
{
	switch(iColor)
	{
	case COLOR_BLACK :
		{
			m_iImgGroup = TXT_BG_BLACK_IMG;
		}break;
	case COLOR_BLUE:
		{
			m_iImgGroup = TXT_BG_BLUE_IMG;
		}break;
	case COLOR_RED:
		{
			m_iImgGroup = TXT_BG_RED_IMG;
		}break;
	case COLOR_BLUEGREEN:
		{
			m_iImgGroup = TXT_BG_BLUEGREEN_IMG;
		}break;
	case COLOR_BROWN:
		{
			m_iImgGroup = TXT_BG_BROWN_IMG;
		}break;
	case COLOR_GREEN:
		{
			m_iImgGroup = TXT_BG_GREEN_IMG;
		}break;
	default:
		{
			m_iImgGroup = TXT_BG_BLACK_IMG;
			return 0;
		}break;
	}
	return 1;
}

CTextOutMgr::~CTextOutMgr()
{
}

int CTextOutMgr::Init(const WORD iColor, const int iBGWidth, const int iAlphaValue)
{//���� ���� �ٲ���� ��׶��尡 �����°��� ���� ���� ���� �ɼ� �����ϴ�. ������ ���� ���� �ϱ� �����Դϴ�.
	SetBgImgColor(iColor);
	SetAlphaValue(iAlphaValue);
	switch(iBGWidth)
	{
	case TXT_BG_SHORT_WIDTH://150 ���� 
		{
			m_iHeadImgNo = 17;
			m_iBodyImgNo = 15;	
			m_iTailImgNo = 16;
			m_iBlkImgNo	 = 5;
		}break;
	case TXT_BG_LONG_WIDTH://250 ����
		{
			m_iHeadImgNo = 23;
			m_iBodyImgNo = 21;	
			m_iTailImgNo = 22;
			m_iBlkImgNo	 = 7;
		}break;
	case TXT_BG_VERY_LONG_WIDTH://300 ����
		{
			m_iHeadImgNo = 26;
			m_iBodyImgNo = 24;	
			m_iTailImgNo = 25;
			m_iBlkImgNo	 = 8;
		}break;
	case TXT_BG_NORMAL_WIDTH://200 ����
	default:
		{
			m_iHeadImgNo = 20;
			m_iBodyImgNo = 18;	
			m_iTailImgNo = 19;
			m_iBlkImgNo	 = 6;
		}break;
	}
	return 0;
}

int	CTextOutMgr::RcTXTOut(const int iExplainX, const int iExplainY, const int iWidth, const int iGab,char* msg,...)
{
    char szBuffer[1024]; // 確保緩衝區夠大

    va_list arg;
    va_start(arg, msg);
    // 移除 if (arg == NULL) 判斷
    // 改用安全的 vsnprintf
    vsnprintf(szBuffer, sizeof(szBuffer), msg, arg);
    va_end(arg);

    szBuffer[sizeof(szBuffer) - 1] = '\0'; // 確保結尾

	const int line	= GetLineRectTextPut( iWidth-iGab, szBuffer);
	for( int i = 0; i<line; i++ )
	{	// m_iImgGroup
		FieldTypeNomalPutFx3( iExplainX-4, iExplainY+BG_HEIGHT*i-1, 0,0,m_iBodyImgNo , m_iImgGroup,GetAlpha(), GetBlendType() );
	}
    //SDL_DrawRect(iExplainX-4, iExplainY, iWidth+10, BG_HEIGHT*line-1, SDL_Color {0,0,0,125});
	RectTextPutNotEatWhiteChar(iExplainX+iGab,iExplainY, iWidth-iGab, szBuffer,BG_HEIGHT);
	return line;
}

int	CTextOutMgr::RcTXTOutNoBG(const int iExplainX, const int iExplainY, const int iWidth, const int iGab,char* msg,...)
{
    if (!msg) return 0;

    char szBuffer[1024]; // 確保緩衝區夠大

    va_list arg;
    va_start(arg, msg);
    // 移除 if (arg == NULL) 判斷
    // 改用安全的 vsnprintf
    vsnprintf(szBuffer, sizeof(szBuffer), msg, arg);
    va_end(arg);

    szBuffer[sizeof(szBuffer) - 1] = '\0'; // 確保結尾

	const int line	= GetLineRectTextPut( iWidth-iGab, szBuffer);
	RectTextPutNotEatWhiteChar(iExplainX+iGab,iExplainY, iWidth-iGab, szBuffer,GetLineGab());
	return line;
}

int	CTextOutMgr::RcTXTOutB(const int iExplainX, const int iExplainY, const int iWidth, const int iGab,char* msg,...)
{
    char szBuffer[1024]; // 確保緩衝區夠大

    va_list arg;
    va_start(arg, msg);
    // 移除 if (arg == NULL) 判斷
    // 改用安全的 vsnprintf
    vsnprintf(szBuffer, sizeof(szBuffer), msg, arg);
    va_end(arg);

    szBuffer[sizeof(szBuffer) - 1] = '\0'; // 確保結尾

	const int line	= GetLineRectTextPut( iWidth-iGab, szBuffer);
	for( int i = 0; i<line; i++ )
	{	// m_iImgGroup
		FieldTypeNomalPutFx3( iExplainX-4, iExplainY+BG_HEIGHT*i-1, 0,0, m_iBodyImgNo, m_iImgGroup,GetAlpha(), GetBlendType() );
	}
    //SDL_DrawRect(iExplainX-4, iExplainY-1, iWidth+10, BG_HEIGHT*line-1, SDL_Color {0,0,0,125});
	RectBoldTextPutNotEatWhiteChar(iExplainX+iGab,iExplainY, iWidth-iGab, szBuffer,BG_HEIGHT);//020823 lsw
	return line;
}

int	CTextOutMgr::RcTXTOutBNoBG(const int iExplainX, const int iExplainY, const int iWidth, const int iGab,char* msg,...)
{
    char szBuffer[1024]; // 確保緩衝區夠大

    va_list arg;
    va_start(arg, msg);
    // 移除 if (arg == NULL) 判斷
    // 改用安全的 vsnprintf
    vsnprintf(szBuffer, sizeof(szBuffer), msg, arg);
    va_end(arg);

    szBuffer[sizeof(szBuffer) - 1] = '\0'; // 確保結尾

	const int line	= GetLineRectTextPut( iWidth-iGab, szBuffer);
	RectBoldTextPutNotEatWhiteChar(iExplainX+iGab,iExplainY, iWidth-iGab, szBuffer,GetLineGab());//020823 lsw
	return line;
}

int CTextOutMgr::RcChatHeaderOut(const int iExplainX, const int iExplainY, const int iWidth, const int iGab,const int iChatColorType,char* msg,...)
{
    char szBuffer[1024]; // 確保緩衝區夠大

    va_list arg;
    va_start(arg, msg);
    // 移除 if (arg == NULL) 判斷
    // 改用安全的 vsnprintf
    vsnprintf(szBuffer, sizeof(szBuffer), msg, arg);
    va_end(arg);

    szBuffer[sizeof(szBuffer) - 1] = '\0'; // 確保結尾

	const int iGabY = 10;
	switch(iChatColorType)
	{
	case CHAT_COLOR_TYPE_DEFAULT:
		{
			::PutCompressedImageFX( iExplainX+ spr[72].ox,	iExplainY+spr[72].oy+2,	&spr[72], GetAlpha(), 1 );//����
		}break;
	case CHAT_COLOR_TYPE_SIT_DOWN:
		{
			::PutCompressedImageFX( iExplainX+ 96,			iExplainY+11,			chatimage[iChatColorType], GetAlpha(), 1 );//Ŀ����
		}break;
	case CHAT_COLOR_TYPE_MERCHANT_BBS:
		{
			::FieldTypeNomalPutFx3( iExplainX,				iExplainY-12, 0,0,0,		MERCHANT_BBS_BACK_IMG, GetAlpha(), GetBlendType() ); //����
		}break;
	default:
		{
			::PutCompressedImageFX( iExplainX+ 96,			iExplainY+11,			chatimage[iChatColorType], GetAlpha(), 1 );//Ŀ����
		}break;
	}
	
	switch(iChatColorType)
	{
	case CHAT_COLOR_TYPE_MERCHANT_BBS:
		{
			RectTextPutNotEatWhiteChar(iExplainX+iGab+60,iExplainY+iGabY-14, iWidth-iGab, lan->OutputMessage(4,562),BG_HEIGHT);
		}break;
	default:
		{
			RectTextPutNotEatWhiteChar(iExplainX+iGab,iExplainY+iGabY, iWidth-iGab, szBuffer,BG_HEIGHT);
		}break;
	}
	return 1;
}

int CTextOutMgr::RcChatOut(const int iExplainX, const int iExplainY, const int iWidth, const int iGab, const int iChatColorType, char* msg,...)
{
    if (!msg) return 0;

    char szBuffer[1024]; // 確保緩衝區夠大

    va_list arg;
    va_start(arg, msg);
    // 移除 if (arg == NULL) 判斷
    // 改用安全的 vsnprintf
    vsnprintf(szBuffer, sizeof(szBuffer), msg, arg);
    va_end(arg);

    szBuffer[sizeof(szBuffer) - 1] = '\0'; // 確保結尾

	const int x = iExplainX;
	const int y = iExplainY;
	
	const int iLG = 18;
	const int iGabY = 4;
	
	const int iLCt = GetLineRectTextPut( iWidth-iGab, szBuffer);

	int line  =0;
	
	switch(iChatColorType)
	{
	case CHAT_COLOR_TYPE_MERCHANT_BBS:
		{
			line = (((iLCt*iLG)+iGabY) /MERCHANT_CHAT_BG_HEIGHT);
		}break;
	default:
		{
			line = (((iLCt*iLG)+iGabY) /CHAT_BG_HEIGHT)+1;
		}break;
	}
	for(int i = line; 0 < i; i--)
	{	
		switch(iChatColorType)
		{
		case CHAT_COLOR_TYPE_DEFAULT:
			{
				if(1==i)//������ ���϶�.
				{
					::PutCompressedImageFX( x+spr[70].ox-6, y+spr[ 70].oy -(CHAT_BG_HEIGHT*i), &spr[ 70], GetAlpha(), 1 ); //ǳ����
				}
				else
				{
					::PutCompressedImageFX( x+spr[71].ox, y+spr[ 71].oy -(CHAT_BG_HEIGHT*i), &spr[ 71], GetAlpha(), 1 ); 
				}				
			}break;
		case CHAT_COLOR_TYPE_SIT_DOWN:
			{
				if(1==i)//������ ���϶�.
				{
					::PutCompressedImageFX( x+98, y+13-(CHAT_BG_HEIGHT*i), chatimage[iChatColorType+2], GetAlpha(), 1 ); 
				}
				else
				{
					::PutCompressedImageFX( x+96, y+10-(CHAT_BG_HEIGHT*i), chatimage[iChatColorType+1], GetAlpha(), 1 ); 
				}
			}break;
		case CHAT_COLOR_TYPE_MERCHANT_BBS://���θ� ä�� �ٴ��� �ٸ��ϴ�.
			{
				::FieldTypeNomalPutFx3( x, y-(MERCHANT_CHAT_BG_HEIGHT*i), 0,0,1,MERCHANT_BBS_BACK_IMG, GetAlpha(), GetBlendType() ); 
				if(1==i)//������ ���϶�.
				{
					::FieldTypeNomalPutFx3( x, y-(MERCHANT_CHAT_BG_HEIGHT*(i-1)), 0,0,2,MERCHANT_BBS_BACK_IMG, GetAlpha(), GetBlendType() ); 
				}
			}break;
		default:
			{
				if(1==i)//������ ���϶�.
				{
					::PutCompressedImageFX( x+93, y+13-(CHAT_BG_HEIGHT*i), chatimage[iChatColorType+2], GetAlpha(), 1 ); 
				}
				else
				{
					::PutCompressedImageFX( x+96, y+10-(CHAT_BG_HEIGHT*i), chatimage[iChatColorType+1], GetAlpha(), 1 ); 
				}
			}break;
		}
	}
	switch(iChatColorType)
	{
	case CHAT_COLOR_TYPE_MERCHANT_BBS:
		{
			RectTextPutNotEatWhiteChar(iExplainX+iGab,iExplainY-(line*MERCHANT_CHAT_BG_HEIGHT)+iGabY, iWidth-iGab, szBuffer,iLG);
		}break;
	default:
		{
			RectTextPutNotEatWhiteChar(iExplainX+iGab,iExplainY-(line*CHAT_BG_HEIGHT)+iGabY, iWidth-iGab, szBuffer,iLG);
		}break;
	}
	return line;
}

int	CTextOutMgr::RcBlankOut(const int iExplainX, const int iExplainY, const int iWidth,const int iBlankCount)
{
	for( int i = 0; i<iBlankCount; i++ )
	{
		FieldTypeNomalPutFx3( iExplainX-4, iExplainY+m_iBlankGabSize*i-1, 0,0, m_iBlkImgNo	, m_iImgGroup,GetAlpha(), GetBlendType() );
	}
    //SDL_DrawRect(iExplainX-4, iExplainY, iWidth+10, BG_HEIGHT*iBlankCount, SDL_Color {0,0,0,125});
	return iBlankCount*m_iBlankGabSize;
}

void CTextOutMgr::PutTextHeaderBGImg(const int iExplainX, const int iExplainY, const int iWidth)
{	//-5 �� ��� �׵θ� ������
    //SDL_DrawRect(iExplainX-4, iExplainY-5, iWidth+10, BG_HEIGHT+2, SDL_Color {0,0,0,125});
	FieldTypeNomalPutFx3( iExplainX-4, iExplainY-1-5, 0,0, m_iHeadImgNo, m_iImgGroup,GetAlpha(), GetBlendType() );
}

void CTextOutMgr::PutTextTailBGImg(const int iExplainX, const int iExplainY, const int iWidth)
{
    //SDL_DrawRect(iExplainX-4, iExplainY, iWidth+10, BG_HEIGHT+2, SDL_Color {0,0,0,125});
	FieldTypeNomalPutFx3( iExplainX-4, iExplainY-1, 0,0, m_iTailImgNo, m_iImgGroup,GetAlpha(), GetBlendType() );
}
/*
int RectTextPutBasic(const int x, const int y, int nWidth, char* sSource,
                     const int hight, const int line_max, bool bBold, bool bEat,
                     char** pEnd) {

    // 1. 基本檢查
    if (!sSource || *sSource == '\0') return 0;
    if (nWidth <= 0) nWidth = 100;

    const int MAX_TEXT_LENGTH = 200; // 注意：如果文字很長，這裡可能會截斷

    // 獲取字串長度
    size_t len = strnlen(sSource, MAX_TEXT_LENGTH);
    if (len == 0) return 0;

    // ------------------------------------------------------------
    // 【步驟 1: 預先清洗整個字串，並在副本上操作】
    // ------------------------------------------------------------
    std::string clean_source_str = SanitizeString(std::string(sSource, len));

    // 更新指標和長度，所有後續操作都在這個乾淨的字串上進行
    const char* safe_source = clean_source_str.c_str();
    const char* safe_end = safe_source + clean_source_str.length();

    int nLine = 0;
    const char* line_start = safe_source;
    const char* end_of_string = safe_end;

    TTF_Font* baseFont = g_SDLInfo.gFont;
    TTF_Font* fallbackFont = g_SDLInfo.gEmojiFont;

    if (!baseFont) return 0;

    // ------------------------------------------------------------
    // 【步驟 2: 核心換行邏輯】
    // ------------------------------------------------------------
    while (line_start < end_of_string) {
        // 檢查是否超過最大行數
        if (line_max && nLine >= line_max) {
            if (pEnd) {
                // 計算偏移量並回推到原始字串
                // 注意：如果 SanitizeString 改變了字串長度，這裡的 pEnd 可能會稍微不準
                ptrdiff_t offset = line_start - safe_source;
                *pEnd = sSource + offset;
            }
            return nLine;
        }

        int current_line_width = 0;
        const char* it = line_start;
        const char* line_end = line_start;
        const char* last_possible_break = line_start;

        while (it < end_of_string) {
            const char* char_start = it;

            // 使用 utf8::next 取出字碼 (因為已清洗過，這裡相對安全)
            Uint32 codepoint = 0;
            try {
                codepoint = utf8::next(it, end_of_string);
            } catch (...) {
                it++; // 萬一發生異常，跳過一個 byte 避免死循環
                continue;
            }

            if (codepoint == 0) break;

            // 處理強制換行符
            if (codepoint == '\n') {
                line_end = char_start; // 這一行結束在 \n 之前
                // it 已經由 utf8::next 移動到 \n 之後了，所以下一次迴圈會從下一行開始
                // 但我們這裡需要強制跳出內層迴圈來畫圖
                break;
            }

            // ★★★ 修改點 1: 使用線程安全的字型檢查 (修正邏輯版) ★★★
            TTF_Font* font_for_char = baseFont; // 預設先給主字型

            // 1. 先明確檢查主字型有沒有這個字 (IsProvided 回傳非 0 代表存在)
            if (TTF_GlyphIsProvided32_Safe(baseFont, codepoint)) {
                font_for_char = baseFont;
            }
                // 2. 如果主字型沒有，才去查備用字型
            else if (fallbackFont && TTF_GlyphIsProvided32_Safe(fallbackFont, codepoint)) {
                font_for_char = fallbackFont;
            }
            // 3. 如果都沒有，font_for_char 保持為 baseFont (顯示缺字框)

            int char_width = 0;

            // ★★★ 修改點 2: 使用線程安全的寬度測量 ★★★
            // 這裡不需要再判斷 if，因為上面已經選好字型了
            // 加上 != 0 的檢查是為了防止萬一測量失敗時使用未初始化的值
            if (TTF_GlyphMetrics32_Safe(font_for_char, codepoint, nullptr, nullptr, nullptr, nullptr, &char_width) != 0) {
                char_width = 0; // 測量失敗時歸零，避免數值錯亂
            }
            // 自動換行邏輯 (Word Wrap)
            if (current_line_width + char_width > nWidth && char_start > line_start) {
                // 如果這一行已經有字了，且加上這個字會超寬 -> 換行
                if (last_possible_break > line_start) {
                    line_end = last_possible_break; // 在上一個標點符號或空格處切斷
                    it = last_possible_break;       // 重設迭代器，下一次從切斷處開始讀
                }
                else {
                    line_end = char_start; // 強制在當前字之前切斷
                    it = char_start;       // 重設迭代器
                }
                break; // 跳出內層迴圈去畫圖
            }

            current_line_width += char_width;

            // 記錄可換行點 (空格、標點)
            if (codepoint == ' ' || codepoint == '\t') {
                last_possible_break = it; // it 已經指到空格後面了
            }
            else if (codepoint > 127) {
                // 中日韓文字通常可以在任意字後換行
                last_possible_break = it;
            }

            line_end = it; // 預設行尾推進到當前
        } // 內層 while 結束

        // 繪圖
        if (hight > 0) {
            std::string line_to_print(line_start, line_end - line_start);
            if (!line_to_print.empty()) {
                // 呼叫你的繪圖函式 (確保 Hprint 內部也是使用 AsyncTextManager 或安全的 Render)
                Hprint(x, y + nLine * hight, nullptr, line_to_print.c_str());
            }
        }

        // 準備下一行
        nLine++;
        const char* old_line_start = line_start;

        // 如果是因為 \n 換行的，it 會在 \n 之後，line_end 在 \n 之前
        // 如果是因為超寬換行的，it 會被重設回 line_end
        // 這裡需要小心處理邏輯

        if (it > line_end) {
            // 這是遇到 \n 的情況
            line_start = it;
        } else {
            // 這是自動換行的情況
            line_start = line_end;
        }

        // 跳過行首的空白字元 (除了第一次)
        while (line_start < end_of_string && (*line_start == ' ' || *line_start == '\t')) {
            line_start++;
        }

        // 防死迴圈機制
        if (line_start == old_line_start && line_start < end_of_string) {
            line_start++;
        }
    } // 外層 while 結束

    if (pEnd) {
        *pEnd = sSource + clean_source_str.length();
    }

    return nLine;
}
*/

int RectTextPutBasic(const int x, const int y, int nWidth, char* sSource,
                     const int hight, const int line_max, bool bBold, bool bEat,
                     char** pEnd) {

    // --- 效能計時開始 ---
    //auto t_start = std::chrono::high_resolution_clock::now();

    // 1. 基本檢查
    if (!sSource || *sSource == '\0') return 0;
    if (nWidth <= 0) nWidth = 100;

    const int MAX_TEXT_LENGTH = 200; // 注意：如果文字很長，這裡可能會截斷

    // 獲取字串長度
    size_t len = strnlen(sSource, MAX_TEXT_LENGTH);
    if (len == 0) return 0;

    // ------------------------------------------------------------
    // 【步驟 1: 預先清洗】
    // ------------------------------------------------------------
    const char* clean_source_str = SanitizeString(sSource, len);

    const char* safe_source = clean_source_str;
    const char* safe_end = safe_source + strlen(clean_source_str);

    int nLine = 0;
    const char* line_start = safe_source;
    const char* end_of_string = safe_end;

    TTF_Font* baseFont = g_SDLInfo.gFont;
    TTF_Font* fallbackFont = g_SDLInfo.gEmojiFont;

    if (!baseFont) return 0;

    // ★★★ 優化重點 1: 判斷是否啟用快速通道 ★★★
    // 如果沒有 fallback font，或者兩個 font 一樣，我們就不用逐字檢查是否存在
    bool isSimpleMode = (!fallbackFont || fallbackFont == baseFont);

    // 為了線程安全，我們在這裡鎖定 Font 資源 (假設你有 fontMutex)
    // 如果你的 TTF_GlyphMetrics32_Safe 內部已經有鎖，這裡可以拿掉
    // std::lock_guard<std::mutex> lock(AsyncTextManager::Instance().fontMutex);

    // ------------------------------------------------------------
    // 【步驟 2: 核心換行邏輯】
    // ------------------------------------------------------------
    while (line_start < end_of_string) {
        // 檢查是否超過最大行數
        if (line_max && nLine >= line_max) {
            if (pEnd) {
                ptrdiff_t offset = line_start - safe_source;
                *pEnd = sSource + offset;
            }
            break; // 跳出迴圈，準備結束
        }

        int current_line_width = 0;
        const char* it = line_start;
        const char* line_end = line_start;
        const char* last_possible_break = line_start;

        while (it < end_of_string) {
            const char* char_start = it;
            Uint32 codepoint = 0;

            // 使用 utf8::next 取出字碼
            try {
                codepoint = utf8::next(it, end_of_string);
            } catch (...) {
                it++; continue;
            }

            if (codepoint == 0) break;

            if (codepoint == '\n') {
                line_end = char_start;
                break;
            }

            // ★★★ 優化重點 2: 快速通道邏輯 ★★★
            TTF_Font* font_for_char = baseFont;

            if (!isSimpleMode) {
                // 只有在「複雜模式」下才去檢查字元是否存在
                // 這會節省大量的 CPU 查詢時間
                if (!TTF_GlyphIsProvided32_Safe(baseFont, codepoint) &&
                    TTF_GlyphIsProvided32_Safe(fallbackFont, codepoint)) {
                    font_for_char = fallbackFont;
                }
            }

            int char_width = 0;
            // 測量寬度 (這裡必須做，因為要計算換行)
            if (TTF_GlyphMetrics32_Safe(font_for_char, codepoint, nullptr, nullptr, nullptr, nullptr, &char_width) != 0) {
                char_width = 0;
            }

            // 自動換行判斷
            if (current_line_width + char_width > nWidth && char_start > line_start) {
                if (last_possible_break > line_start) {
                    line_end = last_possible_break;
                    it = last_possible_break;
                } else {
                    line_end = char_start;
                    it = char_start;
                }
                break;
            }

            current_line_width += char_width;

            if (codepoint == ' ' || codepoint == '\t' || codepoint > 127) {
                last_possible_break = it;
            }

            line_end = it;
        } // 內層 while 結束

        // 繪圖
        if (hight > 0) {
            std::string line_to_print(line_start, line_end - line_start);
            if (!line_to_print.empty()) {
                // ★ 注意：這裡呼叫 Hprint，Hprint 內部必須使用我們先前寫的 AsyncTextManager
                // 否則這裡還是會卡住
                Hprint(x, y + nLine * hight, nullptr, line_to_print.c_str());
            }
        }

        nLine++;
        const char* old_line_start = line_start;

        if (it > line_end) { // 遇到 \n
            line_start = it;
        } else { // 自動換行
            line_start = line_end;
        }

        // 跳過行首空白
        while (line_start < end_of_string && (*line_start == ' ' || *line_start == '\t')) {
            line_start++;
        }

        if (line_start == old_line_start && line_start < end_of_string) {
            line_start++;
        }
    } // 外層 while 結束

    if (pEnd && (!line_max || nLine < line_max)) {
        *pEnd = sSource + strlen(clean_source_str);
    }

    // --- 效能測試結束與 Log ---
    /*auto t_end = std::chrono::high_resolution_clock::now();
    double total_ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();

    if (total_ms > 2.0) {
        SDL_Log("[LAG WARNING] RectTextPut Layout took: %.2f ms | Text: %s", total_ms, sSource);
    }*/

    return nLine;
}

int RectTextPutNotEatWhiteChar(const int x, const int y, int nWidth,
                               char* sSource, const int hight,
                               const int line_max) {
  return RectTextPutBasic(x, y, nWidth, sSource, hight, line_max, false, false,
                          NULL);
}

int RectTextPut(const int x, const int y, int nWidth, char* sSource,
                const int hight, const int line_max) {
  return RectTextPutBasic(x, y, nWidth, sSource, hight, line_max, false, true,
                          NULL);
}

int RectBoldTextPutNotEatWhiteChar(const int x, const int y, int nWidth,
                                   char* sSource, const int hight,
                                   const int line_max) {
  return RectTextPutBasic(x, y, nWidth, sSource, hight, line_max, true, false,
                          NULL);
}

int RectBoldTextPut(const int x, const int y, int nWidth, char* sSource,
                    const int hight, const int line_max) {
  return RectTextPutBasic(x, y, nWidth, sSource, hight, line_max, true, true,
                          NULL);
}

int GetRectTextLine(char* sSource, int nWidth, const int line_max) {
  return RectTextPutBasic(0, 0, nWidth, sSource, 0, line_max, false, false,
                          NULL);
}