#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>

class VertexShader 
{
public:
	VertexShader() = default;
	
	HRESULT Init(ID3D11Device* mDevice, D3D11_INPUT_ELEMENT_DESC* layoutDesc, UINT numElements, LPCWSTR pFileName);

	ID3D11VertexShader* Get();
	ID3DBlob* GetBuffer();
	ID3D11InputLayout* GetInputLayout();
	
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> mShader;
	Microsoft::WRL::ComPtr<ID3DBlob> mShaderBuffer;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout;
};

class PixelShader
{
public:
	PixelShader() = default;

	HRESULT Init(ID3D11Device* mDevice, LPCWSTR pFileName);

	ID3D11PixelShader* Get();
	ID3DBlob* GetBuffer();

private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> mShader;
	Microsoft::WRL::ComPtr<ID3DBlob> mShaderBuffer;
};

class GeometryShader
{
public:
	GeometryShader() = default;

	HRESULT Init(ID3D11Device* mDevice, LPCWSTR pFileName);

	ID3D11GeometryShader* Get();
	ID3DBlob* GetBuffer();

private:
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> mShader;
	Microsoft::WRL::ComPtr<ID3DBlob> mShaderBuffer;
};