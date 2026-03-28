#include "GameObjects/Entities/PersonBaseClass.hpp"

Coordinates PersonBaseClass::getPosition() {
    return position;
}

void PersonBaseClass::setPosition(Coordinates pos) {
    position = pos;
}

int PersonBaseClass::getHealthPoints() const {
    return healthPoints;
}

void PersonBaseClass::minusHealthPoints(int hp) {
    healthPoints -= hp;
    if (healthPoints < 0) {
        healthPoints = 0;
    }
}

void PersonBaseClass::setHealthPoints(int hp) {
    healthPoints = hp;
}

int PersonBaseClass::getDamagePoints() const {
    return damagePoints;
}

void PersonBaseClass::setDamagePoints(int dmg) {
    damagePoints = dmg;
}

bool PersonBaseClass::isDisabled() const {
    return disabled;
}

void PersonBaseClass::changeDisabled() {
    disabled = !disabled;
}

void PersonBaseClass::setDisabled(bool d) {
    disabled = d;
}

int PersonBaseClass::getMaxHP() const {
    return maxHP;
}

void PersonBaseClass::setMaxHP(int hp) {
    maxHP = hp;
}
