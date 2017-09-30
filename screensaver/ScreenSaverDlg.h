#ifndef __SCREENSAVERDLG_H__
#define __SCREENSAVERDLG_H__


#include <scrnsave.h>

class CScreenSaverDlg : public CDialog
{
	DECLARE_DYNAMIC(CScreenSaverDlg)
public:
	CScreenSaverDlg();
	virtual BOOL OnInitDialog();
	virtual ~CScreenSaverDlg();
	static CScreenSaverDlg* sm_pTheConfigureDialog;

protected:
	virtual LRESULT WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT DefWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	afx_msg void OnNcDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

};

inline CScreenSaverDlg* AfxGetScreenSaverDialog()
{
		return CScreenSaverDlg::sm_pTheConfigureDialog;
}

BOOL WINAPI ScreenSaverConfigureDialog(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL WINAPI RegisterDialogClasses(HANDLE hInstance);

#endif
