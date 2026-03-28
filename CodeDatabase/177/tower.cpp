#include "tower.h"
#include <cmath>
#include <stdexcept>

Tower::Tower(int range, int cooldown) 
    : position(0, 0),
      attackRange(range), 
      attackCooldown(cooldown), 
      currentCooldown(0),
      spell(DEFAULT_DAMAGE, range) {  
    
    if (range <= 0 || cooldown <= 0) {
        throw std::invalid_argument("Range and cooldown must be positive");
    }
}

Position Tower::getPosition() const {
    return position;
}

void Tower::setPosition(const Position& newPosition) {
    position = newPosition;
}

bool Tower::canAttack() const {
    return currentCooldown == 0;
}

bool Tower::isInRange(const Position& targetPosition) const {
    int distanceX = std::abs(targetPosition.getX() - position.getX());
    int distanceY = std::abs(targetPosition.getY() - position.getY());
    int distance = distanceX + distanceY;
    
    return distance <= attackRange; 
}

void Tower::attack() {
    if (!canAttack()) {
        throw std::runtime_error("Tower cannot attack - cooldown active");
    }
    currentCooldown = attackCooldown; 
}

void Tower::update() {
    if (currentCooldown > 0) {
        --currentCooldown; 
    }
}

int Tower::getDamage() const {
    return spell.getDamage(); 
}

int Tower::getRange() const {
    return attackRange;
}

int Tower::getCooldown() const {
    return attackCooldown;
}

int Tower::getCurrentCooldown() const {
    return currentCooldown;
}