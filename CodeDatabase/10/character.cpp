#include "character.h"
#include <iostream>

Character::Character(std::string name, Field& field, int startX, int startY, int initialLives)
    : name(name), lives(initialLives), damage(0), x(startX), y(startY), field(field) {}

void Character::takeDamage(int val) {
        damage += val;
        lives -= val;
        if (lives <= 0) lives = 0;
};
int Character::getLives() const { return lives; }
int Character::getDamage() const {return damage;}
bool Character::isAlive() const { return lives > 0; }
std::string Character::getname() const { return name; }
int Character::getX() const { return x; }
int Character::getY() const { return y; }
