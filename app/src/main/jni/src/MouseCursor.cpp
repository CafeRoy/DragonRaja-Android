#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
	
	
#include "Dragon.h"
#include "Char.h"
#include "Tool.h"
#include "GameProc.h"
#include "Map.h"
#include "Hangul.h"
#include "DirectSound.h"
#include "Hong_Sprite.h"
#include "Hong_Light.h"
#include "Hong_Sub.h"
#include "Convert565to555.h"
#include "Skill.h"
#include "Item.h"
	
#define __MOUSE_CURSOR_H__
#include "MouseCursor.h"
	
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////
/////	extern variables...
/////


extern SMENU SMenu[ MAX_MENU]; // menustart.cpp


extern Spr *SkillIcon;

std::vector<LPSKILLCURSOR> g_SkillCursorDeletionQueue;

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////	
/////	user variables...
/////

//////////// 0527 lkh �߰� ////////////
t_queue		g_queue;

int CurMouseCursorNo = 1;
static int CurMouseCursorSpotX = 18, CurMouseCursorSpotY = 10;
static int 	CurMouseCursorStart, CurMouseCursorEnd, CurMouseCursorAni;


static int OldMouseCursorNo;
static int PushPopMoueCursor;


unsigned char *McBuf;
Spr mc[ MAX_MOUSE_CURSOR][20];

/* = { 
	{1,11},
	{1,5},
 {1,2},
 {0,0},
 {0,0},
 {0,0},
 {0,0},
 {1,1},
 {1,1},
 {1,1}, // 10
 {1,1}, 
 {1,1},
 {0,0},
 {0,0},
 {0,0},
 {0,0},
 {0,0},
 {0,0},
 {0,0},
 {0,0}, // 20
 {0,0}, 
 {0,0},
 {0,0},
 {0,0},
 {0,0},
 {0,0},
 {0,0},
 {1,5},
 {1,4},
 {1,5}, // 30
 {1,5}, 
 {0,0},
 {1,4},
 {1,4},
 {1,3},
 {1,4},
 {1,3},
 {1,4},
 {1,4},
 {1,3}, // 40
 {1,2}, 
 {0,0},
 {0,0},
 {0,0},
 {0,0},
 {0,0},
 {0,0},
 {0,0},
 {0,0},  
{0,0}};  // 49*/



void CursorNo( int no );

	
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////
/////		user functions..
/////



void LoadCursorAni(void)
{
    // [Android] 1. 路徑修正
    const char* txtPath = "data/cursorani.txt";
    const char* binPath = "data/cursorani.b";

    bool bLoadedFromText = false;

    // =========================================================
    // 嘗試讀取 TXT 檔案
    // =========================================================
    SDL_RWops* fp = SDL_RWFromFile(txtPath, "rb");

    if (fp)
    {
        // 取得檔案大小
        Sint64 fileSize = SDL_RWsize(fp);
        if (fileSize > 0)
        {
            // 讀取全部內容
            std::string content;
            content.resize(fileSize);
            SDL_RWread(fp, &content[0], fileSize, 1);

            // 使用 stringstream 解析
            std::stringstream ss(content);
            std::string line;
            int i = 0;

            // 逐行讀取
            while (std::getline(ss, line) && i < MAX_MOUSE_CURSOR)
            {
                // 移除 Windows 的 \r
                if (!line.empty() && line.back() == '\r') line.pop_back();

                // 跳過空行
                if (line.empty()) continue;

                // 使用 stringstream 取代 sscanf，自動解析整數
                std::stringstream lineSS(line);
                lineSS >> cursorani[i].start
                       >> cursorani[i].end
                       >> cursorani[i].offx
                       >> cursorani[i].offy;
                i++;
            }
            bLoadedFromText = true;
            SDL_Log("Info: Loaded CursorAni from TXT.");
        }
        SDL_RWclose(fp);

        // =====================================================
        // 寫入 BIN 檔案 (僅限 Windows/PC，Android 跳過)
        // =====================================================
#ifndef __ANDROID__
        // 在 PC 端生成 .b 檔，下次打包 APK 時就可以包含進去
        SDL_RWops* fpOut = SDL_RWFromFile(binPath, "wb");
        if (fpOut)
        {
            SDL_RWwrite(fpOut, cursorani, sizeof(t_cursorani), MAX_MOUSE_CURSOR);
            SDL_RWclose(fpOut);
            SDL_Log("Info: Generated CursorAni.b");
        }
#endif
    }
    else
    {
        // =====================================================
        // 如果 TXT 讀不到，嘗試讀取 BIN 檔案
        // =====================================================
        fp = SDL_RWFromFile(binPath, "rb");
        if (fp)
        {
            // 直接將二進位資料讀入 struct 陣列
            // 參數順序: (context, ptr, size, count)
            size_t readCount = SDL_RWread(fp, cursorani, sizeof(t_cursorani), MAX_MOUSE_CURSOR);

            if (readCount == MAX_MOUSE_CURSOR) {
                SDL_Log("Info: Loaded CursorAni from BIN.");
            } else {
                SDL_Log("Warning: CursorAni.b read count mismatch. Expected %d, got %zu", MAX_MOUSE_CURSOR, readCount);
            }

            SDL_RWclose(fp);
        }
        else
        {
            SDL_Log("Error: Failed to load CursorAni (TXT or BIN not found).");
        }
    }
}


