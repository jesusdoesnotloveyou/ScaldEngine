#include "Engine.h"
#include <sstream>

#include "../../Games/Katamari/KatamariPlayer.h"
#include "../../Data/ModelData.h"
#include "../../Graphics/Camera.h"
#include "../../Graphics/ThirdPersonCamera.h"
#include "../../Objects/Geometry/Actor.h"
#include "../../Data/ModelData.h"
#include "../../Graphics/Light/Light.h"

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
	ModelData* boxModel			= new ModelData("./Data/Models/Box/box2.obj",						L"./Data/Textures/valakas.png");
	ModelData* rockModel		= new ModelData("./Data/Models/Rock/rock.obj",						L"./Data/Textures/planks.png");

#pragma region Light
	Light* light1 = new Light("./Data/Models/Light/light.obj");
	light1->GetTransform()->SetScale(1.0f, 1.0f, 1.0f);
	light1->GetTransform()->SetPosition(0.0f, 4.0f, 2.0f);
	light1->GetTransform()->SetRotation(-XM_PIDIV2, 0.0f, 0.0f);
	//light1->GetCollisionComponent()->DisableCollision();
	light1->SetPointLightParams(XMFLOAT4(1.0f, 1.0f, 1.0f, 0.9f), XMFLOAT4(1.0f, 1.0f, 1.0f, 5.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.1f, 0.1f));

	Light* light2 = new Light("./Data/Models/Light/light.obj");
	light2->GetTransform()->SetScale(1.0f, 1.0f, 1.0f);
	light2->GetTransform()->SetPosition(0.0f, 4.0f, 6.0f);
	light2->GetTransform()->SetRotation(-XM_PIDIV2, 0.0f, 0.0f);
	//light2->GetCollisionComponent()->DisableCollision();
	light2->SetPointLightParams(XMFLOAT4(1.0f, 1.0f, 1.0f, 0.9f), XMFLOAT4(1.0f, 1.0f, 1.0f, 5.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.1f, 0.1f));

	Light* light3 = new Light("./Data/Models/Light/light.obj");
	light3->GetTransform()->SetScale(1.0f, 1.0f, 1.0f);
	light3->GetTransform()->SetPosition(0.0f, 4.0f, 10.0f);
	light3->GetTransform()->SetRotation(-XM_PIDIV2, 0.0f, 0.0f);
	//light3->GetCollisionComponent()->DisableCollision();
	light3->SetPointLightParams(XMFLOAT4(1.0f, 1.0f, 1.0f, 0.9f), XMFLOAT4(1.0f, 1.0f, 1.0f, 5.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.1f, 0.1f));

	Light* light4 = new Light("./Data/Models/Light/light.obj");
	light4->GetTransform()->SetScale(1.0f, 1.0f, 1.0f);
	light4->GetTransform()->SetPosition(0.0f, 4.0f, 14.0f);
	light4->GetTransform()->SetRotation(-XM_PIDIV2, 0.0f, 0.0f);
	//light4->GetCollisionComponent()->DisableCollision();
	light4->SetPointLightParams(XMFLOAT4(1.0f, 1.0f, 1.0f, 0.9f), XMFLOAT4(0.0f, 1.0f, 1.0f, 5.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.1f, 0.1f));

	Light* light5 = new Light("./Data/Models/Light/light.obj");
	light5->GetTransform()->SetScale(1.0f, 1.0f, 1.0f);
	light5->GetTransform()->SetPosition(0.0f, 4.0f, 18.0f);
	light5->GetTransform()->SetRotation(-XM_PIDIV2, 0.0f, 0.0f);
	//light5->GetCollisionComponent()->DisableCollision();
	light5->SetPointLightParams(XMFLOAT4(1.0f, 1.0f, 1.0f, 0.9f), XMFLOAT4(1.0f, 0.0f, 1.0f, 5.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.1f, 0.1f));

	Light* light6 = new Light("./Data/Models/Light/light.obj");
	light6->GetTransform()->SetScale(1.0f, 1.0f, 1.0f);
	light6->GetTransform()->SetPosition(0.0f, 4.0f, 22.0f);
	light6->GetTransform()->SetRotation(-XM_PIDIV2, 0.0f, 0.0f);
	//light6->GetCollisionComponent()->DisableCollision();
	light6->SetPointLightParams(XMFLOAT4(1.0f, 1.0f, 1.0f, 0.9f), XMFLOAT4(1.0f, 1.0f, 0.0f, 5.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.1f, 0.1f));

	Light* light7 = new Light("./Data/Models/Light/light.obj");
	light7->GetTransform()->SetScale(1.0f, 1.0f, 1.0f);
	light7->GetTransform()->SetPosition(0.0f, 4.0f, 26.0f);
	light7->GetTransform()->SetRotation(-XM_PIDIV2, 0.0f, 0.0f);
	//light7->GetCollisionComponent()->DisableCollision();
	light7->SetPointLightParams(XMFLOAT4(1.0f, 1.0f, 1.0f, 0.9f), XMFLOAT4(1.0f, 1.0f, 1.0f, 5.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.1f, 0.1f));

	Light* light8 = new Light("./Data/Models/Light/light.obj");
	light8->GetTransform()->SetScale(1.0f, 1.0f, 1.0f);
	light8->GetTransform()->SetPosition(0.0f, 4.0f, 30.0f);
	light8->GetTransform()->SetRotation(-XM_PIDIV2, 0.0f, 0.0f);
	//light8->GetCollisionComponent()->DisableCollision();
	light8->SetPointLightParams(XMFLOAT4(1.0f, 1.0f, 1.0f, 0.9f), XMFLOAT4(0.0f, 0.0f, 1.0f, 5.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.1f, 0.1f));

	Light* light9 = new Light("./Data/Models/Light/light.obj");
	light9->GetTransform()->SetScale(1.0f, 1.0f, 1.0f);
	light9->GetTransform()->SetPosition(0.0f, 4.0f, 34.0f);
	light9->GetTransform()->SetRotation(-XM_PIDIV2, 0.0f, 0.0f);
	//light9->GetCollisionComponent()->DisableCollision();
	light9->SetPointLightParams(XMFLOAT4(1.0f, 1.0f, 1.0f, 0.9f), XMFLOAT4(0.0f, 1.0f, 0.0f, 5.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.1f, 0.1f));

	Light* light10 = new Light("./Data/Models/Light/light.obj");
	light10->GetTransform()->SetScale(1.0f, 1.0f, 1.0f);
	light10->GetTransform()->SetPosition(0.0f, 4.0f, 38.0f);
	light10->GetTransform()->SetRotation(-XM_PIDIV2, 0.0f, 0.0f);
	//light10->GetCollisionComponent()->DisableCollision();
	light10->SetPointLightParams(XMFLOAT4(1.0f, 1.0f, 1.0f, 0.9f), XMFLOAT4(1.0f, 0.0f, 0.0f, 5.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.1f, 0.1f));

	mLights.push_back(light1);
	mLights.push_back(light2);
	mLights.push_back(light3);
	mLights.push_back(light4);
	mLights.push_back(light5);
	mLights.push_back(light6);
	mLights.push_back(light7);
	mLights.push_back(light8);
	mLights.push_back(light9);
	mLights.push_back(light10);
	// could be moved to setup i suppose
	mRenderWindow.GetGfx().AddLightSourceParams(light1->GetPointLightParams());
	mRenderWindow.GetGfx().AddLightSourceParams(light2->GetPointLightParams());
	mRenderWindow.GetGfx().AddLightSourceParams(light3->GetPointLightParams());
	mRenderWindow.GetGfx().AddLightSourceParams(light4->GetPointLightParams());
	mRenderWindow.GetGfx().AddLightSourceParams(light5->GetPointLightParams());
	mRenderWindow.GetGfx().AddLightSourceParams(light6->GetPointLightParams());
	mRenderWindow.GetGfx().AddLightSourceParams(light7->GetPointLightParams());
	mRenderWindow.GetGfx().AddLightSourceParams(light8->GetPointLightParams());
	mRenderWindow.GetGfx().AddLightSourceParams(light9->GetPointLightParams());
	mRenderWindow.GetGfx().AddLightSourceParams(light10->GetPointLightParams());
