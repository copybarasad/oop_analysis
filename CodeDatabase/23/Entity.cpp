#include "Entity.h"

Entity::Entity(int x, int y, int hp) : x(x), y(y), hp(hp) {}

int Entity::getX() const {
    return x;
}

int Entity::getY() const {
    return y;
}

void Entity::setPosition(int x, int y) {
    this->x = x;
    this->y = y;
}
