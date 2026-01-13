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
#include "object.h"
#include "map.h"
#include "Hong_Sub.h"
#include "convert565to555.h"
#include "Hong_Sprite.h"
#include "Hong_Light.h"
#include "Tool.h"
#include "Item.h"
#include "SDLRenderer.h"


///////////////////////////////////////////////////////////////////////////////
//

MAPOBJECTIMAGE      MoImage[MAX_MAPOBJECTIMAGE];
WORD				TotalMapObjectID;
WORD				TotalMapObject;

MAPOBJECT			Mo[MAX_MAPOBJECT_];


MAPOBJECTIMAGE		TempMoImage;

//////////// SoundUp lkh �߰� ////////////
extern void		ObjectSoundPlay(void);

///////////////////////////////////////////////////////////////////////////////
//

int OldSoundNo, OldSoundFrame, OldSoundDealy;

extern void changemapobjectname(char* oldname, char* newname);  // map.cpp
extern void changesourcename(char* oldname, char* newname);  // map.cpp

int LoadMapObjectCSP(int id, LPMAPOBJECTIMAGE mi)
{
    SDL_RWops* fp = nullptr;
    char filename[FILENAME_MAX];
    char tempmapobject[FILENAME_MAX];
    // short ox, oy; // 原始代碼宣告了但沒用到，可以註解掉或保留
    int i;

    if (mi->spr[0].img != NULL) return 0;

    // 保留原始邏輯 (雖然在這裡看起來 tempmapobject 沒被後續使用)
    changemapobjectname(MapName, tempmapobject);

    int lev = id / 1000;
    if (lev == 1) lev = 0;

    for (i = 0; i < ITEM_FRAME_MAX_; i++)
    {
        // 1. 組合檔名
        if (i == 0)
            sprintf(filename, "./object/%d/%04d.CSP", lev, id);
        else
            sprintf(filename, "./object/%d/%04d%02d.CSP", lev, id, i);

        // [Android] 2. 路徑處理：移除開頭的 ./
        std::string path = filename;
        if (path.find("./") == 0) path = path.substr(2);

        // [Android] 3. 開啟檔案
        fp = SDL_RWFromFile(path.c_str(), "rb");
        if (fp == NULL) {
            // 如果讀不到第 0 張圖，或是讀到一半中斷，就回傳目前的張數
            // 建議加個 Log 方便除錯
            // SDL_Log("LoadMapObjectCSP: Stop at %d (File not found: %s)", i, path.c_str());
            return i;
        }

        // [Android] 4. 逐一讀取 Header (這很安全，不會有對齊問題)
        SDL_RWread(fp, &mi->spr[i].xl, sizeof(short), 1);
        SDL_RWread(fp, &mi->spr[i].yl, sizeof(short), 1);

        SDL_RWread(fp, &mi->spr[i].ox, sizeof(short), 1);
        SDL_RWread(fp, &mi->spr[i].oy, sizeof(short), 1);

        SDL_RWread(fp, &mi->spr[i].size, sizeof(unsigned int), 1);

        // 5. 分配記憶體與讀取圖片數據
        mi->spr[i].img = NULL;
        MemAlloc(mi->spr[i].img, mi->spr[i].size);

        // 讀取 img 數據
        SDL_RWread(fp, mi->spr[i].img, 1, mi->spr[i].size); // 注意參數順序：每次讀1byte，讀 size 次

        // 6. 格式轉換
        convert565to555(&mi->spr[i]); // Todo finished

        // [Android] 7. 關閉檔案
        SDL_RWclose(fp);
    }
    return i;
}