// 0810 KHS
void CurMouseCursorFrameCounter( void )
{
	static int del;
	del ++;

	if( tool_MouseCursorBlock ) 
	{
		if( ++del >= 2 ) {	CurMouseCursorAni ++; del = 0; }
				if( CurMouseCursorAni > CurMouseCursorEnd ) 
					CurMouseCursorAni = CurMouseCursorStart; 
		
		return;
	}

	switch( CurMouseCursorNo )
	{
		// �Ϲ�Ŀ��.
	case 1 : 	if( ++del >= 2 ) {	CurMouseCursorAni ++; del = 0; }
				if( CurMouseCursorAni > CurMouseCursorEnd ) 
					CurMouseCursorAni = CurMouseCursorStart; 
		break;

		// �Ϲ�����Ѽ�.
	case 2 :	if( ++del >= 2 ) {	CurMouseCursorAni ++; del = 0; }
				if( CurMouseCursorAni > CurMouseCursorEnd ) CurMouseCursorAni = CurMouseCursorEnd;
		break;

		// ���콺�� �̵�����Ʈ 
	case 3 :	if( ++del >= 2 ) {	CurMouseCursorAni ++; del = 0; }
				if( CurMouseCursorAni > CurMouseCursorEnd ) 
				{
					CursorNo( 1 );
				}	
		break;

	case 53 :	CurMouseCursorAni ++; 
				if( CurMouseCursorAni > CurMouseCursorEnd ) 
				{
					CursorNo( 1 );
				}	
		break;
		
	default :	CurMouseCursorAni = CurMouseCursorStart;
		break;
	}
}



void GetImageBox( char *backbuf, char *ground, int sx, int sy, int ex, int ey )
{	
	int xl = (ex - sx + 1) * 2;
	int c=0;

	for( ; sy <= ey ; sy ++) 
	{
		memcpy( backbuf + c * xl, ground + sx * 2 + sy * dDxSize, xl );
		c++;
	}
}

void PutImageBox( char *backbuf, char *ground, int sx, int sy, int ex, int ey )
{						
	int xl = (ex - sx + 1) * 2;
	int c=0;

	for( ; sy <= ey ; sy ++) 
	{
		memcpy( ground + sx * 2 + sy * dDxSize, backbuf + c * xl, xl );
		c++;
	}
}

