#pragma once

#include <medusa/renderer/renderer.h>

namespace medusa
{

    class IPass;

    class Renderer : public IRenderer
    {
    public:
        Renderer();
        virtual ~Renderer();

    private:
    };
}
