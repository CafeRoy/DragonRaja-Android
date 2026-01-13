// EditWndMgr.cpp: implementation of the CEditWndMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Chat.h"
#include "Hangul.h"
#include "Menu.h"
#include "dragon.h"
#include "Hong_Sub.h"
#include "map.h"
#include "CharDataTable.h"
#include "teambattle.h"
#include "gameproc.h"
#include "LineCommand.h"
#include "Tool.h"
#include "Hong_Sprite.h"
#include "directsound.h"
#include "SmallMenu.h"
#include "MenuSet.h"
#include "SmallMenuSet.h"
#include "EditWndMgr.h"
#include "SDLRenderer.h"
#include "SymbolMgr_Client.h"
bool g_ControlKeyOn;

extern void DoButtonCheckOfMenu( int i, int j );
extern void DoQuickmemoryByKeyInput(SDL_Scancode iInputKey,bool bIsDikCheck);
extern void SDLChatWndProc(uint32_t vk);
extern void SendSelfieImage(LPCHARACTER ch, ImageTexture& texture);
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CEditWndMgr EWndMgr;
CEditWndMgr::CEditWndMgr()
{
}

CEditWndMgr::~CEditWndMgr()
{
}

int CEditWndMgr::GetTxt(const int iIndex, char* txt)
{
	InputBox* box = FindInputBox(iIndex);
	if (box)
	{
		sprintf(txt, "%s", box->text.c_str());
	}

	return 0;
}

int CEditWndMgr::GetTxt(const int iIndex, char* txt, const int iSize)
{
	/*InputBox* box = FindInputBox(iIndex);
	if (box && box->isVisible)
	{
		sprintf(txt, box->text.c_str());
	}*/
	sprintf(txt, "%s", chat_Target[iIndex].c_str());
	return 0;
}
std::string CEditWndMgr::GetTxt(const int iIndex)
{
	InputBox* box = FindInputBox(iIndex);
	if (box)
	{
		return box->text;
	}
	return std::string("");
}


