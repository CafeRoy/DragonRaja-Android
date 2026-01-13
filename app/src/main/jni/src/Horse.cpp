#include "stdafx.h"
#include <fstream>

#include "gameproc.h"
#include "NetWork.h"
#include "menu.h"
#include "SmallMenu.h"
#include "map.h"
#include "LineCommand.h"
#include "tool.h"
#include "hong_sub.h"
#include "Horse.h"
#include "char.h"
#include "Battle.h"
using namespace battle;

#define CHECKSUM_ERROR_ANIMATIONDATA	20008

cHorseRider	g_HorseRider;

extern DWORD		g_curr_time;

int8_t HorseWeaponConvertTable[2][38]=
{											                     // 18    	
	{ 0, 0,-1,-1,-1, 4, 4, 6, 6,10, 8,10, 8, 9, 9,11,11,12,12,13,13,-1,-1,-1,-1,-1,17,17,17,17,17,17,18,19,20,21,22,23},
	{ 0, 0,-1,-1,-1, 4, 4, 5, 5, 6, 6,10, 8, 9, 9,10, 8,11,11,12,12,13,13,-1,-1,-1,17,17,17,17,17,17,18,19,20,21,22,23}	
};	// Sprite File

int8_t HorseEffectWeaponConvertTable[2][10] =
{	//< CSD-031020											                     // 18    	
	{-1, 0, 12, -1, 6, 9, 10, 11, -1, -1},
	{-1, 0, 12, 13, 6, 9, 10, 11, 13, 5}	
};	//> CSD-031020

extern int			convert565to555_LoadSprite(Spr *sp);
extern void			PutCompressedImage(int x, int y, Spr *sp );
extern BOOL			PutDeadEffectBottom(LPCHARACTER ch);
extern void			PutCompressedImageCharRGB( int x, int y, Spr *sp, DWORD RGB );
extern void			PutCompressedImageBlendRGB( int x, int y, Spr *sp, DWORD RGB );
extern void			PutCompressedImageFX(int x, int y, Spr *sp, DWORD alpha, DWORD op);
extern void			PutCompressedImageVoid(int x, int y, Spr *sp );
extern void			PutCompressedImageShadow( int x, int y, Spr *sp );
extern int			IsDead( LPCHARACTER ch );






void InitHorseAniTable()
{
	g_HorseRider.LoadHorseRiderAniMationTable(0,0,"./char/HRAniTable0000.txt");
	g_HorseRider.LoadHorseRiderAniMationTable(0,1,"./char/HRAniTable0100.txt");
	g_HorseRider.LoadHorseRiderAniMationTable(0,2,"./char/HRAniTable0200.txt");
	g_HorseRider.LoadHorseRiderAniMationTable(1,0,"./char/HRAniTable0001.txt");
	g_HorseRider.LoadHorseRiderAniMationTable(1,1,"./char/HRAniTable0101.txt");
	g_HorseRider.LoadHorseRiderAniMationTable(1,2,"./char/HRAniTable0201.txt");
}

cHorseRider::cHorseRider()
{
}

cHorseRider::~cHorseRider()
{
	ClearSpriteAll();
}

// [REFRACTOR] Added by Gemini
// This is a new, unified function designed to replace the duplicated code in all the Load...Sprite functions.
// It handles the specific binary format of the .spr files used in this project.
//
// Parameters:
//   - filename: The path to the .spr file to load.
//   - outBuffer: A reference to a char pointer. The function will allocate a memory block for the raw sprite image data
//                and assign its address to this pointer. The caller is responsible for freeing this buffer via MemFree.
//   - outSprites: A pointer to an array of Spr structures (e.g., Spr[8][75]) that will be filled with sprite metadata.
//   - error_code: The application-specific error code to exit with if the sprite file is corrupt.
//
// Returns:
//   - true if the file was loaded successfully, false otherwise.
// ============================================================================
// 1. LoadSpriteSheet (讀取坐騎精靈圖)
// ============================================================================

bool cHorseRider::LoadSpriteSheet(const char* filename, char*& outBuffer, Spr* outSprites, int error_code)
{
    // 如果 buffer 不是 NULL，代表已經載入過了
    if (outBuffer != NULL)
    {
        return false;
    }

    // [Android] 路徑修正：移除開頭的 ./
    std::string path = filename;
    if (path.find("./") == 0) path = path.substr(2);

    // [Android] 使用 SDL 開檔
    SDL_RWops* fp = SDL_RWFromFile(path.c_str(), "rb");
    if (fp == NULL)
    {
        SDL_Log("Error: Failed to open sprite sheet %s", path.c_str());
        return false;
    }

    // 1. 讀取數量 (Short)
    short no = 0;
    SDL_RWread(fp, &no, 2, 1);

    // 2. 跳過 Offset Table (no * 4 bytes)
    SDL_RWseek(fp, no * 4, RW_SEEK_CUR);

    // 3. 計算 sprfilelength
    // 原本: _filelength(_fileno(fp)) - no * 4 * 2 - 2;
    // SDL: 使用 SDL_RWsize 取得檔案總大小
    Sint64 totalSize = SDL_RWsize(fp);
    DWORD sprfilelength = (DWORD)(totalSize - no * 4 * 2 - 2);

    // 分配記憶體 (保持原樣)
    MemAlloc(outBuffer, sprfilelength);

    if (outBuffer == NULL) {
        SDL_Log("Error: Memory allocation failed for %s", path.c_str());
        SDL_RWclose(fp);
        return false;
    }

    DWORD nOff = 0; // 目前在 buffer 中的偏移量

    // 4. 讀取 8x75 的 Sprite 區塊
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 75; j++)
        {
            Spr* s = &outSprites[(i * 75) + j];

            DWORD size = 0;
            SDL_RWread(fp, &size, 4, 1);

            if (size == 0)
            {
                s->xl = 0; s->yl = 0; s->size = 0; s->img = NULL;
                continue;
            }

            // 讀取 Sprite Metadata
            SDL_RWread(fp, &(s->xl), sizeof(short), 1);
            SDL_RWread(fp, &(s->yl), sizeof(short), 1);
            SDL_RWread(fp, &(s->ox), sizeof(short), 1);
            SDL_RWread(fp, &(s->oy), sizeof(short), 1);
            SDL_RWread(fp, &(s->size), sizeof(unsigned int), 1);

            // 安全檢查
            if (nOff + s->size > sprfilelength)
            {
                SDL_Log("Error: File overflow or corrupted: %s", path.c_str());
                // JustMsg("%s File Error !!", filename);
                // ExitApplication(static_cast<eExitAppType>(error_code));
                SDL_RWclose(fp);
                return false;
            }

            // 讀取像素資料到 buffer
            SDL_RWread(fp, outBuffer + nOff, s->size, 1);

            // 設定 img 指標指向 buffer 內部
            s->img = outBuffer + nOff;

            // 格式轉換 (如果有需要請解開註解)
            // convert565to555_LoadSprite(s);

            // 重要：偏移量增加的是區塊開頭讀到的 size (包含 header 的大小)，不是 s->size
            nOff += size;
        }
    }

    SDL_RWclose(fp);

    // 建立紋理 Atlas (使用修正後的路徑)
    // g_SDLInfo.CreateOutlinePointsHorseRider(outSprites);
    g_SDLInfo.CreateAtlasForHorseRider(path.c_str(), outSprites);

    return true;
}

