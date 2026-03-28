#include "Unit.h"

Unit::Unit(int x, int y, int hp, int damage)
    : x_(x), y_(y), hp_(hp), damage_(damage) {
}

int Unit::getX() const { return x_; }
int Unit::getY() const { return y_; }

void Unit::setPosition(int x, int y) {
    x_ = x;
    y_ = y;
}

int Unit::getHP() const { return hp_; }
int Unit::getDamage() const { return damage_; }

void Unit::takeDamage(int d) {
    hp_ -= d;
    if (hp_ < 0) hp_ = 0;
}

bool Unit::isAlive() const { return hp_ > 0; }