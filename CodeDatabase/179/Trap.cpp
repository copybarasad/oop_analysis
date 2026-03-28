#include "Trap.h"

Trap::Trap(int damage, const std::string& name)
    : GameObject(1, name), damage(damage), active(true) {}

int Trap::getDamage() const {
    return damage;
}

bool Trap::isActive() const {
    return active;
}

void Trap::trigger() {
    active = false;
    takeDamage(1); // Уничтожаем ловушку
}

std::string Trap::getType() const {
    return "Trap";
}
