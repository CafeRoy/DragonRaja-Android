// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//
#if !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
#define AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#define _TEXTURE
#ifndef WINVER
#define WINVER 0x0A00
#endif

#pragma warning(disable: 4996)

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
/*
 * #ifdef _DEBUG
#define DBG_LOG(...) SDL_Log(__VA_ARGS__)
#else
#define DBG_LOG(...) // Release 模式下這行會變成空，完全不執行
#endif
 */
#define _HAS_STD_BYTE 0
// Windows Header Files:
#include "HSEL.h"
#include "Shared.h"
#define __ANDROID__
#define USA_LOCALIZING_
//#define _TEXTURE
#define WAIT_TIME_RESPONSE_DURATION	20000
// ==========================================
// Android 移植專用補丁 (開始)
// ==========================================
#include <cstdint>  // 引入標準整數庫
extern int GetSysInfo( int type );
// 讓 Android 看懂 Windows 的整數寫法
typedef int32_t __int32;
typedef int64_t __int64;
typedef int16_t __int16;
typedef int8_t  __int8;
typedef int32_t     LONG;
// 預判您可能還會遇到這些錯誤，先幫您補上
typedef uint32_t DWORD;
typedef uint16_t WORD;
typedef uint8_t  BYTE;
typedef int      BOOL;
typedef void*    HANDLE;
// ==========================================
// Android 移植專用補丁 (結束)
// ==========================================

typedef struct RECT_
{
    LONG    left;
    LONG    top;
    LONG    right;
    LONG    bottom;
}RECT;

typedef struct POINTS_
{
    int x, y;
}POINTS;

const int MAX_NAME =41;

const int CMD_CHECK_ACCEL = 6060;

typedef struct server_check_accel {
    DWORD dwCheckAccelNowtime;
} t_server_check_accel;

#include <cstdarg>  // 包含 va_list, va_start, va_end
#include <cstdio>   // 包含 vsprintf, vsnprintf
#include "language.h"
#include "mytypes.h"
#include "MenuDef.h"
#include "BattleMgr.h"
#include "FileMgr.h"
#include "network.h"
#include "DebugLog.h"
#include "MapInfo.h"
// 1. 顏色與控制碼
typedef uint32_t COLORREF;
#define RGB(r,g,b) ((uint32_t)(((uint8_t)(r)|((uint16_t)((uint8_t)(g))<<8))|(((uint32_t)((uint8_t)(b)))<<16)))

// 2. 視窗控制代碼 (Handle)
// 在 Android 上這些沒有意義，用 void* (空指針) 佔位即可
typedef void* HWND;
typedef void* HINSTANCE;
typedef void* HDC;
typedef void* HGLRC;
typedef void* LPVOID;
typedef void* HANDLE;

// 3. 幾何結構 (SIZE, RECT, POINT)
typedef struct tagSIZE {
    int cx;
    int cy;
} SIZE;

// ==========================================
// Android 移植補丁 V3 (標準庫與時間結構)
// ==========================================

// ==========================================
// Android 移植補丁 V4 (最終完整版)
// ==========================================

// 1. 補上 C++ 標準庫
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>       // 時間庫
#include <sys/time.h>   // Linux 時間庫
#include <vector>
#include <list>
#include <map>
#include <unordered_map> // 解決 unordered_map 錯誤
#include <memory>        // 解決 unique_ptr 錯誤
#include <string>
#include <algorithm>
#include <filesystem> // 必須引入這個標頭檔
#include <sstream>  //
// 2. 解決 ZeroMemory
#ifndef ZeroMemory
#define ZeroMemory(Destination,Length) memset((Destination),0,(Length))
#endif
#define _SDL
// 3. 解決 SYSTEMTIME 結構 (Windows 的時間格式)
typedef struct _SYSTEMTIME {
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
} SYSTEMTIME, *PSYSTEMTIME, *LPSYSTEMTIME;

// 4. 手動實作 GetLocalTime (把 Linux 時間轉成 Windows 格式)
// 這段代碼會讓 Chat.h 裡的報錯消失
static void GetLocalTime(SYSTEMTIME* st) {
    if (!st) return;
    time_t rawtime;
    struct tm * timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    st->wYear = timeinfo->tm_year + 1900;
    st->wMonth = timeinfo->tm_mon + 1;
    st->wDayOfWeek = timeinfo->tm_wday;
    st->wDay = timeinfo->tm_mday;
    st->wHour = timeinfo->tm_hour;
    st->wMinute = timeinfo->tm_min;
    st->wSecond = timeinfo->tm_sec;
    st->wMilliseconds = 0; // 手機上聊天不需要毫秒，填 0 即可
}

