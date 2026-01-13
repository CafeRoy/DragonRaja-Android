#include "StdAfx.h"
#include "ParticleSystem.h"
#include "Battle.h"
#include "SDLRenderer.h"

// 這是新的「前方角度表」，假設向下為 90 度
static const float s_backward_angles_deg[] = {
    90.0f,    // DIRECTION_DOWN (0) -> 向下 (90°)
    135.0f,   // DIRECTION_LEFTDOWN (1) -> 左下 (135°)
    155.0f,   // DIRECTION_LEFT (2) -> 左方 (180°)
    200.0f,  // DIRECTION_LEFTUP (3) -> 左上 (225°)
    270.0f,  // DIRECTION_UP (4) -> 上方 (270°)
    340.0f,  // DIRECTION_RIGHTUP (5) -> 右上 (315°)
    25.0f,  // DIRECTION_RIGHT (6) -> 右方 (360°)
    45.0f,  // DIRECTION_RIGHTDOWN (7) -> 右下 (45°)

    // 以下是更細緻的 8 個方向 (請自行根據邏輯調整)
    112.5f,   // DIRECTION_LEFTDOWNDOWN (8)
    157.5f,   // DIRECTION_LEFTLEFTDOWN (9)
    202.5f,  // DIRECTION_LEFTLEFTUP (10)
    247.5f,  // DIRECTION_LEFTUPUP (11)
    292.5f,  // DIRECTION_RIGHTUPUP (12)
    337.5f,  // DIRECTION_RIGHTRIGHTUP (13)
    22.5f,  // DIRECTION_RIGHTRIGHTDOWN (14)
    67.5f   // DIRECTION_RIGHTDOWNDOWN (15)
};

ParticleSystem::ParticleSystem(size_t pool_size)
    : m_pool_size(pool_size), m_active_count(0) {
	// 預先分配粒子池的記憶體
    m_particle_pool.resize(pool_size);

	m_available_colors = nullptr;
	m_particleSurfaces = nullptr;
	m_particleTexture = nullptr;
	// 初始化所有粒子為非活躍狀態
    for (auto& p : m_particle_pool) {
        p.isActive = false;
    }
}
/**
 * @brief 更新粒子系統中所有活躍粒子的狀態。
 * @param deltaTime 自上一幀以來經過的時間（秒），用於實現與幀率無關的物理模擬。
 */
/*void ParticleSystem::Update(CCharacter* owner, float deltaTime) {
    // 定義影響粒子行為的物理常數
    const float DRAG = 0.2f;          // 空氣阻力係數，讓粒子速度隨時間衰減
    const float FLOAT_FORCE = -8.0f;  // 向上浮力，負值代表向上的力（Y軸向下為正）

    // 遍歷整個粒子池
    for (auto& p : m_particle_pool) {
        // 跳過所有非活躍的粒子，這是物件池模式的關鍵，避免處理無效資料
        if (!p.isActive) continue;

        // 更新剩餘生命週期
        p.lifetime -= deltaTime;

        // 檢查粒子生命是否結束
        if (p.lifetime <= 0) {
            p.isActive = false;  // 歸還粒子到物件池，將其標記為非活躍
            m_active_count--;    // 更新活躍粒子計數
        }
        else {
            // --- 物理模擬 ---
            // 1. 施加上浮力，改變垂直速度
            p.vy += FLOAT_FORCE * deltaTime;
            // 2. 施加空氣阻力，讓速度隨時間衰減
            p.vx *= (1.0f - DRAG * deltaTime);
            p.vy *= (1.0f - DRAG * deltaTime);
            // 3. 根據速度更新粒子的相對位置
            p.relative_x += p.vx * deltaTime;
            p.relative_y += p.vy * deltaTime;

            // --- 視覺效果：呼吸式大小變化 ---
            // 計算生命週期比例 (從 1.0 降到 0.0)
            float life_ratio = p.lifetime / p.initial_lifetime;
            // 使用 sin 函數，讓粒子大小在生命週期內呈現由小變大再變小的呼吸效果
            float size_multiplier = sin(life_ratio * 3.14159f);
            const float MAX_SIZE = 18.0f;
            p.size = MAX_SIZE * size_multiplier;
        }
    }
}*/

