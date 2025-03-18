#include "Shaders.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

/////////////////// VERTEX SHADER ///////////////////
HRESULT VertexShader::Init(ID3D11Device* pDevice, D3D11_INPUT_ELEMENT_DESC* layoutDesc, UINT numElements)
{
    // Step 04: Compile the Shaders
    Microsoft::WRL::ComPtr<ID3DBlob> mErrorVertexCode = nullptr;

    HRESULT hr = D3DCompileFromFile(L"./Shaders/MyVeryFirstShader.hlsl",
        nullptr /*macros*/,
        nullptr /*include*/,
        "main",
        "vs_5_0",
        D3DCOMPILE_DEBUG, //| D3DCOMPILE_SKIP_OPTIMIZATION,
        0u,
        mShaderBuffer.GetAddressOf(),
        &mErrorVertexCode);

    if (FAILED(hr)) return hr;

    hr = pDevice->CreateVertexShader(mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), nullptr, &mShader);
    if (FAILED(hr)) return hr;
    
    return pDevice->CreateInputLayout(layoutDesc, numElements, mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), &mInputLayout);
}

ID3D11VertexShader* VertexShader::Get()
{
    return mShader.Get();
}

ID3DBlob* VertexShader::GetBuffer()
{
    return mShaderBuffer.Get();
}

ID3D11InputLayout* VertexShader::GetInputLayout()
{
    return mInputLayout.Get();
}

/////////////////// PIXEL SHADER ///////////////////
HRESULT PixelShader::Init(ID3D11Device* pDevice)
{
    Microsoft::WRL::ComPtr<ID3DBlob> mErrorPixelCode = nullptr;
    HRESULT hr = D3DCompileFromFile(L"./Shaders/MyVerySecondShader.hlsl",
        nullptr /*macros*/,
        nullptr /*include*/,
        "main",
        "ps_5_0" /*pixel shader*/,
        D3DCOMPILE_DEBUG, //| D3DCOMPILE_SKIP_OPTIMIZATION,
        0u,
        mShaderBuffer.GetAddressOf(),
        &mErrorPixelCode);

    if (FAILED(hr)) return hr;

    return pDevice->CreatePixelShader(mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), nullptr, &mShader);
}

ID3D11PixelShader* PixelShader::Get()
{
    return mShader.Get();
}

ID3DBlob* PixelShader::GetBuffer()
{
    return mShaderBuffer.Get();
}