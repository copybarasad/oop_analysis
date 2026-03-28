#pragma once
#include "spell.h"

class AreaDamageSpell : public Spell {
public:
    void Cast(Player& player, EnemyManager& enemies, GameField& field) override;
    std::string GetName() const override { return "Взрыв"; }
    SpellType GetType() const override { return SpellType::AreaDamage; }
};