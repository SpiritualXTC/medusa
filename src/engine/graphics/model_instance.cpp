#include "model_instance.h"

#include <medusa/engine/context.h>
#include <medusa/graphics/descriptor.h>

using namespace medusa;


//
ModelInstance::ModelInstance(std::shared_ptr<IContext> context, std::shared_ptr<IMesh> model, std::shared_ptr<GenericArray<glm::mat4>> transformBuffer)
    : _mesh(model)
    , _transformBuffer(transformBuffer)
{
    _instanceMap = context->createArray<uint32_t>(BufferType::Array, BufferUsage::StaticDraw);

    descriptor()->bind();
    _instanceMap->bind();
    descriptor()->addInstanceAttribute(medusa::types::Int, sizeof(uint32_t), InstanceAttributeLocation::TransformIndex);
    descriptor()->unbind();
    _instanceMap->unbind();
}


//
ModelInstance::~ModelInstance()
{

}


//
uint32_t ModelInstance::pushInstance(uint32_t worldIndex)
{
    auto transformBuffer = _transformBuffer.lock();

    glm::mat4 matrix = glm::identity<glm::mat4>();

    // Insert new Transform into transform buffer [should be handled by the component]
    //auto worldIndex = transformBuffer->insert(matrix);

    auto insertIndex = _instanceMap->insert(worldIndex);

    return insertIndex;
}


//
uint32_t ModelInstance::popInstance(uint32_t instanceIndex)
{
    auto transformBuffer = _transformBuffer.lock();

    throw MedusaError("Erase not supported by containers yet :(");

    return -1;
}
