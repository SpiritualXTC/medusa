#pragma once

#include <medusa/medusa.h>

namespace medusa
{
    union ComponentBitField
    {
        struct field
        {
            uint8_t component : 1;



            uint64_t padding : 63;
        };

        uint64_t bits;
    };


    /// <summary>
    /// Base interface for all components
    /// </summary>
    class IComponent
    {
    public:
        IComponent() {}
        virtual ~IComponent() {}

        virtual bool update() = 0;

    private:
    };


    /// <summary>
    /// Base interface for all component containers
    /// </summary>
    class IComponentContainer
    {
    public:
        virtual bool hasComponent(const std::string& name) = 0;
        virtual bool addComponent(const std::string& name, std::shared_ptr<IComponent> component) = 0;
        virtual bool removeComponent(const std::string& name) = 0;
        virtual std::shared_ptr<IComponent> getComponent(const std::string& name) = 0;
    };
}
