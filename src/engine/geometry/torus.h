#pragma once

#include <medusa/engine_fwd.h>
#include <medusa/math.h>

#include "geometry.h"

namespace medusa
{
    /// <summary>
    /// Builds a torus (ring) geometry.
    ///
    /// The torus is centred at the origin, lying in the XZ plane.
    ///
    ///   majorRadius   – distance from the centre of the tube to the centre of the torus
    ///   minorRadius   – radius of the tube itself
    ///   majorSegments – number of segments around the main ring         (>= 3)
    ///   minorSegments – number of segments around the tube cross-section (>= 3)
    /// </summary>
    class Torus : public IGeometryBuilder
    {
    public:
        Torus(float majorRadius = 1.0f, float minorRadius = 0.25f, uint32_t majorSegments = 32, uint32_t minorSegments = 16);
        virtual ~Torus();

        // Accessors
        const inline float    majorRadius()    const { return _majorRadius; }
        const inline float    minorRadius()    const { return _minorRadius; }
        const inline uint32_t majorSegments()  const { return _majorSegments; }
        const inline uint32_t minorSegments()  const { return _minorSegments; }

        // Mutators
        inline void majorRadius(float r) { _majorRadius = r; }
        inline void minorRadius(float r) { _minorRadius = r; }
        inline void majorSegments(uint32_t s) { _majorSegments = s; }
        inline void minorSegments(uint32_t s) { _minorSegments = s; }

        std::shared_ptr<Geometry> build(std::shared_ptr<IContext> context, const Material& material) const override;

    private:
        float    _majorRadius;
        float    _minorRadius;
        uint32_t _majorSegments;
        uint32_t _minorSegments;
    };
}