bool ParticleSystem::IsFinished() const {
    // 預設情況下，一個粒子系統永遠不會自己結束。
    // 這個行為非常適合「常駐」的特效，例如光環 (Aura)。
    // 它的生命週期由它的主人 CHARACTER 決定。
    //
    // 對於「一次性」的特效（如火箭、爆炸），它們必須覆寫 (override) 這個函式，
    // 提供自己判斷結束的條件（例如所有粒子都消失）。
    return false;
}
#ifdef _TEXTURE
bool ParticleSystem::SetSharedResources(std::map<std::string, SDL_Texture*>* _particleTexture,
    std::vector<std::string>* _available_colors)
{

    m_particleTexture = _particleTexture;
    m_available_colors = _available_colors;
    return true;
}
#else
/**
 * @brief 載入粒子紋理，並預先生成所有顏色和透明度的變化版本。
 * @param bmp 基礎粒子圖片的路徑 (BMP 格式)。
 * @return true 如果載入和生成成功，否則 false。
 */
bool ParticleSystem::SetSharedResources(std::map<std::string, SDL_Surface*>* _particleSurface,
    std::vector<std::string>* _available_colors)
{

    m_particleSurfaces = _particleSurface;
    m_available_colors = _available_colors;
    return true;
}
#endif
/**
 * @brief 將所有活躍的粒子渲染到目標畫布上。
 * @param targetCanvas 目標 SDL_Surface，通常是後台緩衝區。
 * @param emitter_screen_x 發射器在螢幕上的X座標。
 * @param emitter_screen_y 發射器在螢幕上的Y座標。
 */
void ParticleSystem::Render(SDL_Surface* targetCanvas, float emitter_screen_x,
    float emitter_screen_y) {
    if (!targetCanvas || !m_particleSurfaces || m_particleSurfaces->empty()) return;

    for (const auto& p : m_particle_pool) {
        if (!p.isActive) continue;

        // --- 根據生命週期計算透明度 ---
        float life_ratio = p.lifetime / p.initial_lifetime;
        int alpha_percentage = static_cast<int>(life_ratio * 100);
        int closest_alpha = (alpha_percentage / 10) * 10;
        if (closest_alpha > 50) closest_alpha = 50;
        if (closest_alpha < 0) closest_alpha = 0;

       

        // --- 計算最終繪製位置 ---
        SDL_Rect destRect;
        destRect.w = static_cast<int>(p.size);
        destRect.h = static_cast<int>(p.size);
		destRect.x = static_cast<int>(p.world_x - emitter_screen_x - p.size / 2.0f);
		destRect.y = static_cast<int>(p.world_y - emitter_screen_y - p.size / 2.0f);
        destRect.y -= 10;  // 額外的 Y 軸偏移
#ifdef _TEXTURE
        std::string key = p.color + "_alpha_" + std::to_string(closest_alpha);

        SDL_Texture* current_texture =
            m_particleTexture->count(key) ? m_particleTexture->at(key) : nullptr;

        if (current_texture == nullptr) {
            continue;
        }
        int w, h;
        SDL_QueryTexture(current_texture, 0, 0, &w, &h);
        SDL_Rect rc{ 0,0,w,h };
        SDL_RenderCopy(g_SDLInfo.renderer, current_texture, &rc, &destRect);
#else
        // --- 【核心修改】使用粒子自身的顏色來組合 key ---
        std::string key = p.color + "_alpha_" + std::to_string(closest_alpha);

        SDL_Surface* current_surface =
            m_particleSurfaces->count(key) ? m_particleSurfaces->at(key) : nullptr;

        if (current_surface == nullptr) {
            continue;
        }
        // 將粒子紋理繪製到目標畫布上
        SDL_BlitScaled(current_surface, NULL, targetCanvas, &destRect);
#endif
    }
}


AuraParticleSystem::AuraParticleSystem(size_t pool_size)
    : ParticleSystem(pool_size) {
}

