#pragma once
#include "spell.h"

class HealSpell : public Spell {
public:
    void Cast(Player& player, EnemyManager& enemies, GameField& field) override;
    std::string GetName() const override { return "Исцеление"; }
    SpellType GetType() const override { return SpellType::Heal; }
};