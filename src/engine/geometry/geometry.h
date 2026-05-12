#pragma once

#include <medusa/medusa.h>
#include <medusa/graphics/model.h>
#include <medusa/graphics/shader.h>   // ?Need

#include <medusa/math.h>

namespace medusa
{
    class IContext;

    // TODO: Should be private to Geometry
    struct GeometryData
    {
        DType dtype;

        size_t count;                   // Number of elements in array

        std::vector<uint8_t> data;

        AttributeLocation location;
    };


    /// <summary>
    ///
    /// </summary>
    class Geometry
    {
    public:
        Geometry();
        virtual ~Geometry();


        /// <summary>
        ///
        /// </summary>
        /// <returns></returns>
        inline size_t stride() const { return _stride; }


        /// <summary>
        /// Add Vertex Data
        /// </summary>
        /// <param name="data"></param>
        /// <param name="dt"></param>
        /// <param name="count"></param>
        void addVertexData(void* data, DType& dt, size_t count, AttributeLocation location);


        /// <summary>
        ///
        /// </summary>
        /// <param name="data"></param>
        /// <param name="count"></param>
        void addIndexData(uint32_t* data, size_t count); // TODO: include an auto_offset parameter so new indices are offset by the existing ones.


        /// <summary>
        ///
        /// </summary>
        /// <param name="buffer"></param>
        /// <param name="stride"></param>
        void interleave(uint8_t* buffer, size_t stride = 0);


        /// <summary>
        /// Add GLM Typed data
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <typeparam name="I"></typeparam>
        /// <typeparam name="Q"></typeparam>
        /// <param name="data"></param>
        /// <param name="count"></param>
        template<int I, typename T, glm::qualifier Q>
        void addVertexData(glm::vec<I, T, Q>* data, size_t count, AttributeLocation location)
        {
            DType dt = GLMDataType<glm::vec<I, T, Q>>();
            addVertexData(data, dt, count, location);
        }

        /// <summary>
        ///
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="data"></param>
        /// <param name="count"></param>
        /// <param name="elements"></param>
        template <typename T>
        void addVertexData(T* data, size_t count, size_t elements, AttributeLocation location)
        {
            DType dt = ScalarDataType<T>(elements);
            addVertexData(data, dt, count, location);
        }

        /// <summary>
        /// Interleave using a typed structure
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="buffer"></param>
        /// <returns></returns>
        template <typename T>
        std::vector<T> interleave(std::vector<T>& buffer)
        {
            interleave((uint8_t*)&buffer[0], sizeof(T));

            return buffer;
        }

        /// <summary>
        /// Interleave using a typed structure, interally created
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <returns></returns>
        template <typename T>
        std::vector<T> interleave()
        {
            std::vector<T> buffer(_vertices);

            interleave<T>(buffer);

            return std::move(buffer);
        }


        /// <summary>
        ///
        /// </summary>
        /// <returns></returns>
        std::vector<uint32_t>& getIndices() { return _indices; }


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
        size_t _vertices = 0;
        size_t _stride = 0;
        std::vector<GeometryData> _geometry;
        std::vector<uint32_t> _indices;

        std::unordered_map<AttributeLocation, GeometryData> _geometryData;


        std::vector<std::shared_ptr<ITexture>> _textures;
        std::vector<Material> _materials;

        std::vector<ModelData> _modelData;
    };


    /// <summary>
    ///
    /// </summary>
    class IGeometryBuilder
    {
    public:
        /// <summary>
        /// Builds and returns a Model populated with this shape's
        /// vertex/index data and the provided material.
        /// </summary>
        virtual std::shared_ptr<Geometry> build(const Material& material) const = 0;
    };
}
