#include "stdafx.h"
#include <SDL_net.h>
#include "SDLRenderer.h"
#include "EditWndMgr.h"
#include "Hong_Sprite.h"
#include "CRenderMethod.h"
#include "Chat.h"
#include "dragon.h"
#include "Hangul.h"
#include "ParticleEffectManager.h"
#include "ParticleSystem.h"
#include "CharacterContxtMenu.h"
#include "utf8.h"
#include "RouteSelector.h"
#include "MobileInputManager.h"
#include "CimGui.h"
const char* font_path_chs = "ttf/SourceHanSansSC-Regular.otf";
const char* font_path_cht = "ttf/lazy.ttf";
char font_path[260] = { 0 };
#ifdef _TEXTURE
#define _LOADSPRITE
#endif
// 這是我們用來追蹤鍵盤狀態的陣列 (類似 DirectInput 的鍵盤緩衝區)
// 方便你在每個遊戲迴圈中查詢某個鍵是否被按下
const Uint8* keyboardState = nullptr;

// 內部鍵盤狀態追蹤：判斷一個鍵是否是「第一次按下」
bool keysJustPressed[SDL_NUM_SCANCODES] = { false }; // 追蹤每個掃描碼的狀態
std::vector<InputBox> inputBoxes; // 全局的 InputBox 實例列表

// 4. 定義你的文字快取 map
//    使用 unique_ptr 配合自定義刪除器，自動管理 SDL_Surface 的生命週期
std::map<TextRenderKey, std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)>> g_textSurfaceCache;
std::map<TextRenderKey, std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>> g_textTextureCache;
// 全域的紋理快取，用 Spr 指標來快速查找其紋理資訊
// 鍵(Key)是原始的 Spr 指標，值(Value)是它對應的紋理資訊
std::unordered_map<const Spr*, TextureInfo> g_TextureCache;
// 檔案名稱 -> 指向該檔案紋理圖集的指標
// 這個 map 是我們的「總管」，負責紋理的生命週期。
std::unordered_map<std::string, SDL_Texture*> g_AtlasManager;

// 這個 map 用來儲存每個臨時物件的 Spr 結構本身。
// Key 是物件的 ID，Value 是 Spr 結構。
std::unordered_map<int, Spr> g_TempObjectSprCache;
// 這個 map 用來管理為每個 Spr 分配的圖像資料記憶體。
// 使用 std::unique_ptr 可以確保在 map 刪除條目時，記憶體會被自動釋放。
std::unordered_map<int, std::unique_ptr<char[]>> g_TempObjectImageBufferCache;
// 追蹤當前活躍的輸入框
InputBox* activeInputBox = nullptr; // 全局的活躍輸入框指針
SDL_DirectDrawInfo g_SDLInfo;
bool isWindowMinimized = false;

// 計算大於等於 n 的最小的 2 的次方值
int NextPowerOfTwo(int n) {
    if (n == 0) return 0;
    int power = 1;
    while (power < n) {
        power *= 2;
    }
    return power;
}
// 構造函數
SDL_DirectDrawInfo::SDL_DirectDrawInfo()
    : bFullscreen(false),gFont(nullptr), window(nullptr), renderer(nullptr), cpuRenderSurface(nullptr),
    updateTimer(false), g_BoldFont(nullptr), texture_format(nullptr),
    bRenderPc(false){
    // 構造時初始化為 nullptr，實際資源分配在 InitSDLGraphics 中
    limit_time = SDL_GetTicks();
    day_of_week = 0; SaveScreen = "";
    serverTime = "";
    bOutputSmallMap = false;
    streamingTexture = nullptr;
    gEmojiFont = nullptr;
    memset(&originalDesktopMode, 0, sizeof(originalDesktopMode));
    memset(&keyboardEventThisframe, 0, sizeof(keyboardEventThisframe));
    memset(param, 0, sizeof(param));
    LoadMenuIndex();
}

// 析構函數
SDL_DirectDrawInfo::~SDL_DirectDrawInfo() {
    CleanupSDLGraphics(); // 確保資源被釋放
}


// 清理函數
void SDL_DirectDrawInfo::CleanupSDLGraphics() {

    CleanupAllAtlases();
    g_GlyphWidthCache.clear();
    g_GlyphProvidedCache.clear();

    AsyncTextManager::Instance().Quit();
    if (gFont) {
        TTF_CloseFont(gFont);
        gFont = nullptr;
    }

    if (g_BoldFont) {
        TTF_CloseFont(g_BoldFont);
        g_BoldFont = nullptr;
    }

    if (gEmojiFont) {
        TTF_CloseFont(gEmojiFont);
        gEmojiFont = nullptr;
    }
    if (g_MainFontBuffer) { free(g_MainFontBuffer); g_MainFontBuffer = nullptr; }
    if (g_EmojiFontBuffer) { free(g_EmojiFontBuffer); g_EmojiFontBuffer = nullptr; }


    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    
    if (cpuRenderSurface) { // 釋放 CPU 渲染目標 Surface
        SDL_FreeSurface(cpuRenderSurface);
        cpuRenderSurface = nullptr;
    }

    if(streamingTexture) { // 釋放串流紋理
        SDL_DestroyTexture(streamingTexture);
        streamingTexture = nullptr;
	}   

    //SDL_SetWindowDisplayMode(window, &originalDesktopMode);
        
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    g_textSurfaceCache.clear(); g_textTextureCache.clear();
    g_SpriteCache.clear();
    g_audioManager.Shutdown();
    // 在遊戲結束的 Shutdown() 函式中...
    g_fireflySystem.Cleanup();
    SDLNet_Quit();
    SDL_Quit(); // 最後退出 SDL 子系統
    SDL_Log("INFO: SDL_DirectDrawInfo cleaned up and SDL quit."); // 新增日誌
}

char* SDL_DirectDrawInfo::setBackbuffersurface(char*& pixels, DWORD& dxSize, WORD& wDxsize)
{
#ifdef _TEXTURE
  // 4. 鎖定紋理並呼叫【帶裁剪功能】的 C++ 函式
  /*void* pix;
  int pitch;
  if (SDL_LockTexture(streamingTexture, NULL, &pix, &pitch) != 0)
  {
      return pixels;
  }
  pixels = (char*) pix;
  dxSize = wDxsize = pitch;

  memset(pixels, 0, pitch * SCREEN_HEIGHT);  // 清空紋理數據
  // 3. 取得這個紋理的像素格式
  // 你需要這個資訊來正確地使用 SDL_MapRGBA
  Uint32 format_enum;
  int access;
  int w, h;
  if (!texture_format) {
      // 用來儲存紋理的像素格式
      SDL_QueryTexture(streamingTexture, &format_enum, &access, &w, &h);
      texture_format = SDL_AllocFormat(format_enum); // 從枚舉值建立 PixelFormat 結構
  }
  if (!texture_format) {
    SDL_Log("Failed to allocate pixel format: %s", SDL_GetError());
  }*/
#else
    pixels = (char*)cpuRenderSurface->pixels;
    //dxSize = wDxsize = SCREEN_WIDTH * cpuRenderSurface->format->BytesPerPixel;
    dxSize = wDxsize = cpuRenderSurface->pitch;
#endif
    return pixels;
}

// SDL_GetWindowHandle (保持不變)
HWND SDL_DirectDrawInfo::SDL_GetWindowHandle() {
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(window, &wmInfo);
    // 注意：原始代碼是 wmInfo.info.win.window，這可能不是 HWND 類型。
    // 在 Windows 上，正確的是 wmInfo.info.win.hwnd
    return wmInfo.info.android.window;
}

// SDL_EraseScreen (清空 CPU 渲染目標 Surface)
void SDL_DirectDrawInfo::SDL_EraseScreen() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
#ifdef _TEXTURE
    // a. 設定清空顏色為黑色 (R=0, G=0, B=0)
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);

    // b. 清空整個渲染目標
    SDL_RenderClear(renderer);
#else

    if (cpuRenderSurface) {
        // 使用黑色填充整個 Surface
        SDL_FillRect(cpuRenderSurface, NULL, SDL_MapRGB(cpuRenderSurface->format, 0, 0, 0));
    }
    SDL_RenderClear(renderer);
#endif
}

void SDL_DirectDrawInfo::SDL_drawText(int x, int y, std::string str)
{
    SDL_Color c = { 255, 255, 0, 255 };
    SDL_Hprint(x, y, str.c_str(), c, HangulAlign);
}

void SDL_DirectDrawInfo::InitAudioManager()
{
    g_audioManager.Init();
}
void SDL_DirectDrawInfo::SDL_InitInputBox()
{
    // 創建至少四個輸入框實例
    //inputBoxes.emplace_back(100, 100, 400, 40); // 輸入框 1
    //inputBoxes.emplace_back(100, 150, 400, 40); // 輸入框 2
    //inputBoxes.emplace_back(100, 200, 400, 40); // 輸入框 3
    //inputBoxes.emplace_back(100, 250, 400, 41, true); // 輸入框 4
    // 你可以添加更多，例如：
    // inputBoxes.emplace_back(100, 300, 400, 40); // 輸入框 5

    for (int i = 0; i < HWND_GAME_MAIN; i++)
    {
        int y = TTF_FontHeight(g_SDLInfo.gFont) + 1;
        inputBoxes.emplace_back(100, y + i * y + y, 197, y, i);

    }

    /*InputBox* box = FindInputBox(BOXID_EMAIL_BODY);
    if (box)
    {
        box->isMultiLine = true;
        box->h = 10 * box->h;
        box->isVisible = true;
    }*/
}

// SDL_Draw (所有 CPU 端繪圖指令的入口，例如調用 PutCompressedImage, Line 等)
// 這個函數應該是遊戲中所有軟體繪圖操作的彙總點
void SDL_DirectDrawInfo::SDL_DrawInputbox() {

    for (auto& box : inputBoxes) {
        renderInputBox(box, gFont);
    }

}

void SDL_DirectDrawInfo::aura_effect_Emit(LPCHARACTER ch)
{
    if (ch)
    {
        //npc 不顯示
        if (ch->IsNpc() || ch->viewtype == ViewTypes::VIEWTYPE_GHOST_ ||
            ch->viewtype == ViewTypes::VIEWTYPE_TRANSPARENCY_) return;
        if (Hero->HorseNo) return;

        if (auto aura = g_particle_manager.GetAura(ch)) {
            // 1. 取得角色當前動畫影格對應的 Spr 指標
            int dir = ch->direction;
            int sprc = ch->AnimationTable[ch->nCurrentAction].cFrame[ch->nCurrentFrame.GetDecrypted()];
            int sprno = ch->sprno;
            Spr* cloth = &CharAccessorySpr[sprno][ch->accessory[0]].sp[dir][sprc]; //衣服的 Spr
            Spr* weapon = &CharAccessorySpr[sprno][ch->accessory[2]].sp[dir][sprc]; //武器的 Spr
            Spr* head = &CharAccessorySpr[sprno][ch->accessory[1]].sp[dir][sprc]; //武器的 Spr

            // 2. 在我們的快取中，查找這個 Spr 的輪廓點
            if (cloth->size != 0 && g_outlineCache.count(cloth))
            {
                const std::vector<SDL_Point>& outline = g_outlineCache.at(cloth);

                // 3. 如果找到了輪廓，就呼叫 Emit
                if (!outline.empty())
                {
                    // 為了讓輪廓看起來飽滿，可以一次發射多個粒子
                    int particlesToEmit = 1;
                    for (int j = 0; j < particlesToEmit; ++j) {
                        aura->Emit(ch, outline);
                    }
                }
            }

            if (weapon->size != 0 && g_outlineCache.count(weapon) && ch->peacests)
            {
                const std::vector<SDL_Point>& outline = g_outlineCache.at(weapon);

                // 3. 如果找到了輪廓，就呼叫 Emit
                if (!outline.empty())
                {
                    // 為了讓輪廓看起來飽滿，可以一次發射多個粒子
                    int particlesToEmit = 1;
                    for (int j = 0; j < particlesToEmit; ++j) {
                        aura->Emit(ch, outline);
                    }
                }
            }

            if (head->size != 0 && g_outlineCache.count(head))
            {
                const std::vector<SDL_Point>& outline = g_outlineCache.at(head);

                // 3. 如果找到了輪廓，就呼叫 Emit
                if (!outline.empty())
                {
                    // 為了讓輪廓看起來飽滿，可以一次發射多個粒子
                    int particlesToEmit = 1;
                    for (int j = 0; j < particlesToEmit; ++j) {
                        aura->Emit(ch, outline);
                    }
                }
            }
        }
    }
}

void SDL_DirectDrawInfo::particle_effect_Render(LPCHARACTER ch)
{
	if (ch->IsNpc()) return; // npc 不顯示
    g_particle_manager.RenderAll(cpuRenderSurface, ch, Mapx, Mapy);
    particle_effect_Emit(ch);
}

void SDL_DirectDrawInfo::particle_effect_Emit(LPCHARACTER ch)
{
	aura_effect_Emit(ch);

    const int padding_x = 0;
    const int padding_y = 11;
    if (!ch->IsRunning() || ch->HorseNo<=0 ) return;// npc / 或不是跑步中不顯示


    if (ch->IsNpc() || ch->viewtype == ViewTypes::VIEWTYPE_GHOST_ ||
        ch->viewtype == ViewTypes::VIEWTYPE_TRANSPARENCY_) return;

    if (auto sparks = g_particle_manager.GetSparks(ch)) {
        sparks->Emit(ch, padding_x, padding_y);
    }
}

void SDL_DirectDrawInfo::FireFlySystemRender()
{
    if (DayLightControl >= 30) return;

    // 3. 繪製螢火蟲，傳入地圖的偏移量
    int map_offset_x = Mapx;
    int map_offset_y = Mapy;
    g_fireflySystem.Render(cpuRenderSurface, map_offset_x, map_offset_y);
}

void SDL_DirectDrawInfo::AllParticelSystemUpdate(float deltaTime)
{
	if (Hero == nullptr) return;

    g_particle_manager.UpdateAll(deltaTime); // 更新粒子系統

    // 1. 計算出鏡頭中心點的「世界座標」
    int camera_center_world_x = Mapx + (SCREEN_WIDTH / 2);
    int camera_center_world_y = Mapy + (SCREEN_HEIGHT / 2);
    // 2. 更新螢火蟲系統，傳入鏡頭的世界座標
    g_fireflySystem.Update(deltaTime, camera_center_world_x, camera_center_world_y);
}

void SDL_DirectDrawInfo::RenderAllDebugShapes()
{
    // 遍歷「方框訂單列表」中的每一張訂單
    for (const auto& order : g_linesToDrawThisFrame)
    {
        // 1. 顏色轉換：將您舊有的 16-bit 顏色值，轉換為 SDL_Renderer 能理解的 R,G,B,A
        Uint8 r, g, b;
        const int& color = FONT_COLOR_SOLID_YELLOW;
        // 假設您的 color 是 16-bit RGB565 格式
        r = (color & 0xF800) >> 8;
        g = (color & 0x07E0) >> 3;
        b = (color & 0x001F) << 3;
        SDL_SetRenderDrawColor(renderer, r, g, b, 255); // 設定繪圖顏色為黃色

        // 2. 準備 SDL_Rect
        //    注意：SDL_Rect 需要 x, y, w, h，而您的 RECT 是 left, top, right, bottom
        SDL_Rect sdl_rect;
        sdl_rect.x = order.rect.left;
        sdl_rect.y = order.rect.top;
        sdl_rect.w = order.rect.right - order.rect.left;
        sdl_rect.h = order.rect.bottom - order.rect.top;

        // 3. 【關鍵優化】呼叫 SDL 內建的、由 GPU 執行的繪圖函式
        SDL_RenderDrawRect(renderer, &sdl_rect);
    }
	
}

void SDL_DirectDrawInfo::LoadPNGImage(std::string filename) {
  // 建立一個新的 ImageTexture 物件，並由 unique_ptr 管理
  auto texture = std::make_unique<ImageTexture>(renderer);

  // 嘗試載入圖片
  if (texture->loadFromFile(filename)) {
    // 如果載入成功，將這個物件存入快取
    g_PngCache[filename] = std::move(texture);
  } else {
    // 如果載入失敗，unique_ptr 會在函式結束時自動銷毀，
    // 不會造成記憶體洩漏
    SDL_Log("Failed to load PNG image: %s", filename.c_str());
  }
}

void SDL_DirectDrawInfo::printCachedImages() {
  SDL_Log("--- Currently PNG cached images ---");
  // 使用範圍式 for 迴圈遍歷快取
  for (const auto& pair : g_PngCache) {
    // pair.first 就是 string 鍵值
    SDL_Log("  - %s", pair.first.c_str());
  }
  SDL_Log("-----------------------------------");
}

void SDL_DirectDrawInfo::OnWindowResized(int new_width, int new_height)
{
    SDL_Log("視窗大小變更請求：目標邏輯尺寸 %d x %d", new_width, new_height);

    // 0. 檢查是否需要實際改變
    // 如果邏輯尺寸相同，就無需操作。bFullscreen 不參與此判斷，因為它是固定的。
    if (new_width == SCREEN_WIDTH && new_height == SCREEN_HEIGHT) {
        SDL_Log("INFO: 邏輯尺寸已匹配請求。無需變更。");
        return;
    }

    // 1. 更新全域的邏輯尺寸變數
    // 這些變數現在代表了你『希望渲染的邏輯尺寸』
    SCREEN_WIDTH = new_width;
    SCREEN_HEIGHT = new_height;
    GAME_SCREEN_XSIZE = new_width; // 根據你的用途更新
    GAME_SCREEN_YSIZE = new_height; // 根據你的用途更新

    // --- 確保 window 和 renderer 已初始化 ---
    if (!g_SDLInfo.window || !g_SDLInfo.renderer) {
        SDL_Log("ERROR: 視窗或渲染器未初始化！無法執行尺寸變更。");
        return;
    }

    // 3. 設定窗口尺寸和渲染器邏輯尺寸
    // 這裡的邏輯將根據 bFullscreen（遊戲啟動時的設定）來執行
    if (bFullscreen) { 
        // 如果遊戲是啟動在全螢幕模式
        /*if (renderer) {
            SDL_DestroyRenderer(renderer); renderer = nullptr;
        }
        if (window) {
            SDL_DestroyWindow(window); window = nullptr;
        }

        Uint32 windowFlags = SDL_WINDOW_SHOWN;
        if (bFullscreen) {
            windowFlags |= SDL_WINDOW_FULLSCREEN;
            SDL_Log("INFO: Attempting to create fullscreen window.");
        }

        window = SDL_CreateWindow(
            "", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH, SCREEN_HEIGHT,
            windowFlags | SDL_WINDOW_RESIZABLE // 加上这个 flag 让窗口可调整大小
        );
        // 检查 window 是否创建成功
        if (!window) {
            SDL_Log("ERROR: Failed to create window: %s", SDL_GetError());
            // 处理致命错误
            return;
        }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        // 检查 renderer 是否创建成功
        if (!renderer) {
            SDL_Log("ERROR: Failed to create renderer: %s", SDL_GetError());
            SDL_DestroyWindow(window); // 销毁窗口以避免资源泄露
            window = nullptr;
            return;
        }*/


		g_TextureCache.clear(); // 清空紋理快取，因為全螢幕模式可能需要重新載入紋理

    }
    else { // 窗口模式
        // 窗口模式：直接設定窗口尺寸為目標邏輯尺寸 (new_width, new_height)
        SDL_SetWindowSize(g_SDLInfo.window, new_width, new_height);

        // 也設定渲染器的邏輯尺寸，確保內部渲染邏輯固定在 new_width x new_height
        if (SDL_RenderSetLogicalSize(g_SDLInfo.renderer, new_width, new_height) < 0) {
            SDL_Log("ERROR: 無法設定邏輯尺寸：%s", SDL_GetError());
        }

        // 窗口居中邏輯 (僅當窗口模式時)
        SDL_DisplayMode display_mode;
        SDL_GetDesktopDisplayMode(0, &display_mode);
        SDL_SetWindowPosition(g_SDLInfo.window, (display_mode.w - new_width) / 2, (display_mode.h - new_height) / 2);
        SDL_RestoreWindow(g_SDLInfo.window);
    }

    SDL_RenderSetViewport(g_SDLInfo.renderer, NULL); // 重置視口，確保渲染覆蓋整個新窗口

    // 4. 重新建立 cpuRenderSurface (其尺寸應與邏輯尺寸匹配)
    // 销毁旧的 Surface
    if (cpuRenderSurface) { SDL_FreeSurface(cpuRenderSurface); cpuRenderSurface = nullptr; }
    cpuRenderSurface = SDL_CreateRGBSurfaceWithFormat(
        0, SCREEN_WIDTH, SCREEN_HEIGHT, 16, SDL_PIXELFORMAT_RGB565
    );
    if (!cpuRenderSurface) { SDL_Log("ERROR: 無法建立 cpuRenderSurface：%s", SDL_GetError()); return; }

    // 5. 重新建立 streamingTexture (其尺寸也应与逻辑尺寸匹配)
    // 销毁旧的 Texture
    if (streamingTexture) { SDL_DestroyTexture(streamingTexture); streamingTexture = nullptr; }
    streamingTexture = SDL_CreateTexture(g_SDLInfo.renderer,
        cpuRenderSurface->format->format,
        SDL_TEXTUREACCESS_STREAMING,
        cpuRenderSurface->w, // 使用邏輯寬度
        cpuRenderSurface->h); // 使用邏輯高度
    if (!streamingTexture) { SDL_Log("ERROR: 無法建立 streamingTexture：%s", SDL_GetError()); return; }

    setBackbuffersurface(g_DestBackBuf, dDxSize, wDxSize); // 更新後台緩衝區

    SDL_Log("INFO: 視窗尺寸變更完成。新邏輯尺寸：%d x %d", SCREEN_WIDTH, SCREEN_HEIGHT);
}

