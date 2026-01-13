#include "Stdafx.h"
#include "CharacterContxtMenu.h"
#include "CurrentMsgMgr.h"
#include "Chat.h"
#include "TextOutMgr.h"
#include "SDLRenderer.h"
#include "Hangul.h"
#include "dragon.h"


CharacterContxtMenu CharContxtMenu;

void MenuAction_CopyName(LPCHARACTER pData) {
  SDL_Log("Copy Name: %s", pData->name);
  if (pData && pData->name[0] != '\0') {
      SDL_SetClipboardText(pData->name);
      AddCurrentStatusMessage(255, 255, 0, "Name Copy : %s", pData->name);
  }
}

void MenuAction_AddFriend(LPCHARACTER pData) {
  SDL_Log("Add Friend: %s", pData->name);
  if (pData && pData->name[0] != '\0') {
      if (strcmp(Hero->name, pData->name) == 0) return;
      RegistFriend(pData->name);
      AddCurrentStatusMessage(255, 255, 0, "Added %s ", pData->name);
  }
}

void MenuAction_BlockUser(LPCHARACTER pData) {
  SDL_Log("Block User: %s", pData->name);
  if (pData && pData->name[0] != '\0') {
      // TODO: Implement Block User logic
      ChatMgr.SetRefuseWhiper(pData->name);
      //AddCurrentStatusMessage(255, 0, 0, "TODO: Blocked %s", pData->GetName());
  }
}

void MenuAction_Party(LPCHARACTER pData) { 
  if (pData) {
    std::string name(pData->name);
    LC_SendInputParty(name);
  }
}

void MenuAction_makeTrade(LPCHARACTER ch) { 

    if(ch)
        SendExchangeItemWithOtherCh(ch->id, 1); //1 =EXCHANGE_TYPE_NORMAL;
}
void MenuAction_makeDual(LPCHARACTER ch) {
    SendReqManToMan(ch->name);
}

CharacterContxtMenu::CharacterContxtMenu() { 

}

CharacterContxtMenu::~CharacterContxtMenu() {

}


void CharacterContxtMenu::InitContextMenu(int lang) {
  if (lang == 0) {
      m_contextMenuItems.push_back({ u8"Copy Name", &MenuAction_CopyName });
      m_contextMenuItems.push_back({ u8"Add Friend", &MenuAction_AddFriend });
      m_contextMenuItems.push_back({ u8"Block User", &MenuAction_BlockUser });
      m_contextMenuItems.push_back({ u8"Party Up", &MenuAction_Party });
      m_contextMenuItems.push_back({ u8"Trade", &MenuAction_makeTrade });
      m_contextMenuItems.push_back({ u8"Duel", &MenuAction_makeDual });
  }
  else {
      m_contextMenuItems.push_back({ u8"複製名字", &MenuAction_CopyName });
      m_contextMenuItems.push_back({ u8"加為好友", &MenuAction_AddFriend });
      m_contextMenuItems.push_back({ u8"屏蔽此人", &MenuAction_BlockUser });
      m_contextMenuItems.push_back({ u8"進行組隊", &MenuAction_Party });
      m_contextMenuItems.push_back({ u8"進行交易", &MenuAction_makeTrade });
      m_contextMenuItems.push_back({ u8"進行決鬥", &MenuAction_makeDual });
  }
}

void CharacterContxtMenu::OpenContextMenu(LPCHARACTER ch, int x, int y) {
  if(!ch) return;
  m_pSelectedCharacter = ch;
  m_bIsContextMenuOpen = true;
  m_iContextMenuX = x;
  m_iContextMenuY = y;
  m_iMenuHoverIndex = -1;  // Nothing is hovered initially
}

void CharacterContxtMenu::CloseContextMenu() {
  m_bIsContextMenuOpen = false;
  m_pSelectedCharacter = nullptr;
}
void CharacterContxtMenu::DisplayContextMenu() {
  if (!m_bIsContextMenuOpen) {
    return;
  }

  // --- 1. 計算選單尺寸 ---
  int max_width = TXT_BG_SHORT_WIDTH;     // 最小寬度
  int item_height = TxtOut.GetLineGab();  // 每個選項的高度

  // 背景的總高度
  int menu_height = m_contextMenuItems.size() * item_height;

  // 更新選單的矩形區域，用於後續的點擊判斷
  m_menuRect = {m_iContextMenuX, m_iContextMenuY, max_width, menu_height};
  // --- 2. 繪製選單背景 ---
  // 我們使用 CTextOutMgr 來繪製一個類似工具提示的背景
  TxtOut.Init(COLOR_BLACK, TXT_BG_SHORT_WIDTH);  // 使用標準的黑色背景
  TxtOut.PutTextHeaderBGImg(m_iContextMenuX, m_iContextMenuY, max_width);

  // --- 3. 繪製選單選項 ---
  int current_y = m_iContextMenuY;
  for (size_t i = 0; i < m_contextMenuItems.size(); ++i) {
    // 如果滑鼠滑過，就用高亮顏色
    if ((int)i == m_iMenuHoverIndex) {
      Hcolor(255,255,0);
    } else {
      Hcolor(233,233,233);
    }
    char *MenuStr = const_cast<char *>(m_contextMenuItems[i].text.c_str());
    TxtOut.RcTXTOut(m_iContextMenuX, current_y, TXT_BG_SHORT_WIDTH, 1, MenuStr);
    current_y += item_height;
  }

  TxtOut.PutTextTailBGImg(m_iContextMenuX, current_y, max_width);
}
void CharacterContxtMenu::HandleContextMenuMouseClick(int mouseX, int mouseY) {
  if (!m_bIsContextMenuOpen) {
    return;
  }

  // 如果點擊位置在選單之外，或者沒有選中任何項目，則關閉選單
  if (m_iMenuHoverIndex == -1) {
    CloseContextMenu();
    return;
  }

  // 執行對應的回呼函式
  const auto &item = m_contextMenuItems[m_iMenuHoverIndex];
  if (item.action) {
    item.action(m_pSelectedCharacter);
  }

  // 執行完操作後關閉選單
  CloseContextMenu();
}

void CharacterContxtMenu::CheckContextMenuMouseOver(int mouseX, int mouseY) {
  if (!m_bIsContextMenuOpen) {
    return;
  }

  m_iMenuHoverIndex = -1;  // 預設為沒有任何項目被滑過

  // 檢查滑鼠是否在整個選單的大矩形內
  if (mouseX >= m_menuRect.x && mouseX <= m_menuRect.x + m_menuRect.w &&
      mouseY >= m_menuRect.y && mouseY <= m_menuRect.y + m_menuRect.h) {
    int item_height = TxtOut.GetLineGab();
    int relative_y =
        mouseY - (m_menuRect.y);  // 計算相對於第一個選項文字的 Y 座標

    if (relative_y > 0) {
      int index = relative_y / item_height;
      if (index >= 0 && index < (int)m_contextMenuItems.size()) {
        m_iMenuHoverIndex = index;
        SDL_Log("Hovering over Character menu item index: %d", m_iMenuHoverIndex);
      }
    }
  }
}

