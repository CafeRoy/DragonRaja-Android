#include "StdAfx.h"
#include <algorithm> // for std::remove
#include "ParticleEffectManager.h"
#include "RocketParticleSystem.h"   
#include "ExplosionParticleSystem.h"



ParticleEffectManager g_particle_manager;

// 【修正1】加上建構函式實作
ParticleEffectManager::ParticleEffectManager() {
    // 可以在這裡初始化，或保持為空，由外部邏輯呼叫 Load
}

ParticleEffectManager::~ParticleEffectManager() {
    for (auto& pair : m_particleSurfaces) {
        if (pair.second) SDL_FreeSurface(pair.second);
    }
    m_particleSurfaces.clear();
#ifdef _TEXTURE
    for (auto& pair : m_particleTexture) {
        if (pair.second) SDL_DestroyTexture(pair.second);
    }
    m_particleTexture.clear();
#endif
}
void ParticleEffectManager::AddAura(CCharacter* ch) {
    if (!ch) return;
    auto& components = m_character_effects[ch];
    if (components.find(EffectType::Aura) == components.end()) {
        // 1. 創建 AuraParticleSystem 實例，並傳入其構造函數需要的參數
        auto new_system = std::make_unique<AuraParticleSystem>(MAX_PARTICLE_FOR_AURA); 
#ifdef _TEXTURE
        new_system->SetSharedResources(&m_particleTexture, &m_available_colors);
#else
        // 2. 【關鍵】呼叫正確的函式 SetSharedResources，將管理器的資源「借」給它
        new_system->SetSharedResources(&m_particleSurfaces, &m_available_colors);
#endif
        // 3. 使用 emplace 將 new_system 插入到 map 中
        // std::move 是必要的，因為 unique_ptr 不能被複製，只能被移動
        components.emplace(EffectType::Aura, std::move(new_system));
    }
}

void ParticleEffectManager::AddSparks(CCharacter* ch) {
    if (!ch) return;
    auto& components = m_character_effects[ch];
    if (components.find(EffectType::Spark) == components.end()) {
        // 1. 創建 AuraParticleSystem 實例，並傳入其構造函數需要的參數
        auto new_system = std::make_unique<SparkParticleSystem>(MAX_PARTICLE_FOR_SPARK); 
#ifdef _TEXTURE
        new_system->SetSharedResources(&m_particleTexture, &m_available_colors);
#else
        // 2. 【關鍵】呼叫正確的函式 SetSharedResources，將管理器的資源「借」給它
        new_system->SetSharedResources(&m_particleSurfaces, &m_available_colors);
#endif
        // 3. 【推薦作法】使用 emplace 將 new_system 插入到 map 中
        // std::move 是必要的，因為 unique_ptr 不能被複製，只能被移動
        components.emplace(EffectType::Spark, std::move(new_system));
    }
}

void ParticleEffectManager::Remove(CCharacter* ch) {
    m_character_effects.erase(ch);
}

AuraParticleSystem* ParticleEffectManager::GetAura(CCharacter* ch) {
    auto it_char = m_character_effects.find(ch);
    if (it_char != m_character_effects.end()) {
        auto it_effect = it_char->second.find(EffectType::Aura);
        if (it_effect != it_char->second.end()) {
            return dynamic_cast<AuraParticleSystem*>(it_effect->second.get());
        }
    }
    return nullptr;
}

SparkParticleSystem* ParticleEffectManager::GetSparks(CCharacter* ch) {
    auto it_char = m_character_effects.find(ch);
    if (it_char != m_character_effects.end()) {
        auto it_effect = it_char->second.find(EffectType::Spark);
        if (it_effect != it_char->second.end()) {
            return dynamic_cast<SparkParticleSystem*>(it_effect->second.get());
        }
    }
    return nullptr;
}

