#pragma once

#include <medusa/medusa.h>
#include <medusa/engine/resource.h>
#include <medusa/graphics_fwd.h>
#include <medusa/game/transform.h>

#include <medusa/renderer/renderable.h>

#include <engine/graphics/model_instance.h>
//#include <engine/game/components/model_component.h>

namespace medusa
{
    class Transform;

    /// <summary>
    ///
    /// </summary>
    class BatchInstance : public IMeshInstance
    {
    public:
        BatchInstance() {}
        virtual ~BatchInstance() {}

    private:
        uint32_t _worldIndex = -1;  // Index into the transform matrix
        uint32_t _instanceIndex = -1;  // Batch index
    };


    /// <summary>
    /// Replaces ModelInstance
    /// </summary>
    class Batch : public IMeshBatch
    {
    public:
        Batch(std::shared_ptr<IContext> context, std::shared_ptr<IMesh> model, std::shared_ptr<TransformBuffer> transformBuffer);
        virtual ~Batch();

        std::shared_ptr<IMeshInstance> pushInstance(std::shared_ptr<Transform> transform);
        void popInstance(std::shared_ptr<IMeshInstance> instance);

        void batchRender();

    private:
        std::shared_ptr<IMesh> _mesh = nullptr;

        std::shared_ptr<GenericArray<uint32_t>> _instanceMap;       // Maps every single instance to the worldIndex. For now ... render EVERYTHING ALWAYS ALL AT ONCE
        std::weak_ptr<TransformBuffer> _transformBuffer;    // Reference the transform buffer
    };


    // TODO: Needs an interface for rendering -- NOT for creating instance?
    //      Could probably just reuse the container? -- only
    // TODO: Temp Interface [rename]
    class BatchRenderer : public IRenderableContainer
    {
    public:
        BatchRenderer(std::shared_ptr<IContext> context, std::shared_ptr<TransformBuffer> transformBuffer);
        virtual ~BatchRenderer();


        /// <summary>
        ///
        /// </summary>
        /// <param name="mesh"></param>
        /// <param name="transform"></param>
        /// <returns></returns>
        std::shared_ptr<IMeshInstance> createBatchInstance(std::shared_ptr<IMesh> mesh, std::shared_ptr<Transform> transform); // NEW

        /// <summary>
        /// Render all the items in the iterable
        /// </summary>
        /// <returns></returns>
        bool render() override;
    private:

        std::weak_ptr<IContext> _context;
        std::weak_ptr<TransformBuffer> _transformBuffer;

        std::unordered_map<uint64_t, std::shared_ptr<Batch>> _batches;
        std::unordered_map<uint64_t, std::shared_ptr<ModelInstance>> _instances;

    };
}
