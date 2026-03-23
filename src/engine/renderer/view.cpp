#include "view.h"

#include <core/utilities/logging.h>

#include <medusa/engine/context.h>
#include <medusa/renderer/layer.h>
#include <engine/window.h>

using namespace medusa;


//
View::View(std::shared_ptr<IContext> context, float left, float top, float width, float height)
    : _context(context)
    , _position(left, top)
    , _size(width, height)
    , _screen(0, 0)
{
    _screen = context->window()->getDisplaySize();

}


//
View::~View()
{

}


//
bool View::resize(uint32_t width, uint32_t height)
{
    logging::info(fmt::format("View::resize() - {}x{}", width, height));

    _screen = glm::ivec2(width, height);

    float w = (float)width;
    float h = (float)height;

    onViewResize(w, h);




    return true;
}


//
bool View::render()
{
    uint32_t left = _screen.x * _position.x;
    uint32_t top = _screen.y * _position.y;
    uint32_t width = _screen.x * _size.x;
    uint32_t height = _screen.y * _size.y;

    _context->setViewport(left, top, width, height);

    layer()->render();
    return true;
}
