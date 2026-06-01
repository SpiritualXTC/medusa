#pragma once

#include <SDL.h>

#include <medusa/engine_fwd.h>
#include <medusa/engine/context.h>

#include "window_gl.h"


namespace medusa
{
    namespace opengl
    {
        class PipelineStateGL;

        class ContextGL : public IContext, public std::enable_shared_from_this<ContextGL>
        {
        public:
            ContextGL(std::shared_ptr<IConfig> config);
            virtual ~ContextGL();

            // IContext
            virtual std::shared_ptr<IWindow> window() override { return _window; }

            virtual bool next() override;
            virtual bool present() override;

            // Context States
            void setClearColour(float r, float g, float b, float a) override;
            void setClearDepth(float f) override;
            void setClearStencil(int32_t i) override;

            void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

            // Factory
            std::shared_ptr<IMemory> createMemory(BufferType bufferType, BufferUsage usage) override;


            std::shared_ptr<IShader> createShader() override;
            std::shared_ptr<IDescriptor> createDescriptor() override;
            //std::shared_ptr<IMesh> create_mesh() override;
            std::shared_ptr<ITexture> createTexture() override;

        private:
            SDL_GLContext _context = nullptr;

            std::shared_ptr<WindowGL> _window;

            std::unordered_map<BufferType, ResourceIndex> _binding;

            std::shared_ptr<PipelineStateGL> _pipelineState;
        };
    }
}
