#pragma once

#include <medusa/engine_fwd.h>

#include <medusa/engine/assets.h>

namespace medusa
{
    class Config;


    struct TextureInfo;


    class IAssetLocation
    {
    public:
        IAssetLocation() {}
        virtual ~IAssetLocation() {}


        /// <summary>
        /// Read a file
        /// </summary>
        /// <param name="path">Assetr Path</param>
        /// <returns></returns>
        virtual std::string readFile(const std::string& path) = 0;
        virtual std::vector<uint8_t> readBinary(const std::string& path) = 0;

        bool loadAssets(std::list<std::string>& assets);


        bool getInfo(const std::string& name, TextureInfo& info);

    protected:


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
