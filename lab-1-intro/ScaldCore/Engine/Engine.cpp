#include "Engine.h"
#include <sstream>

#include "../../Objects/Geometry/Rectangle.h"
#include "../../Objects/Geometry/Circle.h"
#include "../../Objects/Geometry/Star.h"

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
		// process all messages pending, but to not block
		if (const auto eCode = RenderWindow::ProcessMessages())
		{
		 	// if return optional has a value, means we're quitting
			return *eCode;
		}
		// otherwise
		mTimer.Tick();

		CalculateFrameStats();
		PollInput();
		UpdateScene(mTimer.DeltaTime());
		RenderFrame();
	}
}

void Engine::SetupScene()
{
	STransform firstRocketTransform;
	std::vector<Vertex> firstRocketVertex = {
		{ DirectX::XMFLOAT4(-0.95f, 0.2f, 0.5f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(-0.93f, 0.2f, 0.5f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(-0.93f, -0.2f, 0.5f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(-0.95f, -0.2f, 0.5f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
	};

	STransform secondRocketTransform;
	std::vector<Vertex> secondRocketVertex = {
		{ DirectX::XMFLOAT4(0.93f, 0.2f, 0.5f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(0.95f, 0.2f, 0.5f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(0.95f, -0.2f, 0.5f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(0.93f, -0.2f, 0.5f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
	};

	PrimitiveGeometry* firstRocket = new Rect(firstRocketVertex);
	PrimitiveGeometry* secondRocke = new Rect(secondRocketVertex);
	PrimitiveGeometry* ball = new Circle(0.03f);
	GameObjects.push_back(firstRocket);
	GameObjects.push_back(secondRocke);
	GameObjects.push_back(ball);

	for (auto geometry : GameObjects)
	{
		if (!geometry) continue;
		geometry->Initialize(mRenderWindow.GetGfx().GetDevice(), mRenderWindow.GetGfx().GetDeviceContext());
	}
}

void Engine::PollInput()
{
	float speed = 0.05f;
	while (!mRenderWindow.kbd.IsKeyEmpty())
	{
		const auto e = mRenderWindow.kbd.ReadKey();
		switch (e.GetCode())
		{
			// W
		case 87:
		{
			//GameObjects[0]->GetMovementComponent()->SetSpeed(DirectX::XMFLOAT3(0.0f, speed, 0.0f));
			if (GameObjects[0]->posY > 1.f) break;
			GameObjects[0]->posY += speed;
			break;
		}
			// S
		case 83:
			if (GameObjects[0]->posY < -1.f) break;
			GameObjects[0]->posY -= speed;
			break;
			// up
		case 38:
		{
			if (GameObjects[1]->posY > 1.f) break;
			GameObjects[1]->posY += speed;
			break;
		}
		// down
		case 40:
			if (GameObjects[1]->posY < -1.f) break;
			GameObjects[1]->posY -= speed;
			break;
		}
	}

	// test logic that might be delete
	/*static int i = 0;
	while (!mRenderWindow.mouse.IsEmpty())
	{
		const auto e = mRenderWindow.mouse.Read();
		switch (e.GetType())
		{
		case Mouse::Event::Type::Move:
		{
			std::ostringstream oss;
			oss << "Mouse Position: (" << e.GetPosX() << "," << e.GetPosY() << ")";
			mRenderWindow.SetTitle(oss.str());
			break;
		}
		case Mouse::Event::Type::Leave:
		{
			mRenderWindow.SetTitle("Left window region!");
			break;
		}
		case Mouse::Event::Type::WheelUp:
			i++;
			{
				std::ostringstream oss;
				oss << "Wheel Up: " << i;
				mRenderWindow.SetTitle(oss.str());
			}
			break;
		case Mouse::Event::Type::WheelDown:
			i--;
			{
				std::ostringstream oss;
				oss << "Wheel Down: " << i;
				mRenderWindow.SetTitle(oss.str());
			}
			break;
		}
	}*/
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