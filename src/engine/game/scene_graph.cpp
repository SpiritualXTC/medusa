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
    _transformBuffer = engine->context()->createArray<glm::mat4>(medusa::BufferType::ShaderStorage, medusa::BufferUsage::DynamicDraw);
    _transformBuffer->insert(identity);

    // Create the root of the scene graph
    _root = std::make_shared<ObjectComponent>(_transformBuffer);
}


//
SceneGraph::~SceneGraph()
{

}


//
bool SceneGraph::update()
{
    bool b = _root->update();

    // Resync the transform buffer
    _transformBuffer->sync();

    return b;
}
