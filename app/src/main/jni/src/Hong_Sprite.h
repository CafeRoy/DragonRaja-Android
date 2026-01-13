#define SYSTEM_SPRITE_MAX_	  1500	// 0121 YGI

#define MAX_CHARACTER_SPRITE_ 200 // CSD-030417
#define MAX_CHARACTOR_PART_   6
#define MAX_CHARACTER_LAYER_  6

#define MUTANT_COMMON    0
#define MUTANT_ENCHANT   1
#define MUTANT_MADDER    2
#define MUTANT_BOSS      3
#define MUTANT_HIGH_RANK 4
#define MUTANT_SPECIAL   5
#define MUTANT_COMPANY   6

#define LD_CHAR_SPRITE_ONLY_MEM_ALLOC		0
#define LD_CHAR_SPRITE_ALL_LOAD				1

typedef struct {
		DWORD	BitMaskR;
		DWORD	BitMaskG;
		DWORD	BitMaskB; 

		DWORD	ShiftRightR;
		DWORD	ShiftRightG;
		DWORD	ShiftRightB;

		DWORD	ShiftLeftR;
		DWORD	ShiftLeftG;
		DWORD	ShiftLeftB;
}TPixelInfo;









#ifndef __HONG_SPRITE_H__
#define __HONG_SPRITE_H__

	extern void	InitSpriteTransTable( );

	extern void	PutCmprsImgNOTCliping(int x, int y, int yl, LPVOID img, LPVOID dest );
	extern void	PutCmprsImgCliping(int x, int y, LPVOID img, LPVOID dest, int lx, int rx, int ty, int by);
	extern void	PutCompressedImage(int x, int y, Spr *sp );

	extern void PutCmprsImgClipingHBlend565( int x, int y, LPVOID Stmp, LPVOID dest, int lx, int rx, int ty, int by);
	extern void PutCmprsImgNOTClipingHBlend565( int x, int y, int yl, LPVOID Stmp, LPVOID dest);
	extern void PutCmprsImgClipingBlend565( int x, int y, LPVOID Stmp, LPVOID dest, int lx, int rx, int ty, int by, DWORD alpha);
	extern void PutCmprsImgNOTClipingBlend565( int x, int y, int yl, LPVOID Stmp, LPVOID dest, DWORD alpha);
	extern void PutCmprsImgClipingAdd565( int x, int y, LPVOID Stmp, LPVOID dest, int lx, int rx, int ty, int by, DWORD alpha);
	extern void PutCmprsImgNOTClipingAdd565(int x, int y, int yl, LPVOID Stmp, LPVOID dest,  DWORD alpha);
	extern void PutCmprsImgClipingSub565( int x, int y, LPVOID Stmp, LPVOID dest, int lx, int rx, int ty, int by, DWORD alpha);
	extern void PutCmprsImgNOTClipingSub565(int x, int y, int yl, LPVOID Stmp, LPVOID dest , DWORD alpha);

	
	extern void PutCompressedImageFX(int x, int y, Spr *sp, DWORD alpha, DWORD op);

	extern void	PutCmprsImgNOTClipingShadow565(int x, int y, int yl, LPVOID Stmp, LPVOID dest);
	extern void PutCompressedImageShadow( int x, int y, Spr *sp );

	
	extern void PutCmprsImgNOTClipingBlendRGB565(int x, int y, int yl, LPVOID Stmp, LPVOID dest, DWORD RGB );
	
	extern void PutCompressedImageBlendRGB( int x, int y, Spr *sp, DWORD RGB );

	
	extern void PutCmprsImgNOTClipingCharRGB565(int x, int y, int yl, LPVOID Stmp, LPVOID dest, DWORD RGB );
	extern void PutCompressedImageCharRGB( int x, int y, Spr *sp, DWORD RGB );

	extern void	PutCmprsImgVoidCliping(int x, int y, LPVOID Stmp, LPVOID dest, int lx, int rx, int ty, int by);
	extern void	PutCmprsImgVoidNOTCliping(int x, int y, int yl, LPVOID Stmp, LPVOID dest );
	extern void	PutCompressedImageVoid(int x, int y, Spr *sp );
	
	extern void	PutCompressImageApart( int x, int y, Spr *sp, int sx, int sy, int ex, int ey, int alpha );
	

	extern int	LoadCharSpriteData( char *filename, char **buf, DWORD *length, CharacterSprite *charactersprite, int mode );
	extern int LoadCharSpriteData_Sub( char *filename, char **buf, DWORD *length, CharacterSprite *charactersprite, int mode );
	extern void FreeCharacterSpriteData( char *buf );

	extern void	FreeCharacterSpriteDataAll( int s, int e );
	
	extern void LoadHeroSpriteData( void );
	extern void FreeHeroSpriteData( void );

	extern void LoadHeroClothAccessoryData( int s, int e );
	extern void FreeHeroClothAccessoryData( void );
	extern void FreeHeroClothAccessoryDataOne( int sprno, int no );
	extern void LoadHeroClothAccessoryDataOne( int sprno, int no );




	extern void RainAnimationOutput( void );
	extern void RainAnimationSetting( void );
	extern void RainStart( int number, int flag );
	extern void RainStop( void );
	extern void ThunderStop( void ); //Eleval 12/08/09 - Thunderstorm
	extern void ToggleRainHide(bool flag); //Eleval 16/08/09
	extern void ToggleSnowHide(bool flag); //Eleval 16/08/09
	extern void SnowAnimationOutput( void );
	extern void SnowAnimationSetting( void );
	extern void SnowStart( int number, int flag );
	extern void SnowStop( void );

    extern void UpdateSnowSmoothly(float dt);
    extern void UpdateRainSmoothly(float dt);

	extern int  InitWeather( void );
	extern void WeatherDataProcess( DWORD value, DWORD another, DWORD rainstart, DWORD rainend, int amount, int temperature );

	extern void CharWalkTime( LPCHARACTER ch, int ani );
	extern int CharCalcMovP( LPCHARACTER ch );
	extern void LoadChacterAnimationData( LPCHARACTER lpCharacter );
    extern void LoadChacterAnimationData_Tool( LPCHARACTER lpCharacter );
	extern void LoadClothOrderData( void );

	extern void	PutTileCliping( int x, int y, LPVOID Stmp, LPVOID dest, int lx, int rx, int ty, int by );
	extern void	PutTileNotCliping( int x, int y, LPVOID Stmp, LPVOID dest );
	extern void	PutTile( int x, int y, Spr* s, LPVOID Stmp, LPVOID dest );
    extern void PutTileAlpha(int x, int y, Spr *s, LPVOID Stmp, LPVOID dest,int alpha);

	extern void PutCompressedImageWaveTile(int x, int y, Spr *sp );

	extern WORD RGB16( int r, int g, int b );
	extern DWORD ReturnBlendRGB( int r, int g, int b );

