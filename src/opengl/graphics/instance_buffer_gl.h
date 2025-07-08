#pragma once

#include <medusa/medusa.h>
#include <medusa/graphics/vertex_buffer.h>
#include <opengl/gl.h>

#include "buffer_gl.h"

namespace medusa
{
    namespace opengl
    {
        class InstanceBufferGL : virtual public BufferGL, public IInstanceBuffer
        {
        public:
            InstanceBufferGL() : BufferGL(GL_ARRAY_BUFFER)
            {

            }
            virtual ~InstanceBufferGL()
            {

            }

            bool bind() override { return BufferGL::bind(); }
            bool unbind() override { return BufferGL::unbind(); }

            inline size_t instances() override { return BufferGL::count(); }


            bool create(const void* ptr, size_t count, size_t stride) override
            {
                return BufferGL::create((void*)ptr, count, stride);
            }
            bool update(const void* ptr, size_t offset, size_t count) override
            {
                return BufferGL::update((void*)ptr, offset, count);
            }

        };
    }
}
