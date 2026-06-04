#pragma once

#include <medusa/medusa.h>
#include <medusa/engine_fwd.h>
#include <medusa/graphics_fwd.h>

#include <engine/resources/asset_manager.h>


namespace medusa
{
    class Geometry;


    /// <summary>
    /// Data struct to load model info
    /// </summary>
    struct ModelInfo
    {
        inline static const std::string type = "models";

        std::string meshFilename;
        std::string materialFilename;
    };


    namespace loaders
    {
        /// <summary>
        /// Loads the model
        /// </summary>
        class ModelLoader
        {
        public:
            ModelLoader() = delete;

            static std::shared_ptr<Geometry> loadModel(std::shared_ptr<IContext> context, const std::string& filename);
        };
    }


    /// <summary>
    /// Template specialization for reading model info
    /// </summary>
    /// <param name="assetName"></param>
    /// <param name="info"></param>
    /// <returns></returns>
    template<>
    bool IAssetReader::getInfo(const std::string& assetName, ModelInfo& info);


    /// <summary>
    /// Template specialisation for loading models
    /// </summary>
    /// <param name="context"></param>
    /// <param name="info"></param>
    /// <param name="reader"></param>
    /// <returns></returns>
    template<>
    std::shared_ptr<IModel> AssetManager::load(std::shared_ptr<IContext> context, const ModelInfo& info, std::shared_ptr<IAssetReader> reader);
}
