#pragma once

#include "GameFramework/Components/MeshComponent.h"

#include <memory>

namespace Scald
{
    class Actor;
    class Model;
    struct Texture;

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

        void SetStaticMesh(const Model* staticMeshModel);
        // TODO: for now we assume that every model has only 1 associated texture
        void SetTexture(const Texture* meshTexture);
        
    protected:
        virtual std::unique_ptr<PrimitiveSceneProxy> CreateSceneProxy() override;
    };
}