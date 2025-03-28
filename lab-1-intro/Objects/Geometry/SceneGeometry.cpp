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

    // will be removed since we will pass only world matrix to the constant buffer
    // all calculations are in scene geometry class
    mCB.SetTransform(GetTransform());

    UpdateWorldMatrix();

    GetTransform()->Translation.x = GetTransform()->orbitRadius;
}

void SceneGeometry::Update(const ScaldTimer& st)
{
    UpdateRotation(st);
    UpdateOrbitRotation(st);

    UpdateWorldMatrix();
}

void SceneGeometry::UpdateOrbitRotation(const ScaldTimer& st)
{
    GetTransform()->orbitRot += XMConvertToRadians(GetTransform()->orbitAngle) * st.DeltaTime();
    if (GetTransform()->orbitRot >= 6.28f)
        GetTransform()->orbitRot = 0.0f;
}

void SceneGeometry::UpdateRotation(const ScaldTimer& st)
{
    GetTransform()->rot += XMConvertToRadians(GetTransform()->rotAngle) * st.DeltaTime();
    if (GetTransform()->rot >= 6.28f)
        GetTransform()->rot = 0.0f;
}

void SceneGeometry::UpdateLocalMatrix()
{
    // SRT - default order of matrix multiplication
    // (S)TR - orbit effect for Solar system could be used
    GetTransform()->mLocalMatrix =
        XMMatrixScaling(GetTransform()->Scale.x, GetTransform()->Scale.y, GetTransform()->Scale.z) *
        XMMatrixRotationY(GetTransform()->rot) *
        XMMatrixTranslation(GetTransform()->Translation.x, GetTransform()->Translation.y, GetTransform()->Translation.z) *
        XMMatrixRotationY(GetTransform()->orbitRot);
}

void SceneGeometry::UpdateWorldMatrix()
{
    UpdateLocalMatrix();

    if (GetTransform()->ParentTransform)
    {
        XMVECTOR outScale;
        XMVECTOR outRot;
        XMVECTOR outTrans;
        XMMatrixDecompose(&outScale, &outRot, &outTrans, GetTransform()->ParentTransform->mWorldMatrix);
        GetTransform()->mWorldMatrix = GetTransform()->mLocalMatrix * XMMatrixTranslationFromVector(outTrans);
    }
    else
    {
        GetTransform()->mWorldMatrix = GetTransform()->mLocalMatrix;
    }
}

void SceneGeometry::UpdateObjectCBs(const ScaldTimer& st)
{
    mCB.SetTransform(GetTransform());
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