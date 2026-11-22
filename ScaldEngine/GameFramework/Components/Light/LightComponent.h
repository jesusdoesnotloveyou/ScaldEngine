#pragma once

#include "LightComponentBase.h"

namespace Scald
{
	class LightComponent : public LightComponentBase
	{
		using Super = LightComponentBase;
    public:
        LightComponent() = default;
        virtual ~LightComponent() override = default;

        void OnRegister() override;
        void OnUnregister() override;

	protected:
        // LightComponents subclasses (direct?, point, spot) provide proxies
        virtual std::unique_ptr<LightSceneProxy> CreateSceneProxy() /* = 0*/;
        std::unique_ptr<LightSceneProxy> m_lightProxy;
	};
}  // namespace Scald