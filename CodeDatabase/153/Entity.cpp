#include "Entity.h"

Entity::Entity(int startHealth, int startDamage)
    : health(startHealth), damage(startDamage) {
}

int Entity::getHealth() const {
    return health;
}

int Entity::getDamage() const {
    return damage;
}

void Entity::takeDamage(int amount) {
    health -= amount;
    if (health < 0) {
        health = 0;
    }
}

bool Entity::isAlive() const {
    return health > 0;
}
