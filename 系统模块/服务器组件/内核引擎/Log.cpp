#include "stdafx.h"
#include "Log.h"
#include "TraceService.h"
#include<stdarg.h>

CLog::CLog() {
    m_pFile = NULL;
}


CLog::~CLog() {
}

CLog& CLog::GetInstance() {
    static CLog log;
    return log;
}

void CLog::Logf(const char* lpFormat, ...) {
    CWHDataLocker lock(m_CriticalSection);
    try {
        GetFileFp();
        //ASSERT(m_pFile!=NULL);
        if (m_pFile == NULL) return;
        DWORD dwWrite = 0;
        fwrite(m_szContentTitle, sizeof(char), strlen(m_szContentTitle), m_pFile);

        va_list args;
        va_start(args, lpFormat);
        vfprintf(m_pFile, lpFormat, args);
        va_end(args);
        fwrite("\n\r", sizeof(char), 1, m_pFile);
        return;
    } catch (...) {}
}
void CLog::warn(LPCTSTR pszFormat, ...) {
    CWHDataLocker lock(m_CriticalSection);
    TCHAR szBuffer[1024] = { 0 };
    va_list arglist;
    va_start(arglist, pszFormat);
    _vstprintf_s(szBuffer, CountArray(szBuffer), pszFormat, arglist);
    va_end(arglist);
    CTraceService::TraceString(szBuffer, TraceLevel_Warning);
}
void CLog::print(const char* lpFormat, ...) {
	CWHDataLocker lock(m_CriticalSection);
	char szBuffer[2048] = { 0 };
	va_list args;
	va_start(args, lpFormat);
	vsprintf(szBuffer, lpFormat, args);
	va_end(args);
	printf(szBuffer);
	printf("\n");
}
FILE* CLog::GetFileFp() {
    SYSTEMTIME sys;
    GetLocalTime(&sys);
    sprintf_s(m_szContentTitle, CONTENT_TITLE, "%02d:%02d:%02d %02d:%02d:%02d:%d->", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);
    if (sys.wDay == m_Time.wDay) {
        return m_pFile;
    }
    if (m_pFile != NULL) {
        fclose(m_pFile);
    }
    m_Time = sys;
    char szFileName[32] = { 0 };
    sprintf_s(szFileName, sizeof(szFileName), "log\\%02d-%02d-%02d.txt", m_Time.wYear, m_Time.wMonth, m_Time.wDay);
    errno_t err;
    if ((err = fopen_s(&m_pFile, szFileName, "a+")) != 0) {
        //´ò¿ªÊ§°Ü
        m_pFile = NULL;
    }
    return m_pFile;
}
