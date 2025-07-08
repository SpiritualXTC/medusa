#pragma once

#include <memory>

#include <engine/renderer/renderer.h>

namespace medusa
{
    namespace opengl
    {
        class WindowGL;

        class RendererGL : public Renderer
        {
        public:
            RendererGL(const std::shared_ptr<WindowGL>& window);
            virtual ~RendererGL();


            void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

        private:

            std::weak_ptr<WindowGL> _window;
        };
    }
}
