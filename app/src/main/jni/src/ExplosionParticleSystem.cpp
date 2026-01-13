#include "StdAfx.h"
#include "ExplosionParticleSystem.h"
#include <cmath>
#include "SDLRenderer.h"

ExplosionParticleSystem::ExplosionParticleSystem(size_t pool_size)
    : ParticleSystem(pool_size)
{
}

void ExplosionParticleSystem::Init(float world_x, float world_y, CCharacter* owner) {
    // 爆炸特效一次性產生所有粒子
    Emit(world_x, world_y, owner);
}

// 爆炸的發射邏輯：一次性、向四周爆發
void ExplosionParticleSystem::Emit(float world_x, float world_y, CCharacter* owner) {
    if (!m_available_colors || m_available_colors->empty()) return;

    // 決定這次爆炸要產生多少個粒子
    int particle_count = 30 + (rand() % 20); // 產生 30 到 49 個火花

    for (int i = 0; i < particle_count; ++i) {
        if (m_active_count >= m_pool_size) break; // 如果池滿了就提前結束

        // 尋找一個閒置粒子
        for (auto& p : m_particle_pool) {
            if (!p.isActive) {
                p.isActive = true;
                m_active_count++;

                // --- 初始化爆炸火花的屬性 ---
                p.color = m_available_colors->at(rand() % m_available_colors->size());

                // 所有火花都從同一個中心點誕生
                p.world_x = world_x;
                p.world_y = world_y;

                // 初始速度：向 360 度隨機方向發射
                float angle_rad = (rand() % 360) * 3.14159f / 180.0f;
                float speed = static_cast<float>(20 + (rand() % 40)); // 爆炸速度可以快一些
                p.vx = cos(angle_rad) * speed;
                p.vy = sin(angle_rad) * speed;

                // 生命週期
                p.lifetime = 2.8f + (rand() % 50) / 100.0f; // 存活 0.8 到 1.3 秒
                p.initial_lifetime = p.lifetime;

                p.size = 4.0f; // 初始大小
                p.alpha_base = 90.0f;//不透明
                break; // 處理完一個粒子，跳出內層 for 迴圈
            }
        }
    }
}

// 覆寫 Update，實現火花的物理行為（與走路火花類似，帶有重力）
std::optional<EffectCommand> ExplosionParticleSystem::Update(CCharacter* owner, float deltaTime) {
    const float GRAVITY = 28.0f;
    const float DRAG = 0.1f;

    for (auto& p : m_particle_pool) {
        if (!p.isActive) continue;

        p.lifetime -= deltaTime;
        if (p.lifetime <= 0) {
            p.isActive = false;
            m_active_count--;
        }
        else {
            // --- 物理模擬 ---
            p.vy += GRAVITY * deltaTime; // 施加重力
            p.vx *= (1.0f - DRAG * deltaTime);
            p.vy *= (1.0f - DRAG * deltaTime);

            p.world_x += p.vx * deltaTime;
            p.world_y += p.vy * deltaTime;

            // 視覺效果：在生命週期中逐漸變小
            p.size = 8.0f * (p.lifetime / p.initial_lifetime);
            // ==========================================================
            // 【核心邏輯】根據生命週期計算當前幀的顏色
            // ==========================================================
            float life_ratio = p.lifetime / p.initial_lifetime; // 比例從 1.0 降到 0.0

            // 顏色插值：從「亮黃白」(255, 255, 220) 過渡到「暗紅色」(200, 50, 0)
            p.r = static_cast<Uint8>(200 + 55 * life_ratio);
            p.g = static_cast<Uint8>(50 + 205 * life_ratio);
            p.b = static_cast<Uint8>(0 + 220 * life_ratio);

            // 大小變化邏輯
            //p.size = 12.0f * sin(life_ratio * 3.14159f); // 呼吸效果
        }
    }

    // 【重要】爆炸是特效的終點，它不會再觸發任何後續指令
    return std::nullopt;
}

// 覆寫 IsFinished
bool ExplosionParticleSystem::IsFinished() const {
    // 當所有爆炸火花都消失後，這個特效系統就完成了使命
    return m_active_count == 0;
}

void ExplosionParticleSystem::Render(SDL_Surface* targetCanvas, float emitter_screen_x,
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
        closest_alpha = max((float)closest_alpha, p.alpha_base);
        

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
        //如果想要彩色的爆炸就把這句刪掉或 // 掉
        //SDL_SetSurfaceColorMod(current_surface, p.r, p.g, p.b);
        // 將粒子紋理繪製到目標畫布上
        SDL_BlitScaled(current_surface, NULL, targetCanvas, &destRect);
#endif
    }
}