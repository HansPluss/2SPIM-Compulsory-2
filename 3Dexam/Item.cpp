#include "Item.h"
#include "Player.h"
#include "Component.h"
#include <vector>

Item::Item() : ItemID(0)
{
    AddComponent<PositionComponent>();
    AddComponent<RenderComponent>(glm::vec3(0.0f), glm::vec3(1.0f), "cube");
    // AddComponent<CollisionComponent>();
}

Item::~Item()
{
}

void Item::Pickup(Player& player)
{
	// Adding a random item to the player's inventory
    player.AddInventoryItem(ItemID);
}

void Item::checkCollision(Player& player) 
{
    glm::vec3 futurePosA = player.GetComponent<PositionComponent>()->GetPosition()
        + player.GetComponent<VelocityComponent>()->GetVelocity();

    // Calculating the distance between the centers of both objects
    float distanceCenters = glm::length(futurePosA - this->GetComponent<PositionComponent>()->GetPosition());

    // Checking if the distance is less than the sum of the radii
    if (distanceCenters <= (player.GetComponent<RenderComponent>()->size.x + this->GetComponent<RenderComponent>()->size.x))
    {
        Pickup(player); 
        isMarkedForDeletion = true;
    }
}
