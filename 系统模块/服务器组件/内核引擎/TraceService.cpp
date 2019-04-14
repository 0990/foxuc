#include "StdAfx.h"
#include "TraceService.h" // #include "EventService.h"

//////////////////////////////////////////////////////////////////////////

//颜色定义
#define COLOR_TIME						RGB(0,0,0)
#define COLOR_NORMAL					RGB(125,125,125)
#define COLOR_WARN						RGB(255,128,0)
#define COLOR_EXCEPTION					RGB(200,0,0)
#define COLOR_DEBUG						RGB(0,128,128)

#define EVENT_LEVEL_COUNT				5

#define IDM_MENU0						100
#define IDM_MENU1						110
#define IDM_MENU2						120
#define IDM_MENU3						130
#define IDM_MENU4						140
#define IDM_MENU5						150

//////////////////////////////////////////////////////////////////////////

//追踪服务
//////////////////////////////////////////////////////////////////////////
static ITraceService *g_pITraceService = NULL;
//函数定义
//////////////////////////////////////////////////////////////////////////
//构造函数
CTraceService::CTraceService()
{

}

//服务配置
//////////////////////////////////////////////////////////////////////////
//设置服务
bool CTraceService::SetTraceService(IUnknownEx * pIUnknownEx)
{
	g_pITraceService = QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx, ITraceService);
	return NULL != g_pITraceService;
}

//获取服务
VOID * CTraceService::GetTraceService(const IID & Guid, DWORD dwQueryVer)
{
	ASSERT(g_pITraceService);
	if (g_pITraceService) return NULL;

	return (VOID*)g_pITraceService->QueryInterface(Guid, dwQueryVer);
	return NULL;
}

//获取服务
VOID * CTraceService::GetTraceServiceManager(const IID & Guid, DWORD dwQueryVer)
{
	return NULL;
}

//状态管理
//////////////////////////////////////////////////////////////////////////
//追踪状态
bool CTraceService::IsEnableTrace(enTraceLevel TraceLevel)
{
	return false;
}

//追踪控制
bool CTraceService::EnableTrace(enTraceLevel TraceLevel, bool bEnableTrace)
{
	return false;
}

//功能函数
//////////////////////////////////////////////////////////////////////////
//追踪信息
bool CTraceService::TraceString(LPCTSTR pszString, enTraceLevel TraceLevel)
{
	if (g_pITraceService)
		return g_pITraceService->TraceString(pszString, TraceLevel);

	return false;
}

//追踪信息
bool CTraceService::TraceStringEx(enTraceLevel TraceLevel, LPCTSTR pszFormat, ...)
{
	TCHAR szBuffer[1024] = {0};

	va_list arglist;
	va_start(arglist, pszFormat);
	_vstprintf_s(szBuffer, CountArray(szBuffer), pszFormat, arglist);
	va_end(arglist);

	if (g_pITraceService)
		return g_pITraceService->TraceString(szBuffer, TraceLevel);

	return false;
}

//////////////////////////////////////////////////////////////////////////
//追踪服务
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CTraceServiceControl, CRichEditCtrl)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(IDM_MENU0, OnCopyString)
	ON_COMMAND(IDM_MENU1, OnSelectAll)
	ON_COMMAND(IDM_MENU2, OnDeleteString)
	ON_COMMAND(IDM_MENU3, OnClearAll)
	ON_COMMAND(IDM_MENU4, OnSaveString)
END_MESSAGE_MAP()

//函数定义
//////////////////////////////////////////////////////////////////////////
//构造函数
CTraceServiceControl::CTraceServiceControl()
{
	CTraceService::SetTraceService((IUnknownEx*)QueryInterface(IID_IUnknownEx, VER_IUnknownEx));
}

//析构函数
CTraceServiceControl::~CTraceServiceControl()
{
	CTraceService::SetTraceService(NULL);
}

//基础接口
//////////////////////////////////////////////////////////////////////////
//接口查询
VOID * CTraceServiceControl::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITraceService, Guid, dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITraceService, Guid, dwQueryVer);
	return NULL;
}

