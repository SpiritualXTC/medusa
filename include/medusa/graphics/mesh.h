#pragma once

#include <medusa/medusa.h>
#include <medusa/graphics_fwd.h>
#include <medusa/containers_fwd.h>

#include <medusa/engine/resource.h>

namespace medusa
{
    /// <summary>
    ///
    /// </summary>
    class IMesh : public ResourceID<IMesh>
    {
    public:
        IMesh() {}
        virtual ~IMesh() {}

        virtual const inline std::shared_ptr<IVertexBuffer> vertexBuffer() = 0;
        virtual const inline std::shared_ptr<IIndexBuffer> indexBuffer() = 0;
        virtual const inline std::shared_ptr<IDescriptor> descriptor() = 0;

        virtual bool render(size_t instances = 0) = 0;

    private:

    };


}
