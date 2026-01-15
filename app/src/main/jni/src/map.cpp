/*

*/

#include "stdafx.h"
#include <cstdio>


#include "map.h"
#include "Hong_Sprite.h"
#include "Hong_Sub.h"

#include "Char.h"
#include "gameproc.h"
#include "Convert565to555.h"
#include "Tool.h"
#include "Hangul.h"
#include "Hong_Light.h"
#include "Menu.h"
#include "SkillTool.h"
#include "MouseCursor.h"
#include "DirectSound.h"
#include "house.h"
#include "CharDataTable.h"
#include "SmallMenu.h"
#include <filesystem>
#include "LottoMgr.h"//soto-030501
#include "SDLRenderer.h"
#include "object.h"
#include "MapInfo.h"

MAP					g_Map;
TILE TileMap[VILLAGE_SIZE][VILLAGE_SIZE] = {0};
ROOF_ANIM HouseTileMap[VILLAGE_SIZE][VILLAGE_SIZE] = {0};
ROOFHEADER			g_RoofHeader;
EVENTLISTHEADER		g_EventListHeaderMap;
EVENTLISTHEADER		g_EventListHeaderRoom;
EVENTLISTHEADER		g_EventListHeaderMusic;

Spr Sou[TILE_MAX_];
BYTE* SouBuf;


///////////////////// 0430 lkh �߰� ///////////////////////////
Spr		SouHouse[HOUSETILE_MAX_];
BYTE* SouHouseBuf;
///////////////////////////////////////////////////////////////


char MapName[NM_LENGTH];

int LimitMapSx;
int LimitMapSy;
int LimitMapEx;
int LimitMapEy;

int OldEventMx, OldEventMy, EventMx, EventMy;


int StartMapx, StartMapy, StartMapFlag;

int  MapNumber;

///////////// SoundUp lkh �߰� /////////////
BOOL	g_BGE_Flag = 0;

int  MapNumberX, MapNumberY;
int  MapNumberFloor;
char MapNumberName[20];

int  TotalTileNo;



//////////////////////// 0430 lkh �߰� ///////////////////////////
int	TotalHouseTileNo;


int SmallMapX = 100, SmallMapY = 100;






///////////////////////////////////////////////////////////////////////////////
//		
bool	MapBuild_Refactored(LPMAP lpMap, char* mapname);
BOOL	MapBuild(LPMAP lpMap, char* filename);
void	MapDestroy(LPMAP lpMap);
void	MapCleanupSurface(LPMAP lpMap);
BOOL	MapRestoreSurface(LPMAP lpMap);
BOOL	MapReadFile(LPMAPFILE lpMapFile, char* pszFileName);
bool	MapReadFile_Refactored(LPMAPFILE lpMapFile, const char* pszFileName);
BOOL	MapWirteFile(LPMAPFILE lpMapFile, char* pszFileName);
void	MapDisplay(int nType);


bool		BuildRoofHeader(LPROOFHEADER lpRoofHeader, char* lpszFileName);
void		DestroyRoofHeader(LPROOFHEADER lpRoofHeader);
LPROOFGROUP	AddRoofGroup(LPROOFHEADER lpRoofHeader);
void		DeleteRoofGroup(LPROOFHEADER lpRoofHeader, LPROOFGROUP lpRoofGroup);
LPROOF		AddRoof(LPROOFGROUP lpRoofGroup, WORD x, WORD y);
void		DeleteRoof(LPROOFGROUP lpRoofGroup, LPROOF lpRoof);
LPROOFGROUP	FindRoofGroup(LPROOFHEADER lpRoofHeader, WORD index);
LPROOF		FindRoof(LPROOFGROUP lpRoofGroup, WORD x, WORD y);
bool		ReadRoofData(LPROOFHEADER lpRoofHeader, char* lpszFileName);

BOOL		BuildEventList(char* lpszFileName);
void		DestroyEventList();
LPEVENTLIST	AddEventList(LPEVENTLISTHEADER lpEventListHeader, WORD x, WORD y, WORD wIndex);
void		DeleteEventList(LPEVENTLISTHEADER lpEventListHeader, LPEVENTLIST lpEventList);
LPEVENTLIST	FindEventList(LPEVENTLISTHEADER lpEventListHeader, WORD x, WORD y);
BOOL		ReadEventList(char* lpszFileName);

DWORD		ReturnSizeOfSource2(const char* source2name);
BOOL		IsScreenArea(LPCHARACTER ch);
void		SetView(LPCHARACTER ch, int range);
//void        UpdateCamera_CPP(LPCHARACTER player_char, int sight_range);
int			MoveScreen(DIRECTION direction);

#define SmallMapXL		125
#define SmallMapYL		77

extern NPC_INFO g_infNpc[MAX_CHARACTER_SPRITE_]; // CSD-030419
extern ITEM_INFO g_itemref[MAX_ITEM_MUTANT_KIND]; // CSD-030419
extern void CalcOrder(void);
///////////////////////////////////////////////////////////////////////////////
//

void LoadMapInfo(int lang)
{
    static bool m_bRead = false;
    if(m_bRead) return;
    m_bRead = true;
    SDL_RWops* fp = nullptr;
    std::string filename = "";

    // 1. 選擇檔案
    filename = (system_info.lang == 0) ? "./data/map_info.bin" : "./data/map_info_ch.bin";

    // [Android] 2. 路徑處理
    if (filename.find("./") == 0) filename = filename.substr(2);

    // [Android] 3. 開啟檔案
    fp = SDL_RWFromFile(filename.c_str(), "rb");
    if (!fp) {
        SDL_Log("LoadMapInfo Error: Not Found %s", filename.c_str());
        return;
    }

    // 4. 驗證大小 (雖然理論上是安全的，但檢查一下更放心)
    // 預期大小 = 121 * 地圖數量
    size_t structSize = sizeof(t_MapInfo);
    Sint64 fileSize = SDL_RWsize(fp);
    Sint64 expectedTotalSize = structSize * MAX_MAP_;

    // 如果 Log 出現 121，那就代表完全正確！
    SDL_Log("LoadMapInfo: StructSize=%zu, FileSize=%lld", structSize, fileSize);

    if (fileSize < expectedTotalSize) {
        SDL_Log("[Warning] MapInfo 檔案大小似乎比預期小，可能讀取不完整！");
    }

    // [Android] 5. 直接讀取 (Direct Read)
    // 因為結構體是對齊的且無指標，直接讀入陣列即可
    size_t itemsRead = SDL_RWread(fp, MapInfo, sizeof(t_MapInfo), 106);//MAX_MAP_

    SDL_Log("LoadMapInfo: 成功讀取 %zu 筆地圖資料", itemsRead);

    // [Android] 6. 關閉檔案
    SDL_RWclose(fp);
}

int GetIndexByPort(int port)
{
	for (int i = 0; i < MAX_MAP_; i++)
	{
		if (MapInfo[i].port == port)
		{
			return i;
		}
	}
	return -1;
}
int GetIndexByMapfile(char* map_file)
{
	EatRearWhiteChar(map_file);
	CharToUpper(map_file);
	for (int i = 0; i < MAX_MAP_; i++)
	{
		if (strcmp(MapInfo[i].mapfile, map_file) == 0)
		{
			return i;
		}
	}
	return -1;
}

char* GetMapNameByIndex(int i)
{
    if (i < 0 ||  i >= MAX_MAP_) return NULL;
	return MapInfo[i].mapname;
}

char* GetMapNameByPort(int port)
{
	return GetMapNameByIndex(GetIndexByPort(port));
}
char* GetMapNameByMapfile(char* map_file)
{
	return GetMapNameByIndex(GetIndexByMapfile(map_file));
}

// 010724 KHS
BOOL MapBuild(LPMAP lpMap, char* mapname)
{
	/*lpMap->wScreenWidth = 20;
	lpMap->wScreenHeight = 11;
	char szFileName[FILENAME_MAX];
	int  c;

	CharToUpper(mapname);
	for (c = 0; MapInfo[c].mapfile[0]; c++)
	{
		if (strcmp(mapname, MapInfo[c].mapfile) == 0)
		{	//< CSD-030509
			strcpy(MapNumberName, MapInfo[c].mapname);
			MapNumber = c;
			break;
		}	//> CSD-030509
	}
	if (!MapInfo[c].mapname[0]) Error(lan->OutputMessage(3, 133), mapname);//010216 lsw


	InitWeather();

	sprintf(szFileName, "%s/map/%s.map", GetCurrentWorkingDirectory(), MapName);
	if (!MapReadFile(&lpMap->file, szFileName))
	{
		Error("[%s] LoadError!", szFileName);
		return	FALSE;
	}

	for (int i = 0; i < g_Map.file.wHeight; i++)
		for (int j = 0; j < g_Map.file.wWidth; j++)
		{
			TileMap[j][i].show_roof = 0;
			TileMap[j][i].occupied = 0;
		}


	LoadSkillMapTable();*/

	return	TRUE;
}

/**
 * @brief 【重構版】載入一張新地圖的核心函式。
 * 此版本修復了 strcpy 和 sprintf 帶來的緩衝區溢位安全隱患，
 * 並呼叫了經過效能優化的檔案讀取函式。
 * @param lpMap 指向全域地圖物件的指標。
 * @param mapname 要載入的地圖檔案名稱 (不含副檔名)。
 * @return 成功返回 TRUE，失敗返回 FALSE。
 */
bool MapBuild_Refactored(LPMAP lpMap, char* mapname)
{
	lpMap->wScreenWidth = 20;
	lpMap->wScreenHeight = 11;
	char szFileName[FILENAME_MAX];
	int c;

	CharToUpper(mapname);
	bool map_found = false;
	for (c = 0; MapInfo[c].mapfile[0]; c++)
	{
		if (stricmp(mapname, MapInfo[c].mapfile) == 0)
		{
			// 【安全修正 1】使用 strncpy 來取代 strcpy
			strncpy(MapNumberName, MapInfo[c].mapname, sizeof(MapNumberName) - 1);
			MapNumberName[sizeof(MapNumberName) - 1] = '\0'; // 確保字串總是正常結束

			// 我們也應該安全地複製 MapName
			strncpy(MapName, MapInfo[c].mapfile, sizeof(MapName) - 1);
			MapName[sizeof(MapName) - 1] = '\0';

			MapNumber = c;
			map_found = true;
			break;
		}
	}
	if (!map_found) {
		Error(lan->OutputMessage(3, 133), mapname);
		return false;
	}

	InitWeather();

	// 【安全修正 2】使用 snprintf (或 _snprintf) 來取代 sprintf
	snprintf(szFileName, sizeof(szFileName), "map/%s.map", MapName);

	// 【呼叫優化版】呼叫我們新的、經過效能優化的檔案讀取函式
	if (!MapReadFile_Refactored(&lpMap->file, szFileName))
	{
		// MapReadFile_Refactored 內部已經處理了錯誤訊息
		return false;
	}

	// 這段初始化迴圈的邏輯保持不變
	for (int i = 0; i < g_Map.file.wHeight; i++) {
		for (int j = 0; j < g_Map.file.wWidth; j++) {
			TileMap[j][i].show_roof = 0;
			TileMap[j][i].occupied = 0;
            HouseTileMap[j][i].alpha = 255;  // 初始化屋頂透明度
		}
	}

	// 【呼叫優化版】呼叫我們新的、安全讀取技能表的函式
	LoadSkillMapTable_Refactored();

	return true;
}




void MapDestroy(LPMAP lpMap)
{
	MapCleanupSurface(lpMap);
}


void MapCleanupSurface(LPMAP lpMap)
{
	/*if (lpMap->SurfaceInfo.lpSurface != NULL)
	{
		CleanupSurface(lpMap->SurfaceInfo.lpSurface);
		lpMap->SurfaceInfo.lpSurface = NULL;
	}*/
}

BOOL MapRestoreSurface(LPMAP lpMap)
{
	//	char	szFileName[ _MAX_PATH ];

	//	sprintf( szFileName, "%s/art/back%04d.bmp", __szCurrentDirectory, __GameInfo.nStageMajor );
	//	if ( !ReLoadMapImage( &lpMap->lpSurfaceBack, szFileName ) )
	//	{
	//		return	FALSE;
	//	}

	//	sprintf( szFileName, "%s/art/tile%04d.bmp", __szCurrentDirectory, __GameInfo.nStageMajor );
	//	if ( !ReLoadMapImage( &lpMap->lpSurfaceTile, szFileName ) )
	//	{
	//		return	FALSE;
	//	}

	return	TRUE;
}


/**
 * @brief 【重構版】從 .map 檔案讀取地圖資料。
 * 此版本將多次 I/O 呼叫合併為一次，極大提升載入效能。
 * @param lpMapFile 指向 MAPFILE 結構的指標，用於儲存地圖資訊。
 * @param pszFileName 要讀取的地圖檔案路徑。
 * @return 成功返回 TRUE，失敗返回 FALSE。
 */
