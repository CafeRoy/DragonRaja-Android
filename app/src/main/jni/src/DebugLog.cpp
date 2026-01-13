// Debug.cpp: implementation of the CDebugLog class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "DebugLog.h"
#include <assert.h>
#include <time.h>
#include <filesystem> // 必須引入這個標頭檔
CDebugLog g_DBGLog;
CDebugLog g_CrashLog;
#ifdef _DEBUG
	#undef THIS_FILE
	static char THIS_FILE[]=__FILE__;
	#define new DEBUG_NEW
#endif

CDebugLog::CDebugLog()//初始化成員
{
	m_fp = NULL;
	m_hConsole = 0;

	m_iLogYear = 0;
	m_iLogMon = 0;
	m_iLogDay = 0;
	Clear();
}

CDebugLog::~CDebugLog()
{
	Clear();
	CloseConsole();

	if (m_hLogFileMutex != NULL)
	{
		//::CloseHandle(m_hLogFileMutex);
		m_hLogFileMutex = NULL;
	}
}

bool CDebugLog::Init(const eSZDTOT OutputMode, const char* szIdentifyName)
{
	switch(OutputMode)
	{
	case OUTPUT_NOT_SET:
		{
			assert(0);
			break;
		}//OUTPUT_NOT_SET:
	default:
		{	
			m_iMode = OutputMode;
			break;
		}
	}//switch(OutputMode)

	if(m_iMode & OUTPUT_JUST_CONSOLE)
	{
		OpenConsole();
	}

	::strcpy(m_szIdentify,szIdentifyName);


	// 1. 構建唯一的 Mutex 名稱
	char szMutexName[MAX_PATH];
	sprintf(szMutexName, "%s_%s", LOG_MUTEX_NAME, szIdentifyName); // 使用IdentifyName確保不同日誌有不同的鎖

	// 2. 創建或打開 Mutex
	// CreateMutexW/A: 如果 Mutex 存在則打開它，如果不存在則創建它
	/*m_hLogFileMutex = ::CreateMutex(
		NULL,       // 預設安全屬性
		FALSE,      // 初始不被擁有
		szMutexName // Mutex 名稱
	);

	if (m_hLogFileMutex == NULL)
	{
		// 處理錯誤，可能無法創建 Mutex
		assert(0);
		return false;
	}*/

	return true;
}

void CDebugLog::Clear()
{
	m_iMode		= OUTPUT_NOT_SET;
	::strcpy(m_szIdentify,"");
	SAFE_FCLOSE(m_fp);//���� Ŭ����
}

void CDebugLog::SetTime()//�ð� ������
{
	time_t nowTime;//�ð� �ޱ�
	::time( &nowTime );
	struct tm *now = ::localtime(&nowTime);

	m_iYear = now->tm_year+1900;
	m_iMon  = now->tm_mon+1;
	m_iDay  = now->tm_mday;
	m_iHour = now->tm_hour;
	m_iMin  = now->tm_min;
	m_iSec  = now->tm_sec;
}

void CDebugLog::SetWriteFile()//���� ������
{
	/*if (m_iLogDay != m_iDay
		|| m_iLogMon != m_iMon
		|| m_iLogYear != m_iYear
		|| !m_fp)
	{
		m_iLogDay = m_iDay, m_iLogMon = m_iMon, m_iLogYear = m_iYear; // 更新已記錄的日期
		SAFE_FCLOSE(m_fp);//關閉舊檔案

        std::__fs::filesystem::create_directory(szLogFolder);
		char szFileName[MAX_PATH + 500] = { 0, };
		::sprintf(szFileName, "%s/%d-%d-%d %s.log", szLogFolder, m_iLogYear, m_iLogMon, m_iLogDay, m_szIdentify);
		m_fp = ::fopen(szFileName, "at");//開啟新檔案
		if (!m_fp)
		{
			assert(0);
		}
	}*/
}

