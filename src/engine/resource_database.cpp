#include "resource_database.h"

#include <format>
#include <unordered_map>

#include <medusa/engine/context.h>
#include <medusa/graphics/shader.h>
#include <medusa/graphics/mesh.h>

#include <core/utilities/logging.h>

#include <engine/resources/model_loader.h>
#include <engine/resources/texture_loader.h>

using namespace medusa;


//
ResourceDatabase::ResourceDatabase(const std::shared_ptr<IContext> context)
    : Config("../data/resource.yaml")
    , _context(context)
{

}


//
std::shared_ptr<IShader> ResourceDatabase::getShader(const std::string& shaderName)
{
    // ALWAYS. Load a new Shader. No References!

    std::unordered_map<ShaderType, std::string> keys{
        {ShaderType::VertexShader, "vertex"},
        {ShaderType::FragmentShader, "fragment"},
        {ShaderType::GeometryShader, "geometry"},
    };

    // Lock the context pointer
    auto context = _context.lock();

    auto shader = context->createShader();

    // TODO: Move to a loader
    for (auto it = keys.begin(); it != keys.end(); ++it)
    {
        std::string resourceKey = std::format("resources.shaders.{}.{}", shaderName, it->second);

        std::string s = getValue<std::string>(resourceKey, "");
        if (s != "")
        {
            std::string filepath = "../data/" + s;
            shader->attachShader(it->first, filepath);
        }
    }

    shader->linkShader();

    return shader;

}


//
std::shared_ptr<Model> ResourceDatabase::getModel(const std::string& modelName, std::shared_ptr<GenericMap<Material>> materials, std::shared_ptr<TextureManager> textures)
{
    logging::error("Loading Model");

    auto context = _context.lock();

    std::string resourceKey = std::format("resources.models.{}.model", modelName);
    std::string resourceFile = getValue<std::string>(resourceKey, "");

    if (resourceFile == "")
    {
        logging::error("Unable to find resource with matching key: {}", modelName);
        return nullptr;
    }


    std::string filepath = "../data/" + resourceFile;

    std::shared_ptr<loaders::ModelLoader> loader = std::make_shared<loaders::ModelLoader>(context);

    return loader->load(filepath);
}
