#pragma once

#include <medusa/medusa.h>
#include <medusa/engine/assets.h>

#include <engine/resources/asset_manager.h>

namespace medusa
{
    //
    class AssetDirectory : public IAssetLocation
    {
    public:
        //
        AssetDirectory(const std::string& path);

        /// <summary>
        /// Read a text file
        /// </summary>
        /// <param name="path"></param>
        /// <returns></returns>
        std::string readFile(const std::string& path) override;


        /// <summary>
        /// Read a binary file
        /// </summary>
        /// <param name="path"></param>
        /// <returns></returns>
        std::vector<uint8_t> readBinary(const std::string& path) override;

    private:
        std::string _path;
    };
} // end namespace medusa