void CDebugLog::Log(const eSZDTLT LogLv, const char *szLogMsg, ...)
{
    return;

	// 設定一個超時時間 (例如 500 毫秒)。
	// 避免在 UI 執行緒中因 Mutex 被長時間佔用而導致無限期阻塞。
	const DWORD LOG_TIMEOUT_MS = 500;

	// 步驟 1: 嘗試獲取跨進程互斥鎖
	//DWORD dwWaitResult = ::WaitForSingleObject(m_hLogFileMutex, LOG_TIMEOUT_MS);

	// 檢查等待結果。如果成功獲得鎖 (WAIT_OBJECT_0)，則繼續。
	//if (dwWaitResult == WAIT_OBJECT_0)
	{
		// *** 關鍵操作開始 ***
		SetTime();
		SetWriteFile();

		//char szTemp[MAX_ERROR_MSG_SIZE] = { 0, };
		char szResultMsg[MAX_ERROR_MSG_SIZE + 20];

        char szBuffer[1024]; // 確保緩衝區夠大

        va_list arg;
        va_start(arg, szLogMsg);
        // 移除 if (arg == NULL) 判斷
        // 改用安全的 vsnprintf
        vsnprintf(szBuffer, sizeof(szBuffer), szLogMsg, arg);
        va_end(arg);

        szBuffer[sizeof(szBuffer) - 1] = '\0'; // 確保結尾

		::sprintf(szResultMsg, "<LogLv%d(%d:%d:%d)> %s", LogLv, m_iHour, m_iMin, m_iSec, szBuffer);

		if (m_iMode & OUTPUT_JUST_FILE)
		{
			OutFile(szResultMsg);
		}
		if (m_iMode & OUTPUT_JUST_TRACE)
		{
			OutTrace(szResultMsg);
		}
		if (m_iMode & OUTPUT_JUST_CONSOLE)
		{
			OutConsole(szResultMsg);
		}
		// *** 關鍵操作結束 ***

		// 步驟 2: 釋放跨進程互斥鎖
		//::ReleaseMutex(m_hLogFileMutex);
	}
	//else
	{
		// dwWaitResult 是 WAIT_TIMEOUT 或 WAIT_ABANDONED。
		// 為了確保 UI 響應，我們直接跳過這次日誌寫入。
		// 在日誌系統中，丟失一條日誌通常比凍結整個應用程式更好。
		return;
	}
}

void CDebugLog::OutFile(const char* szLogMsg)
{
	// 注意：OutFile 必須在 Log() 內部的鎖定範圍內調用，
	// 否則這裡會出現競爭條件。
	if (m_fp)
	{
		::fprintf(m_fp, "%s\n", szLogMsg);
		::fflush(m_fp); // 這一行是關鍵，確保內容立即寫入磁碟
	}
}

void CDebugLog::OutTrace(const char* szLogMsg)
{
	//::OutputDebugString(szLogMsg);
}

void CDebugLog::OutConsole(const char* szLogMsg)
{
	//DWORD dwBytes = 0;
	//::WriteFile(m_hConsole, szLogMsg, ::strlen(szLogMsg), &dwBytes, NULL);
	//::WriteFile(m_hConsole, "\n", ::strlen("\n"), &dwBytes, NULL);
}

bool CDebugLog::OpenConsole()
{
	/*if(m_iMode & OUTPUT_JUST_CONSOLE)
	{
		if(!::AllocConsole()){return false;}
		m_hConsole = ::GetStdHandle(STD_OUTPUT_HANDLE);
		if(INVALID_HANDLE_VALUE == m_hConsole) 
		{
			::FreeConsole();
			return false;
		}
		return true;
	}*/
	return false;
}

void CDebugLog::CloseConsole()
{
	/*if(m_iMode & OUTPUT_JUST_CONSOLE)
	{
		::FreeConsole();
		::CloseHandle(m_hConsole);
		m_hConsole	= INVALID_HANDLE_VALUE;
	}*/
}