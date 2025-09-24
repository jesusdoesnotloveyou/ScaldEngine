#include "stdafx.h"
#include "Engine.h"
#include "Data/ModelData.h"
#include "Games/Katamari/KatamariPlayer.h"
#include "Graphics/Camera/ThirdPersonCamera.h"
#include "Graphics/Light/PointLight.h"
#include "Graphics/Light/DirectionalLight.h"

Engine::Engine()
    : mRenderWindow(1600, 900, "Direct3DApp")
{}

Engine::~Engine()
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
	m_models["alien"] = std::make_unique<ModelData>("./Data/Models/AlienFemale/AlienFemale.obj", L"./Data/Textures/brick.png");
	m_models["angrybird"] = std::make_unique<ModelData>("./Data/Models/AngryBird/AngryBird.obj", L"./Data/Models/AngryBird/AngryBird.png");
	m_models["minion"] = std::make_unique<ModelData>("./Data/Models/MinionPig/MinionPig.obj", L"./Data/Models/MinionPig/MinionPig.png");
	m_models["chair"] = std::make_unique<ModelData>("./Data/Models/Chair/PlasticChair.obj", L"./Data/Textures/valakas.png");
	m_models["tony"] = std::make_unique<ModelData>("./Data/Models/Tony/Tony.obj", L"./Data/Models/Tony/Tony.png");
	m_models["box"] = std::make_unique<ModelData>("./Data/Models/Box/Box.obj", L"./Data/Models/Box/CardboardBox.png");
	m_models["rock"] = std::make_unique<ModelData>("./Data/Models/Rock/Rock.obj", L"./Data/Textures/planks.png");
	
#pragma region Light
#pragma region PointLight
	/*auto light1 = std::make_unique<PointLight>("./Data/Models/Light/light.obj");
	light1->GetTransform()->SetPosition(0.0f, 4.0f, 2.0f);
	light1->GetTransform()->SetRotation(-XM_PIDIV2, 0.0f, 0.0f);
	light1->SetAmbientColor(1.0f, 1.0f, 1.0f, 0.0f);
	light1->SetDiffuseColor(1.0f, 0.0f, 0.0f, 5.0f);
	light1->SetAttenuation(1.0f, 0.1f, 0.1f);

	auto light2 = std::make_unique<PointLight>("./Data/Models/Light/light.obj");
	light2->GetTransform()->SetPosition(0.0f, 4.0f, 6.0f);
	light2->GetTransform()->SetRotation(-XM_PIDIV2, 0.0f, 0.0f);
	light2->SetAmbientColor(1.0f, 1.0f, 1.0f, 0.0f);
	light2->SetDiffuseColor(0.0f, 0.0f, 1.0f, 5.0f);
	light2->SetAttenuation(1.0f, 0.1f, 0.1f);*/
#pragma endregion PointLight

#pragma region DirectionalLight
	auto directionalLight = std::make_shared<DirectionalLight>("./Data/Models/Light/light.obj");
	directionalLight->GetTransform()->SetPosition(60.0f, 30.0f, 0.0f);
	directionalLight->GetCollisionComponent()->DisableCollision();
	directionalLight->SetAmbientColor(0.4f, 0.4f, 0.5f, 1.0f);
	directionalLight->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	directionalLight->GetPosition();
	// opposite to dir light pos vector
	directionalLight->SetDirection(-60.0f, -30.0f, 0.0f);
#pragma endregion DirectionalLight

