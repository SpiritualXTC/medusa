#pragma once

//#include <medusa/math/matrix.h>

#include <medusa/game/component.h>
#include <medusa/game/transform.h>

#include <core/math/matrix.h>


namespace medusa
{
    /// <summary>
    ///
    /// </summary>
    class TransformComponent : public IComponent, public Matrix, public Transform
    {
    public:
        TransformComponent(std::shared_ptr<TransformBuffer> transformBuffer);
        virtual ~TransformComponent();

        bool update() override;
    };
}
