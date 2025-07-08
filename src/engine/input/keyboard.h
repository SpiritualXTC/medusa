#pragma once

#include <medusa/engine/events.h>


namespace medusa
{
    /// <summary>
    ///
    /// </summary>
    struct KeyEvent
    {
        int x;
    };

    typedef boost::signals2::signal<void(KeyEvent&)> OnKeyDown;
    typedef boost::signals2::signal<void(KeyEvent&)> OnKeyUp;

    /// <summary>
    ///
    /// </summary>
    class Keyboard : public medusa::IEventHandler
    {
    public:
        Keyboard();
        virtual ~Keyboard();

        //
        virtual bool handleEvents(const SDL_Event& evt);


        // Event Handlers
        OnKeyDown onKeyDown;
        OnKeyUp onKeyUp;

    private:
    };
}
