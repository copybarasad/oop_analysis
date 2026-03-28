#include "Building.hpp"

Building::Building(int x, int y, int health)
    : positionX_(x), positionY_(y), health_(health) {
        validateParameters();
}

void Building::validateParameters() const {
    if (health_ < 0) {
        throw std::invalid_argument("Health cannot be negative");
    }
}

int Building::getPositionX() const { 
    return positionX_; 
}

int Building::getPositionY() const { 
    return positionY_; 
}

int Building::getHealth() const { 
    return health_; 
}

bool Building::isDestroyed() const {
    return health_ <= 0;
}

void Building::takeDamage(int damage) {
    if (damage < 0) {
        throw std::invalid_argument("Damage cannot be negative");
    }
    health_ -= damage;
    if (health_ < 0) health_ = 0;
}

