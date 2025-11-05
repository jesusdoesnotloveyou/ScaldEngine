#include "stdafx.h"
#include "Engine.h"
#include "Data/ModelData.h"
#include "Games/Katamari/KatamariPlayer.h"
#include "Graphics/Camera/ThirdPersonCamera.h"
#include "Graphics/Light/Light.h"

Engine::Engine()
    :
    mRenderWindow(1600, 900, "Direct3DApp")
{}

Engine::~Engine() {}

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
	m_models["alien"] = std::make_unique<ModelData>("./Data/Models/AlienFemale/Alien_Female_Lores.obj", L"./Data/Textures/brick.png");
	m_models["angrybird"] = std::make_unique<ModelData>("./Data/Models/AngryBird/Angry_Bird.obj", L"./Data/Models/AngryBird/Angry_Bird.png");
	m_models["minion"] = std::make_unique<ModelData>("./Data/Models/MinionPig/MinionPig.obj", L"./Data/Models/MinionPig/AngryBirdsChancho.png");
	m_models["chair"] = std::make_unique<ModelData>("./Data/Models/Chair/monoblock_CHAIR.obj", L"./Data/Textures/planks.png");
	m_models["tony"] = std::make_unique<ModelData>("./Data/Models/Tony/Tony.obj", L"./Data/Models/Tony/AngryBirdCeleste.png");
	m_models["box"] = std::make_unique<ModelData>("./Data/Models/Box/box2.obj", L"./Data/Textures/brick.png");
	m_models["rock"] = std::make_unique<ModelData>("./Data/Models/Rock/rock.obj", L"./Data/Textures/planks.png");

#pragma region Light
#pragma region PointLight
	auto pointLight1 = std::make_shared<PointLight>("./Data/Models/Light/light.obj");
	pointLight1->GetTransform()->SetPosition(20.0f, 4.0f, 60.0f);
	pointLight1->SetDiffuseColor(1.0f, 1.0f, 0.5f, 5.0f);
	pointLight1->SetAttenuation(1.0f, 0.5f, 1.1f);
	//pointLight1->SetRange(3.0f);

	auto pointLight2 = std::make_shared<PointLight>("./Data/Models/Light/light.obj");
	pointLight2->GetTransform()->SetPosition(60.0f, 4.0f, 60.0f);
	pointLight2->SetDiffuseColor(0.0f, 1.0f, 1.0f, 5.0f);
	pointLight2->SetAttenuation(1.0f, 0.7f, 1.8f);
	//pointLight2->SetRange(2.0f);

	auto pointLight3 = std::make_shared<PointLight>("./Data/Models/Light/light.obj");
	pointLight3->GetTransform()->SetPosition(0.0f, 4.0f, 60.0f);
	pointLight3->SetDiffuseColor(1.0f, 0.3f, 0.0f, 4.0f);
	pointLight3->SetAttenuation(1.0f, 0.5f, 2.0f);
	
	auto pointLight4 = std::make_shared<PointLight>("./Data/Models/Light/light.obj");
	pointLight4->GetTransform()->SetPosition(20.0f, 4.0f, 10.0f);
	pointLight4->SetDiffuseColor(0.5f, 1.0f, 0.5f, 5.0f);
	pointLight4->SetAttenuation(1.0f, 0.7f, 2.0f);

	auto pointLight5 = std::make_shared<PointLight>("./Data/Models/Light/light.obj");
	pointLight5->GetTransform()->SetPosition(30.0f, 4.0f, 30.0f);
	pointLight5->SetDiffuseColor(1.0f, 0.0f, 0.8f, 5.0f);
	pointLight5->SetAttenuation(1.0f, 0.5f, 2.0f);

	auto pointLight6 = std::make_shared<PointLight>("./Data/Models/Light/light.obj");
	pointLight6->GetTransform()->SetPosition(50.0f, 4.0f, 40.0f);
	pointLight6->SetDiffuseColor(0.2f, 1.0f, 0.8f, 5.0f);
	pointLight6->SetAttenuation(1.0f, 0.5f, 2.0f);

	auto pointLight7 = std::make_shared<PointLight>("./Data/Models/Light/light.obj");
	pointLight7->GetTransform()->SetPosition(40.0f, 4.0f, 50.0f);
	pointLight7->SetDiffuseColor(1.0f, 0.0f, 1.0f, 5.0f);
	pointLight7->SetAttenuation(1.0f, 0.5f, 2.0f);

	auto pointLight8 = std::make_shared<PointLight>("./Data/Models/Light/light.obj");
	pointLight8->GetTransform()->SetPosition(30.0f, 4.0f, 0.0f);
	pointLight8->SetDiffuseColor(0.2f, 0.2f, 1.0f, 4.0f);
	pointLight8->SetAttenuation(1.0f, 0.5f, 2.0f);
#pragma endregion PointLight
#pragma region SpotLight
	SpotLight* spotLight1 = new SpotLight("./Data/Models/Light/light.obj");
	spotLight1->GetTransform()->SetPosition(50.0f, 4.0f, 55.0f);
	spotLight1->SetDiffuseColor(0.0f, 1.0f, 0.0f, 1.0f);
	spotLight1->SetAttenuation(1.0f, 0.7f, 1.8f);
