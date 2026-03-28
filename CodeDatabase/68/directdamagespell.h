#ifndef DIRECTDAMAGESPELL_H
#define DIRECTDAMAGESPELL_H

#include "spell.h"

class DirectDamageSpell : public Spell {
private:
    int damage;
    int manaCost;
    int range;
    
public:
    DirectDamageSpell(int damage = 25, int manaCost = 10, int range = 3);
    
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