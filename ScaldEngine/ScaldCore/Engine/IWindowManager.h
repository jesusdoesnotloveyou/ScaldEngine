#pragma once

#include <unordered_map>
#include <expected>

namespace Scald
{
    class IWindow;
    struct WindowSettings;

    enum class WindowCreationError
    {
        ManagerIsNotInitialized,
        CreationFailed,
        InvalidWindow
    };

    struct WindowInfo
    {
        uint32_t id;
        //bool operator==(const WindowInfo otherWindowInfo) const { return id == otherWindowInfo.id; }
    };

    /*std::hash<WindowInfo> hash(const WindowInfo& windowInfo)
    {
        return std::hash<uint32_t>(windowInfo.id);
    }*/

    class IWindowManager
    {
    public:
        IWindowManager() = default;
        virtual ~IWindowManager() = default;

        void Update();

        std::expected<uint32_t, WindowCreationError> CreateEngineWindow(const WindowSettings& settings);

        IWindow* GetEngineWindow(const WindowInfo winInfo) const;
        bool AreAllWindowsClosed() const;

    private:
        bool m_bIsInitialized{false};

        std::unordered_map<uint32_t, std::shared_ptr<IWindow>> m_windows;
    };
}