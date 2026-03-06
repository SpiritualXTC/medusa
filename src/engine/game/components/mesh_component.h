#pragma once

#include <medusa/graphics.h>

#include <medusa/game/component.h>


namespace medusa
{
    class MeshInstanceComponent : public IComponent
    {
    public:
        // TODO: This will be a "mesh reference" not an IMesh
        //  A Mesh reference is part of the "global GeometryBuffer"
        MeshInstanceComponent(std::shared_ptr<IMeshReference> meshRef, size_t transIndex);
        virtual ~MeshInstanceComponent();

        bool update() override;

    private:
        std::shared_ptr<IMeshReference> _meshRef;
        size_t _instanceIndex = 0;
        size_t _transformIndex = 0;
    };


} // namespace medusa
