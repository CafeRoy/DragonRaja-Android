#include "stdafx.h"
#include "mytypes.h"	// LTS NEW LOCALWAR
#include "gameproc.h"
#include "network.h"
#include "menu.h"
#include "SmallMenu.h"
#include "map.h"
#include "LineCommand.h"
#include "DirectSound.h"
#include "tool.h"
#include "hong_sub.h"
#include "NATIONSUPPORT.H"//010910 LSW
#include "NationSys.h"
#include "cScreenEffect.h"	// LTS DRAGON
#include "Hangul.h"
#include "Chat.h"
#include "language.h"
#define BLUETEAM					1				// LTS DRAGONLORD
#define REDTEAM						2

extern void RecvCMD_LOCALWAR_CHAT(t_packet* p);// LTS NEW LOCALWAR

extern void RecvCMD_NATION_CHAT(t_packet* p);

// Global Variable Define _______________________________________________________
int		g_LocalWarBegin=0;
char	g_SealStoneStatus[3][9];
short	g_AreaCount[7][3];
unsigned short	g_LocalWarPoint[3]={0,0,0};			// 020130 LTS
unsigned short	g_LocalWarDefencePoint[3]={0,0,0};	// LTS NEW LOCALWAR
char	g_LocalWarResult=0;

char*	LocalWarInfoPicBuffer=NULL;
Spr		LocalWarInfoPic[5];				// LTS BUG
char*	ChairPicBuffer[3];
Spr		ChairPic[3][8];
__int64	g_LocalWarRemainTime;		// 020115 LTS

int		g_EventLocalWarBegin=0;		// 020115 LTS
DWORD	g_dEventPoint=0;				// LTS BUG
__int64	g_dEventRemainTime=0;			// LTS BUG
char	g_LocalWarMapMoveIndex=-1;			// LTS NEW LOCALWAR
int		g_ActionAnger=0;				// LTS ACTION

// Glocal Variable Define End ___________________________________________________

			
// Extern Variable Define _______________________________________________________
extern char*		g_DestBackBuf;
extern Spr			SquadPic[3][5];			// LTS HORSERIDER
extern int			g_nLButtonState;		// LTS NEW LOCALWAR
// Extern Variable Define End ___________________________________________________

// Extern Function Define _______________________________________________________
extern void			LoadHeroClothAccessoryDataOne( int sprno, int no );
extern LPCHARACTER	FindHim(int ServerID);
extern Spr*			GetSprOfMenu( int type, int nImageNumber );
extern void			PutCompressedImage(int x, int y, Spr *sp );
extern void			PutCompressedImageFX(int x, int y, Spr *sp, DWORD alpha, DWORD op);
extern void			PutCompressedImageShadow(int x, int y, Spr *sp );
extern void			Hcolor( WORD color );
extern void			HprintBold( int x, int y, int fc, int bc, char *s, ... );
extern int			convert565to555_LoadSprite(Spr *sp);
extern void			GetTime(DWORD RemainTime,WORD& Hour,WORD& Min,WORD& Sec);
// Extern Function Define End ___________________________________________________

extern BOOL IsNeoWarfieldServer();		// BBD 040311
extern bool g_bNewWarBegin;				// BBD 040313

int GetNationIndex(int Nation)
{
	switch(Nation)
	{
	case NW_BY : return 0;
	case NW_ZY : return 1;
	case NW_YL : return 2;
	}
	return 0;
}


void DrawLocalWarDefeat(int ImageType,int MenuNumber,int BaseX,int BaseY)		// LTS NEW LOCALWAR
{
	POINTS DefeatPoint[3]= { {55,108},{174,242}, {336,172}};
	Spr* DefeatSpr;
	if (g_LocalWarDefencePoint[0]==0)
	{
		DefeatSpr=GetSprOfMenu(ImageType,36);
		int BaseXX=BaseX+DefeatSpr->ox+DefeatPoint[0].x;
		int BaseYY=BaseY+DefeatSpr->oy+DefeatPoint[0].y;
		if (DefeatSpr) PutCompressedImage(BaseXX,BaseYY, DefeatSpr );
	}
	if (g_LocalWarDefencePoint[1]==0)
	{
		DefeatSpr=GetSprOfMenu(ImageType,36);
		int BaseXX=BaseX+DefeatSpr->ox+DefeatPoint[1].x;
		int BaseYY=BaseY+DefeatSpr->oy+DefeatPoint[1].y;
		if (DefeatSpr) PutCompressedImage(BaseXX,BaseYY, DefeatSpr );
	}
	if (g_LocalWarDefencePoint[2]==0)
	{
		DefeatSpr=GetSprOfMenu(ImageType,36);
		int BaseXX=BaseX+DefeatSpr->ox+DefeatPoint[2].x;
		int BaseYY=BaseY+DefeatSpr->oy+DefeatPoint[2].y;
		if (DefeatSpr) PutCompressedImage(BaseXX,BaseYY, DefeatSpr );
	}
}

void DrawLocalWarJoinInterface(int ImageType,int MenuNumber,int BaseX,int BaseY)		// LTS NEW LOCALWAR
{
	const POINT MapPoint[7][12]={
		{ {0,5},{0,6},{0,7},{0,8},{1,5},{1,6},{1,7},{1,8},{2,5},{2,6},{2,7},{2,8} },	// ´ëÃÊ¿ø 
		{ {0,2},{0,3},{0,4},{4,0},{4,0},{4,0},{4,0},{4,0},{4,0},{4,0},{4,0},{4,0} },	// ÈÞ´ÙÀÎ°í°³	{4.0} ºóÀÚ·á
		{ {1,2},{1,3},{1,4},{4,0},{4,0},{4,0},{4,0},{4,0},{4,0},{4,0},{4,0},{4,0} },	// ·¹³Ê½º¿ÜºÎ
		{ {2,2},{2,3},{2,4},{4,0},{4,0},{4,0},{4,0},{4,0},{4,0},{4,0},{4,0},{4,0} },	// ´ëÁ¤±Û
		{ {0,0},{0,1},{4,0},{4,0},{4,0},{4,0},{4,0},{4,0},{4,0},{4,0},{4,0},{4,0} },	// ÈÞ´ÙÀÎ°­
		{ {1,0},{1,1},{4,0},{4,0},{4,0},{4,0},{4,0},{4,0},{4,0},{4,0},{4,0},{4,0} },	// Ä®¶óÀÏ¸¶À»
		{ {2,0},{2,1},{4,0},{4,0},{4,0},{4,0},{4,0},{4,0},{4,0},{4,0},{4,0},{4,0} } };	// ¼¼µé·¹½º
	const POINT	Locate[3][9]= {									// Interface Sealstone Locate x,y
								{ {143,289},{ 87,283},{140,302},{ 61,302},{100,307},{ 88,298},{ 75,313},{ 67,300},{ 78,278} },
								{ { 85,308},{119,311},{ 85,305},{ 90,327},{120,303},{124,342},{ 99,336},{136,332},{113,324} },
								{ { 89,283},{112,329},{128,350},{117,314},{ 89,286},{168,270},{144,262},{123,269},{133,286} } };
	const char SprNo[3][2]={ {21,20}, {17,16},{19,18} };	// Á×¾ú´Ù, »ì¾Ò´Ù

	const char MatchTBL[7]= { 15, 9,11,14,10,12,13 };
	Spr* s;

	s=GetSprOfMenu(ImageType, MatchTBL[MenuNumber]);
	if (s) PutCompressedImage(BaseX+s->ox+15,BaseY+s->oy+239, s );

	for (int i=0;i<12;i++)
	{
		if (MapPoint[MenuNumber][i].x<=2)	//µ¥ÀÌÅÍ°¡ ÀÖ´Â°Í
		{
			if (g_SealStoneStatus[MapPoint[MenuNumber][i].x][MapPoint[MenuNumber][i].y])	//»ì¾ÆÀÖ´Ù
			{
				s=GetSprOfMenu(ImageType,SprNo[MapPoint[MenuNumber][i].x][1]);
			}
			else
			{
				s=GetSprOfMenu(ImageType,SprNo[MapPoint[MenuNumber][i].x][0]);
			}
			if (s) PutCompressedImage(
				BaseX+s->ox+Locate[MapPoint[MenuNumber][i].x][MapPoint[MenuNumber][i].y].x,
				BaseY+s->oy+Locate[MapPoint[MenuNumber][i].x][MapPoint[MenuNumber][i].y].y, s );
		}
	}
	Hcolor( FONT_COLOR_NUMBER );
	Hprint2( BaseX+410, BaseY+290, g_DestBackBuf, lan->OutputMessage(0,333),g_AreaCount[MenuNumber][0] );
	Hprint2( BaseX+410, BaseY+320, g_DestBackBuf, lan->OutputMessage(0,333),g_AreaCount[MenuNumber][1] );
	Hprint2( BaseX+410, BaseY+350, g_DestBackBuf, lan->OutputMessage(0,333),g_AreaCount[MenuNumber][2] );

}

void DrawWinSpr()
{
	static char count=0;

	if (count>120)
	{
		g_LocalWarResult=0;
		count=0;
		return;
	}
	switch(count)
	{
	case 0		:
	case 104	: MP3(2211);	break;
	case 44		: MP3(2204);	break;
	case 60		: MP3(60);		break;
	}

	Spr* s=GetSprOfMenu( FIGHT_MAP_WIN,count);
	Spr* sfx=GetSprOfMenu( FIGHT_MAP_WIN,count+1);
	if (s) PutCompressedImage(370,250, s );
	if (sfx) PutCompressedImageFX(370,250, sfx,17,2 );

	count+=2;
}

void DrawLooseSpr()
{
	static char count=0;

	if (count>90)
	{
		g_LocalWarResult=0;
		count=0;
		return;
	}

	switch(count)
	{
	case 22 :
	case 32 : MP3(302);		break;
	case 70 : MP3(2203);	break;
	case 84 : MP3(252);		break;
	}

	Spr* s=GetSprOfMenu( FIGHT_MAP_LOSE,count);
	Spr* sfx=GetSprOfMenu( FIGHT_MAP_LOSE,count+1);
	if (s) PutCompressedImage(380,270, s );
	if (sfx) PutCompressedImageFX(380,270, sfx,17,2 );

	count+=2;
}

