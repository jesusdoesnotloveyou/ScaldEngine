#pragma once

#include <cstdint>

namespace Scald
{
    struct WindowSettings
    {
        const char* name;
        uint32_t width = 1280u;
        uint32_t height = 720u;
        uint32_t leftTopX = 100u;
        uint32_t leftTopY = 100u;
    };

    class IWindow
    {
    public:
        IWindow(const WindowSettings& settings)
        {

        }

        virtual ~IWindow() = default;
  
        bool IsValid() const
        {
            // ptr to GLFW window
            // ptr to hWnd
            return m_bIsInitialized == true;
        }

        bool ShouldShutdown() const
        {
            return !IsValid();
        }

    private:
        bool m_bIsInitialized = false;
    };
}