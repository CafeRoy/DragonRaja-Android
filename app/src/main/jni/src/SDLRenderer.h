#pragma once

// --- SDL 版本的 DirectDrawInfo 替換 ---
#include "StdAfx.h"
#include <SDL.h> // 包含 SDL 核心庫
#include <SDL_ttf.h>
#include <SDL_syswm.h>
#include <iostream>
#include <string>
#include <map>
#include <memory> // For std::unique_ptr
#include <chrono> // For std::chrono::steady_clock
#include <unordered_map>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <unordered_set>
#include <functional>
#include "utf8_helper.h"
#include "AudioManager.h"
#include "mytypes.h"
#include "ImageTexture.h"

extern const char* font_path_cht;
extern const char* font_path_chs;
extern char font_path[260];
enum LANG_ {
    CHT_,
    CHS_
};
// 用於儲存變之後的 Sprite 
struct RecoloredSurface {
    SDL_Surface* originSurface;
    SDL_Surface* newSurface;   
};

struct LineDrawInfo {
    RECT rect; // 要繪製的方框
    bool is_screen_coords; // 是否使用螢幕座標 (保留您 Line 函式的原始功能)
};
// 使用 std::vector 作為精靈的容器
using SPRITE = std::vector<Spr>;

struct SPRITE_DATA {
    std::vector<char> bufptr;
    SPRITE    SpriteArray;
};
enum RENDER_TYPE
{
    PUT_NORMAL,
    PUT_APART,
    PUT_BLENDRGB,
    PUT_CHARRGB,
    PUT_FX,
    PUT_ONECOLOR,
    PUT_SHADOW,
    PUT_VOID,
    PUT_WAVTILE,
    PUT_TILE,
    PUT_TILE_ALPHA,
    PUT_MAX,
};
class SDL_DirectDrawInfo {
public:
    SDL_DirectDrawInfo();
    ~SDL_DirectDrawInfo();

public:
    int Language = CHT_;
    // ---------------------------------------------------------
    // 全域變數：用來「永久」保存字型資料的記憶體
    // 必須是全域的，因為 TTF_Font 活著的時候，這塊記憶體不能被釋放！
    // ---------------------------------------------------------
    void* g_MainFontBuffer = nullptr;   // 主字型 (SourceHanSans)
    void* g_EmojiFontBuffer = nullptr;  // 表情字型 (NotoColorEmoji)

    TTF_Font* gFont;
    TTF_Font* g_BoldFont;
    TTF_Font* gEmojiFont;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_KeyboardEvent keyboardEventThisframe;
    SDL_DisplayMode originalDesktopMode;
	std::vector<LineDrawInfo> g_linesToDrawThisFrame; // 用於儲存文字渲染的矩形區域
    // --- 新增：用於 CPU 像素操作的後台緩衝區 ---
    SDL_Surface* cpuRenderSurface;
    // 新增的成員變數，用於儲存可串流紋理
    SDL_Texture* streamingTexture;
    SDL_PixelFormat* texture_format;
    Uint32   limit_time;
	bool   bRenderPc;

    std::string serverTime;
    int day_of_week;
    std::string SaveScreen;
    bool bOutputSmallMap;
    bool updateTimer;
    bool bFullscreen;

    int param[7];

    std::map<const Spr*, std::vector<SDL_Point>> g_outlineCache;
    std::map<const Spr*, RecoloredSurface> g_SurfaceCache;
    std::unordered_map<std::string, std::unique_ptr<ImageTexture>> g_PngCache;
	std::map<std::string, SPRITE_DATA> g_SpriteCache;
	std::map<int, std::string> g_ImageFileMap;
   public:
    void CleanupSDLGraphics();
    char* setBackbuffersurface(char*& pixels, DWORD& dxSize, WORD& wDxsize);
    HWND SDL_GetWindowHandle();

    void SDL_EraseScreen(); // 清空 cpuRenderSurface

    void SDL_drawText(int x, int y, std::string str);

    void InitAudioManager();

    void SDL_InitInputBox();

    void SDL_DrawInputbox();
    
    void FireFlySystemRender();

    void aura_effect_Emit(LPCHARACTER ch);

    void particle_effect_Render(LPCHARACTER ch);
    
    void particle_effect_Emit(LPCHARACTER ch);

