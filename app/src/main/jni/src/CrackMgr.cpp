// CrackMgr.cpp: implementation of the CCrackMgr class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include <fstream>
#include <sstream>
#include "CrackMgr.h"
#include "dragon.h"
#include "Effect.h"
#include "Battle.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const int CHECK_ACCEL_RECOMMAND_TIME = 20000;

CCrackMgr CrackMgr;


// 編譯時字串加密 (不變)
constexpr char xor_char(char c) { return c ^ 0xAF; }

template <size_t N>
struct EncryptedString {
    char data[N];
    constexpr EncryptedString(const char* str) : data{} {
        for (size_t i = 0; i < N - 1; ++i) {
            data[i] = xor_char(str[i]);
        }
        data[N - 1] = '\0';
    }
    const char* get() const { return data; }
};

// 惡意視窗的通用結構
struct CheatWindowEntry {
    char className[100];
    char windowTitle[100];
};

// --- 函式實現 (不變) ---
void XorDecrypt(char* str) {
    for (unsigned int i = 0; str[i] != '\0'; ++i) {
        str[i] ^= 0xAF;
    }
}

// --- 惡意視窗列表的動態初始化 ---
// 這個函式只會在第一次被呼叫時執行
const std::vector<CheatWindowEntry>& GetEncryptedWindows() {
    static std::vector<CheatWindowEntry> list;
    if (!list.empty()) {
        return list;
    }

    // 在這裡使用編譯時加密
    const auto enc_ce = EncryptedString<13>("Cheat Engine");
    const auto enc_olly = EncryptedString<8>("OllyDbg");
    const auto enc_x64dbg = EncryptedString<7>("x64dbg");
    //const auto enc_ida = EncryptedString<4>("IDA");
    const auto enc_ph = EncryptedString<15>("Process Hacker");
    const auto enc_gm = EncryptedString<12>("Game Master");
    //const auto enc_ajjl = EncryptedString<5>("按鍵精靈");
    //const auto enc_jdy = EncryptedString<4>("簡單遊");
    //const auto enc_wi = EncryptedString<17>("Window Inspector");

    // 將加密字串複製到可變的結構體中
    auto add_entry = [&](const char* className, const char* windowTitle) {
        CheatWindowEntry entry;
        if (strlen(className) > 0) {
            strcpy(entry.className, className);
        }
        else {
            entry.className[0] = '\0';
        }
        if (strlen(windowTitle) > 0) {
            strcpy(entry.windowTitle, windowTitle);
        }
        else {
            entry.windowTitle[0] = '\0';
        }
        list.push_back(entry);
        };

    add_entry("", enc_ce.get());
    add_entry("", enc_olly.get());
    add_entry("", enc_x64dbg.get());
    //add_entry("", enc_ida.get());
    add_entry("", enc_ph.get());
    add_entry("", enc_gm.get());
    //add_entry("", enc_ajjl.get());
    //add_entry("", enc_jdy.get());
    //add_entry(enc_wi.get(), "");

    return list;
}
/*
// --- EnumWindows 的回呼函式 ---
BOOL CALLBACK EnumWindowsCallback(HWND hwnd, LPARAM lparam) {
    char tempClassName[100];
    char tempWindowTitle[100];

    // 獲取視窗標題和類別名稱
    GetClassNameA(hwnd, tempClassName, sizeof(tempClassName));
    GetWindowTextA(hwnd, tempWindowTitle, sizeof(tempWindowTitle));

    // 獲取加密列表
    const auto& enc_list = GetEncryptedWindows();

    // 遍歷列表進行比對
    for (const auto& entry : enc_list) {
        char decryptedClassName[100] = { 0 };
        char decryptedWindowTitle[100] = { 0 };

        // 解密
        if (entry.className[0] != '\0') {
            strcpy(decryptedClassName, entry.className);
            XorDecrypt(decryptedClassName);
        }
        if (entry.windowTitle[0] != '\0') {
            strcpy(decryptedWindowTitle, entry.windowTitle);
            XorDecrypt(decryptedWindowTitle);
        }
        CharToUpper(tempClassName); CharToUpper(decryptedClassName);
        CharToUpper(tempWindowTitle); CharToUpper(decryptedWindowTitle);
        // 轉換為大寫進行不區分大小寫的比對
        if ((decryptedClassName[0] != '\0' && strstr((tempClassName), (decryptedClassName)) != nullptr) ||
            (decryptedWindowTitle[0] != '\0' && strstr((tempWindowTitle), (decryptedWindowTitle)) != nullptr)) {

            // 找到作弊工具，返回 FALSE 以停止遍歷
            return FALSE;
        }
    }
    return TRUE;
}
*/

