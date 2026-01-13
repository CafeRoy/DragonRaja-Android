#include "AudioManager.h"  // 包含我們定義的頭文件
#include <sstream>
#include <algorithm>  // For std::min, std::find_if (字串處理)
#include <cctype>     // For std::isspace (字串處理)
#include <cmath>
#include <cstring>  // For std::memcpy, std::memset, std::strncpy (記憶體和字串操作)
#include <filesystem>  // For std::filesystem::exists (檔案存在性檢查)
#include <fstream>     // For std::ifstream, std::ofstream (檔案操作)
#include <numeric>     // For std::accumulate (如果 CalculateChecksum 使用)

// 在這裡定義全局變數，以避免在多個 .cpp 文件中重複定義
// 如果 CheckSumError 只在這個 .cpp 文件中使用，可以不加 extern
// 你的遊戲世界中，聲音完全衰減的最大距離。
const float MAX_AUDIBLE_DISTANCE_GAME_UNITS =
    30 * 32.0f;  // 聲音在遊戲單位中能聽到的最大距離// 32 = TILE_SIZE
// 聲音最大音量區域的半徑，在此範圍內音量不會衰減。
const float MIN_DISTANCE_FOR_MAX_VOL_GAME_UNITS =
    3.5 * 32.0f;  // 遊戲單位中，音量開始衰減的距離

AudioManager g_audioManager;

// --- AudioManager 類別方法的實現 ---

// 建構子
AudioManager::AudioManager() :  default_volume(50),m_isInitialized(false),
      old_music(0),
      old_sound(0),
      music_volume_ptr(nullptr),
      sound_volume_ptr(nullptr) 
{
    music_volume_ptr = 0;
    sound_volume_ptr = 0;
  // 建議建構子中只做初始化列表賦值，不執行耗時或可能失敗的操作
}

// 解構子
AudioManager::~AudioManager() {
  Shutdown();  // 確保在物件銷毀時釋放所有資源
}

// 初始化 SDL_mixer 系統
bool AudioManager::Init() {
  if (m_isInitialized) {
    SDL_Log("AudioManager already initialized.");
    return true;  // 已經初始化，直接返回成功
  }

  // 初始化 SDL_mixer 子系統，指定需要的音頻格式
  // MIX_INIT_OGG | MIX_INIT_MP3 | MIX_INIT_WAV 是常見的格式
  if (Mix_Init(MIX_INIT_OGG | MIX_INIT_MP3) == 0) {
    SDL_Log("Mix_Init error: %s", Mix_GetError());
    return false;
  }

  // 開啟音頻設備
  // 22050 Hz 採樣率, 默認格式, 2 個聲道 (立體聲), 2048 字節的緩衝區大小
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
    SDL_Log("Mix_OpenAudio error: %s", Mix_GetError());
    Mix_Quit();  // 如果開啟音頻設備失敗，應調用 Mix_Quit 進行清理
    return false;
  }
  // Mix_AllocateChannels 會返回當前分配的頻道總數
  int allocatedChannels = Mix_AllocateChannels(48);
  m_playingChannelInfo.resize(allocatedChannels);
  m_isInitialized = true;

  SDL_Log("AudioManager initialized successfully with %d channels.",
          allocatedChannels);
  return true;
}

// 關閉 SDL_mixer 系統並釋放所有資源
void AudioManager::Shutdown() {
  if (!m_isInitialized) {
    return;  // 未初始化，無需關閉
  }

  // 停止所有正在播放的音效和音樂
  Mix_HaltChannel(-1);  // 停止所有頻道 (音效)
  Mix_HaltMusic();      // 停止所有音樂

  // 釋放所有已加載的 Mix_Chunk 資源
  for (Mix_Chunk* chunk : m_loadedChunks) {
    if (chunk) {
      FreeChunk(chunk);  // 調用私有輔助函數釋放
    }
  }
  m_loadedChunks.clear();  // 清空儲存 chunk 指針的 vector

  // 釋放非靜態音效的快取
  for (auto const& [key, val] : m_nonStaticChunkCache) {
    if (val) {
      FreeChunk(val);
    }
  }
  m_nonStaticChunkCache.clear();

  m_soundListInternal.clear();   // 清空儲存 SOUNDLIST 數據的 vector
  m_playingChannelInfo.clear();  // 清空頻道資訊

  // 關閉音頻設備並退出 SDL_mixer
  Mix_CloseAudio();
  Mix_Quit();

  m_isInitialized = false;
  SDL_Log("AudioManager shutdown successfully.");
}