void LocalWarInfoDisPlay()					// LTS BUG				// LTS NEW LOCALWAR
{	//< CSD-030723
	Spr* s = NULL;
	WORD Hour = 0, Min= 0, Sec= 0;

	if (g_LocalWarBegin&&Hero->JoinLocalWar)
	{
		const int iClockX = 710;
		s=&LocalWarInfoPic[0];
		if(s){::PutCompressedImage(iClockX+s->ox+5 ,s->oy+7+40, s );}
		GetTime(g_LocalWarRemainTime-(g_ClientTime/1000),Hour,Min,Sec);			// 020115 LTS
		::HprintBold( iClockX+32, 55, ::Convert16to32(FONT_COLOR_WHITE) ,::Convert16to32(FONT_COLOR_BLACK),
					"[%02d:%02d]",Hour,Min);

		const int iByX = 720;
		s=&LocalWarInfoPic[1];
		if (s) {PutCompressedImage(iByX+s->ox,s->oy+7, s );}

		::HprintBold( iByX+32, 10,	::Convert16to32(FONT_COLOR_BLIGHT_BLUE) ,::Convert16to32(FONT_COLOR_BLACK),
					"DP:<%4d>",g_LocalWarDefencePoint[0] );
		::HprintBold( iByX+32, 25, ::Convert16to32(FONT_COLOR_BLIGHT_BLUE), ::Convert16to32(FONT_COLOR_BLACK),	
					"WP:[%4d]",g_LocalWarPoint[0] );

		const int iZyX = 815;
		s=&LocalWarInfoPic[2];
		if (s) {PutCompressedImage(iZyX+s->ox,s->oy+7, s );}

		::HprintBold( iZyX+32, 10,	::Convert16to32(FONT_COLOR_BLIGHT_RED), ::Convert16to32(FONT_COLOR_BLACK),
					"DP:<%4d>",g_LocalWarDefencePoint[1] );
		::HprintBold( iZyX+32, 25, ::Convert16to32(FONT_COLOR_BLIGHT_RED), ::Convert16to32(FONT_COLOR_BLACK),	
					"WP:[%4d]",g_LocalWarPoint[1] );

		const int iYlX = 910;
		s=&LocalWarInfoPic[3];
		if (s) {PutCompressedImage(iYlX+s->ox,s->oy+7, s );}
		::HprintBold( iYlX+32, 10,	::Convert16to32(FONT_COLOR_SOLID_YELLOW), ::Convert16to32(FONT_COLOR_BLACK),	
					"DP:<%4d>",g_LocalWarDefencePoint[2] );
		::HprintBold( iYlX+32, 25, ::Convert16to32(FONT_COLOR_SOLID_YELLOW), ::Convert16to32(FONT_COLOR_BLACK),	
					"WP:[%4d]",g_LocalWarPoint[2] );
	}

	switch (g_LocalWarResult)
	{
	case 0 : break;
	case 1 : DrawWinSpr();		break;					// WIN
	case 2 : DrawLooseSpr();	break;					// LOOSE
	}

	if ((MapNumber==39)&&(g_EventLocalWarBegin==3))		// ¸Ê¹øÈ£ ÀÌ¿Ü¿¡ ÁÁÀº°Ô ¾ø´Ù. ::stricmp¸¦ ÇÏ¸é ¼Óµµ°¡ ³Ñ ´À¸®´Ù.
	{
		s=&LocalWarInfoPic[0];
		if (s) PutCompressedImage(600+s->ox,s->oy+10, s );
		s=&LocalWarInfoPic[4];
		if (s) PutCompressedImage(700+s->ox,s->oy+10, s );
		GetTime(g_dEventRemainTime-(g_ClientTime/1000),Hour,Min,Sec);		
		HprintBold( 650+30, 18, RGB(255, 255,   0),0, "[%02d:%02d:%02d]",Hour,Min,Sec);
		HprintBold( 725+30, 18, RGB(255, 255,   0),0, "[%5d]",g_dEventPoint );
	}
}	//> CSD-030723

int LoadLocalWarInfoPic(char* FileName)    // LTS BUG
{
    SDL_RWops *fp = NULL;
    short no;
    unsigned int sprfilelength, size; // DWORD -> unsigned int
    unsigned int nOff = 0;            // DWORD -> unsigned int
    int   i;
    Spr  *s;
    int length;

    // [Android] 1. 路徑處理
    std::string path = FileName;
    if (path.find("./") == 0) path = path.substr(2);

    // [Android] 2. 開啟檔案
    fp = SDL_RWFromFile(path.c_str(), "rb" );
    if( fp == NULL ) {
        SDL_Log("LoadLocalWarInfoPic: Failed to open %s", path.c_str());
        return 0;
    }

    // 讀取數量 (short)
    SDL_RWread(fp, &no, sizeof(short), 1);

    // [Android] 3. 移動指標 (Seek)
    // fseek(fp, offset, SEEK_CUR) -> SDL_RWseek(fp, offset, RW_SEEK_CUR)
    SDL_RWseek(fp, no * 4, RW_SEEK_CUR);  // 跳過 Offset 表

    // [Android] 4. 取得檔案大小
    // _filelength -> SDL_RWsize
    Sint64 totalFileSize = SDL_RWsize(fp);

    // 計算需要分配的記憶體大小 (保留原本邏輯)
    length = sprfilelength = (int)totalFileSize - no * 4 * 2 - 2;

    if( LocalWarInfoPicBuffer != NULL )
    {
        SDL_RWclose(fp); // 記得關閉檔案
        return 0;
    }

    MemFree( LocalWarInfoPicBuffer );     // 0201 YGI
    MemAlloc( LocalWarInfoPicBuffer, sprfilelength );

    for( i = 0 ; i < 5 ; i ++)
    {
        s = &LocalWarInfoPic[i];

        // 讀取每個區塊的大小 (4 bytes)
        SDL_RWread(fp, &size, sizeof(unsigned int), 1);

        if( size == 0 )
        {
            s->xl = 0;
            s->yl = 0;
            s->size = 0;
            s->img = NULL;
            continue;
        }

        // 讀取 Spr Header
        SDL_RWread(fp, &(s->xl), sizeof( short ), 1);
        SDL_RWread(fp, &(s->yl), sizeof( short ), 1);
        //s->yl ++;
        SDL_RWread(fp, &(s->ox), sizeof( short ), 1);
        SDL_RWread(fp, &(s->oy), sizeof( short ), 1);
        SDL_RWread(fp, &(s->size), sizeof( unsigned int ), 1);

        // 檢查邊界
        if( nOff + s->size >= sprfilelength )
        {
            // JustMsg 在 Android 可能無法彈窗，改用 Log
            SDL_Log("%s File Error !! nOff:%d size:%d len:%d", FileName, nOff, s->size, sprfilelength);
            // JustMsg( "%s File Error !!",FileName);

            SDL_RWclose(fp); // 退出前關閉檔案
            ExitApplication(EA_LOAD_LOACL_WAR_INFO_SPR_DATA);
            return 0; // ExitApplication 可能不會立即終止，加個 return
        }

        // [Android] 讀取圖片數據到 Buffer
        // LocalWarInfoPicBuffer 是 char* 或 byte*，指標運算沒問題
        SDL_RWread(fp, LocalWarInfoPicBuffer + nOff, 1, s->size);

        s->img = LocalWarInfoPicBuffer + nOff;

        convert565to555_LoadSprite( s ); // Todo finished

        nOff += size;
    }

    // [Android] 5. 關閉檔案
    SDL_RWclose(fp);

    g_SDLInfo.CreateAtlasFromLoadedData(FileName, LocalWarInfoPic, 5);

    return sprfilelength;
}

void InitSitDownData()		// ÀÇÀÚ ¹öÆÛÀÇ ÃÊ±âÈ­ // LTS SITDOWN BUG
{
	ChairPicBuffer[0]=NULL;		// ¾øÀ½
	ChairPicBuffer[1]=NULL;		// 920001.DAT	
	ChairPicBuffer[2]=NULL;		// 920002.DAT
}

int LoadChairPic(int ChairNum)        // LTS SITDOWN BUG
{
    SDL_RWops *fp = NULL;
    short no;
    unsigned int sprfilelength, size; // DWORD -> unsigned int
    unsigned int nOff = 0;            // DWORD -> unsigned int
    int   i;
    Spr  *s;
    int length;
    char FileName[MAX_PATH];

    if (ChairNum <= 0 || ChairNum > 2) return 0;

    // 1. 格式化檔名
    sprintf(FileName, "./char/92000%d.spr", ChairNum);

    // [Android] 2. 路徑處理：移除開頭的 ./
    std::string path = FileName;
    if (path.find("./") == 0) path = path.substr(2);

    // [Android] 3. 開啟檔案
    fp = SDL_RWFromFile(path.c_str(), "rb");
    if( fp == NULL ) {
        // SDL_Log("LoadChairPic Error: File not found %s", path.c_str());
        return 0;
    }

    // 讀取圖片數量
    SDL_RWread(fp, &no, 2, 1);

    // [Android] 4. 移動指標跳過 Offset Table
    // fseek(fp, offset, SEEK_CUR) -> SDL_RWseek
    SDL_RWseek(fp, no * 4, RW_SEEK_CUR);

    // [Android] 5. 計算檔案大小與緩衝區長度
    // _filelength(_fileno(fp)) -> SDL_RWsize(fp)
    Sint64 totalFileSize = SDL_RWsize(fp);

    // 保留原始的長度計算邏輯：總長度 - (no * 4 * 2) - 2
    // 注意：這裡原始代碼是 no * 4 * 2，可能是因為有兩份 Offset 表或是預留空間？
    // 我們保持原樣以確保相容性。
    length = sprfilelength = (int)totalFileSize - no * 4 * 2 - 2;

    if( ChairPicBuffer[ChairNum] != NULL ) {
        SDL_RWclose(fp);
        return 0;
    }

    MemFree( ChairPicBuffer[ChairNum]);       // 0201 YGI
    MemAlloc( ChairPicBuffer[ChairNum], sprfilelength );

    for( i = 0 ; i < 8 ; i ++)
    {
        s = &ChairPic[ChairNum][i];

        // 讀取區塊大小 header
        SDL_RWread(fp, &size, 4, 1);

        if( size == 0 )
        {
            s->xl = 0;
            s->yl = 0;
            s->size = 0;
            s->img = NULL;
            continue;
        }

        SDL_RWread(fp, &(s->xl), sizeof( short ), 1);
        SDL_RWread(fp, &(s->yl), sizeof( short ), 1);
        //s->yl ++;
        SDL_RWread(fp, &(s->ox), sizeof( short ), 1);
        SDL_RWread(fp, &(s->oy), sizeof( short ), 1);
        SDL_RWread(fp, &(s->size), sizeof( unsigned int ), 1);

        if( nOff + s->size >= sprfilelength )
        {
            // JustMsg 在 Android 改用 Log 並關閉檔案
            SDL_Log("%s File Error !! (Buffer Overflow)", FileName);
            SDL_RWclose(fp);
            ExitApplication(EA_LOAD_LOACL_CHAIR_SPR_DATA);
            return 0; // 確保函式結束
        }

        // 讀取圖片數據
        SDL_RWread(fp, ChairPicBuffer[ChairNum] + nOff, 1, s->size);
        s->img = ChairPicBuffer[ChairNum] + nOff;

        convert565to555_LoadSprite( s ); //Todo finished

        nOff += size;
    }

    // [Android] 6. 關閉檔案
    SDL_RWclose(fp);

    g_SDLInfo.CreateAtlasFromLoadedData(FileName, ChairPic[ChairNum], 8);
    return sprfilelength;
}


void FreeLocalWarInfoPicBuffer()
{
	MemFree( LocalWarInfoPicBuffer );		
}

void FreeChairPicBuffer()
{
	MemFree( ChairPicBuffer[0] );		
	MemFree( ChairPicBuffer[1] );		
	MemFree( ChairPicBuffer[2] );		
}


extern SafeValue<int>	Mapx;
extern SafeValue<int>   Mapy;		// ÇöÀçº¸¿©Áö´Â ¸ÊÀÇ ½ÃÀÛÀý´ëÁÂÇ¥.

void DrawChair(LPCHARACTER ch)		// LTS SITDOWN
{
	const char CVTBL[8] = { 4,5,6,7,0,1,2,3 };	//½ºÇÁ¶óÀÌÆ® ¹æÇâ°ú DIRECTÀÇ ±³Á¤Å×ÀÌºí
		
	if (ch->ChairNum<=0||ch->ChairNum>2) return;

	if (ChairPicBuffer[ch->ChairNum]==NULL)
	{
		LoadChairPic(ch->ChairNum);
	}
	Spr* s=&ChairPic[ch->ChairNum][ch->direction];
	if (s) 
	{
		PutCompressedImageShadow(ch->x-Mapx,ch->y-Mapy,s);
		PutCompressedImage(ch->x-Mapx,ch->y-Mapy,s);
	}
}

void SendCMD_HERO_ACTION_CHANGED(LPCHARACTER ch)		// 011214 LTS
{
	if (ch->nCurrentAction==ACTION_SITDOWN)
	{
		t_packet packet;

		packet.h.header.type=CMD_HERO_ACTION_CHANGED;
		packet.u.LocalWar.CommonDataC.Data=ch->nCurrentAction;
		packet.h.header.size=sizeof(t_CommonDataC);
		QueuePacket(&packet,1);
	}
}

