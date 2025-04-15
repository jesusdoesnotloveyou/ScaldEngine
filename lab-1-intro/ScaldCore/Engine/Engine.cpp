#include "Engine.h"
#include <sstream>

#include "../../Graphics/Camera.h"
#include "../../Graphics/ThirdPersonCamera.h"
#include "../../Objects/Geometry/Actor.h"
#include "../../Data/ModelData.h"
#include "../../Objects/Light/Light.h"

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

	Light* light = new Light("./Data/Models/Light/light.obj");
	light->GetTransform()->SetScale(1.0f, 1.0f, 1.0f);
	light->GetTransform()->SetPosition(0.0f, 0.0f, 3.0f);

	SceneGeometry* alien = new Actor(alienFemaleModel);
	alien->GetTransform()->SetScale(0.025f, 0.025f, 0.025f);
	alien->GetMovement()->SetRotAngle(60.0f);

	SceneGeometry* box = new Actor(boxModel);
	box->GetTransform()->SetScale(1.0f, 1.0f, 1.0f);
	box->GetTransform()->SetPosition(10.0f, 0.0f, 0.0f);

	SceneGeometry* chair = new Actor(chairModel);
	chair->GetTransform()->SetScale(1.0f, 1.0f, 1.0f);
	chair->GetTransform()->SetRotation(0.0f, XM_PIDIV2, 0.0f);
	chair->GetTransform()->SetPosition(20.0f, 0.0f, 0.0f);
	chair->GetTransform()->SetParentTransform(alien->GetTransform());
	chair->GetMovement()->SetRotAngle(60.0f);
	chair->GetMovement()->SetOrbitAngle(80.0f);

	mSceneObjects.push_back(box);
	mSceneObjects.push_back(alien);
	mSceneObjects.push_back(chair);
	mSceneObjects.push_back(light);
	
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
			mRenderWindow.GetGfx().GetCamera()->AdjustRotation((float)mouseEvent.GetPosY() * 0.01f, (float)mouseEvent.GetPosX() * 0.01f, 0.0f);
		}
	}
#pragma endregion CameraRotation

#pragma region CameraMovement
	const float cameraSpeed = 15.f;
	const auto forward = (XMVectorSetY(mRenderWindow.GetGfx().GetCamera()->GetForwardVector(), 0.0f));
	const auto right = (XMVectorSetY(mRenderWindow.GetGfx().GetCamera()->GetRightVector(), 0.0f));
	if (mRenderWindow.kbd.IsKeyPressed('W'))
	{
		mSceneObjects[0]->GetTransform()->AdjustPosition(forward * cameraSpeed * mTimer.DeltaTime());
	}
	if (mRenderWindow.kbd.IsKeyPressed('S'))
	{
		mSceneObjects[0]->GetTransform()->AdjustPosition(-forward * cameraSpeed * mTimer.DeltaTime());
	}
	if (mRenderWindow.kbd.IsKeyPressed('D'))
	{
		mSceneObjects[0]->GetTransform()->AdjustPosition(right * cameraSpeed * mTimer.DeltaTime());
	}
	if (mRenderWindow.kbd.IsKeyPressed('A'))
	{
		mSceneObjects[0]->GetTransform()->AdjustPosition(-right * cameraSpeed * mTimer.DeltaTime());
	}
	if (mRenderWindow.kbd.IsKeyPressed('C'))
	{
		mSceneObjects.back()->GetTransform()->SetPosition(mRenderWindow.GetGfx().GetCamera()->GetPosition());
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
	mRenderWindow.GetGfx().GetCamera()->Update(st);
	mSceneObjects[2]->AdjustRotation(0.0f, 1.0f * st.DeltaTime(), 0.0f);

#pragma region CameraPosDebug
	std::ostringstream oss;
	const auto CameraPos = mRenderWindow.GetGfx().GetCamera()->GetPosition();
	oss << "Camera's position: " << XMVectorGetX(CameraPos) << ", " << XMVectorGetY(CameraPos) << ", " << XMVectorGetZ(CameraPos) << "\n"
		<< "Camera forward vector: " << XMVectorGetX(mRenderWindow.GetGfx().GetCamera()->GetForwardVector()) << ", "
		<< XMVectorGetY(mRenderWindow.GetGfx().GetCamera()->GetForwardVector()) << ", "
		<< XMVectorGetZ(mRenderWindow.GetGfx().GetCamera()->GetForwardVector()) << "\n"
		<< "Character forward vector: " << XMVectorGetX(mSceneObjects[0]->GetForwardVector()) << ", "
		<< XMVectorGetY(mSceneObjects[0]->GetForwardVector()) << ", "
		<< XMVectorGetZ(mSceneObjects[0]->GetForwardVector()) << "\n";
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