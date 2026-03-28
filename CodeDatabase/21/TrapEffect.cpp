#include "TrapEffect.hpp"


TrapEffect::TrapEffect() : TrapEffect('o', 100, 1) {}


TrapEffect::TrapEffect(char symbol, int trapDamage, int duration) {
    this->stateSymbol = symbol;
    this->damage = trapDamage;
}


void TrapEffect::incDuration() {
    ++this->durationOfState;
}


void TrapEffect::decDuration() {
    --this->durationOfState;
}


int TrapEffect::getDamage() {
    return this->damage;
}


void TrapEffect::setDamage(int damage) {
    this->damage = damage;
}


void TrapEffect::setDuration(int countOfRounds) {
    this->durationOfState = countOfRounds;
}


int TrapEffect::getDuration() const {
    return this->durationOfState;
}


char TrapEffect::getSymbol() const {
    return this->stateSymbol;
}


StateSaveData TrapEffect::getStateSaveData() {
    StateSaveData data{};
    data.damage = this->damage;
    data.durationOfState = this->durationOfState;
    data.stateSymbol = this->stateSymbol;
    return data;
}