#pragma once
#include <vector>
#include <string>
#include <SDL_rect.h> // For SDL_Rect

// Define the callback function pointer type for menu actions
using CharContxtMenuActionCallback = void (*)(LPCHARACTER);

// Define the structure for a single context menu item
struct CharContxtMenuItem {
  std::string text;
  CharContxtMenuActionCallback action;
};

class CharacterContxtMenu {
 public:
   CharacterContxtMenu();
  ~CharacterContxtMenu();

 public:
  void InitContextMenu(int lang);
  void OpenContextMenu(LPCHARACTER ch, int x, int y);
  void CloseContextMenu();
  void DisplayContextMenu();
  void HandleContextMenuMouseClick(int mouseX, int mouseY);
  bool IsContextMenuOpen() const { return m_bIsContextMenuOpen; }
  //CChatData* CheckNameClick(int mouseX, int mouseY);
  void CheckContextMenuMouseOver(int mouseX, int mouseY);

 private: 


  int x, y;
  bool m_bIsContextMenuOpen;
  int m_iContextMenuX, m_iContextMenuY;
  //CChatData* m_pSelectedChatData;
  int m_iMenuHoverIndex;
  std::vector<CharContxtMenuItem> m_contextMenuItems;
  SDL_Rect m_menuRect;
  LPCHARACTER m_pSelectedCharacter;
};
extern CharacterContxtMenu CharContxtMenu;

extern void LC_SendInputParty(std::string &name);
extern void SendReqManToMan(const char *name);
extern void RegistFriend(const char* szName);
extern void SendExchangeItemWithOtherCh(short int you_id, const int iExchangeType);