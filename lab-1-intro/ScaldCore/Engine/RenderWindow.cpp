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

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	wc.hIcon = nullptr;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm = nullptr;

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
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&RenderWindow::HandleMsgThunk));
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
	// clear keystate when window loses focus to prevent input getting zombie
	case WM_KILLFOCUS:
		kbd.ClearState();
		break;
		/****************** KEYBOARD MESSAGES *******************/
	case WM_KEYDOWN:
		// syskey commands need to be handled to track ALT key (VK_MENU)
	case WM_SYSKEYDOWN:
		if (!(lParam & 0x40000000) || kbd.IsAutorepeatEnabled()) // filter autorepeat key event
		{
			kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;

	case WM_CHAR:
		kbd.OnChar(static_cast<unsigned char>(wParam));
		break;
		/****************** END KEYBOARD MESSAGES *******************/

		/****************** MOUSE MESSAGES *******************/
	case WM_MOUSEMOVE:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		// or xPos = GET_X_LPARAM(lParam); yPos = GET_Y_LPARAM(lParam);
		// in client region -> log move, log enter + capture mouse
		if (pt.x > 0 && pt.x < width && pt.y > 0 && pt.y < height)
		{
			mouse.OnMouseMove(pt.x, pt.y);
			if (!mouse.IsInWindow())
			{
				SetCapture(hWnd);
				mouse.OnMouseEnter();
			}
		}
		// not in client region -> log move / maintain capture if button down
		else
		{
			if (wParam & (MK_LBUTTON | MK_RBUTTON))
			{
				mouse.OnMouseMove(pt.x, pt.y);

			}
			// button up -> release capture / log event for leaving
			else {
				ReleaseCapture();
				mouse.OnMouseLeave();
			}
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftPressed(pt.x, pt.y);
		break;
	}
	case WM_LBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftReleased(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightPressed(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightReleased(pt.x, pt.y);
		break;
	}
	case WM_MOUSEWHEEL:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		mouse.OnWheelDelta(pt.x, pt.y, delta);
		break;
	}
	/****************** END MOUSE MESSAGES *******************/
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}