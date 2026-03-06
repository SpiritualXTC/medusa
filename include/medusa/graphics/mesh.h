#pragma once

#include <medusa/medusa.h>
#include <medusa/graphics_fwd.h>
#include <medusa/containers_fwd.h>

#include <medusa/engine/resource.h>

namespace medusa
{
    /// <summary>
    /// References a Mesh used for batch rendering
    /// </summary>
    class IMeshReference
    {
    public:
        IMeshReference() {}
        virtual ~IMeshReference() {}

        virtual void pushInstance(size_t transformIndex) = 0;
        virtual void popInstance(size_t transformIndex) = 0;

        virtual size_t instances() const = 0;
    private:
    };


    /// <summary>
    /// Interface for mesh/geometry data
    /// </summary>
    class IMesh : public ResourceID<IMesh>
    {
    public:
        IMesh() {}
        virtual ~IMesh() {}

        virtual const inline std::shared_ptr<VertexBuffer> vertexBuffer() = 0;
        virtual const inline std::shared_ptr<IndexBuffer> indexBuffer() = 0;
        virtual const inline std::shared_ptr<IDescriptor> descriptor() = 0;


        virtual bool render() = 0;
        // TODO: Batch rendering should not be handled by the base mesh
        virtual bool renderBatch(size_t instances = 0) = 0;

    private:

    };


}
