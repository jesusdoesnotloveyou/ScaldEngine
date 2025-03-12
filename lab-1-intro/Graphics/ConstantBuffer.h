#pragma once

#include "../ScaldException.h"
#include <wrl.h>
#include <d3d11.h>

#include "ScaldCoreTypes.h"

#pragma comment(lib, "d3d11.lib")

template<typename T>
class ConstantBuffer
{
public:
	ConstantBuffer()
	{
		cb.transform = {
			(768.f / 1024.f) * 1.0f,	0.0f,	0.0f,	0.0f,
			0.0f,						1.0f,	0.0f,	0.0f,
			0.0f,						0.0f,	1.0f,	0.0f,
			0.0f,						0.0f,	0.0f,	1.0f
		};
	}
	ConstantBuffer(const ConstantBuffer& rhs) = delete;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pBuffer;
	ID3D11DeviceContext* pDeviceContext = nullptr;

	STransform mTransform;
public:
	// ?
	ConstBuffer cb;
	T curr_data;


	void SetTransform(const STransform& transform)
	{
		mTransform = transform;
	}
	
	ID3D11Buffer* Get() const { return pBuffer.Get(); }
	ID3D11Buffer* const* GetAddressOf() const { return pBuffer.GetAddressOf(); }
	
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

		return device->CreateBuffer(&constantBufDesc, 0, pBuffer.GetAddressOf());
	}

	bool ApplyChanges(const T& data)
	{
		curr_data = data;
		DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixTranslation(0.0f, mTransform.Position.y, 0.0f);

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		ThrowIfFailed(pDeviceContext->Map(pBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

		curr_data.transform = DirectX::XMMatrixTranspose(worldMatrix);
		CopyMemory(mappedResource.pData, &curr_data, sizeof(T));
		pDeviceContext->Unmap(pBuffer.Get(), 0);
		return true;
	}
};