/*
// 1�� �ǹ��ϴ°��� ���ø� ���Ѵ�. 
// 1�� ���� �Լ�������	��������, 
//							�⺻���¿� ���� ���콺�� �ٲ��ش�. 

  1  11  25  36   // 1 : �Ϲ�Ŀ�� 
  1  10  22  37   // 2 : �Ϲ����Ŀ�� 
  1   3  22  37   // 3 : �̵� Point ȿ�� 
  0   0  -7   0   // 4 : �Ϲ� ��ġĿ�� 
  0   0   0   0   // 5 : ����Ŀ�� 
  0   0   0   0   // 6 : ������ġĿ��1 
  1   1 -19  23   // 7 : ���� Ŀ��1 
  1   5 -24   2   // 8 : ȭ�� ��ũ�� Ŀ��1 
  1   5   2   4   // 9 : ȭ�� ��ũ�� Ŀ��2 
  1   5  20  15   // 10 : ȭ�� ��ũ�� Ŀ��3 
  1   5  17  26   // 11 : ȭ�� ��ũ�� Ŀ��4 
  1   8 -12  33   // 12 : ȭ�� ��ũ�� Ŀ��5 
  1   5 -44  48   // 13 : ȭ�� ��ũ�� Ŀ��6 
  1   5 -62   3   // 14 : ȭ�� ��ũ�� Ŀ��7 
  1   5 -53  13   // 15 : ȭ�� ��ũ�� Ŀ��8 
  1  10  22  20   // 16 : ��ų�Ϲ�Ŀ��1 
  1   6  89  17   // 17 : ��ų��ġĿ��1 
  1   8 -17  21   // 18 : ��ų ����ġ Ŀ�� 
  1   8  10  25   // 19 : ��ųdetectĿ��1 
  1   4  25  31   // 20 : ��ųOBPointĿ�� 
  1   6  25  31   // 21 : ��ųOBPoint��ġĿ�� 
  1  10  23  20   // 22 : ��ųIdentifiedĿ�� 
  1  10  23  20   // 23 : ��ųIdentified��ġĿ�� 
  1  10  23  20   // 24 : ��ų����Ŀ�� 
  1  10  23  20   // 25 : ��ų������ġ 
  0   0   0   0   // 26 : ��ųHideĿ�� 
  1   8 -17  20   // 27 : ��ųHide ��ġĿ�� 
  1  10  18  12   // 28 : ��ų ��Ŀ�� 
  1   8  25  20   // 29 : ��ų ��Ŀ�� 
  1   8   7  21   // 30 : ��ų ���Ŀ�� 
  1  18  89  17   // 31 : ��ų ���ô�Ŀ�� 
  1   6 -12  14   // 32 : ��ų ����ĮĿ�� 
  1  11 -37  -9   // 33 : ��ų �׹�Ŀ�� 
  1   8  61 -27   // 34 : ��ų ����Ŀ�� 
  1   5  44  14   // 35 : ��ų ��Ŀ�� 
  1   7  53  -6   // 36 : ��ų ����ĮĿ�� 
  1   7   0   0   // 37 : ��ų ȣ��Ŀ�� 
  1   5  78  37   // 38 : ��ų �����Ŀ�� 
  1   9 -18   8   // 39 : ��ų ������Ŀ�� 
  1   5   0  11   // 40 : ��ų ������Ŭ��Ŀ�� 
  1   4  -6  23   // 41 : ��ų ������ġĿ�� 
  1  10  23  20   // 42 : ��ų ����  
  1  10  23  20   // 43 : ��ų ���� ��ġĿ�� 
  1  10  22  37   // 44 : �Ϲ�Ŀ���׵θ������� 
  1  10  22  37   // 45 : �Ϲ����Ŀ���׵θ������� 
  1   3  22  37   // 46 : �Ϲ� ��ġĿ���׵θ������� 
  1  10  22  37   // 47 : �Ϲ�Ŀ���ʷϻ� 
  1  10  22  37   // 48 : �Ϲ����Ŀ���ʷϻ� 
  1   3  22  37   // 49 : �Ϲ� ��ġĿ���ʷϻ� 
  1   9  -1  18   // 50 : �ν˵� 
  1   8 -17  21   // 51 : �̵�����Ʈ 
  0   0   0   0   // 52 : ���� ĳ���� ȿ�� 
  0   0   0   0   // 53 :  
  0   0   0   0   // 54 :  
  0   0   0   0   // 55 :  
  0   0   0   0   // 56 :  
  0   0   0   0   // 57 :  
  0   0   0   0   // 58 :  
  0   0   0   0   // 59 :  
  0   0   0   0   // 60 :  
	
*/	
	
	
	
