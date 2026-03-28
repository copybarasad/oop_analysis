#include "Trap.hpp"

Trap::Trap(const Coords& pos, int damage)
	: position(pos), damage(damage), active(true) {}

Trap::Trap(const Trap& other)
	: position(other.position), damage(other.damage), active(other.active) {}

Trap::Trap(Trap&& other) noexcept
	: position(other.position), damage(other.damage), active(other.active) {
	other.damage = 0;
	other.active = false;
}

Trap& Trap::operator=(const Trap& other) {
	if (this != &other) {
		position = other.position;
		damage = other.damage;
		active = other.active;
	}
	return *this;
}

Trap& Trap::operator=(Trap&& other) noexcept {
	if (this != &other) {
		position = other.position;
		damage = other.damage;
		active = other.active;
		other.damage = 0;
		other.active = false;
	}
	return *this;
}

const Coords& Trap::getPosition() const noexcept {
	return position;
}

int Trap::getDamage() const noexcept {
	return damage;
}

bool Trap::isActive() const noexcept {
	return active;
}

int Trap::trigger() {
	if (!active) return 0;
	active = false;
	return damage;
}


