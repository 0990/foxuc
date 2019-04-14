#include "StdAfx.h"
#include "TraceService.h" // #include "EventService.h"

//////////////////////////////////////////////////////////////////////////

//��ɫ����
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

//׷�ٷ���
//////////////////////////////////////////////////////////////////////////
static ITraceService *g_pITraceService = NULL;
//��������
//////////////////////////////////////////////////////////////////////////
//���캯��
CTraceService::CTraceService()
{

}

//��������
//////////////////////////////////////////////////////////////////////////
//���÷���
bool CTraceService::SetTraceService(IUnknownEx * pIUnknownEx)
{
	g_pITraceService = QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx, ITraceService);
	return NULL != g_pITraceService;
}

//��ȡ����
VOID * CTraceService::GetTraceService(const IID & Guid, DWORD dwQueryVer)
{
	ASSERT(g_pITraceService);
	if (g_pITraceService) return NULL;

	return (VOID*)g_pITraceService->QueryInterface(Guid, dwQueryVer);
	return NULL;
}

//��ȡ����
VOID * CTraceService::GetTraceServiceManager(const IID & Guid, DWORD dwQueryVer)
{
	return NULL;
}

//״̬����
//////////////////////////////////////////////////////////////////////////
//׷��״̬
bool CTraceService::IsEnableTrace(enTraceLevel TraceLevel)
{
	return false;
}

//׷�ٿ���
bool CTraceService::EnableTrace(enTraceLevel TraceLevel, bool bEnableTrace)
{
	return false;
}

//���ܺ���
//////////////////////////////////////////////////////////////////////////
//׷����Ϣ
bool CTraceService::TraceString(LPCTSTR pszString, enTraceLevel TraceLevel)
{
	if (g_pITraceService)
		return g_pITraceService->TraceString(pszString, TraceLevel);

	return false;
}

//׷����Ϣ
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
//׷�ٷ���
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CTraceServiceControl, CRichEditCtrl)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(IDM_MENU0, OnCopyString)
	ON_COMMAND(IDM_MENU1, OnSelectAll)
	ON_COMMAND(IDM_MENU2, OnDeleteString)
	ON_COMMAND(IDM_MENU3, OnClearAll)
	ON_COMMAND(IDM_MENU4, OnSaveString)
END_MESSAGE_MAP()

//��������
//////////////////////////////////////////////////////////////////////////
//���캯��
CTraceServiceControl::CTraceServiceControl()
{
	CTraceService::SetTraceService((IUnknownEx*)QueryInterface(IID_IUnknownEx, VER_IUnknownEx));
}

//��������
CTraceServiceControl::~CTraceServiceControl()
{
	CTraceService::SetTraceService(NULL);
}

//�����ӿ�
//////////////////////////////////////////////////////////////////////////
//�ӿڲ�ѯ
VOID * CTraceServiceControl::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITraceService, Guid, dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITraceService, Guid, dwQueryVer);
	return NULL;
}

//��Ϣ�ӿ�
//////////////////////////////////////////////////////////////////////////
//׷����Ϣ
bool CTraceServiceControl::TraceString(LPCTSTR pszString, enTraceLevel TraceLevel)
{
	//Ч�����
	ITraceService *pITraceService = (ITraceService*)(QueryInterface(IID_ITraceService, VER_ITraceService));
	ASSERT(pITraceService != NULL);
	if (pITraceService == NULL) return false;

	//��������
	CHARFORMAT2 CharFormat;
	ZeroMemory(&CharFormat, sizeof(CharFormat));

	//��������
	CharFormat.cbSize = sizeof(CharFormat);
	CharFormat.dwMask = CFM_COLOR | CFM_BACKCOLOR;
	CTraceServiceControl *pRichEditTrace = static_cast<CTraceServiceControl*>(pITraceService);
	if (pRichEditTrace == NULL) return false;
	CharFormat.crTextColor = RGB(0, 0, 0);
	CharFormat.crBackColor = RGB(255, 255, 255);

	lstrcpyn(CharFormat.szFaceName, TEXT("����"), sizeof(CharFormat.szFaceName));
	//��ȡʱ��
	SYSTEMTIME SystemTime;
	TCHAR szTimeBuffer[512] = {0};
	GetLocalTime(&SystemTime);
	_sntprintf(szTimeBuffer, sizeof(szTimeBuffer), TEXT("�� %04d-%02d-%02d %02d:%02d:%02d ��"), SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour, SystemTime.wMinute, SystemTime.wSecond);

//	InsertString(szTimeBuffer, CharFormat);

	_sntprintf(szTimeBuffer, sizeof(szTimeBuffer), TEXT("%s%s\n"), szTimeBuffer, pszString);
	CharFormat.crTextColor = pRichEditTrace->GetTraceColor(TraceLevel);
	return InsertString(szTimeBuffer, CharFormat);
}

