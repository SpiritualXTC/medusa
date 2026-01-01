#pragma once

#include <medusa/game/scene_graph.h>
#include <engine/game/objects/entity.h>
#include <medusa/renderer/renderable.h>
#include <medusa/graphics/containers.h>

#include <engine/game/components/object_component.h>

namespace medusa
{
    class SceneManager;

    class SceneGraph : public ISceneGraph, public IComponentContainer
    {
    public:
        SceneGraph(std::shared_ptr<SceneManager> manager);
        virtual ~SceneGraph();

        bool update() override;

        // Facade into the IComponentContainer
        inline bool hasComponent(const std::string& name) override { return _root->hasComponent(name); }
        bool addComponent(const std::string& name, std::shared_ptr<IComponent> component) override { return _root->addComponent(name, component); }
        std::shared_ptr<IComponent> getComponent(const std::string& name) override { return _root->getComponent(name); }


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


        inline std::shared_ptr<TransformBuffer> getTransformBuffer() { return _transformBuffer; }


    private:
        std::shared_ptr<SceneManager> _manager;


        std::shared_ptr<TransformBuffer> _transformBuffer;

        std::shared_ptr<ObjectComponent> _root;

        std::unordered_map<std::string, std::shared_ptr<IRenderableContainer>> _renderableContainers;
    };
}
