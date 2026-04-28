#include "scene_manager.h"

#include <medusa/engine/context.h>
#include <medusa/graphics.h>
#include <medusa/graphics/containers.h>
#include <engine/resource_database.h>

#include <engine/engine.h>
#include <engine/geometry/geometry.h>
#include <engine/geometry/geometry_buffer.h>

#include <engine/resources/texture_manager.h>


using namespace medusa;


//
SceneManager::SceneManager(std::shared_ptr<Engine> engine)
    : _engine(engine)
    , _context(engine->context())
{
    auto context = engine->context();
    auto assets = engine->assets();

    // Create Material Database
    _textures = std::make_shared<TextureManager>(context, assets);

    _materials = context->createMap<Material>(BufferType::ShaderStorage, BufferUsage::StaticDraw);

    Material dummy;
    dummy.ambient(glm::vec4(0.3f, 0.0f, 0.3f, 1.0f));
    dummy.diffuse(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
    dummy.specular(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    dummy.emissive(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

    _materials->insert("[default]", dummy);

    // Create Geometry Buffer
    _geometry = std::make_shared<medusa::GeometryBuffer>(context);
}


//
bool SceneManager::addGeometry(const std::string& name, const std::shared_ptr<Geometry> geometry)
{
    _geometry->loadMesh(name, geometry, _materials, _textures);

    return true;
}


//
bool SceneManager::addTexture(const std::string& name, std::shared_ptr<ITexture> texture)
{
    // Add to the Texture Manager
    _textures->addTexture(name, texture);
    return true;
}


//
std::shared_ptr<ITexture> SceneManager::loadTexture(const std::string& assetName)
{
    auto engine = _engine.lock();

    return _textures->getTexture(assetName);
}


//
std::shared_ptr<IShader> SceneManager::loadShader(const std::string& assetName)
{
    auto engine = _engine.lock();

    // Shaders are never cached
    return engine->assets()->loadShader(assetName);
}
