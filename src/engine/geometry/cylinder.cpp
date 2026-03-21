#include "cylinder.h"

#include <medusa/engine/context.h>
#include <medusa/graphics.h>

#include <cmath>

using namespace medusa;


// =============================================================================
// Shared helpers (file-local)
// =============================================================================

namespace
{
    struct MeshBuffers
    {
        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> uvs;
        std::vector<uint32_t>  materialIndices;
        std::vector<uint32_t>  indices;
    };


    // Append a CCW quad (a, b, c, d) as two triangles into the index buffer.
    // Vertices should be wound so that a->b->c->d is CCW when viewed from outside.
    inline void pushQuad(std::vector<uint32_t>& idx, uint32_t a, uint32_t b, uint32_t c, uint32_t d)
    {
        idx.push_back(a); idx.push_back(c); idx.push_back(b);
        idx.push_back(a); idx.push_back(d); idx.push_back(c);
    }


    // Append a CCW triangle.
    inline void pushTri(std::vector<uint32_t>& idx, uint32_t a, uint32_t b, uint32_t c)
    {
        idx.push_back(a); idx.push_back(b); idx.push_back(c);
    }


    // Append a single vertex, returning its index.
    inline uint32_t pushVertex(MeshBuffers& mb,
                                const glm::vec3& pos,
                                const glm::vec3& nrm,
                                const glm::vec2& uv)
    {
        uint32_t idx = static_cast<uint32_t>(mb.positions.size());
        mb.positions.push_back(pos);
        mb.normals.push_back(nrm);
        mb.uvs.push_back(uv);
        mb.materialIndices.push_back(0u);
        return idx;
    }


    // -------------------------------------------------------------------------
    // Side wall ring: (segments+1) vertices at a given Y, with outward normals.
    // Returns the index of the first vertex in the ring.
    // -------------------------------------------------------------------------
    static uint32_t pushSideRing(MeshBuffers& mb,
                                  float radius, float y, float vCoord,
                                  uint32_t segments)
    {
        uint32_t first = static_cast<uint32_t>(mb.positions.size());

        for (uint32_t i = 0; i <= segments; ++i)
        {
            float theta    = glm::two_pi<float>() * static_cast<float>(i) / static_cast<float>(segments);
            float cosTheta = std::cos(theta);
            float sinTheta = std::sin(theta);

            glm::vec3 pos(radius * cosTheta, y, radius * sinTheta);
            glm::vec3 nrm(cosTheta, 0.0f, sinTheta);
            glm::vec2 uv(static_cast<float>(i) / static_cast<float>(segments), vCoord);

            pushVertex(mb, pos, nrm, uv);
        }

        return first;
    }


    // -------------------------------------------------------------------------
    // Inner wall ring: same as above but normals point inward.
    // -------------------------------------------------------------------------
    static uint32_t pushInnerRing(MeshBuffers& mb,
                                   float radius, float y, float vCoord,
                                   uint32_t segments)
    {
        uint32_t first = static_cast<uint32_t>(mb.positions.size());

        for (uint32_t i = 0; i <= segments; ++i)
        {
            float theta    = glm::two_pi<float>() * static_cast<float>(i) / static_cast<float>(segments);
            float cosTheta = std::cos(theta);
            float sinTheta = std::sin(theta);

            glm::vec3 pos(radius * cosTheta, y, radius * sinTheta);
            glm::vec3 nrm(-cosTheta, 0.0f, -sinTheta);
            glm::vec2 uv(static_cast<float>(i) / static_cast<float>(segments), vCoord);

            pushVertex(mb, pos, nrm, uv);
        }

        return first;
    }


    // -------------------------------------------------------------------------
    // Flat cap disc (top or bottom). Uses a flat normal pointing up or down.
    // The cap centre is a single vertex; the rim is a ring of (segments+1).
    // -------------------------------------------------------------------------
    static void pushCap(MeshBuffers& mb,
                         float radius, float y, bool top,
                         uint32_t segments)
    {
        glm::vec3 capNormal(0.0f, top ? 1.0f : -1.0f, 0.0f);

        uint32_t centreIdx = pushVertex(mb,
            glm::vec3(0.0f, y, 0.0f),
            capNormal,
            glm::vec2(0.5f, 0.5f));

        uint32_t rimStart = static_cast<uint32_t>(mb.positions.size());

        for (uint32_t i = 0; i <= segments; ++i)
        {
            float theta    = glm::two_pi<float>() * static_cast<float>(i) / static_cast<float>(segments);
            float cosTheta = std::cos(theta);
            float sinTheta = std::sin(theta);

            glm::vec3 pos(radius * cosTheta, y, radius * sinTheta);
            glm::vec2 uv(cosTheta * 0.5f + 0.5f, sinTheta * 0.5f + 0.5f);

            pushVertex(mb, pos, capNormal, uv);
        }

        // Fan triangles. Top cap is CCW when viewed from above (+Y),
        // bottom cap is CCW when viewed from below (-Y).
        for (uint32_t i = 0; i < segments; ++i)
        {
            uint32_t curr = rimStart + i;
            uint32_t next = rimStart + i + 1;

            if (top)
                pushTri(mb.indices, centreIdx, next, curr);
            else
                pushTri(mb.indices, centreIdx, curr, next);
        }
    }


