#include "stdafx.h"
#include "SceneGeometry.h"
#include "Objects/Components/TransformComponent.h"

#include <exception>

using namespace Scald;

// could be used to set materials and models
SceneGeometry::SceneGeometry()
{
    mCollisionComponent = new CollisionComponent(this);
    // mRenderComponent = new RenderComponent{};
}

SceneGeometry::SceneGeometry(const std::tuple<std::vector<VertexPositionNormalUV>, std::vector<DWORD>>& vi) {}

SceneGeometry::~SceneGeometry()
{
    if (mCollisionComponent) delete mCollisionComponent;
    // if (mRenderComponent) delete mRenderComponent;
}

void SceneGeometry::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const std::string& filePath, const std::wstring& texturePath)
{
    if (!model.Init(pDevice, pDeviceContext, filePath, texturePath))
    {
        throw std::exception{};
    }
}

void SceneGeometry::Update(const ScaldTimer& st)
{
    SceneComponent::Update(st);

    mCollisionComponent->Update(st);
    
    UpdateObjectCBs(st);
}

void SceneGeometry::Draw() const
{
    model.Draw();
}

void SceneGeometry::UpdateObjectCBs(const ScaldTimer& st)
{
    // TODO: make void const and move const buffer logic to UpdateCB method
    ConstantBufferPerObject bufferVS = {};

    const XMMATRIX world = GetTransform()->mWorldMatrix;
    auto det = XMMatrixDeterminant(world);

    const XMMATRIX invTransWorld = XMMatrixInverse(&det, XMMatrixTranspose(world));

    bufferVS.gWorld = XMMatrixTranspose(world);
    bufferVS.gInvTransWorld = XMMatrixTranspose(invTransWorld);

    model.GetConstantBufferVS().SetAndApplyData(bufferVS);
}