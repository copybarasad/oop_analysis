#ifndef TRAP_SPELL_H
#define TRAP_SPELL_H

#include "Spell.h"
#include "Player.h"
#include "GameField.h"
#include <iostream>

class TrapSpell : public Spell {
    int trapDamage;

public:
    TrapSpell(int dmg = DEFAULT_TRAP_DAMAGE);
    bool cast(GameField& field, const Position& pos, Player& player, Game& game) override;
    std::string getName() const override;
    bool isBuffSpell() const override { return false; }
};

#endif