    // -------------------------------------------------------------------------
    // Flat annular cap (top or bottom) for the Tube variant.
    // Outer rim is at outerRadius, inner rim at innerRadius, both with flat normals.
    // -------------------------------------------------------------------------
    static void pushAnnularCap(MeshBuffers& mb,
                                 float outerRadius, float innerRadius, float y, bool top,
                                 uint32_t segments)
    {
        glm::vec3 capNormal(0.0f, top ? 1.0f : -1.0f, 0.0f);

        uint32_t outerStart = static_cast<uint32_t>(mb.positions.size());

        for (uint32_t i = 0; i <= segments; ++i)
        {
            float theta    = glm::two_pi<float>() * static_cast<float>(i) / static_cast<float>(segments);
            float cosTheta = std::cos(theta);
            float sinTheta = std::sin(theta);

            // UV: radially lerped from inner (0.5 centre) to outer (edge at 1.0)
            auto radialUV = [&](float r)
            {
                float ur = r / outerRadius;    // normalise to outer radius
                return glm::vec2(cosTheta * ur * 0.5f + 0.5f, sinTheta * ur * 0.5f + 0.5f);
            };

            pushVertex(mb,
                glm::vec3(outerRadius * cosTheta, y, outerRadius * sinTheta),
                capNormal, radialUV(outerRadius));

            pushVertex(mb,
                glm::vec3(innerRadius * cosTheta, y, innerRadius * sinTheta),
                capNormal, radialUV(innerRadius));
        }

        // Each step produces one quad: outer[i], outer[i+1], inner[i+1], inner[i]
        // Stride is 2 (outer, inner alternating).
        for (uint32_t i = 0; i < segments; ++i)
        {
            uint32_t oa = outerStart + i * 2;
            uint32_t ob = outerStart + (i + 1) * 2;
            uint32_t ib = outerStart + (i + 1) * 2 + 1;
            uint32_t ia = outerStart + i * 2 + 1;

            if (top)
                pushQuad(mb.indices, oa, ob, ib, ia);
            else
                pushQuad(mb.indices, ob, oa, ia, ib);
        }
    }

} // anonymous namespace


// =============================================================================
// Cylinder
// =============================================================================

Cylinder::Cylinder(float radius, float height, uint32_t segments, uint32_t stacks)
    : _radius(radius)
    , _height(height)
    , _segments(segments)
    , _stacks(stacks)
{
}


Cylinder::~Cylinder()
{
}


std::shared_ptr<Geometry> Cylinder::build(std::shared_ptr<IContext> context,
                                           const Material& material) const
{
    const uint32_t S   = std::max(_segments, 3u);
    const uint32_t ST  = std::max(_stacks, 1u);
    const float    R   = _radius;
    const float    H   = _height;
    const float    bot = -H * 0.5f;
    const float    top =  H * 0.5f;

    MeshBuffers mb;

    // ------------------------------------------------------------------ side wall
    // Generate (stacks+1) rings and connect them with quads.
    std::vector<uint32_t> ringStarts;
    ringStarts.reserve(ST + 1);

    for (uint32_t s = 0; s <= ST; ++s)
    {
        float t      = static_cast<float>(s) / static_cast<float>(ST);
        float y      = bot + t * H;
        float vCoord = 1.0f - t;  // v=1 at bottom, v=0 at top

        ringStarts.push_back(pushSideRing(mb, R, y, vCoord, S));
    }

    const uint32_t stride = S + 1;

    for (uint32_t s = 0; s < ST; ++s)
    {
        uint32_t lo = ringStarts[s];
        uint32_t hi = ringStarts[s + 1];

        for (uint32_t i = 0; i < S; ++i)
        {
            // lo ring is below hi ring; CCW from outside
            pushQuad(mb.indices,
                lo + i, lo + i + 1,
                hi + i + 1, hi + i);
        }
    }

    // ------------------------------------------------------------------ caps
    pushCap(mb, R, bot, false, S);
    pushCap(mb, R, top, true,  S);

    // ------------------------------------------------------------------ populate Geometry
    auto geo = std::make_shared<Geometry>(context);

    geo->addVertexData(mb.positions.data(),       mb.positions.size(),       1, AttributeLocation::Position);
    geo->addVertexData(mb.normals.data(),         mb.normals.size(),         1, AttributeLocation::Normal);
    geo->addVertexData(mb.uvs.data(),             mb.uvs.size(),             1, AttributeLocation::TextureDiffuse);
    geo->addVertexData(mb.materialIndices.data(), mb.materialIndices.size(), 1, AttributeLocation::MaterialIndex);

    geo->addIndexData(mb.indices.data(), mb.indices.size());

    geo->addMaterial(material);

    ModelData md;
    md.vertexStart   = 0;
    md.indexStart    = 0;
    md.indices       = static_cast<uint32_t>(mb.indices.size());
    md.vertices      = static_cast<uint32_t>(mb.positions.size());
    md.materialIndex = 0;
    geo->addModelData(md);

    return geo;
}


