#pragma once

#include "RenderWindow.h"
#include "ScaldTimer.h"

#include <memory>

namespace Scald
{
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

        RenderWindow m_renderWindow;
        ScaldTimer m_timer;
        std::unique_ptr<AssetManager> m_assetManager;
    };
}