    void AllParticelSystemUpdate(float deltaTime);

    void RenderAllDebugShapes();

    void LoadPNGImage(std::string filename);

    // 打印PNG快取中所有的圖片索引路徑
    void printCachedImages();

    void OnWindowResized(int new_width, int new_height);

    int SaveRendererToPNG(const char* filename);

    void SaveScreenShot(std::string filename);

    void SDL_FlipScreen(); // 將 cpuRenderSurface 顯示到螢幕
    void cleanupTextCache(std::chrono::milliseconds unused_threshold);
    void setKeyboardState();
    int CalculateActualWidth(const Spr* spr);
    SDL_Surface* CreateSurfaceFromRawTile(const Spr* spr);
    bool LoadTempSprite_Cached_CPP(int id, Spr** out_spr);
    void CreateOutlinePointsHorseRider(Spr* horse);
    void CreateOutlinePoints(CharacterSprite* charspr);
    SDL_Surface* CreateSurfaceFromSpr(const Spr* spr, uint16_t transparentColor, bool tile = false);
    bool RenderMethod(int x, int y, Spr* s, RENDER_TYPE type);
    void UnloadSprAtlas(const char* filename);
    void CleanupAllAtlases();
    SDL_Texture* GetOrCreateAtlas(const char* filename);
    SDL_Texture* CreateTextureFromSingleSpr_CPP(const char* filename, const Spr* s, uint16_t transparentColor = 0);
    SDL_Texture* CreateAtlasForMap_CPP(const char* mapname, Spr* loaded_sprites, int sprite_count, bool tile=false);
    SDL_Texture* CreateAtlasFromSprArray2D_CPP(const char* filename, const Spr* spr_array_2d, int num_rows, int num_cols);
    SDL_Texture* CreateAtlasForCharacter(const char* filename, const CharacterSprite* char_sprite, int frames_per_direction);
    SDL_Texture* CreateAtlasForHorseRider(const char* filename,const Spr* horse_rider);
    SDL_Texture* CreateAtlasForChImageNumber_CPP(const char* filename, const Spr* ani_array, const int* max_ani, int num_rows, int max_cols);
    SDL_Texture* CreateAtlasFromLoadedData(const char* filename, const Spr* loaded_spr_array, int sprite_count);
    void LoadMenuIndex(int IndexType=0, std::string path="");
    Spr* GetSpriteFromCache(const char* filename, int nImageNumber);
    Spr* LoadSpriteFromFile(const char* filename, int nImageNumber);
    void launchfirework(int shot);
};

struct InputBox {
    std::string text; std::string passwordDisplay;
    int x, y, w, h; // 位置和大小
    SDL_Color textColor = { 255, 255, 255, 255 }; // 白色文字
    SDL_Color backColor = {0, 0, 0, 200};         // 半透明黑色背景
    SDL_Color boxColor = { 50, 50, 50, 255 };    // 深灰色背景
    SDL_Color borderColor = { 100, 100, 100, 255 }; // 邊框顏色
    SDL_Color activeBorderColor = { 190, 190, 0, 255 }; // 活躍時的邊框顏色
    int cursorIndex = 0; // 游標在文字中的位置
    bool isActive = false; // 輸入框是否處於活躍狀態 (可輸入)
    bool isVisible = false;
    bool isPassword = false;
    bool acceptNonAscii = true;
    bool numberic = false;
    int myID;

    // --- 新增用於輸入法合成文字的變數 ---
    std::string compositionText; // 正在編輯的合成文字
    int compositionCursor = 0;   // 合成文字內的游標位置
    int compositionSelectionLength = 0; // 合成文字內選中的長度
    bool isMultiLine; // <-- 新增：標示此輸入框是否支援多行

    // 多行模式下才需要維護的狀態變數
    int scrollOffsetLines; // 垂直滾動的行數偏移 (用於渲染可見部分)
    int currentRenderLine; // 游標目前所在的渲染行索引 (從 0 開始，考慮滾動)
    int cursorXInRenderLine; // 游標在當前渲染行內的字元索引 (從 0 開始)

    // --- 新增游標閃爍相關變數 ---
    Uint32 lastCursorToggleTime = 0; // 上次游標狀態切換的時間
    bool cursorVisible = true;       // 游標當前是否可見
    const Uint32 CURSOR_BLINK_RATE = 700; // 游標閃爍頻率（毫秒），例如每 500ms 切換一次

