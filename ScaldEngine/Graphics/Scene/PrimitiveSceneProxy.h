#pragma once

#include "Graphics/DXHelper.h"

namespace Scald
{
    using XMMATRIX = DirectX::XMMATRIX;
    using XMFLOAT4X4 = DirectX::XMFLOAT4X4;

    class Model;
    struct Texture;

    class PrimitiveSceneProxy
    {
    public:
        PrimitiveSceneProxy();
        virtual ~PrimitiveSceneProxy() noexcept;

        void SetWorld(XMMATRIX world);
        const XMMATRIX GetWorld() const;

        void SetModel(const Model* model);
        const Model* GetModel() const;
        
        void SetTexture(const Texture* texture);
        const Texture* GetTexture() const;
    private:
        XMFLOAT4X4 m_world;
        // Mesh -> StaticMesh
        // Mesh -> SkinnedMesh
        // Mesh -> AnimatedMesh
        const Model* m_model;
        // TODO: consider storing ptr to a whole texture/only srv
        // But the logic is correct here since in proxy classes we allow graphics api dependencies
        const Texture* m_texture;

    };
}