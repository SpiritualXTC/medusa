#pragma once

#include <type_traits>

#include <medusa/graphics/buffer.h>

namespace medusa
{
    class UniformBuffer : public IUniformBuffer
    {
    public:
        UniformBuffer(size_t size)
            : _data(size)
        {

        }

        virtual ~UniformBuffer() {}

        inline size_t size() override { return _data.size(); }
        //inline void* data() override { return _data.data(); }

    private:
        std::vector<uint8_t> _data;
    };
}