void DisplayMapObject(LPMAPOBJECT mo)
{

	int mx = mo->x + mo->offx - Mapx;
	int my = mo->y + mo->offy - Mapy;
	int fspflag = 0;

	Spr* sp, * fsp;
	sp = &MoImage[mo->id - 1].spr[mo->curframe];


	if (MapNumber == 2)
	{
		if (4040 <= mo->id && mo->id <= 4043) { PutCompressedImageWaveTile(mx, my, sp); goto END_; }
		if (4045 == mo->id) { PutCompressedImageWaveTile(mx, my, sp); goto END_; }
	}
	fsp = &MoImage[mo->id - 1].spr[0];

	// 020305 YGI
	// ����Ʈ ȭ���� ���� �ʿ䰡 ����.
	if (!GetSysInfo(SI_NOT_CONNECT_MODE) && mo->objectoritem == ITEMTYPE_COLOSSUS_STONE)	goto END_;

	switch (mo->animationType)
	{
	case OBJECTANI_NO:
	case OBJECTANI_RETURNLOOP:
	case OBJECTANI_LOOP:
	case OBJECTANI_TOGGLE_ANI:	switch (mo->attr1 & 0x0003)
	{
	case OBJECTSPRITE_NORMAL:	PutCompressedImage(mx, my, sp);			break;
	case OBJECTSPRITE_TRANS:  PutCompressedImageFX(mx, my, sp, mo->attr1 >> 3, 1);	break;
	case OBJECTSPRITE_ALPHA:
		if (mo->light && DayLightControl >= 31)
		{
		}
		else
		{
			PutCompressedImageFX(mx, my, sp, mo->attr1 >> 3, 2);	break;
		}
	}
							 break;

	case OBJECTANI_1_RETURNLOOP:
	case OBJECTANI_1_LOOP:	PutCompressedImage(mx, my, fsp);
		fspflag = 1;

		// Light�� ������ �ִٸ�..

		if (mo->light && DayLightControl >= 31)
		{
		}
		else
		{
			switch (mo->attr1 & 0x0003)
			{
			case OBJECTSPRITE_NORMAL:	PutCompressedImage(mx, my, sp);						break;
			case OBJECTSPRITE_TRANS:  PutCompressedImageFX(mx, my, sp, mo->attr1 >> 3, 1);	break;
			case OBJECTSPRITE_ALPHA:  PutCompressedImageFX(mx, my, sp, mo->attr1 >> 3, 2);	break;
			}
		}
		break;
	}

#ifdef _DEBUG											
	if (mo->attr2 && GetSysInfo(SI_GAME_MAKE_MODE))
	{
		//		Box_( mx - fsp->ox, my - fsp->oy, fsp->xl, fsp->yl );
		Line(mo->dx0[0] + mo->x, mo->dy0[0] + mo->y, mo->dx1[1] + mo->x, mo->dy1[1] + mo->y, RGB16(0x00, 0xff, 0x00), false);
	}
#endif													


END_:

	int i, j;
	int tsx, tsy, tex, tey;

	int sx = mx - sp->ox + Mapx;
	int sy = my - sp->oy + Mapy;
	int ex = sx + sp->xl + TILE_SIZE - 1;
	int ey = sy + sp->yl + TILE_SIZE - 1;

	if (fspflag)
	{
		tsx = mx - fsp->ox + Mapx;
		tsy = my - fsp->oy + Mapy;
		tex = sx + fsp->xl + TILE_SIZE - 1;
		tey = sy + fsp->yl + TILE_SIZE - 1;

		if (tsx < sx) sx = tsx;	if (tsy < sy) sy = tsy;
		if (tex > ex) ex = tex;	if (tey > ey) ey = tey;
	}
	ex += ((ex - sx) / 2);

	sx /= TILE_SIZE;
	sy /= TILE_SIZE;
	ex /= TILE_SIZE;
	ey /= TILE_SIZE;

	my += Mapy;

	if (sx < 0) sx = 0;
	if (sy < 0) sy = 0;
	if (ex >= g_Map.file.wWidth) ex = g_Map.file.wWidth;
	if (ey >= g_Map.file.wHeight) ey = g_Map.file.wHeight;


	for (i = sx; i < ex; i++)
		for (j = sy; j < ey; j++)
		{
			int front = TileMap[i][j].front;
			if (front)
				if (TileMap[i][j].frontoffset * TILE_SIZE >= (DWORD)mo->y)
				{
					if (Sou[front].xl == -1)		PutTile(i * TILE_SIZE - Mapx, j * TILE_SIZE - Mapy, &Sou[front], Sou[front].img, g_DestBackBuf);
					else							PutCompressedImage(i * TILE_SIZE - Mapx, j * TILE_SIZE - Mapy, &Sou[front]);
				}
		}
}

