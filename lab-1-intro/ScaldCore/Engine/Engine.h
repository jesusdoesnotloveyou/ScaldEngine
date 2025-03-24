#pragma once

#include "RenderWindow.h"
#include "ScaldTimer.h"

class Engine
{
public:
	Engine();
	int Launch();

	float AspectRatio() const;
	float randomFloat(float min, float max);

private:
	void SetupScene();
	void UpdateScene(float DeltaTime);
	void PollInput();
	void RenderFrame();
	void CalculateFrameStats();

	// Pong Specific functions
	bool CheckCollision(PrimitiveGeometry* ball, PrimitiveGeometry* otherActor);
	void UpdateCollisionWithPaddle(PrimitiveGeometry* ball, PrimitiveGeometry* otherActor);
	void UpdateCollisionWithBonus(PrimitiveGeometry* bonus, PrimitiveGeometry* player);
	void SetInvisibleAndMove(PrimitiveGeometry* bonus);

protected:
	std::vector<PrimitiveGeometry*> GameObjects;
	RenderWindow mRenderWindow;
	ScaldTimer mTimer;

	int mClientWidth = 1024;
	int mClientHeight = 768;

	int leftPlayerScore = 0;
	int rightPlayerScore = 0;
};