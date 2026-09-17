#pragma once

#include "DXHelper.h"
#include "ScaldException.h"

#include <vector>
#include <cassert>

namespace Scald
{
    using namespace Microsoft::WRL;

    template <typename T>
    class StructuredBuffer
    {
    public:
        // Created an empty buffer
        StructuredBuffer(ID3D11Device* device, ID3D11DeviceContext* deviceContext, uint32_t elementsNum)
            : m_elementsNum(elementsNum)
        {
            D3D11_BUFFER_DESC structuredBufferDesc = {};
            structuredBufferDesc.ByteWidth = static_cast<UINT>((sizeof(T) * m_elementsNum + D3D11_ALIGNMENT_SIZE) & ~D3D11_ALIGNMENT_SIZE);
            structuredBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
            structuredBufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE; // ?
            structuredBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            structuredBufferDesc.MiscFlags = 0u;
            structuredBufferDesc.StructureByteStride = sizeof(T);
            
            ThrowIfFailed(device->CreateBuffer(&structuredBufferDesc, nullptr, m_resource.GetAddressOf()));
        }

        // Creates a buffer with initial data
        StructuredBuffer(ID3D11Device* device, ID3D11Buffer** buffer, const std::vector<T>& bufferData)
        {
            UINT stride = static_cast<UINT>(sizeof(T));
            UINT byteWidth = stride * static_cast<UINT>(bufferData.size());

            D3D11_BUFFER_DESC desc = {};
            desc.ByteWidth = byteWidth;
            desc.Usage = D3D11_USAGE_DYNAMIC;              // to use map/unmap to update
            desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;   // to get from GPU
            desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;  // from cpu
            desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
            desc.StructureByteStride = stride;

            D3D11_SUBRESOURCE_DATA data = {};
            data.pSysMem = bufferData.data();
            data.SysMemPitch = 0u;
            data.SysMemSlicePitch = 0u;

            ThrowIfFailed(device->CreateBuffer(&desc, &data, buffer));
        }

        ~StructuredBuffer() noexcept {} 

        StructuredBuffer(const StructuredBuffer& lhs) = delete;
        StructuredBuffer& operator=(const StructuredBuffer& lhs) = delete;
        StructuredBuffer(StructuredBuffer&& rhs) noexcept  = delete;
        StructuredBuffer& operator=(StructuredBuffer&& rhs) noexcept  = delete;

    public:
        ID3D11Buffer* Get() const { return m_resource.Get(); }
        ID3D11Buffer* const* GetAddressOf() const { return m_resource.GetAddressOf(); }

        bool SetAndApplyData(const std::vector<T>& buffer)
        {
            D3D11_MAPPED_SUBRESOURCE mappedResource;
            ThrowIfFailed(m_pDeviceContext->Map(m_resource.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
            
            CopyMemory(mappedResource.pData, buffer.data(), sizeof(T) * buffer.size());
            m_pDeviceContext->Unmap(m_resource.Get(), 0);
            return true;
        }

    private:
        uint32_t m_elementsNum = 0u;
        ID3D11DeviceContext* m_pDeviceContext;
        ComPtr<ID3D11Buffer> m_resource;
        // Should consider on storing the whole source buffer in here
    };
}