// ============================================================================
// 2. LoadHorseRiderAniMationTable (讀取坐騎動畫表)
// ============================================================================

void cHorseRider::LoadHorseRiderAniMationTable(int Sex, int HorseNo, char* FileName)
{
    char filenameBin[MAX_PATH]={0};

    // [Android] 1. 建構二進位檔案路徑
    // 移除 ./ 並修正資料夾路徑
    snprintf(filenameBin, MAX_PATH, "char/HRAniTable%02d%02d.bin", HorseNo, Sex);

    // [Android] 2. 強制僅讀取二進位檔 (.bin)
    // 原本的 TXT 解析與寫入 BIN 邏輯在 Android (Release Mode) 上是不需要的且無法運作

    SDL_RWops* fp = SDL_RWFromFile(filenameBin, "rb");

    // 如果讀取失敗 (例如檔案不存在)
    if (!fp)
    {
        // 嘗試回退機制：如果是 PC Debug 模式，這裡可以放原本的 TXT 讀取代碼
        // 但在 Android 上我們只能報錯
        if (GetSysInfo(SI_GAME_MAKE_MODE)) {
            SDL_Log("Warning: %s not found. Text parsing is disabled on Android.", filenameBin);
        }
        CheckSumError = CHECKSUM_ERROR_ANIMATIONDATA;
        return;
    }

    // [Android] 3. 讀取二進位資料
    if (fp)
    {
        char crc1 = 0;
        char crc = 0;

        // 跳過前 20 bytes 的 garbage data
        SDL_RWseek(fp, 20, RW_SEEK_SET);

        // 讀取 CheckSum
        SDL_RWread(fp, &crc1, 1, 1);

        // 讀取動畫表資料
        SDL_RWread(fp, HorseRiderAniTable[Sex][HorseNo], sizeof(PCANIMATIONTABLE), MAX_ANIMATION);

        SDL_RWclose(fp);

        // 4. 計算 CheckSum 驗證
        char *tt = (char *)HorseRiderAniTable[Sex][HorseNo];
        for(int i = 0; i < sizeof(PCANIMATIONTABLE) * MAX_ANIMATION; i++)
        {
            crc += *tt;
            tt++;
        }

        if (abs(crc1 - crc))
        {
            SDL_Log("Error: Animation Data CheckSum Failed! File: %s", filenameBin);
            CheckSumError = CHECKSUM_ERROR_ANIMATIONDATA;
        }
    }
}
// [REFRACTOR] The original Load...Sprite functions are now simple wrappers around the new unified LoadSpriteSheet function.
// This eliminates massive code duplication. The public interface remains the same, so no other code needs to change.

int cHorseRider::LoadRiderSprite(int SexType, int SpriteKind)
{
    char tempFileName[MAX_PATH];
    sprintf(tempFileName, "./char/hr%03d%03d.spr", SexType, SpriteKind);
    return LoadSpriteSheet(tempFileName, RiderPicBuffer[SexType][SpriteKind], &RiderPic[SexType][SpriteKind][0][0], EA_LOAD_RIDER_SPR_DATA_SUB);
}

int cHorseRider::LoadRiderBandSprite(int SexType, int SpriteKind)
{
    char tempFileName[MAX_PATH];
    sprintf(tempFileName, "./char/rb%03d%03d.spr", SexType, SpriteKind);
    return LoadSpriteSheet(tempFileName, RiderBandPicBuffer[SexType][SpriteKind], &RiderBandPic[SexType][SpriteKind][0][0], EA_LOAD_RIDER_BAND_SPR_DATA_SUB);
}

int cHorseRider::LoadRiderMantleSprite(int SexType)
{
    char tempFileName[MAX_PATH];
    sprintf(tempFileName, "./char/rm%03d.spr", SexType);
    return LoadSpriteSheet(tempFileName, RiderMantlePicBuffer[SexType], &RiderMantlePic[SexType][0][0], EA_LOAD_RIDER_MANTLE_SPR_DATA_SUB);
}

int cHorseRider::LoadRiderGradeSprite(int SexType, int SpriteKind, int GradeKind)
{
    char tempFileName[MAX_PATH];
    sprintf(tempFileName, "./char/rgr%02d%02d%02d.spr", SexType, SpriteKind, GradeKind);
    return LoadSpriteSheet(tempFileName, RiderGradePicBuffer[SexType][SpriteKind][GradeKind], &RiderGradePic[SexType][SpriteKind][GradeKind][0][0], EA_LOAD_RIDER_GRADE_SPR_DATA_SUB);
}

int cHorseRider::LoadHorseSprite(int SpriteKind)
{
    char tempFileName[MAX_PATH];
    sprintf(tempFileName, "./char/h%03d.spr", SpriteKind);
    return LoadSpriteSheet(tempFileName, HorsePicBuffer[SpriteKind], &HorsePic[SpriteKind][0][0], EA_LOAD_HORSE_SPR_DATA_SUB);
}

int cHorseRider::LoadHorseSaddleSprite(int SpriteKind)
{
    char tempFileName[MAX_PATH];
    sprintf(tempFileName, "./char/hs%03d.spr", SpriteKind);
    // NOTE: Original code had a bug here, it passed &HorsePic instead of &HorseSaddlePic to g_SDLInfo. This has been corrected.
    return LoadSpriteSheet(tempFileName, HorseSaddlePicBuffer[SpriteKind], &HorseSaddlePic[SpriteKind][0][0], EA_LOAD_HORSE_SADDEL_SPR_DATA_SUB);
}

int cHorseRider::LoadHorseGradeSprite(int HorseKind, int GradeKind)
{
    char tempFileName[MAX_PATH];
    sprintf(tempFileName, "./char/hgr%02d%03d.spr", HorseKind, GradeKind);
    return LoadSpriteSheet(tempFileName, HorseGradePicBuffer[HorseKind][GradeKind], &HorseGradePic[HorseKind][GradeKind][0][0], EA_LOAD_HORSE_GRADE_SPR_DATA_SUB);
}

int cHorseRider::LoadWeaponSprite(int SexType, int SpriteKind)
{
    char tempFileName[MAX_PATH];
    sprintf(tempFileName, "./char/w%03d%03d.spr", SexType, SpriteKind);
    return LoadSpriteSheet(tempFileName, WeaponPicBuffer[SexType][SpriteKind], &WeaponPic[SexType][SpriteKind][0][0], EA_LOAD_WEAPON_SPR_DATA);
}

int cHorseRider::LoadGuardSprite(int SexType, int SpriteKind)
{
    char tempFileName[MAX_PATH];
    // NOTE: The original logic for Guard sprite ignored the 'SpriteKind' parameter. This behavior is preserved.
    sprintf(tempFileName, "./char/w%03d017.spr", SexType);
    return LoadSpriteSheet(tempFileName, GuardPicBuffer[SexType], &GuardPic[SexType][0][0], EA_LOAD_GUARD_SPR_DATA);
}

