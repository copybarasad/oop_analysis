#ifndef TRAPSPELL_H
#define TRAPSPELL_H

#include "ConcreteSpell.h"

class TrapSpell : public ConcreteSpell {
public:
    TrapSpell();
    void cast(const Position& casterPos, const Position& targetPos) override;
    Spell* clone() const override;
};

#endif