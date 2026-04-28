#pragma once

#include <medusa/medusa.h>
#include <medusa/math.h>

#include <medusa/graphics/shader.h>

#include "../gl.h"

namespace medusa
{
    namespace opengl
    {
        class PipelineStateGL;

        class ShaderGL : public IShader
        {
        public:
            ShaderGL(std::shared_ptr<PipelineStateGL> pipelineStateGL);
            virtual ~ShaderGL();

            bool attachShader(ShaderType shaderType, const std::string& filepath) override;
            bool attachShaderFromSource(ShaderType shaderType, const std::string& source) override;
            bool linkShader() override;

            const inline GLHandle handle() const { return _handle; }


            bool bind() override;
            bool unbind() override;



            size_t getUniformBlockSize(const std::string& name) override;


            bool bindBuffer(const std::string& name, std::shared_ptr<MemoryView> view) override;


            /* Get Handle of a Uniform  */
            const GLint getUniformHandle(const std::string & name) const
            {
                return glGetUniformLocation(handle(), name.c_str());
            }

            const GLint getBufferHandle(const std::string& name) const
            {
                return glGetProgramResourceIndex(handle(), GL_SHADER_STORAGE_BLOCK, name.c_str());
            }



            inline void setUniform(const std::string& name, const glm::mat4& m) const override
            {
                glUniformMatrix4fv(getUniformHandle(name), 1, false, glm::value_ptr(m));
            }

            inline void setUniform(const std::string& name, const glm::mat3& m) const override
            {
                glUniformMatrix3fv(getUniformHandle(name), 1, false, glm::value_ptr(m));
            }


            inline void setUniform(const std::string& name, const glm::fvec2& v) const override
            {
                glUniform2fv(getUniformHandle(name), 1, glm::value_ptr(v));
            }
            inline void setUniform(const std::string& name, const glm::fvec3& v) const override
            {
                glUniform3fv(getUniformHandle(name), 1, glm::value_ptr(v));
            }
            inline void setUniform(const std::string& name, const glm::fvec4& v) const override
            {
                glUniform4fv(getUniformHandle(name), 1, glm::value_ptr(v));
            }

            inline void setUniform(const std::string& name, const glm::ivec2& v) const override
            {
                glUniform2iv(getUniformHandle(name), 1, glm::value_ptr(v));
            }
            inline void setUniform(const std::string& name, const glm::ivec3& v) const override
            {
                glUniform3iv(getUniformHandle(name), 1, glm::value_ptr(v));
            }
            inline void setUniform(const std::string& name, const glm::ivec4& v) const override
            {
                glUniform4iv(getUniformHandle(name), 1, glm::value_ptr(v));
            }

#if 0
            inline void setUniformTexture(const std::string& name, UInt32 bindIndex) const override
            {
                glUniform1i(getUniformHandle(name), bindIndex);
            }
#endif
            inline void setUniform(const std::string& name, const float f) const override
            {
                glUniform1f(getUniformHandle(name), f);
            }
            inline void setUniform(const std::string& name, const int32_t i) const override
            {
                glUniform1i(getUniformHandle(name), i);
            }
            inline void setUniform(const std::string& name, const bool b) const override
            {
                glUniform1i(getUniformHandle(name), b);
            }

            //inline void setUniform(const std::string& name, std::shared_ptr<ITexture> texture) const override
            //{
            //  glUniform1i(getUniformHandle(name), 0);
            //}

        private:
            GLHandle _handle = 0;

            std::shared_ptr<PipelineStateGL> _pipelineStateGL;
        };
    }
}
