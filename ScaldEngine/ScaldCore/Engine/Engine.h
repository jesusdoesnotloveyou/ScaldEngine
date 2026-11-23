#pragma once

#include "RenderWindow.h"
#include "ScaldTimer.h"

#include <memory>

namespace Scald
{
    class IWindowManager;
    class AssetManager;
    class World;
    // class Renderer;

    class Engine
    {
    public:
        explicit Engine(uint32_t width = 1280u, uint32_t height = 720u);
        ~Engine();
        int Launch();

    private:
        void Initialize();
        void SetupRenderer();
        void SetupInputSubsystem();
        void SetupAssetManager();
        void SetupWorld();

        void PollInput();
        void Update(float deltaTime);
        void RenderFrame(float deltaTime);

        void CalculateFrameStats();
    protected:
        std::unique_ptr<World> m_world;
        // std::unique_ptr<Renderer> m_renderer;
        // std::unique_ptr<AssetManager> m_manager;

        // to remove
        RenderWindow m_renderWindow;
        std::unique_ptr<AssetManager> m_assetManager;
        std::unique_ptr<IWindowManager> m_windowManager;
        ScaldTimer m_timer;

        bool m_bIsInitialized = false;
    };
}