void DisplayMapObjectAfter(LPMAPOBJECT mo, int selected)
{
	//DisplayMapObjectAfter_Adapted(mo, selected);
	//return;

	int mx = mo->x + mo->offx - Mapx;
	int my = mo->y + mo->offy - Mapy;

	Spr* sp, * fsp;
	fsp = &MoImage[mo->id - 1].spr[0];
	sp = &MoImage[mo->id - 1].spr[mo->curframe];

	if (selected)
	{
		PutCompressedImageBlendRGB(mx, my, sp, ReturnBlendRGB(0xff, 0, 0));
	}

	int x = mo->x;
	int y = mo->y;

	Line(x - 5, y, x + 5, y, RGB16(0xff, 0x1f, 0x1f), false);
	Line(x, y - 5, x, y + 5, RGB16(0xff, 0x1f, 0x1f), false);

	if (MapNumber == 2)	// firedun1....
	{
		if ((4040 <= mo->id && mo->id <= 4043) || mo->id == 4045) { return; }
	}

	switch (mo->animationType)
	{
	case OBJECTANI_NO:
	case OBJECTANI_RETURNLOOP:
	case OBJECTANI_LOOP:
	case OBJECTANI_TOGGLE_ANI:	switch (mo->attr1)
	{
	case OBJECTSPRITE_NORMAL:	break;
	case OBJECTSPRITE_TRANS:  break;
	case OBJECTSPRITE_ALPHA:  break;
	}
							 break;

	case OBJECTANI_1_RETURNLOOP:
	case OBJECTANI_1_LOOP:	/*PutCompressedImage( mx, my, fsp );*/
		switch (mo->attr1)
		{
		case OBJECTSPRITE_NORMAL:	/*PutCompressedImage( mx, my, sp );				*/break;
		case OBJECTSPRITE_TRANS:  /*PutCompressedImageFX( mx, my, sp, 20, 1 );	*/break;
		case OBJECTSPRITE_ALPHA:  /*PutCompressedImageFX( mx, my, sp, 20, 2 );	*/break;
		}
		break;
	}


	if (mo->attr2)
	{
		Line(mo->dx0[mo->curframe] + mo->x, mo->dy0[mo->curframe] + mo->y,
			mo->dx1[mo->curframe] + mo->x, mo->dy1[mo->curframe] + mo->y, RGB16(0x00, 0xff, 0x00), false);
	}
}




void CheckMapObject(LPMAPOBJECT mo)
{
	if (--mo->delay < 0) { mo->delay = 3; }
	else return;

	int totalframe = MoImage[mo->id - 1].nFrameCount;

	switch (mo->animationType)
	{
	case OBJECTANI_NO:	if (mo->curframe < 0 || mo->curframe >= totalframe)
	{
		mo->curframe = 0;
	}
					 break;

	case OBJECTANI_RETURNLOOP:	if (mo->anidir)
	{
		mo->curframe++;
		if (mo->curframe >= totalframe) { mo->anidir = !mo->anidir; mo->curframe = totalframe - 1; }
	}
							 else
	{
		mo->curframe--;
		if (mo->curframe < 0) { mo->anidir = !mo->anidir; mo->curframe = 0; }
	}
							 break;

	case OBJECTANI_LOOP:	mo->curframe++;
		if (mo->curframe >= totalframe) { mo->curframe = 0; }
		break;

	case OBJECTANI_TOGGLE_ANI:	mo->curframe = 0;
		break;

	case OBJECTANI_1_RETURNLOOP:	if (mo->anidir)
	{
		mo->curframe++;
		if (mo->curframe >= totalframe) { mo->anidir = !mo->anidir; mo->curframe = totalframe - 1; }
	}
							   else
	{
		mo->curframe--;
		if (mo->curframe < 1) { mo->anidir = !mo->anidir; mo->curframe = 1; }
	}
							   break;
	case OBJECTANI_1_LOOP:	mo->curframe++;
		if (mo->curframe >= totalframe) { mo->curframe = 1; }
		break;
	}
}





/////////////////////////////////////////////////////////////////////////////////////////////

int TempMoImageID[2000];


#include "menuset.h"		// 021030 YGI

////////////////////// SoundUp lkh ���� ///////////////////////	
// newtoi == true  �̸�   ����κ��̵����� ���Ѵ�. 
// 輔助函式：處理路徑 (如果之前有定義過，可以共用)
static std::string FixPath(const char* path) {
    std::string s = path;
    if (s.find("./") == 0) s = s.substr(2);
    return s;
}

