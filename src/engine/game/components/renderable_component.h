#pragma once

#include <medusa/renderer/renderable.h>

#include <engine/game/components/transform_component.h>


namespace medusa
{
    /// <summary>
    ///
    /// </summary>
    class RenderableComponent : public IRenderable
    {
    public:
        RenderableComponent(std::shared_ptr<IRenderableContainer> container);
        virtual ~RenderableComponent();

    protected:
        inline std::shared_ptr<IRenderableContainer> container() { return _container; }

    private:
        std::shared_ptr<IRenderableContainer> _container;
    };
}
