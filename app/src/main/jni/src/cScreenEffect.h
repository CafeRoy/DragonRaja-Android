#pragma once

#include <list>
#include <memory>
#include <vector>
#include "ParticleFactory.h"  // 包含工廠以使用 ParticlePtr 型別


namespace EffectConstants {
constexpr int MaxEffectSprite = 20;
constexpr int MaxSpriteType = 50;
constexpr int MaxFrameCount = 40;
}  // namespace EffectConstants

struct RenderContext {
  int mapX;
  int mapY;
  unsigned long currentTime;
};

struct BaseEffectArgs {};
struct FogArgs : public BaseEffectArgs {
  char SpriteIndex;
  POINT Location;
  DWORD StartTime;
  DWORD DelayTime;
  // (建議新增) 為 FogArgs 也加上預設建構函式
  FogArgs() : SpriteIndex(0), Location{0, 0}, StartTime(0), DelayTime(0) {}
};
struct BreathArgs : public BaseEffectArgs {
  char SpriteIndex;
  POINT StartLocation;
  int Direction;
  int Power;
  DWORD StartTime;
  // =======================================================================
  // REFACTOR: (新增) 增加一個預設建構函式，將所有成員初始化為 0 或 null。
  // 這樣 `BreathArgs args = {};` 語法才能被成功呼叫。
  BreathArgs()
      : SpriteIndex(0),
        StartLocation{0, 0},
        Direction(0),
        Power(0),
        StartTime(0) {}
  // REFACTOR: 增加一個建構函式來處理初始化
  BreathArgs(char spriteIndex, POINT startLocation, int direction, int power,
             DWORD startTime)
      : SpriteIndex(spriteIndex),
        StartLocation(startLocation),
        Direction(direction),
        Power(power),
        StartTime(startTime) {}
};

class cParticle {
 public:
  virtual ~cParticle() = default;
  virtual cParticle* Insert() = 0;  // 此函式返回原始指標，需由呼叫者小心處理
  virtual int Draw(const RenderContext& context) = 0;
  virtual bool IsFinished(DWORD currentTime) const = 0;
};

using tParticleList = std::list<ParticleFactory::ParticlePtr>;

class cParticleManager {
 private:
  tParticleList m_particleList;
  cParticle* m_pContinueParticle;  // 注意：此指標不由 Manager 管理其生命週期
  int m_iContinueparticleCount;
  bool m_bDisplay;

 public:
  cParticleManager();
  ~cParticleManager();
  void DeleteAllParticles();
  void Insert(ParticleFactory::ParticlePtr particle);
  void Draw(const RenderContext& context);
  void SetContinueParticle(cParticle* particle, int count);
  void CheckContinueParticle();
  void SetDisplay(bool bDisplay) { m_bDisplay = bDisplay; }
};

class cFog : public cParticle {
 private:
  char m_cSpriteIndex;
  POINT m_Location;
  unsigned char m_cFrameCount;
  DWORD m_dwStartTime;
  DWORD m_dwDelayTime;

 public:
  explicit cFog(const FogArgs& args);
  cParticle* Insert() override;
  int Draw(const RenderContext& context) override;
  bool IsFinished(DWORD currentTime) const override;
};

class cBreath : public cParticle {
 private:
  char m_cSpriteIndex;
  POINT m_Start;
  char m_cDirection;
  unsigned char m_ucPower;
  unsigned char m_cFrameCount;
  unsigned long m_uiStartTime;
  unsigned long m_lastTimeInMs;
  float m_fRemainX;
  float m_fRemainY;
  float m_fPowerRemain;

 public:
  explicit cBreath(const BreathArgs& args);
  cParticle* Insert() override;
  int Draw(const RenderContext& context) override;
  bool IsFinished(DWORD currentTime) const override;

};

struct EffectFrameList {
  bool IsRepeat;
  unsigned char MaxFrame;
  unsigned char TotalFrame;
  std::vector<unsigned char> FrameIndices;
};

class cEffectSpriteManager {
 private:
  std::unique_ptr<char[]> m_pSpriteData[EffectConstants::MaxSpriteType];
  Spr m_Sprite[EffectConstants::MaxSpriteType][EffectConstants::MaxFrameCount];
  EffectFrameList m_pEffectList[EffectConstants::MaxSpriteType];

 public:
  cEffectSpriteManager();
  ~cEffectSpriteManager() = default;
  bool LoadEFFile(int index);
  bool LoadSprite(int index, int frameCount);
  void LoadAllSprite();
  Spr* GetSpriteData(int index, unsigned char& frame);
  void CheckAndLoadSpriteData(int index);
};

extern cParticleManager g_ParticleManager;
extern cEffectSpriteManager g_EffectSpriteManager;
extern LPCHARACTER FindHimBySprNo(int SprNo);
extern void DrawParticleEffect(const RenderContext& context);  // LTS DRAGON