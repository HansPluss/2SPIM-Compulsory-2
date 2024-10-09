#pragma once
class BaseItem
{
public:
    virtual ~BaseItem() {}  // Virtual destructor for proper cleanup
    virtual void Use() = 0;
};

