#pragma once

#include <medusa/renderer/layer.h>

namespace medusa
{
    // Forward Declarations
    class IPipeline;
    class IMatrix;


    /// <summary>
    /// Renders objects from the SceneGraph using the Pipeline
    /// </summary>
    class Layer : public ILayer
    {
    public:
        Layer(std::shared_ptr<IPipeline> pipeline);
        virtual ~Layer();

        bool render() override;

        // TODO: Needs to know where/how to get a camera from the SceneGraph

    private:
        std::shared_ptr<IPipeline> _pipeline;
    };


    /// <summary>
    /// Renders objects from the SceneGraph using multiple layers. The Camera /should?/ be shared across the layers
    /// </summary>
    class LayerGroup : public ILayer
    {
    public:
        LayerGroup();
        virtual ~LayerGroup();

        bool render() override;

        bool addLayer(std::shared_ptr<ILayer> layer);


    private:
        std::vector<std::shared_ptr<ILayer>> _layers;
    };
}
