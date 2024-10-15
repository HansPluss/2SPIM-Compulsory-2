#include "Item.h"
#include "Player.h"
#include "CollisionSystem.h"


Item::Item()
{
		AddComponent<PositionComponent>();
		AddComponent<RenderComponent>(glm::vec3(0.0f), glm::vec3(1.0f), "cube");
		AddComponent<CollisionComponent>();
}

Item::~Item()
{
}


void Item::Pickup(Player player)
{
	player.AddInventoryItem(ItemID);
}

void Item::checkCollision(Player player)
{
	CollisionSystem collisionSystem;
	if (collisionSystem.SphereCollision(player, *this, 0.0f))
	{
		Pickup(player);
	}
}
