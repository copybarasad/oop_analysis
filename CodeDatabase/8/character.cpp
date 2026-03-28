#include "character.h"
#include <iostream>

Character::Character(std::string name, Field& field, int startX, int startY)
    : name(name), field(field), x(startX), y(startY), lives(10), damage(0) {}

void Character::setPosition(int newX, int newY) {
    if (field.validpos(x, y)) {
        field.getNode(x, y).removeEntity();
    }
    x = newX;
    y = newY;
}
void Character::takeDamage(int val) {
        damage += val;
        lives -= val;
        if (lives <= 0) lives = 0;
};
void Character::addLives(int val) { lives += val; }
void Character::removeLives(int val) { lives -= val; }
int Character::getLives() const { return lives; }
int Character::getDamage() const {return damage;}
bool Character::isAlive() const { return lives > 0; }
std::string Character::getname() const { return name; }
int Character::getX() const { return x; }
int Character::getY() const { return y; }
