#include "../../../ScaldException.h"
#include "PrimitiveGeometry.h"
#include <sstream>

PrimitiveGeometry::PrimitiveGeometry()
{
    pRenderComponent = new RenderComponent{};
    pCollisionComponent = new CollisionComponent{};
    pMovementComponent = new MovementComponent{};
}

PrimitiveGeometry::PrimitiveGeometry(const std::tuple<std::vector<Vertex>, std::vector<DWORD>>& viPair)
{}

PrimitiveGeometry::~PrimitiveGeometry()
{
    if (pRenderComponent) delete pRenderComponent;
    if (pCollisionComponent) delete pCollisionComponent;
    if (pMovementComponent) delete pMovementComponent;
}

void PrimitiveGeometry::Update(const ScaldTimer& st)
{   
    ObjectTransform.rotationAngle += XMConvertToRadians(ObjectTransform.rotationSpeed) * st.DeltaTime();
    ObjectTransform.Rotation.y = ObjectTransform.rotationAngle;

    ObjectTransform.Translation.x = ObjectTransform.orbitRadius;
    // there is no need in manual translation update, everything is done through matrix multiplications
    /*ObjectTransform.orbitAngle += XMConvertToRadians(ObjectTransform.orbitSpeed) * st.DeltaTime();
    ObjectTransform.Translation.x = ObjectTransform.orbitRadius * cosf(XMConvertToRadians(ObjectTransform.orbitAngle));
    ObjectTransform.Translation.z = ObjectTransform.orbitRadius * sinf(XMConvertToRadians(ObjectTransform.orbitAngle));*/

    UpdateWorldMatrix();
}

void PrimitiveGeometry::UpdateWorldMatrix()
{
    // SRT - default order of matrix multiplication
    // (S)TR - orbit effect for Solar system could be used
    ObjectTransform.localMatrix = XMMatrixScaling(ObjectTransform.Scale.x, ObjectTransform.Scale.y, ObjectTransform.Scale.z) *
        XMMatrixTranslation(ObjectTransform.Translation.x, ObjectTransform.Translation.y, ObjectTransform.Translation.z) *
        XMMatrixRotationRollPitchYaw(ObjectTransform.Rotation.x, ObjectTransform.Rotation.y, ObjectTransform.Rotation.z);
        // orbit rotation: 0 (no rotation) or some value
        //*XMMatrixRotationY(ObjectTransform.orbitAngle);

    if (ObjectTransform.ParentTransform)
    {
        ObjectTransform.worldMatrix = ObjectTransform.localMatrix * ObjectTransform.ParentTransform->worldMatrix;
    }
}

void PrimitiveGeometry::Initialize(ID3D11Device* mDevice, ID3D11DeviceContext* pDeviceContext)
{
    ThrowIfFailed(vertexBuffer.Init(mDevice, vertices.data(), (UINT)vertices.size()));
    ThrowIfFailed(indexBuffer.Init(mDevice, indeces.data(), (UINT)indeces.size()));
    ThrowIfFailed(constantBuffer.Init(mDevice, pDeviceContext));
    constantBuffer.SetTransform(&ObjectTransform);
}

void PrimitiveGeometry::UpdateObjectCBs(const ScaldTimer& st)
{
    constantBuffer.SetTransform(&ObjectTransform);
}

VertexBuffer<Vertex>& PrimitiveGeometry::GetVertexBuffer()
{
    return vertexBuffer;
}

IndexBuffer& PrimitiveGeometry::GetIndexBuffer()
{
    return indexBuffer;
}

ConstantBuffer<ConstBuffer>& PrimitiveGeometry::GetConstantBuffer()
{
    return constantBuffer;
}

RenderComponent* PrimitiveGeometry::GetRenderComponent() const
{
    return pRenderComponent;
}

CollisionComponent* PrimitiveGeometry::GetCollisionComponent() const
{
    return pCollisionComponent;
}

MovementComponent* PrimitiveGeometry::GetMovementComponent() const
{
    return pMovementComponent;
}