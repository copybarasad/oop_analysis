#include "GameObjects/Entities/AttackTower.hpp"


AttackTower::AttackTower() {}

Coordinates AttackTower::getPosition() {
    return position;
};

void AttackTower::setPosition(Coordinates pos) {
    position = pos;
}

int AttackTower::getHealthPoints() {
    return healthPoints;
}

void AttackTower::setHealthPoints(int hp) {
    this->healthPoints = hp;
}

int AttackTower::getMaxHP() {
    return maxHP;
}

void AttackTower::setMaxHP(int hp) {
    this->maxHP = hp;
}

void AttackTower::minusHealthPoints(int hp) {
    healthPoints -= hp;
}
