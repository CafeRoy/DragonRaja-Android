// CrackMgr.h: interface for the CCrackMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRACKMGR_H__916F254F_DB1F_4269_BC2B_36BC59E48BC9__INCLUDED_)
#define AFX_CRACKMG_H__916F254F_DB1F_4269_BC2B_36BC59E48BC9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER > 1000
#include <atomic>
#include <chrono>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

// datBin 結構
struct datBin
{
    int sprite_num = 0;
    std::string path;
    unsigned long crc = 0;
};



class CCrackMgr {
  struct CLOCK {
    DWORD dwLow;
    DWORD dwHi;
  };

 public:
  CCrackMgr();
  virtual ~CCrackMgr();

  // --- Public Interface ---
  bool StartCrackThread();
  bool StopCrackThread();
  bool IsCrackThreadRun();
  void AutoCheckAccel();
  void RecvCheckAccel(t_server_check_accel* packet);
  void InitCrackMgr();
  void SetInitClock();
  bool IsHardwareBreakpointSet();
  bool IsTimeDiscrepancy();
  void count_magiclist_for_cheat_detection();

  bool IsTimeDiscrepancy2();

  bool IsDebugger();

  unsigned long AddCRC(const void* pSource, int size, int step);

  bool checkCrc(std::string sprite_Name, const char* buf, int size);

  bool LoadCrcDat();
 private:
  // --- Thread-Safe Packet Queue ---
  void PushPacketToQueue(const std::string& msg);
  void ProcessPacketQueue();
  std::mutex m_packetQueueMutex;
  std::vector<std::string> m_PacketQueue;

  // --- Anti-Cheat Threads ---
  void CrackCheckThread();
  void WatchdogThread();
  
  std::thread m_crackThread;
  std::thread m_watchdogThread;

  // --- Thread Synchronization & State ---
  std::atomic<bool> m_bIsCTRun;
  std::atomic<unsigned int> m_heartbeat;

  // --- Detection Methods ---
  bool IsDebuggerDetected();
  bool IsCheatToolRunning();
  bool IsFunctionHooked();

  //effect file crc check
  std::vector<datBin> datCrc;

  //anti cheat magic / combat skill effect
  std::map<int, int> magicEffectCount; // 用來儲存每個特效ID的數量
  std::map<int, int> maxEffectCount; // 用來儲存每個特效ID上限的數量

  // --- Original Member Variables ---
  t_server_check_accel RecvPacketSave;
  DWORD dwRecommandTime;
  DWORD dwRecvedTime;
  DWORD dwRealTime;
  __int64 m_i64Clock;
  DWORD m_dwThreadApplyGab;

  // --- Original Clock Functions ---
  __int64 GetAverageClock();
  void __stdcall GetNowClock(CLOCK* pClock);
  DWORD GetThreadApplyGab() { return m_dwThreadApplyGab; }
  void SetThreadApplyGab(const DWORD dwValue) { m_dwThreadApplyGab = dwValue; }
};

extern CCrackMgr CrackMgr;

#endif  // !defined(AFX_CRACKMGR_H__916F254F_DB1F_4269_BC2B_36BC59E48BC9__INCLUDED_)