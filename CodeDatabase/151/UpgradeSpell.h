#pragma once
#ifndef UPGRADE_SPELL_H
#define UPGRADE_SPELL_H

#include "Spell.h"

class UpgradeSpell : public Spell {
private:
    int upgradeStrength;

public:
    UpgradeSpell(const std::string& name = "Empower", int strength = 1, int manaCost = 10);

    bool cast(GameField* field, const Position& casterPos, const Position& targetPos) override;
    Spell* clone() const override;

    int getUpgradeStrength() const;
};

#endif