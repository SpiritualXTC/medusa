#pragma once

#include <medusa/medusa.h>
#include <medusa/graphics_fwd.h>

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
        Texture0 = 0x02,
        Texture1 = 0x03,
        Colour = 0x04,
    };

    enum class InstanceAttributeLocation
    {
        // Transforms
        World = 0x04,   // 0xF0

        // Lighting :: These are the old-style lighting, probably revisit this
        MaterialAmbient = 0x08,
        MaterialDiffuse = 0x15,
        MaterialSpecular = 0x16,
        MaterialEmissive = 0x17,
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
        virtual bool linkShader() = 0;

        virtual bool bind() = 0;
        virtual bool unbind() = 0;


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

    private:

    };



}
