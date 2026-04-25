#include "resource_database.h"

#include <format>
#include <unordered_map>

#include <medusa/engine/context.h>
#include <medusa/graphics/shader.h>
#include <medusa/graphics/mesh.h>

#include <core/utilities/logging.h>

#include <engine/resources/model_loader.h>
#include <engine/resources/texture_loader.h>
#include <engine/resources/shader_loader.h>
#include <engine/resources/pipeline_state_parser.h>


using namespace medusa;


//
ResourceDatabase::ResourceDatabase(const std::shared_ptr<IContext> context)
    : Config("../data/assets.yaml")
    , _context(context)
{

}


//
std::shared_ptr<IShader> ResourceDatabase::getShader(const std::string& shaderName, PipelineState& pipelineState)
{
    // Load a new Shader. No References!
    std::unordered_map<ShaderType, std::string> keys{
        {ShaderType::VertexShader, "vertex"},
        {ShaderType::FragmentShader, "fragment"},
        {ShaderType::GeometryShader, "geometry"},
    };

    std::unordered_map<ShaderType, std::string> paths{};

    // Retrieve the state
    std::string stateKey = std::format("resources.shaders.{}.state", shaderName);

    YAML::Node stateNode;

    if (getNode(stateKey, stateNode))
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

        PipelineStateParser::apply(pipelineState, state);
    }

    // Retrieve the filenames from the resource reference
    for (auto it = keys.begin(); it != keys.end(); ++it)
    {
        std::string resourceKey = std::format("resources.shaders.{}.{}", shaderName, it->second);

        std::string s = getValue<std::string>(resourceKey, "");
        if (s != "")
        {
            // Make the path relative to the data directory
            std::string filepath = "../data/" + s;
            paths.insert({ it->first, filepath });
        }
    }

    return loaders::ShaderLoader::loadShader(_context.lock(), paths, pipelineState);

}


//
std::shared_ptr<Geometry> ResourceDatabase::getModel(const std::string& modelName)
{
    logging::error("Loading Model");

    std::string resourceKey = std::format("resources.models.{}.model", modelName);
    std::string resourceFile = getValue<std::string>(resourceKey, "");

    if (resourceFile == "")
    {
        logging::error("Unable to find resource with matching key: {}", modelName);
        return nullptr;
    }


    std::string filepath = "../data/" + resourceFile;

    return loaders::ModelLoader::loadModel(_context.lock(), filepath);
}
