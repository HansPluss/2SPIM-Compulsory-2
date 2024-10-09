#pragma once
#include "BaseItem.h"
class ItemData
{
public:
    // Constructor with default values
    ItemData(BaseItem* itemReference, int itemID, bool isStackable, int stackSize = 1, int numItems = 1)
        : itemReference(itemReference), itemID(itemID), isStackable(isStackable),
        stackSize(stackSize), numItems(numItems) {}

    // Getters to retrieve data from ItemObjects
    BaseItem* GetItemReference()  { return itemReference; }
    int GetItemID() const { return itemID; }
    bool GetIsStackable() const { return isStackable; }
    int GetStackSize() const { return stackSize; }
    int GetNumItems() const { return numItems; }
    int numItems;     // amount of current  items

private:
    BaseItem* itemReference;  // Reference to the class of the item, used to call use to use the item
    int itemID;       // Unique ID of the item to more easily identify it and add/remove it from stacks in inventory
    bool isStackable; // Whether the item is stackable
    int stackSize;    // max number of items in a stack
};




