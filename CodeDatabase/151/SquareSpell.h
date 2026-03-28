#pragma once
#ifndef SQUARE_SPELL_H
#define SQUARE_SPELL_H

#include "Spell.h"

class SquareSpell : public Spell {
private:
    int damage;
    int baseAreaSize;
    int currentAreaSize;
    int range;

public:
    SquareSpell(const std::string& name = "Earthquake", int damage = 20,
        int areaSize = 2, int range = 2, int manaCost = 30);

    bool cast(GameField* field, const Position& casterPos, const Position& targetPos) override;
    Spell* clone() const override;

    void upgradeAreaSize(int additionalSize);
    int getCurrentAreaSize() const;
};

#endif