void SendCMD_CHECK_ABILITY_CHANGE_MONEY()   // LTS HORSERIDER  //¾îºôÀçºÐ¹è...
{
	t_packet packet;

	if (!Hero->aStepInfo[CLS_STEP])
	{
		packet.h.header.type=CMD_CHECK_ABILITY_CHANGE_MONEY;
		packet.h.header.size=0;
		QueuePacket(&packet,1);
	}
	else CallOkCancelMessageBox(TRUE, 0,0, lan->OutputMessage(0,334), FALSE); // ¾îºôÀçºÐ¹è¸¦ ÇÏ¿´½À´Ï´Ù. 
}

void RecvCMD_CHECK_ABILITY_CHANGE_MONEY(t_packet* p)	// 020115 LTS
{
	switch (p->u.NationWar.CommonDataC.Data)
	{
	case 0 : CallOkCancelMessageBox(TRUE, 0,0, lan->OutputMessage(0,336), FALSE); break; // ÀºÇà ÀÜ°í°¡ ºÎÁ·ÇÏ¿© ¾îºôÀçºÐ¹è¸¦ ÇÏÁö ¸øÇß½À´Ï´Ù.
	case 1 : CallOkCancelMessageBox(TRUE, 0,0, lan->OutputMessage(0,335), FALSE); break;// ¾îºôÀçºÐ¹è¸¦ ÇÏ¿´½À´Ï´Ù.
	case 5 : CallOkCancelMessageBox(TRUE, 0,0, lan->OutputMessage(0,348), FALSE); break;// µà¾óÅ¬·¡½º´Â ¾îºô¸®Æ¼ ÀçºÐ¹è¸¦ÇÒ ¼ö ¾ø½À´Ï´Ù.
	}
}

void LocalWarMapMoveClick(int MapNo)	// LTS NEW LOCALWAR
{
	g_LocalWarMapMoveIndex=MapNo;
}

void CloseLocalWarInfo()				// LTS NEW LOCALWAR
{
	g_LocalWarMapMoveIndex=-1;	// ¼±ÅÃµÈ°Å ÇØÁ¦ÇØ ÁØ´Ù.
}

int GetLocalWarMapMoveIndex()			// LTS NEW LOCALWAR
{
	return g_LocalWarMapMoveIndex;
}

void ChangeHeroLocalWarArmor()
{
	LPCHARACTER	ch=Hero;			// ½Ã¾ßÀÇ ¸ðµç »ç¶÷ÀÇ ¿ÊÀ» ¹Ù²Û´Ù.

	if (ch->sprno<=1)
	{
		if (g_LocalWarBegin)
		{
			if (ch->JoinLocalWar)
			{
				switch (ch->name_status.nation)
				{
				case NW_BY : ch->accessory[0] = fight_map_acc_num[1]; break;
				case NW_ZY : ch->accessory[0] = fight_map_acc_num[0]; break;
				case NW_YL : ch->accessory[0] = fight_map_acc_num[2]; break;
				}
				ch->accessory[1] = 1;
				LoadHeroClothAccessoryDataOne( ch->sprno, ch->accessory[0] );
				LoadHeroClothAccessoryDataOne( ch->sprno, ch->accessory[1] ); 
			}
		}
	}
}

void ChangeUserLocalWarArmor()
{
	LPCHARACTER	ch=Hero;			// ½Ã¾ßÀÇ ¸ðµç »ç¶÷ÀÇ ¿ÊÀ» ¹Ù²Û´Ù.

	while (ch!=NULL)
	{
		if (ch->sprno<=1)
		{
			if (g_LocalWarBegin)
			{
				if (ch->JoinLocalWar)
				{
					switch (ch->name_status.nation)
					{
					case NW_BY : ch->accessory[0] = fight_map_acc_num[1]; break;
					case NW_ZY : ch->accessory[0] = fight_map_acc_num[0]; break;
					case NW_YL : ch->accessory[0] = fight_map_acc_num[2]; break;
					}
					ch->accessory[1] = 1;
					LoadHeroClothAccessoryDataOne( ch->sprno, ch->accessory[0] );
					LoadHeroClothAccessoryDataOne( ch->sprno, ch->accessory[1] ); 
				}
			}
		}
		ch=ch->lpNext;
	}
}

int GetNationFromIndex(int NationIndex)		// LTS NEW HORSERIDER
{
	switch(NationIndex)
	{
	case 0 : return NW_BY;
	case 1 : return NW_ZY;
	case 2 : return NW_YL;
	}
	return NW_BY;
}


void SetLocalWarArmor(t_server_change_equip *p,LPCHARACTER ch)		// NetWork.cpp RecvChangeEquip() Call
{
	if (g_LocalWarBegin)
	{
		if (ch->JoinLocalWar)
		{
			switch(ch->name_status.nation)
			{
			case NW_BY : p->equip[0]=fight_map_acc_num[1];	break;
			case NW_ZY : p->equip[0]=fight_map_acc_num[0];	break;
			case NW_YL : p->equip[0]=fight_map_acc_num[2];	break;
			}
			p->equip[1]=1;
			LoadHeroClothAccessoryDataOne( ch->sprno, p->equip[0] );
			LoadHeroClothAccessoryDataOne( ch->sprno, p->equip[1] ); 
		}
	}
}

void SetLocalWarArmor(t_server_user_add *c)							// NetWork.cpp	RecvUserAdd() Call
{
	if (g_LocalWarBegin)
	{
		if (c->JoinLocalWar)
		{
			switch(c->nation)
			{
			case NW_BY : c->equip0=fight_map_acc_num[1];	break;
			case NW_ZY : c->equip0=fight_map_acc_num[0];	break;
			case NW_YL : c->equip0=fight_map_acc_num[2];	break;
			}
			c->equip1=1;
			LoadHeroClothAccessoryDataOne( c->spr_no, c->equip0 );
			LoadHeroClothAccessoryDataOne( c->spr_no, c->equip1 ); 
		}
	}
}


void SendCMD_LOCALWAR_STATUS()	//±¹ÁöÀü »óÅÂ
{
	t_packet packet;

	packet.h.header.type=CMD_LOCALWAR_STATUS;
	packet.h.header.size=0;

	QueuePacket(&packet,1);
}

void SendCMD_REQUEST_LOCALWAR_INFO()
{
	t_packet packet;
	packet.h.header.type=CMD_REQUEST_LOCALWAR_INFO;	//Á¤º¸¿äÃ»
	packet.h.header.size=0;
	QueuePacket(&packet,1);
}

void RecvCMD_LOCALWAR_STATUS(t_packet* p)	// ±¹ÁöÀü »óÅÂ
{
	if (p->u.LocalWar.CommonDataC.Data)
	{
		g_LocalWarBegin=1;
		SendCMD_REQUEST_LOCALWAR_INFO();
	}
	else CallOkCancelMessageBox(TRUE, 0,0, lan->OutputMessage(0,258), FALSE); //"[ÀüÀïÁß] ±â°£¿¡¸¸ »ç¿ëÇÒ ¼ö ÀÖ½À´Ï´Ù."
}

void RecvCMD_ANSWER_LOCALWAR_INFO(t_packet* p)
{
	short BY_Point=0,ZY_Point=0,YL_Point=0;
	short BY_Count=0,ZY_Count=0,YL_Count=0;

	for (int i=0;i<7;i++)	// ÅäÅ» Ä«¿îÆ®													// too Many
	{

		BY_Count+=p->u.LocalWar.LocalWarInfo.LocalWarInfo1[i].ManCount[0];		// ±¹ÁöÀü Âü¿© ÀÎ¿ø ±¸ÇÔ ÃßÈÄ »ç¿ë ¿¹Á¤
		ZY_Count+=p->u.LocalWar.LocalWarInfo.LocalWarInfo1[i].ManCount[1];
		YL_Count+=p->u.LocalWar.LocalWarInfo.LocalWarInfo1[i].ManCount[2];

		g_AreaCount[i][0]=p->u.LocalWar.LocalWarInfo.LocalWarInfo1[i].ManCount[0];
		g_AreaCount[i][1]=p->u.LocalWar.LocalWarInfo.LocalWarInfo1[i].ManCount[1];
		g_AreaCount[i][2]=p->u.LocalWar.LocalWarInfo.LocalWarInfo1[i].ManCount[2];
	}

	g_LocalWarRemainTime=(g_ClientTime/1000)+p->u.LocalWar.LocalWarInfo.LocalWarPointChanged.RemainTime;	// 020115 LTS
	g_LocalWarPoint[0]=p->u.LocalWar.LocalWarInfo.LocalWarPointChanged.Point[0];
	g_LocalWarPoint[1]=p->u.LocalWar.LocalWarInfo.LocalWarPointChanged.Point[1];
	g_LocalWarPoint[2]=p->u.LocalWar.LocalWarInfo.LocalWarPointChanged.Point[2];


	sprintf(SMenu[MN_LOCALWAR_JOIN].nField[9].temp,  "%4d",g_LocalWarPoint[0]);
	sprintf(SMenu[MN_LOCALWAR_JOIN].nField[10].temp,  "%4d",g_LocalWarPoint[1]);
	sprintf(SMenu[MN_LOCALWAR_JOIN].nField[11].temp,  "%4d",g_LocalWarPoint[2]);

	g_SealStoneStatus[0][0]=p->u.LocalWar.LocalWarInfo.LocalWarInfo2.SealStoneInfo[0][0];		// LOOP UNROLL
	g_SealStoneStatus[0][1]=p->u.LocalWar.LocalWarInfo.LocalWarInfo2.SealStoneInfo[0][1];
	g_SealStoneStatus[0][2]=p->u.LocalWar.LocalWarInfo.LocalWarInfo2.SealStoneInfo[0][2];
	g_SealStoneStatus[0][3]=p->u.LocalWar.LocalWarInfo.LocalWarInfo2.SealStoneInfo[0][3];
	g_SealStoneStatus[0][4]=p->u.LocalWar.LocalWarInfo.LocalWarInfo2.SealStoneInfo[0][4];
	g_SealStoneStatus[0][5]=p->u.LocalWar.LocalWarInfo.LocalWarInfo2.SealStoneInfo[0][5];
	g_SealStoneStatus[0][6]=p->u.LocalWar.LocalWarInfo.LocalWarInfo2.SealStoneInfo[0][6];
	g_SealStoneStatus[0][7]=p->u.LocalWar.LocalWarInfo.LocalWarInfo2.SealStoneInfo[0][7];
	g_SealStoneStatus[0][8]=p->u.LocalWar.LocalWarInfo.LocalWarInfo2.SealStoneInfo[0][8];

	g_SealStoneStatus[1][0]=p->u.LocalWar.LocalWarInfo.LocalWarInfo2.SealStoneInfo[1][0];		// LOOP UNROLL
	g_SealStoneStatus[1][1]=p->u.LocalWar.LocalWarInfo.LocalWarInfo2.SealStoneInfo[1][1];
	g_SealStoneStatus[1][2]=p->u.LocalWar.LocalWarInfo.LocalWarInfo2.SealStoneInfo[1][2];
	g_SealStoneStatus[1][3]=p->u.LocalWar.LocalWarInfo.LocalWarInfo2.SealStoneInfo[1][3];
	g_SealStoneStatus[1][4]=p->u.LocalWar.LocalWarInfo.LocalWarInfo2.SealStoneInfo[1][4];
	g_SealStoneStatus[1][5]=p->u.LocalWar.LocalWarInfo.LocalWarInfo2.SealStoneInfo[1][5];
	g_SealStoneStatus[1][6]=p->u.LocalWar.LocalWarInfo.LocalWarInfo2.SealStoneInfo[1][6];
	g_SealStoneStatus[1][7]=p->u.LocalWar.LocalWarInfo.LocalWarInfo2.SealStoneInfo[1][7];
	g_SealStoneStatus[1][8]=p->u.LocalWar.LocalWarInfo.LocalWarInfo2.SealStoneInfo[1][8];

	g_SealStoneStatus[2][0]=p->u.LocalWar.LocalWarInfo.LocalWarInfo2.SealStoneInfo[2][0];		// LOOP UNROLL
	g_SealStoneStatus[2][1]=p->u.LocalWar.LocalWarInfo.LocalWarInfo2.SealStoneInfo[2][1];
	g_SealStoneStatus[2][2]=p->u.LocalWar.LocalWarInfo.LocalWarInfo2.SealStoneInfo[2][2];
	g_SealStoneStatus[2][3]=p->u.LocalWar.LocalWarInfo.LocalWarInfo2.SealStoneInfo[2][3];
	g_SealStoneStatus[2][4]=p->u.LocalWar.LocalWarInfo.LocalWarInfo2.SealStoneInfo[2][4];
	g_SealStoneStatus[2][5]=p->u.LocalWar.LocalWarInfo.LocalWarInfo2.SealStoneInfo[2][5];
	g_SealStoneStatus[2][6]=p->u.LocalWar.LocalWarInfo.LocalWarInfo2.SealStoneInfo[2][6];
	g_SealStoneStatus[2][7]=p->u.LocalWar.LocalWarInfo.LocalWarInfo2.SealStoneInfo[2][7];
	g_SealStoneStatus[2][8]=p->u.LocalWar.LocalWarInfo.LocalWarInfo2.SealStoneInfo[2][8];

	g_LocalWarDefencePoint[0]=p->u.LocalWar.LocalWarInfo.LocalWarDefencePoint[0];		// LTS NEW LOCALWAR
	g_LocalWarDefencePoint[1]=p->u.LocalWar.LocalWarInfo.LocalWarDefencePoint[1];
	g_LocalWarDefencePoint[2]=p->u.LocalWar.LocalWarInfo.LocalWarDefencePoint[2];


	CallSmallMenu(MN_LOCALWAR_JOIN);
}

