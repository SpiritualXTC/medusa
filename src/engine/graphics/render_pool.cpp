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
bool RenderPool::render(std::shared_ptr<IPass> pass)
{
    // TODO Register the World Matrix, and pass it through to the shaders as an instanced variable
    //  Each instance needs to append it's matrix into the instance buffer
    //  The mesh "PAYLOAD" also needs to be included as texturing is currently not working as it's binding is handled by the mesh component.
    //  Can textures be included in the descriptor binding? that would be convenient :)


    // Update and Render Batches
    for (auto& it : _batch)
    {
        auto& batch = it.second;

        // TODO: Try and do instance buffer update via OpenCL or OpenGL Compute Shader
        // TODO: This will occur for EVERY view :(
        //batch->buffer->update(batch->items.data(), 0, batch->count);

        // This needs to be called for every view
        batch->mesh->render(batch->instances->elements());
    }

    return true;
}
