#ifndef TRAP_H
#define TRAP_H

#include "Position.h"

class Trap {
private:
    Pos position_;
    int damage_;

public:
    Trap(Pos pos, int damage) : position_(pos), damage_(damage) {}
    
    Pos getPosition() const { return position_; }
    int getDamage() const { return damage_; }
};

#endif