void SDLChatWndProc(uint32_t vk)
{
	switch (vk)
	{
	case SDLK_TAB:
	{
		const int iTarget = GetChatTarget();
		switch (iTarget)
		{
		case SELECT_WISPER_MODE:
		case CHAT_TARGET_WISPER1:
		case CHAT_TARGET_WISPER2:
		case CHAT_TARGET_WISPER3:
		case CHAT_TARGET_WISPER4:
		case CHAT_TARGET_WISPER5:
		{
				EWndMgr.ClearTxt(HWND_CHAT_INPUT);
			SetChatTarget(iTarget + 1);
		}break;
		case CHAT_TARGET_PARTY:
		{
			EWndMgr.ClearTxt(HWND_CHAT_INPUT);
			SetChatTarget(SELECT_WISPER_MODE);
		}break;
		case CHAT_TARGET_GUILD:
		{
			EWndMgr.ClearTxt(HWND_CHAT_INPUT);
			SetChatTarget(CHAT_TARGET_PARTY);
		}break;
		case CHAT_TARGET_NORMAL:
		{
			EWndMgr.ClearTxt(HWND_CHAT_INPUT);
			SetChatTarget(CHAT_TARGET_GUILD);
		}break;
		}
	}break;
	case SDLK_RETURN:
	{
		bool IsChatModeChange = true;
		switch (GetChatTarget())
		{
		case SELECT_WISPER_MODE:
		case CHAT_TARGET_WISPER1:
		case CHAT_TARGET_WISPER2:
		case CHAT_TARGET_WISPER3:
		case CHAT_TARGET_WISPER4:
		case CHAT_TARGET_WISPER5:
		{
			char temp[MAX_CHAT_STRING_] = { 0, };
			char name[NM_LENGTH] = { 0, };
			std::string t = EWndMgr.GetTxt(HWND_CHAT_INPUT);
			std::string n = EWndMgr.GetTxt(HWND_CHAT_TARGET_INPUT);

			sprintf(temp, "%s", t.c_str());
			sprintf(name, "%s", n.c_str());

			if (!name[0])//�̸��� ���ٸ�
			{
				//SetFocus2(HWND_CHAT_TARGET_INPUT);//021001 lsw
			}
			else
			{
				IsChatModeChange = SendChatWisper(name, temp);
				const int iNextFocus = InsertWisperList(name);
				if (!IsChatBoxLock())
				{
					SetChatTarget(CHAT_TARGET_NORMAL);//020530 lsw
				}
				else
				{
					SetChatTarget(iNextFocus);
				}
				EWndMgr.ClearTxt(HWND_CHAT_INPUT);
			}
		}break;
		case CHAT_TARGET_PARTY:
		{
			char temp[MAX_CHAT_STRING_] = { "" };
			std::string t = EWndMgr.GetTxt(HWND_CHAT_INPUT);
			sprintf(temp, "%s", t.c_str());
			IsChatModeChange = SendPartyChatByLineCommand(temp);
			EWndMgr.ClearTxt(HWND_CHAT_INPUT);
		}break;
		case CHAT_TARGET_GUILD:
		{
			char temp[MAX_CHAT_STRING_] = { "" };
			std::string t = EWndMgr.GetTxt(HWND_CHAT_INPUT);
			sprintf(temp, "%s", t.c_str());
			IsChatModeChange = SendChatGuild(temp);
			EWndMgr.ClearTxt(HWND_CHAT_INPUT);
		}break;
		case CHAT_TARGET_NORMAL:
		default:
		{
			char temp[MAX_CHAT_STRING_] = { "" };
			std::string t = EWndMgr.GetTxt(HWND_CHAT_INPUT);
			sprintf(temp, "%s", t.c_str());
			IsChatModeChange = SendChatNormal(temp);
			EWndMgr.ClearTxt(HWND_CHAT_INPUT);
		}break;
		}

		if (IsChatModeChange && (!IsChatBoxLock()))//ä�ø�� �ݱ� �ΰ�?
		{
			SetChatMode(CM_COMMAND, true);
		}
		else
		{
			SetChatMode(CM_MESSAGE);
		}
	}break;//case	VK_RETURN: 
	case	SDLK_ESCAPE:
	{
		SetChatTarget(CHATTYPE_NORMAL);//020705 lsw
		SetChatMode(CM_COMMAND, true);
	}break;
	}
}

extern void RunAllParticleTests();  // for test 

