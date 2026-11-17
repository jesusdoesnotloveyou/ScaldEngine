#pragma once

#include "Graphics/LightHelper.h"

namespace Scald
{
    // Another type of proxy class specifically for lighs. It is not inherited from PrimitiveSceneProxy class.
    class LightSceneProxy
    {
    public:
        LightSceneProxy();
        virtual ~LightSceneProxy() noexcept;

    protected:
        //LIGHT_DESC m_lightParams;
    };
}