#pragma endregion SpotLight
#pragma region DirectionalLight
	auto directionalLight = std::make_shared<DirectionalLight>("./Data/Models/Light/light.obj");
	directionalLight->GetTransform()->SetPosition(10.0f, 50.0f, 100.0f);
	directionalLight->GetCollisionComponent()->DisableCollision();
	directionalLight->SetAmbientColor(0.25f, 0.25f, 0.35f, 1.0f);
	directionalLight->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	// opposite to dir light pos vector
	directionalLight->SetDirection(-10.0f, -50.0f, -100.0f);
#pragma endregion DirectionalLight
#pragma endregion Light

	auto alien = std::make_shared<Actor>(m_models["alien"].get());
	alien->GetTransform()->SetScale(0.03f, 0.03f, 0.03f);
	alien->GetTransform()->SetPosition(40.0f, 0.0f, 30.0f);
	alien->ObjectName = std::string("alien");
	alien->GetCollisionComponent()->SetRadius(2.0f);

	auto box = std::make_shared<Actor>(m_models["box"].get());
	box->GetTransform()->SetScale(3.0f, 3.0f, 3.0f);
	box->GetTransform()->SetPosition(0.0f, 0.0f, 15.0f);
	box->ObjectName = std::string("box");
	box->GetCollisionComponent()->SetRadius(3.0f);

	auto chair = std::make_shared<Actor>(m_models["chair"].get());
	chair->GetTransform()->SetScale(1.0f, 1.0f, 1.0f);
	chair->GetTransform()->SetPosition(50.0f, 2.3f, 60.0f);
	chair->ObjectName = std::string("chair");
	chair->GetCollisionComponent()->SetRadius(4.0f);

	m_player = std::make_shared<KatamariPlayer>(m_models["angrybird"].get());
	m_player->GetTransform()->SetScale(0.02f, 0.02f, 0.02f);
	m_player->GetTransform()->SetPosition(10.0f, 1.9f, 20.0f);
	m_player->ObjectName = std::string("Player");
	m_player->GetCollisionComponent()->SetRadius(4.0f);

	auto pig = std::make_shared<Actor>(m_models["minion"].get());
	pig->GetTransform()->SetScale(0.02f, 0.02f, 0.02f);
	pig->GetTransform()->SetPosition(30.0f, 0.3f, 35.0f);
	pig->ObjectName = std::string("pig");
	pig->GetCollisionComponent()->SetRadius(3.0f);

	auto tony = std::make_shared<Actor>(m_models["tony"].get());
	tony->GetTransform()->SetScale(0.02f, 0.02f, 0.02f);
	tony->GetTransform()->SetPosition(10.0f, 0.0f, 40.0f);
	tony->ObjectName = std::string("angryBird");
	tony->GetCollisionComponent()->SetRadius(2.0f);

	auto rockFloor = std::make_shared<Actor>(m_models["rock"].get());
	rockFloor->GetTransform()->SetScale(10.0f, 0.1f, 10.0f);
	rockFloor->GetTransform()->SetPosition(0.0f, -1.7f, 0.0f);
	rockFloor->ObjectName = std::string("rock");
	rockFloor->GetCollisionComponent()->DisableCollision();

	mRenderWindow.GetGfx().AddPlayer(m_player);
	mRenderWindow.GetGfx().AddToRenderPool(m_player);
	mRenderWindow.GetGfx().AddToRenderPool(std::move(tony));
	mRenderWindow.GetGfx().AddToRenderPool(std::move(box));
	mRenderWindow.GetGfx().AddToRenderPool(std::move(alien));
	mRenderWindow.GetGfx().AddToRenderPool(std::move(chair));
	mRenderWindow.GetGfx().AddToRenderPool(std::move(pig));
	mRenderWindow.GetGfx().AddToRenderPool(std::move(rockFloor));

#pragma region LightPool
	mRenderWindow.GetGfx().AddToRenderPool(directionalLight);
	mRenderWindow.GetGfx().AddToRenderPool(pointLight1);
	mRenderWindow.GetGfx().AddToRenderPool(pointLight2);
	mRenderWindow.GetGfx().AddToRenderPool(pointLight3);
	mRenderWindow.GetGfx().AddToRenderPool(pointLight4);
	mRenderWindow.GetGfx().AddToRenderPool(pointLight5);
	mRenderWindow.GetGfx().AddToRenderPool(pointLight6);
	mRenderWindow.GetGfx().AddToRenderPool(pointLight7);
	mRenderWindow.GetGfx().AddToRenderPool(pointLight8);
	//mRenderWindow.GetGfx().AddToRenderPool(spotLight1);
#pragma endregion LightPool
	
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
	// deferred additional task specific
	if (mRenderWindow.kbd.IsKeyPressed('1'))
	{
		mRenderWindow.GetGfx().SwitchGBufferLayer(0);
	}
	if (mRenderWindow.kbd.IsKeyPressed('2'))
	{
		mRenderWindow.GetGfx().SwitchGBufferLayer(1);
	}
	if (mRenderWindow.kbd.IsKeyPressed('3'))
	{
		mRenderWindow.GetGfx().SwitchGBufferLayer(2);
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
	mRenderWindow.GetGfx().DrawScene(st);
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