bool MapReadFile_Refactored(LPMAPFILE lpMapFile, const char* pszFileName)
{
    // [Android] 1. 路徑處理
    std::string path = pszFileName;
    if (path.find("./") == 0) path = path.substr(2);

    // [Android] 2. 使用 SDL_RWops 開啟檔案
    SDL_RWops* file = SDL_RWFromFile(path.c_str(), "rb");
    if (!file) {
        // Error 函式在 Android 上可能需要改用 SDL_Log
        SDL_Log("[%s] LoadError![%s]", path.c_str(), SDL_GetError());
        // Error("[%s] LoadError![%s]", pszFileName, SDL_GetError());
        return false;
    }

    unsigned int ver; // DWORD -> unsigned int
    SDL_RWread(file, &ver, sizeof(ver), 1);

    if (ver != 0xffff) {
        // fseek(0, beg) -> SDL_RWseek
        SDL_RWseek(file, 0, RW_SEEK_SET);
    }

    SDL_RWread(file, &lpMapFile->wType, sizeof(lpMapFile->wType), 1);
    SDL_RWread(file, &lpMapFile->wWidth, sizeof(lpMapFile->wWidth), 1);
    SDL_RWread(file, &lpMapFile->wHeight, sizeof(lpMapFile->wHeight), 1);

    // 【安全檢查】
    if (lpMapFile->wWidth > 720 || lpMapFile->wHeight > 720) {
        SDL_Log("[%s] Map dimensions (%d x %d) exceed TileMap buffer size!", path.c_str(), lpMapFile->wWidth, lpMapFile->wHeight);
        SDL_RWclose(file);
        return false;
    }

    int t = 0;
    int t1 = 0;

    // --- 【最高效能改造】 ---
    // 1. 計算總圖塊數量
    size_t total_tiles = static_cast<size_t>(lpMapFile->wWidth) * lpMapFile->wHeight;

    // 2. 記憶體分配 (保持原樣，這是很好的 C++ 寫法)
    auto temp_buffer = std::make_unique<TILE[]>(total_tiles);

    // 3. 【關鍵 I/O 優化】一次性讀取
    // 參數: (context, ptr, object_size, object_count)
    size_t objectsRead = SDL_RWread(file, temp_buffer.get(), sizeof(TILE), total_tiles);

    if (objectsRead != total_tiles) {
        SDL_Log("[%s] Read Tile Data Error! Expected: %zu, Read: %zu", path.c_str(), total_tiles, objectsRead);
        SDL_RWclose(file);
        return false;
    }

    // 4. 【關鍵邏輯保留】資料轉置與 Checksum 計算
    // 這裡直接操作記憶體，非常高效
    for (int y = 0; y < lpMapFile->wHeight; y++) {
        for (int x = 0; x < lpMapFile->wWidth; x++) {
            TileMap[x][y] = temp_buffer[y * lpMapFile->wWidth + x];

            // 注意：這裡強制轉型取前 4 bytes 當作 int 加總，這是原版 Checksum 邏輯
            t += *(int*)&TileMap[x][y];
        }
    }

    if (ver == 0xffff)
    {
        SDL_RWread(file, &t1, sizeof(t1), 1);

        CheckSumError = t1 - t;
        if (CheckSumError != 0)
        {
            // lan->OutputMessage 在 Android 可能需要適配，這裡先用 Log
            SDL_Log("Map CheckSum Error! File: %s, Diff: %d", path.c_str(), CheckSumError);
            // Error(lan->OutputMessage(3, 134), pszFileName, CheckSumError);
            SDL_RWclose(file);
            return false;
        }
    }

    // 讀取結尾資訊
    SDL_RWread(file, &lpMapFile->cOdd, sizeof(lpMapFile->cOdd), 1);
    SDL_RWread(file, &lpMapFile->cEven, sizeof(lpMapFile->cEven), 1);

    // [Android] 記得關檔
    SDL_RWclose(file);

    return true;
}
/*
BOOL MapReadFile(LPMAPFILE lpMapFile, char* pszFileName)
{
	FILE* fp;
	int		t = 0, t1;
	double* cc = NULL;
	DWORD	ver;

	if (fp = Fopen(pszFileName, "rb"))
	{
		fread(&ver, sizeof(ver), 1, fp);
		if (ver == 0xffff)
		{
			fread(&lpMapFile->wType, sizeof(lpMapFile->wType), 1, fp);
			fread(&lpMapFile->wWidth, sizeof(lpMapFile->wWidth), 1, fp);
			fread(&lpMapFile->wHeight, sizeof(lpMapFile->wHeight), 1, fp);

			for (int y = 0; y < lpMapFile->wHeight; y++)
			{
				for (int x = 0; x < lpMapFile->wWidth; x++)
				{
					fread(&TileMap[x][y], sizeof(TILE), 1, fp);

#ifdef _DEBUG //Eleval

					cc = (double*)&TileMap[x][y];
					*cc -= (x + y);
					cc = ((double*)&TileMap[x][y]) + 1;
					*cc -= (y);
#endif
					t += *(int*)&TileMap[x][y];
				}
				//HandleRunning(&connections );
			}

			fread(&t1, sizeof(t), 1, fp);
			CheckSumError = t1 - t;
			fread(&lpMapFile->cOdd, sizeof(lpMapFile->cOdd), 1, fp);
			fread(&lpMapFile->cEven, sizeof(lpMapFile->cEven), 1, fp);
		}
		else
		{
			fseek(fp, 0, SEEK_SET);

			fread(&lpMapFile->wType, sizeof(lpMapFile->wType), 1, fp);

			fread(&lpMapFile->wWidth, sizeof(lpMapFile->wWidth), 1, fp);

			fread(&lpMapFile->wHeight, sizeof(lpMapFile->wHeight), 1, fp);


			for (int y = 0; y < lpMapFile->wHeight; y++)
			{
				for (int x = 0; x < lpMapFile->wWidth; x++)
				{
					fread(&TileMap[x][y], sizeof(TILE), 1, fp);
				}
				//HandleRunning(&connections );
			}
			fread(&lpMapFile->cOdd, sizeof(lpMapFile->cOdd), 1, fp);
			fread(&lpMapFile->cEven, sizeof(lpMapFile->cEven), 1, fp);
		}
		fclose(fp);
	}

	return	TRUE;
}
*/

BOOL MapWirteFile(LPMAPFILE lpMapFile, char* pszFileName)
{
	/*FILE* fp;
	int		t = 0;
	int     count = 0;
	DWORD	ver = 0xffff;

	//	JustMsg( " �����Ҽ� �����ϴ�. �� ����ȭ�����̿��ϼ���..." );
	//	return FALSE;

	DWORD fileattr = GetFileAttributes(pszFileName);
	if (fileattr & FILE_ATTRIBUTE_READONLY)
	{
		JustMsg(lan->OutputMessage(6, 17));//010216 lsw
	}

	fp = Fopen(pszFileName, "wb");
	{
		fwrite((char*)&lpMapFile->wType, sizeof(lpMapFile->wType), 1, fp);
		fwrite((char*)&lpMapFile->wWidth, sizeof(lpMapFile->wWidth), 1, fp);
		fwrite((char*)&lpMapFile->wHeight, sizeof(lpMapFile->wHeight), 1, fp);

		for (int y = 0; y < lpMapFile->wHeight; y++)
		{
			for (int x = 0; x < lpMapFile->wWidth; x++)
			{
				fwrite((char*)&TileMap[x][y], sizeof(TILE), 1, fp);
			}
		}


		fwrite((char*)&lpMapFile->cOdd, sizeof(lpMapFile->cOdd), 1, fp);
		fwrite((char*)&lpMapFile->cEven, sizeof(lpMapFile->cEven), 1, fp);

		fclose(fp);
	}


	strcat(pszFileName, "C");
	fp = Fopen(pszFileName, "wb");
	{
		fwrite((char*)&ver, sizeof(ver), 1, fp);
		fwrite((char*)&lpMapFile->wType, sizeof(lpMapFile->wType), 1, fp);
		fwrite((char*)&lpMapFile->wWidth, sizeof(lpMapFile->wWidth), 1, fp);
		fwrite((char*)&lpMapFile->wHeight, sizeof(lpMapFile->wHeight), 1, fp);

		for (int y = 0; y < lpMapFile->wHeight; y++)
		{
			for (int x = 0; x < lpMapFile->wWidth; x++)
			{
				t += *(int*)&TileMap[x][y];
				//				cc = (double*)&TileMap[ x][ y];
				//				*cc += (x+y);
				//				cc = ((double*)&TileMap[ x][ y]) + 1;
				//				*cc += (y);
				fwrite((char*)&TileMap[x][y], sizeof(TILE), 1, fp);
			}
		}

		fwrite((char*)&t, sizeof(t), 1, fp);
		fwrite((char*)&lpMapFile->cOdd, sizeof(lpMapFile->cOdd), 1, fp);
		fwrite((char*)&lpMapFile->cEven, sizeof(lpMapFile->cEven), 1, fp);

		fclose(fp);
	}


	for (int y = 0; y < lpMapFile->wHeight; y++)
	{
		for (int x = 0; x < lpMapFile->wWidth; x++)
		{
			//			cc = (double*)&TileMap[ x][ y];
			//			*cc -= (x+y);
			//			cc = ((double*)&TileMap[ x][ y]) + 1;
			//			*cc -= (y);
		}
	}
*/
	return	TRUE;
}

int GetAlphaFromRoof(TILE* t, int x, int y) {
  // 獲取對應的動畫狀態
  ROOF_ANIM* a = &HouseTileMap[x][y];
  const int FADE_SPEED = 1;
  // === 淡入淡出動畫邏輯 ===
  // 這段邏輯會根據 TILE 結構的 show_roof 標記來更新 alpha 值
  if (t->show_roof == 0)  // 如果 show_roof 標記為不顯示 (淡出)
  {
    if (a->alpha < 255) {
      a->alpha += FADE_SPEED;
      if (a->alpha > 255) a->alpha = 255;
    }
  } 
  else  // 如果 show_roof 標記為顯示 (淡入)
  {
    if (a->alpha > 0) {
      a->alpha -= FADE_SPEED;
      if (a->alpha < 0) a->alpha = 0;
    }
  }

  int alphaMapping = 32;
#ifdef _TEXTURE
  alphaMapping = 24;
#endif

  // 在這裡進行數值轉換
  int game_alpha = (a->alpha * alphaMapping) / 255;
  if (game_alpha < 0) game_alpha = 0;
  if (game_alpha > alphaMapping) game_alpha = alphaMapping;

  return game_alpha;
  // =======================
}

void MapDisplay(int nType) {
    if (g_StartMenuOn) return;

    int x=0, y=0;
    int mapx = g_Map.x;
    int mapy = g_Map.y;
    int				nIndex=0;
    TILE* t = nullptr;
    // 直接使用偏移量，不再做複雜的 if-else
    int displayPoint_X = g_Map.oX;
    int displayPoint_Y = g_Map.oY;

    // 為了涵蓋邊緣，繪製區域要往左/往上多推一個格子 (如果偏移量不為 0)
    if (displayPoint_X < 0) {
        // 不需要減 mapx，因為循環會處理
    }

    // 計算需要繪製的寬高 (多加一個 TILE_SIZE 以防露白)
    int mapTileWidth = SCREEN_WIDTH + TILE_SIZE;
    int mapTileHeight = GAME_SCREEN_YSIZE + TILE_SIZE;

    int temp_X = mapx;

    for (y = displayPoint_Y; y < mapTileHeight; y += TILE_SIZE, mapy++) {
        for (x = displayPoint_X; x < mapTileWidth; x += TILE_SIZE, mapx++) {
            t = &TileMap[mapx][mapy];

            int alpha = GetAlphaFromRoof(t, mapx, mapy);
            //HandleRunning(&connections);
            switch (nType)
            {
                case 0:
                {
#ifdef _DEBUG
                    if (tool_DrawMap) return;
#endif
                    if (nType == 0)
                    {
                        //  ٴ
                        if (t->roof == 0 || t->show_roof != 0 || t->rooftype == 1)
                        {
                            {
                                nIndex = t->back;
                                if (nIndex && nIndex < TotalTileNo)
                                {
                                    if (Sou[nIndex].xl == -1)
                                    {
                                        PutTile(x, y, &Sou[nIndex], Sou[nIndex].img, g_DestBackBuf);
                                    }
                                    else
                                    {
                                        PutCompressedImage(x, y, &Sou[nIndex]);
                                    }
                                }
                            }
                        }
                    }
                    break;
                }
                case 1:	//
                {
                    nIndex = t->roof;
                    if (nIndex && nIndex < TotalTileNo)
                    {
                        //if (t->show_roof == 0)
                        {
                            if (alpha > 0) {
                                if (Sou[nIndex].xl != -1) {
                                    //屋頂spr有問題
                                    if (Sou[nIndex].xl == 31) {
                                        Sou[nIndex].xl = 32;
                                    }
                                    PutCompressedImageFX(x, y, &Sou[nIndex], alpha, 1);
                                }
                                else {
                                    PutTileAlpha(x, y, &Sou[nIndex], Sou[nIndex].img, g_DestBackBuf, alpha);
                                }
                            }
                        }
                    }
                    break;
                }
                case 2:
                {	//
#ifdef _DEBUG
                    if (tool_DrawFrontMap) return;
#endif
                    if (t->roof == 0 || t->show_roof || t->rooftype == 1)
                    {
                        nIndex = t->front;
                        if (nIndex && nIndex < TotalTileNo)
                        {
                            if (Sou[nIndex].xl == -1)
                                PutTile(x, y, &Sou[nIndex], Sou[nIndex].img, g_DestBackBuf);
                            else
                                PutCompressedImage(x, y, &Sou[nIndex]);
                        }
                    }
                    break;
                }
            }// switch
        }
        mapx = temp_X;
    }
}

/////  View Mapattr..

