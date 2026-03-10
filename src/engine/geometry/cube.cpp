#include "cube.h"

#include <medusa/graphics/mesh.h>
#include <medusa/graphics/descriptor.h>

#include <core/utilities/logging.h>


using namespace medusa;


std::vector<glm::vec3> cube_position(float w, float h, float d)
{
    std::vector<glm::vec3> position
    {
        // Pos Z - FRONT
        { 1, 1, 1 },{ -1, 1, 1 },{ 1, -1, 1 },{ -1, -1, 1 },
        // Neg X - LEFT
        { -1, 1, 1 },{ -1, 1, -1 },{ -1, -1, 1 },{ -1, -1, -1 },
        // Neg Z - BACK
        { -1, 1, -1 },{ 1, 1, -1 },{ -1, -1, -1 },{ 1, -1, -1 },
        // Pos X - RIGHT
        { 1, 1, -1 },{ 1, 1, 1 },{ 1, -1, -1 },{ 1, -1, 1 },
        // Pos Y - TOP
        { -1, 1, 1 },{ 1, 1, 1 },{ -1, 1, -1 },{ 1, 1, -1 },
        // Neg Y - BOTTOM
        { -1, -1, -1 },{ 1, -1, -1 },{ -1, -1, 1 },{ 1, -1, 1 },
    };

    glm::vec3 size{ w * 0.5f, h * 0.5f, d * 0.5f };

    for (auto& p : position)
        p *= size;

    return position;
}


std::vector<glm::vec3> cube_normals()
{
    std::vector<glm::vec3> normal
    {
        // Pos Z - FRONT
        { 0, 0, 1}, { 0, 0, 1}, { 0, 0, 1}, { 0, 0, 1},
        // Neg X - LEFT
        { -1, 0, 0}, { -1, 0, 0}, { -1, 0, 0}, { -1, 0, 0},
        // Neg Z - BACK
        { 0, 0, -1}, { 0, 0, -1}, { 0, 0, -1}, { 0, 0, -1},
        // Pos X - RIGHT
        {1, 0, 0}, {1, 0, 0}, {1, 0, 0}, {1, 0, 0},
        // Pos Y - TOP
        {0, 1, 0}, {0, 1, 0}, {0, 1, 0}, {0, 1, 0},
        // Neg Y - BOTTOM
        {0, -1, 0}, {0, -1, 0}, {0, -1, 0}, {0, -1, 0},
    };

    return normal;
}


std::vector<glm::vec2> cube_texture()
{
    std::vector<glm::vec2> texture
    {
        {1, 1}, {0, 1}, {1, 0}, {0, 0},
        {1, 1}, {0, 1}, {1, 0}, {0, 0},
        {1, 1}, {0, 1}, {1, 0}, {0, 0},
        {1, 1}, {0, 1}, {1, 0}, {0, 0},
        {1, 1}, {0, 1}, {1, 0}, {0, 0},
        {1, 1}, {0, 1}, {1, 0}, {0, 0},
    };

    return texture;
}


std::vector<uint32_t> cube_indices()
{
    const uint32_t SIDES = 6;
    const uint32_t INDICES_PER_SIDE = 6;
    const uint32_t VERTICES_PER_SIDE = 4;
    const uint32_t INDICES[INDICES_PER_SIDE] = { 0, 1, 2, 2, 1, 3 };

    std::vector<uint32_t> indices(SIDES * INDICES_PER_SIDE);

    uint32_t offset = 0;
    uint32_t* pindices = indices.data();

    for (uint32_t i = 0; i < SIDES; ++i)
    {
        for (uint32_t j = 0; j < INDICES_PER_SIDE; ++j)
            *pindices++ = INDICES[j] + offset;

        offset += VERTICES_PER_SIDE;
    }
    return indices;
}




//
Cube::Cube(std::shared_ptr<IContext> context, float width, float height, float depth)
    : _context(context)
    , _width(width)
    , _height(height)
    , _depth(depth)
{

}


//
Cube::~Cube()
{

}


//
std::shared_ptr<Model> Cube::mesh(uint32_t materialIndex)
{
    Geometry g(_context.lock());

    auto position = cube_position(_width, _height, _depth);
    auto normals = cube_normals();
    auto texture = cube_texture();

    auto indices = cube_indices();

    // Add Vertex Data
    g.addVertexData(position.data(), position.size(), AttributeLocation::Position);
    g.addVertexData(normals.data(), normals.size(), AttributeLocation::Normal);
    g.addVertexData(texture.data(), texture.size(), AttributeLocation::TextureDiffuse);

    // Add Index Data
    g.addIndexData(indices.data(), indices.size());

    return g.mesh(materialIndex);
}