void ParticleEffectManager::UpdateAll(float deltaTime) {

    // 創建一個佇列，用來收集所有特效回傳的指令
    std::vector<EffectCommand> command_queue;

    // ----- 階段一：更新所有常駐特效 -----
    for (auto& char_pair : m_character_effects) {
        CHARACTER* owner = char_pair.first;
        auto& components = char_pair.second;
        for (auto& effect_pair : components) {
            auto command = effect_pair.second->Update(owner, deltaTime);
            if (command.has_value()) {
                command_queue.push_back(command.value());
            }
        }
    }

    // ----- 階段二：更新所有一次性特效 -----
    for (auto& effect_ptr : m_one_shot_effects) {
        // 一次性特效沒有主人，所以 owner 傳遞 nullptr
        auto command = effect_ptr->Update(nullptr, deltaTime);
        if (command.has_value()) {
            command_queue.push_back(command.value());
        }
    }

    // ----- 階段三：清理已完成的一次性特效 -----
    // 使用 C++20 的 std::erase_if，如果您的編譯器支援的話會更簡潔
    // list::remove_if 是 C++11 的標準作法
    m_one_shot_effects.remove_if([](const std::unique_ptr<ParticleSystem>& effect) {
        return effect->IsFinished();
        });

    // ----- 階段四：執行收集到的所有指令 -----
    for (const auto& cmd : command_queue) {
        if (cmd.type == CommandType::SpawnEffect) {
            // 根據指令的內容，創建對應的新特效
            switch (cmd.effect_to_spawn) {
            case EffectType::Rocket:
                CreateOneShotEffect<RocketParticleSystem>(cmd.world_x, cmd.world_y, cmd.owner);
                break;
            case EffectType::Explosion:
                CreateOneShotEffect<ExplosionParticleSystem>(cmd.world_x, cmd.world_y, cmd.owner);
                break;
                // ... 未來可以擴充 ...
            }
        }
    }
}

void ParticleEffectManager::RenderAll(SDL_Surface* targetCanvas, CCharacter* chr, int map_x, int map_y) {
    for (auto& char_pair : m_character_effects) {
        CHARACTER* ch = char_pair.first;
        if (ch == chr) {
            for (auto& effect_pair : char_pair.second) {
                effect_pair.second->Render(targetCanvas, static_cast<float>(map_x), static_cast<float>(map_y));
            }
        }
        
    }

    // 【新增】渲染所有一次性特效
    for (auto& effect_ptr : m_one_shot_effects) {
        // 一次性特效是獨立的，它們直接使用自己的世界座標進行渲染
        // 所以我們傳遞 0, 0 作為 emitter_screen_x/y (因為它們沒有 emitter)
        // Render 函式內部會直接用粒子的 world_x/y - map_x/y
        effect_ptr->Render(targetCanvas, static_cast<float>(map_x), static_cast<float>(map_y));
    }
}

bool ParticleEffectManager::LoadBaseParticleTexture(std::string path)
{
    // 1. 清理舊資源，防止重複載入時記憶體洩漏
    for (auto const& [key, surface] : m_particleSurfaces) {
        if (surface) SDL_FreeSurface(surface);
    }
    m_particleSurfaces.clear();
    m_available_colors.clear();  // <-- 同時清空顏色列表

    // 2. 載入基礎圖片
    SDL_Surface* baseSurface = SDL_LoadBMP(path.c_str());;
    if (!baseSurface) {
        SDL_Log("cannot load img : %s", SDL_GetError());
        return false;
    }

    // 3. 設定基礎圖片的混合模式和透明色
    SDL_SetSurfaceBlendMode(baseSurface, SDL_BLENDMODE_ADD);  // 使用加色混合，適合光暈效果
    Uint32 blackColorKey = SDL_MapRGB(baseSurface->format, 0, 0, 0);
    SDL_SetColorKey(baseSurface, SDL_TRUE, blackColorKey);

    // 4. 定義要預先生成的顏色
    struct Color {
        Uint8 r, g, b;
    };
    std::map<std::string, Color> colors = {
        {"purple", {218, 112, 214}},  // 淡紫色 (Orchid)
        {"green", {50, 255, 200}},    // 亮海綠色 (Aquamarine)
        {"blue", {100, 180, 255}},     // 蔚藍色 (Cornflower Blue)
        {"orange", {255, 140, 0} },   // 暗橙色 (DarkOrange)
        {"pink", {255, 105, 180}},     // 熱粉色 (HotPink)
        {"red", {220, 40, 40}},        // 深紅色 (Crimson)
        {"gold", {255, 215, 0}},       // 金色 (Gold)
        {"cyan", {0, 255, 255}},       // 藍綠色 (Cyan)
        {"teal", {0, 128, 128}},       // 青色 (Teal)
        {"brown", {139, 69, 19}},      // 鞍褐色 (SaddleBrown)
        {"gray", {169, 169, 169}}      // 暗灰色 (DarkGray)
    };

    // 定義透明度等級 (0%, 10%, 20%... 100%)
    std::vector<int> alphaSteps;
    for (int i = 0; i <= 90; i += 10) {
        alphaSteps.push_back(i);
    }

    // 5. 遍歷所有顏色和透明度組合，預先生成所有可能的 Surface 並存入 map
    for (const auto& colorEntry : colors) {
        // 將顏色名稱 (如 "purple") 存入可用列表中，供 Emit 函式隨機選取
        m_available_colors.push_back(colorEntry.first);

        for (int alpha : alphaSteps) {
            // 複製基礎紋理，為其套上新的顏色和透明度
            SDL_Surface* newSurface =
                SDL_ConvertSurface(baseSurface, baseSurface->format, 0);
            SDL_SetSurfaceColorMod(newSurface, colorEntry.second.r,
                colorEntry.second.g, colorEntry.second.b);
            SDL_SetSurfaceAlphaMod(newSurface, (Uint8)(alpha * 255 / 100));

            // 用 "顏色_alpha_透明度" (例如 "purple_alpha_50") 作為 Key 存入 map
            std::string key = colorEntry.first + "_alpha_" + std::to_string(alpha);
            m_particleSurfaces[key] = newSurface;
#ifdef _TEXTURE
            SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer, newSurface);
            if (newTexture) {
                m_particleTexture[key] = newTexture;
                SDL_FreeSurface(newSurface);
            }
            else {
                SDL_Log("ERROR....colorEntry[%s]...newTexture si null....", colorEntry.first.c_str());
            }
#endif
        }
    }

    // 6. 釋放已不需要的基礎圖片
    SDL_FreeSurface(baseSurface);


    return true;
}

