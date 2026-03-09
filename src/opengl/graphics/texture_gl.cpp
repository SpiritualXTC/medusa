#include "texture_gl.h"



#include <core/utilities/logging.h>

using namespace medusa;
using namespace medusa::opengl;


//
TextureGL::TextureGL()
{
    _texture = 0;
    _handle = 0;
}


//
TextureGL::~TextureGL()
{
    if (_texture)
    {
        glMakeTextureHandleNonResidentARB(_texture);
        glDeleteTextures(1, &_texture);
    }
    _texture = 0;
}


//
bool TextureGL::create(const void* ptr, uint32_t width, uint32_t height, TextureFormat textureFormat)
{
    GLenum internalFormat = mapEnum(textureFormat);
    GLenum format = GL_RGBA;
    GLenum dataType = GL_UNSIGNED_BYTE;


    GLenum err = 0;


    _width = width;
    _height = height;

    // Create 2D Texture
    glCreateTextures(GL_TEXTURE_2D, 1, &_texture);
    glTextureStorage2D(_texture, 1, internalFormat, width, height);

    // Associate with initial data
    if (ptr)
    {
        glTextureSubImage2D(_texture, 0, 0, 0, width, height, format, dataType, ptr);
        glGenerateTextureMipmap(_texture);
    }

    // Get the Bindless handle
    _handle = glGetTextureHandleARB(_texture);
    glMakeTextureHandleResidentARB(_handle);


    logging::debug(fmt::format("Allocate Texture: {}x{}", _width, _height));

    setMagFilter(TextureMagFilter::Linear);
    setMinFilter(TextureMinFilter::Linear);

    setWrapS(TextureWrapMode::ClampToEdge);
    setWrapR(TextureWrapMode::ClampToEdge);

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

    glTextureSubImage2D(_texture, 0, left, top, width, height, format, data_type, ptr);

    return true;
}





//
bool TextureGL::setBaseMipMapLevel(int32_t baseLevel)
{
    glTextureParameteri(_texture, GL_TEXTURE_BASE_LEVEL, baseLevel);
    return glGetError() == GLEW_OK;
}


//
bool TextureGL::setMaxMipMapLevel(int32_t baseLevel)
{
    glTextureParameteri(_texture, GL_TEXTURE_MAX_LEVEL, baseLevel);
    return glGetError() == GLEW_OK;
}


//
bool TextureGL::setBorderColour(float r, float g, float b, float a)
{
    GLfloat rgba[4] = { r, g, b, a };
    glTextureParameterfv(_texture, GL_TEXTURE_BORDER_COLOR, &rgba[0]);

    return glGetError() == GLEW_OK;
}


//
bool TextureGL::setLODBias(float bias)
{
    glTextureParameterf(_texture, GL_TEXTURE_LOD_BIAS, bias);
    return glGetError() == GLEW_OK;
}


//
bool TextureGL::setMinFilter(TextureMinFilter filter)
{
    GLenum minFilter = mapEnum(filter);
    glTextureParameteri(_texture, GL_TEXTURE_MIN_FILTER, minFilter);
    return glGetError() == GLEW_OK;
}


//
bool TextureGL::setMagFilter(TextureMagFilter filter)
{
    GLenum maxFilter = mapEnum(filter);
    glTextureParameteri(_texture, GL_TEXTURE_MAG_FILTER, maxFilter);
    return glGetError() == GLEW_OK;
}


//
bool TextureGL::setMininumLOD(float minLOD)
{
    glTextureParameterf(_texture, GL_TEXTURE_MIN_LOD, minLOD);
    return glGetError() == GLEW_OK;
}


//
bool TextureGL::setMaximumLOD(float maxLOD)
{
    glTextureParameterf(_texture, GL_TEXTURE_MAX_LOD, maxLOD);
    return true;
}


//
bool TextureGL::setSwizzleR(Swizzle swizzle)
{
    GLenum swizzleR = mapEnum(swizzle);
    glTextureParameteri(_texture, GL_TEXTURE_SWIZZLE_R, swizzleR);
    return glGetError() == GLEW_OK;
}


//
bool TextureGL::setSwizzleG(Swizzle swizzle)
{
    GLenum swizzleG = mapEnum(swizzle);
    glTextureParameteri(_texture, GL_TEXTURE_SWIZZLE_G, swizzleG);
    return glGetError() == GLEW_OK;
}


//
bool TextureGL::setSwizzleB(Swizzle swizzle)
{
    GLenum swizzleB = mapEnum(swizzle);
    glTextureParameteri(_texture, GL_TEXTURE_SWIZZLE_B, swizzleB);
    return glGetError() == GLEW_OK;
}


//
bool TextureGL::setSwizzleA(Swizzle swizzle)
{
    GLenum swizzleA = mapEnum(swizzle);
    glTextureParameteri(_texture, GL_TEXTURE_SWIZZLE_A, swizzleA);
    return glGetError() == GLEW_OK;
}


//
bool TextureGL::setSwizzleRGBA(Swizzle sr, Swizzle sg, Swizzle sb, Swizzle sa)
{
    GLenum swizzleRGBA[4] = { mapEnum(sr), mapEnum(sg), mapEnum(sb), mapEnum(sa) };

    glTextureParameterIuiv(_texture, GL_TEXTURE_SWIZZLE_RGBA, &swizzleRGBA[0]);
    return glGetError() == GLEW_OK;
}


//
bool TextureGL::setWrapS(TextureWrapMode wrapMode)
{
    GLenum maxFilter = mapEnum(wrapMode);
    glTextureParameteri(_texture, GL_TEXTURE_WRAP_S, maxFilter);
    return glGetError() == GLEW_OK;
}


//
bool TextureGL::setWrapT(TextureWrapMode wrapMode)
{
    GLenum maxFilter = mapEnum(wrapMode);
    glTextureParameteri(_texture, GL_TEXTURE_WRAP_T, maxFilter);
    return glGetError() == GLEW_OK;
}


//
bool TextureGL::setWrapR(TextureWrapMode wrapMode)
{
    GLenum maxFilter = mapEnum(wrapMode);
    glTextureParameteri(_texture, GL_TEXTURE_WRAP_R, maxFilter);
    return glGetError() == GLEW_OK;
}