// 010314 KHS �Լ���ü... .SKB�� NPC�Ӽ����� ������ �ִ� �͵� ǥ������ �Ͽ���. 
void  MapDisplay_Attr(void)
{
	
#ifndef _DEBUG
	return;
#endif			

int	x, y;
	int				mapx, mapy;
	int sx, sy;
	static int to;
	int range_X;
	int range_Y;


	if (tool_ViewAttrRoofOnOff ||
		tool_ViewAttrRiver ||
		tool_ViewAttrGEffect ||
		tool_ViewAttrOccupied ||
		tool_ViewAttrInside ||
		tool_ViewAttrNoBattle ||
		tool_ViewAttrCanMatch ||
		tool_ViewAttrEventFlag ||
		tool_ViewAttrDontFlag ||
		tool_ViewAttrFrontFlag ||
		tool_ID_INPUT_MAP_ATTR ||
		tool_ID_SKILL_INPUT ||
		tool_ViewAttrTop ||			//	010906 LTS
		tool_ID_INPUT_MYHOUSE
		) goto  GO_PROCESS_;

	return;

GO_PROCESS_:

	to = !to;

	LPEVENTLIST		lpEventList;
	mapy = g_Map.y;
	for (y = 0; y < GAME_SCREEN_YSIZE; y += TILE_SIZE, mapy++)
	{
		for (mapx = g_Map.x, x = 0; x < GAME_SCREEN_XSIZE; x += TILE_SIZE, mapx++)
		{
			if (0)
				if (TileMap[mapx][mapy].attr_room)
				{
					sx = mapx * TILE_SIZE;
					sy = mapy * TILE_SIZE;
					Box(sx + 4, sy + 4, sx + TILE_SIZE - 8, sy + TILE_SIZE - 8, RGB16(0, 255, 0));

					lpEventList = FindEventList(&g_EventListHeaderRoom, (WORD)mapx, (WORD)mapy);
					if (lpEventList)
					{
						if (TileMap[mapx][mapy].attr_room == 1) // �����....
						{
							//Hcolor(0);
							//Hprint2(sx - Mapx, sy - Mapy, g_DestBackBuf, lan->OutputMessage(7, 10), lpEventList->index);//010216 lsw
							Hcolor(0, 255, 0);
							Hprint2(sx - Mapx - 1, sy - Mapy - 1, g_DestBackBuf, lan->OutputMessage(7, 10), lpEventList->index);//010216 lsw
						}
						else
						{
							//Hcolor(0);
							//Hprint2(sx - Mapx, sy - Mapy, g_DestBackBuf, lan->OutputMessage(7, 29), lpEventList->index);//010216 lsw
							Hcolor(255, 255, 0);
							Hprint2(sx - Mapx - 1, sy - Mapy - 1, g_DestBackBuf, lan->OutputMessage(7, 29), lpEventList->index);//010216 lsw
						}
					}
				}
			if (1)
				if (TileMap[mapx][mapy].attr_river)
				{
					sx = mapx * TILE_SIZE;
					sy = mapy * TILE_SIZE;
					Box(sx, sy, sx + TILE_SIZE, sy + TILE_SIZE, RGB16(0, 100, 200));
					Hprint2(sx - Mapx, sy - Mapy, g_DestBackBuf, "river");
				}
			if (0)
				if (TileMap[mapx][mapy].attr_light)
				{
					sx = mapx * TILE_SIZE;
					sy = mapy * TILE_SIZE;
					Box(sx + 2, sy + 2, sx + TILE_SIZE - 4, sy + TILE_SIZE - 4, RGB16(255, 255, 255));
					Hprint2(sx - Mapx, sy - Mapy, g_DestBackBuf, "attr_light");
				}
			if (1)
				if (TileMap[mapx][mapy].attr_map)
				{
					sx = mapx * TILE_SIZE;
					sy = mapy * TILE_SIZE;
					Box(sx + 3, sy + 3, sx + TILE_SIZE - 6, sy + TILE_SIZE - 6, RGB16(0, 0, 255));
					Hprint2(sx - Mapx, sy - Mapy, g_DestBackBuf, "%d", ReturnEventNo(mapx, mapy));
				}
			if (0)
				if (TileMap[mapx][mapy].attr_inside)
				{
					sx = mapx * TILE_SIZE;
					sy = mapy * TILE_SIZE;
					Box(sx + 3, sy + 3, sx + TILE_SIZE - 6, sy + TILE_SIZE - 6, RGB16(255, 100, 100));
					Hprint2(sx - Mapx, sy - Mapy, g_DestBackBuf, "in");
				}
			if (0)
				if (TileMap[mapx][mapy].attr_no_battle)
				{
					sx = mapx * TILE_SIZE;
					sy = mapy * TILE_SIZE;
					Box(sx + 3, sy + 3, sx + TILE_SIZE - 6, sy + TILE_SIZE - 6, RGB16(100, 255, 100));
					Hprint2(sx - Mapx, sy - Mapy, g_DestBackBuf, "npk");
				}
			if (0)
				if (TileMap[mapx][mapy].canmatch)
				{
					sx = mapx * TILE_SIZE;
					sy = mapy * TILE_SIZE;
					Box(sx + 3, sy + 3, sx + TILE_SIZE - 6, sy + TILE_SIZE - 6, RGB16(100, 0, 100));
					Hprint2(sx - Mapx, sy - Mapy, g_DestBackBuf, "cget");
				}

			//___________________________________________________________________	// 010904 LTS
			if (tool_ViewAttrTop)			// ������������ ����� Ÿ��.
				if (TileMap[mapx][mapy].empty__4)
				{
					sx = mapx * TILE_SIZE;
					sy = mapy * TILE_SIZE;
					Box(sx + 4, sy + 4, sx + TILE_SIZE - 5, sy + TILE_SIZE - 5, RGB16(255, 255, 0));
				}

			if (0)		// ��ã�⿡����  ����Ÿ�� 
				if (TileMap[mapx][mapy].occupied)
				{
					sx = mapx * TILE_SIZE;
					sy = mapy * TILE_SIZE;
					Box(sx + 3, sy + 3, sx + TILE_SIZE - 6, sy + TILE_SIZE - 6, RGB16(0, 255, 100));
					Hprint2(sx - Mapx, sy - Mapy, g_DestBackBuf, "not");
				}
			//___________________________________________________________________	// 010904 LTS

			/*
			if (tool_ViewAttrDontFlag)
			{
				if (TileMap[mapx][mapy].occupied)
				{
					sx = mapx * TILE_SIZE;
					sy = mapy * TILE_SIZE;
					Box(sx + 3, sy + 3, sx + TILE_SIZE - 6, sy + TILE_SIZE - 6, RGB16(0, 255, 100));
				}


				if (TileMap[mapx][mapy].attr_dont)
				{
					sx = mapx * TILE_SIZE;
					sy = mapy * TILE_SIZE;
					Box(sx + 1, sy + 1, sx + 30, sy + 30, RGB16(255, 0, 0));
					Hprint2(sx - Mapx, sy - Mapy, g_DestBackBuf, "attr_dont");
				}
			}
			if (tool_ViewAttrFrontFlag)
				if (TileMap[mapx][mapy].front)
				{
					sx = mapx * TILE_SIZE;
					sy = mapy * TILE_SIZE;

					if (to && tMapAttr_Offset_Modify == 1 && tMapAttrX == mapx && tMapAttrY == mapy)
					{

					}
					else
					{
						Box(sx + 5, sy + 5, sx + 24, sy + 24, RGB16(255, 0, 255));
					}
					Hcolor(0xffff);
					Hprint2(sx - Mapx, sy - Mapy + 10, g_DestBackBuf, "%3d", TileMap[mapx][mapy].frontoffset);
				}
				*/



			lpMAPSKILLTABLE	result = { 0 };
			DWORD		lineColor=0;
			int nx, ny;

			nx = (mapx - g_Map.x) * TILE_SIZE;
			ny = (mapy - g_Map.y) * TILE_SIZE;

			if (tool_ID_SKILL_INPUT)
				if (TileMap[mapx][mapy].attr_skill)
				{
					int count = 0;
					sx = mapx * TILE_SIZE;
					sy = mapy * TILE_SIZE;
					if (g_Map.file.wWidth + 7 < 8 || g_Map.file.wHeight + 7 < 8) {
						return;
					}
					int x = (mapx / ((g_Map.file.wWidth + 7) / 8));
					int y = (mapy / ((g_Map.file.wHeight + 7) / 8));
					if (x >= 8 || y >= 8)return;
					while ((result = FindSkill(&Header[x][y], mapx, mapy, count)))
						if (result != NULL)
						{
							count++;
							switch (result->skillno)
							{
							case TOOL_DONTSKILL:
								Hcolor(150, 255, 150);
								Hprint2((mapx - g_Map.x) * 32 + 2, (mapy - g_Map.y) * 32 + 10, g_DestBackBuf, "Don't");
								Box(sx + 2, sy + 2, sx + TILE_SIZE - 2, sy + TILE_SIZE - 2, RGB16(255, 50, 50));
								break;
							case TOOL_MINING:
							{
								switch (result->subType)
								{
								case 0:	lineColor = RGB16(203, 185, 156);	break;
								case 1:	lineColor = RGB16(158, 139, 117);	break;
								case 2:	lineColor = RGB16(118, 100, 85);	break;
								case 3:	lineColor = RGB16(82, 68, 60);		break;
								case 4:	lineColor = RGB16(51, 42, 41);		break;
								}
								break;
							}
							case TOOL_FARMING:
							{
								switch (result->subType)
								{
								case 0:	lineColor = RGB16(221, 189, 140);			break;
								case 1:	lineColor = RGB16(200, 164, 110);			break;
								case 2:	lineColor = RGB16(169, 131, 79);			break;
								case 3:	lineColor = RGB16(132, 92, 42);				break;
								case 4:	lineColor = RGB16(91, 55, 20);				break;
								case 5:	lineColor = RGB16(165, 84, 5);				break;
								case 6:	lineColor = RGB16(155, 77, 0);				break;
								case 7:	lineColor = RGB16(197, 79, 0);				break;
								case 8:	lineColor = RGB16(74, 33, 6);				break;
								case 9:	lineColor = RGB16(55, 23, 2);				break;
								}
								break;
							}
							case TOOL_HUB:
							{
								switch (result->subType)
								{
								case 0:	lineColor = RGB16(137, 202, 157);		break;
								case 1:	lineColor = RGB16(47, 180, 114);		break;
								case 2:	lineColor = RGB16(0, 105, 63);			break;
								case 3:	lineColor = RGB16(0, 75, 44);			break;
								case 4:	lineColor = RGB16(31, 64, 50);			break;
								case 5:	lineColor = RGB16(66, 109, 91);			break;
								case 6:	lineColor = RGB16(82, 132, 19);			break;
								case 7:	lineColor = RGB16(60, 102, 8);			break;
								case 8:	lineColor = RGB16(103, 181, 4);			break;
								case 9:	lineColor = RGB16(35, 52, 14);			break;
								}
								break;
							}
							case TOOL_NPC_GENER:
								lineColor = RGB16(50, 50, 255);
								break;
							case TOOL_BUILDHOUSE:
								lineColor = RGB16(0, 0, 0);
								break;
							}

							if (result->skillno != TOOL_DONTSKILL)
							{

								{
									int xl, yl;
									xl = (result->tile_Range * 2 * TILE_SIZE) + TILE_SIZE;
									yl = (result->tile_Range * 2 * TILE_SIZE) + TILE_SIZE;
									range_X = (mapx - result->tile_Range) * TILE_SIZE;
									range_Y = (mapy - result->tile_Range) * TILE_SIZE;

									Box(sx + 1, sy + 1, sx + TILE_SIZE - 1, sy + TILE_SIZE - 1, RGB16(255, 255, 0));
									switch (radio_Statue)
									{
									case 0:
										//Box(range_X, range_Y, xl, yl, lineColor);
										break;
									case 1:
										if (result->tile_Range != 0 && result->skillno == TOOL_FARMING)
											Box(range_X, range_Y, xl, yl, lineColor);
										break;
									case 2:
										if (result->tile_Range != 0 && result->skillno == TOOL_MINING)
											Box(range_X, range_Y, xl, yl, lineColor);
										break;
									case 3:
										if (result->tile_Range != 0 && result->skillno == TOOL_FISHING)
											Box(range_X, range_Y, xl, yl, lineColor);
										break;
									case 4:
										if (result->tile_Range != 0 && result->skillno == TOOL_CHOPPING)
											Box(range_X, range_Y, xl, yl, lineColor);
										break;
									case 5:
										if (result->tile_Range != 0 && result->skillno == TOOL_HUB)
											Box(range_X, range_Y, xl, yl, lineColor);
										break;
									case 6:

										break;
									case 7:
										if (result->tile_Range != 0 && result->skillno == TOOL_BUILDHOUSE)
											Box(range_X, range_Y, xl, yl, lineColor);
										break;
									}
								}

								if (result->skillno == 6)
								{
									// ��ȿ����(�߻���)  ����Ȯ��(Script)   NPC������ȣ	��������Data 
									static char othername[6][20] = { " ", "Enchant", "Mad", "Boss"," ", " " };
									HprintBold(nx, ny, RGB(200, 200, 200), RGB16(0, 0, 0), lan->OutputMessage(7, 35), result->tile_Range, result->probability);//010216 lsw

									int tmon;
									int tenchant;
									//< CSD-030419
									if (result->type_Num < MAX_CHARACTER_SPRITE_)
									{
										tmon = result->type_Num;
										tenchant = 0;
									}
									else
									{
										tmon = result->type_Num % 100;
										tenchant = result->type_Num / 100;
									}
									//> CSD-030419
									HprintBold(nx, ny + 16, RGB(200, 50 * tenchant + 50, 50 * tenchant + 50), RGB16(0, 0, 0), "%d:%s %s", result->type_Num, othername[tenchant], g_infNpc[tmon].szName); // CSD-030419
								}
								else
								{
									HprintBold(nx, ny, RGB(200, 200, 200), RGB16(0, 0, 0), "%d-%d-%d-%d", result->skillno, result->subType, result->tile_Range, result->probability);
								}
							}
						}

					if (count > 1)	HprintBold(nx - 40, ny, RGB(200, 200, 200), RGB16(0, 0, 0), "<<%d>>", count);
				}
		}
	}
}




///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// roof
// 假設你的 LPROOFHEADER 定義在某處
// 建議將 BOOL 改為標準 bool 或 SDL_bool，這裡先沿用 bool
bool BuildRoofHeader(LPROOFHEADER lpRoofHeader, char* mapname)
{
    char lpszFileName[256]; // Android 路徑通常不會超過這個長度

    // 初始化結構
    lpRoofHeader->lpFirst = lpRoofHeader->lpLast = NULL;
    lpRoofHeader->count = 0;

    // 1. 格式化路徑
    // 注意：Android 對大小寫敏感 (Case Sensitive)。
    // 如果你的資料夾在電腦上是 "Event"，這裡就必須是 "Event"，不能是 "event"。
    // 建議統一使用斜線 '/' 作為路徑分隔符
    snprintf(lpszFileName, sizeof(lpszFileName), "Event/%s.rof", mapname);

    // 3. 呼叫讀取函式
    // 注意：原本的 ReadRoofData 可能接受 char* 或 FILE*。
    // 你必須修改 ReadRoofData 讓它接受 SDL_RWops* (詳見下方說明)。
    if (!ReadRoofData(lpRoofHeader, lpszFileName))
    {
        SDL_Log("[%s] LoadError", lpszFileName);
        return false;
    }


    return true;
}

// ���� �̺�Ʈ�� xy�� ������ �ִ� ����Ʈ���� ���� ã�� �� ����Ʈ�� �Ѱ� �ش�.
// 031110 YGI
LPROOFGROUP
FindRoofGroupByXY(LPROOFHEADER lpRoofHeader, int x, int y)
{
	LPROOFGROUP		lpRoofGroup, lpRoofGroupNext;
	LPROOF			lpRoof, lpRoofNext;

	lpRoofGroup = lpRoofGroupNext = lpRoofHeader->lpFirst;

	while (lpRoofGroup)
	{
		lpRoof = lpRoofNext = lpRoofGroup->lpFirst;
		while (lpRoof)
		{
			if (lpRoof->x == x && lpRoof->y == y)
			{
				return lpRoofGroup;
			}
			lpRoofNext = lpRoof->lpNext;
			lpRoof = lpRoofNext;
		}

		lpRoofGroupNext = lpRoofGroup->lpNext;
		lpRoofGroup = lpRoofGroupNext;
	}
	return NULL;
}

void
DestroyRoofHeader(LPROOFHEADER lpRoofHeader)
{
	LPROOFGROUP		lpRoofGroup, lpRoofGroupNext;
	LPROOF			lpRoof, lpRoofNext;

	lpRoofGroup = lpRoofGroupNext = lpRoofHeader->lpFirst;

	while (lpRoofGroup)
	{
		lpRoof = lpRoofNext = lpRoofGroup->lpFirst;

		while (lpRoof)
		{
			lpRoofNext = lpRoof->lpNext;
			DeleteRoof(lpRoofGroup, lpRoof);
			lpRoof = lpRoofNext;
		}

		lpRoofGroupNext = lpRoofGroup->lpNext;
		DeleteRoofGroup(lpRoofHeader, lpRoofGroup);
		lpRoofGroup = lpRoofGroupNext;
	}
}

