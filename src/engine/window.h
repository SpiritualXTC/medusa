#pragma once

#include <SDL.h>

#include <medusa/engine/window.h>


namespace medusa
{

    // TODO: Improve "handle", and most of this class is SDL specific. NOT API specific
    class Window : public IWindow
    {
    public:
        Window(uint32_t width, uint32_t height);
        virtual ~Window();

        virtual bool handleEvents(const SDL_Event& evt);

        virtual SDL_Window* handle() const = 0;



        // Setters
        void setTitle(const std::string& title) override;


        void close();


    private:

    };
}
