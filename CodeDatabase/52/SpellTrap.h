#pragma once
#include "ISpell.h"

class SpellTrap final : public ISpell {
    int baseDamage_;
public:
    explicit SpellTrap(int dmg = 5) : baseDamage_{dmg} {}
    std::string Name() const noexcept override { return "Trap"; }
    int ManaCost() const noexcept override { return 0; }
    bool Cast(class Player& player, class GameField& field, const SpellContext& ctx) override;
};