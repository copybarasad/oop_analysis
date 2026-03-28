#include "AttackEffect.hpp"

AttackEffect::AttackEffect() : AttackEffect('/', 0, 3) {}

AttackEffect::AttackEffect(char effectSympol, int effectDelay, int effectDuration) {
    this->stateSymbol = effectSympol;
    this->durationOfState = effectDuration + effectDelay;
    this->effectDelay = effectDelay;
}

void AttackEffect::incDuration() {
    ++this->durationOfState;
    ++this->effectDelay;
}

void AttackEffect::decDuration() {
    --this->durationOfState;
    --this->effectDelay;
}

int AttackEffect::getDamage() {
    return 0;
}

void AttackEffect::setDamage(int damage) {
    this->damage = damage;
}

void AttackEffect::setDuration(int countOfRounds) {
    this->durationOfState = countOfRounds;
}

int AttackEffect::getDuration() const {
    return this->durationOfState;
}

char AttackEffect::getSymbol() const {
    if (this->effectDelay <= 0) {
        return this->stateSymbol;
    }
    return '_';
}

StateSaveData AttackEffect::getStateSaveData() {
    StateSaveData data{};
    data.damage = 0;
    data.durationOfState = this->durationOfState;
    data.stateSymbol = this->stateSymbol;
    return data;
}