void CursorStatus( void )
{		

	return;



	static int cursor= -1, oldcursor;
		
	oldcursor = cursor;
	if( PushPopMoueCursor == 0 ) 
	if( SkillStatus == SKILL_OFF || CurMouseCursorNo == 1 ) // ��Ȳ�� ���߾� ����϶�..
	{	
		if( Hero->peacests == 0	)
		{		
			if( g_GameInfo.lpcharacter )
			{	
				switch( g_GameInfo.nSelectedSpriteType )
				{
				case SPRITETYPE_ON_THE_CEILING_CHAR	 :
				case SPRITETYPE_MONSTER		:
				case SPRITETYPE_NPC			:
				case SPRITETYPE_CHARACTER	:	cursor = 4; // �Ϲ� ��ġĿ�� 
				break;
				}
			}	
			else
			{	
				cursor = 1;
			}	
		}
		else	
		{
			if( g_GameInfo.lpcharacter )
			{	
				switch( g_GameInfo.nSelectedSpriteType )
				{
				case SPRITETYPE_ON_THE_CEILING_CHAR	 :
				case SPRITETYPE_MONSTER		:
				case SPRITETYPE_NPC			:
				case SPRITETYPE_CHARACTER	:	cursor = 6; // ������ġĿ��
				break;
				}
			}	
			else
			{	
				cursor = 5;
			}	
		}
		
		if( CursorCheckedItem )		cursor = 2; // �Ϲ����Ŀ�� 
		
		// ������ ����������
		if( Hero )
		if( Hero->ready_Magic == 2 || Hero->ready_Magic == 3 ) cursor = 7; //���� Ŀ��1 
		
		
		if( TileMap[Mox / TILE_SIZE][ Moy/TILE_SIZE].attr_dont )
		{
			cursor = 44; // 44 : �Ϲ�Ŀ���׵θ������� 
		
		}

		//	BBD 031114	�̷��� �Ǹ� g_MouseInMenu�� �޴���ȣ�� ���� �ǹ̰� ������
		//	���� PM ���� �ǵ��� �ڵ带 ���µ��Ͽ� �ӽ÷� �ּ�ó��
/*
		if( g_MouseInMenu == true )	
		{
		}
		*/
	}	
		
	if( oldcursor != cursor ) // ���콺Ŀ�Ű� �ٲ������....
	{	
		CursorNo( cursor );
	}	
}






// 0810 KHS
void CursorNo( int no )
{	
	int x = 0, y = 0;	
	
	if( no == 0 ) return;
	switch( no )
	{
		case 53 : 
			break;

		default :	if( no >=16 ) no /= 10;
			break;
	}
	
	CurMouseCursorNo = no;
	
	CurMouseCursorStart = cursorani[no-1].start;
	CurMouseCursorEnd   = cursorani[no-1].end;
	
	CurMouseCursorSpotX = cursorani[no-1].offx;
	CurMouseCursorSpotY = cursorani[no-1].offy;
	
	CurMouseCursorAni   = CurMouseCursorStart;
}	
	
	
	
	
void PushMouseCursor( void )
{	
	if( 8 <= CurMouseCursorNo && CurMouseCursorNo <= 15 )
	{
		return;
	}
	
	if( PushPopMoueCursor ) return;
	PushPopMoueCursor =1;
		 
	OldMouseCursorNo = CurMouseCursorNo;
}	
	
void PopMouseCursor( void )
{	
	if( SMenu[MN_MAININTERFACE].bActive == FALSE ) return;
	
	
	if( PushPopMoueCursor )
	{
		CursorNo( OldMouseCursorNo );
		PushPopMoueCursor = 0;
	}
}	
	
	
	
	
	
extern void		PutItemIcon(int x, int y, int item_number, int delay=0, int effect = 2, int is_not_equip = 0 );
extern int     CheckAndCraftVertical(int page, int column);
extern	POINT	MouseItemInRect;
	
extern ItemAttr	InvItemAttr[3][3][8];
extern ItemAttr	EquipItemAttr[ITEM_EQUIP_MAX];
extern ItemAttr	QuickItemAttr[7];
extern ItemAttr SaveArmor[5][8];
extern bool     EquipSaveFlag[5];
	
void withSkillItem( void )
{	
	int x, y;
	DWORD no;
	
	if( SkillStatus != SKILL_READY ) return;
		
	switch( SkillNo )
	{
		// Sow
	case 22  :	break;
	
	default : return;
	}
	
	x = g_pointMouseX + CurMouseCursorSpotX;
	y = g_pointMouseY + CurMouseCursorSpotY;
	
	PutItemIcon( x, y + 20, SkillItemNo );
	
	
	switch( SkillItemPOS.type )
	{
	case QUICK :	no = QuickItemAttr[ SkillItemPOS.p3].attr[ IATTR_MUCH];	break;
	case INV  :		no = InvItemAttr[ SkillItemPOS.p1][ SkillItemPOS.p2][ SkillItemPOS.p3].attr[ IATTR_MUCH];	break;
	case EQUIP :	no = EquipItemAttr[ SkillItemPOS.p3].attr[ IATTR_MUCH];	break;
	default : return;
	}
	
	HprintBold( x, y+30, 0xffff, 0, "%2d", no );
}	
	
