#pragma once
#include "BaseItem.h"
#include <string>
class ItemData
{
public:
    // Constructor with default values
    ItemData(BaseItem* itemReference, int numItems)
        : itemReference(itemReference), numItems(numItems) 
	{
		itemID = itemReference->GetItemID();
		isStackable = itemReference->GetIsStackable();
		stackSize = itemReference->GetStackSize();
		ItemName = itemReference->GetItemName();
    }

    // Getters to retrieve data from ItemObjects
    BaseItem* GetItemReference()  { return itemReference; }
    int GetItemID() const { return itemID; }
    bool GetIsStackable() const { return isStackable; }
    int GetStackSize() const { return stackSize; }
    int GetNumItems() const { return numItems; }
	std::string GetItemName() { return ItemName; }
    int numItems;     // amount of current  item

private:
    BaseItem* itemReference;  // Reference to the class of the item, used to call use to use the item
    int itemID;       // Unique ID of the item to more easily identify it and add/remove it from stacks in inventory
    bool isStackable; // Whether the item is stackable
    int stackSize;    // max number of items in a stack
	std::string ItemName;
};




