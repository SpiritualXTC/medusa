#include "texture_gl.h"

#include <core/utilities/logging.h>

using namespace medusa;
using namespace medusa::opengl;


//
TextureGL::TextureGL()
{
    glGenTextures(1, &_handle);

}


//
TextureGL::~TextureGL()
{
    if (_handle)
        glDeleteTextures(1, &_handle);
    _handle = 0;
}


//
bool TextureGL::create(const void* ptr, uint32_t width, uint32_t height, TextureFormat textureFormat)
{
    _width = width;
    _height = height;

    GLenum internalFormat = mapEnum(textureFormat);
    GLenum format = GL_RGBA;
    GLenum dataType = GL_UNSIGNED_BYTE;

    logging::debug(fmt::format("Allocate Texture: {}x{}", _width, _height));

    bind();
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, _width, _height, 0, format, dataType, ptr);

    // TODO: Should be able to configure these
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    unbind();

    return true;
}


//
bool TextureGL::update(const void* ptr, uint32_t left, uint32_t top, uint32_t width, uint32_t height)
{
    if (width == 0)
        width = _width;
    if (height == 0)
        height = _height;

    if (left + width > _width || _width == 0)
        throw MedusaError("Invalid Width");
    if (top + height > _height || _height == 0)
        throw MedusaError("Invalid Height");

    logging::debug(fmt::format("Copy Texture: {}x{}", _width, _height));

    GLenum format = GL_RGBA;
    GLenum data_type = GL_UNSIGNED_BYTE;

    bind();
    glTexSubImage2D(GL_TEXTURE_2D, 0, left, top, width, height, format, data_type, ptr);
    unbind();

    return true;
}


//
bool TextureGL::bind(uint32_t index)
{
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, _handle);

    return true;
}


//
bool TextureGL::unbind(uint32_t index)
{
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}
