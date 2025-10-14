#pragma once

#include <medusa/medusa.h>

namespace medusa
{
    enum class TextureFormat
    {
        RGBA8 = 1,
        RGBA16F = 2,
        RGBA32F = 3,

        Depth16 = 4,
        Depth24 = 5,
        Depth32 = 6,
        DepthStencil = 7,
    };

    enum class StorageFormat
    {
        RGBA,
        DepthComponent,
        DepthStencil,
    };

    enum class TextureMagFilter
    {
        Nearest,
        Linear,
    };

    enum class TextureMinFilter
    {
        Nearest,
        Linear,
        NearestMipMapNearest,
        LinearMipMapNearest,
        NearestMapMapLinear,
        LinearMipMapLinear,
    };

    enum class TextureWrapMode
    {
        Repeat,
        MirroredRepeat,
        ClampToEdge,
    };


    /// <summary>
    /// This is only for a 2D Texture/Image
    /// </summary>
    class ITexture
    {
    public:
        ITexture() {}
        virtual ~ITexture() {}

        virtual bool bind(uint32_t index) = 0;
        virtual bool unbind(uint32_t index) = 0;

        virtual bool create(const void* ptr, uint32_t width, uint32_t height, TextureFormat textureFormat) = 0;
        virtual bool update(const void* ptr, uint32_t left = 0, uint32_t top = 0, uint32_t width = 0, uint32_t height = 0) = 0;

        virtual uint32_t width() = 0;
        virtual uint32_t height() = 0;
    };
}
