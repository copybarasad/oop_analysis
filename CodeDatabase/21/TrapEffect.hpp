#pragma once
#include "IState.hpp"

class TrapEffect : public IState{
private:
    int effectDelay;
public:
    TrapEffect();
    TrapEffect(char effectSympol, int trapDamage, int duration);
    void incDuration() override;
    void decDuration() override;

    int getDamage() override;
    void setDamage(int damage) override;

    void setDuration(int countOfRounds) override;
    int getDuration() const override;
    char getSymbol() const override;

    StateSaveData getStateSaveData() override;
};