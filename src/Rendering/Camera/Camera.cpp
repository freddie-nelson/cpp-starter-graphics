#include "../../../include/Rendering/Camera/Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <stdexcept>
#include <iostream>

Rendering::Camera::Camera(float width, float height, float near, float far)
    : width(width), height(height), near(near), far(far)
{
    setWidth(width);
    setHeight(height);
    setNear(near);
    setFar(far);
    updateAABB();
}

void Rendering::Camera::setWidth(float width)
{
    if (width <= 0)
        throw std::invalid_argument("Width cannot be less than or equal to 0.");

    this->width = width;
    updateAABB();
}

float Rendering::Camera::getWidth() const
{
    return width;
}

void Rendering::Camera::setHeight(float height)
{
    if (height <= 0)
        throw std::invalid_argument("Height cannot be less than or equal to 0.");

    this->height = height;
    updateAABB();
}

float Rendering::Camera::getHeight() const
{
    return height;
}

void Rendering::Camera::setNear(float near)
{
    if (near < 0)
        throw std::invalid_argument("Near cannot be less than 0.");
    if (near > far)
        throw std::invalid_argument("Near cannot be greater than far.");

    this->near = near;
}

float Rendering::Camera::getNear() const
{
    return near;
}

void Rendering::Camera::setFar(float far)
{
    if (far < 0)
        throw std::invalid_argument("Far cannot be less than 0.");
    if (far < near)
        throw std::invalid_argument("Far cannot be less than near.");

    this->far = far;
}

float Rendering::Camera::getFar() const
{
    return far;
}

void Rendering::Camera::setViewportSize(float width, float height)
{
    setWidth(width);
    setHeight(height);
}

glm::vec2 Rendering::Camera::getViewportSize() const
{
    return glm::vec2(width, height);
}

glm::mat4 Rendering::Camera::getViewProjectionMatrix(const Core::Transform &t) const
{
    glm::vec3 centre = glm::vec3(t.getTranslation(), -t.getZIndex());
    glm::vec3 up = glm::rotate(glm::vec3(0.0f, 1.0f, 0.0f), t.getRotation(), glm::vec3(0.0f, 0.0f, 1.0f));

    auto min = aabb.getMin() * t.getScale();
    auto max = aabb.getMax() * t.getScale();

    // camera is at 0.0f on the z axis looking towards z = far (looks towards positive z)
    glm::mat4 viewMatrix = glm::lookAt(centre, glm::vec3(centre.x, centre.y, far + 1.0f), up);
    glm::mat4 projectionMatrix = glm::ortho(min.x, max.x, min.y, max.y, near, far);

    return projectionMatrix * viewMatrix;
}

const Core::AABB &Rendering::Camera::getAABB() const
{
    return aabb;
}

Core::AABB Rendering::Camera::getScaledAndTranslatedAabb(const Core::Transform &t) const
{
    auto min = aabb.getMin() * t.getScale();
    auto max = aabb.getMax() * t.getScale();

    return Core::AABB(min + t.getTranslation(), max + t.getTranslation());
}

void Rendering::Camera::updateAABB()
{
    aabb.setMin(glm::vec2(-width / 2.0f, -height / 2.0f));
    aabb.setMax(glm::vec2(width / 2.0f, height / 2.0f));
}