///////////////////////////////////////////////////////////////////

	extern Spr	spr[ SYSTEM_SPRITE_MAX_];

	extern CharacterSprite		CharSpr[MAX_CHARACTER_SPRITE_];
	extern char					*CharBuf[MAX_CHARACTER_SPRITE_];
	extern DWORD				 CharBufLength[ MAX_CHARACTER_SPRITE_];
	extern int					 CharBufCount[MAX_CHARACTER_SPRITE_];

	extern CharacterSprite		CharAccessorySpr[2][CHARACTER_ACCESSORY_MAX_];	// ����..
	extern char					*CharAccessoryBuf[2][CHARACTER_ACCESSORY_MAX_];
	extern DWORD				 CharAccessoryBufLength[2][CHARACTER_ACCESSORY_MAX_];
	extern int					 CharAccessoryCount[2][CHARACTER_ACCESSORY_MAX_];

	extern CharacterSprite CharLayerSpr[MAX_CHARACTER_SPRITE_][MAX_CHARACTOR_PART_][MAX_CHARACTER_LAYER_];
	extern char*           CharLayerBuf[MAX_CHARACTER_SPRITE_][MAX_CHARACTOR_PART_][MAX_CHARACTER_LAYER_];
	extern DWORD				   CharLayerBufLength[MAX_CHARACTER_SPRITE_][MAX_CHARACTOR_PART_][MAX_CHARACTER_LAYER_];
	extern int					   CharLayerBufCount[MAX_CHARACTER_SPRITE_][MAX_CHARACTOR_PART_][MAX_CHARACTER_LAYER_];

	
	

	extern TPixelInfo			_PixelInfo;
	extern WORD 				wDxSize;
	extern DWORD				dDxSize;
	extern 	WORD 				wSurDxSize;
	extern DWORD				dSurDxSize;

	extern int					WaveTileStart;

	//////////////////// SoundUp lkh ���� ///////////////////
	extern DWORD	RainTime;
	extern DWORD	RainStartTime, RainEndTime;
	extern int		RainAmount;
	extern BOOL		Statue_Weather;

