#include "StdAfx.h"
#include <cmath> // for cos, sin
#include "RocketParticleSystem.h"
RocketParticleSystem::RocketParticleSystem(size_t pool_size)
    : ParticleSystem(pool_size)
{
}

// 公開的初始化介面，它只做一件事：呼叫內部的發射邏輯
void RocketParticleSystem::Init(float world_x, float world_y, CCharacter* owner) {
    Emit(world_x, world_y, owner);
}

// 私有的發射函式，負責設定粒子的初始狀態
void RocketParticleSystem::Emit(float world_x, float world_y, CCharacter* owner) {
    if (m_active_count >= m_pool_size) return; // 池已滿

    for (auto& p : m_particle_pool) {
        if (!p.isActive) {
            p.isActive = true;
            m_active_count++;

            // --- 初始化火箭粒子的屬性 ---
            if (m_available_colors && !m_available_colors->empty()) {
                p.color = m_available_colors->at(rand() % m_available_colors->size());
            }

            p.world_x = world_x;
            p.world_y = world_y;

            // 初始速度：垂直向上，帶有一點點隨機的水平偏移
            p.vx = (rand() % 20) - 10.0f; // 水平速度在 [-10, 9] 之間
            p.vy = -50.0f;               // 垂直速度，負數代表向上

            // 生命週期：決定了火箭能飛多高
            p.lifetime = 1.0f + (rand() % 80) / 100.0f; // 存活 1.0 到 1.5 秒
            p.initial_lifetime = p.lifetime;

            p.size = MAX_SIZE; // 火箭的大小
            p.alpha_base = 90.0f;//不透明
            return; // 成功發射了一個粒子，結束函式
        }
    }
}

// 覆寫 Update，實現火箭的物理行為
std::optional<EffectCommand> RocketParticleSystem::Update(CCharacter* owner, float deltaTime) {
    const float THRUST = 500.0f; // 火箭自身的向上推力
    const float GRAVITY = 120.0f; // 世界的重力
    const float DRAG = 0.5f;    // 空氣阻力

    for (auto& p : m_particle_pool) {
        if (!p.isActive) continue;

        p.lifetime -= deltaTime;
        if (p.lifetime <= 0) {
            p.isActive = false;
            m_active_count--;

            // 【核心邏輯】
            // 火箭壽命結束！創建一個「生成爆炸」的指令並回傳給管理器
            EffectCommand cmd = {};
            cmd.type = CommandType::SpawnEffect;
            cmd.effect_to_spawn = EffectType::Explosion; // 指定要生成的特效類型
            cmd.world_x = p.world_x;                     // 在火箭死亡的位置生成
            cmd.world_y = p.world_y;
            cmd.owner = owner;                           // 爆炸效果的主人與火箭相同

            return cmd; // 將指令回傳
        }
        else {
            // --- 物理模擬 ---
            // 火箭的垂直速度 = 自身推力 - 重力
            p.vy -= THRUST * deltaTime;
            p.vy += GRAVITY * deltaTime;

            // 施加空氣阻力
            p.vx *= (1.0f - DRAG * deltaTime);
            p.vy *= (1.0f - DRAG * deltaTime);

            // 更新世界座標
            p.world_x += p.vx * deltaTime;
            p.world_y += p.vy * deltaTime;

            //float life_ratio = p.lifetime / p.initial_lifetime;
            //p.size = MAX_SIZE * life_ratio;
        }
    }

    // 如果沒有粒子死亡，就不回傳任何指令
    return std::nullopt;
}

// 覆寫 IsFinished，告訴管理器我什麼時候可以被安全刪除
bool RocketParticleSystem::IsFinished() const {
    // 當我管理的所有粒子（通常只有一個火箭）都非活躍時，
    // 我這個系統的使命就結束了，可以被清除了。
    return m_active_count == 0;
}