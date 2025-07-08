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


void RenderPool::create_mesh_instance(std::shared_ptr<IMesh> mesh)
{
    auto descriptor = mesh->descriptor();
    auto instanceBuffer = _context->create_instance_buffer();

    // Bind Instance Buffer to Descriptor
    descriptor->bind();
    instanceBuffer->bind();


    for (auto& id : _data)
    {
        // TODO: Shouldn't be part of the descriptor...
        descriptor->addInstanceAttribute(id.data_type, sizeof(Payload), id.location);
    }

    // Unbind Descriptor
    descriptor->unbind();

    // Unbind Instance Buffer
    instanceBuffer->unbind();

    std::shared_ptr<Batch> batch = std::make_shared<Batch>(mesh, instanceBuffer, std::vector<Payload>({}));

    _pool.insert({ mesh->id(), mesh });
    _batch.insert({ mesh->id(), batch }); // DIS ONE

    batch->buffer->create(nullptr, 10000, sizeof(Payload));


    batch->items.resize(10000);

    return;
}


//
bool RenderPool::add(std::shared_ptr<IRenderable> renderable)
{
    //logging::info(fmt::format("Pool: Add id={}", renderable->resource_id()));

    auto it = _batch.find(renderable->resource_id());

    if (it == _batch.end())
        return false;

    auto& batch = it->second;

    Payload pl{ renderable->transform() };


    batch->items[batch->count] = pl;
    batch->count++;

    return true;
}


//
bool RenderPool::clear()
{
    for (auto& it : _batch)
    {
        auto& batch = it.second;
        batch->count = 0;
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
        batch->buffer->update(batch->items.data(), 0, batch->count);

        // This needs to be called for every view
        batch->mesh->render(batch->count);
    }

    return true;
}
