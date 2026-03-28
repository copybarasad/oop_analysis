#include "Character.h"
#include "Field.h"
#include <iostream>

// добавили maxLives
Character::Character(std::string name, Field* field, int startX, int startY, int initialLives)
    : name(name), field(field), x(startX), y(startY), lives(initialLives), maxLives(initialLives), damage(0) {}

void Character::takeDamage(int amount) {
    lives -= amount;
    if (lives < 0) {
        lives = 0;
    }
    std::cout << name << " получает " << amount << " урона. Осталось " << lives << " HP." << std::endl;

    if (!isAlive()) {
        std::cout << name << " повержен!" << std::endl;
        if (field) {
           field->getNode(x, y).setType(Type::EMPTY); // теперь в момент смерти враг исчезает
        }
    }
}

bool Character::isAlive() const {
    return lives > 0;
}

int Character::getX() const {
    return x;
}

int Character::getY() const {
    return y;
}

int Character::getLives() const {
    return lives;
}

int Character::getMaxLives() const {
    return maxLives;
}

std::string Character::getName() const {
    return name;
}

void Character::restoreHealth() { // восстановление здоровья
    lives = maxLives;
}

void Character::setField(Field* newField) {
    field = newField;
}

void Character::setPosition(int newX, int newY) {
    x = newX;
    y = newY;
}