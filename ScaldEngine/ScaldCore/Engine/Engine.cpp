#include "stdafx.h"

#include "Engine.h"
#include "GameFramework/World.h"
#include "Graphics/Scene/Scene.h"
#include "Data/ModelData.h"

using namespace Scald;

Engine::Engine(uint32_t width /*= 1280u*/, uint32_t height /*= 720u*/)
    : m_renderWindow(width, height, "Scald Engine Direct3D11")
{}

Engine::~Engine() {}

int Engine::Launch()
{
    Initialize();
    
    while (true /*!renderWindow.ShouldClose()*/)
    {
        m_timer.Tick();
        // process all messages pending, but to not block
        if (const auto eCode = RenderWindow::ProcessMessages())
        {
            // if return optional has a value, means we're quitting
            return *eCode;
        }
        // otherwise
        CalculateFrameStats();
        PollInput();
        Update(m_timer.DeltaTime());
        RenderFrame(m_timer.DeltaTime());
    }
}

void Engine::Initialize()
{
    SetupRenderer();
    //SetupInputSubsystem();
    //SetupPhysicsSubsystem();

    SetupWorld();

    m_timer.Reset();
}

void Engine::SetupRenderer()
{
    // Some parameters or setting to choose between different renderers
    // m_renderer = std::make_unique<DeferredRenderer>();
    m_renderWindow.GetGfx().Setup(/*m_renderer*/);
}

void Engine::SetupWorld()
{
    m_world = std::make_unique<World>();

    // TODO: AssetManager
    m_models["alien"] = std::make_unique<ModelData>("./Data/Models/AlienFemale/Alien_Female_Lores.obj", L"./Data/Textures/brick.png");
    m_models["angrybird"] = std::make_unique<ModelData>("./Data/Models/AngryBird/Angry_Bird.obj", L"./Data/Models/AngryBird/Angry_Bird.png");
    m_models["minion"] = std::make_unique<ModelData>("./Data/Models/MinionPig/MinionPig.obj", L"./Data/Models/MinionPig/AngryBirdsChancho.png");
    m_models["chair"] = std::make_unique<ModelData>("./Data/Models/Chair/monoblock_CHAIR.obj", L"./Data/Textures/planks.png");
    m_models["tony"] = std::make_unique<ModelData>("./Data/Models/Tony/Tony.obj", L"./Data/Models/Tony/AngryBirdCeleste.png");
    m_models["box"] = std::make_unique<ModelData>("./Data/Models/Box/box2.obj", L"./Data/Textures/brick.png");
    m_models["rock"] = std::make_unique<ModelData>("./Data/Models/Rock/rock.obj", L"./Data/Textures/planks.png");

//#pragma region Light
//#pragma region PointLight
//    auto pointLight1 = std::make_shared<PointLight>("./Data/Models/Light/light.obj");
//    pointLight1->GetTransform()->SetPosition(20.0f, 4.0f, 60.0f);
//    pointLight1->SetDiffuseColor(1.0f, 1.0f, 0.5f, 5.0f);
//    pointLight1->SetAttenuation(1.0f, 0.5f, 1.1f);
//    // pointLight1->SetRange(3.0f);
//#pragma endregion PointLight
//#pragma region DirectionalLight
//    auto directionalLight = std::make_shared<DirectionalLight>("./Data/Models/Light/light.obj");
//    directionalLight->GetTransform()->SetPosition(10.0f, 50.0f, 100.0f);
//    directionalLight->GetCollisionComponent()->DisableCollision();
//    directionalLight->SetAmbientColor(0.25f, 0.25f, 0.35f, 1.0f);
//    directionalLight->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
//    // opposite to dir light pos vector
//    directionalLight->SetDirection(-10.0f, -50.0f, -100.0f);
//#pragma endregion DirectionalLight
//#pragma endregion Light
//#pragma region Actors
//    auto alien = std::make_shared<Actor>(m_models["alien"].get());
//    alien->GetTransform()->SetScale(0.03f, 0.03f, 0.03f);
//    alien->GetTransform()->SetPosition(40.0f, 0.0f, 30.0f);
//    alien->m_actorName = std::string("alien");
//    alien->GetCollisionComponent()->SetRadius(2.0f);
//#pragma endregion Actors

#pragma region PlayerInputDelegates
    //m_renderWindow.kbd.OnKeyPressedEvent.AddRaw(m_player->GetMovement(), &KatamariMovementComponent::OnKeyPressed);
    //m_renderWindow.kbd.OnKeyReleasedEvent.AddRaw(m_player->GetMovement(), &KatamariMovementComponent::OnKeyReleased);
#pragma endregion PlayerInputDelegates
}

