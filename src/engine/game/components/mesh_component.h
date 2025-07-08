#pragma once

#include <medusa/graphics_fwd.h>

#include <medusa/renderer/renderable.h>
#include <engine/game/components/renderable_component.h>
#include <medusa/graphics/mesh.h>


namespace medusa
{
    class MeshComponent : public RenderableComponent, public TransformComponent, public std::enable_shared_from_this<MeshComponent>
    {
    public:
        MeshComponent(std::shared_ptr<IRenderableContainer> container);
        virtual ~MeshComponent();

        // Update the component
        bool update() override;
        bool render(std::shared_ptr<IPass> pass) override;

        inline uint64_t resource_id() {return _mesh->id();}


        inline const glm::mat4& transform() override {return matrix();}


        // Setters
        inline MeshComponent& setMesh(std::shared_ptr<IMesh> mesh) { _mesh = mesh; return *this; }

        inline MeshComponent& setTexture(std::shared_ptr<ITexture> texture) { _texture = texture; return *this; }

    private:
        std::shared_ptr<IMesh> _mesh;
        std::shared_ptr<ITexture> _texture;
    };
}
