#pragma once
#include "BaseItem.h"
class HealthPotion : public BaseItem
{
public:
	HealthPotion();
	~HealthPotion();
	void Use();
	int GetItemID() { return ItemID; }
private:
	int healthAmount = 20; // Amount of health restored by the potion
	int stackSize = 5;     // Maximum number of potions in a stack
	int ItemID = 0;
};

