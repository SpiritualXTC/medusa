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
            std::shared_ptr<IMemory> createMemory(BufferType bufferType, BufferUsage usage) override;


            std::shared_ptr<IShader> createShader() override;
            std::shared_ptr<IDescriptor> createDescriptor() override;
            //std::shared_ptr<IMesh> create_mesh() override;
            std::shared_ptr<ITexture> createTexture() override;

        private:
            SDL_GLContext _context = nullptr;

            std::weak_ptr<MedusaState> _state;
            std::shared_ptr<WindowGL> _window;
            std::shared_ptr<RendererGL> _renderer;

            std::unordered_map<BufferType, ResourceIndex> _binding;
        };
    }
}