// Update 函式：貼上您原本光環的物理邏輯（向上漂浮）
std::optional<EffectCommand> AuraParticleSystem::Update(CCharacter* owner, float deltaTime) {
    // 定義影響粒子行為的物理常數
    const float DRAG = 0.2f;          // 空氣阻力係數，讓粒子速度隨時間衰減
    const float FLOAT_FORCE = -8.0f;  // 向上浮力，負值代表向上的力（Y軸向下為正）

    // 遍歷整個粒子池
    for (auto& p : m_particle_pool) {
        // 跳過所有非活躍的粒子，這是物件池模式的關鍵，避免處理無效資料
        if (!p.isActive) continue;

        // 更新剩餘生命週期
        p.lifetime -= deltaTime;

        // 檢查粒子生命是否結束
        if (p.lifetime <= 0) {
            p.isActive = false;  // 歸還粒子到物件池，將其標記為非活躍
            m_active_count--;    // 更新活躍粒子計數
        }
        else {
            // --- 物理模擬 ---
            // 1. 施加上浮力，改變垂直速度
            p.vy += FLOAT_FORCE * deltaTime;
            // 2. 施加空氣阻力，讓速度隨時間衰減
            p.vx *= (1.0f - DRAG * deltaTime);
            p.vy *= (1.0f - DRAG * deltaTime);
            p.world_x = owner->visual_x + p.relative_x;
            p.world_y = owner->visual_y + p.relative_y;
            // 3. 根據速度更新粒子的相對位置
            // 光環粒子本身也可以有自己的小範圍漂浮運動
            p.world_x += p.vx * deltaTime;
            p.world_y += p.vy * deltaTime;

            // --- 視覺效果：呼吸式大小變化 ---
            // 計算生命週期比例 (從 1.0 降到 0.0)
            float life_ratio = p.lifetime / p.initial_lifetime;
            // 使用 sin 函數，讓粒子大小在生命週期內呈現由小變大再變小的呼吸效果
            float size_multiplier = sin(life_ratio * 3.14159f);
            const float MAX_SIZE = 18.0f;
            p.size = MAX_SIZE * size_multiplier;
        }
    }

    return std::nullopt;
}

std::string AuraParticleSystem::GetParticleColorByPassiveCombat(int passive)
{
    int max = m_available_colors->size();// max = 10 + 1
    switch (passive)
    {
    case battle::GUST:return m_available_colors->at(5);
	case battle::TYBERN_GIFT: return m_available_colors->at(1);
	case battle::BURNING_BLOODY: return m_available_colors->at(10);
	case battle::STONE_ARMOR: return m_available_colors->at(3);
	case battle::LIGHTNING_SHIELD: return m_available_colors->at(8);
    }
    return "";
}

// Emit 函式：光環設計的發射邏輯
void AuraParticleSystem::Emit(CCharacter* owner, const std::vector<SDL_Point>& outlinePoints) {
    // 安全性檢查：如果沒有輪廓點、粒子池已滿、或沒有可用顏色，則不發射
    if (outlinePoints.size() < 2) return;
    if (m_active_count >= m_pool_size) return;
    if (!m_available_colors || m_available_colors->empty()) return;
	if (owner->nPassiveCombat <= 0) return;

    // 遍歷粒子池，尋找第一個非活躍的粒子來重複利用
    for (auto& p : m_particle_pool) {
        if (!p.isActive) {
            // 找到了閒置粒子，將其激活
            p.isActive = true;
            m_active_count++;

            // --- 初始化粒子屬性 ---

            // 【核心修改】在粒子誕生時，從可用顏色列表中隨機賦予一個顏色
            //p.color = m_available_colors[rand() % m_available_colors.size()];
			p.color = GetParticleColorByPassiveCombat(owner->nPassiveCombat);
            int padding_y = owner->peacests ? 22 : 16;
            // 在輪廓線段上隨機取一個點作為出生位置
            int index1 = rand() % outlinePoints.size();
            int index2 = (index1 + 1) % outlinePoints.size();
            const SDL_Point& p1 = outlinePoints[index1];
            const SDL_Point& p2 = outlinePoints[index2];
            float t = (rand() % 100) / 100.0f;
            float offset_x = p1.x + t * (p2.x - p1.x);
            float offset_y = p1.y + t * (p2.y - p1.y) - padding_y;

            p.relative_x = offset_x;
            p.relative_y = offset_y;
            p.world_x = owner->visual_x + p.relative_x;
            p.world_y = owner->visual_y + p.relative_y;
            // 設定隨機的初始速度、方向和生命週期
            float angle = (rand() % 360) * 3.14159f / 180.0f;
            float speed = static_cast<float>(5 + (rand() % 10));
            p.vx = cos(angle) * speed;
            p.vy = sin(angle) * speed;
            p.lifetime = 0.5f + (rand() % 100) / 100.0f;
            p.initial_lifetime = p.lifetime;

            return;  // 成功發射了一個粒子，結束函式
        }
    }
}

