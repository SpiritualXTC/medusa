#pragma once

#include <medusa/engine_fwd.h>
#include <medusa/math.h>

#include "geometry.h"

namespace medusa
{
    /// <summary>
    /// Builds a closed cylinder with flat top and bottom caps.
    ///
    /// The cylinder is centred at the origin along the Y axis, with the top cap
    /// at +height/2 and the bottom cap at -height/2.
    ///
    ///   radius   – radius of the cylinder
    ///   height   – total height
    ///   segments – number of radial segments (>= 3)
    ///   stacks   – number of vertical subdivisions along the side wall (>= 1)
    /// </summary>
    class Cylinder : public IGeometryBuilder
    {
    public:
        Cylinder(float    radius   = 1.0f,
                 float    height   = 2.0f,
                 uint32_t segments = 32,
                 uint32_t stacks   = 1);

        virtual ~Cylinder();

        // Accessors
        const inline float    radius()   const { return _radius; }
        const inline float    height()   const { return _height; }
        const inline uint32_t segments() const { return _segments; }
        const inline uint32_t stacks()   const { return _stacks; }

        // Mutators
        inline void radius(float r)       { _radius   = r; }
        inline void height(float h)       { _height   = h; }
        inline void segments(uint32_t s)  { _segments = s; }
        inline void stacks(uint32_t s)    { _stacks   = s; }

        std::shared_ptr<Geometry> build(const Material& material) const override;

    protected:
        float    _radius;
        float    _height;
        uint32_t _segments;
        uint32_t _stacks;
    };


    /// <summary>
    /// Builds a hollow tube with flat annular top and bottom caps.
    ///
    /// The tube is centred at the origin along the Y axis. The outer wall, inner
    /// wall, and both annular caps are generated as separate quad strips so that
    /// hard normals are preserved at the cap edges.
    ///
    ///   outerRadius  – radius of the outer wall
    ///   innerRadius  – radius of the inner wall (must be < outerRadius)
    ///   height       – total height
    ///   segments     – number of radial segments (>= 3)
    ///   stacks       – number of vertical subdivisions on each wall (>= 1)
    ///   capThickness – reserved for future per-cap inset; currently unused
    /// </summary>
    class Tube : public Cylinder
    {
    public:
        Tube(float    outerRadius = 1.0f,
             float    innerRadius = 0.5f,
             float    height      = 2.0f,
             uint32_t segments    = 32,
             uint32_t stacks      = 1);

        virtual ~Tube();

        // Accessors
        const inline float outerRadius() const { return _radius; }
        const inline float innerRadius() const { return _innerRadius; }

        // Mutators
        inline void outerRadius(float r) { _radius      = r; }
        inline void innerRadius(float r) { _innerRadius = r; }

        std::shared_ptr<Geometry> build(const Material& material) const override;

    private:
        float _innerRadius;
    };
}
