#pragma once
#include "InventoryComponent.h"
#include "BaseItem.h"
#include <iostream>

void InventoryComponent::AddItem(std::shared_ptr<BaseItem> item, int quantity)
{
    // Create an ItemData object with the shared pointer and quantity
    ItemData newItem(item, quantity);

    // Check for stackable items
    for (auto& i : items) {
        if (i.GetItemID() == newItem.GetItemID() &&
            (i.GetNumItems() + newItem.GetNumItems() <= i.GetStackSize())) {
            i.numItems += newItem.GetNumItems();
            return;
        }
    }

    // If not stackable, add as a new item
    items.push_back(newItem);
}

void InventoryComponent::RemoveItem(int itemID)
{
    for (auto it = items.begin(); it != items.end(); ++it) {
        if (it->GetItemID() == itemID) {
            items.erase(it);
            break;
        }
    }
}

void InventoryComponent::UseItem(int itemslot)
{
    // Check if itemslot is valid
    if (itemslot >= 0 && itemslot < items.size()) {
        auto& item = items[itemslot]; // Use a reference to avoid copying

        if (item.GetIsStackable() && item.GetNumItems() > 0 && item.GetItemReference() != nullptr) {
            item.GetItemReference()->Use(); // Use the item
            item.numItems--; // Decrease the count

            // If no items left, remove it
            if (item.GetNumItems() == 0) {
                RemoveItem(item.GetItemID());
            }
        }
        else if (!item.GetIsStackable() && item.GetItemReference() != nullptr) {
            item.GetItemReference()->Use(); // Use the item
            RemoveItem(item.GetItemID()); // Remove it since it's not stackable
        }
    }
}

void InventoryComponent::listItems()
{
    for (auto& item : items) {
        std::cout << "Item Name: " << item.GetItemName() << " Num Items: " << item.GetNumItems() << std::endl;
    }
}
