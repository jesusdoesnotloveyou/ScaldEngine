#pragma once

#include "ScaldWindows.h"
#include "ScaldException.h"
#include "Graphics.h"
#include <optional>
#include <string>
#include <memory>

class RenderWindow
{
private:
	class WindowClass
	{
	public:
		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;

	private:
		WindowClass() noexcept;
		~WindowClass();

		WindowClass(const WindowClass& w) = delete;
		WindowClass operator=(const WindowClass& w) = delete;
		static constexpr const char* applicationName = "Scald Direct3D Engine Window";
		// to return hInst via static class object
		static WindowClass wndClass;
		HINSTANCE hInst;
	};

public:
	RenderWindow(int width, int height, const char* windowTitle);
	~RenderWindow();

	void SetTitle(const std::string& title);
	static std::optional<int> ProcessMessages() noexcept;
	Graphics& GetGfx();

private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
private:
	int width;
	int height;
	HWND hWnd;

	std::unique_ptr<Graphics> pGfx;
};

#define SCALDWND_LAST_EXCEPT() WindowException(__LINE__, __FILE__, GetLastError())