#pragma once

#include <medusa/medusa.h>

#include <medusa/renderer/scene.h>

namespace medusa
{
    // Forward Declarations
    class IView;

    class Scene : public IScene
    {
    public:
        Scene(std::shared_ptr<IView> view);
        virtual ~Scene();


        bool update(float ft);
        bool render();

    private:

        std::shared_ptr<IView> _view;

    };


}
