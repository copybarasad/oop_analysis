#include "entity.h"
#include <algorithm>

Entity::Entity(int health, int x, int y) : health(health), x(x), y(y) {}

int Entity::getHealth() const {
    return health;
}

int Entity::getX() const {
    return x;
}

int Entity::getY() const {
    return y;
}

void Entity::setPosition(int newX, int newY) {
    x = newX;
    y = newY;
}

void Entity::takeDamage(int damage) {
    health = std::max(0, health - damage);
}

bool Entity::isAlive() const {
    return health > 0;
}