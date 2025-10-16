#include "descriptor_gl.h"

#include <format>

#include <medusa/graphics/containers.h>

#include <core/utilities/logging.h>

using namespace medusa;
using namespace medusa::opengl;


//
DescriptorGL::DescriptorGL()
{
    glGenVertexArrays(1, &_vao);

}


//
DescriptorGL::~DescriptorGL()
{

}


//
bool DescriptorGL::addDescription(const DType& dtype, uint32_t stride, AttributeLocation attributeLocation)
{
    GLenum dt = mapEnum(dtype);
    GLuint index = (uint32_t)attributeLocation;

    glVertexAttribPointer(index, dtype.elements(), dt, GL_FALSE, stride, (void*)_stride);
    glEnableVertexAttribArray(index);

    _stride += dtype.size();

    return true;
}


bool DescriptorGL::addInstanceAttribute(const DType& dtype, uint32_t stride, InstanceAttributeLocation instanceAttributeLocation)
{
    GLenum dt = mapEnum(dtype);
    GLuint index = (uint32_t)instanceAttributeLocation;

    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, dtype.elements(), dt, GL_FALSE, stride, (void*)_instanceStride); // TODO: instance ptr offset

    glVertexAttribDivisor(index, 1);

    logging::warn(fmt::format("Instance Attribute: location={} size={} elements={} offset={}", (int)instanceAttributeLocation, dtype.size(), dtype.elements(), _instanceStride));

    _instanceStride += dtype.size();

    getCheckError();


    return true;
}


//
bool DescriptorGL::bind()
{
    glBindVertexArray(_vao);

    return true;
}


//
bool DescriptorGL::unbind()
{
    glBindVertexArray(0);

    return true;
}


//
bool DescriptorGL::render(PrimitiveType primitiveType, size_t vertices, size_t indices)
{
    GLenum pt = mapEnum(primitiveType);

    bind();

    if (indices)
        glDrawElements(pt, indices, GL_UNSIGNED_INT, 0);
    else
        glDrawArrays(pt, 0, vertices);

    unbind();

    return true;
}


bool DescriptorGL::render(PrimitiveType primitiveType, size_t vertices, size_t indices, size_t instances)
{
    GLenum pt = mapEnum(primitiveType);

    //logging::info(fmt::format("Desc: Rendering {}", instances));

    bind();

    if (indices)
        //glDrawElements(pt, indices, GL_UNSIGNED_INT, 0);
        glDrawElementsInstanced(pt, indices, GL_UNSIGNED_INT, 0, instances);
    else
        //glDrawArrays(pt, 0, vertices);
        glDrawArraysInstanced(pt, 0, vertices, instances);

    unbind();

    return true;
}


bool DescriptorGL::renderIndirect(PrimitiveType primitiveType, std::shared_ptr<GenericArray<Indirect>> indirect)
{
    GLenum pt = mapEnum(primitiveType);

    bind();

    indirect->bind();

    glMultiDrawElementsIndirect(pt, GL_UNSIGNED_INT, (void*)0, indirect->elements(), indirect->stride());

    indirect->unbind();

    unbind();

    return true;
}
