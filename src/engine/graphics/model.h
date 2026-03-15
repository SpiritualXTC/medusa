#pragma once

#include <medusa/medusa.h>

#include <medusa/engine_fwd.h>
#include <medusa/graphics_fwd.h>

#include <medusa/graphics/mesh.h>
#include <medusa/graphics/material.h>
#include <medusa/graphics/containers.h>

#include <engine/geometry/geometry.h>

namespace medusa
{

    struct Indirect
    {
        // TODO: Move this elsewhere -- IDescriptor?
        uint32_t count;
        uint32_t instanceCount;
        uint32_t firstIndex;
        uint32_t baseVertex;
        uint32_t baseInstance;
    };


    struct ModelData
    {
        uint32_t vertexStart;
        uint32_t indexStart;
        uint32_t indices;
        uint32_t vertices;

        uint32_t materialIndex;
    };


    class Model : public Geometry
    {
    public:
        Model(std::shared_ptr<IContext> context) : Geometry(context) {}
        virtual ~Model() {}

        /// <summary>
        ///
        /// </summary>
        /// <param name="name"></param>
        /// <param name="texture"></param>
        /// <returns></returns>
        uint64_t addTexture(const std::string& name, std::shared_ptr<ITexture> texture);

        /// <summary>
        ///
        /// </summary>
        /// <param name="material"></param>
        /// <returns></returns>
        uint64_t addMaterial(const Material& material);

        /// <summary>
        ///
        /// </summary>
        /// <param name="modelData"></param>
        /// <returns></returns>
        uint64_t addModelData(const ModelData& modelData);

        const std::vector<std::shared_ptr<ITexture>>& getTextures() const { return _textures; }
        const std::vector<Material>& getMaterials() const { return _materials; }
        const std::vector<ModelData>& getModelData() const { return _modelData; }

    private:

        std::vector<std::shared_ptr<ITexture>> _textures;
        std::vector<Material> _materials;

        std::vector<ModelData> _modelData;
    };


    /*
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

        const inline std::shared_ptr<GenericArray<Indirect>> submeshes() { return _submeshes; }


        bool render() override;
        bool renderBatch(size_t instances = 0) override;

    private:
        std::shared_ptr<IDescriptor> _descriptor = nullptr;

        std::shared_ptr<VertexBuffer> _vertices;
        std::shared_ptr<IndexBuffer> _indices;
        std::shared_ptr<GenericArray<Indirect>> _submeshes;

        size_t _cache_instances = 0;
    };
    */
}
