#include "stdafx.h"

#include "Engine.h"
#include "GameFramework/World.h"
#include "GameFramework/Components/StaticMeshComponent.h"
#include "GameFramework/Components/Light/DirectionalLightComponent.h"

#include "Graphics/AssetManager.h"
#include "Graphics/Scene/Scene.h"
#include "Graphics/Camera/Camera.h"
#include "WinApiWindowManager.h"

using namespace Scald;

Engine::Engine(uint32_t width /*= 1280u*/, uint32_t height /*= 720u*/)
    : m_renderWindow(width, height, "Scald Engine Direct3D11")
{
    m_windowManager = std::make_unique<WinApiWindowManager>();
}

// For unique_ptr destruction
Engine::~Engine() {}

int Engine::Launch()
{
    Initialize();
    
    if (!m_bIsInitialized) return 0;

    while (true /*!renderWindow.ShouldClose()*/)
    {
        m_windowManager->Update();
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
    SetupAssetManager();

    SetupWorld();

    m_timer.Reset();

    m_bIsInitialized = true;
}

void Engine::SetupRenderer()
{
    // Some parameters or setting to choose between different renderers
    // m_renderer = std::make_unique<DeferredRenderer>();
    m_renderWindow.GetGfx().Setup(/*m_renderer*/);
}

void Engine::SetupInputSubsystem()
{
#pragma region PlayerInputDelegates
    // m_renderWindow.kbd.OnKeyPressedEvent.AddRaw(m_player->GetMovement(), &KatamariMovementComponent::OnKeyPressed);
    // m_renderWindow.kbd.OnKeyReleasedEvent.AddRaw(m_player->GetMovement(), &KatamariMovementComponent::OnKeyReleased);
#pragma endregion PlayerInputDelegates
}

void Engine::SetupAssetManager()
{
    m_assetManager = std::make_unique<AssetManager>(m_renderWindow.GetGfx().GetDevice());
}

void Engine::SetupWorld()
{
    m_world = std::make_unique<World>();

    assert(m_assetManager->LoadModel("./Data/Models/Tony.obj") == true && "Model is failed to load");
    assert(m_assetManager->LoadModel("./Data/Models/Angry_Bird.obj") == true && "Model is failed to load");
    assert(m_assetManager->LoadModel("./Data/Models/Alien_Female_Lores.obj") == true && "Model is failed to load");
    assert(m_assetManager->LoadModel("./Data/Models/MinionPig.obj") == true && "Model is failed to load");
    assert(m_assetManager->LoadModel("./Data/Models/monoblock_CHAIR.obj") == true && "Model is failed to load");
    assert(m_assetManager->LoadModel("./Data/Models/box2.obj") == true && "Model is failed to load");
    assert(m_assetManager->LoadModel("./Data/Models/rock.obj") == true && "Model is failed to load");

    assert(m_assetManager->LoadTexture("./Data/Textures/AngryBirdCeleste.png") == true && "Texture is failed to load");

    auto testActor = m_world->SpawnActor();
    //auto testSceneComp = testActor->CreateComponent<PrimitiveComponent>();
    auto testMeshComp = testActor->CreateComponent<StaticMeshComponent>();
    testMeshComp->SetStaticMesh(m_assetManager->GetLoadedModel("./Data/Models/AlienFemale/Alien_Female_Lores.obj"));
    testMeshComp->SetTexture(m_assetManager->GetLoadedTexture("./Data/Textures/AngryBirdCeleste.png"));
    testMeshComp->SetPosition(4.0f, 0.0f, 3.0f);
    testMeshComp->SetScale(0.01f, 0.01f, 0.01f);

    //auto testLight = m_world->SpawnLight<DirectionalLightComponent>();
}

// TODO: move to window manager
void Engine::PollInput()
{
#pragma region CameraOrPlayerControl
    // InputSubsystem::Poll();
    while (!m_renderWindow.kbd.IsKeyEmpty())
    {
        const Keyboard::Event keyEvent = m_renderWindow.kbd.ReadKey();
        unsigned char keyCode = keyEvent.GetCode();
        
        m_world->SendInputToControllableSceneComponent(keyEvent, keyCode);
        //if (m_renderWindow.kbd.IsKeyPressed(VK_SPACE) && !m_player->IsFalling())
        //{
        //    m_player->Jump();
        //}
    }
#pragma endregion CameraOrPlayerControl

#pragma region CameraRotation
    while (!m_renderWindow.mouse.IsEmpty())
    {
        const auto mouseEvent = m_renderWindow.mouse.Read();
        
        if (m_renderWindow.mouse.IsRightPressed())
        {
            if (mouseEvent.GetType() == Mouse::Event::Type::RawMove)
            {
                m_renderWindow.GetGfx().GetActiveCamera()->AdjustRotation(
                    (float)mouseEvent.GetPosY() * m_timer.DeltaTime() * 0.1f,
                    (float)mouseEvent.GetPosX() * m_timer.DeltaTime() * 0.1f,
                    0.0f
                );
            }
        }
    }
#pragma endregion CameraRotation
    
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