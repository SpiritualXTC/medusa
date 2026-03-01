#include "mesh_component.h"

#include <core/utilities/logging.h>

#include <engine/geometry/geometry_buffer.h>

using namespace medusa;


//
MeshInstanceComponent::MeshInstanceComponent(std::shared_ptr<IMeshReference> meshRef, size_t transIndex)
    : _meshRef(meshRef)
{
    _instanceIndex = _meshRef->pushInstance(transIndex);
}


//
MeshInstanceComponent::~MeshInstanceComponent()
{
    _meshRef->popInstance(_instanceIndex);
}


//
bool MeshInstanceComponent::update()
{
    return true;
}
