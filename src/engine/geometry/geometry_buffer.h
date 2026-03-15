#pragma once

#include <medusa/medusa.h>
#include <medusa/engine_fwd.h>
#include <medusa/graphics_fwd.h>
#include <medusa/containers_fwd.h>

#include <medusa/graphics/mesh.h>

#include <core/utilities/logging.h>


namespace medusa
{
    class Model;
    class IGeometry;
    class Geometry;
    class TextureManager;

    /// <summary>
    /// Basic description of a submesh
    /// </summary>
    struct SubMesh
    {
        uint32_t count;
        uint32_t firstIndex;
        uint32_t baseVertex;
    };


    /// <summary>
    /// References the mesh inside the global geometry buffer
    /// </summary>
    class MeshReference : public IMeshReference
    {
    public:
        MeshReference(std::shared_ptr<IContext> context);
        virtual ~MeshReference();

        /// <summary>
        /// Push an Instance
        /// </summary>
        void pushInstance(size_t transformIndex);


        /// <summary>
        /// Pop an Instance
        /// </summary>
        void popInstance(size_t transformIndex);


        /// <summary>
        /// Get total number of instances that are active with this mesh
        /// </summary>
        /// <returns></returns>
        size_t instances() const override { return _count; }


        /// <summary>
        /// Get the set of transform indices this mesh instance references
        /// </summary>
        /// <returns></returns>
        const std::unordered_set<size_t>& transformIndices() const { return _transformIndices; }


        /// <summary>
        /// Get the submesh data
        /// </summary>
        /// <returns></returns>
        std::vector<SubMesh>& subMeshes() { return _submeshes; }


        /// <summary>
        /// Add submesh data
        /// </summary>
        /// <param name="sm"></param>
        void addSubMesh(SubMesh& sm)
        {
            _submeshes.push_back(sm);
        }


    private:
        size_t _count = 0;


        std::unordered_set<size_t> _transformIndices;

        std::vector<SubMesh> _submeshes;
    };


    /// <summary>
    /// Global geometry Buffer
    /// </summary>
    class GeometryBuffer
    {
    public:
        GeometryBuffer(std::shared_ptr<IContext> context);
        virtual ~GeometryBuffer();


        /// <summary>
        /// Loads a mesh into the buffer :: should probably just be a copy rather than handle the loading -- would make more sense imho
        /// Give it a name, and the model to add the buffer
        /// TODO: Refactor
        /// </summary>
        /// <returns></returns>

        bool loadMesh(const std::string& name, std::shared_ptr<Model> model, std::shared_ptr<GenericMap<Material>> materials, std::shared_ptr<TextureManager> textures);
        bool loadMesh(const std::string& name, std::shared_ptr<Geometry> geometry, int32_t materialOverride = -1);


        std::shared_ptr<MeshReference> referenceMesh(const std::string& name);

        // Create a descriptor from the current geometry
        //  TODO: Allow creating a descriptor using only a subset of the geometry
        std::shared_ptr<IDescriptor> createDescriptor();


        inline std::weak_ptr<IContext> context() { return _context; }


        inline std::vector<SubMesh>& submesh() { return _submeshes; }
        inline std::unordered_map<std::string, std::shared_ptr<MeshReference>> meshRefs() { return _refs; }

    private:
        std::weak_ptr<IContext> _context;

        // Description :: This is NOT a Descriptor.
        // TODO: Defines the attributes of what a descriptor will look like. Descriptors are built from the above createDescriptor using this data


        // Geometry Data
        std::shared_ptr<VertexBuffer> _vertices;
        std::shared_ptr<IndexBuffer> _indices;

        std::vector<SubMesh> _submeshes;

        std::unordered_map<std::string, std::shared_ptr<MeshReference>> _refs;

    };

}
