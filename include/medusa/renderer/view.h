#pragma once

#include <medusa/medusa.h>

namespace medusa
{
    class IView
    {
    public:
        IView() {}
        virtual ~IView() {}

        virtual bool resize(uint32_t width, uint32_t height) = 0;

        virtual bool render() = 0;
    };
}
