#include "mesh_instance.h"

#include <medusa/engine/context.h>
#include <medusa/graphics/instance_buffer.h>

using namespace medusa;


//
MeshInstance::MeshInstance(std::shared_ptr<IContext> context)
    : Mesh(context)
{
    _instanceBuffer = context->create_instance_buffer();
}


//
MeshInstance::~MeshInstance()
{

}


//
bool MeshInstance::render(size_t instances)
{
    _instanceBuffer->bind();

    Mesh::render(instances);

    _instanceBuffer->unbind();

    return true;
}
