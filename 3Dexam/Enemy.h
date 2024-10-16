#pragma once
#include "Entity.h"
#include "PhysicsSystem.h"
#include "Player.h"
class EntityManager;
class RenderingSystem;
class Enemy : public Entity {
public:
    
    Enemy();
    void FollowEntity(Entity& target, PhysicsSystem& physicssystem);
    void Death(EntityManager& manager, std::vector<Entity*>& entityList, RenderingSystem& rendersystem);
private:
    Entity* playerref;
};