void AudioManager::SetDefaultVolume(int volume) { default_volume = volume; }

// 私有輔助函數：計算數據塊的 CRC 校驗和
uint8_t AudioManager::CalculateChecksum(const SOUNDLIST_* data,
                                        size_t size_in_bytes) {
  uint8_t crc = 0;
  const char* ptr =
      reinterpret_cast<const char*>(data);  // 將結構體數據視為字節流
  for (size_t i = 0; i < size_in_bytes; ++i) {
    crc += static_cast<uint8_t>(ptr[i]);  // 逐字節累加
  }
  return crc;
}

// 私有輔助函數：從檔案路徑加載 Mix_Chunk
// 私有輔助函數：從檔案路徑加載 Mix_Chunk
Mix_Chunk* AudioManager::LoadChunk(const std::string& filePath) {
    // [Android 修改] 移除開頭的 ./
    std::string androidPath = filePath;
    if (androidPath.find("./") == 0) {
        androidPath = androidPath.substr(2);
    }

    // 使用 SDL_RWFromFile 檢查檔案是否存在
    SDL_RWops* rw = SDL_RWFromFile(androidPath.c_str(), "rb");
    if (!rw) {
        //SDL_Log("File not found: %s", androidPath.c_str());
        return nullptr;
    }

    // 使用 Mix_LoadWAV_RW 從記憶體串流載入
    // 1 = 載入後自動關閉 rw
    Mix_Chunk* chunk = Mix_LoadWAV_RW(rw, 1);

    if (!chunk) {
        SDL_Log("Failed to load sound WAV: %s - %s", androidPath.c_str(), Mix_GetError());
        return nullptr;
    }
    Mix_VolumeChunk(chunk, default_volume);
    return chunk;
}

// 私有輔助函數：釋放 Mix_Chunk 資源
void AudioManager::FreeChunk(Mix_Chunk* chunk) {
  if (chunk) {
    Mix_FreeChunk(chunk);  // 釋放 Mix_Chunk 佔用的記憶體
  }
}

// 私有輔助函數：根據 soundId 查找其在 m_soundListInternal 中的索引
int AudioManager::GetSoundListIndex(int soundId) {
  for (size_t i = 0; i < m_soundListInternal.size(); ++i) {
    if (m_soundListInternal[i].list_Index == soundId) {
      return static_cast<int>(i);  // 找到匹配的 ID，返回其索引
    }
  }
  return -1;  // 未找到
}

