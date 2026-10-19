#pragma once

#include "DXHelper.h"
#include "ScaldException.h"

namespace Scald
{
    /// <summary>
    /// Probably the first time I use CRTP personally, but it seems like a good idea to avoid code duplication for shader classes.
    /// The classes below are self-explanatory.
    /// </summary>
    template <typename TDerived, typename TShader>
    class Shader
    {
    public:
        Shader() = default;
        virtual ~Shader() = default;

        void Init(ID3D11Device* pDevice, LPCWSTR pFileName, const D3D_SHADER_MACRO* pDefines = nullptr,
            ID3DInclude* pInclude = D3D_COMPILE_STANDARD_FILE_INCLUDE, LPCSTR pEntryPoint = "main")
        {
            Compile(pDevice, pFileName, pDefines, pInclude, pEntryPoint);
            static_cast<TDerived*>(this)->Create(pDevice);
            // Blob is no longer needed after shader creation, so we can release it to free memory
            m_shaderBuffer.Reset();
        }

        TShader*  Get()       const { return m_shader.Get(); }
        ID3DBlob* GetBuffer() const { return m_shaderBuffer.Get(); }

    protected:

        void Compile(ID3D11Device* pDevice, LPCWSTR pFileName, const D3D_SHADER_MACRO* pDefines, ID3DInclude* pInclude, LPCSTR pEntryPoint)
        {
#if defined(DEBUG) || defined(_DEBUG)
            UINT flags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
            Microsoft::WRL::ComPtr<ID3DBlob> errorCode = nullptr;
            HRESULT hr = D3DCompileFromFile(pFileName, pDefines, pInclude, pEntryPoint,
                TDerived::ShaderTarget(), flags, 0u, m_shaderBuffer.GetAddressOf(), &errorCode);
            
            if (FAILED(hr))
            {
                if (errorCode)
                {
                    std::string errorMsg(static_cast<const char*>(errorCode->GetBufferPointer()), errorCode->GetBufferSize());
                    OutputDebugStringA(errorMsg.c_str());
                }
                ThrowIfFailed(hr);
            }
        }

    protected:
        Microsoft::WRL::ComPtr<TShader>  m_shader;
        Microsoft::WRL::ComPtr<ID3DBlob> m_shaderBuffer;
    };

    class VertexShader final : public Shader<VertexShader, ID3D11VertexShader>
    {
        using Super = Shader<VertexShader, ID3D11VertexShader>;
    public:
        VertexShader() = default;
        ~VertexShader() override = default;

        static constexpr const char* ShaderTarget() { return "vs_5_0"; }

        // Different Init function for vertex shader that also creates input layout, so don't call the base Init function directly
        void Init(ID3D11Device* mDevice, LPCWSTR pFileName, D3D11_INPUT_ELEMENT_DESC* layoutDesc, UINT numElements, const D3D_SHADER_MACRO* pDefines = nullptr,
            ID3DInclude* pInclude = D3D_COMPILE_STANDARD_FILE_INCLUDE, LPCSTR pEntryPoint = "main");
    
        void Create(ID3D11Device* pDevice);
        void CreateInputLayout(ID3D11Device* pDevice, D3D11_INPUT_ELEMENT_DESC* layoutDesc, UINT numElements);

        ID3D11InputLayout* GetInputLayout() const;

    private:
        Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
    };

    class PixelShader final : public Shader<PixelShader, ID3D11PixelShader>
    {
        using Super = Shader<PixelShader, ID3D11PixelShader>;
    public:
        PixelShader() = default;
        ~PixelShader() override = default;

        static constexpr const char* ShaderTarget() { return "ps_5_0"; }

        void Create(ID3D11Device* pDevice);
    };

    class GeometryShader final : public Shader<GeometryShader, ID3D11GeometryShader>
    {
        using Super = Shader<GeometryShader, ID3D11GeometryShader>;
    public:
        GeometryShader() = default;
        ~GeometryShader() override = default;

        static constexpr const char* ShaderTarget() { return "gs_5_0"; }
        
        void Create(ID3D11Device* pDevice);
    };

    class ComputeShader final : public Shader<ComputeShader, ID3D11ComputeShader>
    {
        using Super = Shader<ComputeShader, ID3D11ComputeShader>;
    public:
        ComputeShader() = default;
        ~ComputeShader() override = default;
        
        static constexpr const char* ShaderTarget() { return "cs_5_0"; }

        void Create(ID3D11Device* pDevice);
    };
}