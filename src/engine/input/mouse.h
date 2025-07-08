#pragma once

#include <medusa/engine/events.h>

namespace medusa
{
    /// <summary>
    ///
    /// </summary>
    struct MouseButtonEvent
    {
        int button; // Enum Class or Enum Flag or boring old integer :)
    };


    /// <summary>
    ///
    /// </summary>
    struct MouseMoveEvent
    {
        int x;
        int y;
    };


    /// <summary>
    ///
    /// </summary>
    struct MouseWheelEvent
    {
        int delta;
    };

    typedef boost::signals2::signal<void(MouseButtonEvent&)> OnMouseDown;
    typedef boost::signals2::signal<void(MouseButtonEvent&)> OnMouseUp;
    typedef boost::signals2::signal<void(MouseWheelEvent&)> OnMouseWheel;
    typedef boost::signals2::signal<void(MouseMoveEvent&)> OnMouseMove;


    /// <summary>
    ///
    /// </summary>
    class Mouse : public IEventHandler
    {
    public:
        Mouse();
        virtual ~Mouse();

        virtual bool handleEvents(const SDL_Event& evt);

        OnMouseDown onMouseDown;
        OnMouseUp onMouseUp;
        OnMouseWheel onMouseWheel;
        OnMouseMove onMouseMove;

    private:
    };
}
