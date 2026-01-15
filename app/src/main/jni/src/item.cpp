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
#include <Stdio.h>
#include "Dragon.h"
#include "Hong_Sprite.h"
#include "Hong_Light.h"
#include "Hong_Sub.h"
#include "map.h"
#include "GameProc.h"
#include "Convert565to555.h"
#include "Tool.h"
#include "Skill.h"
#include "CharDataTable.h"
#include "Char.h"
#include "ItemTable.h"
#include "DirectSound.h"
#include "Effect.h"
#include "Path.h"
#include "Object.h"
#include "hangul.h"
#include "item.h"
#include "menuset.h"//021030 YGI	
#include "TextOutMgr.h"
#include "SmallMenu.h"
extern MAP		g_Map;		
extern int		g_MouseItemType;	// menu.cpp
extern int		g_MouseItemNumber;
extern int		g_StartMenuOn;
extern void changemapobjectname( char *oldname, char *newname );
LPITEMGROUND g_Item, CursorCheckedItem;
LPITEMSPRITE  g_ItemSprite;

std::vector<LPITEMGROUND> g_DeleteItem;
ItemGroundPool g_itemground_pool;
ItemSpritePool g_itemsprite_pool;

// --- 【新代碼】 ---
void InitializeItemGroundPool() {
	g_itemground_pool.free_list_head = NULL;
	g_itemground_pool.free_count = 0;

	for (int i = 0; i < MAX_GROUND_ITEMS; i++) {
		LPITEMGROUND new_node = &g_itemground_pool.pool_data[i];

		// 假設 ITEMGROUND 也用 lpNext 來串接 free list
		// 如果不是，請改成它串接 free list 用的那個指標成員
		new_node->next = g_itemground_pool.free_list_head;

		g_itemground_pool.free_list_head = new_node;
		g_itemground_pool.free_count++;
	}
}

// --- 【新代碼】 ---
void InitializeItemSpritePool() {
	g_itemsprite_pool.free_list_head = NULL;
	g_itemsprite_pool.free_count = 0;

	for (int i = 0; i < MAX_ITEM_SPRITES; i++) {
		LPITEMSPRITE new_node = &g_itemsprite_pool.pool_data[i];

		// 假設 ITEMSPRITE 也用 next 來串接 free list
		new_node->next = g_itemsprite_pool.free_list_head;

		g_itemsprite_pool.free_list_head = new_node;
		g_itemsprite_pool.free_count++;
	}
}

int LoadItemSprite( Spr *s, int no, int type, int order, int return_null = 0 )
{
    char temp[FILENAME_MAX];
    SDL_RWops *fp = NULL; // 替換 FILE*

    // [Android] 為了方便處理路徑，我們直接在 sprintf 格式化字串中移除 "./"
    // 如果您的資源檔在 APK 內的 assets/item/ 下，就直接寫 "item/..."

    if( type < 13 )       // 0811 YGI
    {
        // 1. 嘗試讀取目標道具
        sprintf( temp, "item/%07d.CSP", no ); // Removed ./
        fp = SDL_RWFromFile( temp, "rb" );

        // 2. 找不到則讀取預設道具 (Dummy)
        if( fp == NULL )
        {
            sprintf( temp, "item/0000000.CSP" ); // Removed ./
            fp = SDL_RWFromFile( temp, "rb" );
        }
    }
    else
#ifdef _MAPOBJECT_DIRECTORY_CHANGE_
    {
        int lev;

        if( order == 0 )
        {
            no -= 13000;

            lev = no / 1000;
            if( lev == 1 ) lev = 0;

            // 1. 嘗試讀取物件
            sprintf( temp, "object/%d/%04d.CSP", lev, no ); // Removed ./
            fp = SDL_RWFromFile( temp, "rb" );

            // 2. 找不到且允許 fallback
            if( fp == NULL )
            {
                if( !return_null )
                {
                    sprintf( temp, "item/0000000.CSP" ); // Fallback to dummy
                    fp = SDL_RWFromFile( temp, "rb" );
                }
            }
        }
        else
        {
            no -= 1300000;

            lev = no / 100000;
            if( lev == 1 ) lev = 0;

            // 1. 嘗試讀取物件 (Order != 0)
            sprintf( temp, "object/%d/%06d.CSP", lev, no ); // Removed ./
            fp = SDL_RWFromFile( temp, "rb" );

            // 2. 找不到且允許 fallback
            if( fp == NULL )
            {
                if( !return_null )
                {
                    sprintf( temp, "item/0000000.CSP" ); // Fallback to dummy
                    fp = SDL_RWFromFile( temp, "rb" );
                }
            }
        }
    }
#endif

    // 開始讀取資料
    if( fp )
    {
        // 逐一讀取變數，這在跨平台時是安全的 (不涉及 Struct Alignment)
        SDL_RWread(fp, &s->xl, sizeof( short ), 1);
        SDL_RWread(fp, &s->yl, sizeof( short ), 1);
        SDL_RWread(fp, &s->ox, sizeof( short ), 1);
        SDL_RWread(fp, &s->oy, sizeof( short ), 1);

        SDL_RWread(fp, &s->size, sizeof( unsigned int ), 1);

        s->img = NULL;
        MemAlloc( s->img, s->size );

        // 讀取圖片數據
        SDL_RWread(fp, s->img, 1, s->size);

        convert565to555( s ); // 維持原樣

        SDL_RWclose(fp); // 關閉檔案

        // 建立紋理
        g_SDLInfo.CreateTextureFromSingleSpr_CPP( temp, s );

        return 1;
    }

    // 如果 return_null 為真且找不到檔案，或是連 dummy 都找不到，就會回傳 0
    return 0;
}

void FreeItemSprite( LPITEMSPRITE i)
{
	MemFree( i->sp.img );
}

LPITEMSPRITE FindItemSprite( int no )
{
	LPITEMSPRITE is = g_ItemSprite;

	while( is != NULL )
	{
		if( is->no == no ) return is;
		is = is->next;
	}

	return NULL;
}

void DeleteItemSprite( int no )
{		
	LPITEMSPRITE  t = g_ItemSprite;
			
	while( t != NULL )
	{		
		if( t->no == no )
		{	
			t->count --;
					
			if( t->count == 0 )
			{		
				if( t == g_ItemSprite )
				{	
					t = g_ItemSprite->next;
					FreeItemSprite( g_ItemSprite );
					//MemFree( g_ItemSprite );
					// 【替換後】
					//g_ItemSprite->~ITEMSPRITE(); // 呼叫解構
					g_ItemSprite->next = g_itemsprite_pool.free_list_head;
					g_itemsprite_pool.free_list_head = g_ItemSprite;
					g_itemsprite_pool.free_count++;
					// 【修正結束】

					g_ItemSprite = t;
					if( t != NULL )	g_ItemSprite->prev = NULL;
					return;
				}
				else 
				{
					if ( t->prev != NULL )
					{
						t->prev->next = t->next;
					}
					if( t->next != NULL )
					{
						t->next->prev = t->prev;
					}
					FreeItemSprite( t );
					//MemFree( t );
					// 【替換後】
					//t->~ITEMSPRITE(); // 呼叫解構
					t->next = g_itemsprite_pool.free_list_head;
					g_itemsprite_pool.free_list_head = t;
					g_itemsprite_pool.free_count++;
					// 【修正結束】
					return;
				}
			}
		}	
		t = t->next;
	}		
}		

