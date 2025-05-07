#include "../../ScaldException.h"
#include "SceneGeometry.h"

// could be used to set materials and models
SceneGeometry::SceneGeometry()
{
    mCollisionComponent = new CollisionComponent(this);
    //mRenderComponent = new RenderComponent{};
}

SceneGeometry::SceneGeometry(const tuple<vector<VertexTex>, vector<DWORD>>& vi)
{}

SceneGeometry::~SceneGeometry()
{
    if (mCollisionComponent) delete mCollisionComponent;
    //if (mRenderComponent) delete mRenderComponent;
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

void SceneGeometry::Draw(const XMMATRIX& viewProjectionMatrix)
{
    ConstBufferVS bufferVS = {};
    bufferVS.gWorldViewProj = XMMatrixTranspose(GetTransform()->mWorldMatrix * viewProjectionMatrix);
    bufferVS.gWorld = XMMatrixTranspose(GetTransform()->mWorldMatrix);

    model.GetConstantBufferVS().SetData(bufferVS);
    model.GetConstantBufferVS().ApplyChanges();
    model.Draw();
}

void SceneGeometry::UpdateObjectCBs(const ScaldTimer& st)
{
    model.GetConstantBufferVS().ApplyChanges();
}