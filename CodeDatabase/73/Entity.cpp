#include "Entity.h"

Entity::Entity(int x, int y) : x_(x), y_(y) {}

int Entity::getX() const { return x_; }
int Entity::getY() const { return y_; }

void Entity::setPosition(int x, int y) {
    x_ = x;
    y_ = y;
}
