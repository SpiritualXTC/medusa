#pragma once

#include <medusa/game/scene_graph.h>
#include <engine/game/objects/entity.h>
#include <medusa/renderer/renderable.h>
#include <medusa/graphics/containers.h>

namespace medusa
{
    class SceneManager;

    class SceneGraph : public EntityComponent, public ISceneGraph
    {
    public:
        SceneGraph(std::shared_ptr<SceneManager> manager);
        virtual ~SceneGraph();

        bool update() override;


        // TODO: Add Renderable? -- Only RenderableContainer IS an IRenderable?
        bool addContainer(const std::string& name, std::shared_ptr<IRenderableContainer> container)
        {
            _renderableContainers.insert({ name, container });
            return true;
        }


        std::shared_ptr<IRenderableContainer> getContainer(const std::string& name)
        {
            auto it = _renderableContainers.find(name);
            return it == _renderableContainers.end() ? nullptr : it->second;
        }


        inline std::shared_ptr<GenericArray<glm::mat4>> getTransformBuffer() { return _matrixBuffer; }


    private:
        std::shared_ptr<SceneManager> _manager;


        std::shared_ptr<GenericArray<glm::mat4>> _matrixBuffer; // TODO: This PROBABLY makes more sense in the SceneGraph, but for simplicity in testing, lets leave it here for now

        std::unordered_map<std::string, std::shared_ptr<IRenderableContainer>> _renderableContainers;

    };
}
