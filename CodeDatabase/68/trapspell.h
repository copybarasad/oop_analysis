#ifndef TRAPSPELL_H
#define TRAPSPELL_H

#include "spell.h"

class TrapSpell : public Spell {
private:
    int damage;
    int manaCost;
    int range;
    
public:
    TrapSpell(int damage = 30, int manaCost = 20, int range = 2);
    
    void cast(GameField* field, const Position& targetPosition) override;
    std::string getName() const override;
    int getManaCost() const override;
    int getDamage() const override;
    int getRange() const override;

    void upgradeDamage(int additionalDamage) override;
    void upgradeRange(int additionalRange) override;
    void reduceManaCost(int reduction) override;
};

#endif