#include "ScaldException.h"
#include "PrimitiveGeometry.h"

PrimitiveGeometry::PrimitiveGeometry()
{
    pRenderComponent = new RenderComponent();
}

PrimitiveGeometry::PrimitiveGeometry(const std::vector<Vertex>& v, const std::vector<int>& i)
    : PrimitiveGeometry()
{
    vertices = v;
    indeces = i;
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

    // Step 07: Create Vertex and Index Buffers
    D3D11_BUFFER_DESC vertexBufDesc = {};
    vertexBufDesc.ByteWidth = UINT(sizeof(Vertex) * vertices.size());
    vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufDesc.CPUAccessFlags = 0u;
    vertexBufDesc.MiscFlags = 0u;
    vertexBufDesc.StructureByteStride = sizeof(Vertex);

    D3D11_SUBRESOURCE_DATA vertexData = {};
    vertexData.pSysMem = vertices.data();
    vertexData.SysMemPitch = 0;
    vertexData.SysMemSlicePitch = 0;

    ThrowIfFailed(pDevice->CreateBuffer(&vertexBufDesc, &vertexData, &pVertexBuffer));

    D3D11_BUFFER_DESC indexBufDesc = {};
    indexBufDesc.ByteWidth = UINT(sizeof(int) * indeces.size());
    indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufDesc.CPUAccessFlags = 0u;
    indexBufDesc.MiscFlags = 0u;
    indexBufDesc.StructureByteStride = sizeof(int);

    D3D11_SUBRESOURCE_DATA indexData = {};
    indexData.pSysMem = indeces.data();
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