void cHorseRider::LoadHorseData(LPCHARACTER ch)
{
	if (ch->HorseNo>0)
	{
		int Sex=CheckSex(ch);
		int RiderKind=CheckRiderKind(ch);
		int RiderGrade=CheckRiderGrade(ch);
		int HorseKind=CheckHorseKind(ch);
		int HorseGrade=CheckHorseGrade(ch);
		int WeaponKind=CheckWeaponKind(ch);
		int GuardKind=CheckGuardKind(ch);
		
		if (RiderKind>=0) 
		{
			LoadRiderSprite(Sex,RiderKind);
			LoadRiderBandSprite(Sex,RiderKind);
		}
		if (RiderGrade>=0) LoadRiderGradeSprite(Sex,RiderKind,RiderGrade);
		if (HorseKind>=0) 
		{
			LoadHorseSprite(HorseKind);
			LoadHorseSaddleSprite(HorseKind);
		}
		if (HorseGrade>=0) LoadHorseGradeSprite(HorseKind,HorseGrade);
		if (WeaponKind>=0) LoadWeaponSprite(Sex,WeaponKind);
		if (GuardKind>=0) LoadGuardSprite(Sex,GuardKind);
		if (ch->mantle) LoadRiderMantleSprite(Sex);
	}
}

int	cHorseRider::CheckWeaponSpriteData(int SexType,int SpriteKind)
{
	if (SpriteKind==13)
	{
		if (SexType==HTYPE_MAN)
		{
			if( WeaponPicBuffer[SexType][SpriteKind] != NULL )
			{
				return FALSE;
			}
		}
		else
		{
			if( WeaponPicBuffer[SexType][SpriteKind] != NULL )
			{
				return FALSE;
			}
		}
		
	}
	else
	{
		if( WeaponPicBuffer[SexType][SpriteKind] != NULL )
		{
			return FALSE;
		}
	}
	return TRUE;
}

void cHorseRider::GetWeaponFileName(int SexType,int SpriteKind,char* FileName)
{
	if (SpriteKind==13)
	{
		if (SexType==HTYPE_WOMAN)
		{
			SpriteKind++;
		}
		sprintf(FileName,"./char/w001%03d.spr",SpriteKind);
	}
	else
	{
		sprintf(FileName,"./char/w001%03d.spr",SpriteKind);
	}
}

void cHorseRider::CheckAnimation(LPCHARACTER ch)
{
	int HorseNo=CheckHorseKind(ch);
	if (ch->nCurrentFrame.GetDecrypted()>=HorseRiderAniTable[CheckSex(ch)][HorseNo][ ch->nCurrentAction].nCount )
	{	
		ch->nCurrentFrame.PutEncrypted(0);
	}
}

void cHorseRider::CheckDeathAnimation(LPCHARACTER ch)
{
	int HorseNo=CheckHorseKind(ch);
	if (ch->nCurrentFrame.GetDecrypted()>=HorseRiderAniTable[CheckSex(ch)][HorseNo][ ch->nCurrentAction].nCount )
	{	
		ch->nCurrentFrame.PutEncrypted(HorseRiderAniTable[CheckSex(ch)][HorseNo][ ch->nCurrentAction].nCount-1);
		if (ch->nCurrentAction==ACTION_DIE)
		{
#ifdef _DEBUG
			if( SysInfo.notconectserver )
			{
				if( ch->dieani == 0 ) // ׾..
				{
					ch->diedelay	=  3;	// µ õõ...
					ch->dieani		= 80;	// 32   .
					ch->dieanitype  =  0;	// 
				}
			}
			else	//  
#endif
			{
				if( ch->blood_Set == false && ch->dieani == 0 )
				{
					ch->diedelay	= 3;
					ch->dieani		= 80;
					ch->blood_Set	= true;
				}
			}
		}
	}
}

void cHorseRider::CheckMoveAnimation(LPCHARACTER ch)
{
	int HorseNo=CheckHorseKind(ch);
	if (ch->nCurrentFrame.GetDecrypted()>=HorseRiderAniTable[CheckSex(ch)][HorseNo][ ch->nCurrentAction].nCount )
	{
		if( !IsDead(ch) && (ch->pathcount > 2 || ch->moveFlag == TRUE) )
		{
			CharDoAni_Moving(ch, ch->movetype );
		}
		else
		{
			ch->nCurrentFrame.PutEncrypted(0);
		}
	}
}


void cHorseRider::CheckAttackAnimation(LPCHARACTER ch)
{
	int HorseNo=CheckHorseKind(ch);
	if (ch->nCurrentFrame.GetDecrypted()>=HorseRiderAniTable[CheckSex(ch)][HorseNo][ ch->nCurrentAction].nCount )
	{
		if( !IsDead(ch) && (ch->pathcount > 2 || ch->moveFlag == TRUE) )
		{
			CharDoAni_Moving(ch, ch->movetype );
		}
		else
		{
			ch->changeActionFlag	= 0;
			ch->nCurrentAction		= ch->basicAction;
			ch->nCurrentFrame.PutEncrypted(0);
			ch->offset.x = 0; ch->offset.y = 0;

			if (ch->nActiveCombat.GetDecrypted() == SHARK_MISSILE)
			{
				ch->attackanimationON.PutEncrypted(0);
			}
		}
	}
}

int cHorseRider::GetDistance(LPCHARACTER ch)
{
	int HorseNo=CheckHorseKind(ch);
	return HorseRiderAniTable[CheckSex(ch)][HorseNo][ ch->nCurrentAction].dx[ ch->nCurrentFrame.GetDecrypted()];
}

inline void cHorseRider::FreeRiderSprite(int SexType,int SpriteKind)
{
	MemFree(RiderPicBuffer[SexType][SpriteKind]);
}

inline void cHorseRider::ClearRiderSprite()
{
	for (int i=0;i<HSEX_TYPE_MAX;i++)
	{
		for (int j=0;j<HRIDER_TYPE_MAX;j++)
		{
			FreeRiderSprite(i,j);
		}
	}
}

inline void cHorseRider::FreeRiderBandSprite(int SexType,int SpriteKind)
{
	MemFree(RiderBandPicBuffer[SexType][SpriteKind]);
}

inline void cHorseRider::ClearRiderBandSprite()
{
	for (int i=0;i<HSEX_TYPE_MAX;i++)
	{
		for (int j=0;j<HRIDER_TYPE_MAX;j++)
		{
			FreeRiderBandSprite(i,j);
		}
	}
}


inline void cHorseRider::FreeHorseSprite(int SpriteKind)
{
	MemFree(HorsePicBuffer[SpriteKind]);
}

inline void cHorseRider::ClearHorseSprite()
{
	for (int i=0;i<HHORSE_TYPE_MAX;i++)
	{
		FreeHorseSprite(i);
	}
}

inline void cHorseRider::FreeHorseSaddleSprite(int SpriteKind)
{
	MemFree(HorseSaddlePicBuffer[SpriteKind]);
}

inline void cHorseRider::ClearHorseSaddleSprite()
{
	for (int i=0;i<HHORSE_TYPE_MAX;i++)
	{
		FreeHorseSaddleSprite(i);
	}
}

inline void cHorseRider::FreeWeaponSprite(int SexType,int SpriteKind)
{
	MemFree(WeaponPicBuffer[SexType][SpriteKind]);
}

inline void cHorseRider::ClearWeaponSprite()
{
//	for (int i=0;i<HSEX_TYPE_MAX;i++)
//	{
		for (int j=0;j<HWEAPON_TYPE_MAX;j++)
		{
			FreeWeaponSprite(0,j);
			FreeWeaponSprite(1,j);
		}
//	}
}

inline void cHorseRider::FreeGuardSprite(int SexType,int SpriteKind)
{
	MemFree(GuardPicBuffer[SexType]);
}

inline void cHorseRider::ClearGuardSprite()
{
//	for (int i=0;i<HSEX_TYPE_MAX;i++)
//	{
//		for (int j=0;j<HGUARD_TYPE_MAX;j++)
//		{
			//FreeGuardSprite(i,j);
	FreeGuardSprite(0,0);
//		}
//	}
}

