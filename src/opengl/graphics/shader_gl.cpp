#include "shader_gl.h"

#include <fstream>
#include <unordered_map>

#include <medusa/graphics.h>
#include <core/utilities/logging.h>

#include "../gl.h"

using namespace medusa;
using namespace medusa::opengl;


class ShaderSource
{
public:
    ShaderSource(ShaderType shaderType, const std::string& filepath)
    {
        GLenum shader = mapEnum(shaderType);

        if (shader == GL_INVALID_ENUM)
            throw MedusaError("Invalid Shader Type");

        std::ifstream file;

        file.open(filepath);

        std::string content((std::istreambuf_iterator<char>(file)),
            (std::istreambuf_iterator<char>()));

        // Create Handle
        _handle = glCreateShader(shader);

        const GLchar* source = content.c_str();
        GLint length = content.length();
        glShaderSource(_handle, 1, &source, &length);

        glCompileShader(_handle);

        GLint compiled;
        glGetShaderiv(_handle, GL_COMPILE_STATUS, &compiled);
        if (!compiled)
        {
            GLint logLength;
            glGetShaderiv(_handle, GL_INFO_LOG_LENGTH, &logLength);

            GLchar* log = new GLchar[logLength + 1];
            glGetShaderInfoLog(_handle, logLength, NULL, log);

            logging::info(content);
            logging::error(log);

            delete[] log;
        }

    }

    const inline GLHandle handle() const { return _handle; }

private:
    GLHandle _handle = 0;
};


//
ShaderGL::ShaderGL()
{
    _handle = glCreateProgram();
}


//
ShaderGL::~ShaderGL()
{

}


// Attach Shader
bool ShaderGL::attachShader(ShaderType shaderType, const std::string& filepath)
{
    logging::debug("Attaching {}Shader from `{}`", (int)shaderType, filepath);

    auto shader = std::make_shared<ShaderSource>(shaderType, filepath);

    glAttachShader(_handle, shader->handle());

    return true;
}


// Link the Shader
bool ShaderGL::linkShader()
{
    logging::debug("Linking Shader");

    glLinkProgram(_handle);

    GLint linked;
    glGetProgramiv(_handle, GL_LINK_STATUS, &linked);

    if (!linked)
    {
        GLint logLength;
        glGetProgramiv(_handle, GL_INFO_LOG_LENGTH, &logLength);

        GLchar* log = new GLchar[logLength + 1];
        glGetProgramInfoLog(_handle, logLength, NULL, log);

        logging::error(log);

        delete[] log;
    }

    GLint attributes;
    GLint uniforms;

    glGetProgramInterfaceiv(_handle, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &attributes);
    glGetProgramInterfaceiv(_handle, GL_UNIFORM, GL_ACTIVE_RESOURCES, &uniforms);

    logging::info(fmt::format("Linking Shader: attributes={}, uniforms={}", attributes, uniforms));

    // Debugging
    for (size_t i = 0; i < uniforms; ++i)
    {
        char name[256] = { '\0' };
        GLsizei len = 0;

        glGetActiveUniformName(_handle, i, 256, &len, name);

        logging::info(fmt::format("Uniform: {} [{}]", name, len));
    }

    return true;
}


//
size_t ShaderGL::getUniformBlockSize(const std::string& name)
{
    GLuint handle = getUniformHandle(name);
    GLint params = 0;

    auto block_index = glGetUniformBlockIndex(_handle, name.c_str());

    glGetActiveUniformBlockiv(_handle, block_index, GL_UNIFORM_BLOCK_DATA_SIZE, &params);

    logging::info(fmt::format("block index={}, size={}", block_index, params));

    return 0;
}


// Create a Uniform Block
bool ShaderGL::bindUniformBlock(const std::string& name, std::shared_ptr<IUniformBuffer> uniformBuffer)
{
    auto blockIndex = glGetUniformBlockIndex(_handle, name.c_str());

    GLHandle handle = (GLuint)uniformBuffer->handle();

    logging::info(fmt::format("Bind Uniform: block={}, index={}, handle={}, size={}", blockIndex, uniformBuffer->bindingIndex(), handle, uniformBuffer->size()));

    glUniformBlockBinding(_handle, blockIndex, uniformBuffer->bindingIndex());
//  glBindBufferRange(GL_UNIFORM_BUFFER, bindingIndex, handle, 0, uniformBuffer->size());   // Does this make sense here? Shouldn't this be part of the uniformbuffer declaration/class: storage buffers, has this in it's own class uniformBuffer->bindingIndex()?

    return true;
}


//
bool ShaderGL::bindBufferBlock(const std::string& name, std::shared_ptr<IStorageBuffer> storageBuffer)
{
    auto blockIndex = glGetProgramResourceIndex(_handle, GL_SHADER_STORAGE_BLOCK, name.c_str());

    GLHandle handle = (GLHandle)storageBuffer->handle();

    logging::info(fmt::format("Bind Buffer: block={}, index={}, handle={}", blockIndex, storageBuffer->bindingIndex(), handle));

    glShaderStorageBlockBinding(_handle, blockIndex, storageBuffer->bindingIndex());

    return false;
}


// Bind this shader as the active shader
bool ShaderGL::bind()
{
    glUseProgram(_handle);
    return true;
}


// Unbind this shader
bool ShaderGL::unbind()
{
    glUseProgram(0);
    return true;
}
