#pragma once

#include <medusa/medusa.h>

namespace medusa
{
    class IPass;

    class IPipeline
    {
    public:
        IPipeline() {}
        virtual ~IPipeline() {}

        virtual bool render() = 0;
    };
}
