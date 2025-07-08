#pragma once

#include <medusa/medusa.h>

namespace medusa
{
    class IPass;

    class IRenderer
    {
    public:
        IRenderer() {}
        virtual ~IRenderer() {}

        virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
    };
}
