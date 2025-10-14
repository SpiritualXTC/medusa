#pragma once

#include <medusa/graphics/memory.h>

#include <opengl/gl.h>


namespace medusa
{
    namespace opengl
    {
        class MemoryGL : public IMemory
        {
        public:
            MemoryGL(BufferType bufferType, BufferUsage usage, ResourceIndex index);
            virtual ~MemoryGL();

            const inline void* handle() const override { return (void*)_handle; }
            const inline size_t size() const override { return _size; }

            bool bind() const override;
            bool unbind() const override;

            bool allocate(size_t size, const void* ptr = nullptr) override;
            bool write(size_t offset, size_t size, const void* ptr) override;
            bool read(size_t offset, size_t size, void* ptr) override;

            bool copy(size_t src_offset, size_t dst_offset, size_t size, std::shared_ptr<IMemory> memory) override;

        private:
            GLHandle _handle = 0;
            GLenum _bufferType = GL_INVALID_ENUM;

            size_t _size = 0;
        };

    }
}
