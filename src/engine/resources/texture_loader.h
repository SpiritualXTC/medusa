#pragma once

#include <medusa/medusa.h>
#include <medusa/engine_fwd.h>
#include <medusa/graphics_fwd.h>


namespace medusa
{
    namespace loaders
    {
        class TextureLoader
        {
        public:
            TextureLoader() = delete;

            static std::shared_ptr<ITexture> loadTexture2D(std::shared_ptr<IContext> context, const std::string& filename);

        private:
            std::weak_ptr<IContext> _context;
        };
    }
}
