#include "ScaldException.h"
#include "PrimitiveGeometry.h"

PrimitiveGeometry::PrimitiveGeometry()
{
    pRenderComponent = new RenderComponent();
}

PrimitiveGeometry::~PrimitiveGeometry()
{
    if (pRenderComponent)
    {
        delete pRenderComponent;
    }
}

void PrimitiveGeometry::Initialize(ID3D11Device* pDevice)
{
    pRenderComponent->Initialize(pDevice);

    // Step 06: Create Set of Points
    Vertex points[4]
    {
        { DirectX::XMFLOAT4(0.3f, 0.3f, 0.5f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
        { DirectX::XMFLOAT4(-0.3f, -0.3f, 0.5f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
        { DirectX::XMFLOAT4(0.3f, -0.3f, 0.5f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
        { DirectX::XMFLOAT4(-0.3f, 0.3f, 0.5f, 1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) }
    };

    // Step 07: Create Vertex and Index Buffers
    D3D11_BUFFER_DESC vertexBufDesc = {};
    vertexBufDesc.ByteWidth = sizeof(points);
    vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufDesc.CPUAccessFlags = 0u;
    vertexBufDesc.MiscFlags = 0u;
    vertexBufDesc.StructureByteStride = sizeof(Vertex);

    D3D11_SUBRESOURCE_DATA vertexData = {};
    vertexData.pSysMem = points;
    vertexData.SysMemPitch = 0;
    vertexData.SysMemSlicePitch = 0;

    ThrowIfFailed(pDevice->CreateBuffer(&vertexBufDesc, &vertexData, &pVertexBuffer));

    int indeces[] = { 0, 1, 2,  1, 0, 3 };
    D3D11_BUFFER_DESC indexBufDesc = {};
    indexBufDesc.ByteWidth = sizeof(indeces);
    indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufDesc.CPUAccessFlags = 0u;
    indexBufDesc.MiscFlags = 0u;
    indexBufDesc.StructureByteStride = sizeof(int);

    D3D11_SUBRESOURCE_DATA indexData = {};
    indexData.pSysMem = indeces;
    indexData.SysMemPitch = 0u;
    indexData.SysMemSlicePitch = 0u;

    ThrowIfFailed(pDevice->CreateBuffer(&indexBufDesc, &indexData, &pIndexBuffer));
}

ID3D11Buffer* PrimitiveGeometry::GetVertexBuffer() const
{
    return pVertexBuffer.Get();
}

ID3D11Buffer* const* PrimitiveGeometry::GetAddressOfVertexBuffer() const
{
    return pVertexBuffer.GetAddressOf();
}

ID3D11Buffer* const* PrimitiveGeometry::GetAddressOfIndexBuffer() const
{
    return pIndexBuffer.GetAddressOf();
}

ID3D11Buffer* PrimitiveGeometry::GetIndexBuffer() const
{
    return pIndexBuffer.Get();
}

RenderComponent* PrimitiveGeometry::GetRenderComponent() const
{
    return pRenderComponent;
}
