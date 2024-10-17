#pragma once
#include "Entity.h"
#include "PhysicsSystem.h"
#include "Player.h"
class EntityManager;
class RenderingSystem;
class Enemy : public Entity {
public:
    
    Enemy();
    void FollowEntity(Entity& target, const std::shared_ptr<PhysicsSystem>& physicssystem);
    void Death(const std::shared_ptr<EntityManager>&  manager, std::vector<Entity*>& entityList, const std::shared_ptr<RenderingSystem>& rendersystem);
private:
    Entity* playerref;
};