#pragma once

#include "ScaldCoreTypes.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"

class Mesh
{
private:
    Mesh(ID3D11DeviceContext* deviceContext);

public:
    Mesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const std::vector<VertexTex>& vertices, const std::vector<DWORD>& indeces);
    Mesh(const Mesh& mesh);

    void Draw();

    VertexBuffer<VertexTex>& GetVertexBuffer();
    IndexBuffer& GetIndexBuffer();

private:
    ID3D11DeviceContext* mDeviceContext = nullptr;

    VertexBuffer<VertexTex> mVB;
    IndexBuffer mIB;
};