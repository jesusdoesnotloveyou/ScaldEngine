#include "stdafx.h"
#include "Shaders.h"

using namespace Scald;
using namespace Microsoft::WRL;

/////////////////// VERTEX SHADER ///////////////////
HRESULT VertexShader::Init(ID3D11Device* pDevice, D3D11_INPUT_ELEMENT_DESC* layoutDesc, UINT numElements, LPCWSTR pFileName)
{
    HRESULT hr = Init(pDevice, pFileName);
    if (FAILED(hr)) return hr;

    return pDevice->CreateInputLayout(layoutDesc, numElements, m_shaderBuffer->GetBufferPointer(), m_shaderBuffer->GetBufferSize(), &m_inputLayout);
}

HRESULT VertexShader::Init(ID3D11Device* pDevice, LPCWSTR pFileName)
{
    ComPtr<ID3DBlob> errorVertexCode = nullptr;
    HRESULT hr = D3DCompileFromFile(pFileName, nullptr /*macros*/, nullptr /*include*/, "main", "vs_5_0",
        D3DCOMPILE_DEBUG,  //| D3DCOMPILE_SKIP_OPTIMIZATION,
        0u, m_shaderBuffer.GetAddressOf(), &errorVertexCode);

    if (FAILED(hr)) return hr;

    return pDevice->CreateVertexShader(m_shaderBuffer->GetBufferPointer(), m_shaderBuffer->GetBufferSize(), nullptr, &m_shader);
}

ID3D11InputLayout* VertexShader::GetInputLayout() const
{
    return m_inputLayout.Get();
}

/////////////////// PIXEL SHADER ///////////////////
HRESULT PixelShader::Init(ID3D11Device* pDevice, LPCWSTR pFileName)
{
    ComPtr<ID3DBlob> errorPixelCode = nullptr;
    HRESULT hr = D3DCompileFromFile(pFileName, nullptr /*macros*/, nullptr /*include*/, "main", "ps_5_0" /*pixel shader*/,
        D3DCOMPILE_DEBUG,  //| D3DCOMPILE_SKIP_OPTIMIZATION,
        0u, m_shaderBuffer.GetAddressOf(), &errorPixelCode);

    if (FAILED(hr)) return hr;

    return pDevice->CreatePixelShader(m_shaderBuffer->GetBufferPointer(), m_shaderBuffer->GetBufferSize(), nullptr, &m_shader);
}

/////////////////// GEOMETRY SHADER ///////////////////
HRESULT GeometryShader::Init(ID3D11Device* pDevice, LPCWSTR pFileName)
{
    ComPtr<ID3DBlob> errorGeometryCode = nullptr;
    HRESULT hr = D3DCompileFromFile(pFileName, nullptr /*macros*/, nullptr /*include*/, "main", "gs_5_0" /*geometry shader*/,
        D3DCOMPILE_DEBUG,  //| D3DCOMPILE_SKIP_OPTIMIZATION,
        0u, m_shaderBuffer.GetAddressOf(), &errorGeometryCode);

    if (FAILED(hr)) return hr;

    return pDevice->CreateGeometryShader(m_shaderBuffer->GetBufferPointer(), m_shaderBuffer->GetBufferSize(), nullptr, &m_shader);
}

/////////////////// COMPUTE SHADER ///////////////////
HRESULT ComputeShader::Init(ID3D11Device* pDevice, LPCWSTR pFileName)
{
    ComPtr<ID3DBlob> errorComputeCode = nullptr;
    HRESULT hr = D3DCompileFromFile(pFileName, nullptr /*macros*/, nullptr /*include*/, "main", "cs_5_0" /*compute shader*/,
        D3DCOMPILE_DEBUG,  //| D3DCOMPILE_SKIP_OPTIMIZATION,
        0u, m_shaderBuffer.GetAddressOf(), &errorComputeCode);

    if (FAILED(hr)) return hr;

    return pDevice->CreateComputeShader(m_shaderBuffer->GetBufferPointer(), m_shaderBuffer->GetBufferSize(), nullptr, &m_shader);
}