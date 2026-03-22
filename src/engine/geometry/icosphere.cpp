#include "icosphere.h"

#include <medusa/engine/context.h>
#include <medusa/graphics.h>

#include <unordered_map>
#include <cmath>

using namespace medusa;


Icosphere::Icosphere(float radius, uint32_t subdivisions)
    : _radius(radius)
    , _subdivisions(subdivisions)
{
}


Icosphere::~Icosphere()
{
}


// -------------------------------------------------------------------------
// Internal helpers (file-local)
// -------------------------------------------------------------------------

namespace
{
    // Returns the midpoint of two vertices projected onto the unit sphere.
    // Results are cached by an edge key so each unique edge produces exactly
    // one midpoint vertex, keeping the mesh manifold.
    static uint32_t midpoint(uint32_t a, uint32_t b,
        std::vector<glm::vec3>& verts,
        std::unordered_map<uint64_t, uint32_t>& cache)
    {
        // Order-independent edge key
        uint64_t lo = static_cast<uint64_t>(std::min(a, b));
        uint64_t hi = static_cast<uint64_t>(std::max(a, b));
        uint64_t key = (lo << 32u) | hi;

        auto it = cache.find(key);
        if (it != cache.end())
            return it->second;

        glm::vec3 mid = glm::normalize(verts[a] + verts[b]);
        uint32_t idx = static_cast<uint32_t>(verts.size());
        verts.push_back(mid);
        cache.insert({ key, idx });

        return idx;
    }


    // Spherical UV mapping (equirectangular).
    static glm::vec2 sphericalUV(const glm::vec3& n)
    {
        float u = 0.5f + std::atan2(n.z, n.x) / glm::two_pi<float>();
        float v = 0.5f - std::asin(glm::clamp(n.y, -1.0f, 1.0f)) / glm::pi<float>();
        return { u, v };
    }


    // Returns true when a triangle straddles the antimeridian seam (U wraps
    // from ~1 back to ~0 across one of its edges). Detected by checking whether
    // any pair of U coordinates differs by more than 0.5, which cannot happen on
    // a legitimately small triangle but always occurs on a seam-crossing one.
    static bool crossesSeam(const glm::vec2& uvA, const glm::vec2& uvB, const glm::vec2& uvC)
    {
        auto crossesPair = [](float u0, float u1) { return std::abs(u0 - u1) > 0.5f; };
        return crossesPair(uvA.x, uvB.x)
            || crossesPair(uvB.x, uvC.x)
            || crossesPair(uvA.x, uvC.x);
    }


    // Wraps a U coordinate to the "high" side (>= 0.5) for seam-duplicate vertices.
    static float wrapHigh(float u) { return u < 0.5f ? u + 1.0f : u; }


    // Appends a duplicate of vertex srcIdx with a corrected U coordinate and
    // returns the index of the new vertex.
    static uint32_t duplicateSeamVertex(uint32_t srcIdx,
        std::vector<glm::vec3>& positions,
        std::vector<glm::vec3>& normals,
        std::vector<glm::vec2>& uvs,
        std::vector<uint32_t>& materialIndices)
    {
        uint32_t newIdx = static_cast<uint32_t>(positions.size());
        positions.push_back(positions[srcIdx]);
        normals.push_back(normals[srcIdx]);
        glm::vec2 fixed = uvs[srcIdx];
        fixed.x = wrapHigh(fixed.x);
        uvs.push_back(fixed);
        materialIndices.push_back(materialIndices[srcIdx]);
        return newIdx;
    }
}


// -------------------------------------------------------------------------