void Engine::PollInput()
{
    // InputSubsystem::Poll();
    while (!m_renderWindow.kbd.IsKeyEmpty())
    {
        const auto keyEvent = m_renderWindow.kbd.ReadKey();
        unsigned char keyCode = keyEvent.GetCode();
    }

#pragma region CameraRotation
    const auto mouseEvent = m_renderWindow.mouse.Read();
    if (m_renderWindow.mouse.IsRightPressed())
    {
        if (mouseEvent.GetType() == Mouse::Event::Type::RawMove)
        {
            //m_renderWindow.GetGfx().GetCamera()->AdjustRotation((float)mouseEvent.GetPosY() * 0.01f, (float)mouseEvent.GetPosX() * 0.01f, 0.0f);
        }
    }
#pragma endregion CameraRotation

#pragma region PlayerMovement
    //// TODO: refactoring
    //// Camera forward without Y (XoZ)
    //auto forward = XMVectorSetY(m_renderWindow.GetGfx().GetCamera()->GetForwardVector(), 0.0f);
    //forward = XMVector3Normalize(forward);
    //m_player->SetForwardVector(forward);

    //// Camera right without Y (XoZ)
    //auto right = XMVectorSetY(m_renderWindow.GetGfx().GetCamera()->GetRightVector(), 0.0f);
    //right = XMVector3Normalize(right);
    //m_player->SetRightVector(right);

    //if (m_renderWindow.kbd.IsKeyPressed(VK_SPACE) && !m_player->IsFalling())
    //{
    //    m_player->Jump();
    //}
    // deferred additional task specific
    if (m_renderWindow.kbd.IsKeyPressed('1'))
    {
        m_renderWindow.GetGfx().SwitchGBufferLayer(0);
    }
    if (m_renderWindow.kbd.IsKeyPressed('2'))
    {
        m_renderWindow.GetGfx().SwitchGBufferLayer(1);
    }
    if (m_renderWindow.kbd.IsKeyPressed('3'))
    {
        m_renderWindow.GetGfx().SwitchGBufferLayer(2);
    }

#pragma endregion PlayerMovement
}

void Engine::Update(float deltaTime)
{
    m_world->Tick(deltaTime);

    //Physics::Update(deltaTime);
    m_renderWindow.GetGfx().Update(deltaTime);
}

void Engine::RenderFrame(float deltaTime)
{
    m_renderWindow.GetGfx().ClearBuffer(0.0f);
    m_renderWindow.GetGfx().DrawScene(m_world->GetScene());
    m_renderWindow.GetGfx().EndFrame();
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
    if ((m_timer.TotalTime() - timeElapsed) >= 1.0f)
    {
        float fps = (float)frameCnt;  // fps = frameCnt / 1
        float mspf = 1000.0f / fps;
        std::ostringstream outs;
        outs.precision(6);
        outs << "FPS: " << fps << " "
             << "Frame Time: " << mspf << "(ms)\n";
        m_renderWindow.SetTitle(outs.str());
        // Reset for next average.
        frameCnt = 0;
        timeElapsed += 1.0f;
    }
}