#include "../../ScaldException.h"
#include "PrimitiveGeometry.h"

PrimitiveGeometry::PrimitiveGeometry()
{
    pRenderComponent = new RenderComponent{};
    pCollisionComponent = new CollisionComponent{};
    pInputComponent = new InputComponent{};
    pMovementComponent = new MovementComponent{};
}

PrimitiveGeometry::PrimitiveGeometry(const std::vector<Vertex>& v, const std::vector<DWORD>& i)
    : PrimitiveGeometry()
{
    vertices = v;
    indeces = i;
}

//PrimitiveGeometry::PrimitiveGeometry(const PrimitiveGeometry& lhs)
//{
//
//}
//
//PrimitiveGeometry::PrimitiveGeometry(PrimitiveGeometry&& rhs)
//{
//    
//}
//
//PrimitiveGeometry& PrimitiveGeometry::operator=(const PrimitiveGeometry& lhs)
//{
//
//}
//
//PrimitiveGeometry& PrimitiveGeometry::operator=(PrimitiveGeometry&& rhs)
//{
//
//}

PrimitiveGeometry::~PrimitiveGeometry()
{
    if (pRenderComponent) delete pRenderComponent;
    if (pCollisionComponent) delete pCollisionComponent;
    if (pMovementComponent) delete pMovementComponent;
    if (pInputComponent) delete pInputComponent;
}

void PrimitiveGeometry::Update(float DeltaTime)
{
    constantBuffer.SetTransform(ObjectTransform);

    pCollisionComponent->UpdateOwnerTransform(ObjectTransform);

    // need a cycle for object's components
    //pCollisionComponent->Update(DeltaTime);
    //pRenderComponent->Update(DeltaTime);
    //pMovementComponent->Update(DeltaTime);
    //pInputComponent->Update(DeltaTime);
}

void PrimitiveGeometry::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
    pRenderComponent->Initialize(pDevice);
    pCollisionComponent->Initialize(ObjectTransform);
    //pInputComponent->Initialize();

    ThrowIfFailed(vertexBuffer.Init(pDevice, vertices.data(), (UINT)vertices.size()));
    ThrowIfFailed(indexBuffer.Init(pDevice, indeces.data(), (UINT)indeces.size()));
    ThrowIfFailed(constantBuffer.Init(pDevice, pDeviceContext));
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