#ifndef TRAP_H
#define TRAP_H

#include "Position.h"

class Trap {
private:
    Position position;
    bool active;

public:
    Trap() : position(0, 0), active(false) {} // Default constructor
    Trap(const Position& pos);
    
    const Position& getPosition() const { return position; }
    bool isActive() const { return active; }
    void trigger() { active = false; }
    int getDamage() const { return 30; } // One-shot damage
};

#endif