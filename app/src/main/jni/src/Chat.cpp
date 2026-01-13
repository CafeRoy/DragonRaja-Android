// Chat.cpp: implementation of the CChat class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Chat.h"
#include "Hangul.h"
#include "Hong_Sub.h"
#include "Menu.h"
#include "dragon.h"
#include <SDL.h> // For Clipboard
#include <SDL_ttf.h> // For TTF_SizeUTF8
#include "Hong_Sub.h"
#include "Map.h"
#include "CharDataTable.h"
#include "TeamBattle.h"
#include "GameProc.h"
#include "LineCommand.h"
#include "Tool.h"
#include "Hong_Sprite.h"
#include "directsound.h"
#include "SmallMenu.h"
#include "TextOutMgr.h"
#include "CurrentMsgMgr.h"
#include "QuestFunction.h" // 021128 kyo
#include <sstream>
#include "CharacterContxtMenu.h"
#include <algorithm>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern int g_LocalWarBegin;
extern int GetNationIndex(int Nation);
CChatMgr ChatMgr;

// --- Context Menu Action Implementations ---

void MenuAction_CopyName(CChatData* pData)
{
    if (pData && pData->GetName()[0] != '\0') {
        SDL_SetClipboardText(pData->GetName());
        AddCurrentStatusMessage(255, 255, 0, "Name Copy : %s", pData->GetName());
    }
}

void MenuAction_AddFriend(CChatData* pData)
{
    if (pData && pData->GetName()[0] != '\0') {
        // TODO: Implement Add Friend logic, e.g., by calling a line command
		if (strcmp(Hero->name, pData->GetName()) == 0) return;

		RegistFriend(pData->GetName());
        AddCurrentStatusMessage(255, 255, 0, "Added %s ", pData->GetName());
    }
}

void MenuAction_BlockUser(CChatData* pData)
{
    if (pData && pData->GetName()[0] != '\0') {
        // TODO: Implement Block User logic
		ChatMgr.SetRefuseWhiper(pData->GetName());
        //AddCurrentStatusMessage(255, 0, 0, "TODO: Blocked %s", pData->GetName());
    }
}

void MenuAction_CopyContent(CChatData* pData)
{
    if (pData && pData->GetData()) {
        // Since GetData() returns a UTF-8 string, we need to convert it back to Big5 for clipboard on some systems
        // But for simplicity, we'll copy as UTF-8, which is fine for modern OS.
        SDL_SetClipboardText(pData->GetData());
        AddCurrentStatusMessage(255, 255, 0, "Copied Content");
    }
}

// --- CChatMgr Context Menu Method Implementations ---

void CChatMgr::InitContextMenu(int lang)
{
	if (lang == 0) {
		m_contextMenuItems.push_back({ u8"Copy Name", &MenuAction_CopyName });
		m_contextMenuItems.push_back({ u8"Add Friend", &MenuAction_AddFriend });
		m_contextMenuItems.push_back({ u8"Block User", &MenuAction_BlockUser });
		m_contextMenuItems.push_back({ u8"Copy Content", &MenuAction_CopyContent });
	}
	else {
		m_contextMenuItems.push_back({ "複製名字", &MenuAction_CopyName });
		m_contextMenuItems.push_back({ "加為好友", &MenuAction_AddFriend });
		m_contextMenuItems.push_back({ "屏蔽此人", &MenuAction_BlockUser });
		m_contextMenuItems.push_back({ "複製內容", &MenuAction_CopyContent });
	}
}

void CChatMgr::OpenContextMenu(CChatData* pData, int x, int y)
{
    if (!pData) return;
    m_pSelectedChatData = pData;
    m_bIsContextMenuOpen = true;
    m_iContextMenuX = x;
    m_iContextMenuY = y;
    m_iMenuHoverIndex = -1; // Nothing is hovered initially
}

void CChatMgr::CloseContextMenu()
{
    m_bIsContextMenuOpen = false;
    m_pSelectedChatData = nullptr;
}

WORD CHAT_COLOR_NORMAL				= FONT_COLOR_WHITE;
WORD CHAT_COLOR_GUILD				= FONT_COLOR_RARE_MAIN;
WORD CHAT_COLOR_COLOSSUS			= FONT_COLOR_BLIGHT_VIOLET;
WORD CHAT_COLOR_WHISPER				= FONT_COLOR_GREEN;	
WORD CHAT_COLOR_WAR				= FONT_COLOR_BLIGHT_ORANGE;
WORD CHAT_COLOR_NATION				= FONT_COLOR_BLIGHT_ORANGE;
WORD CHAT_COLOR_PARTY				= FONT_COLOR_BORA;
WORD CHAT_COLOR_NOTICE				= FONT_COLOR_SOLID_YELLOW;

void CChatMgr::AddString(const char *szName, const char *szChatData,
                         const int iChatType) {
  CChatData *InputData = new CChatData;
  InputData->SetName(FONT_COLOR_NAME, szName);

  switch (iChatType) {
    case CHATTYPE_MERCHANT_BBS:
    case CHATTYPE_NORMAL:
      InputData->SetChatTime();
      InputData->SetChatType(iChatType, ++m_iTotalCounter, ++m_iNormalCounter);
      InputData->SetData(CHAT_COLOR_NORMAL, szChatData);
      InputData->SetHeader(CHAT_COLOR_NORMAL, lan->OutputMessage(0, 600));
      break;

    case CHATTYPE_GUILD:
      InputData->SetChatTime();
      InputData->SetChatType(iChatType, ++m_iTotalCounter, ++m_iGuildCounter);
      InputData->SetData(CHAT_COLOR_GUILD, szChatData);
      InputData->SetHeader(CHAT_COLOR_GUILD, lan->OutputMessage(0, 601));
      break;

    case CHATTYPE_COLOSSUS_GLOBAL:
      InputData->SetChatType(iChatType, ++m_iTotalCounter,
                             ++m_iColosussCounter);
      InputData->SetData(CHAT_COLOR_COLOSSUS, szChatData);
      InputData->SetHeader(CHAT_COLOR_COLOSSUS, lan->OutputMessage(0, 602));
      break;

    case CHATTYPE_COLOSSUS:
      InputData->SetChatType(iChatType, ++m_iTotalCounter,
                             ++m_iColosussCounter);
      InputData->SetData(CHAT_COLOR_COLOSSUS, szChatData);
      InputData->SetHeader(CHAT_COLOR_COLOSSUS, lan->OutputMessage(0, 603));
      break;

    case CHATTYPE_WHISPER:
      InputData->SetChatTime();
      InputData->SetChatType(iChatType, ++m_iTotalCounter, ++m_iWhisperCounter);
      InputData->SetData(CHAT_COLOR_WHISPER, szChatData);
      InputData->SetHeader(CHAT_COLOR_WHISPER, lan->OutputMessage(0, 604));
      break;
	case CHATTYPE_ALL:
    case CHATTYPE_LOCAL_WAR:
      InputData->SetChatType(iChatType, ++m_iTotalCounter,
                             ++m_iLocalWarCounter);
      InputData->SetData(CHAT_COLOR_WAR, szChatData);
      InputData->SetHeader(CHAT_COLOR_WAR, lan->OutputMessage(0, 605));
      break;

    case CHATTYPE_NATION_CHAT:
      InputData->SetChatType(iChatType, ++m_iTotalCounter,
                             ++m_iNationChatCounter);
      InputData->SetData(CHAT_COLOR_NATION, szChatData);
      InputData->SetHeader(CHAT_COLOR_NATION, lan->OutputMessage(0, 659));
      break;

    case CHATTYPE_NATION_WAR:
      InputData->SetChatType(iChatType, ++m_iTotalCounter,
                             ++m_iNationWarCounter);
      InputData->SetData(CHAT_COLOR_WAR, szChatData);
      InputData->SetHeader(CHAT_COLOR_WAR, lan->OutputMessage(0, 606));
      break;

    case CHATTYPE_PARTY:
      InputData->SetChatTime();
      InputData->SetChatType(iChatType, ++m_iTotalCounter, ++m_iPartyCounter);
      InputData->SetData(CHAT_COLOR_PARTY, szChatData);
      InputData->SetHeader(CHAT_COLOR_PARTY, lan->OutputMessage(0, 607));
      break;

    case CHATTYPE_GUILD_BBS:
      InputData->SetChatTime();
      InputData->SetChatType(iChatType, ++m_iTotalCounter,
                             ++m_iGuildBBSCounter);
      InputData->SetData(CHAT_COLOR_GUILD, szChatData);
      InputData->SetHeader(CHAT_COLOR_GUILD, lan->OutputMessage(0, 608));
      break;

    case CHATTYPE_YES_OR_NO:
      InputData->SetChatType(iChatType, ++m_iTotalCounter, ++m_iYesNoCounter);
      InputData->SetData(FONT_COLOR_BLIGHT_BLUE, szChatData);
      InputData->SetHeader(FONT_COLOR_RED, lan->OutputMessage(0, 611));
      break;

    case CHATTYPE_LMS:
    case CHATTYPE_NOTICE:
      InputData->SetChatType(iChatType, ++m_iTotalCounter, ++m_iYesNoCounter);
      InputData->SetData(CHAT_COLOR_NOTICE, szChatData);
      InputData->SetHeader(CHAT_COLOR_NOTICE, lan->OutputMessage(0, 612));
      break;

    default:
      InputData->SetChatType(iChatType, ++m_iTotalCounter, m_iTotalCounter);
      InputData->SetData(FONT_COLOR_WHITE, szChatData);
      InputData->SetHeader(FONT_COLOR_WHITE, "");
      break;
  }

  InputData->MakeTotalData();
  ChatList.push_back(InputData);
}
void CChatMgr::ChatLog(chatitor itor) {
	std::string log;
	std::stringstream ss; // 創建一個字串流

	if ((*itor)->IsHaveHeader()) {
		// 構造帶 Header 的字串：[時間] [Header] 名字: 資料
		ss << "[" << (*itor)->GetTime() << "] ";
		ss << "[" << (*itor)->GetHeader() << "] ";
		ss << (*itor)->GetName() << ": ";
		ss << (*itor)->GetData();
	}
	else {
		// 構造不帶 Header 的字串：[時間] 名字: 資料
		ss << "[" << (*itor)->GetTime() << "] ";
		ss << (*itor)->GetName() << ": ";
		ss << (*itor)->GetData();
	}

	log = ss.str(); // 將字串流的內容賦值給 log 

	ofstream file("./chatlog.txt", ios::app);
	if (file.is_open()) {
		// 3. 寫入 log 內容
		file << log << "\n"; // 寫入 log 字串，並在末尾添加一個換行符
		// 4. 關閉文件 (可選：當 file 物件超出作用域時，它會自動關閉，但手動關閉是良好的習慣)
		file.close();
	}
	else {
		// 5. 文件打開失敗時的錯誤處理
		//std::cerr << "無法打開文件 chatlog.txt 進行寫入。\n";
	}
}

