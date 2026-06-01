#pragma once

#include <medusa/medusa.h>
#include <medusa/engine_fwd.h>
#include <medusa/graphics_fwd.h>
#include <medusa/engine/assets.h>


#include <engine/resources/asset_manager.h>


namespace medusa
{

    /// <summary>
    ///
    /// </summary>
    class TextureAsset : public AssetInfo<ITexture>
    {
    public:

        bool info(const std::string& assetName, std::shared_ptr<IConfig> config) override;
        std::shared_ptr<ITexture> load(std::shared_ptr<IContext> context, std::shared_ptr<IAssetReader> reader) override;


        const std::string& getFilename() const { return _filename; }

        void setFilename(const std::string& filename) { _filename = filename; }


    private:
        std::string _filename;
    };



    namespace loaders
    {
        /// <summary>
        ///
        /// </summary>
        class TextureLoader
        {
        public:
            TextureLoader() = delete;

            static std::shared_ptr<ITexture> loadTexture2D(std::shared_ptr<IContext> context, const std::string& filename);
            static std::shared_ptr<ITexture> loadTexture2D(std::shared_ptr<IContext> context, std::vector<uint8_t>& bytes);
        };
    }
}
