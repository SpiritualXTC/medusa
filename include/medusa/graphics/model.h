#pragma once

#include <medusa/medusa.h>
#include <medusa/graphics_fwd.h>
#include <medusa/graphics/containers.h> // TODO: Used for struct Vertex... this should support variant

namespace medusa
{
    /// <summary>
    ///
    /// </summary>
    struct ModelData
    {
        uint32_t vertexStart;
        uint32_t indexStart;
        uint32_t indices;
        uint32_t vertices;

        uint32_t materialIndex;
    };

    /// <summary>
    /// Interface for loaded Model data
    /// </summary>
    class IModel
    {
    public:
        IModel() {}
        virtual ~IModel() {}

        virtual const std::vector<Vertex>& getVertices() const = 0;
        virtual const std::vector<uint32_t>& getIndices() const = 0;

        virtual const std::vector<std::shared_ptr<ITexture>>& getTextures() const = 0;
        virtual const std::vector<Material>& getMaterials() const = 0;
        virtual const std::vector<ModelData>& getModelData() const = 0;
    };

} // end namespace medusa
