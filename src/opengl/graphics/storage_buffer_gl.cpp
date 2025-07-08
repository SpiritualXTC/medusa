#include "storage_buffer_gl.h"

using namespace medusa;
using namespace medusa::opengl;


//
UniformBufferGL::UniformBufferGL(uint32_t bindingIndex, size_t size)
    : GenericBufferGL(BufferType::Uniform)
    , _bindingIndex(bindingIndex)
{
    create(nullptr, size, BufferUsage::StaticDraw);

    glBindBufferRange(GL_UNIFORM_BUFFER, bindingIndex, (GLHandle)handle(), 0, size);
}


//
StorageBufferGL::StorageBufferGL(uint32_t bindingIndex, size_t size)
    : GenericBufferGL(BufferType::ShaderStorage)
    , _bindingIndex(bindingIndex)
{
    create(nullptr, size, BufferUsage::DynamicCopy);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingIndex, (GLHandle)handle());
}