void CursorDisplayBack( void )
{		
	if( MouseCursorOnOff == false ) return;// ���������.
	if( SMenu[MN_LOGO_START].bActive==true ) return;

	PmcSx = SCREEN_WIDTH;
	PmcSy = SCREEN_HEIGHT;
	PmcEx = PmcEy = 0;
	Spr *s = &mc[ CurMouseCursorNo-1][ CurMouseCursorAni];

	int sx = g_pointMouseX + CurMouseCursorSpotX - s->ox;
	int sy = g_pointMouseY + CurMouseCursorSpotY - s->oy;
	int ex = sx + s->xl;
	int ey = sy + s->yl;
		
	if( sx < PmcSx ) PmcSx = sx;
	if( sy < PmcSy ) PmcSy = sy;
	if( ex > PmcEx ) PmcEx = ex;
	if( ey > PmcEy ) PmcEy = ey;
		
	if( PmcSx < 0 ) PmcSx = 0;
	if( PmcSy < 0 ) PmcSy = 0;
	if( PmcEx > SCREEN_WIDTH -1) PmcEx = SCREEN_WIDTH-1;
	if( PmcEy > SCREEN_HEIGHT-1) PmcEy = SCREEN_HEIGHT-1;



	if( SMenu[MN_LODING].bActive != TRUE )  // ȭ�� Loading�Ҷ��� ���� �ʴ´�.
	{	
		PutCompressedImage( g_pointMouseX + CurMouseCursorSpotX, g_pointMouseY + CurMouseCursorSpotY, s );
		//g_SDLInfo.testRenderCopyImg(g_pointMouseX + CurMouseCursorSpotX, g_pointMouseY + CurMouseCursorSpotY, s);
		int ttx = g_pointMouseX + Mapx, tty = g_pointMouseY + Mapy;

		static int to = 1, tcolor = 233;
		WORD ccolor;

		if (to) {
			tcolor += 1;
			if (tcolor >= 255) {
				tcolor = 255;
				to = !to;
			}
		}
		else {
			tcolor -= 1;
			if (tcolor <= 233) {
				to = !to;
			}
		}

		ccolor = RGB16(tcolor, tcolor, tcolor);

		Line( ttx-2, tty, ttx+3, tty, ccolor, false);
		Line( ttx, tty-2, ttx, tty+3, ccolor, false);

		withSkillItem();
	}

#ifdef _DEBUG
	if( tool_MouseCursorBlock )
	{
		PutCompressedImage( 320 + CurMouseCursorSpotX, 240 + CurMouseCursorSpotY, s );
		int ttx = 320 + Mapx, 
			tty = 240 + Mapy;
		Line( ttx-5, tty, ttx+5, tty, 0xffff, false);
		Line( ttx, tty-5, ttx, tty+5, 0xffff, false);
	}
#endif
}

