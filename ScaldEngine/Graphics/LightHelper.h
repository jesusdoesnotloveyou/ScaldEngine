#pragma once

#include "Graphics/DXHelper.h"

namespace Scald
{
    using namespace DirectX;

#ifndef MAX_DIRECT_LIGHTS
    #define MAX_DIRECT_LIGHTS 1   
#endif
#ifndef MAX_POINT_LIGHTS
    #define MAX_POINT_LIGHTS 1024
#endif
#ifndef MAX_SPOT_LIGHTS
    #define MAX_SPOT_LIGHTS 1024
#endif

    struct DirectionalLight
    {
        DirectionalLight() { ZeroMemory(this, sizeof(this)); }

        XMFLOAT4 ambient = {0.0f, 0.0f, 0.0f, 0.0f};           // 16 bytes
        XMFLOAT4 diffuse = {0.0f, 0.0f, 0.0f, 0.0f};           // 16 bytes
        XMFLOAT4 specular = {0.0f, 0.0f, 0.0f, 0.0f};          // 16 bytes
        XMFLOAT3 direction = {0.57735f, -0.57735f, 0.57735f};  // 12 bytes
        float pad = 0.0f;                                      // 4 bytes
    };

    struct PointLight
    {
        PointLight() { ZeroMemory(this, sizeof(this)); }

        XMFLOAT4 diffuse = {0.0f, 0.0f, 0.0f, 0.0f};   // 16 bytes
        XMFLOAT4 specular = {0.0f, 0.0f, 0.0f, 0.0f};  // 16 bytes
        // Packed into 4D vector: (Position, Range)
        XMFLOAT3 position = {0.0f, 0.0f, 0.0f};  // 12 bytes
        float range = 0.0f;                      // 4 bytes
        // Packed into 4D vector: (A0, A1, A2, pad)
        XMFLOAT3 attenuation = {0.0f, 0.0f, 0.0f};  // 12 bytes
        float pad = 0.0f;                           // 4 bytes
                                                    // 64 bytes
    };

    struct SpotLight
    {
        SpotLight() { ZeroMemory(this, sizeof(this)); }

        XMFLOAT4 diffuse = {0.0f, 0.0f, 0.0f, 0.0f};   // 16 bytes
        XMFLOAT4 specular = {0.0f, 0.0f, 0.0f, 0.0f};  // 16 bytes
        // Packed into 4D vector: (position, range)
        XMFLOAT3 position = {0.0f, 0.0f, 0.0f};  // 12 bytes
        float range = 0.0f;                      // 4 bytes
        // Packed into 4D vector: (direction, spot)
        XMFLOAT3 direction = {0.57735f, -0.57735f, 0.57735f};  // 12 bytes
        float spot = 0.0f;                                     // 4 bytes
        // Packed into 4D vector: (attenuation, pad)
        XMFLOAT3 attenuation = {0.0f, 0.0f, 0.0f};  // 12 bytes
        float pad = 0.0f;                           // 4 bytes
                                                    // 80 bytes
    };
}