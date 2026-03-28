#pragma once
#include "ISpell.h"

class SpellEnhance final : public ISpell {
    int r_, a_, t_, s_;
public:
    SpellEnhance(int r = 1, int a = 1, int t = 2, int s = 1)
        : r_{r}, a_{a}, t_{t}, s_{s} {}
    std::string Name() const noexcept override { return "Enhance"; }
    int ManaCost() const noexcept override { return 0; }
    bool Cast(class Player& player, class GameField& field, const SpellContext& ctx) override;
};