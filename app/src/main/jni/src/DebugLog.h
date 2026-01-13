// Debug.h: interface for the CDebugLog class.
//
//////////////////////////////////////////////////////////////////////
#pragma once
#if !defined(AFX_DEBUG_H__78933F14_CBDC_417D_B700_95257702E393__INCLUDED_)
#define AFX_DEBUG_H__78933F14_CBDC_417D_B700_95257702E393__INCLUDED_

#define MAX_PATH 260
#include "StdAfx.h"
/*--------------------------------------------------------------------
					SynCzone open source project 
						Debug-Tracer v0.1.0

					Prototype By NetSerenity.
	Modify By HwoarangSangwoo(E-Mail: hwoarang_sangwoo@hotmail.com)
----------------------------------------------------------------------*/
namespace DebugLog
{
	enum eSZDTOT//SyncZone Debug Tracer Ouptput Types
	{
		OUTPUT_NOT_SET			=	0x00000000,
		OUTPUT_JUST_FILE		=	0x00000001,
		OUTPUT_JUST_TRACE		=	0x00000002,
		OUTPUT_JUST_CONSOLE		=	0x00000004,
		OUTPUT_FILE_AND_TRACE	=	OUTPUT_JUST_FILE|OUTPUT_JUST_TRACE,
		OUTPUT_FILE_AND_CONSOLE	=	OUTPUT_JUST_FILE|OUTPUT_JUST_CONSOLE,
		OUTPUT_ALL				=	OUTPUT_JUST_FILE|OUTPUT_JUST_TRACE|OUTPUT_JUST_CONSOLE,	
	};

	enum eSZDTLT//SyncZone Debug Tracer Log Types
	{
		LOG_NORMAL	= 0,
		LOG_LV1		= 1,
		LOG_LV2		= 2,
		LOG_LV3		= 3,
		LOG_LV4		= 4,
		LOG_LV5		= 5,
		LOG_LV6		= 6,
		LOG_LV7		= 7,
		LOG_LV8		= 8,
		LOG_LV9		= 9,
		LOG_LV10	= 10,
	};

	const int MAX_ERROR_MSG_SIZE = 1024;
	const char szLogFolder[] = "./CustomerService";
#define LOG_MUTEX_NAME "Global_Unique_LogFile_Mutex_Name_For_Identify"

	class CDebugLog
	{
	public:
		CDebugLog();
		virtual ~CDebugLog();

	public:
		bool Init(const eSZDTOT OutputMode, const char* strIdentifyName);
		void Log(const eSZDTLT LogLv, const char* szLogMsg, ...);

	private:
		void Clear();
		void SetTime();
		void SetWriteFile();

		void OutFile(const char* szLogMsg);
		void OutTrace(const char* szLogMsg);
		void OutConsole(const char* szLogMsg);

		bool OpenConsole();
		void CloseConsole();

	private:
		// === 數據成員 ===
		__int32	m_iMode;
		char 	m_szIdentify[MAX_PATH];
		FILE* m_fp;

		HANDLE	m_hConsole;

		// 當前寫日誌時的時間戳
		__int32 m_iYear, m_iMon, m_iDay;
		__int32 m_iHour, m_iMin, m_iSec;

		// 追蹤當前已開啟的日誌文件的日期 (防止重複開啟)
		__int32 m_iLogYear, m_iLogMon, m_iLogDay;

		// 跨進程/執行緒同步 Mutex
		HANDLE m_hLogFileMutex;

	private:
		// === 禁用拷貝和賦值 (使用 C++11 = delete) ===
		CDebugLog(const CDebugLog& old) = delete;
		CDebugLog& operator=(const CDebugLog& old) = delete;
	};

}
using namespace DebugLog;

extern CDebugLog g_CrashLog;
extern CDebugLog g_DBGLog;
#endif // !defined(AFX_DEBUG_H__78933F14_CBDC_417D_B700_95257702E393__INCLUDED_)
