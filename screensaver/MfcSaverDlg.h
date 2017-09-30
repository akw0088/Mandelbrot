#ifndef __MFCSAVERDLG_H__
#define __MFCSAVERDLG_H__

#include "ScreenSaverDlg.h"

class CMfcSaverDlg : public CScreenSaverDlg
{
public:
	CMfcSaverDlg();
	enum { IDD = DLG_SCRNSAVECONFIGURE };
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	HICON m_hIcon;

	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
};


#endif
