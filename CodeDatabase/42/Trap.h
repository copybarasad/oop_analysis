#pragma once
#include "Position.h"

class Trap {
private:
    Position position;
    int damage;
    bool trapActive;
    
public:
    Trap(const Position& pos, int trapDamage);
    
    Position getPosition() const;
    int getDamage() const;
    bool isActive() const;
    void trigger();
};
