#ifndef SUMMON_SPELL_H
#define SUMMON_SPELL_H

#include "Spell.h"
#include "Player.h"
#include "GameField.h"
#include "AllyUnit.h"
#include <memory>

class SummonSpell : public Spell {
    int count;

public:
    SummonSpell(int count);
    bool cast(GameField& field, const Position& playerPos, Player& player, Game& game) override;
    std::string getName() const override;
};

#endif