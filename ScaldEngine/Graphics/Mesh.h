#pragma once

#include "ScaldCoreTypes.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"

namespace Scald
{    
    class Mesh
    {
    private:
        Mesh(ID3D11DeviceContext* deviceContext);
        
    public:
        Mesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const std::vector<VertexPositionNormalUV>& vertices, const std::vector<DWORD>& indices);
        Mesh(const Mesh& mesh);
        
        void Draw() const;
        
        VertexBuffer<VertexPositionNormalUV>& GetVertexBuffer();
        IndexBuffer& GetIndexBuffer();
        
    private:
        ID3D11DeviceContext* mDeviceContext = nullptr;
        
        VertexBuffer<VertexPositionNormalUV> mVB;
        IndexBuffer mIB;
    };
}