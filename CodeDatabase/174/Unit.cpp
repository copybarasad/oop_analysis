#include "Unit.h"

Unit::Unit() : maxHealthPoints(0), healthPoints(0), damage(0) {}

Unit::Unit(int health, int damageValue, int x, int y) : 
    maxHealthPoints(health), healthPoints(health), damage(damageValue), position(x, y) {}

int Unit::getHealthPoints() const { return healthPoints; }
int Unit::getMaxHealthPoints() const { return maxHealthPoints; }
int Unit::getDamage() const { return damage; }
Position Unit::getPosition() const { return position; }

void Unit::setPosition(int x, int y) { 
    position.setX(x); 
    position.setY(y); 
}

void Unit::setHealth(int health) { 
    healthPoints = health; 
    if (healthPoints > maxHealthPoints) {
        healthPoints = maxHealthPoints;
    }
}

void Unit::setMaxHealth(int maxHealth) { 
    maxHealthPoints = maxHealth;
    if (healthPoints > maxHealthPoints) {
        healthPoints = maxHealthPoints;
    }
}

void Unit::setDamage(int damageValue) { 
    damage = damageValue; 
}

void Unit::takeDamage(int damageAmount) {
    healthPoints -= damageAmount;
    if (healthPoints < 0) {
        healthPoints = 0;
    }
}

bool Unit::isAlive() const { 
    return healthPoints > 0; 
}