void CChatMgr::DisplayPastData(const int iType)
{
	for( chatitor itor = ChatList.begin() ; itor != ChatList.end();itor++)
	{	// Ǿ ְ  ȭ鿡 ȳ°
		ChatLog(itor);
		if( iType == (*itor)->GetChatType() && !(*itor)->IsDisplayAble() )
		{
			((*itor)->IsHaveHeader())?
			AddCurrentStatusMessage((*itor)->GetHeaderColor(),"[%s] [%s] %s: %s",(*itor)->GetTime(),(*itor)->GetHeader(),(*itor)->GetName(),(*itor)->GetData())
			:AddCurrentStatusMessage((*itor)->GetHeaderColor(),"[%s] %s: %s",(*itor)->GetTime(),(*itor)->GetName(),(*itor)->GetData());

		
		}
	}
}

void CChatMgr::DeleteCheck()//iIndex2     
{
	for( chatitor itor = ChatList.begin() ; itor != ChatList.end();)
	{
		int iCompareCount = 0;// ܾ  
		switch((*itor)->GetChatType())
		{
		case CHATTYPE_ALL:
		case CHATTYPE_NORMAL:			{iCompareCount = m_iNormalCounter	- MAX_NORMAL_COUNT;		}break;
		case CHATTYPE_GUILD:			{iCompareCount = m_iGuildCounter	- MAX_GUILD_COUNT;		}break;
		case CHATTYPE_COLOSSUS_GLOBAL:
		case CHATTYPE_COLOSSUS	:		{iCompareCount = m_iColosussCounter	- MAX_COLOSUSS_COUNT;	}break;
		case CHATTYPE_WHISPER:			{iCompareCount = m_iWhisperCounter	- MAX_WHISPER_COUNT;	}break;
		case CHATTYPE_LOCAL_WAR:		{iCompareCount = m_iLocalWarCounter	- MAX_LOCALWAR_COUNT;	}break;
		case CHATTYPE_NATION_CHAT:		{iCompareCount = m_iNationChatCounter - MAX_NATIONCHAT_COUNT;}
		case CHATTYPE_NATION_WAR:		{iCompareCount = m_iNationWarCounter- MAX_NATIONWAR_COUNT;	}break;
		case CHATTYPE_PARTY:			{iCompareCount = m_iPartyCounter	- MAX_PARTY_COUNT;		}break;
		case CHATTYPE_GUILD_BBS:		{iCompareCount = m_iGuildBBSCounter	- MAX_GUILDBBS_COUNT;	}break;
		case CHATTYPE_YES_OR_NO:		{iCompareCount = m_iYesNoCounter	- MAX_YES_NO_COUNT;		}break;
		case CHATTYPE_NOTICE:			{iCompareCount = m_iNoticeCounter	- MAX_NOTICE_COUNT;		}break;
		case CHATTYPE_LMS:				{iCompareCount = m_iNoticeCounter	- MAX_NOTICE_COUNT;		}break;
		default:			{iCompareCount= 0;}break;//  
		}
		const int iMyCount = (*itor)->GetIndex2();
		if( iMyCount < iCompareCount)// Max Ѿ 
		{
			SAFE_DELETE((*itor));
			itor = ChatList.erase(itor);
		}
		else
		{
			itor++;
		}
	}
}

void CChatMgr::DisplayCheck()//ü ε üũؼ 9   DisplayAble false  ش
{//DeleteCheck() ⼭ ڵ ⸦ Ѵ,
	const DWORD dwNowTime = SDL_GetTicks();
	
	int iCount = 0;
	for( chatitor itor = ChatList.begin() ; itor != ChatList.end();itor++)
	{
		if( !(*itor)->IsDisplayAble() ){continue;}
		const DWORD dwMaxDisplayTime =	(*itor)->GetMaxDisplayTime();
		const int iDisplayAbleCounter =   GetDisplayAbleCounter();
		if	( (dwNowTime > dwMaxDisplayTime) //÷ ð ̰ų
// 		|| 	( (*itor)->GetIndex() <= iDisplayAbleCounter) 	//ȭ  ؾ ϴµ īͰ ڶ
			)
		{
			(*itor)->SetDisplayAble(false);
		}
	}

	if( m_dwSelectWaitTime && dwNowTime > m_dwSelectWaitTime)
	{//ð ʰ 
		ClearYesNo();//Ŭ~
	}
}

int CChatMgr::DrawChatData(CChatData* pData, const int iY, const int iX, const int iLineGab)//  κ Դϴ.//   մϴ.
{//  Լ.
	const int iTextX = iX+2;
	const int iTextY = iY+2;

	const int iResult =  GetRectTextLine(pData->GetTotalData(),m_iWidth);//ܼ μ  κ
	
	Hcolor(pData->GetDataColor());//021030 lsw
	TxtOut.RcTXTOutBNoBG(iTextX, iTextY, m_iWidth, 0,"%s",pData->GetTotalData());//򺰷  κ ü

	if(pData->IsHaveHeader())
	{
		Hcolor(pData->GetNameColor());
		TxtOut.RcTXTOutBNoBG(iTextX, iTextY, m_iWidth, 0,"%s %s",pData->GetHeader(),pData->GetName());//򺰷  κ ü
		
		Hcolor(pData->GetHeaderColor());
		TxtOut.RcTXTOutBNoBG(iTextX, iTextY, m_iWidth, 0,"%s",pData->GetHeader());//򺰷  κ ü
	}
	else
	{
		Hcolor(pData->GetNameColor());
		TxtOut.RcTXTOutBNoBG(iTextX, iTextY, m_iWidth, 0,"%s",pData->GetName());//򺰷  κ ü
	}

	    // --- 新增：計算並儲存名字的點擊熱區 ---
        int nameX = iTextX;
        const int nameY = iTextY;
        int nameW = 0;
        int nameH = 0;

        // RcTXTOutBNoBG 暗示了使用粗體，所以我們用粗體字型來測量
        TTF_Font * font =  g_SDLInfo.g_BoldFont ? g_SDLInfo.g_BoldFont : g_SDLInfo.gFont;

        if (font)  // 僅在字型有效時執行
        {
          if (pData->IsHaveHeader()) {
            // 如果有標頭，先計算標頭和空格的寬度，以確定名字的起始 X 座標
            int headerW = 0;
              TTF_SizeUTF8_Safe(font, pData->GetHeader(), &headerW, NULL);

            int spaceW = 0;
              TTF_SizeUTF8_Safe(font, " ", &spaceW, NULL);

            nameX += headerW + spaceW;
          }

          // 現在，計算名字本身的像素寬度和高度
          if (pData->GetName() && pData->GetName()[0] != '\0') {
              TTF_SizeUTF8_Safe(font, pData->GetName(), &nameW, &nameH);
          }

          // 將計算好的矩形區域，透過我們剛才新增的函式，儲存到聊天資料物件中
		  SDL_Rect nameRect = { nameX, nameY, nameW < 20 ? 20 : nameW, nameH };
          pData->SetNameRect(nameRect);
        }
        // --- 點擊熱區邏輯結束 ---
	return iResult;
}

void CChatMgr::DisplayData( const int iX, const int iY ,const int iOption)
{
	DisplayCheck();
	DeleteCheck();

	// Define chatbox geometry
	const int CHAT_BOX_WIDTH = 350;
	const int CHAT_BOX_HEIGHT = 150;
	const int CHAT_BOX_X = iX;
	const int CHAT_BOX_Y = iY; // Draw upwards from the input line


	// 2. Create a temporary vector of messages that should be displayed based on filters
	std::vector<CChatData*> displayable_messages;
	for (chatitor itor = ChatList.begin(); itor != ChatList.end(); ++itor)
	{
		if (IsDisplayAbleType((*itor)->GetChatType()) && !IsRefuseGuy((*itor)->GetName()))
		{
			displayable_messages.push_back(*itor);
		}
	}

	// 3. Render the messages from bottom to top, respecting the scroll offset
	int current_y = CHAT_BOX_Y + CHAT_BOX_HEIGHT - m_iLineGab; 
	
	if (m_iScrollOffset >= displayable_messages.size())
	{
		m_iScrollOffset = displayable_messages.size() -1;
	}
	if (m_iScrollOffset < 0)
	{
		m_iScrollOffset = 0;
	}


	int start_index = displayable_messages.size() - 1 - m_iScrollOffset;

	for (int i = start_index; i >= 0; --i)
	{
		CChatData* pData = displayable_messages[i];
		if (!pData) continue;

		const int line_height = GetRectTextLine(pData->GetTotalData(), m_iWidth) * m_iLineGab;
		
		if(i != start_index)//第一行不用減去行高
			current_y -= line_height;

		if (current_y < CHAT_BOX_Y) // Stop if we're about to draw outside the top of the box
		{
			break;
		}
		FieldTypeNomalPutFx3(CHAT_BOX_X + 0, current_y, 0, 0, 11, NEW_CHAT_BACK_IMG, m_iAlpha, m_iBlendType);
		DrawChatData(pData, current_y, CHAT_BOX_X, m_iLineGab); 
	}
}

