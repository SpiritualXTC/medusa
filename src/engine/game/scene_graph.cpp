#include "scene_graph.h"

#include <core/utilities/logging.h>

using namespace medusa;


//
SceneGraph::SceneGraph()
{

}


//
SceneGraph::~SceneGraph()
{

}


//
bool SceneGraph::update()
{
    bool b = EntityComponent::update();

    return b;
}
