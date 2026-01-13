#ifndef ROUTE_SELECTOR_H
#define ROUTE_SELECTOR_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include <string>

class RouteSelector {
public:
    RouteSelector();
    ~RouteSelector();
    // 初始化：設定按鈕位置與大小
    void Init(int x, int y, int w, int h, int* route);


    // 新增線路名稱 (例如: "線路 A", "線路 B")
    void AddRouteName(const std::string& name);

    void ClearPingProbe();

    void StartPingProbe();

    void EndPingProbe();

    std::string GetPingString();

    // 渲染按鈕 (需要傳入字型)
    void Render(TTF_Font* font);

    // 處理點擊事件 (回傳 true 代表點到了按鈕)
    bool HandleInput(int mouseX, int mouseY);

    void SetStartMenu(bool m_StartMenu) { m_bStartMenu = m_StartMenu; }
private:
    SDL_Rect m_Rect{};                    // 按鈕的矩形範圍
    std::vector<std::string> m_RouteNames; // 儲存所有線路名稱
    
    // 樣式設定 (可依需求改為公開變數)
    SDL_Color m_TextColor{};
    SDL_Color m_BgColor{};
    SDL_Color m_BorderColor{};

	int* routeSelectedIndex=nullptr; // 被選中的線路索引
    bool m_bStartMenu = true;
};

// 宣告一個全域實例，方便在任何地方存取 (這行看你架構，通常方便移植舊遊戲)
extern RouteSelector g_RouteSelector;

#endif // ROUTE_SELECTOR_H