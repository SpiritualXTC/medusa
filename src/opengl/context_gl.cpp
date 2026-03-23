#include "context_gl.h"

#include <string>

#include <SDL.h>
#include <SDL_image.h>

#include <medusa/exception.h>

#include <core/utilities/logging.h>
#include <core/utilities/config.h>

#include "gl.h"
#include "window_gl.h"

#include "graphics/memory_gl.h"

#include "graphics/shader_gl.h"
#include "graphics/descriptor_gl.h"
#include "graphics/texture_gl.h"


using namespace medusa;
using namespace medusa::opengl;


void APIENTRY debugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    logging::critical(fmt::format("GL Error: source={}, type={}, id={}, severity={}, msg={}", source, type, id, severity, message));

}


//
ContextGL::ContextGL(std::shared_ptr<Config> config)
{
    // Initialise SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
        throw MedusaError("Unable to Initialise SDL");
    logging::info("SDL Initialised");

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    uint32_t imageFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    uint32_t imageFlagsLoaded = 0;
    if ((imageFlagsLoaded = IMG_Init(imageFlags)) == 0)
        throw MedusaError("Unable to Initialise SDLImage");
    else if (imageFlagsLoaded != imageFlags)
    {
        logging::error(fmt::format("Unable to initialise all Image formats for SDL Image Loader. expected={}, loaded={}", imageFlags, imageFlagsLoaded));
    }

    // Create the Window
    int w = config->getValue<int>("medusa.window.width", 800);
    int h = config->getValue<int>("medusa.window.height", 600);

    logging::info(fmt::format("Resolution: {}x{}", w, h));
    _window = std::make_shared<WindowGL>(w, h);

    // Create the Context
    _context = SDL_GL_CreateContext(_window->handle());
    logging::info("OpenGL Context Created");

    if (SDL_GL_MakeCurrent(_window->handle(), _context) != 0)
    {
        std::string err = fmt::format("Unable to configure SDL Context: {}", SDL_GetError());
        logging::error(err);
        throw MedusaError(err);
    }

    // Initialise the GLEW Extensions
    GLenum err;
    if ((err = glewInit()) != 0)
    {
        logging::error(fmt::format("Error initialising GLEW: {}", err));
        throw MedusaError("Unable to Intialise GLEW");
    }
    logging::info("GLEW Initialised");

    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(debugMessageCallback, this);

    int major, minor;
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &major);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minor);

    logging::info(fmt::format("OpenGL Version: {}.{}", major, minor));

    // Enable/Disable VSync [TODO: Should be an option]
    SDL_GL_SetSwapInterval(0);  // 1 to enable VSync
}


//
ContextGL::~ContextGL()
{
    if (_context != nullptr)
        SDL_GL_DeleteContext(_context);

    IMG_Quit();

    _context = nullptr;
}


//
bool ContextGL::next()
{
    float r = 0.5;// (float)(rand() % 255) / 255;
    float g = 0.5;// (float)(rand() % 255) / 255;
    float b = 0.5;// (float)(rand() % 255) / 255;
    //glClearColor(r, g, b, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    return true;
}


//
bool ContextGL::present()
{
    SDL_GL_SwapWindow(_window->handle());
    return true;
}


//
void ContextGL::setClearColour(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
}


//
void ContextGL::setClearDepth(float f)
{
    glClearDepth(f);
}


//
void ContextGL::setClearStencil(int32_t i)
{
    glClearStencil(i);
}


//
void ContextGL::setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
    glViewport(x, y, width, height);
}


//
std::shared_ptr<IShader> ContextGL::createShader()
{
    auto shader = std::make_shared<ShaderGL>();

    return shader;
}



std::shared_ptr<IMemory> ContextGL::createMemory(BufferType bufferType, BufferUsage usage)
{
    auto it = _binding.find(bufferType);
    if (it == _binding.end())
        _binding.insert({ bufferType, 0 });

    ResourceIndex& resource = _binding[bufferType];
    ++resource;

    auto memory = std::make_shared<MemoryGL>(bufferType, usage, resource);

    return memory;
}


//
std::shared_ptr<IDescriptor> ContextGL::createDescriptor()
{
    auto descriptor = std::make_shared<DescriptorGL>();

    return descriptor;

}


//
std::shared_ptr<ITexture> ContextGL::createTexture()
{
    auto texture = std::make_shared<TextureGL>();

    return texture;
}