void CEditWndMgr::SDLCommonEditWndProc(SDL_KeyboardEvent& keyEvent) {
	if (keyEvent.repeat != 0) return;

	uint32_t VK = keyEvent.keysym.sym;

	//for debug
	if (VK == SDLK_v)
	{
        //g_SDLInfo.bOutputSmallMap = true;
        //ImageTexture img(g_SDLInfo.renderer);
        //img.loadFromFile("face.png");
        //RunAllParticleTests();
        //SendSelfieImage(Hero, img);
		//g_SDLInfo.limit_time = SDL_GetTicks() + 5000;
		//RainStart(900, 1);
		//SendReqSkillInfo(Hero);
		//QuickItemAttr[5].item_no++;
		//g_SymBolMgr.SetDualItem(QuickItemAttr[5].item_no, 5);
		//if (QuickItemAttr[5].item_no > 23)QuickItemAttr[5].item_no = 1;
	}
	//debug finished

	if (isKeyPressed(SDL_SCANCODE_LALT) && VK == SDLK_F4) { CallMenu(MN_SYSTEM_LOGOUT, 193+240, 75+60, true); return; }

	if ((keyEvent.keysym.scancode >= SDL_SCANCODE_F1 && keyEvent.keysym.scancode <= SDL_SCANCODE_F8) ||
		(keyEvent.keysym.scancode >= SDL_SCANCODE_6 && keyEvent.keysym.scancode <= SDL_SCANCODE_0)) {
		DoQuickmemoryByKeyInput(keyEvent.keysym.scancode, false);
		return;
	}
	switch (VK)
	{
	/*case SDLK_F1:DoQuickmemoryByKeyInput(SDL_SCANCODE_F1, false); return; break;
	case SDLK_F2:DoQuickmemoryByKeyInput(SDL_SCANCODE_F2, false); return; break;
	case SDLK_F3:DoQuickmemoryByKeyInput(SDL_SCANCODE_F3, false); return; break;
	case SDLK_F4:DoQuickmemoryByKeyInput(SDL_SCANCODE_F4, false); return; break;
	case SDLK_F5:DoQuickmemoryByKeyInput(SDL_SCANCODE_F5, false); return; break;
	case SDLK_F6:DoQuickmemoryByKeyInput(SDL_SCANCODE_F6, false); return; break;
	case SDLK_F7:DoQuickmemoryByKeyInput(SDL_SCANCODE_F7, false); return; break;
	case SDLK_F8:DoQuickmemoryByKeyInput(SDL_SCANCODE_F8, false); return; break;*/
	case SDLK_h:
	{
		if (!IsChatBoxActive())
		{
			ViewTipToggle = !ViewTipToggle;
            return;
		}
	}break;
	case SDLK_F12:
	{
          CaptureScreenShot();
          return;
	}break;
	case SDLK_DELETE:
	{
       g_GameInfo.mouseClick = !g_GameInfo.mouseClick;
	}break;
	}

	switch (GetFocusWndIndex())
	{
	case HWND_CHAT_TARGET_INPUT:
	{
		switch (VK)
		{
		case SDLK_RETURN:
		case SDLK_TAB:
		{
			int iTarget = GetChatTarget();
			switch (iTarget)
			{
			case SELECT_WISPER_MODE://�Ӹ� ��� �Է� �϶� 
			{
				char name[20] = { 0 };
				ClearTxt(HWND_CHAT_INPUT);
				//name = GetTxt(HWND_CHAT_TARGET_INPUT);
				GetTxt(HWND_CHAT_TARGET_INPUT, name, 0);
				if (name[0]<=0)//if No Name
				{
					SetChatTarget(CHAT_TARGET_WISPER1);//goto Next 
				}
				else
				{	
					SDL_Log("HWND_CHAT_TARGET_INPUT %s", name);
					EWndMgr.showInputBox(HWND_CHAT_INPUT);
				}
			}break;
			default:
			{
				SetChatMode(CM_MESSAGE);
			}break;
			}
		}
		}
	}break;
	case HWND_CHAT_INPUT:
	{
		SDLChatWndProc(VK);
	}break;
	case HWND_MAIL1:
	{
		switch (VK)
		{
		case	SDLK_TAB:
		case	SDLK_RETURN:
		{
			EWndMgr.setActiveInputBox(HWND_MAIL2);
		}break;
		}
	}break;
	case HWND_MAIL2:
	{
		switch(VK)
		{
		case	SDLK_TAB:
		case	SDLK_RETURN:
		{
			EWndMgr.setActiveInputBox(HWND_MAIL3);
		}break;
		}
	}break;
	case HWND_MAIL3:
	{
		switch (VK)
		{
		case	SDLK_UP:
		{
			
		}break;
		case	SDLK_DOWN:
		{
			/*DWORD selStart, selEnd;
			int selection;
			char text[2048] = { 0, };

			SendMessage(hWnd, EM_GETSEL, (WPARAM)&selStart, (LPARAM)&selEnd);
			// 1000 is a hard coded value for max number of characters in the mail window (see kh_menuset aroundline 6000)
			GetWindowText(hWnd, text, 1000);
			selection = selStart;
			MakeCursorUpDown(text, selection, false);

			SendMessage(hWnd, EM_SETSEL, (WPARAM)selection, (LPARAM)selection);*/
		}break;
		case	SDLK_TAB:
		{
			EWndMgr.setActiveInputBox(HWND_MAIL1);
		}break;
		}
	}break;
	default:
	{
		/*if (VK != SDLK_TAB && VK != SDLK_RETURN) break;//not tab or return , just break

		if (SMenu[MN_GUILD_REGIST_DEGREE].bActive)
		{
			SMenu[MN_GUILD_REGIST_DEGREE].work++;
			SMenu[MN_GUILD_REGIST_DEGREE].work %= 5;
			//SetWindowText(hWnd, "");
		}
		if (SMenu[MN_LOGIN].bActive)//���� �������̽� �Է� ���̶��
		{
			switch (EWndMgr.GetFocusWndIndex())
			{
			case HWND_1:
			{
				id_password = false;
			}break;
			case HWND_2:
			{
				switch (VK)
				{
				case	SDLK_RETURN:
				{
					SMenu[MN_LOGIN].nField[0].fCheakFlag = TRUE;
				}break;
				case	SDLK_TAB:
				{
					id_password = true;
				}break;
				}
			}break;
			}
		}*/
	}break;
	}
}

