#pragma once

#include "DXHelper.h"

namespace Scald
{
    using namespace Microsoft::WRL;
    using namespace DirectX;

    template <typename T = VertexPositionNormalUV>
    class VertexBuffer
    {
    public:
        VertexBuffer()
        {
            m_stride = 0u;
            m_offset = 0u;
            m_size = 0u;
        }

        VertexBuffer(const VertexBuffer<T>& lhs)
        {
            m_resource = lhs.m_resource;
            m_size = lhs.m_size;
            m_stride = lhs.m_stride;
            m_offset = lhs.m_offset;
        }
        
        VertexBuffer(VertexBuffer<T>&& lhs) noexcept
        {
            m_resource = std::move(lhs.m_resource);
            m_size = lhs.m_size;
            m_stride = lhs.m_stride;
            m_offset = lhs.m_offset;
            
            lhs.Reset();
        }

        VertexBuffer<T>& operator=(const VertexBuffer<T>& lhs)
        {
            m_resource = lhs.m_resource;
            m_size = lhs.m_size;
            m_stride = lhs.m_stride;
            m_offset = lhs.m_offset;
            return *this;
        }

        VertexBuffer<T>& operator=(VertexBuffer<T>&& lhs) noexcept
        {
            m_resource = std::move(lhs.m_resource);
            m_size = lhs.m_size;
            m_stride = lhs.m_stride;
            m_offset = lhs.m_offset;

            lhs.Reset();

            return *this;
        }

        ID3D11Buffer* Get() const { return m_resource.Get(); }
        ID3D11Buffer* const* GetAddressOf() const { return m_resource.GetAddressOf(); }
        UINT GetBufferSize() const { return m_size; }

        UINT GetStride() const { return m_stride; }
        const UINT* GetStridePtr() const { return &m_stride; }

        UINT GetOffset() const { return m_offset; }
        const UINT* GetOffsetPtr() const { return &m_offset; }

        HRESULT Init(ID3D11Device* device, const T* data, UINT numVertices)
        {
            m_size = numVertices;
            m_stride = (UINT)sizeof(T);
            
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
            
            return device->CreateBuffer(&vertexBufDesc, &vertexData, m_resource.GetAddressOf());
        }

    private:
        void Reset()
        {
            m_resource.Reset();
            m_stride = 0u;
            m_offset = 0u;
            m_size = 0u;
        }

    private:
        ComPtr<ID3D11Buffer> m_resource;
        UINT m_stride = 0u;
        UINT m_offset = 0u;
        UINT m_size = 0u;
    };
}