#pragma once

#include <medusa/medusa.h>

#include <medusa/engine_fwd.h>

#include <medusa/graphics/material.h>
#include <engine/graphics/mesh.h>

namespace medusa
{
    struct SubmeshData
    {
        uint32_t vertexOffset;
        uint32_t vertexCount;
        uint32_t indexOffset;
        uint32_t indexCount;

        uint32_t materialIndex;
    };


    /// <summary>
    /// More complex mesh, includes textures/material/subset information
    /// </summary>
    class Model : public Mesh
    {
    public:
        Model(std::shared_ptr<IContext> context);
        virtual ~Model();

        void addMaterial(Material& material);
        void addSubmeshData(uint32_t vertices, uint32_t indices, uint32_t materialIndex);

        bool render(size_t instances = 0) override;

    private:
        std::vector<Material> _materials;
        std::vector<SubmeshData> _submeshes;
    };
}