int LoadTOI(char* filename)
{
    SDL_RWops* fp = nullptr;
    char szFilePath[FILENAME_MAX]={0};
    char temptoiname[FILENAME_MAX]={0};
    // WORD i, j; // 建議改用 int 或 size_t
    int i, j;
    int  no;
    int  count = 0;
    int  ox, oy;
    short id;
    MAPOBJECT_old moold;
    MAPOBJECT mo;
    bool newtoi;

    // 轉大寫
    CharToUpper(filename);

    // 處理檔名對應
    changemapobjectname(filename, temptoiname);

    // =========================================================================
    // 1. 嘗試開啟 .toi2 檔案
    // =========================================================================

    // 優先檢查活動物件 (Christmas 等)
    if (g_Menu_Variable.m_pEventObjcet)
    {
        //std::string path;
        if (true == g_bIsChristmas)
        {
            sprintf(szFilePath, "./object/%s/CHRISTMAS_%s", temptoiname, g_Menu_Variable.m_pEventObjcet->object_toi);
            fp = SDL_RWFromFile(FixPath(szFilePath).c_str(), "rb");
            if (!fp)
            {
                sprintf(szFilePath, "./object/%s/%s", temptoiname, g_Menu_Variable.m_pEventObjcet->object_toi);
                fp = SDL_RWFromFile(FixPath(szFilePath).c_str(), "rb");
            }
        }
        else
        {
            sprintf(szFilePath, "./object/%s/%s", temptoiname, g_Menu_Variable.m_pEventObjcet->object_toi);
            fp = SDL_RWFromFile(FixPath(szFilePath).c_str(), "rb");
        }
    }

    // 如果上面沒開到，嘗試開啟標準 .toi2
    if (!fp)
    {
        if (true == g_bIsChristmas)
        {
            sprintf(szFilePath, "object/%s/CHRISTMAS_%s.toi2", temptoiname, filename);
            fp = SDL_RWFromFile(FixPath(szFilePath).c_str(), "rb");
            if (!fp)
            {
                sprintf(szFilePath, "object/%s/%s.toi2", temptoiname, filename);
                fp = SDL_RWFromFile(FixPath(szFilePath).c_str(), "rb");
            }
        }
        else
        {
            sprintf(szFilePath, "object/%s/%s.toi2", temptoiname, filename);
            fp = SDL_RWFromFile(FixPath(szFilePath).c_str(), "rb");
        }
    }

    // =========================================================================
    // 2. 讀取 .toi2 邏輯
    // =========================================================================
    if (fp)
    {
        // 讀取 TotalMapObject (2 bytes)
        SDL_RWread(fp, &TotalMapObject, 2, 1);

        if (TotalMapObject == 0xffff)
        {
            newtoi = true;
            SDL_RWread(fp, &TotalMapObject, 2, 1);
        }
        else
            newtoi = false;

        if (TotalMapObject > MAX_MAPOBJECT_)
        {
            // Error 函式需要確保在 Android 上能顯示 Log 或彈窗
            SDL_Log("Error: Too Many MapObject (%d)", TotalMapObject);
            SDL_RWclose(fp); // 安全退出
            return 0;
        }

        SDL_RWread(fp, &TotalMapObjectID, 2, 1);

        // 跳過 ID 列表
        // fseek -> SDL_RWseek
        SDL_RWseek(fp, TotalMapObject * sizeof(int), RW_SEEK_CUR);

        for (i = 0; i < TotalMapObject; i++)
        {
            // if (count == 140) { _asm nop; } // 移除組合語言

            if (newtoi == true)
            {
                // ★★★ 危險：請確保 MAPOBJECT 有 #pragma pack(1) ★★★
                SDL_RWread(fp, &mo, sizeof(MAPOBJECT), 1);
            }
            else
            {
                // ★★★ 危險：請確保 MAPOBJECT_old 有 #pragma pack(1) ★★★
                SDL_RWread(fp, &moold, sizeof(MAPOBJECT_old), 1);
                memcpy(&mo, &moold, sizeof(MAPOBJECT_old));

                // 手動初始化新欄位
                mo.soundno = 0;
                mo.soundframe = 0;
                mo.sounddelay = 0;
            }

            // 過濾不需要的物件類型
            if (mo.objectoritem == 0 || SysInfo.loaditemfromtoi2 ||
                mo.objectoritem == ITEMTYPE_WORKTABLE_MINING ||
                mo.objectoritem == ITEMTYPE_WORKTABLE_BATTLE ||
                mo.objectoritem == ITEMTYPE_WORKTABLE_SUNBAN ||
                mo.objectoritem == ITEMTYPE_WORKTABLE_MORU ||
                mo.objectoritem == ITEMTYPE_WORKTABLE_HWADUK ||
                mo.objectoritem == ITEMTYPE_WORKTABLE_SIHUMKWAN ||
                mo.objectoritem == ITEMTYPE_WORKTABLE_MULAE ||
                mo.objectoritem == ITEMTYPE_WORKTABLE_BULDAE ||
                mo.objectoritem == ITEMTYPE_SIGNBOARD ||
                mo.objectoritem == ITEMTYPE_DIRECTBOARD ||
                mo.objectoritem == ITEMTYPE_HP_UP ||
                mo.objectoritem == ITEMTYPE_MP_UP ||
                mo.objectoritem == ITEMTYPE_SCALE ||
                mo.objectoritem == ITEMTYPE_NATION_STONE ||
                mo.objectoritem == ITEMTYPE_GUILD_STONE ||
                mo.objectoritem == ITEMTYPE_SENARIO_STONE ||
                mo.objectoritem == ITEMTYPE_COLOSSUS_STONE)
            {
                memcpy(&Mo[count], &mo, sizeof(MAPOBJECT));

                // 載入 CSP (使用上一輪轉換過的函式)
                int frame = LoadMapObjectCSP(Mo[count].id, &MoImage[Mo[count].id - 1]);

                if (frame) MoImage[Mo[count].id - 1].nFrameCount = frame;

                if (MoImage[Mo[count].id - 1].nFrameCount != 0)
                {
                    Mo[count].curframe = Random(MoImage[Mo[count].id - 1].nFrameCount);
                }
                else
                {
                    Mo[count].curframe = 0;
                }

                count++;
            }

            // 網路/系統訊息處理
            // 注意：HandleRunning 內部如果是 Win32 消息循環，需要另外處理
            if (HandleRunning(&connections) <= 0)
            {
                Release(&connections);
                SDL_RWclose(fp);
                return(0);
            }
        }
        SDL_RWclose(fp);

        TotalMapObject = count;

        // 建立 Atlas
        for (int k = 0; k < MAX_MAPOBJECTIMAGE; k++)
        {
            // 修正 std::string 的使用方式
            std::string indexname = szFilePath; // szFilePath 是最後一次成功開啟的路徑
            indexname += std::to_string(k);

            g_SDLInfo.CreateAtlasForMap_CPP(indexname.c_str(), MoImage[k].spr, ITEM_FRAME_MAX_);
        }
        return 1;
    }

        // =========================================================================
        // 3. 讀取 .toi 邏輯 (舊格式 Fallback)
        // =========================================================================
    else
    {
        // 嘗試開啟舊版 .toi
        sprintf(szFilePath, "object/%s/%s.toi", temptoiname, filename);
        fp = SDL_RWFromFile(szFilePath, "rb");

        if (fp == NULL)
        {
            TotalMapObject = 0;
            return 0; // 真的都找不到，放棄
        }

        // 原代碼邏輯修正：如果找到了 .toi，繼續往下執行讀取
    }

    // 讀取舊版 Header
    SDL_RWread(fp, &TotalMapObjectID, 2, 1);

    // 跳過舊版 Header 的大量數據
    // 注意：这里的 sizeof 指標在 Android 64bit 是 8 bytes，Windows 是 4 bytes
    // 這會導致 fseek 距離錯誤！必須手動計算 Windows 上的大小
    // 原代碼: sizeof(SIZE)(8) + sizeof(WORD)(2) + sizeof(POINT)(8) + 4 + sizeof(LPOBJECT)*2 + ...
    // 建議直接寫死數值，或者使用固定的 int offset

    // Windows 32-bit 指標大小為 4
    const int PTR_SIZE_WIN32 = 4;
    // 假設 SIZE=8, WORD=2, POINT=8, 4 bytes padding?
    // 原代碼寫死：fseek(fp, 4 + 4 + 2, SEEK_CUR); (這看起來更像是一個簡單的跳過)
    // 但下面有一行被註解的長 fseek，我們依照您提供的啟用代碼：
    SDL_RWseek(fp, 4 + 4 + 2, RW_SEEK_CUR);

    for (i = 0; i < TotalMapObjectID; i++)
    {
        SDL_RWread(fp, &no, 4, 1);

        // 另一個巨大的 fseek，同樣涉及指標大小
        // sizeof(SIZE) + sizeof(WORD) + sizeof(POINT) + 4 + sizeof(LPOBJECT)*2 + sizeof(struct tagOBJECTHEADER*)*2 + 2
        // 我們必須模擬 Windows 32bit 的大小：
        // SIZE(8) + WORD(2) + POINT(8) + 4 + (4*2) + (4*2) + 2 = 8+2+8+4+8+8+2 = 40 bytes
        // 請根據您的結構體定義確認這些大小

        // 暫時依照您的代碼邏輯轉換，但這行非常危險：
        long seek_offset = sizeof(SIZE) + sizeof(WORD) + sizeof(POINT) + 4 + 8 + 8 + 2;
        SDL_RWseek(fp, 36, RW_SEEK_CUR);

        TempMoImageID[i] = no;

        for (j = 0; j < no; j++)
        {
            SDL_RWread(fp, &id, 2, 1);

            Mo[count].id = id;

            SDL_RWread(fp, &Mo[count].objectoritem, 2, 1);
            SDL_RWread(fp, &Mo[count].dum, 2, 1);
            SDL_RWread(fp, &Mo[count].animationType, 1, 1);
            SDL_RWread(fp, &Mo[count].attr, 1, 1);
            SDL_RWread(fp, &Mo[count].attr1, 1, 1);
            SDL_RWread(fp, &Mo[count].attr2, 1, 1);

            SDL_RWseek(fp, 2, RW_SEEK_CUR);
            SDL_RWread(fp, &Mo[count].dir, 2, 1);
            SDL_RWseek(fp, 2, RW_SEEK_CUR);

            SDL_RWread(fp, &Mo[count].x, 4, 1);
            SDL_RWread(fp, &Mo[count].y, 4, 1);
            SDL_RWread(fp, &Mo[count].offx, 4, 1);
            SDL_RWread(fp, &Mo[count].offy, 4, 1);
            SDL_RWread(fp, &ox, 4, 1);
            SDL_RWread(fp, &oy, 4, 1);

            SDL_RWseek(fp, 8, RW_SEEK_CUR);

            Mo[count].x = Mo[count].x * TILE_SIZE;
            Mo[count].y = Mo[count].y * TILE_SIZE;

            Mo[count].objectoritem = 0;

            if (j == 0)
            {
                MoImage[Mo[count].id - 1].nFrameCount = LoadMapObjectCSP(Mo[count].id, &MoImage[Mo[count].id - 1]);
            }

            Mo[count].curframe = Random(MoImage[Mo[count].id - 1].nFrameCount);

            count++;
        }
    }

    TotalMapObject = count;
    SDL_RWclose(fp);
    return 1;
}





