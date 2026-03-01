#include "pipeline.h"

#include <medusa/renderer/pass.h>
#include <medusa/graphics/containers.h>


#include <medusa/engine/context.h>

#include <medusa/graphics/shader.h>
#include <medusa/graphics/descriptor.h>
#include <engine/geometry/geometry_buffer.h>

#include <engine/graphics/model.h>

using namespace medusa;


//
Pipeline::Pipeline(std::shared_ptr<IPass> pass, std::shared_ptr<GeometryBuffer> geometryBuffer)
    : _pass(pass), _geometryBuffer(geometryBuffer)
{
    if (!_pass)
        throw MedusaError("Invalid Pass");

    auto context = geometryBuffer->context().lock();

    _instanceMap = context->createArray<uint32_t>(BufferType::Array, BufferUsage::StaticDraw);
    _indirect = context->createArray<Indirect>(BufferType::DrawIndirect, BufferUsage::StaticDraw);

    _descriptor = geometryBuffer->createDescriptor();

    _descriptor->bind();
    _instanceMap->bind();
    _descriptor->addInstanceAttribute(medusa::types::Int, sizeof(uint32_t), InstanceAttributeLocation::TransformIndex);
    _descriptor->unbind();
    _instanceMap->unbind();

}


//
Pipeline::~Pipeline()
{

}


//
bool Pipeline::render()
{
    if (_pass == nullptr)
        return false;


    // Reset the Maps
    _indirect->reset();
    _instanceMap->reset();


    // This is an unoptimised, not quite complete process to renderer "everything" all at once
    size_t instanceOffset = 0;

    for (auto& it : _geometryBuffer->meshRefs())
    {
        std::shared_ptr<MeshReference> meshRef = it.second;
        std::shared_ptr<GenericArray<size_t>> instanceMap = meshRef->instanceMap();

        // Copy the transformIndices to the transform->Instance buffer
        //  This is essentially where visibility "culling" occcurs
        //  This just assumes everything is visible for now ... :)
        //  THIS NEEDS TO BE OPTIMISED -- PROBABLY ON GPU <,<

        size_t instanceCount = 0;
        for (size_t transformIndex : instanceMap->buffer()) // NOTE: This doesn't even accommodate entries that have been deleted :(
        {
            // TODO: Should be able to accommodate the Material SSBO as instance attributes -- not just vertex attributes :D
            _instanceMap->insert(transformIndex);
            instanceCount++;
        }

        // Mesh has no visible instances, so it doesn't require adding
        if (instanceCount == 0)
            continue;

        // Draw all the submeshes
        for (auto& submesh : meshRef->subMeshes())
        {
            Indirect indirect;
            indirect.baseVertex = submesh.baseVertex;
            indirect.count = submesh.count;
            indirect.firstIndex = submesh.firstIndex;

            indirect.baseInstance = instanceOffset;
            indirect.instanceCount = instanceCount;

            _indirect->insert(indirect);
        }

        instanceOffset += instanceCount;
    }

    // Sync teh maps from CPU -> GPU
    // Ideally the above will be handled directly ON the GPU. So no CPU processing/transferring will be required
    _indirect->sync();
    _instanceMap->sync();


    // Render the Pipeline.
    auto& shader = _pass->shader();

    shader->bind();

    // One call to render them all
    _descriptor->renderIndirect(PrimitiveType::Triangles, _indirect);

    shader->unbind();

    return true;
}
