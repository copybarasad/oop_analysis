#include "tower.h"
#include <cmath>

Tower::Tower(int health, int radius, std::pair<int, int> position, int damage) 
    : health(health), radius(radius), position(position), damage(damage), 
      cooldown(0), attackCooldown(2) {
}

std::pair<int, int> Tower::getPosition() const { 
    return position; 
}

bool Tower::isAlive() const { 
    return health > 0; 
}

void Tower::takeDamage(int dmg) {
    health -= dmg;
    if (health < 0) health = 0;
}

bool Tower::isInRange(std::pair<int, int> target) const {
    int dx = std::abs(position.first - target.first);
    int dy = std::abs(position.second - target.second);
    return dx <= radius && dy <= radius;
}

bool Tower::canAttack() const {
    return cooldown <= 0;
}

void Tower::setCooldown() {
    cooldown = attackCooldown;
}

void Tower::updateCooldown() {
    if (cooldown > 0) cooldown--;
}

int Tower::getHealth() const {
    return health;
}
int Tower::getDamage() const {
    return damage;
}

int Tower::getRadius() const{
    return radius;
}