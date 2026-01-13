
#include "stdafx.h"
#include <iostream>   // 用於錯誤輸出
#include <fstream>    // C++ 檔案流
#include <filesystem> // C++ 檔案系統
#include <cstdio>
#include <sstream>  // *** 新增 ***: 為了使用 stringstream
#include <iomanip>  // *** 新增 ***: 為了使用 setw 和 setfill
#include "Hong_Sprite.h"
#include "SDLRenderer.h"
#include "cScreenEffect.h"
// 實例化全域管理器
cParticleManager g_ParticleManager;
cEffectSpriteManager g_EffectSpriteManager;

// --- 全域輔助函式 ---
/*
 * 如何呼叫更新後的函式：
 *
 * void YourGameLoop()
 * {
 * // 1. 建立 RenderContext 並填入當前的遊戲狀態
 * RenderContext context = { Mapx, Mapy, g_curr_time };
 *
 * // 2. 呼叫管理器函式
 * DrawParticleEffect(context);
 * CheckParticleEffect();
 * }
 *
 * 如何創建粒子:
 *
 * void CreateSomeFog()
 * {
 * FogArgs args = { ... };
 * if (auto fogParticle = g_ParticleFactory.createFog(args))
 * {
 * g_ParticleManager.Insert(std::move(fogParticle));
 * }
 * }
 */


void DrawParticleEffect(const RenderContext& context) {
  g_ParticleManager.Draw(context);
}

void CheckParticleEffect() { g_ParticleManager.CheckContinueParticle(); }

// --- cFog 實作 ---
cFog::cFog(const FogArgs& args)
    : m_cSpriteIndex(args.SpriteIndex),
      m_Location(args.Location),
      m_cFrameCount(0),
      m_dwStartTime(args.StartTime),
      m_dwDelayTime(args.DelayTime) {}

cParticle* cFog::Insert() { return nullptr; }

int cFog::Draw(const RenderContext& context) {
  if (context.currentTime >= this->m_dwStartTime) {

    Spr* sprite = g_EffectSpriteManager.GetSpriteData(this->m_cSpriteIndex,
                                                      this->m_cFrameCount);
    if (sprite) {
      PutCompressedImageFX(this->m_Location.x - context.mapX,
                           this->m_Location.y - context.mapY, sprite, 15, 2);
    }
  }
  return 1;
}

bool cFog::IsFinished(DWORD currentTime) const {
  return currentTime >= this->m_dwDelayTime;
}

// --- cBreath 實作 ---
static const POINTS BREATH_PROGRESS[16] = {
    {0, 2},  {-1, 2}, {-2, 2}, {-2, 1}, {-2, 0}, {-2, -1}, {-2, -2}, {-1, -2},
    {0, -2}, {1, -2}, {2, -2}, {2, -1}, {2, 0},  {2, 1},   {2, 2},   {1, 2}};

cBreath::cBreath(const BreathArgs& args)
    : m_cSpriteIndex(args.SpriteIndex),
      m_Start(args.StartLocation),
      m_cDirection(args.Direction),
      m_ucPower(args.Power),
      m_cFrameCount(0),
      m_uiStartTime(args.StartTime),
      m_lastTimeInMs(0){}

cParticle* cBreath::Insert() {
  LPCHARACTER dragonNpc = FindHimBySprNo(50);
  if (!dragonNpc || dragonNpc->nCurrentAction != 4) return nullptr;

  static const int NpcDirection[26] = {3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 3,
                                       3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2};
  int frame = dragonNpc->nCurrentFrame.GetDecrypted();
  if (frame < 0 || frame >= 26) return nullptr;

  // 使用標準的建構函式語法，而不是大括號列表
  BreathArgs newArgs(this->m_cSpriteIndex, this->m_Start, NpcDirection[frame],
                     this->m_ucPower, this->m_uiStartTime);

  // 注意：這裡直接 new 了一個物件，沒有使用工廠。
  // 這意味著這個由 Insert 產生的新粒子，它的記憶體不是由物件池管理的。
  // 這保留了原始邏輯，但如果 Insert 被頻繁呼叫，這裡可能會成為新的效能瓶頸。
  return new cBreath(newArgs);
}

