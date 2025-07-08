#pragma once

#include <medusa/medusa.h>
#include <medusa/engine/resource.h>

namespace medusa
{
	class IVertexBuffer;
	class IIndexBuffer;
	class IDescriptor;

	class IInstanceBuffer;

	/// <summary>
	///
	/// </summary>
	class IMesh : public ResourceID<IMesh>
	{
	public:
		IMesh() {}
		virtual ~IMesh() {}

		virtual bool render(size_t instances = 0) = 0;

		virtual const inline std::shared_ptr<IVertexBuffer> vertexBuffer() = 0;
		virtual const inline std::shared_ptr<IIndexBuffer> indexBuffer() = 0;
		virtual const inline std::shared_ptr<IDescriptor> descriptor() = 0;

	private:

	};


	/// <summary>
	///
	/// </summary>
	class IMeshInstance : virtual public IMesh
	{
	public:
		IMeshInstance() {}
		virtual ~IMeshInstance() {}

		virtual const inline std::shared_ptr<IInstanceBuffer> instanceBuffer() = 0;
	};
}