void CChatMgr::HandleMouseWheel(int direction)
{
	if (direction > 0) // Scroll Up
	{
		m_iScrollOffset += 3; // Increase offset to see older messages, scroll by 3 lines
	}
	else if (direction < 0) // Scroll Down
	{
		m_iScrollOffset -= 3; // Decrease offset to see newer messages, scroll by 3 lines
	}

	// Clamp the scroll offset
	if (m_iScrollOffset < 0)
	{
		m_iScrollOffset = 0;
	}
	// We don't know the max size here, so we clamp it in DisplayData
}


int CChatMgr::IsDisplayAbleType(const int iChatType)
{
	switch(iChatType)
	{
	case CHATTYPE_ALL:
	case CHATTYPE_NORMAL	:	
	case CHATTYPE_COLOSSUS_GLOBAL	:
	case CHATTYPE_COLOSSUS		:
	case CHATTYPE_LOCAL_WAR		:
	case CHATTYPE_NATION_CHAT		:
	case CHATTYPE_NATION_WAR		:
		{
			return (m_iNormalChatView)?1:0;
		}
	case CHATTYPE_WHISPER		:
		{
			return (m_iWhisperChatView)?1:0;
		}
	case CHATTYPE_GUILD		:
		{
			return (m_iGuildChatView)?1:0;
		}
	case CHATTYPE_PARTY		:	// Finito for use with new party chat type
		{
			return (m_iPartyChatView)?1:0;
		}
	case CHATTYPE_LMS:
	case CHATTYPE_YES_OR_NO:
	case CHATTYPE_NOTICE:
	case CHATTYPE_GUILD_BBS:
		{
			return 1;
		}
	default:			{}
		break;
	}	
	return 0;
}

void CChatMgr::Clear()
{//ī͵ ʱȭ Ѵ
	m_iTotalCounter		= 0;
	m_iNormalCounter	= 0;
	m_iWhisperCounter	= 0;	
	m_iGuildCounter		= 0;
	m_iGuildBBSCounter	= 0;
	m_iPartyCounter		= 0;
	m_iNationWarCounter	= 0;
	m_iLocalWarCounter	= 0;
	m_iNationChatCounter = 0;
	m_iColosussCounter	= 0;
	m_iYesNoCounter		= 0;
	m_iNoticeCounter	= 0;
	
	for( chatitor itor = ChatList.begin() ; itor != ChatList.end();)
	{
		SAFE_DELETE((*itor));
		itor = ChatList.erase(itor);
	}
	ChatList.clear();

	{
		m_iRefuseAllWhisper = 0;
		vRefuseList.clear();
	}
	ClearYesNo();
	ClearLimiteBBS();	// 031021 kyo //kyo
}