int cBreath::Draw(const RenderContext& context) {
    unsigned long currentTimeInMs = SDL_GetTicks();

    // 檢查是否為第一次繪製，並初始化所有成員變數
    // 或是更推薦在 cBreath 的建構函式中初始化
    if (this->m_lastTimeInMs == 0) {
        this->m_lastTimeInMs = currentTimeInMs;
        this->m_fRemainX = 0.0f;
        this->m_fRemainY = 0.0f;
        this->m_fPowerRemain = 0.0f;
    }

    float deltaTime = (currentTimeInMs - this->m_lastTimeInMs) / 1000.0f;
    this->m_lastTimeInMs = currentTimeInMs;

    if (this->m_ucPower > 0) {
        Spr* sprite = g_EffectSpriteManager.GetSpriteData(this->m_cSpriteIndex,
            this->m_cFrameCount);
        if (sprite) {
            // 計算基於時間的移動量，並累積到成員變數
            float speedX = BREATH_PROGRESS[this->m_cDirection].x * 17.0f;
            float speedY = BREATH_PROGRESS[this->m_cDirection].y * 17.0f;
            this->m_fRemainX += speedX * deltaTime;
            this->m_fRemainY += speedY * deltaTime;

            // 將累積的移動量轉換為整數，並更新粒子位置
            LONG moveX = static_cast<LONG>(this->m_fRemainX);
            LONG moveY = static_cast<LONG>(this->m_fRemainY);
            this->m_Start.x += moveX;
            this->m_Start.y += moveY;

            // 從累積值中減去已經移動的整數
            this->m_fRemainX -= moveX;
            this->m_fRemainY -= moveY;

            PutCompressedImageFX(this->m_Start.x - context.mapX,
                this->m_Start.y - context.mapY, sprite, 12, 2);

            // 對生命週期做同樣的處理
            float decayRate = 17.0f;
            this->m_fPowerRemain += decayRate * deltaTime;
            int powerDecay = static_cast<int>(this->m_fPowerRemain);

            if (this->m_ucPower >= powerDecay) {
                this->m_ucPower -= powerDecay;
                this->m_fPowerRemain -= powerDecay;
            }
            else {
                this->m_ucPower = 0;
                this->m_fPowerRemain = 0.0f;
            }
        }
    }
    return 1;
}

bool cBreath::IsFinished(DWORD currentTime) const {
  return this->m_ucPower == 0;
}

// --- cParticleManager 實作 ---
cParticleManager::cParticleManager()
    : m_pContinueParticle(nullptr),
      m_iContinueparticleCount(0),
      m_bDisplay(true) {}
cParticleManager::~cParticleManager() { DeleteAllParticles(); }
void cParticleManager::DeleteAllParticles() { m_particleList.clear(); }

void cParticleManager::Insert(ParticleFactory::ParticlePtr particle) {
  if (particle) {
    m_particleList.push_back(std::move(particle));
  }
}

void cParticleManager::Draw(const RenderContext& context) {
  if (!m_bDisplay) return;
  for (auto itor = m_particleList.begin(); itor != m_particleList.end();) {
    (*itor)->Draw(context);
    if ((*itor)->IsFinished(context.currentTime)) {
      itor = m_particleList.erase(itor);
    } else {
      ++itor;
    }
  }
}

void cParticleManager::SetContinueParticle(cParticle* particle, int count) {
  this->m_pContinueParticle = particle;
  this->m_iContinueparticleCount = count;
}

void cParticleManager::CheckContinueParticle() {
  if (this->m_iContinueparticleCount > 0 &&
      this->m_pContinueParticle != nullptr) {
    cParticle* newParticleRawPtr = this->m_pContinueParticle->Insert();
    if (newParticleRawPtr) {
      // 這個新粒子不是由工廠創建的，它需要手動管理。
      // 為了安全，我們將它包裝在一個 unique_ptr 中，使用預設的 deleter。
      // 這意味著它會被 delete，而不是歸還給池。
      Insert(ParticleFactory::ParticlePtr(newParticleRawPtr,
                                          [](cParticle* p) { delete p; }));
    }
    this->m_iContinueparticleCount--;
  }
}
// 建構函式: 為空
cEffectSpriteManager::cEffectSpriteManager() {
  
}

