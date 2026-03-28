#include "Character.h"

Character::Character(int initialHealth, int characterDamage) 
    : health(initialHealth), maxHealth(initialHealth), damage(characterDamage) {
}

int Character::getHealth() const {
    return health;
}

int Character::getMaxHealth() const {
    return maxHealth;
}

int Character::getDamage() const {
    return damage;
}

void Character::setHealth(int newHealth) {
    if (newHealth < 0) {
        newHealth = 0;
    }
    if (newHealth > 1000) {
        newHealth = 100;
    }
    
    health = newHealth;
    if (health > maxHealth) {
        health = maxHealth;
    }
}

void Character::setMaxHealth(int newMaxHealth) {
    if (newMaxHealth < 0) {
        newMaxHealth = 0;
    }
    if (newMaxHealth > 1000) {
        newMaxHealth = 100;
    }
    
    maxHealth = newMaxHealth;
    if (health > maxHealth) {
        health = maxHealth;
    }
}

void Character::takeDamage(int damageAmount) {
    if (damageAmount < 0) {
        damageAmount = 0;
    }
    
    int newHealth = health - damageAmount;
    setHealth(newHealth);
}

void Character::heal(int healAmount) {
    if (healAmount < 0) {
        healAmount = 0;
    }
    
    int newHealth = health + healAmount;
    setHealth(newHealth);
}

bool Character::isAlive() const {
    return health > 0;
}