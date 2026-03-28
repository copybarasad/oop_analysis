#ifndef DIRECTDAMAGESPELL_H
#define DIRECTDAMAGESPELL_H

#include "Spell.h"

class DirectDamageSpell : public Spell {
private:
    int damage;
    int range;
    
public:
    // Урон 50, дальность 3 клетки
    DirectDamageSpell(int dmg = 50, int rng = 3) : damage(dmg), range(rng) {}
    
    bool cast(const Position& target, GameField& field, Character& caster) override;
    std::string getName() const override { return "Firebolt"; }
    std::string getDescription() const override { return "Deals " + std::to_string(damage) + " damage to first enemy in line (range " + std::to_string(range) + ")"; }
    int getRange() const override { return range; }
    bool requiresTarget() const override { return true; }
};

#endif