#pragma once

#include "RenderWindow.h"
#include "ScaldTimer.h"

class Engine
{
public:
	Engine();
	int Launch();

private:
	void RenderFrame();
	void Update();

private:
	RenderWindow renderWindow;
	ScaldTimer timer;
};