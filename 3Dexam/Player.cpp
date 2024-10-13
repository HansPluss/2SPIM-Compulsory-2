#include "Player.h"
#include "ItemData.h"
#include "BaseItem.h"
#include "HealthPotion.h"
#include "SpeedPotion.h"
Player::Player()
{
	HealthPotion healthPotion;
	SpeedPotion speedPotion;
	ItemData healthPotionData(&healthPotion, 2);
	ItemData SpeedPotionData(&speedPotion, 2);
	m_inventory.AddItem(healthPotionData);
	m_inventory.AddItem(SpeedPotionData);
}

Player::~Player()
{
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
		HealthPotion healthPotion;
		ItemData healthPotionData(&healthPotion, 1);
		m_inventory.AddItem(healthPotionData);
		break;
	}
	case 2:
	{
		SpeedPotion speedPotion;
		ItemData SpeedPotionData(&speedPotion, 1);
		m_inventory.AddItem(SpeedPotionData);
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
