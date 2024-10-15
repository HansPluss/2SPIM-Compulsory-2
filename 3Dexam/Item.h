#pragma once
#include "Entity.h"
#include "Component.h"
class Player; 
class Item : public Entity {
public:
	Item();
	~Item();
	void checkCollision(Player player);

private:
	void Pickup(Player player);
	int ItemID = rand() % 3; // 0, 1, 2

};