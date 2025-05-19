#include "Engine.h"
#include "../../Data/ModelData.h"
#include "../../Games/Katamari/KatamariPlayer.h"
#include "../../Graphics/Camera/ThirdPersonCamera.h"
#include "../../Graphics/Light/PointLight.h"
#include "../../Graphics/Light/DirectionalLight.h"

#include <sstream>

Engine::Engine()
    :
    mRenderWindow(1600, 900, "Direct3DApp")
{}

Engine::~Engine()
{
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
	ModelData* chairModel		= new ModelData("./Data/Models/Chair/monoblock_CHAIR.obj",			L"./Data/Textures/planks.png");
	ModelData* tonyModel		= new ModelData("./Data/Models/Tony/Tony.obj",						L"./Data/Models/Tony/AngryBirdCeleste.png");
	ModelData* boxModel			= new ModelData("./Data/Models/Box/box2.obj",						L"./Data/Textures/brick.png");
	ModelData* rockModel		= new ModelData("./Data/Models/Rock/rock.obj",						L"./Data/Textures/planks.png");

#pragma region Light
#pragma region PointLight
	/*PointLight* light1 = new PointLight("./Data/Models/Light/light.obj");
	light1->GetTransform()->SetPosition(0.0f, 4.0f, 2.0f);
	light1->GetTransform()->SetRotation(-XM_PIDIV2, 0.0f, 0.0f);
	light1->SetAmbientColor(1.0f, 1.0f, 1.0f, 0.0f);
	light1->SetDiffuseColor(1.0f, 0.0f, 0.0f, 5.0f);
	light1->SetAttenuation(1.0f, 0.1f, 0.1f);

	PointLight* light2 = new PointLight("./Data/Models/Light/light.obj");
	light2->GetTransform()->SetPosition(0.0f, 4.0f, 6.0f);
	light2->GetTransform()->SetRotation(-XM_PIDIV2, 0.0f, 0.0f);
	light2->SetAmbientColor(1.0f, 1.0f, 1.0f, 0.0f);
	light2->SetDiffuseColor(0.0f, 0.0f, 1.0f, 5.0f);
	light2->SetAttenuation(1.0f, 0.1f, 0.1f);*/
#pragma endregion PointLight

#pragma region DirectionalLight
	DirectionalLight* directionalLight = new DirectionalLight("./Data/Models/Light/light.obj");
	directionalLight->GetTransform()->SetPosition(10.0f, 50.0f, 100.0f);
	directionalLight->GetCollisionComponent()->DisableCollision();
	directionalLight->SetAmbientColor(1.0f, 1.0f, 1.0f, 1.0f);
	directionalLight->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	// opposite to dir light pos vector
	directionalLight->SetDirection(-10.0f, -50.0f, -100.0f);
#pragma endregion DirectionalLight

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

	SceneGeometry* rock = new Actor(rockModel);
	rock->GetTransform()->SetScale(10.0f, 0.1f, 10.0f);
	rock->GetTransform()->SetPosition(0.0f, -1.7f, 0.0f);
	rock->ObjectName = std::string("rock");
	rock->GetCollisionComponent()->DisableCollision();

	mRenderWindow.GetGfx().AddToRenderPool(Player);
	mRenderWindow.GetGfx().AddToRenderPool(angryBird);
	mRenderWindow.GetGfx().AddToRenderPool(box);
	mRenderWindow.GetGfx().AddToRenderPool(alien);
	mRenderWindow.GetGfx().AddToRenderPool(chair);
	mRenderWindow.GetGfx().AddToRenderPool(pig);
	mRenderWindow.GetGfx().AddToRenderPool(rock);

#pragma region DirectionalLight
	mRenderWindow.GetGfx().AddToRenderPool(directionalLight);
#pragma endregion DirectionalLight
#pragma region PointLights
	/*mRenderWindow.GetGfx().AddToRenderPool(light1);
	mRenderWindow.GetGfx().AddToRenderPool(light2);*/
#pragma endregion PointLights
	
	mRenderWindow.GetGfx().InitSceneObjects();

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
	// @todo: refactoring
	// Camera forward without Y (XoZ)
	auto forward = XMVectorSetY(mRenderWindow.GetGfx().GetCamera()->GetForwardVector(), 0.0f);
	forward = XMVector3Normalize(forward);
	Player->SetForwardVector(forward);

	// Camera right without Y (XoZ)
	auto right = XMVectorSetY(mRenderWindow.GetGfx().GetCamera()->GetRightVector(), 0.0f);
	right = XMVector3Normalize(right);
	Player->SetRightVector(right);

	if (mRenderWindow.kbd.IsKeyPressed(VK_SPACE) && !Player->IsFalling())
	{
		Player->Jump();
	}
#pragma endregion PlayerMovement
}

void Engine::UpdateScene(const ScaldTimer& st)
{
	for (auto sceneObject : mRenderWindow.GetGfx().mRenderObjects)
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
	mRenderWindow.GetGfx().Update(st);
}

void Engine::RenderFrame(const ScaldTimer& st)
{
	//const float color = static_cast<float>(sin(mTimer.DeltaTime()) + 1.0f);
	mRenderWindow.GetGfx().ClearBuffer(0.0f);
	mRenderWindow.GetGfx().DrawScene();
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