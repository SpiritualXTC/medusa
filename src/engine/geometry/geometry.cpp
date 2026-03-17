#include "geometry.h"

#include <core/utilities/logging.h>

#include <medusa/engine/context.h>
#include <medusa/graphics.h>


using namespace medusa;


//
Geometry::Geometry(std::shared_ptr<IContext> context)
    : _context(context)
{

}


//
Geometry::~Geometry()
{

}


//
void Geometry::addVertexData(void* data, DType& dt, size_t count, AttributeLocation location)
{
    // TODO: Add an optional indexing method for duplicating the vertices... tho it makes the overall indexing harder
    //  This is how .obj files are stored anyway

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

    _geometryData.insert({ location, g });
}


//
void Geometry::addIndexData(uint32_t* data, size_t count)
{
    // Mesh Subsets could "PROBABLY" be handled here... but this could get confusing. So for now just clear and set
    _indices.clear();

    // Precreate
    _indices.resize(count);
    for (uint32_t i = 0; i < count; ++i)
        _indices[i] = data[i];
}


//
void Geometry::interleave(uint8_t* buffer, size_t stride)
{
    // TODO: Implement, interleaving SUBDATA, if specified. Default will be all

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


//
uint64_t Geometry::addTexture(const std::string& name, std::shared_ptr<ITexture> texture)
{
    uint64_t insertIndex = _textures.size();
    _textures.push_back(texture);

    return texture->handle();
}


//
uint64_t Geometry::addMaterial(const Material& material)
{
    uint64_t insertIndex = _materials.size();
    _materials.push_back(material);

    return insertIndex;
}


//
uint64_t Geometry::addModelData(const ModelData& modelData)
{
    uint64_t insertIndex = _modelData.size();
    _modelData.push_back(modelData);
    return insertIndex;
}
