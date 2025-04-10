#include "../../ScaldException.h"
#include "SceneGeometry.h"

// could be used to set materials and models
SceneGeometry::SceneGeometry()
{
    mTransformComponent = new TransformComponent();
    mRenderComponent = new RenderComponent{};
    mCollisionComponent = new CollisionComponent{};
    mMovementComponent = new MovementComponent{};
}

SceneGeometry::SceneGeometry(const tuple<vector<VertexTex>, vector<DWORD>>& vi)
{}

SceneGeometry::~SceneGeometry()
{
    if (mTransformComponent) delete mTransformComponent;
    if (mRenderComponent) delete mRenderComponent;
    if (mCollisionComponent) delete mCollisionComponent;
    if (mMovementComponent) delete mMovementComponent;
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
    mTransformComponent->AdjustRotation(0.0f, 0.1f * st.DeltaTime(), 0.0f);
    mTransformComponent->Update(st);
    mCollisionComponent->Update(st);
    mMovementComponent->Update(st);
    UpdateObjectCBs(st);
}

void SceneGeometry::Draw(const XMMATRIX& viewProjectionMatrix)
{
    ConstBufferVS bufferVS = {};
    bufferVS.gWorldViewProj = XMMatrixTranspose(GetTransform()->mWorldMatrix * viewProjectionMatrix);

    ConstBufferPS bufferPS = {};
    bufferPS.ambientLightColor = { 1.0f, 1.0f, 1.0f };
    bufferPS.ambientLightStrength = 0.8f;

    model.GetConstantBufferVS().SetData(bufferVS);
    model.GetConstantBufferPS().SetData(bufferPS);
    
    model.Draw();
}

void SceneGeometry::UpdateObjectCBs(const ScaldTimer& st)
{
    model.GetConstantBufferVS().ApplyChanges();
    model.GetConstantBufferPS().ApplyChanges();
}

XMVECTOR SceneGeometry::GetForwardVector() const
{
    return mTransformComponent->GetForwardVector();
}

XMVECTOR SceneGeometry::GetRightVector() const
{
    return mTransformComponent->GetRightVector();
}

XMVECTOR SceneGeometry::GetUpVector() const
{
    return mTransformComponent->GetUpVector();
}