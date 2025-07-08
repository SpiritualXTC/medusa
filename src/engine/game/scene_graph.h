#pragma once

#include <medusa/game/scene_graph.h>
#include <engine/game/objects/entity.h>

namespace medusa
{
    class SceneGraph : public EntityComponent, public ISceneGraph
    {
    public:
        SceneGraph();
        virtual ~SceneGraph();

        bool update() override;

    };
}
