#include "model.h"

#include <medusa/graphics/descriptor.h>

#include <core/utilities/logging.h>

using namespace medusa;


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
bool Model::render(size_t instances)
{
    if (_submeshes)
    {
        // Advanced Rendering: This should still be cleaned up
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
