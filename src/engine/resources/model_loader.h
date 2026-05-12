#pragma once

#include <medusa/medusa.h>
#include <medusa/engine_fwd.h>
#include <medusa/graphics_fwd.h>

#include <engine/resources/asset_manager.h>


namespace medusa
{
    class Geometry;

    class ModelAsset : public AssetInfo<IModel>
    {
    public:

        bool info(const std::string& assetName, std::shared_ptr<Config> config) override;
        std::shared_ptr<IModel> load(std::shared_ptr<IContext> context, std::shared_ptr<IAssetReader> reader) override;

        const std::string& getModelFilename() const { return _meshFilename; }
        const std::string& getMaterialFilename() const { return _materialFilename; }

        void setModelFilename(const std::string& filename) { _meshFilename = filename; }
        void setMaterialFilename(const std::string& filename) { _materialFilename = filename; }

    private:
        std::string _meshFilename;
        std::string _materialFilename;
    };


    namespace loaders
    {
        /// <summary>
        ///
        /// </summary>
        class ModelLoader
        {
        public:
            ModelLoader() = delete;

            static std::shared_ptr<Geometry> loadModel(std::shared_ptr<IContext> context, const std::string& filename);
        };
    }
}
