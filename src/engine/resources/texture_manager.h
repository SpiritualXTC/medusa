#pragma once

#include <medusa/medusa.h>
#include <medusa/engine_fwd.h>
#include <medusa/graphics_fwd.h>

namespace medusa
{
    class TextureManager
    {
    public:
        TextureManager(std::weak_ptr<IContext> context);
        virtual ~TextureManager();

        bool hasTexture(const std::string& name);
        std::shared_ptr<ITexture> getTexture(const std::string& name);

        std::shared_ptr<ITexture> addTexture(const std::string& name, std::shared_ptr<ITexture> texture);
        std::shared_ptr<ITexture> loadTexture(const std::string& filename);

    private:

        std::weak_ptr<IContext> _context;

        std::unordered_map<std::string, std::shared_ptr<ITexture>> _textures;
    };


}