#else
	
	void	InitSpriteTransTable( );
	
	void	PutCmprsImgNOTCliping(int x, int y, int yl, LPVOID img, LPVOID dest );
	void	PutCmprsImgCliping(int x, int y, LPVOID img, LPVOID dest, int lx, int rx, int ty, int by);
	void	PutCompressedImage(int x, int y, Spr *sp );

	void	PutCmprsImgClipingHBlend565( int x, int y, LPVOID Stmp, LPVOID dest, int lx, int rx, int ty, int by);
	void	PutCmprsImgNOTClipingHBlend565( int x, int y, int yl, LPVOID Stmp, LPVOID dest);
	void	PutCmprsImgClipingBlend565( int x, int y, LPVOID Stmp, LPVOID dest, int lx, int rx, int ty, int by, DWORD alpha);
	void	PutCmprsImgNOTClipingBlend565( int x, int y, int yl, LPVOID Stmp, LPVOID dest, DWORD alpha);
	void	PutCmprsImgClipingAdd565( int x, int y, LPVOID Stmp, LPVOID dest, int lx, int rx, int ty, int by, DWORD alpha);
	void	PutCmprsImgNOTClipingAdd565(int x, int y, int yl, LPVOID Stmp, LPVOID dest,  DWORD alpha);
	void	PutCmprsImgClipingSub565( int x, int y, LPVOID Stmp, LPVOID dest, int lx, int rx, int ty, int by, DWORD alpha);
	void	PutCmprsImgNOTClipingSub565(int x, int y, int yl, LPVOID Stmp, LPVOID dest , DWORD alpha);

	void	PutCompressedImageFX(int x, int y, Spr *sp, DWORD alpha, DWORD op);


	void	PutCmprsImgNOTClipingShadow565(int x, int y, int yl, LPVOID Stmp, LPVOID dest);
	void	PutCompressedImageShadow( int x, int y, Spr *sp );

	
	void	PutCmprsImgNOTClipingBlendRGB565(int x, int y, int yl, LPVOID Stmp, LPVOID dest, DWORD RGB );
	void	PutCompressedImageBlendRGB( int x, int y, Spr *sp, DWORD RGB );

	void	PutCmprsImgNOTClipingCharRGB565(int x, int y, int yl, LPVOID Stmp, LPVOID dest, DWORD RGB );
	void	PutCompressedImageCharRGB( int x, int y, Spr *sp, DWORD RGB );

	void	PutCmprsImgVoidCliping(int x, int y, LPVOID Stmp, LPVOID dest, int lx, int rx, int ty, int by);
	void	PutCmprsImgVoidNOTCliping(int x, int y, int yl, LPVOID Stmp, LPVOID dest );
	void	PutCompressedImageVoid(int x, int y, Spr *sp );

	void	PutCompressImageApart( int x, int y, Spr *sp, int sx, int sy, int ex, int ey, int alpha );

	void	FreeCharacterSpriteDataAll( int s, int e );

		
	void    LoadHeroSpriteData( void );
	void    FreeHeroSpriteData( void );

	

	void	RainAnimationOutput( void );
	void	RainAnimationSetting( void );
	void	RainStart( int number, int flag );
	void	RainStop( void );


	void	PutTileCliping( int x, int y, LPVOID Stmp, LPVOID dest, int lx, int rx, int ty, int by );
	void	PutTileNotCliping( int x, int y, LPVOID Stmp, LPVOID dest );
	void	PutTile( int x, int y, Spr* s, LPVOID Stmp, LPVOID dest );
    void	PutTileAlpha(int x, int y, Spr* s, LPVOID Stmp, LPVOID dest,int alpha);


	void    PutCompressedImageWaveTile(int x, int y, Spr *sp );

	extern void PutCmprsImgClipingLight_CPP(int x, int y, Spr* sp,void* dest, int destPitch,int lx, int rx, int ty, int by,int alpha);
    extern void PutCmprsImgNOTClipingLight_CPP(int x, int y, Spr* sp,void* dest, int destPitch, int alpha); 
	extern void PutCmprsImgNOTClipingWaveTile_CPP(int x, int y, Spr* sp,void* dest,int destPitch); 
	extern void PutCmprsImgClipingWaveTile_CPP(int x, int y, Spr* sp,void* dest, int destPitch,int lx, int rx, int ty,int by);
	extern void PutCmprsImgClipingCharRGB565_CPP(int x, int y, Spr* sp,void* dest, int destPitch, int lx, int rx, int ty,int by, uint32_t RGB);
    extern void PutCmprsImgNOTClipingCharRGB565_CPP(int x, int y, Spr* sp,void* dest,int destPitch, uint32_t RGB);
	extern void PutCmprsImgClipingBlendRGB565_CPP(int x, int y, Spr* sp,void* dest, int destPitch,int lx, int rx, int ty,int by, uint32_t RGB);
    extern void PutCmprsImgNOTClipingBlendRGB565_CPP(int x, int y, Spr* sp,void* dest, int destPitch,uint32_t RGB);
	extern void PutCmprsImgCliping_CPP(int x, int y, Spr* sp, void* dest,int destPitch, int lx, int rx,int ty, int by);
    extern void PutCmprsImgNOTCliping_CPP(int x, int y, Spr* sp, void* dest,int destPitch);
	extern void PutCmprsImgVoidCliping_CPP(int x, int y, Spr* sp, void* dest, int destPitch, int lx, int rx, int ty, int by);
    extern void PutCmprsImgVoidNOTCliping_CPP(int x, int y, Spr* sp, void* dest, int destPitch);
	extern void PutCmprsImgNOTClipingShadow565_CPP(int x, int y, int yl,const void* Stmp,void* dest, uint32_t dDxSize);
	// 1. 標準 Alpha 混合
	extern void PutCmprsImgClipingBlend565_CPP(int x, int y, Spr* sp, void* dest, int destPitch, int lx, int rx, int ty, int by, int alpha);

	// 2. 加色混合
	extern void PutCmprsImgClipingAdd565_CPP(int x, int y, Spr* sp, void* dest, int destPitch, int lx, int rx, int ty, int by, int alpha);

	// 3. 減色混合
	extern void PutCmprsImgClipingSub565_CPP(int x, int y, Spr* sp, void* dest, int destPitch, int lx, int rx, int ty, int by, int alpha);

	// 4. 半透明混合 (50% Alpha)
	extern void PutCmprsImgClipingHBlend565_CPP(int x, int y, Spr* sp, void* dest, int destPitch, int lx, int rx, int ty, int by);

	// 5. 差異加色混合
	extern void PutCmprsImgClipingAddDifer565_CPP(int x, int y, Spr* sp, void* dest, int destPitch, int lx, int rx, int ty, int by, int alpha);

	// 6. 差異 Alpha 混合
	extern void PutCmprsImgClipingBlendDifer565_CPP(int x, int y, Spr* sp, void* dest, int destPitch, int lx, int rx, int ty, int by, int alpha);

	// 1. 標準 Alpha 混合
	extern void PutCmprsImgNOTClipingBlend565_CPP(int x, int y, Spr* sp, void* dest, int destPitch, int alpha);

	// 2. 加色混合
	extern void PutCmprsImgNOTClipingAdd565_CPP(int x, int y, Spr* sp, void* dest, int destPitch, int alpha);

	// 3. 減色混合
	extern void PutCmprsImgNOTClipingSub565_CPP(int x, int y, Spr* sp, void* dest, int destPitch, int alpha);

	// 4. 半透明混合 (50% Alpha)
	extern void PutCmprsImgNOTClipingHBlend565_CPP(int x, int y, Spr* sp, void* dest, int destPitch);

	// 5. 差異加色混合
	extern void PutCmprsImgNOTClipingAddDifer565_CPP(int x, int y, Spr* sp, void* dest, int destPitch, int alpha);

	// 6. 差異 Alpha 混合
	extern void PutCmprsImgNOTClipingBlendDifer565_CPP(int x, int y, Spr* sp, void* dest, int destPitch, int alpha);
