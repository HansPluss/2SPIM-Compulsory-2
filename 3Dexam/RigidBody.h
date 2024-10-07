#pragma once
#include "glm/glm.hpp"
class RigidBody
{
private:

	float gravity = -9.81;
	float mass = 1.0f;
	glm::vec3 velocity; 
	glm::vec3 Acceleration;

public: 

	//void CalculateGravity(float inclineAngle, glm::vec3 slopeVector, glm::vec3 normal);
	//void ApplyForce(glm::vec3 force);
	//void Update(float deltaTime);

	//float GetGravity() { return gravity; };

	//void SetGravity(float newGravity) { gravity = newGravity; }
};

