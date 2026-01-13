#pragma once
#include <stdio.h>
#include <string>

#define MemAlloc(x, y)			__Alloc( (char **)(&(x)), (y), __FILE__, __LINE__ )

#define MemFree( x )			MemFree_Sub( (char **)(&(x)), __FILE__, __LINE__ )

#define NumberType( a, b )		{  INumberStartSprNo = a; INumberTypeWidth = b; }

#define Random( x )				(::rand() % ( (0 < (x) )? (x) : (1) ))

#define Fopen( filename, filemode )   FopenSub_( filename, filemode, __FILE__, __LINE__ )

#define __HONG_SUB_H__

	extern void Error ( char *mssg, ... );
	extern void MemFree_Sub( char **buf, char *file, int line  );
	extern void __Alloc( char **s, int size, const char *file, int line );
	extern FILE *FopenSub_( char *name, char *mode, char *wherefile, int line );
	extern int  Distance( int sx, int sy, int dx, int dy );

	extern void JustMsg(char *msg, ... );		// 011018 KHS

	extern int	YesOrNo( char *s, char *title );


	extern int  INumberStartSprNo, INumberTypeWidth;

	extern int  BoxAndDotCrash( int dx, int dy, int dxl, int dyl, int x, int y );
	extern int  BoxAndDotCrash( int dx, int dy, int dxl, int dyl, int x, int y, int dz, int dzl, int z);
	extern int  BoxAndBoxCrash( int ax, int ay, int axl, int ayl,  int bx, int by, int bxl, int byl );
	extern int  BoxAndBoxCrash( int ax, int ay, int axl, int ayl,  int bx, int by, int bxl, int byl, int az, int azl, int bz, int bzl );
	extern int CheckIntersect(int p1x, int p1y, int p2x, int p2y, int p3x, int p3y, int p4x, int p4y);


	extern int  ViewCheckRoutine( const int t );

	extern int	Determinant( int ax, int ay, int bx, int by, int X, int Y);
	extern int	IsInBox( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int x, int y );

	extern void	ReturnCircleResult(int s_X, int s_Y, int e_X, int e_Y, float &result_X, float &result_Y, int angle);
	extern void CurrentGameDate( DWORD t, int *y, int *mo, int *d, int *h, int *mi, int *sec );
	
	extern int  ITotalUseMemory;
	extern int  IMemUseCount;
    extern CHARACTERLIST g_CharacterList;
