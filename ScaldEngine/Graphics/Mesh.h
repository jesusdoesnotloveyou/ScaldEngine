#pragma once

#include "ScaldCoreTypes.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace Scald
{    
    template<typename V = VertexPositionNormalUV, typename I = DWORD, typename T =  Texture>
    struct MeshData
    {
        // TODO: think about efficient way to pass data
        MeshData(const std::vector<V> vertices,
            const std::vector<I> indices = std::vector<I>(0),
            const std::vector<T> textures = std::vector<T>(0))
        {
            Vertices = vertices;
            Indices = indices;
            Textures = textures;
        }

        MeshData(const MeshData& mesh) = default;
        MeshData(MeshData&& mesh) noexcept = default;
        
        std::vector<V> Vertices;
        std::vector<I> Indices;
        std::vector<T> Textures;
    };

    class Mesh
    {
    public:
        Mesh() = default;
        Mesh(ID3D11Device* device, std::vector<VertexPositionNormalUV> vertices, std::vector<DWORD> indices);
        Mesh(const Mesh& mesh) = default;
        Mesh(Mesh&& mesh) noexcept = default;
        Mesh& operator=(const Mesh& mesh) = default;
        Mesh& operator=(Mesh&& mesh) noexcept = default;
        
        ~Mesh() noexcept = default;

        const VertexBuffer<VertexPositionNormalUV>& GetVertexBuffer() const;
        const IndexBuffer<DWORD>& GetIndexBuffer() const;
        
    private:
        VertexBuffer<VertexPositionNormalUV> m_vertexBuffer;
        IndexBuffer<DWORD> m_indexBuffer;

    };
}