/////////////////////////////////////////////////////////////////////////////////////////
	
	Spr			 spr[SYSTEM_SPRITE_MAX_] = {0,};
	
	CharacterSprite		 CharSpr[MAX_CHARACTER_SPRITE_] = {0,};
	char				*CharBuf[ MAX_CHARACTER_SPRITE_]= {0,};
	DWORD				 CharBufLength[ MAX_CHARACTER_SPRITE_]= {0,};
	int					 CharBufCount[MAX_CHARACTER_SPRITE_]= {0,};

	CharacterSprite		CharAccessorySpr[2][CHARACTER_ACCESSORY_MAX_]= {0,};	// ����..
	char				*CharAccessoryBuf[2][CHARACTER_ACCESSORY_MAX_]= {0,};
	DWORD				 CharAccessoryBufLength[2][CHARACTER_ACCESSORY_MAX_]= {0,};
	int					 CharAccessoryCount[2][CHARACTER_ACCESSORY_MAX_]= {0,};

    CharacterSprite	CharLayerSpr[MAX_CHARACTER_SPRITE_][MAX_CHARACTOR_PART_][MAX_CHARACTER_LAYER_]= {0,};
    char*           CharLayerBuf[MAX_CHARACTER_SPRITE_][MAX_CHARACTOR_PART_][MAX_CHARACTER_LAYER_]= {0,};
	DWORD				    CharLayerBufLength[MAX_CHARACTER_SPRITE_][MAX_CHARACTOR_PART_][MAX_CHARACTER_LAYER_]= {0,};
	int					    CharLayerBufCount[MAX_CHARACTER_SPRITE_][MAX_CHARACTOR_PART_][MAX_CHARACTER_LAYER_]= {0,};
	
	TPixelInfo			_PixelInfo;

	WORD 				wDxSize = 0;
	DWORD				dDxSize = 0;
	WORD 				wSurDxSize = 0;
	DWORD				dSurDxSize = 0;

	int					WaveTileStart = 0;
#endif



