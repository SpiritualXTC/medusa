#include "window.h"

#include <iostream>



#include <medusa/exception.h>
#include <core/utilities/logging.h>


using namespace std;
using namespace medusa;


//
Window::Window(uint32_t width, uint32_t height)
{

    /*
    // Create SDL Window
    _window = SDL_CreateWindow("Medusa Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (_window == nullptr)
        throw MedusaError("Unable to Create SDL Window");
    */
}


//
Window::~Window()
{
    /*
    // Destroy SDL Window
    if (_window != nullptr)
        SDL_DestroyWindow(_window);
    _window = nullptr;
    */
}


// Quit SDL
void Window::close()
{
    // Terminate SDL
    SDL_Quit();
}


//
void Window::setTitle(const std::string& title)
{
    SDL_SetWindowTitle(handle(), title.c_str());
}



// Handle Window Events
bool Window::handleEvents(const SDL_Event& evt)
{
    if (evt.type == SDL_WINDOWEVENT)
    {
        if (evt.window.event == SDL_WINDOWEVENT_RESIZED)
        {
            ResizeEvent e{ evt.window.data1, evt.window.data2 };
            onResize(e);
        }
        else
        {
            cout << "window event type:" << evt.window.type << std::endl;
        }
    }
    else if (evt.type == SDL_QUIT)
    {
        CloseEvent e;
        onClose(e);
    }

    return true;
}
