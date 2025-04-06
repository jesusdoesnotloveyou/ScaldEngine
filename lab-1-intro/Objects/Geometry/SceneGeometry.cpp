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

#pragma region ToDelete
//void SceneGeometry::UpdateOrbitRotation(const ScaldTimer& st)
//{
//    GetTransform()->mOrbitRot += XMConvertToRadians(GetMovement()->GetOrbitAngle()) * st.DeltaTime();
//    if (GetTransform()->mOrbitRot >= 6.28f)
//        GetTransform()->mOrbitRot = 0.0f;
//}
//
//void SceneGeometry::UpdateRotation(const ScaldTimer& st)
//{
//    GetTransform()->mRot.y += XMConvertToRadians(GetMovement()->GetRotAngle()) * st.DeltaTime();
//    if (GetTransform()->mRot.y >= 6.28f)
//        GetTransform()->mRot.y = 0.0f;
//}
#pragma endregion ToDelete

void SceneGeometry::Draw(const XMMATRIX& viewProjectionMatrix)
{
    model.GetConstantBuffer().SetData(XMMatrixTranspose(GetTransform()->mWorldMatrix * viewProjectionMatrix));
    model.Draw();
}

void SceneGeometry::UpdateObjectCBs(const ScaldTimer& st)
{
    model.GetConstantBuffer().ApplyChanges();
}

XMVECTOR SceneGeometry::GetForwardVector() const
{
    return GetTransform()->GetForwardVector();
}

XMVECTOR SceneGeometry::GetRightVector() const
{
    return GetTransform()->GetRightVector();
}

XMVECTOR SceneGeometry::GetBackVector() const
{
    return GetTransform()->GetBackVector();
}

XMVECTOR SceneGeometry::GetLeftVector() const
{
    return GetTransform()->GetLeftVector();
}