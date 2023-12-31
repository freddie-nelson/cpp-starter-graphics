#pragma once

#include <blaze++/Rendering/Renderer.h>
#include <blaze++/Rendering/Window.h>
#include <blaze++/ECS/Registry.h>
#include <blaze++/Engine.h>

#include <string>

class Application : public ECS::System
{
public:
    Application();
    ~Application();

    void run();

private:
    blz::Engine *engine;

    void init();
    void destroy();

    void update(const ECS::Registry &registry, const Core::Timestep &timestep);
};