#include "Trap.h"

Trap::Trap(Position pos, int dmg) : Entity(pos, 1), damage(dmg) {}

int Trap::GetDamage() const { return damage; }