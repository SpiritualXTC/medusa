#include "asset_manager.h"

#include <core/utilities/config_yaml.h>
#include <core/utilities/logging.h>

#include <engine/resources/asset_directory.h>
#include <engine/resources/texture_loader.h>
#include <engine/resources/shader_loader.h>
#include <engine/resources/model_loader.h>

using namespace medusa;


//
bool IAssetLocation::findAssets(std::list<std::string>& assets)
{
    // Load the YAML
    std::string contents = readFile("assets.yaml");
    _config = ConfigYAML::loadFromString(contents);

    std::string types[] = {
        "textures",
        "models",
        "shaders",
    };

    for (auto& assetType : types)
    {
        std::string nodePath = std::format("resources.{}", assetType);
        const IConfig::PTree& assetRoot = _config->node(nodePath);

        if (!assetRoot.empty())
        {
            for (auto it = assetRoot.begin(); it != assetRoot.end(); ++it)
            {
                std::string assetPath = std::format("{}.{}", nodePath, it->first.c_str());
                logging::warn(fmt::format("Found Asset: {}", assetPath));

                assets.push_back(assetPath);
            }
        }
    }

    return true;
}


//
AssetManager::AssetManager(std::shared_ptr<IContext> context)
    : _context(context)
{

}


//
AssetManager::~AssetManager()
{

}


//
bool AssetManager::registerReader(std::shared_ptr<IAssetReader> reader)
{
    std::list<std::string> assetList;

    // Find assets from the reader
    if (!reader->findAssets(assetList))
        throw MedusaError(std::format("Failed to find assets from `{}`", reader->filepath()));

    // Register assets from the reader
    for (auto& item : assetList)
    {
        _assets.insert({ item, reader });
    }

    // Store the reader
    _locations.push_back(reader);

    return true;
}


//
bool AssetManager::registerDirectory(const std::string& path)
{
    std::shared_ptr<AssetDirectory> assetLocation = std::make_shared<AssetDirectory>(path);

    // Register the directory location
    if (!registerReader(assetLocation))
        return false;

    return true;
}


//
bool AssetManager::registerData(const std::string& path)
{
    return false;
}


//
std::shared_ptr<IAssetReader> AssetManager::getReader(const std::string& name)
{
    auto it = _assets.find(name);

    if (it == _assets.end())
    {
        return nullptr;
    }

    return it->second.lock();
}


//
std::shared_ptr<IShader> AssetManager::loadShader(const std::string& name)
{
    return loadAsset<IShader, ShaderInfo>(name);
}


//
std::shared_ptr<ITexture> AssetManager::loadTexture(const std::string& name)
{
    return loadAsset<ITexture, TextureInfo>(name);
}


//
std::shared_ptr<IModel> AssetManager::loadModel(const std::string& name)
{
    return loadAsset<IModel, ModelInfo>(name);
}