inline void cHorseRider::FreeRiderGradeSprite(int SexType,int SpriteKind,int GradeType)
{
	MemFree(RiderGradePicBuffer[SexType][SpriteKind][GradeType]);
}

inline void cHorseRider::ClearRiderGradeSprite()
{
	for (int i=0;i<HSEX_TYPE_MAX;i++)
	{
		for (int j=0;j<HHORSE_TYPE_MAX;j++)
		{
			for (int k=0;k<HGRADE_MAX;k++)
			{
				FreeRiderGradeSprite(i,j,k);
			}
		}
	}
}

inline void cHorseRider::FreeHorseGradeSprite(int SpriteKind,int GradeType)
{
	MemFree(HorseGradePicBuffer[SpriteKind][GradeType]);
}

inline void cHorseRider::ClearHorseGradeSprite()
{
	for (int i=0;i<HHORSE_TYPE_MAX;i++)
	{
		for (int j=0;j<HHORSE_GRADE_MAX;j++)
		{
			FreeHorseGradeSprite(i,j);
		}
	}
}

inline void cHorseRider::FreeRiderMantleSprite(int SexType)
{
	MemFree(RiderMantlePicBuffer[SexType]);

}
inline void	cHorseRider::ClearRiderMantleSprite()
{
	FreeRiderMantleSprite(0);
	FreeRiderMantleSprite(1);
}


void cHorseRider::ClearSpriteAll()
{
	ClearRiderSprite();
	ClearHorseSprite();
	ClearWeaponSprite();
	ClearGuardSprite();
	ClearRiderGradeSprite();
	ClearHorseGradeSprite();
	ClearRiderMantleSprite();
}

inline int cHorseRider::CheckSex(LPCHARACTER ch)
{
	return ch->sprno;
}

inline int cHorseRider::CheckRiderKind(LPCHARACTER ch)			// ü 
{
	return ch->RiderNo-1;
}

inline int cHorseRider::CheckHorseKind(LPCHARACTER ch)			// ü
{
	switch (ch->HorseNo)
	{
	case 0 :
	case 1 :
	case 2 :
	case 3 : return ch->HorseNo-1; break;
	default : return 0;
	}
}

inline int cHorseRider::CheckWeaponKind(LPCHARACTER ch)		// ü
{	//< CSD-031020
	int	Accessory=ch->accessory[2];						// 61  ۹ȣ 

	if (Accessory>=61&&Accessory<93)
	{
		Accessory-=61;										// 0 
	}
	else
	if (Accessory>=121&&Accessory<127)
	{
		Accessory-=121;										// 0
		Accessory+=32;										//  ߵ
	}
	else if (Accessory >= 160 && Accessory < 170)
	{
		Accessory -= 160;
		return HorseEffectWeaponConvertTable[CheckSex(ch)][Accessory];
	}
	else if (Accessory >= 170 && Accessory < 180)
	{
		Accessory -= 170;
		return HorseEffectWeaponConvertTable[CheckSex(ch)][Accessory];
	}
	else 
	{
		return -1;
	}

	return HorseWeaponConvertTable[CheckSex(ch)][Accessory];
}	//> CSD-031020

inline int cHorseRider::CheckGuardKind(LPCHARACTER ch)		// ü
{
	int	Accessory=ch->accessory[3]-87;						// 87  ۹ȣ 

/*	if (Accessory<0||Accessory>6) 
	{
		MessageBox(NULL,"Error!!","Shield Sprite Number Error...",MB_OK);
		return 0;
	}*/
	if (Accessory<0) return -1;
	return 0;
}

inline int cHorseRider::CheckRiderGrade(LPCHARACTER ch)		// ü
{
	return ch->RiderGrade-1;
}

inline int cHorseRider::CheckHorseGrade(LPCHARACTER ch)		// ü
{
	return ch->HorseGrade-1;
}

inline Spr* cHorseRider::GetSpriteData(int SexType,int SpriteType,int SpriteKind,int Direction,int Frame)
{
	switch(SpriteType)
	{
	case HSRIDER	: return &RiderPic[SexType][SpriteKind][Direction][Frame];
	case HSRBAND	: return &RiderBandPic[SexType][SpriteKind][Direction][Frame];
	case HSHORSE	: return &HorsePic[SpriteKind][Direction][Frame];
	case HSSADDLE	: return &HorseSaddlePic[SpriteKind][Direction][Frame];
	case HSWEAPON	: return &WeaponPic[SexType][SpriteKind][Direction][Frame];
	case HSSHEILD	: return &GuardPic[SexType][Direction][Frame];
	}
	return NULL;
}

inline void cHorseRider::PutConfusion(LPCHARACTER ch)
{
	DWORD Color1 = ReturnBlendRGB( 255, 0, 0);
	DWORD Color2 = ReturnBlendRGB( 255, 100, 100);

	DrawRGBColor(ch,Color1,Color2);
}

inline void cHorseRider::PutCurse(LPCHARACTER ch)
{
	int Trans=5;
	int Operation=2;

	DrawFX(ch,Trans,Operation);
}

inline void cHorseRider::PutFrozen(LPCHARACTER ch)
{
	DWORD Color1 = ReturnBlendRGB(0, 0, 255);
	DWORD Color2 = ReturnBlendRGB(100, 100, 255);

	DrawRGBColor(ch,Color1,Color2);
}

inline void cHorseRider::PutGhost(LPCHARACTER ch)
{
	int Trans=10;
	int Operation=1;

	DrawFX(ch,Trans,Operation);
}

inline void cHorseRider::PutGreen(LPCHARACTER ch)
{
	DWORD	//color = ReturnBlendRGB( 116,254,178 );
			//color = ReturnBlendRGB( 192,251,148 );
			Color2 = ReturnBlendRGB( 125,251,226 );

	DrawRGBColor(ch,0,Color2);
}

inline void cHorseRider::PutPoison(LPCHARACTER ch)
{
	DWORD Color1 = ReturnBlendRGB( 0,255, 0);
	DWORD Color2 = ReturnBlendRGB( 100, 255, 100);

	DrawRGBColor(ch,Color1,Color2);
}

inline void cHorseRider::PutRed(LPCHARACTER ch)
{
	DWORD Color2 = ReturnBlendRGB( 255,100,100 );

	DrawRGBColor(ch,0,Color2);
}

inline void cHorseRider::PutSlow(LPCHARACTER ch)
{
	DWORD Color1 = ReturnBlendRGB( 255, 255, 0);
	DWORD Color2 = ReturnBlendRGB( 255, 255, 100);

	DrawRGBColor(ch,Color1,Color2);
}

inline void cHorseRider::PutStone(LPCHARACTER ch)
{
	DWORD Color1 = ReturnBlendRGB( 100, 100, 100);
	DWORD Color2 = ReturnBlendRGB( 100, 100, 100);

	DrawRGBColor(ch,Color1,Color2);
}

inline void cHorseRider::PutTransparency(LPCHARACTER ch)
{
	if (ch==Hero)
	{
		int Trans=6;
		int Operation=1;

		DrawFX(ch,Trans,Operation);
	}
	else
	{
		DrawVoid(ch);
	}
}

