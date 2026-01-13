#include "RouteSelector.h"
#include <algorithm>
#include "SDLRenderer.h"
#include "SDLPingProbe.h"
extern void		WriteInfoFile(void);
extern BOOL LoadGameInfo(void);
// 定義全域實例
#define ROUTE_COUNT 3
RouteSelector g_RouteSelector;
SDLPingProbe* pingGameServer[ROUTE_COUNT] = { 0, };
RouteSelector::RouteSelector() {
    m_Rect = { 0, 0, 0, 0 };
    // 預設樣式
    m_TextColor = { 255, 255, 0, 255 };   // 黃色文字
    m_BgColor = { 0, 0, 0, 130 };         // 半透明黑色背景
    m_BorderColor = { 200, 200, 200, 255 }; // 銀色邊框
}

RouteSelector::~RouteSelector() {
    m_RouteNames.clear();
}

void RouteSelector::Init(int x, int y, int w, int h, int* route) {

    // 【修改重點】如果已經有實體了，就不要再刪除重創，直接 return
    if (pingGameServer[0] != nullptr && pingGameServer[1] != nullptr) {
        SDL_Log("[RouteSelector] Ping probe already running. Skipping re-init.");
        return;
    }

    m_Rect = { x, y, w, h };

    routeSelectedIndex = route;
    // 初始化預設線路名稱 (如果還沒加過)
    if (m_RouteNames.empty()) {
        m_RouteNames.push_back("HKTW (Default)");
        m_RouteNames.push_back("HKCN (CLAWCLOUD)");
    }
    StartPingProbe();
}

void RouteSelector::AddRouteName(const std::string& name) {
    m_RouteNames.push_back(name);
}

void RouteSelector::ClearPingProbe() {

	for (int i = 0; i < ROUTE_COUNT; i++) {
        if (pingGameServer[i]) {
            pingGameServer[i]->Stop();
            delete pingGameServer[i];
            pingGameServer[i] = nullptr;
        }
    }
}

void RouteSelector::StartPingProbe() {
    ClearPingProbe();
    pingGameServer[0] = new SDLPingProbe();
    // 假設伺服器 IP 是 192.168.1.100，UDP Ping Port 是 12345
    if (pingGameServer[0]->Start("116.241.108.100", 12345)) {
		SDL_Log("[HKTW Default] Ping probe started.");
    }
    pingGameServer[1] = new SDLPingProbe();
    // 假設伺服器 IP 是 192.168.1.100，UDP Ping Port 是 12345
    if (pingGameServer[1]->Start("47.74.13.199", 12345)) {
        SDL_Log("[RouteSelector HK] Ping probe started.");
    }
}

void RouteSelector::EndPingProbe() {
	ClearPingProbe();
}

std::string RouteSelector::GetPingString() {

	// 1. 確保 route 索引安全 (防崩潰)
    if (*routeSelectedIndex < 0 || *routeSelectedIndex >= (int)m_RouteNames.size()) {
        *routeSelectedIndex = 0;
    }
    int gamePing = pingGameServer[*routeSelectedIndex]->GetPing();
    return std::string(m_RouteNames[*routeSelectedIndex] + " " + std::to_string(gamePing) + "ms");
}
void RouteSelector::Render(TTF_Font* font) {
    if (!font || !g_SDLInfo.renderer) return;

    // 1. 確保 route 索引安全 (防崩潰)
    if (m_RouteNames.empty()) return;


    // 如果 system_info.route 超出範圍，重置為 0
    if (*routeSelectedIndex < 0 || *routeSelectedIndex >= (int)m_RouteNames.size()) {
        *routeSelectedIndex = 0;
    }
    //int gamePing = pingGameServer[*routeSelectedIndex]->GetPing();
	SDL_FillRect(g_SDLInfo.cpuRenderSurface, &m_Rect, SDL_MapRGBA(g_SDLInfo.cpuRenderSurface->format, 0, 0, 0, 125)); // 清空區域
    
    // 2. 畫背景
    /*SDL_SetRenderDrawBlendMode(g_SDLInfo.renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(g_SDLInfo.renderer, m_BgColor.r, m_BgColor.g, m_BgColor.b, m_BgColor.a);
    SDL_RenderFillRect(g_SDLInfo.renderer, &m_Rect);

    // 3. 畫邊框
    SDL_SetRenderDrawColor(g_SDLInfo.renderer, m_BorderColor.r, m_BorderColor.g, m_BorderColor.b, m_BorderColor.a);
    SDL_RenderDrawRect(g_SDLInfo.renderer, &m_Rect);*/

    // 4. 畫文字 (使用你寫好的 RenderTextWithFallback)
    // 計算簡單的 Padding，讓文字不要貼邊
    int textX = m_Rect.x + 8;
    int textY = m_Rect.y + 6; // 垂直置中
    
    std::string  pingText = GetPingString();
    // 呼叫你的全域渲染函式
    RenderTextWithFallback(
        textX, 
        textY, 
        pingText.c_str(),
        m_TextColor, 
        font, 
        g_SDLInfo.gEmojiFont // 記得傳入 Emoji 字型
    );
}

bool RouteSelector::HandleInput(int mouseX, int mouseY) {

    if (!m_bStartMenu) return false;
    // 1. 檢查是否點擊在矩形內
    if (mouseX >= m_Rect.x && mouseX <= m_Rect.x + m_Rect.w &&
        mouseY >= m_Rect.y && mouseY <= m_Rect.y + m_Rect.h) 
    {
        if (m_RouteNames.empty()) return false;

        // 2. 切換線路 (循環 +1)
        (*routeSelectedIndex)++;
        if (*routeSelectedIndex >= (int)m_RouteNames.size()) {
            *routeSelectedIndex = 0;
        }

		LoadGameInfo(); // 重新載入設定
        // 3. ★ 立即存檔！★
        // 確保玩家下次重開 APP 時會記住這個設定
        WriteInfoFile();

        SDL_Log("[RouteSelector] Switched to: %s (%d)", m_RouteNames[*routeSelectedIndex].c_str(), *routeSelectedIndex);
        
        return true; // 事件已處理
    }
    return false; // 沒點到
}