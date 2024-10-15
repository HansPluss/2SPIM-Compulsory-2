#pragma once
#include "Component.h"
#include <vector>
#include <memory>
#include "ItemData.h"

class BaseItem;

class InventoryComponent : public Component {
public:
    // Adds an item to the inventory
    void AddItem(std::shared_ptr<BaseItem> item, int quantity);
    void RemoveItem(int itemID);
    void UseItem(int itemslot);
    void listItems();

private:
    std::vector<ItemData> items; // Store ItemData objects
};

