#include "Actor.h"

Actor::Actor() : SceneGeometry()
{}

Actor::Actor(const tuple<vector<VertexTex>, vector<DWORD>>& vi) : SceneGeometry()
{
    vertices = std::get<0>(vi);
    indeces = std::get<1>(vi);
}

Actor::~Actor()
{}

void Actor::Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture)
{
    pDevice = device;
    pDeviceContext = deviceContext;
    mTexture = texture;

    SceneGeometry::Init(device, deviceContext);
}

void Actor::Update(const ScaldTimer& st)
{
    SceneGeometry::Update(st);
}

void Actor::SetTexture(ID3D11ShaderResourceView* texture)
{
    mTexture = texture;
}

void Actor::Draw(const XMMATRIX& viewMatrix, const XMMATRIX& projectionMatrix)
{
    const XMMATRIX worldMatrix = GetTransform()->mWorldMatrix;
    mCB.SetData(XMMatrixTranspose(worldMatrix * viewMatrix * projectionMatrix));
    mCB.ApplyChanges();

    pDeviceContext->PSSetShaderResources(0u, 1u, &mTexture);
    pDeviceContext->IASetVertexBuffers(0u, 1u, mVB.GetAddressOf(), mVB.GetStridePtr(), mVB.GetOffsetPtr());
    pDeviceContext->IASetIndexBuffer(mIB.Get(), DXGI_FORMAT_R32_UINT, 0u);
    pDeviceContext->VSSetConstantBuffers(0u, 1u, mCB.GetAddressOf());
    pDeviceContext->DrawIndexed(mIB.GetBufferSize(), 0u, 0);
}