    /*// --- 新增用於文本快取的成員 ---
    SDL_Texture* textTexture;   // 渲染後的文本紋理
    std::string cachedText;     // 該紋理對應的文本內容 (用於判斷是否需要重新渲染)
    bool textNeedsUpdate;       // 標誌：文本內容是否已改變，需要重新渲染紋理*/

    size_t maxTextLength;

    void setMultiline(bool yes_no) { isMultiLine = yes_no; }
    void setVisable(bool yes_no) { isVisible = yes_no; }
    void setPassword() { isPassword = !isPassword; }
    void setNumberic() { numberic = !numberic; }

    InputBox(int ix, int iy, int iw, int ih, int imyID, bool multiLine = false) :
        x(ix), y(iy), w(iw), h(ih),
        text(""), compositionText(""), cursorIndex(0),
        compositionCursor(0), compositionSelectionLength(0), isActive(false), isVisible(false),
        isMultiLine(multiLine), scrollOffsetLines(0), currentRenderLine(0), cursorXInRenderLine(0),
        maxTextLength(0),isPassword(false), acceptNonAscii(true), myID(imyID), passwordDisplay("") {
    }

    void setInfo(int ix, int iy, int iw, int ih, int maxLength, bool isVis, bool isAct, bool mulitLine)
    {
        x = ix;
        y = iy;
        h = ih;
        w = iw;
        isActive = isAct;
        isVisible = isVis;
        isMultiLine = mulitLine;
        setMaxTextLength(maxLength);
    }

    bool operator==(const int ID) const {
        return (myID == ID);
    }

    // 可以添加 setter 方法來動態修改
    void setMaxTextLength(size_t length) {
        maxTextLength = length;
        // 如果當前文本長度超過了新的最大限制，你可能需要截斷文本
        if (utf8_num_chars(text) > maxTextLength) {
            text = utf8_substr(text, 0, maxTextLength);
            // 同時更新游標位置，確保它不超出新限制
            if (cursorIndex > maxTextLength) {
                cursorIndex = maxTextLength;
            }
        }
    }

    void setActive(bool active) {
        isActive = active;
        // 清除舊的合成文字狀態
        compositionText.clear();
        compositionCursor = 0;
        compositionSelectionLength = 0;
    }
};

// 用於儲存 Sprite 在紋理圖集中的資訊
struct TextureInfo {
    SDL_Texture* atlasTexture; // 指向它所屬的那個巨大的紋理圖集
    SDL_Rect     sourceRect;   // 它在圖集中的具體位置和大小 (x, y, w, h)

};

// 用於排序和打包的輔助結構
struct SpriteInfo {
    const Spr* sprite_ptr;
    int width;
    int height;
};

// 必須放在 header 檔中
struct TextRenderKey {
    std::string text;
    SDL_Color color;
    TTF_Font* font;
    mutable std::chrono::steady_clock::time_point lastUsed; // 這可以用來做 LRU 清理，但不參與 Hash

    // 1. 必須實作相等運算符 (這是為了 Map 判斷 Key 是否相同)
    bool operator==(const TextRenderKey& other) const {
        return text == other.text &&
               color.r == other.color.r &&
               color.g == other.color.g &&
               color.b == other.color.b &&
               color.a == other.color.a &&
               font == other.font;
    }

    // 2. 為 TextRenderKey 重載 operator<
    //    std::map 需要 Key 類型是可排序的
    bool operator<(const TextRenderKey& other) const {
        // 按照一定的順序比較所有成員，確保唯一性
        //if (font != other.font) return font < other.font; // 優先比較字體指針
        if (text != other.text) return text < other.text;

        // 比較 SDL_Color，需要逐個成員比較
        if (color.r != other.color.r) return color.r < other.color.r;
        if (color.g != other.color.g) return color.g < other.color.g;
        if (color.b != other.color.b) return color.b < other.color.b;
        return color.a < other.color.a; // 包含 alpha 值
    }
};

