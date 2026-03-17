#include "geometry_buffer.h"

#include <medusa/engine/context.h>
#include <medusa/graphics/containers.h>
#include <medusa/graphics.h>

#include <engine/geometry/geometry.h>

#include <engine/resources/texture_manager.h>


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
bool GeometryBuffer::loadMesh(const std::string& name, std::shared_ptr<Geometry> model, std::shared_ptr<GenericMap<Material>> materials, std::shared_ptr<TextureManager> textures)
{
    std::shared_ptr<MeshReference> ref = std::make_shared<MeshReference>(_context.lock());

    // TODO: The interleave here, needs to use ONLY what the geometry expects.
    std::vector<Vertex> vertices = model->interleave<Vertex>();
    std::vector<uint32_t>& indices = model->getIndices();
    auto& modelData = model->getModelData();
    auto& textureData = model->getTextures();
    auto& materialData = model->getMaterials();

    // Get the Offsets
    size_t vertexOffset = _vertices->elements();
    size_t indexOffset = _indices->elements();
    size_t materialsOffset = materials->elements(); // MaterialBuffer needs to be here :(

    // Size mismatch
    if (model->stride() != _vertices->stride())
        throw MedusaError("Invalid Stride");

    // Update Material Index (Dis is a hacky mc hack)
    for (auto& v : vertices)
        v.material += materialsOffset;

    // Add textures
    for (auto& tex : textureData)
    {
        // TODO: Needs a name
        textures->addTexture(std::format("tex_{}", tex->handle()), tex);
    }

    // Copy Materials to Material Buffer [Name Hack.... does the material really need to be a map?]
    uint32_t idx = 0;
    for (auto& m : materialData)
    {
        // TODO: Needs a name
        materials->insert(fmt::format("mesh_{}", idx++), m);
    }

    // Copy vertices to global buffer
    _vertices->insert(vertices);

    // Indices (Vertex offsets are handled by Indirect rendering)
    _indices->insert(indices);

    // TODO: Optimize to push to GPU buffer all at once
    for (auto& subMesh : modelData)
    {
        SubMesh sm;
        sm.baseVertex = subMesh.vertexStart + vertexOffset;
        sm.firstIndex = subMesh.indexStart + indexOffset;
        sm.count = subMesh.indices;

        _submeshes.push_back(sm);
        ref->addSubMesh(sm);
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