int CChatMgr::GetViewChat(const int iChatType )
{
	switch(iChatType)
	{
	case CHATTYPE_NORMAL:
		{
			return m_iNormalChatView;
		}
	case CHATTYPE_WHISPER:
		{
			return !IsRefuseAllWhisper();
		}
	case CHATTYPE_GUILD:
		{
			return m_iGuildChatView;
		}
	
	case CHATTYPE_PARTY:
		{
			return m_iPartyChatView;
		}
	}
	return 0;
}
void CChatMgr::SetViewChat(const int iChatType , const int iIsView)
{
	switch(iChatType)
	{
	case CHATTYPE_NORMAL:
	{
		if (iIsView)
		{
			AddCurrentStatusMessage(FONT_COLOR_GREEN, lan->OutputMessage(2, 621));
		}
		else
		{
			AddCurrentStatusMessage(FONT_COLOR_RED, lan->OutputMessage(2, 622));
		}
		m_iNormalChatView = iIsView;
	}break;
	case CHATTYPE_WHISPER:
		{
			(iIsView)?SetAcceptWhiper(NULL):SetRefuseWhiper(NULL);
		}break;
	case CHATTYPE_GUILD:
		{
			if(iIsView)
			{
				AddCurrentStatusMessage(FONT_COLOR_GREEN,lan->OutputMessage(2,623));
			}
			else
			{
				AddCurrentStatusMessage(FONT_COLOR_RED,lan->OutputMessage(2,624));
			}
			m_iGuildChatView = iIsView;
		}break;
	case CHATTYPE_PARTY:
		{
			if(iIsView)
			{
				AddCurrentStatusMessage(FONT_COLOR_GREEN,lan->OutputMessage(2,702));
			}
			else
			{
				AddCurrentStatusMessage(FONT_COLOR_RED,lan->OutputMessage(2,703));
			}
			m_iPartyChatView = iIsView;
		}break;
	default:
		{
		}break;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SendChatData(std::string lpStr, const int iChatType) { 		
	if ( lpStr.empty() ){return;}
    const int Len = lpStr.length(); 
	if (Len > TEXT_LENGTH-1) {
      return;
    }
	const int GM_EVENT = 81; // GM EVENT MAP

	if (MapNumber == GM_EVENT) return;

	if( !CheckLanguageForMenu( lpStr.c_str() ) ){return;}	//   ʴ´. 

	static DWORD time = 0;
	if( (SDL_GetTicks() - time) < PACKET_SEND_MINIMUM_TIME ) return;
	time = SDL_GetTicks();
	t_packet	packet;
	packet.h.header.type = CMD_CHAT_DATA;
	packet.h.header.size = sizeof( t_client_chat_data ) - TEXT_LENGTH + Len;
	packet.u.client_chat_data.type = iChatType; // ü . //020701 lsw
	packet.u.client_chat_data.length = Len;
	::strncpy( packet.u.client_chat_data.text, lpStr.data(), Len );
        packet.u.client_chat_data.text[Len] = '\0';
	::QueuePacket( &packet, 1 );


	// Data ʰ  ش. ͸..	
	t_server_chat_data	p;
	if( Hero )
	{
		p.server_id = Hero->id;
		p.type = iChatType;
		p.length = Len;
		if(iChatType == CHATTYPE_PARTY)
		{
			sprintf( p.text, "%s: %s", Hero->name, lpStr.c_str());
			
		}
		else
		{
			::strncpy(p.text, lpStr.c_str(), 200); p.text[199] = 0;
		}
		::RecvChatData( &p );//Ÿ  ó
	} 

	if(Hero && Hero->viewtype == VIEWTYPE_GHOST_)
	{
        if ((!stricmp(lpStr.c_str(), lan->OutputMessage(5, 53))))//以宇宙和大風暴的艾德布洛伊之名
		{
			char type = 1;
			CallServer( CMD_PLEASE_GIVE_LIFE, (char *)&type, sizeof( char ) );
		}
		else if (::stricmp(lpStr.c_str(), lan->OutputMessage(5, 50)) == 0)  // 以紅色的玫瑰和正義的歐雷姆之名
		{
			char type = 2;
			CallServer( CMD_PLEASE_GIVE_LIFE, (char *)&type, sizeof( char ) );
		}

		if( 	!stricmp( lpStr.c_str(), "save my life" ) )
		{
			if (Hero->fight_map_team_num) {
				return;
			}

			if (GetItemMuchByMuchItem(10057) < 3) {
				AddCurrentStatusMessage(FONT_COLOR_NORMAL, lan->OutputMessage(1, 112));
				return;
			}
			char type = 3;
			CallServer(CMD_PLEASE_GIVE_LIFE, (char*)&type, sizeof(char));
			/*packet.h.header.type = CMD_PLEASE_GIVE_LIFE1;
			packet.h.header.size = 0; 
			QueuePacket( &packet, 1 );*/

		}

	}
	
	/*if (!::stricmp(lpStr.c_str(), kein_GetMenuString(33)))  // "Learning Magic"
	{	
		const int mx = Hero->x / TILE_SIZE;
		const int my = Hero->y / TILE_SIZE;
		
		//&&  ġ  ̸ 
		if( 	SCharacterData.nCharacterData[SPELL] == WIZARD_SPELL
			&& MapNumber == 0 && BoxAndDotCrash( 34-3,72-3, 6,6, Hero->x / TILE_SIZE, Hero->y / TILE_SIZE ) && TileMap[ Hero->x / TILE_SIZE][ Hero->y / TILE_SIZE].attr_inside )
		{
			::CallSmallMenu(MN_LEARN_MAGIC_BOOK);
		}
		//&&  ġ   ϳ̸
		if( SCharacterData.nCharacterData[SPELL] == PRIEST_SPELL
			&& MapNumber == 0 &&  TileMap[ mx][ my].attr_inside &&
		(  BoxAndDotCrash( 288-6, 18-6, 15,11, mx, my ) ||
		   BoxAndDotCrash( 327-6, 84-6, 15,11, mx, my ) ||
		   BoxAndDotCrash( 217-6,317-6, 15,11, mx, my ) ) ) 
		{
			CallSmallMenu(MN_LEARN_MAGIC_BOOK);
		}
	}	

	if (!stricmp(lpStr.c_str(), lan->OutputMessage(5, 54)))
	{
		const int mx = Hero->x / TILE_SIZE;
		const int my = Hero->y / TILE_SIZE;
		if(BoxAndDotCrash( 256,133, 15, 11, mx, my ))
		{
			packet.h.header.type = CMD_QUEST_JOB_FINISH;
			packet.h.header.size = 0; 
			QueuePacket( &packet, 1 );
		}
	}
	*/
	// 021127 kyo 
	g_cQuest.SetSpellWord( lpStr.c_str() ); //script spellmapmove 
}

void RecvNPCChatData(t_server_chat_data* p)
{
	std::string temp(lan->OutputMessage(4, p->length));

	strncpy(p->text, temp.c_str(), 240);
	p->length = strlen(p->text);
	p->text[p->length+1] = 0;
	RecvChatData(p);
}
void RecvChatData( t_server_chat_data *p ) 
{	//< CSD-030520	
	char temp[MAX_CHAT_STRING + 20];
		
	if (p->server_id == 1000)
	{
		p->server_id = (short )Hero->id;
	}
		
	LPCHARACTER	ch = ::ReturnCharacterPoint(p->server_id);

	if (ch == NULL)
	{
		return;
	}

	const int iRecvType = p->type;
	if(ch){ch->nChatType = iRecvType;}
	
	switch( iRecvType )		// 010326 YGI
	{
	case CHATTYPE_NORMAL :	
		{
			if( 	ch == NULL ) { /*JustMsg( " ..." ); */ return; }
			if( Hero->viewtype != VIEWTYPE_GHOST_ )// 		 Ʈ ƴϰ		( 	)
			{
				if( 	ch->viewtype == VIEWTYPE_GHOST_ ) //ٸ  Ʈ ̸		( 	)
				{
					if(!CanViewGhost(Hero)) // z   					(     )
					{
						break;
					}
				}
			}
			strcpy( ch->szChatMessage, p->text );
			*(ch->szChatMessage + p->length) = 0;
			switch(GetDisplayChatType(ch))
			{
			case CHAT_COLOR_TYPE_HIGHLEVEL	:
			case CHAT_COLOR_TYPE_NPC		:	
			case CHAT_COLOR_TYPE_GUILDMASTER:	
			case CHAT_COLOR_TYPE_DUAL_CLASS	:	
			case CHAT_COLOR_TYPE_GM		:	
			case CHAT_COLOR_TYPE_SIT_DOWN	:		//011215 lsw
			case CHAT_COLOR_TYPE_ANI2		:	
			case CHAT_COLOR_TYPE_ANI3		:	
				{
					ch->ChatDelay	= CHAT_DELAY_TYPE_LONG_PC;
				}
				break;
			case CHAT_COLOR_TYPE_TUTORIAL_NPC:	
				{
					ch->ChatDelay	=CHAT_DELAY_TYPE_NPC;
				}
				break;
			default:
				{
					ch->ChatDelay = CHAT_DELAY_TYPE_DEFAULT_PC;
				}
				break;
			}
			ChatMgr.AddString( ch->name, p->text, iRecvType );		// äâ
		}
		break;
	case CHATTYPE_MERCHANT_BBS:// .
		{
			if( 	ch->viewtype == VIEWTYPE_GHOST_ ) //ٸ  Ʈ ̸		( 	)
			{
				break;
			}
			sprintf( temp, lan->OutputMessage(5,55), p->text );//lsw
			
			::strcpy( ch->szChatMessage, p->text );
			*(ch->szChatMessage + p->length) = 0;
			ch->ChatDelay = CHAT_DELAY_TYPE_MERCHANT_PC;
//			ChatMgr.AddString( ch->name, p->text, iRecvType );//äâ  ʴ´
		}
		break;
	case CHATTYPE_PARTY :	
		{
			::sprintf( temp, lan->OutputMessage(5,55), p->text );//lsw
			ChatMgr.AddString( NULL, temp, iRecvType );//020612 lsw
		}
		break;
	case CHATTYPE_WHISPER :	
		{
			::sprintf(temp, "%s", p->text );
			ChatMgr.AddString( NULL, temp, iRecvType );//021001 lsw
		}
		break;
	case CHATTYPE_COLOSSUS_GLOBAL:
	case CHATTYPE_COLOSSUS:
		{	
			char* name = ch->name;
			strcpy(ch->szChatMessage, p->text);
			*(ch->szChatMessage + p->length) = 0;
			ch->ChatDelay = CHAT_DELAY_70;

			ChatMgr.AddString(name, p->text, p->type);
			break;
		}
#ifdef _DEBUG
	case CHATTYPE_LMS:
		{
			FILE *fp;
			char temp[MAX_PATH];
			SYSTEMTIME time;
			GetLocalTime(&time);
			sprintf_s( temp, sizeof(temp), "LastManStanding-%2d-%2d-%4d.txt", time.wDay, time.wMonth, time.wYear);
			fp = fopen( temp, "at+" );
			if( !fp ) return;
			fprintf(fp,"%s\n", p->text);
			fclose( fp );

			ChatMgr.AddString(ch->name, p->text, iRecvType);
		}
#endif
	}
} 	//> CSD-030520	

void SendChatSet( char chattype, char *name )
{	
	t_packet p;
	char *temp;
	int len;

	switch( chattype )
	{
		case CHATTYPE_GUILD :
		{
			p.h.header.type = CMD_CHAT_SET;
			{
				p.u.chat_set.chattype = chattype;
				p.u.chat_set.chatname[0] = 0;
				len = 0;
			}
			p.h.header.size = sizeof( t_chat_set ) - 21 + len;
			QueuePacket( &p, 1 );
		break;
		}
		case CHATTYPE_PARTY :
		{
			p.h.header.type = CMD_CHAT_SET;
			{
				p.u.chat_set.chattype = chattype;
				p.u.chat_set.chatname[0] = 0;
				len = 0;
			}
			p.h.header.size = sizeof( t_chat_set ) - 21 + len;
			QueuePacket( &p, 1 );
		break;
		}
		case CHATTYPE_WHISPER :
		{
			if( name == NULL ) 
			{
				len = 0;
			}
			else
			{
				len = strlen( name );
				if( len > 20 )
				{
					AddCurrentStatusMessage( 255,0,0, lan->OutputMessage(5,57), name ); //lsw
					Lc_ChatType = CHATTYPE_NORMAL;
					return; 
				}
			}
			char *t = strtok( name, " \t\n" );
			if( t == NULL )
				temp = NULL;
			else 
				temp = name;

			
			if( chattype == CHATTYPE_WHISPER && temp != NULL )
			{
				strcpy( Lc_TempChatName, temp );
			}
				
			p.h.header.type = CMD_CHAT_SET;
				p.u.chat_set.chattype = chattype;
				if( temp == NULL )
				{
					p.u.chat_set.chatname[0] = 0;
					len = 0;
				}
				else 
				{
					strcpy( p.u.chat_set.chatname, temp );
					len = strlen( temp );
				}
			p.h.header.size = sizeof( t_chat_set ) - 21 + len;
			QueuePacket( &p, 1 );
		break;
		}
	}
}

void RecvChatSetResult( t_chat_set_result *p )
{
	switch( p->result ) 
	{
		case 0 :	AddCurrentStatusMessage( 255,0,0, lan->OutputMessage(5,91), Lc_TempChatName );	Lc_ChatType = CHATTYPE_NORMAL;		break; //010216 lsw
		case 1 :	AddCurrentStatusMessage( 255,0,255, lan->OutputMessage(5,92), Lc_TempChatName );	Lc_ChatType = CHATTYPE_WHISPER;	break;
		case 2 :	AddCurrentStatusMessage( 255,0,255, lan->OutputMessage(5,93) );				Lc_ChatType = CHATTYPE_NORMAL;		break;
		case 3 :	AddCurrentStatusMessage( 255,0,255, lan->OutputMessage(5,93) );				Lc_ChatType = CHATTYPE_NORMAL;		break;

		case 10 :	AddCurrentStatusMessage( 255,0,210, lan->OutputMessage(5,94) );		Lc_ChatType = CHATTYPE_PARTY;		break;
		case 11 :	AddCurrentStatusMessage( 255,0,150, lan->OutputMessage(5,95) );		Lc_ChatType = CHATTYPE_NORMAL;		break;

		case 20 :	AddCurrentStatusMessage( 100,40,150, lan->OutputMessage(5,96) );										Lc_ChatType = CHATTYPE_NORMAL;		break;
		case 21 :	AddCurrentStatusMessage( 100,40,210, lan->OutputMessage(5,97) );		Lc_ChatType = CHATTYPE_GUILD;		break;//010216 lsw
	}
}	
	
Spr *chatimage[NEW_CHAT_SPR_MAX];

void LoadChatImage()
{
	for( int i = 0; i <NEW_CHAT_SPR_MAX ;	 i ++)
	{
		chatimage[i] 	= GetSprOfMenu(NEW_CHAT_IMG, i);
	}
}

void LoadChatColours() // finito 12/07/07
{
    /*string strChatSelect;
    stringstream ss;

    int nR,nG,nB;
    for(int i = 0; i <= 7;	 i++)
    {
        ss << "chat" << i << "r";
        strChatSelect = ss.str();
        nR = GetPrivateProfileInt( "chatcolours", strChatSelect.c_str(), -1, "ChatColour.ini" );
        ss.str("");
        if (nR < 0) continue;

        ss << "chat" << i << "g";
        strChatSelect = ss.str();
        nG = GetPrivateProfileInt( "chatcolours", strChatSelect.c_str(), -1, "ChatColour.ini" );
        ss.str("");
        if (nG < 0) continue;

        ss << "chat" << i << "b";
        strChatSelect = ss.str();
        nB = GetPrivateProfileInt( "chatcolours", strChatSelect.c_str(), -1, "ChatColour.ini" );
        ss.str("");
        if (nB < 0) continue;

        switch (i)
        {
        case 0:
            CHAT_COLOR_NORMAL = ConvertColor(nR,nG,nB);
            break;
        case 1:
            CHAT_COLOR_GUILD = ConvertColor(nR,nG,nB);
            break;
        case 2:
            CHAT_COLOR_COLOSSUS = ConvertColor(nR,nG,nB);
            break;
        case 3:
            CHAT_COLOR_WHISPER = ConvertColor(nR,nG,nB);
            break;
        case 4:
            CHAT_COLOR_WAR = ConvertColor(nR,nG,nB);
            break;
        case 5:
            CHAT_COLOR_PARTY = ConvertColor(nR,nG,nB);
            break;
        case 6:
            CHAT_COLOR_NOTICE = ConvertColor(nR,nG,nB);
            break;

        case 7:
            CHAT_COLOR_NATION = ConvertColor(nR, nG, nB);
default:
            break;
        }
    }*/
}

//011215 lsw
int GetDisplayChatType( LPCHARACTER ch )
{	
//	return 12;//Ʈ 	ª 	׸ 	Ϲ 
	if( ch->type ==SPRITETYPE_CHARACTER)
	{
		if(ch->nChatType == CHATTYPE_MERCHANT_BBS)
		{
			return CHAT_COLOR_TYPE_MERCHANT_BBS;
		}
		if(ch->nCurrentAction==ACTION_SITDOWN)
		{
			return CHAT_COLOR_TYPE_SIT_DOWN;	
		}
		if( CheckGuildMaster( ch ) )
		{
			return CHAT_COLOR_TYPE_GUILDMASTER;
		}
		if( 	ch->call_job_lv == 100)	
		{
			return CHAT_COLOR_TYPE_GM;
		}//̳..
		if(ch->lv>61)
		{
			return CHAT_COLOR_TYPE_HIGHLEVEL;
		}// 
	}
	else
	{
		if( ch->sprno == 80 	||  ch->sprno == 22)	{ return CHAT_COLOR_TYPE_TUTORIAL_NPC;	}
		if( ch->sprno == 18 	||  ch->sprno == 20)	{ return CHAT_COLOR_TYPE_TUTORIAL_NPC;	}
		
		return CHAT_COLOR_TYPE_NPC;
	}
	return CHAT_COLOR_TYPE_DEFAULT;//Ʈ 	ª 	׸ 	Ϲ 
}

void DisplayChatMessage( LPCHARACTER ch )//޼,	 Į 	÷,	
{		
	if( tool_ViewChat ){return;}
	if( 0 	>= ch->ChatDelay )	{return;}

	int	 iPosMoveX = 0, iPosMoveY = 0;
	const int iChatImageType = GetDisplayChatType(ch);
	
	switch(iChatImageType)
	{
	case CHAT_COLOR_TYPE_MERCHANT_BBS://021126 lsw
	case CHAT_COLOR_TYPE_SIT_DOWN	:
		{
			iPosMoveX = -95;
			iPosMoveY = -80;
		}
		break;
default:
		{
			iPosMoveX = +13;
			iPosMoveY = -65;
		}
		break;
	}

	if( ch->viewtype == VIEWTYPE_GHOST_ )
	{	
		iPosMoveY = -90;
	}

	char *pMsg = ch->szChatMessage;

	const int iX = ch->visual_x - Mapx+iPosMoveX;
	const int iY = ch->visual_y - Mapy+iPosMoveY;

	int iLCt = 0;
	int iChatWidth = 0;

	int iLGab = CHAT_BG_HEIGHT;// 	

	switch(iChatImageType)
	{// 
	case CHAT_COLOR_TYPE_DEFAULT:
		{
			iChatWidth = 135;
		}
		break;
	case CHAT_COLOR_TYPE_MERCHANT_BBS:
		{
			iLGab = MERCHANT_CHAT_BG_HEIGHT; iChatWidth = 180;
		}break;	
default:
		{
			iChatWidth = 180;
		}
		break;
	}

	const int iGab = 10;//鿩 	

	TxtOut.Init(COLOR_BLACK, TXT_BG_NORMAL_WIDTH,12);
		
	::Hcolor(FONT_COLOR_WHITE);
	iLCt = TxtOut.RcChatOut(iX, iY, iChatWidth,iGab,iChatImageType,"%s",pMsg);
	::Hcolor(FONT_COLOR_GREEN);
	TxtOut.RcChatHeaderOut(iX, iY-iLGab*(iLCt+1), iChatWidth,iGab,iChatImageType,"%s",ch->name);//iLCt+1   ̸ 
	return;
}

void RecvCMD_LOCALWAR_CHAT(t_packet* p) // LTS NEW LOCALWAR
{
#ifdef _DEBUG
	AddCurrentStatusMessage( 255,255,0,"Received a Local War Message..."); //Eleval - To see if we actually received it...
#endif
	ChatMgr.AddString(p->u.LocalWar.LocalWarChat.mzName, p->u.LocalWar.LocalWarChat.Message, CHATTYPE_NATION_CHAT);	//020515 lsw//020702 lsw
}


void RecvCMD_NATION_CHAT(t_packet* p) // LTS NEW LOCALWAR
{
#ifdef _DEBUG
	AddCurrentStatusMessage(255, 255, 0, "Received a Nation Message..."); //Eleval - To see if we actually received it...
#endif

	ChatMgr.AddString(p->u.LocalWar.LocalWarChat.mzName, p->u.LocalWar.NationChat.Message, CHATTYPE_NATION_CHAT);
}
void RecvCMD_SQUAD_CHAT(t_packet* p)
{
//	char tempMessage[120];
//	char* tempPointer=tempMessage;

//	memcpy(tempMessage,p->u.NationWar.SquadChat.Message,p->u.NationWar.SquadChat.Size);
//	*(tempPointer+p->u.NationWar.SquadChat.Size)='\0';

	p->u.NationWar.SquadChat.Message[MAX_STRING_PK-1] = 0;

	ChatMgr.AddString( NULL, p->u.NationWar.SquadChat.Message, CHATTYPE_NATION_WAR );	//020515 lsw//020702 lsw
	
	switch (p->u.NationWar.SquadChat.Type)
	{
	case 0 :
	case 1 :
	case 2 :
	case 3 :
		PlayListAutoSounds(50,0,0,0);
		break;
	case 4 :
		PlayListAutoSounds(64,0,0,0);
		break;
	}
}

void RecvGuildAllMsg( t_packet *p )
{
	MP3( SN_EAR_MESSAGE_RECV );
	ChatMgr.AddString("!", p->u.kein.default_msg, CHATTYPE_GUILD_BBS);//021001 lsw
}

void CChatMgr::ReplyWhisper( const WORD dSenderServerIndex, const WORD dSenderCn, const char cResult )//   ޽  
{
	t_packet p;
	p.h.header.type = CMD_SEND_EAR_MESSAGE_RESULT;
	p.h.header.size = sizeof( k_send_ear_message_result );

	p.u.kein.send_ear_message_result.send_game_server	=	 dSenderServerIndex;
	p.u.kein.send_ear_message_result.send_server_id		=	 dSenderCn;
	p.u.kein.send_ear_message_result.ret				=	 cResult;

	QueuePacket( &p, 1 );
}

void CChatMgr::RecvWhisper( const char *szName, char *szMsg, const WORD dSenderServerIndex, const WORD dSenderCn)//ӼӸ ޽ϴ.
{
	// Finito - Altered if statements so that GM's and RC's can whisper dead / blocking whisper characters
	LPCHARACTER pWhisperChar = ExistHe((char*)szName);
	char cResult = 1;//1  ̴ ʱġ ٲ 
	if (pWhisperChar == NULL)
	{
		if( ::IsHeroDead() ) 
		{
			cResult = 2;
		}//׾  

		if( IsRefuseAllWhisper()) 
		{
			cResult = 3;
		}
		if( IsRefuseGuy(szName)) 
		{
			cResult = 3;
		}
	}
	else
	{
		if( ::IsHeroDead() && !pWhisperChar->IsCounselor() && !pWhisperChar->IsReporter() )
		{
			cResult = 2;
		}//׾  

		if( IsRefuseAllWhisper() && !pWhisperChar->IsCounselor() && !pWhisperChar->IsReporter() )
		{
			cResult = 3;
		}
		if( IsRefuseGuy(szName) && !pWhisperChar->IsCounselor() && !pWhisperChar->IsReporter() )
		{
			cResult = 3;
		}
	}
	switch(cResult)
	{
	case 1:
		{
			ReplyWhisper( dSenderServerIndex, dSenderCn, cResult );// 
		}
		break;
default:
		{
			ReplyWhisper( dSenderServerIndex, dSenderCn, cResult );// 
			return;// 	ѹ
		}
		break;
	}	
	std::string msg(szMsg);
	::ConvString( msg );	
	::MP3( SN_EAR_MESSAGE_RECV );	

	AddString(szName, msg.c_str(), CHATTYPE_WHISPER);  // 021001 lsw
	::InsertWisperList(szName);//Ӹ   ̸ 
}

int GetChatMode()
{
	if(g_bCommandMode == CM_MESSAGE)
	{
		
	}
	else
	{
		
	}	
	return g_bCommandMode;
}

void SetChatMode(const int iMode,bool bIsForce)// Ŀ Ӹ Ѱ  մϴ.
{
	switch(iMode)
	{
	case CM_MESSAGE:// 	ѹ 
		{
		
			if( SMenu[MN_SKILLGUILD_REPAIR].bActive
			||	SMenu[MN_SKILLGUILD_BUY].bActive 
			||	SMenu[MN_SKILLGUILD_SELL].bActive
			||	SMenu[MN_SKILLGUILD_SKILL].bActive 
			||	SMenu[MN_MEM_BOOK].bActive 
			||	SMenu[MN_MAIL_WRITE].bActive
			||	SMenu[MN_MAIL_READ].bActive
			||	SMenu[MN_MAIL_MENU].bActive
			||	SMenu[MN_DIVIDE_ITEM].bActive
			||	SMenu[MN_FRIEND_REGIST].bActive
			||	SMenu[MN_BANK_REPAYMENT].bActive
			||	SMenu[MN_MERCHANT_REGISTER].bActive
			)
			{
				g_bCommandMode = CM_COMMAND;//Ŀ   ش
				SMenu[MN_MAININTERFACE].nField[44].nType = FT_NONE;
				SMenu[MN_MAININTERFACE].nField[47].nType = FT_NONE;

				return;
			}
//			EWndMgr.ClearAllTxt();//021001 lsw
			
			if(Hero&&Hero->viewtype == VIEWTYPE_OBSERVE_)	// 030522 kyo
			{
				return;
			}
			g_bCommandMode = iMode;
		
		}
		break;
	case CM_COMMAND://Ŀ 
		{
			SMenu[MN_MAININTERFACE].nField[44].nType = FT_NONE;
			SMenu[MN_MAININTERFACE].nField[47].nType = FT_NONE;
			g_bCommandMode = iMode;
			return;
		}
		break;
default:
		{

		}
		break;
	}
}

bool IsChatBoxActive()
{
	return (GetChatMode() == CM_MESSAGE)?true:false;
}

int GetChatTarget()
{
	const int iType =SMenu[MN_WISPER_LIST].work;
	return iType;
}

bool IsAbleChatTargetName(const int iType)
{
	switch(iType)
	{
	case CHAT_TARGET_NORMAL	:
	case CHAT_TARGET_GUILD	:
	case CHAT_TARGET_PARTY	:
	case SELECT_WISPER_MODE	:
		{
			return true;
		}
		break;
	case CHAT_TARGET_WISPER1			:
		case CHAT_TARGET_WISPER2			:
		case CHAT_TARGET_WISPER3			:
		case CHAT_TARGET_WISPER4			:
		case CHAT_TARGET_WISPER5			:
		{
			char wisper_name[20] = { 0 };
			EWndMgr.GetTxt(HWND_CHAT_TARGET_INPUT, wisper_name, 0);
			if(wisper_name[0]!=0)
			{
				return true;
			}
		}
		break;
default:
		{

		}
		break;
	}
	return false;
}

bool IsChatBoxLock()
{
	return (SMenu[MN_MAININTERFACE].nField[47].nWillDo)?true:false;
}


bool SendChatNormal(char* msg)
{
	const int iSize = strlen( msg );
	if(0 >= iSize ){	return true;}

	EatRearWhiteChar( msg );//020725 lsw

	if( IsLineCommand( msg, iSize ) ) 
	{
		EWndMgr.ClearTxt(HWND_CHAT_INPUT);//021001 lsw
		return false;		// 	ɾ 	ó 	
	}
    std::string Msg(msg);
	ConvString( Msg );// 	弳 		//020725 lsw
	Hero->ChatDelay	= CHAT_DELAY_70;
	strncpy( Hero->szChatMessage, Msg.c_str(), 255 );
	Hero->szChatMessage[254] = 0;
	EWndMgr.ClearTxt(HWND_CHAT_INPUT);//021001 lsw
	SendChatData( Msg );						//  	޽ 	.	
	
	return true;
}

struct k_guild_chat_map
{
 short int guild_code;
 char name[20];
 char msg[1024];
};

bool SendChatGuild( std::string msg )
{
	//if( IsDead( Hero ) ){return true;} //Enable guildchat while dead
	EatRearWhiteChar( msg );//020725 lsw
    const int iSize = msg.length();
	if(0 > iSize ){	return true;}
	else if( 0 == iSize )
	{
		SetChatTarget(CHAT_TARGET_GUILD);
		return false;
	}

	if (IsLineCommand(msg.c_str(), iSize)) 
	{
		EWndMgr.ClearTxt(HWND_CHAT_INPUT);//021001 lsw
		return false;		// 	ɾ 	ó 	
	}

	ConvString( msg);// 	弳 		//020725 lsw

	g_Menu_Variable.m_bGuildChatOn = true;

	/*packet.h.header.type = 10376; //Eleval to test a bug
	packet.h.header.size = iSize;
	strcpy( (char *)packet.u.data, msg );
	k_guild_chat_map guild_chat_map;
	guild_chat_map = reinterpret_cast<k_guild_chat_map*>(packet.u.data);
	QueuePacket( &packet, 1 );*/

	/*int guild_code = 38;

	 t_packet packet;
	 packet.h.header.type = 10376;
	 packet.h.header.size = iSize + 2 + 20;
	 int offset = 0;
	 memcpy(packet.u.data + offset, &guild_code, 2);
	 offset+=2;
	 memcpy(packet.u.data + offset, "GMTesting", 20);
	 offset+=20;
	 memcpy(packet.u.data + offset, msg, strlen(msg));
	 QueuePacket( &packet, 1 );*/
	
	t_packet packet;
	packet.h.header.type = CMD_GUILD_CHAT;
	packet.h.header.size = iSize;
	strncpy( (char *)packet.u.data, msg.c_str(), iSize );
        packet.u.data[iSize] = '\0';
	QueuePacket( &packet, 1 );
	return true;
}

bool SendChatWisper(const char *name,const char *msg)
{
	std::string Msg(msg);
  if (Msg.empty()) return true;
	EatRearWhiteChar( Msg );//020725 lsw

	if (IsLineCommand(Msg.c_str(), Msg.length())) 
	{
		EWndMgr.ClearTxt(HWND_CHAT_INPUT);//021001 lsw
		return false;		// 	ɾ 	ó 	
	}

	
	ConvString( Msg );// 	弳 		//020725 lsw

	const int iNameLen	= strlen(name);
	const int iMsgLen		= Msg.length();
	if( (20 < iNameLen )	|| !iNameLen || (200 <= iMsgLen) )
	{
		AddCurrentStatusMessage( FONT_COLOR_YELLOW,lan->OutputMessage(0,501));//" 	̸ 	߸ 	Ǿų, 	ӼӸ 	ʹ 	ϴ."
		return false;	//޼   	
	}
	
	InsertWisperList(name);

	t_packet p;
	p.h.header.type = CMD_SEND_EAR_MESSAGE;
	p.h.header.size = sizeof( k_send_ear_message )-200+iMsgLen;
	strcpy( p.u.kein.send_ear_message.name, name);
	strncpy( p.u.kein.send_ear_message.message, Msg.data(),199);
        p.u.kein.send_ear_message.message[199] = '\0';
	QueuePacket( &p, 1 );
	return true;
}

void SendWisperByLineCommand(std::string name, std::string text)//SendEarMessageByLineCommand̰Ÿ  ؼ  äâ ޼ 
{
    if (!text.empty()) 
	{
    SendChatWisper(name.c_str(), text.c_str());
	}
	else
	{
		SetChatMode(CM_MESSAGE);//ä 	Է 	 
		SetChatTarget(SELECT_WISPER_MODE);//ӼӸ 	Է 
		EWndMgr.SetTxt(HWND_CHAT_TARGET_INPUT,name.c_str());//021001 lsw//Էâ 	Ŭ//̸ 	Է 	
		EWndMgr.ClearTxt(HWND_CHAT_INPUT);//021001 lsw//Էâ 	Ŭ
	}
}

bool SendPartyChatByLineCommand(const char *str_buf )//SendEarMessageByLineCommand̰Ÿ  ؼ  äâ ޼ 
{
	const int iSize = strlen( str_buf );
	if(0 >= iSize ){	return true;}
	else if( 0 == iSize )
	{
		SetChatTarget(CHAT_TARGET_PARTY);
		return false;
	}

	if( IsLineCommand( str_buf, iSize) ) 
	{
		EWndMgr.ClearTxt(HWND_CHAT_INPUT);//021001 lsw
		return false;		// 	ɾ 	ó 	
	}

	if(str_buf[0])//̸ 	Է 	Ǿ 	޼ 	ù 	ִٸ
	{
		EWndMgr.ClearTxt(HWND_CHAT_INPUT);//021001 lsw
		SendChatData( str_buf, CHATTYPE_PARTY);
	}
	return true;
}

void BannedUser(const char* szCharName)
{
	t_packet packet;
	ZeroMemory(&packet, sizeof(packet));

	packet.h.header.type = CMD_ABSOLUTE_LOGOUT;
	packet.h.header.size = sizeof(t_ban_account);
	::QueuePacket(&packet, 1);

	char szTmpName[MAX_NAME_CHARACTER] = { 0, };
	//g_pHero->GetName(szTmpName);
	ExistHe(szTmpName);
	/*if (strcmp(szTmpName, szCharName) == 0)
		m_bIsBannedUser = false;*/
} 	//> LTH-040805-KO.

void SendNationChat(std::string text)		// LTS NEW LOCALWAR	
{
	t_packet packet;
	packet.h.header.type = CMD_LOCALWAR_CHAT;

	packet.u.LocalWar.LocalWarChat.NationIndex = Hero->name_status.nation;		// Broad Cast Use
	sprintf(packet.u.LocalWar.LocalWarChat.Message, "%s", text.c_str());
	EatRearWhiteChar(packet.u.LocalWar.NationChat.Message);
	packet.h.header.size = sizeof(t_LocalWarChat);
	QueuePacket(&packet, 1);
}

void SendLocalWarChat(const char* Arg)		// LTS NEW LOCALWAR	
{
	if (g_LocalWarBegin)
	{
		if (Hero->JoinLocalWar)
		{
			t_packet packet;
			packet.h.header.type = CMD_LOCALWAR_CHAT;

			packet.u.LocalWar.LocalWarChat.NationIndex = GetNationIndex(Hero->name_status.nation);		// Broad Cast Use
            std::string msg(Arg);
			ConvString(msg);// 	弳 
			sprintf(packet.u.LocalWar.LocalWarChat.Message, "%s : %s", Hero->name, msg.c_str());
			
			EatRearWhiteChar(packet.u.LocalWar.LocalWarChat.Message);

			packet.h.header.size = strlen(packet.u.LocalWar.LocalWarChat.Message) + 1;
			QueuePacket(&packet, 1);

#ifdef _DEBUG
			AddCurrentStatusMessage(255, 255, 0, "Sent a Local War Message..."); //Eleval - To see if we actually sent it...
#endif
		}
		else
		{
			AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 370));//" 	ϼž߸ 	Ҽ 	ֽϴ."
		}
	}
	else AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 371));//" 	Ȳ 	Ҽ 	ֽϴ."
}
/////////////////////////////////////////////////////////////////////////////
//Yes or No ý
// yes no system ----------------------------------------------------------
//-------------------------------------------------------------------------
int CChatMgr::RecvNeedYesNoMsg( const int iType, const int iDemanderCn, const char *szMsg, ... ) 
{
	if(IsCheckYesNoAble())
	{
#ifdef _DEBUG
		::AddCurrentStatusMessage( FONT_COLOR_BLIGHT_BLUE, "recv Type = %d", iType );
#endif
		::AddCurrentStatusMessage( FONT_COLOR_BLIGHT_BLUE, lan->OutputMessage(2,760));
		return 0;
	}
    if (!szMsg) return 0;

    char szBuffer[1024]{}; // 確保緩衝區夠大

    va_list arg;
    va_start(arg, szMsg);
    // 移除 if (arg == NULL) 判斷
    // 改用安全的 vsnprintf
    vsnprintf(szBuffer, sizeof(szBuffer), szMsg, arg);
    va_end(arg);

    szBuffer[sizeof(szBuffer) - 1] = '\0'; // 確保結尾

	m_iYesOrNoType			= iType;
	m_iYesOrNoDemanderCn	= iDemanderCn;
	m_dwSelectWaitTime		= SDL_GetTicks()+MAX_CHAT_DISPLAY_TIME;
	
	AddString(NULL,szMsg,CHATTYPE_YES_OR_NO);//޼   ־
	return 1;
}
//////////////////////////////////////////////////////////////////////////		yex_no system

