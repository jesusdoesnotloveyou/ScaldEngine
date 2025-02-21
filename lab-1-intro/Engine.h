#pragma once

#include "RenderWindow.h"

class Engine
{
public:
	Engine();
	int Launch();

private:
	void PresentFrame();

private:
	RenderWindow renderWindow;
};