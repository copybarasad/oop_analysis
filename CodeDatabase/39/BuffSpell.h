#pragma once

#include "Spell.h"
#include <string>
#include <cstddef>

class Game;

class BuffSpell : public Spell {
public:
    BuffSpell(int stacks, int cost);

    std::string GetName() const override;

    int GetCost() const override;

    bool CanCast(const Game &game,
                 std::size_t x,
                 std::size_t y) const override;

    bool Cast(Game &game,
              std::size_t x,
              std::size_t y) override;

private:
    int stacks_;
    int cost_;

    void EnsureInvariants() const;
};
