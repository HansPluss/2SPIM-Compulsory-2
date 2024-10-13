#pragma once
#include "Component.h"
#include <vector>
#include "ItemData.h"
#include <iostream>
#include "BaseItem.h"
#include <memory>

class InventoryComponent : public Component
{
public:
    // Adds an item to the inventory
    void AddItem(const ItemData& item)
    {
		for (auto& i : items)
		{
			if (i.GetItemID() == item.GetItemID() && i.GetNumItems() + item.GetNumItems() < i.GetStackSize() + 1)
			{
				i.numItems += item.numItems;
				return;
			}
		}
        items.push_back(item);
    }

    void RemoveItem(int itemID)
    {
        // Find item by ID and remove it
        for (auto it = items.begin(); it != items.end(); ++it)
        {
            if (it->GetItemID() == itemID)
            {
                items.erase(it);
                break;
            }
        }
    }

    void UseItem(int itemID)
    {
        for (auto& item : items)
        {
            if (item.GetItemID() == itemID)
            {
                if (item.GetIsStackable() && item.GetNumItems() > 0)
                {
                    // Use the item (e.g., consume a potion, equip a weapon)
                    item.GetItemReference()->Use();  // Notice the '->' for pointer dereference
                    item.numItems--;
                    if (item.GetNumItems() == 0)
                    {
                        // Remove the item from the inventory
                        RemoveItem(itemID);
                    }
                }
                else
                {
                    // Use the item (e.g., consume a potion, equip a weapon)
                    item.GetItemReference()->Use(); 
                    // Remove the item from the inventory
                    RemoveItem(itemID);
                }
            }
        }
    }
	void listItems()
	{
		for (auto& item : items)
		{
			std::cout << "Item Name: " << item.GetItemName() << " Num Items: " << item.GetNumItems() << std::endl;
		}
	}
private:
    std::vector<ItemData> items;
};

