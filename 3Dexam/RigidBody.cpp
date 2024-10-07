#include "RigidBody.h"

//void RigidBody::CalculateGravity(float inclineAngle, glm::vec3 slopeVector, glm::vec3 normal)
//{
//    // Downward gravity force
//    glm::vec3 gravityForce(0.0f, gravity, 0.0f);
//
//    // Calculate normal force (perpendicular to the slope)
//    float normalForceMagnitude = glm::dot(gravityForce, normal); // Gravity along the normal
//    glm::vec3 normalForce = normal * normalForceMagnitude;
//
//    // Calculate gravitational force acting parallel to the slope (slope vector)
//    glm::vec3 gravityParallel = gravityForce - normalForce; // Parallel force along the slope
//
//    // Project this parallel gravity onto the slope's horizontal direction (slopeVector)
//    glm::vec3 gravityAlongSlope = glm::dot(gravityParallel, slopeVector) * slopeVector;
//
//    // Apply the force along the slope
//    ApplyForce(gravityAlongSlope);
//}
//
//void RigidBody::ApplyForce(glm::vec3 force)
//{
//    Acceleration += force / mass; 
//}
//
//void RigidBody::Update(float deltaTime)
//{
//    velocity += Acceleration * deltaTime;
//   // position += velocity * deltaTime;                 // need
//    Acceleration = glm::vec3(0.0f, gravity, 0.0f);
//}
