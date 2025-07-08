#pragma once

namespace medusa
{
	class IInstanceBuffer
	{
	public:
		IInstanceBuffer() {}
		virtual ~IInstanceBuffer() {}

		virtual bool bind() = 0;
		virtual bool unbind() = 0;

		virtual bool create(const void* ptr, size_t count, size_t stride) = 0;
		virtual bool update(const void* ptr, size_t offset, size_t count) = 0;

		virtual size_t instances() = 0;

		// TODO: Mapping

	private:
	};
}
