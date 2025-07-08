#pragma once

#include <medusa/medusa.h>
#include <medusa/game/component.h>

#include <engine/game/components/object_component.h>

namespace medusa
{
    /// <summary>
    /// Handles Component Heirarccy. Everything can be a parent. EVERYTHING
    /// Base of the entity, so code can be defined in the source file
    /// </summary>
    class EntityComponent : public ObjectComponent
    {
    public:
        EntityComponent();
        virtual ~EntityComponent();


        bool update() override; //{ return true; }
        //bool updateComponents();

        bool hasComponent(const std::string& name);
        bool addComponent(const std::string& name, std::shared_ptr<IComponent> component);
        std::shared_ptr<IComponent> getComponent(const std::string& name);

    private:

        std::unordered_map<std::string, std::shared_ptr<IComponent>> _components;
    };


    /// <summary>
    /// Merge the component parent with that of the child component.
    /// This is a wrapping class, and cannot be extended directly. It can (and probably should) be typedef'd
    /// eg: typedef Entity<Item> ItemEntity;
    /// </summary>
    /// <typeparam name="T"></typeparam>
    template <class T>
    class Entity : public EntityComponent, public T
    {
    public:
        Entity() {}
        virtual ~Entity() {}

        bool update() override
        {
            return  T::update() & EntityComponent::update();
        }

    private:
    };
}
