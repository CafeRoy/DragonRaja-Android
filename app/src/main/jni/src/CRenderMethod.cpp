#include "CRenderMethod.h"
#include "SDLRenderer.h"
#include <filesystem> // C++17 for path handling
#include <vector>
#include <string>
#include <fstream>
#include <optional>
namespace fs = std::filesystem;

extern int tilewavetable[400];
/* = {2, 2, 2, 2, 4, 4, 4, 4, 6, 6, 6, 6, 8, 8, 8, 8, 8,10,10,10,
10,10, 8, 8, 8, 8, 8, 6, 6, 6, 6, 6, 4, 4, 4, 4, 2, 2, 2, 2,
2, 2, 2, 4, 4, 4, 4, 6, 6, 6, 6, 8, 8, 8, 8, 8,10,10,10,10,
10, 8, 8, 8, 8, 8, 6, 6, 6, 6, 6, 4, 4, 4, 4, 2, 2, 2,
2,2,2,2,4,4,4,4,6,6,6,6,8,8,8,8,8,10,10,10,10,10,8,8,8,8,8,6,6,6,6,6,4,4,4,4,2,2,2,
2,2,2,2,4,4,4,4,6,6,6,6,8,8,8,8,8,10,10,10,10,10,8,8,8,8,8,6,6,6,6,6,4,4,4,4,2,2,2,
2,2,2,2,4,4,4,4,6,6,6,6,8,8,8,8,8,10,10,10,10,10,8,8,8,8,8,6,6,6,6,6,4,4,4,4,2,2,2,
2,2,2,2,4,4,4,4,6,6,6,6,8,8,8,8,8,10,10,10,10,10,8,8,8,8,8,6,6,6,6,6,4,4,4,4,2,2,2,
};*/

CRenderMethod g_RenderMethod;
FireflyParticleSystem g_fireflySystem;

CRenderMethod::~CRenderMethod()
{
}

void CRenderMethod::PutCompressedImage(int x, int y, Spr* s, TextureInfo& info, SDL_Renderer* renderer)
{
	if (s == nullptr) return;

	int xl = s->xl; // 圖片寬度
	int yl = s->yl; // 圖片高度

	if (xl == 0 || yl == 0) return;

	// 1. 根據 Spr 結構體中的偏移量 (ox, oy) 計算螢幕左上角座標
	int dest_x = x - s->ox;
	int dest_y = y - s->oy;

	// 2. 創建目標矩形 (Destination Rectangle)
	SDL_Rect destRect = {
		dest_x, // 螢幕上的 X 座標
		dest_y, // 螢幕上的 Y 座標
		xl,     // 圖片寬度 (目標寬度)
		yl      // 圖片高度 (目標高度)
	};

	// 3. 執行渲染呼叫
	// 參數: Renderer, 來源紋理, 紋理來源區域 (Atlas 上的位置), 螢幕目標區域
	int ret = SDL_RenderCopy(renderer, info.atlasTexture, &info.sourceRect, &destRect);
	if (ret < 0)
	{
		SDL_Log("PutCompressedImage error[%s]", SDL_GetError());
	}
}

