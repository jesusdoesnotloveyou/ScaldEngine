#include "../../../ScaldException.h"
#include "PrimitiveGeometry.h"
#include <sstream>

PrimitiveGeometry::PrimitiveGeometry()
{
    pRenderComponent = new RenderComponent{};
    pCollisionComponent = new CollisionComponent{};
    pMovementComponent = new MovementComponent{};
}

PrimitiveGeometry::PrimitiveGeometry(const STransform& transform) : PrimitiveGeometry()
{
    ObjectTransform = transform;
    // ?
    constantBuffer.SetTransform(ObjectTransform);
}

PrimitiveGeometry::PrimitiveGeometry(const std::tuple<std::vector<Vertex>, std::vector<DWORD>>& viPair)
{

}

PrimitiveGeometry::~PrimitiveGeometry()
{
    if (pRenderComponent) delete pRenderComponent;
    if (pCollisionComponent) delete pCollisionComponent;
    if (pMovementComponent) delete pMovementComponent;
}

void PrimitiveGeometry::Update(const ScaldTimer& st)
{   
    ObjectTransform.Rotation.y += ObjectTransform.Rotation.y / 180.f * XM_PI * st.DeltaTime();
    ObjectTransform.OrbitalRotation.y += ObjectTransform.OrbitalRotation.y / 180.f * XM_PI * st.DeltaTime();
}

void PrimitiveGeometry::Initialize(ID3D11Device* mDevice, ID3D11DeviceContext* pDeviceContext)
{
    ThrowIfFailed(vertexBuffer.Init(mDevice, vertices.data(), (UINT)vertices.size()));
    ThrowIfFailed(indexBuffer.Init(mDevice, indeces.data(), (UINT)indeces.size()));
    ThrowIfFailed(constantBuffer.Init(mDevice, pDeviceContext));
    constantBuffer.SetTransform(ObjectTransform);
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