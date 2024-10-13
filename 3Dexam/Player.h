#pragma once
#include "Entity.h"
#include "InventoryComponent.h"
#include "SpeedPotion.h"
#include "HealthPotion.h"

class InventoryComponent;
class Player : public Entity
{
public: 
	Player(); 
	~Player();
	void GetInventory();
	void AddInventoryItem(int ID);
	void UseInventoryItem(int ID);
private:
	void AddItemsToInventory();
	InventoryComponent m_inventory;
	std::shared_ptr<BaseItem> healthPotion = std::make_shared<HealthPotion>();
	std::shared_ptr<BaseItem> speedPotion = std::make_shared<SpeedPotion>();
};