void SendCMD_LOCALWAR_JOIN()				// LTS NEW LOCALWAR
{
	if (g_LocalWarBegin)
	{
		int Nation=GetNationIndex(Hero->name_status.nation);
		if (g_LocalWarDefencePoint[Nation]>0)
		{
			t_packet packet;
			
			packet.h.header.type=CMD_LOCALWAR_JOIN;
			packet.h.header.size=0;
			QueuePacket(&packet,1);
		}
		else
		{
			CallOkCancelMessageBox(TRUE, 0,0,lan->OutputMessage(0,372),FALSE);	// ±¹ÁöÀü¿¡ ÆÐ¹èÇÏ¿´À¸¹Ç·Î Âü °¡ÇÒ¼ö ¾ø½À´Ï´Ù.
		}
	}
	else
	{
		CallOkCancelMessageBox(TRUE, 0,0,lan->OutputMessage(0,371),FALSE);	// ±¹ÁöÀüÀÌ ½ÃÀÛµÇÁö ¾Ê¾Ò½À´Ï´Ù.
	}

}

void SendCMD_LOCALWAR_MAPMOVE()				// LT SNEW LOCALWAR
{
	if (g_LocalWarMapMoveIndex>=0)
	{
		t_packet packet;
		packet.h.header.type=CMD_LOCALWAR_MAPMOVE;
		packet.u.LocalWar.CommonDataC.Data=g_LocalWarMapMoveIndex;
		packet.h.header.size=sizeof(t_CommonDataC);
		QueuePacket(&packet,1);
		g_LocalWarMapMoveIndex=-1;			// ÃÊ±âÈ­
	}
	else
	{
		AddCurrentStatusMessage(255,255,0,lan->OutputMessage(0,371),FALSE); // "ÀÌµ¿ÇÏ½Ç ¸ÊÀ» ¼±ÅÃÇØ¾ß ÇÕ´Ï´Ù.");
	}
}

void RecvCMD_LOCALWAR_JOIN_RESULT(t_packet* p)
{
	if (p->u.LocalWar.CommonDataC.Data)
	{
		Hero->JoinLocalWar=TRUE;
		ChangeHeroLocalWarArmor();
		//CallOkCancelMessageBox(TRUE, 0,0,"±¹ÁöÀü¿¡ Âü¿© ÇÕ´Ï´Ù.", FALSE);
		strcpy(SMenu[MN_LOCALWAR_OK].nField[3].temp, lan->OutputMessage(0,378)); // LTS LOCALWAR MENU
		PlayListAutoSounds(62,0,0,FALSE);					// 020115 LTS
		CallSmallMenu(MN_LOCALWAR_OK);

	}
	else CallOkCancelMessageBox(TRUE, 0,0, lan->OutputMessage(0,337), FALSE); 
}

// 031110 YGI
void LocalWarDoneBoxText(bool Button)						// LTS LOCALWAR MENU
{
#ifdef USA_LOCALIZING_
	int MessageNo[6]={378,390,379,391,380,392};							// MessageNo
	const int iMaxPage = 5;
#else
	//int MessageNo[3]={378,379,380};							// MessageNo
	//const int iMaxPage = 2;
#endif

	switch(Button)
	{
	case false	: // PREV
		{
			SMenu[MN_LOCALWAR_OK].work--;
			if (SMenu[MN_LOCALWAR_OK].work<0) SMenu[MN_LOCALWAR_OK].work=0;
			strcpy(SMenu[MN_LOCALWAR_OK].nField[3].temp, lan->OutputMessage(0,MessageNo[SMenu[MN_LOCALWAR_OK].work])); 
			break;
		}
	case true	: // NEXT
		{
			SMenu[MN_LOCALWAR_OK].work++;
			if (SMenu[MN_LOCALWAR_OK].work>iMaxPage) SMenu[MN_LOCALWAR_OK].work=iMaxPage;
			strcpy(SMenu[MN_LOCALWAR_OK].nField[3].temp, lan->OutputMessage(0,MessageNo[SMenu[MN_LOCALWAR_OK].work])); 
			break;
		}
	}
}


void SendCMD_REQUEST_COMPARE_PHASE()
{
	t_packet packet;

	packet.h.header.type=CMD_REQUEST_COMPARE_PHASE;
	packet.h.header.size=0;
	QueuePacket(&packet,1);
}

void RecvCMD_USER_JOIN_LOCALWAR(t_packet* p)
{
	LPCHARACTER ch=FindHim(p->u.LocalWar.UserJoinLocalWar.ServerID);
	if (NULL!=ch) 
	{
		ch->JoinLocalWar=TRUE;
		ChangeUserLocalWarArmor();
	}
}

void RecvCMD_LOCALWAR_BEGIN(t_packet* p)		// 011217 LTS
{
	g_LocalWarBegin=TRUE;
	SendCMD_REQUEST_LOCALWAR_INFO();
	AddCurrentStatusMessage(255,255,0,lan->OutputMessage(0,284));
	switch (Hero->name_status.nation)
	{
	case NW_BY : AddCurrentStatusMessage(255,255,0,lan->OutputMessage(0,285));	break;
	case NW_ZY : AddCurrentStatusMessage(255,255,0,lan->OutputMessage(0,286));	break;
	case NW_YL : AddCurrentStatusMessage(255,255,0,lan->OutputMessage(0,287));	break;
	}
	PlayListAutoSounds(271,0,0,FALSE);
}

void RecvCMD_LOCALWAR_BEGIN_TIME(t_packet* p)		// 011217 LTS
{
	AddCurrentStatusMessage(255,255,0,lan->OutputMessage(0,630), p->u.LocalWar.LocalWarBeginTime.nHour);
}

void RecvCMD_LOCALWAR_END(t_packet* p)						// LTS NEW LOCALWAR
{
	if (g_LocalWarDefencePoint[GetNationIndex(Hero->name_status.nation)]!=0)	// ÀÌ¹Ì ±¹ÁöÀü¿¡¼­ ÆÐ¹èÇÑ ³ª¶ó´Â ´Ù½Ã µå·ÎÀ× ÇÏÁö ¾Ê´Â´Ù.
	{
		g_LocalWarBegin=FALSE;
		if (Hero->JoinLocalWar) Hero->JoinLocalWar=0;
		if (Hero->name_status.nation==p->u.LocalWar.CommonDataC.Data) 
		{
			g_LocalWarResult=1; // WIN
			AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,338));
		}
		else 
		{
			g_LocalWarResult=2; // LOOSE
			AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,366));//"ÀÚ±¹°¡ÀÇ °á°è¼®À» º¸È£ÇÏÁö ¾Ê¾Æ ÀüÀï¿¡ ÆÐ¹èÇß½À´Ï´Ù"
		}
	}
}

void RecvCMD_LOCALWAR_END_CHAR_RESULT(t_packet* p)						// LTS NEW LOCALWAR
{
	int data_reserve[20] = { 0 };

	memcpy(data_reserve, p->u.data, sizeof(data_reserve));

	int m_MuchKill_Count = data_reserve[RES_DATA_TOTAL_KILL];
	int m_MuchDead_Count = data_reserve[RES_DATA_DEAD];
	int m_TotalMvpPoint = data_reserve[RES_DATA_ASSISTANT];
	int totalDmg = data_reserve[RES_DATA_ATK_DMG_TOTAL];
	int totalGetDmg = data_reserve[RES_DATA_GET_DMG_TOTAL];

	char mzData[260] = { 0 };
	sprintf(mzData, lan->OutputMessage(0, 97), m_MuchKill_Count, m_MuchDead_Count, m_TotalMvpPoint, totalDmg, totalGetDmg);
	ChatMgr.AddString(nullptr, mzData, CHATTYPE_NORMAL);
}

void RecvCMD_LOCALWAR_RESULT(t_packet* p)						// LTS NEW LOCALWAR
{
	/*
	if (g_LocalWarDefencePoint[GetNationIndex(Hero->name_status.nation)] != 0)	// ÀÌ¹Ì ±¹ÁöÀü¿¡¼­ ÆÐ¹èÇÑ ³ª¶ó´Â ´Ù½Ã µå·ÎÀ× ÇÏÁö ¾Ê´Â´Ù.
	{
		g_LocalWarBegin = FALSE;
		if (Hero->JoinLocalWar) Hero->JoinLocalWar = 0;
		if (Hero->name_status.nation == p->u.LocalWar.CommonDataC.Data)
		{
			g_LocalWarResult = 1; // WIN
			AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 338));
		}
		else
		{
			g_LocalWarResult = 2; // LOOSE
			AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 366));//"ÀÚ±¹°¡ÀÇ °á°è¼®À» º¸È£ÇÏÁö ¾Ê¾Æ ÀüÀï¿¡ ÆÐ¹èÇß½À´Ï´Ù"
		}
	}
	*/

	if (p->u.LocalWar.CommonDataC.Data == 250) {
		//SDL_Log("CMD_LOCALWAR_RESULT Get!...packet size=%d", p->h.header.size);
	}

	int m_MuchKill_Count = p->u.LocalWar.CommonDataC.m_MuchKill_Count;
	int m_MuchDead_Count = p->u.LocalWar.CommonDataC.m_MuchDead_Count;
	int m_TotalMvpPoint = p->u.LocalWar.CommonDataC.m_TotalMvpPoint;
	string m_MuchDead_Char = p->u.LocalWar.CommonDataC.m_MuchDead_Char;
	string m_MuchKill_Char = p->u.LocalWar.CommonDataC.m_MuchKill_Char;
	string m_Mvp_Char = p->u.LocalWar.CommonDataC.m_Mvp_Char;
	
	AddCurrentStatusMessage(FONT_COLOR_SOLID_YELLOW, lan->OutputMessage(0, 94));

	AddCurrentStatusMessage(FONT_COLOR_SOLID_YELLOW, lan->OutputMessage(0, 95), 
		m_MuchKill_Char.data(), m_MuchKill_Count,
		m_MuchDead_Char.data(), m_MuchDead_Count,
		m_Mvp_Char.data(), m_TotalMvpPoint);

#ifdef _DEBUG
	SDL_Log("much kill [%s][%d]", m_MuchKill_Char.data(), m_MuchKill_Count);
	SDL_Log("much dead [%s][%d]", m_MuchDead_Char.data(), m_MuchDead_Count);
	SDL_Log("mvp [%s][%d]", m_Mvp_Char.data(), m_TotalMvpPoint);
#endif
}

