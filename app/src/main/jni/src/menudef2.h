/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

#ifndef __MENUDEF2_H__
#define __MENUDEF2_H__



extern bool CheckContinueLine( char check );
extern void SMenuReg(int iMainType,int x, int y , int iImageType,int iImageNumber, int iFieldCount,int iCheakType = 0,int iTemp = 0  ,int iWork = 0,int iKey = 0);
//021030 lsw
extern void SMenuFTReg(const int iMainType,const int iFieldNo,const int iButtonType,const int iSpecailWillDo,
						const int iX, const int iY,const int iWidth, const int iHeight,	const int iImageType, const int iRectImage = 0, const int iImageNumber = 0,const int iWillDo = 0);
#endif