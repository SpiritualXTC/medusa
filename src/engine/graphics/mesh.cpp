#include "mesh.h"

#include <medusa/engine/context.h>

#include <medusa/graphics.h>

#include <core/utilities/logging.h>


using namespace medusa;


//
Mesh::Mesh(std::shared_ptr<IContext> context)
    : _context(context)
{
    _vertexBuffer = context->create_vertex_buffer();
    _indexBuffer = context->create_index_buffer();
    _descriptor = context->create_descriptor();

    // Bind the Vertex/Index buffer to the descriptor
    _descriptor->bind();
    _vertexBuffer->bind();
    _indexBuffer->bind();
    _descriptor->unbind();

    // Unbind vertex/index buffer
    _vertexBuffer->unbind();
    _indexBuffer->unbind();
}


//
Mesh::~Mesh()
{

}


//
bool Mesh::render(size_t instances)
{
    bool result = true;

    if (!_vertexBuffer)
        throw MedusaError("No vertex data");
    if (!_descriptor)
        throw MedusaError("No descriptor");

    if (_indexBuffer && _indexBuffer->indices())
        _descriptor->render(PrimitiveType::Triangles, _vertexBuffer->vertices(), _indexBuffer->indices(), instances);
    else
        _descriptor->render(PrimitiveType::Triangles, _vertexBuffer->vertices(), 0, instances);

    return result;
}