#pragma endregion Light

	SceneGeometry* alien = new Actor(alienFemaleModel);
	alien->GetTransform()->SetScale(0.03f, 0.03f, 0.03f);
	alien->GetTransform()->SetPosition(-15.0f, 0.0f, 10.0f);
	alien->ObjectName = std::string("alien");
	alien->GetCollisionComponent()->SetRadius(2.0f);

	SceneGeometry* box = new Actor(boxModel);
	box->GetTransform()->SetScale(3.0f, 3.0f, 3.0f);
	box->GetTransform()->SetPosition(0.0f, 0.0f, 15.0f);
	box->ObjectName = std::string("box");
	box->GetCollisionComponent()->SetRadius(3.0f);

	SceneGeometry* chair = new Actor(chairModel);
	chair->GetTransform()->SetScale(1.0f, 1.0f, 1.0f);
	chair->GetTransform()->SetPosition(50.0f, 2.3f, 0.0f);
	chair->ObjectName = std::string("chair");
	chair->GetCollisionComponent()->SetRadius(4.0f);

	Player = new KatamariPlayer(angryBirdModel);
	Player->GetTransform()->SetScale(0.02f, 0.02f, 0.02f);
	Player->GetTransform()->SetPosition(10.0f, 1.9f, 20.0f);
	Player->ObjectName = std::string("Player");
	Player->GetCollisionComponent()->SetRadius(4.0f);

	SceneGeometry* pig = new Actor(minionPigModel);
	pig->GetTransform()->SetScale(0.02f, 0.02f, 0.02f);
	pig->GetTransform()->SetPosition(30.0f, 0.3f, 0.0f);
	pig->ObjectName = std::string("pig");
	pig->GetCollisionComponent()->SetRadius(3.0f);

	SceneGeometry* angryBird = new Actor(tonyModel);
	angryBird->GetTransform()->SetScale(0.02f, 0.02f, 0.02f);
	angryBird->GetTransform()->SetPosition(10.0f, 0.0f, 0.0f);
	angryBird->ObjectName = std::string("angryBird");
	angryBird->GetCollisionComponent()->SetRadius(2.0f);

	SceneGeometry* rockFloor = new Actor(rockModel);
	rockFloor->GetTransform()->SetScale(5.0f, 0.1f, 5.0f);
	rockFloor->GetTransform()->SetPosition(0.0f, -1.5f, 0.0f);
	rockFloor->ObjectName = std::string("floor");
	rockFloor->GetCollisionComponent()->DisableCollision();

	mSceneObjects.push_back(Player);
	mSceneObjects.push_back(angryBird);
	mSceneObjects.push_back(box);
	mSceneObjects.push_back(alien);
	mSceneObjects.push_back(chair);
	mSceneObjects.push_back(pig);
	mSceneObjects.push_back(rockFloor);
	mSceneObjects.push_back(light1);
	mSceneObjects.push_back(light2);
	mSceneObjects.push_back(light3);
	mSceneObjects.push_back(light4);
	mSceneObjects.push_back(light5);
	mSceneObjects.push_back(light6);
	mSceneObjects.push_back(light7);
	mSceneObjects.push_back(light8);
	mSceneObjects.push_back(light9);
	mSceneObjects.push_back(light10);
	
	mRenderWindow.GetGfx().InitSceneObjects(mSceneObjects);