void CChatMgr::CheckYesNo( const int iIsYes )
{
	if(!ChatMgr.IsCheckYesNoAble()){return;}//ð 	ʰ 
	switch( m_iYesOrNoType )
	{
	case YN_GUILD_JOIN_OK :
		{
			(iIsYes)?
				SendGuildJoinConfirmOk( m_iYesOrNoDemanderCn ):
				SendGuildJoinConfirmNot( m_iYesOrNoDemanderCn );
		}
		break;
default:
		{	//  ε..	
		}
		break;
	}
	ClearYesNo();//Ŭ 	ش
}

int CheckLanguageForMenu( const char* lpStr) // ޴   ý
{
	if( ChatMgr.IsCheckYesNoAble() )		// yes, no 	ؾ 	Ҷ 
	{
		if( stricmp(lpStr,lan->OutputMessage(7,96))==0 
		||	stricmp(lpStr,lan->OutputMessage(7,97))==0 
		||	stricmp(lpStr,lan->OutputMessage(7,98))==0 
		||	stricmp(lpStr,"YES")==0 )
		{
			ChatMgr.CheckYesNo( 1 );
			return 0;
		}
		if( 	stricmp(lpStr,lan->OutputMessage(7,99))==0 
		||	stricmp(lpStr,"NO")==0 
		|| 	stricmp(lpStr,lan->OutputMessage(7,100))==0  )//010215 lsw
		{
			ChatMgr.CheckYesNo( 0 );
			return 0;
		}
	}

	if( SCharacterData.believe_god )
	{
		/* 	祈禱 	可以增加MP
		93 : "以暴風雨中花朵之名"
		94 : "為了需要時的小幸運。"
		95 : "只依靠榮譽與正義之劍。"
		93 : "In the name of the flower in the storm"
		94 : "For the small luck when it's needed."
		95 : "Only depending on the sword of honor and justice."
		*/
		switch( SCharacterData.believe_god )	//String method for temple
		{
			case EDELBLHOY		: 	if( !stricmp( lpStr, lan->OutputMessage(7,93) )) CallServer( CMD_DIVINE_UP ); 	break;	//010215 lsw
			case TEFFERY		: 	if( !stricmp( lpStr, lan->OutputMessage(7,94))) CallServer(CMD_DIVINE_UP ); 	break;	//010215 lsw
			case LETTY		: 	if( !stricmp( lpStr, lan->OutputMessage(7,95))) CallServer( CMD_DIVINE_UP ); 	break;	//010215 lsw
default :			break;
		}
	}

	const int Len = strlen( lpStr );
	if( g_FightMapStart )
	{
		t_packet packet;
		packet.h.header.type = CMD_CHAT_DATA;
		packet.h.header.size = sizeof( t_client_chat_data ) - TEXT_LENGTH + Len;

		packet.u.client_chat_data.type = CHATTYPE_COLOSSUS; // ü .	
		packet.u.client_chat_data.length = Len;
		memcpy( packet.u.client_chat_data.text, lpStr, Len );
		
		QueuePacket( &packet, 1 );
		return 0;
	}
	languageSystem( lpStr );			// 0605 Woo
	return 1;
}

