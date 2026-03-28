#include "Trap.h"

Trap::Trap(int startX, int startY, int dmg) : x(startX), y(startY), damage(dmg) {}
int Trap::getX() const { return x; }
int Trap::getY() const { return y; }
int Trap::getDamage() const { return damage; }