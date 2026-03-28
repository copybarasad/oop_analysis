//
// Created by bob on 10/4/25.
//

#include "Character.h"

#include <stdexcept>

#define ERROR_DAMAGE "Damage must be greater than zero."
#define ERROR_HEALTH "Health must be greater than zero."


Character::Character(const int &damage, const Position &position, const int &health)
    :Entity(position) {
    this->setDamage(damage);
    this->setHealth(health);
}

void Character::setDamage(const int &damage) {
    this->damage = validateDamage(damage);
}

void Character::setHealth(const int &health) {
    this->health = validateHealth(health);
}

int Character::getDamage() const { return this->damage; }

int Character::getHealth() const { return this->health; }

bool Character::isAlive() const { return this->health > 0; }

void Character::applyDamage(const int & damage) {
    this->health -= validateDamage(damage);
}

int Character::validateDamage(const int &value) const {
    if (value >= 0) return value;
    else throw std::runtime_error(ERROR_DAMAGE);
}

int Character::validateHealth(const int &value) const {
    if (value > 0) return value;
    else throw std::runtime_error(ERROR_HEALTH);
}







