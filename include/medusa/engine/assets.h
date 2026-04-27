#pragma once

#include <medusa/medusa.h>
#include <medusa/engine_fwd.h>
#include <medusa/graphics_fwd.h>

namespace medusa
{
    class Config; // TODO: This is part of the Interface. Config is NOT part of the interface -- but it should be


    /// <summary>
    /// Interface for accessing the asset reader
    /// </summary>
    class IAssetReader
    {
    public:
        virtual std::string readFile(const std::string& path) = 0;
        virtual std::vector<uint8_t> readBinary(const std::string& path) = 0;
    };



    /// <summary>
    /// Base information to load an asset
    /// </summary>
    /// <typeparam name="T"></typeparam>
    template <typename T>
    class AssetInfo {
    public:
        AssetInfo(const std::string& name) : _name(name) {}

        inline const std::string& getAssetName() const { return _name; }

        virtual bool info(std::shared_ptr<Config> config) = 0;
        virtual std::shared_ptr<T> load(std::shared_ptr<IContext> context, std::shared_ptr<IAssetReader> assets) = 0;
    private:
        std::string _name;
    };



    /// <summary>
    /// Interface for accessing the asset manager
    /// </summary>
    class IAssets
    {
    public:
        IAssets() {}

        virtual ~IAssets() {}

        /// <summary>
        /// Loads a texture
        /// </summary>
        /// <param name="name"></param>
        /// <returns></returns>
        virtual std::shared_ptr<ITexture> loadTexture(const std::string& name) = 0;
    };
}