#pragma endregion Light

	auto alien = std::make_shared<Actor>(m_models["alien"].get());
	alien->GetTransform()->SetScale(0.03f, 0.03f, 0.03f);
	alien->GetTransform()->SetPosition(-15.0f, 0.0f, 10.0f);
	alien->ObjectName = std::string("alien");
	alien->GetCollisionComponent()->SetRadius(2.0f);

	auto box = std::make_shared<Actor>(m_models["box"].get());
	box->GetTransform()->SetScale(3.0f, 3.0f, 3.0f);
	box->GetTransform()->SetPosition(0.0f, 0.0f, 15.0f);
	box->ObjectName = std::string("box");
	box->GetCollisionComponent()->SetRadius(3.0f);

	auto chair = std::make_shared<Actor>(m_models["chair"].get());
	chair->GetTransform()->SetScale(1.0f, 1.0f, 1.0f);
	chair->GetTransform()->SetPosition(50.0f, 2.3f, 0.0f);
	chair->ObjectName = std::string("chair");
	chair->GetCollisionComponent()->SetRadius(4.0f);

	m_player = std::make_shared<KatamariPlayer>(m_models["angrybird"].get());
	m_player->GetTransform()->SetScale(0.02f, 0.02f, 0.02f);
	m_player->GetTransform()->SetPosition(10.0f, 1.9f, 20.0f);
	m_player->ObjectName = std::string("Player");
	m_player->GetCollisionComponent()->SetRadius(4.0f);

	auto pig = std::make_shared<Actor>(m_models["minion"].get());
	pig->GetTransform()->SetScale(0.02f, 0.02f, 0.02f);
	pig->GetTransform()->SetPosition(30.0f, 0.3f, 0.0f);
	pig->ObjectName = std::string("minion");
	pig->GetCollisionComponent()->SetRadius(3.0f);

	auto tony = std::make_shared<Actor>(m_models["tony"].get());
	tony->GetTransform()->SetScale(0.02f, 0.02f, 0.02f);
	tony->GetTransform()->SetPosition(10.0f, 0.0f, 0.0f);
	tony->ObjectName = std::string("tony");
	tony->GetCollisionComponent()->SetRadius(2.0f);

	auto rockFloor = std::make_shared<Actor>(m_models["rock"].get());
	rockFloor->GetTransform()->SetScale(5.0f, 0.1f, 5.0f);
	rockFloor->GetTransform()->SetPosition(15.0f, -1.5f, 0.0f);
	rockFloor->ObjectName = std::string("rockFloor");
	rockFloor->GetCollisionComponent()->DisableCollision();

	mRenderWindow.GetGfx().AddPlayer(m_player);
	mRenderWindow.GetGfx().AddToRenderPool(m_player);
	mRenderWindow.GetGfx().AddToRenderPool(std::move(tony));
	mRenderWindow.GetGfx().AddToRenderPool(std::move(box));
	mRenderWindow.GetGfx().AddToRenderPool(std::move(alien));
	mRenderWindow.GetGfx().AddToRenderPool(std::move(chair));
	mRenderWindow.GetGfx().AddToRenderPool(std::move(pig));
	mRenderWindow.GetGfx().AddToRenderPool(std::move(rockFloor));

#pragma region DirectionalLight
	mRenderWindow.GetGfx().AddToRenderPool(std::move(directionalLight));
#pragma endregion DirectionalLight
#pragma region PointLights
	/*mRenderWindow.GetGfx().AddToRenderPool(std::move(light1));
	mRenderWindow.GetGfx().AddToRenderPool(std::move(light2));*/
#pragma endregion PointLights
	
	mRenderWindow.GetGfx().InitSceneObjects();

#pragma region PlayerInputDelegates
	mRenderWindow.kbd.OnKeyPressedEvent.AddRaw(m_player->GetMovement(), &KatamariMovementComponent::OnKeyPressed);
	mRenderWindow.kbd.OnKeyReleasedEvent.AddRaw(m_player->GetMovement(), &KatamariMovementComponent::OnKeyReleased);
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
	m_player->SetForwardVector(forward);

	// Camera right without Y (XoZ)
	auto right = XMVectorSetY(mRenderWindow.GetGfx().GetCamera()->GetRightVector(), 0.0f);
	right = XMVector3Normalize(right);
	m_player->SetRightVector(right);

	if (mRenderWindow.kbd.IsKeyPressed(VK_SPACE) && !m_player->IsFalling())
	{
		m_player->Jump();
	}
#pragma endregion PlayerMovement
}

void Engine::UpdateScene(const ScaldTimer& st)
{
	for (auto&& sceneObject : mRenderWindow.GetGfx().mRenderObjects)
	{
		sceneObject->Update(st);
// Very inefficient code I suppose
#pragma region Collision
		if (sceneObject == m_player) continue;
		// checks for collision should be here...
		if (const auto playerPawnCollision = m_player->GetCollisionComponent()) {
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