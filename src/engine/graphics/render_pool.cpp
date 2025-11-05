#include "render_pool.h"

#include <medusa/graphics.h>
#include <medusa/renderer/pass.h>
#include <medusa/engine/context.h>

#include <core/utilities/logging.h>

using namespace medusa;


//
RenderPool::RenderPool(std::shared_ptr<IContext> context)
    : _context(context)
{

}


//
RenderPool::~RenderPool()
{

}


//
void RenderPool::addAttribute(const DType& dt, InstanceAttributeLocation location)
{
    InstanceData data{ dt, location };

    _data.push_back(data);
}


//
void RenderPool::create_mesh_instance(std::shared_ptr<IMesh> mesh)
{
    auto descriptor = mesh->descriptor();
    auto instanceBuffer = _context->createArray<glm::mat4>(BufferType::Array, BufferUsage::StaticDraw, nullptr, 10000);

    // Bind Instance Buffer to Descriptor
    descriptor->bind();
    instanceBuffer->bind();


    for (auto& id : _data)
    {
        // TODO: Shouldn't be part of the descriptor...
        // THIS IS STILL USED BY THE WIREFRAME RENDERER
        descriptor->addInstanceAttribute(id.data_type, sizeof(glm::mat4), id.location);
    }

    // Unbind Descriptor
    descriptor->unbind();

    // Unbind Instance Buffer
    instanceBuffer->unbind();

    std::shared_ptr<Batch> batch = std::make_shared<Batch>(mesh, instanceBuffer);

    _batch.insert({ mesh->id(), batch }); // DIS ONE

    return;
}


//
bool RenderPool::add(std::shared_ptr<IRenderable> renderable)
{
    auto it = _batch.find(renderable->resource_id());

    if (it == _batch.end())
        return false;

    auto& batch = it->second;

    glm::mat4 m = renderable->transform();

    batch->instances->insert(m);

    return true;
}


//
bool RenderPool::clear()
{
    for (auto& it : _batch)
    {
        auto& batch = it.second;
        batch->instances->reset();
    }

    return true;
}


//
bool RenderPool::render()
{
    // Update and Render Batches
    for (auto& it : _batch)
    {
        auto& batch = it.second;

        // This needs to be called for every view
        batch->mesh->render(batch->instances->elements());
    }

    return true;
}
