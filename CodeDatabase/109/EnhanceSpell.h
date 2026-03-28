#ifndef ENHANCESPELL_H
#define ENHANCESPELL_H

#include "ConcreteSpell.h"

class EnhanceSpell : public ConcreteSpell {
private:
    int basePower;

public:
    EnhanceSpell();
    void cast(const Position& casterPos, const Position& targetPos) override;
    Spell* clone() const override;
    
    void enhancePower(int amount);
    int getBasePower() const;
    void setBasePower(int power);
};

#endif