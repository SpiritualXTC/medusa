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

        std::shared_ptr<Config> getConfig() { return _config; }

        bool findAssets(std::list<std::string>& assets) override;

    private:
        std::shared_ptr<Config> _config;
    };



    class AssetManager : public IAssets
    {
    public:
        AssetManager(std::shared_ptr<IContext> context);
        virtual ~AssetManager();


        /// <summary>
        /// Register a directory as a location of assets
        /// </summary>
        /// <param name="path"></param>
        /// <returns></returns>
        bool registerDirectory(const std::string& path);


        /// <summary>
        /// Register a packaged file as a location of assets
        /// </summary>
        /// <param name="path"></param>
        /// <returns></returns>
        bool registerData(const std::string& path);


        /// <summary>
        /// Loads a Shader by it's registered name
        /// </summary>
        /// <param name="name"></param>
        /// <returns></returns>
        std::shared_ptr<IShader> loadShader(const std::string& name) override;


        /// <summary>
        /// Loads a Texture by it's registered name
        /// </summary>
        /// <param name="name"></param>
        /// <returns></returns>
        std::shared_ptr<ITexture> loadTexture(const std::string& name) override;


        /// <summary>
        /// Loads a Model by it's registered name
        /// </summary>
        std::shared_ptr<IModel> loadModel(const std::string& name) override;

    private:
        std::weak_ptr<IContext> _context;


        std::shared_ptr<IAssetLocation> getLocation(const std::string& name);

        std::vector<std::shared_ptr<IAssetLocation>> _locations;
        std::unordered_map<std::string, std::weak_ptr<IAssetLocation>> _assets;
    };
}