int SaveTOI2(char* filename)
{
	/*FILE* fp, * wfp;
	char szFilePath[FILENAME_MAX];
	char temptoiname[FILENAME_MAX];
	WORD i, j, k;
	int  no = 0;
	int  count = 0;
	int  obcount = 0;
	int  code = 0xffff;

	typedef struct tt
	{
		int id;
		int animationType;
		int objectitem;
		int attr2;
		int light;
		int nFrameCount;
		int dx0[10], dy0[10], dx1[10], dy1[10];
	}t_object;

	MAPOBJECT** ob;

	changemapobjectname(filename, temptoiname);
	sprintf(szFilePath, "./object/%s/%s.toi2", temptoiname, filename);


	fp = Fopen(szFilePath, "rb");
	if (fp)
	{
		fclose(fp);
		DWORD fileattr = GetFileAttributes(szFilePath);
		if (fileattr & FILE_ATTRIBUTE_READONLY)
		{
			JustMsg("'%s'�� '�б�����'���� �Ǿ� �ֽ��ϴ�. \n �������� �ʾҽ��ϴ�.", szFilePath);
			return 0;
		}
	}

	fp = Fopen(szFilePath, "wb");
	if (fp == NULL) return 0;

	fwrite(&code, 2, 1, fp);
	fwrite(&TotalMapObject, 2, 1, fp);
	fwrite(&TotalMapObjectID, 2, 1, fp);
	fwrite(TempMoImageID, TotalMapObject, sizeof(int), fp);

	for (i = 0; i < TotalMapObject; i++)
	{
		////////////// 0810 lkh �߰� //////////////
		Mo[i].soundno = Mo[i].soundno % 1000;		//���� 3�ڸ��� ���� ���� ��ȣ�̰� ���� 3�ڸ��� ���� ī��Ʈ�̱� ����
		fwrite(&Mo[i], sizeof(MAPOBJECT), 1, fp);
	}
	fclose(fp);

	sprintf(szFilePath, "./object/%s/%s_toi2.txt", temptoiname, filename);
	fp = Fopen(szFilePath, "wt");
	if (fp == NULL)
	{
		fp = Fopen(szFilePath, "rb");
		if (fp)
		{
			fclose(fp);
			DWORD fileattr = GetFileAttributes(szFilePath);
			if (fileattr & FILE_ATTRIBUTE_READONLY)
			{
				JustMsg("'%s'�� '�б�����'���� �Ǿ� �ֽ��ϴ�. \n �������� �ʾҽ��ϴ�.", szFilePath);
				return 0;
			}
		}
		return 0;
	}

	fprintf(fp, "# 0:NO Animation  1:�պ�  2:�ݺ�  3:������   4:����������   5:������   6:����������\n");
	fprintf(fp, "# �������ִ� AnimationType Type   Dir  ��¹�� ��¼��� �׸��� Light  Lightxy            DontBox            �ѱ׸���		\n");

	ob = NULL;
	MemAlloc(ob, sizeof(MAPOBJECT*) * TotalMapObject);
	for (i = 0; i < TotalMapObject; i++)
		MemAlloc(ob[i], sizeof(MAPOBJECT));

	obcount = 0;

	for (j = 0; j < TotalMapObject; j++)
	{
		if (Mo[j].objectoritem) // item�̸�...
		{
			for (i = 0; i < obcount; i++)
			{
				if (ob[i]->id == Mo[j].id) goto CONTINUE_;
			}

			memcpy(ob[obcount], &Mo[j], sizeof(MAPOBJECT));

			fprintf(fp, "%d   0   %d   %d  1   %d 0  1  %d 0  -30   -20 0 0 -20 20 0 0 20     %d  ",
				Mo[j].id + 13000,
				Mo[j].animationType,
				Mo[j].objectoritem,
				Mo[j].attr2,
				Mo[j].light,
				MoImage[Mo[j].id - 1].nFrameCount);

			for (k = 0; k < MoImage[Mo[j].id - 1].nFrameCount; k++)
				fprintf(fp, "%d %d %d %d   ", Mo[j].dx0[k], Mo[j].dy0[k], Mo[j].dx1[k], Mo[j].dy1[k]);
			fprintf(fp, "\n");

			obcount++;
		}
	CONTINUE_:;
	}

	fclose(fp);



	// Binary Data Save.....
	sprintf(szFilePath, "./object/%s/%s_toi2.c", temptoiname, filename);
	fp = Fopen(szFilePath, "wb");
	if (fp == NULL) return 0;

	//		fprintf( fp, "# 0:NO Animation  1:�պ�  2:�ݺ�  3:������   4:����������   5:������   6:����������\n" );
	//		fprintf( fp, "# �������ִ� AnimationType Type   Dir  ��¹�� ��¼��� �׸��� Light  Lightxy            DontBox            �ѱ׸���		\n" );

	int t;
	fwrite(&obcount, sizeof(int), 1, fp);
	for (i = 0; i < obcount; i++)
	{
		t = ob[i]->id + 13000;						fwrite(&t, sizeof(int), 1, fp);
		t = 0;										fwrite(&t, sizeof(int), 1, fp);
		t = ob[i]->animationType;					fwrite(&t, sizeof(int), 1, fp);
		t = ob[i]->objectoritem;					fwrite(&t, sizeof(int), 1, fp);
		t = 1; 									fwrite(&t, sizeof(int), 1, fp);
		t = ob[i]->attr2; 							fwrite(&t, sizeof(int), 1, fp);
		t = 0; 									fwrite(&t, sizeof(int), 1, fp);
		t = 1;  									fwrite(&t, sizeof(int), 1, fp);
		t = ob[i]->light;							fwrite(&t, sizeof(int), 1, fp);
		t = 0;  									fwrite(&t, sizeof(int), 1, fp);
		t = -30;									fwrite(&t, sizeof(int), 1, fp);
		t = -20;									fwrite(&t, sizeof(int), 1, fp);
		t = 0; 										fwrite(&t, sizeof(int), 1, fp);
		t = 0; 										fwrite(&t, sizeof(int), 1, fp);
		t = -20;									fwrite(&t, sizeof(int), 1, fp);
		t = 20;									fwrite(&t, sizeof(int), 1, fp);
		t = 0; 										fwrite(&t, sizeof(int), 1, fp);
		t = 0; 										fwrite(&t, sizeof(int), 1, fp);
		t = 20;;									fwrite(&t, sizeof(int), 1, fp);
		t = MoImage[ob[i]->id - 1].nFrameCount;		fwrite(&t, sizeof(int), 1, fp); // 20�� * sizeof( int ) = 80;

		fwrite(&ob[i]->dx0, sizeof(short), ITEM_FRAME_MAX_, fp);
		fwrite(&ob[i]->dy0, sizeof(short), ITEM_FRAME_MAX_, fp);
		fwrite(&ob[i]->dx1, sizeof(short), ITEM_FRAME_MAX_, fp);
		fwrite(&ob[i]->dy1, sizeof(short), ITEM_FRAME_MAX_, fp);	// sizeof( int ) * 40  = 160///
	}
	fclose(fp);

	for (i = 0; i < obcount; i++)
		MemFree(ob[i]);
	MemFree(ob);

	// üũ���� ����ִ´�. 
	sprintf(szFilePath, "./object/%s/%s_toi2.c", temptoiname, filename);
	fp = Fopen(szFilePath, "rb");
	sprintf(szFilePath, "./object/%s/%s_toi2.b", temptoiname, filename);
	wfp = Fopen(szFilePath, "wb");

	DWORD fl = filelength(fileno(fp));

	char crc = 0, tcrc;
	for (k = 0; k < fl; k++)
	{
		fread(&tcrc, 1, 1, fp);
		crc += tcrc;
		fwrite(&tcrc, 1, 1, wfp);
	}

	fwrite(&crc, 1, 1, wfp);

	fclose(fp);
	fclose(wfp);*/

	return 1;
}
//////////////////////////////// ������� ////////////////////////////////