LPITEMSPRITE AddItemSprite( int no, int type, int order, int return_flag = 0  )
{			
	LPITEMSPRITE temp, s = FindItemSprite( no );
			
	if( s == NULL )
	{		
		if( g_ItemSprite == NULL )
		{	
			//MemAlloc( g_ItemSprite, sizeof( ITEMSPRITE ) );

			// 【替換後】
			g_ItemSprite = g_itemsprite_pool.free_list_head;
			if (g_ItemSprite == NULL) { return NULL; } // Sprite 物件池已滿
			g_itemsprite_pool.free_list_head = g_ItemSprite->next;
			g_itemsprite_pool.free_count--;
			*g_ItemSprite = ITEMSPRITE(); // 呼叫建構函式重設

			g_ItemSprite->no = no;
			g_ItemSprite->count = 1;
			
			LoadItemSprite( &g_ItemSprite->sp, no, type, order );
			g_ItemSprite->next = NULL;
			g_ItemSprite->prev = NULL;
			
			s = g_ItemSprite;
		}	
		else
		{
			//MemAlloc(s,  sizeof( ITEMSPRITE ) );
			// 【替換後】
			s = g_itemsprite_pool.free_list_head;
			if (s == NULL) { return NULL; } // Sprite 物件池已滿
			g_itemsprite_pool.free_list_head = s->next;
			g_itemsprite_pool.free_count--;
			*s = ITEMSPRITE(); // C呼叫建構函式重設

			s->no = no;
			s->count = 1;
			if( LoadItemSprite( &s->sp, no, type, order, return_flag ) )
			{
				s->next = NULL;
					
				temp = g_ItemSprite;
				temp->prev = s;
				s->next = temp;
				s->prev = NULL;
				g_ItemSprite = s;
			}
			else
			{
				//MemFree( s );
				// 【替換後】
				//s->~ITEMSPRITE(); // 呼叫解構
				s->next = g_itemsprite_pool.free_list_head;
				g_itemsprite_pool.free_list_head = s;
				g_itemsprite_pool.free_count++;
				s = nullptr;
			}
		}	
	}		
	else	
	{		
		s->count ++;
	}		
			
	return s;
}
// ----------------------------------------------------------------------------
// 輔助 1: 讀取整個文字檔內容
// ----------------------------------------------------------------------------
static bool ReadAllText(SDL_RWops* fp, std::string& outStr) {
    if (!fp) return false;
    Sint64 size = SDL_RWsize(fp);
    if (size <= 0) return false;
    outStr.resize(size);
    SDL_RWseek(fp, 0, RW_SEEK_SET);
    SDL_RWread(fp, &outStr[0], size, 1);
    return true;
}

// ----------------------------------------------------------------------------
// 輔助 2: 初始化 Item 基礎數值 (消除重複的初始化代碼)
// ----------------------------------------------------------------------------
static void InitBaseItem(LPITEMGROUND i) {
    i->getable   = 1;
    i->Anitype   = 0;
    i->type      = 0;
    i->dirno     = 0;
    i->Where     = 0;
    i->Sorttype  = 0;
    i->shadow    = 0;
    i->dx[0] = 0; i->dx[1] = 0; i->dx[2] = 0; i->dx[3] = 0;
    i->framecount = 1;
    i->dsx[0] = 0; i->dsy[1] = 0; i->ddx[0] = 0; i->ddy[1] = 0;
}

// ----------------------------------------------------------------------------
// 輔助 3: 智慧開檔 (處理聖誕節路徑與備用路徑)
// folder: 資料夾 (如 "object")
// subFolder: 子資料夾 (通常是地圖名轉換後的 tempitemname)
// filename: 檔名
// ----------------------------------------------------------------------------
static SDL_RWops* TryOpenObjectFile(const char* folder, const char* subFolder, const char* filename) {
    char path[256];
    SDL_RWops* fp = nullptr;

    // 1. 嘗試聖誕節路徑
    if (g_bIsChristmas) {
        sprintf(path, "%s/CHRISTMAS_%s/%s", folder, subFolder, filename);
        fp = SDL_RWFromFile(path, "rb");
        if (fp) return fp;
    }

    // 2. 嘗試正常路徑
    sprintf(path, "%s/%s/%s", folder, subFolder, filename);
    fp = SDL_RWFromFile(path, "rb");

    return fp;
}

// ----------------------------------------------------------------------------
// 輔助 4: 載入 Sprite 資源 (處理迴圈與 AddItemSprite)
// ----------------------------------------------------------------------------
static void LoadItemSprites(LPITEMGROUND i, int type) {
    LPITEMSPRITE t;
    for(int kk = 0; kk < i->framecount ; kk ++) {
        i->dsx[kk] += i->x;
        i->dsy[kk] += i->y;
        i->ddx[kk] += i->x;
        i->ddy[kk] += i->y;

        if( kk == 0 ) t = AddItemSprite( i->no, type, kk );
        else          t = AddItemSprite( i->no * 100 + kk, type, kk );

        if (t) i->spr[kk] = &t->sp;
    }
}

// ----------------------------------------------------------------------------
// 輔助 5: 從二進位檔讀取 (.toi2.b)
// ----------------------------------------------------------------------------
static bool LoadItem_Binary(SDL_RWops* fp, LPITEMGROUND i, int type) {
    if (!fp) return false;

    static int check = 0;
    check++;

    // --- CRC Check (每5次檢查一次) ---
    if ((check % 5) == 2) {
        Sint64 fl = SDL_RWsize(fp);
        if (fl > 1) {
            std::vector<char> buf(fl); // 使用 vector 自動管理記憶體
            SDL_RWread(fp, buf.data(), fl - 1, 1);

            char crc = 0, crc1;
            for (int k = 0; k < fl - 1; k++) crc += buf[k];

            SDL_RWread(fp, &crc1, 1, 1);
            if (CheckSumError == 0) {
                CheckSumError = std::abs(crc - crc1);
                if (CheckSumError) SDL_Log("LoadItem Binary CheckSumError");
            }
            // 重置檔案指標
            SDL_RWseek(fp, 0, RW_SEEK_SET);
        }
    }

    int obcount;
    SDL_RWread(fp, &obcount, sizeof(int), 1);

    int no;
    // 預先宣告讀取變數
    int getable, anitype, itemtype, dir, where, sorting, shadow, light, lightx, lighty;
    int dx1, dy1, dx2, dy2, dx3, dy3, dx4, dy4, framecount;

    for (int j = 0; j < obcount; j++) {
        // Offset 計算 (Windows int=4, short=2)
        // sizeof(int) [obcount] + j * RecordSize
        // RecordSize = sizeof(int)*20 + sizeof(short)*4*ITEM_FRAME_MAX_
        long recordSize = 4 * 20 + 2 * 4 * ITEM_FRAME_MAX_;
        long offset = 4 + j * recordSize;

        SDL_RWseek(fp, offset, RW_SEEK_SET);
        SDL_RWread(fp, &no, sizeof(int), 1);

        if (no == i->no) {
            // 讀取屬性
            SDL_RWread(fp, &getable, sizeof(int), 1);
            SDL_RWread(fp, &anitype, sizeof(int), 1);
            SDL_RWread(fp, &itemtype, sizeof(int), 1);
            SDL_RWread(fp, &dir, sizeof(int), 1);
            SDL_RWread(fp, &where, sizeof(int), 1);
            SDL_RWread(fp, &sorting, sizeof(int), 1);
            SDL_RWread(fp, &shadow, sizeof(int), 1);
            SDL_RWread(fp, &light, sizeof(int), 1);
            SDL_RWread(fp, &lightx, sizeof(int), 1);
            SDL_RWread(fp, &lighty, sizeof(int), 1);
            SDL_RWread(fp, &dx1, sizeof(int), 1);
            SDL_RWread(fp, &dy1, sizeof(int), 1);
            SDL_RWread(fp, &dx2, sizeof(int), 1);
            SDL_RWread(fp, &dy2, sizeof(int), 1);
            SDL_RWread(fp, &dx3, sizeof(int), 1);
            SDL_RWread(fp, &dy3, sizeof(int), 1);
            SDL_RWread(fp, &dx4, sizeof(int), 1);
            SDL_RWread(fp, &dy4, sizeof(int), 1);
            SDL_RWread(fp, &framecount, sizeof(int), 1);

            // 填入 i 結構
            i->getable = getable;
            i->Anitype = anitype;
            i->type = itemtype;
            i->dirno = dir;
            i->Where = where;
            i->Sorttype = sorting;
            i->shadow = shadow;
            i->dx[0] = i->x + dx1 + i->offx; i->dy[0] = i->y + dy1 + i->offy;
            i->dx[1] = i->x + dx2 + i->offx; i->dy[1] = i->y + dy2 + i->offy;
            i->dx[2] = i->x + dx3 + i->offx; i->dy[2] = i->y + dy3 + i->offy;
            i->dx[3] = i->x + dx4 + i->offx; i->dy[3] = i->y + dy4 + i->offy;
            i->framecount = framecount;
            i->dsx[0] = 0; i->dsy[1] = 0; i->ddx[0] = 0; i->ddy[1] = 0;

            // 讀取陣列
            SDL_RWread(fp, i->dsx, sizeof(short), ITEM_FRAME_MAX_);
            SDL_RWread(fp, i->dsy, sizeof(short), ITEM_FRAME_MAX_);
            SDL_RWread(fp, i->ddx, sizeof(short), ITEM_FRAME_MAX_);
            SDL_RWread(fp, i->ddy, sizeof(short), ITEM_FRAME_MAX_);

            // 載入 Sprites
            LoadItemSprites(i, type);

            SDL_RWclose(fp);
            return true;
        }
    }
    SDL_RWclose(fp);
    return false;
}

