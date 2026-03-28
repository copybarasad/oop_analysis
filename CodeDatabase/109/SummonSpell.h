#ifndef SUMMONSPELL_H
#define SUMMONSPELL_H

#include "ConcreteSpell.h"

class SummonSpell : public ConcreteSpell {
private:
    int baseSummonCount;

public:
    SummonSpell();
    void cast(const Position& casterPos, const Position& targetPos) override;
    Spell* clone() const override;
    
    void enhanceSummonCount(int amount) override;
    int getSummonCount() const;
};

#endif