#pragma region PlayerInputDelegates
	mRenderWindow.kbd.OnKeyPressedEvent.AddRaw(Player->GetMovement(), &KatamariMovementComponent::OnKeyPressed);
	mRenderWindow.kbd.OnKeyReleasedEvent.AddRaw(Player->GetMovement(), &KatamariMovementComponent::OnKeyReleased);
#pragma endregion PlayerInputDelegates
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

#pragma region PlayerMovement
	// Camera forward without Y (XoZ)
	auto forward = XMVectorSetY(mRenderWindow.GetGfx().GetCamera()->GetForwardVector(), 0.0f);
	forward = XMVector3Normalize(forward);
	Player->SetForwardVector(forward);

	// Camera right without Y (XoZ)
	auto right = XMVectorSetY(mRenderWindow.GetGfx().GetCamera()->GetRightVector(), 0.0f);
	right = XMVector3Normalize(right);
	Player->SetRightVector(right);
	
	if (mRenderWindow.kbd.IsKeyPressed('C'))
	{
		XMVECTOR newLightObjectPosition = mRenderWindow.GetGfx().GetCamera()->GetPosition();
		newLightObjectPosition += mRenderWindow.GetGfx().GetCamera()->GetForwardVector() * 2;
		mSceneObjects.back()->SetPosition(newLightObjectPosition);
	}
	if (mRenderWindow.kbd.IsKeyPressed(VK_SPACE) && !Player->IsFalling())
	{
		Player->Jump();
	}