// ----------------------------------------------------------------------------
// 輔助 6: 從文字檔讀取 (.toi2.txt)
// ----------------------------------------------------------------------------
static bool LoadItem_Text(SDL_RWops* fp, LPITEMGROUND i, int type) {
    if (!fp) return false;

    std::string content;
    if (!ReadAllText(fp, content)) {
        SDL_RWclose(fp);
        return false;
    }
    SDL_RWclose(fp); // 讀完就關閉

    std::stringstream ss(content);
    std::string line;
    int itemno;

    // 跳過前兩行 header
    std::getline(ss, line);
    std::getline(ss, line);

    // 變數宣告
    int no, getable, anitype, itemtype, dir, where, sorting, shadow, light, lightx, lighty;
    int dx1, dy1, dx2, dy2, dx3, dy3, dx4, dy4, framecount;
    int dsx, dsy, ddx, ddy;

    while (std::getline(ss, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) continue;

        std::stringstream lineSS(line);
        if (lineSS >> itemno) {
            if (itemno == i->no) {
                // 重新解析整行
                sscanf(line.c_str(), "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d ",
                       &no, &getable, &anitype, &itemtype, &dir, &where, &sorting, &shadow,
                       &light, &lightx, &lighty,
                       &dx1, &dy1, &dx2, &dy2, &dx3, &dy3, &dx4, &dy4,
                       &framecount);

                i->getable = getable;
                i->Anitype = anitype;
                i->type = itemtype;
                i->dirno = dir;
                i->Where = where;
                i->Sorttype = sorting;
                i->shadow = shadow;
                i->dx[0] = i->x + dx1 + i->offx; i->dy[0] = i->y + dy1 + i->offy;
                i->dx[1] = i->x + dx2 + i->offx; i->dy[1] = i->y + dy2 + i->offy;
                i->dx[2] = i->x + dx3 + i->offx; i->dy[2] = i->y + dy3 + i->offy;
                i->dx[3] = i->x + dx4 + i->offx; i->dy[3] = i->y + dy4 + i->offy;
                i->framecount = framecount;
                i->dsx[0] = 0; i->dsy[1] = 0; i->ddx[0] = 0; i->ddy[1] = 0;

                for (int j = 0; j < i->framecount; j++) {
                    if (std::getline(ss, line)) {
                        if (!line.empty() && line.back() == '\r') line.pop_back();
                        sscanf(line.c_str(), "%d %d %d %d", &dsx, &dsy, &ddx, &ddy);

                        i->dsx[j] = dsx + i->x;
                        i->dsy[j] = dsy + i->y;
                        i->ddx[j] = ddx + i->x;
                        i->ddy[j] = ddy + i->y;

                        LPITEMSPRITE t;
                        if (j == 0) t = AddItemSprite(i->no, type, j);
                        else        t = AddItemSprite(i->no * 100 + j, type, j);
                        if (t) i->spr[j] = &t->sp;
                    }
                }
                return true;
            }
        }
    }
    return false;
}

void LoadItem( LPITEMGROUND i )
{
    int type = i->no / 1000;
    int no   = i->no % 1000;
    char tempitemname[FILENAME_MAX]={0};

    // 取得地圖物件名稱 (用於一般物件讀取)
    changemapobjectname(MapName, tempitemname);

    // =========================================================
    // 情況 1: 特殊道具 (Special Item > 25000)
    // =========================================================
    if( i->no > 25000 )
    {
        InitBaseItem(i); // 使用輔助函式

        int spr_no = i->no - 25000 + 13000;
        type = spr_no / 1000;

        if( ( i->attr[IATTR_ATTR] & IA2_SCENARIO_POTAL ) && ( i->attr[IATTR_ATTR] & IA2_OPENED ) )
        {
            DWORD attr = OBJECTSPRITE_ALPHA;
            if( i->attr[IATTR_ATTR] & IA2_POISONED ) attr = OBJECTSPRITE_TRANS;

            i->Anitype = ITEMANIMATION_ACTIVE;
            int al = 15; al <<= 3;
            i->attr[IATTR_SPRITE_TYPE] = al;
            i->attr[IATTR_SPRITE_TYPE] = (i->attr[IATTR_SPRITE_TYPE] & 0xf8) + attr;

            int j;
            LPITEMSPRITE t;
            for( j = 0 ; j < 99 ; j ++)
            {
                if( j == 0 ) t = AddItemSprite( spr_no , type, 0 );
                else         t = AddItemSprite( spr_no * 100 + j, type, j, 1 );
                if( !t ) break;
                i->spr[j] = &t->sp;
            }
            i->framecount = j;
        }
        else
        {
            LPITEMSPRITE t = AddItemSprite( spr_no , type, 0 );
            if (t) i->spr[0] = &t->sp;
        }
        return;
    }

    // =========================================================
    // 情況 2: 一般道具 (Normal Item < 13)
    // =========================================================
    if( type < 13 )
    {
        InitBaseItem(i);

        // 武器動畫邏輯
        if (i->no/1000 == 6) {
            CItem_Weapon* pWeapon = static_cast<CItem_Weapon*>(ItemUnit(i->no));
            i->Anitype = (pWeapon && pWeapon->effect > 0) ? 1 : 0;
        } else {
            i->Anitype = 0;
        }

        CItem *ii = ItemUnit( type, no );
        if( !ii ) return;

        int spr_no = ii->GetObjectNumber()/100;
        int m;
        char temp[256];

        // 檢查 .CSP 是否存在
        for( m = 0 ; m < 99; m++ )
        {
            sprintf( temp, "item/%05d%02d.CSP", spr_no, m ); // Removed ./
            SDL_RWops* fp = SDL_RWFromFile( temp, "rb" );
            if( fp == NULL ) break;
            SDL_RWclose(fp);
        }
        i->framecount = m;

        for( int j = 0 ; j < m ; j ++)
        {
            LPITEMSPRITE t = AddItemSprite( spr_no * 100 + j, type, 0 );
            if (t) i->spr[j] = &t->sp;
        }
        return;
    }

    // =========================================================
    // 情況 3: 地圖物件 (Map Object) - 這是簡化最多的部分
    // =========================================================

    SDL_RWops* fp = nullptr;

    // 3.1 優先嘗試: Event Object
    if( g_Menu_Variable.m_pEventObjcet ) {
        fp = TryOpenObjectFile("object", tempitemname, g_Menu_Variable.m_pEventObjcet->object_b);
    }

    // 3.2 其次嘗試: Map Toi2 (Binary)
    if( !fp ) {
        char binName[128];
        sprintf(binName, "%s_toi2.b", MapName);
        fp = TryOpenObjectFile("object", tempitemname, binName);
    }

    // 3.3 如果成功開啟 Binary，執行讀取
    if (fp) {
        if (LoadItem_Binary(fp, i, type)) {
            return; // 成功讀取
        }
        // 如果 Binary 讀取失敗(沒找到 ID)，fp 已經在函式內關閉了，繼續往下走 Text 邏輯
    }

    // 3.4 最後嘗試: Map Toi2 (Text) - Fallback
    char txtName[128];
    sprintf(txtName, "%s_toi2.txt", MapName);
    fp = TryOpenObjectFile("object", tempitemname, txtName);

    if (fp) {
        LoadItem_Text(fp, i, type);
    }
}
		
