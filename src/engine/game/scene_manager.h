#pragma once

#include <medusa/medusa.h>

#include <medusa/engine_fwd.h>
#include <medusa/graphics_fwd.h>
#include <medusa/containers_fwd.h>

namespace medusa
{
    class Engine;

    class Geometry;
    class GeometryBuffer;
    class IGeometryBuilder;

    class TextureManager;


    /// <summary>
    /// Manages the scene
    /// </summary>
    class SceneManager
    {
    public:
        SceneManager(std::shared_ptr<Engine> engine);

        const std::weak_ptr<Engine> engine() { return _engine; }


        inline std::shared_ptr<GenericMap<Material>> getMaterials() { return _materials; }
        inline const std::shared_ptr<GenericMap<Material>> getMaterials() const { return _materials; }

        inline std::shared_ptr<TextureManager> getTextures() { return _textures; }
        inline const std::shared_ptr<TextureManager> getTextures() const { return _textures; }

        bool addGeometry(const std::string& name, const std::shared_ptr<Geometry> geometry);

        std::shared_ptr<GeometryBuffer> getGeometryBuffer() { return _geometry; }


        /// <summary>
        ///
        /// </summary>
        /// <param name="assetName"></param>
        /// <returns></returns>
        std::shared_ptr<IShader> loadShader(const std::string& assetName);

        /// <summary>
        /// Load a texture via the assets repository, and add to the manager
        /// </summary>
        /// <param name="assetName"></param>
        /// <returns></returns>
        std::shared_ptr<ITexture> loadTexture(const std::string& assetName);

        /// <summary>
        /// Load a model via the assets repository
        /// </summary>
        std::shared_ptr<IModel> loadModel(const std::string& assetName);

        /// <summary>
        /// Add a texture to the manager
        /// </summary>
        /// <param name="name"></param>
        /// <param name="texture"></param>
        /// <returns></returns>
        bool addTexture(const std::string& name, std::shared_ptr<ITexture> texture);

    private:
        std::weak_ptr<Engine> _engine;
        std::weak_ptr<IContext> _context;

        // Texture Manager
        std::shared_ptr<TextureManager> _textures;

        // Material data SSBO
        std::shared_ptr<GenericMap<Material>> _materials;

        // Geometry Buffer
        std::shared_ptr<medusa::GeometryBuffer> _geometry;
    };
}
