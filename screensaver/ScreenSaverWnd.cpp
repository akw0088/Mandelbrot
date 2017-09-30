#include <afxwin.h> 
#include "ScreenSaverWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CScreenSaverWnd, CWnd)

BEGIN_MESSAGE_MAP(CScreenSaverWnd, CWnd)
	//{{AFX_MSG_MAP(CScreenSaverWnd)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_PALETTECHANGED()
	ON_WM_QUERYNEWPALETTE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CScreenSaverWnd* CScreenSaverWnd::sm_pTheScreenSaver = NULL;

CScreenSaverWnd::CScreenSaverWnd()
{
	sm_pTheScreenSaver = this;
	m_bAutoBlack = TRUE;
	m_pPalette = NULL;
}

CScreenSaverWnd::~CScreenSaverWnd()
{
	sm_pTheScreenSaver = NULL;
}

int CScreenSaverWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	RestoreOptions();
	OnInitialUpdate();

	return 0;
}

void CScreenSaverWnd::OnInitialUpdate()
{
}

void CScreenSaverWnd::SaveOptions()
{
}

void CScreenSaverWnd::RestoreOptions()
{
}

BOOL CScreenSaverWnd::OnEraseBkgnd(CDC* pDC) 
{
	if (m_bAutoBlack)
	{
		CRect rcClient;
		GetClientRect(&rcClient);
		pDC->FillSolidRect(&rcClient, RGB(0, 0, 0));
	}
	return TRUE;
}

void CScreenSaverWnd::OnPaint() 
{
	CPaintDC dc(this);
	if (m_pPalette)
	{
		dc.SelectPalette(m_pPalette, FALSE);
		dc.RealizePalette();
	}

	OnDraw(&dc);
}

void CScreenSaverWnd::OnDraw(CDC* pDC)
{
}

CPalette* CScreenSaverWnd::GetPalette() const
{
	return m_pPalette;
}

CPalette* CScreenSaverWnd::SetPalette(CPalette* pPalette)
{
	CPalette* pOldPalette = m_pPalette;
	m_pPalette = pPalette;
	OnQueryNewPalette();
	return pOldPalette;
}

BOOL CScreenSaverWnd::OnQueryNewPalette() 
{
	if (!m_pPalette)
		return FALSE;

	CClientDC dc(this);
	dc.SelectPalette(m_pPalette, FALSE);
	UINT uChanged = dc.RealizePalette();

	return TRUE;
}

void CScreenSaverWnd::OnPaletteChanged(CWnd* pFocusWnd) 
{
	if (pFocusWnd == this)
		return;

	OnQueryNewPalette();
}

BOOL CScreenSaverWnd::IsAutoBlack() const
{
	return m_bAutoBlack;
}

void CScreenSaverWnd::SetAutoBlack(BOOL bAutoBlack /* = TRUE */)
{
	m_bAutoBlack = bAutoBlack;
}

LRESULT CScreenSaverWnd::WindowProc(UINT uMsg,
                                    WPARAM wParam,
                                    LPARAM lParam)
{
	return CWnd::WindowProc(uMsg, wParam, lParam);
}

LRESULT CScreenSaverWnd::DefWindowProc(UINT uMsg,
                                       WPARAM wParam,
                                       LPARAM lParam)
{
	return ::DefScreenSaverProc(m_hWnd, uMsg, wParam, lParam);
}

LRESULT WINAPI ScreenSaverProc(HWND hWnd, UINT uMsg,
                               WPARAM wParam, LPARAM lParam)
{
	ASSERT(CScreenSaverWnd::sm_pTheScreenSaver);
	if (!CScreenSaverWnd::sm_pTheScreenSaver)
		return 0L;

	if (!CScreenSaverWnd::sm_pTheScreenSaver->m_hWnd)
	{
		AfxWinInit((HINSTANCE)::GetWindowLong(hWnd, GWL_HINSTANCE),
		           NULL, "", SW_SHOWNORMAL);
		CScreenSaverWnd::sm_pTheScreenSaver->Attach(hWnd);
	}

	LRESULT lResult =
		::AfxCallWndProc(
			CScreenSaverWnd::sm_pTheScreenSaver,
			CScreenSaverWnd::sm_pTheScreenSaver->m_hWnd,
			uMsg, wParam, lParam);

	if (uMsg == WM_NCDESTROY)
	{
		ASSERT(!CScreenSaverWnd::sm_pTheScreenSaver ||
		       !CScreenSaverWnd::sm_pTheScreenSaver->m_hWnd);
		AfxWinTerm();
	}

	return lResult;
}