void AddItemList( LPITEMGROUND i )
{		
	LPITEMGROUND t, temp; 
	int mx, my;
		
	if( i->no > 13000 )
	{
		mx = (i->x )/ TILE_SIZE;
		my = (i->y )/ TILE_SIZE;
	}
	else
	{
		mx = (i->x )/ TILE_SIZE;
		my = (i->y + 20)/ TILE_SIZE;
		i->y += 20;
	}

		
	if( g_Item == NULL )	 
	{	
		//MemAlloc( g_Item, sizeof( ITEMGROUND ));
		// 【替換後】
		g_Item = g_itemground_pool.free_list_head;
		if (g_Item == NULL) { return; } // 物件池已滿，直接返回
		g_itemground_pool.free_list_head = g_Item->next;
		g_itemground_pool.free_count--;
		*g_Item = ITEMGROUND(); // 呼叫建構函式重設

		g_Item->server_id = i->server_id;
		g_Item->no	  = i->no;
		g_Item->sx    = i->sx;
		g_Item->sy	  = i->sy;
		g_Item->offx  = i->offx;
		g_Item->offy  = i->offy;
		g_Item->x     = i->x; 
		g_Item->y     = i->y; 
		g_Item->mx    = i->x / TILE_SIZE;
		g_Item->my    = i->y / TILE_SIZE;

		g_Item->attr[0]	= i->attr[0];
		g_Item->attr[1]	= i->attr[1];
		g_Item->attr[2]	= i->attr[2];
		g_Item->attr[3]	= i->attr[3];
		g_Item->attr[4]	= i->attr[4];
		g_Item->attr[5]	= i->attr[5];

		LoadItem( g_Item );  

		g_Item->prev = NULL; 
		g_Item->next = NULL; 
	}	
	else
	{	
		t = NULL;
		//MemAlloc( t, sizeof( ITEMGROUND ));
		t = g_itemground_pool.free_list_head;
		if (t == NULL) { return; } // 物件池已滿，直接返回
		g_itemground_pool.free_list_head = t->next;
		g_itemground_pool.free_count--;
		*t = ITEMGROUND(); // 呼叫建構函式重設

		t->server_id= i->server_id;
		t->no		= i->no;
		t->sx		= i->sx;
		t->sy		= i->sy;
		t->offx		= i->offx;
		t->offy		= i->offy;
		t->x		= i->x;
		t->y		= i->y;
		t->mx		= i->x / TILE_SIZE;
		t->my		= i->y / TILE_SIZE;
		
		t->attr[0]	= i->attr[0];
		t->attr[1]	= i->attr[1];
		t->attr[2]	= i->attr[2];
		t->attr[3]	= i->attr[3];
		t->attr[4]	= i->attr[4];
		t->attr[5]	= i->attr[5];


		LoadItem( t );
		temp = g_Item;
		temp->prev = t;
		t->next = temp;
		t->prev = NULL;
		g_Item = t;
	}	
		
	// Add�� Item�� Animation����..
//-----------------------
		if( g_Item->attr[2] & IA2_FARM0 )	g_Item->Anitype = ITEMANIMATION_FARM0;
		if( g_Item->attr[2] & IA2_FARM1 )	g_Item->Anitype = ITEMANIMATION_FARM1;
		if( g_Item->attr[2] & IA2_FARM2 )	g_Item->Anitype = ITEMANIMATION_FARM2;
		if( g_Item->attr[2] & IA2_FARM3 )	g_Item->Anitype = ITEMANIMATION_FARM3;
		if( g_Item->attr[2] & IA2_FARM4 )	g_Item->Anitype = ITEMANIMATION_FARM4;
		if( g_Item->attr[2] & IA2_FARM5 )	g_Item->Anitype = ITEMANIMATION_FARM5;
		if( g_Item->attr[2] & IA2_FARM6 )	g_Item->Anitype = ITEMANIMATION_FARM6;
		if( g_Item->attr[2] & IA2_FARM7 )	g_Item->Anitype = ITEMANIMATION_FARM7;

		if( g_Item->attr[2] & IA2_FIRE  )	g_Item->Anitype = ITEMANIMATION_FIRE;
		
		if( g_Item->attr[ IATTR_ATTR] & IA2_BOX )
		{
			g_Item->type = ITEMTYPE_BOX;
			if( g_Item->attr[ IATTR_ATTR] & IA2_OPENED )	g_Item->Anitype = ITEMANIMATION_OPENED;
			else											g_Item->Anitype = ITEMANIMATION_CLOSED;
		}
		else if( g_Item->attr[ IATTR_ATTR] & IA2_DOOR )
		{
			g_Item->type = ITEMTYPE_DOOR;
			if( g_Item->attr[ IATTR_ATTR] & IA2_OPENED )	g_Item->Anitype = ITEMANIMATION_OPENED;
			else											g_Item->Anitype = ITEMANIMATION_CLOSED;
		}
		else if( g_Item->attr[ IATTR_ATTR] & IA2_TRAPED1 )	g_Item->curframe = rand()%4+1;
		else 
		{
			CItem *t = ItemUnit( g_Item->no / 1000, g_Item->no % 1000 );
			if( t )
			{//wait i think iknow why cause the mapserver onlym generates it normally and not by weapon type plant type ect so would think its a normal itemaccessory ok
				if( t->GetMouse_MR_Effect() )
					g_Item->Anitype = ITEMANIMATION_REPEAT;
			}
		}

}		
		
void DeleteItemList( int server_id )
{		
	int i;
	LPITEMGROUND  t = g_Item;
			
	while( t != NULL )
	{		
		if( t->server_id == server_id )
		{	
			if( t == g_Item )
			{
				t = g_Item->next;
				g_DeleteItem.push_back(g_Item);
				/*for (i = 0; i < g_Item->framecount; i++)
					DeleteItemSprite( g_Item->no * 100 + i );
				
				MemFree( g_Item );*/
				g_Item = t;
				if( t != NULL )	g_Item->prev = NULL;
				return;
			}
			else 
			{
				if ( t->prev != NULL )
				{
					t->prev->next = t->next;
				}
				if( t->next != NULL )
				{
					t->next->prev = t->prev;
				}
				g_DeleteItem.push_back(t);
				/*for (i = 0; i < t->framecount; i++)
					DeleteItemSprite( t->no * 100 + i );
				MemFree( t );*/
		
				return;
			}
		}	
		t = t->next;
	}		
}		

void FlushPendingDeletesItemList() {
	// 如果待刪除列表中有東西
	if (!g_DeleteItem.empty())
	{
		for (LPITEMGROUND item_to_delete : g_DeleteItem)
		{
			for (int i = 0; i < item_to_delete->framecount; i++) {
				DeleteItemSprite(item_to_delete->no * 100 + i);
			}

			// 【【【 關鍵修正：歸還到池中 】】】

			// 【替換前】
			// MemFree(item_to_delete);

			// 【替換後】
			// 1. 結束物件生命週期 (呼叫解構函式)
			//item_to_delete->~ITEMGROUND();

			// 2. 歸還到 free list
			item_to_delete->next = g_itemground_pool.free_list_head;
			g_itemground_pool.free_list_head = item_to_delete;
			g_itemground_pool.free_count++;
			// 【【【 修正結束 】】】
		}
		g_DeleteItem.clear();
	}
}

//	
LPITEMGROUND FindItemList( int server_id )
{	   	
	LPITEMGROUND t;
	t = g_Item;
	while( t != NULL )
	{	
		if( t->server_id == server_id )
		{
			return t;
		}
		t = t->next;
	}		
	return NULL;
}		
			
	
	
	
//	���� ������ǥ...
LPITEMGROUND FindItemList( int x, int y )
{	   	
	LPITEMGROUND t;
	t = g_Item;
	while( t != NULL )
	{	
		if( BoxAndDotCrash( x - 16, y-16, 32, 32, t->x, t->y ) )
		{
			return t;
		}
		t = t->next;
	}		
	return NULL;
}	

