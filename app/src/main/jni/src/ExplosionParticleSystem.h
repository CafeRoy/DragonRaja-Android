#pragma once
#include "ParticleSystem.h"


class ExplosionParticleSystem : public ParticleSystem {
public:
    // 建構函式，爆炸需要較多的粒子，所以預設池大小可以大一些
    ExplosionParticleSystem(size_t pool_size = 50);

    virtual void Render(SDL_Surface* targetCanvas, float emitter_screen_x, float emitter_screen_y) override;

    // 覆寫 Update，實現爆炸火花的物理效果（重力、阻力）
    std::optional<EffectCommand> Update(CCharacter* owner, float deltaTime);

    // 覆寫 IsFinished，告訴管理器何時可以清理
    bool IsFinished() const override;

    // 同樣擁有一個 Init 函式，由管理器在處理指令時呼叫
    void Init(float world_x, float world_y, CCharacter* owner);

private:
    // 內部發射函式
    void Emit(float world_x, float world_y, CCharacter* owner);
};