inline void cHorseRider::DrawRider(LPCHARACTER ch)			// LTS HORSERIDER ADD
{
	int RiderGrade=CheckRiderGrade(ch);
	int	SexType=CheckSex(ch);
	int RiderKind=CheckRiderKind(ch);		
	//int LocateX=ch->x-Mapx;
	//int LocateY=ch->y-Mapy;
	int LocateX = ch->visual_x - Mapx;
	int LocateY = ch->visual_y - Mapy;
	int HorseNo=CheckHorseKind(ch);
	int Frame=HorseRiderAniTable[CheckSex(ch)][HorseNo][ch->nCurrentAction].cFrame[ch->nCurrentFrame.GetDecrypted()];

	Spr* RiderSpr=GetSpriteData(SexType,HSRIDER,RiderKind,ch->direction,Frame);
	Spr* RiderBandSpr=GetSpriteData(SexType,HSRBAND,RiderKind,ch->direction,Frame);
	Spr* RiderGradeSpr=NULL;
	Spr* RiderMantleSpr=NULL;

	if (ch->mantlecolor!=0&&ch->mantlecolor!=65535) RiderMantleSpr=&RiderMantlePic[SexType][ch->direction][Frame];
	
	if (RiderGrade>=0) RiderGradeSpr=&RiderGradePic[SexType][RiderKind][RiderGrade][ch->direction][Frame];	// 꿡  .

	if (ch->direction==DIRECTION_DOWN||ch->direction==DIRECTION_RIGHTDOWN||ch->direction==DIRECTION_LEFTDOWN)
		PutCompressedImageCharRGB(LocateX,LocateY,RiderMantleSpr,ch->mantlecolor);
	//PutCompressedImage(LocateX,LocateY,RiderMantleSpr);
	PutCompressedImageCharRGB(LocateX,LocateY,RiderMantleSpr,ch->mantlecolor);
	PutCompressedImage(LocateX,LocateY,RiderSpr);
	PutCompressedImageCharRGB(LocateX,LocateY,RiderBandSpr, ch->bodycolor);
	PutCompressedImage(LocateX,LocateY,RiderGradeSpr);
	if (ch->direction!=DIRECTION_DOWN&&ch->direction!=DIRECTION_RIGHTDOWN&&ch->direction!=DIRECTION_LEFTDOWN)
		PutCompressedImageCharRGB(LocateX,LocateY,RiderMantleSpr,ch->mantlecolor);
	PutCompressedImageCharRGB(LocateX,LocateY,RiderMantleSpr,ch->mantlecolor);
	//PutCompressedImage(LocateX,LocateY,RiderMantleSpr);
}

inline void cHorseRider::DrawRiderColor(LPCHARACTER ch,DWORD Color1,DWORD Color2)
{
	int RiderGrade=CheckRiderGrade(ch);
	int	SexType=CheckSex(ch);
	int RiderKind=CheckRiderKind(ch);		
	//int LocateX=ch->x-Mapx;
	//int LocateY=ch->y-Mapy;
	int LocateX = ch->visual_x - Mapx;
	int LocateY = ch->visual_y - Mapy;
	int HorseNo=CheckHorseKind(ch);
	int Frame=HorseRiderAniTable[CheckSex(ch)][HorseNo][ch->nCurrentAction].cFrame[ch->nCurrentFrame.GetDecrypted()];


	Spr* RiderSpr=GetSpriteData(SexType,HSRIDER,RiderKind,ch->direction,Frame);
	Spr* RiderGradeSpr=NULL;
	Spr* RiderMantleSpr=NULL;

	if (ch->mantlecolor!=0&&ch->mantlecolor!=65535) RiderMantleSpr=&RiderMantlePic[SexType][ch->direction][Frame];
	if (RiderGrade>=0) RiderGradeSpr=&RiderGradePic[SexType][RiderKind][RiderGrade][ch->direction][Frame];	// 꿡  .

	if (ch->direction==DIRECTION_DOWN||ch->direction==DIRECTION_RIGHTDOWN||ch->direction==DIRECTION_LEFTDOWN)
		PutCompressedImageCharRGB(LocateX,LocateY,RiderMantleSpr,Color2);
	PutCompressedImageCharRGB(LocateX,LocateY,RiderSpr,Color2);
	PutCompressedImageCharRGB(LocateX,LocateY,RiderGradeSpr,Color2);
	if (ch->direction!=DIRECTION_DOWN&&ch->direction!=DIRECTION_RIGHTDOWN&&ch->direction!=DIRECTION_LEFTDOWN)
		PutCompressedImageCharRGB(LocateX,LocateY,RiderMantleSpr,Color2);
}

inline void cHorseRider::DrawRiderFX(LPCHARACTER ch,int Trans,int Operation)	// LTS HORSERIDER ADD	
{
	int RiderGrade=CheckRiderGrade(ch);
	int SexType=CheckSex(ch);
	int RiderKind=CheckRiderKind(ch);		
	//int LocateX=ch->x-Mapx;
	//int LocateY=ch->y-Mapy;
	int LocateX = ch->visual_x - Mapx;
	int LocateY = ch->visual_y - Mapy;
	int HorseNo=CheckHorseKind(ch);
	int Frame=HorseRiderAniTable[CheckSex(ch)][HorseNo][ch->nCurrentAction].cFrame[ch->nCurrentFrame.GetDecrypted()];

	Spr* RiderSpr=GetSpriteData(SexType,HSRIDER,RiderKind,ch->direction,Frame);
	Spr* RiderGradeSpr=NULL;
	Spr* RiderMantleSpr=NULL;

	if (ch->mantlecolor!=0&&ch->mantlecolor!=65535) RiderMantleSpr=&RiderMantlePic[SexType][ch->direction][Frame];
	if (RiderGrade>=0) RiderGradeSpr=&RiderGradePic[SexType][RiderKind][RiderGrade][ch->direction][Frame];	// 꿡  .

	if (ch->direction==DIRECTION_DOWN||ch->direction==DIRECTION_RIGHTDOWN||ch->direction==DIRECTION_LEFTDOWN)
		PutCompressedImageFX(LocateX,LocateY,RiderMantleSpr,Trans,Operation);
	PutCompressedImageFX(LocateX,LocateY,RiderSpr,Trans,Operation);
	PutCompressedImageFX(LocateX,LocateY,RiderGradeSpr,Trans,Operation);
}

inline void cHorseRider::DrawRiderVoid(LPCHARACTER ch)
{
	int  RiderGrade=CheckRiderGrade(ch);
	int	SexType=CheckSex(ch);
	int RiderKind=CheckRiderKind(ch);
	//int LocateX=ch->x-Mapx;
	//int LocateY=ch->y-Mapy;
	int LocateX = ch->visual_x - Mapx;
	int LocateY = ch->visual_y - Mapy;
	int HorseNo=CheckHorseKind(ch);
	int Frame=HorseRiderAniTable[CheckSex(ch)][HorseNo][ch->nCurrentAction].cFrame[ch->nCurrentFrame.GetDecrypted()];


	Spr* RiderSpr=GetSpriteData(SexType,HSRIDER,RiderKind,ch->direction,Frame);
	Spr* RiderGradeSpr=&RiderGradePic[RiderGrade][SexType][RiderKind][ch->direction][Frame];	// 꿡  .

	PutCompressedImageVoid(LocateX+(rand()%5-2),LocateY+(rand()%5-2),RiderSpr);
	PutCompressedImageVoid(LocateX+(rand()%5-2),LocateY+(rand()%5-2),RiderGradeSpr);
}

