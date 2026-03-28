#pragma once

#include "Spell.h"

class SummonSpell : public Spell {
private:
    int summonCount;
    int range;

public:
    SummonSpell(int summonCount = 1, int range = 1);

    SpellResult cast(GameController& game, const Position& target, EnhancementContext& enhancement) override;
    std::string getName() const override;
    std::string getDescription() const override;
    char getSymbol() const override;
    int getRange() const override;
    std::unique_ptr<Spell> clone() const override;
    
    int getSummonCount() const { return summonCount; }
    void setSummonCount(int count) { summonCount = count; }
    void setRange(int r) { range = r; }
};
