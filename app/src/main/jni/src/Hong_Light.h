#define LIGHT_COL		80
#define LIGHT_ROW		44
#define LIGHT_MAX_    100
////////////////////// �̱��� 1029 ///////////////////////
#define MAX_OUTLIGHT_	50
//////////////////////////////////////////////////////////
////////////////////// �̱��� ���� 1030 ///////////////////////
typedef struct tagLIGHT{ 
	int x, y;
	int light;
	int flag;
	int frame;
	int terminateFrame;
}LIGHT, *LPLIGHT;


extern uint16_t	SCREEN_WIDTH;
extern uint16_t  SCREEN_HEIGHT;
extern uint16_t  GAME_SCREEN_XSIZE;
extern uint16_t  GAME_SCREEN_YSIZE;

extern int  MapBright( void );
extern void LoadBrightData( void );

extern void MakergbTable( void);

extern void OutputLight( int x, int y, int lightness, int color=0 );



extern int LightT[LIGHT_ROW][LIGHT_COL];

extern int LightTableX;
extern int LightTableY;

extern int DayLightControl;
extern int always_lightOn;

extern unsigned char *BrightSprBuf[ 21][2];

extern int   LightCount;
extern LIGHT Light[ LIGHT_MAX_];
extern LIGHT g_OutLight[ MAX_OUTLIGHT_];	


extern unsigned char BrightBuf[(960)*(540)];
extern WORD rgbTable[32 * 65536];
//////////////////// 0807 lkh �߰� //////////////////
extern void	PutCharacterLight(LPCHARACTER	lpChar, int time);