template<typename T>
void ParticleEffectManager::CreateOneShotEffect(float world_x, float world_y, CCharacter* owner) {
    // 1. 創建一個新的特效實例
    auto new_effect = std::make_unique<T>(); // 假設 T 有預設建構函式

#ifdef _TEXTURE
    new_effect->SetSharedResources(&m_particleTexture, &m_available_colors);
#else
    // 2. 將共享的紋理資源借給它
    new_effect->SetSharedResources(&m_particleSurfaces, &m_available_colors);
#endif
    // 3. 【關鍵】如果這個特效需要一個初始狀態（例如發射點），我們需要一個 Init 函式
    //    我們假設所有一次性特效都有一個 Init 函式
    if constexpr (std::is_invocable_v<decltype(&T::Init), T, float, float, CCharacter*>) {
        new_effect->Init(world_x, world_y, owner);
    }

    // 4. 將這個新的特效加入到一次性特效列表中
    m_one_shot_effects.push_back(std::move(new_effect));
}

// 【新增】為了讓範本函式能被其他檔案呼叫，我們需要明確地實例化它
// 未來每新增一種一次性特效，都需要在這裡加一行
template void ParticleEffectManager::CreateOneShotEffect<RocketParticleSystem>(float, float, CCharacter*);
template void ParticleEffectManager::CreateOneShotEffect<ExplosionParticleSystem>(float, float, CCharacter*);

void ParticleEffectManager::LaunchFirework(CCharacter* g_pMainCharacter, int mapx, int mapy, int shot)
{
    // 安全性檢查
    if (!g_pMainCharacter) {
        SDL_Log("Cannot launch firework, main character not found.");
        return;
    }

    // 2. 【關鍵】呼叫管理器，創建一個「火箭」一次性特效
    //    這就是我們需要做的全部事情！
    //    我們不需要關心爆炸，火箭自己會在生命結束時透過「指令」系統請求爆炸。
    for (int i = 0; i < shot; i++) {
        // 1. 決定煙火的發射點（世界座標）
    //    這裡我們選擇在主角腳下的世界座標發射
        float start_x = mapx;
        float ranx = rand() % SCREEN_WIDTH;
        float start_y = mapy;
        float rany = rand() % SCREEN_HEIGHT; // 假設角色高度為 48，讓煙火從腳底出來
        rany = (float)max(rany, MAININTERFACE);
        start_x += ranx; start_y += rany;
        //SDL_Log("Launching firework from world position (%f, %f)", start_x, start_y);
        CreateOneShotEffect<RocketParticleSystem>(start_x, start_y, g_pMainCharacter);
    }
}