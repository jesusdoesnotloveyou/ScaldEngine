#include "../../ScaldException.h"
#include "SceneGeometry.h"
#include <sstream>

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

void SceneGeometry::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11ShaderResourceView* pTexture)
{
    ThrowIfFailed(mVB.Init(pDevice, vertices.data(), (UINT)vertices.size()));
    ThrowIfFailed(mIB.Init(pDevice, indeces.data(), (UINT)indeces.size()));
    ThrowIfFailed(mCB.Init(pDevice, pDeviceContext));
}

void SceneGeometry::Update(const ScaldTimer& st)
{
    UpdateRotation(st);
    UpdateOrbitRotation(st);
    mTransformComponent->Update(st);

    mCollisionComponent->Update(st);
    mMovementComponent->Update(st);
}

void SceneGeometry::UpdateOrbitRotation(const ScaldTimer& st)
{
    GetTransform()->mOrbitRot += XMConvertToRadians(GetMovement()->GetOrbitAngle()) * st.DeltaTime();
    if (GetTransform()->mOrbitRot >= 6.28f)
        GetTransform()->mOrbitRot = 0.0f;
}

void SceneGeometry::UpdateRotation(const ScaldTimer& st)
{
    GetTransform()->mRot.y += XMConvertToRadians(GetMovement()->GetRotAngle()) * st.DeltaTime();
    if (GetTransform()->mRot.y >= 6.28f)
        GetTransform()->mRot.y = 0.0f;
}

void SceneGeometry::Draw(const XMMATRIX& viewMatrix, const XMMATRIX& projectionMatrix)
{

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

VertexBuffer<VertexTex>& SceneGeometry::GetVertexBuffer()
{
    return mVB;
}

IndexBuffer& SceneGeometry::GetIndexBuffer()
{
    return mIB;
}

ConstantBuffer<ConstBufferVS>& SceneGeometry::GetConstantBuffer()
{
    return mCB;
}