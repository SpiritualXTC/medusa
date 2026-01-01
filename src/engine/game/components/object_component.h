#pragma once

#include <engine/game/components/transform_component.h>

namespace medusa
{
    /// <summary>
    ///
    /// </summary>
    class ObjectComponent : public TransformComponent, public IComponentContainer
    {
    public:
        ObjectComponent(std::shared_ptr<TransformBuffer> transformBuffer = nullptr);
        virtual ~ObjectComponent();

        bool hasComponent(const std::string& name) override;
        bool addComponent(const std::string& name, std::shared_ptr<IComponent> component) override;
        std::shared_ptr<IComponent> getComponent(const std::string& name) override;


        bool update() override;

    private:

        std::unordered_map<std::string, std::shared_ptr<IComponent>> _components;
    };
}
