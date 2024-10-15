#pragma once
#include "Entity.h"
#include "PhysicsSystem.h"
class Enemy : public Entity {
public:
    
    Enemy();
    void FollowEntity(Entity& follower, Entity& target, PhysicsSystem& physicssystem);

};