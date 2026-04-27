#pragma once

#include <medusa/engine_fwd.h>

#include <medusa/engine/assets.h>

#include <core/utilities/config.h>

namespace medusa
{
    class TextureAsset;


    class Config;

    class IAssetLocation : public IAssetReader
    {
    public:
        IAssetLocation() {}
        virtual ~IAssetLocation() {}

        bool loadAssets(std::list<std::string>& assets);

        std::shared_ptr<Config> getConfig() { return _config; }


    private:
        std::shared_ptr<Config> _config;
    };



    class AssetManager : public IAssets
    {
    public:
        AssetManager(std::shared_ptr<IContext> context);
        virtual ~AssetManager();


        bool registerDirectory(const std::string& path);
        bool registerData(const std::string& path);

        //
        std::shared_ptr<ITexture> loadTexture(const std::string& name) override;


    private:
        std::weak_ptr<IContext> _context;


        std::shared_ptr<IAssetLocation> getLocation(const std::string& name);

        std::vector<std::shared_ptr<IAssetLocation>> _locations;
        std::unordered_map<std::string, std::weak_ptr<IAssetLocation>> _assets;
    };
}
