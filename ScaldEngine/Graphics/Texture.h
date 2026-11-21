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
    public:
        Texture()
            : Srv(nullptr)
            , Id(-1u)
        {}

        Texture(const Texture& texture) = default;
        Texture(Texture&& texture) noexcept = default;
        Texture& operator=(const Texture& texture) = default;
        Texture& operator=(Texture&& texture) noexcept = default;

    public:
        ComPtr<ID3D11ShaderResourceView> Srv;
        std::filesystem::path Path;
        uint32_t Id;
        //ETextureType Type;
    };
}