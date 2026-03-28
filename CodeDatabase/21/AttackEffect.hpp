#pragma once
#include "IState.hpp"

class AttackEffect : public IState{
private:
    int effectDelay;
public:
    AttackEffect();
    AttackEffect(char effectSympol, int effectDelay, int effectDuration);
    void incDuration() override;
    void decDuration() override;

    int getDamage() override;
    void setDamage(int damage) override;

    void setDuration(int countOfRounds) override;
    int getDuration() const override;
    char getSymbol() const override;

    StateSaveData getStateSaveData() override;
};
