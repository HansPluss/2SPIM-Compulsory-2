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
class AccelerationComponent : public Component {

    glm::vec3 acceleration;

    AccelerationComponent(float ax = 0, float ay = 0, float az = 0)
        : acceleration(ax, ay, az) {}
};
class HealthComponent : public Component {
    float health;
    HealthComponent(float hp = 100)
        : health(hp){}

};
class DamageComponent : public Component {
    float damage;
    DamageComponent(float dmg = 10)
        : damage(dmg){}

};
class RenderComponent : public Component {
public:
    glm::vec3 color;
    glm::vec3 size;
    std::string shape; // Could be "cube", "plane", "sphere", etc.

    RenderComponent(const glm::vec3& color, const glm::vec3& size, const std::string& shape)
        : color(color), size(size), shape(shape) {}
};
class CollisionComponent : public Component {
public:
    glm::vec3 size;
    float radius;
    bool isColliding = false;
    CollisionComponent(glm::vec3& size, float radius) 
        : size(size), radius(radius) {


    }

};
class AIComponent : public Component {
public:
    float speed;
    float detectionRadius;

    AIComponent(float speed, float radius)
        : speed(speed), detectionRadius(radius) {


    }

};