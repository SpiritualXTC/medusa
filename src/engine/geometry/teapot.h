#pragma once

#include <medusa/engine_fwd.h>
#include <medusa/math.h>

#include "geometry.h"

namespace medusa
{
    /// <summary>
    /// Builds the Utah Teapot geometry from its canonical 32 bicubic Bezier patches.
    ///
    /// Each patch is tessellated into a uniform (resolution x resolution) quad grid
    /// and normals are computed analytically from the patch partial derivatives.
    ///
    ///   resolution – number of subdivisions per patch edge (>= 2, recommended: 8-16)
    ///   scale      – uniform scale applied to the final geometry
    /// </summary>
    class Teapot : public IGeometryBuilder
    {
    public:
        explicit Teapot(uint32_t resolution = 8, float scale = 1.0f);

        virtual ~Teapot();

        // Accessors
        const inline uint32_t resolution() const { return _resolution; }
        const inline float    scale()       const { return _scale; }

        // Mutators
        inline void resolution(uint32_t r) { _resolution = r; }
        inline void scale(float s)         { _scale      = s; }

        std::shared_ptr<Geometry> build(std::shared_ptr<IContext> context,
                                        const Material& material) const override;

    private:
        uint32_t _resolution;
        float    _scale;
    };
}
