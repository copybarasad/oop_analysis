#include "Trap.h"

Trap::Trap(int x, int y, int damage)
	: positionX(x), positionY(y), damage(damage), triggered(false) {
}

int Trap::GetX() const {
	return positionX;
}

int Trap::GetY() const {
	return positionY;
}

int Trap::GetDamage() const {
	return damage;
}

bool Trap::IsTriggered() const {
	return triggered;
}

void Trap::Trigger() {
	triggered = true;
}
