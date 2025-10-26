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
	ConstantBuffer(const ConstantBuffer& lhs) = delete;
public:
	ID3D11Buffer* Get() const { return mBuffer.Get(); }
	ID3D11Buffer* const* GetAddressOf() const { return mBuffer.GetAddressOf(); }
	
	HRESULT Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	{
		pDeviceContext = deviceContext;

		// Create Constant Buffer
		D3D11_BUFFER_DESC constantBufDesc = {};
		constantBufDesc.ByteWidth = UINT((sizeof(T) + 15) & ~15);
		constantBufDesc.Usage = D3D11_USAGE_DYNAMIC;
		constantBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constantBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		constantBufDesc.MiscFlags = 0u;
		constantBufDesc.StructureByteStride = 0u;

		return device->CreateBuffer(&constantBufDesc, 0, mBuffer.GetAddressOf());
	}

	bool ApplyChanges()
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		ThrowIfFailed(pDeviceContext->Map(mBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

		CopyMemory(mappedResource.pData, &curr_data, sizeof(T));
		pDeviceContext->Unmap(mBuffer.Get(), 0);
		return true;
	}

public:

	T* GetData() const
	{
		return &curr_data;
	}

	void SetData(const T& data)
	{
		curr_data = data;
	}

	void SetAndApplyData(const T& data)
	{
		curr_data = data;
		ApplyChanges();
	}

private:
	T curr_data;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mBuffer;
	ID3D11DeviceContext* pDeviceContext = nullptr;
};