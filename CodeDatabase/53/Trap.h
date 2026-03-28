#ifndef TRAP_H
#define TRAP_H

#include "Cell.h"
#include "Character.h"
#include <iostream>

class Trap {
private:
    int x, y;
    int damage;
    bool active;

public:
    Trap(int x, int y, int damage);
    
    void activate(Character& character);
    bool isActive() const { return active; }
    int getX() const { return x; }
    int getY() const { return y; }
    int getDamage() const { return damage; }
    void setDamage(int newDamage) { damage = newDamage; }
};

#endif