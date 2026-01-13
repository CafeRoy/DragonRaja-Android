/*****************************************************************************\
* Copyright (c), Future Entertainment World / Seoul, Republic of Korea        *
* All Rights Reserved.                                                        *
*                                                                             *
* This document contains proprietary and confidential information.  No        *
* parts of this document or the computer program it embodies may be in        *
* any way copied, duplicated, reproduced, translated into a different         *
* programming language, or distributed to any person, company, or             *
* corporation without the prior written consent of Future Entertainment World *

	.���ѱ����� �ų�� ������, �ϸ�ȴ�.
	.â�Ƿ°� ���߷����� â������ ����, �ʵǸ� �ǰ� �϶�.
	.����� ���/ ����� ������ ���� �Ƹ����.

		
\*****************************************************************************/


#include "stdafx.h"
#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>
#include <math.h>
#include "Resource.h"
#include "Dragon.h"
#include "Hong_Sub.h"
#include "Hong_Sprite.h"
#include "Hangul.h"
#include "Tool.h"

int ITotalUseMemory;
int IMemUseCount;
int YesOrNo( char *s, char *title )
{
	
	return SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "", s, g_SDLInfo.window);
}	

/*��������������������������������������������������������������������������������
�Լ��� : void  ( char *mssg,... )
��  �� : Error Message�� ����ϰ� OK�� ������ �����Ѵ�.
��  �� :
��  �� : ����.
��������������������������������������������������������������������������������*/
void Error(char *msg, ...) {
  char caTemp[FILENAME_MAX];
  va_list va;

  if (!msg) return;
  va_start(va, msg);
  vsprintf(caTemp, msg, va);
  va_end(va);


  SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "", caTemp, g_SDLInfo.window);
  fflush(NULL);
  std::exit(0);

  return;
}

void MemFree_Sub( char **buf, char *file, int line  )
{
	if( *buf == NULL ) {return;}

    IMemUseCount--;

	SAFE_DELETE_ARRAY(*buf);
}

void __Alloc( char **s, int size, const char *file, int line )
{	
	SAFE_DELETE_ARRAY(*s);
    *s = new char[size];
	ITotalUseMemory += size;
    if( !(*s) )
	{
        Error( lan->OutputMessage(6,62), file, line, size );//lsw
	}
	::memset( *s, 0, size );
    IMemUseCount++;
}	

FILE *FopenSub_( char *name, char *filemode, char *wherefile, int line )
{ 
	return fopen( name, filemode );
}	
/*��������������������������������������������������������������������������������
�Լ��� : int Distance( int sx, int sy, int dx, int dy )
��  �� : ���������� �Ÿ��� ������ �����Ѵ�.
��  �� :
��  �� :
��������������������������������������������������������������������������������*/
int Distance( int sx, int sy, int dx, int dy )
{
   int ddx = abs( dx - sx );
   int ddy = abs( dy - sy );

   return ddx * ddx + ddy * ddy;
}

void JustMsg(char *s, ... )
{
    char szBuffer[1024]; // 確保緩衝區夠大

    va_list arg;
    va_start(arg, s);
    // 移除 if (arg == NULL) 判斷
    // 改用安全的 vsnprintf
    vsnprintf(szBuffer, sizeof(szBuffer), s, arg);
    va_end(arg);

    szBuffer[sizeof(szBuffer) - 1] = '\0'; // 確保結尾

	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"", szBuffer, g_SDLInfo.window);
    //g_DBGLog.Log(LOG_LV1, temp);
}	
	
int BoxAndDotCrash( int dx, int dy, int dxl, int dyl, int x, int y )
{
//	BoxOneColor( dx + IMapX, dy+ IMapY, dxl, dyl, 255);
	if( dx <= x && x <= dx + dxl  ) if( dy <= y && y <= dy+dyl ) return 1;
	return 0;
}	
int BoxAndBoxCrash( int ax, int ay, int axl, int ayl,  int bx, int by, int bxl, int byl )
{	
	if( ax + axl >= bx && bx + bxl >= ax &&	ay + ayl >= by && by + byl >= ay ) return 1;
	return 0;
}	

/////////////////////////////// �̱��� 1025 ���� /////////////////////////////////////////////	
int BoxAndDotCrash( int dx, int dy, int dxl, int dyl, int x, int y, int dz, int dzl, int z)
{
//	BoxOneColor( dx + IMapX, dy+ IMapY, dxl, dyl, 255);
	if( dx <= x && x <= dx + dxl  ) if( dy <= y && y <= dy+dyl )
		if(dz<=z && z<=dz+dzl)	return 1;		//���̰� üũ		
		else					return 0;
	return 0;
}	
	
	
int BoxAndBoxCrash( int ax, int ay, int axl, int ayl,  int bx, int by, int bxl, int byl, int az, int azl, int bz, int bzl )
{	
	
	if( ax + axl >= bx && bx + bxl >= ax &&	ay + ayl >= by && by + byl >= ay ) 
		if(az+azl >= bz && bz+bzl>= az)		return 1;		//���̰� üũ
		else								return 0;
	return 0;
}	
/////////////////////////////////////////////////////////////////////////////////////////////	
// ------------------------------------------------------------------
int ViewCheckRoutine( const int t )
{	
	return 0;
}					


int Determinant( int ax, int ay, int bx, int by, int X, int Y)
{
	int ret = ( ax * ( by - Y ) - ay * ( bx - X ) + bx * Y - (by * X ));
	return ret;
}

