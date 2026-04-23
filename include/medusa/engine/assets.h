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

        virtual std::shared_ptr<ITexture> loadTexture(const std::string& name) = 0;
    };
}
