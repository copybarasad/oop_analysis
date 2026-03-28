#include "character.h"
#include "field.h"  
#include <stdexcept>

Character::Character(const std::string& name, int health, const Position& position)
    : name_(name), health_(health), position_(position) {
    if (health < 0) {
        throw std::invalid_argument("Health cannot be negative");
    }
}

std::string Character::getName() const { 
    return name_; 
}

int Character::getHealth() const { 
    return health_; 
}

Position Character::getPosition() const { 
    return position_; 
}

void Character::setHealth(int health) {
    if (health < 0) {
        throw std::invalid_argument("Health cannot be negative");
    }
    health_ = health;
}

bool Character::isAlive() const {
    return health_ > 0;
}
bool Character::baseMoveWithField(int dx, int dy, Field& field) {
    Position new_position = position_.translated(dx, dy);
    
    if (field.isPassable(new_position)) {
        position_ = new_position;
        return true;
    }
    
    return false;
}
bool Character::moveWithField(Direction direction, Field& field) {
    int dx = directionToDx(direction);
    int dy = directionToDy(direction);
    return moveWithField(dx, dy, field);
}

bool Character::moveWithField(int dx, int dy, Field& field) {
    Position newPos = position_.translated(dx, dy);
    
    if (baseMoveWithField(dx, dy, field)) {
        field.applyCellEffect(*this);  
        return true;
    }
    return false;
}