inline void cHorseRider::DrawHorse(LPCHARACTER ch)
{
	int HorseGrade=CheckHorseGrade(ch);
	int HorseKind=CheckHorseKind(ch);
	//int LocateX=ch->x-Mapx;
	//int LocateY=ch->y-Mapy;
	int LocateX = ch->visual_x - Mapx;
	int LocateY = ch->visual_y - Mapy;
	int Frame=HorseRiderAniTable[CheckSex(ch)][HorseKind][ch->nCurrentAction].cFrame[ch->nCurrentFrame.GetDecrypted()];

	Spr* HorseSpr=GetSpriteData(0,HSHORSE,HorseKind,ch->direction,Frame);
	Spr* HorseSaddleSpr=GetSpriteData(0,HSSADDLE,HorseKind,ch->direction,Frame);
	Spr* HorseGradeSpr=NULL;

	if (HorseGrade>=0) HorseGradeSpr=&HorseGradePic[HorseKind][HorseGrade][ch->direction][Frame];

	switch (HorseKind)
	{
	case 0 :
	case 1 :	PutCompressedImageShadow(LocateX,LocateY,HorseSpr);
				PutCompressedImage(LocateX,LocateY,HorseSpr);
				PutCompressedImage(LocateX,LocateY,HorseSaddleSpr);
				PutCompressedImage(LocateX,LocateY,HorseGradeSpr);
				break;
	case 2 :
				PutCompressedImageFX(LocateX,LocateY,HorseSpr,3,2);	
				PutCompressedImage(LocateX,LocateY,HorseSaddleSpr);
				PutCompressedImage(LocateX,LocateY,HorseGradeSpr);
				break;
	}
}

inline void cHorseRider::DrawHorseColor(LPCHARACTER ch,DWORD Color1,DWORD Color2)
{
	int HorseGrade=CheckHorseGrade(ch);
	int HorseKind=CheckHorseKind(ch);
	//int LocateX=ch->x-Mapx;
	//int LocateY=ch->y-Mapy;
	int LocateX = ch->visual_x - Mapx;
	int LocateY = ch->visual_y - Mapy;
	int Frame=HorseRiderAniTable[CheckSex(ch)][HorseKind][ch->nCurrentAction].cFrame[ch->nCurrentFrame.GetDecrypted()];

	Spr* HorseSpr=GetSpriteData(0,HSHORSE,HorseKind,ch->direction,Frame);
	Spr* HorseGradeSpr=NULL;

	if (HorseGrade>=0) HorseGradeSpr=&HorseGradePic[HorseKind][HorseGrade][ch->direction][Frame];

	PutCompressedImageShadow(LocateX,LocateY,HorseSpr);
	PutCompressedImageCharRGB(LocateX,LocateY,HorseSpr,Color2);
	PutCompressedImageCharRGB(LocateX,LocateY,HorseGradeSpr,Color2);
}

inline void cHorseRider::DrawHorseFX(LPCHARACTER ch,int Trans,int Operation)
{
	int HorseGrade=CheckHorseGrade(ch);
	int HorseKind=CheckHorseKind(ch);
	//int LocateX=ch->x-Mapx;
	//int LocateY=ch->y-Mapy;
	int LocateX = ch->visual_x - Mapx;
	int LocateY = ch->visual_y - Mapy;
	int Frame=HorseRiderAniTable[CheckSex(ch)][HorseKind][ch->nCurrentAction].cFrame[ch->nCurrentFrame.GetDecrypted()];

	Spr* HorseSpr=GetSpriteData(0,HSHORSE,HorseKind,ch->direction,Frame);
	Spr* HorseGradeSpr=NULL;

	if (HorseGrade>=0) HorseGradeSpr=&HorseGradePic[HorseKind][HorseGrade][ch->direction][Frame];

	PutCompressedImageFX(LocateX,LocateY,HorseSpr,Trans,Operation);
	PutCompressedImageFX(LocateX,LocateY,HorseGradeSpr,Trans,Operation);
}

inline void cHorseRider::DrawHorseVoid(LPCHARACTER ch)
{
	int HorseGrade=CheckHorseGrade(ch);
	int HorseKind=CheckHorseKind(ch);
	//int LocateX=ch->x-Mapx;
	//int LocateY=ch->y-Mapy;
	int LocateX = ch->visual_x - Mapx;
	int LocateY = ch->visual_y - Mapy;
	int Frame=HorseRiderAniTable[CheckSex(ch)][HorseKind][ch->nCurrentAction].cFrame[ch->nCurrentFrame.GetDecrypted()];

	Spr* HorseSpr=GetSpriteData(0,HSHORSE,HorseKind,ch->direction,Frame);
	Spr* HorseGradeSpr=&HorseGradePic[HorseKind][HorseGrade][ch->direction][Frame];

	PutCompressedImageVoid(LocateX+(rand()%5-2),LocateY+(rand()%5-2),HorseSpr);
	PutCompressedImageVoid(LocateX+(rand()%5-2),LocateY+(rand()%5-2),HorseGradeSpr);
}

inline void cHorseRider::DrawFX(LPCHARACTER ch,int Trans,int Operation)
{
	int SexType=CheckSex(ch);
	int WeaponKind=CheckWeaponKind(ch);
	int GuardKind=CheckGuardKind(ch);
	//int LocateX=ch->x-Mapx;
	//int LocateY=ch->y-Mapy;
	int LocateX = ch->visual_x - Mapx;
	int LocateY = ch->visual_y - Mapy;
	int HorseNo=CheckHorseKind(ch);
	int Frame=HorseRiderAniTable[CheckSex(ch)][HorseNo][ch->nCurrentAction].cFrame[ch->nCurrentFrame.GetDecrypted()];
	Spr* Weapon=NULL;
	Spr* Guard=NULL;
	
	if (ch->peacests==1)
	{
		if (WeaponKind>=0) Weapon=GetSpriteData(SexType,HSWEAPON,WeaponKind,ch->direction,Frame);
		if (GuardKind>=0) Guard=GetSpriteData(SexType,HSSHEILD,GuardKind,ch->direction,Frame);
	}

	int tempDirection=ch->direction;
	if (WeaponKind==5 || WeaponKind==13)			// հ  ȱⰡ  ö´.
	{
		tempDirection=DIRECTION_UP;
	}

	switch (tempDirection)
	{
	case DIRECTION_UP			:		
									DrawHorseFX(ch,Trans,Operation);
									PutCompressedImageFX(LocateX,LocateY,Guard,Trans,Operation);
									DrawRiderFX(ch,Trans,Operation);
									PutCompressedImageFX(LocateX,LocateY,Weapon,Trans,Operation);	break;
	case DIRECTION_RIGHTUP		:
	case DIRECTION_RIGHT		:
	case DIRECTION_RIGHTDOWN	:
									PutCompressedImageFX(LocateX,LocateY,Guard,Trans,Operation);
									DrawHorseFX(ch,Trans,Operation);
									DrawRiderFX(ch,Trans,Operation);
									PutCompressedImageFX(LocateX,LocateY,Weapon,Trans,Operation);	break;
	case DIRECTION_DOWN			:		
									DrawHorseFX(ch,Trans,Operation);
									PutCompressedImageFX(LocateX,LocateY,Weapon,Trans,Operation);
									DrawRiderFX(ch,Trans,Operation);
									PutCompressedImageFX(LocateX,LocateY,Guard,Trans,Operation);	break;
	case DIRECTION_LEFTDOWN		:		
	case DIRECTION_LEFT			:
	case DIRECTION_LEFTUP		:		
									PutCompressedImageFX(LocateX,LocateY,Weapon,Trans,Operation);
									DrawHorseFX(ch,Trans,Operation);
									DrawRiderFX(ch,Trans,Operation);
									PutCompressedImageFX(LocateX,LocateY,Guard,Trans,Operation);	break;
	}
}

