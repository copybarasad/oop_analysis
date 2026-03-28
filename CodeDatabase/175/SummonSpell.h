#pragma once
#include "Spell.h"

class SummonSpell : public Spell {
private:
    int summonCount_;

public:
    SummonSpell(int count = 1);
    std::string getName() const override;
    bool apply(Field& field, const Player& player, int targetX, int targetY) override;
    void setSummonCount(int count) { summonCount_ = count; }
    int getSummonCount() const { return summonCount_; }
    bool needsTarget() const override { return false; }
};