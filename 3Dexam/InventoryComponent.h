#pragma once
#include "Component.h"
#include <vector>
#include "ItemData.h"
#include <iostream>
#include "BaseItem.h"
#include <memory>

class InventoryComponent : public Component {
public:
    // Adds an item to the inventory
    void AddItem(std::shared_ptr<BaseItem> item, int quantity) {
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

    void RemoveItem(int itemID) {
        for (auto it = items.begin(); it != items.end(); ++it) {
            if (it->GetItemID() == itemID) {
                items.erase(it);
                break;
            }
        }
    }

    void UseItem(int itemID) {
        for (auto& item : items) {
            if (item.GetItemID() == itemID) {
                if (item.GetIsStackable() && item.GetNumItems() > 0 && item.GetItemReference() != nullptr) {
                    item.GetItemReference()->Use();
                    item.numItems--;
                    if (item.GetNumItems() == 0) {
                        RemoveItem(itemID);
                    }
                } else if (!item.GetIsStackable() && item.GetItemReference() != nullptr) {
                    item.GetItemReference()->Use();
                    RemoveItem(itemID);
                }
            }
        }
    }

    void listItems() {
        for (auto& item : items) {
            std::cout << "Item Name: " << item.GetItemName() << " Num Items: " << item.GetNumItems() << std::endl;
        }
    }

private:
    std::vector<ItemData> items; // Store ItemData objects
};

