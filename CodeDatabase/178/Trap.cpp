#include "Trap.h"

Trap::Trap(const Position &p, int dmg) : pos(p), damage(dmg) {}

const Position &Trap::getPosition() const { return pos; }
int Trap::getDamage() const { return damage; }