#pragma once
#include "spell.h"

class FireballSpell : public Spell {
public:
    void Cast(Player& player, EnemyManager& enemies, GameField& field) override;
    std::string GetName() const override { return "Огненный шар"; }
    SpellType GetType() const override { return SpellType::Fireball; }
};