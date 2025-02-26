#pragma once

#include "ScaldCoreTypes.h"
#include "RenderComponent.h"

class RenderComponent;

class PrimitiveGeometry
{
public:
	PrimitiveGeometry();
	/*PrimitiveGeometry(const PrimitiveGeometry&) = delete;
	PrimitiveGeometry(PrimitiveGeometry&&);
	PrimitiveGeometry& operator=(const PrimitiveGeometry&) = delete;
	PrimitiveGeometry operator=(PrimitiveGeometry&&);*/
	virtual ~PrimitiveGeometry();

	void Initialize(ID3D11Device* pDevice);

	ID3D11Buffer* const* GetAddressOfVertexBuffer() const;
	ID3D11Buffer* const* GetAddressOfIndexBuffer() const;
	ID3D11Buffer* GetVertexBuffer() const;
	ID3D11Buffer* GetIndexBuffer() const;
	
	RenderComponent* GetRenderComponent() const;

	UINT stride = { sizeof(Vertex) };
	UINT offset = { 0 };
	
protected:
	RenderComponent* pRenderComponent{ nullptr };

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
};