#include "Beast.h"
#include <iostream>

Beast::Beast(int health, int damage)
    : hp(health), damage(damage), isActive(true) {}

int Beast::getHealth() const {
    return hp;
}

int Beast::getDamage() const {
    return damage;
}

bool Beast::getIsActive() const {
    return isActive;
}

void Beast::takeDamage(int damageAmount) {
    hp -= damageAmount;
    if (hp <= 0) {
        hp = 0;
        isActive = false;
        std::cout << "Beast defeated!\n";
    }
}

void Beast::setActive(bool active) {
    isActive = active;
}

bool Beast::isAlive() const {
    return hp > 0;
}