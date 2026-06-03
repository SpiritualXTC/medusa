#pragma once

#include <medusa/medusa.h>
#include <medusa/engine_fwd.h>
#include <medusa/graphics_fwd.h>

namespace medusa
{
    /// <summary>
    /// Interface for accessing the asset reader
    /// </summary>
    class IAssetReader
    {
    public:

        /// <summary>
        /// Path associated with this reader
        /// </summary>
        /// <returns></returns>
        virtual std::string filepath() = 0;

        /// <summary>
        /// Read file contents into a string
        /// </summary>
        /// <param name="path"></param>
        /// <returns></returns>
        virtual std::string readFile(const std::string& path) = 0;


        /// <summary>
        /// Read file contents into a binary buffer
        /// </summary>
        /// <param name="path"></param>
        /// <returns></returns>
        virtual std::vector<uint8_t> readBinary(const std::string& path) = 0;


        /// <summary>
        /// Find assets associated with this reader
        /// </summary>
        /// <param name="assets"></param>
        /// <returns></returns>
        virtual bool findAssets(std::list<std::string>& assets) = 0;


        /// <summary>
        /// Get the asset info
        /// </summary>
        /// <returns></returns>
        virtual std::shared_ptr<IConfig> getConfig() = 0;
    };



    /// <summary>
    /// Base information to load an asset
    /// </summary>
    /// <typeparam name="T"></typeparam>
    template <typename T>
    class AssetInfo {
    public:
        AssetInfo() {}

        virtual bool info(const std::string& name, std::shared_ptr<IConfig> config) { return false; }
        virtual std::shared_ptr<T> load(std::shared_ptr<IContext> context, std::shared_ptr<IAssetReader> assets) { return nullptr; }
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
        /// Loads a shader
        /// </summary>
        /// <param name="name"></param>
        /// <returns></returns>
        virtual std::shared_ptr<IShader> loadShader(const std::string& name) = 0;


        /// <summary>
        /// Loads a texture
        /// </summary>
        /// <param name="name"></param>
        /// <returns></returns>
        virtual std::shared_ptr<ITexture> loadTexture(const std::string& name) = 0;


        /// <summary>
        ///
        /// </summary>
        /// <param name="name"></param>
        /// <returns></returns>
        virtual std::shared_ptr<IModel> loadModel(const std::string& name) = 0;


        /// <summary>
        /// Register a reader
        /// </summary>
        /// <param name="reader"></param>
        /// <returns></returns>
        virtual bool registerReader(std::shared_ptr<IAssetReader> reader) = 0;

    protected:
        virtual std::shared_ptr<IAssetReader> getReader(const std::string& name) = 0;
    };
}
