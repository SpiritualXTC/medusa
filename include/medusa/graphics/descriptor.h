#pragma once

#include <medusa/medusa.h>
#include <medusa/data_type.h>

#include <medusa/graphics/enum.h>
#include <medusa/graphics/shader.h>


namespace medusa
{
	class IDescriptor
	{
	public:
		IDescriptor() {}
		virtual ~IDescriptor() {}


		virtual bool addDescription(const DType& dtype, uint32_t stride, AttributeLocation attributeLocation) = 0;
		virtual bool addInstanceAttribute(const DType& dtype, uint32_t stride, InstanceAttributeLocation instanceAttributeLocation) = 0;


		virtual bool bind() = 0;
		virtual bool unbind() = 0;

		virtual bool render(PrimitiveType primitiveType, size_t vertices, size_t indices = 0) = 0;
		virtual bool render(PrimitiveType primitiveType, size_t vertices, size_t indices = 0, size_t instances = 1) = 0;

	private:

	};
}