inline void cHorseRider::DrawRGBColor(LPCHARACTER ch,DWORD Color1,DWORD Color2)
{
	int SexType=CheckSex(ch);
	int WeaponKind=CheckWeaponKind(ch);
	int GuardKind=CheckGuardKind(ch);
	//int LocateX=ch->x-Mapx;
	//int LocateY=ch->y-Mapy;
	int LocateX = ch->visual_x - Mapx;
	int LocateY = ch->visual_y - Mapy;
	int HorseNo=CheckHorseKind(ch);
	int Frame=HorseRiderAniTable[CheckSex(ch)][HorseNo][ch->nCurrentAction].cFrame[ch->nCurrentFrame.GetDecrypted()];
	Spr* Weapon=NULL;
	Spr* Guard=NULL;

	if (ch->peacests==1)
	{
		if (WeaponKind>=0) Weapon=GetSpriteData(SexType,HSWEAPON,WeaponKind,ch->direction,Frame);
		if (GuardKind>=0) Guard=GetSpriteData(SexType,HSSHEILD,GuardKind,ch->direction,Frame);
	}

	int tempDirection=ch->direction;
	if (WeaponKind==5 || WeaponKind==13)			// հ  ȱⰡ  ö´.
	{
		tempDirection=DIRECTION_UP;
	}

	switch (tempDirection)
	{
	case DIRECTION_UP			:	
									DrawHorseColor(ch,Color1,Color2);
									PutCompressedImageCharRGB(LocateX,LocateY,Guard,Color2);
									DrawRiderColor(ch,Color1,Color2);
									PutCompressedImageCharRGB(LocateX,LocateY,Weapon,Color2);		break;
	case DIRECTION_RIGHTUP		:
	case DIRECTION_RIGHT		:
	case DIRECTION_RIGHTDOWN	:
									PutCompressedImageCharRGB(LocateX,LocateY,Guard,Color2);
									DrawHorseColor(ch,Color1,Color2);
									DrawRiderColor(ch,Color1,Color2);
									PutCompressedImageCharRGB(LocateX,LocateY,Weapon,Color2);		break;
	case DIRECTION_DOWN			:	
									DrawHorseColor(ch,Color1,Color2);
									PutCompressedImageCharRGB(LocateX,LocateY,Weapon,Color2); 
									DrawRiderColor(ch,Color1,Color2);
									PutCompressedImageCharRGB(LocateX,LocateY,Guard,Color2);		break;
	case DIRECTION_LEFTDOWN		:	
	case DIRECTION_LEFT			:
	case DIRECTION_LEFTUP		:		
									PutCompressedImageCharRGB(LocateX,LocateY,Weapon,Color2); 
									DrawHorseColor(ch,Color1,Color2);
									DrawRiderColor(ch,Color1,Color2);
									PutCompressedImageCharRGB(LocateX,LocateY,Guard,Color2);		break;
	}
}

inline void cHorseRider::DrawVoid(LPCHARACTER ch)
{
	int SexType=CheckSex(ch);
	int WeaponKind=CheckWeaponKind(ch);
	int GuardKind=CheckGuardKind(ch);
	//int LocateX=ch->x-Mapx;
	//int LocateY=ch->y-Mapy;
	int LocateX = ch->visual_x - Mapx;
	int LocateY = ch->visual_y - Mapy;
	int HorseNo=CheckHorseKind(ch);
	int Frame=HorseRiderAniTable[SexType][HorseNo][ch->nCurrentAction].cFrame[ch->nCurrentFrame.GetDecrypted()];
	Spr* Weapon=NULL;
	Spr* Guard=NULL;

	if (ch->peacests==1)
	{
		if (WeaponKind>=0) Weapon=GetSpriteData(SexType,HSWEAPON,WeaponKind,ch->direction,Frame);
		if (GuardKind>=0) Guard=GetSpriteData(SexType,HSSHEILD,GuardKind,ch->direction,Frame);
	}

	int tempDirection=ch->direction;
	if (WeaponKind==5 || WeaponKind==13)			// հ  ȱⰡ  ö´.
	{
		tempDirection=DIRECTION_UP;
	}

	switch (ch->direction)
	{
	case DIRECTION_UP			:	
									DrawHorseVoid(ch);
									PutCompressedImageVoid(LocateX+(rand()%5-2),LocateY+(rand()%5-2),Guard);
									DrawRiderVoid(ch);
									PutCompressedImageVoid(LocateX+(rand()%5-2),LocateY+(rand()%5-2),Weapon);		break;
	case DIRECTION_RIGHTUP		:
	case DIRECTION_RIGHT		:
	case DIRECTION_RIGHTDOWN	:
									PutCompressedImageVoid(LocateX+(rand()%5-2),LocateY+(rand()%5-2),Guard);
									DrawHorseVoid(ch);
									DrawRiderVoid(ch);
									PutCompressedImageVoid(LocateX+(rand()%5-2),LocateY+(rand()%5-2),Weapon);		break;
	case DIRECTION_DOWN			:	
									DrawHorseVoid(ch);
									PutCompressedImageVoid(LocateX+(rand()%5-2),LocateY+(rand()%5-2),Weapon); 
									DrawRiderVoid(ch);
									PutCompressedImageVoid(LocateX+(rand()%5-2),LocateY+(rand()%5-2),Guard);		break;
	case DIRECTION_LEFTDOWN		:	
	case DIRECTION_LEFT			:
	case DIRECTION_LEFTUP		:	
									PutCompressedImageVoid(LocateX+(rand()%5-2),LocateY+(rand()%5-2),Weapon); 
									DrawHorseVoid(ch);
									DrawRiderVoid(ch);
									PutCompressedImageVoid(LocateX+(rand()%5-2),LocateY+(rand()%5-2),Guard);		break;
	}
}

inline void cHorseRider::Draw1(LPCHARACTER ch,int LocateX,int LocateY,Spr* First,Spr* Second)
{
	DrawHorse(ch);
	PutCompressedImage(LocateX,LocateY,First);
	DrawRider(ch);
	PutCompressedImage(LocateX,LocateY,Second);
}
inline void cHorseRider::DrawRGB1(LPCHARACTER ch,int LocateX,int LocateY,Spr* First,Spr* Second,DWORD Color)
{
	DrawHorseColor(ch,0,Color);
	PutCompressedImageCharRGB(LocateX,LocateY,First,Color);
	DrawRiderColor(ch,0,Color);
	PutCompressedImageCharRGB(LocateX,LocateY,Second,Color);
}

inline void cHorseRider::Draw2(LPCHARACTER ch,int LocateX,int LocateY,Spr* First,Spr* Second)
{
	PutCompressedImage(LocateX,LocateY,First);
	DrawHorse(ch);
	DrawRider(ch);
	PutCompressedImage(LocateX,LocateY,Second);
}
inline void cHorseRider::DrawRGB2(LPCHARACTER ch,int LocateX,int LocateY,Spr* First,Spr* Second,DWORD Color)
{
	PutCompressedImageCharRGB(LocateX,LocateY,First,Color);
	DrawHorseColor(ch,0,Color);
	DrawRiderColor(ch,0,Color);
	PutCompressedImageCharRGB(LocateX,LocateY,Second,Color);
}

