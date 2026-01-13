#pragma once

// ===================================================================================
//
// 粒子工廠 (Particle Factory)
// 職責：
// 1. 擁有並管理所有粒子型別的 ObjectPool。
// 2. 負責從池中創建 (acquire) 和銷毀 (release) 粒子。
// 3. 產出帶有自訂刪除器 (Custom Deleter) 的智慧指標，確保粒子能被正確歸還給池。
//
// ===================================================================================

#include <functional>
#include <memory>
#include "ObjectPool.h"
// 前向宣告，避免與 cScreenEffect.h 循環依賴
class cParticle;
class cFog;
class cBreath;
struct FogArgs;
struct BreathArgs;

class ParticleFactory {
 private:
  ObjectPool<cFog> m_fogPool;
  ObjectPool<cBreath> m_breathPool;

 public:
  // 使用 std::function
  // 作為通用的刪除器型別，這樣可以將不同型別的粒子指標存儲在同一個容器中
  using ParticlePtr =
      std::unique_ptr<cParticle, std::function<void(cParticle*)>>;

  // 建構函式，初始化物件池的大小
  ParticleFactory();

  // 創建霧效粒子
  ParticlePtr createFog(const FogArgs& args);

  // 創建吐息粒子
  ParticlePtr createBreath(const BreathArgs& args);
};

// 讓工廠成為一個全域變數，以便在現有架構中輕鬆訪問
extern ParticleFactory g_ParticleFactory;