void RecvCMD_SEALSTONE_BROKED(t_packet* p)//結界石被破壞
{
	/*AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 340),
		MapInfo[p->u.LocalWar.SealStoneBroked.MapNumber].mapname,
		p->u.LocalWar.SealStoneBroked.LocateX,
		p->u.LocalWar.SealStoneBroked.LocateY);
	PlayListAutoSounds(262,0,0,FALSE);*/					// 0215 LTS
}

void RecvCMD_SEALSTONE_ATTACKED(t_packet* p)//結界石被攻擊中
{
	/*AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 341),
		MapInfo[p->u.LocalWar.SealStoneAttacked.MapNumber].mapname,
		p->u.LocalWar.SealStoneAttacked.LocateX,
		p->u.LocalWar.SealStoneAttacked.LocateY);*/
}

void RecvCMD_LOCALWAR_POINT_BBS(t_packet* p)	// LTS BUG
{
	g_LocalWarRemainTime=(g_ClientTime/1000)+p->u.LocalWar.LocalWarPointChanged.RemainTime;		// 020115 LTS
	g_LocalWarPoint[0]=p->u.LocalWar.LocalWarPointChanged.Point[0];
	g_LocalWarPoint[1]=p->u.LocalWar.LocalWarPointChanged.Point[1];
	g_LocalWarPoint[2]=p->u.LocalWar.LocalWarPointChanged.Point[2];
}

void RecvCMD_ACTION_STATUS(t_packet* p)
{
	LPCHARACTER ch=FindHim(p->u.LocalWar.ActionStatus.ServerID);
	if (NULL!=ch) 
	{
		ch->nCurrentAction=p->u.LocalWar.ActionStatus.nCurrentAction;
		ch->direction=(DIRECTION)p->u.LocalWar.ActionStatus.Direction;
	}
}

void RecvCMD_SITDOWN_DIRECTION_CHANGED(t_packet* p)
{
	LPCHARACTER ch=FindHim(p->u.LocalWar.SDChanged.ServerID);
	if (NULL!=ch) 
	{
		ch->direction=(DIRECTION)p->u.LocalWar.SDChanged.Direction;
	}
}

void RecvCMD_LOCALWAR_STARTUP_STATUS(t_packet* p)
{
	g_LocalWarBegin=p->u.LocalWar.CommonDataC.Data;
}


// Event LocalWar ___________________________________________________________________
/*
void EventLocalWarControl(char* Arg)	// 020115 LTS
{
#ifdef _DEBUG

	t_packet packet;
	char temp=Arg[0]-48;		// 0 :»ç¿ë¾ÈÇÔ, 1 : Àá¿© ¹× ÀÌµ¿, 2 : ½ÃÀÛ, 3 : Á¾·á

	packet.h.header.type=CMD_EVENT_LOCALWAR_CONTROL;
	packet.u.LocalWar.CommonDataC.Data=temp;
	packet.h.header.size=sizeof(t_CommonDataC);
	QueuePacket(&packet,1);
	
#endif
}*/

void SendCMD_EVENT_LOCALWAR_JOIN()
{
	t_packet packet;

	packet.h.header.type=CMD_EVENT_LOCALWAR_JOIN;
	packet.h.header.size=0;
	QueuePacket(&packet,1);
	CallOkCancelMessageBox(TRUE, 0,0,lan->OutputMessage(0,349) , FALSE); //"ÀÌº¥Æ® ±¹ÁöÀü¿¡ Âü°¡½ÅÃ»À» ÇÏ¿´½À´Ï´Ù."
}

void SendCMD_EVENT_LOCALWAR_MOVE()
{
	t_packet packet;

	packet.h.header.type=CMD_EVENT_LOCALWAR_MOVE;
	packet.h.header.size=0;
	QueuePacket(&packet,1);
}


void SetEventLocalWarArmor()
{
	LPCHARACTER	ch=Hero;			// ½Ã¾ßÀÇ ¸ðµç »ç¶÷ÀÇ ¿ÊÀ» ¹Ù²Û´Ù.

	while (ch!=NULL)
	{
		if (ch->sprno<=1)
		{
			switch (ch->name_status.nation)
			{
			case NW_BY : ch->accessory[0]=fight_map_acc_num[1];	break;
			case NW_ZY : ch->accessory[0]=fight_map_acc_num[0];	break;
			case NW_YL : ch->accessory[0]=fight_map_acc_num[2];	break;
			}
			ch->accessory[1]=1;
			LoadHeroClothAccessoryDataOne( ch->sprno, ch->accessory[0] );
			LoadHeroClothAccessoryDataOne( ch->sprno, ch->accessory[1] ); 
		}
		ch=ch->lpNext;
	}
}

/*void ProcessEventLocalWarStart()
{
	AddCurrentStatusMessage( 255,255,0,"ÀÌº¥Æ® ±¹ÁöÀüÀÌ ½ÃÀÛµÇ¾ú½À´Ï´Ù.");
	SetEventLocalWarArmor();
}

void RecvCMD_EVENT_LOCALWAR_STATUS_CHANGE(t_packet* p)
{
	g_EventLocalWarBegin=p->u.LocalWar.CommonDataC.Data;
	//¸Þ½ÃÁö Ã³¸®
	switch (g_EventLocalWarBegin)
	{
	case 0 :	break;
	case 1 :	break;
	case 2 :	break;
	case 3 : 	
				AddCurrentStatusMessage( 255,255,0,"ÀÌº¥Æ® ±¹ÁöÀüÀÌ ½ÃÀÛµÇ¾ú½À´Ï´Ù.");
				//SetEventLocalWarArmor(); 
				break;
	case 4 :	
				AddCurrentStatusMessage( 255,255,0,"ÀÌº¥Æ® ±¹ÁöÀüÀÌ Á¾·áµÇ¾ú½À´Ï´Ù.");
				break;
	}
}

void RecvCMD_EVENT_LOCALWAR_REMAIN_TIME(t_packet* p)
{
	char temp[MAX_PATH];
	WORD Hour,Min,Sec;
	GetTime(p->u.LocalWar.CommonDataDw.Data,Hour,Min,Sec);			// 020115 LTS

	sprintf(temp,"ÀÌº¥Æ® ±¹ÁöÀüÀÌ %02d:%02d:%02d ³²¾Ò½À´Ï´Ù.",Hour,Min,Sec);
	AddCurrentStatusMessage( 255,255,0,temp);
}*/

void RecvCMD_EVENT_LOCALWAR_JOIN_RESULT(t_packet* p)
{
	switch (p->u.LocalWar.CommonDataC.Data)
	{
	case 0 : CallOkCancelMessageBox(TRUE, 0,0,lan->OutputMessage(0,350) , FALSE); break;//"È¨ÆäÀÌÁö¿¡¼­ ÀÌº¥Æ® Âü°¡½ÅÃ»À» ÇÏÁö ¾Ê¾Ò½À´Ï´Ù.."
	case 1 : CallOkCancelMessageBox(TRUE, 0,0,lan->OutputMessage(0,349) , FALSE); break;//"Âü¿©½ÅÃ»À» ÇÏ¿´½À´Ï´Ù."
	case 2 : CallOkCancelMessageBox(TRUE, 0,0,lan->OutputMessage(0,351) , FALSE); break;//"ÇöÀç ´ëÈ¸¿¡ Âü¿©ÇÒ ¼ö ÀÖ´Â ·¹º§ÀÌ ¾Æ´Õ´Ï´Ù. Âü¿©°¡´ÉÇÑ ´ëÈ¸¿¡ µî·ÏÇÏ½Ã±â ¹Ù¶ø´Ï´Ù."
	case 3 : CallOkCancelMessageBox(TRUE, 0,0,lan->OutputMessage(0,352) , FALSE); break;//"¹«ÇÑ PK ´ëÈ¸ Âü°¡ÀÚ ÀÎ¿øÀÌ ÀüºÎ Ã¡½À´Ï´Ù. ´ÙÀ½ ´ëÈ¸¿¡ Âü°¡ÇØ ÁÖ½Ã±â ¹Ù¶ø´Ï´Ù."
	}
}

void RecvCMD_EVENT_LOCALWAR_MOVE_RESULT(t_packet* p)
{
	if (!p->u.LocalWar.CommonDataC.Data)
	{
		CallOkCancelMessageBox(TRUE, 0,0,lan->OutputMessage(0,353) , FALSE); //"´ëÈ¸ Âü°¡ ½ÅÃ»À» ÇØ¾ß¸¸ ´ëÈ¸Àå¼Ò·Î ÀÌµ¿ÇÒ ¼ö°¡ ÀÖ½À´Ï´Ù."
	}
}

void GetMapName(int Nation,char* MapName,int& x, int &y)
{
	switch(Nation)
	{
	case NW_BY : sprintf(MapName,"%s", lan->OutputMessage(9,201));			x=100;	y=100;	break;	// ÇïÅÏÆ®
	case NW_ZY : sprintf(MapName,"%s", lan->OutputMessage(9,220));			x=100;	y=100;	break;	// ·¹³Ê½º½Ã
	case NW_YL : sprintf(MapName,"%s", lan->OutputMessage(9,263));			x=100;	y=100;	break;	// ¹Ù¶õÅº
	}
}

void ProcessEventMessage(t_EventLocalWarMessage* Message)	//¸Þ½ÃÁö Ã³¸®	// LTS BUG
{
	char	tempMapName[MAX_PATH];
	int		x,y;
	POINTS	EventJoinLocation[3];

	GetMapName(Hero->name_status.nation,tempMapName,x,y);

	EventJoinLocation[0].x=Message->EventJoinLocation[0].x;
	EventJoinLocation[0].y=Message->EventJoinLocation[0].y;
	EventJoinLocation[1].x=Message->EventJoinLocation[1].x;
	EventJoinLocation[1].y=Message->EventJoinLocation[1].y;
	EventJoinLocation[2].x=Message->EventJoinLocation[2].x;
	EventJoinLocation[2].y=Message->EventJoinLocation[2].y;

	x=EventJoinLocation[GetNationIndex(Hero->name_status.nation)].x;
	y=EventJoinLocation[GetNationIndex(Hero->name_status.nation)].y;

	switch(Message->EventStatus)
	{
	case 0 : AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,354)); break;	//"¿À´ÃÀÇ ¸ðµçÀÌº¥Æ®°¡ ³¡³µ½À´Ï´Ù."
	case 1 : AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,355),Message->MinLevel,Message->MaxLevel,tempMapName,x,y); break; // "[ %d ] - [ %d ]·¹º§ ¹«ÇÑ PK ´ëÈ¸ Âü¿©ÀÚ µî·ÏÀ» [ %s ]¸ÊÀÇ [ %d, %d ]¿¡¼­ ¹Þ°íÀÖ½À´Ï´Ù."
	case 2 : AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,356),Message->MinLevel,Message->MaxLevel,tempMapName,x,y); break;//"[ %d ] - [ %d ]·¹º§ ¹«ÇÑ PK ´ëÈ¸ Âü¿©ÀÚ´Â [ %s ]¸ÊÀÇ [ %d, %d ]¿¡¼­ Âü¿©ÇØÁÖ½Ã±â ¹Ù¶ø´Ï´Ù."
	case 3 : AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,357),Message->MinLevel,Message->MaxLevel); break;//"[ %d ] - [ %d ]·¹º§ ¹«ÇÑ PK ´ëÈ¸°¡ ½ÃÀÛµÇ¾ú½À´Ï´Ù."
	case 4 : AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,358),Message->MinLevel,Message->MaxLevel); break;//"[ %d ] - [ %d ]·¹º§ ¹«ÇÑ PK ´ëÈ¸°¡ Á¾·áµÇ¾ú½À´Ï´Ù."
	}
	PlayListAutoSounds(62,0,0,FALSE);

}

