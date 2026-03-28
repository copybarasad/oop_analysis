#include <Entity.hpp>

Entity::Entity(int x, int y, Type type) : x(x), y(y), type(type) {}

int Entity::getX() const {return x;}
int Entity::getY() const {return y;}

void Entity::setX(int x) {this->x = x;}
void Entity::setY(int y) {this->y = y;}

Entity::Type Entity::getType() const {return type;}

int Entity::getHealth() const {
    return -1;
}