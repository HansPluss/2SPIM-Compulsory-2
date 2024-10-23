#include "Enemy.h"
#include "Component.h"
#include "EntityManager.h"
#include "Item.h"
#include "RenderingSystem.h"
Enemy::Enemy()
{
    AddComponent<PositionComponent>(0.0f, 10.0f, -10.0f);
    AddComponent<RenderComponent>(glm::vec3(0.0f), glm::vec3(1.0f), "cube");
    AddComponent<VelocityComponent>();
    AddComponent<AccelerationComponent>();
    AddComponent<AIComponent>();
    AddComponent<PhysicsComponet>(10);
    AddComponent<HealthComponent>(50);
    AddComponent<DamageComponent>(10);

}

void Enemy::FollowEntity( Entity& target, const std::shared_ptr<PhysicsSystem>& physicssystem, AccelerationStorage& aStorage)
{
    auto* followerPos = GetComponent<PositionComponent>();
    auto* velocity = GetComponent<VelocityComponent>();
    auto* targetPos = target.GetComponent<PositionComponent>();
    auto* ai = GetComponent<AIComponent>();
  
    if (followerPos && velocity && targetPos && ai) {
        glm::vec3 direction = targetPos->position - followerPos->position;

        // Computing the distance between entities
        float distance = glm::length(direction);
        playerref = &target;
        // Normalizing the direction vector to prevent zero-length vectors
        if (distance > 0.0001f) {
            glm::vec3 dirvec = glm::normalize(direction);

            // Applying force to move the follower toward the target
            glm::vec3 force = dirvec * ai->speed;

            // Updating the follower's velocity based on the force
           // physicssystem->ApplyDODForce(aStorage, force, this->GetId());
            physicssystem->ApplyForce(*this, force);
        }
    }
}

void Enemy::Death(const std::shared_ptr<EntityManager>& manager, std::vector<Entity*>& entityList, const std::shared_ptr<RenderingSystem>& render, PositionStorage& storage, AccelerationStorage& aStorage, VelocityStorage& vStorage)
{
    //Spawn the item
    if (dead || isMarkedForDeletion)
        return;
    Item& item = manager->CreateEntityDerivedFromClass<Item>();
    glm::vec3 position = GetComponent<PositionComponent>()->position;
    item.GetComponent<PositionComponent>()->position = position + glm::vec3(0, 10, 0);
    render->initalize(item);
    storage.AddPosition(item.GetComponent<PositionComponent>()->position, item.GetId());
    aStorage.AddAcceleration(item.GetComponent<AccelerationComponent>()->acceleration, item.GetId());
    vStorage.AddVelocity(item.GetComponent<VelocityComponent>()->velocity, item.GetId());
    entityList.push_back(&item);
    dead = true;
    //mark this entity for deletion
    //isMarkedForDeletion = true;
    


}
