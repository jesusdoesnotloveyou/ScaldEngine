#pragma once

#include "GameFramework/Components/PrimitiveComponent.h"
#include <memory>

namespace Scald
{
    class Actor;

    // MeshComponent is an abstract base for any component that is an instance of a renderable collection of triangles.
    // SkinnedMeshComponent
    // StaticMeshComponent
    // WaterMeshComponent
    // WidgetComponent
    
    class MeshComponent : public PrimitiveComponent
    {
        using Super = PrimitiveComponent;
    public:
        MeshComponent();
        MeshComponent(std::shared_ptr<Actor> owner);
        virtual ~MeshComponent() noexcept override;

        virtual void Tick(float deltaTime) override;

        FORCEINLINE void DisableShadowCasting() { m_bCastsShadow = false; }
        
    protected:
        bool m_bCastsShadow = false;
    };
}