/**
 * @brief 將當前 SDL 渲染器的內容擷取並儲存為 PNG 檔案。
 *
 * @param renderer 遊戲的 SDL 渲染器。
 * @param filename 要儲存的 PNG 檔案路徑。
 * @return 成功返回 0，失敗返回 -1。
 */
int SDL_DirectDrawInfo::SaveRendererToPNG(const char* filename)
{
    if (!renderer || !filename) {
        SDL_Log("ERROR: Renderer 或檔案名無效。");
        return -1;
    }

    int width, height;
    // 1. 取得渲染目標的尺寸
    if (SDL_GetRendererOutputSize(renderer, &width, &height) != 0) {
        SDL_Log("ERROR: 無法取得渲染器尺寸: %s", SDL_GetError());
        return -1;
    }

    // 2. 在 RAM 中分配緩衝區來儲存 VRAM 讀回的像素
    // 使用 32 位元 ARGB 格式 (每個像素 4 位元組)
    const Uint32 pixel_format = SDL_PIXELFORMAT_ARGB8888;
    const size_t bytes_per_pixel = 4;
    const int pitch = width * bytes_per_pixel;
    const size_t buffer_size = height * pitch;

    // 使用 malloc 分配記憶體，以便稍後釋放
    Uint8* pixels = (Uint8*)malloc(buffer_size);
    if (!pixels) {
        SDL_Log("ERROR: 記憶體分配失敗 (Out of memory)。");
        return -1;
    }

    // 3. 從 VRAM 讀取像素到 RAM
    // 這是將 GPU 數據傳輸回 CPU/系統記憶體的關鍵步驟
    if (SDL_RenderReadPixels(renderer, NULL, pixel_format, pixels, pitch) != 0) {
        SDL_Log("ERROR: 無法從渲染器讀取像素: %s", SDL_GetError());
        free(pixels);
        return -1;
    }

    // 4. 將 RAM 中的像素數據包裝成 SDL_Surface
    // SDL_CreateRGBSurfaceWithFormatFrom 只是包裝，並不會複製數據
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormatFrom(
        pixels,
        width,
        height,
        32, // 位元深
        pitch,
        pixel_format
    );

    if (!surface) {
        SDL_Log("ERROR: 無法建立 Surface: %s", SDL_GetError());
        free(pixels);
        return -1;
    }

    // 5. 使用 SDL_image 函式庫儲存為 PNG 檔案
    int result = IMG_SavePNG(surface, filename);

    // 6. 清理資源
    SDL_FreeSurface(surface); // 釋放 Surface 結構體，但不會釋放 pixels 數據
    free(pixels);             // 釋放第一步 malloc 的像素緩衝區

    if (result != 0) {
        SDL_Log("ERROR: 無法儲存 PNG 檔案 %s: %s", filename, IMG_GetError());
        return -1;
    }

    // SDL_Log("INFO: 成功儲存截圖至 %s", filename);
    return 0;
}


void SDL_DirectDrawInfo::SaveScreenShot(std::string filename)
{
    SaveScreen = filename;
}
// SDL_FlipScreen (將 CPU 渲染 Surface 顯示到螢幕)
void SDL_DirectDrawInfo::SDL_FlipScreen() {

#ifdef _TEXTURE
    //if (activeInputBox)
        //updateIMERect(activeInputBox);
    RenderAllDebugShapes();
    setKeyboardState();
    // 將渲染器中的內容呈現到螢幕 (翻頁)
    SDL_RenderPresent(renderer);
#else
    if (!renderer || !cpuRenderSurface) {
        SDL_Log("ERROR: SDL_FlipScreen: Renderer or cpuRenderSurface is null. Cannot present frame.");
        return;
    }
    if (SDL_UpdateTexture(streamingTexture, NULL, cpuRenderSurface->pixels, cpuRenderSurface->pitch) != 0) {
        SDL_Log("ERROR: SDL_FlipScreen: Failed to update streaming texture: %s", SDL_GetError());
        return;
    }
    // 將 streamingTexture 複製到渲染器
    SDL_RenderCopy(renderer, streamingTexture, NULL, NULL);
    RenderAllDebugShapes();
    
    //if(activeInputBox)
        //updateIMERect(activeInputBox);
    setKeyboardState();
    // ★★★ [插入點 3-B] 繪製 ImGui (畫在最上層) ★★★
    // 這會把 GM 面板畫在所有遊戲內容之上
    CimGui::Instance().Render();
    // 將渲染器中的內容呈現到螢幕 (翻頁)
    SDL_RenderPresent(renderer);
#endif

    if (SaveScreen != "")
    {
#ifdef _TEXTURE
      SaveRendererToPNG(SaveScreen.c_str());
#else
      IMG_SavePNG(cpuRenderSurface, SaveScreen.c_str());
#endif
      SaveScreen = "";
    }
}

void SDL_DirectDrawInfo::cleanupTextCache(std::chrono::milliseconds unused_threshold) {
    // 獲取當前時間
    auto now = std::chrono::steady_clock::now();

    // 使用 erase-remove idiom 或直接遍歷並刪除
    // 因為在 map 遍歷時刪除元素可能導致迭代器失效，所以通常會先收集要刪除的 Key，然後再刪除
    std::vector<TextRenderKey> keys_to_remove;
    //SDL_Log("g_textSurfaceCache=%d", g_textSurfaceCache.size());
    for (const auto& pair : g_textSurfaceCache) {
        // 計算自上次使用以來經過的時間
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - pair.first.lastUsed);

        // 如果超過閾值，則標記為待刪除
        if (elapsed > unused_threshold) {
            keys_to_remove.push_back(pair.first);
        }
    }

    // 刪除標記的元素
    for (const auto& key : keys_to_remove) {
        g_textSurfaceCache.erase(key);
        //SDL_Log("Removed text from cache: %s", key.text.c_str()); // 打印日誌方便調試
    }

	keys_to_remove.clear(); // 清空待刪除的鍵列表
    // 接下來處理 g_textTextureCache
	// 這裡的邏輯與 g_textSurfaceCache 類似
    for (const auto& pair : g_textTextureCache) {
        // 計算自上次使用以來經過的時間
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - pair.first.lastUsed);
        // 如果超過閾值，則標記為待刪除
        if (elapsed > unused_threshold) {
            keys_to_remove.push_back(pair.first);
            //SDL_Log("CLEANUP: Removing Key: %s, Texture Address: %p", pair.first.text.c_str(), (void*)pair.second.get());
        }
    }
    // 刪除標記的元素
    for (const auto& key : keys_to_remove) {
        g_textTextureCache.erase(key);
	}
}

void SDL_DirectDrawInfo::setKeyboardState()
{
    //std::fill(keysJustPressed, keysJustPressed + SDL_NUM_SCANCODES, false);
    //memset(&keyboardEventThisframe, 0, sizeof(SDL_KeyboardEvent));
}

/**
 * @brief 透過掃描 RLE 資料來計算 Sprite 的實際最大像素寬度。
 * @param spr 要計算的 Sprite。
 * @return 該 Sprite 的實際最大寬度（單位：像素）。
 */
int SDL_DirectDrawInfo::CalculateActualWidth(const Spr* spr)
{
    if (!spr || !spr->img || spr->yl <= 0) {
        return 0;
    }

    int maxWidthInPixels = 0;
    const uint8_t* src = (const uint8_t*)spr->img;

    for (int i = 0; i < spr->yl; ++i)
    {
        int currentLineWidthInBytes = 0;
        while (true)
        {
            uint16_t skip_bytes = *reinterpret_cast<const uint16_t*>(src);
            if (skip_bytes == 0xFFFF) {
                src += 2; // 跳過行尾標記
                break;
            }
            uint16_t copy_bytes = *reinterpret_cast<const uint16_t*>(src + 2);
            src += 4;

            currentLineWidthInBytes += skip_bytes + copy_bytes;
            src += copy_bytes;
        }

        // 將這一行的位元組寬度轉換為像素寬度 (假設 16bpp)
        int currentLineWidthInPixels = currentLineWidthInBytes / 2;
        if (currentLineWidthInPixels > maxWidthInPixels) {
            maxWidthInPixels = currentLineWidthInPixels;
        }
    }

    return maxWidthInPixels;
}

SDL_Surface* SDL_DirectDrawInfo::CreateSurfaceFromRawTile(const Spr* spr) {

    constexpr int TILE_WIDTH_BYTES = 64;       // 圖塊的寬度 (位元組)
    constexpr int TILE_HEIGHT_LINES = TILE_SIZE;      // 圖塊的高度 (行數)

    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, TILE_SIZE, TILE_SIZE, 16, SDL_PIXELFORMAT_RGB565);

    if (surface) {
        // --- 核心變動: 將 x 和 y 直接設為 0，並使用 SDL_LockSurface ---
        SDL_LockSurface(surface); // 最佳實踐：鎖定 Surface

        const int destination_pitch = surface->pitch;

        // 直接定義 x 和 y 為 0
        constexpr int x_offset = 0;
        constexpr int y_offset = 0;

        // 計算目標起始位址：使用 0, 0
        char* Dtmp_start_ptr = static_cast<char*>(surface->pixels) +
            (static_cast<std::size_t>(y_offset) * destination_pitch) +
            (static_cast<std::size_t>(x_offset) * 2);

        // 設定來源和目標指標
        const char* Stmp_ptr = static_cast<const char*>(spr->img);
        char* Dtmp_ptr = Dtmp_start_ptr;

        // 逐行複製，共 TILE_HEIGHT_LINES 行
        for (int i = 0; i < TILE_HEIGHT_LINES; ++i)
        {
            std::memcpy(Dtmp_ptr, Stmp_ptr, TILE_WIDTH_BYTES);

            // 更新指標到下一行
            Dtmp_ptr += destination_pitch;
            Stmp_ptr += TILE_WIDTH_BYTES;
        }

        SDL_UnlockSurface(surface); // 解鎖 Surface
        // --- 核心變動結束 ---
    }
    return surface;
}

/**
 * @brief 【核心函式】載入一個臨時 Sprite，如果已載入則從快取中讀取。
 * 這是對原始 LoadTempSprite 函式的直接、功能正確的替代品。
 *
 * @param id 要載入的物件 ID。
 * @param renderer 遊戲的 SDL 渲染器。
 * @param out_spr 一個指向 Spr* 的指標，函式會將找到或載入的 Sprite 指標賦給它。
 * @return 成功找到或載入則返回 true，失敗返回 false。
 */
bool SDL_DirectDrawInfo::LoadTempSprite_Cached_CPP(int id, Spr** out_spr)
{
    // --- 步驟 1: 檢查快取 ---
    // 先檢查這個 ID 對應的 Spr 資料是否已經被我們持久化儲存了。
    auto it = g_TempObjectSprCache.find(id);
    if (it != g_TempObjectSprCache.end()) {
        // 找到了！直接返回快取中的 Spr 指標
        *out_spr = &it->second;
        return true;
    }

    // --- 步驟 2: 如果快取中沒有，則從檔案載入 ---
    char filename[256];
    int lev = id / 1000;
    if (lev == 1) lev = 0;

    // [Android] 路徑修正：移除開頭的 ./
    // 例如：object/0/1234.CSP
    sprintf(filename, "object/%d/%04d.CSP", lev, id);

    // [Android] 使用 SDL_RWFromFile 開啟檔案
    SDL_RWops* fp = SDL_RWFromFile(filename, "rb");
    if (!fp) {
        // 找不到檔案是正常的 (例如特效 ID 不存在)
        // SDL_Log("LoadTempSprite_Cached_CPP: Not Found %s", filename);
        *out_spr = nullptr;
        return false;
    }

    // 建立一個新的 Spr 結構
    Spr new_spr;

    // [Android] 使用 SDL_RWread 讀取各個變數
    // 因為是逐變數讀取，不需要擔心結構對齊問題
    SDL_RWread(fp, &new_spr.xl, sizeof(short), 1);
    SDL_RWread(fp, &new_spr.yl, sizeof(short), 1);
    SDL_RWread(fp, &new_spr.ox, sizeof(short), 1);
    SDL_RWread(fp, &new_spr.oy, sizeof(short), 1);
    SDL_RWread(fp, &new_spr.size, sizeof(unsigned int), 1);

    if (new_spr.size == 0) {
        SDL_RWclose(fp);
        *out_spr = nullptr;
        return false;
    }

    // 使用智能指標為圖像資料分配記憶體
    auto image_buffer = std::make_unique<char[]>(new_spr.size);

    // 這裡先賦值 raw pointer 給 img，稍後 image_buffer 所有權移轉後，這個地址依然有效
    new_spr.img = image_buffer.get();

    // [Android] 讀取圖片數據
    // 參數順序: context, ptr, object_size, object_count
    SDL_RWread(fp, new_spr.img, 1, new_spr.size);

    // [Android] 關閉檔案
    SDL_RWclose(fp);

    // --- 步驟 3: 為這個新的 Sprite 建立紋理 ---
    // 這裡我們直接傳入 new_spr 的指標
    SDL_Texture* texture = CreateTextureFromSingleSpr_CPP(filename, &new_spr);
    if (!texture) {
        // 紋理建立失敗 (可能是格式不支援或 convert565 問題)
        SDL_Log("LoadTempSprite_Cached_CPP: CreateTexture Failed %s", filename);
        *out_spr = nullptr;
        return false;
    }

    // --- 步驟 4: 將新載入的資料存入我們的持久化快取中 ---

    // 1. 移動 Buffer 所有權 (確保記憶體不會被釋放)
    g_TempObjectImageBufferCache[id] = std::move(image_buffer);

    // 2. 複製 Spr 結構 (new_spr.img 指向的記憶體現在由 g_TempObjectImageBufferCache 管理)
    g_TempObjectSprCache[id] = new_spr;

    // 3. 返回 map 中的指針 (這是穩定的)
    *out_spr = &g_TempObjectSprCache.find(id)->second;

    return true;
}

void SDL_DirectDrawInfo::CreateOutlinePointsHorseRider(Spr* horse)
{
	return;// 目前不啟用

    int i = 0, j = 0;
    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 75; j++)
        {
            std::vector<SDL_Point> outline;
            Spr* s = &horse[i*j+j];
            if (s->img == 0 || s->yl == 0 || s->size == 0) continue;

            if (g_outlineCache.find(s) != g_outlineCache.end()) {
              // 如果已經計算過輪廓點，則跳過
              continue;
            }
            if (g_SurfaceCache.find(s) != g_SurfaceCache.end()) {
              // 如果已經有對應的 Surface，則跳過
              continue;
            }

            // 1. 將 Spr 解碼成一個臨時的 Surface
            SDL_Surface* tempSurface = CreateSurfaceFromSpr(s, 0); // 假設您有這個輔助函式
            if (!tempSurface) continue;

            // 鎖定 Surface 以便安全地讀取像素
            SDL_LockSurface(tempSurface);

            Uint32 transparentColorKey32;
            SDL_GetColorKey(tempSurface, &transparentColorKey32);
            Uint16* pixels =
                (Uint16*)
                    tempSurface->pixels;  // 假設是 32-bit，如果不是需要調整
            Uint16 transparentColorKey = (Uint16)transparentColorKey32;
            // 2. 遍歷所有像素
            for (int y = 0; y < s->yl; ++y) {
              for (int x = 0; x < s->xl; ++x) {
                // 如果當前像素不是透明的
                if (pixels[y * s->xl + x] != transparentColorKey) {
                  // --- 【關鍵】使用更安全、更正確的邏輯來檢查邊界 ---
                  bool isOutline = false;

                  // 情況 A：如果像素本身就在圖片的四個邊緣上，那它一定是輪廓點
                  if (x == 0 || x == s->xl - 1 || y == 0 || y == s->yl - 1) {
                    isOutline = true;
                  } else {
                    // 情況 B：如果像素不在邊緣上，則檢查它的四個鄰居是否為透明
                    // 這樣可以避免讀取到陣列之外的記憶體
                    if (pixels[y * s->xl + (x - 1)] == transparentColorKey)
                      isOutline = true;  // 左鄰居
                    else if (pixels[y * s->xl + (x + 1)] == transparentColorKey)
                      isOutline = true;  // 右鄰居
                    else if (pixels[(y - 1) * s->xl + x] == transparentColorKey)
                      isOutline = true;  // 上鄰居
                    else if (pixels[(y + 1) * s->xl + x] == transparentColorKey)
                      isOutline = true;  // 下鄰居
                  }

                  // 如果它是輪廓點，就記錄它的相對座標
                  if (isOutline) {
                    // 我們需要減去 Spr 的繪圖偏移量，得到以 Spr
                    // 中心為原點的座標
                    outline.push_back({x - s->ox, y - (s->oy / 2)});
                  }
                }
              }
            }

            SDL_UnlockSurface(tempSurface);

            //SDL_FreeSurface(tempSurface);
            g_SurfaceCache[s].originSurface = tempSurface; // 將 Surface 存入快取
			g_SurfaceCache[s].newSurface = nullptr; // 初始化為 nullptr
            g_outlineCache[s] = outline;
            outline.clear();  // 清空輪廓點容器，為下一個 Spr 做準備
        }
    }

    //SDL_Log("INFO: 輪廓點計算完成，共計 %zu 個 Spr 被儲存至 Surface", g_SurfaceCache.size());
}

// 這個函式會分析一個 Spr，並返回其所有輪廓點的座標
void SDL_DirectDrawInfo::CreateOutlinePoints(CharacterSprite* charspr)
{
    int i = 0, j = 0;
    for(i = 0; i < 8; i++)
    {
        for(j = 0; j < 75; j++)
        {
            std::vector<SDL_Point> outline;
			Spr* s = &charspr->sp[i][j];
            if (s->img == 0 || s->yl == 0 || s->size ==0) continue;
            
            if(g_outlineCache.find(s) != g_outlineCache.end())
            {
                // 如果已經計算過輪廓點，則跳過
                continue;
			}
            if (g_SurfaceCache.find(s) != g_SurfaceCache.end())
            {
                // 如果已經有對應的 Surface，則跳過
                continue;
            }
            // 1. 將 Spr 解碼成一個臨時的 Surface
            SDL_Surface* tempSurface = CreateSurfaceFromSpr(s, 0); // 假設您有這個輔助函式
            if (!tempSurface) continue;

            // 鎖定 Surface 以便安全地讀取像素
            SDL_LockSurface(tempSurface);

            Uint32 transparentColorKey32;
            SDL_GetColorKey(tempSurface, &transparentColorKey32);
            Uint16* pixels = (Uint16*)tempSurface->pixels; // 假設是 32-bit，如果不是需要調整
            Uint16 transparentColorKey = (Uint16)transparentColorKey32;
            // 2. 遍歷所有像素
            for (int y = 0; y < s->yl; ++y) {
                for (int x = 0; x < s->xl; ++x) {
                    // 如果當前像素不是透明的
                    if (pixels[y * s->xl + x] != transparentColorKey) {

                        // --- 【關鍵】使用更安全、更正確的邏輯來檢查邊界 ---
                        bool isOutline = false;

                        // 情況 A：如果像素本身就在圖片的四個邊緣上，那它一定是輪廓點
                        if (x == 0 || x == s->xl - 1 || y == 0 || y == s->yl - 1) {
                            isOutline = true;
                        }
                        else {
                            // 情況 B：如果像素不在邊緣上，則檢查它的四個鄰居是否為透明
                            // 這樣可以避免讀取到陣列之外的記憶體
                            if (pixels[y * s->xl + (x - 1)] == transparentColorKey) isOutline = true; // 左鄰居
                            else if (pixels[y * s->xl + (x + 1)] == transparentColorKey) isOutline = true; // 右鄰居
                            else if (pixels[(y - 1) * s->xl + x] == transparentColorKey) isOutline = true; // 上鄰居
                            else if (pixels[(y + 1) * s->xl + x] == transparentColorKey) isOutline = true; // 下鄰居
                        }

                        // 如果它是輪廓點，就記錄它的相對座標
                        if (isOutline) {
                            // 我們需要減去 Spr 的繪圖偏移量，得到以 Spr 中心為原點的座標
                            outline.push_back({ x - s->ox, y -(s->oy/2) });
                        }
                    }
                }
            }

            SDL_UnlockSurface(tempSurface);
            //SDL_FreeSurface(tempSurface);
            g_SurfaceCache[s].originSurface = tempSurface; // 將 Surface 存入快取
            g_SurfaceCache[s].newSurface = nullptr; // 初始化為 nullptr
            g_outlineCache[s] = outline;
			outline.clear(); // 清空輪廓點容器，為下一個 Spr 做準備
        }
	}

	//SDL_Log("INFO: 輪廓點計算完成，共計 %zu 個 Spr 的輪廓點被儲存。", g_outlineCache.size());
    //SDL_Log("INFO: 輪廓點計算完成，共計 %zu 個 Spr 被儲存至 Surface", g_SurfaceCache.size());
}

/**
 * @brief 從壓縮的 Spr 資料建立一個新的 SDL_Surface。
 * 這個函式會將 Sprite 解壓縮到一個新的 16-bit Surface 上。
 * @param spr 指向 Spr 結構的指標。
 * @return 成功則返回一個新的 SDL_Surface*，失敗則返回 nullptr。
 */
