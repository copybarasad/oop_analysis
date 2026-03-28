#ifndef AOESPELL_H
#define AOESPELL_H

#include "spell.h"

class AOESpell : public Spell {
    private:
        int damage;
        int manaCost;
        int range;

    public:
        AOESpell(int damage = 20, int manaCost = 15, int range = 2);

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