#pragma once
#include "spell.h"

class DirectDamageSpell : public Spell {
public:
    void Cast(Player& player, EnemyManager& enemies, GameField& field) override;
    std::string GetName() const override { return "Магический удар"; }
    SpellType GetType() const override { return SpellType::DirectDamage; }
};