// 載入音效列表數據
// 載入音效列表數據
bool AudioManager::LoadSoundList(const std::string& textFilePath,
                                 const std::string& binFilePath) {
    if (!m_isInitialized) {
        SDL_Log("AudioManager not initialized. Call Init() first.");
        return false;
    }

    m_soundListInternal.clear();
    m_loadedChunks.clear();
    m_nonStaticChunkCache.clear();
    m_soundListInternal.resize(MAX_SOUND);
    m_loadedChunks.resize(MAX_SOUND, nullptr);

    // [Android 修改] 路徑處理
    std::string androidTextPath = textFilePath;
    if (androidTextPath.find("./") == 0) androidTextPath = androidTextPath.substr(2);

    std::string androidBinPath = binFilePath;
    if (androidBinPath.find("./") == 0) androidBinPath = androidBinPath.substr(2);

    // =========================================================
    // 優先嘗試讀取 .DAT (純文字檔)
    // =========================================================
    SDL_RWops* rwText = SDL_RWFromFile(androidTextPath.c_str(), "rb");
    bool read_text_success = false;

    if (rwText) {
        SDL_Log("Attempting to read %s...", androidTextPath.c_str());

        // 讀取整個檔案到記憶體
        Sint64 size = SDL_RWsize(rwText);
        std::string content;
        content.resize(size);
        SDL_RWread(rwText, &content[0], size, 1);
        SDL_RWclose(rwText);

        // 轉成 stringstream 解析
        std::stringstream ss(content);
        std::string line;
        std::string filename_str_temp;

        int i = 0;
        while(std::getline(ss, line) && i < MAX_SOUND) {
            // 移除 \r
            if (!line.empty() && line.back() == '\r') line.pop_back();
            if (line.empty()) continue;

            std::stringstream lineSS(line);
            if (lineSS >> m_soundListInternal[i].list_Index
                       >> m_soundListInternal[i].static_Sound
                       >> filename_str_temp)
            {
                std::memset(m_soundListInternal[i].sound_FileName, 0, sizeof(m_soundListInternal[i].sound_FileName));
                std::strncpy(m_soundListInternal[i].sound_FileName, filename_str_temp.c_str(), sizeof(m_soundListInternal[i].sound_FileName) - 1);

                // 載入靜態音效
                if (m_soundListInternal[i].static_Sound == 1) {
                    m_loadedChunks[i] = LoadChunk("Sound/" + std::string(m_soundListInternal[i].sound_FileName));
                }
                i++;
            }
        }
        read_text_success = true;
        SDL_Log("Successfully loaded sound list from %s (Text mode)", androidTextPath.c_str());

        // [Android] 我們不寫入 .bin 檔，因為 APK 是唯讀的。
        return true;
    }

    // =========================================================
    // 如果 .DAT 不存在，嘗試讀取 .BIN (二進位檔)
    // =========================================================
    SDL_Log("%s not found. Attempting to read %s...", androidTextPath.c_str(), androidBinPath.c_str());

    SDL_RWops* rwBin = SDL_RWFromFile(androidBinPath.c_str(), "rb");
    if (!rwBin) {
        SDL_Log("Error: Could not open %s for reading.", androidBinPath.c_str());
        return false;
    }

    uint8_t crc1_read, crc2_read;

    // 1. 讀取開頭 CRC
    SDL_RWread(rwBin, &crc1_read, 1, 1);

    // 2. 讀取主資料
    size_t total_data_size_bytes = sizeof(SOUNDLIST_) * MAX_SOUND;
    size_t read_bytes = SDL_RWread(rwBin, m_soundListInternal.data(), 1, total_data_size_bytes);

    if (read_bytes != total_data_size_bytes) {
        SDL_Log("Error reading sound list data. Expected %zu bytes, got %zu.", total_data_size_bytes, read_bytes);
        SDL_RWclose(rwBin);
        return false;
    }

    // 3. 讀取結尾 CRC
    SDL_RWread(rwBin, &crc2_read, 1, 1);
    SDL_RWclose(rwBin);

    // 載入靜態音效
    for (int i = 0; i < MAX_SOUND; ++i) {
        if (m_soundListInternal[i].static_Sound == 1) {
            m_loadedChunks[i] = LoadChunk("Sound/" + std::string(m_soundListInternal[i].sound_FileName));
        }
    }

    // CRC 檢查
    uint8_t calculated_crc = CalculateChecksum(m_soundListInternal.data(), total_data_size_bytes);
    if (calculated_crc != crc1_read || calculated_crc != crc2_read) {
        SDL_Log("Warning: Checksum mismatch in %s!", androidBinPath.c_str());
        if (CheckSumError == 0) CheckSumError = CHECKSUM_ERROR_SOUNDLIST;
    }

    SDL_Log("Successfully loaded sound list from %s (Binary mode)", androidBinPath.c_str());
    return true;
}

void AudioManager::getWavInformation(std::string filename, int index) {
  // 這裡的邏輯已經過時，因為 PlayingSoundInfo 已經重構
  // 此函數目前沒有被調用，可以移除
}

void AudioManager::MoveWave(int index, int distance) {
  int channel = IsPlayingSDL(index);
  if (channel >= 0) {
    // 使用距離來設定音量，這部分邏輯看起來是正確的
    int newVolume = std::min(distance, 100);
    SetChannelVolume(channel, newVolume);
  }
}

void AudioManager::SetVolumePtr(int* music_volume, int* sound_volume) {
  music_volume_ptr = music_volume;
  sound_volume_ptr = sound_volume;
}

