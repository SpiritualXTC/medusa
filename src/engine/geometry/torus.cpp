#include "torus.h"

#include <medusa/engine/context.h>
#include <medusa/graphics.h>

using namespace medusa;


//
Torus::Torus(float majorRadius, float minorRadius,
    uint32_t majorSegments, uint32_t minorSegments)
    : _majorRadius(majorRadius)
    , _minorRadius(minorRadius)
    , _majorSegments(majorSegments)
    , _minorSegments(minorSegments)
{
}


//
Torus::~Torus()
{
}


//
std::shared_ptr<Geometry> Torus::build(std::shared_ptr<IContext> context, const Material& material) const
{
    const uint32_t M = _majorSegments;   // longitude rings
    const uint32_t N = _minorSegments;   // tube cross-section rings
    const float    R = _majorRadius;
    const float    r = _minorRadius;

    // (M+1) * (N+1) vertices — duplicate the seam vertices so UVs wrap cleanly
    const uint32_t vertexCount = (M + 1) * (N + 1);
    const uint32_t indexCount = M * N * 6;

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;
    std::vector<uint32_t>  materialIndices;
    std::vector<uint32_t>  indices;

    positions.reserve(vertexCount);
    normals.reserve(vertexCount);
    uvs.reserve(vertexCount);
    materialIndices.reserve(vertexCount);
    indices.reserve(indexCount);

    // ------------------------------------------------------------------ vertices
    for (uint32_t i = 0; i <= M; ++i)
    {
        // Angle around the main ring (Y axis)
        const float phi = glm::two_pi<float>() * static_cast<float>(i) / static_cast<float>(M);
        const float cosPhi = std::cos(phi);
        const float sinPhi = std::sin(phi);

        for (uint32_t j = 0; j <= N; ++j)
        {
            // Angle around the tube cross-section
            const float theta = glm::two_pi<float>() * static_cast<float>(j) / static_cast<float>(N);
            const float cosTheta = std::cos(theta);
            const float sinTheta = std::sin(theta);

            // Centre of the tube at this longitude step (in the XZ plane)
            const glm::vec3 tubeCenter(R * cosPhi, 0.0f, R * sinPhi);

            // Outward radial direction from the tube centre
            const glm::vec3 radial(cosPhi, 0.0f, sinPhi);

            // Vertex position
            glm::vec3 pos = tubeCenter
                + r * cosTheta * radial
                + r * sinTheta * glm::vec3(0.0f, 1.0f, 0.0f);

            // Normal points away from the tube centreline
            glm::vec3 nrm = glm::normalize(pos - tubeCenter);

            // UV: u wraps around the main ring, v around the tube
            glm::vec2 uv(
                static_cast<float>(i) / static_cast<float>(M),
                static_cast<float>(j) / static_cast<float>(N)
            );

            positions.push_back(pos);
            normals.push_back(nrm);
            uvs.push_back(uv);
            materialIndices.push_back(0u);
        }
    }

    // ------------------------------------------------------------------ indices
    // Each quad (i,j) → two CW triangles when viewed from outside
    for (uint32_t i = 0; i < M; ++i)
    {
        for (uint32_t j = 0; j < N; ++j)
        {
            const uint32_t a = i * (N + 1) + j;
            const uint32_t b = (i + 1) * (N + 1) + j;
            const uint32_t c = (i + 1) * (N + 1) + (j + 1);
            const uint32_t d = i * (N + 1) + (j + 1);

            // Triangle 1 (CCW)
            indices.push_back(a);
            indices.push_back(c);
            indices.push_back(b);

            // Triangle 2 (CCW)
            indices.push_back(a);
            indices.push_back(d);
            indices.push_back(c);
        }
    }

    // ------------------------------------------------------------------ Geometry
    auto geo = std::make_shared<Geometry>(context);

    geo->addVertexData(positions.data(), positions.size(), 1, AttributeLocation::Position);
    geo->addVertexData(normals.data(), normals.size(), 1, AttributeLocation::Normal);
    geo->addVertexData(uvs.data(), uvs.size(), 1, AttributeLocation::TextureDiffuse);
    geo->addVertexData(materialIndices.data(), materialIndices.size(), 1, AttributeLocation::MaterialIndex);

    geo->addIndexData(indices.data(), indices.size());

    geo->addMaterial(material);

    ModelData md;
    md.vertexStart = 0;
    md.indexStart = 0;
    md.indices = static_cast<uint32_t>(indices.size());
    md.vertices = static_cast<uint32_t>(positions.size());
    md.materialIndex = 0;
    geo->addModelData(md);

    return geo;
}
