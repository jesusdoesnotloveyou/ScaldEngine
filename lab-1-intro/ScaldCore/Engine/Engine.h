#pragma once

#include "RenderWindow.h"
#include "ScaldTimer.h"

class KatamariPlayer;

class Engine
{
public:
	Engine();
	~Engine();
	int Launch();
	
	// Katamari specific
	KatamariPlayer* Player = nullptr;

private:
	void SetupScene();
	void PollInput();
	void UpdateScene(const ScaldTimer& st);
	void RenderFrame(const ScaldTimer& st);

	void CalculateFrameStats();
	float AspectRatio() const;

protected:
	RenderWindow mRenderWindow;
	ScaldTimer mTimer;

	int mClientWidth = 1600;
	int mClientHeight = 900;
};