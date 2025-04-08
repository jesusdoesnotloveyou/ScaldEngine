#include "Engine.h"
#include <sstream>

#include "../../Objects/Geometry/Actor.h"
#include "../../Data/ModelData.h"
#include "../../Graphics/Camera.h"
#include "../../Graphics/ThirdPersonCamera.h"

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
		UpdateScene(mTimer);
		RenderFrame(mTimer);
	}
}

void Engine::SetupScene()
{
	ModelData* alienFemaleModel = new ModelData("./Data/Models/AlienFemale/Alien_Female_Lores.obj", L"./Data/Textures/brick.png");
	ModelData* boxModel = new ModelData("./Data/Models/Box/box2.obj", L"./Data/Textures/planks.png");
	ModelData* chairModel = new ModelData("./Data/Models/Chair/monoblock_CHAIR.obj", L"./Data/Textures/valakas.png");
	ModelData* rock = new ModelData("./Data/Models/Rock/rock.obj", L"./Data/Textures/planks.png");
	ModelData* marvel = new ModelData("./Data/Models/Marvel/Model.obj", L"./Data/Textures/planks.png");

	SceneGeometry* alien = new Actor(alienFemaleModel);
	alien->GetTransform()->SetScale(0.25f, 0.25f, 0.25f);
	alien->GetMovement()->SetRotAngle(60.0f);

	SceneGeometry* box = new Actor(boxModel);
	box->GetTransform()->SetScale(1.0f, 1.0f, 1.0f);
	box->GetTransform()->SetPosition(30.0f, 0.0f, 0.0f);
	box->GetTransform()->SetParentTransform(alien->GetTransform());
	box->GetMovement()->SetRotAngle(60.0f);
	box->GetMovement()->SetOrbitAngle(80.0f);

	SceneGeometry* chair = new Actor(chairModel);
	chair->GetTransform()->SetScale(1.0f, 1.0f, 1.0f);
	chair->GetTransform()->SetPosition(50.0f, 0.0f, 0.0f);;

	mSceneObjects.push_back(alien);
	mSceneObjects.push_back(box);
	mSceneObjects.push_back(chair);
	
	mRenderWindow.GetGfx().InitSceneObjects(mSceneObjects);
}

void Engine::PollInput()
{
	while (!mRenderWindow.kbd.IsKeyEmpty()) {
		const auto keyEvent = mRenderWindow.kbd.ReadKey();
		unsigned char keyCode = keyEvent.GetCode();
	}

#pragma region FPSCamera
#pragma region CameraRotation
	const auto mouseEvent = mRenderWindow.mouse.Read();
	if (mRenderWindow.mouse.IsRightPressed()) {
		if (mouseEvent.GetType() == Mouse::Event::Type::RawMove) 
		{
			mRenderWindow.GetGfx().mTPCamera->AdjustRotation((float)mouseEvent.GetPosY() * 0.01f, (float)mouseEvent.GetPosX() * 0.01f, 0.0f);
		}
	}
#pragma endregion CameraRotation

#pragma region CameraMovement
	const float cameraSpeed = 15.f;
	if (mRenderWindow.kbd.IsKeyPressed('W'))
	{
		mRenderWindow.GetGfx().mTPCamera->AdjustPosition(mRenderWindow.GetGfx().mTPCamera->GetForwardVector() * cameraSpeed * mTimer.DeltaTime());
		mSceneObjects[1]->GetTransform()->AdjustPosition(mRenderWindow.GetGfx().mTPCamera->GetForwardVector() * cameraSpeed * mTimer.DeltaTime());
	}
	if (mRenderWindow.kbd.IsKeyPressed('S'))
	{
		mRenderWindow.GetGfx().mTPCamera->AdjustPosition(mRenderWindow.GetGfx().mTPCamera->GetBackVector() * cameraSpeed * mTimer.DeltaTime());
		mSceneObjects[1]->GetTransform()->AdjustPosition(mRenderWindow.GetGfx().mTPCamera->GetBackVector() * cameraSpeed * mTimer.DeltaTime());
	}
	if (mRenderWindow.kbd.IsKeyPressed('D'))
	{
		mRenderWindow.GetGfx().mTPCamera->AdjustPosition(mRenderWindow.GetGfx().mTPCamera->GetRightVector() * cameraSpeed * mTimer.DeltaTime());
		mSceneObjects[1]->GetTransform()->AdjustPosition(mRenderWindow.GetGfx().mTPCamera->GetRightVector() * cameraSpeed * mTimer.DeltaTime());
	}
	if (mRenderWindow.kbd.IsKeyPressed('A'))
	{
		mRenderWindow.GetGfx().mTPCamera->AdjustPosition(mRenderWindow.GetGfx().mTPCamera->GetLeftVector() * cameraSpeed * mTimer.DeltaTime());
		mSceneObjects[1]->GetTransform()->AdjustPosition(mRenderWindow.GetGfx().mTPCamera->GetLeftVector() * cameraSpeed * mTimer.DeltaTime());
	}
	XMVECTOR up = XMVector3Cross(mRenderWindow.GetGfx().mTPCamera->GetLeftVector(), mRenderWindow.GetGfx().mTPCamera->GetForwardVector());
	if (mRenderWindow.kbd.IsKeyPressed('E'))
	{
		mRenderWindow.GetGfx().mTPCamera->AdjustPosition(up * cameraSpeed * mTimer.DeltaTime());
		mSceneObjects[1]->GetTransform()->AdjustPosition(up * cameraSpeed * mTimer.DeltaTime());
	}
	if (mRenderWindow.kbd.IsKeyPressed('Q'))
	{
		mRenderWindow.GetGfx().mTPCamera->AdjustPosition(-up * cameraSpeed * mTimer.DeltaTime());
		mSceneObjects[1]->GetTransform()->AdjustPosition(-up * cameraSpeed * mTimer.DeltaTime());
	}
#pragma endregion CameraMovement
#pragma endregion FPSCamera
}

void Engine::UpdateScene(const ScaldTimer& st)
{
	for (auto sceneObject : mSceneObjects)
	{
		sceneObject->Update(st);
	}

#pragma region CameraPosDebug
	std::ostringstream oss;
	const auto CameraPos = mRenderWindow.GetGfx().mTPCamera->GetPosition();
	oss << "Camera's position: " << XMVectorGetX(CameraPos) << ", " << XMVectorGetY(CameraPos) << ", " << XMVectorGetZ(CameraPos) << "\n";
	OutputDebugString(oss.str().c_str());
#pragma endregion CameraPosDebug
}

void Engine::RenderFrame(const ScaldTimer& st)
{
	//const float color = static_cast<float>(sin(mTimer.DeltaTime()) + 1.0f);
	mRenderWindow.GetGfx().ClearBuffer(0.1f);
	mRenderWindow.GetGfx().DrawScene(mSceneObjects);
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