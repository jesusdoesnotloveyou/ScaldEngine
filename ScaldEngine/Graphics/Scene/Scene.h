#pragma once

#include "Graphics/ScaldCoreTypes.h"

#include <vector>

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
        void RemovePrimitive(PrimitiveSceneProxy* proxy);
        void UpdatePrimitiveTransform(PrimitiveSceneProxy* proxy, const XMFLOAT4X4& transform);

        void AddLight(LightSceneProxy* proxy);
        void RemoveLight(LightSceneProxy* proxy);

        // renderer reads these
        const std::vector<PrimitiveSceneProxy*>& GetPrimitives() const { return m_primitives; }
        const std::vector<LightSceneProxy*>&     GetLights()     const { return m_lights; }

    private:
        std::vector<PrimitiveSceneProxy*> m_primitives;  // non-owning — proxies owned by components
        std::vector<LightSceneProxy*>     m_lights;
};
}