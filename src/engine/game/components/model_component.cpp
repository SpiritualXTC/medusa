#include "model_component.h"

#include <engine/graphics/model_instance.h>

using namespace medusa;


//
ModelComponent::ModelComponent(std::shared_ptr<ModelInstance> mesh, std::shared_ptr<GenericArray<glm::mat4>> transformBuffer)
    : _mesh(mesh)
    , _transformBuffer(transformBuffer)
{
    // TODO: This should be attached the ENTITY, and multiply around... somehow
    _transformIndex = transformBuffer->insert(matrix());

    _mesh->pushInstance(_transformIndex);
}


//
ModelComponent::~ModelComponent()
{
    auto transformBuffer = _transformBuffer.lock();
    transformBuffer->erase(_transformIndex);
}


//
bool ModelComponent::update()
{
    // TODO: This should only happen on a change ... ? It should also NOT go to the GPU directly, but in bulk push. (Later problems)

    TransformComponent::update();

    auto transformBuffer = _transformBuffer.lock();

    auto& m = transformBuffer->data(_transformIndex);
    m = matrix();
    transformBuffer->sync(_transformIndex);

    return true;
}
