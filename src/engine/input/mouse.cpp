#include "mouse.h"

using namespace medusa;

Mouse::Mouse()
{

}

Mouse::~Mouse()
{

}

bool Mouse::handleEvents(const SDL_Event& evt)
{
    if (evt.type == SDL_MOUSEBUTTONDOWN)
    {
        const SDL_MouseButtonEvent& mb = evt.button;

        MouseButtonEvent e{ mb.button };
        onMouseDown(e);
    }
    else if (evt.type == SDL_MOUSEBUTTONUP)
    {
        const SDL_MouseButtonEvent& mb = evt.button;

        MouseButtonEvent e{ mb.button };
        onMouseUp(e);
    }
    else if (evt.type == SDL_MOUSEWHEEL)
    {
        const SDL_MouseWheelEvent& mw = evt.wheel;

        MouseWheelEvent e{ mw.y };
        onMouseWheel(e);
    }
    else if (evt.type == SDL_MOUSEMOTION)
    {
        const SDL_MouseMotionEvent& mm = evt.motion;

        MouseMoveEvent e{ mm.xrel, mm.yrel };
        onMouseMove(e);
    }

    return true;
}
