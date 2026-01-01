#include "transform_component.h"


using namespace medusa;


//
TransformComponent::TransformComponent(std::shared_ptr<TransformBuffer> transformBuffer)
    : Transform(matrix(), transformBuffer)
{

}


//
TransformComponent::~TransformComponent()
{

}


//
bool TransformComponent::update()
{
    pushTransform(calculate());
    return true;
}