SDL_Surface* SDL_DirectDrawInfo::CreateSurfaceFromSpr(const Spr* spr, uint16_t transparentColor, bool tile)
{
  // 先做最基本的檢查
  if (!spr || !spr->img || spr->xl <= 0 || spr->yl <= 0 || spr->size <= 4) {
    SDL_Surface* nothing =
        SDL_CreateRGBSurfaceWithFormat(0, 0, 0, 16, SDL_PIXELFORMAT_RGB565);
    if (nothing) {
      transparentColor = SDL_MapRGB(nothing->format, 0, 248, 0);
      SDL_FillRect(nothing, NULL, transparentColor);
      // SDL_SetColorKey(nothing, SDL_TRUE, transparentColor);
      SDL_Surface* s =
          SDL_ConvertSurfaceFormat(nothing, SDL_PIXELFORMAT_RGB565, 0);
      SDL_FreeSurface(nothing);
      return s;
    } else
      return nullptr;
  }

  // 【核心修正】呼叫輔助函式計算真實寬度
  int actualWidth = spr->xl;
  int actualHeight = spr->yl;
  if (tile && spr->xl == 31) {
      actualWidth = actualHeight = TILE_SIZE;
}
  // 建立一個 16-bit (RGB565) 的 Surface 來存放解壓縮後的圖像
  // 注意：SDL 使用 Big Endian (高位在前)，所以 Mask 順序是 R, G, B, A
  SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(
      0, actualWidth, actualHeight, 16,
      SDL_PIXELFORMAT_RGB565);  // Alpha 遮罩 (我們沒有)
  if (!surface) {
    SDL_Log("Failed to create surface for sprite: %s", SDL_GetError());
    return nullptr;
  }

  // 用 Sprite 的背景色 (0) 預先填滿整個 Surface
  // 這樣未被像素資料覆蓋的區域就是透明色
  transparentColor = SDL_MapRGB(surface->format, 0, 248, 0);
  SDL_FillRect(surface, NULL, transparentColor);
  SDL_SetColorKey(surface, SDL_TRUE, transparentColor);

  SDL_LockSurface(surface);
  Spr s = *spr;
  PutCmprsImgNOTCliping_CPP(0, 0, &s, surface->pixels, surface->pitch);
  /*uint8_t* dest_pixels = static_cast<uint8_t*>(surface->pixels);
  int pitch = surface->pitch;

  const uint8_t* src = reinterpret_cast<const uint8_t*>(spr->img);
  const uint8_t* end_ptr = src + spr->size;

  // 輔助函式：以小端序安全地讀取 uint16_t
  // 這確保了數據讀取不會受到記憶體對齊和端序的影響
  auto read_uint16_le = [&]() -> uint16_t {
    if (src + sizeof(uint16_t) > end_ptr) {
      return 0;  // 數據不完整，返回 0 避免越界
    }
    uint16_t value = static_cast<uint16_t>(src[0] | (src[1] << 8));
    src += sizeof(uint16_t);
    return value;
  };

  for (int i = 0; i < spr->yl; ++i) {
    // 每一行的起始位址，這裡使用了正確的 pitch
    uint8_t* dest_ptr_for_line = dest_pixels + i * pitch;

    while (src < end_ptr) {
      uint16_t skip_bytes = read_uint16_le();
      if (skip_bytes == 0xFFFF) {
        // 遇到行結束標記，結束這一行的解碼
        break;
      }

      uint16_t copy_bytes = read_uint16_le();

      // 跳過透明像素的位元組數
      dest_ptr_for_line += skip_bytes;

      // 確保要複製的數據不會超出源數據的範圍
      if (src + copy_bytes > end_ptr) {
        copy_bytes = end_ptr - src;
      }

      // 將像素數據塊複製到 Surface 上
      memcpy(dest_ptr_for_line, src, copy_bytes);

      // 更新源和目的指標
      src += copy_bytes;
      dest_ptr_for_line += copy_bytes;
    }
  }*/

  SDL_UnlockSurface(surface);
  return surface;
}

bool SDL_DirectDrawInfo::RenderMethod(int x, int y, Spr* s, RENDER_TYPE type)
{
#ifndef _TEXTURE
    return false;
#endif
	/*auto it = g_SurfaceCache.find(s);
    if (it != g_SurfaceCache.end())
    {
        auto& surface = it->second; // 取得 Surface
        if(surface.newSurface == nullptr) {
            SDL_SetColorKey(surface.originSurface, SDL_TRUE, SDL_MapRGB(surface.originSurface->format, 0, 0, 0));
            SDL_Color* color = nullptr;
            //if(renderCh && s == renderCh->sp0)
                //color = getOrCreategGradient(renderCh->clothcolor);
            if (color == nullptr) return false;  // 如果顏色為空，直接返回
            SDL_Surface* effectSurface = CreateRecoloredSurface(surface.originSurface, color);
			surface.newSurface = effectSurface; // 更新快取中的新 Surface
		}
        if (surface.newSurface) {
            SDL_Rect destRect = { x - s->ox, y - s->oy, surface.newSurface->w, surface.newSurface->h };
            // 1. 將效果 Surface 繪製到 CPU 渲染 Surface 上
            SDL_BlitSurface(surface.newSurface, NULL, cpuRenderSurface, &destRect); 
            return true;
        }
    }
    return false;*/
    if (!s) return false;
    // 1. 使用 Spr* 指標，從快取中快速查找紋理資訊
    auto it = g_TextureCache.find(s);
    //SDL_Rect destRect;
    // 2. 如果找到了
    if (it != g_TextureCache.end())
    {
        auto& info = it->second; // 取得 TextureInfo

        switch (type)
        {
        case PUT_NORMAL:
        {
            CRenderMethod::PutCompressedImage(x, y, s, info, renderer);
        }break;
        case PUT_APART:
        {
            //CRenderMethod::PutCompressImageApart_CPP(x, y, s, info, param1, param2, param3, param4, param5);
        }break;
        case PUT_BLENDRGB:
        {
            //CRenderMethod::PutCompressedImageBlendRGB(x, y, s, info, param1);
        }break;
        case PUT_CHARRGB:
        {
            CRenderMethod::PutCompressedImageCharRGB(x, y, s, info, param[0], renderer);
        }break;
        case PUT_FX:
        {
            CRenderMethod::PutCompressedImageFX(x, y, s, info, param[0], param[1], renderer);
        }break;
        case PUT_ONECOLOR:
        {
            //CRenderMethod::PutCompressedImageOneColor(x, y, s, info, param1);
        }break;
        case PUT_SHADOW:
        {
            //CRenderMethod::PutCompressedImageShadow(x, y, s, info);
        }break;
        case PUT_VOID:
        {
            //CRenderMethod::PutCompressedImageVoid(x, y, s, info);
        }break;
        case PUT_WAVTILE:
        {
            //CRenderMethod::PutCompressedImageWaveTile(x, y, s, info);
        }break;
        case PUT_TILE:
        {
            if (s->xl != 32) {
                //SDL_Log("%d", s->xl);
                //s->yl = s->xl = TILE_SIZE;
            }
            CRenderMethod::PutTile(x, y, info);
        }break;
        case PUT_TILE_ALPHA:
        {
            CRenderMethod::PutTileFX(x, y, info, param[0]);
        }break;
        }
    }
    //else SDL_Log("cannot find the spr pointer file %d (%d,%d)", type, x-s->ox, y-s->oy);

    return true;
}

/**
 * @brief 卸載一個 .spr 檔案對應的紋理圖集和所有相關快取。
 *
 * @param filename 要卸載的 .spr 檔案的路徑。
 */
void SDL_DirectDrawInfo::UnloadSprAtlas(const char* filename)
{
    // 1. 從管理器中尋找這個檔案對應的紋理圖集
    auto it_manager = g_AtlasManager.find(filename);
    if (it_manager == g_AtlasManager.end()) {
        // 這個檔案從未被載入，或者已經被卸載了。
        return;
    }

    SDL_Texture* texture_to_destroy = it_manager->second;

    // 2. 【關鍵】安全地銷毀 SDL_Texture 物件
    if (texture_to_destroy) {
        SDL_DestroyTexture(texture_to_destroy);
        // SDL_Log("成功銷毀紋理圖集 for %s", filename);
    }

    // 3. 從管理器中移除記錄
    g_AtlasManager.erase(it_manager);

    // 4. 清理 g_TextureCache 中所有指向這個已銷毀紋理的條目
    // 我們需要遍歷整個快取，找出所有相關的 Spr* 並刪除它們。
    for (auto it_cache = g_TextureCache.begin(); it_cache != g_TextureCache.end(); ) {
        // it_cache->second 是 TextureInfo
        // it_cache->second.atlasTexture 是指向大圖集的指標
        if (it_cache->second.atlasTexture == texture_to_destroy) {
            // 如果這個 Sprite 屬於我們剛剛銷毀的圖集，就從快取中移除它。
            // map::erase 會返回下一個有效的迭代器，這是安全的刪除方式。
            it_cache = g_TextureCache.erase(it_cache);
        }
        else {
            // 如果不屬於，就繼續檢查下一個。
            ++it_cache;
        }
    }
}

void SDL_DirectDrawInfo::CleanupAllAtlases()
{
    // 為了安全地遍歷和刪除，我們先複製一份檔名列表
    std::vector<std::string> filenames_to_unload;
    for (const auto& pair : g_AtlasManager) {
        filenames_to_unload.push_back(pair.first);
    }

    // 然後根據列表來卸載
    for (const auto& filename : filenames_to_unload) {
        UnloadSprAtlas(filename.c_str());
    }

    for (auto it_cache = g_SurfaceCache.begin(); it_cache != g_SurfaceCache.end();  ++it_cache) {
        if (it_cache->second.originSurface ) {
            SDL_FreeSurface(it_cache->second.originSurface);
        }
        if (it_cache->second.newSurface) {
            SDL_FreeSurface(it_cache->second.newSurface);
        }
    }
	g_SurfaceCache.clear(); // 清空 Surface 快取
}

SDL_Texture* SDL_DirectDrawInfo::GetOrCreateAtlas(const char* filename)
{
    // 1. **最高優先級：檢查 VRAM 快取**
    auto it = g_AtlasManager.find(filename);

    if (it != g_AtlasManager.end()) {
        // VRAM 中已存在！直接返回現有的紋理。
        // 這個步驟避免了重新讀取硬碟、重新計算尺寸、重新上傳 VRAM。
        return it->second;
    }

    return nullptr;
}
/**
 * @brief 【單一 Sprite 專用】從一個已載入的 Spr 結構，建立一個獨立的 SDL_Texture。
 *
 * @param s 指向已載入資料的 Spr 結構的指標。
 * @param renderer 遊戲的 SDL 渲染器。
 * @param transparentColor Sprite 使用的透明色 (例如 0 代表黑色)。
 * @return 成功則返回指向新建的 SDL_Texture 的指標，失敗則返回 nullptr。
 */
SDL_Texture* SDL_DirectDrawInfo::CreateTextureFromSingleSpr_CPP(const char* filename, const Spr* s, uint16_t transparentColor) {
#ifndef _LOADSPRITE
  return nullptr;  // 如果沒有載入 Spr 功能，直接返回
#endif
    if (!s || s->xl <= 0 || s->yl <= 0 || !s->img) {
        return nullptr;
    }

    SDL_Texture* Atlas = GetOrCreateAtlas(filename);
    if (Atlas != nullptr) return Atlas;

    // 步驟 1: 為這個 Sprite 建立一個大小剛好的暫時 Surface
    SDL_Surface* tempSurface = CreateSurfaceFromSpr(s, transparentColor);
    if (!tempSurface) {
        return nullptr;
    }

    // 步驟 2: 從這個 Surface 建立最終的 GPU 紋理
    SDL_Texture* finalTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);

    // 步驟 3: 釋放不再需要的暫時 Surface
    SDL_FreeSurface(tempSurface);

    if (!finalTexture) {
        // SDL_Log("從 Surface 為 %s 建立紋理失敗: %s", filename, SDL_GetError());
        return nullptr;
    }

    // 為紋理設定混合模式
    SDL_SetTextureBlendMode(finalTexture, SDL_BLENDMODE_BLEND);

    // --- 【關鍵】將新建立的紋理納入統一管理 ---

    // 1. 在管理器中註冊這個新紋理，以便未來可以統一釋放。
    g_AtlasManager.emplace(filename, finalTexture);

    // 2. 在快取中為這個 Sprite 建立繪圖資訊。
    TextureInfo info;
    info.atlasTexture = finalTexture; // 它的「圖集」就是它自己
    info.sourceRect = { 0, 0, s->xl, s->yl }; // 來源矩形就是整個紋理
    g_TextureCache[s] = info;
    SDL_Log("INFO: 為 %s 建立獨立紋理成功，地址: %p", filename, (void*)finalTexture);
    return finalTexture;
}

/**
 * @brief 【核心函式】接收已經載入到記憶體的地圖 Sprite 資料，並為其建立一個紋理圖集。
 *
 * @param mapname 地圖名稱，用於在管理器中作為 Key。
 * @param loaded_sprites 指向一個包含了所有地圖圖塊/Sprite 資訊的 Spr 陣列 (例如全域的 Sou 陣列)。
 * @param sprite_count 陣列中有效的 Sprite 數量 (例如全域的 TotalTileNo)。
 * @param renderer 遊戲的 SDL 渲染器。
 * @return 成功則返回指向新建的 SDL_Texture 的指標，失敗則返回 nullptr。
 */
SDL_Texture* SDL_DirectDrawInfo::CreateAtlasForMap_CPP(const char* mapname, Spr* loaded_sprites, int sprite_count,bool tile)
{
#ifndef _LOADSPRITE
  return nullptr;  // 如果沒有載入 Spr 功能，直接返回
#endif
    if (!loaded_sprites || sprite_count <= 0) return nullptr;

    SDL_Texture* Atlas = GetOrCreateAtlas(mapname);
    if (Atlas != nullptr) return Atlas;

    // --- 步驟 1: 收集所有有效的 Sprite 指標 ---
    std::vector<Spr*> sprites_to_pack;
    for (int i = 0; i < sprite_count; ++i) {
        Spr* s = &loaded_sprites[i];
        // 根據 LoadMapSource2 的邏輯，xl=-1 代表原始圖塊，也是有效的
        if (s->img != nullptr/*&& (s->xl > 0 || s->xl == -1) && s->yl > 0*/ ) {
          sprites_to_pack.push_back(s);
        }
    }

    // 按高度降序排序，以優化打包效率
    std::sort(sprites_to_pack.begin(), sprites_to_pack.end(), [](const Spr* a, const Spr* b) {
        int height_a = std::max((int)a->yl, TILE_SIZE);
        int height_b = std::max((int)b->yl, TILE_SIZE);
        return height_a > height_b;
        });

    if (sprites_to_pack.empty()) return nullptr;

    // --- 步驟 2：第一階段 - 模擬打包，計算所需圖集的最小尺寸 ---
    int required_width = 0, required_height = 0, current_x = 0, current_y = 0, max_row_height = 0;
    std::unordered_map<const Spr*, SDL_Rect> temp_positions;
    const int PACKING_WIDTH_LIMIT = 2048; // 設定一個合理的打包寬度上限

    for (const Spr* s : sprites_to_pack) {
        int w = std::max((int)s->xl, TILE_SIZE);
        int h = std::max((int)s->yl, TILE_SIZE);
        if (current_x + w > PACKING_WIDTH_LIMIT) {
            current_y += max_row_height;
            current_x = 0;
            max_row_height = 0;
        }
        temp_positions[s] = { current_x, current_y, w, h };
        current_x += w;
        required_width = max(required_width, current_x);
        max_row_height = max(max_row_height, h);
    }
    required_height = current_y + max_row_height;

    // --- 步驟 3: 將尺寸調整為對 GPU 更友善的「2的次方」尺寸 ---
    int atlas_width = NextPowerOfTwo(required_width);
    int atlas_height = NextPowerOfTwo(required_height);

    // --- 步驟 4：第二階段 - 實際建立圖集並拼貼 ---
    SDL_Surface* atlasSurface = SDL_CreateRGBSurfaceWithFormat(0, atlas_width, atlas_height, 16, SDL_PIXELFORMAT_RGB565);
    if (!atlasSurface) return nullptr;
    Uint32 transparentKey = SDL_MapRGB(atlasSurface->format, 0, 248, 0); // 使用洋紅色作為透明色鍵，方便除錯
    SDL_FillRect(atlasSurface, NULL, transparentKey);
    SDL_SetColorKey(atlasSurface, SDL_TRUE, transparentKey);

    for (const Spr* s : sprites_to_pack) {
        SDL_Surface* tempSurface = nullptr;
        if (s->xl == -1) { // 處理未經壓縮的原始圖塊
            tempSurface = CreateSurfaceFromRawTile(s);
        }
        else { // 處理 RLE 壓縮的 Sprite
               //因為原始tile圖片有一些不對齊,有一些 xl 是 31
               //所以第三個參數,表示加載的是地圖tile 圖片,
               //函式內部會強制轉換為 32*32.不轉換,render copy 畫圖時會少了像素
               //第三參數默認為 false
            tempSurface = CreateSurfaceFromSpr(s, 0, tile); // 第二參數透明色是黑色
            
        }

        if (tempSurface) {
            SDL_Rect destRect = temp_positions[s];
            SDL_BlitSurface(tempSurface, NULL, atlasSurface, &destRect);

            TextureInfo info;
            info.atlasTexture = nullptr; // 最終紋理還沒做好
            info.sourceRect = destRect;
            g_TextureCache[s] = info; // 將 Spr 指標和它在圖集中的位置資訊存入快取

            SDL_FreeSurface(tempSurface);
        }
    }


    // --- 步驟 5：送去工廠製版，建立最終的 GPU 紋理 ---
    SDL_Texture* finalAtlasTexture = SDL_CreateTextureFromSurface(renderer, atlasSurface);
    if (finalAtlasTexture) {
        SDL_SetTextureBlendMode(finalAtlasTexture, SDL_BLENDMODE_BLEND);
        // 在管理器中註冊這個新的圖集
        g_AtlasManager.emplace(mapname, finalAtlasTexture);

        // 回過頭來，更新快取中所有 Sprite 的 atlasTexture 指標
        for (const Spr* s : sprites_to_pack) {
            if (g_TextureCache.count(s)) {
                g_TextureCache.at(s).atlasTexture = finalAtlasTexture;
            }
        }
    }

    // --- 步驟 6：大掃除 ---
    SDL_FreeSurface(atlasSurface);

    SDL_Log("INFO: 為地圖 %s 建立圖集成功，尺寸: %dx%d，包含 %zu 個 Sprite，地址: %p",
        mapname, atlas_width, atlas_height, sprites_to_pack.size(),
        (void*)finalAtlasTexture);
    SDL_Log("INFO: 快取中目前有 %zu 個 Sprite 條目", g_TextureCache.size());

    return finalAtlasTexture;
}

