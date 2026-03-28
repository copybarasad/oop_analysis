#include "Ally.h"

Ally::Ally(int health, int damage)
    : health_(health), maxHealth_(health), damage_(damage), 
      position_({0, 0}), skipNextTurn_(false) {}

void Ally::takeDamage(int amount) {
    if (amount > 0) {
        health_ -= amount;
        if (health_ < 0) health_ = 0;
    }
}

