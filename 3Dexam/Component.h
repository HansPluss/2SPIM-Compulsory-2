#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "draw.h"
// TO DO ADD COMMENTS
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
public:
    glm::vec3 velocity;

    VelocityComponent(float vx = 0, float vy = 0, float vz = 0)
        : velocity(vx, vy, vz) {}
}; 
class AngularVelocityComponent : public Component {
public:
    glm::vec3 angularvelocity;

    AngularVelocityComponent(float avx = 0, float avy = 0, float avz = 0)
        : angularvelocity(avx, avy, avz) {}
};
class AccelerationComponent : public Component {
public:
    glm::vec3 acceleration;

    AccelerationComponent(float ax = 0, float ay = 0, float az = 0)
        : acceleration(ax, ay, az) {}
};
class HealthComponent : public Component {
public:
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
    glm::vec3 rotation;
    std::string shape; // Could be "cube", "plane", "sphere", etc.
    std::vector<Vertex> vertices;
    Draw Draw;
    RenderComponent(const glm::vec3& color, const glm::vec3& size, const std::string& shape, const glm::vec3& rotation = glm::vec3(0.0f,0.0f,0.0f))
        : color(color), size(size), shape(shape), rotation(rotation) {}
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
class InputComponent : public Component {
public:
    bool moveUp, moveDown, moveLeft, moveRight, bTab;

    InputComponent()
        : moveUp(false), moveDown(false), moveLeft(false), moveRight(false), bTab(false) {}

    void updateInput(bool up, bool down, bool left, bool right, bool Tab) {
        moveUp = up;
        moveDown = down;
        moveLeft = left;
        moveRight = right;
		bTab = Tab;
    }
};