// 2. 必須注入 Hash 函式到 std 命名空間 (這是為了 Map 能計算 Hash)
namespace std {
    template <>
    struct hash<TextRenderKey> {
        size_t operator()(const TextRenderKey& k) const {
            // 使用簡單的位元運算混合 Hash
            size_t h1 = std::hash<std::string>{}(k.text);
            size_t h2 = std::hash<void*>{}(k.font); // Font 指針也是特徵
            size_t h3 = (k.color.r << 24) | (k.color.g << 16) | (k.color.b << 8) | k.color.a;

            return h1 ^ (h2 << 1) ^ (h3 << 1); // 簡單混合
        }
    };
}

// 定義任務結構
struct TextTask {
    TextRenderKey key; // 你的 Key，包含文字、顏色、字體
    // 為了線程安全，我們需要確保 key 裡的 string 是深拷貝 (std::string 本身就是)
};

// 定義結果結構
struct TextResult {
    TextRenderKey key;
    SDL_Surface* surface;
};


extern SDL_DirectDrawInfo g_SDLInfo;
class AsyncTextManager {
public:
    // 1. ★★★ 將鎖公開，或者提供 Get 方法 ★★★
    // 這是為了讓主線程在呼叫 TTF_SizeUTF8 時也能鎖住它
    std::mutex fontMutex;
    TTF_Font* ttfThreadEmoji = nullptr;
    TTF_Font* ttfThread = nullptr;
    static AsyncTextManager& Instance() {
        static AsyncTextManager instance;
        return instance;
    }

    void Init(TTF_Font* ThreadFont, TTF_Font* ThreadEmoji) {
        if (running) return;
        running = true;
        ttfThread = ThreadFont;
        ttfThreadEmoji = ThreadEmoji;
        workerThread = std::thread(&AsyncTextManager::ThreadLoop, this);
    }

    void Quit() {
        running = false;
        cv.notify_all();
        if (ttfThread) {
            TTF_CloseFont(ttfThread);
            ttfThread = nullptr;
        }

        if (ttfThreadEmoji) {
            TTF_CloseFont(ttfThreadEmoji);
            ttfThreadEmoji = nullptr;
        }

        if (workerThread.joinable()) workerThread.join();
    }

    void Request(const TextRenderKey& key) {
        std::lock_guard<std::mutex> lock(queueMutex);

        // 如果已經在佇列中，就不重複請求
        if (pendingKeys.find(key) != pendingKeys.end()) {
            return;
        }

        taskQueue.push({ key });
        pendingKeys.insert(key);
        cv.notify_one();
    }

    int FetchResults(std::vector<TextResult>& outResults) {
        std::lock_guard<std::mutex> lock(resultMutex);
        if (resultQueue.empty()) return 0;

        int count = 0;
        while (!resultQueue.empty()) {
            outResults.push_back(resultQueue.front());
            resultQueue.pop();
            count++;
        }
        return count;
    }

    bool IsPending(const TextRenderKey& key) {
        std::lock_guard<std::mutex> lock(queueMutex);
        return pendingKeys.find(key) != pendingKeys.end();
    }

private:
    bool running = false;
    std::thread workerThread;

    std::queue<TextTask> taskQueue;
    std::unordered_set<TextRenderKey> pendingKeys;
    std::mutex queueMutex;
    std::condition_variable cv;

    std::queue<TextResult> resultQueue;
    std::mutex resultMutex;

    void ThreadLoop() {

        while (running) {
            TextTask task;
            {
                std::unique_lock<std::mutex> lock(queueMutex);
                cv.wait(lock, [this] { return !taskQueue.empty() || !running; });

                if (!running) break;

                task = taskQueue.front();
                taskQueue.pop();
            }

            // =========================================================
            // 1. 判斷這是不是 Emoji 任務
            // =========================================================
            // 我們比較「請求的字型」是不是「全域 Emoji 字型」
            bool isEmoji = (task.key.font == g_SDLInfo.gEmojiFont);

            SDL_Surface* surf = nullptr;

            // ★★★ 關鍵修改：選擇後台專用的字型 ★★★
            TTF_Font* renderFont = nullptr;

            if (isEmoji) {
                // 如果你還沒在 AsyncManager 裡初始化 Emoji 字型，請看下面的補充步驟
                renderFont = ttfThreadEmoji;
            } else {
                renderFont = ttfThread; // 這是我們之前初始化的主字型
            }

            // ★★★ 這裡不需要鎖了 (fontMutex)！ ★★★
            // 因為 renderFont 只有這條線程會碰到，絕對安全
            if (renderFont) {
                surf = TTF_RenderUTF8_Blended(renderFont, task.key.text.c_str(), task.key.color);
            }

            {
                std::lock_guard<std::mutex> lock(resultMutex);
                resultQueue.push({ task.key, surf });
            }

            // 3. 任務完成後，才從 pending 移除
            // 建議放在最後，確保在生成期間，同樣的請求不會被重複加入
            {
                std::lock_guard<std::mutex> qLock(queueMutex);
                pendingKeys.erase(task.key);
            }
        }
    }
};

