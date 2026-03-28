#ifndef TRAPSPELL_H
#define TRAPSPELL_H

#include "spell.h"

class TrapSpell : public Spell {
private:
    int damageAmount;
    int castRange;
    int manaCost;

public:
    TrapSpell(int trapDamage, int trapRange, int cost);
    bool apply(const Position& targetPosition, GameEngine& game) override;
    std::string getName() const override;
    int getManaCost() const override;
    std::string getDescription() const override;
};

#endif