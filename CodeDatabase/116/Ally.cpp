#include "Ally.h"

Ally::Ally(int x,int y,int Health, int Damage) 
    : Character(x,y,Health),
      damage(Damage) {}

int Ally::getDamage() const {
    return damage;
}