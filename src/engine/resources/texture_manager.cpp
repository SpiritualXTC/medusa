#include "texture_manager.h"

#include <engine/resources/texture_loader.h>

#include <core/utilities/logging.h>

using namespace medusa;


//
TextureManager::TextureManager(std::weak_ptr<IContext> context)
    : _context(context)
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
std::shared_ptr<ITexture> TextureManager::getTexture(const std::string& name)
{
    auto it = _textures.find(name);

    if (it == _textures.end())
        return nullptr;

    return it->second;
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