void CRenderMethod::PutCompressedImageCharRGB(int x, int y, Spr* s, TextureInfo info, uint32_t RGB, SDL_Renderer* renderer)
{
	if (s == nullptr) return;

	int xl = s->xl; // 圖片寬度
	int yl = s->yl; // 圖片高度

	if (xl == 0 || yl == 0) return;

	// 1. 根據 Spr 結構體中的偏移量 (ox, oy) 計算螢幕左上角座標
	int dest_x = x - s->ox;
	int dest_y = y - s->oy;

	// 2. 創建目標矩形 (Destination Rectangle)
	SDL_Rect destRect = {
		dest_x, // 螢幕上的 X 座標
		dest_y, // 螢幕上的 Y 座標
		xl,     // 圖片寬度 (目標寬度)
		yl      // 圖片高度 (目標高度)
	};

	// 1. 從 uint32_t (0xRRGGBB) 中提取 8-bit R, G, B
	Uint8 r = (RGB >> 16) & 0xFF;
	Uint8 g = (RGB >> 8) & 0xFF;
	Uint8 b = RGB & 0xFF;

	// =======================================================
	// 關鍵修正點
	// =======================================================

	// 2. 設定混合模式：使用標準 Alpha 混合
	// SDL_BLENDMODE_BLEND 會利用紋理中每個像素自帶的 Alpha 值。
	SDL_SetTextureBlendMode(info.atlasTexture, SDL_BLENDMODE_BLEND);

	// 3. 設定顏色調製 (Color Mod)：將整個紋理著色
	// 這會將紋理的 R, G, B 值乘以您傳入的顏色 (R, G, B)。
	// 由於您的紋理是灰階遮罩 (R=G=B)，這個操作會將遮罩染上指定的顏色。
	SDL_SetTextureColorMod(info.atlasTexture, r, g, b);

	// 4. 執行渲染呼叫
	SDL_RenderCopy(renderer, info.atlasTexture, &info.sourceRect, &destRect);

	// 5. 恢復狀態 (非常重要！)
	// 將顏色調製設回白色 (255, 255, 255)，避免影響下一個使用此紋理的指令
	SDL_SetTextureColorMod(info.atlasTexture, 255, 255, 255);
}

void CRenderMethod::PutCompressedImageFX(int x, int y, Spr* s, TextureInfo info, uint32_t alpha, uint32_t op, SDL_Renderer* renderer)
{
	if (s == nullptr) return;

	int xl = s->xl; // 圖片寬度
	int yl = s->yl; // 圖片高度

	if (xl == 0 || yl == 0) return;

	// 1. 根據 Spr 結構體中的偏移量 (ox, oy) 計算螢幕左上角座標
	int dest_x = x - s->ox;
	int dest_y = y - s->oy;

	// 2. 創建目標矩形 (Destination Rectangle)
	SDL_Rect destRect = {
		dest_x, // 螢幕上的 X 座標
		dest_y, // 螢幕上的 Y 座標
		xl,     // 圖片寬度 (目標寬度)
		yl      // 圖片高度 (目標高度)
	};

	// =======================================================
	// 修正點 1: Alpha 映射和設定
	// 1.1. 執行 Alpha 映射 (0-16 -> 0-255)
	Uint8 draw_alpha = 0;
	if (alpha == 0) {
		draw_alpha = 0;
	}
	else {
		// 線性映射並四捨五入
		draw_alpha = (Uint8)(((float)alpha / 24.0) * 255.0 + 0.5);
	}

	switch (op)
	{
	case 1://alpha//屋頂spr有問題
		CRenderMethod::PutCmprsImgBlend565FX(renderer, destRect, info, draw_alpha);
		break;
	case 2://add
		CRenderMethod::PutCmprsImgAdd565FX(renderer, destRect, info, draw_alpha);
		break;
	case 4://H alpha
		CRenderMethod::PutCmprsImgBlend565FX(renderer, destRect, info, draw_alpha);
		break;
	}

	/*if (op == 1)
	{
		// 1.2. 設定混合模式和 Alpha 值
		SDL_SetTextureBlendMode(info.atlasTexture, SDL_BLENDMODE_BLEND);
		SDL_SetTextureAlphaMod(info.atlasTexture, draw_alpha);
		// =======================================================

		// 2. 執行渲染呼叫
		SDL_RenderCopy(renderer, info.atlasTexture, &info.sourceRect, &destRect);

		// 3. 恢復紋理狀態 (非常重要！)
		SDL_SetTextureAlphaMod(info.atlasTexture, 255); // 設回完全不透明
	}
	else if (op == 2) {
		//
	    // 2. 設定為顏色相加模式 (Additive Blending)
		SDL_SetTextureBlendMode(info.atlasTexture, SDL_BLENDMODE_ADD);

		// 3. 設定 Alpha 權重
		// 這個 AlphaMod 會在顏色相加之前，先降低源紋理的強度 (模擬您的 *alpha/16)
		SDL_SetTextureAlphaMod(info.atlasTexture, draw_alpha);

		// 4. 執行渲染呼叫
		SDL_RenderCopy(renderer, info.atlasTexture, &info.sourceRect, &destRect);

		// 5. 恢復狀態 (非常重要！)
		// 恢復 Alpha 值為不透明，並將混合模式設回預設的 BLEND，以避免影響下一個繪圖指令
		SDL_SetTextureAlphaMod(info.atlasTexture, 255);
		SDL_SetTextureBlendMode(info.atlasTexture, SDL_BLENDMODE_BLEND); // 恢復到標準 Alpha 混合模式
	}*/
	
}

