#include "model.h"

#include <medusa/graphics/descriptor.h>
#include <opengl/gl.h> /// TODO: NOOOOO

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
        descriptor()->bind();
        _submeshes->bind();

        // TODO: This should be moved to the descriptor... as that at least has an OpenGL implementation
        glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, (void*)0, _submeshes->elements(), _submeshes->stride());

        _submeshes->unbind();
        descriptor()->unbind();
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
