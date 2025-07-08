#include "window_gl.h"

#include <SDL.h>

#include <medusa/exception.h>


using namespace medusa;
using namespace medusa::opengl;


//
WindowGL::WindowGL(uint32_t width, uint32_t height)
    : Window(width, height)
{
    // Create SDL Window
    _window = SDL_CreateWindow("Medusa Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1600, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (_window == nullptr)
        throw MedusaError("Unable to Create SDL Window");
}


//
WindowGL::~WindowGL()
{
    // Destroy SDL Window
    if (_window != nullptr)
        SDL_DestroyWindow(_window);
    _window = nullptr;
}


//
glm::ivec2 WindowGL::getDisplaySize() const
{
    // TODO: Ofcourse this is a GL function
    int width = 0;
    int height = 0;
    SDL_GL_GetDrawableSize(handle(), &width, &height);

    return glm::ivec2(width, height);
}