void CRenderMethod::PutCompressedImageShadow(int x, int y, Spr* sp, TextureInfo info)
{
	int	lx, rx, ty, by, IsC = 0;
	int xl = sp->xl;
	int yl = sp->yl;

	if (sp == NULL) return;
	if (sp->img == NULL) return;

	x -= sp->ox;
	y -= sp->oy;

	if (x >= SCREEN_WIDTH)			return;
	else if (x + xl < 0)	return;
	if (y >= GAME_SCREEN_YSIZE)			return;
	else if (y + yl < 0)	return;

	rx = xl;
	lx = 0;
	ty = 0;
	by = yl;

	if (x + xl >= SCREEN_WIDTH)
	{
		rx -= x + xl - SCREEN_WIDTH;
		IsC = 1;
	}
	if (x < 0)
	{
		lx = 0 - x;
		x = 0;
		rx -= lx;
		IsC = 1;
	}
	if (y + yl >= GAME_SCREEN_YSIZE)
	{
		by -= y + yl - GAME_SCREEN_YSIZE;
		IsC = 1;
	}
	if (y < 0)
	{
		ty = 0 - y;
		y = 0;
		by -= ty;
		IsC = 1;
	}

	//if (!IsC)
		//PutCmprsImgNOTClipingShadow565_CPP(x, y, sp, pixels, pitch);

}

void CRenderMethod::PutCompressedImageVoid(int x, int y, Spr* sp, TextureInfo info)
{
	int	lx, rx, ty, by, IsC = 0;

	if (sp == NULL) return;
	if (sp->img == NULL) return;


	int xl = sp->xl;
	int yl = sp->yl;

	if (xl == 0) return;
	if (yl == 0) return;

	x -= sp->ox;
	y -= sp->oy;

	if (x >= (SCREEN_WIDTH - 2))	return;
	else if (x + xl < 0)			return;

	if (y >= SCREEN_HEIGHT)		return;
	else if (y + yl < 0)			return;

	rx = xl;
	lx = 0;
	ty = 0;
	by = yl;

	if (x + xl >= (SCREEN_WIDTH - 2))
	{
		rx -= x + xl - (SCREEN_WIDTH - 2);
		IsC = 1;
	}
	if (x < 0)
	{
		lx = 0 - x;
		x = 0;
		rx -= lx;
		IsC = 1;
	}

	if (y + yl >= SCREEN_HEIGHT)
	{
		by -= y + yl - SCREEN_HEIGHT;
		IsC = 1;
	}
	if (y < 0)
	{
		ty = 0 - y;
		y = 0;
		by -= ty;
		IsC = 1;
	}



}

