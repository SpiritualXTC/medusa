#pragma once

#include <medusa/medusa.h>
#include <medusa/graphics/shader.h>

#include <engine/resources/asset_manager.h>

namespace medusa
{
    /// <summary>
    /// Data struct to load shader info
    /// </summary>
    struct ShaderInfo
    {
        inline static const std::string type = "shaders";

        PipelineState pipelineState;
        std::unordered_map<ShaderType, std::string> paths;
    };


    namespace loaders
    {
        /// <summary>
        /// Loads the shader
        /// </summary>
        class ShaderLoader
        {
        public:
            ShaderLoader() = delete;

            static std::shared_ptr<IShader> loadShader(std::shared_ptr<IContext> context, const std::unordered_map<ShaderType, std::string>& shaderFilenames, const PipelineState& pipelineState = PipelineState());
        };
    }


    /// <summary>
    /// Template specialization for reading shader info
    /// </summary>
    /// <param name="assetName"></param>
    /// <param name="info"></param>
    /// <returns></returns>
    template<>
    bool IAssetReader::getInfo<ShaderInfo>(const std::string& assetName, ShaderInfo& info);


    /// <summary>
    /// Template specialisation for loading shaders
    /// </summary>
    /// <param name="context"></param>
    /// <param name="info"></param>
    /// <param name="reader"></param>
    /// <returns></returns>
    template<>
    std::shared_ptr<IShader> AssetManager::load(std::shared_ptr<IContext> context, const ShaderInfo& info, std::shared_ptr<IAssetReader> reader);
}
