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

        std::shared_ptr<IMesh> getModel(const std::string& modelName);
        std::shared_ptr<IMesh> getModel(const std::string& name, const std::shared_ptr<IGeometryBuilder> shape, const Material& material);

        std::shared_ptr<GeometryBuffer> getGeometryBuffer() { return _geometry; }


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
