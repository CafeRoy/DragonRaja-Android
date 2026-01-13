#pragma once

#include <SDL.h>
// ===== 請在您的專案中加入這個類別 =====
#include <vector>
#include <list> // list 在頻繁增刪時效能優於 vector
#include "SDLRenderer.h"

// 螢火蟲粒子的結構體
struct FireflyParticle {
	// 【改造】這兩個變數現在代表粒子在遊戲世界中的「絕對座標」
	float world_x, world_y;
	float vx, vy; // 速度也是在世界空間中的

	float lifetime; // 剩餘生命週期
	// 【核心】控制閃爍的屬性
	float blink_speed;  // 每隻螢火蟲獨特的閃爍速度
	float blink_offset; // 隨機的相位偏移，確保它們不同步閃爍
};

class FireflyParticleSystem {
public:
	void Cleanup();
	void Update(float deltaTime, int camera_world_x, int camera_world_y);
	void Render(SDL_Surface* targetCanvas, int map_offset_x, int map_offset_y);
	bool LoadParticleTexture(const char* bmp_filename);
	// --- 【新增這座「橋樑」】 ---
	// 這個函式會返回一個指向粒子列表的常數引用，
	// 這意味著外部程式碼可以「讀取」但不能「修改」我們的粒子，非常安全。
	const std::list<FireflyParticle>& GetParticles() const {
		return m_particles;
	}
private:

	void Emit(int camera_world_x, int camera_world_y);
    std::list<FireflyParticle> m_particles;
    SDL_Surface* m_pParticleSurface = nullptr;
};

class CRenderMethod
{
public:
	CRenderMethod() { }
	~CRenderMethod();
private:
public:
	static void PutCompressImageApart_CPP(int x, int y, const Spr* spr, TextureInfo info, int clip_sx, int clip_sy, int clip_ex, int clip_ey, int alpha);
	//使用的是PutCmprsImgClipingBlend565
	static void PutCompressedImage(int x, int y, Spr* s,TextureInfo& info, SDL_Renderer* renderer);//普通貼圖
	static void PutCompressedImageBlendRGB(int x, int y, Spr* s, TextureInfo info, uint32_t RGB);
	static void PutCompressedImageCharRGB(int x, int y,Spr* s, TextureInfo info, uint32_t RGB, SDL_Renderer* renderer);
	static void PutCompressedImageFX(int x, int y, Spr* s, TextureInfo info, uint32_t alpha, uint32_t op, SDL_Renderer* renderer);//透明
	static void PutCompressedImageShadow(int x, int y,Spr* sp, TextureInfo info);//陰影
	static void PutCompressedImageVoid(int x, int y,  Spr* spr, TextureInfo info);//有用
	static void PutCompressedImageWaveTile(int x, int y, Spr* s, TextureInfo info);//有用
	static void PutTile(int x, int y, TextureInfo info);
	static void PutTileFX(int x, int y, TextureInfo info, int alpha);
	static void PutCmprsImgBlend565FX(SDL_Renderer* renderer, SDL_Rect rc, TextureInfo info, int draw_alpha);
	static void PutCmprsImgAdd565FX(SDL_Renderer* renderer, SDL_Rect rc, TextureInfo info, int draw_alpha);
	void PutCmprsImgAddDifer565_CPP(int x, int y, Spr* sp, TextureInfo info, int dDxSize, int alpha);
	void PutCmprsImgSub565_CPP(int x, int y, Spr* sp, TextureInfo info, int dDxSize, int alpha);
	void PutCmprsImgBlendDifer565_CPP(int x, int y, Spr* sp, TextureInfo info, int dDxSize, int alpha);
};

extern CRenderMethod g_RenderMethod;
extern FireflyParticleSystem g_fireflySystem;


extern void PutCmprsImgClipingBlend565_CPP(int x, int y, Spr* sp, void* dest, int destPitch, int lx, int rx, int ty, int by, int alpha);

// 2. 加色混合
extern void PutCmprsImgClipingAdd565_CPP(int x, int y, Spr* sp, void* dest, int destPitch, int lx, int rx, int ty, int by, int alpha);

// 3. 減色混合
extern void PutCmprsImgClipingSub565_CPP(int x, int y, Spr* sp, void* dest, int destPitch, int lx, int rx, int ty, int by, int alpha);

// 4. 半透明混合 (50% Alpha)
extern void PutCmprsImgClipingHBlend565_CPP(int x, int y, Spr* sp, void* dest, int destPitch, int lx, int rx, int ty, int by);

// 5. 差異加色混合
extern void PutCmprsImgClipingAddDifer565_CPP(int x, int y, Spr* sp, void* dest, int destPitch, int lx, int rx, int ty, int by, int alpha);

// 6. 差異 Alpha 混合
extern void PutCmprsImgClipingBlendDifer565_CPP(int x, int y, Spr* sp, void* dest, int destPitch, int lx, int rx, int ty, int by, int alpha);

// 1. 標準 Alpha 混合
extern void PutCmprsImgNOTClipingBlend565_CPP(int x, int y, Spr* sp, void* dest, int destPitch, int alpha);

// 2. 加色混合
extern void PutCmprsImgNOTClipingAdd565_CPP(int x, int y, Spr* sp, void* dest, int destPitch, int alpha);

// 3. 減色混合
extern void PutCmprsImgNOTClipingSub565_CPP(int x, int y, Spr* sp, void* dest, int destPitch, int alpha);

// 4. 半透明混合 (50% Alpha)
extern void PutCmprsImgNOTClipingHBlend565_CPP(int x, int y, Spr* sp, void* dest, int destPitch);

// 5. 差異加色混合
extern void PutCmprsImgNOTClipingAddDifer565_CPP(int x, int y, Spr* sp, void* dest, int destPitch, int alpha);

// 6. 差異 Alpha 混合
extern void PutCmprsImgNOTClipingBlendDifer565_CPP(int x, int y, Spr* sp, void* dest, int destPitch, int alpha);

extern void PutCmprsImgVoidCliping_CPP(int x, int y, Spr* sp, void* dest, int destPitch, int lx, int rx, int ty, int by);
extern void PutCmprsImgNOTClipingShadow565_CPP(int x, int y, int yl,const void* Stmp, void* dest, uint32_t dDxSize);
extern void PutCmprsImgVoidNOTCliping_CPP(int x, int y, Spr* sp, void* dest, int destPitch);