LPROOFGROUP AddRoofGroup(LPROOFHEADER lpRoofHeader)
{
    LPROOFGROUP lpRoofGroup;

    if ((lpRoofGroup = new ROOFGROUP) == NULL)
    {
        return NULL;
    }

    // [Fix] 必須初始化內部的 Roof 鏈結串列指標！
    // 否則 AddRoof 函式判斷 lpLast != NULL 時會讀到垃圾值而崩潰
    lpRoofGroup->lpFirst = NULL;
    lpRoofGroup->lpLast  = NULL;
    lpRoofGroup->count   = 0;

    // 處理 Group 之間的鏈結 (這部分原本是對的)
    if (lpRoofHeader->lpLast == NULL)
    {
        lpRoofGroup->lpPrev = NULL; // 第一個節點的 Prev 應該是 NULL
        lpRoofGroup->lpNext = NULL;
        lpRoofHeader->lpFirst = lpRoofHeader->lpLast = lpRoofGroup;
    }
    else
    {
        lpRoofGroup->lpPrev = lpRoofHeader->lpLast;
        lpRoofGroup->lpNext = NULL;

        lpRoofHeader->lpLast->lpNext = lpRoofGroup;
        lpRoofHeader->lpLast = lpRoofGroup;
    }
    lpRoofHeader->count++;

    return lpRoofGroup;
}

void
DeleteRoofGroup(LPROOFHEADER lpRoofHeader, LPROOFGROUP lpRoofGroup)
{
	if (lpRoofGroup != NULL)
	{
		LPROOF		lpRoof, lpRoofNext;

		lpRoof = lpRoofNext = lpRoofGroup->lpFirst;
		while (lpRoof)
		{
			lpRoofNext = lpRoof->lpNext;
			DeleteRoof(lpRoofGroup, lpRoof);
			lpRoof = lpRoofNext;
		}

		if (lpRoofGroup->lpPrev != NULL)
		{
			lpRoofGroup->lpPrev->lpNext = lpRoofGroup->lpNext;
		}
		if (lpRoofGroup->lpNext != NULL)
		{
			lpRoofGroup->lpNext->lpPrev = lpRoofGroup->lpPrev;
		}

		if (lpRoofHeader->lpLast == lpRoofGroup)
		{
			lpRoofHeader->lpLast = lpRoofGroup->lpPrev;
		}
		if (lpRoofHeader->lpFirst == lpRoofGroup)
		{
			lpRoofHeader->lpFirst = lpRoofGroup->lpNext;
		}

		lpRoofHeader->count--;

		delete lpRoofGroup;
		lpRoofGroup = NULL;
	}
}

LPROOF AddRoof(LPROOFGROUP lpRoofGroup, WORD x, WORD y)
{
    LPROOF lpRoof;

    if ((lpRoof = new ROOF) == NULL)
    {
        return NULL;
    }

    lpRoof->x = x;
    lpRoof->y = y;

    // [建議] 這裡最好也明確初始化，雖然下面的邏輯會覆蓋它，
    // 但如果邏輯改動，容易留下隱患。
    lpRoof->lpPrev = NULL;
    lpRoof->lpNext = NULL;

    if (lpRoofGroup->lpLast == NULL)
    {
        lpRoof->lpPrev = NULL; // 修正：頭節點的 prev 應為 NULL
        lpRoof->lpNext = NULL;
        lpRoofGroup->lpFirst = lpRoofGroup->lpLast = lpRoof;
    }
    else
    {
        lpRoof->lpPrev = lpRoofGroup->lpLast;
        lpRoof->lpNext = NULL;

        lpRoofGroup->lpLast->lpNext = lpRoof; // 如果 lpRoofGroup 沒有初始化，這裡就會爆
        lpRoofGroup->lpLast = lpRoof;
    }
    lpRoofGroup->count++; // 記得這裡要增加計數

    return lpRoof;
}

void
DeleteRoof(LPROOFGROUP lpRoofGroup, LPROOF lpRoof)
{
	if (lpRoof != NULL)
	{
		if (lpRoof->lpPrev != NULL)
		{
			lpRoof->lpPrev->lpNext = lpRoof->lpNext;
		}
		if (lpRoof->lpNext != NULL)
		{
			lpRoof->lpNext->lpPrev = lpRoof->lpPrev;
		}

		if (lpRoofGroup->lpLast == lpRoof)
		{
			lpRoofGroup->lpLast = lpRoof->lpPrev;
		}
		if (lpRoofGroup->lpFirst == lpRoof)
		{
			lpRoofGroup->lpFirst = lpRoof->lpNext;
		}

		lpRoofGroup->count--;

		delete lpRoof;
		lpRoof = NULL;
	}
}

LPROOFGROUP
FindRoofGroup(LPROOFHEADER lpRoofHeader, WORD index)
{
	LPROOFGROUP		lpRoofGroup = lpRoofHeader->lpFirst;
	int				nCount = 0;

	while (lpRoofGroup)
	{
		if (nCount++ == index)
		{
			return	lpRoofGroup;
		}

		lpRoofGroup = lpRoofGroup->lpNext;
	}

	return	NULL;
}

LPROOF
FindRoof(LPROOFGROUP lpRoofGroup, WORD x, WORD y)
{
	LPROOF		lpRoof = lpRoofGroup->lpFirst;

	while (lpRoof)
	{
		if (lpRoof->x == x && lpRoof->y == y)
		{
			return	lpRoof;
		}

		lpRoof = lpRoof->lpNext;
	}

	return	NULL;
}

