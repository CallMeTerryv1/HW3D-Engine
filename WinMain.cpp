#include <Windows.h>
#include <string>
#include "WindowsMessageMap.h"
#include <sstream>

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static WindowsMessageMap mm;
	std::string narrowMsg = mm(msg, lParam, wParam);

	// Convert narrow string to wide string
	int wideStrLen = MultiByteToWideChar(CP_UTF8, 0, narrowMsg.c_str(), -1, nullptr, 0);
	std::wstring wideMsg(wideStrLen, L'\0');
	MultiByteToWideChar(CP_UTF8, 0, narrowMsg.c_str(), -1, &wideMsg[0], wideStrLen);

	// Output wide string
	OutputDebugStringW(wideMsg.c_str());

	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(420);
		break;
		case WM_KEYDOWN:
			if (wParam == 'F')
			{
				SetWindowText(hWnd, L"Respects");
			}
			break;
		case WM_KEYUP:
			if (wParam == 'F')
			{
				SetWindowText(hWnd, L"Yoo We Doing Things");
			}
			break;
		case WM_CHAR:
			{
				static std::wstring title;
				title.push_back((wchar_t)wParam);
				SetWindowText(hWnd, title.c_str());
			}
			break;
		case WM_LBUTTONDOWN:
			{
				POINTS pt = MAKEPOINTS(lParam);
				std::wostringstream oss;
				oss << L"X: " << pt.x << L", Y: " << pt.y;
				SetWindowText(hWnd, oss.str().c_str());
			}
			break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int CALLBACK WinMain(
HINSTANCE hInstance,
HINSTANCE hPrevInstance,
LPSTR lpCmdLine,
int nCmdShow)
{
	auto const pClassName = L"hw3d";
	//register the window class
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName	= nullptr;
	wc.lpszClassName = pClassName;
	wc.hIconSm = nullptr;
	RegisterClassEx(&wc);
	//create the window instance
	HWND hWnd = CreateWindowEx(
		0, pClassName, L"Yoo We Doing Things",
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		200, 200, 640, 480,
		nullptr, nullptr, hInstance, nullptr);
	//show the window
	ShowWindow(hWnd, SW_SHOW);
	
	//message pump
	MSG msg;
	BOOL gResult;
	while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	if (gResult == -1)
	{
		return -1;
	}
	else
	{
		return msg.wParam;
	}
}