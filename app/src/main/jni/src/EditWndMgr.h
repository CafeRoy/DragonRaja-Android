// EditWndMgr.h: interface for the CEditWndMgr class.
// Make By Hwoarang Sangwoo 2002 10 01
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EDITWNDMGR_H__CCD50119_39F2_4932_8DEF_B46CD7E4330A__INCLUDED_)
#define AFX_EDITWNDMGR_H__CCD50119_39F2_4932_8DEF_B46CD7E4330A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <SDL.h>
#include <string>
enum//�Ʒ��� enum �� ��ȭ ���� �ʵ��� �ؾ� �մϴ�. ���� �ϼ���. 0���� �ݵ�� HWND_NOW_FOCUS ���� �մϴ�.
{
	HWND_NOW_FOCUS = 0,
	HWND_CHAT_INPUT = 1,
	HWND_CHAT_TARGET_INPUT = 2,
	HWND_1 =3,//hwnd1
	HWND_2 =4,//hwnd2
	HWND_3 =5,//hwndEditBox ;  // �̸��� ����
	HWND_BBS =6, //MN_TOWNHALL_BBS3
	HWND_MAIL1 =7,
	HWND_MAIL2 =8,
	HWND_MAIL3 =9,
	HWND_GUILD_INTRO=10,
	HWND_GUILD_NAME=11,
	HWND_GUILD_LEV1 = 12,
	HWND_GUILD_LEV2,
	HWND_GUILD_LEV3,
	HWND_GUILD_LEV4,
	HWND_GUILD_LEV5=16,
	HWND_GUILD_MODIFY=17,
	HWND_GUILD_DEGREE=18,
	HWND_GUILD_VICEMASTER=19,
	HWND_CREATE_CHAR_NAME=20,
	HWND_DELETE_CHAR_KEY=21,
	HWND_REGISTER_ITEM_SELL=22,
	HWND_GAME_MAIN,//���� ������//�׻� �������� ��ġ
};

class CEditWndMgr  
{
public:
	CEditWndMgr();
	virtual ~CEditWndMgr();
private:
	CEditWndMgr(const CEditWndMgr &old);//�⺻���� �����Ǵ� ���� �����ڸ� ��� �� �� �����ϴ�.

private:	

public:
	int setupInputBoxCoordinate(int type);
	void ClearAllTxt();//�Էµ� ������ ��� ����ϴ�.//""�� �ִ°̴ϴ�.
	void ClearTxt(const int iIndex);
    void CloseAllInputBox();
    void setInputBoxText(const int iIndex, std::string text);
	int	SetTxt(const int iIndex, const char* txt);
	int GetTxt(const int iIndex, char* txt);
	int	GetTxt(const int iIndex, char* txt, const int iSize);
	std::string GetTxt(const int iIndex);
	std::string chat_Target[HWND_GAME_MAIN+1];
	void SDLCommonEditWndProc(SDL_KeyboardEvent& keyEvent);
	int IsFocus(const int iIndex);
	int GetFocusWndIndex();
	bool IsMultiLineBoxShow();
	void hideInputBox(int BoxID);
	bool setActiveInputBox(int BoxID);
	bool checkInputBoxVisibility(int BoxID);
	void showInputBox(int BoxID);
	
private:
};
extern CEditWndMgr EWndMgr;
#endif // !defined(AFX_EDITWNDMGR_H__CCD50119_39F2_4932_8DEF_B46CD7E4330A__INCLUDED_)
