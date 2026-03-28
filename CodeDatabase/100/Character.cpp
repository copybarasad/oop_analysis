#include "Character.h"
#include "GameField.h"
#include <iostream>

Character::Character(int health, int damage, int startX, int startY, char symbol)
    : health(health), maxHealth(health), damage(damage),
    posX(startX), posY(startY), prevX(startX), prevY(startY), symbol(symbol) { }

// Геттеры
int Character::getHealth() const { return health; }
int Character::getMaxHealth() const { return maxHealth; }
int Character::getDamage() const { return damage; }
int Character::getX() const { return posX; }
int Character::getY() const { return posY; }
char Character::getSymbol() const { return symbol; }
int Character::getPrevX() const { return prevX; }
int Character::getPrevY() const { return prevY; }

// Сеттеры
void Character::setPosition(int x, int y) {
    posX = x;
    posY = y;
}

void Character::setHealth(int newHealth) {
    health = newHealth;
}

void Character::setMaxHealth(int newMaxHealth) {
    maxHealth = newMaxHealth;   
}

void Character::setDamage(int newDamage) {
    damage = newDamage;
}

void Character::takeDamage(int damage) {
    health -= damage;
    if (health < 0) health = 0;
}

void Character::heal(int amount) {
    health += amount;
    if (health > maxHealth) health = maxHealth;
}

// Проверки
bool Character::isAlive() const { return health > 0; }

void Character::move(int deltaX, int deltaY, const GameField& field) {
    prevX = posX;
    prevY = posY;
    
    int newX = posX + deltaX;
    int newY = posY + deltaY;

    if (field.isCellPassable(newX, newY)) {
        posX = newX;
        posY = newY;
    }
}

void Character::revertToPreviousPosition() {
    posX = prevX;
    posY = prevY;
}
