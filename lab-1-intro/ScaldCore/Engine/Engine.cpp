#include "Engine.h"
#include <sstream>

#include "../../Objects/Geometry/Rectangle.h"
#include "../../Objects/Geometry/Cube.h"

#include <random>
#include <ctime>

Engine::Engine()
    :
    mRenderWindow(1600, 900, "Direct3DApp")
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
	STransform firstObjectTransform;
	firstObjectTransform.Scale = { 1.0f, 1.0f, 1.0f };
	firstObjectTransform.Translation = { 0.0f, 0.0f, 0.0f };
	PrimitiveGeometry* firstRocket = new Cube(firstObjectTransform);
	
	GameObjects.push_back(firstRocket);

	for (auto geometry : GameObjects)
	{
		if (!geometry) continue;
		geometry->Initialize(mRenderWindow.GetGfx().GetDevice(), mRenderWindow.GetGfx().GetDeviceContext());
	}
}

void Engine::PollInput()
{
	while (!mRenderWindow.kbd.IsKeyEmpty()) {
		const auto keyEvent = mRenderWindow.kbd.ReadKey();
		unsigned char keyCode = keyEvent.GetCode();
	}

#pragma region CameraRotation
	std::ostringstream oss;
	const auto mouseEvent = mRenderWindow.mouse.Read();
	if (mRenderWindow.mouse.IsRightPressed()) {
		if (mouseEvent.GetType() == Mouse::Event::Type::RawMove) 
		{
			mRenderWindow.GetGfx().mCamera.AdjustRotation((float)mouseEvent.GetPosY() * 0.01f, (float)mouseEvent.GetPosX() * 0.01f, 0.0f);
			oss << "Mouse X: " << mouseEvent.GetPosX() <<
				" Mouse Y: " << mouseEvent.GetPosY() << "\n";
			OutputDebugString(oss.str().c_str());
		}
	}
#pragma endregion CameraRotation

#pragma region CameraMovement
	const float cameraSpeed = 3.f;
	if (mRenderWindow.kbd.IsKeyPressed('W'))
	{
		mRenderWindow.GetGfx().mCamera.AdjustPosition(mRenderWindow.GetGfx().mCamera.GetForwardVector() * cameraSpeed * mTimer.DeltaTime());
	}
	if (mRenderWindow.kbd.IsKeyPressed('S'))
	{
		mRenderWindow.GetGfx().mCamera.AdjustPosition(mRenderWindow.GetGfx().mCamera.GetBackwardVector() * cameraSpeed * mTimer.DeltaTime());
	}
	if (mRenderWindow.kbd.IsKeyPressed('D'))
	{
		mRenderWindow.GetGfx().mCamera.AdjustPosition(mRenderWindow.GetGfx().mCamera.GetRightVector() * cameraSpeed * mTimer.DeltaTime());
	}
	if (mRenderWindow.kbd.IsKeyPressed('A'))
	{
		mRenderWindow.GetGfx().mCamera.AdjustPosition(mRenderWindow.GetGfx().mCamera.GetLeftVector() * cameraSpeed * mTimer.DeltaTime());
	}
	if (mRenderWindow.kbd.IsKeyPressed('E'))
	{
		mRenderWindow.GetGfx().mCamera.AdjustPosition(0.0f, cameraSpeed * mTimer.DeltaTime(), 0.0f);
	}
	if (mRenderWindow.kbd.IsKeyPressed('Q'))
	{
		mRenderWindow.GetGfx().mCamera.AdjustPosition(0.0f, -cameraSpeed * mTimer.DeltaTime(), 0.0f);
	}
#pragma endregion CameraMovement
}

void Engine::UpdateScene(float DeltaTime)
{
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