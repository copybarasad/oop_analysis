#pragma once
#ifndef SUMMON_SPELL_H
#define SUMMON_SPELL_H

#include "Spell.h"

class SummonSpell : public Spell {
private:
    int baseAlliesCount;
    int currentAlliesCount;
    int range;

public:
    SummonSpell(const std::string& name = "Summon Ally", int alliesCount = 1,
        int range = 1, int manaCost = 25);

    bool cast(GameField* field, const Position& casterPos, const Position& targetPos) override;
    Spell* clone() const override;

    void upgradeAlliesCount(int additionalCount);
    int getCurrentAlliesCount() const;
};

#endif