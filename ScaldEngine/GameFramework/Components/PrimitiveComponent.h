#pragma once

#include "GameFramework/Components/SceneComponent.h"
#include <memory>

namespace Scald
{
    class PrimitiveSceneProxy;

    // The base class for all components representing any sort of geometry.
    // These components are rendered and tested for collision.
    class PrimitiveComponent : public SceneComponent
    {
        using Super = SceneComponent;
    public:
        PrimitiveComponent();
        PrimitiveComponent(std::shared_ptr<Actor> owner);
        virtual ~PrimitiveComponent() noexcept override;

        virtual void OnRegister() override;
        virtual void OnUnregister() override;
        virtual void Tick(float deltaTime) override;

        // TODO: look at the class description
        void Collision() const;

        FORCEINLINE void DisableShadowCasting() { m_bCastsShadow = false; }
        
    protected:
        virtual std::unique_ptr<PrimitiveSceneProxy> CreateSceneProxy() = 0;

        std::unique_ptr<PrimitiveSceneProxy> m_sceneProxy;

        bool m_bCastsShadow = false;
    };
}