inline int cHorseRider::DirectionChange(LPCHARACTER ch,int WeaponKind)
{
	if (!CheckSex(ch)) return ch->direction;
	if (WeaponKind==5||WeaponKind==13) // հ  ȱⰡ  ö´.
	{
		switch(ch->direction)
		{
		case DIRECTION_UP			:	// Į  ߿ ׸.
		case DIRECTION_LEFT			:	// Į  ߿ ׸.
		case DIRECTION_LEFTUP		:	// Į  ߿ ׸
		case DIRECTION_RIGHTUP		:	// Į  ߿ ׸.
		case DIRECTION_RIGHT		:	// Į  ߿ ׸.
										return DIRECTION_UP;
										break;
		case DIRECTION_DOWN			:	// Į  ׸.
		case DIRECTION_RIGHTDOWN	:	// Į   ׸.
		case DIRECTION_LEFTDOWN		:	// Į   ׸.
										return DIRECTION_DOWN;
										break;
		}
	}
	else return ch->direction;
	return ch->direction;
}

inline void cHorseRider::DrawAction(LPCHARACTER ch)
{
	if( (ch->blood_Set == true || ch->dieani!=0 ) && ch->nCurrentAction==ACTION_DIE ) PutDeadEffectBottom(ch);

	int SexType=CheckSex(ch);
	int WeaponKind=CheckWeaponKind(ch);
	int GuardKind=CheckGuardKind(ch);
	//int LocateX = ch->x - Mapx;
	//int LocateY = ch->y - Mapy; 
	int LocateX = ch->visual_x - Mapx;
	int LocateY = ch->visual_y - Mapy;
	int HorseNo=CheckHorseKind(ch);
	int Frame=HorseRiderAniTable[SexType][HorseNo][ch->nCurrentAction].cFrame[ch->nCurrentFrame.GetDecrypted()];
	Spr* Weapon=NULL;
	Spr* Guard=NULL;
	DWORD Color=0;

	if (ch->peacests==1)
	{
		if (WeaponKind>=0) Weapon=GetSpriteData(SexType,HSWEAPON,WeaponKind,ch->direction,Frame);
		if (GuardKind>=0) Guard=GetSpriteData(SexType,HSSHEILD,GuardKind,ch->direction,Frame);
	}

	int tempDirection=DirectionChange(ch,WeaponKind);							// հ  ȱⰡ  ö´.	

	if (ch==g_GameInfo.lpcharacter)
	{
		if (ch==Hero) Color=ReturnBlendRGB( 125,251,226 );
		else Color=ReturnBlendRGB( 255,100,100 );
	}

	switch (tempDirection)							// ж   ޸Ѵ.
	{
	case DIRECTION_UP			:	
									if (Color) DrawRGB1(ch,LocateX,LocateY,Guard,Weapon,Color);
									else Draw1(ch,LocateX,LocateY,Guard,Weapon);
									break;
	case DIRECTION_RIGHTUP		:
	case DIRECTION_RIGHT		:
	case DIRECTION_RIGHTDOWN	:
									if (Color) DrawRGB2(ch,LocateX,LocateY,Guard,Weapon,Color);
									else Draw2(ch,LocateX,LocateY,Guard,Weapon);
									break;
	case DIRECTION_DOWN			:	
									if (Color) DrawRGB1(ch,LocateX,LocateY,Weapon,Guard,Color);
									else Draw1(ch,LocateX,LocateY,Weapon,Guard);
									break;
	case DIRECTION_LEFTDOWN		:	
	case DIRECTION_LEFT			:
	case DIRECTION_LEFTUP		:		
									if (Color) DrawRGB2(ch,LocateX,LocateY,Weapon,Guard,Color);
									else Draw2(ch,LocateX,LocateY,Weapon,Guard);
									break;
	}
}

void cHorseRider::PreloadAllHorseSprites()
{
    // Preload Horse and Saddle Sprites
    for (int i = 0; i < HHORSE_TYPE_MAX; ++i)
    {
        LoadHorseSprite(i);
        LoadHorseSaddleSprite(i);
    }

    // Preload Rider and Rider Band Sprites
    for (int sex = 0; sex < HSEX_TYPE_MAX; ++sex)
    {
        for (int kind = 0; kind < HRIDER_TYPE_MAX; ++kind)
        {
            LoadRiderSprite(sex, kind);
            LoadRiderBandSprite(sex, kind);
        }
    }

    // Preload Weapon Sprites
    for (int sex = 0; sex < HSEX_TYPE_MAX; ++sex)
    {
        for (int kind = 0; kind < HWEAPON_TYPE_MAX; ++kind)
        {
            LoadWeaponSprite(sex, kind);
        }
    }

    // Preload Guard (Shield) Sprites
    for (int sex = 0; sex < HSEX_TYPE_MAX; ++sex)
    {
        // The SpriteKind parameter is ignored in the original function, so we pass 0.
        LoadGuardSprite(sex, 0);
    }

    // Preload Rider Mantle Sprites
    for (int sex = 0; sex < HSEX_TYPE_MAX; ++sex)
    {
        LoadRiderMantleSprite(sex);
    }

    // Preload Rider Grade Sprites
    for (int sex = 0; sex < HSEX_TYPE_MAX; ++sex)
    {
        for (int kind = 0; kind < HRIDER_TYPE_MAX; ++kind)
        {
            for (int grade = 0; grade < HGRADE_MAX; ++grade)
            {
                LoadRiderGradeSprite(sex, kind, grade);
            }
        }
    }

    // Preload Horse Grade Sprites
    for (int kind = 0; kind < HHORSE_TYPE_MAX; ++kind)
    {
      for (int grade = 0; grade < HGRADE_MAX; ++grade)
        {
            LoadHorseGradeSprite(kind, grade);
        }
    }
}

int cHorseRider::GetWaveNo(LPCHARACTER ch)
{
	int HorseNo=CheckHorseKind(ch);
	return HorseRiderAniTable[CheckSex(ch)][HorseNo][ch->nCurrentAction].wave[ch->nCurrentFrame.GetDecrypted()];
}

int cHorseRider::GetEffectWaveNo(LPCHARACTER ch)
{
	int HorseNo=CheckHorseKind(ch);
	return HorseRiderAniTable[CheckSex(ch)][HorseNo][ch->nCurrentAction].effect[ch->nCurrentFrame.GetDecrypted()];
}

void cHorseRider::DrawHorseRider(LPCHARACTER ch)
{
	switch (ch->viewtype)
	{	//< CSD-030509
	case VIEWTYPE_NORMAL_			: DrawAction(ch);					break;
	case VIEWTYPE_CONFUSION_		: PutConfusion(ch);					break;
	case VIEWTYPE_POISON_			: PutPoison(ch);					break;
	case VIEWTYPE_STONE_			: PutStone(ch);						break;
	case VIEWTYPE_SLOW_				: PutSlow(ch);						break;
	case VIEWTYPE_FROZEN_			: PutFrozen(ch);					break;
	case VIEWTYPE_CURSE_			: PutCurse(ch);						break;
	case VIEWTYPE_TRANSPARENCY_		: PutTransparency(ch);				break;
	case VIEWTYPE_GHOST_			: PutGhost(ch);						break;
	}	//> CSD-030509	
}