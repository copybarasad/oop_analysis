#pragma once
#ifndef TRAP_SPELL_H
#define TRAP_SPELL_H

#include "Spell.h"

class TrapSpell : public Spell {
private:
    int baseDamage;
    int currentDamage;
    int range;

public:
    TrapSpell(const std::string& name = "Bear Trap", int damage = 40,
        int range = 2, int manaCost = 15);

    bool cast(GameField* field, const Position& casterPos, const Position& targetPos) override;
    Spell* clone() const override;

    void upgradeDamage(int additionalDamage);
    int getCurrentDamage() const;
};

#endif