int CEditWndMgr::IsFocus(const int iIndex)
{
	if (activeInputBox) return (activeInputBox->myID == iIndex);
	return 0;
}

int CEditWndMgr::GetFocusWndIndex()
{
	if(activeInputBox)
		return activeInputBox->myID;

	return 0;
}

bool CEditWndMgr::IsMultiLineBoxShow() {
	if (activeInputBox)
		return activeInputBox->isMultiLine;
	return false;
}


void CEditWndMgr::hideInputBox(int BoxID)
{

	if (activeInputBox && BoxID == activeInputBox->myID)
	{
		activeInputBox->isActive = false;
		activeInputBox->isVisible = false;
		activeInputBox = nullptr;
		SDL_StopTextInput();
	}
	else
	{
		InputBox* box = FindInputBox(BoxID);
		if (box)
		{
			box->isActive = false;
			box->isVisible = false;
		}
	}
}

bool CEditWndMgr::setActiveInputBox(int BoxID)
{
	InputBox* box = FindInputBox(BoxID);
	if (box)
	{
		box->setActive(true);
	}

	return (box) ? true:false;
}

bool CEditWndMgr::checkInputBoxVisibility(int BoxID)
{
	InputBox* box = FindInputBox(BoxID);
	if (box)
	{
		return box->isVisible;
	}
	return false;
}


void CEditWndMgr::showInputBox(int BoxID)
{
	if (activeInputBox && BoxID == activeInputBox->myID)
		return;

	InputBox* box = FindInputBox(BoxID);

	//if (!box) return;
	if (box) {
		
		if (activeInputBox) {
			activeInputBox->isActive = false;
			activeInputBox->compositionText.clear();
			activeInputBox->compositionCursor = 0;
			activeInputBox->compositionSelectionLength = 0;
			
		}

		if (box->myID != HWND_CHAT_INPUT)
		{
			box->text = "";
			box->cursorIndex = 0;
		}
		box->isActive = true;
		box->isVisible = true;
		activeInputBox = box;
		//SDL_StartTextInput();
		//updateIMERect(box);
	}
}


