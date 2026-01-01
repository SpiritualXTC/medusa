#include "object_component.h"

using namespace medusa;


//
ObjectComponent::ObjectComponent(std::shared_ptr<TransformBuffer> transformBuffer)
    : TransformComponent(transformBuffer)
{

}


//
ObjectComponent::~ObjectComponent()
{

}


//
bool ObjectComponent::hasComponent(const std::string& name)
{
    auto it = _components.find(name);
    return it != _components.end();
}


//
bool ObjectComponent::addComponent(const std::string& name, std::shared_ptr<IComponent> component)
{
    if (hasComponent(name))
        return false;

    _components.insert({ name, component });

    return true;
}


//
std::shared_ptr<IComponent> ObjectComponent::getComponent(const std::string& name)
{
    auto it = _components.find(name);
    return it != _components.end() ? it->second : nullptr;
}


//
bool ObjectComponent::update()
{
    bool result = TransformComponent::update();

    int idx = 0;
    for (auto& it : _components)
    {
        it.second->update();
    }

    return result;
}
