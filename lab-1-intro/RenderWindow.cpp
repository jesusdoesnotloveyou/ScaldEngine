#include "ScaldWindows.h"
#include <sstream>
#include <iostream>
#include "RenderWindow.h"

RenderWindow::WindowClass RenderWindow::WindowClass::wndClass;

const char* RenderWindow::WindowClass::GetName() noexcept
{
	return applicationName;
}

HINSTANCE RenderWindow::WindowClass::GetInstance() noexcept
{
	return wndClass.hInst;
}

RenderWindow::WindowClass::WindowClass() noexcept
	:
	hInst(GetModuleHandle(nullptr))
{
	// Step 01: Create a Window
#pragma region Window init
	WNDCLASSEX wc = {};

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	wc.hIcon = nullptr;
	wc.hIconSm = nullptr;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);
}

RenderWindow::WindowClass::~WindowClass()
{
	UnregisterClass(applicationName, hInst);
}

RenderWindow::RenderWindow(int width, int height, const char* windowTitle)
	:
	width(width),
	height(height)
{
	RECT wr = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };
	if (AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE) == 0)
	{
		throw SCALDWND_LAST_EXCEPT();
	}

	hWnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		WindowClass::GetName(),
		windowTitle,
		WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_THICKFRAME,
		CW_USEDEFAULT, CW_USEDEFAULT,
		wr.right - wr.left, wr.bottom - wr.top,
		nullptr, // Handle to parent of this window. Since this is the first window, it has no parent
		nullptr,
		WindowClass::GetInstance(),
		this); // param to create window

	if (hWnd == nullptr)
	{
		throw SCALDWND_LAST_EXCEPT();
	}

	ShowWindow(hWnd, SW_SHOW);
	//SetForegroundWindow(hWnd);
	SetFocus(hWnd);

	ShowCursor(true);
#pragma endregion Window init

	pGfx = std::make_unique<Graphics>(hWnd, width, height);
}

RenderWindow::~RenderWindow()
{
	DestroyWindow(hWnd);
}

void RenderWindow::SetTitle(const std::string& title)
{
	SetWindowText(hWnd, title.c_str());
}

std::optional<int> RenderWindow::ProcessMessages() noexcept
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while(PeekMessage(&msg, nullptr/*hWnd*/, 0u, 0u, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return (int)msg.wParam;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return {};
}

Graphics& RenderWindow::GetGfx()
{
	return *pGfx;
}

LRESULT WINAPI RenderWindow::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	if (msg == WM_NCCREATE)
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		RenderWindow* const pWnd = static_cast<RenderWindow*>(pCreate->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(&RenderWindow::HandleMsgThunk));
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT WINAPI RenderWindow::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	RenderWindow* const pWnd = reinterpret_cast<RenderWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT RenderWindow::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(69);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}