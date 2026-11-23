#include "IWindowManager.h"
#include "IWindow.h"

using namespace Scald;

void IWindowManager::Update() {}

std::expected<uint32_t, WindowCreationError> IWindowManager::CreateEngineWindow(const WindowSettings& settings)
{
    if (!m_bIsInitialized)
    {
        OutputDebugStringA("Cannot create engine window. Window manger isn't initialized.");
        return std::unexpected<WindowCreationError>(WindowCreationError::ManagerIsNotInitialized);
    }

    auto window = std::make_shared<IWindow>(settings);

    if (!window->IsValid())
    {
        OutputDebugStringA("Failed to create window.");
        return std::unexpected<WindowCreationError>(WindowCreationError::CreationFailed);
    }

    uint32_t id{1u};
    m_windows[id] = window;

    OutputDebugStringA("Window with id = 1 has been added");
    return id;
}

IWindow* IWindowManager::GetEngineWindow(const WindowInfo winInfo) const
{
    return m_windows.at(winInfo.id).get();
}

bool IWindowManager::AreAllWindowsClosed() const
{
    if (!m_bIsInitialized) return true;

    for (auto& window : m_windows)
    {
        if (!window.second->ShouldShutdown()) return false;
    }

    return true;
}