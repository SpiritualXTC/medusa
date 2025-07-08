#include "scene.h"

#include <engine/renderer/view.h>
#include <engine/renderer/pipeline.h>

using namespace medusa;

//
Scene::Scene(std::shared_ptr<IView> view)
    : _view(view)
{

}


//
Scene::~Scene()
{

}


//
bool Scene::update(float ft)
{
    return true;
}


//
bool Scene::render()
{
    _view->render();


    return true;
}