// 讀取效果定義檔 (.ef 或 .eb)
bool cEffectSpriteManager::LoadEFFile(int index) {
    if (index < 0 || index >= EffectConstants::MaxSpriteType) {
        return false;
    }

    // 1. 產生檔名 (移除 ./，統一使用 data/ 前綴)
    char base_filename[64];
    snprintf(base_filename, sizeof(base_filename), "data/es%03d", index);

    std::string bin_path = std::string(base_filename) + ".eb";
    std::string text_path = std::string(base_filename) + ".ef";

    EffectFrameList& effect = m_pEffectList[index];

    // 2. 優先讀取二進位檔 (.eb) - Android 推薦路徑
    SDL_RWops* fp = SDL_RWFromFile(bin_path.c_str(), "rb");

    if (fp) {
        unsigned char checkRepeat = 0, maxFrame = 0, totalFrame = 0;
        SDL_RWread(fp, &checkRepeat, sizeof(unsigned char), 1);
        SDL_RWread(fp, &maxFrame, sizeof(unsigned char), 1);
        SDL_RWread(fp, &totalFrame, sizeof(unsigned char), 1);

        effect.IsRepeat = (checkRepeat != 0);
        effect.MaxFrame = maxFrame;
        effect.TotalFrame = totalFrame;

        try {
            effect.FrameIndices.resize(effect.TotalFrame);
            if (effect.TotalFrame > 0) {
                SDL_RWread(fp, effect.FrameIndices.data(), sizeof(unsigned char), effect.TotalFrame);
            }
        } catch (const std::bad_alloc&) {
            SDL_RWclose(fp);
            return false;
        }

        SDL_RWclose(fp);
        return true;
    }

    // 3. 如果二進位檔不存在，讀取文字檔 (.ef)
    // SDL 沒有直接的 >> 運算子，所以先讀入記憶體再用 stringstream 解析
    fp = SDL_RWFromFile(text_path.c_str(), "rb");
    if (fp) {
        Sint64 size = SDL_RWsize(fp);
        std::string content;
        content.resize(size);
        SDL_RWread(fp, &content[0], size, 1);
        SDL_RWclose(fp);

        std::stringstream ss(content);
        unsigned int checkRepeat = 0, maxFrame = 0, totalFrame = 0;

        // 解析 Header
        ss >> checkRepeat >> maxFrame >> totalFrame;

        effect.IsRepeat = (checkRepeat != 0);
        effect.MaxFrame = maxFrame;
        effect.TotalFrame = totalFrame;

        try {
            effect.FrameIndices.resize(effect.TotalFrame);
        } catch (const std::bad_alloc&) {
            return false;
        }

        // 解析 Frame Indices
        for (unsigned int i = 0; i < effect.TotalFrame; ++i) {
            int tempInteger = 0;
            ss >> tempInteger;
            if (ss.fail()) return false;
            effect.FrameIndices[i] = static_cast<unsigned char>(tempInteger);
        }

        // [Android] 不執行寫入 .eb 的動作，因為 Assets 是唯讀的
        return true;
    }

    // 兩個檔案都找不到
    return false;
}

