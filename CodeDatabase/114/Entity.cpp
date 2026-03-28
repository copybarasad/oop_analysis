#include "Entity.h"

void Entity::setPosition(int newX, int newY) {
    this->x = newX;
    this->y = newY;
}

void Entity::takeDamage(int damage) {
    this->health -= damage;
    if (this->health < 0) {
        this->health = 0;
    }
}

bool Entity::isAlive() const {
    return health > 0;
}
