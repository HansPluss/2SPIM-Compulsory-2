#pragma once
#include "Entity.h"
#include "Collision.h"
#include "PhysicsSystem.h"
class CollisionSystem {
public:
	Collision collisionHandler;
    PhysicsSystem physicsSystem;
    RenderingSystem render;
	void AABBCollision(Entity& entityA, std::vector<Entity> entities) {
		auto* collisionComponentA = entityA.GetComponent<CollisionComponent>();
		auto* positionComponentA = entityA.GetComponent<CollisionComponent>();

		if (collisionComponentA && positionComponentA) {

		}

	}
    void BarycentricCoordinates(Entity& ballEntity, Entity& planeEntity, PhysicsSystem& physicsSystem) {
        // Accessing relevant components from ballEntity and planeEntity
        auto* positionComponent = ballEntity.GetComponent<PositionComponent>();
        auto* velocityComponent = ballEntity.GetComponent<VelocityComponent>();
        auto* ballRenderComponent = ballEntity.GetComponent<RenderComponent>();
        auto* planePositionComponent = planeEntity.GetComponent<PositionComponent>();
        auto* planeRenderComponent = planeEntity.GetComponent<RenderComponent>();

        if (!positionComponent || !velocityComponent || !ballRenderComponent || !planePositionComponent || !planeRenderComponent) {
            return; // Ensuring components exist
        }

        glm::vec3 point = positionComponent->position;
        glm::vec3 ballSize = ballRenderComponent->size;
        std::vector<Vertex>& planeVertices = planeRenderComponent->vertices;
        float groundThreshold = ballSize.y;

        // If the plane has no vertices, return early
        if (planeVertices.empty()) {
            return;
        }

        for (int i = 0; i < planeVertices.size() - 2; ++i) {
            // Getting vertices of the triangle
            glm::vec3 v0 = glm::vec3((planeVertices[i].x * planeRenderComponent->size.x) + planePositionComponent->position.x,
                (planeVertices[i].y * planeRenderComponent->size.y) + planePositionComponent->position.y,
                (planeVertices[i].z * planeRenderComponent->size.z) + planePositionComponent->position.z);

            glm::vec3 v1 = glm::vec3((planeVertices[i + 1].x * planeRenderComponent->size.x) + planePositionComponent->position.x,
                (planeVertices[i + 1].y * planeRenderComponent->size.y) + planePositionComponent->position.y,
                (planeVertices[i + 1].z * planeRenderComponent->size.z) + planePositionComponent->position.z);

            glm::vec3 v2 = glm::vec3((planeVertices[i + 2].x * planeRenderComponent->size.x) + planePositionComponent->position.x,
                (planeVertices[i + 2].y * planeRenderComponent->size.y) + planePositionComponent->position.y,
                (planeVertices[i + 2].z * planeRenderComponent->size.z) + planePositionComponent->position.z);

            glm::vec3 v0v1 = v1 - v0;
            glm::vec3 v0v2 = v2 - v0;
            glm::vec3 v0p = point - v0;

            // Computing dot products
            float dot00 = glm::dot(v0v1, v0v1);
            float dot01 = glm::dot(v0v1, v0v2);
            float dot02 = glm::dot(v0v1, v0p);
            float dot11 = glm::dot(v0v2, v0v2);
            float dot12 = glm::dot(v0v2, v0p);

            // Computing barycentric coordinates
            float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
            float v = (dot11 * dot02 - dot01 * dot12) * invDenom;
            float w = (dot00 * dot12 - dot01 * dot02) * invDenom;
            float u = 1 - v - w;

            // If the point is inside the triangle (u, v, w > 0)
            if (u >= 0 && v >= 0 && w >= 0) {
                // Calculate the height at the current point
                float height = v0.y * u + v1.y * v + v2.y * w;

                // If the ball is near the ground, adjust its position and velocity
                if (positionComponent->position.y < height + groundThreshold) {
                    // Stopping downward motion and apply corrective force to prevent sinking
                    if (velocityComponent->velocity.y < 0) {
                        velocityComponent->velocity.y = 0.0f; // Stopping downward motion
                    }

                    positionComponent->position.y = height + groundThreshold;

                    // Applying small upward force if it's sinking
                    if (positionComponent->position.y < height + groundThreshold - 0.1) {
                        physicsSystem.ApplyForce(ballEntity, glm::vec3(0.0f, 9.86f, 0.0f)); // Corrective force
                    }

                    // Calculating the slope's normal vector
                    glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));
                    if (glm::length(normal) == 0.0f) continue; // Skipping degenerate triangles

                    float inclineAngle = std::acos(normal.y);
                    glm::vec3 slopeVector = glm::normalize(glm::vec3(normal.x, 0.0f, normal.z)); // Horizontal direction of the slope

                    glm::vec3 currentVelocity = velocityComponent->velocity;

                    // Adjusting velocity based on the slope
                    if (currentVelocity.y > 0) { // Ball is moving upward
                        float speedAdjustment = glm::dot(currentVelocity, slopeVector);
                        currentVelocity.y -= speedAdjustment * sin(inclineAngle); // Reducing upward velocity

                        if (positionComponent->position.y < height + groundThreshold) {
                            positionComponent->position.y = height + groundThreshold;
                            currentVelocity.y = 0; // Stopping upward motion
                        }
                    }
                    else if (currentVelocity.y < 0) { // Ball is moving downward
                        float speedAdjustment = glm::dot(currentVelocity, slopeVector);
                        currentVelocity.y += speedAdjustment * sin(inclineAngle); // Increasing downward speed

                        if (positionComponent->position.y < height + groundThreshold) {
                            positionComponent->position.y = height + groundThreshold;
                            currentVelocity.y = 0; // Stopping downward motion
                        }
                    }

                    velocityComponent->velocity = currentVelocity;

                    // Calculating gravity along the slope and apply it
                    glm::vec3 gravityAlongSlope = physicsSystem.CalculateGravity(inclineAngle, slopeVector, normal);
                    physicsSystem.ApplyForce(ballEntity, gravityAlongSlope);
                }
               
                return; // Exiting after processing the first intersecting triangle
            }
        }

        // TESTING STUFF
        // Final correction to ensure the ball doesn't sink below the ground
        //positionComponent->position.y = groundThreshold;
    }


	bool InvAABBCollision(Entity& entityA, Entity& entityB, float deltaTime) {

		auto* positionA = entityA.GetComponent<PositionComponent>();
		auto* velocityB = entityB.GetComponent<VelocityComponent>();
		auto* positionB = entityB.GetComponent<PositionComponent>();
		glm::vec3 sizeA = entityA.GetComponent<RenderComponent>()->size;
		glm::vec3 sizeB = entityB.GetComponent<RenderComponent>()->size;
        // Ensuring all required components are present
       

        // Extracting the velocity of entity B
        glm::vec3 velocity = velocityB->velocity;

        // Calculating the new position of entity B based on its velocity and deltaTime
        glm::vec3 newPosB = positionB->position + velocity * deltaTime;

        // Checking collision on the X-axis
        if (abs(newPosB.x - positionA->position.x) > (sizeA.x - sizeB.x)) {
            // Inverting X velocity
            velocity.x *= -1.0f;
        }

        // Checking collision on the Z-axis
        if (abs(newPosB.z - positionA->position.z) > (sizeA.z - sizeB.z)) {
            // Inverting Z velocity
            velocity.z *= -1.0f;
        }

        // Updating velocity component of entity B
        velocityB->velocity = velocity;

        // Updating angular velocity if the component exists
        auto* angularVelocityB = entityB.GetComponent<AngularVelocityComponent>();
        if (angularVelocityB) {
            angularVelocityB->angularvelocity = velocity;
        }

        return true;
	}
    bool SphereCollision(Entity& entityA, Entity& entityB, float deltaTime) {
        auto* positionA = entityA.GetComponent<PositionComponent>();
        auto* velocityB = entityB.GetComponent<VelocityComponent>();
        auto* positionB = entityB.GetComponent<PositionComponent>();
    }
};