SDL_Texture* SDL_DirectDrawInfo::CreateAtlasFromSprArray2D_CPP(const char* filename, const Spr* spr_array_2d, int num_rows, int num_cols)
{
#ifndef _LOADSPRITE
  return nullptr;  // 如果沒有載入 Spr 功能，直接返回
#endif
    if (!spr_array_2d || num_rows <= 0 || num_cols <= 0) return nullptr;

    SDL_Texture* Atlas = GetOrCreateAtlas(filename);
    if (Atlas != nullptr) return Atlas;

    // --- 步驟 1: 收集 - 將二維陣列中的所有有效 Sprite 指標，扁平化到一個 vector 中 ---
    std::vector<const Spr*> sprites_to_pack;
    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
            // 透過指標運算來訪問二維陣列的元素
            const Spr* s = &spr_array_2d[i * num_cols + j];
            if (s->xl > 0 && s->yl > 0 && s->img != nullptr) {
                sprites_to_pack.push_back(s);
            }
        }
    }

    // 按高度降序排序，以優化打包效率
    std::sort(sprites_to_pack.begin(), sprites_to_pack.end(), [](const Spr* a, const Spr* b) {
        return a->yl > b->yl;
        });

    if (sprites_to_pack.empty()) {
        return nullptr;
    }

    // --- 步驟 2 & 3: 第一階段 - 模擬打包，計算並調整圖集尺寸 ---
    int required_width = 0;
    int required_height = 0;
    int current_x = 0;
    int current_y = 0;
    int max_row_height = 0;
    std::unordered_map<const Spr*, SDL_Rect> temp_positions;
    const int PACKING_WIDTH_LIMIT = 2048; // 設定一個合理的打包寬度上限

    for (const Spr* s : sprites_to_pack) {
        if (current_x + s->xl > PACKING_WIDTH_LIMIT) {
            current_y += max_row_height;
            current_x = 0;
            max_row_height = 0;
        }
        temp_positions[s] = { current_x, current_y, s->xl, s->yl };
        current_x += s->xl;
        required_width = max(required_width, current_x);
        max_row_height = max(max_row_height, (int)s->yl);
    }
    required_height = current_y + max_row_height;

    int atlas_width = NextPowerOfTwo(required_width);
    int atlas_height = NextPowerOfTwo(required_height);

    // --- 步驟 4：第二階段 - 實際建立圖集並拼貼 ---
    SDL_Surface* atlasSurface = SDL_CreateRGBSurfaceWithFormat(0, atlas_width, atlas_height, 16, SDL_PIXELFORMAT_RGB565);
    if (!atlasSurface) return nullptr;

    Uint32 transparentKey = SDL_MapRGB(atlasSurface->format, 0, 248, 0);
    SDL_FillRect(atlasSurface, NULL, transparentKey);
    SDL_SetColorKey(atlasSurface, SDL_TRUE, transparentKey);

    for (const Spr* s : sprites_to_pack) {
        SDL_Surface* tempSurface = CreateSurfaceFromSpr(s, 0);
        if (!tempSurface) continue;

        SDL_Rect destRect = temp_positions[s];
        SDL_BlitSurface(tempSurface, NULL, atlasSurface, &destRect);

        TextureInfo info;
        info.atlasTexture = nullptr;
        info.sourceRect = destRect;
        g_TextureCache[s] = info;

        SDL_FreeSurface(tempSurface);
    }

    // --- 步驟 5, 6, 7: 建立紋理，更新快取，並清理 ---
    SDL_Texture* finalAtlasTexture = SDL_CreateTextureFromSurface(renderer, atlasSurface);
    if (finalAtlasTexture) {
        SDL_SetTextureBlendMode(finalAtlasTexture, SDL_BLENDMODE_BLEND);
        g_AtlasManager.emplace(filename, finalAtlasTexture);
        for (const Spr* s : sprites_to_pack) {
            if (g_TextureCache.count(s)) {
                g_TextureCache.at(s).atlasTexture = finalAtlasTexture;
            }
        }
    }

    SDL_FreeSurface(atlasSurface);
    SDL_Log("INFO: 為 %s 建立圖集成功，尺寸: %dx%d，包含 %zu 個 Sprite，地址: %p",
        filename, atlas_width, atlas_height, sprites_to_pack.size(),
        (void*)finalAtlasTexture);
    SDL_Log("INFO: 快取中目前有 %zu 個 Sprite 條目", g_TextureCache.size());
    return finalAtlasTexture;
}
/**
 * @brief 【角色專用核心函式】接收已載入的 CharacterSprite 資料，並為其建立一個單一的紋理圖集。
 *
 * @param filename 檔名，用於在管理器中作為 Key。
 * @param char_sprite 指向包含了所有動畫資料的 CharacterSprite 結構的指標。
 * @param renderer 遊戲的 SDL 渲染器。
 * @return 成功則返回指向新建的 SDL_Texture 的指標，失敗則返回 nullptr。
 */
SDL_Texture* SDL_DirectDrawInfo::CreateAtlasForCharacter(const char* filename, const CharacterSprite* char_sprite, int frames_per_direction)
{
#ifndef _LOADSPRITE
  return nullptr;  // 如果沒有載入 Spr 功能，直接返回
#endif
    if (!char_sprite) return nullptr;
    // 直接呼叫我們那個通用的核心函式
    return CreateAtlasFromSprArray2D_CPP(filename, &char_sprite->sp[0][0], 8, frames_per_direction);
}

SDL_Texture* SDL_DirectDrawInfo::CreateAtlasForHorseRider(const char* filename,const Spr* horse_rider)
{
#ifndef _LOADSPRITE
  return nullptr;  // 如果沒有載入 Spr 功能，直接返回
#endif
    if (!horse_rider) return nullptr;
    // 直接呼叫我們那個通用的核心函式
    return CreateAtlasFromSprArray2D_CPP(filename, horse_rider, 8, 75);
}

/**
 * @brief 【最終核心函式】接收一個不規則的二維 Spr 陣列，並為其建立一個單一的紋理圖集。
 *
 * @param filename 檔名，用於在管理器中作為 Key。
 * @param ani_array 指向 Spr Ani[][80] 這樣的二維陣列。
 * @param max_ani 一個陣列，定義了每一「行」動畫的實際影格數。
 * @param num_rows ani_array 的第一維大小 (在您的例子中是 7)。
 * @param max_cols ani_array 的第二維大小 (在您的例子中是 80)。
 * @param renderer 遊戲的 SDL 渲染器。
 * @return 成功則返回指向新建的 SDL_Texture 的指標，失敗則返回 nullptr。
 */
SDL_Texture* SDL_DirectDrawInfo::CreateAtlasForChImageNumber_CPP(
    const char* filename,
    const Spr* ani_array, // 直接傳入 &Ani[0][0]
    const int* max_ani,   // 傳入 max_ani 陣列
    int num_rows,         // 傳入 7
    int max_cols)         // 傳入 80
{
#ifndef _LOADSPRITE
  return nullptr;  // 如果沒有載入 Spr 功能，直接返回
#endif
    if (!ani_array || !max_ani) return nullptr;

    SDL_Texture* Atlas = GetOrCreateAtlas(filename);
    if (Atlas != nullptr) return Atlas;

    // --- 步驟 1: 收集 - 將不規則的二維陣列扁平化到一個 vector 中 ---
    std::vector<const Spr*> sprites_to_pack;

    // 忠實地重現原始 LoadChImageNumber 的迴圈邏輯
    for (int a = 1; a < num_rows; ++a) { // 注意：迴圈從 1 開始
        for (int b = 1; b <= max_ani[a]; ++b) { // 迴圈到 max_ani[a] 為止
            if (b >= max_cols) break; // 安全檢查，防止越界

            // 透過指標運算來訪問二維陣列的元素
            const Spr* s = &ani_array[a * max_cols + b];
            if (s->xl > 0 && s->yl > 0 && s->img != nullptr) {
                sprites_to_pack.push_back(s);
            }
        }
    }

    // 按高度降序排序，以優化打包效率
    std::sort(sprites_to_pack.begin(), sprites_to_pack.end(), [](const Spr* a, const Spr* b) {
        return a->yl > b->yl;
        });

    if (sprites_to_pack.empty()) {
        return nullptr;
    }

    // --- 步驟 2：第一階段 - 模擬打包，計算所需圖集的最小尺寸 ---
    int required_width = 0, required_height = 0, current_x = 0, current_y = 0, max_row_height = 0;
    std::unordered_map<const Spr*, SDL_Rect> temp_positions;
    const int PACKING_WIDTH_LIMIT = 2048;

    for (const Spr* s : sprites_to_pack) {
        if (current_x + s->xl > PACKING_WIDTH_LIMIT) {
            current_y += max_row_height;
            current_x = 0;
            max_row_height = 0;
        }
        temp_positions[s] = { current_x, current_y, s->xl, s->yl };
        current_x += s->xl;
        required_width = max(required_width, current_x);
        max_row_height = max(max_row_height, (int)s->yl);
    }
    required_height = current_y + max_row_height;

    // --- 步驟 3: 將尺寸調整為對 GPU 更友善的「2的次方」尺寸 ---
    int atlas_width = NextPowerOfTwo(required_width);
    int atlas_height = NextPowerOfTwo(required_height);

    // --- 步驟 4：第二階段 - 實際建立圖集並拼貼 ---
    SDL_Surface* atlasSurface = SDL_CreateRGBSurfaceWithFormat(0, atlas_width, atlas_height, 16, SDL_PIXELFORMAT_RGB565);
    if (!atlasSurface) return nullptr;

    Uint32 transparentKey = SDL_MapRGB(atlasSurface->format, 0, 248, 0);
    SDL_FillRect(atlasSurface, NULL, transparentKey);
    SDL_SetColorKey(atlasSurface, SDL_TRUE, transparentKey);

    for (const Spr* s : sprites_to_pack) {
        SDL_Surface* tempSurface = CreateSurfaceFromSpr(s, 0); // 假設透明色是黑色
        if (tempSurface) {
            SDL_Rect destRect = temp_positions[s];
            SDL_BlitSurface(tempSurface, NULL, atlasSurface, &destRect);

            TextureInfo info;
            info.atlasTexture = nullptr;
            info.sourceRect = destRect;
            g_TextureCache[s] = info;

            SDL_FreeSurface(tempSurface);
        }
    }

    // --- 步驟 5, 6, 7: 建立紋理，更新快取，並清理 ---
    SDL_Texture* finalAtlasTexture = SDL_CreateTextureFromSurface(renderer, atlasSurface);
    if (finalAtlasTexture) {
        SDL_SetTextureBlendMode(finalAtlasTexture, SDL_BLENDMODE_BLEND);
        g_AtlasManager.emplace(filename, finalAtlasTexture);
        for (const Spr* s : sprites_to_pack) {
            if (g_TextureCache.count(s)) {
                g_TextureCache.at(s).atlasTexture = finalAtlasTexture;
            }
        }
    }

    SDL_FreeSurface(atlasSurface);
    SDL_Log("INFO: 為 %s 建立圖集成功，尺寸: %dx%d，包含 %zu 個 Sprite，地址: %p",
        filename, atlas_width, atlas_height, sprites_to_pack.size(),
        (void*)finalAtlasTexture);
    SDL_Log("INFO: 快取中目前有 %zu 個 Sprite 條目", g_TextureCache.size());
    return finalAtlasTexture;
}

/**
 * @brief 【動態版核心函式】接收已載入的 Sprite 資料，動態計算最佳圖集尺寸，並建立紋理。
 *
 * @param filename 檔名，用於在管理器中作為 Key。
 * @param loaded_spr_array 包含了所有 Sprite 資訊的陣列。
 * @param sprite_count 陣列中有效的 Sprite 數量。
 * @param renderer 遊戲的 SDL 渲染器。
 * @return 成功則返回指向新建的 SDL_Texture 的指標，失敗則返回 nullptr。
 */
SDL_Texture* SDL_DirectDrawInfo::CreateAtlasFromLoadedData(const char* filename, const Spr* loaded_spr_array, int sprite_count)
{
#ifndef _LOADSPRITE
  return nullptr;  // 如果沒有載入 Spr 功能，直接返回
#endif

  SDL_Texture* Atlas = GetOrCreateAtlas(filename);
  if (Atlas != nullptr) return Atlas;

    // --- 步驟 1: 預處理 - 收集所有有效的 Sprite 並按高度降序排序 ---
    std::vector<SpriteInfo> sprites_to_pack;
    for (int i = 0; i < sprite_count; ++i) {
        const Spr* s = &loaded_spr_array[i];
        if (s->xl > 0 && s->yl > 0 && s->img != nullptr) {
            sprites_to_pack.push_back({ s, s->xl, s->yl });
        }
    }

    // 按高度降序排序。這是一個簡單有效的打包優化策略，
    // 先放高的 Sprite，可以讓後續的空間更規整。
    std::sort(sprites_to_pack.begin(), sprites_to_pack.end(), [](const SpriteInfo& a, const SpriteInfo& b) {
        return a.height > b.height;
        });

    if (sprites_to_pack.empty()) {
        return nullptr; // 沒有任何可打包的圖像
    }

    // --- 步驟 2：第一階段 - 模擬打包，計算所需圖集的最小尺寸 ---
    int required_width = 0;
    int required_height = 0;
    int current_x = 0;
    int current_y = 0;
    int max_row_height = 0;

    // 我們需要一個臨時的 map 來儲存模擬打包時計算出的位置
    std::unordered_map<const Spr*, SDL_Rect> temp_positions;

    for (const auto& info : sprites_to_pack) {
        // 判斷是否需要換行
        // 我們假設一個合理的初始寬度，例如 1024，如果超過了就換行
        if (current_x + info.width > 1024) {
            current_y += max_row_height;
            current_x = 0;
            max_row_height = 0;
        }

        // 記錄這個 Sprite 的位置
        temp_positions[info.sprite_ptr] = { current_x, current_y, info.width, info.height };

        // 更新畫筆位置和尺寸記錄
        current_x += info.width;
        required_width = max(required_width, current_x);
        max_row_height = max(max_row_height, info.height);
    }
    required_height = current_y + max_row_height;

    // --- 步驟 3: 將計算出的尺寸調整為對 GPU 更友善的「2的次方」尺寸 ---
    int atlas_width = NextPowerOfTwo(required_width);
    int atlas_height = NextPowerOfTwo(required_height);

    // SDL_Log("檔案 '%s' 動態計算圖集尺寸: %d x %d", filename, atlas_width, atlas_height);

    // --- 步驟 4：第二階段 - 使用計算出的最佳尺寸，實際建立圖集並拼貼 ---
    SDL_Surface* atlasSurface = SDL_CreateRGBSurfaceWithFormat(0, atlas_width, atlas_height, 16, SDL_PIXELFORMAT_RGB565);
    if (!atlasSurface) {
        // SDL_Log("錯誤：無法建立動態圖集 Surface: %s", SDL_GetError());
        return nullptr;
    }
    // 用一個特殊的透明色填充圖集，方便除錯
    Uint32 transparentKey = SDL_MapRGB(atlasSurface->format, 0, 248, 0);
    SDL_FillRect(atlasSurface, NULL, transparentKey);
    SDL_SetColorKey(atlasSurface, SDL_TRUE, transparentKey);

    SDL_Texture* finalAtlasTexture = nullptr; // 先宣告

    // 遍歷已排序的 Sprite，這次是真的把它們貼上去
    for (const auto& info : sprites_to_pack) {
        const Spr* s = info.sprite_ptr;

        // 為小 Sprite 建立一個暫時的 Surface
        SDL_Surface* tempSurface = CreateSurfaceFromSpr(s, 0); // 假設透明色是黑色
        if (!tempSurface) continue;

        // 從我們第一階段計算好的位置，把它貼到大圖集上
        SDL_Rect destRect = temp_positions[s];
        SDL_BlitSurface(tempSurface, NULL, atlasSurface, &destRect);

        // 記錄最終的紋理資訊到全域快取中
        TextureInfo tex_info;
        tex_info.atlasTexture = nullptr; // 最終紋理還沒完全做好
        tex_info.sourceRect = destRect;
        g_TextureCache[s] = tex_info;

        SDL_FreeSurface(tempSurface);
    }

    // --- 步驟 5：送去工廠製版 ---
    finalAtlasTexture = SDL_CreateTextureFromSurface(renderer, atlasSurface);
    if (finalAtlasTexture) {
        SDL_SetTextureBlendMode(finalAtlasTexture, SDL_BLENDMODE_BLEND);

        // 將新建立的紋理圖集與其檔名關聯起來，存入管理器。
        g_AtlasManager.emplace(filename, finalAtlasTexture);
    }

    // --- 步驟 6：更新通訊錄 ---
    for (const auto& info : sprites_to_pack) {
        const Spr* s = info.sprite_ptr;
        if (g_TextureCache.count(s)) {
            g_TextureCache.at(s).atlasTexture = finalAtlasTexture;
        }
    }

    // --- 步驟 7：大掃除 ---
    SDL_FreeSurface(atlasSurface);
    SDL_Log("檔案 '%s' 的動態圖集建立完成: %d x %d", filename, atlas_width, atlas_height);
    SDL_Log("包含 %zu 個 Sprite", sprites_to_pack.size());
    return finalAtlasTexture;
}

void SDL_DirectDrawInfo::LoadMenuIndex(int IndexType, std::string path)
{
    if (IndexType >= 10000)
    {
        if (g_ImageFileMap.count(IndexType-10000) != 0) return;
        g_ImageFileMap[IndexType - 10000] = path;
        return;
    }
    if (g_ImageFileMap.count(IndexType) != 0) return;

    g_ImageFileMap[100] = "./data/sys_god.dat";
    g_ImageFileMap[110] = "./data/sys_time.dat";
    g_ImageFileMap[120] = "./data/sys_membook.dat";
    g_ImageFileMap[130] = "./data/sys_mail.dat";
    g_ImageFileMap[140] = "./data/start_etc.dat";
    g_ImageFileMap[141] = "./data/skill_etc.dat";
    g_ImageFileMap[150] = "./data/sys_chat.dat";
    g_ImageFileMap[160] = "./data/sys_magic.dat";
    g_ImageFileMap[170] = "./data/main_etc.dat";
    // 這裡我們不包含沒有檔案路徑的項目，如 SMALL_MAP
    g_ImageFileMap[190] = "./data/start_map.dat";
    g_ImageFileMap[200] = "./data/small_etc.dat";
    g_ImageFileMap[210] = "./data/skill_menu.dat";
    g_ImageFileMap[220] = "./data/public_bbs.dat";
    g_ImageFileMap[230] = "./data/hunter.dat";
    g_ImageFileMap[240] = "./data/quest_menu.dat";
    g_ImageFileMap[260] = "./data/chcolorchauj.dat";
    g_ImageFileMap[270] = "./data/itemmakeimageuj.dat";
    g_ImageFileMap[280] = "./data/nation.dat";
    g_ImageFileMap[290] = "./data/fight_start.dat";
    g_ImageFileMap[300] = "./data/fight_stop.dat";
    g_ImageFileMap[310] = "./data/vote.dat";
    // GUILD_MARK 沒有呼叫 LoadOpenningSpr
    g_ImageFileMap[330] = "./data/guild00.dat";
    g_ImageFileMap[340] = "./data/guild01.dat";
    g_ImageFileMap[350] = "./data/start_etc2.dat";
    g_ImageFileMap[360] = "./data/guild_etc.dat";
    g_ImageFileMap[370] = "./data/HELP.dat";
    g_ImageFileMap[380] = "./data/bank.dat";
    g_ImageFileMap[390] = "./data/salvation.dat";
    g_ImageFileMap[400] = "./data/salvation_ani.dat";
    g_ImageFileMap[410] = "./data/team_battle.dat";
    g_ImageFileMap[420] = "./data/select_server.dat";
    g_ImageFileMap[430] = "./data/change_money.dat";
    g_ImageFileMap[440] = "./data/g_wait.dat";
    g_ImageFileMap[450] = "./data/merchant.dat";
    g_ImageFileMap[460] = "./data/fight_time.dat";
    g_ImageFileMap[470] = "./data/fight_win.dat";
    g_ImageFileMap[480] = "./data/fight_lose.dat";
    g_ImageFileMap[490] = "./data/main_etc3.dat";
    g_ImageFileMap[500] = "./data/guild_menu.dat";
    g_ImageFileMap[510] = "./data/gm_2nd_skill.dat";
    g_ImageFileMap[520] = "./data/nation_battle_bbs.dat";
    g_ImageFileMap[530] = "./data/NaInfo.dat";
    g_ImageFileMap[540] = "./data/NaVote.dat";
    g_ImageFileMap[550] = "./data/NaBudae.dat";
    g_ImageFileMap[560] = "./data/NaNow.dat";
    g_ImageFileMap[570] = "./data/NaLadder.dat";
    g_ImageFileMap[580] = "./data/Najoin.dat";
    g_ImageFileMap[590] = "./data/Nagoto_war_map.dat";
    g_ImageFileMap[600] = "./data/NaGuest.dat";
    g_ImageFileMap[610] = "./data/NaMsgbox.dat";
    g_ImageFileMap[620] = "./data/NaDonebox.dat";
    g_ImageFileMap[630] = "./data/NaTodayNotice.dat";
    g_ImageFileMap[640] = "./data/NaWarResult.dat";
    g_ImageFileMap[650] = "./data/NaWarstart.dat";
    g_ImageFileMap[730] = "./data/dual_mark.dat";
    g_ImageFileMap[740] = "./data/ItemMakeRare.dat";
    g_ImageFileMap[750] = "./data/NewChat.dat";
    g_ImageFileMap[760] = "./data/dual_img.dat";
    g_ImageFileMap[770] = "./data/TutorialMenu.dat";
    g_ImageFileMap[780] = "./data/rune.dat";
    g_ImageFileMap[790] = "./data/rune01.dat";
    g_ImageFileMap[800] = "./data/rune02.dat";
    g_ImageFileMap[810] = "./data/rune03.dat";
    g_ImageFileMap[820] = "./data/rune04.dat";
    g_ImageFileMap[830] = "./data/rune_default.dat";
    g_ImageFileMap[840] = "./data/skin_menu.dat";
    g_ImageFileMap[850] = "./data/gamble_img.dat";
    g_ImageFileMap[860] = "./data/exp_gauge.dat";
    g_ImageFileMap[870] = "./data/gamble_effect.dat";
    g_ImageFileMap[880] = "./data/ARedist.dat";
    g_ImageFileMap[881] = "./data/LocalJoin.dat";
    g_ImageFileMap[882] = "./data/LWDoneBox.dat";
    g_ImageFileMap[883] = "./data/NaMsgBox.dat";
    g_ImageFileMap[884] = "./data/NaMsgBox.dat"; // 與 883 相同
    g_ImageFileMap[885] = "./data/WarStart.dat";
    g_ImageFileMap[886] = "./data/WarMove.dat";
    g_ImageFileMap[887] = "./data/WarStatus.dat";
    g_ImageFileMap[888] = "./data/MWarInfo.dat";
    g_ImageFileMap[900] = "./data/TutorialMenutmp.dat";
    g_ImageFileMap[910] = "./data/guild_member.dat";
    g_ImageFileMap[920] = "./data/friend_menu.dat";
    g_ImageFileMap[930] = "./data/potal_menu.dat";
    g_ImageFileMap[940] = "./data/party_button.dat";
    g_ImageFileMap[950] = "./data/guild_item.dat";
    g_ImageFileMap[960] = "./data/guild_change_info.dat";
}

