#ifndef BUFF_SPELL_H
#define BUFF_SPELL_H

#include "Spell.h"
#include "Player.h"
#include "GameField.h"
#include <iostream>

class BuffSpell : public Spell {
public:
    BuffSpell();
    bool cast(GameField& field, const Position& pos, Player& player, Game& game) override;
    std::string getName() const override;
    bool isBuffSpell() const override { return true; }
};

#endif