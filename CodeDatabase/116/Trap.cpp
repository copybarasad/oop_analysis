#include "Trap.h"

Trap::Trap(int x,int y,int Health, int Damage) 
    : Character(x,y,Health),
      damage(Damage) {}

int Trap::getDamage() const {
    return damage;
}