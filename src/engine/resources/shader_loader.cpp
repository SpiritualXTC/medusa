#include "shader_loader.h"

#include <medusa/engine/context.h>
#include <medusa/engine/config.h>

#include <core/utilities/logging.h>

#include <engine/resources/pipeline_state_parser.h>

using namespace medusa;
using namespace medusa::loaders;


//
template<>
bool IAssetReader::getInfo<ShaderInfo>(const std::string& assetName, ShaderInfo& info)
{
    // Load a new Shader. No References!
    std::unordered_map<ShaderType, std::string> keys{
        {ShaderType::VertexShader, "vertex"},
        {ShaderType::FragmentShader, "fragment"},
        {ShaderType::GeometryShader, "geometry"},
    };

    // Retrieve the state
    std::unordered_map<std::string, std::string> state;
    std::string stateKey = std::format("{}.state", assetName);
    std::string enableKey = std::format("{}.state.enable", assetName);

    const IConfig::PTree& stateNode = getConfig()->node(stateKey);
    const IConfig::PTree& enableNode = getConfig()->node(enableKey);

    if (!stateNode.empty())
    {
        for (auto it = stateNode.begin(); it != stateNode.end(); ++it)
        {
            std::string key = it->first.c_str();
            logging::error(fmt::format("Shader State Key: {}, {}", key, it->second.data()));

            state.insert({ it->first.c_str(), it->second.data()});
        }
    }

    if (!enableNode.empty())
    {
        for (auto it = enableNode.begin(); it != enableNode.end(); ++it)
        {
            std::string cap = std::format("enable.{}", it->first.c_str());
            logging::error(fmt::format("Shader State Cap: {}, {}", it->first.c_str(), it->second.data()));
            state.insert({ cap, it->second.data()});
        }
    }

    // Populate the pipelinestate from the Map.
    // TODO: Why is the out param before the input? stupid AI
    if (!state.empty())
    {
        PipelineStateParser::apply(info.pipelineState, state);
    }

    // Retrieve the filenames from the resource reference
    for (auto it = keys.begin(); it != keys.end(); ++it)
    {
        std::string resourceKey = std::format("{}.{}", assetName, it->second);

        std::string s = getConfig()->value<std::string>(resourceKey, "");
        if (s != "")
        {
            // Make the path relative to the data directory
            std::string filepath = "../data/" + s;
            info.paths.insert({ it->first, filepath });
        }
    }

    return true;
}


//
template<>
std::shared_ptr<IShader> AssetManager::load(std::shared_ptr<IContext> context, const ShaderInfo& info, std::shared_ptr<IAssetReader> reader)
{
    auto shader = context->createShader();

    shader->state(info.pipelineState);

    for (auto [type, filename] : info.paths)
    {
        std::string contents = reader->readFile(filename);

        shader->attachShaderFromSource(type, contents);
    }

    shader->linkShader();

    return shader;
}


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
