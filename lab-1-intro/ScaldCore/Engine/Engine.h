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
	void PollInput();
	void UpdateScene(const ScaldTimer& st);
	void RenderFrame(const ScaldTimer& st);

	void CalculateFrameStats();
	float AspectRatio() const;

protected:
	std::vector<PrimitiveGeometry*> mGameObjects;
	RenderWindow mRenderWindow;
	ScaldTimer mTimer;

	int mClientWidth = 1600;
	int mClientHeight = 900;
};