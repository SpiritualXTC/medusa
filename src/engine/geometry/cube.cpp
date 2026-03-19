#include "cube.h"

#include <engine/geometry/geometry.h>


using namespace medusa;


static std::vector<glm::vec3> cube_positions(float w, float h, float d)
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


static std::vector<glm::vec3> cube_normals()
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


static std::vector<glm::vec2> cube_texture_coords()
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


static std::vector<uint32_t> cube_indices()
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
Cube::Cube(float width, float height, float depth)
    : _width(width)
    , _height(height)
    , _depth(depth)
{

}


//
Cube::~Cube()
{

}


//
std::shared_ptr<Geometry> Cube::build(std::shared_ptr<IContext> context, const Material& material) const
{
    auto model = std::make_shared<Geometry>(context);

    auto positions = cube_positions(_width, _height, _depth);
    auto normals = cube_normals();
    auto uvs = cube_texture_coords();
    auto indices = cube_indices();

    const size_t vertexCount = positions.size(); // 24

    model->addVertexData(positions.data(), vertexCount, AttributeLocation::Position);
    model->addVertexData(normals.data(), vertexCount, AttributeLocation::Normal);
    model->addVertexData(uvs.data(), vertexCount, AttributeLocation::TextureDiffuse);

    // All vertices reference material slot 0
    std::vector<uint32_t> materialIndices(vertexCount, 0);
    model->addVertexData(materialIndices.data(), vertexCount, 1, AttributeLocation::MaterialIndex);

    model->addIndexData(indices.data(), indices.size());

    // Register the material - slot 0 matches the indices written above
    model->addMaterial(material);

    // Register the single submesh covering all 36 indices from vertex 0
    ModelData submesh{};
    submesh.vertexStart = 0;
    submesh.indexStart = 0;
    submesh.indices = static_cast<uint32_t>(indices.size());
    submesh.vertices = static_cast<uint32_t>(vertexCount);
    submesh.materialIndex = 0;
    model->addModelData(submesh);

    return model;
}
