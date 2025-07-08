#include "keyboard.h"

using namespace medusa;

Keyboard::Keyboard()
{

}

Keyboard::~Keyboard()
{

}

bool Keyboard::handleEvents(const SDL_Event& evt)
{
    if (evt.type == SDL_KEYDOWN)
    {
        const SDL_KeyboardEvent& kb = evt.key;

        KeyEvent e{ kb.keysym.scancode };
        onKeyDown(e);
    }
    else if (evt.type == SDL_KEYUP)
    {
        const SDL_KeyboardEvent& kb = evt.key;

        KeyEvent e{ kb.keysym.scancode };
        onKeyUp(e);
    }
    return true;
}
