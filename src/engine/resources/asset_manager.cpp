#include "asset_manager.h"

#include <boost/filesystem.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

#include <core/utilities/config.h>
#include <core/utilities/logging.h>

#include <engine/resources/texture_loader.h>

namespace fs = boost::filesystem;

using namespace medusa;

namespace medusa
{
    struct TextureInfo
    {
        std::string filename;
    };
}

//
bool IAssetLocation::getInfo(const std::string& name, TextureInfo& info)
{

    std::string pathNodeName = fmt::format("{}.filename", name);

    info.filename = _config->getValue<std::string>(pathNodeName, "");

    return true;
}

//
bool IAssetLocation::loadAssets(std::list<std::string>& assets)
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
        YAML::Node assetRootNode;
        if (_config->getNode(nodePath, assetRootNode))
        {
            for (auto it = assetRootNode.begin(); it != assetRootNode.end(); ++it)
            {
                std::string assetPath = std::format("{}.{}", nodePath, it->first.as<std::string>());
                logging::warn(fmt::format("Found Asset: {}", assetPath));

                assets.push_back(assetPath);
            }

        }
    }

    return true;
}




class AssetDirectory : public IAssetLocation
{
public:
    AssetDirectory(const std::string& path)
        : _path(path)
    {

    }


    std::string readFile(const std::string& path) override
    {
        fs::path filePath = fs::path(_path + path);

        // Check the file exists and is a regular file
        if (!fs::exists(filePath))
        {
            throw MedusaError("File not found: " + filePath.string());
        }
        if (!fs::is_regular_file(filePath))
        {
            throw MedusaError("Not a regular file: " + filePath.string());
        }

        std::ifstream file(filePath.string());
        if (!file.is_open())
        {
            throw MedusaError("Failed to open: " + filePath.string());
        }

        std::ostringstream ss;
        ss << file.rdbuf();
        return ss.str();;
    }

    std::vector<uint8_t> readBinary(const std::string& path) override
    {
        fs::path filePath = fs::path(_path + path);

        // Check the file exists and is a regular file
        if (!fs::exists(filePath))
        {
            throw MedusaError("File not found: " + filePath.string());
        }
        if (!fs::is_regular_file(filePath))
        {
            throw MedusaError("Not a regular file: " + filePath.string());
        }

        // Open the file at the end, simplifiy getting the size
        std::ifstream file(filePath.string(), std::ios::binary | std::ios::ate);

        // Get filesize
        auto size = file.tellg();
        file.seekg(0);

        // Read the blob
        std::vector<uint8_t> buf(static_cast<size_t>(size));
        file.read(reinterpret_cast<char*>(buf.data()), size);

        return buf;
    }


private:
    std::string _path;
};




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

    if (!assetLocation->loadAssets(assetList))
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
std::shared_ptr<ITexture> AssetManager::loadTexture(const std::string& name)
{
    std::string assetName = fmt::format("resources.textures.{}", name);

    std::shared_ptr<IAssetLocation> location = getLocation(assetName);
    if (! location)
        return nullptr;

    // Read the TextureInfo to get the filename
    TextureInfo info;
    if (!location->getInfo(assetName, info))
        return nullptr;


    std::vector<uint8_t> buffer = location->readBinary(info.filename);


    std::shared_ptr<ITexture> texture = loaders::TextureLoader::loadTexture2D(_context.lock(), buffer);

    return texture;
}
