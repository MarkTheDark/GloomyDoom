#include "stdafx.h"
#include "d2d1.h"
#include <d2d1.h>
#pragma comment(lib, "d2d1")

using namespace std;
using namespace mjh;

// constants
static const int VIDEO_BUFFER_WIDTH =			320;
static const int VIDEO_BUFFER_HEIGHT =			(int)(VIDEO_BUFFER_WIDTH * 0.625f);
static const int WINDOW_WIDTH =					(int)(VIDEO_BUFFER_WIDTH * 4.0f);
static const int WINDOW_HEIGHT =				(int)(VIDEO_BUFFER_HEIGHT * 4.0f);
static const int WINDOW_FRAME_WIDTH =			2;
static const int WINDOW_TITLEBAR_HEIGHT =		21;


// Global Variables:
HINSTANCE               g_hInst =				NULL;
HWND                    g_hWnd =				NULL;
ID2D1Factory*			pD2DFactory =			NULL;
ID2D1HwndRenderTarget*  pRenderTarget =			NULL;
ID2D1Bitmap				*_backBufferBmp =		NULL;
D2D1_PIXEL_FORMAT		desc2D;
D2D1_BITMAP_PROPERTIES	bmpProperties;
D2D1_SIZE_U				bmpsize;
D2D1_RECT_U				bmpsrcrect;
D2D1_RECT_F				bmpsrcrectf;
RenderDevice			device;
Timer					timer;
Input					input;
long					runningMilliseconds =	0;
long					frameCount =			0;
long					totalMilliseconds =		0;
int						printoutCount = 0;


// Forward declarations of functions included in this code module:
HRESULT					InitWindow();
LRESULT CALLBACK		WndProc(HWND, UINT, WPARAM, LPARAM);
void					InitBackBufferVars();

void drawFrame()
{
	long startStamp = (long)timer.getTimestampMilli();

	device.clear();
	int c = RGB(rand() % 256, rand() % 256, rand() % 256);
	device.putPixel(device.getBufferWidth() - 1, device.getBufferHeight() - 1, c);
	device.renderWindowBuffer();

	// update fps 
	long stopStamp = (long)timer.getTimestampMilli();
	long delta = stopStamp - startStamp;
	runningMilliseconds += delta;
	frameCount++;
	if (frameCount == timer.SECONDS_IN_MINUTE) {
		float t = (float)runningMilliseconds / (float)frameCount;
		cout << "frames: " << frameCount << " totalMs: " << runningMilliseconds << " time per frame: " << t << "ms" << endl;
		totalMilliseconds += (int)t;
		printoutCount++;
		cout << "Avg fps: " << 1000.0f / (totalMilliseconds / (float)printoutCount) << "   (totalMs: " << totalMilliseconds << ", printOuts: " << printoutCount << ")" << endl;
		frameCount = 0;
		runningMilliseconds = 0;
	}
}

void render()
{
	drawFrame();

	pRenderTarget->BeginDraw();
	_backBufferBmp->CopyFromMemory(&bmpsrcrect, device.getWindowBuffer(), device.getWindowWidth() * 4);
	pRenderTarget->DrawBitmap(_backBufferBmp, bmpsrcrectf);
	pRenderTarget->EndDraw();
}

int main()
{
	device.initialize(VIDEO_BUFFER_WIDTH, VIDEO_BUFFER_HEIGHT, WINDOW_WIDTH, WINDOW_HEIGHT);

	if (FAILED(InitWindow()))
	{
		device.release();
		return -1;
	}

	device.release();
	return 0;
}

void InitBackBufferVars() {

	desc2D = D2D1::PixelFormat();

	desc2D.format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
	desc2D.alphaMode = D2D1_ALPHA_MODE_IGNORE;

	bmpProperties = D2D1::BitmapProperties();
	pRenderTarget->GetDpi(&bmpProperties.dpiX, &bmpProperties.dpiY);
	bmpProperties.pixelFormat = desc2D;

	bmpsize = D2D1::SizeU(device.getWindowWidth(), device.getWindowHeight());

	bmpsrcrect.left = 0;
	bmpsrcrect.top = 0;
	bmpsrcrect.right = device.getWindowWidth();
	bmpsrcrect.bottom = device.getWindowHeight();

	bmpsrcrectf.left = 0;
	bmpsrcrectf.top = 0;
	bmpsrcrectf.bottom = (float)device.getWindowHeight();
	bmpsrcrectf.right = (float)device.getWindowWidth();
}

HRESULT InitDevice()
{
	HRESULT hr = S_OK;
	if (FAILED(D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		&pD2DFactory
		)))
		return E_FAIL;

	//size of drawing area
	RECT rc;
	GetClientRect(g_hWnd, &rc);

	//Create direct2D render target
	if (FAILED(pD2DFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(
			g_hWnd,
			D2D1::SizeU(
				rc.right - rc.left,
				rc.bottom - rc.top)
			),
		&pRenderTarget
		)))
		return E_FAIL;

	InitBackBufferVars();

	//create the backbufferbmp once!
	pRenderTarget->BeginDraw();
	pRenderTarget->CreateBitmap(bmpsize, device.getWindowBuffer(), device.getWindowWidth() * 4, bmpProperties, &_backBufferBmp);
	pRenderTarget->EndDraw();

	return S_OK;
}

void cleanup()
{
	device.release();
	SafeRelease(_backBufferBmp);
	SafeRelease(pRenderTarget);
	SafeRelease(pD2DFactory);
}

HRESULT InitWindow()
{
	// Register class
	const wchar_t CLASS_NAME[] = L"MJHDoom";

	WNDCLASS wc = {};

	wc.lpfnWndProc = WndProc;
	wc.hInstance = GetModuleHandle(CLASS_NAME);
	wc.lpszClassName = CLASS_NAME;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	//wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	int resolutionWidth = GetSystemMetrics(SM_CXSCREEN);
	int resolutionHeight = GetSystemMetrics(SM_CYSCREEN);

	int upperX = (resolutionWidth - WINDOW_WIDTH) / 2;
	int upperY = (resolutionHeight - WINDOW_HEIGHT) / 2;
	if (upperY < 0)
		upperY = 0;

	RECT rc = { upperX, upperY, WINDOW_WIDTH + upperX + WINDOW_FRAME_WIDTH, WINDOW_HEIGHT + upperY + WINDOW_TITLEBAR_HEIGHT };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	g_hWnd = CreateWindowEx(0, CLASS_NAME, CLASS_NAME, WS_EX_OVERLAPPEDWINDOW | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, 
							CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left - 2, rc.bottom - rc.top - 2, NULL, NULL, 
							GetModuleHandle(CLASS_NAME), NULL);

	SetWindowPos(g_hWnd, NULL, upperX, upperY, rc.right - upperX, rc.bottom - upperY, SWP_SHOWWINDOW);
	if (!g_hWnd) 
	{
		cout << "hwnd null" << endl;
		return E_FAIL;
	}

	ShowWindow(g_hWnd, SW_SHOWDEFAULT);

	if (FAILED(InitDevice()))
		return E_FAIL;
	
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_KEYDOWN)
				input.keyDown(msg.wParam);

			if (msg.message == WM_KEYUP)
				input.keyUp(msg.wParam);

			if (msg.message == WM_MOUSEMOVE)
			{
				int x = GET_X_LPARAM(msg.lParam);
				int y = GET_Y_LPARAM(msg.lParam);
				input.updateMouse(x, y);
			}

			if (input.isKeyDown(0x51))
				break;
		}
		else
		{
			render();
		}
	}

	cleanup();

	return S_OK;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}