void RecvWhisperReply( const int type, const char *szName )		// ӼӸ..		//     
{
	switch( type )
	{
		case CMD_SEND_EAR_MESSAGE_OK	:	::AddCurrentStatusMessage( 255, 0, 255, lan->OutputMessage(5,41), szName); break;	//010215 lsw
		case CMD_SEND_EAR_MESSAGE_NOT	:	::AddCurrentStatusMessage( 255, 0, 0, lan->OutputMessage(5,42), szName); break;	//010215 lsw
	}
}

void RecvWhisperReply2( const int ret )
{
	switch( ret )
	{
	case 1:		::AddCurrentStatusMessage( 255, 0, 255, lan->OutputMessage(5,43));	break;	//010215 lsw
	case 2:		::AddCurrentStatusMessage( 255, 0, 0, lan->OutputMessage(5,44));	break;	//
	case 3:		::AddCurrentStatusMessage( 255, 0, 0, lan->OutputMessage(5,45));	break;	// 	
	case 4:		::AddCurrentStatusMessage( 255, 0, 0, lan->OutputMessage(5,46));	break;	//ٸ
	default :	::AddCurrentStatusMessage( 255, 0, 0, lan->OutputMessage(5,47));	break;	//޽
	}
}

void RecvWhisperReplyEtc( t_packet *p )
{
	const int ret = p->u.kein.default_char;
	switch( ret )
	{
		case -1 :	::AddCurrentStatusMessage( 255, 10, 20, lan->OutputMessage(3,51)); break;	//010215 lsw
		default :	break;
	}
}

