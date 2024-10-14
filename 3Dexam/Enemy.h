#pragma once
#include "Entity.h"
#include "Component.h"
#include "PhysicsSystem.h"
class Enemy : public Entity {
public:
    float speed;
	Enemy() {
		AddComponent<PositionComponent>(0.0f, 0.0f, 0.0f);
		AddComponent<RenderComponent>(glm::vec3(0.0f), glm::vec3(1.0f), "cube");
        AddComponent<VelocityComponent>();
        AddComponent<AccelerationComponent>();
        AddComponent<AIComponent>();
	}
    void FollowEntity(Entity& follower, Entity& target, PhysicsSystem& physicssystem) {
        auto* followerPos = follower.GetComponent<PositionComponent>();
        auto* velocity = follower.GetComponent<VelocityComponent>();
        auto* targetPos = target.GetComponent<PositionComponent>();
        auto* ai = follower.GetComponent<AIComponent>();
        if (followerPos && velocity && targetPos && ai) {
            glm::vec3 direction = targetPos->position - followerPos->position;

            // Compute the distance (magnitude) between entities
            float distance = glm::length(direction);

            // Normalize the direction vector to prevent zero-length vectors
            if (distance > 0.0001f) {
                glm::vec3 dirvec = glm::normalize(direction);

                // Apply force to move the follower toward the target
                glm::vec3 force = dirvec * ai->speed;

                // Update the follower's velocity based on the force
                physicssystem.ApplyForce(follower, force);
            }
        }
    }

};