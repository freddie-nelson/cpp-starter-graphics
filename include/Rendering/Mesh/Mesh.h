#pragma once

#include "../Color.h"

#include <glm/vec2.hpp>
#include <glm/mat2x2.hpp>
#include <vector>

namespace Rendering
{
    /**
     * A 2D mesh.
     *
     * This mesh must be ready to be rendered by OpenGL.
     *
     * A Mesh2D should be rendered using the GL_TRIANGLES primitive.
     *
     * A Mesh2D must represent a simple polygon.
     *
     * A Mesh2D's vertices must be in counter-clockwise order.
     *
     * The mesh is transformed by multiplying each vertex by the transform matrix and then adding the translation vector.
     *
     * The transform matrix is the combined rotation and scale matrix. See [here](https://en.wikipedia.org/wiki/Transformation_matrix#Examples_in_2_dimensions) for an explanation.
     *
     * The default color given to the mesh is (1.0, 1.0, 1.0, 1.0) or fully opaque white.
     *
     * The default zIndex is 0. A higher zIndex means the mesh will be rendered above meshes with a lower zIndex.
     * A higher zIndex is 'closer' to the camera. 0 is the lowest zIndex and will be rendered below everying, whereas `Config::MAX_Z_INDEX` is the largest z index and will be rendered above everything.
     */
    struct Mesh2D
    {
        std::vector<glm::vec2> vertices;
        std::vector<unsigned int> indices;

        unsigned int zIndex = 0;
        glm::vec2 translation = glm::vec2(0.0f);
        glm::mat2 scale = glm::mat2(1.0f);
        glm::mat2 rotation = glm::mat2(1.0f);
        glm::mat2 sheer = glm::mat2(1.0f);

        Color color = Color(1.0f);
    };
}
