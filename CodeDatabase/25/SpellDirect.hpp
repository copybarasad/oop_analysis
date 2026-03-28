#pragma once

#include "spell/ISpell.hpp"

// Direct spell: hits one target in range (enemy or nest).
class SpellDirect : public ISpell {
    int dmg_, radius_, cost_;

public:
    SpellDirect(int dmg, int radius, int cost)
        : dmg_(dmg), radius_(radius), cost_(cost) {}

    std::string name() const override { return "Direct"; }
    int cost() const override { return cost_; }
    int damage() const { return dmg_; }
    int radius() const { return radius_; }
    std::unique_ptr<ISpell> clone() const override { return std::make_unique<SpellDirect>(*this); }

    bool canCast(const Field&, const Player&) const override;
    bool cast(Field&, Player&, std::optional<Pos> target) override;
};
