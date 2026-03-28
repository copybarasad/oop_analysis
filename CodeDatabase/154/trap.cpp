#include "trap.h"

Trap::Trap(int id, int damage): Entity(id, 1), damage_(damage) {}

EntityType Trap::getType() const { return EntityType::Trap; }

int Trap::getDamage() const { return damage_; }

Trap* Trap::asTrap() { return this; }