void DestroyItemList(void)
{
	LPITEMGROUND current_item = g_Item; // 從鏈結串列的頭部開始
	LPITEMGROUND next_item = NULL;     // 用來儲存下一個節點的指標

	while (current_item != NULL)
	{
		// 1. 先記住下一個節點，因為 current_item 馬上要被回收了
		next_item = current_item->next;

		// 2. 釋放這個道具使用的所有 Sprite 資源 (這部分邏輯不變)
		for (int i = 0; i < current_item->framecount; i++)
		{
			DeleteItemSprite(current_item->no * 100 + i);
		}

		// 3. 【【【 關鍵修正：歸還到物件池 】】】
		//   (移除 MemFree( current_item ); )

		//   a. 結束物件生命週期 (呼叫解構函式)
		//current_item->~ITEMGROUND();

		//   b. 將節點加回 free list 的頭部
		current_item->next = g_itemground_pool.free_list_head;
		g_itemground_pool.free_list_head = current_item;
		g_itemground_pool.free_count++;
		// 【【【 修正結束 】】】

		// 4. 移動到下一個節點
		current_item = next_item;
	}

	// 5. 【關鍵】清空全域的 g_Item 指標，表示道具列表現在是空的
	g_Item = NULL;

	return;
}

/*
void DestroyItemList( void )
{	
	LPITEMGROUND t;
	int i;
	t = g_Item;
	while( t != NULL )
	{	
		t = g_Item->next;
			for( i = 0 ; i < g_Item->framecount ; i ++)
		DeleteItemSprite( g_Item->no * 100 + i );
		MemFree( g_Item );
		g_Item = t;
	}		
		
	return;
}		
*/		
		
				
//---------------  Item Animation... 
	
	
void RecvItemAttrChange( t_item_attr_change *p )
{	
	LPITEMGROUND i = FindItemList( p->item_id );
	if( i == NULL ) return;
	int oldattr = i->attr[ p->array];
	int attr = p->attr;
	
	
 switch( p->array )
 {
	case IATTR_ATTR :
	{

		//		110       �츮����.wav
		//		111       �츮������.wav

		if( oldattr & IA2_DOOR )// ���� ���� �Ӽ� Check....
		{
			if( oldattr & IA2_OPENED ) // ��������..
			{
				if( ( attr & IA2_OPENED ) == 0 ) // �� ����������̱�.
					i->Anitype = ITEMANIMATION_CLOSING;

				int distx=i->x-Hero->x;						// LTS SOUND
				int disty=i->y-Hero->y;
				PlayListAutoSounds( 141,distx,disty,0 );
			}
			else
			{
				if( (attr & IA2_OPENED) && (i->Anitype != ITEMANIMATION_OPENED) ) // �� ����� ����̱�.
					i->Anitype = ITEMANIMATION_OPENNING;

				int distx=i->x-Hero->x;						// LTS SOUND
				int disty=i->y-Hero->y;
				PlayListAutoSounds( 140,distx,disty,0 );
			}
		}
		else
		if( oldattr & IA2_BOX )// Box�� ���� �Ӽ� Check..
		{
			if( oldattr & IA2_OPENED ) // ��������..
			{
				if( ( attr & IA2_OPENED ) == 0 ) // �� ����������̱�.
					i->Anitype = ITEMANIMATION_CLOSING;
				int distx=i->x-Hero->x;						// LTS SOUND
				int disty=i->y-Hero->y;
				PlayListAutoSounds( 149,distx,disty,0 );

			}
			else
			{
				if( (attr & IA2_OPENED)  && (i->Anitype != ITEMANIMATION_OPENED) ) // �� ����� ����̱�.
					i->Anitype = ITEMANIMATION_OPENNING;
				int distx=i->x-Hero->x;						// LTS SOUND
				int disty=i->y-Hero->y;
				PlayListAutoSounds( 148,distx,disty,0 );

			}
		}

		if( attr & IA2_FARM0 )	i->Anitype = ITEMANIMATION_FARM0;
		if( attr & IA2_FARM1 )	i->Anitype = ITEMANIMATION_FARM1;
		if( attr & IA2_FARM2 )	i->Anitype = ITEMANIMATION_FARM2;
		if( attr & IA2_FARM3 )	i->Anitype = ITEMANIMATION_FARM3;
		if( attr & IA2_FARM4 )	i->Anitype = ITEMANIMATION_FARM4;
		if( attr & IA2_FARM5 )	i->Anitype = ITEMANIMATION_FARM5;
		if( attr & IA2_FARM6 )	i->Anitype = ITEMANIMATION_FARM6;
		if( attr & IA2_FARM7 )	i->Anitype = ITEMANIMATION_FARM7;


		if( attr & IA2_FIRE  )	i->Anitype = ITEMANIMATION_FIRE;

		if( attr & IA2_TRAPED1  )		i->curframe = (rand()%4)+1;
		if( !(attr & IA2_TRAPED1)   )	i->curframe = 0;
	}
	break;

	case IATTR_DURATION :
		{
			break;
		}

 }

	i->attr[ p->array] = p->attr;
}	
	
void SendItemDoorOpenClose( LPITEMGROUND i, int openclose )
{
	t_packet p;

	p.h.header.type = CMD_ITEM_DOOR_OPEN_CLOSE;
	{
		p.u.item_door_open_close.item_id = i->server_id;
		p.u.item_door_open_close.openclose = openclose;
	}
	p.h.header.size = sizeof( t_item_door_open_close );

	QueuePacket( &p, 1 );
}



	
void ToggleItemAnimation( LPITEMGROUND i )
{	
#ifdef _DEBUG // finito 060507
	if( SysInfo.notconectserver )
	{
		switch( i->Anitype )
		{	
		case ITEMANIMATION_OPENED :		i->Anitype = ITEMANIMATION_CLOSING; 
			break;
		case ITEMANIMATION_CLOSED :		i->Anitype = ITEMANIMATION_OPENNING; 
			break;
		}	
	}
	else 
#endif
	{
		switch( i->type )
		{
		case ITEMTYPE_DOOR : if( i->Anitype == ITEMANIMATION_CLOSED ) { SendItemDoorOpenClose( i, 0 );	break; }
							 if( i->Anitype == ITEMANIMATION_OPENED ) { SendItemDoorOpenClose( i, 1 );	break; }
				break;
							
//		case ITEMTYPE_BOX  : if( i->Anitype == ITEMANIMATION_CLOSED ) Send_CMD_ITEM_BOX_BREAK( i );	break;
//				break;
		}
	}

//	SendOpenDoor( i->server_id );
}		

void check_getable(SDL_Rect rc, LPITEMGROUND ground_item) {

	if (!BoxAndDotCrash(rc.x, rc.y, rc.w, rc.h, g_pointMouseX, g_pointMouseY)) {
		return;
	}

	if (LButtonDownIng != 1 || g_MouseInMenu) {
		return;
	}
	
	int x = 0, y = 0;
	if (!(ground_item->attr[2] & IA2_NOTMOVE) &&
		ground_item->getable)
	{
		CHARACTER t;
		if ((TileMap[Hero->x / TILE_SIZE][Hero->y / TILE_SIZE].attr_inside ==
			TileMap[ground_item->x / TILE_SIZE][ground_item->y / TILE_SIZE].attr_inside))	//  �ǳ��� �ְų� �ǿܿ� �־�� ������ �ݰų� ������ �ִ�. 
			if (Distance(ground_item->x, ground_item->y, Hero->x, Hero->y) < 10000)
			{
				SendTakeItem(ground_item->server_id);
				LButtonDownIng = 0;
				return;
			}
			/*else
			{
				x = Hero->x;	y = Hero->y;
				int mx, my;

				if (ReturnXYNearPosition(&x, &y, &mx, &my, 64, Hero, ground_item->x, ground_item->y))
				{
					memcpy(&t, Hero, sizeof(CHARACTER));
					t.moveFlag = TRUE;
					t.movetype = 0;
					t.gox = x;
					t.goy = y;
					t.destx = mx, t.desty = my;
					if (PathBuild(&t))
					{
						t.moveFlag = FALSE;
						t.gox = x;
						t.goy = y;
						SendMoveData(&t);
					}
				}
			}*/
	}
}
		
bool checK_show(int item_no, unsigned int attr[6]) {
	
	static std::vector<int> itemnoList = { 1046, 1047, 1048, 1049, 1050 };

	if (attr[IATTR_RARE_MAIN] > 0) return true;
	if (item_no / 1000 == ETC) return true;
	auto find_it = find(itemnoList.begin(), itemnoList.end(), item_no);
	if (find_it != itemnoList.end()) {
		return true;
	}
	return false;
}
		
