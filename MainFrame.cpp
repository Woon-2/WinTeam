#include <windows.h>
#include <tchar.h>
#include "FileUtility.h"

RECT client;
HWND h_wnd;
PAINTSTRUCT ps;
HDC h_dc;
HDC h_dc_buf;
HBITMAP h_bit_buf;
HBITMAP old_bit_buf;


HINSTANCE g_h_inst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"windows program";

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
void PrepareToDoubleBuffering();
void DoubleBuffering();
void CleanUpAfterDoubleBuffering();
void TransmitHDCBufferToRealHDC();
void TestRender();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	MSG Message;
	WNDCLASSEX WndClass;
	g_h_inst = hInstance;

	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);

	h_wnd = CreateWindow(lpszClass, lpszWindowName, WS_SYSMENU, 0, 0, 1280, 800, NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(h_wnd, nCmdShow);
	UpdateWindow(h_wnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND h_wnd, UINT u_msg, WPARAM w_param, LPARAM l_param)
{

	switch (u_msg)
	{
	case WM_CREATE:
		GetClientRect(h_wnd, &client);

		return 0;

	case WM_PAINT:
		PrepareToDoubleBuffering();
		DoubleBuffering();
		CleanUpAfterDoubleBuffering();
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(h_wnd, u_msg, w_param, l_param);
}

void PrepareToDoubleBuffering()
{
	h_dc = BeginPaint(h_wnd, &ps);
	h_dc_buf = CreateCompatibleDC(h_dc);
	h_bit_buf = CreateCompatibleBitmap(h_dc, client.right, client.bottom);
	old_bit_buf = (HBITMAP)SelectObject(h_dc_buf, h_bit_buf);
}

void DoubleBuffering()
{
	//Scene.render();
	TestRender();
	TransmitHDCBufferToRealHDC();
}

void CleanUpAfterDoubleBuffering()
{
	SelectObject(h_dc_buf, old_bit_buf);
	DeleteObject(h_bit_buf);
	DeleteDC(h_dc_buf);
	EndPaint(h_wnd, &ps);
}

void TransmitHDCBufferToRealHDC()
{
	BitBlt(h_dc, 0, 0, client.right, client.bottom, h_dc_buf, 0, 0, SRCCOPY);
}

void TestRender()
{
	try {
		Image test;
		test.Load(L"TestImage.png");
		int test_image_width = test.GetWidth();
		int test_image_height = test.GetHeight();
		test.Draw(h_dc_buf, 0, 0, client.right, client.bottom, 0, 0, test_image_width, test_image_height);
		test.Destroy();
	}
	catch (const TCHAR* error_message) {
		MessageBox(h_wnd, error_message, L"Error", MB_OK);
	}
}