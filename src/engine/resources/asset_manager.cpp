#include "asset_manager.h"

#include <core/utilities/config.h>
#include <core/utilities/logging.h>

#include <engine/resources/asset_directory.h>
#include <engine/resources/texture_loader.h>
#include <engine/resources/shader_loader.h>
#include <engine/resources/model_loader.h>

using namespace medusa;


//
bool IAssetLocation::findAssets(std::list<std::string>& assets)
{
    // Get the Assets YAML
    std::string contents = readFile("assets.yaml");

    // Load the YAML
    _config = std::make_shared<Config>();
    _config->loadFromString(contents);

    std::string types[] = {
        "textures",
        "models",
        "shaders",
    };

    for (auto& assetType : types)
    {

        std::string nodePath = std::format("resources.{}", assetType);
        const Config::PTree& assetRoot = _config->node(nodePath);

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
bool AssetManager::registerDirectory(const std::string& path)
{
    std::shared_ptr<AssetDirectory> assetLocation = std::make_shared<AssetDirectory>(path);

    std::list<std::string> assetList;

    if (!assetLocation->findAssets(assetList))
        throw MedusaError(std::format("Failed to load the asset list from `{}`", path));

    std::weak_ptr<IAssetLocation> location = assetLocation;
    for (auto& item : assetList)
    {
        _assets.insert({ item, location });
    }

    // Store the directory
    _locations.push_back(assetLocation);

    return true;
}


//
bool AssetManager::registerData(const std::string& path)
{
    return false;
}


//
std::shared_ptr<IAssetLocation> AssetManager::getLocation(const std::string& name)
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
    std::string assetName = fmt::format("resources.shaders.{}", name);

    // Get the location from the asset map
    std::shared_ptr<IAssetLocation> location = getLocation(assetName);
    if (!location)
        return nullptr;

    // Extract info from config
    ShaderAsset asset = ShaderAsset();
    if (!asset.info(assetName, location->getConfig()))
        return nullptr;

    // Load the asset from the location
    return asset.load(_context.lock(), location);
}


//
std::shared_ptr<ITexture> AssetManager::loadTexture(const std::string& name)
{
    std::string assetName = fmt::format("resources.textures.{}", name);

    // Get the location from the asset map
    std::shared_ptr<IAssetLocation> location = getLocation(assetName);
    if (! location)
        return nullptr;

    // Extract info from config
    TextureAsset asset = TextureAsset();
    if (! asset.info(assetName, location->getConfig()))
        return nullptr;

    // Load the asset from the location
    return asset.load(_context.lock(), location);
}


//
std::shared_ptr<IModel> AssetManager::loadModel(const std::string& name)
{
    std::string assetName = fmt::format("resources.models.{}", name);

    // Get the location from the asset map
    std::shared_ptr<IAssetLocation> location = getLocation(assetName);
    if (!location)
        return nullptr;

    // Extract info from Config
    ModelAsset asset = ModelAsset();
    if (!asset.info(assetName, location->getConfig()))
        return nullptr;

    return asset.load(_context.lock(), location);
}