// --- PlaySoundWithPosition 方法的實現 (替代原來的 PlayWave) ---
int AudioManager::PlaySoundWithPosition(int vol, int hero_x, int hero_y,
                                        int index, int distance_x, int distance_y,
                                        bool isLooping) {
  if (!m_isInitialized) {
    SDL_Log("AudioManager not initialized. Sound ID %d not played.", index);
    return -1;
  }

  // 計算距離
  float actual_distance_game_units = std::sqrt(
      static_cast<float>(distance_x * distance_x + distance_y * distance_y));

  // 如果距離超過最大可聽距離，就直接返回
  if (actual_distance_game_units >= MAX_AUDIBLE_DISTANCE_GAME_UNITS) {
    //SDL_Log("Sound is too far away. Not playing.");
    return -1;
  }

  int current_volume = vol;
  float attenuation_ratio = 0.0f;
  // 只有當距離超過「最大音量距離」時才開始衰減
  if (actual_distance_game_units > MIN_DISTANCE_FOR_MAX_VOL_GAME_UNITS) {
    // 計算衰減比例：0 到 1
    attenuation_ratio =
        (actual_distance_game_units - MIN_DISTANCE_FOR_MAX_VOL_GAME_UNITS) /
        (MAX_AUDIBLE_DISTANCE_GAME_UNITS - MIN_DISTANCE_FOR_MAX_VOL_GAME_UNITS);
    // 使用 vol 進行線性衰減
    current_volume = static_cast<int>(vol * (1.0f - attenuation_ratio));
    // --- 在這裡加入日誌輸出，以確認傳入的數值是否正確 ---
    /* SDL_Log(
        "PlaySoundWithPosition: dist_x=%d, dist_y=%d, actual_dist=%.2f, "
        "initial_vol=%d, soundId=%d",
        distance_x, distance_y, actual_distance_game_units, vol, index);
    
    SDL_Log("Attenuating: actual_dist > MIN_DIST.Origin volume=%d New volume=%d",
            vol, current_volume);*/
  }
  // 確保音量不小於 0
  if (current_volume <= 0) {
      //SDL_Log("Volume is zero or less after decay. Not playing. actual_distance_game_units=%f attenuation_ratio=%f", actual_distance_game_units, attenuation_ratio);
      return -1;
  }

  int loops = isLooping ? -1 : 0;
  int channel = PlaySoundA(index, current_volume, loops);
  return channel;  // 返回播放的通道 ID
}

// 播放音效
int AudioManager::PlaySoundA(int soundId, int volume, int loops, int channel) {
  int index = GetSoundListIndex(soundId);
  if (index < 0) {
    //SDL_Log("Sound with ID %d not found.", soundId);
    return -1;
  }
  Mix_Chunk* chunk = nullptr;
  // 1. 統一取得 Mix_Chunk* 的邏輯
  //    這部分邏輯是正確的，不需要修改。
  if (m_soundListInternal[index].static_Sound == 1) {
    chunk = m_loadedChunks[index];
  }
  else {
    auto it = m_nonStaticChunkCache.find(soundId);
    if (it != m_nonStaticChunkCache.end()) {
      chunk = it->second;
    }
    else {
      std::string filePath =
          "Sound/" + std::string(m_soundListInternal[index].sound_FileName);
      chunk = LoadChunk(filePath);  // LoadChunk 只是載入，不會設定音量
      if (chunk) {
        m_nonStaticChunkCache[soundId] = chunk;
      }
    }
  }
  if (!chunk) {
    //SDL_Log("Chunk for sound ID %d could not be loaded.", soundId);
    return -1;
  }

  // 2. 核心修改：播放音效並設定音量
  //    這個單一的區塊現在處理所有音效的播放，不論是靜態還是非靜態。
  int played_channel = Mix_PlayChannel(channel, chunk, loops);
  if (played_channel == -1) {
      //SDL_Log("Mix_PlayChannel error: %s [%s]", Mix_GetError(),
          //m_soundListInternal[index].sound_FileName);
      return -1;
  }
  //else SDL_Log("Mix_PlayChannel [%d][%d]", played_channel, volume);

  //if (index >= 2000) volume = volume / 2;  // 2000 以上的音效音量減半
    // 統一使用 Mix_Volume 函式來設定頻道音量。
    // 這種方式比 Mix_VolumeChunk 更安全，因為它只影響單一頻道。
    // 如果你的 volume 範圍是 0-128，Mix_Volume 函式也接受 0-128。
    // 所以直接傳入 volume 即可。
    Mix_Volume(played_channel, volume);
 
    // 3. 儲存頻道資訊
    if (played_channel >= 0 && played_channel < m_playingChannelInfo.size()) {
        m_playingChannelInfo[played_channel].channel = played_channel;
        m_playingChannelInfo[played_channel].sound_id = soundId;
        m_playingChannelInfo[played_channel].loaded_chunk = chunk;
  
    }
    return played_channel;
}

