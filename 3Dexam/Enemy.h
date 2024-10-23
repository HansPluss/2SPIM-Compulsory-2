#pragma once
#include "Entity.h"
#include "PhysicsSystem.h"
#include "Player.h"
class EntityManager;
class RenderingSystem;
class Enemy : public Entity {
public:
    
    Enemy();
    //Follows the target specifed, very basic
    void FollowEntity(Entity& target, const std::shared_ptr<PhysicsSystem>& physicssystem, AccelerationStorage& aStorage);
    // Upon death the enemy will spawn an item
    void Death(const std::shared_ptr<EntityManager>&  manager, std::vector<Entity*>& entityList, const std::shared_ptr<RenderingSystem>& rendersystem, PositionStorage& storage, AccelerationStorage& aStorage, VelocityStorage& vStorage);
private:
    Entity* playerref;
    bool dead;
};