//���غ���
//////////////////////////////////////////////////////////////////////////
//�󶨺���
VOID CTraceServiceControl::PreSubclassWindow()
{
	__super::PreSubclassWindow();
	CTraceService::SetTraceService((IUnknownEx*)QueryInterface(IID_IUnknownEx, VER_IUnknownEx));
}

//���ܺ���
//////////////////////////////////////////////////////////////////////////
//������Ϣ
bool CTraceServiceControl::LoadMessage(LPCTSTR pszFileName)
{
	if (static_cast<CTraceServiceControl*>(g_pITraceService) == NULL) return false;
	return static_cast<CTraceServiceControl*>(g_pITraceService)->LoadMessage(pszFileName);
}

//������Ϣ
bool CTraceServiceControl::SaveMessage(LPCTSTR pszFileName)
{
	if (static_cast<CTraceServiceControl*>(g_pITraceService) == NULL) return false;
	return static_cast<CTraceServiceControl*>(g_pITraceService)->SaveMessage(pszFileName);
}

//���ò���
bool CTraceServiceControl::SetParameter(LONG lMaxLineCount, LONG lReserveLineCount)
{
	m_lMaxLineCount = lMaxLineCount;					//�������
	m_lReserveLineCount = lReserveLineCount;			//��������

	return true;
}

//��������
//////////////////////////////////////////////////////////////////////////
//���÷���
VOID CTraceServiceControl::InitializeService()
{

}

//��ȡ��ɫ
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

//�ַ��ж�
bool EfficacyUrlChar(TCHAR chChar)
{
	//�����ַ�
	if (chChar == TEXT('.')) return true;
	if (chChar == TEXT('=')) return true;
	if (chChar == TEXT('+')) return true;
	if (chChar == TEXT('?')) return true;
	if (chChar == TEXT('#')) return true;
	if (chChar == TEXT('%')) return true;
	if (chChar == TEXT('/')) return true;
	if (chChar == TEXT(':')) return true;
	if (chChar == TEXT('&')) return true;

	//�ַ���Χ
	if ((chChar >= TEXT('a')) && (chChar <= TEXT('z'))) return true;
	if ((chChar >= TEXT('A')) && (chChar <= TEXT('Z'))) return true;
	if ((chChar >= TEXT('0')) && (chChar <= TEXT('9'))) return true;

	return false;
}

//��ַ�ж�
bool EfficacyUrlString(LPCTSTR pszUrl)
{
	for (WORD i = 0; i < lstrlen(pszUrl); i++)
	{
		if (!EfficacyUrlChar(pszUrl[i])) return false;
	}

	return true;
}

//�����ִ�
bool CTraceServiceControl::InsertString(LPCTSTR pszString, CHARFORMAT2 & CharFormat)
{
	if (GetTextLength() >= 1024 * 512)
		OnClearAll();

	//������Ϣ
	//��������
	bool bResumeSelect;
	CHARRANGE CharRange;

	//����״̬
	SetSel(-1L, -1L);
	GetSel(CharRange.cpMin, CharRange.cpMax);
	bResumeSelect = (CharRange.cpMax != CharRange.cpMin);

	//��������
	LPCTSTR pszHttp = TEXT("http://");
	const INT nHttpLength = lstrlen(pszHttp);
	const INT nStringLength = lstrlen(pszString);

	//��������
	INT nStringStart = 0;
	INT nStringPause = 0;

	//�ַ�����
	for (INT i = 0; i < nStringLength; i++)
	{
		//��������
		INT nUrlPause = i;

		//��ַ�ж�
		if (((i + nHttpLength) < nStringLength) && (memcmp(&pszString[i], pszHttp, nHttpLength*sizeof(TCHAR)) == 0))
		{
			//��������
			nUrlPause = i + nHttpLength;

			//��ַ����
			while (nUrlPause < nStringLength)
			{
				//�ַ��ж�
				if (EfficacyUrlChar(pszString[nUrlPause]) == true)
				{
					nUrlPause++;
					continue;
				}

				break;
			}
		}

		//��ֹ�ַ�
		if (nUrlPause <= (i + nHttpLength)) nStringPause = (i + 1);

		//�����ַ�
		if ((i == (nStringLength - 1)) || (nUrlPause > (i + nHttpLength)))
		{
			//��ͨ�ַ�
			if (nStringPause > nStringStart)
			{
				//��ȡ����
				CString strNormalString;
				LPTSTR pszNormalString = strNormalString.GetBuffer(nStringPause - nStringStart + 1);

				//�����ַ�
				pszNormalString[nStringPause-nStringStart] = 0;
				CopyMemory(pszNormalString, &pszString[nStringStart], (nStringPause - nStringStart)*sizeof(TCHAR));

				//�ͷŻ���
				strNormalString.ReleaseBuffer();

				//������Ϣ
				SetSel(-1L, -1L);
				SetWordCharFormat(CharFormat);
				ReplaceSel((LPCTSTR)strNormalString);
			}

			//���ӵ�ַ
			if (nUrlPause > (i + nHttpLength))
			{
				//��ȡ����
				CString strUrlString;
				LPTSTR pszUrlString = strUrlString.GetBuffer((nUrlPause - i) + 1);

				//�����ַ�
				pszUrlString[nUrlPause-i] = 0;
				CopyMemory(pszUrlString, &pszString[i], (nUrlPause - i)*sizeof(TCHAR));

				//�ͷŻ���
				strUrlString.ReleaseBuffer();

				//�����ʽ
				CHARFORMAT2 CharFormatHyper;
				CharFormatHyper = CharFormat;

				//Ч���ַ
				if (EfficacyUrlString(strUrlString) == true)
				{
					CharFormatHyper.dwMask |= CFM_LINK;
					CharFormatHyper.dwEffects |= CFE_LINK;
				}

				//������Ϣ
				SetSel(-1L, -1L);
				SetWordCharFormat(CharFormatHyper);
				ReplaceSel((LPCTSTR)strUrlString);
			}

			//��������
			nStringStart = __max(i, nUrlPause);
		}

		//��������
		i += (nUrlPause - i);
	}

	//״̬����
	if (bResumeSelect == true) SetSel(CharRange);
	else PostMessage(WM_VSCROLL, SB_BOTTOM, 0);

	return true;
}

