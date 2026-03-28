#ifndef AREADAMAGESPELL_H
#define AREADAMAGESPELL_H

#include "Spell.h"

class AreaDamageSpell : public Spell {
private:
    int damage;
    int range;
    
public:
    AreaDamageSpell(int dmg = 50, int rng = 3) : damage(dmg), range(rng) {} 
    
    bool cast(const Position& target, GameField& field, Character& caster) override;
    std::string getName() const override { return "Fireball"; }
    std::string getDescription() const override { return "Deals " + std::to_string(damage) + " damage in 2x2 area at end of line (3 cells)"; }
    int getRange() const override { return range; }
    bool requiresTarget() const override { return true; }
};

#endif