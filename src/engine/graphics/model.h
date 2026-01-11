#pragma once

#include <medusa/medusa.h>

#include <medusa/engine_fwd.h>

#include <medusa/graphics/mesh.h>
#include <medusa/graphics/material.h>
#include <medusa/graphics/containers.h>

namespace medusa
{

    struct Indirect
    {
        uint32_t count;
        uint32_t instanceCount;
        uint32_t firstIndex;
        uint32_t baseVertex;
        uint32_t baseInstance;
    };


    /// <summary>
    ///
    /// </summary>
    class Model : public IMesh
    {
    public:
        // TODO: Refactor so the Mesh class does not own the indirect buffer. It should at most own a mesh layout structure.
        //  The IndirectBuffer should be owned by the "BatchMesh"
        Model(std::shared_ptr<IDescriptor> descriptor, std::shared_ptr<VertexBuffer> vb, std::shared_ptr<IndexBuffer> ib, std::shared_ptr<GenericArray<Indirect>> submesh);
        virtual ~Model();

        const inline std::shared_ptr<VertexBuffer> vertexBuffer() override { return _vertices; }
        const inline std::shared_ptr<IndexBuffer> indexBuffer() override { return _indices; }
        const inline std::shared_ptr<IDescriptor> descriptor() override { return _descriptor; }

        bool render() override;
        bool renderBatch(size_t instances = 0) override;

    private:
        std::shared_ptr<IDescriptor> _descriptor = nullptr;

        std::shared_ptr<VertexBuffer> _vertices;
        std::shared_ptr<IndexBuffer> _indices;
        std::shared_ptr<GenericArray<Indirect>> _submeshes;

        size_t _cache_instances = 0;
    };
}
