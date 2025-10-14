#include "memory_gl.h"

using namespace medusa;
using namespace medusa::opengl;


//
MemoryGL::MemoryGL(BufferType bufferType, BufferUsage usage, ResourceIndex index)
    : IMemory(bufferType, usage, index)
    , _bufferType(mapEnum(bufferType))
{
    glGenBuffers(1, &_handle);

    if (bufferType == BufferType::Uniform || bufferType == BufferType::ShaderStorage)
        glBindBufferBase(_bufferType, index, (GLHandle)handle());
}


//
MemoryGL::~MemoryGL()
{
    if (_handle)
        glDeleteBuffers(1, &_handle);
    _handle = 0;
}


//
bool MemoryGL::bind() const
{
    glBindBuffer(_bufferType, _handle);
    return true;
}


//
bool MemoryGL::unbind() const
{
    glBindBuffer(_bufferType, 0);
    return true;
}


//
bool MemoryGL::allocate(size_t size, const void* ptr)
{
    GLenum bufferUsage = mapEnum(usage());

    // Allocate Buffer Data
    glBindBuffer(_bufferType, _handle);
    glBufferData(_bufferType, size, ptr, bufferUsage);
    glBindBuffer(_bufferType, 0);

    _size = size;

    return true;
}


//
bool MemoryGL::write(size_t offset, size_t size, const void* ptr)
{
    if (_size == 0) return false;
    if (offset + size > _size) return false;
    if (ptr == nullptr) return false;

    // Update Buffer Data
    glBindBuffer(_bufferType, _handle);
    glBufferSubData(_bufferType, offset, size, ptr);
    glBindBuffer(_bufferType, 0);

    return true;
}


//
bool MemoryGL::read(size_t offset, size_t size, void* ptr)
{
    if (_size == 0) return false;
    if (offset + size > _size) return false;
    if (ptr == nullptr) return false;

    glBindBuffer(_bufferType, _handle);
    glGetBufferSubData(_bufferType, offset, size, ptr);
    glBindBuffer(_bufferType, 0);

    return false;
}


//
bool MemoryGL::copy(size_t src_offset, size_t dst_offset, size_t size, std::shared_ptr<IMemory> memory)
{
    return false;
}
