#include "RigidBody.h"

glm::vec3 RigidBody::CalculateGravity(float inclineAngle, glm::vec3 slopeVector, glm::vec3 normal)
{
    // Downward gravity force
    
    slopeVector = glm::normalize(slopeVector);
    
    
    glm::vec3 gravityForce(0.0f, gravity, 0.0f);

    // Calculate normal force (perpendicular to the slope)
    float normalForceMagnitude = glm::dot(gravityForce, normal); // Gravity along the normal
    glm::vec3 normalForce = normal * normalForceMagnitude;

    // Calculate gravitational force acting parallel to the slope (slope vector)
    glm::vec3 gravityParallel = gravityForce - normalForce; // Parallel force along the slope

    // Project this parallel gravity onto the slope's horizontal direction (slopeVector)
    glm::vec3 gravityAlongSlope = glm::dot(gravityParallel, slopeVector) * slopeVector;

    // Apply the force along the slope
    return gravityAlongSlope;
}

void RigidBody::ApplyForce(AccelerationComponent& aComponent, glm::vec3 force)
{
    aComponent.acceleration += force / 1.0f; // mass to be added in next update 
}

void RigidBody::Update(PositionComponent& pComponent, VelocityComponent& vComponent, AccelerationComponent& aComponent, float deltaTime)
{
    vComponent.velocity += aComponent.acceleration * deltaTime;
    pComponent.position += vComponent.velocity * deltaTime;               
    aComponent.acceleration = glm::vec3(0.0f, gravity, 0.0f);
}