//信息接口
//////////////////////////////////////////////////////////////////////////
//追踪信息
bool CTraceServiceControl::TraceString(LPCTSTR pszString, enTraceLevel TraceLevel)
{
	//效验参数
	ITraceService *pITraceService = (ITraceService*)(QueryInterface(IID_ITraceService, VER_ITraceService));
	ASSERT(pITraceService != NULL);
	if (pITraceService == NULL) return false;

	//变量定义
	CHARFORMAT2 CharFormat;
	ZeroMemory(&CharFormat, sizeof(CharFormat));

	//构造数据
	CharFormat.cbSize = sizeof(CharFormat);
	CharFormat.dwMask = CFM_COLOR | CFM_BACKCOLOR;
	CTraceServiceControl *pRichEditTrace = static_cast<CTraceServiceControl*>(pITraceService);
	if (pRichEditTrace == NULL) return false;
	CharFormat.crTextColor = RGB(0, 0, 0);
	CharFormat.crBackColor = RGB(255, 255, 255);

	lstrcpyn(CharFormat.szFaceName, TEXT("宋体"), sizeof(CharFormat.szFaceName));
	//获取时间
	SYSTEMTIME SystemTime;
	TCHAR szTimeBuffer[512] = {0};
	GetLocalTime(&SystemTime);
	_sntprintf(szTimeBuffer, sizeof(szTimeBuffer), TEXT("【 %04d-%02d-%02d %02d:%02d:%02d 】"), SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour, SystemTime.wMinute, SystemTime.wSecond);

//	InsertString(szTimeBuffer, CharFormat);

	_sntprintf(szTimeBuffer, sizeof(szTimeBuffer), TEXT("%s%s\n"), szTimeBuffer, pszString);
	CharFormat.crTextColor = pRichEditTrace->GetTraceColor(TraceLevel);
	return InsertString(szTimeBuffer, CharFormat);
}

//重载函数
//////////////////////////////////////////////////////////////////////////
//绑定函数
VOID CTraceServiceControl::PreSubclassWindow()
{
	__super::PreSubclassWindow();
	CTraceService::SetTraceService((IUnknownEx*)QueryInterface(IID_IUnknownEx, VER_IUnknownEx));
}

//功能函数
//////////////////////////////////////////////////////////////////////////
//加载消息
bool CTraceServiceControl::LoadMessage(LPCTSTR pszFileName)
{
	if (static_cast<CTraceServiceControl*>(g_pITraceService) == NULL) return false;
	return static_cast<CTraceServiceControl*>(g_pITraceService)->LoadMessage(pszFileName);
}

//保存信息
bool CTraceServiceControl::SaveMessage(LPCTSTR pszFileName)
{
	if (static_cast<CTraceServiceControl*>(g_pITraceService) == NULL) return false;
	return static_cast<CTraceServiceControl*>(g_pITraceService)->SaveMessage(pszFileName);
}

//设置参数
bool CTraceServiceControl::SetParameter(LONG lMaxLineCount, LONG lReserveLineCount)
{
	m_lMaxLineCount = lMaxLineCount;					//最大行数
	m_lReserveLineCount = lReserveLineCount;			//保留行数

	return true;
}

//辅助函数
//////////////////////////////////////////////////////////////////////////
//配置服务
VOID CTraceServiceControl::InitializeService()
{

}

//获取颜色
COLORREF CTraceServiceControl::GetTraceColor(enTraceLevel TraceLevel)
{
	switch (TraceLevel)
	{
		case TraceLevel_Info:
			return RGB(133, 124, 129);
			break;
		case TraceLevel_Normal:
			return RGB(133, 124, 129);
			break;
		case TraceLevel_Warning:
			return RGB(255, 0, 0);
			break;
		case TraceLevel_Exception:
			return RGB(255, 0, 0);
			break;
		case TraceLevel_Debug:
			return RGB(19, 127, 140);
			break;
	}

	return RGB(0, 0, 0);
}

