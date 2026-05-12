#pragma once

#include <medusa/medusa.h>
#include <medusa/graphics/model.h>
#include <medusa/graphics/material.h>

#include <medusa/graphics/containers.h> // TODO: Remove (included for struct Vertex)


#include <engine/geometry/geometry.h>



namespace medusa
{
    class Model : public IModel
    {
    public:
        Model();
        Model(std::shared_ptr<Geometry> geometry);
        virtual ~Model();

        const std::vector<std::shared_ptr<ITexture>>& getTextures() const { return _textures; }
        const std::vector<Material>& getMaterials() const { return _materials; }
        const std::vector<ModelData>& getModelData() const { return _modelData; }


        const std::vector<Vertex>& getVertices() const { return _vertices; }
        const std::vector<uint32_t>& getIndices() const { return _indices; }

    private:
        std::vector<Vertex> _vertices;
        std::vector<uint32_t> _indices;

        std::vector<std::shared_ptr<ITexture>> _textures;
        std::vector<Material> _materials;

        std::vector<ModelData> _modelData;
    };
}
