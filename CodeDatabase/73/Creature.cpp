#include "Creature.h"

Creature::Creature(int health, int damage, int x, int y)
        : Entity(x, y), health_(health), damage_(damage) {}

bool Creature::isAlive() const {
    return health_ > 0;
}

void Creature::takeDamage(int damage) {
    health_ -= damage;
    if (health_ < 0) {
        health_ = 0;
    }
}

int Creature::getHealth() const {
    return health_;
}

int Creature::getDamage() const {
    return damage_;
}