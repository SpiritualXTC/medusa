#include "geometry_buffer.h"

#include <medusa/engine/context.h>
#include <medusa/graphics/containers.h>
#include <medusa/graphics/descriptor.h>

#include <engine/geometry/geometry.h>
#include <engine/graphics/model.h>


using namespace medusa;


//
MeshReference::MeshReference(std::shared_ptr<IContext> context)
{

}


//
MeshReference::~MeshReference()
{

}


//
void MeshReference::pushInstance(size_t transformIndex)
{
    _count++;
    //logging::debug(fmt::format("creating instance: {}", _count));

    _transformIndices.insert(transformIndex);

    return;
}


//
void MeshReference::popInstance(size_t transformIndex)
{
    //logging::debug(fmt::format("deleting instance: {}", _count));
    _count--;

    _transformIndices.erase(transformIndex);

    return;
}


//
GeometryBuffer::GeometryBuffer(std::shared_ptr<IContext> context)
    : _context(context)
{
    // Create global geometry buffers
    _vertices = context->createVertexBuffer(BufferUsage::StaticDraw);
    _indices = context->createIndexBuffer(BufferUsage::StaticDraw);
}


//
GeometryBuffer::~GeometryBuffer()
{


}


//
bool GeometryBuffer::loadMesh(const std::string& name, std::shared_ptr<Model> model)
{
    std::shared_ptr<MeshReference> ref = std::make_shared<MeshReference>(_context.lock());

    auto vb = model->vertexBuffer();
    auto ib = model->indexBuffer();
    auto smb = model->submeshes();

    // Size mismatch
    // TODO: Refactor how a mesh is accessored
    if (vb->stride() != _vertices->stride())
        throw MedusaError("Invalid Stride");

    size_t vertexOffset = _vertices->elements();
    size_t indexOffset = _indices->elements();

    // Copy to global buffer

    // Copy vertices to global buffer
    _vertices->insert(vb->buffer());

    // Indices (Vertex offsets are handled by Indirect rendering)
    _indices->insert(ib->buffer());

    // Subset Data :: Offset the Base Vertex and First Index in the global buffer

    if (smb == nullptr)
    {
        SubMesh submesh;
        submesh.baseVertex = vertexOffset;
        submesh.firstIndex = indexOffset;
        submesh.count = ib->elements();

        _submeshes.push_back(submesh);
        ref->addSubMesh(submesh);
    }
    else
    {
        // TODO: Optimize to push to GPU buffer all at once
        for (auto& subMesh : smb->buffer())
        {
            SubMesh sm;
            sm.baseVertex = subMesh.baseVertex + vertexOffset;
            sm.firstIndex = subMesh.firstIndex + indexOffset;
            sm.count = subMesh.count;

            _submeshes.push_back(sm);
            ref->addSubMesh(sm);
        }
    }

    // Insert the mesh reference
    _refs.insert({ name, ref });

    return true;
}


//
std::shared_ptr<MeshReference> GeometryBuffer::referenceMesh(const std::string& name)
{
    auto& ref = _refs[name];

    return ref;
}


//
std::shared_ptr<IDescriptor> GeometryBuffer::createDescriptor()
{
    // TODO: This should be combine a few "pipeline" related things
    auto context = _context.lock();

    std::shared_ptr<IDescriptor> desc = context->createDescriptor();

    desc->bind();

    _vertices->bind();
    _indices->bind();

    // Add descriptors ... this not a flexible version :)
    desc->addDescription(types::FloatV3, _vertices->stride(), AttributeLocation::Position);
    desc->addDescription(types::FloatV3, _vertices->stride(), AttributeLocation::Normal);
    desc->addDescription(types::FloatV2, _vertices->stride(), AttributeLocation::TextureDiffuse);

    desc->addDescription(types::Int, _vertices->stride(), AttributeLocation::MaterialIndex);

    desc->unbind();

    _vertices->bind();
    _indices->bind();

    return desc;
}
