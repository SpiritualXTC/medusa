#pragma once

namespace medusa
{
    template <typename S>
    class GenericBuffer;

    template <typename S>
    class GenericArray;

    template <typename S>
    class GenericMap;

    // Specialisations:
    // TODO: VertexBuffer should be templated
    class VertexBuffer;
    class IndexBuffer;

}  // namespace medusa
