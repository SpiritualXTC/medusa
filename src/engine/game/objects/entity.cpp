#include "entity.h"

#include <core/utilities/logging.h>


using namespace medusa;

//
EntityComponent::EntityComponent()
{

}


//
EntityComponent::~EntityComponent()
{

}


//
bool EntityComponent::hasComponent(const std::string& name)
{
    auto it = _components.find(name);
    return it != _components.end();
}


//
bool EntityComponent::addComponent(const std::string& name, std::shared_ptr<IComponent> component)
{
    if (hasComponent(name))
        return false;

    _components.insert({ name, component });

    return true;
}


//
std::shared_ptr<IComponent> EntityComponent::getComponent(const std::string& name)
{
    auto it = _components.find(name);
    return it != _components.end() ? it->second : nullptr;
}


//
bool EntityComponent::update()
{
    bool result = ObjectComponent::update();

    int idx = 0;
    for (auto& it : _components)
    {
        it.second->update();
    }

    return result;
}
