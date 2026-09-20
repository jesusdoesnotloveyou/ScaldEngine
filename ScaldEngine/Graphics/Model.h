#pragma once

#include "Mesh.h"

#include <string>
#include <vector>

namespace Scald
{   
    using namespace Microsoft::WRL;
    using namespace DirectX;

    class Model
    {
    public:
        Model() = default;
        
        Model(const Model& lhs) = default;
        Model(Model&& lhs) noexcept = default;

        Model& operator=(const Model& lhs) = default;
        Model& operator=(Model&& lhs) noexcept = default;

        ~Model() = default;
                     
        void SetMaterial(/*ID3D11ShaderResourceView* texture*/);
        void AddMesh(Mesh&&);
        
        const std::vector<Mesh>& GetMeshes() const;
        // TODO: for now we assume that 1 model has only 1 texture
        Texture* GetTexture() const;
        
    private:
        
        std::vector<Mesh> m_meshes;
        Texture* m_texture;
    };
}