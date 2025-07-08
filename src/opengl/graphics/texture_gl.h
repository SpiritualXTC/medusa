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


            bool bind(uint32_t index = 0) override;
            bool unbind(uint32_t index = 0) override;

            bool create(const void* ptr, uint32_t width, uint32_t height, TextureFormat textureFormat) override;
            bool update(const void* ptr, uint32_t left = 0, uint32_t top = 0, uint32_t width = 0, uint32_t height = 0) override;

            uint32_t width() override { return _height; }
            uint32_t height() override { return _width; }


        private:
            uint32_t _width = 0;
            uint32_t _height = 0;

            GLHandle _handle = 0;
        };
    }
}