Spr* SDL_DirectDrawInfo::GetSpriteFromCache(const char* filename, int nImageNumber)
{
	if (nImageNumber < 0) {
        return nullptr;
    }
    
    // 使用 std::map::find 來尋找快取
    auto it = g_SpriteCache.find(filename);
    
    if (it != g_SpriteCache.end()) {
        // 快取中找到了，現在檢查索引是否有效
        SPRITE_DATA& sprites = it->second;
        if (nImageNumber >= sprites.SpriteArray.size()) {
            return nullptr;
        }
        
        // 使用 .at() 訪問元素，它會自動檢查索引是否越界
        return &sprites.SpriteArray.at(nImageNumber);
    }
    
    // 如果快取中找不到，則嘗試從檔案載入
    return LoadSpriteFromFile(filename, nImageNumber);
}

Spr* SDL_DirectDrawInfo::LoadSpriteFromFile(const char* filename, int nImageNumber)
{
    // [Android] 1. 路徑處理：移除開頭的 ./
    std::string path = filename;
    if (path.find("./") == 0) path = path.substr(2);

    // [Android] 2. 使用 SDL_RWFromFile 開啟檔案
    SDL_RWops* fp = SDL_RWFromFile(path.c_str(), "rb");
    if (!fp) {
        // 開檔失敗，加入 Log 方便除錯
        //SDL_Log("LoadSpriteFromFile Error: Not Found '%s'", path.c_str());
        return nullptr;
    }

    short no;
    // [Android] 3. 讀取數量
    SDL_RWread(fp, &no, sizeof(short), 1);

    // [Android] 4. 跳過 Offset 表 (no * 4 bytes)
    // ifs.seekg -> SDL_RWseek
    SDL_RWseek(fp, no * 4, RW_SEEK_CUR);

    // [Android] 5. 計算剩餘檔案長度
    Sint64 current_pos = SDL_RWtell(fp);
    Sint64 total_file_size = SDL_RWsize(fp);
    long long spr_data_length = total_file_size - current_pos;

    // 如果計算出來的長度異常，做個防呆
    if (spr_data_length <= 0) {
        SDL_RWclose(fp);
        return nullptr;
    }

    SPRITE_DATA temp_spr_vec;

    // 自動管理記憶體
    temp_spr_vec.bufptr.resize(spr_data_length);
    char* buf = temp_spr_vec.bufptr.data();
    temp_spr_vec.SpriteArray.reserve(no); // 預先分配空間

    unsigned int nOff = 0; // DWORD -> unsigned int

    for (int a = 0; a < no; a++) {
        unsigned int size;
        SDL_RWread(fp, &size, sizeof(unsigned int), 1);

        if (size == 0) {
            Spr s{}; // 使用大括號初始化
            temp_spr_vec.SpriteArray.push_back(s);
            continue;
        }

        Spr s;
        // 讀取 Spr 標頭
        SDL_RWread(fp, &s.xl, sizeof(short), 1);
        SDL_RWread(fp, &s.yl, sizeof(short), 1);
        SDL_RWread(fp, &s.ox, sizeof(short), 1);
        SDL_RWread(fp, &s.oy, sizeof(short), 1);
        SDL_RWread(fp, &s.size, sizeof(unsigned int), 1);

        // 檢查緩衝區邊界
        if (nOff + s.size > spr_data_length) {
            SDL_Log("LoadSpriteFromFile Error: Buffer overflow in %s", path.c_str());
            SDL_RWclose(fp); // 記得關閉
            return nullptr;
        }

        // [Android] 6. 讀取圖片數據到 Buffer
        // ifs.read(buf + nOff, s.size) -> SDL_RWread
        SDL_RWread(fp, buf + nOff, 1, s.size);
        s.img = buf + nOff;

        nOff += size; // 這裡加上 size (包含標頭的區塊總長)

#ifdef _TEXTURE
        if (s.xl % 10 == 9) {
            if (s.xl != s.ox * 2) {
                SDL_Log("nImageNumber[%d,%d]", s.xl, s.ox);
                s.xl = s.ox << 1;
                SDL_Log("xl 大小修正為[%d]....", s.xl);
            }
        }
#endif

        temp_spr_vec.SpriteArray.push_back(s);
    }

    // [Android] 7. 關閉檔案
    SDL_RWclose(fp);

    // 將資料移動到 g_SpriteCache
    // 注意：這裡使用原始 filename 當作 Key，確保與其他系統邏輯一致
    g_SpriteCache.emplace(filename, std::move(temp_spr_vec));

    if(nImageNumber >= g_SpriteCache[filename].SpriteArray.size()) {
        return nullptr;
    }

    // 建立 Atlas
    CreateAtlasFromLoadedData(filename, g_SpriteCache[filename].SpriteArray.data(), g_SpriteCache[filename].SpriteArray.size());

    return &g_SpriteCache[filename].SpriteArray.at(nImageNumber); // 返回 Spr 的指標
}

void SDL_DirectDrawInfo::launchfirework(int shot)
{
    if (limit_time > SDL_GetTicks()) {
        if (!(rand() % 18)) {
            g_particle_manager.LaunchFirework(Hero, Mapx, Mapy, shot);
        }
    }
}

// ---------------------------------------------------------
// 通用函式：讀取檔案到記憶體 (回傳 buffer 指標，失敗回傳 nullptr)
// ---------------------------------------------------------
void* LoadFileToMemory(const char* filename, size_t* datasize) {
    // 1. 使用 SDL 開啟檔案 (Android 上會自動讀 APK assets)
    SDL_RWops* file = SDL_RWFromFile(filename, "rb");
    if (!file) {
        SDL_Log("Fatal: Failed to open file: %s", filename);
        return nullptr;
    }

    // 2. 獲取檔案大小
    Sint64 size = SDL_RWsize(file);
    if (size <= 0) {
        SDL_Log("Fatal: File size is 0 or error: %s", filename);
        SDL_RWclose(file);
        return nullptr;
    }

    // 3. 配置記憶體 (多配 1 byte 作為結尾，雖然 TTF 不需要，但良好的習慣)
    char* buffer = (char*)malloc(size + 1);
    if (!buffer) {
        SDL_Log("Fatal: Out of memory loading: %s", filename);
        SDL_RWclose(file);
        return nullptr;
    }

    // 4. 讀取資料
    // SDL_RWread 回傳讀取的區塊數量
    size_t readCount = SDL_RWread(file, buffer, 1, size);

    // 5. 關閉檔案 handle
    SDL_RWclose(file);

    if (readCount != (size_t)size) {
        SDL_Log("Fatal: Read error on file: %s", filename);
        free(buffer);
        return nullptr;
    }

    // 6. 填寫大小並回傳
    if (datasize) *datasize = (size_t)size;
    buffer[size] = '\0'; // 安全結尾

    SDL_Log("Success: Loaded %s to memory (%lld bytes)", filename, size);
    return buffer;
}

void SetGameLanguage() {
    sprintf(font_path, "%s", font_path_chs);

    // [Android 修改] 使用 SDL_RWFromFile 檢查檔案是否存在
    // 注意：font_path_chs 應該是相對路徑，如 "data/font/chs.ttf"
    SDL_RWops* file = SDL_RWFromFile(font_path, "rb");

    if (file == NULL) { // 檔案不存在
        g_SDLInfo.Language = CHT_;
        sprintf(font_path, "%s", font_path_cht);
    } else {
        SDL_RWclose(file); // 檔案存在，記得關閉
    }
}

bool InitFont() {
    const int FONT_SIZE = 15;

    if (TTF_Init() == -1) {
        SDL_Log("FATAL ERROR: SDL_ttf 初始化失敗: %s", TTF_GetError());
        return false;
    }
    SDL_Log("INFO: SDL_ttf initialized successfully.");

    // =========================================================
    // 1. 載入主字型 (SourceHanSans) 到記憶體
    // =========================================================
    size_t mainFontSize = 0;

    // 先清理舊的（如果是重啟遊戲）
    if (g_SDLInfo.g_MainFontBuffer) free(g_SDLInfo.g_MainFontBuffer);

    // 讀取檔案 (這一步會花一點時間，但只會發生一次)
    g_SDLInfo.g_MainFontBuffer = LoadFileToMemory(font_path, &mainFontSize); // font_path 是你的全域變數

    if (!g_SDLInfo.g_MainFontBuffer) {
        SDL_Log("FATAL ERROR: 無法將主字型載入記憶體!");
        return false;
    }

    // A. 建立主線程用的字型 (Layout / GlyphMetrics)
    // 參數 1: 自動釋放 RWops 結構, 但不釋放 Buffer (因為大家共用)
    g_SDLInfo.gFont = TTF_OpenFontRW(SDL_RWFromConstMem(g_SDLInfo.g_MainFontBuffer, mainFontSize),
                                     1, FONT_SIZE);

    if (!g_SDLInfo.gFont) {
        SDL_Log("FATAL ERROR: TTF_OpenFontRW 失敗: %s", TTF_GetError());
        return false;
    }
    SDL_Log("INFO: Font '%s' loaded successfully (From Memory).", font_path);

    // B. 建立粗體字型 (Bold) - 共享同一塊記憶體！
    g_SDLInfo.g_BoldFont = TTF_OpenFontRW(
            SDL_RWFromConstMem(g_SDLInfo.g_MainFontBuffer, mainFontSize), 1, FONT_SIZE);
    if (g_SDLInfo.g_BoldFont) {
        TTF_SetFontStyle(g_SDLInfo.g_BoldFont, TTF_STYLE_BOLD); // 設定為粗體
        // 或者使用你原本的 TTF_SetFontOutline(g_SDLInfo.g_BoldFont, 1);
        SDL_Log("INFO: Bold Font initialized from same memory buffer.");
    }
    // =========================================================
    // 2. 載入 Emoji 字型 到記憶體
    // =========================================================
    // 注意：Emoji 檔案通常很大，一定要讀到記憶體，不然會很卡
    const char *emojiPath = "ttf/Twemoji.Mozilla.ttf";
    size_t emojiFontSize = 0;

    if (g_SDLInfo.g_EmojiFontBuffer) free(g_SDLInfo.g_EmojiFontBuffer);

    g_SDLInfo.g_EmojiFontBuffer = LoadFileToMemory(emojiPath, &emojiFontSize);

    if (g_SDLInfo.g_EmojiFontBuffer) {
        g_SDLInfo.gEmojiFont = TTF_OpenFontRW(
                SDL_RWFromConstMem(g_SDLInfo.g_EmojiFontBuffer, emojiFontSize), 1, FONT_SIZE);

        if (g_SDLInfo.gEmojiFont) {
            SDL_Log("INFO: Emoji Font '%s' loaded successfully (From Memory).", emojiPath);
        } else {
            SDL_Log("WARNING: Failed to open Emoji font RW: %s", TTF_GetError());
        }

        // C. ★★★ 關鍵：建立後台線程專用的字型 ★★★ - 共享同一塊記憶體！
        // 這樣後台 Render 時完全不需要鎖，也不會跟主線程打架
        TTF_Font* TextManagerThreadTTF = TTF_OpenFontRW(
                SDL_RWFromConstMem(g_SDLInfo.g_MainFontBuffer, mainFontSize), 1, FONT_SIZE);

        TTF_Font* TextManagerThreadTTFEmoji = TTF_OpenFontRW(
                SDL_RWFromConstMem(g_SDLInfo.g_EmojiFontBuffer, emojiFontSize), 1, FONT_SIZE);

        AsyncTextManager::Instance().Init(TextManagerThreadTTF, TextManagerThreadTTFEmoji);
        SDL_Log("INFO: Async Thread Font initialized.");
    } else {
        SDL_Log("WARNING: Failed to load Emoji file to memory: %s", emojiPath);
    }

    return true;
}

void PrewarmFontCache() {
    SDL_Log("INFO: Starting Font Cache Pre-warming...");

    // 1. 預熱 ASCII (0-127)
    // 這保證了所有數字、英文、標點符號之後都是 0ms
    for (Uint32 i = 32; i < 127; i++) {
        int advance;
        // 這會觸發快取寫入
        TTF_GlyphMetrics32_Safe(g_SDLInfo.gFont, i, nullptr, nullptr, nullptr, nullptr, &advance);
    }


    // 2. (選用) 預熱常用中文字
    // 你可以列出遊戲中最常用的字，或者直接跑一個範圍
    // 注意：不要跑太多，不然啟動會變慢。這裡示範跑一點點。
    // 常用中文範圍 (一級字庫部分)
    // for (Uint32 i = 0x4E00; i < 0x5000; i++) {
    //    int advance;
    //    TTF_GlyphMetrics32_Safe(g_SDLInfo.gFont, i, nullptr, nullptr, nullptr, nullptr, &advance);
    // }

    SDL_Log("INFO: Font Cache Pre-warming completed.");
}

bool InitSDLGraphics(HWND& hWnd) {
    // 0. 防止重複初始化
    if (g_SDLInfo.window != NULL) {
        SDL_Log("INFO: SDL graphics already initialized. Returning TRUE.");
        return TRUE;
    }

    // =========================================================
    // 1. 設定全域 Hints (在 SDL_Init 之前設定最安全)
    // =========================================================

    if (g_SDLInfo.window != NULL) {
        SDL_Log("INFO: SDL graphics already initialized. Returning TRUE.");
        return TRUE;
    }

    // =========================================================
    // 1. 設定全域 Hints 與 環境變數
    // =========================================================

    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
    SDL_SetHint(SDL_HINT_ANDROID_BLOCK_ON_PAUSE, "0");
    SDL_SetHint(SDL_HINT_ANDROID_TRAP_BACK_BUTTON, "1");

    // 使用 setenv 來指定音效驅動 (解決編譯錯誤)
    SDL_setenv("SDL_AUDIODRIVER", "aaudio", 1);

    // 強制全螢幕並填滿瀏海區域
    SDL_SetHint("SDL_ANDROID_LAYOUT_IN_DISPLAY_CUTOUT_MODE", "1");
    SDL_SetHint(SDL_HINT_ORIENTATIONS, "LandscapeLeft LandscapeRight");
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");


    // =========================================================
    // 2. 初始化 SDL 核心與子系統 (建議順序：Core -> IMG -> Net)
    // =========================================================

    // A. 初始化 SDL 核心 (Video, Audio, Events)
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("FATAL ERROR: SDL_Init failed: %s", SDL_GetError());
        return FALSE;
    }
    SDL_Log("INFO: SDL initialized successfully.");

    SDL_Log("Waiting for Android Window Layout...");
    SDL_Delay(500);
    // B. 初始化 SDL_image (PNG/JPG)
    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if ((IMG_Init(imgFlags) & imgFlags) != imgFlags) {
        SDL_Log("SDL_image could not be initialized! IMG_Error: %s\n", IMG_GetError());
        // 這裡不一定要 return FALSE，視遊戲需求而定，但建議回報錯誤
        return FALSE;
    }

    // C. 初始化 SDL_net
    if (SDLNet_Init() < 0) {
        SDL_Log("SDLNet_Init Error: %s", SDLNet_GetError());
        SDL_Quit();
        return FALSE;
    }
    SDL_Log("INFO: SDL_net initialized successfully.");

    // =========================================================
    // 3. 建立視窗 (Window)
    // =========================================================
    g_SDLInfo.bFullscreen = true;

    // ★ 關鍵設定：
    // 1. 寬高設為 0, 0 -> 讓 SDL 自動抓取手機真實解析度 (如 2856x1280)
    // 2. Flags -> 加入 RESIZABLE 與 ALLOW_HIGHDPI 以獲得最佳兼容性
    g_SDLInfo.window = SDL_CreateWindow(
            "DragonRaja",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            0, 0,
            SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN | SDL_WINDOW_BORDERLESS | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
    );

    if (g_SDLInfo.window == NULL) {
        SDL_Log("FATAL ERROR: SDL_CreateWindow failed: %s", SDL_GetError());
        g_SDLInfo.CleanupSDLGraphics();
        return FALSE;
    }

    // 印出實際視窗大小確認
    int w, h;
    SDL_GetWindowSize(g_SDLInfo.window, &w, &h);
    SDL_Log("SDL window created. Real Resolution: %dx%d", w, h);

    // =========================================================
    // 4. 建立渲染器 (Renderer)
    // =========================================================

    // ★ 關鍵設定：開啟 VSync (PRESENTVSYNC)
    // 這能解決「鼠標動畫過快」和「按鍵反應失靈」的問題，並穩定 FPS
    g_SDLInfo.renderer = SDL_CreateRenderer(g_SDLInfo.window, -1, SDL_RENDERER_ACCELERATED);

    if (g_SDLInfo.renderer == NULL) {
        SDL_Log("FATAL ERROR: SDL_CreateRenderer failed: %s", SDL_GetError());
        // 嘗試 fallback 到軟體渲染 (保險起見)
        SDL_Log("Retrying with SOFTWARE renderer...");
        g_SDLInfo.renderer = SDL_CreateRenderer(g_SDLInfo.window, -1, SDL_RENDERER_SOFTWARE);
        if (g_SDLInfo.renderer == NULL) {
            g_SDLInfo.CleanupSDLGraphics();
            return FALSE;
        }
    }
    SDL_Log("SDL renderer created successfully.");

    SDL_SetRenderDrawBlendMode(g_SDLInfo.renderer, SDL_BLENDMODE_BLEND);
    // ★ 關鍵設定：邏輯解析度
    // 讓 GPU 自動將 1280x720 拉伸到全螢幕 (2856x1280)
    SDL_RenderSetLogicalSize(g_SDLInfo.renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_Log("INFO: Set logical render size to %dx%d.", SCREEN_WIDTH, SCREEN_HEIGHT);
    // =========================================================
    // 5. 建立後緩衝區 (Surface & Texture)
    // =========================================================

    // 維持 RGB565 (16-bit) 以配合遊戲原始素材，提升 Blit 效能
    g_SDLInfo.cpuRenderSurface = SDL_CreateRGBSurfaceWithFormat(
            0, SCREEN_WIDTH, SCREEN_HEIGHT, 16, SDL_PIXELFORMAT_RGB565
    );

    if (!g_SDLInfo.cpuRenderSurface) {
        SDL_Log("FATAL ERROR: Failed to create CPU Render Target Surface: %s", SDL_GetError());
        g_SDLInfo.CleanupSDLGraphics();
        return FALSE;
    }
    SDL_Log("CPU Render Target Surface created successfully (RGB565).");

    // 建立 Streaming Texture (與 Surface 格式保持一致)
    g_SDLInfo.streamingTexture = SDL_CreateTexture(
            g_SDLInfo.renderer,
            g_SDLInfo.cpuRenderSurface->format->format,
            SDL_TEXTUREACCESS_STREAMING,
            g_SDLInfo.cpuRenderSurface->w,
            g_SDLInfo.cpuRenderSurface->h
    );

    // 初始化 ImGui ★★★
    if (!CimGui::Instance().Init(g_SDLInfo.window, g_SDLInfo.renderer)) {
        SDL_Log("ImGui Init Failed!");
        return false;
    }

    // =========================================================
    // 6. 其他子系統初始化
    // =========================================================

    // [Windows Only] 取得 HWND
#ifndef __ANDROID__
    hWnd = g_SDLInfo.SDL_GetWindowHandle();
#endif

    SetGameLanguage();

    if (!InitFont()) {
        SDL_Log("FATAL ERROR: InitFont failed.");
        return FALSE;
    }

    g_SDLInfo.SDL_InitInputBox();
    g_SDLInfo.InitAudioManager();

    // Android 通常隱藏系統鼠標 (使用遊戲內繪製的鼠標)
    SDL_ShowCursor(SDL_DISABLE);

    // 載入粒子貼圖
    g_particle_manager.LoadBaseParticleTexture("ttf/particle.bmp");
    g_fireflySystem.LoadParticleTexture("ttf/particle.bmp");
    PrewarmFontCache();
    //InitGradients();
    return TRUE;
}
// ---------------------------------------------------------
// (選用) 32-bit 版本實作 - 用於支援 Emoji
// ---------------------------------------------------------
int TTF_GlyphIsProvided32_Safe(TTF_Font *font, Uint32 ch) {
    /*if (!font) return 0;
    //std::lock_guard<std::mutex> lock(AsyncTextManager::Instance().fontMutex);
    return TTF_GlyphIsProvided32(font, ch);*/
    if (!font) return 0;

    // 1. 先查快取 (極速，0.0001ms)
    auto it = g_GlyphProvidedCache.find(ch);
    if (it != g_GlyphProvidedCache.end()) {
        return it->second;
    }

    // 2. 快取沒命中，才去呼叫 SDL原始函式 (第一次會慢)
    // 這裡我們完全移除了 lock_guard，因為主線程獨佔這個 font 指標
    int result = TTF_GlyphIsProvided32(font, ch);

    // 3. 存入快取
    g_GlyphProvidedCache[ch] = result;

    return result;
}

