#ifndef EXPLOSIONSPELL_H
#define EXPLOSIONSPELL_H

#include "ConcreteSpell.h"

class ExplosionSpell : public ConcreteSpell {
public:
    ExplosionSpell();
    void cast(const Position& casterPos, const Position& targetPos) override;
    Spell* clone() const override;
};

#endif