int IsInBox( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int x, int y )
{
		if( Determinant( x1,y1,x2,y2,x,y ) > 0 )
		if( Determinant( x2,y2,x3,y3,x,y ) > 0 )
		if( Determinant( x3,y3,x4,y4,x,y ) > 0 )
		if( Determinant( x4,y4,x1,y1,x,y ) > 0 )
			return 1;
	return 0;
}

// 使用 struct 來讓程式碼更清晰
struct Point {
	int x, y;
};

// 輔助函式：判斷三個點的相對方向
// 0 --> p, q, r 共線 (collinear)
// 1 --> 順時針 (Clockwise)
// 2 --> 逆時針 (Counterclockwise)
int orientation(Point p, Point q, Point r) {
	// 這邊的計算與你原本的 Determinant 函式原理相同
	long long val = (long long)(q.y - p.y) * (r.x - q.x) -
		(long long)(q.x - p.x) * (r.y - q.y);

	if (val == 0) return 0;  // 共線
	return (val > 0) ? 1 : 2; // 順時針或逆時針
}

// 輔助函式：檢查點 q 是否在線段 pr 上 (已知三點共線)
bool onSegment(Point p, Point q, Point r) {
	if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
		q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y)) {
		return true;
	}
	return false;
}

// 【修復完成】保留原始簽名的 CheckIntersect 函式
// 回傳 1 代表相交, 0 代表不相交
int CheckIntersect(int p1x, int p1y, int p2x, int p2y, int p3x, int p3y, int p4x, int p4y) {
	// 1. 將傳入的整數座標轉換為 Point 結構，方便處理
	Point p1 = { p1x, p1y };
	Point q1 = { p2x, p2y };
	Point p2 = { p3x, p3y };
	Point q2 = { p4x, p4y };

	// 2. 找出四種方向組合
	int o1 = orientation(p1, q1, p2);
	int o2 = orientation(p1, q1, q2);
	int o3 = orientation(p2, q2, p1);
	int o4 = orientation(p2, q2, q1);

	// 3. 一般情況：兩線段在彼此的兩側
	if (o1 != o2 && o3 != o4) {
		return 1;
	}

	// 4. 特殊情況：處理共線的狀況
	if (o1 == 0 && onSegment(p1, p2, q1)) return 1;
	if (o2 == 0 && onSegment(p1, q2, q1)) return 1;
	if (o3 == 0 && onSegment(p2, p1, q2)) return 1;
	if (o4 == 0 && onSegment(p2, q1, q2)) return 1;

	// 5. 其他所有情況都不相交
	return -1;
	// 注意：舊函式在不相交時回傳 -1，這裡改為回傳 0。
	// 如果你一定需要回傳 -1，可以將所有的 "return 0;" 改為 "return -1;"
}

/*
static inline void swap( int  *a, int *b )
{
	int t;

	t = *a;
	*a = *b;
	*b = t;	
}			
int CheckIntersect(int p1x, int p1y, int p2x, int p2y, int p3x, int p3y, int p4x, int p4y)
{			
    long m;	
    float x;

	if( p1x == p2x ) p1x++;
	if( p1y == p2y ) p1y++;
	if( p3x == p4x ) p3x++;
	if( p3y == p4y ) p3y++;

    if (p1x > p2x)
	{		
	swap(&p1x, &p2x);
	swap(&p1y, &p2y);
	}		
    if (p3x > p4x)
	{		
	swap(&p3x, &p4x);
	swap(&p3y, &p4y);
	}		
    m = (p4x - p3x)*(p2y - p1y) - (p4y - p3y)*(p2x - p1x);
			
    x = ((float)(p3y - p1y)*(p2x - p1x)*(p4x - p3x)
	 + (float)p1x*(p2y - p1y)*(p4x - p3x)
	 - (float)p3x*(p4y - p3y)*(p2x - p1x)) / (float)m;
    if (p1x <= x && x <= p2x && p3x <= x && x <= p4x) return 1;
    else return -1;
}			
*/			
			
				
					
///////////////////// 0525 lkh ���� ///////////////////
void	ReturnCircleResult(int s_X, int s_Y, int e_X, int e_Y, float &result_X, float &result_Y, int angle)
{					
	//int		result_X, result_Y;
	//������ǥ ����(�������� �������� ����)
	double radian = (double )angle * 3.141592 / 180.;
					
	e_X = e_X - s_X;
	e_Y = e_Y - s_Y;
					
	result_X = (float)( cos(radian)*e_X-sin(radian)*e_Y);
	result_Y = (float)(-sin(radian)*e_X+cos(radian)*e_Y);
					
	result_X += s_X;
	result_Y += s_Y;
					
//	sprintf(temp, "%3.1f-%3.1f", result_X, result_Y);
//	MessageBox(NULL, temp, 0, MB_OK);
}					

void CurrentGameDate( DWORD t, int *y, int *mo, int *d, int *h, int *mi, int *sec )
{
	DWORD rest;

	*y		= t / 31536000;		// 0
	rest	= t % 31536000;

	*mo		= rest / 2592000;   // +1
	rest	= rest % 2592000;

	*d		= rest / 86400;		// +1
	rest	= rest % 86400;

	*h		= rest / 3600;		//  0 
	rest	= rest % 3600;

	*mi		= rest / 60;		//  0
	*sec	= rest % 60;
}
