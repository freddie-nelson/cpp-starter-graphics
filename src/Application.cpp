#include "../include/Application.h"
#include "../include/Globals.h"
#include "../include/Rendering/Mesh/Polygons.h"
#include "../include/Rendering/Mesh/Transforms.h"

#include <math.h>
#include <random>
#include <time.h>
#include <iostream>
#include <chrono>
#include <thread>

Application::Application(std::string windowTitle, int windowWidth, int windowHeight) : windowTitle(windowTitle), initialWindowWidth(windowWidth), initialWindowHeight(windowHeight)
{
}

Application::~Application()
{
    destroy();
}

int Application::run()
{
    const int initCode = init();
    if (initCode != 0)
    {
        return initCode;
    }

    return window->run(std::bind(&Application::update, this, std::placeholders::_1, std::placeholders::_2));
}

int Application::init()
{
    // init window
    window = new Rendering::Window(windowTitle, initialWindowWidth, initialWindowHeight);
    if (window->init() != 0)
    {
        std::cout << "Failed to initialize window." << std::endl;
        return 1;
    }

    Rendering::Color clearColor(0.0f);
    clearColor.fromHSLA(0.82f, 0.6f, 0.45f, 1.0f);

    auto renderer = window->getRenderer();
    renderer->setClearColor(clearColor);

    return 0;
}

void Application::destroy()
{
    delete window;
    delete this;
}

void Application::update(float dt, Rendering::Renderer *renderer)
{
    // print timestep info
    std::cout << "\rdt: " << dt << "          ";

    render(renderer);
}

void Application::render(Rendering::Renderer *renderer)
{
    Rendering::Mesh2D shape = Rendering::createPolygon({glm::vec2(0, 0), glm::vec2(100, 0), glm::vec2(20, 20), glm::vec2(0, 100)});
    Rendering::translateMesh(shape, glm::vec2{initialWindowWidth / 2, initialWindowHeight / 2});

    renderer->mesh(shape, Rendering::Color(1.0f));
}
