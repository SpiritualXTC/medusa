#pragma once

#include <medusa/medusa.h>
#include <medusa/engine/resource.h>
#include <medusa/graphics_fwd.h>

#include <medusa/renderer/renderable.h>

#include <engine/graphics/model_instance.h>
#include <engine/game/components/model_component.h>

namespace medusa
{
    // TODO: Could this just be RenderableMap ?
    // TODO: Could this be templated?

    // TODO: Needs an interface for rendering -- NOT for creating instance?
    //      Could probably just reuse the container? -- only
    // TODO: Temp Interface [rename]
    class MeshContainer : public IRenderableContainer
    {
    public:
        MeshContainer(std::shared_ptr<IContext> context, std::shared_ptr<GenericArray<glm::mat4>> transformBuffer);
        virtual ~MeshContainer();


        /// <summary>
        ///
        /// </summary>
        /// <param name="mesh"></param>
        /// <returns></returns>
        std::shared_ptr<ModelComponent> createInstance(std::shared_ptr<IMesh> mesh);


        /// <summary>
        /// Render all the items in the iterable
        /// </summary>
        /// <returns></returns>
        bool render() override;
    private:

        std::weak_ptr<IContext> _context;
        std::weak_ptr<GenericArray<glm::mat4>> _transformBuffer;

        std::unordered_map<uint64_t, std::shared_ptr<ModelInstance>> _instances;

    };
}
