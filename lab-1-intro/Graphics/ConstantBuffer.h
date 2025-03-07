#pragma once

#include "../ScaldException.h"
#include <wrl.h>
#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")

template<typename T>
class ConstantBuffer
{
public:
	ConstantBuffer() {}
	ConstantBuffer(const ConstantBuffer& rhs) = delete;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pBuffer;
	ID3D11DeviceContext* pDeviceContext = nullptr;

public:
	// ?
	T data;
	
	ID3D11Buffer* Get() const { return pBuffer.Get(); }
	ID3D11Buffer* const* GetAddressOf() const { return pBuffer.GetAddressOf(); }
	
	HRESULT Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	{
		pDeviceContext = deviceContext;

		// Create Constant Buffer
		D3D11_BUFFER_DESC constantBufDesc = {};
		//constantBufDesc.ByteWidth = UINT(sizeof(T) + (16 - (sizeof(T) % 16)));
		constantBufDesc.ByteWidth = UINT(sizeof(T));
		constantBufDesc.Usage = D3D11_USAGE_DYNAMIC;
		constantBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constantBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		constantBufDesc.MiscFlags = 0u;
		constantBufDesc.StructureByteStride = 0u;

		/*D3D11_SUBRESOURCE_DATA constantData = {};
		constantData.pSysMem = &cb;
		constantData.SysMemPitch = 0u;
		constantData.SysMemSlicePitch = 0u;*/

		return device->CreateBuffer(&constantBufDesc, 0, pBuffer.GetAddressOf());
	}

	bool ApplyChanges(const T& data)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		ThrowIfFailed(pDeviceContext->Map(pBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
		CopyMemory(mappedResource.pData, &data, sizeof(T));
		pDeviceContext->Unmap(pBuffer.Get(), 0);
		return true;
	}
};