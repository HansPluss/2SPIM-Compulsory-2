#pragma once

#include "RigidBody.h"
#include "Entity.h"
class PhysicsSystem {
public:
	RigidBody rigidBody;
	void Update(Entity& entity,float deltaTime) {
		auto* positionComponent = entity.GetComponent<PositionComponent>();
		auto* velocityComponent = entity.GetComponent<VelocityComponent>();
		auto* accelerationComponent = entity.GetComponent<AccelerationComponent>();
		if (positionComponent && velocityComponent && accelerationComponent) {
			rigidBody.Update(*positionComponent, *velocityComponent, *accelerationComponent, deltaTime);

		}
	}
	void ApplyForce(Entity& entity, glm::vec3 force) {
		
		auto* accelerationComponent = entity.GetComponent<AccelerationComponent>();
		if (accelerationComponent) {

			rigidBody.ApplyForce(*accelerationComponent, force);
		}
	}
	glm::vec3 CalculateGravity(float inclineAngle, glm::vec3 slopeVector, glm::vec3 normal) {

		return rigidBody.CalculateGravity(inclineAngle,slopeVector,normal);
	}

};