bool ReadRoofData(LPROOFHEADER lpRoofHeader, char* lpszFileName)
{
    SDL_RWops* fp = nullptr;
    Uint16 headercount, groupcount; // WORD -> Uint16 (確保是 2 bytes)
    Uint16 x, y;

    // [Android] 1. 路徑處理：移除 ./
    std::string path = lpszFileName;
    if (path.find("./") == 0) path = path.substr(2);

    // [Android] 2. 開啟檔案
    fp = SDL_RWFromFile(path.c_str(), "rb");

    if (fp)
    {
        // 讀取 Header Count
        SDL_RWread(fp, &headercount, sizeof(Uint16), 1);

        for (int i = 0; i < headercount; i++)
        {
            // 讀取 Group Count
            SDL_RWread(fp, &groupcount, sizeof(Uint16), 1);

            AddRoofGroup(lpRoofHeader);

            // 讀取每個屋頂塊的座標
            for (int j = 0; j < groupcount; j++)
            {
                SDL_RWread(fp, &x, sizeof(Uint16), 1);
                SDL_RWread(fp, &y, sizeof(Uint16), 1);

                // 確保指標安全 (防禦性編程)
                if (lpRoofHeader->lpLast) {
                    AddRoof(lpRoofHeader->lpLast, x, y);
                }
            }
        }

        SDL_RWclose(fp);
        return TRUE;
    }

    // 找不到檔案時記錄 Log
    SDL_Log("ReadRoofData: Not found %s", path.c_str());
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// event

BOOL
BuildEventList(char* mapname)
{
	char szFileName[FILENAME_MAX];

	sprintf(szFileName, "Event/%s.ent",  MapName);
	g_EventListHeaderMap.lpFirst = g_EventListHeaderMap.lpLast = NULL;
	g_EventListHeaderMap.wCount = 0;

	g_EventListHeaderRoom.lpFirst = g_EventListHeaderRoom.lpLast = NULL;
	g_EventListHeaderRoom.wCount = 0;

	g_EventListHeaderMusic.lpFirst = g_EventListHeaderMusic.lpLast = NULL;
	g_EventListHeaderMusic.wCount = 0;

	if (!ReadEventList(szFileName))
	{
		return	FALSE;
	}

	return	TRUE;
}

void
DestroyEventListSub(LPEVENTLISTHEADER lpEventListHeader)
{
	LPEVENTLIST		lpEventList, lpEventListNext;

	lpEventList = lpEventListNext = lpEventListHeader->lpFirst;
	while (lpEventList)
	{
		lpEventListNext = lpEventList->lpNext;
		DeleteEventList(lpEventListHeader, lpEventList);
		lpEventList = lpEventListNext;
	}
}



void DestroyEventList(void)
{
	DestroyEventListSub(&g_EventListHeaderMap);
	DestroyEventListSub(&g_EventListHeaderRoom);
	DestroyEventListSub(&g_EventListHeaderMusic);

}


LPEVENTLIST
AddEventList(LPEVENTLISTHEADER lpEventListHeader, WORD x, WORD y, WORD wIndex)
{
	LPEVENTLIST		lpEventList;

	if ((lpEventList = new EVENTLIST)==NULL)
	{
		return	NULL;
	}

	if (lpEventListHeader->lpLast == NULL)
	{
		lpEventList->lpPrev = lpEventList->lpNext = NULL;
		lpEventListHeader->lpFirst = lpEventListHeader->lpLast = lpEventList;
	}
	else
	{
		lpEventList->lpPrev = lpEventListHeader->lpLast;
		lpEventList->lpNext = NULL;
		lpEventListHeader->lpLast->lpNext = lpEventList;
		lpEventListHeader->lpLast = lpEventList;
	}
	lpEventListHeader->wCount++;

	lpEventList->x = x;
	lpEventList->y = y;
	lpEventList->index = wIndex;

	return	lpEventList;
}

void
DeleteEventList(LPEVENTLISTHEADER lpEventListHeader, LPEVENTLIST lpEventList)
{
	if (lpEventList != NULL)
	{
		if (lpEventList->lpPrev != NULL)
		{
			lpEventList->lpPrev->lpNext = lpEventList->lpNext;
		}
		if (lpEventList->lpNext != NULL)
		{
			lpEventList->lpNext->lpPrev = lpEventList->lpPrev;
		}

		if (lpEventListHeader->lpFirst == lpEventList)
		{
			lpEventListHeader->lpFirst = lpEventList->lpNext;
		}
		if (lpEventListHeader->lpLast == lpEventList)
		{
			lpEventListHeader->lpLast = lpEventList->lpPrev;
		}

		lpEventListHeader->wCount--;

		delete lpEventList;
		lpEventList = NULL;
	}
}

LPEVENTLIST
FindEventList(LPEVENTLISTHEADER lpEventListHeader, WORD x, WORD y)
{
	LPEVENTLIST		lpEventList = lpEventListHeader->lpFirst;

	while (lpEventList)
	{
		if (lpEventList->x == x && lpEventList->y == y)
		{
			return	lpEventList;
		}

		lpEventList = lpEventList->lpNext;
	}

	return	NULL;
}

BOOL ReadEventList(char* lpszFileName)
{
    std::string path = lpszFileName;
    if (path.find("./") == 0) path = path.substr(2);

    SDL_RWops* fp = SDL_RWFromFile(path.c_str(), "rb");
    if (!fp) {
        SDL_Log("ReadEventList: Not found %s", path.c_str());
        return FALSE;
    }

    // 定義一個讀取區塊的 Lambda
    auto ReadBlock = [&](LPEVENTLIST* targetListHeader) {
        Uint16 count, x, y, index;
        if (SDL_RWread(fp, &count, 2, 1) == 1) {
            for (int i = 0; i < count; i++) {
                SDL_RWread(fp, &x, 2, 1);
                SDL_RWread(fp, &y, 2, 1);
                SDL_RWread(fp, &index, 2, 1);
                AddEventList(reinterpret_cast<LPEVENTLISTHEADER>(targetListHeader), x, y, index);
            }
        }
    };

    // 依序讀取三個區塊
    ReadBlock(reinterpret_cast<LPEVENTLIST *>(&g_EventListHeaderMap));
    ReadBlock(reinterpret_cast<LPEVENTLIST *>(&g_EventListHeaderRoom));
    ReadBlock(reinterpret_cast<LPEVENTLIST *>(&g_EventListHeaderMusic));

    SDL_RWclose(fp);
    return TRUE;
}







// ---------------------------------------���콺�� ���� Map Scroll...

BOOL IsScreenArea(LPCHARACTER ch)
{
	Spr* sp = ch->sp;
	DWORD	mx = ch->position.x;
	DWORD	my = ch->position.y;

	if (ch->sprno == 50 || ch->sprno == 51 || ch->sprno == 52)	// LTS DRAGON BUG
	{
		ch->direction = DIRECTION_DOWN;
		sp = ch->sp = &CharSpr[ch->sprno].sp[ch->direction][ch->nCurrentFrame.GetDecrypted()];
	}

	if (sp == NULL) return 0;

	if (mx < 0) return 0;
	if (my < 0) return 0;
	if (mx > g_Map.file.wWidth) return 0;
	if (my > g_Map.file.wHeight) return 0;

	if (g_Inside != TileMap[mx][my].attr_inside) return 0;

	int t0 = BoxAndBoxCrash(Mapx, Mapy, GAME_SCREEN_XSIZE, GAME_SCREEN_YSIZE, ch->x - sp->ox, ch->y - sp->oy - ch->height, sp->xl, sp->yl);
	int t1 = BoxAndBoxCrash(Mapx, Mapy, GAME_SCREEN_XSIZE, GAME_SCREEN_YSIZE, ch->x - sp->ox, ch->y - sp->oy, sp->xl, sp->yl);

	if (t0 || t1) return 1;

	return 0;
}

BOOL IsScreenAreaMapObject(int x, int y, SIZE size)
{
	int xl = size.cx;
	int yl = size.cy;

	if (BoxAndBoxCrash(Mapx, Mapy, GAME_SCREEN_XSIZE, GAME_SCREEN_YSIZE, x, y, xl, yl)) return 1;

	return 0;
}

BOOL IsScreenAreaCSP(int x, int y, Spr* sp)
{
	int xl = sp->xl;
	int yl = sp->yl;
	if (BoxAndBoxCrash(Mapx, Mapy, GAME_SCREEN_XSIZE, GAME_SCREEN_YSIZE, x - sp->ox, y - sp->oy, xl, yl)) return 1;
	return 0;
}

///////////////////////////// 0808 lkh �߰� ////////////////////////////////////
BOOL IsScreenAreaView(int x, int y)		//������Ʈ�� ���� ��ǥ
{
	if (BoxAndDotCrash(Hero->x - (OBJECTSOUNDPLAY_XSIZE / 2), Hero->y - (OBJECTSOUNDPLAY_YSIZE / 2), OBJECTSOUNDPLAY_XSIZE, OBJECTSOUNDPLAY_YSIZE, x, y)) return 1;
	return 0;
}

void SetView(LPCHARACTER ch, int range)
{
    if (!ch) return;
    ch->sight = range;

    int rangex = range;
    int rangey = range - 2;

    // 【關鍵修改】：使用 visual_x / visual_y (平滑座標) 來計算中心
    // 原本是用 ch->x / ch->y (這是導致邊緣抖動的元兇)
    int center_x = (int)(ch->visual_x / 32.0f);
    int center_y = (int)(ch->visual_y / 32.0f);

    int mx = center_x - (GAME_SCREEN_XSIZE / 32 / 2) - 1;
    // 如果覺得畫面沒置中，可以把上面的 -1 去掉試試
    int my = center_y - (GAME_SCREEN_YSIZE / 32 / 2) - 1;

    // 計算捲動邊界常量 (保持原本邏輯)
    LimitMapSx = mx - rangex - 3;	if (LimitMapSx < 0) LimitMapSx = 0;
    LimitMapSy = my - rangey + 5;	if (LimitMapSy < 0) LimitMapSy = 0;

    LimitMapEx = mx + rangex + 3;
    if (LimitMapEx > g_Map.file.wWidth - (GAME_SCREEN_XSIZE / 32))
        LimitMapEx = g_Map.file.wWidth - (GAME_SCREEN_XSIZE / 32);

    LimitMapEy = my + rangey - 5;
    if (LimitMapEy > g_Map.file.wHeight - (GAME_SCREEN_YSIZE / 32) - 1)
        LimitMapEy = g_Map.file.wHeight - (GAME_SCREEN_YSIZE / 32) - 1;
}
/*
///////////////////////// 0625 lkh ���� /////////////////////////////
void SetView(LPCHARACTER ch, int range)
{
    if (!ch) return;
    ch->sight = range;

    int rangex = range;
    int rangey = range - 2;

    // 這裡使用邏輯座標 position.x 是對的，因為攝像機目標通常跟隨邏輯格
    int mx = ch->position.x - (GAME_SCREEN_XSIZE / 32 / 2) - 1;
    int my = ch->position.y - (GAME_SCREEN_YSIZE / 32 / 2) - 1;

    // 計算地圖捲動的限制邊界 (原本的邏輯不變)
    LimitMapSx = mx - rangex - 2;	if (LimitMapSx < 0) LimitMapSx = 0;
    LimitMapSy = my - rangey + 5;	if (LimitMapSy < 0) LimitMapSy = 0;
    LimitMapEx = mx + rangex + 3;	if (LimitMapEx > g_Map.file.wWidth - (GAME_SCREEN_XSIZE / 32))  LimitMapEx = g_Map.file.wWidth - (GAME_SCREEN_XSIZE / 32);
    LimitMapEy = my + rangey - 3;	if (LimitMapEy > g_Map.file.wHeight - (GAME_SCREEN_YSIZE / 32) - 1)  LimitMapEy = g_Map.file.wHeight - (GAME_SCREEN_YSIZE / 32) - 1;

    // 設定目標格子
    if (g_Map.tox < LimitMapSx) g_Map.tox = LimitMapSx;
    if (g_Map.tox > LimitMapEx) g_Map.tox = LimitMapEx;
    if (g_Map.toy < LimitMapSy) g_Map.toy = LimitMapSy;
    if (g_Map.toy > LimitMapEy) g_Map.toy = LimitMapEy;

    // 注意：這裡【刪除】原本的 g_Map.x++ 邏輯，因為它太慢了 (17 UPS)
}
*/


// 依據開啟的選單類型，調整英雄角色的螢幕位置。
// 參數 flag 用來控制是否強制更新位置，即使選單狀態沒有改變。
void SetHeroPosition(int flag)
{
	int lc = 0, rc = 0; // lc: 左側選單計數，rc: 右側選單計數
	int guild = 0; // 判斷是否有公會或商店選單開啟
	static WORD changed; // 靜態變數，用來記錄上一次的選單狀態
	WORD t = 0; // 當前選單狀態的暫存變數

	// 定義一個包含所有「小選單」（例如商店、公會、銀行等）的陣列
	int sm[] = {
		MN_BLACKSMITH, MN_BLACKSMITH, MN_FARM, MN_RESTAURANT, MN_HERBSHOP, MN_SHAMBLES, MN_CANDLE, MN_SLOPSHOP,
		MN_ALCHEMY, MN_WOODWORKING, MN_BOWCRAFT, MN_SHIPYARD, MN_SHOP_MAIN, MN_DRINK_MAIN, MN_INN_MAIN,
		MN_MAGIC_GUILD, MN_ACADEMY, MN_TOWNHALL ,MN_HEAL_MAIN,MN_SKILLGUILD_SKILL, MN_SKILLGUILD_KNOWLAGE,
		MN_SKILLGUILD_ITEM, MN_MAIL_WRITE,
		MN_MAIL_MENU, MN_MAIL_READ, MN_BANK_DEPOSIT, MN_BANK_DEFRAYAL, MN_BANK_MAIN, MN_BANK_LOAN,
		MN_BANK_REPAYMENT, MN_BANK_AUCTION, MN_BANK_AUCTION_SALE, MN_DRINK_RUMOR, MN_HEAL_HEALING,
		MN_HEAL_PREVENTING, MN_HEAL_LEARN, MN_INN_REST, MN_INN_SLEEP,
	};
	// 計算小選單陣列的大小
	int sm_max = sizeof(sm) / sizeof(int);

	// 如果英雄角色物件不存在，則直接返回
	if (Hero == NULL) return;

	// ----------------------------------------------------
	// 檢查並記錄當前開啟的選單狀態
	// ----------------------------------------------------
	// 這裡使用位元運算來記錄各個選單是否開啟，效率更高。
	// t 的位元 0 記錄 MN_ITEM
	t |= (SMenu[MN_ITEM].bActive) << 0;
	// t 的位元 7 記錄 MN_ARIGEMENT
	t |= (SMenu[MN_ARIGEMENT].bActive == TRUE) << 7;
	// t 的位元 8 記錄 MN_ABILITY
	t |= (SMenu[MN_ABILITY].bActive == TRUE) << 8;
	// t 的位元 9 記錄 MN_STATUS
	t |= (SMenu[MN_STATUS].bActive == TRUE) << 9;
	// t 的位元 10 記錄 MN_PARTY
	t |= (SMenu[MN_PARTY].bActive == TRUE) << 10;
	// t 的位元 11 記錄 MN_TACTICS
	t |= (SMenu[MN_TACTICS].bActive == TRUE) << 11;

	// 檢查是否有任何小選單（sm 陣列中的選單）開啟
	for (int i = 0; i < sm_max; i++)
		if (SMenu[sm[i]].bActive)
		{
			// 如果有小選單開啟，將 t 的位元 12 設為 1，並標記 guild 為 1
			t |= 1 << 12;
			guild = 1;
			break;
		}
		else t |= 0 << 12; // 這裡的 else 語句是多餘的，因為 t 已經初始化為 0

	// 如果 flag 為 0 且選單狀態沒有改變，則直接返回，避免不必要的重繪
	if (flag == 0 && changed == t)  return;

	// 更新上一次的選單狀態
	changed = t;

	// ----------------------------------------------------
	// 判斷哪些選單開啟，並計算 lc 和 rc 的值
	// ----------------------------------------------------
	// 判斷右側選單是否開啟（道具、隊伍或小選單）
	if (SMenu[MN_ITEM].bActive == TRUE ||
		SMenu[MN_PARTY].bActive == TRUE ||
		guild)		
		rc = 1;

	// 判斷左側選單是否開啟（戰術、配置、能力或狀態）
	if (SMenu[MN_TACTICS].bActive == TRUE ||
		SMenu[MN_ARIGEMENT].bActive == TRUE ||
		SMenu[MN_ABILITY].bActive == TRUE ||
		SMenu[MN_STATUS].bActive == TRUE)		
		lc = 2;

	// ----------------------------------------------------
	// 根據 lc 和 rc 的組合值，調整鏡頭位置
	// ----------------------------------------------------
	// 計算螢幕中心點的瓷磚座標
	POINT center_Tile;
	center_Tile.x = (GAME_SCREEN_XSIZE / 32 / 2);
	center_Tile.y = (GAME_SCREEN_YSIZE / 32 / 2);

	switch (rc + lc)
	{
	// 情況 3: 同時有左右選單開啟 (1+2=3) 或情況 0: 沒有選單開啟 (0+0=0)
	case 3:
	case 0:
		// 鏡頭中心點對準英雄角色
		g_Map.tox = Hero->position.x - center_Tile.x;
		g_Map.toy = Hero->position.y - center_Tile.y;
		break;
	// 情況 1: 只有右側選單開啟
	case 1:
		// 將鏡頭向左偏移，以騰出右側空間
		g_Map.tox = Hero->position.x - center_Tile.x / 2;
		g_Map.toy = Hero->position.y - center_Tile.y;
		break;
	// 情況 2: 只有左側選單開啟
	case 2:
		// 將鏡頭向右偏移，以騰出左側空間
		g_Map.tox = Hero->position.x - ((center_Tile.x * 2) - (center_Tile.x / 2));
		g_Map.toy = Hero->position.y - center_Tile.y;
		break;
	default: 
		return;
	}

	// 確保鏡頭位置不超出地圖邊界
	if (g_Map.tox < 0) g_Map.tox = 0;
	if (g_Map.toy < 0) g_Map.toy = 0;

	// 根據新的鏡頭位置和英雄的視野，更新遊戲畫面顯示
	SetView(Hero, Hero->sight);
}



//////////////////////// 0625 lkh ���� //////////////////////////
void StartSetView(LPCHARACTER ch)
{
	g_Map.tox = g_Map.x = ch->position.x - (GAME_SCREEN_XSIZE / 32 / 2);
	g_Map.toy = g_Map.y = ch->position.y - (GAME_SCREEN_YSIZE / 32 / 2);

	SetView(ch, ch->sight);
}



int MoveScreen(DIRECTION direction)
{
	int oldtox, oldtoy;

	oldtox = g_Map.tox;
	oldtoy = g_Map.toy;

	switch (direction)
	{
	case	DIRECTION_UP:		if (g_Map.toy > LimitMapSy)	g_Map.toy--;		break;
	case	DIRECTION_DOWN:		if (g_Map.toy < LimitMapEy)	g_Map.toy++;		break;
	case	DIRECTION_LEFT:		if (g_Map.tox > LimitMapSx)	g_Map.tox--;		break;
	case	DIRECTION_RIGHT:	if (g_Map.tox < LimitMapEx)	g_Map.tox++;		break;
	}

	if (oldtox != g_Map.tox || oldtoy != g_Map.toy)
	{
		return 1;
	}

	return 0;
}

void changemapobjectname(char* oldname, char* newname)
{
	strcpy(newname, oldname);
	CharToUpper(newname);

	if (strcmp("ICE-W02", newname) == 0)			strcpy(newname, "ICE-W01");
	else if (strcmp("ICE-W03", newname) == 0)	strcpy(newname, "ICE-W01");
	else if (strcmp("MANDUN2", newname) == 0)	strcpy(newname, "MANDUN1");
	else if (strcmp("MANDUN3", newname) == 0)	strcpy(newname, "MANDUN1");
	else if (strcmp("FIREDUN2", newname) == 0)	strcpy(newname, "FIREDUN1");
	else if (strcmp("FIREDUN3", newname) == 0)	strcpy(newname, "FIREDUN1");
	else if (strcmp("HU_VM", newname) == 0)	strcpy(newname, "SOURCE");
	else if (strcmp("GRAY", newname) == 0)	strcpy(newname, "SOURCE");

#ifdef _MAPOBJECT_DIRECTORY_CHANGE_

	if (strcmp("MA-IN", newname) == 0)				strcpy(newname, "0");
	else if (strcmp("SOURCE", newname) == 0)			strcpy(newname, "6");
	else if (strcmp("K_SUNG2", newname) == 0)			strcpy(newname, "7");
	else if (strcmp("FIREDUN1", newname) == 0)			strcpy(newname, "4");
	else if (strcmp("ICE-W01", newname) == 0)			strcpy(newname, "3");
	else if (strcmp("MANDUN1", newname) == 0)			strcpy(newname, "5");
	else if (strcmp("SUNG_TILE_00", newname) == 0)		strcpy(newname, "2");

	else													strcpy(newname, "0");

#endif 

}

void changesourcename(char* oldname, char* newname)
{
	strcpy(newname, oldname);
	CharToUpper(newname);

	if (strcmp("ICE-W02", newname) == 0)			strcpy(newname, "ICE-W01");
	else if (strcmp("ICE-W03", newname) == 0)	strcpy(newname, "ICE-W01");
	else if (strcmp("MANDUN2", newname) == 0)	strcpy(newname, "MANDUN1");
	else if (strcmp("MANDUN3", newname) == 0)	strcpy(newname, "MANDUN1");
	else if (strcmp("FIREDUN2", newname) == 0)	strcpy(newname, "FIREDUN1");
	else if (strcmp("FIREDUN3", newname) == 0)	strcpy(newname, "FIREDUN1");
}


/*
void LoadSmallMap( char *filename )
{
	FILE *fp;
	char temp[ FILENAME_MAX];
	Spr *s;

	switch( MapNumber )
	{
	case 0 :	break;
	default :	return;
	}

	sprintf( temp, "./map/%s.sml", filename );
	fp = Fopen( temp, "rb" );
	if( fp )
	{
		s = &g_Map.sp;
		fread( &s->xl, sizeof( short ), 1, fp);
		fread( &s->yl, sizeof( short ), 1, fp);
		fread( &s->ox, sizeof( short ), 1, fp);
		fread( &s->oy, sizeof( short ), 1, fp);
//		s->ox = s->oy = 0;
		fread( &s->size, sizeof( unsigned int ), 1, fp);
		s->img = NULL;
		MemAlloc( s->img, s->size );
		fread( s->img, s->size, 1, fp );
		convert565to555( s );//todo useless
		fclose(fp);
	}
}*/

#define MAX_SOURCE_NO_ 11
char mapsourcename[MAX_SOURCE_NO_][20] = {
        "MA-IN", "K_SUNG2", "MANDUN1", "FIREDUN1", "GRAY", "SUNG_TILE_00",
        "HU_VM", "ICE-W01", "SOURCE", "10", "11"
};

typedef struct {
    char sourcename[30];
    int size;
} t_sourcesize;

// Helper to convert C-string to uppercase
void StringToUpper(char* str) {
    while (*str) {
        *str = toupper((unsigned char)*str);
        str++;
    }
}

// ----------------------------------------------------------------------------
// LoadMapSource
// ----------------------------------------------------------------------------
void LoadMapSource(char* filename)
{
    char temp[FILENAME_MAX];
    char tempsource[FILENAME_MAX];
    Sint16 no; // short -> Sint16
    int i;
    Spr* s;
    Uint32 sysfilelength, size; // DWORD -> Uint32
    SDL_RWops* fp = nullptr;
    Uint32 nOff = 0;

    changesourcename(filename, tempsource);

    snprintf(temp, FILENAME_MAX, "map/%s.sou", tempsource); // Removed ./
    fp = SDL_RWFromFile(temp, "rb");
    if (fp == NULL) {
        SDL_Log("LoadMapSource: Failed to open %s", temp);
        return;
    }

    // Read number of sprites
    if (SDL_RWread(fp, &no, sizeof(Sint16), 1) != 1) {
        SDL_Log("LoadMapSource: Failed to read header count");
        SDL_RWclose(fp);
        return;
    }

    // Skip offset table (no * 4 bytes)
    SDL_RWseek(fp, no * 4, RW_SEEK_CUR);

    // Calculate data size
    Sint64 fileSize = SDL_RWsize(fp);
    sysfilelength = (Uint32)(fileSize - (no * 4 * 2) - 2);
    // Note: The formula (no * 4 * 2) + 2 seems to account for:
    // 2 bytes (count) + no * 4 bytes (offset table) + maybe another table?
    // We stick to the original logic: file_length - overhead.

    TotalTileNo = no;

    // Reallocate SouBuf
    // Assuming MemAlloc handles reallocation or freeing old memory
    // If not, explicitly free SouBuf first if needed.
    // MemFree(SouBuf);
    MemAlloc(SouBuf, sysfilelength);

    for (i = 0; i < no; i++)
    {
        // Read block size
        SDL_RWread(fp, &size, sizeof(Uint32), 1);

        s = &Sou[i];

        // Read Spr header
        SDL_RWread(fp, &s->xl, sizeof(Sint16), 1);
        SDL_RWread(fp, &s->yl, sizeof(Sint16), 1);

        if (s->xl == -1) // Raw Tile
        {
            s->size = TILE_SIZE * TILE_SIZE * 2;
            s->ox = 0;
            s->oy = 0;

            // Check buffer bounds
            if (nOff + s->size > sysfilelength) {
                SDL_Log("LoadMapSource: Buffer overflow at index %d", i);
                break;
            }

            SDL_RWread(fp, SouBuf + nOff, 1, s->size);
            s->img = (char*)SouBuf + nOff;
            convert565to555RawTile(s->img);
        }
        else // Sprite
        {
            SDL_RWread(fp, &s->ox, sizeof(Sint16), 1);
            SDL_RWread(fp, &s->oy, sizeof(Sint16), 1);
            SDL_RWread(fp, &s->size, sizeof(Uint32), 1); // This size is data size

            if (nOff + s->size > sysfilelength) {
                SDL_Log("LoadMapSource: Buffer overflow at index %d", i);
                break;
            }

            SDL_RWread(fp, SouBuf + nOff, 1, s->size);
            s->img = (char*)SouBuf + nOff;
            convert565to555(s);
        }

        nOff += size; // Advance by block size from file

        // Keep UI responsive
        if ((i % 300) == 0) SDL_PumpEvents(); // Replaces HandleRunning
    }

    SDL_RWclose(fp);
}

// ----------------------------------------------------------------------------
// FreeMapSource
// ----------------------------------------------------------------------------
void FreeMapSource(void)
{
    // MemFree(g_Map.sp.img); // Ensure g_Map is accessible
    MemFree(SouBuf);

    for (int i = 0; i < 3600; i++)
    {
        // memset(&Sou[i], 0, sizeof(Spr)); // Use if Spr is POD
        Sou[i].img = NULL;
        Sou[i].size = 0;
        Sou[i].xl = 0;
        Sou[i].yl = 0;
    }
}

// ----------------------------------------------------------------------------
// ReturnSizeOfSource_bin
// ----------------------------------------------------------------------------
int ReturnSizeOfSource_bin(char* source2name)
{
    int totalsize = 0;

    // Local copy to avoid modifying const string if passed
    char tempName[256];
    strncpy(tempName, source2name, 255);
    StringToUpper(tempName);

    SDL_RWops* fp = SDL_RWFromFile("data/sizeofsou2.bin", "rb");
    if (fp == NULL) return 0;

    t_sourcesize sousize;

    while (SDL_RWread(fp, &sousize, sizeof(t_sourcesize), 1) > 0)
    {
        // Note: Reading struct directly is risky across platforms due to padding.
        // If t_sourcesize is simple {char[30], int}, it's usually fine,
        // but explicit field reading is safer if the file format is strict.

        if (strcmp(tempName, sousize.sourcename) == 0) {
            totalsize = sousize.size;
            break;
        }
    }

    SDL_RWclose(fp);
    return totalsize;
}

// ----------------------------------------------------------------------------
// ReturnSizeOfSource2
// ----------------------------------------------------------------------------
DWORD ReturnSizeOfSource2(char* source2name)
{
    char tempsourcename[FILENAME_MAX];
    SDL_RWops* sou2fp = nullptr;
    SDL_RWops* fp[MAX_SOURCE_NO_] = { nullptr };
    // SOU2 sou2; // Assuming struct definition exists: { short tile_no; short source_no; }
    Uint32 totalsize = 0, size = 0;
    Uint32 Off;

    sou2fp = SDL_RWFromFile(source2name, "rb");
    if (sou2fp == NULL) return 0;

    int i;
    for (i = 0; i < MAX_SOURCE_NO_; i++)
    {
        snprintf(tempsourcename, FILENAME_MAX, "map/%s.sou", mapsourcename[i]);
        fp[i] = SDL_RWFromFile(tempsourcename, "rb");
        // Note: It's possible some files don't exist, we should check fp[i] before use
    }

    Uint32 fs, no;

    // Read Header
    SDL_RWread(sou2fp, &fs, 4, 1);
    SDL_RWread(sou2fp, &no, 4, 1);
    SDL_RWseek(sou2fp, 6, RW_SEEK_CUR);

    for (i = 0; i < (int)no; i++)
    {
        Sint16 tt, ts;

        SDL_RWread(sou2fp, &tt, sizeof(Sint16), 1);
        SDL_RWread(sou2fp, &ts, sizeof(Sint16), 1);

        int tile_no = tt;
        int source_no = ts - 1;

        if (tile_no < 0 || tile_no >= 3600) continue;
        if (source_no < 0 || source_no >= MAX_SOURCE_NO_) continue;

        SDL_RWops* sourceFp = fp[source_no];
        if (!sourceFp) continue;

        // Seek to offset table: 2 (count) + tile_no * 4 (offset entry)
        // Original logic: 4 * tile + 2. This assumes offset table starts at byte 2?
        // Wait, LoadMapSource skipped `no * 4` bytes after the count.
        // Let's assume the original logic `4 * tile + 2` is correct for .sou format.
        SDL_RWseek(sourceFp, 4 * tile_no + 2, RW_SEEK_SET);

        SDL_RWread(sourceFp, &Off, sizeof(Uint32), 1);

        SDL_RWseek(sourceFp, Off, RW_SEEK_SET);
        SDL_RWread(sourceFp, &size, sizeof(Uint32), 1);

        totalsize += size;
    }

    SDL_RWclose(sou2fp);
    for (i = 0; i < MAX_SOURCE_NO_; i++) {
        if (fp[i]) SDL_RWclose(fp[i]);
    }

    return totalsize;
}

// ----------------------------------------------------------------------------
// MakeSizeofSOU2 (Likely won't work on Android/Read-only systems)
// ----------------------------------------------------------------------------
void MakeSizeofSOU2(void)
{
    // Need external definitions for SysInfo, MapInfo, etc.
    /*
    if (SysInfo.makesou2size == 0) return;

    SDL_RWops* fp = SDL_RWFromFile("data/sizeofsou2.bin", "wb");
    if (fp == NULL) {
        SDL_Log("MakeSizeofSOU2: Cannot write to data/sizeofsou2.bin");
        return;
    }

    t_sourcesize writesousize;
    char temp[FILENAME_MAX];
    int i, size;

    for (i = 0; i < MAX_MAP_; i++)
    {
        snprintf(temp, FILENAME_MAX, "map/%s.sou2", MapInfo[i].mapfile);
        StringToUpper(temp);
        size = ReturnSizeOfSource2(temp);

        strncpy(writesousize.sourcename, temp, 29);
        writesousize.size = size;

        SDL_RWwrite(fp, &writesousize, sizeof(t_sourcesize), 1);
        SDL_Log("Processed Map %d/%d: %s Size: %d", i + 1, MAX_MAP_, temp, size);
    }

    SDL_RWclose(fp);
    */
    SDL_Log("MakeSizeofSOU2 skipped (write permission check required)");
}


// ----------------------------------------------------------------------------
// LoadMapSource2
// ----------------------------------------------------------------------------
void LoadMapSource2(char* mapname)
{
    char temp[FILENAME_MAX]={0};
    Uint32 no;
    int i;
    Spr* s;
    Uint32 size;
    SDL_RWops* fp[MAX_SOURCE_NO_] = { nullptr };
    SDL_RWops* sou2fp = nullptr;
    Uint32 nOff = 0;
    char tempsourcename[FILENAME_MAX];
    Uint32 filesize;

    snprintf(temp, FILENAME_MAX, "map/%s.sou2", mapname);
    //StringToUpper(temp);

    sou2fp = SDL_RWFromFile(temp, "rb");
    if (sou2fp == NULL)
    {
        SDL_Log("LoadMapSource2: .sou2 not found, falling back to LoadMapSource");
        LoadMapSource(mapname);
        return;
    }

    // Open all source library files
    for (i = 0; i < MAX_SOURCE_NO_; i++)
    {
        snprintf(tempsourcename, FILENAME_MAX, "map/%s.sou", mapsourcename[i]);
        fp[i] = SDL_RWFromFile(tempsourcename, "rb");
    }

    // Determine memory needed
    // Try cache first
    // filesize = ReturnSizeOfSource_bin(temp);
    // if (filesize == 0)
    filesize = ReturnSizeOfSource2(temp);

    Uint32 fs;
    Uint32 offset = 0;

    // Read SOU2 header
    SDL_RWread(sou2fp, &fs, sizeof(Uint32), 1);
    SDL_RWread(sou2fp, &no, sizeof(Uint32), 1);
    SDL_RWseek(sou2fp, 6, RW_SEEK_CUR);

    // Allocate memory
    MemAlloc(SouBuf, filesize);

    for (i = 0; i < (int)no; i++)
    {
        Sint16 tt, ts;

        SDL_RWread(sou2fp, &tt, sizeof(Sint16), 1);
        SDL_RWread(sou2fp, &ts, sizeof(Sint16), 1);

        int tile_no = tt;
        int source_no = ts - 1;

        s = &Sou[i];

        if (tile_no < 0 || tile_no >= 3600 ||
            source_no < 0 || source_no >= MAX_SOURCE_NO_ ||
            fp[source_no] == nullptr)
        {
            s->xl = 0; s->img = NULL; continue;
        }

        if (tile_no == 0) continue;

        SDL_RWops* sourceFp = fp[source_no];

        // Read offset from .sou file
        SDL_RWseek(sourceFp, 4 * tile_no + 2, RW_SEEK_SET);
        SDL_RWread(sourceFp, &offset, sizeof(Uint32), 1);

        // Jump to sprite data
        SDL_RWseek(sourceFp, offset, RW_SEEK_SET);

        // Read block size
        SDL_RWread(sourceFp, &size, sizeof(Uint32), 1);

        // Read sprite header
        SDL_RWread(sourceFp, &s->xl, sizeof(Sint16), 1);
        SDL_RWread(sourceFp, &s->yl, sizeof(Sint16), 1);

        // Check buffer bounds
        if (nOff + size > filesize) {
            SDL_Log("LoadMapSource2: Buffer overflow at index %d", i);
            s->xl = 0; s->img = NULL;
            break;
        }

        if (s->xl == -1) // Raw Tile
        {
            s->size = TILE_SIZE * TILE_SIZE * 2;
            s->ox = 0;
            s->oy = 0;

            // Read raw data using s->size (fixed)
            // Note: The block size 'size' read earlier usually matches or includes padding
            // We read into SouBuf based on the fixed raw tile size
            SDL_RWread(sourceFp, SouBuf + nOff, 1, s->size);
            s->img = (char*)SouBuf + nOff;
            convert565to555RawTile(s->img);
        }
        else // Sprite
        {
            SDL_RWread(sourceFp, &s->ox, sizeof(Sint16), 1);
            SDL_RWread(sourceFp, &s->oy, sizeof(Sint16), 1);
            SDL_RWread(sourceFp, &s->size, sizeof(Uint32), 1);

            SDL_RWread(sourceFp, SouBuf + nOff, 1, s->size);
            s->img = (char*)SouBuf + nOff;
            convert565to555(s);
        }

        nOff += size; // Advance main buffer offset by file block size

        if ((i % 300) == 0) SDL_PumpEvents();
    }
    TotalTileNo = i;

    SDL_RWclose(sou2fp);

    for (i = 0; i < MAX_SOURCE_NO_; i++)
    {
        if (fp[i]) SDL_RWclose(fp[i]);
    }
}

void FreeMapSource2(void)
{
    // MemFree(g_Map.sp.img);
    MemFree(SouBuf);

    // Optional: clear Spr array pointers
    for(int i=0; i<3600; ++i) {
        Sou[i].img = nullptr;
    }
}


/////////////////////////////////////////////////////////////////////////////


int ReturnEventNo(int mx, int my)
{
	LPEVENTLIST	lpEventList;

	if (TileMap[mx][my].attr_map)
	{
		lpEventList = FindEventList(&g_EventListHeaderMap, (WORD)mx, (WORD)my);
		if (lpEventList == NULL) return -1;
		return lpEventList->index;
	}

	return -1;
}


int ChangeEventNo(int mx, int my, int eventno)
{
	LPEVENTLIST	lpEventList;

	if (TileMap[mx][my].attr_map)
	{
		lpEventList = FindEventList(&g_EventListHeaderRoom, (WORD)mx, (WORD)my);
		lpEventList->index = eventno;

		return TRUE;
	}

	return FALSE;
}


////////////////////////////////////////////////////////////////////////////
/////   Event��  �߻��Ǵ°��� check�Ѵ�. 

int CheckEventWith_NPC(LPCHARACTER hero, LPCHARACTER oppo)
{
	DIRECTION dir = (DIRECTION)0;


	if (hero->eventdelay)
	{
		PlayListAutoSounds(82, 0, 0, 0);
		return 1;
	}

	if (IsDead(oppo)) return 0; // �׾��ִ°Ͱ��� Event�� �߻����� �ʴ´�. 

	switch (oppo->type)
	{
	case SPRITETYPE_MONSTER:
	case SPRITETYPE_NPC:
	{
		if (oppo->sealstone)
		{
		}
		else
		{
			if (oppo->sprno != 100 && oppo->sprno != 101)
			{
				ChangeDirection(&dir, oppo->x, oppo->y, hero->x, hero->y);
				oppo->todir = dir;
				ChangeDirection(&dir, hero->x, hero->y, oppo->x, oppo->y);
				hero->todir = dir;
			}

			SendEvent(EVENT_TYPE_NPC, oppo->id);
			hero->eventdelay = 17;

		}
		return 1;
	}break;
	}

	return 0;
}



int CheckEventAttrTile(void)
{
	int mx = Hero->x / TILE_SIZE;
	int my = Hero->y / TILE_SIZE;
	int eventno;
	static int OntheEventTile;
	if (TileMap[mx][my].attr_map)
	{
		if (OntheEventTile == 0)
		{
			OntheEventTile = 1;
			eventno = ReturnEventNo(mx, my);
			if (eventno != -1)
			{
				//				JustMsg( "int CheckEventAttr( void ) %d", eventno );
				SendEvent(EVENT_TYPE_MAPTILE, eventno);

				return 1;
			}
		}
	}
	else
	{
		OntheEventTile = 0;
	}

	return 0;
}


////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

Spr SmallMapBackSpr;

void LoadSmallMapBack(void)
{
    // [Android 修改] 1. 路徑修正：移除開頭的 ./
    const char* path = "map/add.csp";

    // [Android 修改] 2. 開檔：改用 SDL_RWFromFile
    SDL_RWops* fp = SDL_RWFromFile(path, "rb");

    if (fp == NULL) {
        SDL_Log("Error: Failed to open %s. SDL Error: %s", path, SDL_GetError());
        return;
    }

    Spr* s = &SmallMapBackSpr;

    // [Android 修改] 3. 讀取：改用 SDL_RWread 取代 fread
    // 原本: fread(&s->xl, sizeof(short), 1, fp);
    SDL_RWread(fp, &s->xl, sizeof(short), 1);
    SDL_RWread(fp, &s->yl, sizeof(short), 1);
    SDL_RWread(fp, &s->ox, sizeof(short), 1);
    SDL_RWread(fp, &s->oy, sizeof(short), 1);

    // 邏輯保持不變
    s->ox = s->oy = 0;

    // 讀取圖片大小
    SDL_RWread(fp, &s->size, sizeof(unsigned int), 1);

    // 記憶體分配 (維持原樣，假設 MemAlloc 是您的自定義宏或函式)
    s->img = NULL;
    MemAlloc(s->img, s->size);

    if (s->img != NULL) {
        // 讀取圖片資料
        SDL_RWread(fp, s->img, s->size, 1);
    } else {
        SDL_Log("Error: Memory allocation failed for %s", path);
    }

    // 格式轉換 (維持原樣)
    convert565to555(s); //Todo finished

    // [Android 修改] 4. 關檔：改用 SDL_RWclose
    SDL_RWclose(fp);

    // [Android 修改] 5. 傳遞修正後的路徑
    // 原本: "./map/add.csp"
    g_SDLInfo.CreateTextureFromSingleSpr_CPP(path, s);
}

void PutSmallMapBack(int x, int y)
{
	PutCompressedImage(x, y, &SmallMapBackSpr);
}

void displaySmallmapObject(SDL_Surface* fullMapSurface)
{
	// b. 【移植 smallmap_CalcOrder】收集所有需要排序的物件和前景圖塊
	SDL_Log("正在收集並排序物件...");
	std::vector<ORDERTABLE> sortedList;
	sortedList.reserve(TotalMapObject);

	// b.1 收集物件
	int c = 0, d = 0;
	for (int i = 0; i < TotalMapObject; i++) {
		//LPMAPOBJECT mo = &Mo[i];
		// 根據您的 smallmap_CalcOrder 邏輯，只收集特定屬性的物件 
		if (Mo[i].attr == OBJECTATTR_BACK_ABOVE ||
			Mo[i].attr == OBJECTATTR_FRONT_ABOVE 
			/*|| Mo[i].attr == OBJECTATTR_BACK_BELOW*/ )
		{ // 假設 OBJECTATTR_... 對應 1,2,3
			ORDERTABLE o;
			o.wType = SPRITETYPE_OBJECT;
			o.x = Mo[i].x;
			o.y = Mo[i].y;
			o.offset = Mo[i].y;
			o.rgb = i; // rgb 是用來標記物件的索引

			o.height = 0;
			o.lpvData = (LPVOID)(&Mo[i]);
			o.show = 1;
			o.sort = 1;
			sortedList.push_back(o);
		}
	}
	c = sortedList.size();

	// b.3 【優化】使用 std::sort 來取代您舊有的 O(n²) 氣泡排序
	std::sort(sortedList.begin(), sortedList.end(), [](const ORDERTABLE& a, const ORDERTABLE& b) {
		return a.offset < b.offset;
		});

	SDL_Log("共收集並排序了 %zu 個[物件]圖塊。", c);

	//    我們現在遍歷這個排好序的列表，來繪製物件和前景
	SDL_Log("開始繪製物件...");
	for (const auto& o : sortedList) {
		if (o.wType == SPRITETYPE_OBJECT) // SPRITETYPE_OBJECT
		{
			LPMAPOBJECT mo = (LPMAPOBJECT)o.lpvData;
			Spr* sp = &MoImage[mo->id - 1].spr[0];
			// 我們使用絕對座標，不再需要減去 Mapx, Mapy
			SDL_Surface* MoSurface = g_SDLInfo.CreateSurfaceFromSpr(sp, SDL_MapRGB(fullMapSurface->format, 0, 248, 0));
			if (MoSurface)
			{
				SDL_SetColorKey(MoSurface, SDL_TRUE, SDL_MapRGB(MoSurface->format, 0, 248, 0)); // 設置黑色為透明色
				SDL_Rect destRect = { mo->x + mo->offx - sp->ox, mo->y + mo->offy - sp->oy , sp->xl, sp->yl };
				SDL_BlitSurface(MoSurface, nullptr, fullMapSurface, &destRect);
				if (1)
				{
					std::string dir = "./" + std::string(MapName) + "_MapObject";
					std::__fs::filesystem::create_directory(dir); dir += "/";
					std::string file = dir + std::to_string(mo->id) + "-" + std::to_string(mo->curframe) + ".bmp";
					SDL_SaveBMP(MoSurface, file.c_str());
				}
				SDL_FreeSurface(MoSurface);
			}
		}
	}
	SDL_Log("物件繪製完成。");
}

/**
 * @brief 【核心優化】一個全新的、安全的、用 C++ 寫的 PutTile 函式。
 * 它取代了您舊有的 PutTile, PutTileCliping 和 PutTileNotCliping 的所有功能。
 * @param destSurface 目標繪圖畫布 (可以是任何大小的 Surface)。
 * @param x 要繪製的目標 X 座標。
 * @param y 要繪製的目標 Y 座標。
 * @param Stmp 指向來源圖像資料的指標 (未壓縮的 32x32 像素)。
 */
void PutTile_Raw(SDL_Surface* destSurface, int x, int y, void* Stmp)
{
	if (!destSurface || !Stmp) return;

	// 1. 定義來源圖像的尺寸
	const int TILE_WIDTH = 32;
	const int TILE_HEIGHT = 32;

	// 2. 邊界檢查與裁切 (Clipping)
	//    我們不再使用寫死的 SCREEN_WIDTH，而是使用目標畫布的真實尺寸。
	int dest_w = destSurface->w;
	int dest_h = destSurface->h;

	if (x >= dest_w || y >= dest_h || x + TILE_WIDTH <= 0 || y + TILE_HEIGHT <= 0) {
		return; // 完全在畫布之外
	}

	int src_x = 0, src_y = 0;
	int draw_w = TILE_WIDTH, draw_h = TILE_HEIGHT;

	if (x < 0) {
		src_x = -x;
		draw_w += x;
		x = 0;
	}
	if (y < 0) {
		src_y = -y;
		draw_h += y;
		y = 0;
	}
	if (x + draw_w > dest_w) {
		draw_w = dest_w - x;
	}
	if (y + draw_h > dest_h) {
		draw_h = dest_h - y;
	}

	if (draw_w <= 0 || draw_h <= 0) return;

	// 3. 高效能的記憶體複製
	//    我們用一個簡單的 for 迴圈 + memcpy，來取代複雜的組合語言。
	//    現代編譯器會將這段程式碼優化得非常好。
	Uint16* src_pixels = (Uint16*)Stmp;
	Uint16* dest_pixels = (Uint16*)destSurface->pixels;
	int dest_pitch_in_pixels = destSurface->pitch / sizeof(Uint16);

	for (int i = 0; i < draw_h; ++i) {
		// 計算來源和目標當前行的起始記憶體位址
		void* src_addr = &src_pixels[(src_y + i) * TILE_WIDTH + src_x];
		void* dest_addr = &dest_pixels[(y + i) * dest_pitch_in_pixels + x];

		// 使用 memcpy 來快速複製一整行的像素
		memcpy(dest_addr, src_addr, draw_w * sizeof(Uint16));
	}
}

/**
 * @brief 【核心】渲染並儲存一張高品質的地圖縮圖。
 * @param output_filename 您希望儲存的 .bmp 檔案的路徑 (例如 "minimap.bmp")。
 * @param minimap_size 最終縮圖的尺寸 (例如 200，代表 200x200)。
 * @return 成功返回 true，失敗返回 false。
 */
bool OutputSmallMap_Reloo()
{
	if (TotalMapObject <= 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "錯誤：沒有任何地圖物件被載入 (TotalMapObject is 0)！");
		return false;
	}

	int minimap_size = 4096;
	std::string output_filename = "./" + std::string(MapName) + ".bmp";
	// --- 1. 計算並檢查記憶體需求 ---
	long long full_width = (long long)g_Map.file.wWidth * TILE_SIZE;
	long long full_height = (long long)g_Map.file.wHeight * TILE_SIZE;
	// 假設使用 16-bit (2 bytes) 顏色
	long long required_memory = full_width * full_height * 2;

	SDL_Log("準備渲染完整地圖...");
	SDL_Log("完整尺寸: %lld x %lld 像素", full_width, full_height);
	SDL_Log("預計需要記憶體: %.2f MB", (double)required_memory / (1024 * 1024));

	if (required_memory > 2000000000) { // 設置一個安全上限，例如 2GB
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "錯誤：地圖尺寸過大，需要記憶體超過 2GB 上限！");
		return false;
	}

	// --- 2. 建立足以容納整個地圖的巨大 Surface ---
	SDL_Surface* fullMapSurface = SDL_CreateRGBSurfaceWithFormat(0, full_width, full_height, 16, SDL_PIXELFORMAT_RGB565); // RGB565 格式
	if (!fullMapSurface) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "錯誤：無法建立巨大地圖 Surface: %s", SDL_GetError());
		return false;
	}
	SDL_Log("巨大地圖 Surface 建立成功。");


	char* oldBuffer = g_DestBackBuf;
	int oldDx = dDxSize;
	dDxSize = fullMapSurface->pitch;
	g_DestBackBuf = (char*)fullMapSurface->pixels;

	GAME_SCREEN_XSIZE = SCREEN_WIDTH = full_width;
	GAME_SCREEN_YSIZE = SCREEN_HEIGHT = full_height;
	int Oldg_MapX = g_Map.x;
	int Oldg_MapY = g_Map.y;
	g_Map.x = 0;
	g_Map.y = 0;
	Mapx = g_Map.x * TILE_SIZE;
	Mapy = g_Map.y * TILE_SIZE;
	// --- 3. 將所有圖塊繪製到這個巨大 Surface 上 ---
	SDL_Log("開始繪製縮圖 Surface...");

	g_OrderInfo.count = 0;

	CalcOrder();

	DisplaySpriteUnderTile();
	MapDisplay(0);
	displaySmallmapObject(fullMapSurface);
	DisplaySpriteBottom();
	MapDisplay(2);
	
	DisplaySprite();
	MapDisplay(1);
	DisplaySpriteTrans();
	DisplaySpriteCeiling();
	//RainAnimationOutput();
	//SnowAnimationOutput();
	//MapBright();


	g_DestBackBuf = oldBuffer; // 恢復原來的緩衝區
	dDxSize = oldDx; // 恢復原來的 dDxSize
	GAME_SCREEN_XSIZE = SCREEN_WIDTH = 1280;
	GAME_SCREEN_YSIZE = SCREEN_HEIGHT = 720;
	g_Map.x = Oldg_MapX;
	g_Map.y = Oldg_MapY;
	Mapx = g_Map.x * TILE_SIZE;
	Mapy = g_Map.y * TILE_SIZE;

	// --- 4. 建立一個較小的 Surface，用來存放縮圖 ---
	SDL_Surface* minimapSurface = SDL_CreateRGBSurfaceWithFormat(0, minimap_size, minimap_size, 16, SDL_PIXELFORMAT_RGB565);
	if (!minimapSurface) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "錯誤：無法建立縮圖 Surface: %s", SDL_GetError());
		SDL_FreeSurface(fullMapSurface);
		return false;
	}
	SDL_Log("縮圖 Surface 建立成功。");

	// --- 5. 使用 SDL_BlitScaled 將巨大 Surface 縮小並複製到縮圖 Surface 上 ---
	SDL_Log("正在縮小圖像...");
	if (SDL_BlitScaled(fullMapSurface, NULL, minimapSurface, NULL) != 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "錯誤：縮小圖像失敗: %s", SDL_GetError());
		SDL_FreeSurface(fullMapSurface);
		SDL_FreeSurface(minimapSurface);
		return false;
	}
	SDL_Log("圖像縮小完成。");
	//SDL_SaveBMP(fullMapSurface, "./big.bmp");
	// --- 6. 將最終的縮圖 Surface 儲存為 .bmp 檔案 ---
	SDL_Log("正在儲存檔案至 %s...", output_filename.c_str());
	if (SDL_SaveBMP(minimapSurface, output_filename.c_str()) != 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "錯誤：儲存 BMP 檔案失敗: %s", SDL_GetError());
		SDL_FreeSurface(fullMapSurface);
		SDL_FreeSurface(minimapSurface);
		return false;
	}
	SDL_Log("高畫質小地圖生成成功！");

	// --- 7. 清理所有我們建立的 Surface ---
	SDL_FreeSurface(fullMapSurface);
	SDL_FreeSurface(minimapSurface);

	return true;
}
BYTE* LoadMaskBuf, * LoadMaskBackBuf;
BYTE  LoadMaskFlag;