void CRenderMethod::PutCompressedImageWaveTile(int x, int y, Spr* s, TextureInfo info)
{
	SDL_Texture* texture = info.atlasTexture;

	if (!texture) return;
	int g_wave_counter = 0;
	// 2. 進行座標校正
	x -= s->ox;
	y -= s->oy;

	// 3. 【核心】逐行切片渲染
	for (int i = 0; i < info.sourceRect.h; ++i) {

		// a. 增加全域波浪計數器
		// 注意：原始 ASM 的計數器邏輯可能略有不同，您可能需要根據實際效果
		// 調整這個計數器是在迴圈內還是迴圈外增加。
		g_wave_counter++;

		// b. 計算當前行的水平偏移量
		int wave_offset = tilewavetable[g_wave_counter % 400];

		// c. 定義來源切片 (Source Slice)
		// 我們從圖集中的 Sprite 位置，摳出高度為 1 的一行。
		SDL_Rect src_slice = {
			info.sourceRect.x,          // X 座標不變
			info.sourceRect.y + i,      // Y 座標是當前行
			info.sourceRect.w,          // 寬度不變
			1                           // 高度永遠是 1
		};

		// d. 定義目標位置 (Destination Position)
		// 我們將這個切片畫到螢幕上，並應用波浪偏移。
		SDL_Rect dest_pos = {
			x + wave_offset,            // 【關鍵】應用水平偏移
			y + i,                      // Y 座標是當前行
			info.sourceRect.w,          // 寬度不變
			1                           // 高度是 1
		};

		// e. 執行單行繪製
		SDL_RenderCopy(g_SDLInfo.renderer, texture, &src_slice, &dest_pos);
	}
}

void CRenderMethod::PutTile(int x, int y, TextureInfo info)
{
	//普通貼圖
	SDL_Texture* texture = info.atlasTexture;
	// 4. 設定目標繪製位置和大小，並進行座標校正
	SDL_Rect destRect = {
		x,
		y,
		TILE_SIZE,//info.sourceRect.w,
		TILE_SIZE//info.sourceRect.h
	};
	// 2. 繪製紋理
	SDL_RenderCopy(g_SDLInfo.renderer, texture, &info.sourceRect, &destRect);
}

