#pragma once

#include <boost/functional/hash.hpp>

#include "glm/glm.hpp"

namespace medusa
{
    /// <summary>
    ///
    /// </summary>
    struct DType
    {
    public:
        DType(size_t size, size_t elements)
            : _size(size)
            , _elements(elements)
        {
            //
        }

        const inline size_t size() const { return _size; }
        const inline size_t elements() const { return _elements; }

        // Hashing
        bool operator==(DType const& other) const
        {
            return _size == other.size() && _elements == other.elements();
        }

    private:
        size_t _size = 0;
        size_t _elements = 0;
    };


    /// <summary>
    ///
    /// </summary>
    /// <typeparam name="T"></typeparam>
    /// <typeparam name="E"></typeparam>
    template <typename T, int E>
    struct DataType : DType
    {
    public:
        DataType()
            : DType(sizeof(T) * E, E)
        {
            //
        }
    };


    template <typename T>
    struct ScalarDataType : DType
    {
    public:
        ScalarDataType(size_t elements = 1)
            : DType(sizeof(T) * elements, elements)
        {
            //
        }
    };

    /// <summary>
    ///
    /// </summary>
    /// <typeparam name="V"></typeparam>
    template <typename V>
    struct GLMDataType : DType
    {
    public:
        explicit GLMDataType(int mul = 1)
            : DType(sizeof(V), V::length() * mul)
        {
            //
        }

        explicit GLMDataType(const V& g)
            : DType(sizeof(g), g.length())
        {
            //
        }
    };


    /// <summary>
    ///
    /// </summary>
    /// <typeparam name="T"></typeparam>
    /// <typeparam name="I"></typeparam>
    /// <typeparam name="Q"></typeparam>
    /// <param name="v"></param>
    /// <returns></returns>
    template<int I, typename T, glm::qualifier Q>
    const DType make_dtype(const glm::vec<I, T, Q>& v)
    {
        return DataType(v.length(), I);
    }


    /// <summary>
    ///
    /// </summary>
    /// <typeparam name="T"></typeparam>
    /// <typeparam name="I"></typeparam>
    /// <typeparam name="Q"></typeparam>
    /// <param name="v"></param>
    /// <returns></returns>
    template<int I, int J, typename T, glm::qualifier Q>
    const DType make_dtype(const glm::mat<I, J, T, Q>& v)
    {
        return DataType(v.length(), I * J);
    }


    /// <summary>
    ///
    /// </summary>
    /// <typeparam name="T"></typeparam>
    /// <typeparam name="I"></typeparam>
    /// <typeparam name="Q"></typeparam>
    /// <param name="v"></param>
    /// <returns></returns>
    template<int I, typename T, glm::qualifier Q>
    const DType make_dtype(const glm::vec<I, T, Q>* v)
    {
        return DataType(sizeof(T) * I, I);
    }


    namespace types
    {
        const auto Int = DataType<uint32_t, 1>();
        const auto Int2 = DataType<uint32_t, 2>();
        const auto Int3 = DataType<uint32_t, 3>();
        const auto Int4 = DataType<uint32_t, 4>();

        const auto IntV1 = GLMDataType<glm::ivec1>();
        const auto IntV2 = GLMDataType<glm::ivec2>();
        const auto IntV3 = GLMDataType<glm::ivec3>();
        const auto IntV4 = GLMDataType<glm::ivec4>();

        const auto FloatV1 = GLMDataType<glm::vec1>();
        const auto FloatV2 = GLMDataType<glm::vec2>();
        const auto FloatV3 = GLMDataType<glm::vec3>();
        const auto FloatV4 = GLMDataType<glm::vec4>();

        const auto FloatM44 = GLMDataType<glm::mat4>(4);
    }  // namespace types
}  // namespace medusa
