#pragma once

#include <list>

#include <medusa/graphics/enum.h>
#include <medusa/graphics/descriptor.h>

#include <opengl/gl.h>


namespace medusa
{
    namespace opengl
    {
        class DescriptorGL : public IDescriptor
        {
        public:
            DescriptorGL();
            virtual ~DescriptorGL();

            bool addDescription(const DType& dtype, uint32_t stride, AttributeLocation attributeLocation) override;
            bool addInstanceAttribute(const DType& dtype, uint32_t stride, InstanceAttributeLocation instanceAttributeLocation) override;

            bool bind() override;
            bool unbind() override;

            bool render(PrimitiveType primitiveType, size_t vertices, size_t indices = 0) override;
            bool render(PrimitiveType primitiveType, size_t vertices, size_t indices = 0, size_t instances = 1) override;

        private:

            size_t _stride = 0;

            GLHandle _vao = 0;

            size_t _instanceStride = 0;
        };
    }
}
