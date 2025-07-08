#include "geometry.h"

#include <core/utilities/logging.h>

#include <medusa/engine/context.h>
#include <medusa/graphics.h>
#include <engine/graphics/mesh.h>


using namespace medusa;


/// <summary>
///
/// </summary>
/// <param name="context"></param>
Geometry::Geometry(std::shared_ptr<IContext> context)
    : _context(context)
{

}


/// <summary>
///
/// </summary>
Geometry::~Geometry()
{

}


/// <summary>
///
/// </summary>
/// <param name="data"></param>
/// <param name="dt"></param>
/// <param name="count"></param>
/// <param name="location"></param>
void Geometry::addVertexData(void* data, DType& dt, size_t count, AttributeLocation location)
{
    // TODO: Add an optional indexing method for duplicating the vertices... tho it makes the overall indexing harder
    //  This is how .obj files are stored anyway

    // TODO: An additional stride (default = 0), so extra data can be bundled in, and removed. Default behaviour is current. Just full copy.

    // Calculate vector size in bytes, and copy buffer
    size_t size = count * dt.size();
    std::vector<uint8_t> vector((uint8_t*)data, (uint8_t*)data + size);

    GeometryData g{ dt, count, std::move(vector), location};

    // Maximum length of the array
    if (g.count >= _vertices)
        _vertices = g.count;

    // Update Total Stride
    _stride += dt.size();

    // Add to geometry
    _geometry.push_back(g);
}


/// <summary>
///
/// </summary>
/// <param name="data"></param>
/// <param name="count"></param>
void Geometry::addIndexData(uint32_t* data, size_t count)
{
    // Mesh Subsets could "PROBABLY" be handled here... but this could get confusing. So for now just clear and set
    _indices.clear();

    // Precreate
    _indices.resize(count);
    for (uint32_t i = 0; i < count; ++i)
        _indices[i] = data[i];
}


/// <summary>
/// Interleave implementation
/// </summary>
/// <param name="buffer"></param>
/// <param name="stride"></param>
void Geometry::interleave(uint8_t* buffer, size_t stride)
{
    size_t offset = 0;
    size_t bytes = _vertices * _stride;

    if (stride == 0)
        stride = _stride;

    if (stride < _stride)
        throw MedusaError(fmt::format("Invalid stride, requires minimum of {} bytes", _stride));


    for (auto& g : _geometry)
    {
        // Pointer to data to interleave
        uint8_t* ptr = &g.data[0];

        // Pointer to interleaved data
        uint8_t* pdata = &buffer[offset];

        // Loop through geometry vertices
        for (uint32_t idx = 0; idx < g.count; ++idx)
        {
            // Copy each vertex
            memcpy((void*)pdata, (void*)ptr, g.dtype.size());

            // Update Pointers
            ptr += g.dtype.size();
            pdata += stride;
        }

        // Increase starting offset
        offset += g.dtype.size();
    }

    return;
}


/// <summary>
///
/// </summary>
/// <returns></returns>
std::shared_ptr<IMesh> Geometry::mesh(std::shared_ptr<IMesh> meshIn)
{
    auto context = _context.lock();

    auto mesh = meshIn == nullptr ? context->create_mesh() : meshIn;

    auto vb = mesh->vertexBuffer();
    auto ib = mesh->indexBuffer();
    auto desc = mesh->descriptor();

    size_t size = _stride * _vertices;

    // Interleave vertices
    std::vector<uint8_t> buffer(size);
    interleave(buffer.data(), _stride);

    // Create Buffers
    vb->create(buffer.data(), _vertices, _stride);
    if (_indices.size())
        ib->create(_indices.data(), _indices.size());

    // Bind Buffers to Descriptors
    desc->bind();
    vb->bind();

    if (_indices.size())
        ib->bind();

    // Bind Descriptors
    for (auto& g : _geometry)
        desc->addDescription(g.dtype, _stride, g.location);

    desc->unbind();

    // VertexBuffer requires staying bound while the descriptors are setup
    if (_indices.size())
        ib->unbind();
    vb->unbind();

    return mesh;
}
