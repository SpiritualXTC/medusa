#pragma once

#include <medusa/medusa.h>
#include <medusa/graphics/index_buffer.h>
#include <opengl/gl.h>

#include "buffer_gl.h"

namespace medusa
{
    namespace opengl
    {
        class IndexBufferGL : virtual public BufferGL, public IIndexBuffer
        {
        public:
            IndexBufferGL() : BufferGL(GL_ELEMENT_ARRAY_BUFFER)
            {

            }
            virtual ~IndexBufferGL()
            {

            }

            bool bind() override { return BufferGL::bind(); }
            bool unbind() override { return BufferGL::unbind(); }

            inline size_t indices() override { return BufferGL::count(); }

            bool create(const uint32_t* ptr, size_t count) override
            {
                return BufferGL::create((void*)ptr, count, sizeof(uint32_t));
            }
            bool update(const uint32_t* ptr, size_t offset, size_t count) override
            {
                return BufferGL::update((void*)ptr, offset, count);
            }

        private:
        };
    }
}