void RecvCMD_EVENT_LOCALWAR_MESSAGE(t_packet* p)
{
	g_EventLocalWarBegin=p->u.LocalWar.EventLocalWarMessage.EventStatus;
	switch (g_EventLocalWarBegin)
	{
	case 0 :	break;
	case 1 :	break;
	case 2 :	break;
	case 3 : 	
				//SetEventLocalWarArmor(); 
				break;
	case 4 :	
				break;
	}
	ProcessEventMessage(&p->u.LocalWar.EventLocalWarMessage);	//¸Þ½ÃÁö Ã³¸®
}

void RecvCMD_EVENT_POINT_CHANGE(t_packet* p)					// LTS BUG
{
	g_dEventPoint=p->u.LocalWar.EventPoint.Point;
}

void RecvCMD_EVENT_REMAIN_TIME(t_packet* p)						// LTS BUG
{
	g_dEventRemainTime=(g_ClientTime/1000)+p->u.LocalWar.EventRemainTime.RemainTime;
}



void MakeEarthQuake(int Delay)		// LTS AI2
{
	g_GameInfo.displayOption = DISPLAY_EARTHQUAKE;  g_GameInfo.displayOptionDelay = Delay;	// ÁöÁø Ã³¸®ºÎºÐ..
}

void RecvCMD_EVENT_MONSTER_CREATED(t_packet* p)		// LTS AI2
{
	 AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,289),MapInfo[MapNumber].mapname);//"[ %s ]¿¡ ¸ó½ºÅÍ°¡ Ä§ÀÔÇÏ¿´½À´Ï´Ù."
	 MakeEarthQuake(40);
	 //PlayListAutoSound(260,0,0,0);
}

void DrawLocalWarSquadPic(int x,int y,LPORDERTABLE o,LPCHARACTER ch)	// LTS HORSERIDER
{
	int NationIndex=GetNationIndex(ch->name_status.nation);
	if (ch->viewtype!=VIEWTYPE_TRANSPARENCY_&&ch->viewtype!=VIEWTYPE_GHOST_)	// Åõ¸íÀÌ°Å³ª Á×¾îÀÖ´Â³ðÀº Á¦¿Ü 
	{
		PutCompressedImage(x,y,&SquadPic[NationIndex][3]);
	}
}

void RecvCMD_LOCALWAR_NATION_RESULT(t_packet* p)		// LTS NEW LOCALWAR
{														// ±¹ÁöÀü Áß°£¿¡ ÀÌÆÐÅ¶À» ¹ÞÀ¸¸é ÆÐ¹èÀÌ´Ù.. 		
	int Nation=GetNationFromIndex(p->u.LocalWar.LocalWarResult.NationIndex);
	int Result=p->u.LocalWar.LocalWarResult.Result;

	if (Hero->name_status.nation==Nation)
	{
		g_LocalWarBegin=FALSE;
		if (Result)				// ½Â¸®
		{
			g_LocalWarResult=1; // WIN
			AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,338));
		}
		else 
		{
			g_LocalWarResult=2; // LOOSE
			AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,366));//"ÀÚ±¹°¡ÀÇ °á°è¼®À» º¸È£ÇÏÁö ¾Ê¾Æ ÀüÀï¿¡ ÆÐ¹èÇÏ¿´½À´Ï´Ù");
		}
	}
	else
	{
		switch(Nation)
		{
		case 3 : AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,367),lan->OutputMessage(0,205)); break;//lan->OutputMessage(0,339));
		case 4 : AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,367),lan->OutputMessage(0,206)); break;
		case 6 : AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,367),lan->OutputMessage(0,207)); break;
		}
		PlayListAutoSounds(56,0,0,0);
	}
}

void RecvCMD_LOCALWAR_MAPMOVE_RESULT(t_packet* p)	// LTS NEW LOCALWAR
{
	switch(p->u.LocalWar.CommonDataC.Data)
	{
	case 1 : CallOkCancelMessageBox(TRUE, 0,0,lan->OutputMessage(0,368), FALSE); break;		// "±¹ÁöÀü¿¡ Âü¿©ÇØ¾ß¸¸ ¸ÊÀÌµ¿À» ÇÒ¼ö ÀÖ½À´Ï´Ù."
	case 2 : CallOkCancelMessageBox(TRUE, 0,0,lan->OutputMessage(0,369), FALSE); break;		// "ÀÚ±¹ÀÇ ¸Ê¿¡¸¸ ¸ÊÀÌµ¿À» ÇÒ¼ö ÀÖ½À´Ï´Ù."
	}
}

void RecvCMD_LOCALWAR_FAME_EXP(t_packet* p)			/// LTS NEW LOCALWAR
{
	short int	tempFame=p->u.LocalWar.FameExpChange.GainedFame;
	DWORD		tempExp=p->u.LocalWar.FameExpChange.GainedExp;
	PlayListAutoSounds(62,0,0,FALSE);
	/*AddCurrentStatusMessage( 255,255,0,"============< ÀüÀï º¸»ó >==============");
	AddCurrentStatusMessage( 255,255,0,"ÀüÀï¿¡¼­ ¸¹Àº °øÀ» ¼¼¿ö ±¹°¡¿¡¼­ ¾Æ·¡¿Í °°Àº º¸»óÀ» ÇÏ°Ô‰ç½À´Ï´Ù.");
	AddCurrentStatusMessage( 255,255,0,"   [ ¸í    ¼º] : %d",tempFame);
	AddCurrentStatusMessage( 255,255,0,"   [ °æ Çè Ä¡] : %d",tempExp);*/
	AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,198));
	AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,199));
	AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,200),tempFame);
	AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,201),tempExp);
}

void RecvCMD_LOCALWAR_DEFENCE_POINT(t_packet* p)			// LTS NEW LCOALWAR
{
	g_LocalWarDefencePoint[0]=p->u.LocalWar.LocalWarDefencePoint.Point[0];
	g_LocalWarDefencePoint[1]=p->u.LocalWar.LocalWarDefencePoint.Point[1];
	g_LocalWarDefencePoint[2]=p->u.LocalWar.LocalWarDefencePoint.Point[2];
}

void RecvCMD_GOTO_GUILD_HOUSE_RESULT(t_packet* p)		// ±è¿µÁÖ
{
	char* GuildName=GetGuildName(p->u.LocalWar.CommonDataDw.Data);
	if (GuildName!=NULL)
	{
		AddCurrentStatusMessage(255,255,0,lan->OutputMessage(0,290),GuildName);
	}
}

void RecvCMD_CHECK_GUILD_HOUSE_RESULT(t_packet* p)		// ±è¿µÁÖ
{
	char* GuildName=GetGuildName(p->u.LocalWar.CommonDataDw.Data);
	if (GuildName!=NULL)
	{
		AddCurrentStatusMessage(255,255,0,lan->OutputMessage(0,291),GuildName);
	}
}

LPCHARACTER ReturnCharacterBySprNo(int Sprno)	// LTS DRAGON TEMP
{
	LPCHARACTER ch=Hero;
	while (ch!=NULL)
	{
		if (ch->sprno==Sprno)
		{
			return ch;
		}
		ch=ch->lpNext;
	}
	return NULL;
}

void SendDragonAction(char* Arg)			// LTS DRAGON TEMP
{
	int i=atoi(Arg);
	LPCHARACTER ch=ReturnCharacterBySprNo(50);

	if (!ch) 
	{
#ifdef _DEUBG // finito 060507
		if( GetSysInfo(SI_GAME_MAKE_MODE ) )	// 020925 YGI
			AddCurrentStatusMessage(255,255,0,"ÇØ´ç ½ºÇÁ¶óÀÌÆ® ¹øÈ£¸¦ °ËÃâÇÏÁö ¸øÇÏ¿´½À´Ï´Ù.");
#endif
		return;
	}

	ch->nCurrentAction=i;	//¾×¼Ç¼¼ÆÃ
	ch->nCurrentFrame.PutEncrypted(0);
}


int GetDirection(POINT tStart,POINT tDest)
{
	POINT temp;
	int Quater=0;
	float Ratio=0;
	int Direction;

	temp.x=tStart.x-tDest.x;
	temp.y=tStart.y-tDest.y;
    if (temp.x>0 && temp.y>0) Quater=1;	// ÇØ´ç »çºÐ¸é °áÁ¤
    if (temp.x<0 && temp.y>0) Quater=2;
    if (temp.x>0 && temp.y<0) Quater=3;
    if (temp.x<0 && temp.y<0) Quater=4;

	if (Quater==0) return 0;
	if (temp.y==0) return 0;
    Ratio=(float)temp.x / (float)temp.y;
	if (Ratio<0) Ratio*=-1;	// ABS

	switch(Quater)
	{
	case 1 :
		{
			if (Ratio>=0     && Ratio<0.17) Direction=8;
            if (Ratio>=0.17 && Ratio<0.65) Direction=7;
            if (Ratio>=0.65 && Ratio<1.5)  Direction=6;
            if (Ratio>=1.5  && Ratio<5.5)  Direction=5;
            if (Ratio>=5.5  && Ratio<500)  Direction=4;
		}
		break;
	case 2 :
		{
			if (Ratio>=0     && Ratio<0.17) Direction=8;
            if (Ratio>=0.17 && Ratio<0.65) Direction=9;
            if (Ratio>=0.65 && Ratio<1.5)  Direction=10;
            if (Ratio>=1.5  && Ratio<5.5)  Direction=11;
            if (Ratio>=5.5  && Ratio<500)  Direction=12;
		}
		break;
	case 3 :
		{
			if (Ratio>=0     && Ratio<0.17) Direction=0;
            if (Ratio>=0.17 && Ratio<0.65) Direction=1;
            if (Ratio>=0.65 && Ratio<1.5)  Direction=2;
            if (Ratio>=1.5  && Ratio<5.5)  Direction=3;
            if (Ratio>=5.5  && Ratio<500)  Direction=4;
		}
		break;
	case 4 : 
		{
			if (Ratio>=0     && Ratio<0.17) Direction=0;
            if (Ratio>=0.17 && Ratio<0.65) Direction=15;
            if (Ratio>=0.65 && Ratio<1.5)  Direction=14;
            if (Ratio>=1.5  && Ratio<5.5)  Direction=13;
            if (Ratio>=5.5  && Ratio<500)  Direction=12;
		}
		break;
	}

	return Direction;
}

void MakeBreathData(int Index,int Count,int x,int y)
{
/*	t_CommonArg Arg;
	for (int i=0;i<Count;i++)
	{
		Arg.BreathArg.SpriteIndex=Index;								// Packet Receive
		Arg.BreathArg.StartLocation.x=600;
		Arg.BreathArg.StartLocation.y=100;
		Arg.BreathArg.DestLocation.x=100;
		Arg.BreathArg.DestLocation.y=600;
		Arg.BreathArg.Direction=GetDirection(Arg.BreathArg.StartLocation,Arg.BreathArg.DestLocation);
		Arg.BreathArg.Power=5+rand()%10;
		Arg.BreathArg.StartTime=g_ClientTime+rand()%3000;
		cBreath* tempBreath=new cBreath(&Arg);
		g_ParticleManager.Insert(tempBreath);
	}*/
  // 直接在一個迴圈中創建所有粒子，或者透過計時器每隔幾幀創建一個
Count = rand() % Count + Count;

for (int i = 0; i < Count; ++i) {
  BreathArgs args = {};
  args.SpriteIndex = rand() % Index;
  args.StartLocation = {x,y};
  args.Direction = DIRECTION_LEFTDOWN;
  args.Power = 51 + (rand() % 68);
  args.StartTime = g_curr_time;

  if (auto p = g_ParticleFactory.createBreath(args)) {
    g_ParticleManager.Insert(std::move(p));
  }
}

}

