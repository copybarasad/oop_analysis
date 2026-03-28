#ifndef AREADAMAGESPELL_H
#define AREADAMAGESPELL_H

#include "spell.h"

class AreaDamageSpell : public Spell {
private:
    int damageAmount;
    int castRange;
    int manaCost;

public:
    AreaDamageSpell(int spellDamage, int spellRange, int cost);
    bool apply(const Position& targetPosition, GameEngine& game) override;
    std::string getName() const override;
    int getManaCost() const override;
    std::string getDescription() const override;
};

#endif