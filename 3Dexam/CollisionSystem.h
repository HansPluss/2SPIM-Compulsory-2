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
        // Access relevant components from ballEntity and planeEntity
        auto* positionComponent = ballEntity.GetComponent<PositionComponent>();
        auto* velocityComponent = ballEntity.GetComponent<VelocityComponent>();
        auto* ballRenderComponent = ballEntity.GetComponent<RenderComponent>();
        auto* planePositionComponent = planeEntity.GetComponent<PositionComponent>();
        auto* planeRenderComponent = planeEntity.GetComponent<RenderComponent>();
        if (ballEntity.isMarkedForDeletion) return;
        if (!positionComponent || !velocityComponent || !ballRenderComponent || !planePositionComponent || !planeRenderComponent) {
            return; // Ensure all components exist
        }

        glm::vec3 point = positionComponent->position;
        glm::vec3 ballSize = ballRenderComponent->size;
        std::vector<Vertex>& planeVertices = planeRenderComponent->vertices;
        double groundThreshold = ballSize.y;

        // If the plane has no vertices, return early
        if (planeVertices.empty()) {
            return;
        }

        for (int i = 0; i < planeRenderComponent->Draw.GetIndices().size(); i += 3) {
            int index0 = planeRenderComponent->Draw.GetIndices()[i];
            int index1 = planeRenderComponent->Draw.GetIndices()[i + 1];
            int index2 = planeRenderComponent->Draw.GetIndices()[i + 2];

            glm::vec3 v0 = glm::vec3(
                (planeVertices[index0].x * planeRenderComponent->size.x) + planePositionComponent->position.x,
                (planeVertices[index0].y * planeRenderComponent->size.y) + planePositionComponent->position.y,
                (planeVertices[index0].z * planeRenderComponent->size.z) + planePositionComponent->position.z);

            glm::vec3 v1 = glm::vec3(
                (planeVertices[index1].x * planeRenderComponent->size.x) + planePositionComponent->position.x,
                (planeVertices[index1].y * planeRenderComponent->size.y) + planePositionComponent->position.y,
                (planeVertices[index1].z * planeRenderComponent->size.z) + planePositionComponent->position.z);

            glm::vec3 v2 = glm::vec3(
                (planeVertices[index2].x * planeRenderComponent->size.x) + planePositionComponent->position.x,
                (planeVertices[index2].y * planeRenderComponent->size.y) + planePositionComponent->position.y,
                (planeVertices[index2].z * planeRenderComponent->size.z) + planePositionComponent->position.z);

            glm::vec3 v0v1 = v1 - v0;
            glm::vec3 v0v2 = v2 - v0;
            glm::vec3 v0p = point - v0;

            // Compute dot products
            double dot00 = glm::dot(v0v1, v0v1);
            double dot01 = glm::dot(v0v1, v0v2);
            double dot02 = glm::dot(v0v1, v0p);
            double dot11 = glm::dot(v0v2, v0v2);
            double dot12 = glm::dot(v0v2, v0p);

            // Compute barycentric coordinates
            double invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
            double v = (dot11 * dot02 - dot01 * dot12) * invDenom;
            double w = (dot00 * dot12 - dot01 * dot02) * invDenom;
            double u = 1 - v - w;

            // If the point is inside the triangle (u, v, w > 0)
            if (u >= 0 && v >= 0 && w >= 0) {
                double height = v0.y * u + v1.y * v + v2.y * w;
                // Adjust position and velocity if ball is near the 
                glm::vec3 currentVelocity = velocityComponent->velocity;
                
                

                if (positionComponent->position.y < height + groundThreshold) {
                    // Stop downward motion and apply corrective force
                    if (currentVelocity.y < 0) {
                        currentVelocity.y = 0.0f; // Stop downward motion
                    }
                    
                    // Apply upward force to counteract gravity
                    physicsSystem.ApplyForce(ballEntity, glm::vec3(0.0f, 9.81f, 0.0f));
                    velocityComponent->velocity = currentVelocity;

                    // Apply corrective force if sinking
                    if (positionComponent->position.y < height + groundThreshold) 
                    {
                        positionComponent->position.y = height + groundThreshold;
                        //std::cout << "height+thresh: " << height << " | position.y " << positionComponent->position.y << std::endl;
                    }

                    // Calculate the normal vector for the slope
                    glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));
                    if (glm::length(normal) == 0.0f) continue; // Skip degenerate triangles

                    float inclineAngle = std::acos(normal.y);
                    glm::vec3 slopeVector = glm::normalize(glm::vec3(normal.x, 0.0f, normal.z)); // Slope direction

                    // Adjust velocity based on slope incline
                    float speedAdjustment = glm::dot(currentVelocity, slopeVector);
                    if (currentVelocity.y > 0) { // Ball is moving upward
                        currentVelocity.y -= speedAdjustment * sin(inclineAngle);
                      
                        // Ensure ball doesn't go through the floor
                        if (positionComponent->position.y < height + groundThreshold) {
                            positionComponent->position.y = height + groundThreshold;
                           
                            //std::cout << "gravity!!" << std::endl;
                            currentVelocity.y = 0; // Stop upward motion
                            
                        }
                    }
                    else if (currentVelocity.y < 0) { // Ball is moving downward
                        currentVelocity.y += speedAdjustment * sin(inclineAngle);

                        // Ensure ball doesn't go through the floor
                        if (positionComponent->position.y < height + groundThreshold) {
                            positionComponent->position.y = height + groundThreshold;
                        
                            currentVelocity.y = 0; // Stop downward motion
                            
                        }
                    }

                    velocityComponent->velocity = currentVelocity;
                    

                    // Calculate gravity effect along the slope and apply force
                    if (glm::length(slopeVector) > 0.00000001f) {
                        glm::vec3 gravityAlongSlope = physicsSystem.CalculateGravity(inclineAngle, slopeVector, normal);
                        physicsSystem.ApplyForce(ballEntity, gravityAlongSlope);
                        //
                    }
                   
                }

                return; // Exit after processing the first intersecting triangle
            }
        }
    }



	bool InvAABBCollision(Entity& entityA, Entity& entityB, float deltaTime) {
        if (entityA.isMarkedForDeletion || entityB.isMarkedForDeletion) return false;
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
        auto* velocityA = entityA.GetComponent<VelocityComponent>();
        auto* velocityB = entityB.GetComponent<VelocityComponent>();
        auto* positionB = entityB.GetComponent<PositionComponent>();


        glm::vec3 sizeA = entityA.GetComponent<RenderComponent>()->size;
        glm::vec3 sizeB = entityB.GetComponent<RenderComponent>()->size;

        // Predict positions using velocities
        glm::vec3 futurePosA = positionA->position + velocityA->velocity * deltaTime;
        glm::vec3 futurePosB = positionB->position + velocityB->velocity * deltaTime;

        // Calculate the distance between the centers of both objects
        float distanceCenters = glm::length(futurePosA - futurePosB);

        // Check if the distance is less than the sum of the radii (collision detection)
        if (distanceCenters <= (sizeA.x + sizeB.x)) { // Assuming size.x is the radius for both

            // Calculate the minimum translation vector to resolve penetration
            float minimumTranslation = sizeA.x + sizeB.x - distanceCenters;
            glm::vec3 directionVec = glm::normalize(positionA->position - positionB->position);

            // Move objA out of collision
            positionA->position += directionVec * minimumTranslation;

            // Handle the response (you may update velocities, handle physics, etc.)
            //ObjectCollisionResponse(objA, objB);

            return true;
        }

        // No collision
        return false;
    }
};