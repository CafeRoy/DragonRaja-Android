#pragma once
#include <vector>
#include <SDL.h>
#include <map>
#include <list>
#include "EffectTypes.h"

const int MAX_PARTICLE_FOR_AURA = 200;
const int MAX_PARTICLE_FOR_SPARK = 30;
const float MAININTERFACE = 520.0f;
class ParticleSystem;
class AuraParticleSystem;
class SparkParticleSystem;
class RocketParticleSystem;
class ExplosionParticleSystem;

class CCharacter;

using EffectComponentMap = std::map<EffectType, std::unique_ptr<ParticleSystem>>;


class ParticleEffectManager {
public:
    ParticleEffectManager(); 
    ~ParticleEffectManager();

    bool LoadBaseParticleTexture(std::string path);
    //void setRenderer(SDL_Renderer* render) { renderer = render; }

    void LaunchFirework(CCharacter* g_pMainCharacter, int mapx, int mapy, int shot);

    void AddAura(CCharacter* ch);
    void AddSparks(CCharacter* ch);
    void Remove(CCharacter* ch);

    AuraParticleSystem* GetAura(CCharacter* ch);
    SparkParticleSystem* GetSparks(CCharacter* ch);


    template<typename T>
    void CreateOneShotEffect(float world_x, float world_y, CCharacter* owner = nullptr);


    void UpdateAll(float deltaTime);
    void RenderAll(SDL_Surface* targetCanvas,CCharacter* chr, int map_x, int map_y);
	
private:

    // 使用 std::list，因為我們需要頻繁地從中間移除元素，list 的效率最高
    std::list<std::unique_ptr<ParticleSystem>> m_one_shot_effects;
    std::map<CCharacter*, EffectComponentMap> m_character_effects;
    std::map<std::string, SDL_Surface*> m_particleSurfaces;
    std::vector<std::string> m_available_colors;

#ifdef _TEXTURE
    SDL_Renderer* renderer;
    std::map<std::string, SDL_Texture*> m_particleTexture;
#endif
};

extern ParticleEffectManager g_particle_manager;

