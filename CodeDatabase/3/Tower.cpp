#include "GameObjects/Entities/Tower.hpp"

bool Tower::onCoolDown() {
    if (steps >= coolDown) {
        steps = 0;
        return false;
    }
    return true;
}

void Tower::addStep() {
    steps++;
}

int Tower::getSteps() {
    return steps;
}

void Tower::setSteps(int s) {
    this->steps = s;
}


void Tower::setCoolDown(int cd) {
    coolDown = cd;
}

int Tower::getCoolDown() {
    return coolDown;
}
