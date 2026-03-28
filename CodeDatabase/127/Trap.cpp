// Trap.cpp
#include "Trap.h"

Trap::Trap(int x, int y, int damage) : x_(x), y_(y), damage_(damage) {}
int Trap::getX() const { return x_; }
int Trap::getY() const { return y_; }
int Trap::getDamage() const { return damage_; }