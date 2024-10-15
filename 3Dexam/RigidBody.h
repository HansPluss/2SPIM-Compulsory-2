#pragma once
#include "glm/glm.hpp"
#include "Component.h"
class RigidBody
{
private:

	float gravity = -9.81f;
	float mass = 1.0f;
	glm::vec3 velocity; 
	glm::vec3 Acceleration;
	glm::vec3 position;
public: 

	glm::vec3 CalculateGravity(float inclineAngle, glm::vec3 slopeVector, glm::vec3 normal);
	void ApplyForce(AccelerationComponent& aComponent, glm::vec3 force);
	void Update(PositionComponent& position, VelocityComponent& velocity, AccelerationComponent& acceleration, float deltaTime);

	//float GetGravity() { return gravity; };

	//void SetGravity(float newGravity) { gravity = newGravity; }
};

