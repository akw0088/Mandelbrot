#ifndef __MFCSAVER_H__
#define __MFCSAVER_H__

#include <afxwin.h> 

#include "resource.h"

#include "ScreenSaverWnd.h"

class CMfcSaver : public CScreenSaverWnd
{
public:
	CMfcSaver();

	virtual void OnDraw(CDC* pDC);
	virtual void OnInitialUpdate();
	virtual ~CMfcSaver();

private:
	int xres, yres;
	int *pixel;
	HWND hwnd;
	RECT rect;
	HDC hdcMem;

	float zoom;
	float moveX;
	float moveY;
protected:
	afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_MESSAGE_MAP()
};

#endif
