#pragma once

#include <medusa/medusa.h>
#include <medusa/engine/state.h>
#include <medusa/graphics_fwd.h>


namespace medusa
{
	class IWindow;
	class IRenderer;

	class IContext : public IState
	{
	public:
		IContext() {}
		virtual ~IContext() {}

		virtual std::shared_ptr<IWindow> window() = 0;
		virtual std::shared_ptr<IRenderer> renderer() = 0;

		virtual bool next() = 0;
		virtual bool present() = 0;

		// Factory
		virtual std::shared_ptr<IShader> create_shader() = 0;
		virtual std::shared_ptr<IIndexBuffer> create_index_buffer() = 0;
		virtual std::shared_ptr<IVertexBuffer> create_vertex_buffer() = 0;
		virtual std::shared_ptr<IInstanceBuffer> create_instance_buffer() = 0;
		virtual std::shared_ptr<IUniformBuffer> create_uniform_buffer(size_t size) = 0;
		virtual std::shared_ptr<IStorageBuffer> create_shader_storage_buffer(size_t size) = 0;
		virtual std::shared_ptr<IDescriptor> create_descriptor() = 0;
		virtual std::shared_ptr<IMesh> create_mesh() = 0;
		virtual std::shared_ptr<ITexture> create_texture() = 0;

	private:

	};
}
