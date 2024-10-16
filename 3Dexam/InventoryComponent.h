#pragma once
#include "Component.h"
#include <vector>
#include <memory>
#include "ItemData.h"

class BaseItem;

class InventoryComponent : public Component {
public:
    void AddItem(std::shared_ptr<BaseItem> item, int quantity); // Adding an item to the inventory
    void RemoveItem(int itemID);                                // Removing an item to the inventory
    void UseItem(int itemslot);                                 // Using an item to the inventory
    void listItems();                                           // Listing inventory contents

private:
    std::vector<ItemData> items;                                // Stores ItemData objects
};

