#pragma once

#include "ParticleSystem.h" // 引入我們強大的基礎類別

const float MAX_SIZE = 6.0f;

class RocketParticleSystem : public ParticleSystem {
public:
    // 建構函式，設定一個較小的粒子池，因為火箭通常只有一個主體
    RocketParticleSystem(size_t pool_size = 5);

    // 覆寫 (override) 基類的純虛擬函式 Update
    std::optional<EffectCommand> Update(CCharacter* owner, float deltaTime) override;

    // 覆寫 IsFinished，告訴管理器何時可以清理我
    bool IsFinished() const override;

    // 【新增】這是我們在管理器中約定好的初始化函式
    // 它將負責觸發火箭的發射
    void Init(float world_x, float world_y, CCharacter* owner);

private:
    // 一個私有的內部函式，負責真正的粒子創建邏輯
    void Emit(float world_x, float world_y, CCharacter* owner);
};