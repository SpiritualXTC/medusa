#pragma once

#include <medusa/medusa.h>
#include <medusa/graphics/shader.h>

#include <medusa/math.h>

namespace medusa
{
    class IContext;

    class IMesh;

    struct GeometryData
    {
        DType dtype;

        size_t count;                   // Number of elements in array

        std::vector<uint8_t> data;

        AttributeLocation location;
    };


    class IGeometry
    {
    public:
        virtual std::shared_ptr<IMesh> mesh(std::shared_ptr<IMesh> meshIn = nullptr) = 0;

    private:

    };


    // TODO: operator += to append [Geometry] data into a GeometryList, which also extends IGeometry, but is just a vector of geometry.


    class Geometry : public IGeometry
    {
    public:
        Geometry(std::shared_ptr<IContext> context);
        virtual ~Geometry();

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
        /// Construct a mesh from the geometry
        /// </summary>
        /// <returns></returns>
        std::shared_ptr<IMesh> mesh(std::shared_ptr<IMesh> meshIn = nullptr) override;

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

            return interleave<T>(buffer);
        }

    private:
        std::weak_ptr<IContext> _context;

        size_t _vertices = 0;
        size_t _stride = 0;
        std::vector<GeometryData> _geometry;
        std::vector<uint32_t> _indices;
    };
}
