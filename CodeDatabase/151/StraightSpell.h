#pragma once
#ifndef STRAIGHT_SPELL_H
#define STRAIGHT_SPELL_H

#include "Spell.h"

class StraightSpell : public Spell {
private:
    int damage;
    int baseRange;
    int currentRange;

public:
    StraightSpell(const std::string& name = "Fireball", int damage = 30,
        int range = 3, int manaCost = 20);

    bool cast(GameField* field, const Position& casterPos, const Position& targetPos) override;
    Spell* clone() const override;

    void upgradeRange(int additionalRange);
    int getCurrentRange() const;
};

#endif