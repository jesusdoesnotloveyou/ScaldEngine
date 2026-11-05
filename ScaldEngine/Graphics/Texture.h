#pragma once

#include <wrl.h>
#include <filesystem>

namespace Scald
{
    using Microsoft::WRL::ComPtr;

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
        ComPtr<ID3D11ShaderResourceView> Srv;
        std::filesystem::path Path;
        uint32_t Id;
        ETextureType Type;
    };
}