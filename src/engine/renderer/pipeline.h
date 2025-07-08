#pragma once

#include <medusa/medusa.h>

#include <medusa/renderer/pipeline.h>

namespace medusa
{
    // Forward Declarations

    // Generic Pipeline
    class Pipeline : public IPipeline
    {
    public:
        Pipeline(std::shared_ptr<IPass> pass);
        virtual ~Pipeline();

        bool render();

    private:
        std::shared_ptr<IPass> _pass;
    };
}
