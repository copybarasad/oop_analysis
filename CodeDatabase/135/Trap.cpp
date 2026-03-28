#include "Trap.h"

Trap::Trap(int trap_damage) : damage(trap_damage), active(true) {}

int Trap::getDamage() const {
	return damage;
}

bool Trap::isActive() const {
	return active;
}

void Trap::trigger() {
	active = false;
}
