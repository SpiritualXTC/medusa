#pragma once

#include <medusa/medusa.h>
#include <medusa/graphics/enum.h>


namespace medusa
{
    class IPipeline
    {
    public:
        IPipeline() {}
        virtual ~IPipeline() {}

        virtual bool render() = 0;
    };

} //namespace medusa
