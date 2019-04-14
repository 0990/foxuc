#ifndef LOG_HEAD_FILE
#define LOG_HEAD_FILE
#pragma once

#include "KernelEngineHead.h"

#define CONTENT_TITLE 32
#define FILE_LOG(format, ...) do \
{\
	CLog::GetInstance().Logf(""format"  file:"__FILE__", function:"__FUNCTION__",line: %05d", ##__VA_ARGS__, __LINE__); \
} while (0)\

#define LOG_WARN(format, ...) do \
{\
	CLog::GetInstance().warn(format, ##__VA_ARGS__); \
} while (0)\

#define LOG_PRINT(format, ...) do \
{\
	CLog::GetInstance().print(format, ##__VA_ARGS__); \
} while (0)\

class CLog
{
private:
	CLog(void);
	~CLog(void);
	CLog(const CLog&);
	const CLog& operator=(const CLog&);
private:
	FILE* m_pFile;
	SYSTEMTIME m_Time;
	CCriticalSection m_CriticalSection;
	char m_szContentTitle[CONTENT_TITLE];
private:
	FILE* GetFileFp();
public:
	static CLog& GetInstance();
	void Logf(const char* lpFormat, ...);
	void warn(LPCTSTR pszFormat, ...);
	void print(const char* lpFormat, ...);
};
//////////////////////////////////////////////////////////////////////////////////

#endif

