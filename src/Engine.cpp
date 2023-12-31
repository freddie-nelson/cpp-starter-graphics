#include "../include/Engine.h"
#include "../include/Config.h"

blz::Engine::Engine(EngineConfig config)
{
    this->config = config;
    Config::MAX_Z_INDEX = config.maxZIndex;

    window = new Core::Window(config.windowTitle, config.windowWidth, config.windowHeight, config.windowFullscreen);
    window->init(config.openglMajorVersion, config.openglMinorVersion);
    addSystem(window);

    renderer = new Rendering::Renderer(window->getGLFWWindow(), config.windowWidth, config.windowHeight);
    renderer->init();
    addSystem(renderer);

    registry = new ECS::Registry();

    mouse = new Input::Mouse(window->getGLFWWindow());
    keyboard = new Input::Keyboard(window->getGLFWWindow());
}

blz::Engine::~Engine()
{
    delete keyboard;
    delete mouse;
    delete registry;
    delete renderer;
    delete window;
}

void blz::Engine::run()
{
    // std::cout << "Running engine..." << std::endl;

    const double timeBetweenFixedUpdatesSeconds = 1.0 / config.fixedUpdatesPerSecond;
    const Core::Time timeBetweenFixedUpdates = timeBetweenFixedUpdatesSeconds * 1000.0 * 1000.0;

    Core::Time timeSinceLastFixedUpdate = 0;

    const double timeBetweenUpdatesSeconds = 1.0 / config.updatesPerSecond;
    const Core::Time timeBetweenUpdates = timeBetweenUpdatesSeconds * 1000.0 * 1000.0;

    Core::Time timeSinceLastUpdate = 0;

    std::cout << "Time between fixed updates: " << timeBetweenFixedUpdates << std::endl;
    std::cout << "Time between updates: " << timeBetweenUpdates << std::endl;

    // create timestep at current time
    Core::Timestep tick;

    // poll for events before first update
    glfwPollEvents();

    while (true)
    {
        timeSinceLastFixedUpdate += tick.getMicroseconds();
        timeSinceLastUpdate += tick.getMicroseconds();

        // run fixed updates
        if (timeSinceLastFixedUpdate >= timeBetweenFixedUpdates)
        {
            Core::Timestep fixedTimestep(0);
            fixedTimestep.update(timeBetweenFixedUpdates);

            for (auto system : systems)
            {
                system->fixedUpdate(*registry, fixedTimestep);
            }

            timeSinceLastFixedUpdate = 0;
        }

        // run updates
        if (timeSinceLastUpdate >= timeBetweenUpdates)
        {
            Core::Timestep timestep(0);
            timestep.update(timeSinceLastUpdate);

            // clear renderer
            renderer->clear();

            for (auto system : systems)
            {
                system->update(*registry, timestep);
            }

            // present renderer
            renderer->present();

            // poll for new events after frame has been rendered
            glfwPollEvents();

            // check if window should close
            if (glfwWindowShouldClose(window->getGLFWWindow()))
            {
                break;
            }

            timeSinceLastUpdate = 0;
        }

        // update ticker
        tick.update();
    }
}

void blz::Engine::addSystem(ECS::System *system)
{
    systems.push_back(system);
}

bool blz::Engine::removeSystem(ECS::System *system)
{
    bool removed = false;

    for (auto it = systems.begin(); it != systems.end(); it++)
    {
        if (*it == system)
        {
            systems.erase(it);
            removed = true;
            break;
        }
    }

    return removed;
}

blz::EngineConfig &blz::Engine::getConfig()
{
    return config;
}

Core::Window *const blz::Engine::getWindow()
{
    return window;
}

Rendering::Renderer *const blz::Engine::getRenderer()
{
    return renderer;
}

ECS::Registry *const blz::Engine::getRegistry()
{
    return registry;
}

Input::Mouse *const blz::Engine::getMouse()
{
    return mouse;
}

Input::Keyboard *const blz::Engine::getKeyboard()
{
    return keyboard;
}