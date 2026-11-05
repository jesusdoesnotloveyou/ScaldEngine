#pragma once

#include <DirectXMath.h>

namespace Scald
{
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
        VertexPositionColor(float x, float y, float z, float w, const DirectX::XMVECTORF32 v)
            : position(x, y, z, w)
            , color(v.f[0], v.f[1], v.f[2], v.f[3])
        {
        }

        DirectX::XMFLOAT4 position = {0.0f, 0.0f, 0.0f, 1.0f};
        DirectX::XMFLOAT4 color = {1.0f, 1.0f, 1.0f, 1.0f};
    };

    // position & normal & texture
    struct VertexPositionNormalUV
    {
        VertexPositionNormalUV() {}

        VertexPositionNormalUV(float x, float y, float z, float w, float nx, float ny, float nz, float u, float v)
            : position(x, y, z, w)
            , normal(nx, ny, nz)
            , texCoord(u, v)
        {
        }

        DirectX::XMFLOAT4 position = {0.0f, 0.0f, 0.0f, 1.0f};
        DirectX::XMFLOAT3 normal = {0.0f, 0.0f, 0.0f};
        DirectX::XMFLOAT2 texCoord = {0.0f, 0.0f};
    };
}