void CChatMgr::DisplayRefuseList()
{
	if( ChatMgr.IsRefuseAllWhisper() ) 
	{
		::AddCurrentStatusMessage( 200, 255 , 200 , lan->OutputMessage(7,120) );	//010215 lsw
		return;
	}
	else
	{
		::AddCurrentStatusMessage( 255, 200 , 255 , lan->OutputMessage(7,121) );	//010215 lsw
	}

	for(auto& itor : vRefuseList)
	{
		::AddCurrentStatusMessage( 100, 200 , 100 , lan->OutputMessage(7,122), itor.c_str() );	//010215 lsw
	}		
}

void SetChatTarget(const int iType)
{
	const int iNow = GetChatTarget();
	if(IsAbleChatTargetName(iType) )
	{
		SMenu[MN_WISPER_LIST].work = iType;
		if(SELECT_WISPER_MODE==iType)
		{
			EWndMgr.ClearTxt(HWND_CHAT_TARGET_INPUT);
		}
	}
	else
	{
		SMenu[MN_WISPER_LIST].work = CHAT_TARGET_NORMAL;
		EWndMgr.showInputBox(HWND_CHAT_INPUT);
	}
}

int InsertWisperList(const char* name)
{
	int i = CHAT_TARGET_WISPER_START;
	for(;i<CHAT_TARGET_WISPER_END;	 i++)
	{
		if(!strcmp(SMenu[MN_WISPER_LIST].nField[i].temp,name))	//񱳰 	̸ 	ٸٸ
		{	//̹ 	̸ 	  ϰ־
			return i;	//
		}
		if(!SMenu[MN_WISPER_LIST].nField[i].temp[0])	//ƿ 	̸ 	ٸ
		{
			strcpy(SMenu[MN_WISPER_LIST].nField[i].temp,name);	//ڸ 	ְ
			return i;	// 	ְ 	ֶ
		}							
	}
	if(i==CHAT_TARGET_WISPER_END)	// 	ڸ 	  ϸ
	{
		int activeWhisper;

		if(IsChatBoxActive())
			activeWhisper = GetChatTarget();
		else
			activeWhisper = -1;

		if(activeWhisper != CHAT_TARGET_WISPER5)
		{
			if(activeWhisper != CHAT_TARGET_WISPER4)
				strcpy(SMenu[MN_WISPER_LIST].nField[CHAT_TARGET_WISPER5].temp,SMenu[MN_WISPER_LIST].nField[CHAT_TARGET_WISPER4].temp);
			else
				strcpy(SMenu[MN_WISPER_LIST].nField[CHAT_TARGET_WISPER5].temp,SMenu[MN_WISPER_LIST].nField[CHAT_TARGET_WISPER3].temp);
		}
		if(activeWhisper != CHAT_TARGET_WISPER4)
		{
			if(activeWhisper != CHAT_TARGET_WISPER3)
				strcpy(SMenu[MN_WISPER_LIST].nField[CHAT_TARGET_WISPER4].temp,SMenu[MN_WISPER_LIST].nField[CHAT_TARGET_WISPER3].temp);
			else
				strcpy(SMenu[MN_WISPER_LIST].nField[CHAT_TARGET_WISPER4].temp,SMenu[MN_WISPER_LIST].nField[CHAT_TARGET_WISPER2].temp);
		}
		if(activeWhisper != CHAT_TARGET_WISPER3)
		{
			if(activeWhisper != CHAT_TARGET_WISPER2)
				strcpy(SMenu[MN_WISPER_LIST].nField[CHAT_TARGET_WISPER3].temp,SMenu[MN_WISPER_LIST].nField[CHAT_TARGET_WISPER2].temp);
			else
				strcpy(SMenu[MN_WISPER_LIST].nField[CHAT_TARGET_WISPER3].temp,SMenu[MN_WISPER_LIST].nField[CHAT_TARGET_WISPER1].temp);
		}
		if(activeWhisper != CHAT_TARGET_WISPER2)
		{
			if(activeWhisper != CHAT_TARGET_WISPER1)
				strcpy(SMenu[MN_WISPER_LIST].nField[CHAT_TARGET_WISPER2].temp,SMenu[MN_WISPER_LIST].nField[CHAT_TARGET_WISPER1].temp);
			else
			{
				strcpy(SMenu[MN_WISPER_LIST].nField[CHAT_TARGET_WISPER2].temp,name);
				i = CHAT_TARGET_WISPER2;
			}
		}
		if(activeWhisper != CHAT_TARGET_WISPER1)
		{
			strcpy(SMenu[MN_WISPER_LIST].nField[CHAT_TARGET_WISPER1].temp,name);
			i = CHAT_TARGET_WISPER1;
		}
	}
	return i;
}

