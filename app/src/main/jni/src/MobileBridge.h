#ifndef MOBILE_BRIDGE_H
#define MOBILE_BRIDGE_H

// 這些函式給 CimGui.cpp 呼叫
void Mobile_OnJoystickMove(float x, float y); // 處理移動 (-1.0 ~ 1.0)
void Mobile_OnJoystickStop();                 // 停止移動
void Mobile_DoLeftButton();                       // 執行普通攻擊
void Mobile_DoRightButton(int skillSlot);          // 使用技能 (F1~F8)
void Mobile_UsePotion(int quickSlot);         // 使用藥水 (1~6)
void Mobile_SetQuickMemoryByKeyInput(bool CtrlFlag, SDL_Scancode key);
void Mobile_DoQuickmemory(SDL_Scancode key);
void Mobile_SendPickItem();


extern void		DoRButtonDown(void);
extern void		DoLButtonDown(void);
extern int UseItemByRbutton(POS pos_s, ItemAttr &item_attr);
extern int GetIndexOfCondition( BYTE );
extern bool g_IsJoystickMoving;
extern void MouseClickPosition(int* mx, int* my);
extern void DoHeroMove(int movetype, int x, int y);
extern bool IsHarmfulMagic(int nMagic);
extern	void	DoRButtonCheckOfMenu( int i, int j );
extern void lsw_MenuSubProcessType(SMENU* SubMenu, const int j);//020214 lsw
extern void SaveMemorize();
#endif