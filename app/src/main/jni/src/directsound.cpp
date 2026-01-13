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
#include "dragon.h"
#include "directsound.h"
#include "SmallMenu.h"
#include "Menu.h"

#define MemAlloc(x, y)				__Alloc( (char **)(&##x), (y), __FILE__, __LINE__ )
#define CHECKSUM_ERROR_SOUNDLIST	20009

#define WAVE_ENCODING_   10

extern void __Alloc( char **s, int size, const char *file, int line );

///////////////////////////////////////////////////////////////////////////////
// global variables

//SOUNDINFO				g_SoundInfo;
//SOUNDLIST*				g_SoundList;
//D3DVECTOR				d3dvPos;

extern int	RainAmount;
extern BOOL	Statue_Weather;
extern int	MapNumber;
int			BGM_SoundVolume=0;
BOOL		BGM_FadeInOut=0;
extern		SystemInfo	system_info;
///////////////////////////////////////////////////////////////////////////////
// function prototypes

void	StopWave( int index );
void	SetVolume( int volume );
void	SetVolumeMusic( int volume );
bool	IsPlaying( int index );
//////////////////// SoundUp lkh ���� ////////////////////
BOOL PlayListAutoSounds( int index, int x, int y, int flag)		//index->1~999����
{
	if(g_Volume_Off!=0)		return FALSE;

	if(index >= MAX_SOUND || index <= 0 )	return FALSE; 
	
	g_audioManager.PlaySoundWithPosition(system_info.sound, Hero->x/TILE_SIZE, Hero->y/TILE_SIZE, index, x, y, flag);
	
	return TRUE;
}

//021030 YGI
void
StopWave( int index )
{
	if (index == SN_START)
	{
		StartSound(1); 
		return;
	}
	//if( g_SoundInfo.lpDirectSoundBuffer[ index ] )
		//IDirectSoundBuffer_Stop( g_SoundInfo.lpDirectSoundBuffer[ index ] );
	g_audioManager.StopSound(index);
}



void SetVolumeOne( int index, int volume )
{
	g_audioManager.SetSoundVolume(index, volume);
}	
	
	
// volume : 0 - 100
void						//ȿ���� ���� ������ ���� ����
SetVolume( int volume )
{	
	/*if (volume == 0)
		volume = DSBVOLUME_MIN;
	else
		volume = (100 - (100 - volume)/5)* 100 + DSBVOLUME_MIN;*/
	for ( int i = SN_X_MAS_BGM +1; i < MAX_SOUND; i++ )
	{
		g_audioManager.SetSoundVolume(i, volume);
	}


}	
	
void						//��� ������ ���� ����(201->�������̽�~206�ΰ�����)
SetVolumeMusic( int volume )
{
	/*if (volume == 0)
		volume = DSBVOLUME_MIN;
	else
		volume = (100 - (100 - volume)/5)* 100 + DSBVOLUME_MIN;*/
	//////////////////////// SoundUp lkh ���� //////////////////////////
	for ( int i = SN_START; i <= SN_X_MAS_BGM; i++)
	{
		g_audioManager.SetSoundVolume(i, volume);
	}
}	
	
bool IsPlaying( int index )			//���ϰ��� true->�÷������� / false->Not playing
{	
	return (g_audioManager.IsPlayingSDL(index) != -1);
}	
	
static	int		old_Volume;
void	WeatherFXSound(void)			//RainAmount : 0 ~ 900
{	
	if( Statue_Weather==0 && IsPlaying(SOUND_RAINY))	//�� ���� �ʰ� �񳻸��� �Ҹ��� �÷��� �ǰ� �ִ� ���
		StopWave(SOUND_RAINY);
	
	if( IsPlaying(SOUND_RAINY) )
	{
		//////////////////////// õ�ռҸ� ���� ////////////////////////
		if( rand()%(4700-RainAmount*5)==1 && !IsPlaying(SOUND_LIGHTNING1) )
		{
			if (rand()%10>5)
			PlayListAutoSounds( SOUND_LIGHTNING1, rand()%100+100, rand()%100+50, 0 );	//3D sound�� ǥ��	// LTS SOUND
			else
			PlayListAutoSounds( SOUND_LIGHTNING1, (rand()%100+100)*-1, rand()%100+50, 0 );	//3D sound�� ǥ��
		}
		if( rand()%(4700-RainAmount*5)==1 && !IsPlaying(SOUND_LIGHTNING2) )
		{
			if (rand()%10>5)	// LTS SOUND
			PlayListAutoSounds( SOUND_LIGHTNING2, rand()%100+100, rand()%100+50, 0 );	//3D sound�� ǥ��
			else
			PlayListAutoSounds( SOUND_LIGHTNING2, (rand()%100+100)*-1, rand()%100+50, 0 );	//3D sound�� ǥ��
		}
			
		if(old_Volume!=RainAmount/16)
		{
			g_audioManager.MoveWave( SOUND_RAINY, (RainAmount/60));
			return;
		}
		else						return;
	}
	
	switch(Statue_Weather)
	{
	case 1:		//��Ҹ�(& ����Ҹ����� ����)
		PlayListAutoSounds( SOUND_RAINY, 0, 0/*15-(RainAmount/60)*/, 1 );	//3D sound�� ǥ�� // LTS SOUND
		break;
	case 2:		//���ö� �ٶ��Ҹ�

		break;
	}
	old_Volume = RainAmount/16; //Disabled by Eleval cause made another system
}	
	
static	int		temp_Old_Volume;

void StartSound( int type )		// �ʱ� ���� ����
{	//< CSD-030520
	static int nCount;
	if( type == 1 )		// ��ó��
	{
		nCount = 100;
		
	}

	if( nCount <= 0 ) return;

	g_audioManager.MoveWave(1, (nCount/6));

	nCount -= 3;
	if( nCount < 0) 
	{
		g_audioManager.StopSound(1);
	}
	
}	//> CSD-030520