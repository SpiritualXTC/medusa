#pragma once

#include <medusa/medusa.h>
#include <opengl/gl.h>

namespace medusa
{
    namespace opengl
    {
        /// <summary>
        /// The actual implementation
        /// </summary>
        class BufferGL
        {
        public:
            BufferGL(GLenum bufferType);

            virtual ~BufferGL();

            /*

            */
            const inline GLHandle handle() const { return _handle; }
            const inline size_t count() const { return _count; }
            const inline size_t stride() const { return _stride; }
            const inline size_t bytes() const { return _count * _stride; }
            const inline BufferUsage usage() const { return _bufferUsage; }


            bool bind() const;
            bool unbind() const;

            bool create(const void* ptr, size_t count, size_t stride, BufferUsage usage = BufferUsage::StaticDraw);
            bool update(const void* ptr, size_t offset, size_t count);




        private:

            GLHandle _handle = 0;
            GLenum _bufferType = GL_INVALID_ENUM;

            BufferUsage _bufferUsage = (BufferUsage)0;

            size_t _count = 0;
            size_t _stride = 0;
        };


        /// <summary>
        /// Templated buffer
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <typeparam name="PTR"></typeparam>
        /// <typeparam name=""></typeparam>
        template <typename T, typename PTR = const void*, typename = std::enable_if_t<std::is_base_of_v<IBuffer, T>>>
        class GenericBufferGL : public T
        {
        public:
            GenericBufferGL(BufferType bufferType)
                : _buffer(std::make_unique<BufferGL>(mapEnum(bufferType)))
            {

            }

            inline void* handle() override { return (void*)_buffer->handle(); }
            inline size_t size() override { return _buffer->bytes(); }
            inline BufferUsage usage() override { return _buffer->usage(); }


            inline bool bind() const override { return _buffer->bind(); }
            inline bool unbind() const override { return _buffer->unbind(); }


            bool create(PTR ptr, size_t nBytes, BufferUsage usage)
            {
                return _buffer->create(ptr, 1, nBytes, usage);
            }

            bool update(PTR ptr, size_t nOffset, size_t nBytes) override
            {
                return _buffer->update(ptr, nOffset, nBytes);
            }



        private:
            std::unique_ptr<BufferGL> _buffer = nullptr;
        };
    }
}
