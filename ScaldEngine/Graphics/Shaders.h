#pragma once

#include "DXHelper.h"

namespace Scald
{
    using namespace Microsoft::WRL;

    template <typename T>
    class Shader
    {
    public:
        Shader() = default;
        virtual ~Shader() = default;
        virtual HRESULT Init(ID3D11Device* pDevice, LPCWSTR pFileName) = 0;

        T* Get() const { return m_shader.Get(); }
        ID3DBlob* GetBuffer() const { return m_shaderBuffer.Get(); }

    protected:
        ComPtr<T> m_shader;
        ComPtr<ID3DBlob> m_shaderBuffer;
    };

    class VertexShader final : public Shader<ID3D11VertexShader>
    {
    public:
        VertexShader() = default;
        virtual ~VertexShader() override = default;

        HRESULT Init(ID3D11Device* mDevice, D3D11_INPUT_ELEMENT_DESC* layoutDesc, UINT numElements, LPCWSTR pFileName);
        ID3D11InputLayout* GetInputLayout() const;
        
    private:
        virtual HRESULT Init(ID3D11Device* pDevice, LPCWSTR pFileName) override;

    private:
        ComPtr<ID3D11InputLayout> m_inputLayout;
    };

    class PixelShader final : public Shader<ID3D11PixelShader>
    {
    public:
        PixelShader() = default;
        virtual ~PixelShader() override = default;
        virtual HRESULT Init(ID3D11Device* pDevice, LPCWSTR pFileName) override;
    };

    class GeometryShader final : public Shader<ID3D11GeometryShader>
    {
    public:
        GeometryShader() = default;
        virtual ~GeometryShader() override = default;
        virtual HRESULT Init(ID3D11Device* pDevice, LPCWSTR pFileName) override;

    };

    class ComputeShader final : public Shader<ID3D11ComputeShader>
    {
    public:
        ComputeShader() = default;
        virtual ~ComputeShader() override = default;
        virtual HRESULT Init(ID3D11Device* pDevice, LPCWSTR pFileName) override;
    };
}