#include "Mesh.h"

Mesh::Mesh(ID3D11DeviceContext* deviceContext)
{
    mDeviceContext = deviceContext;
}

Mesh::Mesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::vector<VertexTex>& vertices, std::vector<DWORD> indeces)
    : Mesh(deviceContext)
{
    ThrowIfFailed(mVB.Init(device, vertices.data(), (UINT)vertices.size()));
    ThrowIfFailed(mIB.Init(device, indeces.data(), (UINT)indeces.size()));
}

Mesh::Mesh(const Mesh& mesh) : Mesh(mesh.mDeviceContext)
{
    mIB = mesh.mIB;
    mVB = mesh.mVB;
}

void Mesh::Draw()
{
    mDeviceContext->IASetVertexBuffers(0u, 1u, mVB.GetAddressOf(), mVB.GetStridePtr(), mVB.GetOffsetPtr());
    mDeviceContext->IASetIndexBuffer(mIB.Get(), DXGI_FORMAT_R32_UINT, 0u);
    mDeviceContext->DrawIndexed(mIB.GetBufferSize(), 0u, 0);
}

VertexBuffer<VertexTex>& Mesh::GetVertexBuffer()
{
    return mVB;
}

IndexBuffer& Mesh::GetIndexBuffer()
{
    return mIB;
}