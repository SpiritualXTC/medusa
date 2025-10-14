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

        virtual const inline std::shared_ptr<VertexBuffer> vertexBuffer() = 0;
        virtual const inline std::shared_ptr<IndexBuffer> indexBuffer() = 0;
        virtual const inline std::shared_ptr<IDescriptor> descriptor() = 0;

        virtual bool render(size_t instances = 0) = 0;

    private:

    };


}
