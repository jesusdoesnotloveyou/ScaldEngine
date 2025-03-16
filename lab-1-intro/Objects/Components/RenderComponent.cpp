#include "../../ScaldException.h"
#include "RenderComponent.h"

RenderComponent::RenderComponent()
{

}

RenderComponent::~RenderComponent() noexcept
{

}

void RenderComponent::Update(float DeltaTime)
{

}

void RenderComponent::Initialize(ID3D11Device* pDevice)
{
	// Step 04: Compile the Shaders
	Microsoft::WRL::ComPtr<ID3DBlob> pVertexBC = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> pErrorVertexCode = nullptr;
	ThrowIfFailed(D3DCompileFromFile(L"./Shaders/MyVeryFirstShader.hlsl",
		nullptr /*macros*/,
		nullptr /*include*/,
		"main",
		"vs_5_0",
		D3DCOMPILE_DEBUG, //| D3DCOMPILE_SKIP_OPTIMIZATION,
		0u,
		&pVertexBC,
		&pErrorVertexCode));

	//D3D_SHADER_MACRO Shader_Macros[] = { "TEST", "1", "TCOLOR", "float4(0.0f, 1.0f, 0.0f, 1.0f)", nullptr, nullptr };

	Microsoft::WRL::ComPtr<ID3DBlob> pPixelBC = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> pErrorPixelCode = nullptr;
	ThrowIfFailed(D3DCompileFromFile(L"./Shaders/MyVerySecondShader.hlsl",
		nullptr /*macros*/,
		nullptr /*include*/,
		"main",
		"ps_5_0" /*pixel shader*/,
		D3DCOMPILE_DEBUG, //| D3DCOMPILE_SKIP_OPTIMIZATION,
		0u,
		&pPixelBC,
		&pErrorPixelCode));

	ThrowIfFailed(pDevice->CreateVertexShader(
		pVertexBC->GetBufferPointer(),
		pVertexBC->GetBufferSize(),
		nullptr, &pVertexShader));

	ThrowIfFailed(pDevice->CreatePixelShader(
		pPixelBC->GetBufferPointer(),
		pPixelBC->GetBufferSize(),
		nullptr, &pPixelShader));

	// Step 05: Create Input Layout for IA Stage
	D3D11_INPUT_ELEMENT_DESC inputElements[] = {
		D3D11_INPUT_ELEMENT_DESC {
			"POSITION",
			0u,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0u,
			0u,
			D3D11_INPUT_PER_VERTEX_DATA,
			0u},
		D3D11_INPUT_ELEMENT_DESC {
			"COLOR",
			0u,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0u,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0u}
	};

	ThrowIfFailed(pDevice->CreateInputLayout(
		inputElements,
		2u,
		pVertexBC->GetBufferPointer(),
		pVertexBC->GetBufferSize(),
		&pLayout));
}

ID3D11PixelShader* RenderComponent::GetPixelShader() const
{
    return pPixelShader.Get();
}

ID3D11InputLayout* RenderComponent::GetInputLayout() const
{
	return pLayout.Get();
}

ID3D11VertexShader* RenderComponent::GetVertexShader() const
{
    return pVertexShader.Get();
}