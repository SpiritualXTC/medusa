#pragma once

#include <medusa/engine_fwd.h>
#include <medusa/math.h>

#include "geometry.h"

namespace medusa
{
    /// <summary>
    /// Builds an icosphere geometry by subdividing a base icosahedron.
    ///
    /// Each subdivision level splits every triangle into 4, so the face count is:
    ///   20 * 4^subdivisions
    ///
    /// Vertices are projected back onto the unit sphere after each subdivision,
    /// giving a uniform distribution. The sphere is then scaled by radius.
    ///
    ///   radius       – radius of the resulting sphere
    ///   subdivisions – number of subdivision passes (0 = raw icosahedron, recommended max: 6)
    /// </summary>
    class Icosphere : public IGeometryBuilder
    {
    public:
        explicit Icosphere(float radius = 1.0f, uint32_t subdivisions = 3);

        virtual ~Icosphere();

        // Accessors
        const inline float    radius()       const { return _radius; }
        const inline uint32_t subdivisions() const { return _subdivisions; }

        // Mutators
        inline void radius(float r) { _radius = r; }
        inline void subdivisions(uint32_t s) { _subdivisions = s; }

        std::shared_ptr<Geometry> build(std::shared_ptr<IContext> context,
            const Material& material) const override;

    private:
        float    _radius;
        uint32_t _subdivisions;
    };
}
