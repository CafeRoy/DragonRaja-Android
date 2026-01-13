#define MAX_SOUND			2500

#define	SOUND_RAINY			480
#define	SOUND_LIGHTNING1	481
#define	SOUND_LIGHTNING2	482

#ifdef __cplusplus
extern "C"
{
#endif
extern BOOL		PlayListAutoSounds( int index, int x, int y, int flag);
extern void		StopWave( int index );
extern void		SetVolumeOne( int index, int volume );
extern void		SetVolume( int volume );
extern void		SetVolumeMusic( int volume );
extern bool		IsPlaying( int index );
extern void		WeatherFXSound(void);
extern void		StartSound( int type );		//  ʱ           		//021030 YGI

#ifdef __cplusplus
}
#endif