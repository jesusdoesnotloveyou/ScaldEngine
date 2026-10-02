#include "stdafx.h"
#include "Mesh.h"
#include "ScaldException.h"

using namespace Scald;

Mesh::Mesh(ID3D11Device* device, std::vector<VertexPositionNormalUV> vertices, std::vector<DWORD> indices)
{
    ThrowIfFailed(m_vertexBuffer.Init(device, vertices.data(), static_cast<UINT>(vertices.size())));
    ThrowIfFailed(m_indexBuffer.Init(device, indices.data(), static_cast<UINT>(indices.size())));
}

const VertexBuffer<VertexPositionNormalUV>& Mesh::GetVertexBuffer() const
{
    return m_vertexBuffer;
}

const IndexBuffer<DWORD>& Mesh::GetIndexBuffer() const
{
    return m_indexBuffer;
}