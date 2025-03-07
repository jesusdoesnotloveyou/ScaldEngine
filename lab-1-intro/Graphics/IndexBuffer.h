#pragma once

#include <wrl.h>
#include <d3d11.h>

#pragma comment (lib, "d3d11.lib")

class IndexBuffer
{
public:
	IndexBuffer() {}

	IndexBuffer(const IndexBuffer& rhs) = delete;

	ID3D11Buffer* Get() const { return pBuffer.Get(); }
	ID3D11Buffer* const* GetAddressOf() const { return pBuffer.GetAddressOf(); }
	UINT GetBufferSize() const { return bufferSize; }

	HRESULT Init(ID3D11Device* device, DWORD* data, UINT numIndeces)
	{
		bufferSize = numIndeces;

		// Step 07: Create Index Buffer
		D3D11_BUFFER_DESC indexBufDesc = {};
		indexBufDesc.ByteWidth = sizeof(DWORD) * numIndeces;
		indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufDesc.CPUAccessFlags = 0u;
		indexBufDesc.MiscFlags = 0u;
		indexBufDesc.StructureByteStride = sizeof(DWORD);

		D3D11_SUBRESOURCE_DATA indexData = {};
		indexData.pSysMem = data;
		indexData.SysMemPitch = 0u;
		indexData.SysMemSlicePitch = 0u;

		return device->CreateBuffer(&indexBufDesc, &indexData, pBuffer.GetAddressOf());
	}

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pBuffer;
	UINT bufferSize = 0;
};