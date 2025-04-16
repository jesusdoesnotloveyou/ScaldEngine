#include "Engine.h"
#include <sstream>

#include "../../Games/Katamari/KatamariPlayer.h"
#include "../../Data/ModelData.h"
#include "../../Graphics/Camera.h"
#include "../../Graphics/ThirdPersonCamera.h"

#include <random>
#include <ctime>

Engine::Engine()
    :
    mRenderWindow(1600, 900, "Direct3DApp")
{}

Engine::~Engine()
{
	// I am very sorry for this code...
	for (auto object : mSceneObjects)
	{
		if (object && object != Player) delete object;
	}

	if (Player) delete Player;
}

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
	ModelData* angryBirdModel	= new ModelData("./Data/Models/AngryBird/Angry_Bird.obj",			L"./Data/Models/AngryBird/Angry_Bird.png");
	ModelData* minionPigModel	= new ModelData("./Data/Models/MinionPig/MinionPig.obj",			L"./Data/Models/MinionPig/AngryBirdsChancho.png");
	ModelData* marvelModel		= new ModelData("./Data/Models/Marvel/Model.obj",					L"./Data/Textures/planks.png");
	ModelData* chairModel		= new ModelData("./Data/Models/Chair/monoblock_CHAIR.obj",			L"./Data/Textures/valakas.png");
	ModelData* tonyModel		= new ModelData("./Data/Models/Tony/Tony.obj",						L"./Data/Models/Tony/AngryBirdCeleste.png");
	ModelData* rockModel		= new ModelData("./Data/Models/Rock/rock.obj",						L"./Data/Textures/planks.png");
	ModelData* boxModel			= new ModelData("./Data/Models/Box/box2.obj",						L"./Data/Textures/planks.png");

	SceneGeometry* alien = new Actor(alienFemaleModel);
	alien->GetTransform()->SetScale(0.03f, 0.03f, 0.03f);
	alien->GetTransform()->SetPosition(-10.0f, 0.0f, 10.0f);
	alien->ObjectName = std::string("alien");
	alien->GetCollisionComponent()->SetRadius(2.0f);

	SceneGeometry* box = new Actor(boxModel);
	box->GetTransform()->SetScale(3.0f, 3.0f, 3.0f);
	box->GetTransform()->SetPosition(0.0f, 0.0f, 10.0f);
	box->ObjectName = std::string("box");
	box->GetCollisionComponent()->SetRadius(3.0f);

	SceneGeometry* chair = new Actor(chairModel);
	chair->GetTransform()->SetScale(1.0f, 1.0f, 1.0f);
	chair->GetTransform()->SetPosition(50.0f, 3.0f, 0.0f);
	chair->GetMovement()->SetRotAngle(60.0f);
	chair->GetMovement()->SetOrbitAngle(80.0f);
	chair->ObjectName = std::string("chair");
	chair->GetCollisionComponent()->SetRadius(4.0f);

	Player = new KatamariPlayer(tonyModel);
	Player->GetTransform()->SetScale(0.02f, 0.02f, 0.02f);
	Player->GetTransform()->SetPosition(40.0f, 0.0f, 0.0f);
	Player->ObjectName = std::string("Player");
	Player->GetCollisionComponent()->SetRadius(1.0f);

	SceneGeometry* pig = new Actor(minionPigModel);
	pig->GetTransform()->SetScale(0.02f, 0.02f, 0.02f);
	pig->GetTransform()->SetPosition(30.0f, 0.0f, 0.0f);
	pig->ObjectName = std::string("pig");
	pig->GetCollisionComponent()->SetRadius(3.0f);

	SceneGeometry* angryBird = new Actor(angryBirdModel);
	angryBird->GetTransform()->SetScale(0.02f, 0.02f, 0.02f);
	angryBird->GetTransform()->SetPosition(10.0f, 0.0f, 0.0f);
	angryBird->ObjectName = std::string("angryBird");
	angryBird->GetCollisionComponent()->SetRadius(4.0f);

	mSceneObjects.push_back(Player);
	mSceneObjects.push_back(angryBird);
	mSceneObjects.push_back(box);
	mSceneObjects.push_back(alien);
	mSceneObjects.push_back(chair);
	mSceneObjects.push_back(pig);
	
	mRenderWindow.GetGfx().InitSceneObjects(mSceneObjects);
}

void Engine::PollInput()
{
	while (!mRenderWindow.kbd.IsKeyEmpty()) {
		const auto keyEvent = mRenderWindow.kbd.ReadKey();
		unsigned char keyCode = keyEvent.GetCode();
	}

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
	const auto forward = XMVectorSetY(mRenderWindow.GetGfx().GetCamera()->GetForwardVector(), 0.0f);
	const auto right = XMVectorSetY(mRenderWindow.GetGfx().GetCamera()->GetRightVector(), 0.0f);
	if (mRenderWindow.kbd.IsKeyPressed('W'))
	{
		mSceneObjects[0]->AdjustPosition(forward * cameraSpeed * mTimer.DeltaTime());
	}
	if (mRenderWindow.kbd.IsKeyPressed('S'))
	{
		mSceneObjects[0]->AdjustPosition(-forward * cameraSpeed * mTimer.DeltaTime());
	}
	if (mRenderWindow.kbd.IsKeyPressed('D'))
	{
		mSceneObjects[0]->AdjustPosition(right * cameraSpeed * mTimer.DeltaTime());
	}
	if (mRenderWindow.kbd.IsKeyPressed('A'))
	{
		mSceneObjects[0]->AdjustPosition(-right * cameraSpeed * mTimer.DeltaTime());
	}
#pragma endregion CameraMovement
}

void Engine::UpdateScene(const ScaldTimer& st)
{
	for (auto sceneObject : mSceneObjects)
	{
		sceneObject->Update(st);
		
		if (sceneObject == Player) continue;
		// checks for collision should be here...
		if (const auto playerPawnCollision = Player->GetCollisionComponent())
		{
			if (const auto otherCollision = sceneObject->GetCollisionComponent())
			{
				if (!otherCollision->IsEnabled()) continue;
				if (playerPawnCollision->Intersects(otherCollision))
				{
					playerPawnCollision->OnCollisionOverlapSignature.Broadcast(otherCollision);
				}
			}
		}
	}
	mRenderWindow.GetGfx().GetCamera()->Update(st);

#if 0
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
#endif
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