#pragma once

#include <medusa/medusa.h>
#include <medusa/graphics_fwd.h>
#include <medusa/containers_fwd.h>

#include <medusa/engine/resource.h>

namespace medusa
{
    class Transform;

    /// <summary>
    /// Interface for instancing a mesh
    /// </summary>
    class IMeshInstance
    {
    public:
        IMeshInstance() { }
        virtual ~IMeshInstance() {}
    };

    /// <summary>
    /// Interface for batch rendering
    /// </summary>
    class IMeshBatch
    {
    public:
        IMeshBatch() {}
        virtual ~IMeshBatch() { }

        virtual std::shared_ptr<IMeshInstance> pushInstance(std::shared_ptr<Transform> transform) = 0;
        virtual void popInstance(std::shared_ptr<IMeshInstance> instance) = 0;

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
