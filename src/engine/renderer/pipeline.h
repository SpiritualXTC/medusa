#pragma once

#include <medusa/medusa.h>
#include <medusa/graphics_fwd.h>
#include <medusa/containers_fwd.h>

#include <medusa/renderer/pipeline.h>

namespace medusa
{
    // Forward Declarations
    class GeometryBuffer;

    struct Indirect;


    // Generic Pipeline
    class Pipeline : public IPipeline
    {
    public:
        Pipeline(std::shared_ptr<IPass> pass, std::shared_ptr<GeometryBuffer> geometryBuffer);
        virtual ~Pipeline();

        bool render();

    private:
        std::shared_ptr<IPass> _pass;

        std::shared_ptr<GeometryBuffer> _geometryBuffer;
        std::shared_ptr<IDescriptor> _descriptor;

        std::shared_ptr<GenericArray<uint32_t>> _instanceMap;

        std::shared_ptr<GenericArray<Indirect>> _indirect;
    };
}
