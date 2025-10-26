#pragma once

#include <medusa/graphics/mesh.h>

#include <engine/graphics/model.h>

namespace medusa
{
    /// <summary>
    /// More complex mesh, includes textures/material/subset information
    /// </summary>
    class ModelInstance : public IMesh
    {
    public:
        ModelInstance(std::shared_ptr<IContext> context, std::shared_ptr<IMesh> model, std::shared_ptr<GenericArray<glm::mat4>> transformBuffer);
        virtual ~ModelInstance();

        const inline std::shared_ptr<VertexBuffer> vertexBuffer() override { return _mesh->vertexBuffer(); }
        const inline std::shared_ptr<IndexBuffer> indexBuffer() override { return _mesh->indexBuffer(); }
        const inline std::shared_ptr<IDescriptor> descriptor() override { return _mesh->descriptor(); }


        // TODO: Note, the instances count here is kinda meaningless... as is passing the count through the render function :)
        //bool render(size_t instances = 0) override { return _mesh->render(_instanceMap->size()); }
        bool render(size_t instances = 0) override { return _mesh->render(instances); }


        /// <summary>
        /// Registers a new instance of this mesh, set the world transformation
        /// </summary>
        /// <param name="worldIndex"></param>
        /// <returns>instanceIndex</returns>
        uint32_t pushInstance(uint32_t worldIndex);


        /// <summary>
        /// Removes the mesh instance
        /// </summary>
        /// <param name="instanceIndex">Instance returned by pushInstance</param>
        /// <returns></returns>
        uint32_t popInstance(uint32_t instanceIndex);

    private:

        std::shared_ptr<IMesh> _mesh = nullptr;

        // Instancing
        // TODO: If culling is implemented -- this MUST be handled with a second map. The "full" map, and an "active" map. The active map is updated by the "full" map every "frame -RENDER- against the camera.

        std::shared_ptr<GenericArray<uint32_t>> _instanceMap;       // Maps every single instance to the worldIndex. For now ... render EVERYTHING ALWAYS ALL AT ONCE
        std::weak_ptr<GenericArray<glm::mat4>> _transformBuffer;    // Reference the transform buffer
    };
}
