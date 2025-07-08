#pragma once

#include <medusa/medusa.h>

namespace medusa
{
    /// <summary>
    ///
    /// </summary>
    class IApplication
    {
    public:
        IApplication() {}
        virtual ~IApplication() {}

        // This is NOT the true interface, just a dummy to get some features working. :)
        virtual void update() = 0;
        virtual void render() = 0;
    };
}
