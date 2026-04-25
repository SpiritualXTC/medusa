#include "texture_manager.h"

#include <medusa/engine/assets.h>

#include <engine/resources/texture_loader.h>

#include <core/utilities/logging.h>

using namespace medusa;


//
TextureManager::TextureManager(std::weak_ptr<IContext> context, std::weak_ptr<IAssets> assets)
    : _context(context)
    , _assets(assets)
{

}


//
TextureManager::~TextureManager()
{

}


//
bool TextureManager::hasTexture(const std::string& name)
{
    return _textures.find(name) == _textures.end();;
}


//
std::shared_ptr<ITexture> TextureManager::addTexture(const std::string& name, std::shared_ptr<ITexture> texture)
{
    _textures.insert({ name, texture });

    return texture;
}


//
std::shared_ptr<ITexture> TextureManager::loadTexture(const std::string& filename)
{
    logging::info(fmt::format("Loading texture: `{}`", filename));


    auto texture = loaders::TextureLoader::loadTexture2D(_context.lock(), filename);

    return addTexture(filename, texture);
}


//
std::shared_ptr<ITexture> TextureManager::getTexture(const std::string& name)
{
    std::shared_ptr<ITexture> texture;

    auto it = _textures.find(name);

    if (it != _textures.end())
    {
        logging::debug(fmt::format("Loading asset texture from cache: `{}`", name));
        return it->second;
    }

    auto assets = _assets.lock();
    logging::info(fmt::format("Loading asset texture: `{}`", name));

    return addTexture(name, assets->loadTexture(name));
}
