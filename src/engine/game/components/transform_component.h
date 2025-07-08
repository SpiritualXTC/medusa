#pragma once

//#include <medusa/math/matrix.h>

#include <medusa/game/component.h>

#include <core/math/matrix.h>


namespace medusa
{
    class TransformComponent : public IComponent, public Matrix
    {
    public:
        TransformComponent() {}
        virtual ~TransformComponent() {}

        bool update() override { calculate(); return true; }
    };
}
