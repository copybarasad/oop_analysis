#pragma once

#include <string>
#include <cstddef>
#include "Spell.h"

class TrapSpell : public Spell {
public:
    TrapSpell(int damage, int radius, int cost);

    std::string GetName() const override;

    int GetCost() const override;

    bool CanCast(const Game &game,
                 std::size_t target_x,
                 std::size_t target_y) const override;

    bool Cast(Game &game,
              std::size_t target_x,
              std::size_t target_y) override;

private:
    int damage_;
    int radius_;
    int cost_;

    void EnsureInvariants() const;
};