void CRenderMethod::PutTileFX(int x, int y, TextureInfo info, int alpha)
{
	//普通貼圖
	SDL_Texture* texture = info.atlasTexture;
	// 4. 設定目標繪製位置和大小，並進行座標校正
	SDL_Rect destRect = {
		x,
		y,
		TILE_SIZE,//info.sourceRect.w,
		TILE_SIZE//info.sourceRect.h
	};
	// =======================================================
	// 修正點 1: Alpha 映射和設定
	// 1.1. 執行 Alpha 映射 (0-16 -> 0-255)
	Uint8 draw_alpha = 0;
	if (alpha == 0) {
		draw_alpha = 0;
	}
	else {
		// 線性映射並四捨五入
		draw_alpha = (Uint8)(((float)alpha / 24.0) * 255.0 + 0.5);
	}

	// 1.2. 設定混合模式和 Alpha 值
	SDL_SetTextureBlendMode(info.atlasTexture, SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(info.atlasTexture, draw_alpha);
	// =======================================================

	// 2. 執行渲染呼叫
	SDL_RenderCopy(g_SDLInfo.renderer, info.atlasTexture, &info.sourceRect, &destRect);

	// 3. 恢復紋理狀態 (非常重要！)
	SDL_SetTextureAlphaMod(info.atlasTexture, 255); // 設回完全不透明
}

/*
 * 範例：透明度繪製出來
 */
void CRenderMethod::PutCmprsImgBlend565FX(SDL_Renderer* renderer,SDL_Rect rc, TextureInfo info, int draw_alpha)
{
	// 1.2. 設定混合模式和 Alpha 值
	SDL_SetTextureBlendMode(info.atlasTexture, SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(info.atlasTexture, draw_alpha);
	// =======================================================

	// 2. 執行渲染呼叫
	SDL_RenderCopy(renderer, info.atlasTexture, &info.sourceRect, &rc);

	// 3. 恢復紋理狀態 (非常重要！)
	SDL_SetTextureAlphaMod(info.atlasTexture, 255); // 設回完全不透明
}

/*注意：要讓這個函式生效，紋理的混合模式必須是 SDL_BLENDMODE_BLEND。我們在之前建立紋理時，
已經透過 SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND)
設定好了，所以這裡不需要重複設定。三、Add(加色混合) 的實現方法原始 ASM
做法：逐像素計算 最終顏色 = 來源色 + 背景色。新的 SDL 做法：使用 SDL_SetTextureBlendMode()。
這個函式更為強大，它直接改變了 GPU 混合顏色的數學公式。如何使用：
範例：繪製一個發光的路燈光暈
*/

void CRenderMethod::PutCmprsImgAdd565FX(SDL_Renderer* renderer, SDL_Rect rc, TextureInfo info, int draw_alpha)
{
	// 2. 設定為顏色相加模式 (Additive Blending)
	SDL_SetTextureBlendMode(info.atlasTexture, SDL_BLENDMODE_ADD);

	// 3. 設定 Alpha 權重
	// 這個 AlphaMod 會在顏色相加之前，先降低源紋理的強度 (模擬您的 *alpha/16)
	SDL_SetTextureAlphaMod(info.atlasTexture, draw_alpha);

	// 4. 執行渲染呼叫
	SDL_RenderCopy(renderer, info.atlasTexture, &info.sourceRect, &rc);

	// 5. 恢復狀態 (非常重要！)
	// 恢復 Alpha 值為不透明，並將混合模式設回預設的 BLEND，以避免影響下一個繪圖指令
	SDL_SetTextureAlphaMod(info.atlasTexture, 255);
	SDL_SetTextureBlendMode(info.atlasTexture, SDL_BLENDMODE_BLEND); // 恢復到標準 Alpha 混合模式
}

void CRenderMethod::PutCmprsImgAddDifer565_CPP(int x, int y, Spr* sp, TextureInfo info, int dDxSize, int alpha)
{
	int	lx, rx, ty, by, IsC = 0;
	int xl = sp->xl;
	int yl = sp->yl;

	if (sp == NULL) return;
	if (sp->img == NULL) return;

	x -= sp->ox;
	y -= sp->oy;

	if (x >= SCREEN_WIDTH)			return;
	else if (x + xl < 0)	return;
	if (y >= GAME_SCREEN_YSIZE)			return;
	else if (y + yl < 0)	return;

	rx = xl;
	lx = 0;
	ty = 0;
	by = yl;

	if (x + xl >= SCREEN_WIDTH)
	{
		rx -= x + xl - SCREEN_WIDTH;
		IsC = 1;
	}
	if (x < 0)
	{
		lx = 0 - x;
		x = 0;
		rx -= lx;
		IsC = 1;
	}
	if (y + yl >= GAME_SCREEN_YSIZE)
	{
		by -= y + yl - GAME_SCREEN_YSIZE;
		IsC = 1;
	}
	if (y < 0)
	{
		ty = 0 - y;
		y = 0;
		by -= ty;
		IsC = 1;
	}

}
void CRenderMethod::PutCmprsImgSub565_CPP(int x, int y, Spr* sp, TextureInfo info, int dDxSize, int alpha)
{
	int	lx, rx, ty, by, IsC = 0;
	int xl = sp->xl;
	int yl = sp->yl;

	if (sp == NULL) return;
	if (sp->img == NULL) return;

	x -= sp->ox;
	y -= sp->oy;

	if (x >= SCREEN_WIDTH)			return;
	else if (x + xl < 0)	return;
	if (y >= GAME_SCREEN_YSIZE)			return;
	else if (y + yl < 0)	return;

	rx = xl;
	lx = 0;
	ty = 0;
	by = yl;

	if (x + xl >= SCREEN_WIDTH)
	{
		rx -= x + xl - SCREEN_WIDTH;
		IsC = 1;
	}
	if (x < 0)
	{
		lx = 0 - x;
		x = 0;
		rx -= lx;
		IsC = 1;
	}
	if (y + yl >= GAME_SCREEN_YSIZE)
	{
		by -= y + yl - GAME_SCREEN_YSIZE;
		IsC = 1;
	}
	if (y < 0)
	{
		ty = 0 - y;
		y = 0;
		by -= ty;
		IsC = 1;
	}
}
void CRenderMethod::PutCmprsImgBlendDifer565_CPP(int x, int y, Spr* sp, TextureInfo info, int dDxSize, int alpha)
{
	int	lx, rx, ty, by, IsC = 0;
	int xl = sp->xl;
	int yl = sp->yl;

	if (sp == NULL) return;
	if (sp->img == NULL) return;

	x -= sp->ox;
	y -= sp->oy;

	if (x >= SCREEN_WIDTH)			return;
	else if (x + xl < 0)	return;
	if (y >= GAME_SCREEN_YSIZE)			return;
	else if (y + yl < 0)	return;

	rx = xl;
	lx = 0;
	ty = 0;
	by = yl;

	if (x + xl >= SCREEN_WIDTH)
	{
		rx -= x + xl - SCREEN_WIDTH;
		IsC = 1;
	}
	if (x < 0)
	{
		lx = 0 - x;
		x = 0;
		rx -= lx;
		IsC = 1;
	}
	if (y + yl >= GAME_SCREEN_YSIZE)
	{
		by -= y + yl - GAME_SCREEN_YSIZE;
		IsC = 1;
	}
	if (y < 0)
	{
		ty = 0 - y;
		y = 0;
		by -= ty;
		IsC = 1;
	}

	// 4. 鎖定紋理並呼叫【帶裁剪功能】的 C++ 函式
	void* pixels;
	int pitch;
	if (SDL_LockTexture(g_SDLInfo.streamingTexture, NULL, &pixels, &pitch) != 0) return;

	if (!IsC)
		PutCmprsImgNOTClipingBlendDifer565_CPP(x, y, sp, pixels, pitch, alpha);
	else PutCmprsImgClipingBlendDifer565_CPP(x, y, sp, pixels, pitch, lx, rx, ty, by, alpha);

	SDL_UnlockTexture(g_SDLInfo.streamingTexture);
}

// 載入粒子貼圖 (我們可以使用與裝備光環相同的柔和光暈貼圖)
bool FireflyParticleSystem::LoadParticleTexture(const char* bmp_filename) {
	if (m_pParticleSurface) { SDL_FreeSurface(m_pParticleSurface); }
	m_pParticleSurface = SDL_LoadBMP(bmp_filename);
	if (!m_pParticleSurface) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, u8"無法載入螢火蟲貼圖: %s", SDL_GetError());
		return false;
	}
	// 螢火蟲我們使用 ADD 混合模式，來創造漂亮的發光效果
	SDL_SetSurfaceBlendMode(m_pParticleSurface, SDL_BLENDMODE_ADD);
	SDL_SetColorKey(m_pParticleSurface, SDL_TRUE, SDL_MapRGB(m_pParticleSurface->format, 0, 0, 0));
	return true;
}

