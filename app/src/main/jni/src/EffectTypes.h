#pragma once

class CCharacter;

enum class EffectType {
    Aura,
    Spark,
    Rocket,
    Explosion
};

// ==================================================================
// 指令類型枚舉
// ==================================================================
// 目前我們只需要一種指令：「生成特效」。
// 未來可以擴充，例如 CommandType::PlaySound 等。
enum class CommandType {
    SpawnEffect
};

// ==================================================================
// 【新增】指令本身的結構
// ==================================================================
// 這就像一張標準化的「訂單」，特效系統填好後交給管理器。
struct EffectCommand {
    // 這條指令是什麼類型？
    CommandType type;

    // --- 如果是 SpawnEffect 指令，需要填寫以下資訊 ---

    // 要生成哪一種類型的特效？
    EffectType effect_to_spawn;

    // 要在哪個世界座標生成？
    float world_x;
    float world_y;

    // 這個新特效是否需要一個主人？(例如，爆炸後產生的煙霧可能需要跟隨某個目標)
    // 對於煙火來說，爆炸是沒有主人的，所以這裡會是 nullptr。
    CCharacter* owner;
};


struct Particle {
	bool isActive;
	std::string color;
	float world_x, world_y;
	float relative_x, relative_y;
	float vx, vy;
	float lifetime;
	float initial_lifetime;
	float size;

    //for firework
    Uint8 r, g, b;
    float alpha_base;
};