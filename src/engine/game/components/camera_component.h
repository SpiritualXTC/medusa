#pragma once

#include <medusa/game/component.h>
#include <core/math/view_matrix.h>
#include <core/math/projection_matrix.h>


namespace medusa
{
    // TODO: Should their be a IViewMatrix?
    // TODO: Template against the base IViewMatrix
    class ViewMatrixComponent : public IComponent, public ViewMatrix
    {
    public:
        ViewMatrixComponent() { }
        virtual ~ViewMatrixComponent() {}

        bool update() override { calculate(); return true; }
    };

    // TODO: Should their be an IProjectionMatrix
    // TODO: Template against the IProjectionMatrix
    class ProjectionMatrixComponent : public IComponent, public ProjectionMatrix
    {
    public:
        ProjectionMatrixComponent() {}
        virtual ~ProjectionMatrixComponent() {}

        bool update() override { calculate(); return true; }
    };
}
