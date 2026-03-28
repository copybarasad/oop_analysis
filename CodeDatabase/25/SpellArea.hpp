#pragma once

#include "spell/ISpell.hpp"

// Area spell: damages a 2x2 square near the player.
class SpellArea : public ISpell {
    int dmg_;
    int radius_;
    int cost_;

public:
    SpellArea(int dmg, int radius, int cost)
        : dmg_(dmg), radius_(radius), cost_(cost) {}

    std::string name() const override { return "Area2x2"; }
    int cost() const override { return cost_; }
    int damage() const { return dmg_; }
    int radius() const { return radius_; }
    std::unique_ptr<ISpell> clone() const override { return std::make_unique<SpellArea>(*this); }

    bool canCast(const Field&, const Player& p) const override;
    bool cast(Field&, Player&, std::optional<Pos> target) override;
};
