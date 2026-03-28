#pragma once
#include "ISpell.h"

class SpellSummon final : public ISpell {
    int count_;
public:
    explicit SpellSummon(int base = 1) : count_{base} {}
    std::string Name() const noexcept override { return "Summon"; }
    int ManaCost() const noexcept override { return 0; }
    bool Cast(class Player& player, class GameField& field, const SpellContext& ctx) override;
};