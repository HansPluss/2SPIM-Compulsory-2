#pragma once

#include "RigidBody.h"
#include "Entity.h"
class PhysicsSystem {
public:
	RigidBody rigidBody;
	void Update(Entity& entity, float deltaTime) {
		if (entity.isMarkedForDeletion) return;
		auto* positionComponent = entity.GetComponent<PositionComponent>();
		auto* velocityComponent = entity.GetComponent<VelocityComponent>();
		auto* accelerationComponent = entity.GetComponent<AccelerationComponent>();

		if (positionComponent && velocityComponent && accelerationComponent) {

			rigidBody.Update(*positionComponent, *velocityComponent, *accelerationComponent, deltaTime);
		}
	}
	void UpdatePositions(PositionStorage& storage, std::vector<Entity*>& entityList, float deltatime) {
		// Ensure that both storage and entityList have the same size
		size_t count = std::min(storage.positions.size(), entityList.size());

		for (size_t i = 0; i < storage.positions.size(); ++i) {
			// Update the position directly in PositionStorage
			storage.positions[i] += glm::vec3(1.0f, 0.0f, 0.0f) * deltatime;

			// Access the entity via pointer in the entityList
			if (PositionComponent* posComp = entityList[i]->GetComponent<PositionComponent>()) {
				// Update the PositionComponent with the new position
				posComp->SetPosition(storage.positions[i]);
			}
		}
	}
	void ApplyForce(Entity& entity, glm::vec3 force) {
		
		if (entity.isMarkedForDeletion) return;
		auto* accelerationComponent = entity.GetComponent<AccelerationComponent>();
		if (accelerationComponent) {
			
			rigidBody.ApplyForce(*accelerationComponent, force);
		}
	}
	glm::vec3 CalculateGravity(float inclineAngle, glm::vec3 slopeVector, glm::vec3 normal) {
		

		return rigidBody.CalculateGravity(inclineAngle,slopeVector,normal);
	}

};