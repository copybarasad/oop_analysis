#ifndef INVISIBILITYSPELL_H
#define INVISIBILITYSPELL_H

#include "ConcreteSpell.h"

class InvisibilitySpell : public ConcreteSpell {
private:
    int baseDuration;

public:
    InvisibilitySpell();
    void cast(const Position& casterPos, const Position& targetPos) override;
    Spell* clone() const override;
    
    void enhanceDuration(int amount) override;
    int getDuration() const;
};

#endif