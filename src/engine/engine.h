#pragma once

#include <memory>
#include <vector>

#include <medusa/engine/engine.h>
#include <medusa/engine_fwd.h>

namespace medusa
{
    // Forward Declarations
    class MedusaConfig;
    class Timing;

    class Events;

    class Keyboard;
    class Mouse;

    class ResourceDatabase;

    class IScene;


    /// <summary>
    ///
    /// </summary>
    class Engine : public IEngine
    {
    public:
        Engine();
        virtual ~Engine();

        void run(std::shared_ptr<IApplication> application);
        void quit();

        const inline std::shared_ptr<IWindow>& window() { return _window; }
        const inline std::shared_ptr<IContext>& context() { return _context; }

        const inline std::shared_ptr<Timing>& timing() { return _timing; }
        const inline std::shared_ptr<ResourceDatabase>& resources() { return _resources; }

        // TODO: Input. Would be beneficial to abstract input to an action handler.
        //  Multiple action handles can be registered, each once has customisable "behaviours" and configurable on a per-os basis.
        //  The Actions however remap the OS events into Game specific events
        //  For easier tracking
        inline std::shared_ptr<Mouse>& mouse() { return _mouse; }
        inline std::shared_ptr<Keyboard>& keyboard() { return _keyboard; }



        // Scene management : Maybe this needs to be somewhere else... maybe .... a SceneManager?
        bool addScene(std::shared_ptr<IScene> scene);
        bool removeScene(std::shared_ptr<IScene> scene);


    private:
        bool _running = false;

        std::shared_ptr<MedusaConfig> _config;
        std::shared_ptr<ResourceDatabase> _resources;

        std::shared_ptr<Timing> _timing;

        std::shared_ptr<Events> _events;
        std::shared_ptr<IWindow> _window;
        std::shared_ptr<IContext> _context;

        // Input :: Could be an array, as some of these are optional depending on the platform
        std::shared_ptr<Mouse> _mouse;
        std::shared_ptr<Keyboard> _keyboard;

        std::shared_ptr<IApplication> _application;
        std::vector<std::shared_ptr<IScene>> _scenes;
    };
}
