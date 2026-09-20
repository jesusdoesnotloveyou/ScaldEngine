#pragma once

#include "DXHelper.h"
#include "ScaldException.h"

namespace Scald
{
    using namespace DirectX;
    using namespace Microsoft::WRL;

    template <typename T>
    class ConstantBuffer
    {
    public:
        ConstantBuffer() {}
        ConstantBuffer(const ConstantBuffer& lhs) = delete;

    public:
        ID3D11Buffer* Get() const { return m_resource.Get(); }
        ID3D11Buffer* const* GetAddressOf() const { return m_resource.GetAddressOf(); }

        HRESULT Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
        {
            m_deviceContext = deviceContext;

            // Create Constant Buffer
            D3D11_BUFFER_DESC constantBufDesc = {};
            constantBufDesc.ByteWidth = UINT((sizeof(T) + 15) & ~15);
            constantBufDesc.Usage = D3D11_USAGE_DYNAMIC;
            constantBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            constantBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            constantBufDesc.MiscFlags = 0u;
            constantBufDesc.StructureByteStride = 0u;

            return device->CreateBuffer(&constantBufDesc, 0, m_resource.GetAddressOf());
        }

    public:
        void SetData(const T& data) { m_currData = data; }

        void SetAndApplyData(const T& data)
        {
            m_currData = data;
            ApplyChanges();
        }

    private:
        bool ApplyChanges()
        {
            D3D11_MAPPED_SUBRESOURCE mappedResource;
            ThrowIfFailed(m_deviceContext->Map(m_resource.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

            CopyMemory(mappedResource.pData, &m_currData, sizeof(T));
            m_deviceContext->Unmap(m_resource.Get(), 0);
            return true;
        }
    private:
        T m_currData;
        ComPtr<ID3D11Buffer> m_resource;
        ID3D11DeviceContext* m_deviceContext = nullptr;
    };
}