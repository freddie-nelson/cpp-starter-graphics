#include "../include/Application.h"
#include "../include/Globals.h"
#include "../include/Rendering/Mesh/Polygons.h"
#include "../include/Rendering/Utility/Timestep.h"
#include "../include/ECS/Registry.h"

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

struct Position
{
    float x;
    float y;
};

struct Velocity
{
    float x;
    float y;
};

int Application::init()
{
    // init window
    window = new Rendering::Window(windowTitle, initialWindowWidth, initialWindowHeight);
    if (window->init(4, 3) != 0)
    {
        std::cout << "Failed to initialize window." << std::endl;
        return 1;
    }

    // Rendering::Color clearColor(0.0f);
    // clearColor.fromHSLA(0.82f, 0.6f, 0.45f, 1.0f);

    // auto renderer = window->getRenderer();
    // renderer->setClearColor(clearColor);

    // window->syncRendererSize(false);

    // meshs
    int meshCount = 0;
    for (int i = 0; i < meshCount; i++)
    {
        meshs.push_back(Rendering::Mesh2D(static_cast<float>(rand() % 100 + 50), static_cast<float>(rand() % 100 + 50)));

        meshs[i].setZIndex(rand() % 10);

        auto r = (rand() % 255) / 255.0f;
        auto g = (rand() % 255) / 255.0f;
        auto b = (rand() % 255) / 255.0f;
        meshs[i].setColor(Rendering::Color((meshs[i].getZIndex() + 1) / 10.0f, 0.0f, 0.0f, 1.0f));

        meshs[i].translate(glm::vec2{(rand() % initialWindowWidth) - initialWindowWidth / 2,
                                     (rand() % initialWindowHeight) - initialWindowHeight / 2});
    }

    // instanced meshs
    int instancedMeshCount = 0;
    for (int i = 0; i < instancedMeshCount; i++)
    {
        instancedMeshs.push_back(Rendering::Mesh2D(1.0f, 1.0f));

        instancedMeshs[i].setZIndex(rand() % 10);
        // instancedMeshs[i].zIndex = 65535;

        auto r = (rand() % 255) / 255.0f;
        auto g = (rand() % 255) / 255.0f;
        auto b = (rand() % 255) / 255.0f;
        instancedMeshs[i].setColor(Rendering::Color((instancedMeshs[i].getZIndex() + 1) / 10.0f, 0.0f, 0.0f, 1.0f));

        instancedMeshs[i].translate(glm::vec2((rand() % initialWindowWidth) - initialWindowWidth / 2,
                                              (rand() % initialWindowHeight) - initialWindowHeight / 2));

        instancedMeshs[i].scale(glm::vec2(rand() % 100 + 50, rand() % 100 + 50));
    }

    // batched meshs
    int batchedMeshCount = 1000;
    for (int i = 0; i < batchedMeshCount; i++)
    {
        batchedMeshs.push_back(Rendering::Mesh2D(static_cast<float>(rand() % 100 + 50), static_cast<float>(rand() % 100 + 50)));

        batchedMeshs[i].setZIndex(rand() % 10);

        auto r = (rand() % 255) / 255.0f;
        auto g = (rand() % 255) / 255.0f;
        auto b = (rand() % 255) / 255.0f;
        batchedMeshs[i].setColor(Rendering::Color((batchedMeshs[i].getZIndex() + 1) / 10.0f, 0.0f, 0.0f, 1.0f));

        batchedMeshs[i].translate(glm::vec2{(rand() % initialWindowWidth) - initialWindowWidth / 2,
                                            (rand() % initialWindowHeight) - initialWindowHeight / 2});
    }

    // ECS test
    ECS::Registry registry;

    auto e0 = registry.create();
    registry.add<Position>(e0, {0.0f, 0.0f});
    registry.add<Velocity>(e0, {1.0f, 1.0f});

    auto e1 = registry.create();
    registry.add<Position>(e1, {1.0f, 1.0f});

    auto e2 = registry.create();
    registry.add<Position>(e2, {2.0f, 2.0f});
    registry.add<Velocity>(e2, {2.0f, 2.0f});

    std::cout << "Entities with position: " << std::endl;
    auto entities = registry.view<Position>();
    for (auto &e : entities)
    {
        auto &p = registry.get<Position>(e);
        std::cout << e << ": p: " << p.x << ", " << p.y << std::endl;
    }

    std::cout << "Entities with velocity: " << std::endl;
    entities = registry.view<Velocity>();
    for (auto &e : entities)
    {
        auto &v = registry.get<Velocity>(e);
        std::cout << e << ": v: " << v.x << ", " << v.y << std::endl;
    }

    std::cout << "Entities with position and velocity: " << std::endl;
    entities = registry.view<Position, Velocity>();
    for (auto &e : entities)
    {
        std::cout << e << std::endl;
        auto &p = registry.get<Position>(e);
        auto &v = registry.get<Velocity>(e);
        std::cout << e << ": p: " << p.x << ", " << p.y << ", v: " << v.x << ", " << v.y << std::endl;
    }

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

    // move camera
    auto &camera = renderer->getCamera();

    double n = Rendering::timeSinceEpochMillisec() / 1000.0;
    float camX = sin(n) * 200.0f;
    float camY = cos(n) * 200.0f;

    camera.setCentre(glm::vec2{camX, camY});
    // camera.rotate(std::numbers::pi * 0.5f * dt);

    // rotate meshs
    for (auto &m : meshs)
    {
        m.rotate(-std::numbers::pi * 0.5f * dt);
    }

    for (auto &m : instancedMeshs)
    {
        m.rotate(-std::numbers::pi * 0.5f * dt);
    }

    for (auto &m : batchedMeshs)
    {
        m.rotate(-std::numbers::pi * 0.5f * dt);
    }

    render(renderer);
}

void Application::render(Rendering::Renderer *renderer)
{
    // auto resolution = window->getSize();

    // Rendering::Mesh2D shape = Rendering::createPolygon({glm::vec2(0, 0), glm::vec2(100, 0), glm::vec2(20, 20), glm::vec2(0, 100)});

    // Rendering::Mesh2D shape = Rendering::createPolygon({glm::vec2(0, 0.5), glm::vec2(0.5, -0.5), glm::vec2(-0.5, -0.5)});

    // Rendering::Mesh2D shape({glm::vec2(0, 0.5), glm::vec2(0.5, -0.5), glm::vec2(-0.5, -0.5)});
    // shape.scale(100);
    // renderer->mesh(shape);

    for (auto &m : meshs)
    {
        renderer->mesh(m);
    }

    if (instancedMeshs.size() > 0)
    {
        std::vector<glm::mat4> transforms(instancedMeshs.size());
        std::vector<glm::vec4> colors(instancedMeshs.size());

        int i = 0;
        for (auto &m : instancedMeshs)
        {
            transforms[i] = m.getTransformationMatrix();
            colors[i] = m.getColor().getColor();

            i++;
        }

        renderer->instancedMesh(instancedMeshs[0], transforms, colors);
    }

    if (batchedMeshs.size() > 0)
        renderer->batchedMesh(batchedMeshs);
}