// --- Constructor / Destructor ---
CCrackMgr::CCrackMgr() : m_bIsCTRun(false), m_heartbeat(0) {
  m_i64Clock = 0;
  SetThreadApplyGab(0);
  /*maxEffectCount[battle::WIND_EXTREME] = 20;
  maxEffectCount[battle::LIGHTNING_EXTREME] = 30;
  maxEffectCount[battle::ICING_BLAST] = 30;
  maxEffectCount[battle::ICE_EXTREME] = 30;
  maxEffectCount[battle::FIRE_EXTREME] = 30;
  maxEffectCount[battle::MULTIPLE_FIRE] = 30;
  maxEffectCount[590] = 5;
  maxEffectCount[591] = 5;*/
  for (int i = 0; i < MAX_MAGIC; i++) {
      maxEffectCount[i] = 50;
  }
  //maxEffectCount[battle::LIGHTNING_EXTREME] = 80;
  //maxEffectCount[battle::BLOOD_LIGHTNING] = 80;
  //maxEffectCount[battle::LIGHTNING_BOOM] = 80;
}

CCrackMgr::~CCrackMgr() {
  if (m_bIsCTRun.load()) {
    StopCrackThread();
  }
}

// --- Public Interface Implementation ---

void CCrackMgr::InitCrackMgr() {
  ::memset(&RecvPacketSave, 0, sizeof(t_server_check_accel));
  dwRecommandTime = 0;
  dwRealTime = 0;
  dwRecvedTime = 0;
}

void CCrackMgr::RecvCheckAccel(t_server_check_accel* packet) {
  dwRealTime = SDL_GetTicks();
  RecvPacketSave = *packet;
  dwRecvedTime = dwRealTime;
  dwRecommandTime = dwRealTime + CHECK_ACCEL_RECOMMAND_TIME;
}

void CCrackMgr::AutoCheckAccel() {
  return;
    ProcessPacketQueue();

  if (!IsCrackThreadRun()) {
#ifndef _DEBUG
    ::ExitApplication(EA_CRACK_THREAD_FIND_CRACK);
#endif
  }

  if ((!dwRecommandTime && dwRecvedTime) ||
      (dwRecommandTime && !dwRecvedTime)) {
    ExitApplication(EA_CHECK_ACCEL);
  }
  const int iResult = (int)((int)dwRealTime - (int)dwRecommandTime);
  dwRealTime = SDL_GetTicks();
  if (dwRecommandTime && dwRecvedTime) {
    if (0 < iResult) {
      t_packet packet;
      packet.h.header.type = CMD_CHECK_ACCEL;
      packet.h.header.size = sizeof(t_server_check_accel);
      memcpy(&packet.u.server_check_accel, &RecvPacketSave,
             sizeof(t_server_check_accel));
      QueuePacket(&packet, 1);
      InitCrackMgr();
    }
  }
}

// --- Thread Management ---

bool CCrackMgr::StartCrackThread() {
  /*if (m_bIsCTRun.load()) return true;

  SetThreadApplyGab(1000);  // Value from original StartCrackThread
  m_bIsCTRun.store(true);

  try {
    m_crackThread = std::thread(&CCrackMgr::CrackCheckThread, this);
    m_watchdogThread = std::thread(&CCrackMgr::WatchdogThread, this);
  } catch (...) {
    m_bIsCTRun.store(false);
    return false;
  }
*/
  return true;
}

