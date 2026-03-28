#ifndef DIRECTDAMAGESPELL_H
#define DIRECTDAMAGESPELL_H

#include "spell.h"

class DirectDamageSpell : public Spell {
private:
    int damageAmount;
    int castRange;
    int manaCost;

public:
    DirectDamageSpell(int spellDamage, int spellRange, int cost);
    bool apply(const Position& targetPosition, GameEngine& game) override;
    std::string getName() const override;
    int getManaCost() const override;
    std::string getDescription() const override;
};

#endif