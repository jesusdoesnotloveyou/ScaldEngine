#include "Engine.h"
#include <sstream>

#include "../../Objects/Geometry/Rectangle.h"
#include "../../Objects/Geometry/Circle.h"
#include "../../Objects/Geometry/Star.h"

#include <random>
#include <ctime>

Engine::Engine()
    :
    mRenderWindow(1024, 768, "Direct3DApp")
{}

int Engine::Launch()
{
	mRenderWindow.GetGfx().Setup();
	SetupScene();
	
	mTimer.Reset();
	while (true)
	{
		mTimer.Tick();
		// process all messages pending, but to not block
		if (const auto eCode = RenderWindow::ProcessMessages())
		{
		 	// if return optional has a value, means we're quitting
			return *eCode;
		}
		// otherwise

		CalculateFrameStats();
		PollInput();
		UpdateScene(mTimer.DeltaTime());
		RenderFrame();
	}
}

bool Engine::CheckCollision(PrimitiveGeometry* ball, PrimitiveGeometry* otherActor)
{
	if (ball->GetCollisionComponent()->BoundingBox.Intersects(otherActor->GetCollisionComponent()->BoundingBox)) return true;
	return false;
}

void Engine::UpdateCollisionWithPaddle(PrimitiveGeometry* ball, PrimitiveGeometry* otherActor)
{
	if (!CheckCollision(ball, otherActor)) return;
	
	auto ballSpeed = ball->GetMovementComponent()->GetVelocity();
	ballSpeed.x += 0.002f;
	ballSpeed.y += 0.002f;
	ball->GetMovementComponent()->SetVelocity(ballSpeed.x * (-1.f), ballSpeed.y, ballSpeed.z);
}

void Engine::SetupScene()
{
	STransform pongBallTransform;
	pongBallTransform.Scale = { 0.03f, 0.03f, 0.0f };
	PrimitiveGeometry* ball = new Rect(pongBallTransform);
	ball->SetIsMovable(true);
	ball->Reset(ball->GetMovementComponent()->GetInitialVelocity(), ball->GetMovementComponent()->GetInitialTransition());

	STransform firstRocketTransform;
	firstRocketTransform.Scale = { 0.01f, 0.2f, 0.0f };
	firstRocketTransform.Translation = { -0.95f, 0.0f, 0.0f };
	PrimitiveGeometry* firstRocket = new Rect(firstRocketTransform);
	
	STransform secondRocketTransform;
	secondRocketTransform.Scale = { 0.01f, 0.2f, 0.0f };
	secondRocketTransform.Translation = { 0.95f, 0.0f, 0.0f };
	PrimitiveGeometry* secondRocket = new Rect(secondRocketTransform);

	GameObjects.push_back(firstRocket);
	GameObjects.push_back(secondRocket);
	GameObjects.push_back(ball);

	for (auto geometry : GameObjects)
	{
		if (!geometry) continue;
		geometry->Initialize(mRenderWindow.GetGfx().GetDevice(), mRenderWindow.GetGfx().GetDeviceContext());
	}
}

void Engine::PollInput()
{
	float speed = 0.03f;
	while (!mRenderWindow.kbd.IsKeyEmpty())
	{
		const auto e = mRenderWindow.kbd.ReadKey();
		switch (e.GetCode())
		{
			// W
		case 87:
		{
			if (GameObjects[0]->ObjectTransform.Translation.y + GameObjects[0]->ObjectTransform.Scale.y > 1.f ) break;
			GameObjects[0]->ObjectTransform.Translation.y += speed;
			break;
		}
			// S
		case 83:
			if (GameObjects[0]->ObjectTransform.Translation.y - GameObjects[0]->ObjectTransform.Scale.y < -1.f) break;
			GameObjects[0]->ObjectTransform.Translation.y -= speed;
			break;
			// up
		case 38:
		{
			if (GameObjects[1]->ObjectTransform.Translation.y + GameObjects[1]->ObjectTransform.Scale.y > 1.f) break;
			GameObjects[1]->ObjectTransform.Translation.y += speed;
			break;
		}
		// down
		case 40:
			if (GameObjects[1]->ObjectTransform.Translation.y - GameObjects[1]->ObjectTransform.Scale.y < -1.f) break;
			GameObjects[1]->ObjectTransform.Translation.y -= speed;
			break;
		}
	}
}

void Engine::UpdateScene(float DeltaTime)
{
	UpdateCollisionWithPaddle(GameObjects[2], GameObjects[0]);
	UpdateCollisionWithPaddle(GameObjects[2], GameObjects[1]);
	
	for (auto gameObject : GameObjects)
	{
		gameObject->Update(DeltaTime);
	}
}

void Engine::RenderFrame()
{
	//const float color = static_cast<float>(sin(mTimer.DeltaTime()) + 1.0f);
	mRenderWindow.GetGfx().ClearBuffer(0.1f);
	mRenderWindow.GetGfx().DrawScene(GameObjects);
	mRenderWindow.GetGfx().EndFrame();
}

void Engine::CalculateFrameStats()
{
	// Code computes the average frames per second, and also the
	// average time it takes to render one frame. These stats
	// are appeneded to the window caption bar.
	static int frameCnt = 0;
	static float timeElapsed = 0.0f;
	frameCnt++;
	// Compute averages over one second period.
	if ((mTimer.TotalTime() - timeElapsed) >= 1.0f)
	{
		float fps = (float)frameCnt; // fps = frameCnt / 1
		float mspf = 1000.0f / fps;
		std::ostringstream outs;
		outs.precision(6);
		outs << "FPS: " << fps << " "
			<< "Frame Time: " << mspf << "(ms)\n";
		mRenderWindow.SetTitle(outs.str());
		// Reset for next average.
		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}

float Engine::AspectRatio()const
{
	return static_cast<float>(mClientWidth) / mClientHeight;
}