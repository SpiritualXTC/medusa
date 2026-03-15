#pragma once

#include <medusa/engine/context.h>
#include <medusa/math.h>

#include "geometry.h"

namespace medusa
{
    class Cube : public Geometry
    {
    public:
        Cube(std::shared_ptr<IContext> context, const glm::vec3& dimensions)
            : Cube(context, dimensions.x, dimensions.y, dimensions.z)
        {

        }
        Cube(std::shared_ptr<IContext> context, float width = 1.0f, float height = 1.0f, float depth = 1.0f);

        virtual ~Cube();

        const inline float width() const { return _width; }
        const inline float height() const { return _height; }
        const inline float depth() const { return _depth; }


        inline float width(float width)
        {
            _width = width;
        }

        inline float height(float height)
        {
            _height = height;
        }

        inline float depth(float depth)
        {
            _depth = depth;
        }

        //std::shared_ptr<Model> mesh(uint32_t materialIndex = -1) override;

    private:
        //std::weak_ptr<IContext> _context;

        float _width;
        float _height;
        float _depth;
    };
}