void DrawItemInfoOnScreen() {

	LPITEMGROUND it = g_Item;
	const int iWidth = TXT_BG_SHORT_WIDTH; // 設定文字框的寬度
	int x = 0, y = 0;
	// 初始化一些計數器變數
	int iReultLineCount = 0;
	int iReultBlankSize = 0;
	int iCt = 0, iBlk = 0;
	// 從 TxtOut 物件取得行距和空白間隔的大小
	const int iLGab = TxtOut.GetLineGab();
	const int iBlkSize = TxtOut.GetBlankGabSize();
	
	while (it != nullptr) {
		
		if (!checK_show(it->no, it->attr)) {
			it = it->next;
			continue;
		}

		x = (it->x - Mapx)-iWidth/2;
		y = (it->y - Mapy)- 50;
		ItemAttr t{};
		t.item_no = it->no;
		memcpy(t.attr, it->attr, sizeof(t.attr));

		// 取得物品的稀有屬性
		const RareMain* pRareValue = (RareMain*)(&t.attr[IATTR_RARE_MAIN]);

		// 透過物品屬性取得物品的基礎物件
		CItem* item = ItemUnit(t);
		if (!item) return; // 如果物品不存在，直接返回

		// 判斷物品是否有稀有屬性 (IATTR_RARE_MAIN 不為 0)
		if (t.attr[IATTR_RARE_MAIN])
		{
			// 根據稀有等級 (HighLevel) 決定如何顯示
			switch (pRareValue->iHighLevel)
			{
				// --- 情況一：一般稀有物品 (藍綠色) ---
			case H_LV_NO_HIGH_ITEM:
			{
				TxtOut.Init(COLOR_BLUEGREEN, iWidth);
				TxtOut.PutTextHeaderBGImg(x, y + iBlk + iLGab * iCt, iWidth);

				Hcolor(FONT_COLOR_RARE_MAIN); // 設定稀有屬性文字顏色
				// 迴圈顯示最多三個稀有屬性
				for (int ct = 0; 3 > ct; ct++)
				{
					int iIndex = 0; // 屬性索引
					switch (ct) // 取得第 ct 個屬性
					{
					case 0: { iIndex = pRareValue->soksung1; } break;
					case 1: { iIndex = pRareValue->soksung2; } break;
					case 2: { iIndex = pRareValue->soksung3; } break;
					default: { continue; } break;
					}
					if (!iIndex || iIndex >= 200) { continue; } // 如果屬性不存在，跳過
					
					// 根據 bIsBgOut 決定呼叫有背景或無背景的繪製函式
					// 顯示屬性名稱和稀有等級 (Grade)
					if (pRareValue->IsDynamicRare) { iReultLineCount = TxtOut.RcTXTOut(x, y + iBlk + iLGab * iCt, iWidth, 0, ItemFunction[iIndex].NameAdd); }
					else { iReultLineCount = TxtOut.RcTXTOut(x, y + iBlk + iLGab * iCt, iWidth, 0, ItemRare[iIndex].ExplainAdd); }
					//TxtOut.RcTXTOutNoBG(x + 40, y + iBlk + iLGab * iCt, iWidth, 0, "                 %d Grade", pRareValue->grade);
					iCt += iReultLineCount;
				}

				// 最後繪製物品的原始名稱 (亮綠色)
				Hcolor(FONT_COLOR_BLIGHT_GREEN);
				if (1) {
					if (system_info.lang == 0) iReultLineCount = TxtOut.RcTXTOutB(x, y + iBlk + iLGab * iCt, iWidth, 0, "<%s>", item->GetItemName());
					else iReultLineCount = TxtOut.RcTXTOutB(x, y + iBlk + iLGab * iCt, iWidth, 0, "<%s>", item->GetItemHanName());
					iCt += iReultLineCount;
				}
			}break;
			// --- 情況二：高級稀有物品 (紅色) ---
			case H_LV_HIGH_ITEM:
			{
				TxtOut.Init(COLOR_RED, iWidth);
				Hcolor(FONT_COLOR_BLIGHT_GREEN);
				if (1) {
					TxtOut.PutTextHeaderBGImg(x, y + iBlk + iLGab * iCt, iWidth);
					iBlk += TxtOut.RcBlankOut(x, y + iBlk + iLGab * iCt, iWidth, 1);

					// 格式化輸出，例如：<堅固的 鐵劍 +5>
					if (system_info.lang == 0) iReultLineCount = TxtOut.RcTXTOutB(x, y + iBlk + iLGab * iCt, iWidth, 0, "<%s %s +%d>", ItemRare[pRareValue->soksung1].ExplainAdd, item->GetItemName(), pRareValue->grade);
					else iReultLineCount = TxtOut.RcTXTOutB(x, y + iBlk + iLGab * iCt, iWidth, 0, "<%s %s +%d>", ItemRare[pRareValue->soksung1].ExplainAdd, item->GetItemHanName(), pRareValue->grade);

					// 再用亮紅色單獨繪製一次前綴，達到特殊效果
					Hcolor(FONT_COLOR_BLIGHT_RED);
					TxtOut.RcTXTOutBNoBG(x + 1, y + iBlk + iLGab * iCt, iWidth, 0, "  %s", ItemRare[pRareValue->soksung1].ExplainAdd);
					iCt += iReultLineCount;
				}
			}break;
			// --- 情況三：傳說物品 (藍色/黃色) ---
			case H_LV_LEGEND_ITEM:
			{
				TxtOut.Init(COLOR_BLUE, iWidth);
				Hcolor(FONT_COLOR_HIGH_RARE_MAIN);
				if (1) {
					TxtOut.PutTextHeaderBGImg(x, y + iBlk + iLGab * iCt, iWidth);
					iBlk += TxtOut.RcBlankOut(x, y + iBlk + iLGab * iCt, iWidth, 1);

					// 用黃色顯示傳說稱號，例如：= 龍騎士的證明 =
					Hcolor(FONT_COLOR_SOLID_YELLOW);
					iReultLineCount = TxtOut.RcTXTOutB(x, y + iBlk + iLGab * iCt, iWidth, 0, "= %s %s =", lan->OutputMessage(2, 780), g_aItemLegend[pRareValue->soksung2].aSub[pRareValue->soksung3].szName);
					iCt += iReultLineCount;

					// 再用亮綠色顯示物品原始名稱
					Hcolor(FONT_COLOR_BLIGHT_GREEN);
					if (system_info.lang == 0) iReultLineCount = TxtOut.RcTXTOutB(x, y + iBlk + iLGab * iCt, iWidth, 0, "<%s>", item->GetItemName());
					else iReultLineCount = TxtOut.RcTXTOutB(x, y + iBlk + iLGab * iCt, iWidth, 0, "<%s>", item->GetItemHanName());
					iCt += iReultLineCount;
				}
			}break;
			}
		}
		// --- 如果不是稀有物品，就只顯示普通名稱 ---
		else
		{
			TxtOut.Init(COLOR_BLACK, iWidth);
			Hcolor(FONT_COLOR_BLIGHT_GREEN);
			if (1) {
				TxtOut.PutTextHeaderBGImg(x, y + iBlk + iLGab * iCt, iWidth);
				iBlk += TxtOut.RcBlankOut(x, y + iBlk + iLGab * iCt, iWidth, 1);
				if (system_info.lang == 0) iReultLineCount = TxtOut.RcTXTOutB(x, y + iBlk + iLGab * iCt, iWidth, 0, "<%s>", item->GetItemName());
				else iReultLineCount = TxtOut.RcTXTOutB(x, y + iBlk + iLGab * iCt, iWidth, 0, "<%s>", item->GetItemHanName());
				iCt += iReultLineCount;
			}
		}
		TxtOut.PutTextTailBGImg(x, y + iBlk + iLGab * iCt, iWidth);
		SDL_Rect rc = { x, y , iWidth, iBlk + iLGab * iCt };
		check_getable(rc, it);
		it = it->next;
	}
}
	
			
		
