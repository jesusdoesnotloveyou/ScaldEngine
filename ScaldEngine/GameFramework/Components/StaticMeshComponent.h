#pragma once

#include "GameFramework/Components/MeshComponent.h"
#include "GameFramework/Objects/Actor.h"

#include <memory>

namespace Scald
{
    class Actor;

    class StaticMeshComponent : public MeshComponent
    {
        using Super = MeshComponent;
    public:
        StaticMeshComponent() = default;

        StaticMeshComponent(std::shared_ptr<Actor> owner)
            : Super(owner)
        {

        }

        virtual ~StaticMeshComponent() override = default;

        void SetStaticMesh(const Model* staticMeshModel)
        {
            m_sceneProxy->SetModel(staticMeshModel);
        }
        
    protected:
        virtual std::unique_ptr<PrimitiveSceneProxy> CreateSceneProxy() override;
    };
}