void LoadingLoadMaskTable(void)
{
	int i = 0, j = 0;

	MemFree(LoadMaskBuf);
	MemFree(LoadMaskBackBuf);
	MemAlloc(LoadMaskBuf, SCREEN_WIDTH * SCREEN_HEIGHT);

	for (i = 0; i < SCREEN_HEIGHT; i++)
	{
		for (j = 0; j < SCREEN_WIDTH; j++)
		{
			int t;
			if (j > SCREEN_WIDTH / 2)
			{
				t = (j * 256 / SCREEN_WIDTH);
				if (t > 1 && t < 252) t += (rand() % 3);
			}
			else
			{
				t = ((SCREEN_WIDTH - j) * 256 / SCREEN_WIDTH);
				if (t > 1 && t < 252) t += (rand() % 3);
			}

			*(LoadMaskBuf + j + SCREEN_WIDTH * i) = t;
		}
	}

	MemAlloc(LoadMaskBackBuf, dDxSize * SCREEN_HEIGHT);

	g_DestBackBuf = (char*)LoadMaskBackBuf;
	for (i = 0; i < NewOrderC; i++) { neworder[i].show = 1;  neworder[i].sort = 1; }
	g_OrderInfo.count = g_OldOrderInfoCount;
	for (i = 0; i < g_OrderInfo.count; i++) { g_OrderInfo.order[i].show = 1; }

	g_OrderInfo.count = 0;

	CalcOrder();

	DisplaySpriteUnderTile();
	MapDisplay(0);
	DisplaySpriteBottom();
	MapDisplay(2);
	DisplaySprite();
	MapDisplay(1);
	DisplaySpriteTrans();
	DisplaySpriteCeiling();
	RainAnimationOutput();
	SnowAnimationOutput();
	MapBright();
	MenuDisplay();     //�޴� ��������� �浹üũMenuChecking();     //�޴� ��������� �浹üũMenuDisplay();      //�޴��� Ȱ�����θ� �Ǵ��� ����ϴ� �Լ�	


	WORD* t;
	for (j = 31; j >= 14; j--)
	{
		g_DestBackBuf = (char*)g_SDLInfo.cpuRenderSurface->pixels;
		memcpy(g_DestBackBuf, LoadMaskBackBuf, dDxSize * SCREEN_HEIGHT);

		t = (WORD*)g_DestBackBuf;
		for (i = 0; i < (int)(dDxSize * SCREEN_HEIGHT); i += 2, t++) {
			*t = rgbTable[j * 65536 + *t];
		}

		// 將處理後的像素數據更新到 SDL 紋理
		// g_ScreenTexture 的像素格式應與 transformed_pixels (unsigned short) 和 rgbTable 輸出匹配
		SDL_UpdateTexture(g_SDLInfo.streamingTexture, NULL, g_DestBackBuf, SCREEN_WIDTH * sizeof(unsigned short));

		// 清除渲染器
		SDL_RenderClear(g_SDLInfo.renderer);
		// 將紋理複製到渲染器 (繪製整個畫面)
		SDL_RenderCopy(g_SDLInfo.renderer, g_SDLInfo.streamingTexture, NULL, NULL);
		// 呈現畫面
		SDL_RenderPresent(g_SDLInfo.renderer);

		SDL_Delay(30); // 控制過渡動畫速度
	}

	t = (WORD*)LoadMaskBackBuf;
	for (i = 0; i < (int)(dDxSize * SCREEN_HEIGHT); i += 2, t++)	*t = rgbTable[j * 65536 + *t];

	for (i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
		*(LoadMaskBuf + i) = *(LoadMaskBuf + i) >> 3;

	LoadMaskFlag = 30;

	g_OrderInfo.count = 0;
}

void ViewLoadMask(void)
{
	if (LoadMaskFlag == 0) return;

	WORD* buf, * backbuf;
	BYTE* lm;
	int i, j;

	for (i = 0; i < SCREEN_HEIGHT; i++)
	{
		backbuf = (WORD*)(LoadMaskBackBuf + dDxSize * i);
		buf = (WORD*)(g_DestBackBuf + dDxSize * i);
		lm = LoadMaskBuf + SCREEN_WIDTH * i;
		for (j = 0; j < SCREEN_WIDTH; j++)
		{
			if (*lm)
			{
				(*lm) -= 1;
				*buf = *backbuf;
			}
			buf++;
			lm++;
			backbuf++;
		}
	}

	LoadMaskFlag--;

	if (LoadMaskFlag == 0)
	{
		MemFree(LoadMaskBackBuf);
		MemFree(LoadMaskBuf);
	}
}


void MaskMap(int x, int y, Spr* sp)
{
	int i, j;

	int sx = x - sp->ox;
	int sy = y - sp->oy;
	int ex = sx + sp->xl + TILE_SIZE - 1;
	int ey = sy + sp->yl + TILE_SIZE - 1;

	ex += ((ex - sx) / 2);

	sx /= TILE_SIZE;
	sy /= TILE_SIZE;
	ex /= TILE_SIZE;
	ey /= TILE_SIZE;

	if (sx < 0) sx = 0;
	if (sy < 0) sy = 0;
	if (ex >= g_Map.file.wWidth) ex = g_Map.file.wWidth;
	if (ey >= g_Map.file.wHeight) ey = g_Map.file.wHeight;


	for (i = sx; i < ex; i++)
		for (j = sy; j < ey; j++)
		{
			int front = TileMap[i][j].front;
			if (front)
				//			if(  TileMap[ i][ j].frontoffset * TILE_SIZE >= y )
			{
				if (Sou[front].xl == -1)		PutTile(i * TILE_SIZE - Mapx, j * TILE_SIZE - Mapy, &Sou[front], Sou[front].img, g_DestBackBuf);
				else							PutCompressedImage(i * TILE_SIZE - Mapx, j * TILE_SIZE - Mapy, &Sou[front]);
			}
		}
}

void MaskMapItem(int x, int y, int offy, Spr* sp)
{
	int i, j;

	int sx = x - sp->ox;
	int sy = y - sp->oy;
	int ex = sx + sp->xl + TILE_SIZE - 1;
	int ey = sy + sp->yl + TILE_SIZE - 1;

	//ex += (( ex - sx )/2); // �׸��ڸ� ���� X���� Ȯ��.

	sx /= TILE_SIZE;
	sy /= TILE_SIZE;
	ex /= TILE_SIZE;
	ey /= TILE_SIZE;

	if (sx < 0) sx = 0;
	if (sy < 0) sy = 0;
	if (ex >= g_Map.file.wWidth) ex = g_Map.file.wWidth;
	if (ey >= g_Map.file.wHeight) ey = g_Map.file.wHeight;


	for (i = sx; i < ex; i++)
		for (j = sy; j < ey; j++)
		{
			int front = TileMap[i][j].front;
			if (front)
				if ((int)TileMap[i][j].frontoffset * TILE_SIZE >= offy)
				{
					if (Sou[front].xl == -1)		PutTile(i * TILE_SIZE - Mapx, j * TILE_SIZE - Mapy, &Sou[front], Sou[front].img, g_DestBackBuf);
					else							PutCompressedImage(i * TILE_SIZE - Mapx, j * TILE_SIZE - Mapy, &Sou[front]);
				}
		}
}

void MaskMapEffect(int x, int y, int height, Spr* sp)
{
	int i, j;

	int sx = x - sp->ox;
	int sy = y - sp->oy - height;
	int ex = sx + sp->xl + TILE_SIZE - 1;
	int ey = sy + sp->yl + TILE_SIZE - 1;

	//	if( !TileMap[ x/TILE_SIZE][ y/TILE_SIZE].front )
	//	{
	//		return;
	//	}

	//	int offset = TileMap[ x/TILE_SIZE][ y/TILE_SIZE].frontoffset;

	//	ex += (( ex - sx )/2);

	sx /= TILE_SIZE;
	sy /= TILE_SIZE;
	ex /= TILE_SIZE;
	ey /= TILE_SIZE;

	if (sx < 0) sx = 0;
	if (sy < 0) sy = 0;
	if (ex >= g_Map.file.wWidth) ex = g_Map.file.wWidth;
	if (ey >= g_Map.file.wHeight) ey = g_Map.file.wHeight;

	//	my += Mapy;

	for (i = sx; i < ex; i++)
		for (j = sy; j < ey; j++)
		{
			int front = TileMap[i][j].front;
			if (front)
				if ((int)TileMap[i][j].frontoffset * TILE_SIZE >= y)
				{
					if (Sou[front].xl == -1)		PutTile(i * TILE_SIZE - Mapx, j * TILE_SIZE - Mapy, &Sou[front], Sou[front].img, g_DestBackBuf);
					else							PutCompressedImage(i * TILE_SIZE - Mapx, j * TILE_SIZE - Mapy, &Sou[front]);
				}
		}
}


















void ViewRoofData(void)
{
	if (tool_ViewAttrRoofOnOff == 0) return;

	int sx, sy;
	int tmx = Mox / TILE_SIZE;
	int tmy = Moy / TILE_SIZE;
	static int mx, my;

	static int tindex = -1;
	static int to;
	to = !to;

	if (TileMap[mx][my].attr_room)
	{
		if (TileMap[mx][my].attr_room == 1)
		{
			LPEVENTLIST		lpEventList;
			LPROOFGROUP		lpRoofGroup;
			LPROOF			lpRoof;

			lpEventList = FindEventList(&g_EventListHeaderRoom, (WORD)mx, (WORD)my);
			if (lpEventList != NULL)
			{
				lpRoofGroup = FindRoofGroup(&g_RoofHeader, lpEventList->index);
				if (lpRoofGroup != NULL)
				{
					lpRoof = lpRoofGroup->lpFirst;
					while (lpRoof)
					{
						sx = lpRoof->x * TILE_SIZE;
						sy = lpRoof->y * TILE_SIZE;
						Box(sx + 4, sy + 4, sx + TILE_SIZE - 8, sy + TILE_SIZE - 8, RGB16(0, 255, 0));
						Box(sx + 5, sy + 5, sx + TILE_SIZE - 10, sy + TILE_SIZE - 10, RGB16(30, 255, 30));
						Hcolor(0);
						Hprint2(sx - Mapx, sy - Mapy, g_DestBackBuf, "%d", lpEventList->index);
						Hcolor(0xffff);
						Hprint2(sx - Mapx - 1, sy - Mapy - 1, g_DestBackBuf, "%d", lpEventList->index);
						//						TileMap[ lpRoof->x ][ lpRoof->y ].show_roof = 1;
						lpRoof = lpRoof->lpNext;
					}
				}
			}
		}
		else
		{
			LPEVENTLIST		lpEventList;
			LPROOFGROUP		lpRoofGroup;
			LPROOF			lpRoof;

			lpEventList = FindEventList(&g_EventListHeaderRoom, (WORD)mx, (WORD)my);
			if (lpEventList != NULL)
			{
				lpRoofGroup = FindRoofGroup(&g_RoofHeader, lpEventList->index);
				if (lpRoofGroup != NULL)
				{
					lpRoof = lpRoofGroup->lpFirst;
					while (lpRoof)
					{
						sx = lpRoof->x * TILE_SIZE;
						sy = lpRoof->y * TILE_SIZE;
						if (to) Box(sx + 4, sy + 4, sx + TILE_SIZE - 8, sy + TILE_SIZE - 8, RGB16(0, 255, 0));
						else Box(sx + 5, sy + 5, sx + TILE_SIZE - 10, sy + TILE_SIZE - 10, RGB16(30, 255, 30));
						Hcolor(0);
						Hprint2(sx - Mapx, sy - Mapy, g_DestBackBuf, "%d", lpEventList->index);
						Hcolor(0xffff);
						Hprint2(sx - Mapx - 1, sy - Mapy - 1, g_DestBackBuf, "%d", lpEventList->index);
						//						TileMap[ lpRoof->x ][ lpRoof->y ].show_roof = 1;
//						TileMap[ lpRoof->x ][ lpRoof->y ].show_roof = 0;
						lpRoof = lpRoof->lpNext;
					}
				}
			}
		}
	}

	if (TileMap[tmx][tmy].attr_room)
	{
		mx = tmx, my = tmy;
	}
}




///////////////////////////////////////////////////////////////////////////
// �ε��̳� ���̵��� ������ ���� ������ ����.
// 031110 YGI
void CheckRoof()
{
	LPCHARACTER ch = Hero;

	long	mx = ch->position.x;
	long	my = ch->position.y;

	if (TileMap[mx][my].roof)
	{
		LPROOFGROUP		lpRoofGroup;
		LPROOF			lpRoof;

		lpRoofGroup = FindRoofGroupByXY(&g_RoofHeader, mx, my);
		if (lpRoofGroup != NULL)
		{
			lpRoof = lpRoofGroup->lpFirst;
			while (lpRoof)
			{
				TileMap[lpRoof->x][lpRoof->y].show_roof = 1;
				lpRoof = lpRoof->lpNext;
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////

// ���� Avata�� �������� ��� �������� Check �Ͽ� ������ 
// ������ ����.
void CheckEnterHouse(LPCHARACTER ch)
{
	// ���ΰ��� �ƴϸ� Check���ʿ� ����. 
	if (ch != Hero) return;
	if (ch->position.x < 0 || ch->position.x > VILLAGE_SIZE) return;
	long	mx = ch->position.x;
	long	my = ch->position.y;

	// ���� �ڸ��� �ι� Check�� �ʿ�� ����. 
	if (mx == ch->oldposition.x && my == ch->oldposition.y) return;

	if (TileMap[mx][my].attr_room)
	{
		if (TileMap[mx][my].attr_room == 1)
		{
			LPEVENTLIST		lpEventList;
			LPROOFGROUP		lpRoofGroup;
			LPROOF			lpRoof;

			lpEventList = FindEventList(&g_EventListHeaderRoom, (WORD)mx, (WORD)my);
			if (lpEventList != NULL)
			{
				SendRoofOpenHouse(lpEventList->index);
				lpRoofGroup = FindRoofGroup(&g_RoofHeader, lpEventList->index);
				if (lpRoofGroup != NULL)
				{
					lpRoof = lpRoofGroup->lpFirst;
					while (lpRoof)
					{
						TileMap[lpRoof->x][lpRoof->y].show_roof = 1;
						lpRoof = lpRoof->lpNext;
					}
					ToggleRainHide(true); //Eleval 16/08/09
					ToggleSnowHide(true); //Eleval 16/08/09
				}
			}
		}
		else
		{
			LPEVENTLIST		lpEventList;
			LPROOFGROUP		lpRoofGroup;
			LPROOF			lpRoof;

			lpEventList = FindEventList(&g_EventListHeaderRoom, (WORD)mx, (WORD)my);
			if (lpEventList != NULL)
			{
				SendRoofCloseHouse(lpEventList->index);
				lpRoofGroup = FindRoofGroup(&g_RoofHeader, lpEventList->index);
				if (lpRoofGroup != NULL)
				{
					lpRoof = lpRoofGroup->lpFirst;
					while (lpRoof)
					{
						TileMap[lpRoof->x][lpRoof->y].show_roof = 0;
						lpRoof = lpRoof->lpNext;
					}
					ToggleRainHide(false); //Eleval 16/08/09
					ToggleSnowHide(false); //Eleval 16/08/09
				}
			}
		}
	}
}












void SettingTripAround(void)
{

}

void DeSettingTripAround(void)
{

}

bool IsCollision(int nX, int nY)
{
	return (TileMap[nX][nY].attr_dont ||
		TileMap[nX][nY].empty__4 ||
		TileMap[nX][nY].attr_light) ? true : false;
}

bool IsMovable(int nX, int nY)
{ //< CSD-020706
	return (TileMap[nX][nY].attr_dont || TileMap[nX][nY].attr_inside) ? false : true;
} //> CSD-020706