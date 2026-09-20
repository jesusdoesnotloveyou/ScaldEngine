#pragma once

#include "Graphics/DXHelper.h"
#include "ScaldCoreDefines.h"

namespace Scald
{
    using namespace DirectX;
    using Microsoft::WRL::ComPtr;
    // SCALD MATH
    namespace ScaldMath
    {
        static const XMVECTOR ForwardVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
        static const XMVECTOR RightVector = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
        static const XMVECTOR UpVector = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
        static const XMVECTOR ZeroVector = XMVectorZero();
    }  // namespace ScaldMath

    // just position and color
    struct VertexPositionColor
    {
        VertexPositionColor() {}
        VertexPositionColor(float x, float y, float z, float w, float r, float g, float b, float a)
        : position(x, y, z, w)
        , color(r, g, b, a)
        {
        }
        
        // to support direct colors
        VertexPositionColor(float x, float y, float z, float w, const XMVECTORF32 v)
            : position(x, y, z, w),
            color(v.f[0], v.f[1], v.f[2], v.f[3])
            {
            }
            
            XMFLOAT4 position = {0.0f, 0.0f, 0.0f, 1.0f};
            XMFLOAT4 color = {1.0f, 1.0f, 1.0f, 1.0f};
    };
        
    // position & normal & texture
    struct VertexPositionNormalUV
    {
        VertexPositionNormalUV() {}
        
        VertexPositionNormalUV(float x, float y, float z, float w, 
            float nx, float ny, float nz,
            float u, float v)
            : position(x, y, z, w)
            , normal(nx, ny, nz)
            , texCoord(u, v)
            {
        }
        
        XMFLOAT4 position = {0.0f, 0.0f, 0.0f, 1.0f};
        XMFLOAT3 normal = {0.0f, 0.0f, 0.0f};
        XMFLOAT2 texCoord = {0.0f, 0.0f};
    };

    enum class ETextureType : uint8_t
    {
        None = 0u,
        Diffuse,
        Albedo = Diffuse,
        Specular,
        Normal,
        Roughness,
        Metalness,
        Ambient,
        Max
    };

    struct Texture
    {
        uint32_t Id;
        ETextureType Type;
        ComPtr<ID3D11ShaderResourceView> Srv;
    };

    // Constant buffer types
    struct ConstantBufferPerObject
    {
        XMMATRIX gWorld = XMMatrixIdentity();
        XMMATRIX gInvTransposeWorld = XMMatrixIdentity();
    };

    // Light should be here 7.12.2 Luna
    // But I am using structured buffer in Graphics.cpp for lighting
    // It is even much better approach
    struct ConstantBufferPerFrame
    {
        XMMATRIX gView = XMMatrixIdentity();
        XMMATRIX gProjection = XMMatrixIdentity();
        XMMATRIX gViewProj = XMMatrixIdentity();
        XMVECTOR gEyePos = XMVectorZero();
    };

    // Geometry Shader and Cascade Shadows specific
    // must be divisible by 4
    const UINT kCascadeNumber = 4u;

    struct ConstantBufferCascadeShadows
    {
        ConstantBufferCascadeShadows()
        {
            for (UINT i = 0; i < kCascadeNumber; i++)
            {
                ViewProj[i] = XMMatrixIdentity();
                distances[i] = 0.0f;
            }
        }
        
        XMMATRIX ViewProj[kCascadeNumber];
        float distances[kCascadeNumber];
    };
}