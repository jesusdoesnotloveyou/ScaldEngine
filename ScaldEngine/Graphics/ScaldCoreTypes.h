#pragma once

#include "Graphics/DXHelper.h"
#include "ScaldCoreDefines.h"

namespace Scald
{
    using Microsoft::WRL::ComPtr;
    // SCALD MATH
    namespace ScaldMath
    {
        static const DirectX::XMVECTOR ForwardVector = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
        static const DirectX::XMVECTOR RightVector   = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
        static const DirectX::XMVECTOR UpVector      = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
        static const DirectX::XMVECTOR ZeroVector    = DirectX::XMVectorZero();
    }  // namespace ScaldMath

    // Constant buffer types
    #include "Data/Shaders/CommonTypes.hlsli"

    // Geometry Shader and Cascade Shadows specific
    // must be divisible by 4
    const UINT kCascadeNumber = 4u;

    struct CascadeShadowsConstants
    {
        CascadeShadowsConstants()
        {
            for (UINT i = 0; i < kCascadeNumber; i++)
            {
                ViewProj[i] = DirectX::XMMatrixIdentity();
                Distance[i] = 0.0f;
            }
        }
        
        DirectX::XMMATRIX ViewProj[kCascadeNumber];
        float Distance[kCascadeNumber];
    };
}