int CEditWndMgr::setupInputBoxCoordinate(int type)
{
	CloseAllInputBox();
	int PADDING = 2;
	int fontHeight = TTF_FontHeight(g_SDLInfo.gFont);

	if (type == 0)
	{
		InputBox* id = FindInputBox(HWND_1);
		if (id)
		{
			id->setInfo(SMenu[MN_LOGIN].x + 532, SMenu[MN_LOGIN].y + 485,
                              274, fontHeight, 15, true, true, false);
			id->acceptNonAscii = false;
#ifdef _DEBUG
			id->text = "LUCKY1988";
#endif
			size_t currentChars = utf8_num_chars(id->text);
			id->cursorIndex = currentChars;
		}

		activeInputBox = id;

		InputBox* pw = FindInputBox(HWND_2);
		if (pw)
		{
			pw->setInfo(SMenu[MN_LOGIN].x + 532, SMenu[MN_LOGIN].y + 540,
                              274, fontHeight, 15, true, false, false);
			pw->isPassword = true;
			pw->acceptNonAscii = false;
#ifdef _DEBUG
			pw->text = "yondaime1";
#endif
			size_t currentChars = utf8_num_chars(pw->text);
			pw->cursorIndex = currentChars;
		}

		InputBox* char_name = FindInputBox(HWND_CREATE_CHAR_NAME);
		if (char_name)
		{
			char_name->setInfo(SMenu[MN_MAKECHARACTER_NAME].x + SMenu[MN_MAKECHARACTER_NAME].nField[7].x-1,
				SMenu[MN_MAKECHARACTER_NAME].y + SMenu[MN_MAKECHARACTER_NAME].nField[7].y-4,
				122, fontHeight, 15, false, false, false);

			//SDL_Log("char_name %d %d", char_name->x, char_name->y);
		}

		InputBox* del_char = FindInputBox(HWND_DELETE_CHAR_KEY);
		if (del_char)
		{
			del_char->setInfo((SCREEN_WIDTH - 163) / 2 + 2,
				SMenu[MN_DEL_CHAR_COMFORM].y + SMenu[MN_DEL_CHAR_COMFORM].nField[3].y,
				160, fontHeight, 15, false, false, false);
			del_char->isPassword = true;
			//SDL_Log("del_char %d %d Main(%d,%d)", del_char->x, del_char->y, SMenu[MN_DEL_CHAR_COMFORM].x, SMenu[MN_DEL_CHAR_COMFORM].y);
		}

		InputBox* hWnd3 = FindInputBox(HWND_3);
		if (hWnd3) {
			hWnd3->setNumberic();
			hWnd3->x = -100;
			hWnd3->acceptNonAscii = false;
		}

	}
	else if (type == 1)
	{
		InputBox* chat = FindInputBox(HWND_CHAT_INPUT);
		if (chat)
		{
			chat->setInfo(SMenu[MN_MAININTERFACE].x + SMenu[MN_MAININTERFACE].nField[31].x + 185,
				SMenu[MN_MAININTERFACE].y + SMenu[MN_MAININTERFACE].nField[31].y + 2,
				314, fontHeight, 46, false, false, false);
		}

		InputBox* chat_target = FindInputBox(HWND_CHAT_TARGET_INPUT);
		if (chat_target)
		{
			chat_target->setInfo(SMenu[MN_MAININTERFACE].x + SMenu[MN_MAININTERFACE].nField[42].x + 15,
				SMenu[MN_MAININTERFACE].y + SMenu[MN_MAININTERFACE].nField[42].y,
				123, fontHeight, 15, false, false, false);
		}


		InputBox* mail_name = FindInputBox(HWND_MAIL1);
		if (mail_name)
		{
			mail_name->setInfo(SMenu[MN_MAIL_WRITE].x + SMenu[MN_MAIL_WRITE].nField[3].x - 2,
				SMenu[MN_MAIL_WRITE].y + SMenu[MN_MAIL_WRITE].nField[3].y - 3,
				195, fontHeight, 15, false, false, false);

		}

		InputBox* mail_title = FindInputBox(HWND_MAIL2);
		if (mail_title)
		{
			mail_title->setInfo(SMenu[MN_MAIL_WRITE].x + SMenu[MN_MAIL_WRITE].nField[4].x - 2,
				SMenu[MN_MAIL_WRITE].y + SMenu[MN_MAIL_WRITE].nField[4].y - 3,
				275, fontHeight, 15, false, false, false);
		}

		InputBox* mail_body = FindInputBox(HWND_MAIL3);
		if (mail_body)
		{
			mail_body->setInfo(SMenu[MN_MAIL_WRITE].x + SMenu[MN_MAIL_WRITE].nField[5].x - 2,
				SMenu[MN_MAIL_WRITE].y + SMenu[MN_MAIL_WRITE].nField[5].y - 3,
				320, 14 * fontHeight, 200, false, false, true);
		}

		InputBox* bbs = FindInputBox(HWND_BBS);
		if (bbs)
		{
			bbs->setInfo((SCREEN_WIDTH - SMenu[MN_TOWNHALL_BBS3].nField[0].nRectImage) / 2 + 40,
				SMenu[MN_TOWNHALL_BBS3].y + SMenu[MN_TOWNHALL_BBS3].nField[0].y + 80,
				196, (130 / fontHeight) * fontHeight, 60, false, false, true);

		}
		
		for (int i = 2; i < 7;i++) {
			//MN_GUILD_REGIST_DEGREE
			int x = 620;
			int y = -2 + SMenu[MN_GUILD_REGIST_DEGREE].y + SMenu[MN_GUILD_REGIST_DEGREE].nField[i].y;
			InputBox* guild_lev = FindInputBox(i+10);
			if (guild_lev)
			{
				guild_lev->setInfo(x, y, 80, fontHeight, 15, false, false, false);

				SDL_Log("guild_lev %d %d", guild_lev->x, guild_lev->y);
			}
		}

		InputBox* item_sell = FindInputBox(HWND_REGISTER_ITEM_SELL);
		if (item_sell)
		{
			int x = -50;// SMenu[MN_MERCHANT_REGISTER].x + SMenu[MN_MERCHANT_REGISTER].nField[6].y;
			int y = -50;// SMenu[MN_MERCHANT_REGISTER].y + SMenu[MN_MERCHANT_REGISTER].nField[6].y;
			item_sell->setInfo(x, y, 70, fontHeight, 6, false, false, false);
			item_sell->setNumberic();
			SDL_Log("item_sell %d %d", item_sell->x, item_sell->y);
		}

		/*InputBox* guild_lev2 = FindInputBox(HWND_GUILD_LEV2);
		if (guild_lev2)
		{
			guild_lev2->setInfo(383, 182, 137, fontHeight, 15, false, false, false);

			SDL_Log("guild_lev2 %d %d", guild_lev2->x, guild_lev2->y);
		}

		InputBox* guild_lev3 = FindInputBox(HWND_GUILD_LEV3);
		if (guild_lev3)
		{
			guild_lev3->setInfo(383, 214, 137, fontHeight, 15, false, false, false);

			SDL_Log("guild_lev3 %d %d", guild_lev3->x, guild_lev3->y);
		}

		InputBox* guild_lev4 = FindInputBox(HWND_GUILD_LEV4);
		if (guild_lev4)
		{
			guild_lev4->setInfo(383, 246, 137, fontHeight, 15, false, false, false);

			SDL_Log("guild_lev4 %d %d", guild_lev4->x, guild_lev4->y);
		}

		InputBox* guild_lev5 = FindInputBox(HWND_GUILD_LEV5);
		if (guild_lev5)
		{
			guild_lev5->setInfo(383, 278, 137, fontHeight, 15, false, false, false);

			SDL_Log("guild_lev5 %d %d", guild_lev5->x, guild_lev5->y);
		}*/

		InputBox* guild_name = FindInputBox(HWND_GUILD_NAME);
		if (guild_name)
		{
			guild_name->x = 468 + SMenu[MN_GUILD_REGIST_IMSI].nField[2].x;

			guild_name->y = SMenu[MN_GUILD_REGIST_IMSI].y + SMenu[MN_GUILD_REGIST_IMSI].nField[2].y-3;
			guild_name->h = fontHeight;
			guild_name->w = 196;
			guild_name->setMaxTextLength(10);
			SDL_Log("guild_name %d %d", guild_name->x, guild_name->y);
		}

		InputBox* guild_vice = FindInputBox(HWND_GUILD_VICEMASTER);
		if (guild_vice)
		{
			guild_vice->x = 640-12;

			guild_vice->y = SMenu[MN_GUILD_EXPLAIN].y + SMenu[MN_GUILD_EXPLAIN].nField[10].y;
			guild_vice->h = fontHeight;
			guild_vice->w = 196;
			guild_vice->setMaxTextLength(15);
			SDL_Log("guild_vice %d %d", guild_vice->x, guild_vice->y);
		}

		//SMenu[MN_GUILD_EXPLAIN].nField[10].x=40;
		//SMenu[MN_GUILD_EXPLAIN].nField[10].y = 184;
		InputBox* guild_degree = FindInputBox(HWND_GUILD_DEGREE);
		if (guild_degree)
		{
			guild_degree->x = SMenu[MN_GUILD_REGIST_IMSI].x + SMenu[MN_GUILD_REGIST_IMSI].nField[0].x;

			guild_degree->y = SMenu[MN_GUILD_REGIST_IMSI].y + SMenu[MN_GUILD_REGIST_IMSI].nField[0].y + 80;
			guild_degree->h = fontHeight;
			guild_degree->w = 196;
			guild_degree->setMaxTextLength(15);
			SDL_Log("guild_degree %d %d", guild_degree->x, guild_degree->y);
		}

		InputBox* guild_intro = FindInputBox(HWND_GUILD_INTRO);
		if (guild_intro)
		{
			guild_intro->x = 468 + SMenu[MN_GUILD_REGIST_IMSI].nField[5].x;

			guild_intro->y = SMenu[MN_GUILD_REGIST_IMSI].y + SMenu[MN_GUILD_REGIST_IMSI].nField[5].y-3;
			guild_intro->h = fontHeight;
			guild_intro->w = 196;
			guild_intro->setMaxTextLength(80);
			guild_intro->isMultiLine = true;
			guild_intro->h = guild_intro->h * (130 / guild_intro->h);
			SDL_Log("guild_intro %d %d", guild_intro->x, guild_intro->y);
		}
	}

	

	return 1;
}

