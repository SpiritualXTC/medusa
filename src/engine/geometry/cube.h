#pragma once

#include <medusa/engine_fwd.h>
#include <medusa/math.h>

#include "geometry.h"

namespace medusa
{
    class Cube : public IGeometryBuilder
    {
    public:
        Cube(float width = 1.0f, float height = 1.0f, float depth = 1.0f);

        explicit Cube(const glm::vec3& dimensions)
            : Cube(dimensions.x, dimensions.y, dimensions.z)
        {

        }


        virtual ~Cube();

        const inline float width() const { return _width; }
        const inline float height() const { return _height; }
        const inline float depth() const { return _depth; }

        inline void width(float w) { _width = w; }
        inline void height(float h) { _height = h; }
        inline void depth(float d) { _depth = d; }


        std::shared_ptr<Geometry> build(const Material& material) const override;

    private:

        float _width;
        float _height;
        float _depth;
    };
}
