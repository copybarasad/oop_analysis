#ifndef TRAP_H
#define TRAP_H

#include "Position.h"

class Trap
{
private:
    Position pos;
    int damage;

public:
    Trap(const Position &p = Position(), int dmg = 5);
    const Position &getPosition() const;
    int getDamage() const;
};

#endif