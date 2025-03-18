#pragma once

#include "RenderWindow.h"
#include "ScaldTimer.h"

class Engine
{
public:
	Engine();
	int Launch();

	float AspectRatio() const;
  
private:
	void SetupScene();
	void UpdateScene(float DeltaTime);
	void PollInput();
	void RenderFrame();
	void CalculateFrameStats();

protected:
	std::vector<PrimitiveGeometry*> GameObjects;
	RenderWindow mRenderWindow;
	ScaldTimer mTimer;

	int mClientWidth = 1600;
	int mClientHeight = 900;
};