//字符判断
bool EfficacyUrlChar(TCHAR chChar)
{
	//特殊字符
	if (chChar == TEXT('.')) return true;
	if (chChar == TEXT('=')) return true;
	if (chChar == TEXT('+')) return true;
	if (chChar == TEXT('?')) return true;
	if (chChar == TEXT('#')) return true;
	if (chChar == TEXT('%')) return true;
	if (chChar == TEXT('/')) return true;
	if (chChar == TEXT(':')) return true;
	if (chChar == TEXT('&')) return true;

	//字符范围
	if ((chChar >= TEXT('a')) && (chChar <= TEXT('z'))) return true;
	if ((chChar >= TEXT('A')) && (chChar <= TEXT('Z'))) return true;
	if ((chChar >= TEXT('0')) && (chChar <= TEXT('9'))) return true;

	return false;
}

//地址判断
bool EfficacyUrlString(LPCTSTR pszUrl)
{
	for (WORD i = 0; i < lstrlen(pszUrl); i++)
	{
		if (!EfficacyUrlChar(pszUrl[i])) return false;
	}

	return true;
}

//插入字串
bool CTraceServiceControl::InsertString(LPCTSTR pszString, CHARFORMAT2 & CharFormat)
{
	if (GetTextLength() >= 1024 * 512)
		OnClearAll();

	//插入消息
	//变量定义
	bool bResumeSelect;
	CHARRANGE CharRange;

	//保存状态
	SetSel(-1L, -1L);
	GetSel(CharRange.cpMin, CharRange.cpMax);
	bResumeSelect = (CharRange.cpMax != CharRange.cpMin);

	//搜索变量
	LPCTSTR pszHttp = TEXT("http://");
	const INT nHttpLength = lstrlen(pszHttp);
	const INT nStringLength = lstrlen(pszString);

	//索引定义
	INT nStringStart = 0;
	INT nStringPause = 0;

	//字符解释
	for (INT i = 0; i < nStringLength; i++)
	{
		//变量定义
		INT nUrlPause = i;

		//地址判断
		if (((i + nHttpLength) < nStringLength) && (memcmp(&pszString[i], pszHttp, nHttpLength*sizeof(TCHAR)) == 0))
		{
			//设置索引
			nUrlPause = i + nHttpLength;

			//地址搜索
			while (nUrlPause < nStringLength)
			{
				//字符判断
				if (EfficacyUrlChar(pszString[nUrlPause]) == true)
				{
					nUrlPause++;
					continue;
				}

				break;
			}
		}

		//终止字符
		if (nUrlPause <= (i + nHttpLength)) nStringPause = (i + 1);

		//插入字符
		if ((i == (nStringLength - 1)) || (nUrlPause > (i + nHttpLength)))
		{
			//普通字符
			if (nStringPause > nStringStart)
			{
				//获取缓冲
				CString strNormalString;
				LPTSTR pszNormalString = strNormalString.GetBuffer(nStringPause - nStringStart + 1);

				//拷贝字符
				pszNormalString[nStringPause-nStringStart] = 0;
				CopyMemory(pszNormalString, &pszString[nStringStart], (nStringPause - nStringStart)*sizeof(TCHAR));

				//释放缓冲
				strNormalString.ReleaseBuffer();

				//插入消息
				SetSel(-1L, -1L);
				SetWordCharFormat(CharFormat);
				ReplaceSel((LPCTSTR)strNormalString);
			}

			//连接地址
			if (nUrlPause > (i + nHttpLength))
			{
				//获取缓冲
				CString strUrlString;
				LPTSTR pszUrlString = strUrlString.GetBuffer((nUrlPause - i) + 1);

				//拷贝字符
				pszUrlString[nUrlPause-i] = 0;
				CopyMemory(pszUrlString, &pszString[i], (nUrlPause - i)*sizeof(TCHAR));

				//释放缓冲
				strUrlString.ReleaseBuffer();

				//构造格式
				CHARFORMAT2 CharFormatHyper;
				CharFormatHyper = CharFormat;

				//效验地址
				if (EfficacyUrlString(strUrlString) == true)
				{
					CharFormatHyper.dwMask |= CFM_LINK;
					CharFormatHyper.dwEffects |= CFE_LINK;
				}

				//插入消息
				SetSel(-1L, -1L);
				SetWordCharFormat(CharFormatHyper);
				ReplaceSel((LPCTSTR)strUrlString);
			}

			//设置索引
			nStringStart = __max(i, nUrlPause);
		}

		//设置索引
		i += (nUrlPause - i);
	}

	//状态设置
	if (bResumeSelect == true) SetSel(CharRange);
	else PostMessage(WM_VSCROLL, SB_BOTTOM, 0);

	return true;
}