void FreeMouseCursor ( void )
{	
	if( McBuf ) delete McBuf;
	McBuf = NULL;
}
void LoadMouseCursor( std::string filename )
{
    char temp[FILENAME_MAX];
    short no = 0;
    DWORD sysfilelength = 0;
    DWORD size = 0;
    DWORD nOff = 0;
    int i, j;

    // 清除舊的游標資源
    FreeMouseCursor();

    // [Android 修改] 1. 路徑修正：移除開頭的 ./
    snprintf(temp, FILENAME_MAX, "data/%s", filename.c_str());

    // [Android 修改] 2. 開檔
    SDL_RWops* fp = SDL_RWFromFile(temp, "rb");

    if( fp == NULL ) {
        SDL_Log("Error: Failed to open mouse cursor file %s", temp);
        return;
    }

    // [Android 修改] 3. 讀取數量 (short)
    SDL_RWread(fp, &no, 2, 1);

    // [Android 修改] 4. 移動指標 (Seek)
    // 跳過偏移表 (假設是 no * 4 bytes)
    SDL_RWseek(fp, no * 4, RW_SEEK_CUR);

    // [Android 修改] 5. 計算檔案剩餘所需的資料長度
    // 原本: _filelength( _fileno( fp) ) -> SDL_RWsize(fp)
    Sint64 totalFileSize = SDL_RWsize(fp);
    sysfilelength = (DWORD)(totalFileSize - no * 4 * 2 - 2);

    // 分配記憶體 (維持原樣)
    MemAlloc(McBuf, sysfilelength);

    if (McBuf == NULL) {
        SDL_Log("Error: Memory allocation failed for MouseCursor");
        SDL_RWclose(fp);
        return;
    }

    for( i = 0 ; i < no / 20 ; i ++) // 假設 20 是每組的 frame 數
    {
        for( j = 0 ; j < 20 ; j ++)
        {
            // 防止讀取溢位
            if( nOff >= sysfilelength ) {
                //Error("mouse cursor Sprite file size too big" );
                SDL_Log("Error: Mouse cursor Sprite file size too big or format error");
                break;
            }

            // 讀取該張圖片的 Block Size
            SDL_RWread(fp, &size, 4, 1);

            // 讀取 Spr 結構內容
            SDL_RWread(fp, &mc[i][j].xl, sizeof(short), 1);
            SDL_RWread(fp, &mc[i][j].yl, sizeof(short), 1);
            SDL_RWread(fp, &mc[i][j].ox, sizeof(short), 1);
            SDL_RWread(fp, &mc[i][j].oy, sizeof(short), 1);
            SDL_RWread(fp, &mc[i][j].size, sizeof(unsigned int), 1);

            // 讀取圖片像素資料到緩衝區
            // 注意：這裡是讀取到全域緩衝區 McBuf 的特定偏移位置
            SDL_RWread(fp, (char*)McBuf + nOff, mc[i][j].size, 1);

            // 設定指標指向緩衝區
            mc[i][j].img = (char *)McBuf + nOff;

            // 格式轉換
            convert565to555(&mc[i][j]);

            // 推進偏移量 (注意：這裡是用讀取到的 size，而非 mc[i][j].size，這通常包含 header 或 padding)
            nOff += size;
        }
    }

    // [Android 修改] 6. 關檔
    SDL_RWclose(fp);

    // 建立紋理 (Atlas)
    g_SDLInfo.CreateAtlasFromSprArray2D_CPP(temp, &mc[0][0], no / 20, 20);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------

void CleanUpSkillCursors()
{
	// 遍歷隊列中的每一個指標
	for (LPSKILLCURSOR cursor_ptr : g_SkillCursorDeletionQueue)
	{
		// 執行真正的記憶體釋放
		MemFree(cursor_ptr);
	}

	// 清空 vector，準備下一輪的待刪除物件
	g_SkillCursorDeletionQueue.clear();
}

void DeleteSkillCursor( LPSKILLCURSOR tt )
{		
	LPSKILLCURSOR t = g_SkillCursor;

	while (t != NULL)
	{
		if (t == tt)
		{
			// 執行必要的副作用
			if (t->ch == Hero)
			{
				MouseCursorOnOff = true;
			}

			// **步驟 1：從主清單解鏈 (Unlink)**
			if (t == g_SkillCursor)
			{
				// 處理頭部
				g_SkillCursor = t->next;
				if (g_SkillCursor != NULL)	g_SkillCursor->prev = NULL;
			}
			else
			{
				// 處理中間或尾部
				if (t->prev != NULL)
					t->prev->next = t->next;

				if (t->next != NULL)
					t->next->prev = t->prev;
			}

				// **步驟 2：將指標推入隊列，等待安全刪除**
			g_SkillCursorDeletionQueue.push_back(t);

			// 注意：這裡不能 MemFree(t)！
			return;
		}
		t = t->next;
	}

	/*LPSKILLCURSOR  t = g_SkillCursor;

	while( t != NULL )
	{		
		if( t == tt )
		{	
			if( t->ch == Hero ) 
			{
				MouseCursorOnOff = true;
			}

			if( t == g_SkillCursor )
			{	
				t = g_SkillCursor->next;
				MemFree( g_SkillCursor );
		
				g_SkillCursor = t;
				if( t != NULL )	g_SkillCursor->prev = NULL;
				
				return;
			}
			else 
			{
				if ( t->prev != NULL )
					t->prev->next = t->next;
	
				if( t->next != NULL )
					t->next->prev = t->prev;
	
				
   				MemFree( t );
				return;
			}
		}
		t = t->next;
	}	*/	
}	

LPSKILLCURSOR AddSkillCursor( LPCHARACTER ch, int skillno, int cursorno, int x, int y, int repeat )
{
	LPSKILLCURSOR temp = NULL, t;
	cursorno /= 10;
	if( cursorno == 0 ) return NULL;
	
	//-----------------------------------
	
	if( ch == Hero ) MouseCursorOnOff = false;
	
	MemAlloc( temp, sizeof( SKILLCURSOR ));
	
	temp->ch		= ch;
	temp->skillno	= skillno;
	temp->cursorno	= cursorno-1;
	temp->x			= x;
	temp->y			= y;
	temp->curframe	= cursorani[cursorno-1].start;
	temp->repeat	= repeat;
	temp->endtime   = g_curr_time + repeat;
	temp->bShouldBeDeleted = false;
	
	temp->prev = NULL;
	temp->next = NULL;
	
	
	
	if( g_SkillCursor == NULL )
	{	
		g_SkillCursor		= temp;
	}	
	else
	{	
		t					= g_SkillCursor;
		g_SkillCursor		= temp;
		g_SkillCursor->next	= t;
		t->prev				= g_SkillCursor;
	}		
	
	return g_SkillCursor;
}	
	
bool StopDoingSkillCursor( LPCHARACTER ch )
{	
	LPSKILLCURSOR t, temp;
	
	t = g_SkillCursor;
	while( t != NULL )
	{		
		if( t->ch == ch )
		{
			temp = t->next; 
			DeleteSkillCursor( t ); 
			t = temp; 
			continue; 
		}	
		t = t->next;
	}			
	return false;
}	
	
	
	
bool IsDoingSkillCursor( LPCHARACTER ch )
{	
	LPSKILLCURSOR t;
	
	t = g_SkillCursor;
	while( t != NULL )
	{		
		if( t->ch == ch )
		{
			return true;
		}	
			
		t = t->next;
	}			
	return false;
}	
	
	
	
	
void OutputSkillCursor( void )
{	
	LPSKILLCURSOR t, temp;
	LPORDERTABLE o;
	
	t = g_SkillCursor;
	while( t != NULL )
	{		
		if( t->curframe >= cursorani[ t->cursorno].end ) 
		{	
			t->repeat--;
			if( t->repeat <= 0 ) 
//			if( g_curr_time > t->endtime )
			{
				temp = t->next; 
				DeleteSkillCursor( t ); 
				t = temp; 
				continue; 
			}
			else
			{
				t->curframe = cursorani[t->cursorno].start;
			}
		}	
			
		t->sp = &mc[ t->cursorno ][ t->curframe ];
			
		if( t->curframe == cursorani[t->cursorno].start )
		{
			if( SkillTable[ t->skillno].Effect_wave1 )
			{
				PlayListAutoSounds( SkillTable[ t->skillno].Effect_wave1, 0, 0, 0 );
			}
		}
		
		
		if( BoxAndDotCrash( Mapx, Mapy, GAME_SCREEN_XSIZE, GAME_SCREEN_YSIZE, t->x, t->y ) )
		{		
			if (g_OrderInfo.count >= MAX_ORDERTABLE) break;

			o = &g_OrderInfo.order[ g_OrderInfo.count ];
					
			o->wType		= SPRITETYPE_SKILLCURSOR;
			o->x			= t->x + cursorani[t->cursorno].offx;
			o->y			= t->y + cursorani[t->cursorno].offy;
			o->height		= 0;
			o->offset		= t->y + cursorani[t->cursorno].offy;
			o->lpvData		= (LPVOID)t;
			o->show			= 1;
			o->sort			= 1;
			g_OrderInfo.count++;
		}	
		
		t->curframe ++;
		t = t->next;
	}		
}		
	
/////////////////////////////////////////////////////////////////////////////////////////
	
static int		HEAD, TAIL;
	
void ClearQueue( void )
{	
	HEAD = TAIL = 0;
}
	
int queuein( t_queue *n )
{	
	if( (TAIL+1) %  MAX_MQUEUE != HEAD )
	{	
		memcpy( &aQueue[TAIL], n, sizeof( t_queue ) );
		TAIL ++;
		TAIL = TAIL % MAX_MQUEUE ;
		return 1;
	}	
	return 0;
}	

int queueout( t_queue *n)
{		
	if( TAIL != HEAD )
	{	
		memcpy( n, &aQueue[HEAD], sizeof( t_queue ) );
		HEAD ++;
		HEAD = HEAD % MAX_MQUEUE;
		return 1;
	}	
		
	n = NULL;
	return 0;
}	
	
	
	
	
	
	
	
	
	
		