//////////////////////////////////////////////////////////////////////
//1.3  
/*
int CChatMgr::CompareString(string szLef, string szRig)//kyo ߿ checkLimitedBBSNameĥ Ѵ.
{
	return szLef.compare( szRig);
}

int CompareString(string szLef, string szRig)//kyo ߿ checkLimitedBBSNameĥ Ѵ.
{
	return szLef.compare( szRig);
}
*/
void CChatMgr::ClearLimiteBBS()
{
	m_bIsActiveBBS = true;	
	m_szLimiteName.clear();
}

bool CChatMgr::AddLimintedBBSName(std::string text)
{
  if (text == "" || text.empty())
  {
    return false;
  }

  // Trim leading/trailing whitespace
  text.erase(0, text.find_first_not_of(" \t\n\r"));
  text.erase(text.find_last_not_of(" \t\n\r") + 1);

  if (m_szLimiteName.size() >= MAX_LIMIT_USER)
  {
    return false;
  }

  for (const auto& name : m_szLimiteName)
  {
    if (name == text)
    {
      return false; // Already exists
    }
  }

  m_szLimiteName.push_back(text);
  return true;
}

bool CChatMgr::AddLimintedBBSNameOne( const string szName)
{
	if( "" == szName)
	{
		return false;
	}
	if( m_szLimiteName.size() >= MAX_LIMIT_USER )
	{
		return false;
	}
	
	vector<string>::iterator it;
	for( it = m_szLimiteName.begin(); it != m_szLimiteName.end(); ++it)
	{
		if( (*it) == szName )
		{
			return false;
		}
	}
	m_szLimiteName.push_back( szName );
	return true;
}

bool CChatMgr::IsLimitedBBSName( const string szName)
{
	if( !IsActiveBBS() )
	{
		return true;
	}
	if( 0 == m_szLimiteName.size() )
	{
		return true;
	}

	vector<string>::iterator it;
	for( it = m_szLimiteName.begin(); it != m_szLimiteName.end(); ++it)
	{
		if( (*it) == szName )
		{
			return true;
		}
	}
	return false;
}

void CChatMgr::ShowLimitedList()
{
	//::AddCurrentStatusMessage(255,255,0, "BBS Limite List");

	vector<string>::iterator it;
	for( it = m_szLimiteName.begin(); it != m_szLimiteName.end(); ++it)
	{
		char szMsg[300] = { 0 };
		memcpy(szMsg, (*it).c_str(), (*it).length());
		AddCurrentStatusMessage(255,255,0, szMsg);
	}
}

void CChatMgr::DisplayContextMenu() {
	if (!m_bIsContextMenuOpen) {
		return;
	}

	// --- 1. 計算選單尺寸 ---
	int max_width = TXT_BG_SHORT_WIDTH;  // 最小寬度
	int item_height = TxtOut.GetLineGab();            // 每個選項的高度
	TTF_Font* font = g_SDLInfo.gFont;  // 使用普通字型來測量

	// 背景的總高度
	int menu_height = m_contextMenuItems.size() * item_height;

	// 更新選單的矩形區域，用於後續的點擊判斷
	m_menuRect = { m_iContextMenuX, m_iContextMenuY, max_width, menu_height };
	// --- 2. 繪製選單背景 ---
	// 我們使用 CTextOutMgr 來繪製一個類似工具提示的背景
	TxtOut.Init(COLOR_BLACK, TXT_BG_SHORT_WIDTH);  // 使用標準的黑色背景
	TxtOut.PutTextHeaderBGImg(m_iContextMenuX, m_iContextMenuY, max_width);

	// --- 3. 繪製選單選項 ---
	int current_y = m_iContextMenuY;
	for (size_t i = 0; i < m_contextMenuItems.size(); ++i) {
		// 如果滑鼠滑過，就用高亮顏色
		if ((int)i == m_iMenuHoverIndex) {
			Hcolor(FONT_COLOR_SOLID_YELLOW);
		}
		else {
			Hcolor(FONT_COLOR_WHITE);
		}
		char* MenuStr = const_cast<char*>(m_contextMenuItems[i].text.c_str());
		TxtOut.RcTXTOut(m_iContextMenuX, current_y, TXT_BG_SHORT_WIDTH, 1, MenuStr);
		current_y += item_height;
	}

	TxtOut.PutTextTailBGImg(m_iContextMenuX, current_y, max_width);
}

void CChatMgr::CheckContextMenuMouseOver(int mouseX, int mouseY) {
	if (!m_bIsContextMenuOpen) {
		return;
	}

	m_iMenuHoverIndex = -1;  // 預設為沒有任何項目被滑過

	// 檢查滑鼠是否在整個選單的大矩形內
	if (mouseX >= m_menuRect.x && mouseX <= m_menuRect.x + m_menuRect.w &&
		mouseY >= m_menuRect.y && mouseY <= m_menuRect.y + m_menuRect.h) {
		int item_height = TxtOut.GetLineGab();
		int relative_y = mouseY - (m_menuRect.y);  // 計算相對於第一個選項文字的 Y 座標

		if (relative_y > 0) {
			int index = relative_y / item_height;
			if (index >= 0 && index < (int)m_contextMenuItems.size()) {
				m_iMenuHoverIndex = index;
				SDL_Log("Hovering over Chat menu item index: %d", m_iMenuHoverIndex);
			}
		}
	}
}


void CChatMgr::HandleContextMenuMouseClick(int mouseX, int mouseY) {
	if (!m_bIsContextMenuOpen) {
		return;
	}

	// 如果點擊位置在選單之外，或者沒有選中任何項目，則關閉選單
	if (m_iMenuHoverIndex == -1) {
		CloseContextMenu();
		return;
	}

	// 執行對應的回呼函式
	const auto& item = m_contextMenuItems[m_iMenuHoverIndex];
	if (item.action) {
		item.action(m_pSelectedChatData);
	}

	// 執行完操作後關閉選單
	CloseContextMenu();
}

CChatData* CChatMgr::CheckNameClick(int mouseX, int mouseY)
{
	// 遍歷所有可見的聊天訊息
	for (auto const& pData : ChatList) {
		if (pData /* && pData->IsDisplayAble()*/)
		{
			// 使用 SDL 內建的函式來判斷一個點是否在矩形內
			SDL_Point point = { mouseX, mouseY };
			if (SDL_PointInRect(&point, &pData->m_nameRect))
			{
				return pData;  // 找到了！返回被點擊的聊天資料指標
			}
		}
	}
	return nullptr;  // 沒有點擊到任何名字
}