//回调函数
//////////////////////////////////////////////////////////////////////////
//加载回调
DWORD CALLBACK CTraceServiceControl::LoadCallBack(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG * pcb)
{
	return 0;
}

//保存回调
DWORD CALLBACK CTraceServiceControl::SaveCallBack(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG * pcb)
{
	return 0;
}

//菜单命令
//////////////////////////////////////////////////////////////////////////
//删除信息
VOID CTraceServiceControl::OnClearAll()
{
	SetWindowText(NULL);
}

//全部选择
VOID CTraceServiceControl::OnSelectAll()
{
	__super::SetSel(0, __super::GetTextLength());
}

//拷贝字符
VOID CTraceServiceControl::OnCopyString()
{
	__super::Copy();
}

//保存信息
VOID CTraceServiceControl::OnSaveString()
{
	CFileDialog dlg(FALSE, TEXT("*.txt"), NULL, 4 | 2, TEXT("信息|*.txt||"));
	if (dlg.DoModal() == IDOK)
	{
		CString m_filename = dlg.GetPathName();
		CString strInput;
		GetWindowText(strInput);

		CFile file(m_filename, CFile::modeCreate | CFile::modeWrite);
		file.Write(strInput, strInput.GetLength());
		file.Close();
	}
}

//删除字符
VOID CTraceServiceControl::OnDeleteString()
{
	ReplaceSel(NULL, true);
}

//消息映射
//////////////////////////////////////////////////////////////////////////
//建立消息
INT CTraceServiceControl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	return __super::OnCreate(lpCreateStruct);
}

//右键消息
VOID CTraceServiceControl::OnRButtonDown(UINT nFlags, CPoint point)
{
	CMenu menu;
	ClientToScreen(&point);

	menu.CreatePopupMenu();

	CHARRANGE sl;
	GetSel(sl);
	menu.AppendMenu(MF_STRING | (sl.cpMax != sl.cpMin) ? 0 : MF_DISABLED | MF_GRAYED, IDM_MENU0, TEXT("复制(&C)\tCtrl+C"));
	menu.AppendMenu(MF_STRING | (GetTextLength() > 0 && sl.cpMax - sl.cpMin < GetTextLength()) ? 0 : MF_DISABLED | MF_GRAYED, IDM_MENU1, TEXT("全选(&A)\tCtrl+A"));
	menu.AppendMenu(MF_STRING | (false) ? 0 : MF_DISABLED | MF_GRAYED, IDM_MENU2, TEXT("删除(&D)"));
	menu.AppendMenu(MF_STRING | (GetTextLength() > 0) ? 0 : MF_DISABLED | MF_GRAYED, IDM_MENU3, TEXT("清除信息"));
	menu.AppendMenu(MF_SEPARATOR, 0);
	menu.AppendMenu(MF_STRING | (GetTextLength() > 0) ? 0 : MF_DISABLED | MF_GRAYED, IDM_MENU4, TEXT("保存信息..."));

	TrackPopupMenu(menu.m_hMenu, nFlags, point.x, point.y, 0, m_hWnd, NULL);
}

//追踪消息
LRESULT CTraceServiceControl::OnTraceServiceMessage(WPARAM wParam, LPARAM lParam)
{
	return 0;
}
