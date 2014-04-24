#include "include.h"


int	cxClient = 1, cyClient = 1;
int *pixel = NULL;

#define WM_TICK 1

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);


int HsvToRgb(float hue, float saturation, float value)
{
	int h60 = hue / 60;
	int hi = (int)h60 % 6;
	float f = (hue / 60.0) - h60;

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


void draw_pixels(HDC hdc, HDC hdcMem, int width, int height)
{
	HBITMAP hBitmap, hOldBitmap;

	hBitmap = CreateCompatibleBitmap(hdc, width, height);
	SetBitmapBits(hBitmap, sizeof(int) * width * height, pixel);
	hdcMem = CreateCompatibleDC(hdc);
	hOldBitmap = (HBITMAP)SelectObject(hdcMem, hBitmap);
	// This scaling is a little strange because Stretch maintains aspect ratios
	StretchBlt(hdc, 0, 0, cxClient, cyClient, hdcMem, 0, 0, width, height, SRCCOPY);
	SelectObject(hdcMem, hOldBitmap);
	DeleteDC(hdcMem);
	DeleteObject(hBitmap);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("Mandelbrot Set");
	HWND         hwnd;
	MSG          msg;
	WNDCLASS     wndclass;
     
	wndclass.style         = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc   = WndProc;
	wndclass.cbClsExtra    = 0;
	wndclass.cbWndExtra    = 0;
	wndclass.hInstance     = hInstance;
	wndclass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)(NULL_BRUSH);
	wndclass.lpszMenuName  = NULL;
	wndclass.lpszClassName = szAppName;
          
	if (!RegisterClass (&wndclass))
	{
		MessageBox(NULL, TEXT ("Program requires Windows NT!"), szAppName, MB_ICONERROR);
		return 0;
	} 


	hwnd = CreateWindow (szAppName, TEXT ("Mandelbrot Set"),
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);
     
	while (GetMessage (&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}


LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static RECT	rect = {0, 0, 1, 1};
	static HDC	hdcMem;
	static int	old_style = 0;
	static int	new_style = WS_CHILD | WS_VISIBLE;
	static int	xres, yres;
	HDC		hdc;
	PAINTSTRUCT	ps;
	int		i;
	static char frame_time[80] = "";

     
	switch (message)
	{
	case WM_CREATE:
	{
		HMONITOR hmon;
		MONITORINFO mi = {sizeof(MONITORINFO)};

		hmon = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
		GetMonitorInfo(hmon, &mi);

		xres = abs(mi.rcMonitor.right - mi.rcMonitor.left);
		yres = abs(mi.rcMonitor.bottom - mi.rcMonitor.top);

		pixel = (int *)malloc(sizeof(int) * xres * yres);
		SetTimer(hwnd, WM_TICK, 16, NULL);

		return 0;
	}
	case WM_TIMER:
	{ 
		static float zoom = 1;
		static float moveX = -0.75;
		static float moveY = 0.5;

		int width = cxClient;
		int height = cyClient;
		int start, end;

		start = GetTickCount();



	#pragma omp parallel num_threads(8)
	{
		#pragma omp for
        	for(int y = 0; y < height; y++)
        	for(int x = 0; x < width; x++)
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
            		for(i = 0; i < maxIterations; i++)
            		{
                		oldRe = newRe;
                		oldIm = newIm;

                		//the actual iteration, the real and imaginary part are calculated
                		newRe = oldRe * oldRe - oldIm * oldIm + pr;
                		newIm = 2 * oldRe * oldIm + pi;

                		//if the point is outside the circle with radius 2: stop
                		if((newRe * newRe + newIm * newIm) > 4)
					 break;
            		}
 

			pixel[y * width + x] = HsvToRgb(1, i, (i < maxIterations));
        	}
	}
		end = GetTickCount();

		sprintf(frame_time, "%d ms", end - start);


		zoom += 0.1f;
		if (pixel[(height >> 1) * width + (width >> 1)] != 0)
		{
			moveX += 0.0001;
			moveY -= 0.0001;
		}

		InvalidateRect(hwnd, &rect, FALSE);
         
		return 0;
	}
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		rect.right = cxClient;
		rect.bottom = cyClient;


		return 0;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			old_style = SetWindowLong(hwnd, GWL_STYLE, new_style);
			new_style = old_style;
			SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, xres, yres, 0);
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint (hwnd, &ps);
		draw_pixels(hdc, hdcMem, cxClient, cyClient);
		DrawText(hdc, frame_time, strlen(frame_time), &rect, 0);
		EndPaint(hwnd, &ps);
		return 0;
	     
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc (hwnd, message, wParam, lParam);
}




