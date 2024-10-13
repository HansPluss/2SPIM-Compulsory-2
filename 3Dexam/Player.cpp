#include "Player.h"
#include "BaseItem.h"
#include <memory>
Player::Player()
{
	AddItemsToInventory();
}
void  Player::AddItemsToInventory() {

	m_inventory.AddItem(speedPotion, 2); // Add one SpeedPotion to the inventory
	m_inventory.AddItem(healthPotion, 2); // Add one HealthPotion to the inventory
}

Player::~Player()
{
	// Destructor
}

void Player::GetInventory()
{
	m_inventory.listItems();
}

void Player::AddInventoryItem(int ID)
{
	switch (ID)
	{
	case 1:
	{
		m_inventory.AddItem(healthPotion, 1);
		break;
	}
	case 2:
	{
		m_inventory.AddItem(speedPotion, 1);
		break;
	}
	default:
		break;
	}
}

void Player::UseInventoryItem(int ID)
{
	m_inventory.UseItem(ID);
}
