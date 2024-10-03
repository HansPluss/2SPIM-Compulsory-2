#pragma once
#include <glm/glm.hpp>


// TO DO ADD MORE COMPONENTS LIKE HEALTH, AI, DAMAGE ETC...
class Component {
public:
    virtual ~Component() = default;
};

class PositionComponent : public Component {
public:
    glm::vec3 position;

    PositionComponent(float x = 0, float y = 0, float z = 0)
        : position(x, y, z) {}
};
class VelocityComponent : public Component {
    glm::vec3 velocity;

    VelocityComponent(float vx = 0, float vy = 0, float vz = 0)
        : velocity(vx, vy, vz) {}
}; 
class RenderComponent : public Component {
public:
    glm::vec3 color;
    glm::vec3 size;
    std::string shape; // Could be "cube", "plane", "sphere", etc.

    RenderComponent(const glm::vec3& color, const glm::vec3& size, const std::string& shape)
        : color(color), size(size), shape(shape) {}
};