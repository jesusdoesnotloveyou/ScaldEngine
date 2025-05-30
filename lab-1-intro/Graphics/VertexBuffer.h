#pragma once

#include <wrl.h>
#include <d3d11.h>

#pragma comment (lib, "d3d11.lib")

template<typename T>
class VertexBuffer
{
public:
    VertexBuffer() {}

    VertexBuffer(const VertexBuffer<T>& lhs)
    {
        mBuffer = lhs.mBuffer;
        bufferSize = lhs.bufferSize;
        stride = lhs.stride;
        offset = lhs.offset;
    }

    VertexBuffer<T>& operator=(const VertexBuffer<T>& lhs)
    {
        mBuffer = lhs.mBuffer;
        bufferSize = lhs.bufferSize;
        stride = lhs.stride;
        offset = lhs.offset;
        return *this;
    }

	ID3D11Buffer* Get() const { return mBuffer.Get(); }
	ID3D11Buffer* const* GetAddressOf() const { return mBuffer.GetAddressOf(); }
    UINT GetBufferSize() const { return bufferSize; }
    
    UINT GetStride() const { return stride; }
    const UINT* GetStridePtr() const { return &stride; }

    UINT GetOffset() const { return offset; }
    const UINT* GetOffsetPtr() const { return &offset; }

	HRESULT Init(ID3D11Device* device, T* data, UINT numVertices)
	{
        bufferSize = numVertices;
        stride = (UINT)sizeof(T);

        // Step 07: Create Vertex Buffer
        D3D11_BUFFER_DESC vertexBufDesc = {};
        ZeroMemory(&vertexBufDesc, sizeof(vertexBufDesc));

        vertexBufDesc.ByteWidth = sizeof(T) * numVertices;
        vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
        vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vertexBufDesc.CPUAccessFlags = 0u;
        vertexBufDesc.MiscFlags = 0u;
        vertexBufDesc.StructureByteStride = sizeof(T);

        D3D11_SUBRESOURCE_DATA vertexData = {};
        ZeroMemory(&vertexData, sizeof(vertexData));

        vertexData.pSysMem = data;
        vertexData.SysMemPitch = 0;
        vertexData.SysMemSlicePitch = 0;

        return device->CreateBuffer(&vertexBufDesc, &vertexData, mBuffer.GetAddressOf());
	}
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> mBuffer;
    UINT stride = 0;
    UINT offset = 0;
    UINT bufferSize = 0;
};