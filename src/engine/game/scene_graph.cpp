#include "scene_graph.h"

#include <core/utilities/logging.h>

#include <engine/engine.h>
#include <medusa/engine/context.h>
#include <engine/game/scene_manager.h>

using namespace medusa;


//
SceneGraph::SceneGraph(std::shared_ptr<SceneManager> manager)
    : _manager(manager)
{
    auto engine = manager->engine().lock();

    // Create the World Transform Buffer
    glm::mat4 identity = glm::identity<glm::mat4>();
    _matrixBuffer = engine->context()->createArray<glm::mat4>(medusa::BufferType::ShaderStorage, medusa::BufferUsage::DynamicDraw);
    _matrixBuffer->insert(identity);
}


//
SceneGraph::~SceneGraph()
{

}


//
bool SceneGraph::update()
{
    bool b = EntityComponent::update();

    // Resync the transform buffer
    _matrixBuffer->sync();

    return b;
}
