#pragma once

#include <medusa/medusa.h>
#include <medusa/engine_fwd.h>
#include <medusa/graphics_fwd.h>

#include <medusa/renderer/renderable.h>
#include <medusa/graphics/shader.h>
#include <medusa/math.h>

#include <medusa/graphics/containers.h>

namespace medusa
{
    // Forward-Declarations
    class MeshComponent;


    struct InstanceData
    {
        DType data_type;

        InstanceAttributeLocation location;
    };



    struct Batch
    {
        std::shared_ptr<IMesh> mesh;
        std::shared_ptr<GenericArray<glm::mat4>> instances;
    };


    /// <summary>
    /// TODO: TEMP. Example renderable pool -- the payload should be a template
    /// </summary>
    class RenderPool : public IRenderableContainer
    {
    public:
        RenderPool(std::shared_ptr<IContext> context);

        virtual ~RenderPool();

        void addAttribute(const DType& dt, InstanceAttributeLocation location);

        template<typename T>
        void addAttribute(InstanceAttributeLocation location)
        {

        }

        template<>
        void addAttribute<glm::mat4>(InstanceAttributeLocation location)
        {
            addAttribute(types::FloatV4, location);
            addAttribute(types::FloatV4, InstanceAttributeLocation((int)location + 1));
            addAttribute(types::FloatV4, InstanceAttributeLocation((int)location + 2));
            addAttribute(types::FloatV4, InstanceAttributeLocation((int)location + 3));
        }


        void create_mesh_instance(std::shared_ptr<IMesh> base_mesh);


        bool add(std::shared_ptr<IRenderable> renderable) override;

        bool clear() override;

        bool render() override;

    private:

        std::shared_ptr<IContext> _context;

        std::vector<InstanceData> _data;

        std::unordered_map<uint64_t, std::shared_ptr<Batch>> _batch;
    };
}
