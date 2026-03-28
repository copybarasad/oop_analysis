#ifndef FIREBALLSPELL_H
#define FIREBALLSPELL_H

#include "ConcreteSpell.h"

class FireballSpell : public ConcreteSpell {
public:
    FireballSpell();
    void cast(const Position& casterPos, const Position& targetPos) override;
    Spell* clone() const override;
};

#endif