int TTF_GlyphMetrics32_Safe(TTF_Font *font, Uint32 ch, int *minx, int *maxx, int *miny, int *maxy, int *advance) {
    /*if (!font) return -1;
    //std::lock_guard<std::mutex> lock(AsyncTextManager::Instance().fontMutex);
    return TTF_GlyphMetrics32(font, ch, minx, maxx, miny, maxy, advance);*/

    if (!font) return -1;

    // 1. 先查快取 (極速)
    // 注意：我們只快取 advance (寬度)，因為 Layout 只需要寬度
    // 如果你需要 minx/maxx 等其他資訊，這個快取結構要改大一點
    auto it = g_GlyphWidthCache.find(ch);
    if (it != g_GlyphWidthCache.end()) {
        if (advance) *advance = it->second;
        return 0; // 成功
    }

    // 2. 快取沒命中，才去呼叫慢速的原始函式
    int result = TTF_GlyphMetrics32(font, ch, minx, maxx, miny, maxy, advance);

    // 3. 存入快取
    if (result == 0 && advance) {
        g_GlyphWidthCache[ch] = *advance;
    }

    return result;
}
// 這是封裝好的安全函式
int TTF_SizeUTF8_Safe(TTF_Font *font, const char *text, int *w, int *h) {
    // 1. 防呆檢查
    if (!font || !text) return -1;

    // 這裡建議設成回傳 0 (成功)，但是寬度為 0
    if (text[0] == '\0') {
        if (w) *w = 0;
        if (h) *h = TTF_FontHeight(font);
        return 0;
    }

    int max_h = TTF_FontHeight(font); // 高度是固定的

    // 2. 避免指標讀取越界 (Dangling Pointer 保護)
    // 這裡用 1024 或更長一點比較保險，防止字串太長被截斷，導致後面測量不準
    const int SAFE_READ_LIMIT = 1024;
    size_t len = strnlen(text, SAFE_READ_LIMIT);

    if (len == 0) {
        if (w) *w = 0;
        if (h) *h = max_h;
        return 0;
    }

    // 3. ★ 關鍵：先清洗字串 ★
    // 這裡建立了一個乾淨的 string 副本，保證裡面都是合法的 UTF-8
   const char* clean_text_str = SanitizeString(text, len);

    int total_w = 0;
    const char *it = clean_text_str;
    const char *end = it + strlen(clean_text_str);

    // 4. 安全遍歷
    while (it < end) {
        Uint32 ch = 0;

        // 因為已經 Sanitize 過了，這裡理論上不會 throw，
        // 但保留 try-catch 作為最後一道防線也無妨
        try {
            // 注意：這裡是傳 it，不是 &it
            ch = utf8::next(it, end);
        } catch (...) {
            it++; // 萬一真的還有漏網之魚，跳過 1 byte
            continue;
        }

        if (ch == 0) break;

        int advance = 0;

        // ★ 關鍵：呼叫我們寫好的、有快取的 Metrics 函式！
        // 這裡會極速查找 unordered_map，不會有 I/O 延遲
        TTF_GlyphMetrics32_Safe(font, ch, nullptr, nullptr, nullptr, nullptr, &advance);

        total_w += advance;
    }

    // 5. ★★★ 這一部分你原本漏掉了！ ★★★
    // 算完的結果必須傳出去
    if (w) *w = total_w;
    if (h) *h = max_h;

    return 0; // 0 代表成功
}

bool isKeyDown(SDL_Scancode scancode) {
    
    if (scancode >= SDL_NUM_SCANCODES || scancode < 0) return false;
    // 檢查並消費「第一次按下」事件
    if (keysJustPressed[scancode]) {
        keysJustPressed[scancode] = false; // 消費掉，避免下次重複判斷
        SDL_Log("DEBUG: Key just pressed: %s", SDL_GetScancodeName(scancode));
        return true;
    }
    return false;
}

bool isKeyPressed(SDL_Scancode scancode) {
    if (keyboardState == nullptr) return false;
    if (scancode <= SDL_SCANCODE_UNKNOWN || scancode >= SDL_NUM_SCANCODES) return false;
    return keyboardState[scancode] != 0;
}

// --- 新增函數：更新 IME 輸入矩形 ---
void updateIMERect(InputBox* inputBox) {

    if (!inputBox || !inputBox->isActive) {
        // 如果沒有激活的輸入框，或者輸入框不活躍，就不需要設定 IME 矩形
        return;
    }

    //const int PADDING = 5; // 文字距離邊框的內邊距
    int cursorPixelX = 0;
    int textH = 0; // 字體高度

    // 1. 計算已確認文字部分的像素寬度
    size_t byte_index_at_cursor = utf8_char_to_byte_index(inputBox->text, inputBox->cursorIndex);
    TTF_SizeUTF8_Safe(g_SDLInfo.gFont, inputBox->text.substr(0, byte_index_at_cursor).c_str(), &cursorPixelX, &textH);

    // 2. 如果有合成文字，則再加上合成文字游標前的像素寬度
    if (!inputBox->compositionText.empty()) {
        int compCursorActualPixelX = 0;
        size_t comp_byte_index_at_cursor = utf8_char_to_byte_index(inputBox->compositionText, inputBox->compositionCursor);
        TTF_SizeUTF8_Safe(g_SDLInfo.gFont, inputBox->compositionText.substr(0, comp_byte_index_at_cursor).c_str(), &compCursorActualPixelX, NULL);

        cursorPixelX += compCursorActualPixelX; // 游標總像素位置 = 已確認文字寬度 + 合成文字游標前寬度
    }
    std::string str = inputBox->text;
    //int count = std::count(str.begin(), str.end(), '\n');
    int x = inputBox->isMultiLine ? -200 : 0;// 多行模式下高度隨行數變化，單行模式下固定高度
   
    SDL_Rect inputRect = {inputBox->x+x, inputBox->y, inputBox->w, inputBox->h};
    SDL_SetTextInputRect(&inputRect);
}

// 一個簡單的輔助函數來檢查字串是否包含非 ASCII 字元
// 這將有效地過濾掉中文、日文、韓文等非ASCII字元
bool containsNonAscii(const std::string& str) {
    for (char c : str) {
        // ASCII 字元範圍是 0-127。
        // 任何大於 127 的字元都可能是非 ASCII 字元的一部分 (例如 UTF-8 編碼的多位元組字元)
        if (static_cast<unsigned char>(c) > 127) {
            return true;
        }
    }
    return false;
}

bool containsNonNumber(const std::string& str) {
    for (char c : str) {
        // ASCII 字元範圍是 0-127。
        // 任何大於 127 的字元都可能是非 ASCII 字元的一部分 (例如 UTF-8 編碼的多位元組字元)
        if (static_cast<unsigned char>(c) < '0' ||
            static_cast<unsigned char>(c) > '9') {
            return true;
        }
    }
    return false;
}

void updateGameLogicKeyPressed(InputBox* active, SDL_KeyboardEvent* e)
{
    if (!active || !active->isVisible) return;

    if (e->repeat != 0) return;

    SDL_Scancode scan_code = e->keysym.scancode;

    if (scan_code == SDL_SCANCODE_RETURN)
    {
        if (active->myID == HWND_1)
        {
            //move to BOXID_LOGIN_PW
            InputBox* box = FindInputBox(HWND_2);
            if (!box) return;
            active->setActive(false);
            box->isActive = true;
            box->isVisible = true;
            activeInputBox = box;
        }
        else if (active->myID == HWND_2)
        {
            //send login data to server;?
            //set all input box visible false

            InputBox* box = FindInputBox(HWND_1);
            if (box)
            {
                if (box->text.length() < 4 || active->text.length() < 6)
                {
                    return;
                }
            }
            //SDL_Log("Enter key down in pw input id: %s pw: %s", box->text.c_str(), active->text.c_str());
            SWD_LOGON_Clicked(MN_LOGIN);

            active->text = "";
            active->cursorIndex = 0;
            active->isActive = false;
            active->isVisible = false;

            if (box) {
                box->text = "";
                box->cursorIndex = 0;
                box->setActive(true);
                activeInputBox = box;
            }
        }
    }
    else if (scan_code == SDL_SCANCODE_TAB)
    {
        if (active->myID == HWND_1)
        {
            //move to BOXID_LOGIN_PW
            InputBox* box = FindInputBox(HWND_2);
            if (box)
            {
                active->isActive = false;

                box->isActive = true;
                box->isVisible = true;
                activeInputBox = box;
            }
            
        }
        else if (active->myID == HWND_2)
        {
            InputBox* box = FindInputBox(HWND_1);
            if (box)
            {
                active->isActive = false;

                box->isActive = true;
                box->isVisible = true;
                activeInputBox = box;
            }
        }
    }
}

// 輔助函式：根據全局游標索引，找出其所在的渲染行和行內字元位置，並更新 InputBox 成員
// font: 用於測量文本寬度的 TTF_Font*
// maxWidth: 文本渲染區域的最大寬度
void updateCursorRenderPosition(InputBox& box, TTF_Font* font, int maxWidth) {
    std::vector<std::string> displayLines = getDisplayLines(box.text, font, maxWidth);

    int currentOriginalTextCharIndex = 0; // 在原始文本 (不計入 \n) 中的字符索引
    int originalTextCursorOffset = 0; // 原始文本中游標之前有多少個 '\n'，這是造成 box.cursorIndex 偏大的偏移量

    box.currentRenderLine = 0;
    box.cursorXInRenderLine = 0;

    // 遍歷原始文本，計算真實的字符索引和 \n 的偏移
    // 這是最重要的一步，將全局 box.cursorIndex 轉換為不含 \n 的相對位置
    int effectiveCursorIndex = 0;
    for (int k = 0; k < box.cursorIndex; ++k) {
        if (box.text[k] == '\n') {
            originalTextCursorOffset++;
        }
        else {
            effectiveCursorIndex++;
        }
    }

    // 遍歷所有顯示行，尋找有效游標索引所在的行
    for (size_t i = 0; i < displayLines.size(); ++i) {
        std::string& currentDisplayLine = displayLines[i];
        int displayLineCharCount = utf8_num_chars(currentDisplayLine);

        if (effectiveCursorIndex >= currentOriginalTextCharIndex &&
            effectiveCursorIndex <= currentOriginalTextCharIndex + displayLineCharCount) {

            box.currentRenderLine = i;
            box.cursorXInRenderLine = effectiveCursorIndex - currentOriginalTextCharIndex;

            // Log for debugging
            // SDL_Log("DEBUG: Cursor FOUND on Line %zu! original_char_idx_start (effective): %d, cursorXInRenderLine: %d", 
            //         box.currentRenderLine, currentOriginalTextCharIndex, box.cursorXInRenderLine);
            return;
        }

        currentOriginalTextCharIndex += displayLineCharCount;
    }

    // 處理游標在整個文本末尾的情況（例如 Enter 後游標在新行開頭）
    if (effectiveCursorIndex == currentOriginalTextCharIndex) {
        if (displayLines.empty()) {
            box.currentRenderLine = 0;
            box.cursorXInRenderLine = 0;
        }
        else {
            box.currentRenderLine = displayLines.size(); // 游標在最後一行之後的新行
            box.cursorXInRenderLine = 0; // 在新行的開頭
        }
        // SDL_Log("DEBUG: Cursor at END of text / new line. Final Line %d, X %d", box.currentRenderLine, box.cursorXInRenderLine);

    }
    else {
        // 如果游標索引超出了已處理的 displayLines 範圍
        // SDL_Log("DEBUG: Cursor position NOT found within displayLines for index %d (effective %d). Resetting to last line end.", 
        //         box.cursorIndex, effectiveCursorIndex);
        if (!displayLines.empty()) {
            box.currentRenderLine = displayLines.size() - 1;
            box.cursorXInRenderLine = utf8_num_chars(displayLines.back());
        }
        else {
            box.currentRenderLine = 0;
            box.cursorXInRenderLine = 0;
        }
    }

    // 重新校驗游標位置，以防萬一計算出負值或越界
    if (box.currentRenderLine < 0) box.currentRenderLine = 0;
    if (box.cursorXInRenderLine < 0) box.cursorXInRenderLine = 0;
    if (box.currentRenderLine >= displayLines.size()) {
        if (!displayLines.empty()) {
            box.currentRenderLine = displayLines.size() - 1;
            box.cursorXInRenderLine = utf8_num_chars(displayLines.back());
        }
        else {
            box.currentRenderLine = 0;
            box.cursorXInRenderLine = 0;
        }
    }
}

// 輔助函式：計算文字對齊後的矩形位置
SDL_Rect AlignText(int HangulAlign, int x_anchor, int y_anchor, int texW, int texH)
{
    SDL_Rect dstrect = { x_anchor, y_anchor, texW, texH }; // 這裡的 x_anchor, y_anchor 是對齊的基準點

    switch (HangulAlign)
    {
    case TA_RIGHT:
    {
        dstrect.x = x_anchor - texW; // 基準點是右邊界，所以 x 往左偏移整個文字寬度
        break;
    }
    case TA_CENTER:
    {
        dstrect.x = x_anchor - (texW / 2); // 基準點是中心，所以 x 往左偏移一半文字寬度
        break;
    }
    case TA_LEFT:
    default:
    {
        // 基準點是左邊界，不需要調整 x
        break;
    }
    }

    // 如果需要垂直置中對齊，可以這樣做：
    // dstrect.y = y_anchor - (texH / 2); // 假設 y_anchor 是垂直中心點

    return dstrect;
}


// 修改函式簽名：接收指標 (ptr) 與長度 (len)
SDL_Surface* getOrCreateTextSurface(const char* text_ptr, size_t text_len, SDL_Color color, TTF_Font* font) {
    // 1. 基礎防呆
    if (!font || !text_ptr || text_len == 0) return nullptr;

    // 2. ★★★ 關鍵修正 ★★★
    // 使用 (指標 + 長度) 來建構 string Key
    // 這確保了 key 裡面只包含這一段文字 (例如只包含 "😊😊")
    // 而不會不小心讀到後面的 "你好嗎" (導致方塊亂碼)
    std::string textKey(text_ptr, text_len);

    // 建構查找用的 Key
    TextRenderKey key = { textKey, color, font };

    // 3. 嘗試從快取中查找 (最快路徑)
    auto it = g_textSurfaceCache.find(key);
    if (it != g_textSurfaceCache.end()) {
        // 更新 LRU 時間
        it->first.lastUsed = std::chrono::steady_clock::now();
        return it->second.get();
    }

    // 4. 如果快取沒有，檢查是否正在生成中
    if (AsyncTextManager::Instance().IsPending(key)) {
        // 正在後台生成中...這幀暫時回傳 nullptr
        return nullptr;
    }

    // 5. 既不在快取，也沒在生成，發出請求
    // 注意：這裡會發生一次 key 的拷貝傳給後台，這是無法避免的，
    // 但因為是在「缺字」時才發生，頻率很低，不影響一般渲染效能。
    AsyncTextManager::Instance().Request(key);

    // 暫時回傳 nullptr，等待下幾幀生成完畢
    return nullptr;
}


SDL_Texture* getOrCreateTexture(int x, int y, const std::string& text, SDL_Color color, TTF_Font* font) {


    if (text == "" || text.length() <=0) return nullptr;

    int PADDING = 3;
    // 創建 Key
    TextRenderKey key = { text, color ,font};
    
    auto it = g_textTextureCache.find(key);
    if (it != g_textTextureCache.end()) { 
        
        it->first.lastUsed = std::chrono::steady_clock::now();
        // 找到相同的文字表面，直接返回
        //SDL_Log("g_textTextureCache get cache text %s", text.c_str());
        //更新座標
        //SDL_Rect dstrect = AlignText(HangulAlign, x, y - PADDING, it->first.dstRect.w, it->first.dstRect.h);
		//key.dstRect = dstrect; // 更新 dstRect
        return it->second.get(); // 返回原始指針
    }

    // 渲染文字到新的 SDL_Surface
    // 你可以根據需要選擇渲染模式 (Solid, Shaded, Blended)
    SDL_Surface* newSurface = getOrCreateTextSurface(text.c_str(),text.length(), color, g_SDLInfo.gFont);
    if (!newSurface) {
        SDL_Log("Failed to render text surface: %s", TTF_GetError());
        return nullptr;
    }

    SDL_Texture* newTexture = SDL_CreateTextureFromSurface(g_SDLInfo.renderer, newSurface);
    if (!newTexture)
    {
        SDL_Log("Failed to render text newTexture: %s", TTF_GetError());
        return nullptr;
    }

    //SDL_Rect dstrect = AlignText(HangulAlign, x, y - PADDING, newSurface->w, newSurface->h);
    // 創建一個新的 Key，並設置其 lastUsed 時間
    TextRenderKey new_key = { text, color , font};
    //new_key.dstRect = dstrect;
    new_key.lastUsed = std::chrono::steady_clock::now();
    // 使用 try_emplace 插入新元素
    auto [inserted_it, did_insert] = g_textTextureCache.emplace(
        new_key, // 鍵參數
        std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>(newTexture, &SDL_DestroyTexture)); // 值參數

    return inserted_it->second.get();
}

// 渲染文字的函式
void SDL_HprintOutLine(const int x, const int y, const char* szStr, SDL_Color textColor, int HangulAlign)
{
    return;

    if (strlen(szStr) <= 0) {
        // 如果是空字串，沒有渲染的必要，直接返回
        return;
    }

    if (textColor.a != 255) textColor.a = 255;
#ifdef _TEXTURE
    RenderTextWithTextureFallback(x, y, szStr, textColor, g_SDLInfo.g_BoldFont, g_SDLInfo.gEmojiFont);
#else
    RenderTextWithFallback(x, y, szStr, textColor, g_SDLInfo.g_BoldFont, g_SDLInfo.gEmojiFont);
#endif
}

void SDL_Hprint(const int x, const int y, const char* szStr, SDL_Color textColor, int HangulAlign)
{
    if (strlen(szStr) <= 0) {
        // 如果是空字串，沒有渲染的必要，直接返回
        return;
    }

    if (textColor.a != 255) textColor.a = 255;
#ifdef _TEXTURE
    RenderTextWithTextureFallback(x, y, szStr, textColor, g_SDLInfo.gFont, g_SDLInfo.gEmojiFont);
#else
    RenderTextWithFallback(x, y, szStr, textColor, g_SDLInfo.gFont, g_SDLInfo.gEmojiFont);
#endif
}

void SDL_HprintBold(const int x, const int y, const char* szStr, SDL_Color textColor, int HangulAlign)
{
    if (strlen(szStr) <= 0) return;
#ifdef _TEXTURE
    RenderTextWithTextureFallback(x, y, szStr, textColor, g_SDLInfo.gFont, g_SDLInfo.gEmojiFont);
#else
    // The new fallback logic, now using the bold font as the base.
    RenderTextWithFallback(x, y, szStr, textColor, g_SDLInfo.gFont, g_SDLInfo.gEmojiFont);
#endif
}

void SDL_DrawRect(const int x, const int y, const int w, const int h, SDL_Color color){

    SDL_Rect rect = { x, y, w, h};
    SDL_FillRect(g_SDLInfo.cpuRenderSurface, &rect, SDL_MapRGB(g_SDLInfo.cpuRenderSurface->format, color.r, color.g, color.b));
}

InputBox* FindInputBox(int BoxID)
{
    // 假設 inputBoxes 是一個全局的 std::vector<InputBox>
    // 並且 InputBox 類有一個 myID 成員和一個 operator==(int) 重載
    auto it = std::find(inputBoxes.begin(), inputBoxes.end(), BoxID);

    if (it != inputBoxes.end()) {
        //SDL_Log("INFO: Found BoxID: %d", (it)->myID); // 替換 std::cout
        return &(*it);
    }
    else {
        // SDL_Log("INFO: BoxID not found: %d", BoxID); // 替換 std::cout，如果希望顯示未找到的調試信息
    }
    return nullptr;

}

// 這是與您的渲染邏輯 1:1 對應的測量函式
int GetTextWidthWithFallback(const char* utf8_text, TTF_Font* baseFont, TTF_Font* fallbackFont) {
    // 1. 基本檢查
    if (!utf8_text || *utf8_text == '\0' || !baseFont) return 0;

    const int MAX_TEXT_LENGTH = 120;
    size_t len = strnlen(utf8_text, MAX_TEXT_LENGTH);
    if (len == 0) return 0;

    // 2. ★ 關鍵：使用同樣的 Sanitize 邏輯 ★
    // 如果這裡不洗，而渲染時洗了，長度就會對不上
    const char* clean_text_str = SanitizeString(utf8_text, len);

    const char* safe_it = clean_text_str;
    const char* safe_end = safe_it + strlen(clean_text_str);
    const char* start_of_segment = safe_it;

    // 無 fallback 模式 (直接計算總寬度)
    if (!fallbackFont) {
        int w = 0;
        TTF_SizeUTF8_Safe(baseFont, clean_text_str, &w, NULL);
        return w;
    }

    int total_width = 0;

    // 取得第一個字的字型
    const char* temp_it = safe_it;
    Uint32 first_codepoint = utf8::next(temp_it, safe_end);
    TTF_Font* current_font = TTF_GlyphIsProvided(baseFont, first_codepoint) ? baseFont : fallbackFont;

    while (safe_it < safe_end) {
        const char* segment_start_it = safe_it;
        Uint32 codepoint = utf8::next(safe_it, safe_end);

        if (codepoint == 0) break;

        TTF_Font* required_font = TTF_GlyphIsProvided(baseFont, codepoint) ? baseFont : fallbackFont;

        // 當字型切換時，結算上一段的寬度
        if (required_font != current_font) {
            if (segment_start_it > start_of_segment) {
                std::string segment_str(start_of_segment, segment_start_it - start_of_segment);

                int segment_w = 0;
                TTF_SizeUTF8_Safe(current_font, segment_str.c_str(), &segment_w, NULL);

                // ★ 這裡不用加 XPADDING，因為您的渲染邏輯中，
                // current_x += surface->w; 並沒有加上 XPADDING。
                // 如果您希望游標也把那個空隙算進去，
                // 請在渲染函式的 current_x += ... 補上 XPADDING，
                // 然後這裡也加上: if (current_font == fallbackFont) total_width += 2;

                if (current_font == fallbackFont) total_width += 2;
                total_width += segment_w;
            }
            start_of_segment = segment_start_it;
            current_font = required_font;
        }
    }

    // 結算最後一段
    if (start_of_segment < safe_end) {
        std::string segment_str(start_of_segment, safe_end - start_of_segment);
        int segment_w = 0;
        TTF_SizeUTF8_Safe(current_font, segment_str.c_str(), &segment_w, NULL);
        total_width += segment_w;
    }

    return total_width;
}

