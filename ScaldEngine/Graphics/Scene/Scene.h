#pragma once

#include "Graphics/ScaldCoreTypes.h"
#include "GameFramework/Components/Transform.h"
#include <vector>
#include <mutex>

namespace Scald
{
    class PrimitiveSceneProxy;
    class LightSceneProxy;

    // Render thread Scene class. It contains all renderable proxies to draw in render loop.
    class Scene
    {
    public:
        // called by PrimitiveComponent when it registers
        void AddPrimitive(PrimitiveSceneProxy* proxy);
        // TODO: Do not preserve order of primitive proxies
        void RemovePrimitive(PrimitiveSceneProxy* proxy);
        void UpdatePrimitiveTransform(PrimitiveSceneProxy* proxy, const Transform& componentTransform);

        void AddLight(LightSceneProxy* proxy);
        // TODO: Do not preserve order of light proxies
        void RemoveLight(LightSceneProxy* proxy);

        // renderer reads these
        const std::vector<PrimitiveSceneProxy*>& GetPrimitives() const { return m_primitives; }
        const std::vector<LightSceneProxy*>&     GetLights()     const { return m_lights; }

    private:
        // non-owning — proxies owned by actual components
        std::vector<PrimitiveSceneProxy*> m_primitives;
        std::vector<LightSceneProxy*>     m_lights;
    };
}