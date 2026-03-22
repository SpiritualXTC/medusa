#include "shader_loader.h"

#include <medusa/engine/context.h>

using namespace medusa;
using namespace medusa::loaders;


//
std::shared_ptr<IShader> ShaderLoader::loadShader(std::shared_ptr<IContext> context, const std::unordered_map<ShaderType, std::string>& shaderFilenames)
{
    auto shader = context->createShader();

    for (auto [type, filename] : shaderFilenames)
    {
        shader->attachShader(type, filename);
    }

    shader->linkShader();

    return shader;
}
