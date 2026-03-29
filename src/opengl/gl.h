#pragma once

#include <Windows.h>

#include "gl/glew.h"

#include <gl/gl.h>
#include <gl/glext.h>

#include <medusa/data_type.h>
#include <medusa/graphics/buffer.h>
#include <medusa/graphics/shader.h>
#include <medusa/graphics/texture.h>


namespace medusa
{
    namespace opengl
    {
        typedef GLuint GLHandle;

        GLenum getCheckError(const std::string& message = "Error");


        // Data Types
        GLenum mapEnum(const DType& dtype);

        // Resource Types
        GLenum mapEnum(ShaderType shaderType);
        GLenum mapEnum(BufferUsage bufferUsage);
        GLenum mapEnum(BufferType bufferType);

        // Formats
        GLenum mapEnum(TextureFormat textureFormat);
        GLenum mapEnum(StorageFormat storageFormat);

        // Render states
        GLenum mapEnum(PrimitiveType primitiveType);

        GLenum mapEnum(FaceDirection faceDirection);
        GLenum mapEnum(CullMode cullMode);
        GLenum mapEnum(PolygonMode polygonMode);


        GLenum mapEnum(Function function);

        // Texturing
        GLint mapEnum(TextureMagFilter magFilter);
        GLint mapEnum(TextureMinFilter minFilter);
        GLenum mapEnum(TextureWrapMode wrapMode);
        GLenum mapEnum(Swizzle swizzle);

        // Pipeline state enums
        GLenum mapEnum(Capability cap);

        GLenum mapEnum(BlendEquation equation);
        GLenum mapEnum(BlendFunction function);
        GLenum mapEnum(StencilOperation operation);
        GLenum mapEnum(LogicOperation op);
        GLenum mapEnum(FaceSelector face);
    }
}
