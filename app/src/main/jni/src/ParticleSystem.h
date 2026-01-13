#pragma once
#include <string>
#include <vector>
#include <map>
#include <SDL.h>
#include <optional>
#include "EffectTypes.h"


class CCharacter; // 前向宣告，避免包含 Character.h

class ParticleSystem {
public:
	ParticleSystem(size_t pool_size);

	virtual ~ParticleSystem() {} 

public:
	// 這允許子類別（如 SparkParticleSystem）提供自己的 Update 版本。
	virtual std::optional<EffectCommand> Update(CCharacter* owner, float deltaTime)=0;
	virtual void Render(SDL_Surface* targetCanvas, float emitter_screen_x, float emitter_screen_y);
	virtual bool IsFinished() const;
#ifdef _TEXTURE
	bool SetSharedResources(std::map<std::string, SDL_Texture*>* _particleSurface, std::vector<std::string>* _available_colors);
#else
	bool SetSharedResources(std::map<std::string, SDL_Surface*>* _particleSurface, std::vector<std::string>* _available_colors);
#endif
	size_t GetParticleCount() const { return m_active_count; }

	// 這允許子類別存取這些成員，但對外部程式碼來說，它們和 private 一樣是隱藏的。
protected:
	std::vector<Particle> m_particle_pool;
	size_t m_pool_size;
	size_t m_active_count;

	std::map<std::string, SDL_Surface*>* m_particleSurfaces;
	std::map<std::string, SDL_Texture*>* m_particleTexture;
	std::vector<std::string>* m_available_colors;
};

class AuraParticleSystem : public ParticleSystem {
public:
	AuraParticleSystem(size_t pool_size = 150);

	// 實作基類的純虛擬函式 Update
	std::optional<EffectCommand> Update(CCharacter* owner , float deltaTime) override;
	std::string GetParticleColorByPassiveCombat(int passive);
	// 擁有自己專屬的 Emit 函式
	void Emit(CCharacter* owner, const std::vector<SDL_Point>& outlinePoints);
};


class SparkParticleSystem : public ParticleSystem {
public:
	// 建構函式：直接呼叫基礎類別的建構函式
	SparkParticleSystem(size_t pool_size=30);

	// 覆寫 Update 函式，以實現火花獨有的物理效果 (例如重力)
	std::optional<EffectCommand> Update(CCharacter* owner, float deltaTime) override;

	// 為火花新增一個專屬的發射函式
	void Emit(CCharacter* owner, int x, int y);
};