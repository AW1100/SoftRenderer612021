#include <windows.h>
#include <windowsx.h>
#include <memory>
#include <string>
#include <iostream>
#include "Demo.h"
#include "Shader.h"

HINSTANCE				g_hInstance;				//ʵ�����
static HWND				g_hWnd;					//���ھ��
int						g_width = 800;			//���ڴ�С
int						g_height = 600;

std::shared_ptr<Demo> g_pDemo = std::make_shared<Demo>();

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	//�������
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
	//ע�ᴰ��
	if (!RegisterClassEx(&wcex))
		return 0;

	RECT rc = { 0, 0, 800, 600 };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);

	HWND g_hWnd = CreateWindowEx(WS_EX_APPWINDOW, L"D3D", L"D3D", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
		CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, g_hInstance, nullptr);

	if (!g_hWnd)
	{
		MessageBox(nullptr, L"Window Creation Failed!", L"ERROR", MB_OK);
		return 0;
	}

	ShowWindow(g_hWnd, nShowCmd);

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
			g_pDemo->Update(0.0);
			g_pDemo->Render();
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

	//˫�����ͼ
	static BITMAPINFO s_bitmapInfo;
	static HDC s_hdcBackbuffer;			//�󻺳��豸���	
	static HBITMAP s_hBitmap;
	static HBITMAP s_hOldBitmap;
	static void* s_pData;

	switch (message)
	{
	case WM_CREATE:
	{
		g_pDemo->init(g_hInstance, hWnd);
		//��ʼ���豸�޹�λͼheader
		BITMAPINFOHEADER bmphdr = { 0 };
		bmphdr.biSize = sizeof(BITMAPINFOHEADER);
		bmphdr.biWidth = g_width;
		bmphdr.biHeight = -g_height;
		bmphdr.biPlanes = 1;
		bmphdr.biBitCount = 32;
		bmphdr.biSizeImage = g_height * g_width * 4;
		//�����󻺳���
		//�ȴ���һ���ڴ�dc
		s_hdcBackbuffer = CreateCompatibleDC(nullptr);
		//���ǰ�û�����dc
		HDC hdc = GetDC(hWnd);
		if (!(s_hBitmap = CreateDIBSection(nullptr, (PBITMAPINFO)&bmphdr, DIB_RGB_COLORS,
			reinterpret_cast<void**>(&g_pDemo->GetDevice()->getFrameBuffer()), nullptr, 0)))
		{
			MessageBox(nullptr, L"create dib section failed!", L"error", MB_OK);
			return 0;
		}
		//��bitmapװ���ڴ�dc
		s_hOldBitmap = (HBITMAP)SelectObject(s_hdcBackbuffer, s_hBitmap);
		//�ͷ�dc
		ReleaseDC(hWnd, hdc);
	}
	break;
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		//��backbuffer���ݴ���frontbuffer
		BitBlt(ps.hdc, 0, 0, g_width, g_height, s_hdcBackbuffer, 0, 0, SRCCOPY);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		SelectObject(s_hdcBackbuffer, s_hOldBitmap);
		DeleteDC(s_hdcBackbuffer);
		DeleteObject(s_hOldBitmap);
		PostQuitMessage(0);
		break;

		//��ֹ�������� ��ֹ��˸
	case WM_ERASEBKGND:
		return true;
		break;
		//����¼�
		//��걻����ʱ
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		g_pDemo->OnMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;

		//����ͷ�ʱ
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		g_pDemo->OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;

		//����ƶ�ʱ
	case WM_MOUSEMOVE:
		g_pDemo->OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_MOUSEWHEEL:

		return 0;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}