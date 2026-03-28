#pragma once
#include "spell.h"

class TeleportSpell : public Spell {
public:
    void Cast(Player& player, EnemyManager& enemies, GameField& field) override;
    std::string GetName() const override { return "Телепортация"; }
    SpellType GetType() const override { return SpellType::Teleport; }
};