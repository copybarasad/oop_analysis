#ifndef HEALSPELL_H
#define HEALSPELL_H

#include "Spell.h"

class HealSpell : public Spell {
public:
    bool cast(const Position& target, GameField& field, Character& caster) override;
    std::string getName() const override { return "Heal"; }
    std::string getDescription() const override { return "Heals 20 HP"; }
    int getRange() const override { return 0; }
    bool requiresTarget() const override { return false; }
};

#endif