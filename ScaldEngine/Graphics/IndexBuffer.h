#pragma once

#include "DXHelper.h"

namespace Scald
{
    using namespace Microsoft::WRL;
    using namespace DirectX;

    template <typename T = DWORD>
    class IndexBuffer
    {
    public:
        IndexBuffer()
        {
            m_size = 0u;
        }

        IndexBuffer(const IndexBuffer<T>& ib)
        { 
            m_resource = ib.m_resource;
            m_size = ib.m_size;
        }

        IndexBuffer(IndexBuffer<T>&& ib) noexcept
        {
            m_resource = std::move(ib.m_resource);
            m_size = ib.m_size;

            ib.Reset();
        }
        
        IndexBuffer& operator=(const IndexBuffer<T>& ib)
        {
            m_resource = ib.m_resource;
            m_size = ib.m_size;
            return *this;
        }

        IndexBuffer& operator=(IndexBuffer<T>&& ib) noexcept
        {
            m_resource = std::move(ib.m_resource);
            m_size = ib.m_size;

            ib.Reset();

            return *this;
        }

        ID3D11Buffer* Get() const { return m_resource.Get(); }
        ID3D11Buffer* const* GetAddressOf() const { return m_resource.GetAddressOf(); }
        UINT GetBufferSize() const { return m_size; }

        HRESULT Init(ID3D11Device* device, const T* data, UINT numIndices)
        {
            m_size = numIndices;
            auto stride = sizeof(T);

            // Step 07: Create Index Buffer
            D3D11_BUFFER_DESC indexBufDesc = {};
            indexBufDesc.ByteWidth = stride * numIndices;
            indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
            indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
            indexBufDesc.CPUAccessFlags = 0u;
            indexBufDesc.MiscFlags = 0u;
            indexBufDesc.StructureByteStride = stride;

            D3D11_SUBRESOURCE_DATA indexData = {};
            indexData.pSysMem = data;
            indexData.SysMemPitch = 0u;
            indexData.SysMemSlicePitch = 0u;

            return device->CreateBuffer(&indexBufDesc, &indexData, m_resource.GetAddressOf());
        }

    private:

        void Reset()
        {   
            m_resource.Reset();
            m_size = 0u;
        }

    private:
        ComPtr<ID3D11Buffer> m_resource;
        UINT m_size = 0;
    };
}  // namespace Scald