std::shared_ptr<Geometry> Icosphere::build(const Material& material) const
{
    // ------------------------------------------------------------------ base icosahedron vertices
    // Golden ratio
    const float t = (1.0f + std::sqrt(5.0f)) / 2.0f;

    std::vector<glm::vec3> verts;
    verts.reserve(12);

    // Three mutually perpendicular rectangles whose corners sit on the unit sphere
    verts.push_back(glm::normalize(glm::vec3(-1.0f, t, 0.0f)));
    verts.push_back(glm::normalize(glm::vec3(1.0f, t, 0.0f)));
    verts.push_back(glm::normalize(glm::vec3(-1.0f, -t, 0.0f)));
    verts.push_back(glm::normalize(glm::vec3(1.0f, -t, 0.0f)));

    verts.push_back(glm::normalize(glm::vec3(0.0f, -1.0f, t)));
    verts.push_back(glm::normalize(glm::vec3(0.0f, 1.0f, t)));
    verts.push_back(glm::normalize(glm::vec3(0.0f, -1.0f, -t)));
    verts.push_back(glm::normalize(glm::vec3(0.0f, 1.0f, -t)));

    verts.push_back(glm::normalize(glm::vec3(t, 0.0f, -1.0f)));
    verts.push_back(glm::normalize(glm::vec3(t, 0.0f, 1.0f)));
    verts.push_back(glm::normalize(glm::vec3(-t, 0.0f, -1.0f)));
    verts.push_back(glm::normalize(glm::vec3(-t, 0.0f, 1.0f)));

    // ------------------------------------------------------------------ base icosahedron faces (CCW)
    struct Tri { uint32_t a, b, c; };

    std::vector<Tri> faces;
    faces.reserve(20);

    // 5 faces around vertex 0
    faces.push_back({ 0, 11,  5 });
    faces.push_back({ 0,  5,  1 });
    faces.push_back({ 0,  1,  7 });
    faces.push_back({ 0,  7, 10 });
    faces.push_back({ 0, 10, 11 });

    // 5 adjacent faces
    faces.push_back({ 1,  5, 9 });
    faces.push_back({ 5, 11, 4 });
    faces.push_back({ 11, 10, 2 });
    faces.push_back({ 10,  7, 6 });
    faces.push_back({ 7,  1, 8 });

    // 5 faces around vertex 3
    faces.push_back({ 3, 9, 4 });
    faces.push_back({ 3, 4, 2 });
    faces.push_back({ 3, 2, 6 });
    faces.push_back({ 3, 6, 8 });
    faces.push_back({ 3, 8, 9 });

    // 5 adjacent faces
    faces.push_back({ 4, 9,  5 });
    faces.push_back({ 2, 4, 11 });
    faces.push_back({ 6, 2, 10 });
    faces.push_back({ 8, 6,  7 });
    faces.push_back({ 9, 8,  1 });

    // ------------------------------------------------------------------ subdivision
    for (uint32_t pass = 0; pass < _subdivisions; ++pass)
    {
        std::unordered_map<uint64_t, uint32_t> cache;
        std::vector<Tri> refined;
        refined.reserve(faces.size() * 4);

        for (const Tri& tri : faces)
        {
            uint32_t ab = midpoint(tri.a, tri.b, verts, cache);
            uint32_t bc = midpoint(tri.b, tri.c, verts, cache);
            uint32_t ca = midpoint(tri.c, tri.a, verts, cache);

            // Each original triangle becomes 4 (CCW winding preserved)
            refined.push_back({ tri.a, ab, ca });
            refined.push_back({ tri.b, bc, ab });
            refined.push_back({ tri.c, ca, bc });
            refined.push_back({ ab, bc, ca });
        }

        faces = std::move(refined);
    }

    // ------------------------------------------------------------------ build final arrays
    const float R = _radius;

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;
    std::vector<uint32_t>  materialIndices;
    std::vector<uint32_t>  indexBuffer;

    positions.reserve(verts.size());
    normals.reserve(verts.size());
    uvs.reserve(verts.size());
    materialIndices.reserve(verts.size());
    indexBuffer.reserve(faces.size() * 3);

    // Build base vertex arrays from the subdivided sphere positions
    for (const glm::vec3& n : verts)
    {
        positions.push_back(n * R);
        normals.push_back(n);
        uvs.push_back(sphericalUV(n));
        materialIndices.push_back(0u);
    }

    // Seam fix: per-triangle, duplicate any vertex whose U sits on the wrong
    // side of the antimeridian, then emit corrected indices.
    //
    // A cache maps (original index) -> duplicate index so vertices shared by
    // multiple seam triangles are only duplicated once.
    std::unordered_map<uint32_t, uint32_t> seamCache;

    for (const Tri& tri : faces)
    {
        const glm::vec2& uvA = uvs[tri.a];
        const glm::vec2& uvB = uvs[tri.b];
        const glm::vec2& uvC = uvs[tri.c];

        if (!crossesSeam(uvA, uvB, uvC))
        {
            indexBuffer.push_back(tri.a);
            indexBuffer.push_back(tri.b);
            indexBuffer.push_back(tri.c);
            continue;
        }

        // For each corner: if its U is on the low side (< 0.5) it needs a
        // duplicate with U wrapped to the high side.
        auto resolveSeamVertex = [&](uint32_t idx) -> uint32_t
            {
                if (uvs[idx].x >= 0.5f)
                    return idx;  // already on the correct side

                auto it = seamCache.find(idx);
                if (it != seamCache.end())
                    return it->second;

                uint32_t dup = duplicateSeamVertex(idx, positions, normals, uvs, materialIndices);
                seamCache.insert({ idx, dup });
                return dup;
            };

        indexBuffer.push_back(resolveSeamVertex(tri.a));
        indexBuffer.push_back(resolveSeamVertex(tri.b));
        indexBuffer.push_back(resolveSeamVertex(tri.c));
    }

    // ------------------------------------------------------------------ populate Geometry
    auto geo = std::make_shared<Geometry>();

    geo->addVertexData(positions.data(), positions.size(), 1, AttributeLocation::Position);
    geo->addVertexData(normals.data(), normals.size(), 1, AttributeLocation::Normal);
    geo->addVertexData(uvs.data(), uvs.size(), 1, AttributeLocation::TextureDiffuse);
    geo->addVertexData(materialIndices.data(), materialIndices.size(), 1, AttributeLocation::MaterialIndex);

    geo->addIndexData(indexBuffer.data(), indexBuffer.size());

    geo->addMaterial(material);

    ModelData md;
    md.vertexStart = 0;
    md.indexStart = 0;
    md.indices = static_cast<uint32_t>(indexBuffer.size());
    md.vertices = static_cast<uint32_t>(positions.size());
    md.materialIndex = 0;
    geo->addModelData(md);

    return geo;
}
