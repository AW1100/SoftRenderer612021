#include <windows.h>
#include <windowsx.h>
#include <memory>
#include <string>
#include <iostream>
#include <string>

#include "Demo.h"
#include "Shader.h"

HINSTANCE				g_hInstance;				//实例句柄
static HWND				g_hWnd;					//窗口句柄
int						g_width = 1000;			//窗口大小
int						g_height = 750;
std::shared_ptr<Demo> g_demo = std::make_shared<Demo>();

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	//窗口设计
	WNDCLASSEX wcex;
	wcex.cbClsExtra = 0;
	wcex.cbSize = sizeof(wcex);
	wcex.cbWndExtra = 0;
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
	wcex.hIconSm = wcex.hIcon;
	wcex.hInstance = g_hInstance;
	wcex.lpfnWndProc = WndProc;
	wcex.lpszClassName = L"D3D";
	wcex.lpszMenuName = nullptr;
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	//注册窗口
	if (!RegisterClassEx(&wcex))
		return 0;

	RECT rc = { 0, 0, g_width, g_height };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);

	HWND g_hWnd = CreateWindowEx(WS_EX_APPWINDOW, L"D3D", L"D3D", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
		CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, g_hInstance, nullptr);

	if (!g_hWnd)
	{
		MessageBox(nullptr, L"Window Creation Failed!", L"ERROR", MB_OK);
		return 0;
	}

	ShowWindow(g_hWnd, nShowCmd);
	//消息循环
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			g_demo->update();
			g_demo->Render();
			InvalidateRect(g_hWnd, nullptr, true);
			UpdateWindow(g_hWnd);
		}
	}
	return static_cast<int>(msg.wParam);
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	int count = g_demo->getDrawCalls();
	TCHAR displayLine1[] = L"Triangles drawed count: ";
	TCHAR num[20] = L"";
	_itow_s(count, num, 10);

	bool cullSwitch = g_demo->getCullSwitch();
	TCHAR displayLine2[] = L"Back face culling: ";
	TCHAR cullstatus[4];
	if (cullSwitch)
	{
		wcscpy_s(cullstatus, L"On");
	}
	else
	{
		wcscpy_s(cullstatus, L"Off");
	}


	//双缓冲绘图
	static BITMAPINFO s_bitmapInfo;
	static HDC s_hdcBackbuffer;			//后缓冲设备句柄	
	static HBITMAP s_hBitmap;
	static HBITMAP s_hOldBitmap;
	static void* s_pData;

	switch (message)
	{
	case WM_CREATE:
	{
		g_demo->init(g_hInstance, hWnd);
		//初始化设备无关位图header
		BITMAPINFOHEADER bmphdr = { 0 };
		bmphdr.biSize = sizeof(BITMAPINFOHEADER);
		bmphdr.biWidth = g_width;
		bmphdr.biHeight = -g_height;
		bmphdr.biPlanes = 1;
		bmphdr.biBitCount = 32;
		bmphdr.biSizeImage = g_height * g_width * 4;
		//创建后缓冲区
		//先创建一个内存dc
		s_hdcBackbuffer = CreateCompatibleDC(nullptr);
		//获得前置缓冲区dc
		HDC hdc = GetDC(hWnd);
		if (!(s_hBitmap = CreateDIBSection(nullptr, (PBITMAPINFO)&bmphdr, DIB_RGB_COLORS,
			reinterpret_cast<void**>(&g_demo->GetDevice()->getFrameBuffer()), nullptr, 0)))
		{
			MessageBox(nullptr, L"DIB section Creation Failed!", L"ERROR", MB_OK);
			return 0;
		}
		//将bitmap装入内存dc
		s_hOldBitmap = (HBITMAP)SelectObject(s_hdcBackbuffer, s_hBitmap);
		//释放dc
		ReleaseDC(hWnd, hdc);
	}
	break;
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		//把backbuffer内容传到frontbuffer
		BitBlt(ps.hdc, 0, 0, g_width, g_height, s_hdcBackbuffer, 0, 0, SRCCOPY);
		TextOut(hdc, 5, 5, displayLine1, static_cast<int>(wcslen(displayLine1)));
		TextOut(hdc, 170, 5, num, static_cast<int>(wcslen(num)));
		TextOut(hdc, 46, 25, displayLine2, static_cast<int>(wcslen(displayLine2)));
		if (cullSwitch)
			SetTextColor(hdc, RGB(0, 255, 0));
		else
			SetTextColor(hdc, RGB(255, 0, 0));
		TextOut(hdc, 170, 25, cullstatus, static_cast<int>(wcslen(cullstatus)));
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		SelectObject(s_hdcBackbuffer, s_hOldBitmap);
		DeleteDC(s_hdcBackbuffer);
		DeleteObject(s_hOldBitmap);
		PostQuitMessage(0);
		break;

		//禁止背景擦除 防止闪烁
	case WM_ERASEBKGND:
		return true;
		break;
		//鼠标被按下时
	case WM_KEYDOWN:
	{
		//A:0x41	S:0x53	D:0x44	F:0x46
		switch (wParam)
		{
		case VK_UP:
			g_demo->upKeyPressed();
			break;
		case VK_DOWN:
			g_demo->downKeyPressed();
			break;
		case VK_LEFT:
			g_demo->leftKeyPressed();
			break;
		case VK_RIGHT:
			g_demo->rightKeyPressed();
			break;
		case 0x41:
			g_demo->onKeyPressed(1);
			break;
		case 0x53:
			g_demo->onKeyPressed(2);
			break;
		case 0x44:
			g_demo->onKeyPressed(3);
			break;
		case 0x46:
			g_demo->onKeyPressed(4);
			break;
		case VK_SPACE:
			g_demo->modelSwitch();
			break;
		case 0x5A:
			g_demo->lightMovingSwitch();
			break;
		case 0x58:
			g_demo->toggleCullSwitch();
			break;
		default:
			return 0;
		}
	}
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		g_demo->onMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;

		//鼠标释放时
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		g_demo->onMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;

		//鼠标移动时
	case WM_MOUSEMOVE:
		g_demo->onMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
		return 0;
	case WM_MOUSEWHEEL:
		g_demo->onWheelScroll(GET_WHEEL_DELTA_WPARAM(wParam));
		break;
		return 0;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}