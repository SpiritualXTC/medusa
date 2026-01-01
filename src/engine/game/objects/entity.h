#pragma once

#include <medusa/medusa.h>
#include <medusa/game/component.h>

#include <engine/game/components/object_component.h>

namespace medusa
{
    /// <summary>
    /// Merge the component parent with that of the child component.
    /// This is a wrapping class, and cannot be extended directly. It can (and probably should) be typedef'd
    /// eg: typedef Entity<Item> ItemEntity;
    /// </summary>
    /// <typeparam name="T"></typeparam>
    template <class T>
    class Entity : public ObjectComponent, public T
    {
    public:
        Entity() {}
        virtual ~Entity() {}

        bool update() override
        {
            return  T::update() & ObjectComponent::update();
        }

    private:
    };
}