//�ص�����
//////////////////////////////////////////////////////////////////////////
//���ػص�
DWORD CALLBACK CTraceServiceControl::LoadCallBack(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG * pcb)
{
	return 0;
}

//����ص�
DWORD CALLBACK CTraceServiceControl::SaveCallBack(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG * pcb)
{
	return 0;
}

//�˵�����
//////////////////////////////////////////////////////////////////////////
//ɾ����Ϣ
VOID CTraceServiceControl::OnClearAll()
{
	SetWindowText(NULL);
}

//ȫ��ѡ��
VOID CTraceServiceControl::OnSelectAll()
{
	__super::SetSel(0, __super::GetTextLength());
}

//�����ַ�
VOID CTraceServiceControl::OnCopyString()
{
	__super::Copy();
}

//������Ϣ
VOID CTraceServiceControl::OnSaveString()
{
	CFileDialog dlg(FALSE, TEXT("*.txt"), NULL, 4 | 2, TEXT("��Ϣ|*.txt||"));
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

//ɾ���ַ�
VOID CTraceServiceControl::OnDeleteString()
{
	ReplaceSel(NULL, true);
}

//��Ϣӳ��
//////////////////////////////////////////////////////////////////////////
//������Ϣ
INT CTraceServiceControl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	return __super::OnCreate(lpCreateStruct);
}

//�Ҽ���Ϣ
VOID CTraceServiceControl::OnRButtonDown(UINT nFlags, CPoint point)
{
	CMenu menu;
	ClientToScreen(&point);

	menu.CreatePopupMenu();

	CHARRANGE sl;
	GetSel(sl);
	menu.AppendMenu(MF_STRING | (sl.cpMax != sl.cpMin) ? 0 : MF_DISABLED | MF_GRAYED, IDM_MENU0, TEXT("����(&C)\tCtrl+C"));
	menu.AppendMenu(MF_STRING | (GetTextLength() > 0 && sl.cpMax - sl.cpMin < GetTextLength()) ? 0 : MF_DISABLED | MF_GRAYED, IDM_MENU1, TEXT("ȫѡ(&A)\tCtrl+A"));
	menu.AppendMenu(MF_STRING | (false) ? 0 : MF_DISABLED | MF_GRAYED, IDM_MENU2, TEXT("ɾ��(&D)"));
	menu.AppendMenu(MF_STRING | (GetTextLength() > 0) ? 0 : MF_DISABLED | MF_GRAYED, IDM_MENU3, TEXT("�����Ϣ"));
	menu.AppendMenu(MF_SEPARATOR, 0);
	menu.AppendMenu(MF_STRING | (GetTextLength() > 0) ? 0 : MF_DISABLED | MF_GRAYED, IDM_MENU4, TEXT("������Ϣ..."));

	TrackPopupMenu(menu.m_hMenu, nFlags, point.x, point.y, 0, m_hWnd, NULL);
}

//׷����Ϣ
LRESULT CTraceServiceControl::OnTraceServiceMessage(WPARAM wParam, LPARAM lParam)
{
	return 0;
}
