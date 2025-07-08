#include "buffer_gl.h"

#include <core/utilities/logging.h>

using namespace medusa;
using namespace medusa::opengl;


//
bool validBuffer(GLenum bufferType)
{
    std::vector<GLenum> validBuffers = {
        GL_ELEMENT_ARRAY_BUFFER,
        GL_ARRAY_BUFFER,
        GL_UNIFORM_BUFFER,
        GL_SHADER_STORAGE_BUFFER,
    };

    auto it = std::find(validBuffers.begin(), validBuffers.end(), bufferType);
    return it != validBuffers.end();
}


BufferGL::BufferGL(GLenum bufferType)
    : _bufferType(bufferType)
{
    if (! validBuffer(bufferType))
        throw MedusaError("Invalid buffer type");

    glGenBuffers(1, &_handle);
}


//
BufferGL::~BufferGL()
{
    if (_handle)
        glDeleteBuffers(1, &_handle);
    _handle = 0;
}


//
bool BufferGL::bind() const
{
    glBindBuffer(_bufferType, _handle);
    return true;
}


//
bool BufferGL::unbind() const
{
    glBindBuffer(_bufferType, 0);
    return true;
}


// Allocates GPU memory
bool BufferGL::create(const void* ptr, size_t count, size_t stride, BufferUsage usage)
{
    size_t size = count * stride;
    GLenum bufferUsage = opengl::mapEnum(usage);

    // Allocate Buffer Data
    glBindBuffer(_bufferType, _handle);
    glBufferData(_bufferType, size, ptr, bufferUsage);
    glBindBuffer(_bufferType, 0);

    _count = count;
    _stride = stride;
    _bufferUsage = usage;

    return true;
}


// Updates GPU memory
bool BufferGL::update(const void* ptr, size_t offset, size_t count)
{
    if (_count == 0 || _stride == 0)
        throw MedusaError("Uninitialised buffer. Buffer must be created prior to updating");

    size_t size = count * _stride;

    static bool b = false;

    // Update Buffer Data
    glBindBuffer(_bufferType, _handle);
    glBufferSubData(_bufferType, offset, size, ptr);
    glBindBuffer(_bufferType, 0);

    return true;
}
