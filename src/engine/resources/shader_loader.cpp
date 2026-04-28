#include "shader_loader.h"

#include <medusa/engine/context.h>

#include <core/utilities/logging.h>

#include <engine/resources/pipeline_state_parser.h>

using namespace medusa;
using namespace medusa::loaders;


bool ShaderAsset::info(const std::string& assetName, std::shared_ptr<Config> config)
{
    // Load a new Shader. No References!
    std::unordered_map<ShaderType, std::string> keys{
        {ShaderType::VertexShader, "vertex"},
        {ShaderType::FragmentShader, "fragment"},
        {ShaderType::GeometryShader, "geometry"},
    };


    // Retrieve the state
    std::string stateKey = std::format("{}.state", assetName);

    YAML::Node stateNode;

    if (config->getNode(stateKey, stateNode))
    {
        std::unordered_map<std::string, std::string> state;

        for (auto it = stateNode.begin(); it != stateNode.end(); ++it)
        {
            std::string key = it->first.as<std::string>();
            if (key == "enable")
            {
                YAML::Node enableNode = it->second.as<YAML::Node>();

                for (auto it = enableNode.begin(); it != enableNode.end(); ++it)
                {
                    std::string cap = std::format("enable.{}", it->first.as<std::string>());

                    state.insert({ cap, it->second.as<std::string>() });

                    logging::debug(std::format("Getting cap state from YAML: {}={}", it->first.as<std::string>(), it->second.as<std::string>()));
                }
            }
            else
            {
                state.insert({ it->first.as<std::string>(), it->second.as<std::string>() });

                logging::debug(std::format("Getting state from YAML: {}={}", it->first.as<std::string>(), it->second.as<std::string>()));
            }
        }

        PipelineStateParser::apply(_pipelineState, state);
    }

    // Retrieve the filenames from the resource reference
    for (auto it = keys.begin(); it != keys.end(); ++it)
    {
        std::string resourceKey = std::format("{}.{}", assetName, it->second);

        std::string s = config->getValue<std::string>(resourceKey, "");
        if (s != "")
        {
            // Make the path relative to the data directory
            std::string filepath = "../data/" + s;
            _paths.insert({ it->first, filepath });
        }
    }

    return true;
}


//
std::shared_ptr<IShader> ShaderAsset::load(std::shared_ptr<IContext> context, std::shared_ptr<IAssetReader> reader)
{
    auto shader = context->createShader();

    shader->state(_pipelineState);

    for (auto [type, filename] : _paths)
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
