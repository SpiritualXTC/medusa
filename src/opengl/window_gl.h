#pragma once

#include <SDL.h>

#include <engine/window.h>


namespace medusa
{
    namespace opengl
    {
        class WindowGL : public Window
        {
        public:
            WindowGL(uint32_t width, uint32_t height);
            virtual ~WindowGL();


            // Getters
            glm::ivec2 getDisplaySize() const override;


            inline SDL_Window* handle() const override { return _window; }

        private:
            SDL_Window* _window = nullptr;
        };
    }
}
