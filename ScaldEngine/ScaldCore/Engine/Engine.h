#pragma once

#include "RenderWindow.h"
#include "ScaldTimer.h"

#include <unordered_map>
#include <memory>
#include <string>

namespace Scald
{
    class ModelData;
    class World;
    // class Renderer;
    // class AssetManager;

    class Engine
    {
    public:
        explicit Engine(uint32_t width = 1280u, uint32_t height = 720u);
        ~Engine();
        int Launch();

        std::unordered_map<std::string, std::unique_ptr<ModelData>> m_models;

    private:
        void Initialize();
        void SetupRenderer();
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
    };
}