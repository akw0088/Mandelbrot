#include <afxwin.h> 

#include "ScreenSaverWnd.h"
#include "ScreenSaverDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CScreenSaverDlg, CDialog)

BEGIN_MESSAGE_MAP(CScreenSaverDlg, CCmdTarget)
	//{{AFX_MSG_MAP(CScreenSaverDlg)
	ON_WM_NCDESTROY()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_COMMAND(IDOK, OnOK)
	ON_COMMAND(IDCANCEL, OnCancel)
	ON_MESSAGE(WM_INITDIALOG, HandleInitDialog)
END_MESSAGE_MAP()

CScreenSaverDlg* CScreenSaverDlg::sm_pTheConfigureDialog = NULL;

CScreenSaverDlg::CScreenSaverDlg()
{
	ASSERT(!sm_pTheConfigureDialog);
	sm_pTheConfigureDialog = this;
}

CScreenSaverDlg::~CScreenSaverDlg()
{
	sm_pTheConfigureDialog = NULL;
}


void CScreenSaverDlg::OnNcDestroy()
{
	CDialog::OnNcDestroy();
}

BOOL CScreenSaverDlg::OnInitDialog()
{
	if (AfxGetScreenSaverWnd())
		AfxGetScreenSaverWnd()->RestoreOptions();

	return CDialog::OnInitDialog();
}

int CScreenSaverDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
#ifdef _DEBUG
	SetWindowPos(&CWnd::wndNoTopMost,
		0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
#endif

	return 0;
}

LRESULT CScreenSaverDlg::WindowProc(UINT uMsg,
                                    WPARAM wParam,
                                    LPARAM lParam)
{
	return CDialog::WindowProc(uMsg, wParam, lParam);
}

LRESULT CScreenSaverDlg::DefWindowProc(UINT uMsg,
                                       WPARAM wParam,
                                       LPARAM lParam)
{
	return 0L;
}

BOOL WINAPI ScreenSaverConfigureDialog(HWND hDlg, UINT uMsg,
                                       WPARAM wParam, LPARAM lParam)
{
	ASSERT(AfxGetScreenSaverDialog());
	if (!AfxGetScreenSaverDialog())
		return 0L;

	if (!AfxGetScreenSaverDialog()->m_hWnd)
		AfxGetScreenSaverDialog()->Attach(hDlg);

	LRESULT lResult;
	if (uMsg == WM_INITDIALOG)
	{
		// special case for WM_INITDIALOG
		CDialog* pDlg =
			DYNAMIC_DOWNCAST(CDialog,
				CWnd::FromHandlePermanent(hDlg));
		if (pDlg != NULL)
			lResult = pDlg->OnInitDialog();
		else
			lResult = 1;
	}
	else
	{
		lResult =
			AfxCallWndProc(
				AfxGetScreenSaverDialog(),
				AfxGetScreenSaverDialog()->m_hWnd,
		        uMsg, wParam, lParam);
	}

	if (uMsg == WM_NCDESTROY)
	{
		ASSERT(!AfxGetScreenSaverDialog() ||
		       !AfxGetScreenSaverDialog()->m_hWnd);
	}

	return lResult;
}

BOOL WINAPI RegisterDialogClasses(HANDLE hInstance)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