void RecvBreath(int x,int y)
{
	//g_EffectSpriteManager.CheckAndLoadSpriteData(9);	// Packet Receive

	MakeBreathData(10,25,x,y);
}

void MakeFogData(int Index,int Count)
{
/*	t_CommonArg Arg;
	for (int i=0;i<Count;i++)
	{
		Arg.FogArg.SpriteIndex=Index;								// Packet Receive
		Arg.FogArg.StartTime=g_curr_time+rand()%20;
		Arg.FogArg.DelayTime=Arg.FogArg.StartTime+50+rand()%5;				// Packet Receive
		Arg.FogArg.Location.x=rand()%800;					// Packet Receive
		Arg.FogArg.Location.y=rand()%600;
		cFog* tempFog=new cFog(&Arg);
		g_ParticleManager.Insert(tempFog);
	}*/
}

void RecvFog()					// Effect temp
{
/*	g_EffectSpriteManager.CheckAndLoadSpriteData(6);	// Packet Receive
	g_EffectSpriteManager.CheckAndLoadSpriteData(7);	// Packet Receive
	g_EffectSpriteManager.CheckAndLoadSpriteData(8);	// Packet Receive

	MakeFogData(6,20);
	MakeFogData(7,20);
	MakeFogData(8,40);

//		RecvBreath();*/
}

void RecvCMD_AI_FOG(t_packet* p)		// LTS NEW AI	// LTS DRAGON MODIFY	
{
  // REFACTOR: 不再需要 t_CommonArg Arg;
  t_AIFog* AIFog = &p->u.LocalWar.AIFog;
  /*
  // 這部分的邏輯保持不變，用於預先載入資源
  switch (AIFog->Type) {
    case 0:
      return;
    case 1:
      g_EffectSpriteManager.CheckAndLoadSpriteData(0);
      g_EffectSpriteManager.CheckAndLoadSpriteData(1);
      g_EffectSpriteManager.CheckAndLoadSpriteData(2);
      break;
    case 2:
      g_EffectSpriteManager.CheckAndLoadSpriteData(3);
      g_EffectSpriteManager.CheckAndLoadSpriteData(4);
      g_EffectSpriteManager.CheckAndLoadSpriteData(5);
      break;
    case 3:
      g_EffectSpriteManager.CheckAndLoadSpriteData(6);
      g_EffectSpriteManager.CheckAndLoadSpriteData(7);
      g_EffectSpriteManager.CheckAndLoadSpriteData(8);
      break;
  }*/

  for (int i = 0; i < FOG_MAX; i++) {
    // REFACTOR: 步驟 1 & 2 - 創建一個新的 FogArgs 來取代 t_CommonArg
    FogArgs args = {};  // 初始化結構

    /* switch (AIFog->Type) {
      case 0:
        continue;  // 如果 type 是 0，直接跳過這次迴圈
      case 1:
        args.SpriteIndex = rand() % 3;
        break;
      case 2:
        args.SpriteIndex = 3 + rand() % 3;
        break;
      case 3:
        args.SpriteIndex = 6 + rand() % 3;
        break;
    }*/
    args.SpriteIndex = rand() % 10;
    // 將封包中的資料填入新的 args 結構
    args.StartTime = g_curr_time + rand() % 5;
    args.DelayTime = args.StartTime + AIFog->Delay;
    args.Location.x = Mapx + AIFog->Location[i].x;
    args.Location.y = Mapy + AIFog->Location[i].y;

    // REFACTOR: 步驟 3 - 使用粒子工廠創建粒子
    // g_ParticleFactory.createFog(args) 會從內存池中取得一個 cFog 物件，
    // 並返回一個管理其生命週期的智慧指標 (ParticlePtr)。
    auto fogParticle = g_ParticleFactory.createFog(args);

    // REFACTOR: 步驟 4 - 插入粒子管理器
    // 必須檢查工廠是否成功創建了粒子 (如果池滿了，可能會返回 nullptr)
    if (fogParticle) {
      // 使用 std::move 將 fogParticle 的所有權轉移給管理器。
      // 這樣當粒子生命週期結束時，它會被自動歸還給內存池。
      g_ParticleManager.Insert(std::move(fogParticle));
    }
  }
}

void PrintMessage(int Index)			// LTS NEW AI
{
	if( !GetSysInfo(SI_GAME_MAKE_MODE ) ) return;		// 020925 YGI
#ifdef _DEBUG
	switch (Index)
	{
	case 1: AddCurrentStatusMessage(255, 255, 0, "1 呼叫了閃避/迴避的例行程式。");			break;
	case 2: AddCurrentStatusMessage(255, 255, 0, "2 呼叫了恢復 (Recovery) 的例行程式。");		break;
	case 3: AddCurrentStatusMessage(255, 255, 0, "3 呼叫了恐懼/驚嚇的例行程式。");			break;
	case 4: AddCurrentStatusMessage(255, 255, 0, "4 呼叫了霧氣的例行程式。");			break;
	case 5: AddCurrentStatusMessage(255, 255, 0, "5 呼叫了待機/等待的例行程式。");			break;
	case 6: AddCurrentStatusMessage(255, 255, 0, "6 呼叫了音效/聲音的例行程式。");		break;
	case 7: AddCurrentStatusMessage(255, 255, 0, "7 呼叫了召喚的例行程式。");			break;
	case 8: AddCurrentStatusMessage(255, 255, 0, "8 呼叫了地震的例行程式。");		break;
	case 9: AddCurrentStatusMessage(255, 255, 0, "9 呼叫了與登場/出現相關的例行程式。");		break;
	case 10: AddCurrentStatusMessage(255, 255, 0, "10 呼叫了召喚 2 的例行程式。");		break;
	case 11: AddCurrentStatusMessage(255, 255, 0, "11 呼叫了龍 (Dragon) 傳送 (Teleport) 的例行程式。");		break;
	case 12: AddCurrentStatusMessage(255, 255, 0, "12 呼叫了龍 (Dragon) 霧 (Fog) 的例行程式。");			break;
	}
#endif
}
void RecvCMD_AI_ACTION_TEST(t_packet* p)		// LTS NEW AI
{
   PrintMessage(p->u.LocalWar.ActionTest.Action[0]);
   PrintMessage(p->u.LocalWar.ActionTest.Action[1]);
   PrintMessage(p->u.LocalWar.ActionTest.Action[2]);
}

void RecvCMD_AI_MOVE(t_packet* p)		// LTS NEW AI
{
	LPCHARACTER ch=FindHim(p->u.LocalWar.AIMove.ID+10000);
	if (NULL!=ch) 
	{
		ch->x=p->u.LocalWar.AIMove.X;
		ch->y=p->u.LocalWar.AIMove.Y;
	}
}

void RecvCMD_AI_SOUND(t_packet* p)
{
	PlayListAutoSounds(62,0,0,FALSE);					// 020115 LTS
}

void RecvCMD_AI_NPC_ACTION(t_packet* p)	// LTS NEW AI
{
	LPCHARACTER ch=FindHim(p->u.LocalWar.AINPCAction.ID+10000);
	if (!ch) return;

	ch->nCurrentAction=p->u.LocalWar.AINPCAction.Action;
	ch->nCurrentFrame.PutEncrypted(0);
	ch->direction=DIRECTION_LEFTDOWN;

	if (p->u.LocalWar.AINPCAction.Action==4)
	{
		RecvBreath(ch->x,ch->y);
	}
#ifdef _DEBUG
	if( GetSysInfo(SI_GAME_MAKE_MODE ) )	// 020925 YGI
	AddCurrentStatusMessage(255, 255, 0, "已透過模式 (Pattern) 更改動作 (Action)。 動作：%d", ch->nCurrentAction);
#endif
}

void RecvCMD_AI_EARTHQUAKE(t_packet* p)					// LTS DRAGON MODIFY
{
	MakeEarthQuake(p->u.LocalWar.CommonDataC.Data*5);
}

void SendCMD_SET_QUESTMODE(const char* Arg)					// LTS DRAGON MODIFY
{
	t_packet packet;
	packet.h.header.type=CMD_SET_QUESTMODE;
	if (stricmp("ON",Arg)==0) packet.u.LocalWar.CommonDataC.Data=true;
	else packet.u.LocalWar.CommonDataC.Data=false;
	packet.h.header.size=sizeof(t_CommonDataC);
	QueuePacket(&packet,1);
	if (packet.u.LocalWar.CommonDataC.Data)
		AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,373));	//"QUEST MODE [ON], Äù½ºÆ®¸¦ ¼öÇàÇÏ½Ç¼ö ÀÖ´Â ¸ðµåÀÔ´Ï´Ù.");
	else
		AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,374));	//"QUEST MODE [OFF], Äù½ºÆ®¸¦ ¼öÇàÇÏ½Ç¼ö ¾ø´Â ¸ðµåÀÔ´Ï´Ù.");

}

void SendAction(int i,int Direction)			// LTS ACTION 
{
	if (Hero->hp<=0)
	{
		return;
	}// LTS ACTION BUG
	if (Hero->HorseNo>0) // LTS ACTION BUG
	{
		return;
	}
  if (Hero->nCurrentAction==ACTION_SITDOWN)   // LTS ACTION BUG
    return;
	if (!Hero->peacests)
	{
		t_packet packet;
		packet.h.header.type=CMD_COMMUNITY_ACTION;
		packet.u.LocalWar.CommunityActionS.Action=i;
		packet.u.LocalWar.CommunityActionS.Direction=Direction;
		packet.h.header.size=sizeof(t_CommunityActionS);
		
		QueuePacket(&packet,1);
	}
	else
		AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,375)); //"ÀüÅõ¸ðµå¿¡¼­´Â µ¿ÀÛÀ» ¼öÇàÇÒ ¼ö ¾ø½À´Ï´Ù.");
}

void RecvCMD_COMMUNITY_ACTION(t_packet* p)			// LTS ACTION
{
	if (p->u.LocalWar.CommunityAction.Result)
	{
		LPCHARACTER ch=FindHim(p->u.LocalWar.CommunityAction.ID);
		if (NULL!=ch) 
		{
			ch->nCurrentAction=p->u.LocalWar.CommunityAction.Action;
			ch->direction=(DIRECTION)p->u.LocalWar.CommunityAction.Direction;
			ch->nCurrentFrame.PutEncrypted(0);
		}
	}
	else
	{
		switch(p->u.LocalWar.CommunityAction.Action)
		{
		case ACTION_VICTORY	: AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,376)); break; //"½Â¸®Æ÷Áî´Â ·¹º§ÀÌ µÇ¾î¾ß »ç¿ëÇÒ¼ö ÀÖ½À´Ï´Ù."); break;
		case ACTION_ANGER	: AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,377)); break;//"È­³»±â´Â ·¹º§ÀÌ µÇ¾î¾ß »ç¿ëÇÒ¼ö ÀÖ½À´Ï´Ù."); break;
		}
	}
}

void SetDragonLordWarArmor(t_server_user_add *c)				// LTS DRAGONLORD
{
	switch(c->DragonLordWarTeam)
	{
	case BLUETEAM : c->equip0=fight_map_acc_num[1];	break;
	case REDTEAM  : c->equip0=fight_map_acc_num[0];	break;
	}
	c->equip1=1;
	LoadHeroClothAccessoryDataOne( c->spr_no, c->equip0 );
	LoadHeroClothAccessoryDataOne( c->spr_no, c->equip1 ); 
}

