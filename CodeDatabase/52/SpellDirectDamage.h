#pragma once
#include "ISpell.h"

class SpellDirectDamage final : public ISpell {
    int baseDamage_;
    int baseRadius_;
public:
    SpellDirectDamage(int dmg = 6, int radius = 3)
        : baseDamage_{dmg}, baseRadius_{radius} {}
    std::string Name() const noexcept override { return "DirectDamage"; }
    int ManaCost() const noexcept override { return 0; }
    bool Cast(class Player& player, class GameField& field, const SpellContext& ctx) override;
};