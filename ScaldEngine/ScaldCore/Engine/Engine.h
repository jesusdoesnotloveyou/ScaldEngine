#pragma once

#include "RenderWindow.h"
#include "ScaldTimer.h"

#include <unordered_map>
#include <memory>

class KatamariPlayer;
class ModelData;

namespace Scald
{
    class Engine
    {
    public:
        Engine();
        ~Engine();
        int Launch();

        // Katamari specific
        std::shared_ptr<KatamariPlayer> m_player = nullptr;
        std::unordered_map<std::string, std::unique_ptr<ModelData>> m_models;

    private:
        void SetupScene();
        void PollInput();
        void Update(const ScaldTimer& st);
        void RenderFrame(const ScaldTimer& st);

        void CalculateFrameStats();
        float AspectRatio() const;

    protected:
        uint32_t m_clientWidth;
        uint32_t m_clientHeight;

        RenderWindow m_renderWindow;
        ScaldTimer m_timer;
    };
}