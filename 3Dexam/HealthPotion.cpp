#include "HealthPotion.h"
#include <iostream>
#include "Player.h"
HealthPotion::HealthPotion(Player& player) : ref(player)
{

}
HealthPotion::~HealthPotion() 
{
}

void HealthPotion::Use()
{
	
	ref.GetComponent<HealthComponent>()->health += healthAmount;
	std::cout << "Health restored by " << healthAmount << " points." << std::endl;
	std::cout << "Current health: " << ref.GetComponent<HealthComponent>()->health << std::endl;
}
