
// ==============================================================================
// Weather System Refactored
// 重構說明:
// 1. 將分散的陣列整合為結構體 (struct)，提升記憶體存取效率。
// 2. 修復了 Wind 邏輯中複製貼上導致的錯誤 (如 windsy[MAX_SNOW])。
// 3. 優化渲染判斷，增加視錐剔除 (Culling)。
// 4. 變數命名規範化，保留原函式簽名以便相容。
// ==============================================================================

#include <cstring>
#include <cstdlib> // For rand/abs if needed, assuming Random() is global
#include <algorithm> // For std::min, std::max

// ------------------------------------------------------------------------------
// 常數定義 (集中管理，方便修改)
// ------------------------------------------------------------------------------
#define MAX_WEATHER_BUFFER  4000 

// 為了相容你的代碼習慣，重新定義這兩個宏指向新的緩衝區大小
#define MAX_RAIN MAX_WEATHER_BUFFER
#define MAX_SNOW MAX_WEATHER_BUFFER
#define MAX_WIND MAX_WEATHER_BUFFER
// 定義一個「基準解析度」，也就是您覺得 900 顆雪花看起來最剛好的那個解析度
// 假設原本設計是在 1024x768 左右看起來不錯
const int BASE_RES_WIDTH = 1280;
const int BASE_RES_HEIGHT = 720;
const int BASE_PARTICLE_COUNT = 900; // 在基準解析度下的數量

#define SPRITE_RAIN_BASE    880
#define SPRITE_SNOW_BASE    889 // 請確保外部有定義 SNOW_SPRITE_
#define SPRITE_WIND_BASE    79
#define Random( x )				(::rand() % ( (0 < (x) )? (x) : (1) ))
// ------------------------------------------------------------------------------
// 資料結構定義
// ------------------------------------------------------------------------------

// 粒子類型枚舉
enum WeatherType {
	WEATHER_RAIN = 0,
	WEATHER_SNOW,
	WEATHER_WIND
};

// 統一的粒子結構，取代原本散亂的 int 陣列
struct WeatherParticle {
    float x, y;          // 必須float，否則移動會卡頓
    float speedX, speedY; // 改為 float
    float endY;          // 觸地高度
	int spriteIdx;      // 圖片索引偏移量
	int alpha;          // 透明度 (Original: raintrans)
	int lifeState;      // 生命週期/狀態 (0: 下落中, >0: 觸地/消失動畫)

	// 雪/風 專用屬性
	int driftTimer;     // 飄動計時器 (Original: snowwind/windwind)
	int active;         // 是否啟用
	int animationPhase; // 動畫階段 (Original: snowflag)

	// 重置粒子狀態
	void Reset(WeatherType type, int mapY) {
		lifeState = 0;
		active = 1;
		animationPhase = 0;
		driftTimer = 0;

		int tt = (type == WEATHER_RAIN) ? 3 : 5;

		// 根據類型初始化不同參數
		if (type == WEATHER_RAIN) {
			// 雨的邏輯：根據高度決定速度和圖層
			// 優化：簡化原本複雜的 switch/case
			int distFactor = endY;
			if (distFactor < 200) {
				speedY = 15 + Random(5);
				spriteIdx = 6 + Random(3);
				speedX = 2 + Random(3);
			}
			else if (distFactor < 500) {
				speedY = 22 + Random(6);
				spriteIdx = 4 + Random(4);
				speedX = 2 + Random(3);
			}
			else {
				speedY = 30 + Random(4);
				spriteIdx = 2 + Random(2);
				speedX = 2 + Random(3);
			}
			alpha = tt + Random(3);

		}
		else { // SNOW and WIND share similar logic
			int relY = endY - mapY; // 相對螢幕距離

			// 根據距離設定速度 (模擬景深)
			int baseSpeed = 0;
			if (relY < 100) baseSpeed = 1;
			else if (relY < 200) baseSpeed = 3;
			else if (relY < 300) baseSpeed = 5;
			else baseSpeed = 6;

			speedY = baseSpeed + Random(4);
			spriteIdx = (type == WEATHER_SNOW) ? (5 + Random(2)) : (5 + Random(2));
			alpha = tt + Random(4);

			// 雪和風的初始水平速度較小，主要靠 Update 裡的風力影響
			speedX = 0;
		}
	}
};
