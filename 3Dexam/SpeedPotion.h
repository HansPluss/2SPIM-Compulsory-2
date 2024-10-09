#pragma once
#include "BaseItem.h"
class SpeedPotion : public BaseItem
{
public:
	SpeedPotion();
	~SpeedPotion();
	void Use();
	int GetItemID() { return ItemID; }

private:
	int speedAmount = 10; // Amount of speed boost provided by the potion
	int stackSize = 5;    // Maximum number of potions in a stack
	int ItemID = 1;
};

