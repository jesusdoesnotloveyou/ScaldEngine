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

void SceneGeometry::Draw(const XMMATRIX& viewMatrix, const XMMATRIX& ProjectionMatrix)
{
    ConstBufferVS bufferVS = {};

    XMMATRIX world = GetTransform()->mWorldMatrix;

    auto det = XMMatrixDeterminant(world);
    XMMATRIX invTransWorld = XMMatrixInverse(&det, XMMatrixTranspose(world));

    bufferVS.gWorld = XMMatrixTranspose(world);
    bufferVS.gInvTransWorld = XMMatrixTranspose(invTransWorld);
    bufferVS.gView = XMMatrixTranspose(viewMatrix);
    bufferVS.gProjection = XMMatrixTranspose(ProjectionMatrix);

    model.GetConstantBufferVS().SetData(bufferVS);
    model.GetConstantBufferVS().ApplyChanges();
    model.Draw();
}

void SceneGeometry::UpdateObjectCBs(const ScaldTimer& st)
{
    model.GetConstantBufferVS().ApplyChanges();
}