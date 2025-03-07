#pragma once

#include <chrono>

class ScaldTimer
{
public:
	ScaldTimer();
	// Gives time elapsed since Mark was called last time
	float Mark();
	// The same but without resetting last point
	float Peek() const;

private:
	_CHRONO steady_clock::time_point last;
};