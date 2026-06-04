#pragma once

#include <medusa/medusa.h>
#include <medusa/engine_fwd.h>
#include <medusa/graphics_fwd.h>
#include <medusa/engine/assets.h>


#include <engine/resources/asset_manager.h>


namespace medusa
{
    /// <summary>
    /// Data struct to load Texture info
    /// </summary>
    struct TextureInfo
    {
        inline static const std::string type = "textures";

        std::string filename;
    };


    namespace loaders
    {
        /// <summary>
        /// Loads the texture
        /// </summary>
        class TextureLoader
        {
        public:
            TextureLoader() = delete;

            static std::shared_ptr<ITexture> loadTexture2D(std::shared_ptr<IContext> context, const std::string& filename);
            static std::shared_ptr<ITexture> loadTexture2D(std::shared_ptr<IContext> context, std::vector<uint8_t>& bytes);
        };
    }


    /// <summary>
    /// Template specialization for reading texture info
    /// </summary>
    /// <param name="assetName"></param>
    /// <param name="info"></param>
    /// <returns></returns>
    template<>
    bool IAssetReader::getInfo<TextureInfo>(const std::string& assetName, TextureInfo& info);


    /// <summary>
    /// Template specialisation for loading textures
    /// </summary>
    /// <param name="context"></param>
    /// <param name="info"></param>
    /// <param name="reader"></param>
    /// <returns></returns>
    template<>
    std::shared_ptr<ITexture> AssetManager::load(std::shared_ptr<IContext> context, const TextureInfo& info, std::shared_ptr<IAssetReader> reader);
}
