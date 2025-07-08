#pragma once

#include <medusa/graphics/buffer.h>
#include <opengl/graphics/buffer_gl.h>

namespace medusa
{
    namespace opengl
    {

        /// <summary>
        ///
        /// </summary>
        class UniformBufferGL : public GenericBufferGL<IUniformBuffer>
        {
        public:
            UniformBufferGL(uint32_t bindingIndex, size_t size);


            inline const uint32_t bindingIndex() const override { return _bindingIndex; }

        private:
            const uint32_t _bindingIndex;
        };


        /// <summary>
        ///
        /// </summary>
        class StorageBufferGL : public GenericBufferGL<IStorageBuffer>
        {
        public:
            StorageBufferGL(uint32_t bindingIndex, size_t size);

            inline const uint32_t bindingIndex() const override { return _bindingIndex; }

        private:
            const uint32_t _bindingIndex;

        };
    }
}
