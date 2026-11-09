#pragma once

#include "Mesh.h"

#include <vector>

namespace Scald
{   
    using Microsoft::WRL::ComPtr;

    class Model
    {
    public:
        Model() = default;
        
        Model(const Model& lhs) = default;
        Model(Model&& lhs) noexcept = default;
        Model& operator=(const Model& lhs) = default;
        Model& operator=(Model&& lhs) noexcept = default;

        ~Model() = default;
                     
        void AddMesh(Mesh&&);
        const std::vector<Mesh>& GetMeshes() const;
        
    private:
        std::vector<Mesh> m_meshes;
    };
}