bool CCrackMgr::StopCrackThread() {
  if (!m_bIsCTRun.load()) return true;
/*
  m_bIsCTRun.store(false);

  if (m_watchdogThread.joinable()) {
    m_watchdogThread.join();
  }
  if (m_crackThread.joinable()) {
    m_crackThread.join();
  }
*/
  return true;
}

bool CCrackMgr::IsCrackThreadRun() { return m_bIsCTRun.load(); }

// --- Thread-Safe Packet Queue ---

void CCrackMgr::PushPacketToQueue(const std::string& msg) {
  std::lock_guard<std::mutex> lock(m_packetQueueMutex);
  m_PacketQueue.push_back(msg);
}

void CCrackMgr::ProcessPacketQueue() {
  std::lock_guard<std::mutex> lock(m_packetQueueMutex);

  if (m_PacketQueue.empty()) {
    return;
  }

  for (const auto& message : m_PacketQueue) {
    t_packet p;
    p.h.header.type = CMD_SEND_ACCEL_TYPE;
    p.h.header.size = sizeof(t_ACCEL_TYPE);
    strcpy(p.u.Accel_Type.strMsg, message.c_str());
    ::QueuePacket(&p, 1);
  }
  m_PacketQueue.clear();
}

// --- Anti-Cheat Threads Implementation ---
void CCrackMgr::CrackCheckThread() {
#ifndef _DEBUG
  const int i1Sec = 1000;
#endif

  while (m_bIsCTRun.load()) {
    m_heartbeat++;

#ifndef _DEBUG
#ifdef _WIN32
    /*if (IsDebuggerDetected()) {//todo
      //g_DBGLog.Log(LOG_LV1, "Hacking tool or Debugger detected.");
      //std::quick_exit(EA_CRACK_THREAD_FIND_CRACK);//todo
      //return;
    }
    
    if (IsCheatToolRunning()) {
        g_DBGLog.Log(LOG_LV1, "ToolRunning");
        //std::quick_exit(EA_CRACK_THREAD_FIND_CRACK);//todo
        return;
    }*/

    if (IsFunctionHooked()) {
      //g_DBGLog.Log(LOG_LV1, "Detected Inline Hook on a critical API.");
      PushPacketToQueue("API_HOOK");
      g_DBGLog.Log(LOG_LV1, "API_HOOK");
      //std::quick_exit(EA_CRACK_THREAD_FIND_CRACK);//todo
      return;
    }

    /*if (IsHardwareBreakpointSet()) {
      //std::quick_exit(EA_CRACK_THREAD_FIND_CRACK);//todo
      //return;
    }

    if (IsTimeDiscrepancy2()) {
        g_DBGLog.Log(LOG_LV1, "API_HOOK 2");
        return;
    }*/

    BOOL bTGT_SLP = false;
    BOOL bGTC_SLP = false;
    BOOL bTGT_GTC = false;
    DWORD dwPrevTGT = 0, dwNowTGT = 0;
    DWORD dwPrevGTC = 0, dwNowGTC = 0;

    dwPrevTGT = SDL_GetTicks();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    dwNowTGT = SDL_GetTicks();
    DWORD dwRetTGT = (dwNowTGT - dwPrevTGT);
    if (i1Sec + GetThreadApplyGab() < dwRetTGT) {
      //g_DBGLog.Log(LOG_LV1, "Your System have problem Type(%d) ", dwRetTGT);
      //std::quick_exit(EA_CRACK_THREAD_FIND_CRACK);//todo
      bTGT_SLP = true;
    }

    dwPrevGTC = ::GetTickCount();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    dwNowGTC = ::GetTickCount();
    DWORD dwRetGTC = (dwNowGTC - dwPrevGTC);
    if (i1Sec + GetThreadApplyGab() < dwRetGTC) {
      bGTC_SLP = true;
    }

    dwPrevGTC = GetTickCount();
    dwPrevTGT = SDL_GetTicks();
    while (GetTickCount() - dwPrevGTC < 1000) {
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    dwNowTGT = SDL_GetTicks();
    dwRetTGT = dwNowTGT - dwPrevTGT;
    if (i1Sec + GetThreadApplyGab() < dwRetTGT) {
      bTGT_GTC = true;
    }

    if (bTGT_SLP) {
      if (!bGTC_SLP) {
        if (bTGT_GTC) PushPacketToQueue("timeGetTime");
      } else {
        if (bTGT_GTC)
          PushPacketToQueue("timeGetTime and GetTickCount and Sleep");
        else
          PushPacketToQueue("Sleep");
      }
    } else if (bTGT_GTC) {
      if (bGTC_SLP)
        PushPacketToQueue("GetTickCount");
      else
        PushPacketToQueue("Unknown");
    } else if (bGTC_SLP) {
      PushPacketToQueue("Unknown");
    }

    if (bTGT_SLP || bGTC_SLP || bTGT_GTC) {
      //__asm int 3;  // Trigger Breakpoint Exception
      g_DBGLog.Log(LOG_LV1, "Your System have problem Type(%d) ", dwRetTGT);
      //std::quick_exit(EA_CRACK_THREAD_FIND_CRACK);//todo
      return;
    }
#endif
#endif
    std::this_thread::sleep_for(std::chrono::seconds(3));
  }
}

void CCrackMgr::WatchdogThread() {
#ifndef _DEBUG
  std::this_thread::sleep_for(std::chrono::seconds(5));

  unsigned int lastHeartbeat = m_heartbeat.load();

  while (m_bIsCTRun.load()) {
    std::this_thread::sleep_for(std::chrono::seconds(5));

    if (lastHeartbeat == m_heartbeat.load()) {
      g_DBGLog.Log(LOG_LV1, "Anti-Cheat thread suspended or terminated.");
      //std::quick_exit(EA_CRACK_THREAD_FIND_CRACK);//todo
      return;
    }

    // 其他檢查保持不變
    /*if (IsHardwareBreakpointSet()) {
      //std::quick_exit(EA_CRACK_THREAD_FIND_CRACK);//todo
    }*/

    if (IsTimeDiscrepancy()) {
        return;
    }
    lastHeartbeat = m_heartbeat.load();
  }
#endif
}

bool CCrackMgr::IsDebugger() {
#ifdef _WIN32
#ifndef _DEBUG
    return (IsDebuggerPresent() );
#endif
#endif
	return false;
}

bool CCrackMgr::IsDebuggerDetected() { 

  // 使用組合語言內聯函式讀取 FS:[0x30]
  // 該處為 PEB 的位址
#ifdef _WIN32
  DWORD peb_address;
  __asm {
        mov eax, dword ptr fs:[0x30]
        mov peb_address, eax
  }

  // 檢查 PEB 的 IsDebugged 旗標（偏移量為 0x02）
  if (*(PBYTE)(peb_address + 0x02) == 1) {
    return true;
  }

#endif

  return false;
}

bool CCrackMgr::IsCheatToolRunning() {
#ifdef _WIN32
    return EnumWindows(EnumWindowsCallback, NULL) == FALSE;
#endif
    return true;
}

// 這是主要的偵測函式
bool CCrackMgr::IsFunctionHooked() {
#ifdef _WIN32
  // 使用編譯時加密來混淆字串
  const char* enc_winmm_dll_str = EncryptedString<11>("winmm.dll").data;
  const char* enc_timeGetTime_str = EncryptedString<12>("timeGetTime").data;
  const char* enc_kernel32_dll_str = EncryptedString<13>("kernel32.dll").data;
  const char* enc_GetTickCount_str = EncryptedString<14>("GetTickCount").data;
  const char* enc_Sleep_str = EncryptedString<6>("Sleep").data;

  // 將加密字串複製到可修改的緩衝區
  char winmm_dll[11] = {0};
  char timeGetTime[12] = {0};
  char kernel32_dll[13] = {0};
  char GetTickCount[14] = {0};
  char Sleep[6] = {0};

  strcpy(winmm_dll, enc_winmm_dll_str);
  strcpy(timeGetTime, enc_timeGetTime_str);
  strcpy(kernel32_dll, enc_kernel32_dll_str);
  strcpy(GetTickCount, enc_GetTickCount_str);
  strcpy(Sleep, enc_Sleep_str);

  // 在使用前解密
  XorDecrypt(winmm_dll);
  XorDecrypt(timeGetTime);
  XorDecrypt(kernel32_dll);
  XorDecrypt(GetTickCount);
  XorDecrypt(Sleep);

  // 進行 Hook 檢查
  HMODULE hModule;
  void* functionAddress;
  char* pFunction;

  // 檢查 winmm.dll::timeGetTime
  hModule = GetModuleHandleA(winmm_dll);
  if (hModule != NULL) {
    functionAddress = (void*)GetProcAddress(hModule, timeGetTime);
    if (functionAddress != NULL) {
      pFunction = (char*)functionAddress;
      if (pFunction[0] == 0xE9 || pFunction[0] == 0xEB) {
        return true;
      }
    }
  }

  // 檢查 kernel32.dll::GetTickCount
  hModule = GetModuleHandleA(kernel32_dll);
  if (hModule != NULL) {
    functionAddress = (void*)GetProcAddress(hModule, GetTickCount);
    if (functionAddress != NULL) {
      pFunction = (char*)functionAddress;
      if (pFunction[0] == 0xE9 || pFunction[0] == 0xEB) {
        return true;
      }
    }
  }

  // 檢查 kernel32.dll::Sleep
  if (hModule != NULL) {
    functionAddress = (void*)GetProcAddress(hModule, Sleep);
    if (functionAddress != NULL) {
      pFunction = (char*)functionAddress;
      if (pFunction[0] == 0xE9 || pFunction[0] == 0xEB) {
        return true;
      }
    }
  }
 #endif
  return false;
}

// --- Original Clock Functions ---

void CCrackMgr::SetInitClock() { m_i64Clock = GetAverageClock(); }

__int64 CCrackMgr::GetAverageClock() {
 return 0;
}

void __stdcall CCrackMgr::GetNowClock(CLOCK* pClock) {

}


// --- (2) 檢查硬體中斷點，使用 GetThreadContext API ---
// 原理：讀取處理器的 DR0-DR3 除錯暫存器
bool CCrackMgr::IsHardwareBreakpointSet() {

  return false;
}

void CCrackMgr::count_magiclist_for_cheat_detection()
{
    // 遍歷你的 MAGICLIST 鍊表
    MAGICLIST* current = g_lpML;
    while (current != nullptr) {

        if(current->lpChar_Target->IsNpc())
            magicEffectCount[current->magicSerial]++; // magicNum 是魔法的ID

        current = current->Next;
    }

    for (auto it : magicEffectCount)
    {
        if (it.second > maxEffectCount[it.first])
        //if (magicEffectCount[i] > 0)
        {
            //SDL_Log("just test for count magic list %d:[%d]", i, magicEffectCount[i]);
            std::string msg = "magic list " + std::to_string(it.first) + ":" + std::to_string(maxEffectCount[it.first]);
            PushPacketToQueue(msg);
            //magicEffectCount[i] = 0;
        }
    }

    magicEffectCount.clear();
}

// --- (3) 檢查時間戳，不使用 API ---
// 原理：測量一段迴圈的執行時間，與正常時間進行對比
bool CCrackMgr::IsTimeDiscrepancy2() {
    // 使用 C++11 chrono 庫來獲取高精度時間
    auto start = std::chrono::high_resolution_clock::now();

    // 執行一個耗時但不執行任何有意義操作的迴圈
    for (volatile int i = 0; i < 100000; ++i) {
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;

    // 假設在調試環境下，執行時間會顯著增加
    // 這裡的閾值需要根據你的機器性能進行調整，
    // 在我的機器上，正常執行時間約為 0.0001 秒
    // 但在調試器單步執行時會遠遠超過這個時間
    if (diff.count() > 5.0f) {
        return true;
    }

    return false;
}

// --- (3) 檢查時間戳，不使用 API ---
// 原理：測量一段迴圈的執行時間，與正常時間進行對比
bool CCrackMgr::IsTimeDiscrepancy() {
  // 使用 C++11 chrono 庫來獲取高精度時間
  auto start = std::chrono::high_resolution_clock::now();

  // 執行一個耗時但不執行任何有意義操作的迴圈
  for (volatile int i = 0; i < 100000; ++i) {
  }

  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> diff = end - start;

  // 假設在調試環境下，執行時間會顯著增加
  // 這裡的閾值需要根據你的機器性能進行調整，
  // 在我的機器上，正常執行時間約為 0.0001 秒
  // 但在調試器單步執行時會遠遠超過這個時間
  if (diff.count() > 5.0f) {
    return true;
  }

  return false;
}

unsigned long CCrackMgr::AddCRC(const void* pSource, int size, int step)
{
    // 將 void* 轉換為 const unsigned char* 以進行位元組操作
    const unsigned char* data = static_cast<const unsigned char*>(pSource);

    // 定義一個 custom accumulator 函式
    // std::accumulate 通常用於連續區間，但因為有 step，我們仍需使用 for 迴圈或自定義迭代器。
    // 為了保持與原邏輯的匹配性（跳躍累積），我們使用範圍 for 迴圈的現代版本：

    unsigned long count = 0;
    for (int i = 0; i < size; i += step)
    {
        count += data[i];
    }

    return count;

    /* 如果 step 總是 1，則可以使用 std::accumulate:
    return std::accumulate(data, data + size, 0UL);

    由於您的邏輯涉及 step > 1，最清晰的現代化方式是保留 for 迴圈，但使用更嚴格的類型。
    */
}


bool CCrackMgr::checkCrc(std::string sprite_Name, const char* /*memBuf*/, int /*memSize*/)
{
    // 使用範圍 for 迴圈遍歷 datCrc 向量
    for (const auto& db : datCrc)
    {
        // 1. 尋找對應的檔案紀錄
        if (db.path == sprite_Name)
        {
            // ---------------------------------------------------------
            // Android 檔案讀取流程 (取代 std::ifstream)
            // ---------------------------------------------------------

            // [Android] 路徑修正：移除開頭的 ./
            std::string filePath = db.path;
            if (filePath.find("./") == 0) filePath = filePath.substr(2);

            SDL_RWops* fp = SDL_RWFromFile(filePath.c_str(), "rb");

            if (!fp)
            {
                // 檔案開啟失敗
                SDL_Log("CCrackMgr::checkCrc Error: Failed to open file %s", filePath.c_str());
                return false;
            }

            // 取得檔案大小
            Sint64 fileSize = SDL_RWsize(fp);
            if (fileSize < 0) {
                SDL_RWclose(fp);
                return false;
            }

            // 分配緩衝區並讀取
            std::vector<char> rawData(fileSize);
            size_t bytesRead = SDL_RWread(fp, rawData.data(), 1, fileSize);
            SDL_RWclose(fp);

            if (bytesRead != static_cast<size_t>(fileSize)) {
                SDL_Log("CCrackMgr::checkCrc Error: Read incomplete %s", filePath.c_str());
                return false;
            }

            // ---------------------------------------------------------
            // 計算 CRC
            // ---------------------------------------------------------
            unsigned long crc = AddCRC(rawData.data(), (int)fileSize, 1000);

            if (db.crc != crc) {
                SDL_Log("[CRC ERROR] ID:%d File:%s DB_CRC:%lu Calc_CRC:%lu",
                        db.sprite_num, db.path.c_str(), db.crc, crc);
                return false;
            }

            // 找到且 CRC 正確，檢查通過
            // SDL_Log("CRC OK: %s", filePath.c_str());
            return true;
        }
    }

    // 如果迴圈結束後，沒有找到匹配的 sprite_num
    // 原邏輯是 return true (沒在清單裡的檔案不檢查)
    return true;
}

bool CCrackMgr::LoadCrcDat()
{
    // =========================================================
    // Android 修正：使用 SDL_RWops 讀取 APK 內的 Assets
    // =========================================================

    // 1. 注意路徑：Android 不需要 "./"，直接從 assets 根目錄算起
    // 檔名大小寫必須完全正確！
    const char* filename = "Event/EVENT.rof";

    SDL_RWops* file = SDL_RWFromFile(filename, "rb");

    if (file == NULL)
    {
        // 讀取失敗，印出錯誤訊息
        SDL_Log("Error: Failed to open asset file %s. SDL Error: %s", filename, SDL_GetError());
        return false;
    }

    // 2. 取得檔案大小
    Sint64 fileSize = SDL_RWsize(file);
    if (fileSize <= 0) {
        SDL_Log("Error: File %s is empty or invalid size.", filename);
        SDL_RWclose(file);
        return false;
    }

    // 3. 讀取整個檔案內容到 std::string
    std::string fileContent;
    fileContent.resize(fileSize);

    // 讀取資料 (這裡是一次讀進記憶體)
    SDL_RWread(file, &fileContent[0], fileSize, 1);

    // 4. 關閉檔案指標
    SDL_RWclose(file);

    // 5. 轉換成 stringstream，這樣下面的程式碼幾乎不用動
    // 將原本的 inFile 替換成這個 ssInFile
    std::stringstream inFile(fileContent);

    // =========================================================
    // 以下邏輯保持不變 (除了 inFile 變數本身變成了 stringstream)
    // =========================================================

    std::string line;
    // 使用 std::getline 逐行讀取
    while (std::getline(inFile, line)) // 這裡 inFile 現在是記憶體串流，用法跟 ifstream 一樣
    {
        // 移除 Windows 可能造成的 \r (CR) 換行符號，避免 stoi 報錯
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        if (line.empty()) continue; // 跳過空行

        datBin db;
        std::stringstream ss(line);
        std::string token;

        // 1. 讀取 sprite_num
        if (std::getline(ss, token, ';'))
        {
            try {
                db.sprite_num = std::stoi(token);
            }
            catch (...) {
                SDL_Log("Warning: Invalid sprite_num format in line: %s", line.c_str());
                continue;
            }
        }

        // 2. 讀取 path
        if (std::getline(ss, token, ';'))
        {
            // *** 重要提醒 ***
            // Windows 的路徑是反斜線 '\'，Android 是正斜線 '/'
            // 這裡建議順手把路徑修正，以免後面讀取圖片失敗
            for (char &c : token) {
                if (c == '\\') c = '/';
            }
            // 移除可能存在的 "./" 開頭
            if (token.length() > 2 && token[0] == '.' && token[1] == '/') {
                token = token.substr(2);
            }

            db.path = token;
        }

        // 3. 讀取 crc
        if (std::getline(ss, token, ';'))
        {
            try {
                db.crc = std::stoul(token);
            }
            catch (...) {
                // 有些舊檔案最後可能有奇怪的空白，忽略錯誤
                continue;
            }
        }

        datCrc.push_back(std::move(db));
    }

    // 檢查有沒有讀到資料
    SDL_Log("LoadCrcDat Success. Loaded %zu entries.", datCrc.size());
    /*for (const auto& db : datCrc)
    {
        if (db.path != "")
        {
            std::ifstream inFile(db.path, std::ios::binary | std::ios::ate);

            if (!inFile.is_open())
            {
                // 檔案開啟失敗
                SDL_Log("Error: Failed to open input file %s", db.path.c_str());
                continue;
            }
            std::vector<char>buf;
            std::streamsize file_size = inFile.tellg();
            inFile.seekg(0, std::ios::beg);

            buf.resize(file_size);
            if (!inFile.read(buf.data(), file_size)) {
                return false;
            }
            unsigned long crc = AddCRC(buf.data(), file_size, 1000);
            if (db.crc != crc) {
                SDL_Log("%d;%s;%d", db.sprite_num, db.path.c_str(), crc);
            }
            
        }
    }*/
    return (!datCrc.empty());
}