// 新的游標渲染函式
void renderMultiLineCursor(InputBox& box, TTF_Font* font, int currentPixelY,
                           int cursorPixelX) {
  if (!box.isActive || !box.cursorVisible) {
    return;
  }

  // 獲取游標顏色
  Uint32 cursorColorValue =
      SDL_MapRGB(g_SDLInfo.cpuRenderSurface->format, box.textColor.r,
                 box.textColor.g, box.textColor.b);

  // 創建游標矩形
  const int PADDING = 4;
  int fontHeight = TTF_FontHeight(font);
  SDL_Rect cursorRect = {
      box.x + PADDING + cursorPixelX,
      box.y + currentPixelY-1, 2,
      fontHeight - 5};
#ifdef _TEXTURE
  SDL_SetRenderDrawColor(g_SDLInfo.renderer, box.textColor.r, box.textColor.g, box.textColor.b, 255); // 設定繪圖顏色
  SDL_RenderFillRect(g_SDLInfo.renderer, &cursorRect);
#else
  // 渲染游標
  SDL_FillRect(g_SDLInfo.cpuRenderSurface, &cursorRect, cursorColorValue);
#endif
}

// 完整的 renderInputBox 函式，支援多行顯示、游標閃爍、顏色自定義和地圖座標兼容性
void renderInputBox(InputBox& box, TTF_Font* font) {
    if (!box.isVisible) {
        return;
    }
    const int FONT_COLOR_LIGHT_BLUE = RGB(173, 216, 230);  // 淺藍色
    // --- 渲染背景 ---
    //SDL_Rect fillRect = { box.x, box.y, box.w, box.h };
    //Uint32 bgColorValue = SDL_MapRGB(g_SDLInfo.cpuRenderSurface->format, box.boxColor.r, box.boxColor.g, box.boxColor.b);
    //SDL_FillRect(g_SDLInfo.cpuRenderSurface, &fillRect, bgColorValue);
    //SDL_RenderDrawRect(g_SDLInfo.renderer, &fillRect);
    // --- 渲染邊框 ---
    /*int border_int_color = 0;
    if (box.isActive) {
        border_int_color = RGB(box.activeBorderColor.r, box.activeBorderColor.g, box.activeBorderColor.b);
    }
    else {
        border_int_color = RGB(box.borderColor.r, box.borderColor.g, box.borderColor.b);
    }*/

    
    // 呼叫 Line 函式繪製邊框，並設置 is_screen_coords 為 true (UI 元素)
    //Line(fillRect.x, fillRect.y, fillRect.x + fillRect.w - 1, fillRect.y, border_int_color, true); // 頂邊
    //Line(fillRect.x, fillRect.y + fillRect.h - 1, fillRect.x + fillRect.w - 1, fillRect.y + fillRect.h - 1, border_int_color, true); // 底邊
    //Line(fillRect.x, fillRect.y, fillRect.x, fillRect.y + fillRect.h - 1, border_int_color, true); // 左邊
    //Line(fillRect.x + fillRect.w - 1, fillRect.y, fillRect.x + fillRect.w - 1, fillRect.y + fillRect.h - 1, border_int_color, true); // 右邊
    Hcolor(FONT_COLOR_WHITE);
    int PADDING = 0;
    int fontHeight = TTF_FontHeight(font);
    int maxWidth = box.w - 2 * PADDING;

    // --- 游標閃爍邏輯  ---
    Uint32 currentTime = SDL_GetTicks();
    if (box.isActive) {
      if (currentTime - box.lastCursorToggleTime >= box.CURSOR_BLINK_RATE) {
        box.cursorVisible = !box.cursorVisible;
        box.lastCursorToggleTime = currentTime;
      }
    } else {
      box.cursorVisible = false;
    }

    // --- 多行渲染邏輯 ---
    if (box.isMultiLine) {
      PADDING = 4;  // 多行模式下增加內邊距
      updateCursorRenderPosition(box, font, maxWidth);
      std::vector<std::string> displayLines =
          getDisplayLines(box.text, font, maxWidth);
      //int visibleLines = (box.h - 2 * PADDING) / fontHeight;

      // ... (滾動偏移量處理邏輯) ...
      
      int currentPixelY = PADDING;
      int preCom = 0;
      int preComX = 0;
      for (size_t i = 0; i < displayLines.size(); ++i) {
        if ((int)i < box.scrollOffsetLines) {
          continue;
        }
        
        if (currentPixelY + fontHeight > box.h - PADDING) {
          break;
        }
        std::string lineToRender = displayLines[i];
        bool isCursorRenderLine = ((int)i == box.currentRenderLine);
        int cursorPixelX = 0;
        std::string preComposition = "";
        std::string postComposition = "";

        // --- 文本渲染：處理 IME 合成文字 或 完整行 ---
        if (box.isActive && isCursorRenderLine &&
            !box.compositionText.empty()) {
          // 有合成文字：拆分渲染 前綴 + 合成文字 + 後綴
          preComposition =
              utf8_substr(lineToRender, 0, box.cursorXInRenderLine);
          postComposition = utf8_substr(lineToRender, box.cursorXInRenderLine);

          int preCompWidth = GetTextWidthWithFallback(preComposition.c_str(), font,
                                                  g_SDLInfo.gEmojiFont);
          int compWidth = GetTextWidthWithFallback(box.compositionText.c_str(),
                                               font, g_SDLInfo.gEmojiFont);

          if (!preComposition.empty()) {
            HprintBold(box.x + PADDING, box.y + currentPixelY,
                       RGB(255, 255, 255), 0,
                       const_cast<char*>(preComposition.c_str()));
          }
          if (i > 0 && box.text.back() == '\n') {
            preCom = fontHeight;
            preComX = 0;
          } else {
            preCom = 0;
            preComX = box.x + PADDING + preCompWidth;
          }
          HprintBold(preComX, box.y + currentPixelY + preCom,
                     FONT_COLOR_LIGHT_BLUE, 0,
                     const_cast<char*>(box.compositionText.c_str()));
          if (!postComposition.empty()) {
            HprintBold(preComX + compWidth,
                       box.y + currentPixelY + preCom,
                       FONT_COLOR_LIGHT_BLUE, 0,
                       const_cast<char*>(postComposition.c_str()));
          }

          // 計算游標 X 座標
          cursorPixelX = preCompWidth + GetTextWidthWithFallback(
                                            utf8_substr(box.compositionText, 0,
                                                        box.compositionCursor)
                                                .c_str(),
                                            font, g_SDLInfo.gEmojiFont);

        } else {
          // 沒有合成文字，或不是游標所在渲染行
          if (!lineToRender.empty()) {
            Hprint(box.x + PADDING, box.y + currentPixelY, nullptr,
                   lineToRender.c_str());
          }
          // 計算游標 X 座標
          cursorPixelX = GetTextWidthWithFallback(
              utf8_substr(lineToRender, 0, box.cursorXInRenderLine).c_str(),
              font, g_SDLInfo.gEmojiFont);
        }

        // 渲染游標（只在游標所在行渲染）
        if (isCursorRenderLine) {
          if (!box.text.empty() && box.text.back() == '\n') {
              renderMultiLineCursor(box, font, currentPixelY+fontHeight,0);
          }
          else renderMultiLineCursor(box, font, currentPixelY, cursorPixelX);
        }

        currentPixelY += fontHeight;
      }

      // --- 處理空行游標 (包括 Enter 後的新空行) ---
      // 當前游標所在行超出了已渲染的文本行數，表示游標在一個新行
      if (box.isActive && box.cursorVisible &&(!box.text.empty()&&box.text.back() == '\n')) {
        // 計算新行所在的 Y 座標
        int cursorY =
            PADDING +
            (box.currentRenderLine - box.scrollOffsetLines+1) * fontHeight;
        // 確保新行在可見範圍內才渲染游標
        if (cursorY + fontHeight <= box.h - PADDING) {
          renderMultiLineCursor(box, font, cursorY, 0);  // X 座標為 0
        }
      }
    } else { // --- 單行渲染邏輯 ---
        std::string textToRender = "";
        if (box.isPassword) {
            box.passwordDisplay = "";
            for (char const& c : box.text) { // 如果 box.text 存儲的是 UTF-8，這裡應該用 utf8_num_chars 來計算星號數量
                box.passwordDisplay += "*";
            }
            textToRender = box.passwordDisplay;
        }
        else {
            textToRender = box.text;
        }
        PADDING = 3;
        int Y_PADDING = 4;
        
        if (box.myID == HWND_1 || box.myID == HWND_2) {
          PADDING =4;
          Y_PADDING=fontHeight/3;
        }

        // 獲取游標前的文字
        std::string prefix = utf8_substr(textToRender, 0, box.cursorIndex);

        // 獲取游標後的文字
        std::string suffix = utf8_substr(textToRender, box.cursorIndex);

        int currentX = box.x + PADDING;
        int currentY = box.y + Y_PADDING;

        // 1. 渲染游標前的文字
        if (!prefix.empty()) {
            Hprint(currentX, currentY, nullptr, prefix.c_str());
            currentX += GetTextWidthWithFallback(prefix.c_str(), font, g_SDLInfo.gEmojiFont);
        }

        // 2. 渲染輸入法預覽文字 (如果有)
        if (!box.compositionText.empty()) {
            // 使用不同的顏色或樣式來顯示預覽文字
            Hcolor(173, 216, 230);
            Hprint(currentX, currentY, nullptr, box.compositionText.c_str());
            currentX += GetTextWidthWithFallback(box.compositionText.c_str(), font, g_SDLInfo.gEmojiFont);
            Hcolor(FONT_COLOR_WHITE); // 記得切換回原本的顏色
        }

        // 3. 渲染游標後的文字
        if (!suffix.empty()) {
            Hprint(currentX, currentY, nullptr, suffix.c_str());
        }
        //Hprint(box.x + PADDING, box.y+Y_PADDING, nullptr, textToRender.c_str());

        int textWidth = GetTextWidthWithFallback(textToRender.c_str(), font, g_SDLInfo.gEmojiFont);
        int maxTextWidth = box.w - 2 * PADDING; // 使用 box.rect.w
        int clipStartChar = 0;

        if (textWidth > maxTextWidth) {
            int currentRenderCursorPos = box.cursorIndex + utf8_num_chars(box.compositionText);
            int currentPixelWidth = 0;
            for (int i = currentRenderCursorPos; i >= 0; --i) {
                int charWidth = GetTextWidthWithFallback(utf8_substr(textToRender, i, 1).c_str(), font, g_SDLInfo.gEmojiFont);
                currentPixelWidth += charWidth;
                if (currentPixelWidth > maxTextWidth) {
                    clipStartChar = i + 1;
                    break;
                }
            }
        }
        
        // 渲染游標 (到 g_SDLInfo.cpuRenderSurface)
        if (box.isActive) {
            Uint32 cursorColorValue = SDL_MapRGB(g_SDLInfo.cpuRenderSurface->format, box.textColor.r, box.textColor.g, box.textColor.b);
            int cursorPixelX = GetTextWidthWithFallback(utf8_substr(textToRender, clipStartChar, box.cursorIndex - clipStartChar).c_str(), font, g_SDLInfo.gEmojiFont);
            int compCursorWidth = GetTextWidthWithFallback(utf8_substr(box.compositionText, 0, box.compositionCursor).c_str(), font, g_SDLInfo.gEmojiFont);
            cursorPixelX += compCursorWidth;

            SDL_Rect cursorRect = {
                box.x + PADDING + cursorPixelX, box.y + Y_PADDING-2, 2,
                fontHeight-6};  // 使用 box.rect.x, box.y, box.h
            if (box.cursorVisible) {
#ifdef _TEXTURE
                SDL_SetRenderDrawColor(g_SDLInfo.renderer, box.textColor.r, box.textColor.g, box.textColor.b, 255); // 設定繪圖顏色
                SDL_RenderFillRect(g_SDLInfo.renderer, &cursorRect);
#else
                SDL_FillRect(g_SDLInfo.cpuRenderSurface, &cursorRect, cursorColorValue);
#endif
            }
        }
    }
}

bool handleCharacterContxtMenuClick(int mouseX, int mouseY) {
  // --- 新增：右鍵選單點擊邏輯 ---
  if (CharContxtMenu.IsContextMenuOpen()) {
    // 如果選單已開啟，則這次點擊由選單處理
    CharContxtMenu.HandleContextMenuMouseClick(mouseX, mouseY);
    return false;  // 中斷事件，不讓點擊穿透到遊戲世界
  } else {
    // 如果選單未開啟，檢查是否點擊了某個名字
    LPCHARACTER ch = g_GameInfo.lpcharacter;
    if (ch && !ch->IsNpc() && ch != Hero && Hero->peacests <= 0) {
      // 如果點中了，就打開選單
      CharContxtMenu.OpenContextMenu(ch, mouseX+32, mouseY);
      return false;  // 中斷事件
    }
  }
  return true;
  // --- 選單邏輯結束 ---
}

bool handleChatConetxtMenuClick(int mouseX, int mouseY) {
  // --- 新增：右鍵選單點擊邏輯 ---
  if (ChatMgr.IsContextMenuOpen()) {
    // 如果選單已開啟，則這次點擊由選單處理
    ChatMgr.HandleContextMenuMouseClick(mouseX, mouseY);
    return false;  // 中斷事件，不讓點擊穿透到遊戲世界
  } else {
    // 如果選單未開啟，檢查是否點擊了某個名字
    CChatData* clicked_chat = ChatMgr.CheckNameClick(mouseX, mouseY);
    if (clicked_chat) {
      // 如果點中了，就打開選單
      ChatMgr.OpenContextMenu(clicked_chat, mouseX, mouseY);
      return false;  // 中斷事件
    }
  }
  return true;
  // --- 選單邏輯結束 ---
 }

void handleMouseInputBoxActivation(int mouseX, int mouseY) {
    bool clickedOnAnyBox = false;

    // 1. 檢查是否點擊了任何輸入框
    for (auto& box : inputBoxes) {
        if (mouseX >= box.x && mouseX <= box.x + box.w &&
            mouseY >= box.y && mouseY <= box.y + box.h && box.isVisible) {

            clickedOnAnyBox = true;

            // 如果點擊的是「已經激活」的框，什麼都不做
            if (box.isActive) {
                activeInputBox = &box;
                updateIMERect(activeInputBox);
            }
                // 如果點擊的是「未激活」的框 -> 激活它
            else {
                if (activeInputBox) {
                    activeInputBox->isActive = false;
                    activeInputBox->compositionText.clear();
                    activeInputBox->compositionCursor = 0;
                    activeInputBox->compositionSelectionLength = 0;
                }
                box.isActive = true;
                activeInputBox = &box;
                activeInputBox->cursorIndex = utf8_num_chars(activeInputBox->text);

                // ★ 只有當輸入法還沒開的時候，才去開
                if (!SDL_IsTextInputActive()) {
                    SDL_StartTextInput();
                }
                updateIMERect(activeInputBox);
            }
            break;
        }
    }

    // 2. ★★★ 關鍵修正：點擊地面時的邏輯 ★★★
    // 只有當「沒點到任何框」且「目前有激活的框」時，才執行關閉邏輯
    if (!clickedOnAnyBox) {
        if (activeInputBox != nullptr) {
            // 確實有一個框是開著的，現在要把它關掉
            activeInputBox->isActive = false;
            activeInputBox->compositionText.clear();
            activeInputBox->compositionCursor = 0;
            activeInputBox->compositionSelectionLength = 0;
            activeInputBox = nullptr;

            // 呼叫 JNI 關閉鍵盤 (這會導致一次必要的卡頓，因為是收起鍵盤)
            SDL_StopTextInput();
        }
        else {
            // ★★★ 救命恩人 ★★★
            // 如果本來就沒有激活的框 (activeInputBox == nullptr)
            // 絕對！絕對！不要呼叫 SDL_StopTextInput()！
            // 這樣點擊地面走路時，就不會去騷擾 Android 系統，也就不會卡頓了！
        }
    }
}

void handleRightArrowKey(InputBox* activeInputBox, SDL_KeyboardEvent& keyEvent)
{
  if (activeInputBox->cursorIndex < utf8_num_chars(activeInputBox->text)) {
    activeInputBox->cursorIndex++;
    activeInputBox->lastCursorToggleTime = 0;
  }
}

void handleLeftArrowKey(InputBox* activeBox, SDL_KeyboardEvent& keyEvent)
{
  if (activeInputBox->cursorIndex > 0) {
    activeInputBox->cursorIndex--;
    activeInputBox->lastCursorToggleTime = 0;
  }
}

void handleDeleteKey(InputBox* activeBox, SDL_KeyboardEvent& keyEvent)
{
    if (!activeInputBox->compositionText.empty()) {
        activeInputBox->compositionText.clear();
        activeInputBox->compositionCursor = 0;
        activeInputBox->compositionSelectionLength = 0;
    }
    else if (!activeInputBox->text.empty() && activeInputBox->cursorIndex < utf8_num_chars(activeInputBox->text)) {
        size_t byte_index = utf8_char_to_byte_index(activeInputBox->text, activeInputBox->cursorIndex);
        size_t next_byte_index = utf8_char_to_byte_index(activeInputBox->text, activeInputBox->cursorIndex + 1);
        activeInputBox->text.erase(byte_index, next_byte_index - byte_index);
        
    }
}

void handleBackspaceKey(InputBox* activeBox, SDL_KeyboardEvent& keyEvent)
{
    if (!activeInputBox->compositionText.empty()) {
        if (activeInputBox->compositionCursor > 0) {
            size_t byte_index = utf8_char_to_byte_index(activeInputBox->compositionText, activeInputBox->compositionCursor);
            size_t prev_byte_index = utf8_char_to_byte_index(activeInputBox->compositionText, activeInputBox->compositionCursor - 1);
            activeInputBox->compositionText.erase(prev_byte_index, byte_index - prev_byte_index);
            activeInputBox->compositionCursor--;
        }
        activeInputBox->compositionSelectionLength = 0;
    }
    else if (!activeInputBox->text.empty() && activeInputBox->cursorIndex > 0) {
        size_t byte_index = utf8_char_to_byte_index(activeInputBox->text, activeInputBox->cursorIndex);
        size_t prev_byte_index = utf8_char_to_byte_index(activeInputBox->text, activeInputBox->cursorIndex - 1);
        activeInputBox->text.erase(prev_byte_index, byte_index - prev_byte_index);
        activeInputBox->cursorIndex--;
        
    }
}

void handleReturnKey(InputBox* activeBox, SDL_KeyboardEvent& keyEvent)
{
    if (activeInputBox->isMultiLine && activeInputBox->compositionText.empty()) {
        activeInputBox->text.insert(utf8_char_to_byte_index(activeInputBox->text, activeInputBox->cursorIndex), "\n");
        activeInputBox->cursorIndex++;
        
    }
    else if (!activeInputBox->isMultiLine) {
        updateGameLogicKeyPressed(activeInputBox, &keyEvent); // 注意這裡的 e 參數，可能需要調整
    }
}