//	ȭ�鿡 ����ϸ鼭 ���콺�� �ɸ����� ã�´�.
void DrawItemList( void )
{			
	LPITEMGROUND t = g_Item;
	static int light;
	static int toggle;
	Spr *s;	
	LPORDERTABLE o;
			
	if( toggle ){	++light;	if( light >= 12 )	toggle = !toggle;	}
	else		{	--light;	if( light <= 0 )	toggle = !toggle;	}
			
	PathCollisionLineCount = 0; //   ��?�⿡ ���� �������� �浹������ PathCollisionLine[ MAX_ITEM_COLLISION ]�� ��� ���´�.
			
	CursorCheckedItem = NULL;
	while( t != NULL )
	{		
		// 檢查 t->curframe 是否越界
		int frame_index = t->curframe / 2;
		if (frame_index < 0 || frame_index >= 10) {
			//SDL_Log("Error: curframe index (%d) is out of bounds [0, %d)", frame_index, t->framecount);
			t->curframe = 0; // 嘗試修復
			frame_index = 0;
		}
		// ========= 健全性檢查結束 =========
		s = t->spr[frame_index];
		if( s == NULL ) { goto NEXT_; }
			
		if( BoxAndBoxCrash( t->x - s->ox + t->offx, t->y - s->oy + t->offy, s->xl, s->yl, Mapx, Mapy, GAME_SCREEN_XSIZE, GAME_SCREEN_YSIZE ) )
		{	
			if (g_OrderInfo.count >= MAX_ORDERTABLE) break;
			o = &g_OrderInfo.order[ g_OrderInfo.count ];
			o->rgb			= 0; // ���콺�� ���� Check�Ǿ���. 
			
			if( (t->attr[ IATTR_ATTR] & IA2_BOX) && (t->attr[ IATTR_ATTR] & IA2_OPENED ) ) { goto NEXT_OPENED_BOX_; }
			if( t->attr[ IATTR_ATTR] & IA2_TRAPED0 || t->attr[ IATTR_ATTR] & IA2_TRAPED1) 
			{	
				// ���� ������ �ִ� ��ź�̳� ���� ã�� ������ item�� Ŭ���Ǿ�� �Ѵ�. 
				if( SkillStatus && (SkillNo == 16 || SkillNo == 18 || SkillNo == 5 || SkillNo == 6 ))
				{
					//1019 zhh	//6	Remove Traps //5	Find Traps	//16	Detect Bomb	//18	Remove Bomb										
					if( (CanViewBomb(Hero) && SkillNo == 18 ) || (CanViewTrap(Hero) && SkillNo == 6 ) )//020320 lsw
					{						
					//	CursorCheckedItem = t;
					//	if(o->rgb		!= 1)	o->rgb		= 1;						
					//	if(o->alpha		!= light)	o->alpha	= light;
						goto zoung_;
					}
					//1019 zhh //�������
				}
				goto NEXT_OPENED_BOX_; 
			}
zoung_:

			if( g_MouseItemType == 0 ) 
//			if( t->host == Hero->id || t->host == 100000000 ) // �����ų� ������ ���°��.
			{
				int x = t->x-s->ox+t->offx;
				int y = t->y-s->oy+t->offy;

				if( Hero->viewtype == VIEWTYPE_GHOST_ 
					|| !IsDead( Hero ) 
					|| Hero->viewtype == VIEWTYPE_OBSERVE_)
				if( ( (Hero->x-x)*(Hero->x-x) + (Hero->y-y)*(Hero->y-y) < 30000 
					&& !(t->attr[ IATTR_ATTR] & IA2_BOX )) || (t->attr[ IATTR_ATTR] & IA2_BOX ) ) // Box��� �ƹ��� �ָ� �־ ���õɼ� �ִ�. 
				if( ( Hero->viewtype == VIEWTYPE_GHOST_ && ( t->attr[ IATTR_ATTR] & IA2_DOOR ) ) 
					|| Hero->viewtype != VIEWTYPE_GHOST_ )// Ghost���¿����� Item�� ���� ���� �ִ�. 
				if( BoxAndDotCrash( x, y, s->xl, s->yl, Mox, Moy ) )
				if( CursorCheckedItem == NULL )
				if( (Hero->canviewitem && (t->attr[ IATTR_ATTR] & IA2_HIDE) ) 
					|| !(t->attr[ IATTR_ATTR] & IA2_HIDE) )
				{
						//if( Distance( Mox, Moy, Hero->x, Hero->y ) < 10000 )
					{	//if( (t->attr[ IATTR_ATTR] & IA2_BOX) == 0 ) 
						CursorCheckedItem = t;
					}
					o->rgb		= 1;
					o->alpha	= light;
					
				}	
			}

NEXT_OPENED_BOX_:

			if( t->attr[ IATTR_ATTR] & IA2_FIRE )	OutputLight( t->x, t->y, 10 );


				
			o->wType		= SPRITETYPE_ITEM; //t->Sorttype;
			o->x			= t->x;
			o->y			= t->y;
			o->height		= 0;
			o->offset		= t->y;
			o->lpvData		= (LPVOID)t;
			o->sort			= 1;
			o->show			= 1;
			
			g_OrderInfo.count++;


			if(  t->type == ITEMTYPE_DOOR )
				if( t->Anitype == ITEMANIMATION_CLOSED ) 
				{
					PathCollisionLine[ PathCollisionLineCount].sx = t->dsx[0];
					PathCollisionLine[ PathCollisionLineCount].sy = t->dsy[0];
					PathCollisionLine[ PathCollisionLineCount].dx = t->ddx[0];
					PathCollisionLine[ PathCollisionLineCount].dy = t->ddy[0];
					PathCollisionLineCount++;
				}
		}			
		else	
		{		
			//	���࿡ AREA�ۿ� �ִٸ� DeleteItemList( t->server_id )ȣ���� ������.
		}		
			
		switch( t->Anitype )
		{	
		case ITEMANIMATION_NO : 
			break;
		case ITEMANIMATION_REPEAT	:	t->curframe ++;
										if( t->curframe / 2 >= t->framecount ) t->curframe = 0;
			break;
		case ITEMANIMATION_RETURN	:	if( t->anitoggle )
										{
											t->curframe ++;
											if( t->curframe / 2 >= t->framecount ) { t->curframe--; t->anitoggle = !t->anitoggle; }
										}
										else 
										{
											t->curframe --;
											if( t->curframe / 2 < 0 ) { t->curframe++; t->anitoggle = !t->anitoggle; }
										}
			break;
		case ITEMANIMATION_OPENNING	:	t->curframe ++;
			 							if( t->curframe /2 >= t->framecount ) { t->curframe = (t->framecount*2) -1; t->Anitype = ITEMANIMATION_OPENED; }
			break;
		case ITEMANIMATION_OPENED	:	t->curframe = (t->framecount*2) -1;
			break;
		case ITEMANIMATION_CLOSING	:	t->curframe --;
			 							if( t->curframe < 0  ) { t->curframe = 0; t->Anitype = ITEMANIMATION_CLOSED; }
			break;
		case ITEMANIMATION_CLOSED	:	t->curframe = 0;
			break;


		case ITEMANIMATION_FARM0	:	t->curframe = 0; break;
		case ITEMANIMATION_FARM1	:	t->curframe = 2; break;
		case ITEMANIMATION_FARM2	:	t->curframe = 4; break;
		case ITEMANIMATION_FARM3	:	t->curframe = 6; break;
		case ITEMANIMATION_FARM4	:	t->curframe = 8; break;
		case ITEMANIMATION_FARM5	:	t->curframe = 10; break;
		case ITEMANIMATION_FARM6	:	t->curframe = 12; break;
		case ITEMANIMATION_FARM7	:	t->curframe = 14; break;

		case ITEMANIMATION_ACTIVE :		// 020701 YGI
		case ITEMANIMATION_FIRE		:	t->curframe ++;
										if( t->curframe / 2 >= t->framecount ) t->curframe = 2;
				break;		
		}	 
NEXT_:        
             
		t = t->next;
	}		 	
}			 
////////////////////////////////////////////////////////////////////////////
			 

