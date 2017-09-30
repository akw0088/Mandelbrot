#include <afxwin.h>
#include <afxcmn.h>
#include "MfcSaver.h"
#include "MfcSaverDlg.h"

BEGIN_MESSAGE_MAP(CMfcSaverDlg, CScreenSaverDlg)
	//{{AFX_MSG_MAP(CMfcSaverDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CMfcSaverDlg::CMfcSaverDlg()
{
}

BOOL CMfcSaverDlg::OnInitDialog()
{
	CScreenSaverDlg::OnInitDialog();

	return TRUE;
}

void CMfcSaverDlg::DoDataExchange(CDataExchange* pDX)
{
	CScreenSaverDlg::DoDataExchange(pDX);

	CMfcSaver* pSaver = (CMfcSaver*)AfxGetScreenSaverWnd();
	if (!pSaver)
		return;

	if (!pDX->m_bSaveAndValidate)
	{
		pSaver->RestoreOptions();
	}
	else
	{
	}
}

