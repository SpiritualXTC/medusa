#pragma once

#include <medusa/medusa.h>
#include <medusa/graphics_fwd.h>

#include <medusa/renderer/renderable.h>

namespace medusa
{
    class IPass
    {
    public:
        IPass() {}

        virtual ~IPass() {}

        virtual const inline std::shared_ptr<IShader> shader() = 0;
    };
}
