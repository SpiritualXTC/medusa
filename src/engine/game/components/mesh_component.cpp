#include "mesh_component.h"

#include <core/utilities/logging.h>

#include <medusa/graphics/mesh.h>
#include <medusa/graphics/shader.h>
#include <medusa/graphics/texture.h>

#include <medusa/renderer/pass.h>


using namespace medusa;

//
MeshComponent::MeshComponent(std::shared_ptr<IRenderableContainer> container)
    : RenderableComponent(container)
{


}


//
MeshComponent::~MeshComponent()
{

}


//
bool MeshComponent::update()
{
    // Send Material/Transform information directly to the InstanceBuffer

    // TODO: Can MultiDrawInstance, be used for "Subsets"? Worry about it once the main instancing is handled

    bool b = TransformComponent::update();

    // Add to traversable
    container()->add(this->shared_from_this());

    return true;
}


//
bool MeshComponent::render(std::shared_ptr<IPass> pass)
{
    auto shader = pass->shader();

    // TODO: This needs a magic solution :(


//  shader->setUniform("world", matrix());
//  shader->setUniform("colour", glm::vec4(1, 0, 1, 1));

//  _texture->bind(0);
//  shader->setUniform("texture", 0);

//  _mesh->render();

//  _texture->unbind(0);

    return true;
}