// 1. 建構函式：使用成員初始化列表來呼叫基類的建構函式
SparkParticleSystem::SparkParticleSystem(size_t pool_size)
    : ParticleSystem(pool_size) {
}

// 2. 覆寫 Update 函式，實現帶有重力的物理模擬
std::optional<EffectCommand> SparkParticleSystem::Update(CCharacter* owner, float deltaTime) {
    const float DRAG = 0.1f;
    const float GRAVITY = 100.0f; // 火花受重力影響會下墜

    // 迴圈邏輯與基類相似，但物理行為是客製化的
    for (auto& p : m_particle_pool) {
        if (!p.isActive) continue;

        p.lifetime -= deltaTime;
        if (p.lifetime <= 0) {
            p.isActive = false;
            m_active_count--;
        }
        else {
            // --- 火花專屬的物理模擬 ---
            p.vy += GRAVITY * deltaTime; // 施加重力
            p.vx *= (1.0f - DRAG * deltaTime);
            p.vy *= (1.0f - DRAG * deltaTime);
            // 直接更新粒子自己的世界座標，完全不參考 owner 的位置！
            p.world_x += p.vx * deltaTime;
            p.world_y += p.vy * deltaTime;
            //如果想要特效跟著角色走,可以把上面兩行改為使用下面代碼
            //p.world_x = owner->visual_x + p.vx * deltaTime;
            //p.world_y = owner->visual_y + p.vy * deltaTime;
            // 火花在生命週期中逐漸變小
            float life_ratio = p.lifetime / p.initial_lifetime;
            const float MAX_SIZE = 12.0f;
            p.size = MAX_SIZE * life_ratio;
        }
    }
    return std::nullopt;
}

// 3. 實現專屬的發射函式
void SparkParticleSystem::Emit(CCharacter* owner, int x, int y) {
    // 檢查顏色列表是否可用
    if (!m_available_colors || m_available_colors->empty()) return;

    // 獲取後方基礎角度，使用 dir 作為陣列索引
    // 這裡我們假設 dir 值在 0 到 15 的範圍內
    float base_angle_deg = s_backward_angles_deg[owner->direction];
    float backward_angle_deg = fmod(base_angle_deg + 180.0f, 360.0f);

    // 如果粒子池已滿，則停止發射
    if (m_active_count >= m_pool_size) return;

    // 尋找第一個可用的粒子
    for (auto& p : m_particle_pool) {
        if (!p.isActive) {
            p.isActive = true;
            m_active_count++;

            // --- 初始化火花屬性 ---
            p.color = m_available_colors->at(owner->HorseNo== 2 ? 8:9);

            //初始發射粒子時,一定要是角色的座標
            p.world_x = owner->visual_x + static_cast<float>(x);
            p.world_y = owner->visual_y + static_cast<float>(y);

            // 在基礎角度上增加隨機偏移量，讓效果更自然
            // 這裡的偏移範圍是 -20 度到 +19 度
            float angle_offset_deg = static_cast<float>(rand() % 40) - 20.0f;
            float final_angle_deg = backward_angle_deg + angle_offset_deg;

            // 將最終角度轉換為弧度，供 cos 和 sin 函式使用
            float angle_rad = final_angle_deg * 3.14159f / 180.0f;
            float speed = static_cast<float>(40 + (rand() % 50));

            // 根據角度和速度計算粒子的水平和垂直初速度
            p.vx = cos(angle_rad) * speed;
            p.vy = sin(angle_rad) * speed;

            // 設定粒子的生命週期
            p.lifetime = 0.3f + (rand() % 30) / 100.0f; // 0.4-0.7 秒
            p.initial_lifetime = p.lifetime;

            break; // 找到一個粒子後就跳出迴圈
        }
    }
}