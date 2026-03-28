
#include "Creature.h"



void Creature::setYCoordinate(int y) noexcept{
    this->yCoordinate = y;
}

void Creature::setXCoordinate(int x) noexcept{
    this->xCoordinate = x;
}

bool Creature::isDisabled() noexcept {
    return isDisabledFlag;
}

int Creature::getStepRange() const noexcept {
    return this->stepRange;
}
void Creature::setDisabledFlag(bool flag) noexcept {
    this->isDisabledFlag = flag;
}

void Creature::setStepRange(int stepRange) {
    Creature::stepRange = stepRange;
}




