#pragma once
#include "IState.hpp"

class DecelerationEffect : public IState{
public:
    DecelerationEffect();
    DecelerationEffect(int duration, char symbol);
    void incDuration() override;
    void decDuration() override;

    int getDamage() override;
    void setDamage(int damage) override;

    void setDuration(int countOfRounds) override;
    int getDuration() const override;
    char getSymbol() const override;

    StateSaveData getStateSaveData() override;
};
