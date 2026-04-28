#pragma once

#include <medusa/medusa.h>
#include <medusa/graphics/shader.h>

#include <engine/resources/asset_manager.h>

namespace medusa
{
    class ShaderAsset
    {
    public:
        bool info(const std::string& assetName, std::shared_ptr<Config> config);
        std::shared_ptr<IShader> load(std::shared_ptr<IContext> context, std::shared_ptr<IAssetReader> reader);

    protected:
        PipelineState _pipelineState;
        std::unordered_map<ShaderType, std::string> _paths;
    };


    template <>
    class AssetInfo<IShader> : public ShaderAsset
    {
    public:
        bool info(const std::string& assetName, std::shared_ptr<Config> config)
        {
            return ShaderAsset::info(assetName, config);
        }

        std::shared_ptr<IShader> load(std::shared_ptr<IContext> context, std::shared_ptr<IAssetReader> reader)
        {
            return ShaderAsset::load(context, reader);
        }
    };


    namespace loaders
    {
        class ShaderLoader
        {
        public:
            ShaderLoader() = delete;

            static std::shared_ptr<IShader> loadShader(std::shared_ptr<IContext> context, const std::unordered_map<ShaderType, std::string>& shaderFilenames, const PipelineState& pipelineState = PipelineState());
        };
    }
}
