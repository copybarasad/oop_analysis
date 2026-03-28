#pragma once

#include "Spell.h"
#include <string>
#include <cstddef>

class Game;

class SummonSpell : public Spell {
public:
    SummonSpell(int ally_health, int ally_damage, int cost);

    std::string GetName() const override;

    int GetCost() const override;

    bool CanCast(const Game &game,
                 std::size_t x,
                 std::size_t y) const override;

    bool Cast(Game &game,
              std::size_t x,
              std::size_t y) override;

private:
    int ally_health_;
    int ally_damage_;
    int cost_;

    void EnsureInvariants() const;
};
