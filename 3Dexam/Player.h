#pragma once
#include "Entity.h"
#include "InventoryComponent.h"
#include "SpeedPotion.h"
#include "HealthPotion.h"
#include "BaseItem.h"

class InventoryComponent;
class Player : public Entity
{
public: 
	Player();
	~Player();
	std::vector<ItemData> GetInventory();
	void AddInventoryItem(int ID);
	void UseInventoryItem(int ID);
	int GetMaxSpeed() { return maxSpeed; }
	int GetSpeed() { return speed; }
private:
	void AddItemsToInventory();
	InventoryComponent m_inventory;
	std::shared_ptr<BaseItem> healthPotion = std::make_shared<HealthPotion>();
	std::shared_ptr<BaseItem> speedPotion = std::make_shared<SpeedPotion>();

	int maxSpeed = 20;
	int speed = 10; 
};