// 【改造】Emit 函式現在在鏡頭周圍的「世界空間」中生成粒子
void FireflyParticleSystem::Emit(int camera_world_x, int camera_world_y) {
	// --- 【關鍵改造：加入距離判斷】 ---
	const float MIN_DISTANCE = 96.0f; // 螢火蟲之間的最小間距
	const float MIN_DISTANCE_SQUARED = MIN_DISTANCE * MIN_DISTANCE;
	const int MAX_ATTEMPTS = 20; // 最多嘗試 20 次來尋找一個安全的位置

	for (int attempt = 0; attempt < MAX_ATTEMPTS; ++attempt) {
		// a. 產生一個隨機的「候選世界座標」
		float candidate_world_x = static_cast<float>(camera_world_x + (rand() % 2000 - 1000));
		float candidate_world_y = static_cast<float>(camera_world_y + (rand() % 2000 - 1000));

		// b. 檢查這個位置是否與現有粒子距離太近
		bool is_safe_position = true;
		for (const auto& existing_particle : m_particles) {
			float dx = existing_particle.world_x - candidate_world_x;
			float dy = existing_particle.world_y - candidate_world_y;
			if ((dx * dx + dy * dy) < MIN_DISTANCE_SQUARED) {
				is_safe_position = false;
				break;
			}
		}

		// c. 如果位置安全，就在這裡生成粒子並結束函式
		if (is_safe_position) {
			FireflyParticle p;
			p.world_x = candidate_world_x;
			p.world_y = candidate_world_y;

			// 賦予一個非常緩慢的、隨機的漂浮速度
			p.vx = (rand() % 100 / 50.0f - 1.0f) * 10.0f; // -10 到 +10 的速度
			p.vy = (rand() % 100 / 50.0f - 1.0f) * 10.0f;
			p.lifetime = 10.0f + (rand() % 1000) / 100.0f; // 10-20 秒
			p.blink_speed = 0.1f + (rand() % 100) / 1000.0f;
			p.blink_offset = (rand() % 1000 / 100.0f);

			m_particles.push_back(p);
			return; // 成功生成，返回
		}
	}
	// 如果嘗試多次都找不到安全位置，這一幀就不再生成
}