// ==========================================
// 4. 定義一些常用的 Windows 宏
#define MAKEWORD(a, b)      ((WORD)(((BYTE)(((DWORD)(a)) & 0xff)) | ((WORD)((BYTE)(((DWORD)(b)) & 0xff))) << 8))
#define MAKELONG(a, b)      ((long)(((WORD)(((DWORD)(a)) & 0xffff)) | ((DWORD)((WORD)(((DWORD)(b)) & 0xffff))) << 16))
#define LOWORD(l)           ((WORD)(((DWORD)(l)) & 0xffff))
#define HIWORD(l)           ((WORD)((((DWORD)(l)) >> 16) & 0xffff))
#define LOBYTE(w)           ((BYTE)(((DWORD)(w)) & 0xff))
#define HIBYTE(w)           ((BYTE)((((DWORD)(w)) >> 8) & 0xff))

#define MAX_PATH          260
#ifndef NULL
#define NULL 0
#endif
#ifndef TRUE
#define TRUE 1
#endif
#define stricmp strcasecmp
#define MK_NORMAL      0
#define MK_WARNING     1
#define MK_SHORTAGE    2
#define MK_INFORMATION 3
#define OUTPUT g_mgrBattle.Message
#define TA_LEFT                      0
#define TA_RIGHT                     2
#define TA_CENTER                    6
// ==========================================
// Windows 安全字串函式相容 (strcpy_s, strcat_s...)
// ==========================================

// 1. 解決 strcpy_s
// 用法 1: strcpy_s(dst, size, src) -> 轉成 strncpy
// 用法 2: strcpy_s(dst, src)       -> 轉成 strcpy
// 這裡使用 C++ 樣板 (Template) 來自動判斷參數數量，這是最穩的做法
#ifdef __cplusplus
// 兩個參數的情況 (你的錯誤就是這種)
template <typename T>
inline int strcpy_s(T* dest, const char* src) {
    strcpy(dest, src);
    return 0;
}
// 三個參數的情況
template <typename T>
inline int strcpy_s(T* dest, size_t size, const char* src) {
    strncpy(dest, src, size);
    return 0;
}
#else
// 純 C 語言的備案 (如果需要)
    #define strcpy_s(dst, src) strcpy(dst, src)
#endif

// 2. 順便解決 strcat_s (常常跟 strcpy_s 一起出現)
#define strcat_s(dst, len, src) strncat(dst, src, len)
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

// ==========================================
// Android 移植補丁 V6 (檔案長度與描述符)
// ==========================================

#include <sys/stat.h> // 為了 fstat
#include <unistd.h>   // 為了 fileno

// 1. 對應 _fileno 到標準的 fileno
#ifndef _fileno
#define _fileno fileno
#endif

// 2. 實作 _filelength
// Windows 的 _filelength 傳入 file handle，回傳檔案大小
static inline long _filelength(int fd) {
    struct stat st;
    if (fstat(fd, &st) == 0) {
        return st.st_size;
    }
    return 0; // 發生錯誤
}

static int android_memicmp(const void *s1, const void *s2, size_t n) {
    const unsigned char *p1 = (const unsigned char *)s1;
    const unsigned char *p2 = (const unsigned char *)s2;
    while (n-- > 0) {
        if (tolower(*p1) != tolower(*p2)) {
            return tolower(*p1) - tolower(*p2);
        }
        p1++;
        p2++;
    }
    return 0;
}

// 模仿 Windows 的 SetRect
inline void SetRect(RECT* prc, int l, int t, int r, int b) {
    if (prc) {
        prc->left = l;
        prc->top = t;
        prc->right = r;
        prc->bottom = b;
    }
}
// ==========================================

// =============================================================
// 自動路徑修正工具
// =============================================================
#ifdef __ANDROID__
// 如果是 Android，且路徑開頭是 "./"，則跳過前兩個字元，回傳指針+2
// 例如 "./data/file.dat" -> "data/file.dat"
#define GET_PATH(p) ( (p[0]=='.' && p[1]=='/') ? (p + 2) : p )
#else
// Windows 下不做任何改變
    #define GET_PATH(p) (p)
#endif
#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)