void FreeTOI(void)
{
	for (int i = 0; i < MAX_MAPOBJECTIMAGE; i++)
	{
		for (int j = 0; j < ITEM_FRAME_MAX_; j++)
		{
			MemFree(MoImage[i].spr[j].img);
		}
	}
}


///////////////////////// SoundUp lkh ���� /////////////////////////		
void InsertMapObject(int moid, int x, int y)
{
	int i;

	if (TotalMapObject > MAX_MAPOBJECT_)
	{
		JustMsg("��,, ���� MapObject������ 5000�Ѿ����.. ���̻��� ��������..");
		return;
	}

	for (i = 0; i < TotalMapObject; i++)
	{
		if (Mo[i].id == moid) break;
	}

	if (i == TotalMapObject)
	{
		memset(&Mo[TotalMapObject], 0, sizeof(MAPOBJECT));
	}
	else
	{
		memcpy(&Mo[TotalMapObject], &Mo[i], sizeof(MAPOBJECT));
	}

	Mo[TotalMapObject].id = moid;
	Mo[TotalMapObject].x = x;
	Mo[TotalMapObject].y = y;

	Mo[TotalMapObject].animationType = OBJECTANI_NO;

	LPMAPOBJECT mo = &Mo[TotalMapObject];


	int frame = LoadMapObjectCSP(mo->id, &MoImage[mo->id - 1]);
	if (frame) MoImage[mo->id - 1].nFrameCount = frame;

	Mo[TotalMapObject].soundno = 0;
	Mo[TotalMapObject].soundframe = 0;
	Mo[TotalMapObject].sounddelay = 0;
	TotalMapObject++;
}

