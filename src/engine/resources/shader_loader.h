#pragma once

#include <medusa/medusa.h>
#include <medusa/graphics/shader.h>

namespace medusa
{
    namespace loaders
    {
        class ShaderLoader
        {
        public:
            ShaderLoader() = delete;

            static std::shared_ptr<IShader> loadShader(std::shared_ptr<IContext> context, const std::unordered_map<ShaderType, std::string>& shaderFilenames);
        };
    }
}
