#include "engine.h"

#include <medusa/app.h>
#include <medusa/renderer/scene.h>

#include <core/utilities/logging.h>
#include <core/utilities/timing.h>
#include <engine/config.h>
#include <engine/events.h>
#include <engine/window.h>
#include <engine/resource_database.h>
#include <engine/input/mouse.h>
#include <engine/input/keyboard.h>
#include <opengl/context_gl.h>

#include <engine/resources/asset_manager.h>


using namespace medusa;


//
Engine::Engine()
{
    logging::info("Launch Application");

    // Config
    _config = std::make_shared<MedusaConfig>();

    // Events
    _events = std::make_shared<Events>();

    // Timing
    _timing = std::make_shared<Timing>();

    // Graphics context
    _context = std::make_shared<opengl::ContextGL>(_config);
    _window = _context->window();

    // Create Resource Database
    _assets = std::make_shared<AssetManager>(_context);
    _resources = std::make_shared<ResourceDatabase>(_context);
    _resources->getShader("basic");

    // compute context

    // Input Devices
    _keyboard = std::make_shared<Keyboard>();
    _mouse = std::make_shared<Mouse>();

    // Register system event handlers
    _events->registerHandler(_window);

    // Register input event handlers
    _events->registerHandler(_keyboard);
    _events->registerHandler(_mouse);


    // Register Window Events
    _window->onClose.connect([this](CloseEvent&) {this->quit(); });

    // Register timing events [temp]
    _timing->onSecond.connect([](int fps) {logging::debug("FPS: {0}", fps); });
    _timing->onSecond.connect([this](int fps) {this->window()->setTitle(std::format("Medusa: {0}", fps)); });
}


//
Engine::~Engine()
{
    logging::info("Quit Application");

}


//
bool Engine::registerDirectory(const std::string& path)
{
    assert(_assets != nullptr);
    return _assets->registerDirectory(path);
}


//
void Engine::run(std::shared_ptr<IApplication> application)
{
    _application = application;

    this->_running = true;
    while (this->_running)
    {
        if (_events->poll() == false)
            break;

        // Update the timing
        _timing->update();

        // Render
        _context->next();

        // Update the application :: This is NOT how it's supposed to look
        _application->update();
        _application->render();

        for (auto& scene : _scenes)
        {
            scene->update(_timing->frametime());
        }


        for (auto& scene : _scenes)
        {
            scene->render();
        }


        _context->present();
    }
}

void Engine::quit()
{
    logging::info("event quitting");
    this->_running = false;
}




bool Engine::addScene(std::shared_ptr<IScene> scene)
{
    _scenes.push_back(scene);
    return true;
}

bool Engine::removeScene(std::shared_ptr<IScene> scene)
{
    auto it = std::find(_scenes.begin(), _scenes.end(), scene);

    if (it == _scenes.end())
        return false;

    _scenes.erase(it);

    return true;
}
