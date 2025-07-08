#pragma once

#include <SDL.h>

#include <medusa/engine/context.h>

#include "window_gl.h"
#include "renderer_gl.h"

namespace medusa
{
    // Forward Declarations
    class Config;
    class MedusaState;


    namespace opengl
    {
        class ContextGL : public IContext, public std::enable_shared_from_this<ContextGL>
        {
        public:
            ContextGL(std::shared_ptr<Config> config);
            virtual ~ContextGL();

            // IState
            virtual bool reset() override;

            // IContext
            virtual std::shared_ptr<IWindow> window() override { return _window; }
            virtual std::shared_ptr<IRenderer> renderer() override { return _renderer; }

            virtual bool next() override;
            virtual bool present() override;

            // Factory
            std::shared_ptr<IShader> create_shader() override;
            std::shared_ptr<IIndexBuffer> create_index_buffer() override;
            std::shared_ptr<IVertexBuffer> create_vertex_buffer() override;
            std::shared_ptr<IInstanceBuffer> create_instance_buffer() override;
            std::shared_ptr<IUniformBuffer> create_uniform_buffer(size_t size) override;
            std::shared_ptr<IStorageBuffer> create_shader_storage_buffer(size_t size) override;
            std::shared_ptr<IDescriptor> create_descriptor() override;
            std::shared_ptr<IMesh> create_mesh() override;
            std::shared_ptr<ITexture> create_texture() override;

        private:
            SDL_GLContext _context = nullptr;

            std::weak_ptr<MedusaState> _state;
            std::shared_ptr<WindowGL> _window;
            std::shared_ptr<RendererGL> _renderer;

            // Managers
            uint32_t _uniformBindingIndex = 0;
            uint32_t _storageBindingIndex = 0;

            std::list<std::weak_ptr<IUniformBuffer>> _uniforms;
            std::list<std::weak_ptr<IStorageBuffer>> _buffers;
        };
    }
}
