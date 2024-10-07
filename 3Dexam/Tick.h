#pragma once

class Tick {
public:
    // Mark the destructor virtual if this class is meant to be a base class
    virtual ~Tick() {}

    // Pure virtual function - this forces derived classes to implement UpdateTick
    virtual void UpdateTick(float deltaTime) = 0;
};