void DeleteMapObject_sub(int no)
{
	MAPOBJECT mo;

	memcpy(&mo, &Mo[no], sizeof(MAPOBJECT));
	memmove(&Mo[no], &Mo[no + 1], sizeof(MAPOBJECT) * (MAX_MAPOBJECT_ - no - 1));
	TotalMapObject--;

	int c = 0;
	int i;
	for (i = 0; i < TotalMapObject; i++)
	{
		if (Mo[i].id == mo.id) c++;
	}
	if (c == 0)
	{
		for (i = 0; i < ITEM_FRAME_MAX_; i++)
			MemFree(MoImage[mo.id - 1].spr[i].img);
	}
}

void DeleteMapObject(int no)
{
return;

	/*if (no == -1) return;

	if (YesOrNo("�ʿ�����Ʈ�� ���ַ��� ?", "Ȥ�ó�..") == IDYES)
	{
	}
	else
	{
		return;
	}*/

	DeleteMapObject_sub(no);
}



void DeleteMapObjectAll(int no)
{
	int j;
return;

	/*if (no == -1) return;

	if (YesOrNo("�ʿ��� no�� �ʿ�����Ʈ�� ��� ���ַ��� ????", "Ȥ�ó�..") == IDYES)
	{
	}
	else
	{
		return;
	}*/

	for (j = 0; j < TotalMapObject; j++)
	{
		if (Mo[j].id == no)
		{
			DeleteMapObject_sub(j);
			j--;
		}
	}
}
/*
void LoadTempSprite(int id)
{
	static int	mapobjectno = -1;
	if (id == mapobjectno) return;
	mapobjectno = id;

	char	filename[FILENAME_MAX];
	FILE* fp;
	int    lev;

	MemFree(TempMoImage.spr[0].img);

	lev = id / 1000;
	if (lev == 1) lev = 0;

	sprintf(filename, "./object/%d/%04d.csp", lev, id);
	fp = Fopen(filename, "rb");
	if (fp == NULL) return;
	fread(&TempMoImage.spr[0].xl, sizeof(short), 1, fp);
	fread(&TempMoImage.spr[0].yl, sizeof(short), 1, fp);
	fread(&TempMoImage.spr[0].ox, sizeof(short), 1, fp);
	fread(&TempMoImage.spr[0].oy, sizeof(short), 1, fp);
	fread(&TempMoImage.spr[0].size, sizeof(unsigned int), 1, fp);
	TempMoImage.spr[0].img = NULL;
	MemAlloc(TempMoImage.spr[0].img, TempMoImage.spr[0].size);
	fread(TempMoImage.spr[0].img, TempMoImage.spr[0].size, 1, fp);
	convert565to555(&TempMoImage.spr[0]);//Todo useless
	fclose(fp);
    //g_SDLInfo.CreateTextureFromSingleSpr_CPP(filename, &TempMoImage.spr[0]);
}
*/