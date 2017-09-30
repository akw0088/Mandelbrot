#include <afxwin.h>
#include "MfcSaver.h"
#include "MfcSaverDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_TICK 1


BEGIN_MESSAGE_MAP(CMfcSaver, CScreenSaverWnd)
	ON_WM_TIMER()
END_MESSAGE_MAP()

CMfcSaver theSaver;
CMfcSaverDlg theSaverDialog;

CMfcSaver::CMfcSaver()
{
}

CMfcSaver::~CMfcSaver()
{
}

void CMfcSaver::OnInitialUpdate()
{
	if (!m_hWnd)
		return;

	zoom = 1;
	moveX = -0.75;
	moveY = 0.5;


	HMONITOR hmon;
	MONITORINFO mi = { sizeof(MONITORINFO) };

	hmon = MonitorFromWindow(m_hWnd, MONITOR_DEFAULTTONEAREST);
	GetMonitorInfo(hmon, &mi);

	xres = abs(mi.rcMonitor.right - mi.rcMonitor.left);
	yres = abs(mi.rcMonitor.bottom - mi.rcMonitor.top);

	pixel = (int *)malloc(sizeof(int) * xres * yres);
	::SetTimer(m_hWnd, WM_TICK, 1, NULL);

}

int HsvToRgb(float hue, float saturation, float value)
{
	int h60 = hue / 60;
	int hi = (int)h60 % 6;
	float f = (hue / 60.0f) - h60;

	value = value * 255;
	int v = value;
	int p = value * (1 - saturation);
	int q = value * (1 - f * saturation);
	int t = value * (1 - (1 - f) * saturation);

	if (hi == 0)
		return RGB(v, t, p);
	else if (hi == 1)
		return RGB(q, v, p);
	else if (hi == 2)
		return RGB(p, v, t);
	else if (hi == 3)
		return RGB(p, q, v);
	else if (hi == 4)
		return RGB(t, p, v);
	else
		return RGB(v, p, q);
}

void CMfcSaver::OnDraw(CDC* pDC)
{
	int width = xres;
	int height = yres;

	int start, end;

	start = GetTickCount();

#pragma omp parallel num_threads(8)
	{
#pragma omp for
		for (int y = 0; y < height; y++)
			for (int x = 0; x < width; x++)
			{
				float pr, pi;
				float newRe, newIm, oldRe, oldIm;
				int maxIterations = 128;
				int i;

				//calculate the initial real and imaginary part of z, based on the pixel location and zoom and position values
				pr = 1.5f * (x - (width >> 1)) / (0.5f * zoom * width) + moveX;
				pi = (y - (height >> 1)) / (0.5f * zoom * height) + moveY;

				newRe = 0;
				newIm = 0;
				oldRe = 0;
				oldIm = 0;

				//start the iteration process
				for (i = 0; i < maxIterations; i++)
				{
					oldRe = newRe;
					oldIm = newIm;

					//the actual iteration, the real and imaginary part are calculated
					newRe = oldRe * oldRe - oldIm * oldIm + pr;
					newIm = 2 * oldRe * oldIm + pi;

					//if the point is outside the circle with radius 2: stop
					if ((newRe * newRe + newIm * newIm) > 4)
						break;
				}


				pixel[y * width + x] = HsvToRgb(1, i, (i < maxIterations));
			}
	}
	end = GetTickCount();



	zoom += 0.1f;
	if (pixel[(height >> 1) * width + (width >> 1)] != 0)
	{
		moveX += 0.0001f;
		moveY -= 0.0001f;
	}


	HBITMAP hBitmap, hOldBitmap;
	HDC hdc = ::GetDC(NULL);

	hBitmap = CreateCompatibleBitmap(hdc, width, height);
	SetBitmapBits(hBitmap, sizeof(int) * width * height, pixel);
	hdcMem = CreateCompatibleDC(hdc);
	hOldBitmap = (HBITMAP)SelectObject(hdcMem, hBitmap);
	// This scaling is a little strange because Stretch maintains aspect ratios
	StretchBlt(hdc, 0, 0, xres, yres, hdcMem, 0, 0, width, height, SRCCOPY);
	SelectObject(hdcMem, hOldBitmap);
	DeleteDC(hdcMem);
	DeleteObject(hBitmap);
}
void CMfcSaver::OnTimer(UINT nIDEvent) 
{
	CScreenSaverWnd::OnTimer(nIDEvent);

	Invalidate(FALSE);
}
