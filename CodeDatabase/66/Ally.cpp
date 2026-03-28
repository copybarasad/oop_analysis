#include "../include/Ally.h"

Ally::Ally(int health, int damage) : health(health), damage(damage) {}

int Ally::getHealth() const {
    return health;
}

int Ally::getDamage() const {
    return damage;
}

void Ally::takeDamage(int damage) {
    health -= damage;
    if (health < 0) {
        health = 0;
    }
}

bool Ally::isAlive() const {
    return health > 0;
}
