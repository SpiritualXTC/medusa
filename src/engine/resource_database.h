#pragma once

#include <medusa/medusa.h>
#include <medusa/engine_fwd.h>
#include <medusa/graphics_fwd.h>
#include <medusa/containers_fwd.h>


#include <core/utilities/config.h>

namespace medusa
{
    class Geometry;
    class TextureManager;

    class ResourceDatabase : public Config
    {
    public:
        ResourceDatabase(const std::shared_ptr<IContext> context);

        std::shared_ptr<IShader> getShader(const std::string& shaderName);

        std::shared_ptr<Geometry> getModel(const std::string& modelName);

    private:
        std::weak_ptr<IContext> _context;
    };
}