// =============================================================================
// Tube
// =============================================================================

Tube::Tube(float outerRadius, float innerRadius, float height,
           uint32_t segments, uint32_t stacks)
    : Cylinder(outerRadius, height, segments, stacks)
    , _innerRadius(innerRadius)
{
}


Tube::~Tube()
{
}


std::shared_ptr<Geometry> Tube::build(std::shared_ptr<IContext> context,
                                       const Material& material) const
{
    const uint32_t S    = std::max(_segments, 3u);
    const uint32_t ST   = std::max(_stacks, 1u);
    const float    Ro   = _radius;
    const float    Ri   = std::min(_innerRadius, Ro - 1e-4f);  // guard degenerate
    const float    H    = _height;
    const float    bot  = -H * 0.5f;
    const float    top  =  H * 0.5f;

    MeshBuffers mb;

    // ------------------------------------------------------------------ outer wall
    {
        std::vector<uint32_t> ringStarts;
        ringStarts.reserve(ST + 1);

        for (uint32_t s = 0; s <= ST; ++s)
        {
            float t      = static_cast<float>(s) / static_cast<float>(ST);
            float y      = bot + t * H;
            float vCoord = 1.0f - t;

            ringStarts.push_back(pushSideRing(mb, Ro, y, vCoord, S));
        }

        for (uint32_t s = 0; s < ST; ++s)
        {
            uint32_t lo = ringStarts[s];
            uint32_t hi = ringStarts[s + 1];

            for (uint32_t i = 0; i < S; ++i)
            {
                pushQuad(mb.indices,
                    lo + i, lo + i + 1,
                    hi + i + 1, hi + i);
            }
        }
    }

    // ------------------------------------------------------------------ inner wall
    // Inner wall normals point inward; winding is reversed so CCW is still
    // correct when viewed from inside the tube.
    {
        std::vector<uint32_t> ringStarts;
        ringStarts.reserve(ST + 1);

        for (uint32_t s = 0; s <= ST; ++s)
        {
            float t      = static_cast<float>(s) / static_cast<float>(ST);
            float y      = bot + t * H;
            float vCoord = 1.0f - t;

            ringStarts.push_back(pushInnerRing(mb, Ri, y, vCoord, S));
        }

        for (uint32_t s = 0; s < ST; ++s)
        {
            uint32_t lo = ringStarts[s];
            uint32_t hi = ringStarts[s + 1];

            for (uint32_t i = 0; i < S; ++i)
            {
                // Reversed quad order so inner face is CCW from inside
                pushQuad(mb.indices,
                    lo + i + 1, lo + i,
                    hi + i, hi + i + 1);
            }
        }
    }

    // ------------------------------------------------------------------ annular caps
    pushAnnularCap(mb, Ro, Ri, bot, false, S);
    pushAnnularCap(mb, Ro, Ri, top, true,  S);

    // ------------------------------------------------------------------ populate Geometry
    auto geo = std::make_shared<Geometry>(context);

    geo->addVertexData(mb.positions.data(),       mb.positions.size(),       1, AttributeLocation::Position);
    geo->addVertexData(mb.normals.data(),         mb.normals.size(),         1, AttributeLocation::Normal);
    geo->addVertexData(mb.uvs.data(),             mb.uvs.size(),             1, AttributeLocation::TextureDiffuse);
    geo->addVertexData(mb.materialIndices.data(), mb.materialIndices.size(), 1, AttributeLocation::MaterialIndex);

    geo->addIndexData(mb.indices.data(), mb.indices.size());

    geo->addMaterial(material);

    ModelData md;
    md.vertexStart   = 0;
    md.indexStart    = 0;
    md.indices       = static_cast<uint32_t>(mb.indices.size());
    md.vertices      = static_cast<uint32_t>(mb.positions.size());
    md.materialIndex = 0;
    geo->addModelData(md);

    return geo;
}
