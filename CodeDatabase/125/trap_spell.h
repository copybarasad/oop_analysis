#pragma once
#include "spell.h"

class TrapSpell : public Spell {
public:
    void Cast(Player& player, EnemyManager& enemies, GameField& field) override;
    std::string GetName() const override { return "Ловушка"; }
    SpellType GetType() const override { return SpellType::Trap; }
};