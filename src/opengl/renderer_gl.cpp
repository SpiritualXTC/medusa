#include "renderer_gl.h"

#include <core/utilities/logging.h>

#include "gl.h"

using namespace medusa;
using namespace medusa::opengl;




/*

*/
RendererGL::RendererGL(const std::shared_ptr<WindowGL>& window)
    : Renderer()
    , _window(window)
{
    logging::info("Create OpenGL Renderer");

    //self._renderer = sdl2.ext.Renderer(self._window)


    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_BLEND);


    glClearColor(0.1, 0.1, 0.1, 1.0);

}


/*

*/
RendererGL::~RendererGL()
{

}

void RendererGL::setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
    glViewport(x, y, width, height);
}
