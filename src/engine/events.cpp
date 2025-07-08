#include "events.h"

#include <algorithm>

#include <SDL.h>

using namespace medusa;


//
Events::Events()
{

}


//
Events::~Events()
{

}


// Register Handler
bool Events::registerHandler(std::shared_ptr<IEventHandler> handler)
{
    _handlers.push_back(handler);
    return true;
}


// Unregister Handler
bool Events::unregisterHandler(std::shared_ptr<IEventHandler> handler)
{
    auto search = std::find_if(_handlers.begin(), _handlers.end(), [handler](const auto& h) {return h.lock().get() == handler.get(); });
    if (search != _handlers.end())
        _handlers.erase(search);
    return true;
}


// Poll Events
bool Events::poll()
{
    SDL_Event e;

    if (SDL_PollEvent(&e))
    {
        for (auto& it : _handlers)
        {
            if (auto handler = it.lock())
                handler->handleEvents(e);
        }
    }

    return true;
}