// 讀取 Sprite 圖檔 (.dat)
bool cEffectSpriteManager::LoadSprite(int index, int frameCount) {
    if (index < 0 || index >= EffectConstants::MaxSpriteType) {
        return false;
    }

    // 1. 產生檔名 (注意原始碼是用大寫 'Es')
    char filename[64]={0};
    snprintf(filename, sizeof(filename), "data/es%03d.dat", index);

    // 2. 開啟檔案
    SDL_RWops* file = SDL_RWFromFile(filename, "rb");
    if (!file) {
        SDL_Log("Failed to open sprite file: %s", filename);
        return false;
    }

    // 3. 取得檔案大小
    Sint64 total_file_size = SDL_RWsize(file);

    // 4. 讀取 Sprite 數量 (Header)
    short no = 0;
    SDL_RWread(file, &no, sizeof(short), 1);

    // 計算 Sprite 資料區塊的總長度
    const size_t SPRITE_COUNT_FIELD_SIZE = 2;
    const size_t SPRITE_OFFSET_FIELD_SIZE = 4;
    const size_t header_size = SPRITE_COUNT_FIELD_SIZE + (no * SPRITE_OFFSET_FIELD_SIZE);

    // 這裡要轉型避免 signed/unsigned 警告
    const DWORD sprfilelength = (DWORD)(total_file_size - header_size);

    try {
        // 5. 分配記憶體 (使用 unique_ptr 管理)
        m_pSpriteData[index] = std::make_unique<char[]>(sprfilelength);

        // 6. 跳過 Offset Table (Seek)
        SDL_RWseek(file, no * SPRITE_OFFSET_FIELD_SIZE, RW_SEEK_CUR);

        DWORD nOff = 0;
        for (int j = 0; j < frameCount; j++) {

            if (j >= EffectConstants::MaxFrameCount) {
                SDL_Log("Error: Frame count %d exceeds limit %d for sprite %d",
                        frameCount, EffectConstants::MaxFrameCount, index);
                // 為了安全，直接跳出迴圈，不再讀取後面的 Frame
                break;
            }
            Spr* s = &m_Sprite[index][j];
            DWORD size = 0;

            // 讀取單個 Sprite 的大小
            SDL_RWread(file, &size, sizeof(DWORD), 1);

            if (size == 0) {
                s->xl = 0; s->yl = 0; s->size = 0; s->img = nullptr;
                continue;
            }

            // 讀取 Sprite 元數據
            SDL_RWread(file, &s->xl, sizeof(short), 1);
            SDL_RWread(file, &s->yl, sizeof(short), 1);
            SDL_RWread(file, &s->ox, sizeof(short), 1);
            SDL_RWread(file, &s->oy, sizeof(short), 1);
            SDL_RWread(file, &s->size, sizeof(unsigned int), 1);

            // 邊界檢查
            if (nOff + s->size > sprfilelength) {
                SDL_Log("[%s] File overflow error!", filename);
                SDL_RWclose(file);
                return false;
            }

            // 讀取像素資料
            SDL_RWread(file, m_pSpriteData[index].get() + nOff, s->size, 1);
            s->img = m_pSpriteData[index].get() + nOff;

            nOff += size;
        }

    } catch (const std::bad_alloc& e) {
        SDL_Log("Memory allocation failed for %s: %s", filename, e.what());
        SDL_RWclose(file);
        return false;
    }

    SDL_RWclose(file);

    // 7. 建立紋理 Atlas (SDL 整合)
    g_SDLInfo.CreateAtlasFromLoadedData(filename, &m_Sprite[index][0], frameCount);

    return true;
}
void cEffectSpriteManager::LoadAllSprite() {
  for (int i = 0; i < 10; i++) {
    g_EffectSpriteManager.CheckAndLoadSpriteData(i);
  }
}
// CheckAndLoadSpriteData 和 GetSpriteData
// 的實作保持不變，因為它們不直接涉及檔案 I/O
void cEffectSpriteManager::CheckAndLoadSpriteData(int index) {
  if (index < 0 || index >= EffectConstants::MaxSpriteType) {
    return;
  }

  if (!m_pSpriteData[index]) {
    if (this->LoadEFFile(index)) {
      this->LoadSprite(index, m_pEffectList[index].MaxFrame);
    }
  }
}

Spr* cEffectSpriteManager::GetSpriteData(int index, unsigned char& frame) {
  if (index < 0 || index >= EffectConstants::MaxSpriteType) {
    return nullptr;
  }

  EffectFrameList& effect = m_pEffectList[index];

  if (effect.FrameIndices.empty() || effect.TotalFrame == 0) {
    return nullptr;
  }

  if (effect.IsRepeat) {
    frame = (frame + 1) % effect.TotalFrame;
  } else {
    if (frame < effect.TotalFrame - 1) {
      frame++;
    }
  }

  unsigned char frameIndex = effect.FrameIndices[frame];
  if (frameIndex >= effect.MaxFrame) {
    return nullptr;
  }
 

  return &m_Sprite[index][frameIndex];
}