#include "CoupreAnimate.hpp"

CoupreAnimate::CoupreAnimate() : CoupreAnimate('x', 0, 3) {}

CoupreAnimate::CoupreAnimate(char effectSympol, int effectDelay, int effectDuration) {
    this->stateSymbol = effectSympol;
    this->durationOfState = effectDuration + effectDelay;
    this->effectDelay = effectDelay;
}

void CoupreAnimate::incDuration() {
    ++this->durationOfState;
    ++this->effectDelay;
}

void CoupreAnimate::decDuration() {
    --this->durationOfState;
    --this->effectDelay;
}

int CoupreAnimate::getDamage() {
    return 0;
}

void CoupreAnimate::setDamage(int damage) {
    this->damage = damage;
}

void CoupreAnimate::setDuration(int countOfRounds) {
    this->durationOfState = countOfRounds;
}

int CoupreAnimate::getDuration() const {
    return this->durationOfState;
}

char CoupreAnimate::getSymbol() const {
    return this->stateSymbol;
}


StateSaveData CoupreAnimate::getStateSaveData() {
    StateSaveData data{};
    data.damage = 0;
    data.durationOfState = this->durationOfState;
    data.stateSymbol = this->stateSymbol;
    return data;
}