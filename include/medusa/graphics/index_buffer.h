#pragma once

namespace medusa
{
	class IIndexBuffer
	{
	public:
		IIndexBuffer() {}
		virtual ~IIndexBuffer() {}

		virtual bool bind() = 0;
		virtual bool unbind() = 0;

		virtual bool create(const uint32_t* ptr, size_t count) = 0;
		virtual bool update(const uint32_t* ptr, size_t offset, size_t count) = 0;

		virtual size_t indices() = 0;

	private:
	};
}
