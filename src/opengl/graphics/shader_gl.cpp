#include "shader_gl.h"

#include <fstream>
#include <unordered_map>

#include <medusa/graphics.h>
#include <core/utilities/logging.h>

#include <opengl/gl.h>
#include <opengl/graphics/pipeline_state_gl.h>

using namespace medusa;
using namespace medusa::opengl;


class ShaderSource
{
protected:
    ShaderSource() {}

public:
    //ShaderSource(ShaderType shaderType, const std::string& filepath)
    ShaderSource(ShaderType shaderType, const std::string& content)
    {
        load(shaderType, content);
    }

    bool load(ShaderType shaderType, const std::string& content)
    {
        GLenum shader = mapEnum(shaderType);

        if (shader == GL_INVALID_ENUM)
            throw MedusaError("Invalid Shader Type");

        // Create Handle
        _handle = glCreateShader(shader);

        const GLchar* source = content.c_str();
        GLint length = (GLint)content.length();
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


class ShaderFile : public ShaderSource
{
public:
    ShaderFile(ShaderType shaderType, const std::string& filepath)
    {
        std::ifstream file;

        file.open(filepath);

        std::string content((std::istreambuf_iterator<char>(file)),
            (std::istreambuf_iterator<char>()));

        load(shaderType, content);
    }
};


//
ShaderGL::ShaderGL(std::shared_ptr<PipelineStateGL> pipelineStateGL)
    : _pipelineStateGL(pipelineStateGL)
{
    _handle = glCreateProgram();
}


//
ShaderGL::~ShaderGL()
{

}


// Attach Shader from File
bool ShaderGL::attachShader(ShaderType shaderType, const std::string& filepath)
{
    logging::debug("Attaching {} Shader from `{}`", (int)shaderType, filepath);

    auto shader = std::make_shared<ShaderFile>(shaderType, filepath);

    glAttachShader(_handle, shader->handle());

    return true;
}


// Attach Shader from Source
bool ShaderGL::attachShaderFromSource(ShaderType shaderType, const std::string& source)
{
    logging::debug("Attaching {} Shader from source", (int)shaderType);

    auto shader = std::make_shared<ShaderSource>(shaderType, source);

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
    for (GLuint i = 0; i < uniforms; ++i)
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


//
bool ShaderGL::bindBuffer(const std::string& name, std::shared_ptr<MemoryView> view)
{
    GLuint blockIndex = GL_INVALID_INDEX;
    GLHandle handle = (GLHandle)view->handle();

    if (view->type() == BufferType::Uniform)
    {
        blockIndex = glGetUniformBlockIndex(_handle, name.c_str());
        if (blockIndex != GL_INVALID_INDEX)
            glUniformBlockBinding(_handle, blockIndex, view->index());
    }
    else if (view->type() == BufferType::ShaderStorage)
    {
        blockIndex = glGetProgramResourceIndex(_handle, GL_SHADER_STORAGE_BLOCK, name.c_str());
        if (blockIndex != GL_INVALID_INDEX)
            glShaderStorageBlockBinding(_handle, blockIndex, view->index());
    }
    else
        throw MedusaError("Invalid buffer type to bind");

    if (blockIndex != GL_INVALID_INDEX)
        logging::info(fmt::format("Resource: Bind Buffer: block={}, index={}", blockIndex, view->index(), handle));
    else
        logging::error(fmt::format("Resource: Failed to bind buffer with bindindIndex={}", view->index()));

    return blockIndex == GL_INVALID_INDEX ? false : true;
}


// Bind this shader as the active shader
bool ShaderGL::bind()
{
    glUseProgram(_handle);
    _pipelineStateGL->push();
    _pipelineStateGL->apply(_state);
    return true;
}


// Unbind this shader
bool ShaderGL::unbind()
{
    _pipelineStateGL->pop();
    glUseProgram(0);
    return true;
}