void handleCtrl_C_copy(InputBox* activeBox) {

    if (activeBox && !activeBox->text.empty()) {
        SDL_SetClipboardText(activeBox->text.c_str());
        AddCurrentStatusMessage(FONT_COLOR_NORMAL, "Copied text!");
    }
}
// === 新增：處理 Ctrl+V 貼上功能 ===
void handleCtrl_V_paste(InputBox* activeBox)
{
    if (!activeBox) return;

    if (SDL_HasClipboardText()) {
      char* raw_clipboard_text = SDL_GetClipboardText();
      if (raw_clipboard_text) {

        std::string clipboard_str(raw_clipboard_text);
        SDL_free(raw_clipboard_text);  // 立即釋放，防止記憶體洩漏
        //如果只接受數字的輸入框,但剪貼簿有文字則不複製 return
        if (!activeInputBox->acceptNonAscii&&containsNonAscii(clipboard_str)) {
          return;
        }
        std::string text_to_insert = clipboard_str;
        // 1. 如果設定了最大長度，則在插入前計算並截斷
        if (activeInputBox->maxTextLength > 0) {
          size_t current_chars = utf8_num_chars(activeInputBox->text);
          size_t clipboard_chars = utf8_num_chars(clipboard_str);

          if (current_chars + clipboard_chars > activeInputBox->maxTextLength) {
            size_t chars_to_keep =
                activeInputBox->maxTextLength - current_chars;
            text_to_insert = utf8_substr(clipboard_str, 0, chars_to_keep);
          }
        }
        // 2. 如果確實有文字需要插入
        if (!text_to_insert.empty()) {
          // 將字元位置的游標，轉換為位元組位置
          size_t byte_pos = utf8_char_to_byte_index(
              activeInputBox->text, activeInputBox->cursorIndex);
          // 在指定位元組位置插入文字
          activeInputBox->text.insert(byte_pos, text_to_insert);
          // 3. 更新游標位置
          activeInputBox->cursorIndex += utf8_num_chars(text_to_insert);
        }
        //SDL_StartTextInput();
        updateIMERect(activeInputBox);
      }
    }
}
// === 貼上功能結束 ===
void handleKeyboardInput(SDL_KeyboardEvent& keyEvent, InputBox* activeBox) {
    if (keyEvent.repeat == 0) {
        keysJustPressed[keyEvent.keysym.scancode] = true;
        g_SDLInfo.keyboardEventThisframe = keyEvent;
    }

    if (activeInputBox && activeInputBox->isVisible) {
        bool cursorMoved = false;
        SDL_Keycode key = keyEvent.keysym.sym;

        switch (key) {
        case SDLK_RETURN:
            handleReturnKey(activeInputBox, keyEvent);
            cursorMoved = true; // Enter 可能會換行，視為游標移動
            break;
        case SDLK_BACKSPACE:
            handleBackspaceKey(activeInputBox, keyEvent);
            cursorMoved = true;
            break;
        case SDLK_DELETE:
            handleDeleteKey(activeInputBox, keyEvent);
            cursorMoved = true;
            break;
        case SDLK_LEFT:
            handleLeftArrowKey(activeInputBox, keyEvent);
            cursorMoved = true;
            break;
        case SDLK_RIGHT:
            handleRightArrowKey(activeInputBox, keyEvent);
            cursorMoved = true;
            break;
        case SDLK_TAB:
            // Tab 鍵通常不移動游標，但會觸發其他遊戲邏輯
            updateGameLogicKeyPressed(activeInputBox, &keyEvent);
            break;
        case SDLK_v:
            if (SDL_GetModState() & KMOD_CTRL) {
                handleCtrl_V_paste(activeInputBox);
                cursorMoved = true; // 貼上後游標會移動
            }
            break;
        case SDLK_c:
            if (SDL_GetModState() & KMOD_CTRL) {
                handleCtrl_C_copy(activeInputBox);
            }
            break;
        /*case SDLK_HOME:
            handleHomeKey(activeInputBox);
            cursorMoved = true;
            break;
        case SDLK_END:
            handleEndKey(activeInputBox);
            cursorMoved = true;
            break;
        case SDLK_UP:
            handleUpArrowKey(activeInputBox);
            cursorMoved = true;
            break;
        case SDLK_DOWN:
            handleDownArrowKey(activeInputBox);
            cursorMoved = true;
            break;*/
        default:
            // 如果需要，可以在這裡處理其他通用按鍵
            break;
        }

        if (cursorMoved) {
            //SDL_StartTextInput();
            updateIMERect(activeInputBox);
        }
    }
}

void handleTextInput(SDL_TextInputEvent & textEvent, InputBox * activeInputBox) {
    if (activeInputBox && activeInputBox->isVisible && activeInputBox->isActive) {
        bool allowInput = true;
        if (!activeInputBox->acceptNonAscii && containsNonAscii(std::string(textEvent.text))) {
            allowInput = false;
        }
        if (!activeInputBox->compositionText.empty()) {
          allowInput = false;
        } 
        if (activeInputBox->numberic && containsNonNumber(std::string(textEvent.text))) {
            allowInput = false;
        }

        std::string LimintedText = textEvent.text;
        if (LimintedText.find('\'') != std::string::npos){
            allowInput = false;
        }
        if (allowInput) {
            //size_t currentChars = utf8_num_chars(activeInputBox->text);
            size_t currentChars = activeInputBox->text.length();
            size_t inputChars = utf8_num_chars(textEvent.text);

            if (activeInputBox->maxTextLength == 0 || (currentChars + inputChars <= activeInputBox->maxTextLength)) {
                activeInputBox->text.insert(utf8_char_to_byte_index(activeInputBox->text, activeInputBox->cursorIndex), textEvent.text);
                activeInputBox->cursorIndex += inputChars;
                activeInputBox->compositionText.clear();
                activeInputBox->compositionCursor = 0;
                activeInputBox->compositionSelectionLength = 0;
                //SDL_StartTextInput();
                updateIMERect(activeInputBox);
            }
        }
    }
}

void handleTextEditing(SDL_TextEditingEvent & editEvent, InputBox * activeInputBox) {
    if (activeInputBox && activeInputBox->isVisible && activeInputBox->isActive) {
        if (activeInputBox->acceptNonAscii) {
            activeInputBox->compositionText = editEvent.text;
            activeInputBox->compositionCursor = editEvent.start;
            activeInputBox->compositionSelectionLength = editEvent.length;
            //SDL_StartTextInput();
            updateIMERect(activeInputBox);
        }
    }
}

void handleMouseWheel(SDL_MouseWheelEvent *e)
{
    if (e->y != 0) {
        ChatMgr.HandleMouseWheel(e->y);
    }
}

bool SDL_input(void)
{
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0)
    {
        // ★★★ [插入點 2] 讓 ImGui 優先處理輸入 ★★★
        // ProcessEvent 會回傳 true 代表 ImGui 想要攔截這個輸入 (例如滑鼠正停在 UI 上)
        if (CimGui::Instance().ProcessEvent(&e)) {
            // 如果 ImGui 處理了這個事件 (例如你在拖動視窗，或在輸入文字)
            // 我們就直接 continue，跳過下面所有的遊戲邏輯
            // 這樣就不會發生「點了 GM 面板按鈕，結果角色跑去移動」的慘劇
            continue;
        }

        // 1. 先處理 Android/iOS 特有的系統消息 (加入這行)
        // ★★★ 唯一的改動：呼叫 Manager ★★★
        //MobileInputManager::Instance().HandleMobileEvents(&e);

        switch (e.type)
        {
        case SDL_QUIT:
            return false;

        case MY_CUSTOM_EVENT_CODE:
            g_SDLInfo.updateTimer = true;
            break;

        case SDL_MOUSEBUTTONDOWN:
        {
            int mouseX = e.button.x;
            int mouseY = e.button.y;

            // 1. 優先檢查是否點了線路按鈕
            if (g_RouteSelector.HandleInput(mouseX, mouseY)) {
                // 如果點到了，播放音效並跳出，不處理後面的邏輯
                // g_audioManager.PlaySound(BUTTON_SOUND);
                break;
            }

            if (isKeyPressed(SDL_SCANCODE_LCTRL) || CimGui::Instance().MobileCtrlFlag) {
                bool br = handleChatConetxtMenuClick(mouseX, mouseY);
                br = handleCharacterContxtMenuClick(mouseX, mouseY);
                if(!br) break;
            }
            handleMouseInputBoxActivation(mouseX, mouseY);
            MouseProcess(&e); // 假設這是一個處理通用滑鼠事件的函式
            break;
        }
        case SDL_MOUSEMOTION:
        { 
            int mouseX = e.button.x;
            int mouseY = e.button.y;
            // 1. 檢查滑鼠是否在選單上
            ChatMgr.CheckContextMenuMouseOver(mouseX, mouseY);
            CharContxtMenu.CheckContextMenuMouseOver(mouseX, mouseY);
            MouseProcess(&e); // 處理滑鼠移動和按鈕抬起
        } break;
        case SDL_MOUSEBUTTONUP:
            MouseProcess(&e); // 處理滑鼠移動和按鈕抬起
            break;
        case SDL_KEYDOWN:
            handleKeyboardInput(e.key, activeInputBox);
            break;

        case SDL_KEYUP:
            // 只在按鍵抬起時重置 keysJustPressed 狀態，避免重複。
            // 整個陣列會在函式開始時被 `std::fill` 清除。
            // 如果需要 "Just Released" 狀態，考慮另一組 bool 陣列。
            // keysJustPressed[e.key.keysym.scancode] = false; // 這行其實可以省略，因為下一幀會重置
            break;
        case SDL_TEXTINPUT:
            handleTextInput(e.text, activeInputBox);
            break;

        case SDL_TEXTEDITING:
            handleTextEditing(e.edit, activeInputBox);
            break;
        case SDL_MOUSEWHEEL:
            handleMouseWheel(&e.wheel);
            break;
                // -------------------------------------------------------
                // 修改 WINDOWEVENT (整合手機的邏輯)
                // -------------------------------------------------------
            case SDL_WINDOWEVENT:
            {
                switch (e.window.event)
                {
                    case SDL_WINDOWEVENT_RESIZED:
                    case SDL_WINDOWEVENT_SIZE_CHANGED: // 手機旋轉時通常觸發這個
                    {
                        //int newW = e.window.data1;

                        //int newH = e.window.data2;
                        // 更新你的渲染器 Viewport 或 UI 佈局
                    }   break;

                        // PC 版的最小化 (手機通常走 SDL_APP_... 事件，但保留也無妨)
                    case SDL_WINDOWEVENT_MINIMIZED:
                        if (!isWindowMinimized) {
                            g_audioManager.SetMasterVolume(0);
                            isWindowMinimized = true;
                        }
                        break;
                    case SDL_WINDOWEVENT_RESTORED:
                        if (isWindowMinimized) {
                            g_audioManager.SetMasterVolume(-1);
                            isWindowMinimized = false;
                        }
                        break;
                }
            } break; // End WINDOWEVENT
        default:
            // 處理其他未列出的事件類型，或留空
            break;
        }
    }
    keyboardState = SDL_GetKeyboardState(NULL);
    return true;
}

// 這個函式負責把字串裡的爛字換成空白
const char* SanitizeString(const char* input, size_t len) {
    // 1. 靜態緩衝區 (只在遇到壞字時才用)
    static std::string result;

    // 2. [極速通道] 先快速掃描一遍，看有沒有壞字
    // 這一步非常快，因為只是讀取記憶體，完全不寫入，也不配置
    const char* it = input;
    const char* end = input + len;
       bool dirty = false;

    while (it < end) {
        try {
            // utf8::next 會移動 it 指標
            utf8::next(it, end);
        } catch (...) {
            dirty = true;
            break; // 抓到了！有壞字，跳出
        }
    }

    // ★★★ 關鍵優化 ★★★
    // 如果掃描完發現是乾淨的，直接回傳原始指標！
    // 耗時：接近 0.00ms (沒有 malloc，沒有 memcpy)
    if (!dirty) {
        return input;
    }

    // ------------------------------------------------
    // 3. [慢速通道] 只有真的髒了，才啟用 static buffer
    // ------------------------------------------------
    if (result.capacity() < 1024) {
        result.reserve(1024);
    }
    result.clear();

    // 重置 iterator 再跑一次，這次要修復
    it = input;

    while (it < end) {
        auto start = it;
        try {
            utf8::next(it, end);
            // 合法：拷貝進 buffer
            result.append(start, it);
        }
        catch (...) {
            // 非法：補空白，跳過 1 byte
            result.push_back(' ');
            it = start + 1;
        }
    }

    return result.c_str();
}

// 假設這是你在外部定義的常數
const int DEFAULT_CHAR_WIDTH_ESTIMATE = 15; // 當字還沒生成時，用來佔位的估計寬度(像素)

// ---------------------------------------------------------
// 輔助函式：判斷是否為 Emoji 範圍 (強制切換字型用)
// ---------------------------------------------------------
static bool IsLikelyEmoji(Uint32 ch) {
    // 簡單判斷：大部分 Emoji 都在 0x1F000 以上
    // 雜項符號 (☀, ☁, ☂) 在 0x2600 - 0x27BF
    return (ch >= 0x1F000) || (ch >= 0x2600 && ch <= 0x27BF);
}

void RenderTextWithFallback(int x, int y, const char* utf8_text,
                            SDL_Color color, TTF_Font* baseFont,
                            TTF_Font* fallbackFont) {

    //auto t_start = std::chrono::high_resolution_clock::now();
    //double blit_time_ms = 0.0;

    if (!utf8_text || *utf8_text == '\0' || !baseFont) return;

    const int MAX_TEXT_LENGTH = 260;
    size_t len = strnlen(utf8_text, MAX_TEXT_LENGTH);
    if (len == 0) return;

    // 1. 字串清洗 (使用無配置版本)
    const char* clean_text_str = SanitizeString(utf8_text, len);

    // 重新計算長度 (因為 Sanitize 可能改變了內容)
    size_t clean_len = strlen(clean_text_str);
    const int PADDING = 6;

    // 2. ASCII 預判 (快速通道)
    bool isPureASCII = true;
    const char* p = clean_text_str;
    while (*p) {
        if (*p & 0x80) { isPureASCII = false; break; }
        p++;
    }

    if (isPureASCII || !fallbackFont || fallbackFont == baseFont) {
        // 直接傳指標和長度
        SDL_Surface* surface = getOrCreateTextSurface(clean_text_str, clean_len,color, baseFont);

        if (surface) {
            SDL_Rect dstRect = AlignText(HangulAlign, x, y - PADDING, surface->w, surface->h);

            //auto t_blit_start = std::chrono::high_resolution_clock::now();
            SDL_BlitSurface(surface, NULL, g_SDLInfo.cpuRenderSurface, &dstRect);
            //blit_time_ms += std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - t_blit_start).count();
        }
        goto REPORT_PERFORMANCE;
    }

    // =========================================================
    // 通道 C: 複雜通道
    // =========================================================
    {
        const char* safe_it = clean_text_str;
        const char* safe_end = safe_it + clean_len;
        const char* start_of_segment = safe_it;
        int current_x = x;

        // 初始化第一個字的字型
        const char* temp_it = safe_it;
        Uint32 first_codepoint = 0;
        try { first_codepoint = utf8::next(temp_it, safe_end); } catch(...) {}

        bool firstIsEmoji = IsLikelyEmoji(first_codepoint);
        TTF_Font* current_font = (firstIsEmoji) ? fallbackFont :
                                 (TTF_GlyphIsProvided32_Safe(baseFont, first_codepoint) ? baseFont : fallbackFont);

        while (safe_it < safe_end) {
            const char* segment_start_it = safe_it;
            Uint32 codepoint = 0;
            try { codepoint = utf8::next(safe_it, safe_end); } catch (...) { safe_it++; continue; }
            if (codepoint == 0) break;

            TTF_Font* required_font = baseFont;
            if (IsLikelyEmoji(codepoint)) required_font = fallbackFont;
            else if (!TTF_GlyphIsProvided32_Safe(baseFont, codepoint)) required_font = fallbackFont;

            // 字型切換點
            if (required_font != current_font) {
                if (segment_start_it > start_of_segment) {
                    // ★★★ 優化：直接傳指標和長度，不建立 std::string ★★★
                    size_t seg_len = segment_start_it - start_of_segment;
                    SDL_Color actualColor = (current_font == fallbackFont) ? SDL_Color{255, 255, 255, 255} : color;

                    SDL_Surface* surface = getOrCreateTextSurface(start_of_segment,seg_len, actualColor, current_font);

                    int xpadding = (current_font == fallbackFont) ? 2 : 0;

                    if (surface) {
                        // 如果是 Emoji，這裡 surface->w 應該要大於 0
                        // 如果 surface != nullptr 但看不見，通常是 BlendMode 問題
                        int fallbackPADDING = (current_font == fallbackFont) ? PADDING - 1 : 0;
                        SDL_Rect dstRect = AlignText(HangulAlign, current_x + xpadding, y - PADDING + fallbackPADDING, surface->w, surface->h);

                        //auto t_blit_start = std::chrono::high_resolution_clock::now();
                        SDL_BlitSurface(surface, NULL, g_SDLInfo.cpuRenderSurface, &dstRect);
                        //blit_time_ms += std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - t_blit_start).count();

                        current_x += surface->w + xpadding;
                    } else {
                        // 如果是 Emoji 且 surface 為 NULL，代表字型載入失敗或大小不對
                        // 這裡可以考慮畫一個問號或空格
                        int estimated_w = 14 * (seg_len / 3); // 粗略估算
                        current_x += estimated_w;
                    }
                }
                start_of_segment = segment_start_it;
                current_font = required_font;
            }
        }

        // 處理最後一段
        if (start_of_segment < safe_end) {
            size_t seg_len = safe_end - start_of_segment;
            SDL_Color actualColor = (current_font == fallbackFont) ? SDL_Color{255, 255, 255, 255} : color;

            SDL_Surface* surface = getOrCreateTextSurface(start_of_segment,seg_len, actualColor, current_font);

            int XPADDING = (current_font == fallbackFont) ? 2 : 0;
            if (surface) {
                int fallbackPADDING = (current_font == fallbackFont) ? PADDING - 2 : 0;
                SDL_Rect dstRect = AlignText(HangulAlign, current_x + XPADDING, y - PADDING + fallbackPADDING, surface->w, surface->h);

                //auto t_blit_start = std::chrono::high_resolution_clock::now();
                SDL_BlitSurface(surface, NULL, g_SDLInfo.cpuRenderSurface, &dstRect);
                //blit_time_ms += std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - t_blit_start).count();
            }
        }
    }

    REPORT_PERFORMANCE:
    /*auto t_end = std::chrono::high_resolution_clock::now();
    double total_ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();
    if (total_ms > 2.0) {
        double blit_p = (total_ms > 0) ? (blit_time_ms / total_ms) * 100.0 : 0.0;
        SDL_Log("[LAG ANALYSIS] Text: %s | Total: %.2f ms | Blit: %.2f ms (%.1f%%)", utf8_text, total_ms, blit_time_ms, blit_p);
    }*/
}

//下面是 Texture 渲染時才會用到 cpu 渲染不會用到
void RenderTextureText(int x, int y,int& texW, SDL_Texture* lineText, SDL_Renderer* render) {
    // 1. 查詢紋理的實際寬高
    int texH = 0;
    SDL_QueryTexture(lineText, NULL, NULL, &texW, &texH);
    y -= 6;
    // 2. 建立螢幕目標區域 (Destination Rect)
    /*SDL_Rect destRect = {
        x,      // 螢幕 X 座標
        y,      // 螢幕 Y 座標
        texW,   // 目標寬度 (與紋理寬度一致，避免拉伸)
        texH    // 目標高度 (與紋理高度一致，避免拉伸)
    };*/
    SDL_Rect destRect = AlignText(HangulAlign, x, y , texW, texH);

    // 3. 執行渲染
    // Source Rect 傳入 NULL，表示繪製整個紋理
    SDL_RenderCopy(render, lineText, NULL, &destRect);
}

void RenderTextWithTextureFallback(int x, int y, const char* utf8_text,
    SDL_Color color, TTF_Font* baseFont,
    TTF_Font* fallbackFont) {

    // 檢查無效輸入
    if (!utf8_text || *utf8_text == '\0' || !baseFont)
    {
        return;
    }

    const int MAX_TEXT_LENGTH = 260;
    size_t len = strnlen(utf8_text, MAX_TEXT_LENGTH);
    if (len == 0) {
        return;
    }

    // --- [步驟 1: 預先清洗整個字串] ---
    // 建立一個乾淨的 std::string 副本
    const char* clean_text_str = SanitizeString(utf8_text, len);

    // 將所有指標和長度參考到這個乾淨的字串
    const char* safe_it = clean_text_str;
    const char* safe_end = safe_it + strlen(clean_text_str);
    const char* start_of_segment = safe_it;

    // --- [步驟 2: 無 Fallback 字體模式] ---
    const int PADDING = 6;
    if (!fallbackFont) {
        // 使用 clean_text_str (c_str()) 代替原始的 utf8_text
        SDL_Texture* lineText = getOrCreateTexture(x, y, clean_text_str, color, baseFont);

        if (lineText) {
            int texW = 0;
            // 這裡假設 RenderTextureText 會使用 getOrCreateTexture 內計算的寬度或直接使用 texture 的寬度
            RenderTextureText(x, y, texW, lineText, g_SDLInfo.renderer);
        }
        return;
    }

    // --- [步驟 3: 初始化指標和字體] ---
    int current_x = x;

    // 初始化當前字體 (使用 safe_it)
    Uint32 first_codepoint = 0;
    const char* temp_it = start_of_segment;

    // 因為已經清洗過，這裡不再需要 try-catch
    if (temp_it < safe_end) {
        first_codepoint = utf8::next(temp_it, safe_end);
    }

    TTF_Font* current_font = TTF_GlyphIsProvided(baseFont, first_codepoint) ? baseFont : fallbackFont;

    // --- [步驟 4: 主迴圈 (乾淨版)] ---
    while (safe_it < safe_end) {
        const char* segment_start_it = safe_it;

        // 這裡不再需要 try-catch！
        Uint32 codepoint = utf8::next(safe_it, safe_end);

        if (codepoint == 0) break;

        // 判斷所需字體
        TTF_Font* required_font = TTF_GlyphIsProvided(baseFont, codepoint) ? baseFont : fallbackFont;

        // 如果所需字體與當前字體不同，渲染之前的文字段
        if (required_font != current_font) {
            if (segment_start_it > start_of_segment) {
                // 從乾淨的緩衝區切出片段
                std::string segment_str(start_of_segment, segment_start_it - start_of_segment);

                // 【注意】這裡不再需要 SanitizeString(segment_str) 了
                SDL_Texture* textTexture = getOrCreateTexture(x, y, segment_str, color, current_font);

                if (textTexture) {
                    int texW = 0;
                    RenderTextureText(x, y, texW, textTexture, g_SDLInfo.renderer);
                    current_x += texW;
                }
            }
            // 更新當前字體和新的文字段起點
            start_of_segment = segment_start_it;
            current_font = required_font;
        }
    }

    // --- [步驟 5: 渲染最後一段文字] ---
    if (start_of_segment < safe_end) {
        std::string segment_str(start_of_segment, safe_end - start_of_segment);

        // 【注意】這裡不再需要 SanitizeString(segment_str) 了
        SDL_Texture* textTexture = getOrCreateTexture(x, y, segment_str, color, current_font);

        if (textTexture) {
            int texW = 0;
            RenderTextureText(x, y, texW, textTexture, g_SDLInfo.renderer);
        }
    }
}