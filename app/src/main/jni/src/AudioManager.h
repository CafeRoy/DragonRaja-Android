#pragma once  // 防止頭文件重複包含

#include <cstdint>   // For uint8_t
#include <iostream>  // For std::cerr, std::cout
#include <map>       // For std::map (用於音效快取和頻道資訊)
#include <string>    // For std::string
#include <vector>    // For std::vector

// 確保你包含了 SDL 和 SDL_mixer 的核心頭文件
#include <SDL.h>
#include <SDL_mixer.h>

// --- 精確的 SOUNDLIST 結構體定義 ---
// 這是與你原始 BIN 檔案格式兼容的關鍵
typedef struct tagSOUNDLIST_ {
  int list_Index;           // 音效的唯一 ID (例如 0, 1, 2...)
  int static_Sound;         // 是否為靜態音效 (通常在 Windows 下 BOOL 是 int)
  char sound_FileName[10];  // 音效檔案名，精確到 10 個字節 (含空終止符)
} SOUNDLIST_;

#define LOOP -1
#define PLAY_ONCE 0
#define NO_CHANEL -1
#define STOP_ALL -1
// --- 全局常數 (請確保這些定義只存在於一個 .cpp 文件中，否則會重複定義) ---
const int MAX_SOUND = 2500;  // 音效列表的最大數量，已更新為 2500

// 假設你依然有這個全局錯誤變數和錯誤碼，如果沒有，可以移除
extern int CheckSumError;
const int CHECKSUM_ERROR_SOUNDLIST = 1;

// --- 新的： PlayingSoundInfo 結構體 ---
// 用於追蹤每個頻道上播放的音效資訊
struct PlayingSoundInfo {
  int channel;              // 播放該音效的頻道
  int sound_id;             // 該音效的類型 ID
  Mix_Chunk* loaded_chunk;  // 指向被播放的 Mix_Chunk
};

// --- AudioManager 類別定義 ---
class AudioManager {
 public:
  // 建構子和解構子
  AudioManager();
  ~AudioManager();

  // 初始化 SDL_mixer 系統
  bool Init();
  // 關閉 SDL_mixer 系統並釋放所有資源
  void Shutdown();

  void SetDefaultVolume(int volume);

  // 載入音效列表數據 (從文本文件或二進位文件)
  // textFilePath: 文本文件路徑 (e.g., "./SOUND/SOUNDLIST.DAT")
  // binFilePath: 二進位文件路徑 (e.g., "./SOUND/SOUNDLIST.BIN")
  bool LoadSoundList(const std::string& textFilePath,
                     const std::string& binFilePath);

  // --- 播放音效的方法 ---
  // 播放帶有位置衰減的音效 (用於模擬遠近音量)
  // vol: 基礎音量
  // hero_x, hero_y: 聽者 (玩家) 的遊戲世界座標
  // index: 音效ID
  // x, y: 聲音來源的遊戲世界座標
  // isLooping: 是否無限循環播放 (原來的 BOOL flag)
  int PlaySoundWithPosition(int vol, int hero_x, int hero_y, int index, int x,
                            int y, bool isLooping);

  // 播放音效
  // soundId: 要播放音效的 ID
  // volume: 音量 (0-128)
  // loops: 循環次數 (-1 為無限循環，0 為不循環，1 為播放一次)
  // channel: 指定播放的頻道 (-1 為自動選擇可用頻道)
  int PlaySoundA(int soundId, int volume, int loops = 0, int channel = -1);

  // 停止指定頻道的音效
  void StopSound(int index);
  // 停止所有頻道的音效和音樂
  void StopAllSounds();

  // 設定主音量 (0-128)
  void SetMasterVolume(int volume);
  // 設定指定頻道的音量 (0-128)
  void SetChannelVolume(int channel, int volume);
  // 設定特定音效的音量 - 注意：Mix_Chunk 不直接支持，此為提示性接口
  void SetSoundVolume(int soundId, int volume);

  // 暫停指定頻道的音效
  void PauseSound(int channel);
  // 恢復指定頻道的音效
  void ResumeSound(int channel);
  // 暫停所有音效和音樂
  void PauseAllSounds();
  // 恢復所有音效和音樂
  void ResumeAllSounds();

  Mix_Chunk* GetLoadedChunk(int i);

  int IsPlayingSDL(int index);

  void MoveWave(int index, int distance);
  void SetVolumePtr(int* music_volume, int* sound_volume);
 private:
     //Options setting volume pointer
  int* music_volume_ptr;
  int* sound_volume_ptr;
  // 私有輔助函數：根據 soundId 查找其在 m_soundListInternal 中的索引
  int GetSoundListIndex(int soundId);
  int default_volume;
  // 私有輔助函數：從檔案路徑加載 Mix_Chunk
  Mix_Chunk* LoadChunk(const std::string& filePath);
  // 私有輔助函數：釋放 Mix_Chunk 資源
  void FreeChunk(Mix_Chunk* chunk);

  // 私有輔助函數：計算數據塊的 CRC 校驗和
  uint8_t CalculateChecksum(const SOUNDLIST_* data, size_t size_in_bytes);

  // 私有輔助函數：獲取 WAV 檔案資訊（僅用於測試/除錯）
  void getWavInformation(std::string filename, int index);

  // 儲存從文件讀取到的 SOUNDLIST 元數據
  std::vector<SOUNDLIST_> m_soundListInternal;
  // 標記 AudioManager 是否已成功初始化
  bool m_isInitialized;
  int old_music, old_sound;
  // 儲存加載的實際 SDL_mixer 音效數據 (Mix_Chunk)
  // 它的索引與 m_soundListInternal 對應，僅用於靜態音效
  std::vector<Mix_Chunk*> m_loadedChunks;

  // 追蹤每個非靜態音效，以避免重複載入
  std::map<int, Mix_Chunk*> m_nonStaticChunkCache;

  // 這個 vector 用來追蹤每個頻道上播放了什麼音效
  // 它的索引就是頻道編號，這是最有效率的存取方式
  std::vector<PlayingSoundInfo> m_playingChannelInfo;
};

extern AudioManager g_audioManager;
