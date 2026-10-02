#pragma once

#include "Graphics/Model.h"

namespace Scald
{
    class PrimitiveSceneProxy
    {
    public:
        PrimitiveSceneProxy();
        virtual ~PrimitiveSceneProxy() noexcept;

        void SetWorld(XMMATRIX world);
        void SetModel(const Model* model);
        const XMMATRIX GetWorld() const;
        const Model* GetModel() const;
        
    private:
        // Mesh -> StaticMesh
        // Mesh -> SkinnedMesh
        // Mesh -> AnimatedMesh
        const Model* m_model = nullptr;
        XMFLOAT4X4 m_world;
    };
}