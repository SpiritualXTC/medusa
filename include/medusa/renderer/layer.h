#pragma once

#include <medusa/medusa.h>

namespace medusa
{
    class ILayer
    {
    public:
        ILayer() {}
        virtual ~ILayer() {}


        virtual bool render() = 0;
    };
}
