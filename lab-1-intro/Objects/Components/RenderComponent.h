#pragma once

#include "IScaldComponent.h"

#include <wrl.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

class RenderComponent : public IScaldComponent
{
public:
	RenderComponent();
	virtual ~RenderComponent() noexcept override;

	// Begin of IScaldComponent interface
	virtual void Update();
	// End of IScaldComponent interface

	void Initialize(ID3D11Device* pDevice);

	ID3D11VertexShader* GetVertexShader() const;
	ID3D11PixelShader* GetPixelShader() const;
	ID3D11InputLayout* GetInputLayout() const;
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pLayout;
};