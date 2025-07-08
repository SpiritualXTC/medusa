#pragma once

#include <medusa/medusa.h>

namespace medusa
{
    union ComponentBitField
    {
        struct field
        {
            uint8_t component : 1;



            uint64_t padding : 63;
        };

        uint64_t bits;
    };



    class IComponent
    {
    public:
        IComponent() {}
        virtual ~IComponent() {}

        virtual bool update() = 0;

    private:
    };
}
