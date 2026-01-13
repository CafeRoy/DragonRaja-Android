#include "stdafx.h"  // 如果您的專案使用預編譯標頭檔
#include "ParticleFactory.h"
#include "cScreenEffect.h"
// 實例化全域工廠物件
ParticleFactory g_ParticleFactory;

ParticleFactory::ParticleFactory()
    : m_fogPool(50),    // 預設池大小：最多同時存在 200 個霧效
      m_breathPool(50)  // 預設池大小：最多同時存在 500 個吐息
{}

ParticleFactory::ParticlePtr ParticleFactory::createFog(const FogArgs& args) {
  // 從池中獲取一個 cFog 物件
  cFog* fog = m_fogPool.acquire(args);
  if (!fog) {
    // 池已滿，無法創建新的粒子
    return nullptr;
  }

  // 創建一個自訂刪除器 (lambda)，它知道如何將 cFog 物件歸還給正確的池
  auto deleter = [this](cParticle* p) {
    // 我們知道這個 p 實際上是 cFog*，所以可以安全地轉換
    m_fogPool.release(static_cast<cFog*>(p));
  };

  // 返回一個包含物件指標和自訂刪除器的 unique_ptr
  return ParticlePtr(fog, deleter);
}

ParticleFactory::ParticlePtr ParticleFactory::createBreath(
    const BreathArgs& args) {
  // 從池中獲取一個 cBreath 物件
  cBreath* breath = m_breathPool.acquire(args);
  if (!breath) {
    // 池已滿
    return nullptr;
  }

  // 創建對應的自訂刪除器
  auto deleter = [this](cParticle* p) {
    m_breathPool.release(static_cast<cBreath*>(p));
  };

  return ParticlePtr(breath, deleter);
}