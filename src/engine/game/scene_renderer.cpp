#include "scene_renderer.h"

#include <medusa/engine/context.h>
#include <medusa/graphics.h>

#include <core/utilities/logging.h>



using namespace medusa;


//
MeshContainer::MeshContainer(std::shared_ptr<IContext> context, std::shared_ptr<GenericArray<glm::mat4>> transformBuffer)
    : _context(context)
    , _transformBuffer(transformBuffer)
{

}


//
MeshContainer::~MeshContainer()
{


}


//
std::shared_ptr<ModelComponent> MeshContainer::createInstance(std::shared_ptr<IMesh> mesh)
{
    auto transformBuffer = _transformBuffer.lock();
    auto context = _context.lock();

    // RAII: Create the Model Instances
    std::shared_ptr<ModelInstance> instance = nullptr;

    auto it = _instances.find(mesh->id());
    if (it == _instances.end())
    {
        // Add new instance
        instance = std::make_shared<ModelInstance>(context, mesh, transformBuffer);
        _instances.insert({ mesh->id(), instance });
    }
    else
    {
        instance = it->second;
    }

    std::shared_ptr<ModelComponent> component = std::make_shared<ModelComponent>(instance, transformBuffer);

    return component;
}


//
bool MeshContainer::render()
{
    for (auto instance : _instances)
        instance.second->render();

    return true;
}
