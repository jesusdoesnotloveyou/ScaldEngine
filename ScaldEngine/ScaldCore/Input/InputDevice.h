#pragma once

#include <queue>
#include <bitset>
#include "Keys.h"
#include "../../Delegates/Delegates.h"

// KEYBOARD INPUT
class Keyboard
{
	friend class RenderWindow;
public:
	class Event
	{
	public:
		enum class Type
		{
			Press = 0,
			Release,
			Invalid,
			MAX = 3
		};
	private:
		Type type;
		unsigned char code;
	public:
		Event()
			:
			type(Type::Invalid),
			code(0u)
		{}

		Event(Type type, unsigned char code) noexcept
			:
			type(type),
			code(code)
		{}
		bool IsPress() const noexcept
		{
			return type == Type::Press;
		}
		bool IsRelease() const noexcept
		{
			return type == Type::Release;
		}
		bool IsValid() const noexcept
		{
			return type != Type::Invalid;
		}
		unsigned char GetCode() const noexcept
		{
			return code;
		}
	};

public:
	Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;
	// key event stuff
	bool IsKeyPressed(unsigned char keyCode) const noexcept;
	Event ReadKey() noexcept;
	bool IsKeyEmpty() const noexcept;
	void FlushKey() noexcept;
	// char event stuff
	char ReadChar() noexcept;
	bool IsCharEmpty() const noexcept;
	void FlushChar() noexcept;
	void Flush() noexcept;
	// autorepeat control
	void EnableAutorepeat() noexcept;
	void DisableAutorepeat() noexcept;
	bool IsAutorepeatEnabled() const noexcept;
private:
	// stuff used by window
	void OnKeyPressed(unsigned char keyCode) noexcept;
	void OnKeyReleased(unsigned char keyCode) noexcept;
	void OnChar(char character) noexcept;
	void ClearState() noexcept;

	template<typename T>
	static void TrimBuffer(std::queue<T>& buffer) noexcept
	{
		while (buffer.size() > bufferSize)
		{
			buffer.pop();
		}
	}

public:
#pragma region Input Delegates
	MulticastDelegate<unsigned char> OnKeyPressedEvent;
	MulticastDelegate<unsigned char> OnKeyReleasedEvent;
	MulticastDelegate<unsigned char> OnCharEvent;
#pragma endregion Input Delegates

private:
	static constexpr unsigned int nKeys = 256u;
	static constexpr unsigned int bufferSize = 16u;

	bool autorepeatEnabled = false;
	std::bitset<nKeys> keyStates;
	std::queue<Event> keyBuffer;
	std::queue<char> charBuffer;
};

// MOUSE INPUT
class Mouse
{
	friend class RenderWindow;
public:
	class Event
	{
	public:
		enum class Type
		{
			LPress = 0,
			LRelease,
			RPress,
			RRelease,
			WheelUp,
			WheelDown,
			Move,
			RawMove,
			Enter,
			Leave,
			Invalid,
			MAX = 11
		};
	private:
		Type type;
		bool bIsLeftPressed;
		bool bIsRightPressed;
		int x;
		int y;
	public:
		Event()
			:
			type(Type::Invalid),
			bIsLeftPressed(false),
			bIsRightPressed(false),
			x(0),
			y(0)
		{
		}
		Event(Type type, const Mouse& parent) noexcept
			:
			type(type),
			bIsLeftPressed(parent.bIsLeftPressed),
			bIsRightPressed(parent.bIsRightPressed),
			x(parent.x),
			y(parent.y)
		{
		}
		bool IsValid() const noexcept { return type != Type::Invalid; }
		Type GetType() const noexcept { return type; }
		std::pair<int, int> GetPos() const noexcept { return std::pair{ x, y }; }
		int GetPosX() const noexcept { return x; }
		int GetPosY() const noexcept { return y; }
		bool IsLeftPressed() const noexcept { return bIsLeftPressed; }
		bool IsRightPressed() const noexcept { return bIsRightPressed; }
	};

public:
	Mouse() = default;
	Mouse(const Mouse&) = delete;
	std::pair<int, int> GetPos() const noexcept;
	int GetPosX() const noexcept;
	int GetPosY() const noexcept;
	bool IsInWindow() const noexcept;
	int  GetWheelDelta() const noexcept { return wheelDeltaCarry; }
	bool IsLeftPressed() const noexcept;
	bool IsRightPressed() const noexcept;
	Mouse::Event Read() noexcept;
	bool IsEmpty() const noexcept
	{
		return mouseBuffer.empty();
	}
	void Flush() noexcept;

private:
	void OnMouseMove(int x, int y) noexcept;
	void OnMouseMoveRaw(int x, int y) noexcept;
	void OnMouseEnter() noexcept;
	void OnMouseLeave() noexcept;
	void OnLeftPressed(int x, int y) noexcept;
	void OnLeftReleased(int x, int y) noexcept;
	void OnRightPressed(int x, int y) noexcept;
	void OnRightReleased(int x, int y) noexcept;
	void OnWheelUp(int x, int y) noexcept;
	void OnWheelDown(int x, int y) noexcept;
	void TrimBuffer() noexcept;
	void OnWheelDelta(int x, int y, int delta) noexcept;
private:
	static constexpr unsigned int bufferSize = 16u;
	int x = 0;
	int y = 0;
	bool bIsLeftPressed = false;
	bool bIsRightPressed = false;
	bool bIsInWindow = false;
	int wheelDeltaCarry = 0;
	std::queue<Event> mouseBuffer;
};