#include "Shaders.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

/////////////////// VERTEX SHADER ///////////////////
HRESULT VertexShader::Init(ID3D11Device* mDevice, D3D11_INPUT_ELEMENT_DESC* layoutDesc, UINT numElements, LPCWSTR pFileName)
{
    // Step 04: Compile the Shaders
    Microsoft::WRL::ComPtr<ID3DBlob> mErrorVertexCode = nullptr;
    HRESULT hr = D3DCompileFromFile(pFileName,
        nullptr /*macros*/,
        nullptr /*include*/,
        "main",
        "vs_5_0",
        D3DCOMPILE_DEBUG, //| D3DCOMPILE_SKIP_OPTIMIZATION,
        0u,
        mShaderBuffer.GetAddressOf(),
        &mErrorVertexCode);

    if (FAILED(hr)) return hr;

    hr = mDevice->CreateVertexShader(mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), nullptr, &mShader);
    if (FAILED(hr)) return hr;
    
    return mDevice->CreateInputLayout(layoutDesc, numElements, mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), &mInputLayout);
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
HRESULT PixelShader::Init(ID3D11Device* mDevice, LPCWSTR pFileName)
{
    Microsoft::WRL::ComPtr<ID3DBlob> mErrorPixelCode = nullptr;
    HRESULT hr = D3DCompileFromFile(pFileName,
        nullptr /*macros*/,
        nullptr /*include*/,
        "main",
        "ps_5_0" /*pixel shader*/,
        D3DCOMPILE_DEBUG, //| D3DCOMPILE_SKIP_OPTIMIZATION,
        0u,
        mShaderBuffer.GetAddressOf(),
        &mErrorPixelCode);

    if (FAILED(hr)) return hr;

    return mDevice->CreatePixelShader(mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), nullptr, &mShader);
}

ID3D11PixelShader* PixelShader::Get()
{
    return mShader.Get();
}

ID3DBlob* PixelShader::GetBuffer()
{
    return mShaderBuffer.Get();
}

/////////////////// GEOMETRY SHADER ///////////////////
HRESULT GeometryShader::Init(ID3D11Device* mDevice, LPCWSTR pFileName)
{
    Microsoft::WRL::ComPtr<ID3DBlob> mErrorPixelCode = nullptr;
    HRESULT hr = D3DCompileFromFile(pFileName,
        nullptr /*macros*/,
        nullptr /*include*/,
        "main",
        "gs_5_0" /*pixel shader*/,
        D3DCOMPILE_DEBUG, //| D3DCOMPILE_SKIP_OPTIMIZATION,
        0u,
        mShaderBuffer.GetAddressOf(),
        &mErrorPixelCode);

    if (FAILED(hr)) return hr;

    return mDevice->CreateGeometryShader(mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), nullptr, &mShader);
}

ID3D11GeometryShader* GeometryShader::Get()
{
    return mShader.Get();
}

ID3DBlob* GeometryShader::GetBuffer()
{
    return mShaderBuffer.Get();
}

/////////////////// COMPUTE SHADER ///////////////////
HRESULT ComputeShader::Init(ID3D11Device* mDevice, LPCWSTR pFileName)
{
    Microsoft::WRL::ComPtr<ID3DBlob> mErrorPixelCode = nullptr;
    HRESULT hr = D3DCompileFromFile(pFileName,
        nullptr /*macros*/,
        nullptr /*include*/,
        "main",
        "gs_5_0" /*pixel shader*/,
        D3DCOMPILE_DEBUG, //| D3DCOMPILE_SKIP_OPTIMIZATION,
        0u,
        mShaderBuffer.GetAddressOf(),
        &mErrorPixelCode);

    if (FAILED(hr)) return hr;

    return mDevice->CreateComputeShader(mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), nullptr, &mShader);
}

ID3D11ComputeShader* ComputeShader::Get()
{
    return mShader.Get();
}

ID3DBlob* ComputeShader::GetBuffer()
{
    return mShaderBuffer.Get();
}