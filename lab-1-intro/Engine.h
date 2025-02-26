#pragma once

#include "RenderWindow.h"
#include "ScaldTimer.h"

class Engine
{
public:
	Engine();
	int Launch();

private:
	void PresentFrame();

private:
	RenderWindow renderWindow;
	ScaldTimer timer;
};