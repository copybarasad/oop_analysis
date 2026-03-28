#pragma once
#include "ISpell.h"

class SpellAreaDamage final : public ISpell {
    int baseDamage_;
    int baseRadius_;
    int baseSize_;
public:
    SpellAreaDamage(int dmg = 3, int radius = 4, int size = 2)
        : baseDamage_{dmg}, baseRadius_{radius}, baseSize_{size} {}
    std::string Name() const noexcept override { return "AreaDamage"; }
    int ManaCost() const noexcept override { return 0; }
    bool Cast(class Player& player, class GameField& field, const SpellContext& ctx) override;
};