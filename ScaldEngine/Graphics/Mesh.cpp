#include "stdafx.h"
#include "Mesh.h"

using namespace Scald;

Mesh::Mesh(ID3D11DeviceContext* deviceContext)
{
    mDeviceContext = deviceContext;
}

Mesh::Mesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const std::vector<VertexPositionNormalUV>& vertices, const std::vector<DWORD>& indices = std::vector<DWORD>(0))
    : Mesh(deviceContext)
{
    ThrowIfFailed(mVB.Init(device, vertices.data(), static_cast<UINT>(vertices.size())));
    ThrowIfFailed(mIB.Init(device, indices.data(), static_cast<UINT>(indices.size())));
}

Mesh::Mesh(const Mesh& mesh)
    : Mesh(mesh.mDeviceContext)
{
    mIB = mesh.mIB;
    mVB = mesh.mVB;
}

void Mesh::Draw() const
{
    mDeviceContext->IASetVertexBuffers(0u, 1u, mVB.GetAddressOf(), mVB.GetStridePtr(), mVB.GetOffsetPtr());
    mDeviceContext->IASetIndexBuffer(mIB.Get(), DXGI_FORMAT_R32_UINT, 0u);
    mDeviceContext->DrawIndexed(mIB.GetBufferSize(), 0u, 0);
}

VertexBuffer<VertexPositionNormalUV>& Mesh::GetVertexBuffer()
{
    return mVB;
}

IndexBuffer& Mesh::GetIndexBuffer()
{
    return mIB;
}