void SetDragonLordWarArmor(t_server_change_equip *p,LPCHARACTER ch)		// LTS DRAGONLORD
{
	switch(ch->DragonLordWarTeam)
	{
	case BLUETEAM : p->equip[0]=fight_map_acc_num[1];	break;
	case REDTEAM : p->equip[0]=fight_map_acc_num[0];	break;
	}
	p->equip[1]=1;
	LoadHeroClothAccessoryDataOne( ch->sprno, p->equip[0] );
	LoadHeroClothAccessoryDataOne( ch->sprno, p->equip[1] ); 
}

void SendActiveDragonLordWar()
{
	t_packet packet;

	packet.h.header.type=CMD_ACTIVE_DRAGONLORD_WAR;
	packet.h.header.size=0;
	QueuePacket(&packet,1);
}

void RecvCMD_DRAGONLORDWAR_TEAM_CHANGE(t_packet* p)
{
	LPCHARACTER	ch=FindHim(p->u.LocalWar.LordWarTeamChange.ID);
	if (!ch) return;

	ch->DragonLordWarTeam=p->u.LocalWar.LordWarTeamChange.TeamNo;
}

void SendCheckDualFame()    // LTS DUAL FAME
{
	t_packet packet;

	packet.h.header.type=CMD_REQUEST_CHECK_DUAL_FAME;
	packet.h.header.size=0;

	QueuePacket(&packet,1);
}

void RecvCMD_ANSWER_CHECK_DUAL_FAME(t_packet* p)  // LTS DUAL FAME
{
	AddCurrentStatusMessage(255,255,0,lan->OutputMessage(0,202),p->u.LocalWar.CommonDataDw.Data);	
	// "ÇöÀç µà¾ó¸í¼ºÀº [%d]ÀÔ´Ï´Ù. µà¾ó¸í¼ºÀº µà¾óÀÎ °æ¿ì¿¡¸¸ Àû¿ëµË´Ï´Ù."
}

int CheckHandleByLTSSupport(t_packet* p)
{
	switch (p->h.header.type)
	{
	case CMD_CHECK_ABILITY_CHANGE_MONEY :
											RecvCMD_CHECK_ABILITY_CHANGE_MONEY(p);
											break;
	case CMD_LOCALWAR_STATUS :
											RecvCMD_LOCALWAR_STATUS(p);
											break;
	case CMD_ANSWER_LOCALWAR_INFO :
											RecvCMD_ANSWER_LOCALWAR_INFO(p);
											break;
	case CMD_LOCALWAR_JOIN_RESULT :
											RecvCMD_LOCALWAR_JOIN_RESULT(p);
											break;
	case CMD_USER_JOIN_LOCALWAR :
											RecvCMD_USER_JOIN_LOCALWAR(p);
											break;
	case CMD_LOCALWAR_BEGIN :
											RecvCMD_LOCALWAR_BEGIN(p);
											break;
	case CMD_LOCALWAR_BEGIN_TIME :
											RecvCMD_LOCALWAR_BEGIN_TIME(p);
											break;
	case CMD_LOCALWAR_END :
											RecvCMD_LOCALWAR_END(p);
											break; 
	case CMD_LOCALWAR_RESULT:
											RecvCMD_LOCALWAR_RESULT(p);
											break;
	case CMD_LOCALWAR_END_CHAR_RESULT:
											RecvCMD_LOCALWAR_END_CHAR_RESULT(p); 
											break;
	case CMD_SEALSTONE_BROKED :
											RecvCMD_SEALSTONE_BROKED(p);
											break;
	case CMD_SEALSTONE_ATTACKED :
											RecvCMD_SEALSTONE_ATTACKED(p);
											break;
	case CMD_LOCALWAR_POINT_BBS :
											RecvCMD_LOCALWAR_POINT_BBS(p);
											break;
	case CMD_ACTION_STATUS :
											RecvCMD_ACTION_STATUS(p);
											break;
	case CMD_SITDOWN_DIRECTION_CHANGED :
											RecvCMD_SITDOWN_DIRECTION_CHANGED(p);
											break;
	case CMD_LOCALWAR_STARTUP_STATUS :
											RecvCMD_LOCALWAR_STARTUP_STATUS(p);
											break;
/*	case CMD_EVENT_LOCALWAR_STATUS_CHANGE :
											RecvCMD_EVENT_LOCALWAR_STATUS_CHANGE(p);
											break;*/
/*	case CMD_EVENT_LOCALWAR_REMAIN_TIME :
											RecvCMD_EVENT_LOCALWAR_REMAIN_TIME(p);
											break;*/
	case CMD_EVENT_LOCALWAR_JOIN_RESULT :
											RecvCMD_EVENT_LOCALWAR_JOIN_RESULT(p);
											break;
	case CMD_EVENT_LOCALWAR_MOVE_RESULT :
											RecvCMD_EVENT_LOCALWAR_MOVE_RESULT(p);
											break;
	case CMD_EVENT_LOCALWAR_MESSAGE :
											RecvCMD_EVENT_LOCALWAR_MESSAGE(p);
											break;
	case CMD_EVENT_POINT_CHANGE :
											RecvCMD_EVENT_POINT_CHANGE(p);
											break;
	case CMD_EVENT_REMAIN_TIME :												// LTS BUG
											RecvCMD_EVENT_REMAIN_TIME(p);
											break;
	case CMD_EVENT_MONSTER_CREATED :											// LTS AI2
											RecvCMD_EVENT_MONSTER_CREATED(p);
											break;
	case CMD_LOCALWAR_NATION_RESULT :											// LTS NEW LOCALWAR	
											RecvCMD_LOCALWAR_NATION_RESULT(p);
											break;
	case CMD_LOCALWAR_MAPMOVE_RESULT :
											RecvCMD_LOCALWAR_MAPMOVE_RESULT(p);	// LTS NEW LOCALWAR	
											break;
	case CMD_LOCALWAR_FAME_EXP :												// LTS NEW LOCALWAR
											RecvCMD_LOCALWAR_FAME_EXP(p);
											break;
	case CMD_LOCALWAR_DEFENCE_POINT :											// LTS NEW LOCALWAR	
											RecvCMD_LOCALWAR_DEFENCE_POINT(p);
											break;
	case CMD_LOCALWAR_CHAT :
											RecvCMD_LOCALWAR_CHAT(p);
											break;



	case CMD_NATION_CHAT:
										RecvCMD_NATION_CHAT(p);
									    break;





	case CMD_AI_FOG :															// LTS NEW AI	
											RecvCMD_AI_FOG(p);
											break;
	case CMD_AI_SOUND :															// LTS NEW AI	
											RecvCMD_AI_SOUND(p);
											break;
	case CMD_AI_ACTION_TEST :													// LTS NEW AI	
											RecvCMD_AI_ACTION_TEST(p);
											break;
	case CMD_AI_MOVE :															// LTS NEW AI	
											RecvCMD_AI_MOVE(p);
											break;
	case CMD_GOTO_GUILD_HOUSE_RESULT :											//±è¿µÁÖ		
											RecvCMD_GOTO_GUILD_HOUSE_RESULT(p);
											break;
	case CMD_CHECK_GUILD_HOUSE_RESULT :											// ±è¿µÁÖ
											RecvCMD_CHECK_GUILD_HOUSE_RESULT(p);
											break;
	case CMD_AI_NPC_ACTION :													// LTS NEW AI
											RecvCMD_AI_NPC_ACTION(p);
											break;
	case CMD_AI_EARTHQUAKE :
											RecvCMD_AI_EARTHQUAKE(p);			// LTS DRAGON MODIFY
											break;	
	case CMD_COMMUNITY_ACTION :
											RecvCMD_COMMUNITY_ACTION(p);		// LTS ACTION
											break;
	case CMD_DRAGONLORDWAR_TEAM_CHANGE :
											RecvCMD_DRAGONLORDWAR_TEAM_CHANGE(p);	// LTS DRAGONLORD
											break;
  case CMD_ANSWER_CHECK_DUAL_FAME :                         // LTS DUAL FAME
											RecvCMD_ANSWER_CHECK_DUAL_FAME(p);
											break;

	default : return -31;
	}
	return 1;
}

//< LTH-040314-KO
extern DWORD g_dwRemainTime;
extern int g_aRemainGuard[2][4];
//> LTH-040314

//<! BBD 040311
void ShowMonsterWarInfo()
{
	// ¸ó½ºÅÍ ±¹°¡Àü ¸ÊÀÌ ¾Æ´Ï¶ó¸é ¶ç¿ìÁö ¸¶¶ó, ±¹°¡Àü ½ÃÀÛÀÌ ¾Æ´Ò¶§µµ ¸¶Âù°¡Áö
	if(!IsNeoWarfieldServer() || !g_bWarBegin)	// BBD 040313 // LTH-040314-KO ÀüÀï ÆÇ´Ü º¯¼ö°¡ g_bWarBeginÀÌ¾ú´ç -_-;
	{
		return;
	}

	// ¸ó½ºÅÍ ±¹°¡ÀüÀÌ´Ù ÀÌ¹ÌÁö¿Í ±ÛÀÚ¸¦ ¾Ë¸Â°Ô ¶ç¿öÁÖÀÚ
	Spr * pSpr = 0L;
	
	pSpr = GetSprOfMenu( MONSTER_WAR_IMG, 3);
	PutCompressedImageFX( 654,  16 , pSpr, 20, 1 );
	
	//< LTH-040314-KO Ç¥½ÃÇÒ ½Ã°£ °è»ê
	WORD wHour = 0;
	WORD wMin = 0;
	WORD wSec = 0;
	DWORD dwRemainTime = 0;
	dwRemainTime = g_dwRemainTime - (g_ClientTime / 1000);
	GetTime(dwRemainTime, wHour, wMin, wSec);
	char szTime[10] = {0,};
	sprintf(szTime, "[%2d:%2d]", wHour, wMin);	// LTH-040323-KO ÃÊ´ÜÀ§ Ç¥½Ã¸¦ ¾ø¾Ý´Ù
	char szGuardCount[3] = {0,};
	//> LTH-040314-KO
	
	int x = 545;
	int y = 32;
	for(int i = 0; i < 3; i++, x += 70)
	{
		pSpr = GetSprOfMenu( MONSTER_WAR_IMG, i);
		if(pSpr)
		{
			PutCompressedImage(x, y, pSpr);
		}
		
		ZeroMemory(szGuardCount, sizeof(char) * 3);
		switch (i)
		{
		case 0:
            snprintf(szGuardCount, sizeof(szGuardCount), "%d", g_aRemainGuard[TEAM2][0]);
			//itoa(g_aRemainGuard[TEAM2][0], szGuardCount, 10);
			::HprintBold( x, y-22, ::Convert16to32(FONT_COLOR_WHITE) ,::Convert16to32(FONT_COLOR_BLACK), \
				szGuardCount);
			break;

		case 1:
            snprintf(szGuardCount, sizeof(szGuardCount), "%d", g_aRemainGuard[TEAM2][3] + g_aRemainGuard[TEAM2][1]);
			//itoa(g_aRemainGuard[TEAM2][3] + g_aRemainGuard[TEAM2][1], szGuardCount, 10);
			::HprintBold( x, y-22, ::Convert16to32(FONT_COLOR_WHITE) ,::Convert16to32(FONT_COLOR_BLACK), \
				szGuardCount);
			break;

		case 2:
            snprintf(szGuardCount, sizeof(szGuardCount), "%d", g_aRemainGuard[TEAM2][2]);
			//itoa(g_aRemainGuard[TEAM2][2], szGuardCount, 10);
			::HprintBold( x, y-22, ::Convert16to32(FONT_COLOR_WHITE) ,::Convert16to32(FONT_COLOR_BLACK), \
				szGuardCount);
			break;
		}
	}
	// BBD 040313
	::HprintBold( x - 22, y-22, ::Convert16to32(FONT_COLOR_WHITE) ,::Convert16to32(FONT_COLOR_BLACK),
				szTime);

}
//> BBD 040311