#pragma once

#include <medusa/game/scene_graph.h>
#include <engine/game/objects/entity.h>
#include <medusa/renderer/renderable.h>


namespace medusa
{
    class SceneGraph : public EntityComponent, public ISceneGraph
    {
    public:
        SceneGraph();
        virtual ~SceneGraph();

        bool update() override;


        // TODO: Add Renderable? -- Only RenderableContainer IS an IRenderable?
        bool addContainer(const std::string& name, std::shared_ptr<IRenderableContainer> container)
        {
            _sceneContainers.insert({ name, container });
            return true;
        }


        std::shared_ptr<IRenderableContainer> getContainer(const std::string& name)
        {
            auto it = _sceneContainers.find(name);
            return it == _sceneContainers.end() ? nullptr : it->second;
        }


    private:
        std::unordered_map<std::string, std::shared_ptr<IRenderableContainer>> _sceneContainers;

    };
}
