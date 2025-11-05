#pragma once

#include <medusa/graphics/containers.h>

#include <medusa/game/component.h>
#include <engine/game/components/transform_component.h>


namespace medusa
{
    class ModelInstance;

    class ModelComponent : public TransformComponent
    {
    public:
        ModelComponent(std::shared_ptr<ModelInstance> mesh, std::shared_ptr<GenericArray<glm::mat4>> transformBuffer);
        virtual ~ModelComponent();

        bool update() override;

    private:
        std::shared_ptr<ModelInstance> _mesh;

        // World Transform Buffer
        std::weak_ptr<GenericArray<glm::mat4>> _transformBuffer;
        size_t _transformIndex = -1;
    };

} // namespace medusa
