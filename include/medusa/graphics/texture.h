#pragma once

#include <medusa/medusa.h>
#include <medusa/graphics/enum.h>

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


    enum class Swizzle
    {
        Red,
        Green,
        Blue,
        Alpha,
    };


    /// <summary>
    /// This is only for a 2D Texture/Image
    /// </summary>
    class ITexture
    {
    public:
        ITexture() {}
        virtual ~ITexture() {}

        virtual bool create(const void* ptr, uint32_t width, uint32_t height, TextureFormat textureFormat) = 0;
        virtual bool update(const void* ptr, uint32_t left = 0, uint32_t top = 0, uint32_t width = 0, uint32_t height = 0) = 0;

        virtual uint32_t width() = 0;
        virtual uint32_t height() = 0;

        virtual size_t handle() = 0;  // TODO: Should have a unique "handle" type `handle_t`?


        /// <summary>
        ///
        /// </summary>
        /// <param name="baseLevel"></param>
        /// <returns></returns>
        virtual bool setBaseMipMapLevel(int32_t baseLevel = 0) = 0;

        /// <summary>
        ///
        /// </summary>
        /// <param name="mipMapLevel"></param>
        /// <returns></returns>
        virtual bool setMaxMipMapLevel(int32_t mipMapLevel = 1000) = 0;

        /// <summary>
        ///
        /// </summary>
        /// <param name="r"></param>
        /// <param name="g"></param>
        /// <param name="b"></param>
        /// <param name="a"></param>
        /// <returns></returns>
        virtual bool setBorderColour(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 0.0f) = 0;

        /// <summary>
        ///
        /// </summary>
        /// <param name="filter"></param>
        /// <returns></returns>
        virtual bool setMinFilter(TextureMinFilter filter) = 0;

        /// <summary>
        ///
        /// </summary>
        /// <param name="filter"></param>
        /// <returns></returns>
        virtual bool setMagFilter(TextureMagFilter filter) = 0;

        /// <summary>
        ///
        /// </summary>
        /// <param name="bias"></param>
        /// <returns></returns>
        virtual bool setLODBias(float bias = 0.0f) = 0;

        /// <summary>
        ///
        /// </summary>
        /// <param name="minLOD"></param>
        /// <returns></returns>
        virtual bool setMininumLOD(float minLOD = -1000.0) = 0;

        /// <summary>
        ///
        /// </summary>
        /// <param name="maxLOD"></param>
        /// <returns></returns>
        virtual bool setMaximumLOD(float maxLOD = 1000.0) = 0;

        /// <summary>
        ///
        /// </summary>
        /// <param name="swizzle"></param>
        /// <returns></returns>
        virtual bool setSwizzleR(Swizzle swizzle = Swizzle::Red) = 0;

        /// <summary>
        ///
        /// </summary>
        /// <param name="swizzle"></param>
        /// <returns></returns>
        virtual bool setSwizzleG(Swizzle swizzle = Swizzle::Green) = 0;

        /// <summary>
        ///
        /// </summary>
        /// <param name="swizzle"></param>
        /// <returns></returns>
        virtual bool setSwizzleB(Swizzle swizzle = Swizzle::Blue) = 0;

        /// <summary>
        ///
        /// </summary>
        /// <param name="swizzle"></param>
        /// <returns></returns>
        virtual bool setSwizzleA(Swizzle swizzle = Swizzle::Alpha) = 0;

        /// <summary>
        ///
        /// </summary>
        /// <param name="sr"></param>
        /// <param name="sg"></param>
        /// <param name="sb"></param>
        /// <param name="sa"></param>
        /// <returns></returns>
        virtual bool setSwizzleRGBA(Swizzle sr = Swizzle::Red, Swizzle sg = Swizzle::Green, Swizzle sb = Swizzle::Blue, Swizzle sa = Swizzle::Alpha) = 0;

        /// <summary>
        ///
        /// </summary>
        /// <param name="wrapMode"></param>
        /// <returns></returns>
        virtual bool setWrapS(TextureWrapMode wrapMode = TextureWrapMode::Repeat) = 0;

        /// <summary>
        ///
        /// </summary>
        /// <param name="wrapMode"></param>
        /// <returns></returns>
        virtual bool setWrapT(TextureWrapMode wrapMode = TextureWrapMode::Repeat) = 0;

        /// <summary>
        ///
        /// </summary>
        /// <param name="wrapMode"></param>
        /// <returns></returns>
        virtual bool setWrapR(TextureWrapMode wrapMode = TextureWrapMode::Repeat) = 0;

    };

    /*
        // Some of these will only apply to IDepthTexture....
        virtual bool setDepthStencilTextureMode() = 0;
        virtual bool setTextureCompareFunction(Function comparisonFunction) = 0;
        virtual bool setTextureCompareMode() = 0; //* CompareRefToTexture, None
    */
}
