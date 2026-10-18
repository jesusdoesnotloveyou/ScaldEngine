#include "stdafx.h"
#include "Shaders.h"

using namespace Scald;
using namespace Microsoft::WRL;

/////////////////// VERTEX SHADER ///////////////////
void VertexShader::Init(ID3D11Device* mDevice, LPCWSTR pFileName, D3D11_INPUT_ELEMENT_DESC* layoutDesc, UINT numElements, const D3D_SHADER_MACRO* pDefines, ID3DInclude* pInclude, LPCSTR pEntryPoint)
{
    Super::Compile(mDevice, pFileName, pDefines, pInclude, pEntryPoint);
    
    CreateInputLayout(mDevice, layoutDesc, numElements);
    Create(mDevice);

    m_shaderBuffer.Reset();
}

void VertexShader::CreateInputLayout(ID3D11Device* pDevice, D3D11_INPUT_ELEMENT_DESC* layoutDesc, UINT numElements)
{
    ThrowIfFailed(pDevice->CreateInputLayout(layoutDesc, numElements, m_shaderBuffer->GetBufferPointer(), m_shaderBuffer->GetBufferSize(), &m_inputLayout));
}

void VertexShader::Create(ID3D11Device* pDevice)
{
    ThrowIfFailed(pDevice->CreateVertexShader(m_shaderBuffer->GetBufferPointer(), m_shaderBuffer->GetBufferSize(), nullptr, &m_shader));
}

ID3D11InputLayout* VertexShader::GetInputLayout() const
{
    return m_inputLayout.Get();
}

void PixelShader::Create(ID3D11Device* pDevice)
{
    ThrowIfFailed(pDevice->CreatePixelShader(m_shaderBuffer->GetBufferPointer(), m_shaderBuffer->GetBufferSize(), nullptr, &m_shader));
}

void GeometryShader::Create(ID3D11Device* pDevice)
{
    ThrowIfFailed(pDevice->CreateGeometryShader(m_shaderBuffer->GetBufferPointer(), m_shaderBuffer->GetBufferSize(), nullptr, &m_shader));
}

void ComputeShader::Create(ID3D11Device* pDevice)
{
    ThrowIfFailed(pDevice->CreateComputeShader(m_shaderBuffer->GetBufferPointer(), m_shaderBuffer->GetBufferSize(), nullptr, &m_shader));
}