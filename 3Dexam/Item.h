#pragma once
#include "Entity.h"
#include "Player.h"
#include "Tick.h"
class Item : public Entity, public Tick {
public:
	Item(Player& player);
	~Item();
	void checkCollision(Player& player);

private:
	void Pickup(Player& player);
	int ItemID = rand() % 3; // 0, 1, 2
	Player* playerref;

    void UpdateTick(float deltaTime) override;
};