//  �������� ���´�.
//  �������� 1�� ����.
int CheckEventWith_Item( void )
{			  
	if( g_StartMenuOn == TRUE )  return 0;
	if( IsMouseOnMenu() ) return 0;

	int x, y;
	CHARACTER t;
	if( CursorCheckedItem != NULL )
	{		  
		if( SkillStatus == SKILL_OFF )
		if( !(CursorCheckedItem->attr[2] & IA2_NOTMOVE) &&
			CursorCheckedItem->getable )
		{
			if( (TileMap[ Hero->x / TILE_SIZE][ Hero->y / TILE_SIZE].attr_inside ==
				TileMap[ CursorCheckedItem->x / TILE_SIZE][ CursorCheckedItem->y / TILE_SIZE].attr_inside ) )	//  �ǳ��� �ְų� �ǿܿ� �־�� ������ �ݰų� ������ �ִ�. 
			if( Distance( CursorCheckedItem->x, CursorCheckedItem->y, Hero->x, Hero->y ) < 20736 ) //原本是10000 = 3.15格左右,改成 20736就是4.5格
			{	  
				SendTakeItem( CursorCheckedItem->server_id );              
				return 1;
			}			
			else 
			{	
				x = Hero->x;	y = Hero->y;
				int mx, my;

				if( ReturnXYNearPosition( &x, &y, &mx, &my, 64, Hero, CursorCheckedItem->x, CursorCheckedItem->y ) )
				{
					memcpy( &t, Hero, sizeof( CHARACTER ) );
					t.moveFlag = TRUE;
					t.movetype = 0;
					t.gox = x;
					t.goy = y;
					t.destx = mx, t.desty = my;
					if( PathBuild( &t )) 
					{	
						t.moveFlag = FALSE;
						t.gox = x;
						t.goy = y;
						SendMoveData( &t );
					}		
				}		
			}			
		}					
		else				
		{	  
			switch( CursorCheckedItem->type ) 
			{ 
			case ITEMTYPE_COLOSSUS_STONE  :
			case ITEMTYPE_BOX  :	return 0;
//			  			DoHeroAttackBox( int x, int y )
				break;
              
			case ITEMTYPE_DOOR :	
			  			ToggleItemAnimation( CursorCheckedItem );
			  	break;
			} 
		}	  
		return 1;
	}		  
			  
	return 0; 
}			




void DrawObjectItem_SealStone( void )
{
	if( tool_ViewAllCharacter == false ) return;
	if( tool_SkillTool_ITEM_SEAL == false ) return;


	return;



	int i;
	int mxl, myl;
	int px, py;

	mxl = g_Map.file.wWidth  *TILE_SIZE;
	myl = g_Map.file.wHeight *TILE_SIZE;

	for( i = 0 ; i < TotalMapObject ; i ++)
	{
		if( Mo[ i].objectoritem == ITEMTYPE_SEAL_NOR ||
			Mo[ i].objectoritem == ITEMTYPE_SEAL_SPC )
		{
			px = Mo[i].x * RADARXL / mxl;
			py = Mo[i].y * RADARYL / myl;

			Hcolor( 255,255,100 );
			Hprint2( px, py, g_DestBackBuf, lan->OutputMessage(9,10), Mo[ i].dum );//010215 lsw
		}
	}


	return;

	/*

	LPITEMGROUND t = g_Item;
	int mxl, myl;
	int px, py;
	WORD color;
	int sxl, syl;
	static int tog;
	int smx, smy, smxl, smyl;
			
	mxl = g_Map.file.wWidth  * TILE_SIZE;
	myl = g_Map.file.wHeight * TILE_SIZE;
	
	while( t != NULL )
	{		
		if( (t->attr[ IATTR_ATTR] & IA2_SEAL_NOR ) || (t->attr[ IATTR_ATTR] & IA2_SEAL_SPC ) ) 
		{	
			px = t->x * TILE_SIZE * RADARXL / mxl;
			py = t->y * TILE_SIZE * RADARYL / myl;
			Hcolor( 255,255,100 );
			Hprint2( px, py, g_DestBackBuf, "��%d", t->attr[ IATTR_SEAL_STONE_NO] );
		}
             
		t = t->next;
	}		 	
	*/
}



void CheckTrap( void )
{			
	int x, y;
	static int mx, my;
			
	x = Hero->x;
	y = Hero->y;
	if( mx == x / TILE_SIZE && my == y/ TILE_SIZE ) return ;
	mx = x/ TILE_SIZE, my = y / TILE_SIZE;
			
	LPITEMGROUND  t = g_Item;
			
	while( t != NULL )
	{		
		if( ( t->attr[IATTR_ATTR] & IA2_TRAPED0 ) || 
			( t->attr[IATTR_ATTR] & IA2_TRAPED1 ) )
		if( BoxAndDotCrash( t->x - 16, t->y - 16, TILE_SIZE, TILE_SIZE, x, y ) )
		{	
			if( t->attr[IATTR_ATTR] & IA2_TRAPED0 ) 
			{
				SendCMD_FOOT_ON_TRAP_0_ITEM( t->server_id );
			}
			else if( t->attr[IATTR_ATTR] & IA2_TRAPED1 )
			{
				SendCMD_FOOT_ON_TRAP_1_ITEM( t->server_id );
			}
			
			return;
		}	
			
		t = t->next;
	}		
			
	return;	
}			
			
//extern int aItemInventory[3][3][8];
extern ItemAttr  InvItemAttr[3][3][8];
			
void RecvGiveItem( t_server_give_item *p )
{			
	InvItemAttr[p->p1][p->p2][p->p3]   = p->item;

	//Hero->stolen = true;
}			

	 
void Send_CMD_JUST_ATTACK_ANIMATION( LPITEMGROUND i )
{	
	t_packet p;
	
	p.h.header.type = CMD_JUST_ATTACK_ANIMATION;
		p.u.client_just_attack_animation.item_id = i->server_id;
	p.h.header.size = sizeof( t_client_just_attack_animation );
	
	QueuePacket( &p, 1 );
}


void Send_CMD_ITEM_BOX_BREAK( LPITEMGROUND i )
{	
	t_packet p;
	
	p.h.header.type = CMD_ITEM_BOX_BREAK;
	{
		p.u.client_item_box_break.item_id = i->server_id;
	}
	p.h.header.size = sizeof( t_client_item_box_break );
	
	QueuePacket( &p, 1 );
}	
	
void Recv_CMD_ITEM_BOX_BREAK( t_server_item_box_break *p )
{	
	int ani;
	LPITEMGROUND i = FindItemList( p->item_id );
	if( i == NULL ) return;
	LPCHARACTER ch = ReturnCharacterPoint( p->server_id );
	if( ch == NULL ) return;

	//if( ch != Hero ) 
	{
		ch->nAttackedType	= SPRITETYPE_ITEM;
		ch->lpAttacked		= (void *)i;

		ani = AttackAction( ch );

		if( ani == ch->nCurrentAction )	return;
		else  ch->nCurrentAction = ani;
		ChangeDirection( &ch->direction, ch->x, ch->y, i->x, i->y );
		ch->todir = ch->direction;
		ch->nCurrentFrame.PutEncrypted(0);
	}

	ch->boxitembreaktype = p->type;
}

void Send_CMD_ITEM_BOX_BREAK_RESULT( int item_id )
{
	t_packet p;

	p.h.header.type = CMD_ITEM_BOX_BREAK_RESULT;
		p.u.client_item_box_break_result.item_id = item_id;
	p.h.header.size = sizeof( t_client_item_box_break_result );

	QueuePacket( &p, 1 );
}

void Send_CMD_ITEM_BOX_MAGIC_BREAK( int item_id )
{
	t_packet p;

	p.h.header.type = CMD_ITEM_BOX_MAGIC_BREAK;
		p.u.client_item_box_magic_break.item_id = item_id;
	p.h.header.size = sizeof( t_client_item_box_magic_break );

	QueuePacket( &p, 1 );
}
void Recv_CMD_ITEM_BOX_MAGIC_BREAK_RESULT( t_server_item_box_magic_break_result *p )
{
	LPITEMGROUND i = FindItemList( p->item_id );
	if( i == NULL ) return;

	if( p->type == 0 ) //  ����..
	{
		InsertMagic( Hero, Hero, 208,0, i->x, i->y, 0,0 );

	}
	else 
	{

	}
}

void InitItemAttrs()
{
	memset(InvItemAttr		,0,sizeof(ItemAttr)*3*3*8);
	memset(EquipItemAttr	,0,sizeof(ItemAttr)*8);
	memset(QuickItemAttr	,0,sizeof(ItemAttr)*7);
	memset(BankItemAttr		,0,sizeof(ItemAttr)*5*3*6);
	memset(&HandItemAttr	,0,sizeof(ItemAttr));
}
