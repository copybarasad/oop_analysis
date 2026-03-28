#pragma once
#include "SpellCard.h"
#include "SpellCardVisitor.h"

class SummonSpell : public SpellCard {
    int allies_count;
    int summon_range;
    double health_ally = 10;
    double damage_ally = 5;
public:
    SummonSpell(int count = 1, int range = 1) : allies_count(count), summon_range(range) {}
    bool apply(const SpellContext& context, GameArea& gamearea) override;
    std::string accept(const SpellCardVisitor& visitor) const override {
        return visitor.format(this);
    }
};