#pragma endregion PlayerMovement
}

void Engine::UpdateScene(const ScaldTimer& st)
{
	for (auto sceneObject : mSceneObjects)
	{
		sceneObject->Update(st);
// Very inefficient code I suppose
#pragma region Collision
		if (sceneObject == Player) continue;
		// checks for collision should be here...
		if (const auto playerPawnCollision = Player->GetCollisionComponent()) {
			if (const auto otherCollision = sceneObject->GetCollisionComponent()) {
				if (!otherCollision->IsEnabled()) continue;
				if (playerPawnCollision->Intersects(otherCollision)) {
					playerPawnCollision->OnCollisionOverlapSignature.Broadcast(otherCollision);
				}
			}
		}
#pragma endregion Collision
	}
	mRenderWindow.GetGfx().GetCamera()->Update(st);
	// LightManager->Update()
	mRenderWindow.GetGfx().UpdateLightParams(mLights);

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

#pragma region LightPosDebug
	std::ostringstream oss;
	const auto LightPos = mSceneObjects[1]->GetPosition();
	oss << "Object's position: " << XMVectorGetX(LightPos) << ", " << XMVectorGetY(LightPos) << ", " << XMVectorGetZ(LightPos) << "\n";
	OutputDebugString(oss.str().c_str());
#pragma endregion LightPosDebug

#pragma region PlayerPosDebug
	const auto PlayerPos = Player->GetPosition();
	oss << "Player's position: " << XMVectorGetX(PlayerPos) << ", " << XMVectorGetY(PlayerPos) << ", " << XMVectorGetZ(PlayerPos) << "\n";
	OutputDebugString(oss.str().c_str());
#pragma endregion PlayerPosDebug
#endif

#pragma region LightPosDebug
	std::ostringstream oss;
	const auto LightPos = mSceneObjects[1]->GetPosition();
	oss << "Object's position: " << XMVectorGetX(LightPos) << ", " << XMVectorGetY(LightPos) << ", " << XMVectorGetZ(LightPos) << "\n";
	OutputDebugString(oss.str().c_str());
#pragma endregion LightPosDebug

#pragma region PlayerPosDebug
	const auto PlayerPos = Player->GetPosition();
	oss << "Player's position: " << XMVectorGetX(PlayerPos) << ", " << XMVectorGetY(PlayerPos) << ", " << XMVectorGetZ(PlayerPos) << "\n";
	OutputDebugString(oss.str().c_str());
#pragma endregion PlayerPosDebug
}

void Engine::RenderFrame(const ScaldTimer& st)
{
	//const float color = static_cast<float>(sin(mTimer.DeltaTime()) + 1.0f);
	mRenderWindow.GetGfx().ClearBuffer(0.0f);
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