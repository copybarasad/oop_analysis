#ifndef SPELLBASE_H
#define SPELLBASE_H

#include "Spell.h"
#include "GameField.h"
#include <iostream>
#include <cmath>

class SpellBase : public Spell {
protected:
    std::string name;
    int manaCost;
    int range;
    int damage;

    bool validateTarget(const SpellTarget& target, const GameField& field) const;

public:
    SpellBase(const std::string& spellName, int cost, int spellRange, int spellDamage);

    virtual bool cast(const SpellTarget& target, GameField& field,
        std::vector<Enemy>& enemies, Player& player) override = 0;

    std::string getDescription() const override = 0;
    std::unique_ptr<Spell> clone() const override = 0;

    std::string getName() const override;
    int getManaCost() const override;
    int getRange() const override;
};

#endif