// 用於識別自定義事件的類型
#define MY_CUSTOM_EVENT_CODE SDL_USEREVENT + 1
// 定義一個全域的快取 (字碼 -> 寬度)
// 使用 unordered_map 速度比 map 快 O(1)
static std::unordered_map<Uint32, int> g_GlyphWidthCache;
static std::unordered_map<Uint32, int> g_GlyphProvidedCache;
//static std::unordered_map<Uint32, int> g_SizeUTF8Cache;

extern char* g_DestBackBuf;
extern unsigned char BrightBuf[(960) * (540)];
extern int HangulAlign;
extern AudioManager g_audioManager;

extern std::vector<InputBox> inputBoxes;
extern InputBox* activeInputBox;
extern std::map<TextRenderKey, std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)>> g_textSurfaceCache;
extern std::unordered_map<const Spr*, TextureInfo> g_TextureCache;
extern SafeValue<int> Mapx;
extern SafeValue<int> Mapy;
extern int DayLightControl;

extern int TTF_GlyphMetrics32_Safe(TTF_Font *font, Uint32 ch, int *minx, int *maxx, int *miny, int *maxy, int *advance);
extern int TTF_GlyphIsProvided32_Safe(TTF_Font *font, Uint32 ch);
extern int TTF_SizeUTF8_Safe(TTF_Font *font, const char *text, int *w, int *h);
extern bool containsNonAscii(const std::string& str);
extern bool is_valid_utf8_string(const std::string& str);
extern void updateIMERect(InputBox* inputBox);
extern InputBox* FindInputBox(int BoxID);
extern void renderInputBox(InputBox& box, TTF_Font* font);
extern void MouseProcess(SDL_Event* e);
extern bool isKeyPressed(SDL_Scancode scancode);
extern bool isKeyDown(SDL_Scancode scancode);
extern bool InitSDLGraphics(HWND &hWnd);
extern bool SDL_input(void);
extern void SDL_HprintOutLine(const int x, const int y, const char* szStr, SDL_Color textColor, int HangulAlign);
extern void SDL_Hprint(const int x, const int y, const char* szStr, SDL_Color textColor, int HangulAlign);
extern void SDL_DrawRect(const int x, const int y, const int w, const int h, SDL_Color color);
extern void SDL_HprintBold(const int x, const int y, const char* szStr, SDL_Color textColor, int HangulAlign);
extern void updateGameLogicKeyPressed(InputBox* active, SDL_KeyboardEvent* e);
// 假设 Line 函数已修改为绘制到 g_SDLInfo.cpuRenderSurface (并且其 color 参数是 Uint32 像素值)
extern void Line(int sx, int sy, int ex, int ey, int color, bool is_screen_coords = false);
extern SDL_Surface* CreateRecoloredSurface(SDL_Surface* original_surface, const SDL_Color* gradient);
extern void InitGradients();
extern void RenderTextWithFallback(int x, int y, const char* utf8_text, SDL_Color color, TTF_Font* baseFont, TTF_Font* fallbackFont);
extern void RenderTextWithTextureFallback(int x, int y, const char* utf8_text, SDL_Color color, TTF_Font* baseFont, TTF_Font* fallbackFont);

extern void	HprintBold(const int x, const int y, const int fc, const int bc, char *s, ... );
extern void Hprint(const int x, const int y, char* destbuf, const char* szStr);
extern SDL_Color* getOrCreategGradient(DWORD color);
extern void SWD_LOGON_Clicked(const int nMenuIndex);
extern void PutCmprsImgNOTCliping_CPP(int x, int y, Spr* sp, void* dest, int destPitch);
extern const char* SanitizeString(const char* input, size_t len);