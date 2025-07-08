#pragma once

#include <medusa/medusa.h>
#include <medusa/graphics_fwd.h>

#include <engine/graphics/mesh.h>



namespace medusa
{
    /// <summary>
    /// Not sure if this is useful. Depends whether the render pool becomes more than just semi useful.
    /// </summary>
    class MeshInstance : public Mesh, public IMeshInstance
    {
    public:
        MeshInstance(std::shared_ptr<IContext> context);
        virtual ~MeshInstance();


        const inline std::shared_ptr<IInstanceBuffer> instanceBuffer() { return _instanceBuffer; }

        bool render(size_t instances = 0);

    private:
        std::shared_ptr<IInstanceBuffer> _instanceBuffer = nullptr;
    };
}
