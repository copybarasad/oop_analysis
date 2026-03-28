#include "point.h"

Point::Point(bool isPassable) : isPassable_(isPassable) {}

bool Point::isPassable() const {
    return isPassable_;
}

void Point::setPassable(bool passable) {
    isPassable_ = passable;
}