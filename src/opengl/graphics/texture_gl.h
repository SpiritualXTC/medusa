#pragma once

#include <medusa/graphics/texture.h>

#include "../gl.h"

namespace medusa
{
    namespace opengl
    {
        class TextureGL : public ITexture
        {
        public:
            TextureGL();
            virtual ~TextureGL();

            bool create(const void* ptr, uint32_t width, uint32_t height, TextureFormat textureFormat) override;
            bool update(const void* ptr, uint32_t left = 0, uint32_t top = 0, uint32_t width = 0, uint32_t height = 0) override;

            uint32_t width() override { return _height; }
            uint32_t height() override { return _width; }


            size_t handle() override { return _handle; }


            bool setBaseMipMapLevel(int32_t baseLevel = 0) override;
            bool setMaxMipMapLevel(int32_t mipMapLevel = 1000) override;

            bool setBorderColour(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 0.0f) override;

            bool setMinFilter(TextureMinFilter filter) override;
            bool setMagFilter(TextureMagFilter filter) override;

            bool setLODBias(float bias = 0.0f) override;
            bool setMininumLOD(float minLOD = -1000.0) override;
            bool setMaximumLOD(float maxLOD = 1000.0) override;

            bool setSwizzleR(Swizzle swizzle = Swizzle::Red);
            bool setSwizzleG(Swizzle swizzle = Swizzle::Green);
            bool setSwizzleB(Swizzle swizzle = Swizzle::Blue);
            bool setSwizzleA(Swizzle swizzle = Swizzle::Alpha);
            bool setSwizzleRGBA(Swizzle sr = Swizzle::Red, Swizzle sg = Swizzle::Green, Swizzle sb = Swizzle::Blue, Swizzle sa = Swizzle::Alpha);

            bool setWrapS(TextureWrapMode wrapMode = TextureWrapMode::Repeat) override;
            bool setWrapT(TextureWrapMode wrapMode = TextureWrapMode::Repeat) override;
            bool setWrapR(TextureWrapMode wrapMode = TextureWrapMode::Repeat) override;

        private:
            uint32_t _width = 0;
            uint32_t _height = 0;

            GLHandle _texture = 0;
            GLHandle _handle = 0;
        };
    }
}
