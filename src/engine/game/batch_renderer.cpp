#include "batch_renderer.h"

#include <medusa/engine/context.h>
#include <medusa/graphics.h>

#include <core/utilities/logging.h>


using namespace medusa;


//
Batch::Batch(std::shared_ptr<IContext> context, std::shared_ptr<IMesh> model, std::shared_ptr<TransformBuffer> transformBuffer)
    : _mesh(model)
    , _transformBuffer(transformBuffer)
{
    _instanceMap = context->createArray<uint32_t>(BufferType::Array, BufferUsage::StaticDraw);

    model->descriptor()->bind();
    _instanceMap->bind();
    model->descriptor()->addInstanceAttribute(medusa::types::Int, sizeof(uint32_t), InstanceAttributeLocation::TransformIndex);
    model->descriptor()->unbind();
    _instanceMap->unbind();
}


//
Batch::~Batch()
{

}


//
std::shared_ptr<IMeshInstance> Batch::pushInstance(std::shared_ptr<Transform> transform)
{
    auto transformBuffer = _transformBuffer.lock();

    auto insertIndex = _instanceMap->insert(transform->transformIndex());

    std::shared_ptr<BatchInstance> instance = std::make_shared<BatchInstance>();

    return instance;
}


//
void Batch::popInstance(std::shared_ptr<IMeshInstance> instance)
{
    throw MedusaError("Erase not supported by containers yet :(");
}


//
void Batch::batchRender()
{
    _mesh->render(_instanceMap->size());
}



//
BatchRenderer::BatchRenderer(std::shared_ptr<IContext> context, std::shared_ptr<TransformBuffer> transformBuffer)
    : _context(context)
    , _transformBuffer(transformBuffer)
{

}


//
BatchRenderer::~BatchRenderer()
{


}


//
std::shared_ptr<IMeshInstance> BatchRenderer::createBatchInstance(std::shared_ptr<IMesh> mesh, std::shared_ptr<Transform> transform)
{
    auto transformBuffer = _transformBuffer.lock();
    auto context = _context.lock();

    // RAII: Create the Model Instances
    std::shared_ptr<Batch> batch = nullptr;

    auto it = _batches.find(mesh->id());
    if (it == _batches.end())
    {
        // Add new instance
        batch = std::make_shared<Batch>(context, mesh, transformBuffer);
        _batches.insert({ mesh->id(), batch });
    }
    else
    {
        batch = it->second;
    }

    return batch->pushInstance(transform);
}


//
bool BatchRenderer::render()
{
    for (auto instance : _instances)
        instance.second->render();

    for (auto batch : _batches)
    {
        // TODO: Need a way to render from the interface
        batch.second->batchRender();
    }

    return true;
}
