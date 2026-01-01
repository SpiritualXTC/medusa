#include "mesh_component.h"

#include <engine/graphics/model_instance.h>

using namespace medusa;


//
MeshComponent::MeshComponent(std::shared_ptr<IMeshInstance> instance)
    : _instance(instance)
{

}


//
MeshComponent::~MeshComponent()
{

}


//
bool MeshComponent::update()
{

    return true;
}