void FireflyParticleSystem::Cleanup() {
	if (m_pParticleSurface) {
		SDL_FreeSurface(m_pParticleSurface);
		m_pParticleSurface = nullptr;
	}
}

// 【改造】Update 函式現在需要知道鏡頭的中心點在哪裡
void FireflyParticleSystem::Update(float deltaTime, int camera_world_x, int camera_world_y) {
	// 1. 根據鏡頭位置，在可視範圍內補充新的螢火蟲
	const int MAX_FIREFLIES = 10;
	if (m_particles.size() < MAX_FIREFLIES) {
		Emit(camera_world_x, camera_world_y);
	}

	// 2. 更新所有已存在的螢火蟲
	for (auto it = m_particles.begin(); it != m_particles.end(); ) {
		it->lifetime -= deltaTime;
		// 如果生命結束，或者飄得離鏡頭太遠，就移除
		if (it->lifetime <= 0 ||
			abs(it->world_x - camera_world_x) > 1500 || // 假設視野範圍是 1000 像素
			abs(it->world_y - camera_world_y) > 1500)
		{
			it = m_particles.erase(it);
		}
		else {
			// 更新世界座標
			it->world_x += it->vx * deltaTime;
			it->world_y += it->vy * deltaTime;
			++it;
		}
	}
}

// 【改造】Render 函式現在需要知道地圖的偏移量，來轉換座標
void FireflyParticleSystem::Render(SDL_Surface* targetCanvas, int map_offset_x, int map_offset_y) {
	if (!targetCanvas || !m_pParticleSurface) return;
	const float PARTICLE_SIZE = 5.0f; // 定義粒子大小
	for (const auto& p : m_particles) {
		// ... 閃爍的亮度計算保持不變 ...
		float brightness = (sin(p.lifetime * p.blink_speed + p.blink_offset) + 1.0f) / 2.0f;
		SDL_SetSurfaceColorMod(m_pParticleSurface, (Uint8)(255 * brightness), (Uint8)(255 * brightness), (Uint8)(150 * brightness));
		SDL_SetSurfaceAlphaMod(m_pParticleSurface, (Uint8)(brightness * 255));

		// 【關鍵】將粒子的「世界座標」轉換為「螢幕座標」
		int screen_x = static_cast<int>(p.world_x) - map_offset_x;
		int screen_y = static_cast<int>(p.world_y) - map_offset_y;

		SDL_Rect destRect;
		destRect.w = static_cast<int>(PARTICLE_SIZE);
		destRect.h = static_cast<int>(PARTICLE_SIZE);
		destRect.x = static_cast<int>(screen_x - PARTICLE_SIZE / 2.0f);
		destRect.y = static_cast<int>(screen_y - PARTICLE_SIZE / 2.0f);

		SDL_BlitScaled(m_pParticleSurface, NULL, targetCanvas, &destRect);
	}
}