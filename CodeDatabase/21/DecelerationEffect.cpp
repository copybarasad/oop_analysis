#include "DecelerationEffect.hpp"


DecelerationEffect::DecelerationEffect() : DecelerationEffect(1, '=') {}


DecelerationEffect::DecelerationEffect(int duration, char symbol) {
    this->durationOfState = duration;
    this->stateSymbol = symbol;
}


void DecelerationEffect::incDuration() {
    ++this->durationOfState;
}


void DecelerationEffect::decDuration() {
    --this->durationOfState;
}


int DecelerationEffect::getDamage() {
    return 0;
}


void DecelerationEffect::setDamage(int damage) {
    this->damage = damage;
}


void DecelerationEffect::setDuration(int countOfRounds) {
    this->durationOfState = countOfRounds;
}


int DecelerationEffect::getDuration() const {
    return this->durationOfState;
}


char DecelerationEffect::getSymbol() const {
    return this->stateSymbol;
}


StateSaveData DecelerationEffect::getStateSaveData() {
    StateSaveData data{};
    data.damage = 0;
    data.durationOfState = this->durationOfState;
    data.stateSymbol = this->stateSymbol;
    return data;
}