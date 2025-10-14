#include "scene_manager.h"

#include <medusa/engine/context.h>
#include <medusa/graphics.h>
#include <medusa/graphics/containers.h>
#include <engine/resource_database.h>

#include <engine/engine.h>

using namespace medusa;

//
SceneManager::SceneManager(std::shared_ptr<Engine> engine)
    : _engine(engine)
    , _context(engine->context())
{
    auto context = engine->context();

    // Create Material Database
    _materials = context->createMap<Material>(BufferType::ShaderStorage, BufferUsage::StaticDraw);

    Material dummy;
    dummy.ambient(glm::vec4(0.3f, 0.0f, 0.3f, 1.0f));
    dummy.diffuse(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
    dummy.specular(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    dummy.emissive(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

    _materials->insert("[default]", dummy);
}


//
std::shared_ptr<IMesh> SceneManager::getModel(const std::string& modelName)
{
    auto engine = _engine.lock();

    return engine->resources()->getModel(modelName, _materials);
}
