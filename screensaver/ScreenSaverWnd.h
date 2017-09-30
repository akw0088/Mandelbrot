#ifndef __SCREENSAVERWND_H__
#define __SCREENSAVERWND_H__

#include <scrnsave.h>

class CScreenSaverWnd : public CWnd
{
	DECLARE_DYNAMIC(CScreenSaverWnd)
public:
	CScreenSaverWnd();

	BOOL IsAutoBlack() const;
	void SetAutoBlack(BOOL bAutoBlack = TRUE);
	//
	CPalette* GetPalette() const;
	CPalette* SetPalette(CPalette* pPalette);
	virtual void OnDraw(CDC* pDC);
	virtual void OnInitialUpdate();
	virtual void SaveOptions();
	virtual void RestoreOptions();

	//{{AFX_VIRTUAL(CScreenSaverWnd)
	//}}AFX_VIRTUAL
	virtual ~CScreenSaverWnd();
	static CScreenSaverWnd* sm_pTheScreenSaver;

protected:
	virtual LRESULT WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT DefWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg BOOL OnQueryNewPalette();
	afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
	DECLARE_MESSAGE_MAP()

	BOOL m_bAutoBlack;
	CPalette* m_pPalette;

};

inline CScreenSaverWnd* AfxGetScreenSaverWnd()
{
		return CScreenSaverWnd::sm_pTheScreenSaver;
}

LRESULT WINAPI ScreenSaverProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif
