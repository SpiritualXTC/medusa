#pragma once

#include <medusa/medusa.h>

namespace medusa
{
    class ISceneGraph
    {
    public:
        ISceneGraph() {}
        virtual ~ISceneGraph(){ }


        virtual bool update() = 0;
    };
}