// 停止指定頻道的音效
void AudioManager::StopSound(int index) {
  int channel = IsPlayingSDL(index);
  if (channel >= 0) {
    Mix_FadeOutChannel(channel, 2000);
  }
}

// 停止所有頻道的音效和音樂
void AudioManager::StopAllSounds() {
  Mix_HaltChannel(-1);  // -1 表示所有頻道
  Mix_HaltMusic();      // 停止音樂
}

// 設定主音量 (0-128)
void AudioManager::SetMasterVolume(int volume) {

    if (!music_volume_ptr || !sound_volume_ptr) return;

    if (volume == -1)
    {
        *music_volume_ptr = old_music;

        *sound_volume_ptr = old_sound;
    }
    else {
        old_music = *music_volume_ptr;

        old_sound = *sound_volume_ptr;

        *music_volume_ptr = volume;

        *sound_volume_ptr = volume;
    }
}

// 設定指定頻道的音量 (0-128)
void AudioManager::SetChannelVolume(int channel, int volume) {
  Mix_Volume(channel, volume);
}

// 設定特定音效的音量 (此功能在 SDL_mixer 的 Mix_Chunk 層面不直接支持)
void AudioManager::SetSoundVolume(int soundId, int volume) {
  int channel = IsPlayingSDL(soundId);
  if (channel >= 0) {
    SetChannelVolume(channel, volume);
  }
}

// 暫停指定頻道的音效
void AudioManager::PauseSound(int channel) {
  if (channel != -1) {
    Mix_Pause(channel);
  }
}

// 恢復指定頻道的音效
void AudioManager::ResumeSound(int channel) {
  if (channel != -1) {
    Mix_Resume(channel);
  }
}

// 暫停所有音效和音樂
void AudioManager::PauseAllSounds() {
  Mix_Pause(-1);     // 暫停所有頻道
  Mix_PauseMusic();  // 暫停音樂
}

// 恢復所有音效和音樂
void AudioManager::ResumeAllSounds() {
  Mix_Resume(-1);     // 恢復所有頻道
  Mix_ResumeMusic();  // 恢復音樂
}

Mix_Chunk* AudioManager::GetLoadedChunk(int i) {
  if (i < 0 || i > m_loadedChunks.size()) return nullptr;
  return m_loadedChunks[i];
}

// return channel (int) -1 表示沒有播放 不是-1 返回的是 channel number
int AudioManager::IsPlayingSDL(int index) {
  if (index < 0 || index >= m_soundListInternal.size()) return -1;

  // 檢查該音效是否正在任何頻道上播放
  int numChannels = Mix_AllocateChannels(-1);

  Mix_Chunk* chunkToFind = nullptr;
  // 檢查是否為靜態音效
  if (m_soundListInternal[index].static_Sound == 1) {
    chunkToFind = m_loadedChunks[index];
  } else {
    // 如果是非靜態，從快取中查找
    auto it = m_nonStaticChunkCache.find(index);
    if (it != m_nonStaticChunkCache.end()) {
      chunkToFind = it->second;
    } else {
      // 未載入，則肯定沒在播放
      return -1;
    }
  }

  if (chunkToFind == nullptr) {
    return -1;
  }

  for (int i = 0; i < numChannels; ++i) {
    if (Mix_Playing(i) && Mix_GetChunk(i) == chunkToFind) {
      return i;  // 找到正在播放的頻道
    }
  }
  return -1;
}
