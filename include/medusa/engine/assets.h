#pragma once

#include <medusa/medusa.h>
#include <medusa/graphics_fwd.h>

namespace medusa
{
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
