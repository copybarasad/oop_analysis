#pragma once
#include "../SaveData.hpp"

class IState {
protected:
    int durationOfState;
    int damage;
    char stateSymbol;
public:
    virtual void incDuration() = 0;
    virtual void decDuration() = 0;

    virtual int getDamage() = 0;
    virtual void setDamage(int damage) = 0;

    virtual void setDuration(int countOfRounds) = 0;
    virtual int getDuration() const = 0;
    virtual char getSymbol() const = 0;

    virtual StateSaveData getStateSaveData() = 0;
};