#ifndef TRAPSPELL_H
#define TRAPSPELL_H

#include "Spell.h"

class TrapSpell : public Spell {
public:
    bool cast(const Position& target, GameField& field, Character& caster) override;
    std::string getName() const override { return "Trap"; }
    std::string getDescription() const override { return "Places a trap that deals 30 damage to enemies"; }
    int getRange() const override { return 1; } // Изменено с 3 на 1
    bool requiresTarget() const override { return true; }
};

#endif