#include "Entity.h"
#include <algorithm>

Entity::Entity(int health, int damage, int x, int y)
    : health(health), damage(damage), pos{ x, y } {
}

int Entity::getHealth() const {
    return health;
}

int Entity::getDamage() const {
    return damage;
}

Coords Entity::getCoords() const {
    return pos;
}

void Entity::move(int dx, int dy) {
    pos.x += dx;
    pos.y += dy;
}

void Entity::takeDamage(int amount) {
    health -= amount;
    if (health < 0) health = 0;
}

bool Entity::isAlive() const {
    return health > 0;
}
