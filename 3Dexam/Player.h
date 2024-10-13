#pragma once
#include "Entity.h"
#include "InventoryComponent.h"

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
	InventoryComponent m_inventory;
};