void CEditWndMgr::ClearAllTxt()//�Էµ� ������ ��� ����ϴ�.//""�� �ִ°̴ϴ�.
{
	CloseAllInputBox();
	/*for (int i = HWND_NOW_FOCUS; HWND_GAME_MAIN >i; i++)
	{
		//ClearTxt(i);
	}*/
}

void CEditWndMgr::ClearTxt(const int iIndex)//������ ����Ʈ �ڽ��� ������ ����ϴ�.
{	
	for(auto &box:inputBoxes)
	{
		if(iIndex == box.myID /* && box.isVisible*/)
		{
			box.text = "";
			box.cursorIndex = 0;
		}
	}
}

void CEditWndMgr::CloseAllInputBox()
{
	for (auto& box : inputBoxes)
	{
		box.isVisible = false;
		box.isActive = false;
		//box.text = "";
		//box.cursorIndex = 0;
		box.compositionText.clear();
		box.compositionCursor = 0;
		box.compositionSelectionLength = 0;
	}
	activeInputBox = nullptr;
	//SDL_StopTextInput();
}

void CEditWndMgr::setInputBoxText(const int iIndex, std::string text) {
  for (auto& box : inputBoxes)
  {
          if (iIndex == box.myID && box.isVisible)
          {
                  box.text = text;
				  box.cursorIndex = strlen(box.text.c_str());
                  //SDL_Log("settxt %s", txt);
          }
  }
}

int CEditWndMgr::SetTxt(const int iIndex, const char* txt)
{
	for (auto& box : inputBoxes)
	{
		if (iIndex == box.myID && box.isVisible)
		{
			box.text = txt; break;
			//SDL_Log("settxt %s", txt);
		}
	}

	chat_Target[iIndex] = txt;
	return 1;
}

