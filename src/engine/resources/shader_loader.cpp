#include "shader_loader.h"

#include <medusa/engine/context.h>

using namespace medusa;
using namespace medusa::loaders;


//
std::shared_ptr<IShader> ShaderLoader::loadShader(std::shared_ptr<IContext> context, const std::unordered_map<ShaderType, std::string>& shaderFilenames, const PipelineState& pipelineState)
{
    auto shader = context->createShader();

    shader->state(pipelineState);

    for (auto [type, filename] : shaderFilenames)
    {
        shader->attachShader(type, filename);
    }

    shader->linkShader();

    return shader;
}
