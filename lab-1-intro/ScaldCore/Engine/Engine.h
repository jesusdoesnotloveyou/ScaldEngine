#pragma once

#include "RenderWindow.h"
#include "ScaldTimer.h"

class Engine
{
public:
	Engine();
	int Launch();

private:
	void SetupScene();
	void UpdateScene(float DeltaTime);
	void PollInput();
	void RenderFrame();
	void CalculateFrameStats();

private:
	std::vector<PrimitiveGeometry*> GameObjects;
	RenderWindow mRenderWindow;
	ScaldTimer mTimer;
};