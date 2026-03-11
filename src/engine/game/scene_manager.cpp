#include "scene_manager.h"

#include <medusa/engine/context.h>
#include <medusa/graphics.h>
#include <medusa/graphics/containers.h>
#include <engine/resource_database.h>

#include <engine/engine.h>
#include <engine/graphics/model.h>
#include <engine/geometry/geometry.h>
#include <engine/geometry/geometry_buffer.h>

#include <engine/resources/texture_manager.h>


using namespace medusa;


//
SceneManager::SceneManager(std::shared_ptr<Engine> engine)
    : _engine(engine)
    , _context(engine->context())
{
    auto& context = engine->context();

    // Create Material Database
    _textures = std::make_shared<TextureManager>(context);

    _materials = context->createMap<Material>(BufferType::ShaderStorage, BufferUsage::StaticDraw);

    Material dummy;
    dummy.ambient(glm::vec4(0.3f, 0.0f, 0.3f, 1.0f));
    dummy.diffuse(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
    dummy.specular(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    dummy.emissive(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

    _materials->insert("[default]", dummy);

    // Create Geometry Buffer
    _geometry = std::make_shared<medusa::GeometryBuffer>(context);
}


//
std::shared_ptr<IMesh> SceneManager::getModel(const std::string& modelName)
{
    auto engine = _engine.lock();

    auto mesh = engine->resources()->getModel(modelName, _materials, _textures);

    // Scale the mesh cos it's bloody huge
    // TODO: Add matrix adjustments to the resource database
    logging::error("Scaling the mesh");
    auto vb = mesh->vertexBuffer();
    for (int index = 0; index < vb->vertices(); ++index)
    {
        auto& v = vb->data(index);
        v.position *= 0.1;
    }
    vb->sync();

    // Add the Mesh into the GeometryBuffer...
    _geometry->loadMesh(modelName, mesh);

    return mesh;
}


std::shared_ptr<IMesh> SceneManager::getModel(const std::string& name, const std::shared_ptr<IGeometry> geometry, const Material& material)
{
    static uint32_t generateIndex = 0;

    std::string matName = std::format("test_{}", generateIndex++);
    auto matIdx = _materials->insert(matName, material);

    auto mesh = geometry->mesh(matIdx);


    _geometry->loadMesh(name, mesh);

    return mesh;
}


bool SceneManager::addTexture(const std::string& name, std::shared_ptr<ITexture> texture)
{
    // TODO: The texture index needs to be pulled from the MANAGER ...

    // Add to the Texture Manager
    _textures->addTexture(name, texture);
    return true;
}


//
std::shared_ptr<ITexture> SceneManager::loadTexture2D(const std::string& filename)
{
    return _textures->loadTexture(filename);
}
