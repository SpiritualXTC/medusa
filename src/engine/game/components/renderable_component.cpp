#include "renderable_component.h"

using namespace medusa;


//
RenderableComponent::RenderableComponent(std::shared_ptr<IRenderableContainer> container)
    : _container(container)
{

}


//
RenderableComponent::~RenderableComponent()
{

}


//
/*
bool RenderableComponent::update()
{
    bool b = TransformComponent::update();

    // Add to traversable
    _container->add(this->shared_from_this());

    return b;
}
*/
