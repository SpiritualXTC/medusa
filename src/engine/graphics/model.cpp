#include "model.h"

#include <medusa/graphics/descriptor.h>
#include <medusa/graphics/texture.h>

#include <core/utilities/logging.h>


using namespace medusa;


//
uint64_t Model::addTexture(const std::string& name, std::shared_ptr<ITexture> texture)
{
    uint64_t insertIndex = _textures.size();
    _textures.push_back(texture);

    return texture->handle();
}


//
uint64_t Model::addMaterial(const Material& material)
{
    uint64_t insertIndex = _materials.size();
    _materials.push_back(material);

    return insertIndex;
}


//
uint64_t Model::addModelData(const ModelData& modelData)
{
    uint64_t insertIndex = _modelData.size();
    _modelData.push_back(modelData);
    return insertIndex;
}


/*
//
Model::Model(std::shared_ptr<IDescriptor> descriptor, std::shared_ptr<VertexBuffer> vb, std::shared_ptr<IndexBuffer> ib, std::shared_ptr<GenericArray<Indirect>> submesh)
    : _descriptor(descriptor)
    , _vertices(vb)
    , _indices(ib)
    , _submeshes(submesh)
{

}


//
Model::~Model()
{

}


//
bool Model::render()
{
    if (_submeshes)
    {
        // IMPROVE: This should be a custom loop. Push to the batch for now
        //  NOTE: This overrides the meshes current batch handling.
        //  TODO: The IMesh interface should NOT have responsibility over instancing
        renderBatch(1);
    }
    else
    {
        // Basic Rendering
        if (_indices && _indices->indices())
            _descriptor->render(PrimitiveType::Triangles, _vertices->vertices(), _indices->indices());
        else
            _descriptor->render(PrimitiveType::Triangles, _vertices->vertices(), 0);
    }

    return true;
}


//
bool Model::renderBatch(size_t instances)
{
    if (_submeshes)
    {
        // Update the instance counts... this is kinda terrible but whatever
        if (instances != _cache_instances)
        {
            // TODO: Add a method to "block" gpu syncs, on data. so it can be done in bulk [this is probably doing it in bulk anyway]
            for (uint32_t i = 0; i < _submeshes->size(); ++i)
            {
                _submeshes->data(i).instanceCount = instances;
            }
            _submeshes->sync();

            _cache_instances = instances;
        }

        // Advanced Indirect Instance Rendering
        descriptor()->renderIndirect(PrimitiveType::Triangles, _submeshes);
    }
    else
    {
        // Basic Rendering
        if (_indices && _indices->indices())
            _descriptor->render(PrimitiveType::Triangles, _vertices->vertices(), _indices->indices(), instances);
        else
            _descriptor->render(PrimitiveType::Triangles, _vertices->vertices(), 0, instances);
    }

    return true;
}
*/
