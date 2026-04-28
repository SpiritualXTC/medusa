#pragma once

#include <medusa/medusa.h>
#include <medusa/engine_fwd.h>
#include <medusa/containers_fwd.h>
#include <medusa/graphics_fwd.h>
#include <medusa/renderer/pipeline_state.h>

namespace medusa
{
    // Forward Declarations
    class IUniformBuffer;
    class IStorageBuffer;


    enum class ShaderType
    {
        VertexShader = 1,
        FragmentShader = 2,
        GeometryShader = 3,
    };


    enum class AttributeLocation
    {
        Position = 0x00,
        Normal = 0x01,
        TextureDiffuse = 0x02,
        Colour = 0x04,

        MaterialIndex = 0x08,
    };

    enum class InstanceAttributeLocation
    {
        // Transforms
        TransformIndex = 0x09,
    };


    /// <summary>
    ///
    /// </summary>
    class IShader
    {
    public:
        IShader() {}
        virtual ~IShader() {}

        virtual bool attachShader(ShaderType shaderType, const std::string& filepath) = 0;
        virtual bool attachShaderFromSource(ShaderType shaderType, const std::string& source) = 0;
        virtual bool linkShader() = 0;

        virtual bool bind() = 0;
        virtual bool unbind() = 0;

        inline PipelineState& state() { return _state; }    // TODO: This should crash once the shader is linked.
        const inline PipelineState& state() const { return _state; }

        inline PipelineState& state(const PipelineState& state)
        {
            _state = state;
            return _state;
        }


        /// <summary>
        /// Bind UniformBuffer or ShaderStorage
        /// </summary>
        /// <param name="name"></param>
        /// <param name="view"></param>
        /// <returns></returns>
        virtual bool bindBuffer(const std::string& name, std::shared_ptr<MemoryView> view) = 0;


        virtual size_t getUniformBlockSize(const std::string& name) = 0;


        // TODO: Should be handled by UBO's
        // Matrices
        virtual inline void setUniform(const std::string& name, const glm::mat3& m) const = 0;
        virtual inline void setUniform(const std::string& name, const glm::mat4& m) const = 0;

        // Float Vec
        virtual inline void setUniform(const std::string& name, const glm::fvec2& v) const = 0;
        virtual inline void setUniform(const std::string& name, const glm::fvec3& v) const = 0;
        virtual inline void setUniform(const std::string& name, const glm::fvec4& v) const = 0;

        // Int Vec
        virtual inline void setUniform(const std::string& name, const glm::ivec2& v) const = 0;
        virtual inline void setUniform(const std::string& name, const glm::ivec3& v) const = 0;
        virtual inline void setUniform(const std::string& name, const glm::ivec4& v) const = 0;

        // Primitives
        virtual inline void setUniform(const std::string& name, const float f) const = 0;
        virtual inline void setUniform(const std::string& name, const int32_t i) const = 0;
        virtual inline void setUniform(const std::string& name, const bool b) const = 0;

    protected:
        PipelineState _state;
    };
}  // namespace medusa
